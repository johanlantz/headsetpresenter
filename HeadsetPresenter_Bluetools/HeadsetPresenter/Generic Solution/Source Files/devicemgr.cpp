//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
// devicemgr.cpp: implementation of the CDeviceMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "sniffusb.h"
#include "devicemgr.h"
#include "multisz.h"

#include "SS_Log.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeviceMgr::CDeviceMgr()
{
	
	TCHAR sEnumerator[] = _T("USB");
	HDEVINFO hDev;
	DWORD dwIndex;
	SP_DEVINFO_DATA devInfo;

	BYTE Buffer[300];
	DWORD BufferSize = 0;
	DWORD DataType;

	m_hDev = SetupDiGetClassDevs(NULL, sEnumerator, NULL, DIGCF_ALLCLASSES);
	if (m_hDev != INVALID_HANDLE_VALUE)
	{
		dwIndex = 0;

		memset(&devInfo, 0, sizeof(devInfo));
		devInfo.cbSize = sizeof(devInfo);

		while (SetupDiEnumDeviceInfo(m_hDev, dwIndex ++, &devInfo))
		{
			CDevice device(m_hDev, devInfo);

			if (SetupDiGetDeviceRegistryProperty(m_hDev,&devInfo,SPDRP_DEVICEDESC,
				&DataType,Buffer,sizeof(Buffer),&BufferSize))
			{
				device.SetDeviceDesc((char *) Buffer);
			}
			if (SetupDiGetDeviceRegistryProperty(m_hDev,&devInfo,SPDRP_FRIENDLYNAME,
				&DataType,Buffer,sizeof(Buffer),&BufferSize))
			{
				device.SetFriendlyName((char *) Buffer);
			}
			if (SetupDiGetDeviceRegistryProperty(m_hDev,&devInfo,SPDRP_HARDWAREID,
				&DataType,Buffer,sizeof(Buffer),&BufferSize))
			{
				device.SetHardwareID((char *) Buffer);
			}
			if (SetupDiGetDeviceRegistryProperty(m_hDev,&devInfo,SPDRP_MFG,
				&DataType,Buffer,sizeof(Buffer),&BufferSize))
			{
				device.SetMfg((char *) Buffer);
			}
			if (SetupDiGetDeviceRegistryProperty(m_hDev,&devInfo,SPDRP_DRIVER,
				&DataType,Buffer,sizeof(Buffer),&BufferSize))
			{
				device.SetDriver((char *) Buffer);
			}
			Log("DM:Device: %s with driver %s", device.GetDeviceDesc(),device.GetDriver());
			if((device.GetDeviceDesc().Find("Blue",0) != -1) || (device.GetDeviceDesc().Find("Jabra",0) != -1) )  //jj
			{
				//BOOL t = device.GetPresent();  //j this should be handeled by routin below, bring all devices there
				deviceList.AddTail(device);
				Log("DM:Added");
			}
			
		}
	}

	hDev = SetupDiGetClassDevs(NULL, sEnumerator, NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT);
	if (hDev != INVALID_HANDLE_VALUE)
	{
		dwIndex = 0;

		memset(&devInfo, 0, sizeof(devInfo));
		devInfo.cbSize = sizeof(devInfo);

		while (SetupDiEnumDeviceInfo(hDev, dwIndex ++, &devInfo))
		{
			//Johan Changes made here to update device presens based on driverID instead of HardwareID since driver id is unique, HardwareID is not
			if (SetupDiGetDeviceRegistryProperty(hDev,&devInfo,SPDRP_DRIVER,
				&DataType,Buffer,sizeof(Buffer),&BufferSize))
			{
				CString driverID;
				POSITION pos;
				
				driverID = ((const char * )Buffer);
				pos = FindDeviceBasedOnDriver(driverID);
				if (pos)
					GetFoundDevice(pos).SetPresent(TRUE);
			}
			/*if (SetupDiGetDeviceRegistryProperty(hDev,&devInfo,SPDRP_HARDWAREID,
				&DataType,Buffer,sizeof(Buffer),&BufferSize))
			{
				CString hardwareID;
				POSITION pos;
				
				hardwareID = ((const char * )Buffer);
				pos = FindDevice(hardwareID);
				if (pos)
					GetFoundDevice(pos).SetPresent(TRUE);
			}*/
		}
	}
	SetupDiDestroyDeviceInfoList(hDev);

	SetHomeDirPath();

}

