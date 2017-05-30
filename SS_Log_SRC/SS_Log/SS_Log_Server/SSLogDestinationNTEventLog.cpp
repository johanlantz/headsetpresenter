// ----------------------------------------------------------------------- //
//
//  FILENAME:	SSLogDestinationNTEventLog.cpp
//  AUTHOR:		Steve Schaneville
//  CREATED:	15 Feb 2003, 18:24
//
//  PURPOSE:	
//
//  Copyright (c) 2003
//
// ----------------------------------------------------------------------- //

// ------------------[       Pre-Include Defines       ]------------------ //
// ------------------[          Include Files          ]------------------ //
#include "stdafx.h"
#include "SSLogDestinationNTEventLog.h"
#include "SS_RegistryKey.h"
#include "EventLogMsgs.h"
#include "SS_String.h"

// ------------------[      Macros/Constants/Types     ]------------------ //
// ------------------[         Global Variables        ]------------------ //
// ------------------[         Global Functions        ]------------------ //
// ------------------[    Class Function Definitions   ]------------------ //

// Standard Constructor
SSLogDestinationNTEventLog::SSLogDestinationNTEventLog() :
	SSLogDestination(LOGTYPE_LOGTOEVENTLOG)
{
    InitObject();
}

// Standard Destructor
SSLogDestinationNTEventLog::~SSLogDestinationNTEventLog()
{
    ::DeregisterEventSource(EventSourceHandle());
}

// Copy Constructor
SSLogDestinationNTEventLog::SSLogDestinationNTEventLog( SSLogDestinationNTEventLog& rhs ) :
	SSLogDestination(rhs.DestinationType())
{
    InitObject();
    *this = rhs;
}

// Assignment Operator
SSLogDestinationNTEventLog& SSLogDestinationNTEventLog::operator = ( SSLogDestinationNTEventLog& rhs )
{
    InitObject();

	EventLogSource(rhs.EventLogSource());
	EventSourceHandle(rhs.EventSourceHandle());

    return *this;
}

// Object Initialization
VOID SSLogDestinationNTEventLog::InitObject()
{
    EventLogSource(SSLOG_DEFAULT_DESTINATION_NAME);
    EventSourceHandle(NULL);
}

// ----------------------------------------------------------------------- //
//  Function:		SS_Log::RegisterEventSourceSS
//  Author:			Steve Schaneville
//  Parameters:
//  Return Values:
//  Notes:			The trailing "SS" on the end of the name is just so
//                  that this function call has a different name than
//                  the API call (less confusion that way).
// ----------------------------------------------------------------------- //
BOOL SSLogDestinationNTEventLog::RegisterEventSourceSS()
{
    BOOL br = TRUE;
	
    // determine if the NT event log has the proper registry entries for our
    // currently set EventLogSource.
    TCHAR szKey[MAX_PATH];
    _tcscpy( szKey, SSLOG_EVENT_LOG_SOURCE_REG );
    _tcscat( szKey, EventLogSource() );
    SS_RegistryKey sKey(HKEY_LOCAL_MACHINE);
    LONG lr = sKey.OpenKey( szKey );
    if( lr != ERROR_SUCCESS )
    {
        // the event log source did not exist in the registry, so create it.
        DWORD dwSupportedTypes = 
			EVENTLOG_ERROR_TYPE			|
			EVENTLOG_WARNING_TYPE		|
			EVENTLOG_INFORMATION_TYPE	|
			EVENTLOG_AUDIT_SUCCESS		|
			EVENTLOG_AUDIT_FAILURE;
		
        TCHAR szModuleName[MAX_PATH];
        _tcscpy(szModuleName, _T("SS_Log_Server.exe"));
		
        lr = sKey.CreateKey(HKEY_LOCAL_MACHINE, szKey);
        if( lr == ERROR_SUCCESS )
            lr = sKey.SetValue(_T("EventMessageFile"), szModuleName);
        if( lr == ERROR_SUCCESS )
            lr = sKey.SetValue(_T("SupportedTypes"), dwSupportedTypes);
    }
	
    if( lr == ERROR_SUCCESS )
    {
        HANDLE hEventSource = ::RegisterEventSource(NULL, EventLogSource());
        if( hEventSource != NULL )
            EventSourceHandle( hEventSource );
        else
            br = FALSE;
    }
    else
        br = FALSE;
	
    return br;
}

