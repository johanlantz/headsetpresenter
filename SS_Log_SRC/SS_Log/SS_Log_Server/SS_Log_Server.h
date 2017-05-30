// SS_Log_Server.h : main header file for the SS_Log_Server application
//

#if !defined(AFX_SS_Log_Server_H__64FC0947_3265_44B2_8B43_6FF5A16BF80F__INCLUDED_)
#define AFX_SS_Log_Server_H__64FC0947_3265_44B2_8B43_6FF5A16BF80F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// SS_Log_ServerApp:
// See SS_Log_Server.cpp for the implementation of this class
//

class SSCommandLineInfo;


class SS_Log_ServerApp : public CWinApp
{
public:
	SS_Log_ServerApp();
    ~SS_Log_ServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SS_Log_ServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(SS_Log_ServerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

    SSCommandLineInfo* CommandLineInfo();

protected:

    VOID InitObject();
    VOID CommandLineInfo(SSCommandLineInfo* pCLI);

private:

    SSCommandLineInfo*  m_pCommandLineInfo;

};


/////////////////////////////////////////////////////////////////////////////
//

inline SSCommandLineInfo*SS_Log_ServerApp::CommandLineInfo()
{ return m_pCommandLineInfo; }

inline VOID SS_Log_ServerApp::CommandLineInfo(SSCommandLineInfo* pCLI)
{ m_pCommandLineInfo = pCLI; }


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SS_Log_Server_H__64FC0947_3265_44B2_8B43_6FF5A16BF80F__INCLUDED_)
