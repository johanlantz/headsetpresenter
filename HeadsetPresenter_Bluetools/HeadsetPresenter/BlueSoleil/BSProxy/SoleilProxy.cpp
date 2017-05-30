#include "StdAfx.h"
#include ".\soleilproxy.h"
#include "ComPort.h"
#include "SS_Log.h"

int devsListIndexCounterAsync = 0;

typedef struct _DEV_LIST{
	BLUETOOTH_DEVICE_INFO dev;
	_DEV_LIST *pNext;
}DEV_LIST;
DEV_LIST *g_pDevList=NULL;
void InquiryDeviceInd(/*in*/PBLUETOOTH_DEVICE_INFO pDevInfo)
{
	 BYTE zeroBuff[32]={0};

	 DEV_LIST* pNode;

 	 Log("Inquiry Ind:");

    if( memcmp(pDevInfo->address,zeroBuff,DEVICE_ADDRESS_LENGTH) == 0 &&
         memcmp(pDevInfo->classOfDevice,zeroBuff,DEVICE_CLASS_LENGTH) == 0)
    {
        //This is the end of the inquiry
		 //Log("Inquiry completes. BlueSoleil might now continue to query device names");
		Log("Inquiry complete");
		 while(g_pDevList)
		 {
			 pNode = g_pDevList;
			 g_pDevList = g_pDevList->pNext;
			 free(pNode);
		 }
		 g_pDevList = NULL;
		 myEventCB("InquiryDone",myController);  //Send InquiryDone event to callback in BSController
		 return;
     }
	 pNode = g_pDevList;
	 while(pNode){
		 if(memcmp(pNode->dev.address,pDevInfo->address,DEVICE_ADDRESS_LENGTH)==0){
			 Log("duplicated dev report.");
			 return;
		 }
		 pNode = pNode->pNext;
	 }
	 pNode = (DEV_LIST*)malloc(sizeof(DEV_LIST));
	 memcpy(&(pNode->dev),pDevInfo,sizeof(BLUETOOTH_DEVICE_INFO));
	 pNode->pNext = g_pDevList;
	 g_pDevList = pNode;

	 
	 Log("Address:%02X:%02X:%02X:%02X:%02X:%02X",
		 pDevInfo->address[5],
		 pDevInfo->address[4],
		 pDevInfo->address[3],
		 pDevInfo->address[2],
		 pDevInfo->address[1],
		 pDevInfo->address[0]);

	 //If get device name afresh
		 DWORD dwMask = 0;
		 BLUETOOTH_DEVICE_INFO_EX devInfo = {0};
		 memcpy(&devInfo.address, pDevInfo->address, DEVICE_ADDRESS_LENGTH);
		 devInfo.dwSize = sizeof(BLUETOOTH_DEVICE_INFO_EX);
		 devInfo.szName[0]='\0';
		 dwMask = MASK_DEVICE_NAME;
		 
		 //Make sure that devInfo is used not pDevInfo if the dev name is accessed
		 //it is not certain that the pDevInfo contains the device name it appears.
		 DWORD dwResult;
		 dwResult = BT_SetRemoteDeviceInfo(dwMask, &devInfo);
		 PrintError("BT_SetRemoteDeviceInfo",dwResult);
		 dwResult = BT_GetRemoteDeviceInfo(dwMask, &devInfo);
		 PrintError("BT_GetRemoteDeviceInfo",dwResult);
		 
		 Log("Name:%s", devInfo.szName);	
		
		 
		 CString deviceInfo;
		//inform HP about the device
		//if(strlen(pDevInfo->szName) < 2)  //2008-03 changed to use the devInfo instead of the pointer it appeared as sometimes devInfo had a device name while pDevInfo had not since we had not maksed it
		if(strlen(devInfo.szName) < 2)
		{
			char  buf[64];
			sprintf(buf,"%02X:%02X:%02X:%02X:%02X:%02X",pDevInfo->address[5],pDevInfo->address[4],pDevInfo->address[3],pDevInfo->address[2],pDevInfo->address[1],pDevInfo->address[0]);
			deviceInfo = CString(buf) + CString("  Device name not found. Ok to use anyway.");
		}
		else
		{
			//deviceInfo = CString(pDevInfo->szName); //2008-03 change to devInfo
			deviceInfo = CString(devInfo.szName);
		}
		//deviceList->AddTail(deviceInfo);
		memcpy(&DevsList[devsListIndexCounterAsync], pDevInfo, sizeof(BLUETOOTH_DEVICE_INFO));
		//Log("BSP: Added %s device to deviceList on index %d",pDevInfo->szName, devsListIndexCounterAsync);
		Log("BSP: Added %s device to deviceList on index %d",devInfo.szName, devsListIndexCounterAsync);
		devsListIndexCounterAsync++;

		//myDeviceCB(devInfo.szName, myController);
		myDeviceCB(deviceInfo.GetBuffer(), myController);

    Log("Class:%02x:%02x:%02x",
		pDevInfo->classOfDevice[2],
		pDevInfo->classOfDevice[1],
		pDevInfo->classOfDevice[0]);
	
	//PrintDeviceClassInfo(pDevInfo->classOfDevice);

	Log("Pair Status: %d",pDevInfo->bPaired);
}

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

