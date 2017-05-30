// BSProxy.h : main header file for the BSProxy DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CBSProxyApp
// See BSProxy.cpp for the implementation of this class
//

class CBSProxyApp : public CWinApp
{
public:
	CBSProxyApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
