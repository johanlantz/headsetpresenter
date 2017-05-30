// ----------------------------------------------------------------------- //
//
//  FILENAME:	sslogconnection.cpp
//  AUTHOR:		Steve Schaneville
//  CREATED:	13 Feb 2003, 10:39
//
//  PURPOSE:
//
//  Copyright (c) 2003
//  Version 3.02
//
// ----------------------------------------------------------------------- //

// ------------------[       Pre-Include Defines       ]------------------ //
// ------------------[          Include Files          ]------------------ //
#include "stdafx.h"
#include "sslogoutput.h"
#include "sslogconnection.h"
#include "SS_String.h"

// ------------------[      Macros/Constants/Types     ]------------------ //
// ------------------[         Global Variables        ]------------------ //
// ------------------[         Global Functions        ]------------------ //

// ----------------------------------------------------------------------- //
//  Function:		PipeThread()
//  Author:			Steve Schaneville
//  Parameters:		pParam [in] - a pointer to the SSLogConnection instance.
//  Return Values:	(see MSDN)
//  Notes:			This function creates a named pipe.  The name is based
//                  on the name of the app's window, which was set by
//                  the SS_Log class (so it is possible to have multiple
//                  pipes, and hence, multiple separate logs).  The function
//                  then waits for a connection to the pipe from a client.
//                  On connection, we check the message to see if it is an
//      OLD,            erase command (szText == "~~WMSS_LOG_ERASE_LOG~~") and
//      WILL            if so, erase the log window.  Assume that if it is not
//      UPDATE            an erase message, then it is a log message, so send the
//      LATER            message to the SS_Log_ServerView::WriteLog() function.
//                  Finally, loop back and wait for another message.
// ----------------------------------------------------------------------- //
DWORD WINAPI PipeThread( LPVOID pParam )
{
    UINT nr = TRUE;

    SSLogConnection* pConnection = (SSLogConnection*)pParam;
    if( pConnection )
        pConnection->Listen();
    else
    {
        ASSERT(FALSE);
        nr = FALSE;
    }

    return nr;
}

// ------------------[    Class Function Definitions   ]------------------ //

// Standard Constructor
SSLogConnection::SSLogConnection(SSLogOutput* pOutput)
{
    InitObject();
    Output( pOutput );
}

// Standard Destructor
SSLogConnection::~SSLogConnection()
{
}

// Copy Constructor
SSLogConnection::SSLogConnection( SSLogConnection& rhs )
{
    InitObject();
    *this = rhs;
}

// Assignment Operator
SSLogConnection& SSLogConnection::operator = ( SSLogConnection& rhs )
{
    InitObject();

	ConnectionName(rhs.ConnectionName());

    return *this;
}

// Object Initialization
VOID SSLogConnection::InitObject()
{
    ShutdownSignaled(FALSE);
    ListeningThread(NULL);
    ConnectionName(_T(""));
    Output(NULL);
    m_nNextMessageID = 1;
    m_nNextProcessingID = 1;
    m_nCurrentConnections = 0;
}

BOOL SSLogConnection::CreateListeningThread()
{
    BOOL br = TRUE;

    // create the pipe thread
    DWORD dwThreadID;
    HANDLE hThread = CreateThread(NULL, 0, PipeThread,
        (LPVOID)this, 0, &dwThreadID);
    if( hThread == NULL || hThread == INVALID_HANDLE_VALUE )
    {
        ASSERT(!"Could not create listening thread... call GetLastError here!");
        br = FALSE;
    }
    else
        CloseHandle(hThread);

    return br;
}

