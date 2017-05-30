//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HeadsetPresenter.h"
#include "DeviceDiscoveryDialog.h"
#include "SuperController.h"
#include "..\header files\devicediscoverydialog.h"
#include "SS_Log.h"




// DeviceDiscoveryDialog dialog

IMPLEMENT_DYNAMIC(DeviceDiscoveryDialog, CDialog)
DeviceDiscoveryDialog::DeviceDiscoveryDialog(CWnd* pParent /*=NULL*/)
	: HPDialog(DeviceDiscoveryDialog::IDD)
{
	hasUpperLine = true;
	imageToDraw = IDB_LOGO_50x50;
}

DeviceDiscoveryDialog::~DeviceDiscoveryDialog()
{
}

void DeviceDiscoveryDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL DeviceDiscoveryDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	PrintStaticHeadlines("Device Selection", "Press Start and select your Headset from the list");
	//PrintStaticMessage(IDC_WIZARD_STEP_DEVICESEARCH, "Device Selection", BOLD_STANDARD_FONT);
	//PrintStaticMessage(IDC_STATIC_DEVICE_SUB_HEADLINE, "Select your Headset from the list", STANDARD_FONT);

	return TRUE;
}

void DeviceDiscoveryDialog::AddDeviceToList(CString deviceName)
{
	CListBox* devicesListBox;
	devicesListBox = (CListBox*)GetDlgItem(IDC_DEVICES);
	devicesListBox->AddString(deviceName);
	Log("DDG:Added device with name : %s", deviceName);
	
}

BEGIN_MESSAGE_MAP(DeviceDiscoveryDialog, CDialog)
	ON_BN_CLICKED(IDSTARTSTOP, OnBnClickedStartStop)
	ON_WM_ERASEBKGND()
	ON_LBN_SELCHANGE(IDC_DEVICES, OnLbnSelchangeDevices)
	ON_BN_CLICKED(IDC_OK, OnBnClickedOk)
//	ON_WM_CTLCOLOR()
ON_WM_CTLCOLOR()
ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
ON_BN_CLICKED(IDC_BACK, OnBnClickedBack)
ON_WM_PAINT()
ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// DeviceDiscoveryDialog message handlers


void DeviceDiscoveryDialog::OnBnClickedStartStop()
{
	LogEnterFunction("DDG:OnBnClickedStartStop");
	CListBox* devicesListBox;
	devicesListBox = (CListBox*)GetDlgItem(IDC_DEVICES);

	devicesListBox->ResetContent();
	devicesListBox->UpdateWindow();
	SetStatus(true);
	myController->DiscoverDevices(true); //false = do not clear stored devices, currently only BS
}

//When using asynch 
void DeviceDiscoveryDialog::DeviceDiscoveryComplete()
{
	SetStatus(false);
}

void DeviceDiscoveryDialog::SetStatus(bool searching)
{
	CButton* myButton;
	myButton = (CButton*)GetDlgItem(IDSTARTSTOP);
	if(searching)
	{	
		isSearching = true;  //class variable use to control if cursor should be updated in OnSetCursor
		myButton->EnableWindow(false);
		myButton->SetWindowText("Searching");
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	}
	else
	{
		isSearching = false;
		myButton->EnableWindow(true);
		myButton->SetWindowText("Start Search");
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}
}

void DeviceDiscoveryDialog::SetController(SuperController* iControl)
{
	myController = iControl;
}

BOOL DeviceDiscoveryDialog::OnEraseBkgnd(CDC* pDC)
{
	
	return HPDialog::OnEraseBkgnd(pDC);
}

void DeviceDiscoveryDialog::OnLbnSelchangeDevices()
{
	//When a device is selected we cancel the discovery
	myController->CancelDeviceDiscovery();

	CListBox* myListBox;
	myListBox = (CListBox*)GetDlgItem(IDC_DEVICES);
	myController->DeviceSelected(myListBox->GetCurSel());

	CButton* myButton;
	myButton = (CButton*)GetDlgItem(IDC_OK);
	myButton->EnableWindow(true);
}

void DeviceDiscoveryDialog::OnBnClickedOk()
{
	OnOK();
}


HBRUSH DeviceDiscoveryDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return HPDialog::OnCtlColor(pDC,pWnd,nCtlColor);
}

void DeviceDiscoveryDialog::OnBnClickedCancel()
{
	OnCancel();
}

void DeviceDiscoveryDialog::OnBnClickedBack()
{
	OnCancel();
}

void DeviceDiscoveryDialog::OnPaint()
{
	HPDialog::OnPaint();
}

BOOL DeviceDiscoveryDialog::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
	if(isSearching)
	{
		return true; //if we are searching the hour glass should be kept
	}
	return HPDialog::OnSetCursor(pWnd, nHitTest, message);
}
