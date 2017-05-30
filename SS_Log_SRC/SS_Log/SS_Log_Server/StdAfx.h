// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A5CCF122_D435_4AED_89FB_0AEA29CC171E__INCLUDED_)
#define AFX_STDAFX_H__A5CCF122_D435_4AED_89FB_0AEA29CC171E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "SS_List.h"
#include "SS_Log_Defines.h"
#include "SS_String.h"

// helper class for passing message info
//
class LOGMESSAGE
{
public:
	
    LOGMESSAGE() 
    { 
        m_nMessageID = 0; 
        m_nProcessingID = 0; 
        m_dwFlags = 0;
        m_szMessageA = NULL; 
        m_szDateTimeA = NULL;
        m_szFilenameA = NULL; 
        m_nLineNumber = 0;
        m_nMessageType = MSGTYPE_LOG_MESSAGE;
    }
	
    ~LOGMESSAGE() 
    { 
        delete [] m_szMessageA;   m_szMessageA = NULL;
        delete [] m_szDateTimeA;  m_szDateTimeA = NULL;
        delete [] m_szFilenameA;  m_szFilenameA = NULL;
    }
	
	LOGMESSAGE(LOGMESSAGE& rhs)
	{
		*this = rhs;
	}

	LOGMESSAGE& operator = (LOGMESSAGE& rhs)
	{
		MessageID(rhs.MessageID());
		ProcessingID(rhs.ProcessingID());
		Flags(rhs.Flags());
		Message(rhs.Message());
		DateTime(rhs.DateTime());
		Filename(rhs.Filename());
		LineNumber(rhs.LineNumber());
		MessageType(rhs.MessageType());

		return *this;
	}

    // accessors
    INT     MessageID	()              { return m_nMessageID; }
    VOID    MessageID	(INT nID)       { m_nMessageID = nID; }
    INT     ProcessingID()              { return m_nProcessingID; }
    VOID    ProcessingID(INT nID)       { m_nProcessingID = nID; }
    DWORD   Flags		()              { return m_dwFlags; }
    VOID    Flags		(DWORD nFlags)  { m_dwFlags = nFlags; }
    LPSTR   Message		()              { return m_szMessageA; }
    VOID    Message		(LPSTR szMsg)   { m_szMessageA = ssAllocString(szMsg); }
    LPSTR   DateTime	()              { return m_szDateTimeA; }
    VOID    DateTime	(LPSTR szDT)    { m_szDateTimeA = ssAllocString(szDT); }
    LPSTR   Filename	()              { return m_szFilenameA; }
    VOID    Filename	(LPSTR szFile)  { m_szFilenameA = ssAllocString(szFile); }
    INT     LineNumber	()				{ return m_nLineNumber; }
    VOID    LineNumber	(INT nLine)		{ m_nLineNumber = nLine; }
    MSGTYPE MessageType	()				{ return m_nMessageType; }
    VOID    MessageType	(MSGTYPE nType) { m_nMessageType = nType; }
	
private:
	
    LPSTR ssAllocString(LPSTR szString)
    {
		LPSTR szNewString;
		if( szString != NULL )
		{
			INT nLen = strlen(szString);
			INT nNewLen = 4;
			while( nNewLen <= nLen )
				nNewLen *= 2;
			szNewString = new char[nNewLen];
			strcpy(szNewString, szString);
		}
		else
		{
			szNewString = new char[256];
			strcpy(szNewString, _T(""));
		}
        return szNewString;
    }
	
    INT         m_nMessageID;
    INT         m_nProcessingID;
    DWORD       m_dwFlags;
    LPSTR       m_szMessageA;
    LPSTR       m_szDateTimeA;
    LPSTR       m_szFilenameA;
    INT         m_nLineNumber;
    MSGTYPE     m_nMessageType;
	
};

// lists 
typedef SS_List<LOGMESSAGE*>                    MsgList;
typedef SS_List<LOGMESSAGE*>::Iterator          MsgListIT;



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A5CCF122_D435_4AED_89FB_0AEA29CC171E__INCLUDED_)
