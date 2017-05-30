// AVRCPDialog.cpp : implementation file
//

#include "stdafx.h"
#include "HeadsetPresenter.h"
#include "AVRCPDialog.h"
#include "..\header files\avrcpdialog.h"

#include "SS_Log.h"


// AVRCPDialog dialog

IMPLEMENT_DYNAMIC(AVRCPDialog, CDialog)
AVRCPDialog::AVRCPDialog(CWnd* pParent /*=NULL*/)
	: HPDialog(AVRCPDialog::IDD)
{
	AVRCPSupported = false;
	
}

AVRCPDialog::~AVRCPDialog()
{
}

void AVRCPDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AVRCPDialog, CDialog)
	ON_BN_CLICKED(IDC_OK, OnBnClickedOk)
END_MESSAGE_MAP()


// AVRCPDialog message handlers
BOOL AVRCPDialog::OnInitDialog()
{
	PrintStaticHeadlines("AVRCP Support", "Does your Headset support AVRCP");
	CheckRadioButton(IDC_RADIO_AVRCP_1, IDC_RADIO_AVRCP_2, IDC_RADIO_AVRCP_2);
	return TRUE;
}

bool AVRCPDialog::IsAVRCPSupported()
{
	return AVRCPSupported;
}

void AVRCPDialog::OnBnClickedOk()
{
	int checkedButton = GetCheckedRadioButton(IDC_RADIO_AVRCP_1, IDC_RADIO_AVRCP_2);
	if(checkedButton == IDC_RADIO_AVRCP_1)
	{
		Log("AVD:AVRCP supported");
		AVRCPSupported = true;
	}
	else
	{
		Log("AVD:AVRCP not supported");
		AVRCPSupported = false;
	}
}
