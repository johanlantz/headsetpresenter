#pragma once
#include "HPDialog.h"

// PreparePairingDlg dialog

class PreparePairingDlg : public HPDialog
{
	DECLARE_DYNAMIC(PreparePairingDlg)

public:
	PreparePairingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~PreparePairingDlg();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_PREPARE_PAIRING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckDevPrep();
	afx_msg void OnBnClickedNext();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};
