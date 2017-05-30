//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HeadsetPresenter.h"
#include "ReconnectFailedDialog.h"
#include ".\reconnectfaileddialog.h"
#include "..\header files\reconnectfaileddialog.h"

#include "SS_Log.h"
// ReconnectFailedDialog dialog

IMPLEMENT_DYNAMIC(ReconnectFailedDialog, CDialog)
ReconnectFailedDialog::ReconnectFailedDialog(CWnd* pParent /*=NULL*/)
	: HPDialog(ReconnectFailedDialog::IDD)
{
	
	hasUpperLine = true;
	buttonPressed = BUTTON_CANCEL;
}

ReconnectFailedDialog::~ReconnectFailedDialog()
{
}

void ReconnectFailedDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL ReconnectFailedDialog::OnInitDialog()
{
	PrintStaticHeadlines("Reconnection failed", "");
	if(myController->GetMode() & ADVANCED_MODE)
	{
		PrintStaticMessage(IDC_REC_FAIL_BODY, "Please check the following:\n\n1. The headset is switched on.\n2. The headset is NOT in pairable mode.\n3. The headset is connected to the Audio Gateway of the PC (the Bluetooth B should most likely green). If your Headset support AVRCP the connection to the Audio Gateway is not needed.\n4. The headset is not currently connected to a cell phone blocking it from connecting with the PC.\n\nIt might help to restart the headset (do NOT put it in pairable mode.)", STANDARD_FONT);
	}
	else if(myController->GetMode() & NORMAL_MODE_WIDCOMM)
	{
		PrintStaticMessage(IDC_REC_FAIL_BODY, "Please check the following:\n\n1. The headset is NOT in pairable mode.\n2. The headset is NOT connected to the Audio Gateway of the PC (the Bluetooth B should most likely be white, not green.\n3. In the \"Quick Connect\" menu there shall NOT be a \"V\" besides the headset you are connecting to. If so click on the headset name so that it disconnects.\n4. The headset is not currently connected to a cell phone blocking it from connecting with the PC.\n\nIt might help to restart the headset (do NOT put it in pairable mode. If nothing helps you might have to re-run the wizard.)", STANDARD_FONT);
	}
	else 
	{
		PrintStaticMessage(IDC_REC_FAIL_BODY, "Please check the following:\n\n1. The headset is switched on.\n2. The headset is NOT in pairable mode.\n3. It might help to restart the headset (do NOT put it in pairable mode. If nothing helps you might have to re-run the wizard.)", STANDARD_FONT);
	}
	return TRUE;
}

BEGIN_MESSAGE_MAP(ReconnectFailedDialog, CDialog)
	ON_BN_CLICKED(IDRETRY, OnBnClickedRetry)
	ON_BN_CLICKED(IDWIZARD, OnBnClickedWizard)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


int ReconnectFailedDialog::GetButtonPressed()
{
	return buttonPressed;
}
// ReconnectFailedDialog message handlers

void ReconnectFailedDialog::OnBnClickedRetry()
{
	if(myController->ConnectToLastUsedDevice())
	{
		buttonPressed = BUTTON_OK;
		OnOK();
	}
	else
	{
		MessageBox("Reconnect retry failed.","Reconnect Retry failed",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		Log("RFD:Reconnection Failed");
		//MessageBox("Reconnect failed, you might need to restart the Headset, do not put in pairable mode.");
	}
	// TODO: Add your control notification handler code here
}

void ReconnectFailedDialog::OnBnClickedWizard()
{
	buttonPressed = BUTTON_WIZARD;
	OnOK();
	// TODO: Add your control notification handler code here
}

void ReconnectFailedDialog::SetController(SuperController* iController)
{
	myController = iController;
}

BOOL ReconnectFailedDialog::OnEraseBkgnd(CDC* pDC)
{

	return HPDialog::OnEraseBkgnd(pDC);
}

HBRUSH ReconnectFailedDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return HPDialog::OnCtlColor(pDC,pWnd,nCtlColor);
}

void ReconnectFailedDialog::OnPaint()
{
	HPDialog::OnPaint();
}

void ReconnectFailedDialog::OnBnClickedCancel()
{
	buttonPressed = BUTTON_CANCEL;
	OnCancel();
}