void BS_DataReceivedOnCOM(char* data)
{
	Log("BSP:Comport data reached BSP %s", data);
	myDataCB(data, myController);
}

void BS_SetLastUsedDevice(CString* lastAddress)
{
	UCHAR g_targetBdAddr[6]={0x00,0x13,0x17,0x28,0x93,0x08};
	myDevice = (BLUETOOTH_DEVICE_INFO*)malloc(sizeof(BLUETOOTH_DEVICE_INFO));
	myDevice->dwSize = sizeof(BLUETOOTH_DEVICE_INFO);
	memcpy(myDevice->address,g_targetBdAddr,6);
	myService = (SPPEX_SERVICE_INFO*)malloc(sizeof(SPPEX_SERVICE_INFO));
	myService->ucServiceChannel = 1;
	myService->dwSize = sizeof(SPPEX_SERVICE_INFO);
	myService->ucComIndex = 0;
	myService->dwSDAPRecordHanlde = 65536;
}

void BS_SetControllerObject(void* BSController_p)
{
	myController = BSController_p;
}

void BS_SetDataCallback(CallbackFunc iCB)
{
	//Log("BSP:Setting DataCallbackFunc");
	myDataCB = iCB;
}

void BS_SetDeviceCallback(CallbackFunc iCB)
{
	//Log("BSP:Setting DeviceCallbackFunc");
	myDeviceCB = iCB;
}

void BS_SetEventCallback(CallbackFunc iCB)
{
	//Log("BSP:Setting EventCallbackFunc");
	myEventCB = iCB;
}

bool BS_IsBlueSoleilAvailable()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	
#define LOG_DISABLED 0x10 //This is just to keep it as in HP SettingsControl
	LogDisableLogging(LOG_DISABLED);  //Disabled until told otherwise by the main app
	HMODULE h = LoadLibrary("btfunc.dll");
    if (h == NULL) {
        return FALSE;
    }
    FreeLibrary(h);
    return TRUE;
}

// See if the BS stack is running
bool BS_InitializeStack(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	if(!BT_InitializeLibrary()){
		//printf("BT_InitializeLibrary() failed!\n");
		return false;
	}

	//memset(&myDevice, 0, sizeof(BLUETOOTH_DEVICE_INFO));
	//memset(&myService, 0, sizeof(SPPEX_SERVICE_INFO));
	noOfServicesFound = 0;
	dwServiceHandle = 0;
	/*dwBtVersion = BT_GetVersion();
	printf("BlueSoleil API version: %d.%d\n",LOBYTE(LOWORD(dwBtVersion)),HIBYTE(LOWORD(dwBtVersion)));
	
	BT_GetBtSpecVersion(&dwBtVersion);
	printf("Compliant BT Spec version: %d.%d\n",LOBYTE(LOWORD(dwBtVersion)),HIBYTE(LOWORD(dwBtVersion)));
	*/	
	DWORD dwResult;
	dwResult=BT_RegisterCallback(EVENT_INQUIRY_DEVICE_REPORT, InquiryDeviceInd);
	//PrintError("BT_RegisterCallback EVENT_INQUIRY_DEVICE_REPORT",dwResult);
	return true;
}

void BS_UnInitializeStack()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	//Log("BSP: Uninit BlueSoleil Lib."); //Do not log this due to log act/deact issues
	BT_UninitializeLibrary();	
}

