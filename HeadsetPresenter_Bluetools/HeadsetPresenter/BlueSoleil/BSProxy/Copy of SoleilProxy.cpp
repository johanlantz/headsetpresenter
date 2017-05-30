#include "StdAfx.h"
#include ".\soleilproxy.h"
#include "SS_Log.h"

void PrintError(CHAR* lpszAPI,DWORD dwError)
{
	//printf(lpszAPI);
	//printf(" returned %d: ",dwError);
	CString msg;
	switch(dwError){
	case BTSTATUS_SUCCESS:
		msg = "Succeeded";
		break;
	case BTSTATUS_FAIL:
		msg = "General failure";
		break;
	case BTSTATUS_SYSTEM_ERROR:
		msg = "System error occured";
		break;
	case BTSTATUS_BT_NOT_READY:
		msg = "BT not ready";
		break;
	case BTSTATUS_ALREADY_PAIRED:
		msg = "BS is already paired with this device";
		break;
	case BTSTATUS_AUTHENTICATE_FAILED:
		msg = "Authentication failed";
		break;
	case BTSTATUS_BT_BUSY:
		msg = "Bluetooth is busy browsing services or connecting to another device";
		break;
	case BTSTATUS_CONNECTION_EXIST:
		msg = "The connection to the service is already established";
		break;
	case BTSTATUS_CONNECTION_NOT_EXIST:
		msg = "The connection does not exist or is released.";
		break;
	case BTSTATUS_PARAMETER_ERROR:
		msg = "Parameter error";
		break;
	case BTSTATUS_SERVICE_NOT_EXIST:
		msg = "Service does not exist.";
        break; 
	default:
		msg = "Unknown error";
	}
	Log("BSP:BS commands returned status msg %s", msg);
	if(dwError != BTSTATUS_SUCCESS)
	{
		CString errorMsg = "An error occured, if this is a repeating problem contact support at support@headsetpresenter.com and provide the following error message: \n\n" + msg;
		//MessageBox(NULL,errorMsg,"BlueSoleil error", MB_OK);
	}
}

bool DoInquiry(CStringList* deviceList)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	
	LogEnterFunction("BSP:DoInquiry");
	int devsListIndexCounter = 0;
	UCHAR ucInqMode = INQUIRY_GENERAL_REFRESH;  //Check if we should use these values instead of hardcoded ones
	UCHAR ucInqLen = 15;
	DWORD dwResult;
	bool InquiryOk = false;
	//CStringList* deviceList = new CStringList();  //This is not a good solution, fix with static and pointer sent from BSController
	BLUETOOTH_DEVICE_INFO LocalDevsList[MAX_DEVICE_COUNT] = {0}; //This worked
	DWORD LocalDevsListLen = sizeof(BLUETOOTH_DEVICE_INFO) * MAX_DEVICE_COUNT;
	
	dwResult = BT_InquireDevices(INQUIRY_GENERAL_REFRESH, 0x0A,  &LocalDevsListLen, LocalDevsList);

	PrintError("BT_InquireDevices",dwResult);
	
	if(dwResult == BTSTATUS_SUCCESS){
		//printf("Devices List\n");
		Log("BSP:Inquiry OK");
		InquiryOk = true;
		DWORD i, offset;
		offset = sizeof(BLUETOOTH_DEVICE_INFO);
		BLUETOOTH_DEVICE_INFO *pDevice;
		for(i=0; i<((LocalDevsListLen)/sizeof(BLUETOOTH_DEVICE_INFO)); i++)
		{
			pDevice=(BLUETOOTH_DEVICE_INFO*)((UCHAR*)LocalDevsList+i*offset);
			Log("Device No.%d Device Address: %02X:%02X:%02X:%02X:%02X:%02X",
				i,
				pDevice->address[5],
				pDevice->address[4],
				pDevice->address[3],
				pDevice->address[2],
				pDevice->address[1],
				pDevice->address[0]);
		
			if(strlen(pDevice->szName) < 2)
			{
				//printf("\nGOT 250");
				//memcpy(&myDevice, &pDevice, sizeof(BLUETOOTH_DEVICE_INFO));
				//deviceList->AddTail("Unknown device");
				Log("BSP: Skip device no name");
			}
			else
			{
				deviceList->AddTail(pDevice->szName);
				memcpy(&lpDevsList[devsListIndexCounter++], pDevice, sizeof(BLUETOOTH_DEVICE_INFO));
				Log("BSP: Added %s device to deviceList on index %d",pDevice->szName, devsListIndexCounter);
			}
		}	
			/*DWORD dwMask = 0;
			BLUETOOTH_DEVICE_INFO_EX devInfo = {0};
			memcpy(&devInfo.address, pDevice->address, DEVICE_ADDRESS_LENGTH);
			devInfo.dwSize = sizeof(BLUETOOTH_DEVICE_INFO_EX);
			devInfo.szName[0]='\0';
			dwMask = MASK_DEVICE_NAME;
			
			DWORD dwResult;
			Log("BSP:Try SetRemoteDeviceInfo");
			dwResult = BT_SetRemoteDeviceInfo(dwMask, &devInfo);
			PrintError("BT_SetRemoteDeviceInfo",dwResult);
			dwResult = BT_GetRemoteDeviceInfo(dwMask, &devInfo);
			Log("BSC:Try GetRemoteDeviceInfo");
			PrintError("BT_GetRemoteDeviceInfo",dwResult);
			
			//printf("        Device Name: %s\n", devInfo.szName);			

			if(strlen(devInfo.szName) < 2)
			{
				//printf("\nGOT 250");
				//memcpy(&myDevice, &pDevice, sizeof(BLUETOOTH_DEVICE_INFO));
				//deviceList->AddTail("Unknown device");
				Log("BSP: Skip device no name");
			}
			else
			{
				deviceList->AddTail(devInfo.szName);
				Log("BSP: Added %s device to deviceList on index %d",devInfo.szName, i);
			}

			//PrintDeviceClassInfo(pDevice->classOfDevice);
			if(pDevice->bPaired)
				Log("Device Is Paired: Yes");
			else
				Log("Device Is Paired: No");
		}*/
	}
	return InquiryOk;
}

