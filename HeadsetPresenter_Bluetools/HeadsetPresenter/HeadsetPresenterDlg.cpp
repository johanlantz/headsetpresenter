//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HeadsetPresenter.h"
#include "HeadsetPresenterDlg.h"
#include ".\headsetpresenterdlg.h"
#include "SS_Log.h"
#include "ReconnectFailedDialog.h"
#include "ModeDialog.h"
#include "InfoDialog.h"
#include "ToshibaInfoDialog.h"
#include "sapi.h" //to access confidence defines

#include "ApplicationDialog.h"

#include "SettingsDialog.h"
#include "AVRCPDialog.h"
#include "ApplicationEditDialog.h"
#include "WizardCompletedDialog.h"




//#include "C:\Projects\SS_Log_Src\SS_RegistryKey.h"



// CHeadsetPresenterDlg dialog



//CHeadsetPresenterDlg::CHeadsetPresenterDlg(CWnd* pParent /*=NULL*/)
//	: CDialog(CHeadsetPresenterDlg::IDD, pParent)   //JCHANGE1
CHeadsetPresenterDlg::CHeadsetPresenterDlg(CWnd* pParent /*=NULL*/)
	: HPDialog(CHeadsetPresenterDlg::IDD)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_HP_ICON);
}

void CHeadsetPresenterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHeadsetPresenterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_START_WIZARD, OnBnClickedButtonStartWizard)
	ON_BN_CLICKED(IDC_BUTTON_RECONNECT, OnBnClickedButtonReconnect)
//	ON_WM_ERASEBKGND()
ON_WM_ERASEBKGND()
ON_WM_CTLCOLOR()
ON_LBN_SELCHANGE(IDC_APPNAME_LISTBOX, OnLbnSelchangeAppnameListbox)
ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
ON_BN_CLICKED(IDC_BTN_SETTINGS, OnBnClickedBtnSettings)
ON_WM_TIMER()
END_MESSAGE_MAP()


// CHeadsetPresenterDlg message handlers

BOOL CHeadsetPresenterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	//NO logging can be done before SettingsControl has been created
	//it will set the disableLogging flag 

	mySettingsControl = new SettingsControl();
	PrintStaticMessage(IDC_STATIC_WELCOME_MSG, "Welcome to the HeadsetPresenter", HEADLINE);
	PrintStaticMessage(IDC_STATIC_SELECT_APP, "Application to control:", STANDARD_FONT);
	
	imageToDraw = IDB_JABRA250;  //Goes to superclass 
	ComposeMessage();
	
	myAppControl = new ApplicationControl(mySettingsControl); 
	
	PopulateAppNameListBox();
		
	myControllerFactory = new ControllerFactory(mySettingsControl);

#ifndef _DEBUG
	InfoDialog myInfoDlg;
	myInfoDlg.SetSettingsControl(mySettingsControl);
	myInfoDlg.SetBoxId(INFO_DLG_STARTUP);
	myInfoDlg.SetText("HeadsetPresenter 3.0","","HeadsetPresenter will allow you to control most Windows applications with you Bluetooth headset.\n\nExactly which features that can be used depends on both your PC software and the capabilities of your headset.\n\nHeadsetPresenter has been successfully tested with a large number of configurations but the number of possible combinations of hardware and software is endless, so if any difficulties arise just contact support and we will try to help you out."); 
	if(myInfoDlg.ShowBox())
	{
		myInfoDlg.DoModal();
	}
#endif
	return TRUE;  // return TRUE  unless you set the focus to a control
}