bool BS_DoInquiry(CStringList* deviceList, bool refresh)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	int devsListIndexCounter = 0;
	devsListIndexCounterAsync = 0;
	LogEnterFunction("BSP:DoInquiry");
	UCHAR ucInqMode = INQUIRY_GENERAL_MODE; 
	UCHAR ucInqLen = 15;
	DWORD dwResult;
	bool InquiryOk = false;
	if(refresh)
	{
		Log("BSP: Changing inquiry mode to REFRESH");
		ucInqMode = INQUIRY_GENERAL_REFRESH;
	}
	//CStringList* deviceList = new CStringList();  //This is not a good solution, fix with static and pointer sent from BSController
	BLUETOOTH_DEVICE_INFO LocalDevsList[MAX_DEVICE_COUNT] = {0}; //This worked
	DWORD LocalDevsListLen = sizeof(BLUETOOTH_DEVICE_INFO) * MAX_DEVICE_COUNT;
	
	bool bAsynchronous = true;
	if(bAsynchronous)
	{
		Log("BSP:Doing asynch inquiry");
		LocalDevsListLen = 0;
	}
	dwResult = BT_InquireDevices(ucInqMode, 0x0A,  &LocalDevsListLen, LocalDevsList);

	PrintError("BT_InquireDevices",dwResult);
	
	if(dwResult == BTSTATUS_SUCCESS && !bAsynchronous){
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
		
			/*if(strlen(pDevice->szName) < 2)
			{
				//printf("\nGOT 250");
				//memcpy(&myDevice, &pDevice, sizeof(BLUETOOTH_DEVICE_INFO));
				//deviceList->AddTail("Unknown device");
				Log("BSP: Skip device no name");
			}
			else
			{*/
				//Store device info
				DWORD dwMask = 0;
				BLUETOOTH_DEVICE_INFO_EX devInfo = {0};
				memcpy(&devInfo.address, pDevice->address, DEVICE_ADDRESS_LENGTH);
				devInfo.dwSize = sizeof(BLUETOOTH_DEVICE_INFO_EX);
				devInfo.szName[0]='\0';
				dwMask = MASK_DEVICE_NAME;
				DWORD dwResult;
				Log("BSP:Try SetRemoteDeviceInfo");
				dwResult = BT_SetRemoteDeviceInfo(dwMask, &devInfo);
				PrintError("BT_SetRemoteDeviceInfo",dwResult);

				CString deviceInfo;
				//inform HP about the device
				if(strlen(pDevice->szName) < 2)
				{
					char  buf[64];
					sprintf(buf,"%02X:%02X:%02X:%02X:%02X:%02X",pDevice->address[5],pDevice->address[4],pDevice->address[3],pDevice->address[2],pDevice->address[1],pDevice->address[0]);
					deviceInfo = CString(buf) + CString("  Device name not found. Ok to use anyway.");
				}
				else
				{
					deviceInfo = CString(pDevice->szName);
				}
				deviceList->AddTail(deviceInfo);
				memcpy(&DevsList[devsListIndexCounter++], pDevice, sizeof(BLUETOOTH_DEVICE_INFO));
				Log("BSP: Added %s device to deviceList on index %d",pDevice->szName, devsListIndexCounter);
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


void BS_DeviceSelected(int index)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	Log("BSP:Selecting device with index %d", index);
	myDevice=&DevsList[index];
	//memcpy(&myDevice, &DevsList[index], sizeof(BLUETOOTH_DEVICE_INFO));
}

bool BS_PairSelectedDevice()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	Log("Pair with Device: %02X:%02X:%02X:%02X:%02X:%02X",
				myDevice->address[5],
				myDevice->address[4],
				myDevice->address[3],
				myDevice->address[2],
				myDevice->address[1],
				myDevice->address[0]);

	char lpPinCode[MAX_PIN_CODE_LENGTH]={0,0,0,0};
	WORD wPinCodeLen = 4;
	
	//Implement HP query for pin
	DWORD dwResult;
	//dwResult=BT_PairDevice(&devInfo, wPinCodeLen, ((wPinCodeLen == 0) ? NULL : (UCHAR*)lpPinCode),bKeepOldKeyOnFail,bShowPincode);
	//WORKS without prompt  dwResult=BT_PairDevice(myDev, wPinCodeLen, ((wPinCodeLen == 0) ? NULL : (UCHAR*)lpPinCode),bKeepOldKeyOnFail,bShowPincode);
	Log("BSP:Try pair without dlg");
	//dwResult=BT_PairDevice(&myDevice, 4, (UCHAR*)lpPinCode,FALSE,TRUE);
	
//dwResult=BT_PairDevice(&myDev, wPinCodeLen, NULL,FALSE,FALSE);
	
	//This work and shows prompt but com port fails
	dwResult=BT_PairDevice(myDevice, 0, NULL,TRUE,FALSE);//This combo show prompt
	PrintError("BT_PairDevice",dwResult);
	if(dwResult == BTSTATUS_SUCCESS)
	{
		//Note myDevice is copied at Inquiry the information in this struct is NOT updated automatically
		myDevice->bPaired = TRUE;
		Log("Pairing Ok");
	}
	else
	{
		Log("Pairing failed or already paired (could be ok)");
	}
	return (myDevice->bPaired == TRUE);
}

void BS_UnpairSelectedDevice()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	DWORD dwResult;
	dwResult = BT_UnpairDevice(myDevice->address);
	PrintError("BT_UnpairDevice",dwResult);
}

