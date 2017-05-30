#pragma once
#include "HpDialog.h"
#include "SettingsControl.h"
#include <sphelper.h> 
// SettingsDialog dialog

class SettingsDialog : public HPDialog
{
	DECLARE_DYNAMIC(SettingsDialog)

public:
	SettingsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~SettingsDialog();
	virtual BOOL OnInitDialog();
	void SetSettingsControl(SettingsControl* iSettingsControl);
// Dialog Data
	enum { IDD = IDD_SETTINGS_DIALOG };

private:
	SettingsControl* mySettingsControl;
	void CheckSpeech();
	void getHelp(int codeID);
	bool pendingRestore;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedDownload();

	
	afx_msg void OnBnClickedTrain();
	afx_msg void OnBnClickedBtnRestore();
	afx_msg void OnBnClickedCheckSpeechActive();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCheckTestMode();
};
