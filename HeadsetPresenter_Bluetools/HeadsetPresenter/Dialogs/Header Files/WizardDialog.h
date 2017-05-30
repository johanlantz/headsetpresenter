//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
#pragma once
#include "HPDialog.h"


class SuperController;

// WizardDialog dialog
#define PROGRESS_RANGE 160  //0->15 could be 150 if all steps work

enum WizardStates
{
 STATE_WIZARD_NO_STATE,
 STATE_WIZARD_FIND_SERVICES,
 STATE_WIZARD_TRY_HFP,
 STATE_WIZARD_TRY_HSP,
 STATE_WIZARD_TRY_AVRCP,
 STATE_WIZARD_TRY_UNKNOWN,
 STATE_WIZARD_DONE,
 STATE_WIZARD_COMPLETED
};
class WizardDialog : public HPDialog
{
	DECLARE_DYNAMIC(WizardDialog)

public:
	WizardDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~WizardDialog();

// Dialog Data
	enum { IDD = IDD_WIZARD_DIALOG };
	void SetController(SuperController* iController);
	void ServiceDiscoveryCompleted(int serviceCount);
	bool CommandReceived(CString ATCommand, int commandType);
	void SetWizardState(int newState);
	int GetWizardState();
	
private:
	bool PerformManualServiceSelection();
	void PairingFailed();
	SuperController* myController;
	int GetVGS(CString ATCommand);
	int lastVGS;
	int wizardState;
	int vgsCounter;
	bool aVGSIsReceived;
	bool anAVRCPIsReceived;
	time_t firstCKPDTime;
	time_t firstVGSTime;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonStuck();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
};
