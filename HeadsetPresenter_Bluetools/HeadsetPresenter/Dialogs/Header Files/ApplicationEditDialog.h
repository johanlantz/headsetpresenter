#pragma once
#include "HPDialog.h"

// ApplicationEditDialog dialog

class ApplicationEditDialog : public HPDialog
{
	DECLARE_DYNAMIC(ApplicationEditDialog)

public:
	ApplicationEditDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ApplicationEditDialog();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_APP_EDIT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

private:
	void LoadData();
	void SaveData();
	CString getPath();

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};