void CHeadsetPresenterDlg::PopulateAppNameListBox()
{
	CListBox* myLB = (CListBox*)GetDlgItem(IDC_APPNAME_LISTBOX);
	CStringList& appNames = myAppControl->GetApplicationNames();
	if(appNames.GetCount() == 0)
	{
		MessageBox("No applications found in ButtonMappings.txt, defaulting to PowerPoint");
		myLB->AddString("PowerPoint*");
	}
	else
	{
		POSITION pos;
		for( pos = appNames.GetHeadPosition(); pos != NULL; )
		{
			myLB->AddString(appNames.GetNext(pos));
		}
	}

	//Set last used application
	//int indexOfLastApp = myLB->SelectString(0,mySettingsControl->GetLastApplicationName());
	int indexOfLastApp = myLB->FindStringExact(0,mySettingsControl->GetLastApplicationName());
	CString lastAppName;
	int n;
	if(indexOfLastApp == LB_ERR)
	{
		Log("HPD:Did not find lastUsedApp in listbox, defaulting to index 0");
		indexOfLastApp = 0; //default to first in list if there is an error finding the last used app
	}
	n = myLB->GetTextLen(indexOfLastApp);
	myLB->GetText(indexOfLastApp, lastAppName.GetBuffer(n));
	lastAppName.ReleaseBuffer();  //lastAppName still valid, buffer is released
	mySettingsControl->SetLastApplicationName(lastAppName); //This is just for security, if something is wrong the correct line will be written again
	myAppControl->SetApplicationName(lastAppName);
	myLB->SetCurSel(indexOfLastApp);	
}

void CHeadsetPresenterDlg::ComposeMessage()
{
	CString message;
	if(mySettingsControl->GetLastControllerMode() ^ADVANCED_MODE)
	{
		if(mySettingsControl->HasLastDevice())
		{
			message = message + CString("\n\nPress Reconnect to reconnect to ") + mySettingsControl->GetLastDeviceName();
		}
		else
		{
			message = message + CString("\n\nThere is no device to reconnect to, please click the button below to start the pairing wizard");
			CButton* myButton;
			myButton = (CButton*)GetDlgItem(IDC_BUTTON_RECONNECT);
			myButton->EnableWindow(false);
		}
	}
	else
	{
		message = message + CString("\n\nPress Start Wizard to configure the HeadsetPresenter or press Reconnect to skip the wizard and directly connect to your headset");
	}
	
	PrintStaticMessage(IDC_STATIC_HP_RECONNECT_INFO, message, STANDARD_FONT);
}

bool CHeadsetPresenterDlg::RunPairingGuide()
{
	bool aDeviceWasConnected = false;
	if(myController->ShowDeviceSearchDialog())
	{
		if(myController->ShowWizardDialog())
		{
			aDeviceWasConnected = true;
		}
	}
	else
	{
		Log("Device discovery was canceled, HeadsetPresenter will not be started"); 
		//What will happen here
	}
	
	return aDeviceWasConnected;
}


void CHeadsetPresenterDlg::ConnectionCompleted()
{
	//this->ShowWindow(SW_SHOWMINIMIZED);
	this->ShowWindow(SW_SHOWNORMAL);
	myAppControl->SetForcedVGSMode(myController->GetForcedVGSMode());
	myAppControl->SetForwardOnlyMode(mySettingsControl->GetForwardOnly());
	myAppControl->SetInWizardMode(false); //No longer in Wizard mode, activate limitations, this is called by both reconnect and StartWizard
	Log("HPD:Connected");
	PrintStaticMessage(IDC_STATIC_WELCOME_MSG, "HeadsetPresenter", HEADLINE);
	CString message;
	if(mySettingsControl->GetLastControllerMode() & ADVANCED_MODE)
	{
		message = "The HeadsetPresenter is active controlling \"" + mySettingsControl->GetLastApplicationName() + "\" Use the Quit button to exit the application.\nNote that in Advanced mode the headset is not disconnected by HeadsetPresenter when quitting.";
	}
	else
	{
		message = "The HeadsetPresenter is active controlling \"" + mySettingsControl->GetLastApplicationName() + "\" Please use the Quit button to disconnect the headset and exit the application";
	}
	PrintStaticMessage(IDC_STATIC_HP_RECONNECT_INFO, message, STANDARD_FONT);
	CButton* myButton = (CButton*)GetDlgItem(IDC_BUTTON_START_WIZARD);
	myButton->EnableWindow(false);
	myButton = (CButton*)GetDlgItem(IDC_BUTTON_RECONNECT);
	myButton->EnableWindow(false);
	myButton = (CButton*)GetDlgItem(IDC_BUTTON_ADD);
	myButton->EnableWindow(false);
	
	CStatic* myStatic = (CStatic*)GetDlgItem(IDC_STATIC_SELECT_APP);
	myStatic->EnableWindow(false);
	CListBox* myLB = (CListBox*)GetDlgItem(IDC_APPNAME_LISTBOX);
	myLB->EnableWindow(false);

	//Currently speech will only be activated in Advanced mode
	//This could change if SCO becomes supported in the MS stack
	if((mySettingsControl->GetSpeechSettings() & VOICE_FEEDBACK_ACTIVE) && (myController->GetMode() & ADVANCED_MODE))
	{
		Log("HPD:Activating voice feedback");
		mySAPI.SetSpeechFeedback();
	}
	if((mySettingsControl->GetSpeechSettings() & SPEECH_CONTROL_ACTIVE) && (myController->GetMode() & ADVANCED_MODE))
	{
		Log("HPD:Activating speech control");
		mySAPI.SetSettingsControl(mySettingsControl);
		mySAPI.InitSAPI();
		if(mySettingsControl->GetSpeechSettings() & REQUIRE_HIGH_CONFIDENCE)
		{
			mySAPI.SetRequiredConfidence(SP_HIGH_CONFIDENCE);
		}
		else
		{
			mySAPI.SetRequiredConfidence(SP_NORMAL_CONFIDENCE);
		}
	}
}