SoleilProxy::SoleilProxy(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	//Beware of what is done in constructor. The soleilProxy is created when Soleil support is checks
	//do inits in initialize stack
	//LogDisableLogging(1);  //Disabled until told otherwise by the main app
	DWORD DevsListLen = sizeof(BLUETOOTH_DEVICE_INFO) * MAX_DEVICE_COUNT;
	/*for(int i = 0;i<MAX_DEVICE_COUNT;i++)
	{
		lpDevsList[i] = {0};
	}*/
}

SoleilProxy::~SoleilProxy(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	Log("BSP: Uninit BlueSoleil Lib.");
	BT_UninitializeLibrary();	
}

bool SoleilProxy::IsBlueSoleilAvailable()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	HMODULE h = LoadLibrary("btfunc.dll");
    if (h == NULL) {
        return FALSE;
    }
    FreeLibrary(h);
    return TRUE;
}

// See if the BS stack is running
bool SoleilProxy::InitializeStack(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	if(!BT_InitializeLibrary()){
		//printf("BT_InitializeLibrary() failed!\n");
		return false;
	}

	//memset(&myDevice, 0, sizeof(BLUETOOTH_DEVICE_INFO));
	//memset(&myService, 0, sizeof(SPPEX_SERVICE_INFO));
	noOfServicesFound = 0;
	/*dwBtVersion = BT_GetVersion();
	printf("BlueSoleil API version: %d.%d\n",LOBYTE(LOWORD(dwBtVersion)),HIBYTE(LOWORD(dwBtVersion)));
	
	BT_GetBtSpecVersion(&dwBtVersion);
	printf("Compliant BT Spec version: %d.%d\n",LOBYTE(LOWORD(dwBtVersion)),HIBYTE(LOWORD(dwBtVersion)));
	*/	
	return true;
}

