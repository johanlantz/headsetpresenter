#pragma once

/*This .h file is included in HeadsetPresenter project but as a dll import
* also the bt_ui.h cannot be included in that proj.
*
* The dll is delayLoaded (option in the Link menu).
* That way if the user does not have btfunc.dll installed HP can run anyway.
*/

//This function prototype is used for all callbacks in a controller taking a string as parameter
//Current users are deviceName and AT commands
typedef int (__stdcall *CallbackFunc)(char* data, void* controller);

#define MAX_DEVICE_COUNT 100 //used in inquiry

#ifdef _EXPORTER
	#define DLL_DIRECTION __declspec(dllexport)
	#include "bt_ui.h"
	
	static BLUETOOTH_DEVICE_INFO DevsList[MAX_DEVICE_COUNT];

	static BLUETOOTH_DEVICE_INFO* myDevice;
	static SPPEX_SERVICE_INFO*	myService;
	static SPPEX_SERVICE_INFO sppex_svc_info[5];
	static int noOfServicesFound;
	static CallbackFunc myDataCB;
	static CallbackFunc myDeviceCB;
	static CallbackFunc myEventCB;
	static void* myController;
	static DWORD dwServiceHandle;
#else
	#define DLL_DIRECTION __declspec(dllimport)
#endif

void PrintError(CHAR* lpszAPI,DWORD dwError);
void BS_DataReceivedOnCOM(char* data);
DLL_DIRECTION void BS_SetDataCallback(CallbackFunc iCB);	
DLL_DIRECTION void BS_SetDeviceCallback(CallbackFunc iCB);
DLL_DIRECTION void BS_SetEventCallback(CallbackFunc iCB);
DLL_DIRECTION void BS_SetControllerObject(void* BSController_p);
DLL_DIRECTION bool BS_IsBlueSoleilAvailable();	
DLL_DIRECTION bool BS_InitializeStack(void);
DLL_DIRECTION bool BS_DoInquiry(CStringList* deviceList, bool refresh);
DLL_DIRECTION bool BS_InitializeStack(void);
DLL_DIRECTION void BS_UnInitializeStack();
DLL_DIRECTION bool BS_DoInquiry(CStringList* deviceList);
DLL_DIRECTION void BS_DeviceSelected(int index);
DLL_DIRECTION bool BS_PairSelectedDevice();
DLL_DIRECTION void BS_UnpairSelectedDevice();
DLL_DIRECTION bool BS_DoServiceSearch(CStringList* serviceList);
DLL_DIRECTION int BS_GetNoOfServicesFound();
DLL_DIRECTION int BS_GetSCNOfService(CString serviceNameToFind);  //is not pointer `??
DLL_DIRECTION void BS_SetLastUsedDevice(CString* lastAddress);
DLL_DIRECTION bool BS_ConnectService(int SCN);
DLL_DIRECTION void BS_DisconnectService();
DLL_DIRECTION bool BS_StartIoOnComport(DWORD dwConnHandle,int nComport, BOOL bClientRole);
DLL_DIRECTION void BS_ActivateLogging();

//Must load btfunc.lib dynamically since it might not exist
/*class DLL_DIRECTION SoleilProxy
{
public:
	SoleilProxy(void);
	~SoleilProxy(void);
	// See if the BS stack is available
	bool IsBlueSoleilAvailable();
	
	bool InitializeStack(void);
	bool DoInquiry(CStringList* deviceList);
	void DeviceSelected(int index);
	bool PairSelectedDevice();
	void UnpairSelectedDevice();
	bool DoServiceSearch(CStringList* serviceList);
	int GetNoOfServicesFound();
	int GetSCNOfService(CString serviceNameToFind);
	bool ConnectService(int SCN);
	bool StartIoOnComport(DWORD dwConnHandle,int nComport, BOOL bClientRole);
	void PrintError(CHAR* lpszAPI,DWORD dwError);
	void ActivateLogging();

//private:
	int noOfServicesFound;

#ifdef _EXPORTER
//private:
	//As soon as any of these params are used it will crash, only locals work???
	//BLUETOOTH_DEVICE_INFO* myDev = (BLUETOOTH_DEVICE_INFO*)malloc(sizeof(BLUETOOTH_DEVICE_INFO));
	BLUETOOTH_DEVICE_INFO myDevice;
	SPPEX_SERVICE_INFO	myService;
	BLUETOOTH_DEVICE_INFO lpDevsList[MAX_DEVICE_COUNT]; //not init to {0} like in SDK, should not matter
	SPPEX_SERVICE_INFO sppex_svc_info[5];
	DWORD DevsListLen;
	int devsListIndexCounter;
	

#endif
};*/
