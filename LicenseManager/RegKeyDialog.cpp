//////////////////////////////////////////////////////////////////////
//LicenseManager by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2005
//////////////////////////////////////////////////////////////////////
// RegKeyDialog.cpp : Implementation of CRegKeyDialog

#include "stdafx.h"
#include "RegKeyDialog.h"
#include ".\regkeydialog.h"

CRegKeyDialog::CRegKeyDialog()
{
	//Since it is ATL we cant use CEdit* myEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	//CWindow myEdit(GetDlgItem(IDC_EDIT1));
	
	field1String = "";
}

CRegKeyDialog::~CRegKeyDialog()
{

}

/* Since myRegKeyDialog is created in the constructor of LicenseHandler state is preserved
   This allows preservation of key data entered in the dialog-
*/
LRESULT CRegKeyDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CAxDialogImpl<CRegKeyDialog>::OnInitDialog(uMsg, wParam, lParam, bHandled);
	this->BringWindowToTop();
	this->CenterWindow();
	if(field1String != "")
	{
		SetDlgItemText(IDC_EDIT1, field1String);
	}
	if(field2String != "")
	{
		SetDlgItemText(IDC_EDIT2, field2String);
	}
	if(field3String != "")
	{
		SetDlgItemText(IDC_EDIT3, field3String);
	}
	if(field4String != "")
	{
		SetDlgItemText(IDC_EDIT4, field4String);
	}
	if(field5String != "")
	{
		SetDlgItemText(IDC_EDIT5, field5String);
	}
	GetDlgItem(IDC_EDIT1).SetFocus();
	this->SetWindowText("License Activation for "+productName+" "+productVersion);
	return 1; 
}

void CRegKeyDialog::SetProductInfo(CString productName, CString productVersion)
{
	this->productName = productName;
	this->productVersion = productVersion;
}

LRESULT CRegKeyDialog::OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	char tempString[100];
	keyString.Empty();

	GetDlgItemText(IDC_EDIT1,tempString,20);
	if(strlen(tempString) != 8)
	{
		MessageBox("Invalid key format.\n\nThe first field must contain 8 characters","Error",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		return 0;
	}
	else if(!CheckAllowedChars(tempString))
	{
		MessageBox("Field 1 contains an illegal character.\n\nOnly 0-9 and A-F are allowed.","LicenseManager",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		return 0;
	}
	else
	{
		field1String = tempString;
		keyString.Append(tempString);
		keyString.Append("-");
	}

	GetDlgItemText(IDC_EDIT2,tempString,20);
	if(strlen(tempString) != 4)
	{
		MessageBox("Invalid key format.\n\nThe second field must contain 4 characters","Error",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		return 0;
	}
	else if(!CheckAllowedChars(tempString))
	{
		MessageBox("Field 2 contains an illegal character.\n\nOnly 0-9 and A-F are allowed.","LicenseManager",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		return 0;
	}
	else
	{
		field2String = tempString;
		keyString.Append(tempString);
		keyString.Append("-");
	}

	GetDlgItemText(IDC_EDIT3,tempString,20);
	if(strlen(tempString) != 4)
	{
		MessageBox("Invalid key format.\n\nThe third field must contain 4 characters","Error",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		return 0;
	}
	else if(!CheckAllowedChars(tempString))
	{
		MessageBox("Field 3 contains an illegal character.\n\nOnly 0-9 and A-F are allowed.","LicenseManager",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		return 0;
	}
	else
	{
		field3String = tempString;
		keyString.Append(tempString);
		keyString.Append("-");
	}

	GetDlgItemText(IDC_EDIT4,tempString,20);
	if(strlen(tempString) != 4)
	{
		MessageBox("Invalid key format.\n\nThe fourth field must contain 4 characters","Error",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		return 0;
	}
	else if(!CheckAllowedChars(tempString))
	{
		MessageBox("Field 4 contains an illegal character.\n\nOnly 0-9 and A-F are allowed.","LicenseManager",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		return 0;
	}
	else
	{
		field4String = tempString;
		keyString.Append(tempString);
		keyString.Append("-");
	}

	GetDlgItemText(IDC_EDIT5,tempString,20);
	if(strlen(tempString) != 12)
	{
		MessageBox("Invalid key format.\n\nThe fifth field must contain 12 characters","Error",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		return 0;
	}
	else if(!CheckAllowedChars(tempString))
	{
		MessageBox("Field 5 contains an illegal character.\n\nOnly 0-9 and A-F are allowed.","LicenseManager",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		return 0;
	}
	else
	{
		field5String = tempString;
		keyString.Append(tempString);
	}
	//If we end the dialog the keyString is also ok
	EndDialog(wID);
	return 1;
}

CString CRegKeyDialog::GetKeyString()
{
	return keyString;
}
// CRegKeyDialog


LRESULT CRegKeyDialog::OnEnChangeEdit1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	char tempString[100];
	GetDlgItemText(IDC_EDIT1,tempString,20);
	if(strlen(tempString) == 8)
		GetDlgItem(IDC_EDIT2).SetFocus();
	return 0;
}

LRESULT CRegKeyDialog::OnEnChangeEdit2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	char tempString[100];
	GetDlgItemText(IDC_EDIT2,tempString,20);
	if(strlen(tempString) == 4)
		GetDlgItem(IDC_EDIT3).SetFocus();

	return 0;
}

LRESULT CRegKeyDialog::OnEnChangeEdit3(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	char tempString[100];
	GetDlgItemText(IDC_EDIT3,tempString,20);
	if(strlen(tempString) == 4)
		GetDlgItem(IDC_EDIT4).SetFocus();

	return 0;
}

LRESULT CRegKeyDialog::OnEnChangeEdit4(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	char tempString[100];
	GetDlgItemText(IDC_EDIT4,tempString,20);
	if(strlen(tempString) == 4)
		GetDlgItem(IDC_EDIT5).SetFocus();

	return 0;
}



LRESULT CRegKeyDialog::OnEnChangeEdit5(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	char tempString[100];
	GetDlgItemText(IDC_EDIT5,tempString,20);
	if(strlen(tempString) == 12)
		GetDlgItem(IDOK).SetFocus();

	return 0;
}

bool CRegKeyDialog::CheckAllowedChars(CString fieldString)
{
	for(int i = 0;i< fieldString.GetLength(); i++)
	{
		if(!(((fieldString.GetAt(i) >= '0') && (fieldString.GetAt(i) <= '9')) || ((fieldString.GetAt(i) >= 'A') && (fieldString.GetAt(i) <= 'F'))))
		{
			return false;
		}
	}
	return true;
}
