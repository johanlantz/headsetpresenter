// ToshibaInfoDialog.cpp : implementation file
//

#include "stdafx.h"
#include "HeadsetPresenter.h"
#include "ToshibaInfoDialog.h"
#include "..\header files\toshibainfodialog.h"


// ToshibaInfoDialog dialog

IMPLEMENT_DYNAMIC(ToshibaInfoDialog, CDialog)
ToshibaInfoDialog::ToshibaInfoDialog(CWnd* pParent /*=NULL*/)
	: HPDialog(ToshibaInfoDialog::IDD)
{
	hasUpperLine = true;
}

ToshibaInfoDialog::~ToshibaInfoDialog()
{
}

void ToshibaInfoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ToshibaInfoDialog, CDialog)
	ON_BN_CLICKED(IDC_OK, OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BACK, OnBnClickedBack)
	ON_BN_CLICKED(IDCANCEL2, OnBnClickedCancel2)
END_MESSAGE_MAP()


// ToshibaInfoDialog message handlers
BOOL ToshibaInfoDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	PrintStaticHeadlines("Toshiba specific info","Device disconnection might be required");
	return TRUE;
}

void ToshibaInfoDialog::OnBnClickedOk()
{
	OnOK();
}

HBRUSH ToshibaInfoDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return HPDialog::OnCtlColor(pDC,pWnd,nCtlColor);
}

BOOL ToshibaInfoDialog::OnEraseBkgnd(CDC* pDC)
{
	return HPDialog::OnEraseBkgnd(pDC);
}

void ToshibaInfoDialog::OnPaint()
{
	HPDialog::OnPaint();
}

void ToshibaInfoDialog::OnBnClickedBack()
{
	OnCancel();
}

void ToshibaInfoDialog::OnBnClickedCancel2()
{
	OnCancel();
}