bool BS_DoServiceSearch(CStringList* serviceList)
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
	dwResult = BT_SearchSPPExServices(myDevice,&dwLength,sppex_svc_info);
	
	//end
	PrintError("BT_SearchSPPExServices",dwResult);
	if(dwResult != BTSTATUS_SUCCESS)
	{
		MessageBox(NULL,"BlueSoleil failed to perform a service search on your device.\nThis is due to limitations in BlueSoleil and out of the control for HeadsetPresenter.\n\nThere is one thing that usually solve this problem and that is doing a Refresh Services in the BlueSoleil user interface.\nThis procedure is described in detail in Chapter 5.4 in HeadsetPresenter For Beginners available in the HeadsetPresenter folder.\n\nIf this problem persists, try using Advanced mode instead","ServiceSearch fail",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		return false;
	}
	noOfServicesFound = dwLength/sizeof(SPPEX_SERVICE_INFO);  //used in getSCN 
	Log("BSP: Found %d services on %s", noOfServicesFound, myDevice->szName);

	for(int i=0;i<dwLength/sizeof(SPPEX_SERVICE_INFO);i++){
		Log("SPPEx Service Info No.%d",i);
		Log("SDAP Record Handle:%d", sppex_svc_info[i].dwSDAPRecordHanlde);
		Log("Service Name:%s", CString(sppex_svc_info[i].szServiceName));
		Log("Service Channel:%02X", sppex_svc_info[i].ucServiceChannel);
		Log("COM Port:%d", sppex_svc_info[i].ucComIndex);
		Log("COM Port:%d\n", sppex_svc_info[i].ucComIndex);

		serviceList->AddTail(CString(sppex_svc_info[i].szServiceName));

		/*if(strstr(sppex_svc_info[i].szServiceName, "Headset") != 0 || strstr(sppex_svc_info[i].szServiceName, "headset") != 0)
		{
			Log("Got Headset service");
			memcpy(&myService, &sppex_svc_info[i], sizeof(SPPEX_SERVICE_INFO));
		}*/
	}
	return true;
}
int BS_GetNoOfServicesFound()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	return noOfServicesFound;
}

int BS_GetSCNOfService(CString serviceNameToFind)
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
			//SCNToReturn = i;
			SCNToReturn = sppex_svc_info[i].ucServiceChannel;
			//memcpy(&myService, &sppex_svc_info[i], sizeof(SPPEX_SERVICE_INFO));
			myService = &sppex_svc_info[i];
		}
	}

	//if we did not get the SCN for this service give a try with either 0 or 1
	//Actually trying to change the SCN for an existing service record, not sure if this
	//will have any chance to work
	//Maybe a SCN value could be passed to this function to override the SCN if there are problems using 1 or 2
	/*if(SCNToReturn == -1 && noOfServicesFound > 0)
	{
		UCHAR SCN = 1;
		if(sppex_svc_info[0].ucServiceChannel == SCN)
		{
			Log("BSP:Service not found for record 0, changing to SCN 2");
			sppex_svc_info[0].ucServiceChannel = 2;
			SCNToReturn = 2;
		}
		else
		{
			Log("BSP:Service not found for record 1, changing to SCN 1");
			sppex_svc_info[0].ucServiceChannel = 1;
			SCNToReturn = 1;
		}
	}*/
	return SCNToReturn;  
}

bool BS_ConnectService(int SCN)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	bool connectionOk = false;
	Log("Will try to connect to myService on SCN=%d",SCN);
	Log("SDAP Record Handle:%d", myService->dwSDAPRecordHanlde);
	Log("Service Name:%s", CString(myService->szServiceName));
	Log("Service Channel:%02X", myService->ucServiceChannel);
	Log("COM Port:%d", myService->ucComIndex);

	DWORD dwResult;
	dwResult = BT_ConnectSPPExService(myDevice,myService,&dwServiceHandle);
	PrintError("BT_SearchSPPExServices",dwResult);
	
	if(dwResult == BTSTATUS_SUCCESS)
	{
		Log("BSP: Connection ok try to open com");
		connectionOk = StartIoOnComport(dwServiceHandle,myService->ucComIndex, TRUE);
	}
	return connectionOk;
}

void BS_DisconnectService()
{
	if(dwServiceHandle != 0) 
	{
		DWORD dwResult;
		dwResult = BT_DisconnectSPPExService(dwServiceHandle);
		if(dwResult == BTSTATUS_SUCCESS)
		{
			Log("BSP:Disconnect ok");
		}
		else
		{
			Log("BSP:Disconnect failed: cause %d", dwResult);
		}
	}
 }
/*bool BS_StartIoOnComport(DWORD dwConnHandle,int nComport, BOOL bClientRole)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	LogEnterFunction("BSP:StartIoOnComport");
	int i;
	DWORD dwID;
	bool bRet = false;

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

	return bRet;
	
}
*/


void BS_ActivateLogging()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));  //Need this apparantly when exporting functions using MFC
	LogDisableLogging(0); //Enable it
	Log("BSP:Log enabled for BSProxy");
}

