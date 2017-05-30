//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HeadsetPresenter.h"
//#include "InfoDialog.h"
#include "..\header files\infodialog.h"
#include "SS_Log.h"


// InfoDialog dialog

IMPLEMENT_DYNAMIC(InfoDialog, CDialog)
InfoDialog::InfoDialog(CWnd* pParent /*=NULL*/)
	: HPDialog(InfoDialog::IDD)
{
	checkboxVisible = INFO_BOX_NOT_VISIBLE;
	mySettingsControl = NULL;
	boxId = INFO_DLG_NO_BOX	;
}

InfoDialog::~InfoDialog()
{
}

void InfoDialog::SetBoxId(int iBoxId)
{
	boxId = iBoxId;
	
	
}

void InfoDialog::SetSettingsControl(SettingsControl* iMySettingsControl)
{
	mySettingsControl= iMySettingsControl;
}

void InfoDialog::SetText(CString header, CString subHeader, CString normText)
{
	headerText = header;
	subHeaderText = subHeader;
	text = normText;
}

void InfoDialog::SetCheckboxVisibility(int iVisible)
{
	checkboxVisible = iVisible;
	CButton* myChkButton = (CButton*)GetDlgItem(IDC_CHECK_DISP_AGAIN);
	myChkButton->SetCheck(checkboxVisible);
}

BOOL InfoDialog::OnInitDialog()
{
	PrintStaticMessage(IDC_INFO_HEADER, headerText, HEADLINE);
	//PrintStaticMessage(IDC_INFO_SUB_HEADER, "For Jabra evaluation only", SMALL_HEADLINE);
	PrintStaticMessage(IDC_INFO_SUB_HEADER, subHeaderText, SMALL_HEADLINE);
	CString message;
	//message = "This is a Beta release of HeadsetPresenter 3.0. It has only been tested on a very limited set of configurations. \n\nAll feeback will be noted and can be sent directly to info@headsetpresenter.com.\n\nNote that most Jabra headsets will only support control with the Connect button at this time. With the fix made last summer for BT250 and BT550 the Jabra headsets offer 3 buttons for control, volume up/down and Connect.\n\nIn this release AVRCP is supported in Advanced mode.\n\n"; 
	//message = "This is a Beta release of HeadsetPresenter 3.0. It has only been tested on a very limited set of configurations. \n\nAll feeback will be noted and can be sent directly to info@headsetpresenter.com.\n\nIn this release AVRCP is supported in Advanced mode.\n\n"; 
	PrintStaticMessage(IDC_INFO_TEXT, text, SMALL_TEXT);
	
	//if there is a box id, there is a box, make it visible
	if(boxId != INFO_DLG_NO_BOX)
	{
		CButton* myChkButton = (CButton*)GetDlgItem(IDC_CHECK_DISP_AGAIN);
		myChkButton->ShowWindow(SW_SHOW);
		
	}
	return TRUE;
}

void InfoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(InfoDialog, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_STN_CLICKED(IDC_INFO_TEXT, OnStnClickedInfoText)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// InfoDialog message handlers

HBRUSH InfoDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return HPDialog::OnCtlColor(pDC,pWnd,nCtlColor);
}

BOOL InfoDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return HPDialog::OnEraseBkgnd(pDC);
}

void InfoDialog::OnPaint()
{
	HPDialog::OnPaint();
}

bool InfoDialog::ShowBox()
{
	if(mySettingsControl == NULL)  //no boxflags just dialog
	{
		return true;
	}

	if(mySettingsControl->GetInfoBoxCheckBoxFlags() & boxId)
	{
		return false;  //bit is set do not show
	}
	else
	{
		return true;
	}

}

void InfoDialog::OnStnClickedInfoText()
{
	// TODO: Add your control notification handler code here
}

void InfoDialog::OnBnClickedOk()
{
	//Check if check box should be available
	Log("IFD:OnOk");
	if(boxId != INFO_DLG_NO_BOX)
	{
		Log("IFD:No NoBox");
		if(mySettingsControl != NULL)
		{
			Log("IFD:Got SettingsControl");
			CButton* myChkButton = (CButton*)GetDlgItem(IDC_CHECK_DISP_AGAIN);
			if(myChkButton->GetCheck() == 1)
			{
				mySettingsControl->SetInfoBoxCheckBoxFlag(boxId, true);
			}
			else
			{
				mySettingsControl->SetInfoBoxCheckBoxFlag(boxId, false);
			}
			
		}
		else
		{
			Log("IFD: Wanted to update a box but SettingsDialog not set");
		}
		
	}
	OnOK();
}
