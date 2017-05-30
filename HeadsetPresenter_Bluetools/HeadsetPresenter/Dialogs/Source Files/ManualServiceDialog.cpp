//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HeadsetPresenter.h"
#include "ManualServiceDialog.h"
#include "..\header files\manualservicedialog.h"
#include "SS_Log.h"


// ManualServiceDialog dialog

IMPLEMENT_DYNAMIC(ManualServiceDialog, CDialog)
ManualServiceDialog::ManualServiceDialog(CWnd* pParent /*=NULL*/)
	: HPDialog(ManualServiceDialog::IDD)
{
	selectManually = false;
	hasUpperLine = true;
	imageToDraw = IDB_LOGO_50x50;
}

ManualServiceDialog::~ManualServiceDialog()
{
}

void ManualServiceDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL ManualServiceDialog::OnInitDialog()
{
	PrintStaticHeadlines("Service Selection", "Service selection mode");
	CString message = "Your headset supports multiple Bluetooth profiles. It is recommended that you let the wizard select which service to use automatically.";
	PrintStaticMessage(IDC_MSERVICE_INFO, message,STANDARD_FONT);
	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
	return TRUE;  // return TRUE unless you set the focus to a control
}

bool ManualServiceDialog::GetSelectManually()
{
	return selectManually;
}

BEGIN_MESSAGE_MAP(ManualServiceDialog, CDialog)
	ON_BN_CLICKED(IDNEXT, OnBnClickedNext)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// ManualServiceDialog message handlers

void ManualServiceDialog::OnBnClickedNext()
{
	int checkedButton = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
	if(checkedButton == IDC_RADIO1)
	{
		Log("MSD:SelectManually = false");
		selectManually = false;
	}
	else
	{
		Log("MSD:SelectManually = true");
		selectManually = true;
	}
	OnOK();
}

HBRUSH ManualServiceDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return HPDialog::OnCtlColor(pDC,pWnd,nCtlColor);
}

BOOL ManualServiceDialog::OnEraseBkgnd(CDC* pDC)
{
	return HPDialog::OnEraseBkgnd(pDC);
}

void ManualServiceDialog::OnPaint()
{
	HPDialog::OnPaint();
}
