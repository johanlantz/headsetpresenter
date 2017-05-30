//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "HeadsetPresenter.h"
#include "..\header files\servicediscoverydialog.h"
#include "SuperController.h"
#include "SS_Log.h"



// ServiceDiscoveryDialog dialog

IMPLEMENT_DYNAMIC(ServiceDiscoveryDialog, CDialog)
ServiceDiscoveryDialog::ServiceDiscoveryDialog(CWnd* pParent /*=NULL*/)
	: HPDialog(ServiceDiscoveryDialog::IDD)
{
	hasUpperLine = true;
	imageToDraw = IDB_LOGO_50x50;
	selectedSCN = SCN_NOT_FOUND;
	selectedServiceName = "NA";
}

ServiceDiscoveryDialog::~ServiceDiscoveryDialog()
{
}

void ServiceDiscoveryDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void ServiceDiscoveryDialog::SetController(SuperController* iControl)
{
	myController = iControl;
}

int ServiceDiscoveryDialog::GetSCN()
{
	return selectedSCN;
}

CString ServiceDiscoveryDialog::GetServiceName()
{
	return selectedServiceName;
}

void ServiceDiscoveryDialog::AddServicesToList()
{
	
	CListBox* myListBox;
	myListBox = (CListBox*)GetDlgItem(IDC_SERVICES_LIST);
	
	CStringList* discoveredServices = myController->GetServiceListAsCStringList();
	//discoveredServices should probaly be deleted after use, allocated by the controllers
	POSITION cur,next;
	cur = discoveredServices->GetHeadPosition();
	while(cur != NULL)
	{
		next = cur;
		myListBox->AddString(discoveredServices->GetNext(next));
		cur = next;
	}
	
	if(discoveredServices->GetCount() < 1)
	{
		Log("SDD: No services found?");
		MessageBox("No Services found, contact support","",MB_OK);
	}
	else if(discoveredServices->GetCount() > 1)
	{
		CheckForKnownSpecialCases();  //Nokia for instance
	}
}

/*This function checks if there is any special information that can be displayed
  Best example is that Nokia "sometimes?" shows same name for both services*/
void ServiceDiscoveryDialog::CheckForKnownSpecialCases()
{
	//Check Nokia case with services having same name
	CListBox* myListBox;
	myListBox = (CListBox*)GetDlgItem(IDC_SERVICES_LIST);
	
	CString service1, service2; 
	CString serviceInfo;
	myListBox->GetText(0,service1.GetBufferSetLength(myListBox->GetTextLen(0)));
	myListBox->GetText(1,service2.GetBufferSetLength(myListBox->GetTextLen(1)));
	
	if(service1.Compare(service2) == 0) //Same service name is displayed for both services
	{
		Log("SDD: Same service name for both services");
		if(service1.Find("Nokia") != -1)
		{
			 serviceInfo = "Your Headset does not present its services correcty. This is a known issue for some Nokia models and it means that HeadsetPresenter could not automatically configure your Headset. You must manually select one of the services above. Most likely you should try no.2 in the list but if you encounter difficulties you can always rerun the wizard and choose the first one";
		}
		else
		{
			serviceInfo = "Your Headset does not present its services correcty. This is a known issue for some headset models and it means that HeadsetPresenter could not automatically configure your Headset. You must manually select one of the services above. Most likely you should try no.1 in the list but if you encounter difficulties you can always rerun the wizard and choose the first one";
		}
		SetDlgItemText(IDC_STATIC_SERVICE_INFO, serviceInfo);
	}
}

BEGIN_MESSAGE_MAP(ServiceDiscoveryDialog, CDialog)
	ON_LBN_SELCHANGE(IDC_SERVICES_LIST, OnLbnSelchangeServicesList)
	ON_BN_CLICKED(IDNEXT, OnBnClickedNext)
//	ON_WM_FONTCHANGE()
ON_WM_ERASEBKGND()
ON_WM_CTLCOLOR()
ON_WM_PAINT()
END_MESSAGE_MAP()


// ServiceDiscoveryDialog message handlers

void ServiceDiscoveryDialog::OnLbnSelchangeServicesList()
{
	CListBox* myListBox;
	myListBox = (CListBox*)GetDlgItem(IDC_SERVICES_LIST);
	selectedSCN = myListBox->GetCurSel() + 1;  //Must add 1 since list is zero based and SCN starts on 1
	Log("SDD: Selected service with index: %d", selectedSCN);

	myListBox->GetText(myListBox->GetCurSel(), selectedServiceName);
	CButton* myButton;
	myButton = (CButton*)GetDlgItem(IDNEXT);
	myButton->EnableWindow(true);
}

void ServiceDiscoveryDialog::OnBnClickedNext()
{
	OnOK();
}

BOOL ServiceDiscoveryDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	PrintStaticHeadlines("Service Selection", "Select a service from the list below");

   	AddServicesToList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//void ServiceDiscoveryDialog::OnFontChange()
//{
//	CDialog::OnFontChange();
//
//	// TODO: Add your message handler code here
//}


BOOL ServiceDiscoveryDialog::OnEraseBkgnd(CDC* pDC)
{
	return HPDialog::OnEraseBkgnd(pDC);
}

HBRUSH ServiceDiscoveryDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return HPDialog::OnCtlColor(pDC,pWnd,nCtlColor);
}

void ServiceDiscoveryDialog::OnPaint()
{
	HPDialog::OnPaint();
}
