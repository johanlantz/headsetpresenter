#pragma once


// ApplicationDialog dialog
/*struct buttonSet
{
	CString name;
	CButton* ctrlCheckButton;
	CButton* altCheckButton;
	CComboBox* VKBox;
};*///This actually seemed to crash windows?????

class ApplicationDialog : public CDialog
{
	DECLARE_DYNAMIC(ApplicationDialog)

public:
	ApplicationDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ApplicationDialog();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_APPLICATION_MANAGER };
private:
	CString VKS[100];
	bool InitVKS();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
