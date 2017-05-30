//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
// SnoopControl.cpp: implementation of the SnoopControl class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
//#include "sniffusb.h"
#include "SnoopControl.h"
#include "..\..\USBSnoop\Shared\Common.h"
#include "SS_Log.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "devicemgr.h"
#include "multisz.h"


SnoopControl::SnoopControl()
{
	//We get an assert if we try to use LoadString, probably related to strange MFC configurations
	//m_sLowerFilters.LoadString(IDS_LOWERFILTERS);
	m_sLowerFilters = "LowerFilters";
	// check Windows 2000 / XP versus Windows 98
	m_bIsWin2K = ((GetVersion() & 0x80000000) == 0);
	m_sFilterName = USBSNOOP_SERVICE_NAME_A;
	Log("SC:FilterName is %s",m_sFilterName);
	// compute the name of the filter
	/*if (m_bIsWin2K)
		//m_sFilterName.LoadString(IDS_FILTERNAME_NT);
		//m_sFilterName = "hspf";
		m_sFilterName = USBSNOOP_SERVICE_NAME_A;
		Log("SC:FilterName is %s",m_sFilterName);
	else
	{
		Log("SC:Trying to start on Windows 98");
		MessageBox(NULL,"It seems as you are using Windows 98.\nThe HeadsetPresenter does not support Windows 98 and might not work.","Warning",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		//m_sFilterName.LoadString(IDS_FILTERNAME_98);
		//m_sFilterName = "hspf";
		m_sFilterName = USBSNOOP_SERVICE_NAME_A;
	}*/

}

SnoopControl::~SnoopControl()
{

}

void SnoopControl::SetSettingsControl(SettingsControl* iSettingsControl)
{
	mySettingsControl = iSettingsControl;
}

bool SnoopControl::InstallDriver()
{
	LogEnterFunction("SC:InstallDriver");
	CDeviceMgr mgr;
	mgr.SetSettingsControl(mySettingsControl);
	sDriver = mgr.EliminateDevices();
	Log("SC:sDriver = %s", sDriver);
	if(sDriver != "")
	{
		POSITION pos;

		pos = mgr.GetHeadPosition();
		while (pos != NULL)
		{
			CDevice& device = mgr.GetNext(pos);

			if (device.GetDriver() == sDriver)
			{
				Log("SC:Found matching device for sDriver");
				if(!device.GetLowerFilters().FindString(m_sFilterName))
				{
					Log("SC:Filter not installed, installing");
					CMultiSz sz; // = device.GetLowerFilters();  // By not including GetLowerFilters() we erase all current lower filters for this device. This can be a problem if any devices use lower filters. Hopefully they dont. Otherwise this line must be included and the deletion of the filter must be handled in a better way
					sz.AddString(m_sFilterName);
					device.SetLowerFilters(sz);			
				}
				else
				{
					Log("SC:Filter already installed, no action");
				}
			}
		}
		return true;
	}
	else //no device could be identified
	{
		Log("SC:No Bluetooth USB device could be identified");
		//MessageBox(NULL,"No Bluetooth USB device could be identified.\nThe HeadsetPresenter will not start.\n\nContact support for further assistance.","msg",MB_OK|MB_ICONINFORMATION);
		return false;
	}

}

void SnoopControl::UninstallDriver()
{
	LogEnterFunction("SC:UninstallDriver");
	CDeviceMgr mgr;
	mgr.SetSettingsControl(mySettingsControl);
	POSITION pos;
	pos = mgr.GetHeadPosition();
	while (pos != NULL)
	{
		CString sName, sFilter, sDescription, sPresent, sDriver;
		CDevice& device = mgr.GetNext(pos);
    
		sName = device.GetHardwareID();
		sDescription = device.GetDeviceDesc();
		sDriver = device.GetDriver();

		if (device.GetPresent() && device.GetLowerFilters().FindString(m_sFilterName))
		{
			Log("SC:Device found and driver uninstalled");
			//MessageBox(NULL,device.GetDeviceDesc(),"Has filter and is present",MB_OK);
			CMultiSz sz;// = device.GetLowerFilters(); //Not a clean way, will delete all lower filters, hopefully noone uses them
			//sz.RemoveString(m_sFilterName);
			sz.AddString("");
			device.SetLowerFilters(sz);
		}
	}
}

void SnoopControl::RestartDevice()
{
	LogEnterFunction("SC:RestartDevice");
	CDeviceMgr mgr;
	mgr.SetSettingsControl(mySettingsControl);
	POSITION pos;

	pos = mgr.GetHeadPosition();
	while (pos != NULL)
	{
		CDevice& device = mgr.GetNext(pos);

		if (device.GetDriver() == sDriver)
		{
			Log("SC:GetDriver==sDriver, Found device to restart");
			if (!device.Restart())
			{
				Log("SC:Restart of device failed, device DriverId=%s",sDriver);
				MessageBox(NULL,"The HeadsetPresenter needs to restart your USB device but this failed.\nPlease try unplugging and replugging it manually.","Restart failed",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
			}
			else
			{
				Log("SC:Device restarted ok");
			}
		}
	}
}



