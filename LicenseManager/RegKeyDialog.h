// RegKeyDialog.h : Declaration of the CRegKeyDialog
//////////////////////////////////////////////////////////////////////
//LicenseManager by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2005
//////////////////////////////////////////////////////////////////////

#pragma once

#include "resource.h"       // main symbols
#include <atlhost.h>


// CRegKeyDialog

class CRegKeyDialog : 
	public CAxDialogImpl<CRegKeyDialog>
{
public:
	CRegKeyDialog();
	

	~CRegKeyDialog();


	enum { IDD = IDD_REGKEYDIALOG };

BEGIN_MSG_MAP(CRegKeyDialog)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	COMMAND_HANDLER(IDC_EDIT1, EN_CHANGE, OnEnChangeEdit1)
	COMMAND_HANDLER(IDC_EDIT2, EN_CHANGE, OnEnChangeEdit2)
	COMMAND_HANDLER(IDC_EDIT3, EN_CHANGE, OnEnChangeEdit3)
	COMMAND_HANDLER(IDC_EDIT4, EN_CHANGE, OnEnChangeEdit4)
	COMMAND_HANDLER(IDC_EDIT5, EN_CHANGE, OnEnChangeEdit5)
	CHAIN_MSG_MAP(CAxDialogImpl<CRegKeyDialog>)
END_MSG_MAP()

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 2;
	}

	CString GetKeyString();
	void SetProductInfo(CString productName, CString productVersion);
private:
	CString keyString;
	CString productName;
	CString productVersion;
	CString field1String;
	CString field2String;
	CString field3String;
	CString field4String;
	CString field5String;
	bool CheckAllowedChars(CString fieldString);
public:
	LRESULT OnEnChangeEdit1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEnChangeEdit2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEnChangeEdit3(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEnChangeEdit4(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEnChangeEdit5(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};


