// ManualStackDialog.cpp : implementation file
//

#include "stdafx.h"
#include "HeadsetPresenter.h"
#include "ManualStackDialog.h"
#include "..\header files\manualstackdialog.h"
#include "SS_Log.h"


// ManualStackDialog dialog

IMPLEMENT_DYNAMIC(ManualStackDialog, CDialog)
ManualStackDialog::ManualStackDialog(CWnd* pParent /*=NULL*/)
	: HPDialog(ManualStackDialog::IDD)
{
	hasUpperLine = true;
	imageToDraw = IDB_LOGO_50x50;
	staticText1 = "";
	staticText2 = "";
}

ManualStackDialog::~ManualStackDialog()
{
}

void ManualStackDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ManualStackDialog, CDialog)
	ON_BN_CLICKED(IDC_OK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_MICROSOFT, OnBnClickedRadioMicrosoft)
	ON_BN_CLICKED(IDC_RADIO_WIDCOMM, OnBnClickedRadioWidcomm)
	ON_BN_CLICKED(IDC_RADIO_BLUESOLEIL, OnBnClickedRadioBluesoleil)
	ON_BN_CLICKED(IDC_RADIO_TOSHIBA, OnBnClickedRadioToshiba)
	ON_BN_CLICKED(IDC_RADIO_OTHER, OnBnClickedRadioOther)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// ManualStackDialog message handlers
BOOL ManualStackDialog::OnInitDialog()
{
	PrintStaticHeadlines("Manual Stack selection", "Select your Bluetooth provider");
	SetDlgItemText(IDC_STATIC_TEXT1,staticText1);
	SetDlgItemText(IDC_STATIC_TEXT2,staticText2);
	return TRUE;
}

void ManualStackDialog::SetText(int textNo)
{
	switch(textNo)
	{
	case TEXT_NO_KNOWN_STACK_FOUND :
		staticText1 = CString("The HeadsetPresenter is primary designed for Widcomm and Microsoft Bluetooth drivers. You can run HeadsetPresenter in Advanced mode with other Bluetooth drivers as well but it may require some more user interaction.");
		staticText2 = CString("HeadsetPresenter could not find any Widcomm or Microsoft drivers on this computer. Below is a list of all drivers tested successfully with HeadsetPresenter. Please select your driver from the list.");
	break;
	case TEXT_MORE_THEN_ONE_STACK_FOUND :
		staticText1 = CString("HeadsetPresenter found two Bluetooth stacks installed on your computer. You must manually select which stack to use.");
		//staticText2 = CString("HeadsetPresenter could not find any Widcomm or Microsoft drivers on this computer. Below is a list of all drivers tested successfully with HeadsetPresenter. Please select your driver from the list.");
	break;
	default:
		Log("Unknown text id passed to setText");
		MessageBox("Unknown textId contact support");
	}
}

int ManualStackDialog::GetSelectedStack()
{
	return selectedStack;
}
	
void ManualStackDialog::OnBnClickedOk()
{
	OnOK();
}

void ManualStackDialog::OnBnClickedRadioMicrosoft()
{
	selectedStack = MICROSOFT;
}

void ManualStackDialog::OnBnClickedRadioWidcomm()
{
	selectedStack = WIDCOMM;	
}

void ManualStackDialog::OnBnClickedRadioBluesoleil()
{
	selectedStack = BLUESOLEIL;
}

void ManualStackDialog::OnBnClickedRadioToshiba()
{
	selectedStack = TOSHIBA;
}

void ManualStackDialog::OnBnClickedRadioOther()
{
	selectedStack = GENERIC;
}

HBRUSH ManualStackDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return HPDialog::OnCtlColor(pDC,pWnd,nCtlColor);
}

BOOL ManualStackDialog::OnEraseBkgnd(CDC* pDC)
{
	return HPDialog::OnEraseBkgnd(pDC);
}

void ManualStackDialog::OnPaint()
{
	HPDialog::OnPaint();
}
