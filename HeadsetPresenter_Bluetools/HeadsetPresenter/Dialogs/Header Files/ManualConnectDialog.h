#pragma once
#include "HPDialog.h"

// ManualConnectDialog dialog

class ManualConnectDialog : public HPDialog
{
	DECLARE_DYNAMIC(ManualConnectDialog)

public:
	ManualConnectDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ManualConnectDialog();
	virtual BOOL OnInitDialog();
	void GotCommand();
	bool IsDialogActive();
	bool isDialogActive;
// Dialog Data
	enum { IDD = IDD_MANUAL_CONNECT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedNext();
	afx_msg void OnStnClickedManualConInfo();
	afx_msg void OnBnClickedButton1();
};
