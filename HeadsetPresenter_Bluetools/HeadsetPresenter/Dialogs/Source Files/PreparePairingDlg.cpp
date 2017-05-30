// PreparePairingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HeadsetPresenter.h"
#include "PreparePairingDlg.h"
#include "..\header files\preparepairingdlg.h"


// PreparePairingDlg dialog

IMPLEMENT_DYNAMIC(PreparePairingDlg, CDialog)
PreparePairingDlg::PreparePairingDlg(CWnd* pParent /*=NULL*/)
	: HPDialog(PreparePairingDlg::IDD)
{
	hasUpperLine = true;
	imageToDraw = IDB_NEWLOGO;
}

PreparePairingDlg::~PreparePairingDlg()
{
}

void PreparePairingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PreparePairingDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_DEV_PREP, OnBnClickedCheckDevPrep)
	ON_BN_CLICKED(IDNEXT, OnBnClickedNext)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL PreparePairingDlg::OnInitDialog()
{
	PrintStaticHeadlines("Welcome to the headset configuration wizard","");
	PrintStaticMessage(IDC_STATIC_DEV_PREP, "Before proceeding refer to the documentation of your headset and configure the headset so the computer can find it.\n\n- Turn it on\n- Make it discoverable/pairable\n\nWhen the headset is ready it usually shines with a fixed light but this can differ between models. ", STANDARD_FONT);
	return TRUE;
}

// PreparePairingDlg message handlers

void PreparePairingDlg::OnBnClickedCheckDevPrep()
{
	GetDlgItem(IDNEXT)->EnableWindow();
}

void PreparePairingDlg::OnBnClickedNext()
{
	OnOK();
}

HBRUSH PreparePairingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return HPDialog::OnCtlColor(pDC,pWnd,nCtlColor);
}

BOOL PreparePairingDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return HPDialog::OnEraseBkgnd(pDC);
}

void PreparePairingDlg::OnPaint()
{
	HPDialog::OnPaint();
}
