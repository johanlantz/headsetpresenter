//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HeadsetPresenter.h"
#include "..\header files\modedialog.h"
#include "SettingsDialog.h"
#include "InfoDialog.h"

#include "SS_Log.h"


// ModeDialog dialog

IMPLEMENT_DYNAMIC(ModeDialog, CDialog)
ModeDialog::ModeDialog(CWnd* pParent /*=NULL*/)
	: HPDialog(ModeDialog::IDD)
{
	hasUpperLine = true;
	imageToDraw = IDB_LOGO_50x50;
}

ModeDialog::~ModeDialog()
{
}

void ModeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL ModeDialog::OnInitDialog()
{
	PrintStaticHeadlines("Choose HeadsetPresenter Mode");
	if(supportedModes & NORMAL_MODE_WIDCOMM || supportedModes & NORMAL_MODE_MICROSOFT || supportedModes & NORMAL_MODE_BLUESOLEIL || supportedModes & NORMAL_MODE_TOSHIBA)
	{
		Log("MD:Supporting normal mode");
		CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
	}
	else
	{
		Log("MD:Supporting Advanced mode");
		CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
		CButton* myRButton = (CButton*)GetDlgItem(IDC_RADIO1);
		myRButton->EnableWindow(false);
		SetDlgItemText(IDC_RADIO1,"Normal (Not supported by the installed Bluetooth drivers)");
	}

	if(!(supportedModes & ADVANCED_MODE))
	{
		Log("MD:Not supporting advanced mode");
		CButton* myRButton = (CButton*)GetDlgItem(IDC_RADIO2);
		myRButton->EnableWindow(false);
		SetDlgItemText(IDC_RADIO2,"Advanced (Not supported by the installed Bluetooth drivers)");
	}
	return TRUE;
}

void ModeDialog::SetSupportedModes(int iSupportedModes)
{
	supportedModes = iSupportedModes;
}


BEGIN_MESSAGE_MAP(ModeDialog, CDialog)
	ON_BN_CLICKED(IDBACK, OnBnClickedBack)
	ON_BN_CLICKED(IDNEXT, OnBnClickedNext)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDCANCEL3, OnBnClickedCancel3)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
END_MESSAGE_MAP()


// ModeDialog message handlers

int ModeDialog::GetSelectedMode()
{
	Log("MD:Returning selected mode as %d:",selectedMode);
	return selectedMode;
}

void ModeDialog::SetSettingsControl(SettingsControl* iSettingsControl)
{
	mySettingsControl = iSettingsControl;
}

void ModeDialog::OnBnClickedBack()
{
	OnCancel();
}

void ModeDialog::OnBnClickedNext()
{
	int checkedButton = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
	if(checkedButton == IDC_RADIO1)
	{
		selectedMode = supportedModes  & ~ADVANCED_MODE;  //Mask away advanced mode so we know which normal mode that is selected
		Log("MD:Selected normal mode no: %d", selectedMode);
	}
	else
	{
		selectedMode = ADVANCED_MODE;
		Log("MD:Selected advanced mode: %d",selectedMode);
	}
	OnOK();
}

HBRUSH ModeDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return HPDialog::OnCtlColor(pDC,pWnd,nCtlColor);
}

BOOL ModeDialog::OnEraseBkgnd(CDC* pDC)
{
	return HPDialog::OnEraseBkgnd(pDC);
}

void ModeDialog::OnPaint()
{
	HPDialog::OnPaint();
}

void ModeDialog::OnBnClickedCancel3()
{
	OnCancel();
}

void ModeDialog::OnBnClickedRadio2()
{
	InfoDialog myInfoDlg;
	myInfoDlg.SetSettingsControl(mySettingsControl);
	myInfoDlg.SetBoxId(INFO_DLG_ADVANCED_MODE_BOX);
	myInfoDlg.SetText("Advanced mode","Important information","Advanced mode sets high requirements on your Bluetooth dongle, software and operating system.\n\nAdvanced mode has been successfully tested with Bluetooth dongles from more then 30 vendors. However especially when using drivers from Toshiba or BlueSoleil some manual tuning might be required.\n\nHeadsetPresenter will try to do this automatically but it might be necessary to consult the documentation or support in case of problems."); 
	if(myInfoDlg.ShowBox())
	{
		myInfoDlg.DoModal();
	}
}
