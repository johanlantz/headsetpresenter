#include "StdAfx.h"
#include ".\bscontroller.h"
#include "HeadsetPresenterDlg.h"
#include "SS_Log.h"

BSController::BSController(void)
{
	//mySoleilProxy = new SoleilProxy();
	controllerMode = NORMAL_MODE_BLUESOLEIL;
	Log("BSC:Set controller mode to NORMAL_MODE_BLUESOLEIL = %d",NORMAL_MODE_BLUESOLEIL);
	servicesListAsCStringList = new CStringList(); //Kept here since it is used by ServiceSelectionDlg in case of manual serviceselection
	
	deviceDiscoveryDialog = new DeviceDiscoveryDialog(); //Should use the same declare in constructor?
	serviceDiscoveryDialog = new ServiceDiscoveryDialog();
	deviceDiscoveryDialog->SetController(this);  //Can this be done in the superclass?????
	serviceDiscoveryDialog->SetController(this);

	Log("BSC:Create BSController");

	isInitialised = false; 
	

}

BSController::~BSController(void)
{
	Log("BSC:Delete BSController");
	if(isInitialised) //If we failed to init, do not uninit
	{
		BS_DisconnectService();  //Always call disconnect the Proxy will know if we are connected
		BS_UnInitializeStack();
	}
}

bool BSController::ShouldHPQueryForPIN()
{
	return true; //could be a setting decide which is best TBI in proxy
}

bool BSController::BondDevice()
{
	LogEnterFunction("BTC:BondDevice");
	//Currently only soleil pin dialog
	bool bondOK = BS_PairSelectedDevice();
	return bondOK;
}

void BSController::CancelDeviceDiscovery()
{
	Log("BSC:CancelDeviceDiscovery not impl. in BSC yet");
}

void BSController::UnbondDevice()
{
	//Pending usage of this for BS might be needed
	//mySoleilProxy.UnpairSelectedDevice();
}


//Used by controller factory when checking for Soleil support
//Could create with new and keep the object instead
bool BSController::IsRadioAvailable()
{
	//DWORD dwBtVersion;
	bool radioAvailable = false;
	//No logging can be done before this code, the IsBlueSoleilAvailable does not log neither produce error messages
	if(BS_IsBlueSoleilAvailable())
	{
		Log("BSC: BlueSoleil dll was found");
		if(BS_InitializeStack())
		{
			Log("BSC: BlueSoleil init ok");
			radioAvailable = true;
			isInitialised = true;
			BS_SetControllerObject(this);
			BS_SetDataCallback(&DataCallbackFunc);
			BS_SetDeviceCallback(&DeviceCallbackFunc);
			BS_SetEventCallback(&EventCallbackFunc);
		}
		else
		{
			Log("BSC: BlueSoleil init failed");
			radioAvailable = false;
		}
	}
	else
	{
		Log("BSC: BlueSoleil dll was not found");
	}
	//NOTE must I make an unit before making init again when creatin the real controller
	return radioAvailable;
}

bool BSController::ConnectToLastUsedDevice()
{
	MessageBox(NULL,"Reconnection is not supported by BlueSoleil.\nThe pairing wizard or Advanced mode must be used.","Reconnection not possible.",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
	//BS_SetLastUsedDevice(NULL);  //Connect without pairing is not possible
	//BS_ConnectService(2);
	return false;
}
bool BSController::ShowDeviceSearchDialog()
{
	return SuperShowDeviceSearchDialog();
}

void BSController::DiscoverDevices(bool refresh)
{
	LogEnterFunction("BSC:DiscoverDevices");
	CStringList*  discoveredDevices = new CStringList();
	BS_DoInquiry(discoveredDevices, refresh);
	POSITION cur;
	cur = discoveredDevices->GetHeadPosition();
	while(cur != NULL)
	{
		deviceDiscoveryDialog->AddDeviceToList(discoveredDevices->GetNext(cur));	
	}
	//deviceDiscoveryDialog->AddDeviceToList("nisse");
}

void BSController::DeviceSelected(int deviceIndex)
{
	BS_DeviceSelected(deviceIndex);
}

/*Called by ServiceSelectionDialog. Gets the stored list from DiscoverServicesSync*/
CStringList* BSController::GetServiceListAsCStringList()
{
	return servicesListAsCStringList;
}

bool BSController::ConnectToSelectedDevice(int SCN, CString serviceName )
{
	BS_ConnectService(SCN);
	return true;
}

void BSController::DiscoverServicesAsync()
{
	
}

/*The service list is always built and kept by DiscoverServicesSync
*if the manual service selection dlg wants the list it gets the stored one*/
bool BSController::DiscoverServicesSync()
{
	BS_DoServiceSearch(servicesListAsCStringList);
	return true;
}

int BSController::GetNoOfServicesDiscovered()
{
	return BS_GetNoOfServicesFound();
}
int BSController::GetSCNOfService(CString serviceNameToFind)
{
	return BS_GetSCNOfService(serviceNameToFind);
}


void BSController::InitLocalSettings() 
{
	if(!mySettingsControl->GetLogDisabled())
	{
		BS_ActivateLogging();
	}
}
int BSController::GetMode()
{
	return controllerMode;
}

bool BSController::Start()
{
	return true;
}

/*The DataCallbackFunc must be static to work with the dll callback
  Therefor the dll gets the object pointer to use in the callback and then
  it is typecasted here to call the non static functions.
  */
int __stdcall BSController::DataCallbackFunc(char* msg, void* controller)
{
	BSController* myController = (BSController*)controller;
	myController->OnReadCompleted(msg);
	return 1;  //Not used
}

/*The DataCallbackFunc must be static to work with the dll callback
  Therefor the dll gets the object pointer to use in the callback and then
  it is typecasted here to call the non static functions.
  */
int __stdcall BSController::DeviceCallbackFunc(char* msg, void* controller)
{
	BSController* myController = (BSController*)controller;
	myController->DeviceFound(msg);
	return 1;  //Not used
}

/*Handle events callback, currently limited to:
1. DeviceDiscoveryComplete
  */
int __stdcall BSController::EventCallbackFunc(char* msg, void* controller)
{
	BSController* myController = (BSController*)controller;
	myController->HandleEvent(msg);
	return 1;  //Not used
}

void BSController::DeviceFound(char* deviceName)
{
	Log("BSC:Device found named %s", deviceName);
	deviceDiscoveryDialog->AddDeviceToList(deviceName);	
}

void BSController::OnReadCompleted(char* data)
{
	//bool dataIsATCommand = false;
	int commandType = 0;
	CString dataString;
	dataString = data;
	Log("GC:data read and copied as : %s",dataString);
	
	
	//1. Check if it is an AT command
	if(dataString.Find("AT+") != -1 )
	{
		Log("GC:Data is AT command");
		//dataIsATCommand = true;
		commandType = AT_COMMAND;
		HandleStatesAndForwardCommand(dataString, commandType, TRUE);
	}
	/*Not AT command, AVRCP not supported
	else
	{
		commandType = AVRCP_COMMAND;
		Log("GC:Data is not AT command, not forwarding : %s",dataString);
	}*/

	
}

void BSController::Write(unsigned char* data){}