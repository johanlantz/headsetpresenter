// SS_Log_ServerView.h : interface of the SS_Log_ServerView class
// Version 3.02
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SS_Log_ServerVIEW_H__DB20F73D_B580_46B7_B9F0_4C5CE4A608B2__INCLUDED_)
#define AFX_SS_Log_ServerVIEW_H__DB20F73D_B580_46B7_B9F0_4C5CE4A608B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// our columns in the ListCtrl
typedef enum SSLWColumn
{
    SSLW_COLUMN_ENTRY,
    SSLW_COLUMN_LEVEL,
    SSLW_COLUMN_MESSAGE,
    SSLW_COLUMN_TIMEDATE,
    SSLW_COLUMN_FILE,
    SSLW_COLUMN_LINE,
    SSLW_COLUMN_COUNT // this MUST be the last item in the list
};


class SS_Log_ServerApp;
class SSFilterSettings;
class SS_FilterSettingsDlg;


class SS_Log_ServerView : public CListView
{
protected: // create from serialization only
	SS_Log_ServerView();
	DECLARE_DYNCREATE(SS_Log_ServerView)

// Attributes
public:
	SS_Log_ServerDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SS_Log_ServerView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void Serialize(CArchive& ar);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnDraw(CDC* pDC);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~SS_Log_ServerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(SS_Log_ServerView)
	afx_msg void OnInsertItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditFilterSettings();
	afx_msg void OnViewRefresh();
	afx_msg void OnEditListviewFilter();
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChanging(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

    VOID EraseLog();

    LPVOID MyAllocMem( DWORD cb);
    LPDEVMODE ChangeDevMode(HWND hWnd, char * pDevice);

    BOOL m_bInitialized;

protected:

    VOID RetreiveMessage(TCHAR* szText, int nItem, int nSize);
	VOID RefreshView();
    VOID AddMessage(LOGMESSAGE* pMsg);
	VOID BeginUpdate();
	VOID FinishUpdate();
	BOOL MessagePassesFilter(LOGMESSAGE* pMsg);
    VOID SetRegistryKey();

    LPDEVMODE lpDevMode;

private:

	SSFilterSettings*	m_pFilterSettings;
	SS_FilterSettingsDlg*	m_pFilterSettingsDlg;
    BOOL                m_bProcessingInOutMatches;

};

#ifndef _DEBUG  // debug version in SS_Log_ServerView.cpp
inline SS_Log_ServerDoc* SS_Log_ServerView::GetDocument()
   { return (SS_Log_ServerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SS_Log_ServerVIEW_H__DB20F73D_B580_46B7_B9F0_4C5CE4A608B2__INCLUDED_)
