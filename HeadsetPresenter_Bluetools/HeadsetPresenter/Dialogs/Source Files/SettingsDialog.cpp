// SettingsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "HeadsetPresenter.h"
#include "..\..\Speech\Header Files\MixerControl.h"
#include "InfoDialog.h"
#include "AudioSelectDialog.h"

#include "..\header files\settingsdialog.h"

#include "SS_Log.h"

// SettingsDialog dialog

IMPLEMENT_DYNAMIC(SettingsDialog, CDialog)
SettingsDialog::SettingsDialog(CWnd* pParent /*=NULL*/)
	: HPDialog(SettingsDialog::IDD)
{
	hasUpperLine = true;
	imageToDraw = IDB_LOGO_50x50;
}

SettingsDialog::~SettingsDialog()
{
	
}

void SettingsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SettingsDialog, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDAPPLY, OnBnClickedApply)
	ON_BN_CLICKED(IDDOWNLOAD, OnBnClickedDownload)
	ON_BN_CLICKED(IDTRAIN, OnBnClickedTrain)
	ON_BN_CLICKED(IDC_BTN_RESTORE, OnBnClickedBtnRestore)
	ON_BN_CLICKED(IDC_CHECK_SPEECH_ACTIVE, OnBnClickedCheckSpeechActive)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CHECK_TEST_MODE, OnBnClickedCheckTestMode)
END_MESSAGE_MAP()


// SettingsDialog message handlers
BOOL SettingsDialog::OnInitDialog()
{
	pendingRestore = false;
	PrintStaticHeadlines("Advanced mode settings");
	if(mySettingsControl->GetLastReplugMode() == REPLUG_AUTOMATIC)
	{
		CheckRadioButton(IDC_RADIO_REPLUG_1, IDC_RADIO_REPLUG_2, IDC_RADIO_REPLUG_1);
	}
	else
	{
		CheckRadioButton(IDC_RADIO_REPLUG_1, IDC_RADIO_REPLUG_2, IDC_RADIO_REPLUG_2);
	}
	if(!mySettingsControl->GetForwardOnly())
	{
		CheckRadioButton(IDC_RADIO_FB_1, IDC_RADIO_FB_2, IDC_RADIO_FB_1);
	}
	else
	{
		CheckRadioButton(IDC_RADIO_FB_1, IDC_RADIO_FB_2, IDC_RADIO_FB_2);
	}
	if(mySettingsControl->GetRestartDeviceHandling() & RESTART_DEVICE_ALWAYS)
	{
		CheckRadioButton(IDC_RADIO_RESTART_1, IDC_RADIO_RESTART_2, IDC_RADIO_RESTART_1);
	}
	else
	{
		CheckRadioButton(IDC_RADIO_RESTART_1, IDC_RADIO_RESTART_2, IDC_RADIO_RESTART_2);
	}
	CListBox* myLB = (CListBox*)GetDlgItem(IDC_KEEP_ALIVE_LB);
	myLB->AddString("0");
	myLB->AddString("100");
	myLB->AddString("500");
	myLB->AddString("1000");
	myLB->AddString("1500");
	myLB->AddString("2500");
	myLB->AddString("5000");
	myLB->AddString("10000");
	myLB->AddString("90000");
	char KeepAliveTemp[10];
	itoa(mySettingsControl->GetKeepAliveTimerVal(),KeepAliveTemp,10);  

	Log("SET: Got keepAliveTimeVal as %s",KeepAliveTemp);
	int res = myLB->SelectString(0,KeepAliveTemp);
	if(res == LB_ERR)
	{
		myLB->SelectString(0,"1500");
	}
	CheckSpeech();
	return TRUE;
}

HBRUSH SettingsDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return HPDialog::OnCtlColor(pDC,pWnd,nCtlColor);
}

BOOL SettingsDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return HPDialog::OnEraseBkgnd(pDC);
}

void SettingsDialog::OnPaint()
{
	HPDialog::OnPaint();
}

void SettingsDialog::OnBnClickedCancel()
{
	OnCancel();
}

