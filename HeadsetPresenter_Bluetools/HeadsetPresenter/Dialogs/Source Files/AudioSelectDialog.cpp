// AudioSelectDialog.cpp : implementation file
//

#include "stdafx.h"
#include "HeadsetPresenter.h"
#include "AudioSelectDialog.h"
#include "..\header files\audioselectdialog.h"
#include "SS_Log.h"


// AudioSelectDialog dialog

IMPLEMENT_DYNAMIC(AudioSelectDialog, CDialog)
AudioSelectDialog::AudioSelectDialog(CWnd* pParent /*=NULL*/)
	: HPDialog(AudioSelectDialog::IDD)
{
	imageToDraw = IDB_NEWLOGO;
	hasUpperLine = true;
}

AudioSelectDialog::~AudioSelectDialog()
{
}


void AudioSelectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL AudioSelectDialog::OnInitDialog()
{
	PrintStaticHeadlines("Manual Audio Selection", "Select the desired audio device");
	PrintStaticMessage(IDC_STATIC_AUDIO, "The HeadsetPresenter could not automatically identify which Bluetooth audio connection to use. You must manually select your audio device.\n\nNote: Do not select devices called Bluetooth Audio High Quality, Bluetooth AV Audio or similar.\n\nLook for a device called simply Bluetooth Audio or Bluetooth SCO Audio.", STANDARD_FONT);
	//int index;
	CListBox* myLB = (CListBox*)GetDlgItem(IDC_AUDIO_LIST);
	
	POSITION pos;
	CString devName;
	for( pos = deviceList.GetHeadPosition(); pos != NULL; )
	{
		myLB->AddString(deviceList.GetNext(pos));
	}
	myLB->SetCurSel(0);
	return TRUE;
}

BEGIN_MESSAGE_MAP(AudioSelectDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

void AudioSelectDialog::AddDevice(CString devName, int tag)
{
	deviceList.AddTail(devName);
	Log("ASD:Added %s to listbox",devName);
	/*int index;
	CListBox* myLB = (CListBox*)GetDlgItem(IDC_AUDIO_LIST);
	index = myLB->AddString(devName);
	myLB->SetItemData(index,tag);
	Log("ASD:Added %s to listbox",devName);*/
}

CString AudioSelectDialog::GetSelectedDeviceName()
{
	return selectedDeviceName;
}
// AudioSelectDialog message handlers

void AudioSelectDialog::OnBnClickedOk()
{
	int index;
	CListBox* myLB = (CListBox*)GetDlgItem(IDC_AUDIO_LIST);
	index = myLB->GetCurSel();
	if(index != LB_ERR)
	{
		myLB->GetText(index,selectedDeviceName);
		
	}
	else
	{
		MessageBox("ASD:An error occured in the listbox, contact support");
		Log("HPD: ListBox->GetCurSel returned an error");
		selectedDeviceName = "error";
	}
	Log("ASD:Selected device : %s", selectedDeviceName);
	OnOK();
}

void AudioSelectDialog::OnBnClickedCancel()
{
	OnCancel();
}

HBRUSH AudioSelectDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return HPDialog::OnCtlColor(pDC,pWnd,nCtlColor);
}

BOOL AudioSelectDialog::OnEraseBkgnd(CDC* pDC)
{
	return HPDialog::OnEraseBkgnd(pDC);
}

void AudioSelectDialog::OnPaint()
{
	HPDialog::OnPaint();
}
