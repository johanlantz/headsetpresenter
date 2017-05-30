// SAPI.h: interface for the SAPI class.
//
//////////////////////////////////////////////////////////////////////
#ifndef SA
#define SA
#include <sphelper.h>                           // Contains definitions of SAPI functions
//#include "..\HS_gen\LicenseManagerProxy.h"
class CHeadsetPresenterDlg;
#include "..\Header Files\MixerControl.h"
#include "..\..\SettingsControl.h"

class SAPI  
{
public:
	SAPI();
	virtual ~SAPI();

	HRESULT InitSAPI();
	void CleanupSAPI(void);
	static void ExecuteCommand(ISpPhrase *pPhrase);
	void SetHeadsetPresenterDlg(CHeadsetPresenterDlg* ihp);
	static void __stdcall NotifyCallback(WPARAM wParam, LPARAM lParam);

	CComPtr<ISpRecoGrammar>         g_cpCmdGrammar; // Pointer to our grammar object
	static CComPtr<ISpRecoContext>  g_cpRecoCtxt;   // Pointer to our recognition context
	CComPtr<ISpRecognizer>		    g_cpEngine;		// Pointer to our recognition engine instance
	static CComPtr<ISpVoice> 		g_cpVoice;


	//void SetPowerPointProxy(ppProxy* imyPpProxy);
	//static ppProxy myPpProxy;
	static bool runInDemoMode;
//	static LicenseManagerProxy* myLicManProxy;
	static void Speak(WCHAR* toSpeak);
	void CreateLicenseManagerProxyAndCheckLicense();
	static bool voiceFeedbackEnabled;
	bool speechControlEnabled;

	TCHAR fullPathToDLL[512];
	void SetHomeDir();
	void SetSpeechFeedback();
	void CheckNoOfRecognisers();
	MixerControl myMixerControl;
	void SetRequiredConfidence(int irequiredConfidence);
	void SetAudioFeedbackMode(int imode);
	void SetSettingsControl(SettingsControl* iSettingsControl);
	
private:
	static CHeadsetPresenterDlg* myHeadsetPresenterCallback;
	static int requiredConfidence;
	static int audioFeedbackMode;
	static SettingsControl* mySettingsControl;
	CString currentAppGrammar;

};
#endif

