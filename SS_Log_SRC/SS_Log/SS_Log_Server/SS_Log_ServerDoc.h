// SS_Log_ServerDoc.h : interface of the SS_Log_ServerDoc class
// Version 3.02
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SS_Log_ServerDOC_H__C6834C2A_D036_42BA_AE0E_679A719E60E3__INCLUDED_)
#define AFX_SS_Log_ServerDOC_H__C6834C2A_D036_42BA_AE0E_679A719E60E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


typedef enum UpdateMessage
{
	SSWM_BEGIN_UPDATE		= (WM_USER + 100),
	SSWM_ADD_MESSAGE,
	SSWM_FINISH_UPDATE,
	SSWM_ERASE_LOG,
    SSWM_SET_REGISTRY_KEY,
    SSWM_REGISTRY_FILTERS_CHANGED,
	SSWM_REFRESH_VIEW
};


class SSLogConnection;
class SSLogOutput;


class SS_Log_ServerDoc : public CDocument
{
protected: // create from serialization only
	SS_Log_ServerDoc();
    DECLARE_DYNCREATE(SS_Log_ServerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SS_Log_ServerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~SS_Log_ServerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(SS_Log_ServerDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

    // accessors
    VOID                AttachConnection            (SSLogConnection* pConnection);
    SSLogConnection*	Connection					();
    VOID                AttachOutput				(SSLogOutput* pOutput);
    SSLogOutput*		Output						();
	MsgList&			MessageList					();
	VOID				RegKey						(LPCTSTR szRegKey);
	LPCTSTR				RegKey						();

	// utilities
	VOID				BeginUpdate					();
	VOID				AddMessage					(LOGMESSAGE* pMsg);
	VOID				FinishUpdate				();
	VOID				EraseLog					();
	VOID				SetRegistryKey				(LOGMESSAGE* pMsg);
    VOID				RegistryFiltersChanged		(LOGMESSAGE* pMsg);

protected:

    VOID                InitObject                  ();
    SS_String           MakeTabDelimitedMessage     (LOGMESSAGE* pMsg);
    BOOL                ParseTabDelimitedMessage    (SS_String sMsg,
                                                     LOGMESSAGE* pMsg);
    VOID                RefreshAllViews             ();
    VOID                SendMessageToAllViews       (UpdateMessage nMessage,
                                                     WPARAM wParam = 0,
                                                     LPARAM lParam = 0);

private:

    SSLogConnection*    m_pConnection;
    SSLogOutput*		m_pOutput;
	TCHAR				m_szRegKey[MAX_PATH];
    MsgList				m_MessageList;

};

/////////////////////////////////////////////////////////////////////////////
//

inline VOID SS_Log_ServerDoc::AttachConnection(SSLogConnection* pConnection)
{ m_pConnection = pConnection; }

inline SSLogConnection* SS_Log_ServerDoc::Connection()
{ return m_pConnection; }

inline VOID SS_Log_ServerDoc::AttachOutput(SSLogOutput* pOutput)
{ m_pOutput = pOutput; }

inline SSLogOutput* SS_Log_ServerDoc::Output()
{ return m_pOutput; }

inline VOID SS_Log_ServerDoc::RegKey(LPCTSTR szRegKey)
{ ASSERT(szRegKey); _tcscpy(m_szRegKey, szRegKey); }

inline LPCTSTR SS_Log_ServerDoc::RegKey()
{ return m_szRegKey; }

inline MsgList& SS_Log_ServerDoc::MessageList()
{ return m_MessageList; }



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.




#endif // !defined(AFX_SS_Log_ServerDOC_H__C6834C2A_D036_42BA_AE0E_679A719E60E3__INCLUDED_)
