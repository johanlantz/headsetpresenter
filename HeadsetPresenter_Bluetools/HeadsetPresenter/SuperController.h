//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
#pragma once
#include "SettingsControl.h"
#include "ServiceDiscoveryDialog.h"  //There is a strange problem with this one, but including resource.h in this class solves it
#include "WizardDialog.h"
#include "DeviceDiscoveryDialog.h"
//It seems as forward declarations in the superclass must be included in the subclass, not the superclass
//CHeadsetPresenterDlg is #included in BluetoolsController.cpp not SuperController.cpp
class CHeadsetPresenterDlg;



#define SCN_NOT_FOUND -1
#define GENERIC_MODE_SERVICE_COUNT -10
#define MICROSOFT_MODE_SERVICE_COUNT -20

//States
#define STATE_NOT_CONNECTED 0
#define STATE_CONNECTED 1

//allowed modes

class SuperController 	
{
public:
	SuperController();
	~SuperController(void);
	virtual bool ConnectToLastUsedDevice() = 0;
	virtual bool ShowDeviceSearchDialog() = 0;
	virtual void DiscoverDevices(bool refresh) = 0;
	virtual void DeviceSelected(int deviceIndex) = 0;
	virtual CStringList* GetServiceListAsCStringList() = 0;

	virtual bool ConnectToSelectedDevice(int SCN, CString serviceName ) = 0;
	virtual void DiscoverServicesAsync()= 0;
	virtual bool DiscoverServicesSync()= 0;
	virtual int GetNoOfServicesDiscovered() = 0;
	virtual int GetSCNOfService(CString serviceNameToFind) = 0;
	//virtual void SetState(int newState) = 0;
	virtual void InitLocalSettings() = 0;
	virtual int GetMode() = 0;
	virtual bool Start() = 0;
	virtual void Write(unsigned char* data) = 0;
	virtual bool IsRadioAvailable() = 0;
	virtual bool ShouldHPQueryForPIN() = 0;
	virtual void UnbondDevice() = 0; //used atleast by Bluetools
	virtual bool BondDevice() = 0;
	virtual void CancelDeviceDiscovery() = 0;

	void SetAVRCPSupport(bool iAVRCPSupport);
	bool GetAVRCPSupport();
	void SetSettingsControl(SettingsControl* iSettingsControl);
	ServiceDiscoveryDialog* GetServiceDiscoveryDialog();
	SettingsControl* GetSettingsControl();
	void SetState(int newState);
	bool ShowWizardDialog();
	bool GetForcedVGSMode();
	void SetForcedVGSMode(bool newMode);
	void SetForwardOnlyMode(bool newMode);  //These functions cannot be run on a subclassed object, will cause havoc
	bool GetForwardOnly();
	void SetHeadsetPresenterDlg(CHeadsetPresenterDlg* ihp);
	void GetHelp(int helpId);
	CString GetPin();
	bool SuperShowDeviceSearchDialog(); //Called by all non MS stacks
	void HandleStatesAndForwardCommand(CString cmd, int commandType, bool ForwardCmd);
	void HandleEvent(CString Event);
	void SuperConnectionLost();  

protected:
	SettingsControl* mySettingsControl;
	ServiceDiscoveryDialog* serviceDiscoveryDialog;
	CHeadsetPresenterDlg* myHeadsetPresenterCallback;
	DeviceDiscoveryDialog* deviceDiscoveryDialog;
	WizardDialog* wizardDialog;
	CString defaultWebAddress;
	static int state;
	bool forceVGS;
	bool forwardOnly;
	int controllerMode;
	bool AVRCPSupported;
	bool isReconnecting;

		
};
