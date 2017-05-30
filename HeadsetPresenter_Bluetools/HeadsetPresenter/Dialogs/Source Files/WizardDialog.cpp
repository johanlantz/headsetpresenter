//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HeadsetPresenter.h"
#include "..\header files\wizarddialog.h"
#include "SuperController.h"
#include "WizardCompletedDialog.h"

#include "SettingsControl.h"  //To access btAddress 
#include "SS_Log.h"
#include "ManualServiceDialog.h"

// WizardDialog dialog

IMPLEMENT_DYNAMIC(WizardDialog, CDialog)
WizardDialog::WizardDialog(CWnd* pParent /*=NULL*/)
	: HPDialog(WizardDialog::IDD)
{
	hasUpperLine = true;
	imageToDraw = IDB_LOGO_50x50;
	SetWizardState(STATE_WIZARD_NO_STATE);
	aVGSIsReceived = false;
	anAVRCPIsReceived = false;
	
}

WizardDialog::~WizardDialog()
{
}

void WizardDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void WizardDialog::SetWizardState(int newState)
{
	Log("WIZ:Changing Wizard State, oldState=%d, newState=%d",wizardState, newState); 
	wizardState = newState;
	if(wizardState == STATE_WIZARD_DONE) //done means that the test is over, when the wizard is exited it will change to COMPLETED
	{
		CButton* myButton = (CButton*)GetDlgItem(IDNEXT);
		myButton->EnableWindow();
		CButton* myStuckButton = (CButton*)GetDlgItem(IDC_BUTTON_STUCK);
		myStuckButton->EnableWindow(false);
		
		myButton = (CButton*)GetDlgItem(IDCANCEL);
		myButton->EnableWindow(false);
		CString message = "Wizard Completed, press Next to continue.";
		if(!aVGSIsReceived && (myController->GetMode() ^ADVANCED_MODE))
		{
			message = message + CString("\n\nNote: Your headset might support moving with the volume buttons in Advanced Mode. It is always possible to rerun the wizard and try this option at a later time if wanted.");
		}
		else if(myController->GetForwardOnly())
		{
			message = message + CString("\n\nRemember that both volume buttons will move forward, when one stops working switch to the other one and so on.");
		}
		else if(anAVRCPIsReceived)
		{
			message = message + CString("\n\nAVRCP was successfully detected, use your AVRCP buttons to control the PC");
		}
		PrintStaticMessage(IDC_WIZ_PROGRESS, message, STANDARD_FONT);
	}
}

int WizardDialog::GetWizardState()
{
	//Log("WIZ:Get wizard state returned %i",wizardState);
	return wizardState;
}


//Make sure that it is a SuperController that is sent here if it is a subclassed object there will be problems
//when calling SetForwardOnlyMode
//Do not call SetController from subclass constructor, object is not instantiated yet ?
void WizardDialog::SetController(SuperController* iController)
{
	myController = iController;
	myController->SetForwardOnlyMode(false);  //The wizard must reset forward only, it might be a new headset we are trying
}


BEGIN_MESSAGE_MAP(WizardDialog, CDialog)
	ON_BN_CLICKED(IDNEXT, OnBnClickedNext)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_STUCK, OnBnClickedButtonStuck)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// WizardDialog message handlers

