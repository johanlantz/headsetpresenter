#pragma once
#include "HPDialog.h"

// AlreadyConnectedToAGDialog dialog

class AlreadyConnectedToAGDialog : public HPDialog
{
	DECLARE_DYNAMIC(AlreadyConnectedToAGDialog)

public:
	AlreadyConnectedToAGDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~AlreadyConnectedToAGDialog();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_ACTIVE_CONNECTION_PROBLEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
};
