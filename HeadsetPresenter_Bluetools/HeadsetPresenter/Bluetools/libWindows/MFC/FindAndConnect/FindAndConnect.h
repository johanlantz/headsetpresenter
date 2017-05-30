// FindAndConnect.h : main header file for the FINDANDCONNECT application
//

#if !defined(AFX_FINDANDCONNECT_H__627F91DF_40BA_423E_B6B2_E89AD75C3A27__INCLUDED_)
#define AFX_FINDANDCONNECT_H__627F91DF_40BA_423E_B6B2_E89AD75C3A27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFindAndConnectApp:
// See FindAndConnect.cpp for the implementation of this class
//

class CFindAndConnectApp : public CWinApp
{
public:
	CFindAndConnectApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindAndConnectApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFindAndConnectApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDANDCONNECT_H__627F91DF_40BA_423E_B6B2_E89AD75C3A27__INCLUDED_)
