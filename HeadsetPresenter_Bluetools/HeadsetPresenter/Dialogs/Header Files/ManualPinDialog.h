#pragma once
#include "HPDialog.h"

// ManualPinDialog dialog

class ManualPinDialog : public HPDialog
{
	DECLARE_DYNAMIC(ManualPinDialog)

public:
	ManualPinDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ManualPinDialog();
	virtual BOOL OnInitDialog();
	CString GetPin();
	void SetEditFocus();
// Dialog Data
	enum { IDD = IDD_MANUAL_PIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
private:
	CString pin;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};
