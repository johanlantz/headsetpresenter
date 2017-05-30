//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
#pragma once
#include "HPDialog.h"
#include "WizardDialog.h"

// WizardCompletedDialog dialog
//This dialog is also reused to display done message for reconnect
enum modes
{
	COMPLETED_MODE_WIZARD,
	COMPLETED_MODE_RECONNECT
};
class WizardCompletedDialog : public HPDialog
{
	DECLARE_DYNAMIC(WizardCompletedDialog)

public:
	WizardCompletedDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~WizardCompletedDialog();
	virtual BOOL OnInitDialog();
	void SetWizardDialog(WizardDialog* iWizDlg);
// Dialog Data
	enum { IDD = IDD_WIZARD_COMPLETED_DIALOG };
	void SetMode(int imode);
	void SetAppName(CString iappName);

private:
	int mode;
	CString appName;
	WizardDialog* myWizardDialog;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFinish();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};
