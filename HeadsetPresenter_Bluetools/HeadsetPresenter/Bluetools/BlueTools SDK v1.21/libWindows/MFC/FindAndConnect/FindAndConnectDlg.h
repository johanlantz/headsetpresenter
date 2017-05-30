// FindAndConnectDlg.h : header file
//

#if !defined(AFX_FINDANDCONNECTDLG_H__5B696CF3_A169_4816_9A06_7B51B3784679__INCLUDED_)
#define AFX_FINDANDCONNECTDLG_H__5B696CF3_A169_4816_9A06_7B51B3784679__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFindAndConnectDlg dialog

class CFindAndConnectDlg : 
	public CDialog,
	// BlueTools: Interfaces that handles events from the xNetwork, xRemoteDevice and xStream
	public xINetworkEvents,	
	public xIRemoteDeviceEvents,
	public xIStreamEvents
	
{
// Construction
public:
	CFindAndConnectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFindAndConnectDlg)
	enum { IDD = IDD_FINDANDCONNECT_DIALOG };
	CStatic	m_labelStatus;
	CEdit	m_textDataWrite;
	CEdit	m_textDataRead;
	CListBox	m_boxServices;
	CListBox	m_boxDevices;
	CButton	m_buttonWrite;
	CButton	m_buttonOpen;
	CButton	m_buttonClose;
	CButton	m_buttonDiscover;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindAndConnectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// BlueTools: Network event interface
	void OnDeviceDiscoveryStarted(xNetwork* source);
	void OnDeviceDiscovered(xNetwork* source, xRemoteDevice* discoveredDevice);
	void OnDeviceLost(xNetwork* source, xRemoteDevice* lostDevice);
	void OnDeviceDiscoveryCompleted(xNetwork* source, xDeviceList* discoveredDevices);
	void OnError(xNetwork* source, int errorCode, xString* errorText);

	// BlueTools: xIRemoteDeviceEvents
	void OnServiceDiscoveryStarted(xRemoteDevice* source);
	void OnServiceDiscovered(xRemoteDevice* source, xRemoteService* discoveredService);
	void OnServiceDiscoveryCompleted(xRemoteDevice* source, xServiceList* discoveredServices);
	void OnError(xRemoteDevice* source, int errorCode, xString* errorText);
	void OnBondStarted(xRemoteDevice* source) {}
	void OnBondCompleted(xRemoteDevice* source) {}
	void OnUnbondStarted(xRemoteDevice* source) {}
	void OnUnbondCompleted(xRemoteDevice* source) {}

	// BlueTools: xIStreamEvents
	void OnReadCompleted(xStream* source, xStreamResult* results);
	void OnWriteCompleted(xStream* source, xStreamResult* results) {} 


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFindAndConnectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDiscover();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnOpen();
	afx_msg void OnWrite();
	afx_msg void OnSelchangeDevices();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	xManager* m_manager;
	xNetwork* m_network;
	xRemoteDevice* m_device;

	xList* m_listDevices;
	xList* m_listServices;

	unsigned char m_buffer[256];

	void m_cleanup();

	void m_show_error(xObject* obj);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDANDCONNECTDLG_H__5B696CF3_A169_4816_9A06_7B51B3784679__INCLUDED_)