void CHeadsetPresenterDlg::ConnectionFailed()
{
	Log("HPD:Not Connected");
	CString message;
	message = "A pairing or connection has failed, please restart HeadsetPresenter in order to guarantee the best performance when you retry the connection";
	PrintStaticMessage(IDC_STATIC_HP_RECONNECT_INFO, message, STANDARD_FONT);
	CButton* myButton;
	myButton = (CButton*)GetDlgItem(IDC_BUTTON_RECONNECT);
	myButton->EnableWindow(false);
	myButton = (CButton*)GetDlgItem(IDC_BUTTON_START_WIZARD);
	myButton->EnableWindow(false);
	//MessageBox("Not Connected");
	this->ShowWindow(SW_NORMAL);
}

//This is currently not used by BluetoolsController since calling this failed to 
//trigger the synch event??
//2008-03 Currently not used at all, kept for reference
void CHeadsetPresenterDlg::ConnectionLost()
{
	Log("HPD:Connection dropped");
	CString message;
	message = "The connection to the headset was dropped, please restart HeadsetPresenter in order to guarantee the best performance when you retry the connection";
	PrintStaticMessage(IDC_STATIC_HP_RECONNECT_INFO, message, STANDARD_FONT);
	CButton* myButton;
	myButton = (CButton*)GetDlgItem(IDC_BUTTON_RECONNECT);
	myButton->EnableWindow(false);
	myButton = (CButton*)GetDlgItem(IDC_BUTTON_START_WIZARD);
	myButton->EnableWindow(false);
}

void CHeadsetPresenterDlg::SetForwardOnlyMode(bool newMode)
{
	mySettingsControl->SetForwardOnly(newMode); //change here
	myAppControl->SetForwardOnlyMode(newMode);
}

void CHeadsetPresenterDlg::CommandCallback(CString command, int dataType)
{
	myAppControl->IncomingCommand(command, dataType);
}

void CHeadsetPresenterDlg::OnBnClickedButtonStartWizard()
{
	//this->ShowWindow(SW_HIDE); //SW_HIDE removed the control bar icon use either EnableWindow(false) or SHOW_MINIMIZED
	this->ShowWindow(SW_SHOWMINIMIZED);
	//this->EnableWindow(false); //seems not to be needed
	ShowStackSpecificInfo();
	ModeDialog myModeDialog;  //mode dialog must have param on allowed modes
	myModeDialog.SetSettingsControl(mySettingsControl);
	myModeDialog.SetSupportedModes(myControllerFactory->GetSupportedModes());
	INT_PTR boxRes = myModeDialog.DoModal();
	if(boxRes == IDOK)
	{
		CreateController(myModeDialog.GetSelectedMode());
		if(RunPairingGuide())
		{
			ConnectionCompleted();
		}
		else
		{
			ConnectionFailed();
		}
	}
	else
	{
		this->ShowWindow(SW_NORMAL);
	}
}

