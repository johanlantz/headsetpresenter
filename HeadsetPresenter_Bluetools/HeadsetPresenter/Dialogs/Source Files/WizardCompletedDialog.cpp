//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
// WizardCompletedDialog.cpp : implementation file
//

#include "stdafx.h"
#include "HeadsetPresenter.h"
#include "WizardCompletedDialog.h"
#include "..\header files\wizardcompleteddialog.h"


// WizardCompletedDialog dialog

IMPLEMENT_DYNAMIC(WizardCompletedDialog, CDialog)
WizardCompletedDialog::WizardCompletedDialog(CWnd* pParent /*=NULL*/)
	: HPDialog(WizardCompletedDialog::IDD)
{
	imageToDraw = IDB_NEWLOGO;
	myWizardDialog = NULL;
}

WizardCompletedDialog::~WizardCompletedDialog()
{
}

void WizardCompletedDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(WizardCompletedDialog, CDialog)
	ON_BN_CLICKED(IDFINISH, OnBnClickedFinish)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// WizardCompletedDialog message handlers

BOOL WizardCompletedDialog::OnInitDialog()
{
	if(mode == COMPLETED_MODE_WIZARD)
	{
		PrintStaticMessage(IDC_WIZ_COMP_HEADER, "Completing the HeadsetPresenter wizard", HEADLINE);
	}
	else if(mode == COMPLETED_MODE_RECONNECT)
	{
		PrintStaticMessage(IDC_WIZ_COMP_HEADER, "Reconnection successful", HEADLINE);
	}
	PrintStaticMessage(IDC_WIZ_COMP_BODY, CString("The headset was successfully connected to your computer. Please start ") + appName + CString(" as usual and use the headset to control it."), STANDARD_FONT);

	return TRUE;
}

void WizardCompletedDialog::SetMode(int imode)
{
	mode = imode;
}

void WizardCompletedDialog::SetAppName(CString iappName)
{
	appName = iappName;
}

void WizardCompletedDialog::SetWizardDialog(WizardDialog* iWizDlg)
{
	myWizardDialog = iWizDlg;
}

void WizardCompletedDialog::OnBnClickedFinish()
{
	//Callback to WizardDlg so that button clicks does not give error msg on last screen
	if(myWizardDialog != NULL)
	{
		myWizardDialog->SetWizardState(STATE_WIZARD_COMPLETED);
		myWizardDialog = NULL; //Reset if this is reused
	}
	OnOK();
}

HBRUSH WizardCompletedDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return HPDialog::OnCtlColor(pDC,pWnd,nCtlColor);
}

BOOL WizardCompletedDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return HPDialog::OnEraseBkgnd(pDC);
}

void WizardCompletedDialog::OnPaint()
{
	HPDialog::OnPaint();
}