// ----------------------------------------------------------------------- //
//  Function:		SSLogConnection::Listen
//  Author:			Steve Schaneville
//  Parameters:		none
//  Return Values:	TRUE if successful, FALSE otherwise.
//  Notes:
// ----------------------------------------------------------------------- //
BOOL SSLogConnection::Listen()
{
    BOOL bResult = FALSE;

    // let's create our named pipe with no restrictions on access.
    SECURITY_DESCRIPTOR sd;
    bResult = InitializeSecurityDescriptor( &sd, SECURITY_DESCRIPTOR_REVISION );
    sd.Dacl = NULL;
    SECURITY_ATTRIBUTES sa;
    sa.bInheritHandle = true;
    sa.lpSecurityDescriptor = &sd;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);

    BOOL bDaclPresent = TRUE;
    BOOL bDaclDefaulted = FALSE;
    bResult = SetSecurityDescriptorDacl( &sd, bDaclPresent,
        (PACL)NULL,
        bDaclDefaulted );

    TCHAR szPipeName[MAX_PATH];
	// wait until ConnectionName() has been filled out before continuing
	// this is due to the fact that this thread is created before the
	// main startup thread has entered idle
	while( !_tcscmp(ConnectionName(), _T("")) )
		Sleep(10);
    sprintf( szPipeName, _T("\\\\.\\pipe\\%s"), ConnectionName() );
    HANDLE hPipe = CreateNamedPipe( szPipeName,
        PIPE_ACCESS_INBOUND,
        PIPE_TYPE_MESSAGE|PIPE_WAIT|PIPE_READMODE_MESSAGE,
        PIPE_UNLIMITED_INSTANCES,
        SSLOG_MAX_MESSAGE_LENGTH,
        0, 5000, &sa );
    if( hPipe == NULL || hPipe == INVALID_HANDLE_VALUE )
        bResult = FALSE;

    // now we loop forever, waiting for messages.  As they come in, send them
    // to the SS_Log_ServerView::EraseLog() and SS_Log_ServerView::WriteLog()
    // functions as appropriate.
    DWORD dwBytesRead;
	LOGMESSAGE* pMsg = NULL;
	SS_String sMessage;
	LPTSTR szMessage;
    while( bResult == TRUE && ShutdownSignaled() == FALSE )
    {
		::ConnectNamedPipe( hPipe, NULL );
		szMessage = sMessage.GetBuffer(SSLOG_MAX_MESSAGE_LENGTH);
        ::ReadFile( hPipe, szMessage, SSLOG_MAX_MESSAGE_LENGTH,
            &dwBytesRead, NULL );
        ::DisconnectNamedPipe( hPipe );
		sMessage.ReleaseBuffer();

        if( dwBytesRead != 0 )
        {
			pMsg = new LOGMESSAGE;

            // package the message
			pMsg->MessageType( (MSGTYPE)_ttoi(sMessage.FirstToken(_T("\t"))) );
            pMsg->DateTime( sMessage.NextToken() );
            pMsg->Filename( sMessage.NextToken() );
            pMsg->LineNumber( _ttoi(sMessage.NextToken()) );
            pMsg->Flags( _ttoi(sMessage.NextToken()) );
            pMsg->Message( sMessage.NextToken() );
            pMsg->ProcessingID( GetNextProcessingID() );
            if( pMsg->MessageType() == MSGTYPE_LOG_MESSAGE )
                pMsg->MessageID( GetNextMessageID() );
            else
                pMsg->MessageID( 0 );

            // dispatch the message
            bResult = DispatchMessage(pMsg); // do we want to cancel loop if this fails?
            bResult = HandleServerSpecificMessages(pMsg);

			if( bResult == TRUE)
				pMsg = NULL;
			else
				delete pMsg;
        }
    }

    CloseHandle(hPipe);

    return bResult;
}

BOOL SSLogConnection::DispatchMessage(LOGMESSAGE* pMsg)
{
    ASSERT(pMsg);
    BOOL br = TRUE;

    Output()->PushMessage(pMsg);

    return br;
}

BOOL SSLogConnection::HandleServerSpecificMessages(LOGMESSAGE* pMsg)
{
    BOOL br = TRUE;
    INT nConnectionCount = 0;
    LOGMESSAGE* pMsgShutDown = NULL;

    switch( pMsg->MessageType() )
    {
    case MSGTYPE_RELEASE_CONNECTION:
        nConnectionCount = ReleaseConnection();
        if( nConnectionCount == 0 )
        {
            ShutdownSignaled(TRUE);
            pMsgShutDown = new LOGMESSAGE;
            pMsgShutDown->MessageType(MSGTYPE_SHUTDOWN_SERVER);
            DispatchMessage(pMsgShutDown);
        }
        break;

    case MSGTYPE_ADD_CONNECTION:
        AddConnection();
        break;

    case MSGTYPE_SHUTDOWN_SERVER:
        ShutdownSignaled(TRUE);
        pMsgShutDown = new LOGMESSAGE;
        pMsgShutDown->MessageType(MSGTYPE_SHUTDOWN_SERVER);
        DispatchMessage(pMsgShutDown);
        break;

    default:
        break;
    }

    return br;
}