void CHeadsetPresenterDlg::OnBnClickedButtonReconnect()
{
	ShowStackSpecificInfo();
	CreateController(mySettingsControl->GetLastControllerMode());
	if(!myController->ConnectToLastUsedDevice())
	{
		ReconnectFailedDialog myRecFailDlg;
		myRecFailDlg.SetController(myController);
		myRecFailDlg.DoModal();
		int res = myRecFailDlg.GetButtonPressed();
		if(res == BUTTON_WIZARD)  //Reconnect failed in reconnect dialog, user has choosen to start pairing dialog
		{
			if(RunPairingGuide())
			{
				ConnectionCompleted();
			}
			else
			{
				ConnectionFailed();
			}
		}
		else if(res == BUTTON_OK)
		{
			ConnectionCompleted(); //Reconnect succeeded throug ReconnectDialog
		}
		else
		{
			//BUTTON_CANCEL pressed, no action, just display main screen
		}
	}
	else
	{
		WizardCompletedDialog myCompletedDlg;
		myCompletedDlg.SetMode(COMPLETED_MODE_RECONNECT); //Reusing the completed dialog to display done message for reconnect
		myCompletedDlg.SetAppName(mySettingsControl->GetLastApplicationName());
		myCompletedDlg.DoModal();
		ConnectionCompleted();
	}
}

/*Creates the class global myController based on either :
1. mode selected in ModeDialog called by Start Wizard button
2. mode loaded from settings called by Reconnect button */
void CHeadsetPresenterDlg::CreateController(int controllerMode)
{
	Log("HPD:Creating controller in mode : %d", controllerMode);
	mySettingsControl->SetLastControllerMode(controllerMode); //Only relavant after running the wizard but kept here anyway, does it one extra time for reconnect but that does not matter
	if(controllerMode == ERROR_MODE)
	{
		MessageBox("An unexpected configuration is encountered, mode will be set to Advanced.\nYou might need to re-run the Wizard if the problem persists","",MB_OK);
		controllerMode = ADVANCED_MODE;
	}
	myController = myControllerFactory->GetController(controllerMode);
	myController->SetHeadsetPresenterDlg(this);
	myController->SetSettingsControl(mySettingsControl);
	myController->Start();
	
	
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHeadsetPresenterDlg::OnPaint() 
{
	//Be aware that stuff in OnPaint might overpaint stuff done in OnEraseBkgnd
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

		/*CRect rect;
		GetClientRect(&rect);
		int width = rect.Width();
		int height = rect.Height();
		
		CPaintDC dc(this);

		dc.MoveTo(0,height-50);
		dc.LineTo(width,height-50);

	 
	   
         CBitmap bmp, *poldbmp; 
         CDC memdc; 


         // Load the bitmap resource 
         bmp.LoadBitmap( IDB_INVISIO ); 


         // Create a compatible memory DC 
         memdc.CreateCompatibleDC( &dc ); 


         // Select the bitmap into the DC 
         poldbmp = memdc.SelectObject( &bmp ); 


         // Copy (BitBlt) bitmap from memory DC to screen DC 
         int res = dc.BitBlt( 1, 1, 212, 162, &memdc, 0, 0, SRCCOPY ); 


         memdc.SelectObject( poldbmp ); 
*/
		HPDialog::OnPaint();  //JCHANGE1  
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHeadsetPresenterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHeadsetPresenterDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	
	OnOK();
}

void CHeadsetPresenterDlg::OnFinalRelease()
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::OnFinalRelease();
}

void CHeadsetPresenterDlg::OnCancel()
{
	//delete myPPProxy;
	
	MessageBox("OnCancel should never be routed here");
	/*delete myController;
	delete mySettingsControl;
	delete myAppControl;
	// TODO: Add your specialized code here and/or call the base class
	CDialog::OnCancel();*/
}


BOOL CHeadsetPresenterDlg::OnEraseBkgnd(CDC* pDC)
{
	//JCHANGE1 This has gone into HPDialog

	//pDC->SetBkColor(RGB(255, 255, 255));
	// Set brush to desired background color
 /*   CBrush backBrush(RGB(255, 255, 255));

    // Save old brush
    CBrush* pOldBrush = pDC->SelectObject(&backBrush);

    CRect rect;
    pDC->GetClipBox(&rect);     // Erase the area needed

    pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(),PATCOPY);
    pDC->SelectObject(pOldBrush);

*/
	//CRect rect;
/*	GetClientRect(&rect);
	int width = rect.Width();
	int height = rect.Height();
	
	CPaintDC dc(this);

	dc.MoveTo(0,height-50);
	dc.LineTo(width,height-50);

		
		
	CBitmap bmp, *poldbmp; 
	CDC memdc; 


	// Load the bitmap resource 
	bmp.LoadBitmap( IDB_INVISIO ); 


	// Create a compatible memory DC 
	memdc.CreateCompatibleDC( &dc ); 


	// Select the bitmap into the DC 
	poldbmp = memdc.SelectObject( &bmp ); 


	// Copy (BitBlt) bitmap from memory DC to screen DC 
	int res = dc.BitBlt( 1, 1, 212, 162, &memdc, 0, 0, SRCCOPY ); 


	memdc.SelectObject( poldbmp ); 

    //return TRUE;
*/
	return HPDialog::OnEraseBkgnd(pDC); //Do not return this, it will not work then
	
}

