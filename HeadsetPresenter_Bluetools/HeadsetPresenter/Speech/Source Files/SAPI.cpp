// SAPI.cpp: implementation of the SAPI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Header Files\SAPI.h"
#include "..\Header Files\gramdef.h"
#include "SS_Log.h"
#include "HeadsetPresenterDlg.h"
#include "AudioSelectDialog.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComPtr<ISpRecoContext>  SAPI::g_cpRecoCtxt = NULL;
CComPtr<ISpVoice> 		 SAPI::g_cpVoice = NULL;

bool SAPI::runInDemoMode = true;
bool SAPI::voiceFeedbackEnabled = true;
int SAPI::requiredConfidence = SP_HIGH_CONFIDENCE;
int SAPI::audioFeedbackMode = AUDIO_FEEDBACK_LAST_WORD_ONLY;
SettingsControl* SAPI::mySettingsControl = NULL;
CHeadsetPresenterDlg* SAPI::myHeadsetPresenterCallback = NULL;
//LicenseManagerProxy* SAPI::myLicManProxy;

SAPI::SAPI()
{
	voiceFeedbackEnabled = false;
	SetHomeDir();
	
	//myPpProxy.StartPowerpoint2();
	CreateLicenseManagerProxyAndCheckLicense();
	

}

SAPI::~SAPI()
{		
	//delete myLicManProxy;
}

void SAPI::SetHeadsetPresenterDlg(CHeadsetPresenterDlg* ihp)
{
	myHeadsetPresenterCallback = ihp;
}

void SAPI::SetSettingsControl(SettingsControl* iSettingsControl)
{
	mySettingsControl = iSettingsControl;

}



void SAPI::SetRequiredConfidence(int irequiredConfidence)
{
	requiredConfidence = irequiredConfidence;
	Log("SAPI:Required confidence set to %d (1=high, 0=normal)",requiredConfidence);
}

void SAPI::SetAudioFeedbackMode(int imode)
{
	audioFeedbackMode = imode;
}


void SAPI::CreateLicenseManagerProxyAndCheckLicense()
{
	//LogEnterFunction("PPP:CreateLicenseManagerProxyAndCheckLicense");
	//::CoInitialize(NULL);  //do uninit somewhere , does not work in destructor, used in other places ?
	// end init
	//myLicManProxy = new LicenseManagerProxy();
	
	/*if(!myLicManProxy->CheckLicense(L"HeadsetPresenterSpeech",L"0.9"))
	{
		runInDemoMode = true;
	}
	else
	{
		runInDemoMode = false;
	}*/
	runInDemoMode = false;
}

void SAPI::CheckNoOfRecognisers()
{
	LogEnterFunction("CTRL:CheckSpeech");
	CComPtr<ISpRecognizer> g_cpEngine;		
	HRESULT hr = S_OK;
	//hr = g_cpEngine.CoCreateInstance(CLSID_SpInprocRecognizer);
	//Log("CTRL:CoCreateInstance returned : %d",hr);	
	CComPtr<IEnumSpObjectTokens>  cpEnum;
	ULONG numTokens = 0;
	hr = SpEnumTokens(SPCAT_RECOGNIZERS,NULL,NULL,&cpEnum);
	Log("CTRL:SPEnumTokens returned: %d",hr);	
	if(SUCCEEDED(hr))
	{
		hr = cpEnum->GetCount(&numTokens);
		Log("CTRL:GetCount returned : %d",hr);	
	}
	
	if(SUCCEEDED(hr) && numTokens > 0)
	{
		Log("SAPI:No of recognizers found is : %d",numTokens);
	}
	else  //Will fall down here also if SpEnumTokens fail
    {
		Log("CTRL:No recognisers found");
    }
	if ( g_cpEngine )
	{
		g_cpEngine.Release();
	}

}
void SAPI::SetHomeDir()
{
	/*
	HMODULE phModule;
	phModule = GetModuleHandle("HeadsetPresenter.exe");  //When running stand alone
	if(phModule == NULL)
	{
		Log("CT:Failed to get instance path of HeadsetPresenter");
		MessageBox(NULL,"Failed to get instance path. Please contact support","HeadsetPresenter",MB_OK);
	}
	
	if(phModule != NULL)  //We got instance for either .dll or .exe
	{
		//Get the dir path only and set \0 after the final \ in the path to remove the filename of the dll
		DWORD dwResult = ::GetModuleFileName(phModule,fullPathToDLL, sizeof(fullPathToDLL));
		if(dwResult == 0)
		{
			MessageBox(NULL,"Failed to get application path. Please contact support","HeadsetPresenter",MB_OK);
		}
		char* t = _tcsrchr(fullPathToDLL,'\\');
		t++;
		*t = '\0';
		Log("SAPI:applicationHomeDir=%s",fullPathToDLL);
	}
	*/
	
}

