#pragma once

#include "HPDialog.h"
// AVRCPDialog dialog

class AVRCPDialog : public HPDialog
{
	DECLARE_DYNAMIC(AVRCPDialog)

public:
	AVRCPDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~AVRCPDialog();
	virtual BOOL OnInitDialog();
	bool IsAVRCPSupported();
// Dialog Data
	enum { IDD = IDD_AVRCP_DIALOG };

private:
	bool AVRCPSupported;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
