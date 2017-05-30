//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
#pragma once

enum ReplugModes
{
	REPLUG_AUTOMATIC,
	REPLUG_MANUAL
};

class SettingsControl
{
public:
	SettingsControl(void);
	~SettingsControl(void);
	void SetLastDeviceAddress(CString iAddress);
	CString GetLastDeviceAddress();
	int GetLastDeviceSCN();
	void SetLastDeviceSCN(int iSCN);
	CString GetLastDeviceName();
	void SetLastDeviceName(CString name);
	CString GetLastDeviceServiceName();
	void SetLastDeviceServiceName(CString servName);
	void SetForcedVGS(bool newMode);
	bool GetForcedVGS();
	void SetForwardOnly(bool newMode);
	bool GetForwardOnly();
	void SetLastApplicationName(CString name);
	CString GetLastApplicationName();
	void SetLastControllerMode(int controllerMode);
	int GetLastControllerMode();
	void SetLastReplugMode(int iReplugMode);
	int GetLastReplugMode();
	void SetStackName(int iStackName);
	int GetStackName();
	void SetOSId(int iOSId);
	int GetOSId();
	CString GetProductVersion();
	double GetProductVersionAsDouble();
	double GetMinAllowedProductVersionAsDouble();
	CString GetLastControllerName();
	void SetLastControllerName(CString iControllerName);
	void SetLogDisabled(int iLogDisabled);
	int GetLogDisabled();
	int GetWindowMatch();
	void SetAudioDevice(CString iAudioDevice);
	CString GetAudioDevice();
	void SetRestartDeviceHandling(int iRestartDeviceHandling);
	int GetRestartDeviceHandling();
	void SetServiceInstalled(bool iServiceInstalled);
	bool GetIsServiceInstalled();
	void SetInfoBoxCheckBoxFlag(int flag, bool set);
	int GetInfoBoxCheckBoxFlags();
	void SetSpeechSettings(int iSpeechSettings);
	int GetSpeechSettings();
	void SetKeepAliveTimerVal(int timerVal);
	int GetKeepAliveTimerVal();
	void SetLetWidcommQueryForPin(int useWPin);
	int GetLetWidcommQueryForPin();
	void SetVolSupportInNormalMode(int vSupport);
	int GetVolSupportInNormalMode();
	void SetBlockDoubleClicks(int iBlock);
	bool GetBlockDoubleClicks();


	void LoadSettings();
	void SaveSettings();

	bool HasLastDevice();

	void RestoreDefaults();

private:
	CString GetDataFromIdentifier(CString dataToGet, CString settings);
	int lastDeviceSCN;
	CString lastDeviceName;
	CString lastDeviceServiceName;
	CString lastDeviceAddress;
	CString lastUsedApplicationName;
	CString getPath();
	bool hasLastDevice;
	bool forcedVGSActive;
	bool forwardOnly;
	int lastControllerMode;
	int replugMode;
	int stackName;
	int OSId;
	CString productVersion;
	CString minAllowedProductVersion;
	CString lastControllerName;
	int speechSettings;
	int logDisabled;
	CString audioDevice;
	int restartDeviceHandling;
	bool serviceInstalled;
	int infoDialogCheckBoxFlags;
	int keepAliveTimerVal;
	int letWidcommQueryForPin;
	int volSupportInNormalMode;
	int blockDoubleClicks;
	
};