void SettingsDialog::CheckSpeech()
{
	LogEnterFunction("CTRL:CheckSpeech");
	//::CoInitialize(NULL);    //done in app control
	CComPtr<ISpRecognizer> g_cpEngine;		
	HRESULT hr = S_OK;
	//hr = g_cpEngine.CoCreateInstance(CLSID_SpInprocRecognizer);
	//Log("CTRL:CoCreateInstance returned : %d",hr);	
	CComPtr<IEnumSpObjectTokens>  cpEnum;
	ULONG numTokens = 0;
	hr = SpEnumTokens(SPCAT_RECOGNIZERS,NULL,NULL,&cpEnum);
	Log("SET:SPEnumTokens returned: %d",hr);	
	if(SUCCEEDED(hr))
	{
		hr = cpEnum->GetCount(&numTokens);
		Log("SET:GetCount returned : %d",hr);	
	}
	
	if(SUCCEEDED(hr) && numTokens > 0)
	{
		Log("SET:No of recognizers found is : %d",numTokens);
		Log("SET:Created recognizer, hide non standard controls");
		CStatic* myStatic = (CStatic*)GetDlgItem(IDC_STATIC_SPEECH_NOT_INSTALLED);
		myStatic->ShowWindow(SW_HIDE);
		CButton* myButton = (CButton*)GetDlgItem(IDDOWNLOAD);
		myButton->ShowWindow(SW_HIDE);
		
		myButton = (CButton*)GetDlgItem(IDC_CHECK_SPEECH_ACTIVE);
		myButton->EnableWindow(TRUE);
		if(mySettingsControl->GetSpeechSettings() & SPEECH_CONTROL_ACTIVE)
		{
			Log("SET:Speech control active, set check");
			myButton->SetCheck(1);
		}
		myButton = (CButton*)GetDlgItem(IDC_CHECK_VOICE_FEEDBACK);
		myButton->EnableWindow(TRUE);
		if(mySettingsControl->GetSpeechSettings() & VOICE_FEEDBACK_ACTIVE)
		{
			myButton->SetCheck(1);
			Log("SET:Voice feedback active, set check");
		}
		myButton = (CButton*)GetDlgItem(IDC_CHECK_CONFIDENCE);
		myButton->EnableWindow(TRUE);
		if(mySettingsControl->GetSpeechSettings() & REQUIRE_HIGH_CONFIDENCE)
		{
			myButton->SetCheck(1);
			Log("SET:High confidence active, set check");
		}
		else
		{
			Log("SET:Normal confidence active, no check");
		}
		//The test mode value is not stored
		myButton = (CButton*)GetDlgItem(IDC_CHECK_TEST_MODE);
		myButton->EnableWindow(TRUE);
		if(mySettingsControl->GetSpeechSettings() & TEST_MODE_ACTIVE)
		{
			myButton->SetCheck(1);
			Log("SET:In test mode, check box");
		}
		else
		{
			Log("SET:Not in test mode, no check");
		}
	}
	else  //Will fall down here also if SpEnumTokens fail
    {
		Log("SET:Failed to create recognizer hiding standard controls");
		//Hide standard controls
		
		CButton* myChkButton = (CButton*)GetDlgItem(IDC_CHECK_SPEECH_ACTIVE);
		myChkButton->SetCheck(0);
		myChkButton->ShowWindow(SW_HIDE);
		myChkButton = (CButton*)GetDlgItem(IDC_CHECK_CONFIDENCE);
		myChkButton->SetCheck(0);
		myChkButton->ShowWindow(SW_HIDE);
		myChkButton = (CButton*)GetDlgItem(IDC_CHECK_SPEECH_ONLY);
		myChkButton->SetCheck(0);
		myChkButton->ShowWindow(SW_HIDE);
		myChkButton = (CButton*)GetDlgItem(IDC_CHECK_VOICE_FEEDBACK);
		myChkButton->SetCheck(0);
		myChkButton->ShowWindow(SW_HIDE);
		
		myChkButton = (CButton*)GetDlgItem(IDC_CHECK_TEST_MODE);
		myChkButton->SetCheck(0);
		myChkButton->ShowWindow(SW_HIDE);


		CButton* myButton = (CButton*)GetDlgItem(IDTRAIN);
		myButton->ShowWindow(SW_HIDE);
		

		
		
		//Display Install controls
    }
	if ( g_cpEngine )
	{
		g_cpEngine.Release();
	}


}
void SettingsDialog::SetSettingsControl(SettingsControl* iSettingsControl)
{
	mySettingsControl = iSettingsControl;
}