bool SoleilProxy::DoInquiry(CStringList* deviceList)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	devsListIndexCounter = 0;
	LogEnterFunction("BSP:DoInquiry");
	UCHAR ucInqMode = INQUIRY_GENERAL_REFRESH;  //Check if we should use these values instead of hardcoded ones
	UCHAR ucInqLen = 15;
	DWORD dwResult;
	bool InquiryOk = false;
	//CStringList* deviceList = new CStringList();  //This is not a good solution, fix with static and pointer sent from BSController
	BLUETOOTH_DEVICE_INFO LocalDevsList[MAX_DEVICE_COUNT] = {0}; //This worked
	DWORD LocalDevsListLen = sizeof(BLUETOOTH_DEVICE_INFO) * MAX_DEVICE_COUNT;
	
	dwResult = BT_InquireDevices(INQUIRY_GENERAL_REFRESH, 0x0A,  &LocalDevsListLen, LocalDevsList);

	PrintError("BT_InquireDevices",dwResult);
	
	if(dwResult == BTSTATUS_SUCCESS){
		//printf("Devices List\n");
		Log("BSP:Inquiry OK");
		InquiryOk = true;
		DWORD i, offset;
		offset = sizeof(BLUETOOTH_DEVICE_INFO);
		BLUETOOTH_DEVICE_INFO *pDevice;
		for(i=0; i<((LocalDevsListLen)/sizeof(BLUETOOTH_DEVICE_INFO)); i++)
		{
			pDevice=(BLUETOOTH_DEVICE_INFO*)((UCHAR*)LocalDevsList+i*offset);
			Log("Device No.%d Device Address: %02X:%02X:%02X:%02X:%02X:%02X",
				i,
				pDevice->address[5],
				pDevice->address[4],
				pDevice->address[3],
				pDevice->address[2],
				pDevice->address[1],
				pDevice->address[0]);
		
			if(strlen(pDevice->szName) < 2)
			{
				//printf("\nGOT 250");
				//memcpy(&myDevice, &pDevice, sizeof(BLUETOOTH_DEVICE_INFO));
				//deviceList->AddTail("Unknown device");
				Log("BSP: Skip device no name");
			}
			else
			{
				deviceList->AddTail(pDevice->szName);
				//memcpy(&lpDevsList[devsListIndexCounter++], pDevice, sizeof(BLUETOOTH_DEVICE_INFO));
				Log("BSP: Added %s device to deviceList on index %d",pDevice->szName, devsListIndexCounter);
			}
		}	
			/*DWORD dwMask = 0;
			BLUETOOTH_DEVICE_INFO_EX devInfo = {0};
			memcpy(&devInfo.address, pDevice->address, DEVICE_ADDRESS_LENGTH);
			devInfo.dwSize = sizeof(BLUETOOTH_DEVICE_INFO_EX);
			devInfo.szName[0]='\0';
			dwMask = MASK_DEVICE_NAME;
			
			DWORD dwResult;
			Log("BSP:Try SetRemoteDeviceInfo");
			dwResult = BT_SetRemoteDeviceInfo(dwMask, &devInfo);
			PrintError("BT_SetRemoteDeviceInfo",dwResult);
			dwResult = BT_GetRemoteDeviceInfo(dwMask, &devInfo);
			Log("BSC:Try GetRemoteDeviceInfo");
			PrintError("BT_GetRemoteDeviceInfo",dwResult);
			
			//printf("        Device Name: %s\n", devInfo.szName);			

			if(strlen(devInfo.szName) < 2)
			{
				//printf("\nGOT 250");
				//memcpy(&myDevice, &pDevice, sizeof(BLUETOOTH_DEVICE_INFO));
				//deviceList->AddTail("Unknown device");
				Log("BSP: Skip device no name");
			}
			else
			{
				deviceList->AddTail(devInfo.szName);
				Log("BSP: Added %s device to deviceList on index %d",devInfo.szName, i);
			}

			//PrintDeviceClassInfo(pDevice->classOfDevice);
			if(pDevice->bPaired)
				Log("Device Is Paired: Yes");
			else
				Log("Device Is Paired: No");
		}*/
	}
	return InquiryOk;
}

void SoleilProxy::DeviceSelected(int index)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	Log("BSP:Selecting device with index %d", index);
	myDevice=lpDevsList[index];
}

