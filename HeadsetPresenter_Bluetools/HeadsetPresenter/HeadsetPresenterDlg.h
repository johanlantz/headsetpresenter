//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
#pragma once
// If there are loads of compile errors here, check so that no ; is forgotten in defines.h or elsewhere
#include "BluetoolsController.h"
#include "GenericController.h"

#include "SettingsControl.h"
#include "ApplicationControl.h"
#include "HPDialog.h"

#include "ControllerFactory.h"
#include ".\Speech\Header Files\SAPI.h"



// CHeadsetPresenterDlg dialog
//class CHeadsetPresenterDlg : public CDialog    //JCHANGE1
class CHeadsetPresenterDlg : public HPDialog
{
// Construction
public:
	CHeadsetPresenterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_HEADSETPRESENTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void ConnectionCompleted();
	void ConnectionFailed();
	void ConnectionLost();
	
	bool RunPairingGuide();
	afx_msg void OnBnClickedOk();
	virtual void OnFinalRelease();
	void CommandCallback(CString command, int commandType);
	void SetForwardOnlyMode(bool newMode);
	CString GetGrammarName();
	void StartTimer();
	void StopTimer();
	int GetSupportedModes();
	//void PrintStaticMessage(int controlId, CString text, int fontMode);
protected:
	virtual void OnCancel();  //most likely not used
public:
	afx_msg void OnBnClickedButtonStartWizard();
	afx_msg void OnBnClickedButtonReconnect();
private:
	//PowerPointProxy* myPPProxy;
	ApplicationControl* myAppControl;
	SettingsControl* mySettingsControl;
	void ComposeMessage();
	void PopulateAppNameListBox();

	//GenericController* myGenericController;
	SuperController* myController;
	ControllerFactory* myControllerFactory;
	void CreateController(int controllerMode);
	void ShowStackSpecificInfo();
	
	
	SAPI mySAPI;
	//void CALLBACK EXPORT TimerProc(HWND hWnd,UINT nMsg, UINT_PTR nIDEvent, DWORD dwTime);
	UINT_PTR myTimer;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLbnSelchangeAppnameListbox();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnSettings();
	afx_msg void OnTimer(UINT nIDEvent);
};
