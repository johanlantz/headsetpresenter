#ifdef MS_STACK
#pragma once
#include "supercontroller.h"
#include <Winsock2.h>  //This is the order it must be in for .NET 
#include <Ws2bth.h>
#include <BluetoothAPIs.h>
#include <bthsdpdef.h>


class MicrosoftController : public SuperController
{
public:
	MicrosoftController(void);
	~MicrosoftController(void);
	void RunSocketReceive();
	

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
	void InitLocalSettings();
	int GetMode();
	bool Start();
	void Write(unsigned char* data){}
	bool IsRadioAvailable();
	bool ShouldHPQueryForPIN() {return false;}
	void UnbondDevice(){}
	bool BondDevice() {return true;}
	void CancelDeviceDiscovery() {};
	
	//End
private:
	bool Connect(bool reconnect);
	BTH_ADDR RestoreHeadsetAddress();
	void StoreHeadsetAddress(BTH_ADDR headsetAddress);
	void OnReadCompleted(char* data);
	void StartReceiveThread();
	void StopThread();
	BLUETOOTH_DEVICE_INFO* myDevice;
	HANDLE hThread; 
	DWORD dwExitCode;
	DWORD dwThreadId;
	bool CMD_QUIT;
	SOCKET s;

};
#endif
