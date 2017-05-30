// ApplicationDialog.cpp : implementation file
//

#include "stdafx.h"
#include "HeadsetPresenter.h"
#include "ApplicationDialog.h"
#include "..\header files\applicationdialog.h"


// ApplicationDialog dialog

IMPLEMENT_DYNAMIC(ApplicationDialog, CDialog)
ApplicationDialog::ApplicationDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ApplicationDialog::IDD, pParent)
{
	
}

ApplicationDialog::~ApplicationDialog()
{
}

void ApplicationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ApplicationDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BOOL ApplicationDialog::OnInitDialog()
{
	InitVKS();
	return TRUE;
}
// ApplicationDialog message handlers
bool ApplicationDialog::InitVKS()
{
	VKS[0] = "VK_N";
	VKS[1] = "VK_P";
	CComboBox* m = (CComboBox*)GetDlgItem(IDC_COMBO1);
	for(int i=0;i<2;i++)
	{
		m->AddString(VKS[i]);
	}
	
	return true;
}

void ApplicationDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
