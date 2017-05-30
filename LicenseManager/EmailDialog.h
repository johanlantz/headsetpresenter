// EmailDialog.h : Declaration of the CEmailDialog

#pragma once

#include "resource.h"       // main symbols
#include <atlhost.h>


// CEmailDialog

class CEmailDialog : 
	public CAxDialogImpl<CEmailDialog>
{
public:
	CEmailDialog()
	{
	}

	~CEmailDialog()
	{
	}

	enum { IDD = IDD_EMAILDIALOG };

BEGIN_MSG_MAP(CEmailDialog)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	CHAIN_MSG_MAP(CAxDialogImpl<CEmailDialog>)
END_MSG_MAP()

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	void SetServerPath(CString path);
	void SetKeyString(CString key);
	void SetRegistrationComment(CString iComment);

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

private:
	CString serverPath;
	CString registrationComment;
	CString keyString;
};


