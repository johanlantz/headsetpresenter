#pragma once

#include "HPDialog.h"
// ManualStackDialog dialog

#define TEXT_NO_KNOWN_STACK_FOUND		1
#define TEXT_MORE_THEN_ONE_STACK_FOUND	2

class ManualStackDialog : public HPDialog
{
	DECLARE_DYNAMIC(ManualStackDialog)

public:
	ManualStackDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ManualStackDialog();
	virtual BOOL OnInitDialog();
	int GetSelectedStack();
	void SetText(int textNo);

// Dialog Data
	enum { IDD = IDD_MANUAL_STACK_SELECTION };

private:
	int selectedStack;
	CString staticText1;
	CString staticText2;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioMicrosoft();
	afx_msg void OnBnClickedRadioWidcomm();
	afx_msg void OnBnClickedRadioBluesoleil();
	afx_msg void OnBnClickedRadioToshiba();
	afx_msg void OnBnClickedRadioOther();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};
