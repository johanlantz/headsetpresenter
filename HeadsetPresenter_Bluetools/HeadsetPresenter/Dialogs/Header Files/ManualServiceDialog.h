//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
#pragma once
#include "HPDialog.h"

// ManualServiceDialog dialog

class ManualServiceDialog : public HPDialog
{
	DECLARE_DYNAMIC(ManualServiceDialog)

public:
	ManualServiceDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ManualServiceDialog();
	virtual BOOL OnInitDialog();
	bool GetSelectManually();
// Dialog Data
	enum { IDD = IDD_MANUAL_SERVICE_DIALOG };
private:
	bool selectManually;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedNext();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};