CDeviceMgr::~CDeviceMgr()
{
	SetupDiDestroyDeviceInfoList(m_hDev);
}

void CDeviceMgr::SetSettingsControl(SettingsControl* iSettingsControl)
{
	mySettingsControl = iSettingsControl;
}

void CDeviceMgr::SetHomeDirPath()
{
	HMODULE phModule;
	TCHAR fullPathToDLL[512];
	phModule = GetModuleHandle("HeadsetPresenter.dll");
	if(phModule == NULL)
	{
		phModule = GetModuleHandle("HeadsetPresenter.exe");  //When running stand alone
		if(phModule == NULL)
		{
			Log("DM:Failed to get instance path of HeadsetPresenter");
			MessageBox(NULL,"Failed to get instance path. Please contact support","HeadsetPresenter",MB_OK);
		}
	}
	if(phModule != NULL)  //We got instance path to either .dll or .exe
	{
		//Get the dir path only and set \0 after the final \ in the path to remove the filename of the dll
		DWORD dwResult = ::GetModuleFileName(phModule,fullPathToDLL, sizeof(fullPathToDLL));
		char* t = _tcsrchr(fullPathToDLL,'\\');
		t++;
		*t = '\0';
		applicationHomeDirPath = fullPathToDLL;
		Log("DM:applicationHomeDirPath=%s", applicationHomeDirPath);
	}
}

POSITION CDeviceMgr::FindDevice(CString hardwareID)
{
	POSITION cur, next;

	cur = deviceList.GetHeadPosition();
	while (cur != NULL)
	{
		next = cur;
		CDevice& r = deviceList.GetNext(next);

		if (r.GetHardwareID() == hardwareID)
			break;

		cur = next;
	}

	return cur;
}

POSITION CDeviceMgr::FindDeviceBasedOnDriver(CString driverID)
{
	POSITION cur, next;

	cur = deviceList.GetHeadPosition();
	while (cur != NULL)
	{
		next = cur;
		CDevice& r = deviceList.GetNext(next);

		if (r.GetDriver() == driverID)
			break;

		cur = next;
	}

	return cur;
}


CDevice& CDeviceMgr::GetFoundDevice(POSITION pos)
{
	return deviceList.GetAt(pos);
}

//Returns the first device in the BT list
CDevice& CDeviceMgr::GetFirstBTDevice()
{
	return deviceListOnlyBT.GetAt(deviceListOnlyBT.FindIndex(0));
}