bool SoleilProxy::PairSelectedDevice()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	Log("Pair with Device: %02X:%02X:%02X:%02X:%02X:%02X",
				myDevice.address[5],
				myDevice.address[4],
				myDevice.address[3],
				myDevice.address[2],
				myDevice.address[1],
				myDevice.address[0]);

	char lpPinCode[MAX_PIN_CODE_LENGTH]={0};
	WORD wPinCodeLen = 0;
	
	//Implement HP query for pin
	DWORD dwResult;
	//dwResult=BT_PairDevice(&devInfo, wPinCodeLen, ((wPinCodeLen == 0) ? NULL : (UCHAR*)lpPinCode),bKeepOldKeyOnFail,bShowPincode);
	//WORKS without prompt  dwResult=BT_PairDevice(myDev, wPinCodeLen, ((wPinCodeLen == 0) ? NULL : (UCHAR*)lpPinCode),bKeepOldKeyOnFail,bShowPincode);
	//dwResult=BT_PairDevice(&myDev, wPinCodeLen, NULL,FALSE,FALSE);
	dwResult=BT_PairDevice(&myDevice, 0, NULL,TRUE,FALSE);//This combo show prompt
	PrintError("BT_PairDevice",dwResult);
	if(myDevice.bPaired == TRUE)
	{
		Log("Pairing Ok");
	}
	else
	{
		Log("Pairing failed");
	}
	return (myDevice.bPaired == TRUE);
}

void SoleilProxy::UnpairSelectedDevice()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	DWORD dwResult;
	dwResult = BT_UnpairDevice(myDevice.address);
	PrintError("BT_UnpairDevice",dwResult);
}

bool SoleilProxy::DoServiceSearch(CStringList* serviceList)
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	//memset(&sppex_svc_info, 0, 5*sizeof(SPPEX_SERVICE_INFO));
	
	sppex_svc_info[0].dwSize = sizeof(SPPEX_SERVICE_INFO);
 
	DWORD dwResult;
	DWORD dwLength = 5*sizeof(SPPEX_SERVICE_INFO);
	
	BYTE UUID[8] = {3,0,0,0,0,0,0,0};
	GUID service_guid;
	service_guid.Data1 = 0x00000003;
	service_guid.Data2 = 0x0000;
	service_guid.Data3 = 0x1000;
	service_guid.Data4[0] = 0x80;
	service_guid.Data4[1] = 0x00;
	service_guid.Data4[2] = 0x00;
	service_guid.Data4[3] = 0x80;
	service_guid.Data4[4] = 0x5F;
	service_guid.Data4[5] = 0x9B;
	service_guid.Data4[6] = 0x34;
	service_guid.Data4[7] = 0xFB;
	
	memcpy(&(sppex_svc_info[0].serviceClassUuid128), &service_guid, sizeof(UUID));

	//After return dwLength contains the actual size, if one service is found it is not 5* anymore
	dwResult = BT_SearchSPPExServices(&myDevice,&dwLength,sppex_svc_info);
	
	//end
	PrintError("BT_SearchSPPExServices",dwResult);
	if(dwResult != BTSTATUS_SUCCESS)
	{
		return false;
	}
	noOfServicesFound = dwLength/sizeof(SPPEX_SERVICE_INFO);  //used in getSCN 
	Log("BSP: Found %d services on %s", noOfServicesFound, myDevice.szName);

	for(int i=0;i<dwLength/sizeof(SPPEX_SERVICE_INFO);i++){
		Log("SPPEx Service Info No.%d",i);
		Log("SDAP Record Handle:	%d", sppex_svc_info[i].dwSDAPRecordHanlde);
		Log("Service Name:	%s", sppex_svc_info[i].szServiceName);
		Log("Service Channel:	%02X", sppex_svc_info[i].ucServiceChannel);
		Log("COM Port:	%d", sppex_svc_info[i].ucComIndex);
		Log("COM Port:	%d\n", sppex_svc_info[i].ucComIndex);

		/*if(strstr(sppex_svc_info[i].szServiceName, "Headset") != 0 || strstr(sppex_svc_info[i].szServiceName, "headset") != 0)
		{
			Log("Got Headset service");
			memcpy(&myService, &sppex_svc_info[i], sizeof(SPPEX_SERVICE_INFO));
		}*/
	}
	return true;
}
int SoleilProxy::GetNoOfServicesFound()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	return noOfServicesFound;
}

