#pragma once
#include "HPDialog.h"

// AudioSelectDialog dialog
#define NO_TAG -1

class AudioSelectDialog : public HPDialog
{
	DECLARE_DYNAMIC(AudioSelectDialog)

public:
	AudioSelectDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~AudioSelectDialog();
	void AddDevice(CString devName, int tag);
	CString GetSelectedDeviceName();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_AUDIO_SELECT };

private:
	CString selectedDeviceName;
	CStringList deviceList;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};
