//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "HeadsetPresenter.h"
#include ".\supercontroller.h"
#include "HeadsetPresenterDlg.h"
#include "SS_Log.h"
#include "ManualPinDialog.h"
#include "PreparePairingDlg.h"
#include "InfoDialog.h"

int SuperController::state = STATE_NOT_CONNECTED;

SuperController::SuperController()
{
	Log("Making supercont");
	defaultWebAddress = "http://www.headsetpresenter.com/content/help.asp";
	state = STATE_NOT_CONNECTED;
	AVRCPSupported = false;
	wizardDialog = NULL;
	deviceDiscoveryDialog = NULL; //Since MS and Generic does not use these dlg´s they are created in the classes using them
	serviceDiscoveryDialog = NULL;
	//deviceDiscoveryDialog = new DeviceDiscoveryDialog(); //Should use the same declare in constructor?
	//serviceDiscoveryDialog = new ServiceDiscoveryDialog();
	wizardDialog = new WizardDialog();

	isReconnecting = false;
	forceVGS=false;
	
}

SuperController::~SuperController(void)
{
	delete deviceDiscoveryDialog;
	delete wizardDialog;
}


void SuperController::SetState(int newState)
{
	Log("SC: Change state, old state = %d, new state = %d", state, newState);
	state = newState;
}

void SuperController::HandleEvent(CString Event)
{
	Log("SC:Handle event %s", Event);
	if(Event.Find("InquiryDone") != -1)
	{
		deviceDiscoveryDialog->DeviceDiscoveryComplete();
	}

}

void SuperController::HandleStatesAndForwardCommand(CString cmd, int commandType, bool ForwardCmd)
{
	Log("SC:Will change state and forward command %s of type %d", cmd, commandType);
	if(wizardDialog->GetWizardState() == STATE_WIZARD_TRY_HFP || wizardDialog->GetWizardState() == STATE_WIZARD_TRY_HSP || wizardDialog->GetWizardState() == STATE_WIZARD_TRY_UNKNOWN)
	{
		wizardDialog->CommandReceived(cmd,commandType);
	}
	else if(wizardDialog->GetWizardState() == STATE_WIZARD_DONE)
	{
		//do nothing with the command if Wizard is done but not completed
		//This is just to avoid the message if pressing the button an extra time
		Log("SC:Pressed button while wizard DONE, ignoring while waiting for COMPLETED");
	}
	else if(state == STATE_CONNECTED)
	{
		myHeadsetPresenterCallback->CommandCallback(cmd, commandType);
	}
	else if(state == STATE_NOT_CONNECTED)
	{
		SetState(STATE_CONNECTED);
		//The first command from advanced mode will activate the Next button in 
		//ManualConnectionDlg, it must not be forwarded to ApplicationControl
		//All other calls currently have ForwardCmd=true
		if(ForwardCmd)
		{
			myHeadsetPresenterCallback->CommandCallback(cmd, commandType);
		}
	}
}

void SuperController::SetSettingsControl(SettingsControl* iSettingsControl)
{
	mySettingsControl = iSettingsControl;
	forwardOnly = mySettingsControl->GetForwardOnly();
	forceVGS = mySettingsControl->GetForcedVGS();
	
	InitLocalSettings(); //overrides default values
}

//Used to set lastApplicationName in WizardCompletedDlg
SettingsControl* SuperController::GetSettingsControl()
{
	return mySettingsControl;
}
ServiceDiscoveryDialog* SuperController::GetServiceDiscoveryDialog()
{
	return serviceDiscoveryDialog;
}

bool SuperController::ShowWizardDialog()
{
	bool isServiceConnected= false;
	wizardDialog->SetController(this);
	INT_PTR boxRes = wizardDialog->DoModal();
	if(boxRes == IDOK)
	{
		isServiceConnected = true;
	}
	else
	{
		isServiceConnected = false;
	}
	return isServiceConnected;
}

void SuperController::SetHeadsetPresenterDlg(CHeadsetPresenterDlg* ihp)
{
	myHeadsetPresenterCallback = ihp;
}