HBRUSH CHeadsetPresenterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	/*if(nCtlColor == CTLCOLOR_STATIC)
	{
		hbr = (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
		pDC->SetBkMode(TRANSPARENT);
	}*/
	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	//return hbr;
	return HPDialog::OnCtlColor(pDC,pWnd,nCtlColor);
}

void CHeadsetPresenterDlg::OnLbnSelchangeAppnameListbox()
{
	CString selectedApp;
	int index;
	CListBox* myLB = (CListBox*)GetDlgItem(IDC_APPNAME_LISTBOX);
	index = myLB->GetCurSel();
	if(index != LB_ERR)
	{
		myLB->GetText(index,selectedApp);
	}
	else
	{
		selectedApp = "PowerPoint";
		Log("HPD: ListBox->GetCurSel returned an error, defaulting to PowerPoint");
	}
	Log("HPD:Selected Application : %s", selectedApp);
	mySettingsControl->SetLastApplicationName(selectedApp);
	myAppControl->SetApplicationName(selectedApp);

}

void CHeadsetPresenterDlg::OnBnClickedButtonAdd()
{
	ApplicationEditDialog myAppEdit;
	myAppEdit.DoModal();	
}

void CHeadsetPresenterDlg::OnBnClickedCancel()
{
	
	delete myControllerFactory;  //also deletes controller
	delete myAppControl;
	delete mySettingsControl;

	// TODO: Add your specialized code here and/or call the base class
	CDialog::OnCancel();
}

void CHeadsetPresenterDlg::OnBnClickedBtnSettings()
{
	SettingsDialog mySettingsDialog;
	mySettingsDialog.SetSettingsControl(mySettingsControl);
	mySettingsDialog.DoModal();
}

void CHeadsetPresenterDlg::ShowStackSpecificInfo()
{
	switch(mySettingsControl->GetStackName())
	{
		case TOSHIBA :
			ToshibaInfoDialog myToshibaDialog;
			myToshibaDialog.DoModal();
		break;
	}
}
//Called by SAPI to get the grammar set in ApplicationControl
CString CHeadsetPresenterDlg::GetGrammarName()
{
	return myAppControl->GetGrammarName();	
}

void CHeadsetPresenterDlg::StartTimer()
{
	Log("Keep alive timer started by Controller");
	//in BSController default value is 1000ms if needed implement dynamic value for BSController
	int timerInterval = mySettingsControl->GetKeepAliveTimerVal();
	if(timerInterval != DONT_USE_KEEPALIVE_TIMER)
	{
		Log("HPD:Keep alive timer period set to %d msec", timerInterval);
		myTimer = SetTimer(1,timerInterval,0);  //Fast in the beginning
	}
	else
	{
		Log("HPD:Dont use keep alive timer");
	}
	
}

void CHeadsetPresenterDlg::StopTimer()
{
	Log("Keep alive timer stopped by Controller");
	KillTimer(myTimer);
}

void CHeadsetPresenterDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	//MessageBox("T","",MB_OK);
	/*static counter = 0;
	if(counter < 5)
	{
		counter++;
	}
	else if(counter == 5)
	{
		Log("Timer period should change now");
		myTimer = SetTimer(1,2500,0);
		counter++;
	}*/

	unsigned char* msg;
	msg = (unsigned char*)"\r\nAT\r\n";  
	myController->Write(msg);
	HPDialog::OnTimer(nIDEvent);
}

int CHeadsetPresenterDlg::GetSupportedModes()
{
	return myControllerFactory->GetSupportedModes();
}