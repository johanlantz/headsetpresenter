//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
#pragma once
#include "SuperController.h"


//#include "DeviceDiscoveryDialog.h"
//#include "ServiceDiscoveryDialog.h"
//#include "WizardDialog.h"
//#include "SettingsControl.h"
#include "xBlueTools.h"
using namespace xCore;
using namespace xBlueTools;

//class CHeadsetPresenterDlg;

//#define SCN_NOT_FOUND -1




//Timers
#define RECONNECT_TIMER 5000
//End Timers
/*
#define NO_SERVICE_CONNECTED 0
#define HFP_SERVICE_CONNECTED 1
#define HSP_SERVICE_CONNECTED 2
*/
class BluetoolsController : 
	public xINetworkEvents,	
	public xIRemoteDeviceEvents,
	public xIStreamEvents,
	public SuperController
{
public:
	BluetoolsController();

	~BluetoolsController(void);

	bool InitBluetooth();
	// BlueTools: Network event interface
	void OnDeviceDiscoveryStarted(xNetwork* source);
	void OnDeviceDiscovered(xNetwork* source, xRemoteDevice* discoveredDevice);
	void OnDeviceLost(xNetwork* source, xRemoteDevice* lostDevice);
	void OnDeviceDiscoveryCompleted(xNetwork* source, xDeviceList* discoveredDevices);
	void OnError(xNetwork* source, int errorCode, xString* errorText);

	// BlueTools: xIRemoteDeviceEvents
	void OnServiceDiscoveryStarted(xRemoteDevice* source);
	void OnServiceDiscovered(xRemoteDevice* source, xRemoteService* discoveredService);
	void OnServiceDiscoveryCompleted(xRemoteDevice* source, xServiceList* discoveredServices);
	void OnError(xRemoteDevice* source, int errorCode, xString* errorText);
	void OnBondStarted(xRemoteDevice* source) {}
	void OnBondCompleted(xRemoteDevice* source) {}
	void OnUnbondStarted(xRemoteDevice* source) {}
	void OnUnbondCompleted(xRemoteDevice* source) {}

	// BlueTools: xIStreamEvents
	void OnReadCompleted(xStream* source, xStreamResult* results);
	void OnWriteCompleted(xStream* source, xStreamResult* results) {} 

	int GetSCNOfService(CString serviceNameToFind);
	bool ConnectToRemoteDevice(xString* deviceAddress,int SCN, xString* serviceName, xString* deviceName);
	void DisconnectRemoteDevice();
	bool BondDevice();
	void Write(unsigned char* data);
	void DiscoverDevices(bool refresh);
	bool ShowDeviceSearchDialog(); 
	void DeviceSelected(int deviceIndex);
//	void SetState(int newState);
	bool ConnectToSelectedDevice(int SCN, CString serviceName );
	bool ConnectToLastUsedDevice();
	void DiscoverServicesAsync();
	bool DiscoverServicesSync();
	int GetNoOfServicesDiscovered();
	xRemoteDevice* GetRemoteDevice();
	bool Start();
	bool IsRadioAvailable();
	bool ShouldHPQueryForPIN();
	void UnbondDevice();
	void CancelDeviceDiscovery();

	CStringList* GetServiceListAsCStringList();
	int GetMode();
	

private:
	xManager* m_manager;
	xNetwork* m_network;
	xRemoteDevice* m_device;

	xList* m_listDevices;
	xList* m_listServices;

	unsigned char m_buffer[256];

	void m_cleanup();

	void m_show_error(xObject* obj);
	int GetIndexOfConnectedService();
    xStream* curStream;

	
//	ServiceDiscoveryDialog* serviceDiscoveryDialog; //moved to SuperController
	//WizardDialog* wizardDialog;
	//SettingsControl* mySettingsControl;
	void PopulateDeviceList();
	void InitLocalSettings();
	//int state;
	//bool isReconnecting;
	static bool isDisconnecting;
	bool lastPairingFailed;  //Not so good way to handle dispose hang but it only occurs after a failed pairing
	HANDLE h_WaitForDisconnect;
	
	
};
