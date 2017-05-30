// ManualConnectDialog.cpp : implementation file
//

#include "stdafx.h"
#include "HeadsetPresenter.h"
#include "ManualConnectDialog.h"
#include "..\header files\manualconnectdialog.h"
#include "..\header files\InfoDialog.h"


// ManualConnectDialog dialog

IMPLEMENT_DYNAMIC(ManualConnectDialog, CDialog)
ManualConnectDialog::ManualConnectDialog(CWnd* pParent /*=NULL*/)
	: HPDialog(ManualConnectDialog::IDD)
{
	hasUpperLine = true;
	imageToDraw = IDB_NEWLOGO;
}

ManualConnectDialog::~ManualConnectDialog()
{
}

void ManualConnectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ManualConnectDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_NEXT, OnBnClickedNext)
	ON_STN_CLICKED(IDC_MANUAL_CON_INFO, OnStnClickedManualConInfo)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// ManualConnectDialog message handlers OnInitDialog


BOOL ManualConnectDialog::OnInitDialog()
{
	PrintStaticHeadlines("Manual connection", "You must manually connect to your headset");
	CString message = "In advanced mode you must establish a connection to the headset through the applications offered by your PC Bluetooth software. If instructions on how to connect to the PC is needed, click the help button.";
	PrintStaticMessage(IDC_MANUAL_CON_INFO, message, STANDARD_FONT);
	message = "To proceed:\nA. If your Headset supports AVRCP try pressing one of the AVRCP buttons (Play, FFWD, REW) and a connection to the PC should be established automatically.\n\nB. If AVRCP is not supported, establish an audio connection to your headset. Then press volume up or volume down to verify the connection.";
	
	message += CString("\n\nThe Next button will be activated when a connection has been detected.");
	PrintStaticMessage(IDC_MAN_CON_INFO2, message, STANDARD_FONT);

	isDialogActive = true;
	return TRUE;
}

void ManualConnectDialog::GotCommand()
{
	CButton* myButton = (CButton*)GetDlgItem(IDC_NEXT);
	myButton->EnableWindow();
}

//This is so that advanced mode should not send AT commands when the ManualConnectDlg is still open
bool ManualConnectDialog::IsDialogActive()
{
	return isDialogActive;
}

void ManualConnectDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

HBRUSH ManualConnectDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return HPDialog::OnCtlColor(pDC,pWnd,nCtlColor);
}

void ManualConnectDialog::OnPaint()
{
	HPDialog::OnPaint();
}

BOOL ManualConnectDialog::OnEraseBkgnd(CDC* pDC)
{
	return HPDialog::OnEraseBkgnd(pDC);
}

void ManualConnectDialog::OnBnClickedNext()
{
	isDialogActive = false;
	OnOK();
}

void ManualConnectDialog::OnStnClickedManualConInfo()
{
	//MessageBox("Help not available yet");
}

void ManualConnectDialog::OnBnClickedButton1()
{
	InfoDialog myInfoDialog;
	myInfoDialog.SetText("Manual Connection","Quick help","For detailed information on how to connect your headset see chapter x in the documentation.\n\n1. Pair your headset with the PC using the application offered by your PC Bluetooth software.\n\n2. If your headset is already paired to the computer you can usually connect either by:\na) Clicking once on the connect button of your headset.\nb) Selecting connect or quick connect in the interface offered by your PC Bluetooth software.\n\nIf AVRCP is supported do not click the connect button since, use the AVRCP buttons instead.");
	myInfoDialog.DoModal();
}