void SettingsDialog::OnBnClickedApply()
{
	if(!pendingRestore)  //if we have clicked restore, do not overwrite restored settings when clicking apply
	{
		int checkedButton = GetCheckedRadioButton(IDC_RADIO_REPLUG_1, IDC_RADIO_REPLUG_2);
		if(checkedButton == IDC_RADIO_REPLUG_1)
		{
			mySettingsControl->SetLastReplugMode(REPLUG_AUTOMATIC);
		}
		else
		{
			mySettingsControl->SetLastReplugMode(REPLUG_MANUAL);
		}
		
		checkedButton = GetCheckedRadioButton(IDC_RADIO_FB_1, IDC_RADIO_FB_2);
		if(checkedButton == IDC_RADIO_FB_1)
		{
			mySettingsControl->SetForwardOnly(false);
		}
		else
		{
			mySettingsControl->SetForwardOnly(true);
		}

		checkedButton = GetCheckedRadioButton(IDC_RADIO_RESTART_1, IDC_RADIO_RESTART_2);
		if(checkedButton == IDC_RADIO_RESTART_1)
		{
			mySettingsControl->SetRestartDeviceHandling(RESTART_DEVICE_ALWAYS);
		}
		else
		{
			mySettingsControl->SetRestartDeviceHandling(RESTART_DEVICE_NEVER);
		}
		int index;
		CString keepAliveTimerVal = "1500";
		CListBox* myLB = (CListBox*)GetDlgItem(IDC_KEEP_ALIVE_LB);
		index = myLB->GetCurSel();
		if(index != LB_ERR)
		{
			myLB->GetText(index,keepAliveTimerVal);
		}
		mySettingsControl->SetKeepAliveTimerVal(atoi(keepAliveTimerVal));

		int isChecked;
		int speechSettings = 0;
		CButton* myChkButton = (CButton*)GetDlgItem(IDC_CHECK_SPEECH_ACTIVE);
		isChecked = myChkButton->GetCheck();
		if(isChecked == 1)
		{
			speechSettings = speechSettings | SPEECH_CONTROL_ACTIVE;
			Log("SET:Speech Control actived, save");
		}
		myChkButton = (CButton*)GetDlgItem(IDC_CHECK_VOICE_FEEDBACK);
		isChecked = myChkButton->GetCheck();
		if(isChecked == 1)
		{
			speechSettings = speechSettings | VOICE_FEEDBACK_ACTIVE;
			Log("SET:Voice feedback active, save");
		}
		myChkButton = (CButton*)GetDlgItem(IDC_CHECK_CONFIDENCE);
		isChecked = myChkButton->GetCheck();
		if(isChecked == 1)
		{
			speechSettings = speechSettings | REQUIRE_HIGH_CONFIDENCE;
			Log("SET:High confidence required, save");
		}
		else
		{
			if(speechSettings & SPEECH_CONTROL_ACTIVE)
			{
				MessageBox("High Confidence is deselected.\nUnless your recognizer has been well trained this setting can increase the number of false recognitions which can cause unpredictable behaivour","Normal confidence",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
			}
		}
		myChkButton = (CButton*)GetDlgItem(IDC_CHECK_TEST_MODE);
		isChecked = myChkButton->GetCheck();
		if(isChecked == 1)
		{
			speechSettings = speechSettings | TEST_MODE_ACTIVE;
			Log("SET:TestMode active, save");
		}

		speechSettings = speechSettings | mySettingsControl->GetLogDisabled();
		
		mySettingsControl->SetSpeechSettings(speechSettings);
		Log("SET:Store SpeechSettings as %d",speechSettings);
	}
	else
	{
		pendingRestore = false;
	}
	OnOK();
}

void SettingsDialog::OnBnClickedDownload()
{
	MessageBox("You will now be directed to the HeadsetPresenter website download page for Microsoft Speech.\n\nThe Settings dialog will shut down during the installation.\n\nWhen the installation has finished, enter this dialog again and activate Speech Control","Microsoft Speech installation",MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
	getHelp(201);	
	OnCancel();
}

void SettingsDialog::getHelp(int codeID)
{
	CString helpURL = "http://www.headsetpresenter.com/content/help.asp";
	helpURL += "?";
	helpURL += "codeID=";
	char codeAsString[5];
	itoa(codeID,codeAsString,10);
	helpURL += codeAsString;
	ShellExecute(NULL, "open", helpURL.GetBuffer(10), NULL, NULL, SW_SHOWNORMAL);
}





void SettingsDialog::OnBnClickedTrain()
{
	MessageBox("Speech Recognizer training.\n\nMake sure that you have an audio connection establised to the computer before pressing OK.","Training",MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
	USES_CONVERSION;
	MixerControl myMixerControl;
	//::CoInitialize(NULL); //Done in app control, only done once per thread
	CComPtr<ISpRecognizer> g_cpEngine;	
	bool foundBTAudio;
	HRESULT hr = S_OK;
	const WCHAR* MY_APP_USER_TRAINING = L"HeadsetPresenter Speech Recognition Training";
	
	hr = g_cpEngine.CoCreateInstance(CLSID_SpInprocRecognizer);
  
	if(FAILED(hr))
    {
		MessageBox("Failed to start training application");
    }
	else
	{
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
							Log("SET:Audio device:%s",tokenDesc.CopyToChar());
							hr = g_cpEngine->SetInput(cpMyToken, TRUE);  //What happens if Bluetooth device is not the last device found, token will be cleared, is it already set?
							noOfBTAudiosFound++;
							nameOfAudioInDevice = tokenDesc.CopyToChar();
						}
					}
					else  //we have a stored name, do compare for stored name not find
					{
						if(wcsstr(tokenDesc,A2W(mySettingsControl->GetAudioDevice())) != NULL)
						{
							Log("SET:Matched stored audio device:%s",tokenDesc.CopyToChar());
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
				Log("SET:User canceled manual audio dialog, dialog canceled");
				
			}
			Log("SET:Will try to match manually selected dev: %s",nameOfAudioInDevice);
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
							Log("SET:Matched a manual audio device:%s",tokenDesc.CopyToChar());
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
			MessageBox("No Bluetooth Audio Device could be found.\n\nSpeech control cannot be used. Contact support to see if this can be resolved","No Bluetooth Audio Device found",MB_OK);
			hr = -1; //failure
			Log("SET:Failed to get Bluetooth audio device");
			
		}
		else
		{
			mySettingsControl->SetAudioDevice(nameOfAudioInDevice);
			myMixerControl.SetPollInterval(1500);  //it is unlikely that this will be changed in PowerPoint, this is just a security measure, hopefully there are no leaks
			myMixerControl.SetNameOfAudioInDevice(nameOfAudioInDevice);
			myMixerControl.StartForceThread();  //We have BT audio, start the thread
			Log("SET:Got Bluetooth audio, start force and run wizard");
			myMixerControl.StartForceThread();
			hr = g_cpEngine->DisplayUI(this->m_hWnd, MY_APP_USER_TRAINING, SPDUI_UserTraining, NULL, NULL);
		}
		//Cleanup
		if ( g_cpEngine )
		{
			g_cpEngine.Release();
		}


	}
	/*The old code

		CComPtr<IEnumSpObjectTokens>  cpEnum;
		CComPtr<ISpObjectToken>  cpMyToken;
		CSpDynamicString tokenDesc;
		ULONG numTokens = 0;
		
		hr = SpEnumTokens(SPCAT_AUDIOIN,NULL,NULL,&cpEnum);
		
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
						Log("SET:Audio device:%s",wcsstr(tokenDesc,L"Bluetooth"));
						hr = g_cpEngine->SetInput(cpMyToken, TRUE);  //What happens if Bluetooth device is not the last device found, token will be cleared, is it already set?
						foundBTAudio = true;
					}
					cpMyToken = NULL;
					tokenDesc.Clear();  //Must clear string othersiwe GetDescription fails
				}
			}
		}
		if(!foundBTAudio)
		{
			MessageBox("No Bluetooth Audio Device could be found.\n\nSpeech control cannot be used. Contact support to see if this can be resolved");
			hr = -1; //failure
			Log("SET:Failed to get Bluetooth audio device");
			
		}
		else
		{
			Log("SET:Got Bluetooth audio, start force and run wizard");
			myMixerControl.StartForceThread();
			hr = g_cpEngine->DisplayUI(this->m_hWnd, MY_APP_USER_TRAINING, SPDUI_UserTraining, NULL, NULL);
		}
		//Cleanup
		if ( g_cpEngine )
		{
			g_cpEngine.Release();
		}
	}
	*/
}


void SettingsDialog::OnBnClickedBtnRestore()
{
	mySettingsControl->SetAudioDevice("NA");
	int tempSettings = 0;
	mySettingsControl->SetSpeechSettings(tempSettings | REQUIRE_HIGH_CONFIDENCE);
	CButton* myChkButton = (CButton*)GetDlgItem(IDC_CHECK_SPEECH_ACTIVE);
	myChkButton->SetCheck(0);
	myChkButton = (CButton*)GetDlgItem(IDC_CHECK_VOICE_FEEDBACK);
	myChkButton->SetCheck(0);
	myChkButton = (CButton*)GetDlgItem(IDC_CHECK_SPEECH_3);
	myChkButton->SetCheck(0);
	myChkButton = (CButton*)GetDlgItem(REQUIRE_HIGH_CONFIDENCE);
	myChkButton->SetCheck(0);
}

void SettingsDialog::OnBnClickedCheckSpeechActive()
{
	InfoDialog myInfoDialog;
	myInfoDialog.SetText("Speech Control","Important information","Speech control is an extension to HeadsetPresenter. It is not a part of the core functionality and no or limited support is offered for Speech related issues.\n\nThe functionality is in a trial state and even though it has been thorougly tested no guarantees on functionality are given.\n\nPlease read the documentation for detailed instructions.");
	myInfoDialog.DoModal();

}

void SettingsDialog::OnBnClickedButton1()
{
	INT_PTR boxRes = MessageBox("Are you sure that you want to restore all default values.\n\nAll information including headset data etc will be lost","Restore",MB_YESNO|MB_ICONINFORMATION|MB_TOPMOST);
	if(boxRes == IDYES)
	{
		Log("SET:Restoring settings");
		mySettingsControl->RestoreDefaults();
		pendingRestore = true;
		MessageBox("Default settings has been loaded.\n\nPlease restart HeadsetPresenter immidiatly for the settings to take effect","Restore done",MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
		OnBnClickedApply();
	}
}

void SettingsDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(nFlags & MK_CONTROL)
	{
		if(mySettingsControl->GetLogDisabled())
		{
			mySettingsControl->SetLogDisabled(0);
			MessageBox("Logging is activated, restart HeadsetPresenter for the change to take effect","Logging activated",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		}
		else
		{
			mySettingsControl->SetLogDisabled(1);
			MessageBox("Logging is deactivated, restart HeadsetPresenter for the change to take effect","Logging deactivated",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		}
	}
}

void SettingsDialog::OnBnClickedCheckTestMode()
{
	CButton* myChkButton = (CButton*)GetDlgItem(IDC_CHECK_TEST_MODE);
	int isChecked = myChkButton->GetCheck();
	if(isChecked == 1)
	{
		InfoDialog myInfoDlg;
		myInfoDlg.SetText("Speech Control Test Mode", "Test Mode enabled","Speech control is now in test mode.\n\nIf not already done, establish an audio conenction to your headset in Advanced mode and try out the commands in your grammar.\n\nCommands will be sent to the selected application and there will be a message displayed after each recognition.");
		myInfoDlg.DoModal();
	}
}