int SoleilProxy::GetSCNOfService(CString serviceNameToFind)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	CString serviceName; 
	int SCNToReturn = -1;  //SCN_NOT_FOUND
	serviceNameToFind.MakeLower();
	for(int i=0;i<noOfServicesFound;i++){
		serviceName = sppex_svc_info[i].szServiceName;
		serviceName.MakeLower();
		Log("BSP: Checking,does serviceName[%d]=%s match serviceToFind = %s (lowercase)",i, serviceName,serviceNameToFind);
		if(serviceName.Find(serviceNameToFind) != -1)
		{
			Log("Found %s service", serviceNameToFind);
			SCNToReturn = i;
			//memcpy(&myService, &sppex_svc_info[i], sizeof(SPPEX_SERVICE_INFO));
		}
	}
	return SCNToReturn;
}

bool SoleilProxy::ConnectService(int SCN)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	bool connectionOk = false;
	Log("Will try to connect to myService on SCN=%d",SCN);
	Log("SDAP Record Handle:	%d", myService.dwSDAPRecordHanlde);
	Log("Service Name:	%s", myService.szServiceName);
	Log("Service Channel:	%02X", myService.ucServiceChannel);
	Log("COM Port:	%d", myService.ucComIndex);

	DWORD dwResult, dwHandle;
	dwResult = BT_ConnectSPPExService(&myDevice,&myService,&dwHandle);
	PrintError("BT_SearchSPPExServices",dwResult);
	
	if(dwResult == BTSTATUS_SUCCESS)
	{
		Log("BSP: Connection OK");
		StartIoOnComport(dwHandle,myService.ucComIndex, TRUE);
		connectionOk = true;
	}
	return connectionOk;
}

bool SoleilProxy::StartIoOnComport(DWORD dwConnHandle,int nComport, BOOL bClientRole)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	LogEnterFunction("BSP:StartIoOnComport");
	int i;
	DWORD dwID;
	bool bRet = false;
/*
	for(i = 0; i<MAX_IO_THREAD; i++){
		if(g_comIoList[i].hThread == NULL){
			g_comIoList[i].nComPort = nComport;
			g_comIoList[i].dwConnHandle = dwConnHandle;
			if(bClientRole)
				g_comIoList[i].hThread = CreateThread(NULL,0,SPPClientIOThread,(LPVOID)i,0,&dwID);		
			else
				g_comIoList[i].hThread = CreateThread(NULL,0,SPPServerIOThread,(LPVOID)i,0,&dwID);
			if(g_comIoList[i].hThread == NULL){
				memset(&g_comIoList[i], 0, sizeof(COM_IO));
				bRet = FALSE;
			}else
				bRet = TRUE;
		   break;
		}
	}
*/
	return bRet;
	
}

void SoleilProxy::PrintError(CHAR* lpszAPI,DWORD dwError)
{
	//printf(lpszAPI);
	//printf(" returned %d: ",dwError);
	CString msg;
	switch(dwError){
	case BTSTATUS_SUCCESS:
		msg = "Succeeded";
		break;
	case BTSTATUS_FAIL:
		msg = "General failure";
		break;
	case BTSTATUS_SYSTEM_ERROR:
		msg = "System error occured";
		break;
	case BTSTATUS_BT_NOT_READY:
		msg = "BT not ready";
		break;
	case BTSTATUS_ALREADY_PAIRED:
		msg = "BS is already paired with this device";
		break;
	case BTSTATUS_AUTHENTICATE_FAILED:
		msg = "Authentication failed";
		break;
	case BTSTATUS_BT_BUSY:
		msg = "Bluetooth is busy browsing services or connecting to another device";
		break;
	case BTSTATUS_CONNECTION_EXIST:
		msg = "The connection to the service is already established";
		break;
	case BTSTATUS_CONNECTION_NOT_EXIST:
		msg = "The connection does not exist or is released.";
		break;
	case BTSTATUS_PARAMETER_ERROR:
		msg = "Parameter error";
		break;
	case BTSTATUS_SERVICE_NOT_EXIST:
		msg = "Service does not exist.";
        break; 
	default:
		msg = "Unknown error";
	}
	Log("BSP:BS commands returned status msg %s", msg);
	if(dwError != BTSTATUS_SUCCESS)
	{
		CString errorMsg = "An error occured, if this is a repeating problem contact support at support@headsetpresenter.com and provide the following error message: \n\n" + msg;
		//MessageBox(NULL,errorMsg,"BlueSoleil error", MB_OK);
	}
}

void SoleilProxy::ActivateLogging()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	LogDisableLogging(0); //Enable it
	Log("BSP:Log enabled for BSProxy");
}

