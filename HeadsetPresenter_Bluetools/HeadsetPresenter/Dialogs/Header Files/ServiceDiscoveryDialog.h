//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
#pragma once

#include "HPDialog.h"
#include "resource.h" //solves the issue but I do not know why if this is removed it says that IDD_SERVICE_DIALOG is undeclared

class SuperController;

// ServiceDiscoveryDialog dialog

class ServiceDiscoveryDialog : public HPDialog
{
	DECLARE_DYNAMIC(ServiceDiscoveryDialog)

public:
	ServiceDiscoveryDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ServiceDiscoveryDialog();

// Dialog Data
	enum { IDD = IDD_SERVICE_DIALOG };
	void SetController(SuperController* iBtControl);
	void AddServicesToList();
	int GetSCN();
	CString GetServiceName();
	void CheckForKnownSpecialCases();

private:
	SuperController* myController;
	int selectedSCN;
	CString selectedServiceName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeServicesList();
	afx_msg void OnBnClickedNext();
	virtual BOOL OnInitDialog();
//	afx_msg void OnFontChange();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
};

