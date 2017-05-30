#pragma once
#include "HPDialog.h"

// ToshibaInfoDialog dialog

class ToshibaInfoDialog : public HPDialog
{
	DECLARE_DYNAMIC(ToshibaInfoDialog)

public:
	ToshibaInfoDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ToshibaInfoDialog();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_TOSHIBAINFO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBack();
	afx_msg void OnBnClickedCancel2();
};
