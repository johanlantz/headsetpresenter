// AlreadyConnectedToAGDialog.cpp : implementation file
//

#include "stdafx.h"
#include "HeadsetPresenter.h"
#include "AlreadyConnectedToAGDialog.h"
#include "..\header files\alreadyconnectedtoagdialog.h"


// AlreadyConnectedToAGDialog dialog

IMPLEMENT_DYNAMIC(AlreadyConnectedToAGDialog, CDialog)
AlreadyConnectedToAGDialog::AlreadyConnectedToAGDialog(CWnd* pParent /*=NULL*/)
	: HPDialog(AlreadyConnectedToAGDialog::IDD)
{
	hasUpperLine = true;
}

AlreadyConnectedToAGDialog::~AlreadyConnectedToAGDialog()
{
}

void AlreadyConnectedToAGDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AlreadyConnectedToAGDialog, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_OK, OnBnClickedOk)
END_MESSAGE_MAP()

BOOL AlreadyConnectedToAGDialog::OnInitDialog()
{
	PrintStaticHeadlines("Connection failed","The headset could not be connected");
	PrintStaticMessage(IDC_STATIC_EXISTING_CONNECTION_MSG, "Most likely this is caused by the headset already having a connection to the computer OR another device for instance a cell phone.\n\nIn Windows this is indicated by the Bluetooth \"B\" beeing green instead of white. If this is the case try disconnecting the headset by right clicking the B, then select \"Quick Connect\" and Headset.\n\nIf your headset has a \"V\" next to the name, there is an active connection to the computer that blocks the HeadsetPresenter.\n\nClick on the headset name to disconnect it and rerun the wizard.", STANDARD_FONT);
	return TRUE;
}
// AlreadyConnectedToAGDialog message handlers

HBRUSH AlreadyConnectedToAGDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return HPDialog::OnCtlColor(pDC,pWnd,nCtlColor);
}

BOOL AlreadyConnectedToAGDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return HPDialog::OnEraseBkgnd(pDC);
}

void AlreadyConnectedToAGDialog::OnPaint()
{
	HPDialog::OnPaint();
}

void AlreadyConnectedToAGDialog::OnBnClickedOk()
{
	OnOK();
}