CString CDeviceMgr::EliminateDevices()
{
	LogEnterFunction(("Eliminate"));
	CString identifiedBTDriver = "";
	
	CStringList knownDevices;
	
	

	BuildKnownDeviceList(knownDevices);
	
	

	identifiedBTDriver = SearchForKnownDevice(knownDevices);
	
	if(identifiedBTDriver == "")
	{
		identifiedBTDriver = TryToIdentifyOneUnknownDevice();	
		if(identifiedBTDriver == "")
		{
			Log("DM:Could not identify a Bluetooth device");
			MessageBox(NULL,"The HeadsetPresenter could not identify a Bluetooth device\n\nPlease contact support at support@headsetpresenter.com to resolve this problem.","No device found",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		}
	}
	Log("DM:Sucessfully identified: %s",identifiedBTDriver);
	return identifiedBTDriver;
}

void CDeviceMgr::BuildKnownDeviceList(CStringList& knownDevices)
{
	LogEnterFunction("DM:BuildKnownDeviceList");
	//knownDevices.AddTail("TDK Bluetooth USB Adaptor");
	knownDevices.AddTail("Billionton USB Bluetooth Device");
	knownDevices.AddTail("Bluetooth by hp");
	knownDevices.AddTail("IBM Integrated Bluetooth II");
//	knownDevices.AddTail("BlueFRITZ! USB (WinXP/2000)");
	knownDevices.AddTail("MSI USB Bluetooth Device");
	knownDevices.AddTail("CSR USB Bluetooth Device");
	knownDevices.AddTail("Kensington Bluetooth Device");
	knownDevices.AddTail("TECOM USB Bluetooth Device");
	knownDevices.AddTail("Jabra A310 USB Bluetooth Device");
	knownDevices.AddTail("Belkin Bluetooth Device");
	knownDevices.AddTail("Alps Bluetooth Device");
	knownDevices.AddTail("Dell TrueMobile Bluetooth Module");
	knownDevices.AddTail("HP integrated module with Bluetooth wireless technology   ");
	knownDevices.AddTail("Jabra - A320s");					  
	
	//add more devices here

	//This function call will cause the DeviceInfo.txt file to be created empty if there is no previous info in it
	//Decision not to care about this now, the only effect is that there will be an empty DeviceInfo.txt file in the HeadsetPresenter directory
	CheckFileForDevice(knownDevices); //will be added last and selected if present
}


CString CDeviceMgr::SearchForKnownDevice(CStringList& knownDevices)
{
	LogEnterFunction("SearchForKnownDevice");
	CString foundDeviceDriver = "";
	//Search the whole device list for known device names
	for(int i = 0;i< knownDevices.GetCount(); i++)
	{
		for(int j = 0; j< deviceList.GetCount(); j++)
		{
			if(deviceList.GetAt(deviceList.FindIndex(j)).GetPresent())
			{
				if(deviceList.GetAt(deviceList.FindIndex(j)).GetDeviceDesc() == knownDevices.GetAt(knownDevices.FindIndex(i)))
				{
					foundDeviceDriver = deviceList.GetAt(deviceList.FindIndex(j)).GetDriver();
					Log("DM:Found: %s for, %s", deviceList.GetAt(deviceList.FindIndex(j)).GetDeviceDesc(), foundDeviceDriver);
					//MessageBox(NULL, deviceList.GetAt(deviceList.FindIndex(j)).GetDeviceDesc(),"Found device",MB_OK);					
				}
			}

		}
	}
	Log("DM:returning with: %s", foundDeviceDriver);
	return foundDeviceDriver;
}

CString CDeviceMgr::TryToIdentifyOneUnknownDevice()
{
	LogEnterFunction("TryToIdentifyOneUnknownDevice");
	CString unknownDeviceDriver = "";
	//For all devices
	for(int j = 0; j< deviceList.GetCount(); j++)
	{
		if((deviceList.GetAt(deviceList.FindIndex(j)).GetDeviceDesc().Find("Bluetooth") != -1) ||
			(deviceList.GetAt(deviceList.FindIndex(j)).GetDeviceDesc().Find("Blue") != -1) || (deviceList.GetAt(deviceList.FindIndex(j)).GetDeviceDesc().Find("Jabra") != -1))  //If desc contain Bluetooth
		{
			if(deviceList.GetAt(deviceList.FindIndex(j)).GetPresent())	//And the dev is present
			{	
				deviceListOnlyBT.AddTail(deviceList.GetAt(deviceList.FindIndex(j)));
				unknownDeviceDriver = deviceList.GetAt(deviceList.FindIndex(j)).GetDriver();
				Log("DM:Added unknownDevice: %s", deviceList.GetAt(deviceList.FindIndex(j)).GetDeviceDesc());
				//MessageBox(NULL, deviceList.GetAt(deviceList.FindIndex(j)).GetDeviceDesc(),"Adding unknown to list",MB_OK|MB_ICONINFORMATION );						
			}
		}

	}

	//No devices could be found, contact support
	if(deviceListOnlyBT.GetCount() == 0)
	{
		Log("DM:No USB Bluetooth device could be found, returning with: %s",unknownDeviceDriver);
		MessageBox(NULL, "No USB Bluetooth device could be found.\nThis does not have to be an error it might just be that your Bluetooth device could not be recognised.\nIf this is the case it can usually be solved by contacting the support at support@headsetpresenter.com.\n\nPlease note that Bluetooth PCMCIA cards are not supported by the HeadsetPresenter at this time.","No Bluetooth device found",MB_OK|MB_TOPMOST|MB_ICONINFORMATION );
		return unknownDeviceDriver;
	}
	//Exactly one device contained the word Bluetooth, it should work
	else if(deviceListOnlyBT.GetCount() == 1)
	{
		Log("DM:Found exactly one unknown Bluetooth device called: %s", unknownDeviceDriver);
		MessageBox(NULL,"Your Bluetooth device has not been tested with the HeadsetPresenter.\n\nA Bluetooth adapter that will probably work fine has been identified.\nIf the HeadsetPresenter does not function after trying this device\nplease contact support at support@headsetpresenter.com.\n\n","No known device found",MB_OK|MB_TOPMOST|MB_ICONINFORMATION );
		WriteUnknownDeviceToFile();		
	}
	//More then one device is found, eliminate
	else
	{
		for(int j = 0; j< deviceListOnlyBT.GetCount(); j++)
		{
			if(!deviceListOnlyBT.GetAt(deviceListOnlyBT.FindIndex(j)).GetDeviceDesc().Find("USB"))  //If desc does not contain USB
			{
				deviceListOnlyBT.RemoveAt(deviceListOnlyBT.FindIndex(j));  // if it did not contain USB delete it
				Log("DM:Multiple Bluetooth devices found, eliminating: %s", deviceListOnlyBT.GetAt(deviceListOnlyBT.FindIndex(j)).GetDeviceDesc());
			}
			else
			{
				unknownDeviceDriver = deviceList.GetAt(deviceList.FindIndex(j)).GetDriver();
				Log("DM:Multiple Bluetooth devices found, saving: %s", deviceListOnlyBT.GetAt(deviceListOnlyBT.FindIndex(j)).GetDeviceDesc());
			}
		}
		MessageBox(NULL, "Several Bluetooth devices was found on your computer and the HeadsetPresenter can not determine which to use.\nThe HeadsetPresenter will try to use the first one.\nIf this does not work it can be solved by support at support@headsetpresenter.com","Several devices found",MB_OK|MB_TOPMOST|MB_ICONINFORMATION );						

	}
	
	//return true;
	Log("DM:returning with %s", unknownDeviceDriver);
	return unknownDeviceDriver;
}

void CDeviceMgr::WriteUnknownDeviceToFile()
{
	LogEnterFunction("WriteUnknownDeviceToFile");
	mySettingsControl->SetLastControllerName(deviceListOnlyBT.GetHead().GetDeviceDesc());
/*
	CString path = applicationHomeDirPath;
	path = path + "DeviceInfo.txt";
	try
	{
		CFile myFile(path, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone);	
		//DWORD dwLength = pFile->GetLength();
		myFile.Write((LPCTSTR)deviceListOnlyBT.GetHead().GetDeviceDesc(),deviceListOnlyBT.GetHead().GetDeviceDesc().GetLength());
		Log("DM:Writing %s to file", deviceListOnlyBT.GetHead().GetDeviceDesc());
		//MessageBox(NULL,deviceListOnlyBT.GetHead().GetDeviceDesc(),"added to local file",MB_OK);
		myFile.Close();	
	}
   catch(CFileException* pEx)
   {
	   Log("DM:Write to file threw exception");
	   pEx->Delete();
   }
   */
}

void CDeviceMgr::CheckFileForDevice(CStringList& knownDevices)
{
	LogEnterFunction("DM:CheckFileForDevice");
	knownDevices.AddTail(mySettingsControl->GetLastControllerName());
/*
	CString deviceName="";
	CFile* pFile = NULL;
	CString path = applicationHomeDirPath;
	path = path + "DeviceInfo.txt";
	try
	{
		CFile myFile(path, CFile::modeCreate | CFile::modeNoTruncate| CFile::shareDenyNone);
		//DWORD dwLength = pFile->GetLength();

		int bytesRead=0;
		char s[100];
		bytesRead = myFile.Read(s,100);
		if(bytesRead > 0)
		{
			s[bytesRead] = 0;
			deviceName = s;
			knownDevices.AddTail(deviceName);
			Log("DM:Added device called: %s", deviceName);
			//MessageBox(NULL,deviceName,"added from file",MB_OK);
		}
		else
		{
			Log("DM:No device found in file");
		}
		myFile.Close();
	}
   catch(CFileException* pEx)
   {
	   pEx->Delete();
	   Log("DM:CheckFile threw exception");
      // Simply show an error message to the user.
   }
*/
}

void CDeviceMgr::Dump()
{
	POSITION pos;

	pos = GetHeadPosition();
	while (pos != NULL)
	{
		CDevice& r = GetNext(pos);

		TRACE("---> HardwareID = %s\n",(const char *)r.GetHardwareID());
		TRACE("     DeviceDesc = %s\n",(const char *)r.GetDeviceDesc());
		TRACE("     FriendlyName = %s\n",(const char *)r.GetFriendlyName());
		TRACE("     Mfg = %s\n",(const char *)r.GetMfg());
//		TRACE("     LowerFilters = %s\n",(const char *)r.GetLowerFilters());
		TRACE("     Present = %s\n",r.GetPresent() ? _T("TRUE") : _T("FALSE"));
	}
}


