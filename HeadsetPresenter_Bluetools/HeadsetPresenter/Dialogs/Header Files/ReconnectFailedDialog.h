//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
#pragma once
#include "SuperController.h"
#include "HPDialog.h"

// ReconnectFailedDialog dialog
//Need 3 buttons, no enough with IDOK and IDCANCEL
#define BUTTON_OK		0
#define BUTTON_CANCEL	1
#define BUTTON_WIZARD	2

class ReconnectFailedDialog : public HPDialog
{
	DECLARE_DYNAMIC(ReconnectFailedDialog)

public:
	ReconnectFailedDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ReconnectFailedDialog();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_RECONNECT_DIALOG };
	void SetController(SuperController* iController);
	int GetButtonPressed();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRetry();
	afx_msg void OnBnClickedWizard();
private:
	SuperController* myController;
	int buttonPressed;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedCancel();
};
