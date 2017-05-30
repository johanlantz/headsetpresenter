// SSCommandLineInfo.cpp: implementation of the SSCommandLineInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SSCommandLineInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SSCommandLineInfo::SSCommandLineInfo()
{
    _tcscpy(m_szServerName, _T(""));
}

SSCommandLineInfo::~SSCommandLineInfo()
{
}

void SSCommandLineInfo::ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast )
{
	UNREFERENCED_PARAMETER(bFlag);
	UNREFERENCED_PARAMETER(bLast);

    //CCommandLineInfo::ParseParam( lpszParam, bFlag, bLast );
    if( _tcscmp(m_szServerName, _T("")) )
        _tcscat(m_szServerName, _T(" "));

    _tcscat(m_szServerName, lpszParam);
}

