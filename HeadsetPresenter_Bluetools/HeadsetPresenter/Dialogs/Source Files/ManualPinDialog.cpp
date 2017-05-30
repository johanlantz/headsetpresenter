// ManualPinDialog.cpp : implementation file
//

#include "stdafx.h"
#include "HeadsetPresenter.h"
#include "ManualPinDialog.h"
#include "..\header files\manualpindialog.h"


// ManualPinDialog dialog

IMPLEMENT_DYNAMIC(ManualPinDialog, CDialog)
ManualPinDialog::ManualPinDialog(CWnd* pParent /*=NULL*/)
	: HPDialog(ManualPinDialog::IDD)
{
	hasUpperLine = true;
	imageToDraw = IDB_NEWLOGO;
}

ManualPinDialog::~ManualPinDialog()
{
}

void ManualPinDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ManualPinDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL ManualPinDialog::OnInitDialog()
{
	PrintStaticHeadlines("Pin Code needed","Please enter the pin code for your headset");
	PrintStaticMessage(IDC_STATIC_PIN_MSG, "Please enter the pin code of your headset. Usually it is 0000 but some models (for instance Nokia) might use another code.", STANDARD_FONT);
	SetDlgItemText(IDC_EDIT_MANUAL_PIN,"0000");
	GetDlgItem(IDC_EDIT_MANUAL_PIN)->SetFocus();
	return TRUE;
}
// ManualPinDialog message handlers


void ManualPinDialog::SetEditFocus()
{
	//GetDlgItem(IDC_EDIT_MANUAL_PIN)->SetFocus();
	//GotoDlgCtrl(GetDlgItem(IDC_EDIT_MANUAL_PIN));
	
}

CString ManualPinDialog::GetPin()
{
	return pin;
}

void ManualPinDialog::OnBnClickedOk()
{
	char buf[10];
	GetDlgItemText(IDC_EDIT_MANUAL_PIN,buf,9);
	pin = CString(buf);
	OnOK();
}

HBRUSH ManualPinDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return HPDialog::OnCtlColor(pDC,pWnd,nCtlColor);
}

BOOL ManualPinDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return HPDialog::OnEraseBkgnd(pDC);
}

void ManualPinDialog::OnPaint()
{
	HPDialog::OnPaint();
}
