//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////

//BSController is intended as a Proxy class for the BlueSoleil DLL in order
//to keep the SuperController concept

#pragma once
#include "supercontroller.h"
#include "SoleilProxy.h" //.h file for the Soleil.dll

class BSController :
	public SuperController
{
public:
	BSController(void);
	~BSController(void);

	//SuperController virtuals
	bool ConnectToLastUsedDevice();
	bool ShowDeviceSearchDialog();
	void DiscoverDevices(bool refresh);
	void DeviceSelected(int deviceIndex);
	CStringList* GetServiceListAsCStringList();
	bool ConnectToSelectedDevice(int SCN, CString serviceName );
	void DiscoverServicesAsync();
	bool DiscoverServicesSync();
	int GetNoOfServicesDiscovered();
	int GetSCNOfService(CString serviceNameToFind);
	//void SetState(int newState);
	void InitLocalSettings();
	int GetMode();
	bool Start();
	void Write(unsigned char* data);
	bool IsRadioAvailable();
	bool ShouldHPQueryForPIN();
	void UnbondDevice();
	bool BondDevice();
	void CancelDeviceDiscovery();
	//End

	//Called by BlueSoleilProxy when data arrives on COM
	static int __stdcall DataCallbackFunc(char* msg, void* controller);
	static int __stdcall DeviceCallbackFunc(char* msg, void* controller);
	static int __stdcall EventCallbackFunc(char* msg, void* controller);
	void DeviceFound(char* deviceName);
	void OnReadCompleted(char* data);
private:
	//SoleilProxy* mySoleilProxy;
	CStringList* servicesListAsCStringList;
	bool isInitialised;
};
