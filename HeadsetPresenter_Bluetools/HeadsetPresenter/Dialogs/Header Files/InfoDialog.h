//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
#pragma once
#include "HPDialog.h"
#include "SettingsControl.h"

// InfoDialog dialog
#define INFO_BOX_NOT_VISIBLE 0
#define INFO_BOX_VISIBLE 1

enum InfoDlgBoxIds
{
	INFO_DLG_NO_BOX				= 0,
	INFO_DLG_ADVANCED_MODE_BOX	= 0x01,
	INFO_DLG_STARTUP			= 0x02,
	INFO_DLG_PINCODE			= 0x04,
	INFO_DLG_SOFT_ON_OLD_DRIVER = 0x08,
	INFO_DLG_WINDOW_NOT_FOUND	= 0x10
};

class InfoDialog : public HPDialog
{
	DECLARE_DYNAMIC(InfoDialog)

public:
	InfoDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~InfoDialog();
	virtual BOOL OnInitDialog();
	void SetText(CString header, CString subHeader, CString normText);
	void SetCheckboxVisibility(int iVisible);
	void SetSettingsControl(SettingsControl* iMySettingsControl);
	void SetBoxId(int iBoxId);
	bool ShowBox();
// Dialog Data
	enum { IDD = IDD_INFO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnStnClickedInfoText();
	afx_msg void OnBnClickedOk();
private:
	CString headerText;
	CString subHeaderText;
	CString text;
	int checkboxVisible;
	int boxId;
	SettingsControl* mySettingsControl;
};
