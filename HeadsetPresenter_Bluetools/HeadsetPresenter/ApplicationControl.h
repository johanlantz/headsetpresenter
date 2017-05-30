//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
#pragma once

#define APPLICATION_NOT_FOUND -1
#include ".\LicenseManager\Header Files\LicenseManagerProxy.h"
#include "comutil.h"
#include "SettingsControl.h"


/*extern "C" 
{
	#include ".\pushkeys\Header Files\pushkeys.h"
}*/

#define NO_VALUE -100  //used when building registration comment
#define MAX_NO_OF_SPEECH_COMMANDS 4

class ApplicationControl
{
public:
	ApplicationControl(SettingsControl* iSettingsControl);
	~ApplicationControl(void);

	static void PushToWindow(HWND hWnd, LPTSTR szKeys);
	static BOOL CALLBACK ApplicationControl::ListWindows(HWND hWnd, LPARAM lparam);
	bool IncomingCommand(CString Command, int commandType);
	void SetApplicationName(CString appName);
	void SetForcedVGSMode(bool VGSMode);
	void SetForwardOnlyMode(bool newForwardOnly);
	CStringList& GetApplicationNames();
	void SetInWizardMode(bool iWizardMode);
	void SetSettingsControl(SettingsControl* iSettingsControl);
	CString GetGrammarName();
private:
	void GetMappings(CString appName);
	int GetVGS(CString ATCommand);
	CString getPath();
	CString GetDataFromIdentifier(CString dataToGet, CString settings);
	CString GetDataFromIdentifierWithOffset(CString dataToGet, CString settings, int startAt);
	void CreateLicenseManagerProxyAndCheckLicense();
	bool CheckCRCAndDemo();
	bstr_t GetRegistrationComment();
	CString GetRegistrationCommentSegment(CString key, CString valueAsString, int valueAsInt=NO_VALUE);
	void SendToWindow();
	void SetWindowMatch(CString windowMatchFromFile);
	void OpenBuyWebpage();

	CString forwardButtonVK;
	CString backwardButtonVK;
	CString connectButtonVK;
	CString playButtonVK;
	CString stopButtonVK;
	CString pauseButtonVK;
	CString ffwdButtonVK;
	CString rewButtonVK;
	CString grammarFileName;
	CStringList myAppNameList;
	CString mySpeechCommandArray[100];  //Statci limitation of 100 commands/app
	
	int demoCounter;
	bool runInDemoMode;
	LicenseManagerProxy* myLicManProxy;
	bool CRCCheckOK;
	char* applicationHomeDir;
	void SetHomeDirPath(); //solve with getPath later
	bool inWizardMode;

	
	bool forcedVGSEnabled;
	bool forwardOnly;
	int lastVGS;

	SettingsControl* mySettingsControl;

	static char szTitle[100];
	//static char szKeys[MAXLEN];
	static char szKeys[100];
	static BOOL useStar;
	//static DWORD dwSent;
	int wildcardMode;
	int windowMatch;

	time_t lastVGSTime; //Handle double click error in some headsets
	time_t thisVGSTime;

};
