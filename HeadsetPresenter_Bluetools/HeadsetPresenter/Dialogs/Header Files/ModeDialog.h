//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
#pragma once
#include "HPDialog.h"
#include "SettingsControl.h"


// ModeDialog dialog

class ModeDialog : public HPDialog
{
	DECLARE_DYNAMIC(ModeDialog)

public:
	ModeDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ModeDialog();
	virtual BOOL OnInitDialog();
	int GetSelectedMode();
	void SetSupportedModes(int iSupportedModes);
	void SetSettingsControl(SettingsControl* iSettingsControl);
// Dialog Data
	enum { IDD = IDD_MODE_DIALOG };
private:
	int selectedMode;
	int supportedModes;
	SettingsControl* mySettingsControl;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBack();
	afx_msg void OnBnClickedNext();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedCancel3();
	afx_msg void OnBnClickedRadio2();
};
