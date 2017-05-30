//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
#pragma once
#include "HPDialog.h"

class SuperController;

// DeviceDiscoveryDialog dialog

class DeviceDiscoveryDialog : public HPDialog
{
	DECLARE_DYNAMIC(DeviceDiscoveryDialog)

public:
	DeviceDiscoveryDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~DeviceDiscoveryDialog();

// Dialog Data
	enum { IDD = IDD_DEVICE_DIALOG };

	void AddDeviceToList(CString deviceName);
	void SetController(SuperController* iController);
	void DeviceDiscoveryComplete();  //Asynch callback
	void SetStatus(bool searching);  //Should be private when widcomm is asynch

private:
	SuperController* myController;
	bool isSearching;
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStartStop();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLbnSelchangeDevices();
	afx_msg void OnBnClickedOk();
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBack();
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