void SAPI::SetSpeechFeedback()
{
	Log("SAPI:Setting voiceFeedbackEnabled");
	voiceFeedbackEnabled = true;
}
//NNNNEEEEDD TO CHECK PATH


/******************************************************************************
* InitSAPI *
*----------*
*   Description:
*       Called once to get SAPI started.
*
******************************************************************************/
HRESULT SAPI::InitSAPI( )
{
	LogEnterFunction("SAPI:InitSAPI");
	HRESULT hr;
	

	hr = S_OK;
	USES_CONVERSION;
	CComPtr<ISpAudio> cpAudio;
	
	const WCHAR* MY_APP_USER_TRAINING = L"Test";

	while ( 1 )
	{
		// create a recognition engine MUST be inproc in order for SetInput to succeed
		hr = g_cpEngine.CoCreateInstance(CLSID_SpInprocRecognizer);
		if ( FAILED( hr ) )
		{
			Log("SAPI:Failed to create recognizer");
			break;
		}
		//Must have a hWnd to display otherwise nothing will show
		//hr = g_cpEngine->DisplayUI(hWnd, MY_APP_USER_TRAINING, SPDUI_UserTraining, NULL, NULL);
		
		// create the command recognition context
		hr = g_cpEngine->CreateRecoContext( &g_cpRecoCtxt );
		//hr = -21709660; // To force an error
		//In the sequence below I try to recover from an error when setting the recoContext. Could be that something is listed but is not really there.
		if ( FAILED( hr ) )
		{
			Log("SAPI:Failed to create reco context, will try to recover with other recogniser");
			CComPtr<IEnumSpObjectTokens>  cpEnum;
			CComPtr<ISpObjectToken>  cpMyToken;
			CSpDynamicString tokenDesc;
			ULONG numTokens = 0;
			int tokenCounter =0;
			bool createRecoContext = false;

			hr = SpEnumTokens(SPCAT_RECOGNIZERS,NULL,NULL,&cpEnum);
			
			if(hr == S_OK)
			{
				hr = cpEnum->GetCount(&numTokens);
				tokenCounter = numTokens;
				Log("SAPI:No of recognizers found is : %d",numTokens);
				if(SUCCEEDED(hr) && numTokens > 0)
				{
					while((cpEnum->Next(1, &cpMyToken, NULL) == S_OK) && createRecoContext == false)
					{
						hr = SpGetDescription(cpMyToken,&tokenDesc);
						hr = g_cpEngine->SetRecognizer(cpMyToken); 
						if ( FAILED( hr ) )
						{
							Log("SAPI:Failed to set recogniser for:%s",tokenDesc.CopyToChar());
						}
						else
						{
							Log("SAPI:Successfully set recognizer for:%s",tokenDesc.CopyToChar());
							hr = g_cpEngine->CreateRecoContext( &g_cpRecoCtxt );
							if ( FAILED( hr ) )
							{
								Log("SAPI:CreateRecoContext for device:%d failed. Error:%d",tokenCounter,hr);
								tokenCounter++;
							}
							else
							{
								Log("SAPI:CreateRecoContext for device:%d succeded",tokenCounter);
								createRecoContext = true;

							}
						}
						cpMyToken = NULL;
						tokenDesc.Clear();  //Must clear string othersiwe GetDescription fails
					}
				}
			}
			if(!createRecoContext)
			{
				break;
			}
		}

		/*This code fetches the Bluetooth AudioIn settings to use with the recognizer*/

		CComPtr<IEnumSpObjectTokens>  cpEnum;
		CComPtr<ISpObjectToken>  cpMyToken;
		CSpDynamicString tokenDesc;
		ULONG numTokens = 0;
		int noOfBTAudiosFound = 0;
		AudioSelectDialog myAudioSelectDialog;
		hr = SpEnumTokens(SPCAT_AUDIOIN,NULL,NULL,&cpEnum);
		CString nameOfAudioInDevice = "";
		if(hr == S_OK)
		{
			hr = cpEnum->GetCount(&numTokens);
			if(SUCCEEDED(hr) && numTokens > 0)
			{
				//Limitation, this config and the mixer will select the first Bluetooth audio found
				//If there are several available this might turn out to be a problem
				while((cpEnum->Next(1, &cpMyToken, NULL) == S_OK) /*&& foundBTAudio == false*/)
				{
					hr = SpGetDescription(cpMyToken,&tokenDesc);
				
					if(mySettingsControl->GetAudioDevice() == "NA") //No name stored do find for Bluetooth
					{
						if(wcsstr(tokenDesc,L"Bluetooth") != NULL)
						{
							Log("SAPI:Audio device:%s",tokenDesc.CopyToChar());
							hr = g_cpEngine->SetInput(cpMyToken, TRUE);  //What happens if Bluetooth device is not the last device found, token will be cleared, is it already set?
							noOfBTAudiosFound++;
							nameOfAudioInDevice = tokenDesc.CopyToChar();
						}
					}
					else  //we have a stored name, do compare for stored name not find
					{
						if(wcscmp(tokenDesc,A2W(mySettingsControl->GetAudioDevice())) == 0)
						{
							Log("SAPI:Matched stored audio device:%s",tokenDesc.CopyToChar());
							hr = g_cpEngine->SetInput(cpMyToken, TRUE);  //What happens if Bluetooth device is not the last device found, token will be cleared, is it already set?
							noOfBTAudiosFound++;
							nameOfAudioInDevice = tokenDesc.CopyToChar();
						}
					}
					myAudioSelectDialog.AddDevice(tokenDesc.CopyToChar(), NO_TAG); //Save incase we do not find a device
					cpMyToken = NULL;
					tokenDesc.Clear();  //Must clear string othersiwe GetDescription fails
				}
			}
			cpEnum = NULL;
		}
		if(noOfBTAudiosFound != 1)//Either no or multiple devices found need user input
		{
			INT_PTR boxRes = myAudioSelectDialog.DoModal();
			if(boxRes == IDOK)
			{
				//User has selected an audio device
				nameOfAudioInDevice = myAudioSelectDialog.GetSelectedDeviceName();
			}
			else
			{
				Log("User canceled manual audio dialog, SAPI canceled");
				break;
			}
			Log("SAPI:Will try to match manually selected dev: %s",nameOfAudioInDevice);
			hr = SpEnumTokens(SPCAT_AUDIOIN,NULL,NULL,&cpEnum);
			if(hr == S_OK)
			{
				hr = cpEnum->GetCount(&numTokens);
				if(SUCCEEDED(hr) && numTokens > 0)
				{
					while(cpEnum->Next(1, &cpMyToken, NULL) == S_OK)
					{
						hr = SpGetDescription(cpMyToken,&tokenDesc);
						if(wcscmp(tokenDesc,A2W(nameOfAudioInDevice)) == 0) //Must do exact compare here 0 = exact match
						{
							Log("SAPI:Matched a manual audio device:%s",tokenDesc.CopyToChar());
							hr = g_cpEngine->SetInput(cpMyToken, TRUE);  
							nameOfAudioInDevice = tokenDesc.CopyToChar();
						}
						cpMyToken = NULL;
						tokenDesc.Clear();  //Must clear string otherwise GetDescription fails
					}
				}
			}
		}


		if(nameOfAudioInDevice == "")
		{
			MessageBox(NULL,"No Bluetooth Audio Device could be found.\n\nSpeech control cannot be used. Contact support to see if this can be resolved","No Bluetooth Audio Device found",MB_OK);
			hr = -1; //failure
			Log("SAPI:Failed to get Bluetooth audio device");
			break; //break out of while loop
		}
		else
		{
			mySettingsControl->SetAudioDevice(nameOfAudioInDevice);
			myMixerControl.SetPollInterval(1500);  //it is unlikely that this will be changed in PowerPoint, this is just a security measure, hopefully there are no leaks
			myMixerControl.SetNameOfAudioInDevice(nameOfAudioInDevice);
			myMixerControl.StartForceThread();  //We have BT audio, start the thread
		}

		// Let SR know the callback we want to send event information to, 
		hr = g_cpRecoCtxt->SetNotifyCallbackFunction(NotifyCallback, 0, 0 );
		if ( FAILED( hr ) )
		{
			Log("SAPI:Failed to set callback");
			break;
		}

		// hr = g_cpRecoCtxt->SetNotifyWindowMessage( hWnd, WM_RECOEVENT, 0, 0 );

		// Tell SR what types of events interest us.  Here we only care about command
		// recognition.
		hr = g_cpRecoCtxt->SetInterest( SPFEI(SPEI_RECOGNITION),SPFEI(SPEI_RECOGNITION) );
		if ( FAILED( hr ) )
		{
			Log("SAPI:Failed to SetInterest");
			break;
		}

		
		// Load our grammar frmo the xml file
	    
		hr = g_cpRecoCtxt->CreateGrammar(1, &g_cpCmdGrammar);
		if (FAILED(hr))
		{
			Log("SAPI:Failed to create grammar");
			break;
		}

		TCHAR fullPathToGrammar[512];
		//strcpy(fullPathToGrammar, fullPathToDLL);
		//strcat(fullPathToGrammar,"HPGrammar.xml"); //Hopefully we are in the correct folder
		//hr =g_cpCmdGrammar->LoadCmdFromFile(T2OLE("HPGrammar.xml"),SPLO_DYNAMIC);
		hr =g_cpCmdGrammar->LoadCmdFromFile(A2W(myHeadsetPresenterCallback->GetGrammarName()),SPLO_DYNAMIC);
		if ( FAILED( hr ) )
		{
			MessageBox(NULL,"Failed to load grammar, speech control will not be available","Grammar load failed",MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
			Log("SAPI:Failed to load grammar %s", myHeadsetPresenterCallback->GetGrammarName());
			break;
		}

		//Set the rule status for the different rules
		//If this fails InitSAPI might have been called twice, check that first
		hr = g_cpCmdGrammar->SetRuleIdState( VID_Trigger, SPRS_ACTIVE );
		if ( FAILED( hr ) )
		{
			Log("SAPI:Failed to SetRuleIdState");
			break;
		}
		/*hr = g_cpCmdGrammar->SetRuleIdState( VID_Command, SPRS_ACTIVE );
		if ( FAILED( hr ) )
		{
			Log("SAPI:Failed to SetRuleIdState2");
			break;
		}*/
		if(voiceFeedbackEnabled)
		{
			hr = g_cpRecoCtxt->GetVoice(&g_cpVoice);
			if ( FAILED( hr ) )
			{
				Log("SAPI:GetVoice failed");
				MessageBox(NULL,"Speech feedback is not available","HPSpeech",MB_OK);
				//break;
			}
			else
			{
				//We have Audio out for TTS, make sure that it is set to Bluetooth Audio otherwise it will only work if the audio channel is enabled when starting SAPI
				hr = SpEnumTokens(SPCAT_AUDIOOUT,NULL,NULL,&cpEnum);
			
				if(hr == S_OK)
				{
					hr = cpEnum->GetCount(&numTokens);
					if(SUCCEEDED(hr) && numTokens > 0)
					{
						while(cpEnum->Next(1, &cpMyToken, NULL) == S_OK)
						{
							hr = SpGetDescription(cpMyToken,&tokenDesc);
							if(wcsstr(tokenDesc,L"Bluetooth") != NULL)
							{
								//MessageBox(NULL,W2T(tokenDesc),"",MB_OK);
								hr = g_cpVoice->SetOutput(cpMyToken, TRUE);  //What happens if Bluetooth device is not the last device found, token will be cleared, is it already set?
								//foundBTAudio = true;
							}
							cpMyToken = NULL;
							tokenDesc.Clear();  //Must clear string othersiwe GetDescription fails
						}
					}
				}
			}
		}
		break;
	}


	// if we failed and have a partially setup SAPI, close it all down
	if ( FAILED( hr ) )
	{
		Log("SAPI:InitSAPI failed, cleaning up");
		CleanupSAPI();
	}

    return ( hr );
}


void __stdcall SAPI::NotifyCallback(WPARAM wParam, LPARAM lParam)
{
	//If Null pointer exceptions are seen here, make sure that InitSAPI has not been called twice
	CSpEvent event;  // Event helper class

    // Loop processing events while there are any in the queue
    while (event.GetFrom(g_cpRecoCtxt) == S_OK)
    {
        // Look at recognition event only
        switch (event.eEventId)
        {
            case SPEI_RECOGNITION:
                ExecuteCommand(event.RecoResult());
                break;
            case SPEI_FALSE_RECOGNITION:
                Log("SAPI:SPEI_FALSE_RECOGNITION");
				//HandleFalseReco(event.RecoResult());
                break;
        }
    }
}

void SAPI::ExecuteCommand(ISpPhrase *pPhrase)
{
	USES_CONVERSION;
	/*if(runInDemoMode)
	{
		int boxres = MessageBox(NULL,"HeadsetPresenter Speech DEMO version\n\nA voice command was recognized and will now be executed.\n\nIf you have problems with command recognition or if the wrong commands are executed you MUST perform speech recognition training.\nPlease read chapter 6 in the HeadsetPresenter for Beginners guide available in the HeadsetPresenter folder for further instructions.\n\nIf you have purchased a HeadsetPresenter Speech license and received a registration code press the YES button below\nto complete the activation of the HeadsetPresenter Speech and remove this message.\n\nNote: HeadsetPresenter Speech requires a separate license key your normal HeadsetPresenter license key\nis not valid for the Speech plug-in.\n\nDo you want to activate the HeadsetPresenter Speech with your registration code?", "Demo version information", MB_YESNO|MB_TOPMOST|MB_ICONINFORMATION);
		if(boxres == IDYES)
		{
			bool licenseOK;
			licenseOK = myLicManProxy->CreateLicFile(L"HeadsetPresenterSpeech",L"0.9");
			if(licenseOK)
			{
				runInDemoMode = false;
				
			}
			
		}
	}*/
	SPPHRASE *pElements;

    // Get the phrase elements, one of which is the rule id we specified in
    // the grammar.  Switch on it to figure out which command was recognized.
    if (SUCCEEDED(pPhrase->GetPhrase(&pElements)))
    {    
		CString t = W2A(pElements->pProperties->pszValue);
		WCHAR* pwszText;
		HRESULT hr;
		SPPHRASE* myPhrase = NULL;
		pPhrase->GetPhrase(&myPhrase);
		if(audioFeedbackMode == AUDIO_FEEDBACK_LAST_WORD_ONLY)
		{
			hr = pPhrase->GetText(myPhrase->Rule.ulCountOfElements-1, 1, FALSE, &pwszText, NULL);
		}
		else
		{
			hr = pPhrase->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &pwszText, NULL);
		}
		Log("SAPI:RECO command %s",t);
		if(hr == S_OK)
		{
			if(myPhrase->Rule.Confidence >= requiredConfidence)
			{
				myHeadsetPresenterCallback->CommandCallback(t,SAPI_COMMAND);
				if(mySettingsControl->GetSpeechSettings() & TEST_MODE_ACTIVE)
				{
					CString msg = CString("Speech Control test mode information!\n\nThe sentence ending with ") + CString(pwszText) + CString(" was recognised and the command ") + t + CString(" will be executed in the selected application");
					MessageBox(NULL,msg,"Command recognised",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
				}
				Speak(pwszText);
				Log("SAPI:confidence is <= requiredConfidence (= %d)",requiredConfidence);
			}
			else
			{
				MessageBox(NULL,"Speech test mode information!\n\nA command was recognised but confidence was to low.\nNo action will be taken by HeadsetPresenter.\n\nRecommendation:\n1. If a command was given -> Train the engine to improve performance.\n2. If no command was given a recognition was made based on other spoken words -> Train the engine and check the \"Require High Confidence\" box","Low confidence recognition",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
				Log("SAPI:Confidence to low");
			}
		}
        // Free the pElements memory which was allocated for us
        ::CoTaskMemFree(myPhrase);
		::CoTaskMemFree(pElements);
    }

}

void SAPI::CleanupSAPI()
{
	 // Release grammar, if loaded
    if ( g_cpCmdGrammar )
    {
        g_cpCmdGrammar.Release();
    }
	if(g_cpVoice)
	{
		g_cpVoice.Release();
	}
    // Release recognition context, if created
    if ( g_cpRecoCtxt )
    {
        g_cpRecoCtxt->SetNotifySink(NULL);
        g_cpRecoCtxt.Release();
    }
    // Release recognition engine instance, if created
	if ( g_cpEngine )
	{
		g_cpEngine.Release();
	}
}

void SAPI::Speak(WCHAR* toSpeak)
{
	if(voiceFeedbackEnabled)
	{
		g_cpVoice->Speak(toSpeak, 0, NULL);
	}
}