void SuperController::SetForwardOnlyMode(bool newMode)
{
	myHeadsetPresenterCallback->SetForwardOnlyMode(newMode);
}

bool SuperController::GetForcedVGSMode()
{
	return mySettingsControl->GetForcedVGS();
}

void SuperController::SetForcedVGSMode(bool newMode)
{
	forceVGS = newMode;
	if(!isReconnecting)
	{
		unsigned char* msg;
		msg = (unsigned char*)"\r\n+VGS=7\r\n";  //The HS is stuck at VGS=15 we must send +VGS=7 to restore
		Write(msg);
		mySettingsControl->SetForcedVGS(forceVGS); 
	}
}

bool SuperController::GetForwardOnly()
{
	return mySettingsControl->GetForwardOnly();
}

void SuperController::SetAVRCPSupport(bool iAVRCPSupport)
{
	AVRCPSupported = iAVRCPSupport;
}

bool SuperController::GetAVRCPSupport()
{
	return AVRCPSupported;
}

void SuperController::GetHelp(int helpId)
{
	CString helpURL = defaultWebAddress;
	helpURL.Append("?deviceName=");
	helpURL.Append(mySettingsControl->GetLastDeviceName());
	helpURL.Append("&codeID=");
	char codeAsString[10];
	itoa(helpId,codeAsString,10);
	helpURL.Append(codeAsString);
	Log("BTC:GetHelp executed with path=%s",helpURL);
	ShellExecute(NULL, "open", helpURL , NULL, NULL, SW_SHOWNORMAL);
}

CString SuperController::GetPin()
{
	LogEnterFunction("GetPin");
	ManualPinDialog myPinDialog;
	INT_PTR res = myPinDialog.DoModal();
	CString pin = "0000";
	if(res == IDOK)
	{
		pin = myPinDialog.GetPin();
	}
	else
	{
		MessageBox(NULL,"Cancel pin dialog","",MB_OK);
	}
	return pin;
}

//Each subclass will implement DiscoverDevices that fills the dialog with devs acc
//to their implementation
bool SuperController::SuperShowDeviceSearchDialog()
{
	//Show preparation dialog before proceeding
	PreparePairingDlg myPreparePairingDlg;
	INT_PTR res = myPreparePairingDlg.DoModal();
	if(res != IDOK)
	{
		Log("SC:User canceled PreparePairing dialog");
		return false;
	}

	bool aDeviceWasSelected = false;
	LogEnterFunction("SC:ShowDeviceSearchDialog");
	
	INT_PTR boxRes = deviceDiscoveryDialog->DoModal();
	if(boxRes == IDOK)
	{
		aDeviceWasSelected = true;  
		Log("BTC: A device was selected");
		UnbondDevice(); //used by Bluetools
		
		if(!ShouldHPQueryForPIN())
		{
			Log("SC:Bonding is handled by native GUI");
			InfoDialog myInfoDlg;
			myInfoDlg.SetSettingsControl(mySettingsControl);
			myInfoDlg.SetBoxId(INFO_DLG_PINCODE);
			myInfoDlg.SetText("Wizard help","A pin-code might be needed","HeadsetPresenter will now connect to the selected headset.\n\nA pin-code might be required to complete pairing. If a \"balloon\" or other message appears on the screen, click on this message and enter the pin-code of your headset."); 
			if(myInfoDlg.ShowBox())
			{
				myInfoDlg.DoModal();
			}
		}
		else
		{
			Log("SC:Bonding is handled by HeadsetPresenter");
			aDeviceWasSelected = BondDevice();
		}
	}
	else
	{
		Log("BTC: Device selection dialog was canceled");
	}
	return aDeviceWasSelected;
}

void SuperController::SuperConnectionLost()
{
	MessageBox(NULL,"The connection was dropped either by the Headset or the PC.\n\nIf this is a reoccuring problem, Chapter 5.3 in the HeadsetPresenter for Beginners document describes a possible solution.","Connection dropped",MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
}