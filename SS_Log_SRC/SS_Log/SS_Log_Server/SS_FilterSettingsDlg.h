#if !defined(AFX_SS_FILTERSETTINGSDLG_H__51E7D46D_D427_4AFD_8163_73A3EF75908D__INCLUDED_)
#define AFX_SS_FILTERSETTINGSDLG_H__51E7D46D_D427_4AFD_8163_73A3EF75908D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SS_FilterSettingsDlg.h : header file
//

#include "SS_String.h"



/////////////////////////////////////////////////////////////////////////////
// SS_FilterSettingsDlg dialog

class SSFilterSettings;

class SS_FilterSettingsDlg : public CDialog
{
// Construction
public:
	SS_FilterSettingsDlg(CWnd* pParent, SSFilterSettings* pFilterSettings);   // standard constructor
	~SS_FilterSettingsDlg();

// Dialog Data
	//{{AFX_DATA(SS_FilterSettingsDlg)
	enum { IDD = IDD_FILTERSETTINGS };
	CButton	m_cRequireAllFiltersRadio;
    CButton	m_cRequire1FilterRadio;
	CButton	m_cAutoApply;
	CButton	m_cSearchFiltersGroup;
	CButton	m_cFiltersGroup;
	CButton	m_cTextInMsgCheck;
	CButton	m_cIDCheck;
	CButton	m_cFilenameCheck;
	CEdit	m_cID2;
	CEdit	m_cID1;
	CEdit	m_cFilename;
	CEdit	m_cTextInMsg;
	BOOL	m_bTextInMsg;
	CString	m_csTextInMsg;
	BOOL	m_bFilename;
	CString	m_csFilename;
	BOOL	m_bID;
	CString	m_csID1;
	CString	m_csID2;
	BOOL	m_bAutoApply;
	//}}AFX_DATA
	BOOL	m_bRequireAllFilters;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SS_FilterSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SS_FilterSettingsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnFilenameCheck();
	afx_msg void OnIdCheck();
	afx_msg void OnTextinmsgCheck();
	afx_msg void OnApply();
	afx_msg void OnReqAllFiltersRadio();
	afx_msg void OnReqOneFilterRadio();
	afx_msg void OnFiltersGroup();
	afx_msg void OnChangeTextinmsgEdit();
	afx_msg void OnChangeFilenameEdit();
	afx_msg void OnChangeIdEdit1();
	afx_msg void OnChangeIdEdit2();
	//}}AFX_MSG
    afx_msg void OnDynamicButton();

	DECLARE_MESSAGE_MAP()

    BOOL				ReadFilters();
    BOOL				WriteFilters();
    BOOL				CreateButtons();
	BOOL				SetupWindow();

public:

protected:
    
    BOOL                AutoApply           ();

private:

    SSFilterSettings*	m_pFilterSettings;

};

/////////////////////////////////////////////////////////////////////////////
//


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SS_FILTERSETTINGSDLG_H__51E7D46D_D427_4AFD_8163_73A3EF75908D__INCLUDED_)