// ----------------------------------------------------------------------- //
//  Function:		SSLogDestinationNTEventLog::WriteToEventLog
//  Author:			Steve Schaneville
//  Parameters:		szBufferA [in] - the buffer to be written to the NT
//                                   system Log.  This works for Win9x
//                                   too.
//  Return Values:	none
//  Notes:
// ----------------------------------------------------------------------- //
BOOL SSLogDestinationNTEventLog::WriteToEventLog(LOGMESSAGE* pMsg)
{
    BOOL br = TRUE;
	
    if( !EventSourceHandle() )
        br = RegisterEventSourceSS();
	
    if( br == TRUE )
    {
        // get our user name information
        PSID pSid = NULL;
		BOOL brSID;

        brSID = GetUserSid(&pSid);

        // map our event type to the NT log's event types
        WORD wType = EVENTLOG_INFORMATION_TYPE;
        if( pMsg->Flags() & LOGTYPE_CRITICAL )
            wType = EVENTLOG_ERROR_TYPE;
        else if( pMsg->Flags() & LOGTYPE_WARNING )
            wType = EVENTLOG_WARNING_TYPE;
        else if( pMsg->Flags() & LOGTYPE_NORMAL )
            wType = EVENTLOG_INFORMATION_TYPE;
        else if( pMsg->Flags() & LOGTYPE_TRACE )
            wType = EVENTLOG_AUDIT_SUCCESS;
        else if( pMsg->Flags() & LOGTYPE_TRACE_FAIL )
            wType = EVENTLOG_AUDIT_FAILURE;
		
        // send the message to the NT event log
        LPCSTR pszBufferListA = pMsg->Message();
        LPCSTR* ppBufferListA = &pszBufferListA;
        br = ::ReportEventA( 
            EventSourceHandle(),    // event log source handle
			wType,                  // event type to log
			0,                      // event category
			SSLOG_BLANK_STRING,     // event identifier
			pSid,                   // user security identifier (optional)
			1,                      // number of strings to merge with message
			0,                      // size of binary data, in bytes
			ppBufferListA,          // array of strings to merge with message
			NULL                    // address of binary data
            );

        if( brSID == TRUE )
            FreeSid(pSid);
    }
	
    return br;
}

//
//  FUNCTION: GetUserSid
//
//  PURPOSE: Obtains a pointer to the SID for the current user
//
//  PARAMETERS:
//	none
//
//  RETURN VALUE:
//	Pointer to the SID
//
//  COMMENTS:
//	The SID buffer returned by this function is allocated with
//	HeapAlloc and should be freed with HeapFree.
//
BOOL SSLogDestinationNTEventLog::GetUserSid(PSID* ppUserSid)
{
    BOOL br = TRUE;
    HANDLE hToken;
    BOOL bRes;
    DWORD cbBuffer;
    DWORD cbRequired = 0;
    PTOKEN_USER pUserInfo = NULL;
    
    // The User's SID can be obtained from the process token
    //
    bRes = OpenProcessToken(GetCurrentProcess(),
        TOKEN_QUERY,
        &hToken);
    if (FALSE == bRes)
        br = FALSE;
    
    // Set buffer size to 0 for first call to determine
    // the size of buffer we need.
    //
    cbBuffer = 0;
    if( br == TRUE )
        bRes = GetTokenInformation(hToken,
            TokenUser,
            NULL,
            cbBuffer,
            &cbRequired);
    if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) 
        br = FALSE;
    
    // Allocate a buffer for our token user data
    //
    cbBuffer = cbRequired;
    if( br == TRUE )
        pUserInfo = (PTOKEN_USER) HeapAlloc(GetProcessHeap(), 0, cbBuffer);
    if (NULL == pUserInfo)
        br = FALSE;
    
    // Make the "real" call
    //
    if( br == TRUE )
        bRes = GetTokenInformation(hToken,
            TokenUser,
            pUserInfo,
            cbBuffer,
            &cbRequired);
    if (FALSE == bRes)
        br = FALSE;
    
    // Make another copy of the SID for the return value
    //
    if( br == TRUE )
        cbBuffer = GetLengthSid(pUserInfo->User.Sid);
    
    if( br == TRUE )
        *ppUserSid = (PSID) HeapAlloc(GetProcessHeap(), 0, cbBuffer);
    if (NULL == *ppUserSid)
        br = FALSE;
    
    if( br == TRUE )
        bRes = CopySid(cbBuffer, *ppUserSid, pUserInfo->User.Sid);
    if (FALSE == bRes)
        br = FALSE;
    
    if( br == TRUE )
        bRes = HeapFree(GetProcessHeap(), 0, pUserInfo);
    if (FALSE == bRes)
        br = FALSE;
    
    return br;
}

BOOL SSLogDestinationNTEventLog::OnWriteMessage(LOGMESSAGE* pMsg)
{
	UNREFERENCED_PARAMETER(pMsg);
	BOOL br = TRUE;

    br = WriteToEventLog(pMsg);

	return br;
}

BOOL SSLogDestinationNTEventLog::OnEraseLog(LOGMESSAGE* pMsg)
{
	UNREFERENCED_PARAMETER(pMsg);
	BOOL br = TRUE;
	
	return br;
}

BOOL SSLogDestinationNTEventLog::OnSetDestinationName(LOGMESSAGE* pMsg)
{
	UNREFERENCED_PARAMETER(pMsg);
	BOOL br = TRUE;

    ::DeregisterEventSource(EventSourceHandle());
    EventLogSource(pMsg->Message());
    RegisterEventSourceSS();
    
	return br;
}

BOOL SSLogDestinationNTEventLog::OnShutDownServer(LOGMESSAGE* pMsg)
{
	UNREFERENCED_PARAMETER(pMsg);
	BOOL br = TRUE;
	
	return br;
}
