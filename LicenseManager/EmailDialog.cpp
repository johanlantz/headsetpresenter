// EmailDialog.cpp : Implementation of CEmailDialog

#include "stdafx.h"
#include "EmailDialog.h"
#include <afxinet.h>
#include "InternetManager.h"

LRESULT CEmailDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CAxDialogImpl<CEmailDialog>::OnInitDialog(uMsg, wParam, lParam, bHandled);
	this->BringWindowToTop();
	this->CenterWindow();
	this->SetWindowText("Optional email registration");	
	return 1;  // Let the system set the focus
}

void CEmailDialog::SetServerPath(CString path)
{
	Log("emailDialog setServerPath to:", serverPath);
	serverPath = path;
}

void CEmailDialog::SetRegistrationComment(CString iComment)
{
	registrationComment = iComment;
}

void CEmailDialog::SetKeyString(CString key)
{
	keyString = key;
}

//The idea is to keep this sub simple. If email reg fails it is not the end of the world
LRESULT CEmailDialog::OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	LogEnterFunction("CEmailDialog");
	InternetManager myInternetManager;
	CString queryString;
	char tempString[100];
	GetDlgItemText(IDC_EDIT1,tempString,99);
	
	queryString = "/access/addRegistrationInfo.asp";
	queryString += "?email=" + CString(tempString) + "&identifier=" + keyString;
	Log("QueryString = %s", queryString);
	Log("SendRequestToServer");
	myInternetManager.SendRequestToServer(queryString);
	Log("RequestToServer sent");
	EndDialog(wID);
	return IDOK;
}

// CEmailDialog
