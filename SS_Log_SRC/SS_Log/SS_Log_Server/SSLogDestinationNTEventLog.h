// ----------------------------------------------------------------------- //
//
//  FILENAME:	SSLogDestinationNTEventLog.h
//  AUTHOR:		Steve Schaneville
//  CREATED:	15 Feb 2003, 18:25
//
//  PURPOSE:	
//
//  Copyright (c) 2003
//
// ----------------------------------------------------------------------- //
#ifndef __SSLogDestinationNTEventLog_h__
#define __SSLogDestinationNTEventLog_h__

// ------------------[       Pre-Include Defines       ]------------------ //
// ------------------[          Include Files          ]------------------ //
#include "SSLogDestination.h"

// ------------------[          Macros/Defines         ]------------------ //
// ------------------[      Constants/Enumerations     ]------------------ //
// ------------------[       Forward Declarations      ]------------------ //
// ------------------[         Global Variables        ]------------------ //
// ------------------[         Global Functions        ]------------------ //
// ------------------[             Classes             ]------------------ //

// ----------------------------------------------------------------------- //
//  Class:			SSLogDestinationNTEventLog
//  Author:			Steve Schaneville
//  Notes:			
// ----------------------------------------------------------------------- //
class SSLogDestinationNTEventLog : public SSLogDestination
{
public:

    // construction, destruction
	SSLogDestinationNTEventLog						();
	virtual ~SSLogDestinationNTEventLog				();

    // assignment, copy
	SSLogDestinationNTEventLog		    		    (SSLogDestinationNTEventLog& rhs);
	SSLogDestinationNTEventLog&		operator =		(SSLogDestinationNTEventLog& rhs);

protected:

    // initialization
    virtual VOID					InitObject		();

public:

	// accessor functions
    LPCTSTR             EventLogSource				() const;
    VOID                EventLogSource				(LPCTSTR szApplicationName);
    const HANDLE        EventSourceHandle			() const;
    VOID                EventSourceHandle			(const HANDLE hEventSource);
	
	// overrides
    virtual BOOL		OnWriteMessage				(LOGMESSAGE* pMsg);
    virtual BOOL		OnEraseLog					(LOGMESSAGE* pMsg);
    virtual BOOL		OnSetDestinationName		(LOGMESSAGE* pMsg);
    virtual BOOL		OnShutDownServer			(LOGMESSAGE* pMsg);
	
    // utilities

protected:

	BOOL				RegisterEventSourceSS		();
	BOOL                WriteToEventLog				(LOGMESSAGE* pMsg);
    BOOL                GetUserSid                  (PSID* ppUserSid);

private:

	TCHAR		m_szEventLogSource[MAX_PATH];	// name of the application for logging to the event log
    HANDLE      m_hEventSource;					// handle to the NT Event Log 

};


// ----------------------------------------------------------------------- //
//  SSLogDestinationNTEventLog Inline Functions
// ----------------------------------------------------------------------- //

// return the log window's name
inline LPCTSTR SSLogDestinationNTEventLog::EventLogSource() const
{ return m_szEventLogSource; }

// set the log window's name
inline VOID SSLogDestinationNTEventLog::EventLogSource(LPCTSTR szApplicationName)
{ 
    ASSERT(szApplicationName); 
    ASSERT(_tcslen(szApplicationName) < MAX_PATH); 
    _tcscpy(m_szEventLogSource, szApplicationName); 
}

// return the handle to the NT Event Log
inline const HANDLE SSLogDestinationNTEventLog::EventSourceHandle() const
{ return m_hEventSource; }

// set the handle to the NT Event Log
inline VOID SSLogDestinationNTEventLog::EventSourceHandle(const HANDLE hEventSource)
{ m_hEventSource = hEventSource; }


#endif // __SSLogDestinationNTEventLog_h__
