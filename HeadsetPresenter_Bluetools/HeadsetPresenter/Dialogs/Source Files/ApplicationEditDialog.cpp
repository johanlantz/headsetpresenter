// ApplicationEditDialog.cpp : implementation file
//

#include "stdafx.h"
#include "HeadsetPresenter.h"
#include "ApplicationEditDialog.h"
#include ".\applicationeditdialog.h"
#include "SS_Log.h"
#include "..\header files\applicationeditdialog.h"


// ApplicationEditDialog dialog

IMPLEMENT_DYNAMIC(ApplicationEditDialog, CDialog)
ApplicationEditDialog::ApplicationEditDialog(CWnd* pParent /*=NULL*/)
	: HPDialog(ApplicationEditDialog::IDD)
{
	hasUpperLine = true;
	imageToDraw = IDB_LOGO_50x50;
}

ApplicationEditDialog::~ApplicationEditDialog()
{
}

void ApplicationEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL ApplicationEditDialog::OnInitDialog()
{
	PrintStaticHeadlines("Application and button mapping editor", "Configure which applications to control and how to control them");
	PrintStaticMessage(IDC_APP_EDIT_STATIC,"See chapter 3 in the documentation for complete instructions on how to add or edit applications.",STANDARD_FONT);
	LoadData();
	return TRUE;
}

BEGIN_MESSAGE_MAP(ApplicationEditDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// ApplicationEditDialog message handlers
void ApplicationEditDialog::LoadData()
{
	CEdit* myBox = (CEdit*)GetDlgItem(IDC_APP_EDIT);
	try
	{
		CString path = getPath();
		path = path + "ButtonMappings.txt";
		CFile myFile(path, CFile::modeCreate | CFile::modeNoTruncate| CFile::shareDenyNone);
		int bytesRead=0;
		char s[10000];
		//CString settings;
		//bytesRead = myFile.Read(settings.GetBufferSetLength(50),50);
		bytesRead = myFile.Read(s,10000);
		
		if(bytesRead > 0)
		{
			s[bytesRead] = 0;
			//settings = s;
			myBox->SetWindowText(s);
		}
		myFile.Close();
	}
	catch(CFileException* pEx)
	{
		pEx->Delete();
		Log("AED:Could not open ButtonMappings.txt");
		MessageBox("Failed to open ButtonMappings.txt, contact support");
	}
}

void ApplicationEditDialog::SaveData()
{
	CEdit* myBox = (CEdit*)GetDlgItem(IDC_APP_EDIT);
	try
	{
		CString path = getPath();
		path = path + "ButtonMappings.txt";
		CFile myFile(path, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone);	

		CString dataToWrite;
		myBox->GetWindowText(dataToWrite);
		myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());

		myFile.Close();
	}
	catch(CFileException* pEx)
	{
		pEx->Delete();
		Log("AED:Could not save ButtonMappings.txt");
		MessageBox("Failed to save ButtonMappings.txt, contact support");
	}

}

CString ApplicationEditDialog::getPath()
{
	/*Get path to Configurator.exe = path to HeadsetPresenter.dll*/
	TCHAR fullPathToEXE[512];
	//Get the dir path only and set \0 after the final \ in the path to remove the filename of the exe
	DWORD dwResult = ::GetModuleFileName(NULL,fullPathToEXE, sizeof(fullPathToEXE));
	char* t = _tcsrchr(fullPathToEXE,'\\');
	t++;
	*t = '\0';
	//end get path
	return CString(fullPathToEXE);
}

void ApplicationEditDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	SaveData();
	MessageBox("Your changes have been saved, HeadsetPresenter must be restarted for the changes to take effect.","Changes saved",MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
	OnOK();
}

HBRUSH ApplicationEditDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = HPDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

BOOL ApplicationEditDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return HPDialog::OnEraseBkgnd(pDC);
}

void ApplicationEditDialog::OnPaint()
{
	HPDialog::OnPaint();
}