BOOL WizardDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	wizardState = STATE_WIZARD_FIND_SERVICES;  //state not used
	PrintStaticHeadlines("Testing headset capabilities", "The wizard will investigate the capabilities of your headset.");
	
	PrintStaticMessage(IDC_WIZ_PROGRESS, "Connecting to Headset, please wait", STANDARD_FONT);
	
	CProgressCtrl* myPBar;
	myPBar = (CProgressCtrl*)GetDlgItem(IDC_WIZARD_PROGRESS_BAR);
	myPBar->SetRange(0,PROGRESS_RANGE);  

	lastVGS = -1;
	vgsCounter = 0;  //If we can move 16 steps we are definetely fine
	
	if(myController->DiscoverServicesSync())
	{
		ServiceDiscoveryCompleted(myController->GetNoOfServicesDiscovered());
	}
	else
	{
		PairingFailed();  //This means we do OnCancel before returning TRUE OnInitDialog? Is this ok
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void WizardDialog::ServiceDiscoveryCompleted(int serviceCount)
{
	LogEnterFunction("WZD: ServiceDiscoveryCompleted");
	//1. Check if the user wants to perform a manual service selection
	if(serviceCount > 1 && PerformManualServiceSelection())
	{
		int SCNToTry;
		INT_PTR res = myController->GetServiceDiscoveryDialog()->DoModal();
		this->ShowWindow(SW_SHOW);

		if(res == IDOK)
		{
			SCNToTry = myController->GetServiceDiscoveryDialog()->GetSCN();
			if(SCNToTry != SCN_NOT_FOUND)
			{
				Log("WIZ: Trying to connect to Headset on SCN index: %d", SCNToTry);
				SetWizardState(STATE_WIZARD_TRY_UNKNOWN);
				if(myController->ConnectToSelectedDevice(SCNToTry,myController->GetServiceDiscoveryDialog()->GetServiceName()))
				{
					CString message = "Please try and press the volume up key until the progress bar reaches 100%. \n\nIf volume up does not work, please try the connect button instead.\n\nIf nothing happens click the Nothing happens button.";
					PrintStaticMessage(IDC_WIZ_PROGRESS, message, STANDARD_FONT);
				}
				else
				{
					PairingFailed();
				}
			}
		}
	}
	
	//2. Automatic service selection
	else if(serviceCount > 0)
	{
		int SCNToTry = myController->GetSCNOfService("Headset");
		if(SCNToTry == SCN_NOT_FOUND)
		{
			SCNToTry = myController->GetSCNOfService("Free");
			if(SCNToTry == SCN_NOT_FOUND)
			{
				//MessageBox("Could not auto connect, perform manual service selection","",MB_OK);
				this->ShowWindow(SW_HIDE);
				INT_PTR res = myController->GetServiceDiscoveryDialog()->DoModal();
				this->ShowWindow(SW_SHOW);

				if(res == IDOK)
				{
					SCNToTry = myController->GetServiceDiscoveryDialog()->GetSCN();
					
					if(SCNToTry != SCN_NOT_FOUND)
					{
						Log("WIZ: Trying to connect after manual selection of SCN index: %d", SCNToTry);
						SetWizardState(STATE_WIZARD_TRY_UNKNOWN);
						if(myController->ConnectToSelectedDevice(SCNToTry,myController->GetServiceDiscoveryDialog()->GetServiceName()))
						{
							CString message = "Please try and press the volume up key until the progress bar reaches 100%. \n\nIf volume up does not work, please try the connect button several times instead.\n\nIf nothing happens click the Nothing happens button.";
							PrintStaticMessage(IDC_WIZ_PROGRESS, message, STANDARD_FONT);
						}
						else
						{
							PairingFailed();
						}
					}
				}
				else
				{
					Log("WIZ: No service selected from list, cancel pressed");
					MessageBox("No service was selected, HeadsetPresenter will not start","No service selected",MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
				}
			}
			else
			{
				Log("WIZ: Trying to connect to Hands-Free on SCN index: %d", SCNToTry);
				SetWizardState(STATE_WIZARD_TRY_HFP);
				if(myController->ConnectToSelectedDevice(SCNToTry, "Hands-Free"))
				{
					PrintStaticMessage(IDC_WIZARD_HEADLINE, "Testing headset capabilities", HEADLINE);
					CString message = "Please try and press the volume up key until the progress bar reaches 100%. \n\nIf volume up does not work, try pressing the Connect button several times instead.\n\nIf nothing happens click the \"Nothing happens\" button.";
					PrintStaticMessage(IDC_WIZ_PROGRESS, message, STANDARD_FONT);
				}
				else
				{
					PairingFailed();
				}
			}
		}
		else
		{
			Log("WIZ: Trying to connect to Headset profile on SCN index: %d", SCNToTry);
			SetWizardState(STATE_WIZARD_TRY_HSP);
			if(myController->ConnectToSelectedDevice(SCNToTry, "Headset"))
			{
				CString message = "Please try and press the volume up key until the progress bar reaches 100%. \n\nIf volume up does not work, please try the connect button instead.\n\nIf nothing happens click the \"Nothing happens\" button.";
				PrintStaticMessage(IDC_WIZ_PROGRESS, message, STANDARD_FONT);
			}
			else
			{
				PairingFailed();
			}
		}
	}
	else if(serviceCount == GENERIC_MODE_SERVICE_COUNT)  //We are in generic mode, services are unknown
	{
		Log("WIZ:Generic mode service count");
		SetWizardState(STATE_WIZARD_TRY_HSP);
		myController->ConnectToSelectedDevice(0, "Headset");  //does nothing we wait for VGS data
		CString message;
		if(myController->GetAVRCPSupport())
		{
			SetWizardState(STATE_WIZARD_TRY_AVRCP);
			message = "Try pressing the AVRCP buttons, (Play, FFWD, REW) to verify that AVRCP commands are sent. If nothing happens click the Nothing happens button.";
		}
		else
		{
			message = "If the headset and PC drivers support AVRCP, press any of the AVRCP buttons (Play, FFWD, REW).\n\nIf AVRCP is not available please try and press the volume up key until the progress bar reaches 100%. \n\nIf nothing happens or if the progress bar gets stuck click the \"Nothing happens\" button.";
		}
		PrintStaticMessage(IDC_WIZ_PROGRESS, message, STANDARD_FONT);
	}
	else if(serviceCount == MICROSOFT_MODE_SERVICE_COUNT)  //We are in generic mode, services are unknown
	{
			SetWizardState(STATE_WIZARD_TRY_HSP);
			myController->ConnectToSelectedDevice(0, "Headset");  //does nothing we wait for VGS data
			CString message;
			if(myController->GetAVRCPSupport())
			{
				SetWizardState(STATE_WIZARD_TRY_AVRCP);
				message = "Try pressing the AVRCP buttons, forward and backward to verify that AVRCP commands are sent. If nothing happens click the Nothing happens button.";
			}
			else
			{
				//Currently Microsoft will never support AVRCP perhaps later in Vista?
				//message = "If the headset and PC drivers support AVRCP, press any of the AVRCP buttons (Play, Pause, Stop, FFWD, REW).\n\nIf AVRCP is not available try and press the volume up key until the progress bar reaches 100%. \n\nIf volume up does not work try the Connect button instead.\n\nIf nothing happens or if the progress bar gets stuck click the \"Nothing happens\" button.";
				message = "Try and press the volume up key until the progress bar reaches 100%. \n\nIf volume up does not work try pressing the Connect button several times instead.\n\nIf nothing happens or if the progress bar gets stuck click the \"Nothing happens\" button.";
			}
			PrintStaticMessage(IDC_WIZ_PROGRESS, message, STANDARD_FONT);
	}
	else
	{
		//Could end up here if pairing is not completed, no services
		SetWizardState(STATE_NOT_CONNECTED);
		MessageBox("No services were found for the selected device.\n\nUnless pairing was aborted this should never happen.\n\nPlease contact support if this problem remains.","No services found",MB_OK|MB_ICONINFORMATION);
	}
}

void WizardDialog::PairingFailed()
{
	Log("WIZ: Pairing failed canceling wizard");
	OnCancel();
}
bool WizardDialog::PerformManualServiceSelection()
{
	ManualServiceDialog myManualServiceDialog;
	myManualServiceDialog.DoModal();	
	this->ShowWindow(SW_SHOW);
	return myManualServiceDialog.GetSelectManually();
}

/*It is not mandatory to send anything during init so we cannot expect that
  Headset profile ignores +VGS on CSR but it works on Nokia*/
bool WizardDialog::CommandReceived(CString Command, int commandType)
{

	Log("WIZ:Command received: %s",Command);
	CProgressCtrl* myPBar;
	myPBar = (CProgressCtrl*)GetDlgItem(IDC_WIZARD_PROGRESS_BAR);

	myController->SetState(STATE_CONNECTED);  //will always set connected not perfect, only when wizard is active though

	//static int lastVGSReceived = -1;
	
	if(commandType == AT_COMMAND)
	{
		//In HFP after CHLD the headset is connected
		if(Command.Find("CHLD") != -1)
		{
			//Atleast for Jabra (CSR?) the HS is connected now
			//MessageBox("Connected","",MB_OK);
		}
		else if(Command.Find("VGS") != -1)
		{
			//This handles the init VGS message in the wizard
			//if we have not received a VGS before the firstCKPDTime is set below
			//If we then receive a VGS within 2 sec after the CKPD it is ignored as the initVGS message
			//This function will never be entered again
			if(!aVGSIsReceived)
			{
				aVGSIsReceived = true;
				time(&firstVGSTime);
				if((firstVGSTime - firstCKPDTime) < 2)
				{
					Log("WIZ:Init VGS received, this should only happen once");
					Log("WIZ:First VGSTime was %d, first CKPD time is ",firstVGSTime, firstCKPDTime);	
					return false;
				}	
			}


			int newVGS = GetVGS(Command);
			vgsCounter++;

			if(!myController->GetForcedVGSMode()) //if advanced mode do not care about forceVGS when moving
			{
				if(lastVGS == newVGS) //VGS must be 15 twice for this to happen
				{
					Log("WIZ:lastVGS=newVGS and NOT forcedVGS, WIZARD COMPLETE");
					if(myController->GetMode() & NORMAL_MODE_MICROSOFT || myController->GetMode() & NORMAL_MODE_WIDCOMM)
					{
						myController->GetSettingsControl()->SetVolSupportInNormalMode(FULL_VOL_SUPPORT);
					}
					SetWizardState(STATE_WIZARD_DONE);
					myPBar->SetPos(PROGRESS_RANGE);
				}
				else
				{
					if(myController->GetMode() & NORMAL_MODE_MICROSOFT || myController->GetMode() & NORMAL_MODE_WIDCOMM)
					{
						myController->GetSettingsControl()->SetVolSupportInNormalMode(PARTIAL_VOL_SUPPORT);
					}
					myPBar->SetPos(newVGS*10);
					lastVGS = newVGS;
				}
			}
			else  //We have activated forcedVGS mode VGS has become 15 once and we have sent VGS=7 to the HS
			{
				Log("WIZ:Forced VGS is active");
				if(newVGS>7)
				{
					Log("WIZ:VGS>7 done");
					myPBar->SetPos(PROGRESS_RANGE);
					SetWizardState(STATE_WIZARD_DONE);
				}
			}
		}
		else if(Command.Find("CKPD") != -1)
		{
			if(!aVGSIsReceived)//No VGS has been received yet, after this CKPD an init CKPD might follow
			{
				Log("WIZ:First CKPD time set to %d",firstCKPDTime);
				time(&firstCKPDTime);
				Log("WIZ:CKPD received at %d time saved.", firstCKPDTime);
			}
			lastVGS = lastVGS + 20;  //We only  check that we can click 5 times on CKPD, if that works it is just fine
			myPBar->SetPos(lastVGS);
			if(lastVGS > 100)
			{
				Log("WIZ:CKPD clicked enough times to finish wizard");
				myPBar->SetPos(PROGRESS_RANGE);
				SetWizardState(STATE_WIZARD_DONE);
			}
		}
	}
	else  //Not AT, AVRCP
	{
		int opIdPos = Command.Find("00 48 7c");
		if(opIdPos != -1)
		{
			opIdPos += 9;
			Log("WIZ:AVRCP command recieved");
			CString opId = Command.Mid(opIdPos,2);
			Log("WIZ:Extracted op_id as :%s",opId);
			myPBar->SetPos(PROGRESS_RANGE);
			anAVRCPIsReceived = true;
			SetWizardState(STATE_WIZARD_DONE);
		}
	}
	return true;
}

int WizardDialog::GetVGS(CString ATCommand)
{
	int VGSVal = -1;
	int pos = ATCommand.Find("VGS",0);
	CString volSub = 0;
	if(ATCommand.GetAt(pos+4) == '0')
	{
		Log("WIZ:VGS has 0 in front");
		volSub = ATCommand.Mid(pos+5,1);
		VGSVal = atoi(volSub);
		Log("WIZ:VGS set to %d",VGSVal);
	}
	else
	{
		
		volSub = ATCommand.Mid(pos+4,2);
		Log("WIZ:Extracted vol as: %s", volSub);
		VGSVal = atoi(volSub);
		Log("WIZ:VGS set to %d",VGSVal);
	}
	return VGSVal;
}
void WizardDialog::OnBnClickedNext()
{
	//SetWizardState(STATE_WIZARD_COMPLETED);//now done in CompletedDlg
	WizardCompletedDialog myWizCompDlg;
	myWizCompDlg.SetMode(COMPLETED_MODE_WIZARD);
	myWizCompDlg.SetWizardDialog(this);  //for complete callback
	myWizCompDlg.SetAppName((myController->GetSettingsControl())->GetLastApplicationName());
	myWizCompDlg.DoModal();
	OnOK();
}

void WizardDialog::OnBnClickedCancel()
{
	OnCancel();
}

void WizardDialog::OnBnClickedButtonStuck()
{
	if(wizardState == STATE_WIZARD_TRY_AVRCP)
	{
		MessageBox("The wizard could not detect any AVRCP commands from the headset.\nTry opening a media player and see if it is reacting to the commands.\nIf the media player acts on the commands but the wizard is not, contact support for assistance.\n\nIf this problem persist you can try connecting to the Headset profile instead and use the volume buttons for control.","AVRCP not detected",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
	}
	if(vgsCounter > 0 ) //We are stuck on VGS 
	{
		//Forced VGS does not work or we are in ADVANCED_MODE where forced VGS will never work -> Set ForwardOnly
		if(myController->GetForcedVGSMode() || myController->GetMode() & ADVANCED_MODE)
		{
			//Forward only mode
			MessageBox("Your Headset does not support moving in both directions.\nThe Wizard has configured HeadsetPresenter to Forward Only mode.\nBoth volume buttons will now take you forward. When volume up does not work anymore, use volume down and vice versa.","Forward Only mode",MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
			myController->SetForwardOnlyMode(true);
			SetWizardState(STATE_WIZARD_DONE);
		}
		else
		{
			MessageBox("Unhandled scenario in wizard, investigate");
		}
	}
	else if(vgsCounter > 0) //We cannot repeat max VGS Try forced VGS for both HFP and HSP
	{
		Log("WIZ:User clicked stuck button, VGS15 cannot be repeated, enable forcedVGS mode");
		myController->SetForcedVGSMode(true);
		MessageBox("The Wizard has taken actions to compensate for your Headset behaivour.\nPlease close this box try and press volume up again");
		PrintStaticMessage(IDC_WIZ_PROGRESS, "The Wizard has taken actions to compensate for your Headset behaivour.\nPlease try and press volume up again\nIf it still does not work, press the Nothing happens button again", STANDARD_FONT);
	}
	/*else if(wizardState == STATE_WIZARD_TRY_HFP && vgsCounter  == 0)  //We must try Headset profile
	{
		MessageBox("The Wizard has identified that your Headset does not support the first mode.\nThe Wizard will now continue to test you Headset, please follow the instructions");
		vgsCounter = 0;
		myBluetoothController->DisconnectRemoteDevice();
		Sleep(1000);
		int SCNToTry = myBluetoothController->GetSCNOfService("Headset");
		if(SCNToTry != SCN_NOT_FOUND)
		{
			CProgressCtrl* myPBar;
			myPBar = (CProgressCtrl*)GetDlgItem(IDC_WIZARD_PROGRESS_BAR);
			myPBar->SetPos(0);
			Log("WIZ: Trying to connect to Headset on SCN index: %d", SCNToTry);
			SetWizardState(STATE_WIZARD_TRY_HSP);
			CString message = "Step 2:\nPlease press the volume up key until the progress bar reaches 100%. \n2. If the volume buttons does not work please try the Connect button.";
			PrintStaticMessage(IDC_WIZ_PROGRESS, message, STANDARD_FONT);
			myBluetoothController->ConnectToSelectedDevice(SCNToTry, "Headset");
		}
		else
		{
			Log("WIZ:Failed to find Headset profile after HFP failure in wizard");
			OnCancel();  //Manual profile selection for headset profile here, this occurs if we find Handsfree and fail and then fail to find Headset profile 
		}
	}
	*/
	else //Here we are in state_try_HSP, Headset is already configured
	{
		MessageBox("The Wizard failed to identify the capabilities of your Headset.\n\nThis does not necessarily indicate an error so the HeadsetPresenter has configured your device anyway.\n\nMost likely it will work, you might have to restart the headset.\n\nIf it does not work please contact support on support@headsetpresenter.com for further assistance.","Connection problem",MB_OK|MB_ICONINFORMATION|MB_TOPMOST);	
		CString message = "The wizard has configured your Headset at best effort. Click Next to Finish";
		PrintStaticMessage(IDC_WIZ_PROGRESS, message, STANDARD_FONT);
		CButton* myButton = (CButton*)GetDlgItem(IDNEXT);
		SetWizardState(STATE_WIZARD_DONE);
		myButton->EnableWindow();
	}
}


BOOL WizardDialog::OnEraseBkgnd(CDC* pDC)
{
	return HPDialog::OnEraseBkgnd(pDC);
}

HBRUSH WizardDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return HPDialog::OnCtlColor(pDC,pWnd,nCtlColor);
}

void WizardDialog::OnPaint()
{
	HPDialog::OnPaint();
}