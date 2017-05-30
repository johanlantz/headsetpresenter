// ----------------------------------------------------------------------- //
//
//  FILENAME:	sslogoutput.cpp
//  AUTHOR:		Steve Schaneville
//  CREATED:	13 Feb 2003, 12:44
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
#include "sslogdestination.h"
#include "SSLogDestinationFile.h"
#include "SSLogDestinationWindow.h"
#include "SSLogDestinationNTEventLog.h"


// ------------------[      Macros/Constants/Types     ]------------------ //
// ------------------[         Global Variables        ]------------------ //
SSLogOutput* g_pOutput = NULL;

// ------------------[         Global Functions        ]------------------ //
UINT OutputThread( LPVOID pParam )
{
	UNREFERENCED_PARAMETER(pParam);
    UINT nr = TRUE;

    if( g_pOutput )
        g_pOutput->DispatchMessages();

    return nr;
}

VOID CALLBACK TimerProcOutput(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	UNREFERENCED_PARAMETER(hwnd);
	UNREFERENCED_PARAMETER(uMsg);
	UNREFERENCED_PARAMETER(idEvent);
	UNREFERENCED_PARAMETER(dwTime);

    // create the pipe thread
    AFX_THREADPROC pfnThreadProc = OutputThread;
    CWinThread* pThread = AfxBeginThread(pfnThreadProc, NULL);
    if( pThread == NULL )
        ASSERT(!"AfxBeginThread failed on the Timer Output thread!");
}

// ------------------[    Class Function Definitions   ]------------------ //

// Standard Constructor
SSLogOutput::SSLogOutput()
{
    InitObject();
}

// Standard Destructor
SSLogOutput::~SSLogOutput()
{
    DeleteCriticalSection(&m_cs);

	// delete our messages
	MsgListIT itMsg = MessageList().Beginning();
	while( itMsg.IsValid() )
	{
		delete itMsg.Current();
		MessageList().RemoveCurrent(itMsg);
        itMsg = MessageList().Beginning();
	}

    // delete our destinations
    DestListIT itDest = DestinationList().Beginning();
    while( itDest.IsValid() )
    {
        delete itDest.Current();
        DestinationList().RemoveCurrent(itDest);
        itDest = DestinationList().Beginning();
    }
}

// Copy Constructor
SSLogOutput::SSLogOutput( SSLogOutput& rhs )
{
    InitObject();
    *this = rhs;
}

// Assignment Operator
SSLogOutput& SSLogOutput::operator = ( SSLogOutput& rhs )
{
    InitObject();

	Dirty(rhs.Dirty());
	TimerID(rhs.TimerID());
	m_DestinationList = rhs.DestinationList();
	m_MessageList = rhs.MessageList();
	CurrentDispatchID(rhs.CurrentDispatchID());
    SetMainWindow(rhs.MainWindow());

    return *this;
}

// Object Initialization
VOID SSLogOutput::InitObject()
{
    InitializeCriticalSection(&m_cs);
    m_bDirty = FALSE;
    g_pOutput = this;
    TimerID(0);
    CurrentDispatchID(1);
    SetMainWindow(NULL);
	UsingWindow(FALSE);
	CurrentlyOutputting(FALSE);

	InitializeDestinations();
}

// ----------------------------------------------------------------------- //
//  Function:		SSLogOutput::CreateDispatchTimer
//  Author:			Steve Schaneville
//  Parameters:		none
//  Return Values:	TRUE if successful, FALSE otherwise
//  Notes:			This function will create the thread
// ----------------------------------------------------------------------- //
BOOL SSLogOutput::CreateDispatchTimer()
{
    BOOL br = TRUE;

    // Begin the timer
    UINT nTimer = ::SetTimer(NULL, 0, 5000, TimerProcOutput);
    if( nTimer != 0 )
        TimerID(nTimer);
    else
    {
        ASSERT(!"Setting the timer failed");
        br = FALSE;
    }

    return br;
}

BOOL SSLogOutput::DispatchMessages()
{
    BOOL br = TRUE;
	BOOL bWillProcessMsg = TRUE;
	BOOL bServerShuttingDown = FALSE;

    if( Dirty() && !CurrentlyOutputting())
    {
		CurrentlyOutputting(TRUE);

		EnterCriticalSection(&m_cs);
        MsgListIT itMsg = MessageList().End();
		Dirty(FALSE);
		LeaveCriticalSection(&m_cs);

		DestListIT itDest = DestinationList().Beginning();
		while( itDest.IsValid() )
        {
            itDest->OnBeginUpdate();
            ++itDest;
        }

        // create a loop through our un-dispatched messages
		INT nDispatchedCount = 0;
        while( itMsg->ProcessingID() != CurrentDispatchID() )
            --itMsg;
        while( itMsg.IsValid() )
        {
            // create a loop through our destinations
            DestListIT itDest = DestinationList().Beginning();
            while( itDest.IsValid() )
            {
				// process messages that will go to all destinations
				// regardless of filters
				switch( itMsg->MessageType() )
				{
				case MSGTYPE_SET_DESTINATION_NAME:
					itDest->OnSetDestinationName( itMsg );
					break;

				case MSGTYPE_LIMIT_OUTPUT_SIZE:
					itDest->OnLimitOutputSize( itMsg );
					break;

				case MSGTYPE_SET_REGISTRY_KEY:
					itDest->OnSetRegistryKey( itMsg );
					break;

                case MSGTYPE_REGISTRY_FILTERS_CHANGED:
                    itDest->OnRegistryFiltersChanged( itMsg );
                    break;

				default:
					break;
				}

				// process messages that will go only to destinations
				// that will be accepted based on filters.
				bWillProcessMsg = itDest->WillProcessMessage( itMsg );
				if( bWillProcessMsg )
				{
					switch( itMsg->MessageType() )
					{
					case MSGTYPE_LOG_MESSAGE:
						if( (itMsg->Flags() & LOGTYPE_LOGTOWINDOW) &&
                            (itDest->DestinationType() == LOGTYPE_LOGTOWINDOW) )
							UsingWindow(TRUE);
						itDest->OnWriteMessage( itMsg );
						break;

					case MSGTYPE_ERASE_LOG:
						itDest->OnEraseLog( itMsg );
						break;

					default:
						break;
					}
				}

                // increment the destination
                ++itDest;
            }

			// if our message was MSGTYPE_SHUTDOWN_SERVER, keep track of that!
			if( itMsg->MessageType() == MSGTYPE_SHUTDOWN_SERVER )
				bServerShuttingDown = TRUE;

            // increment the dispatch message
			EnterCriticalSection(&m_cs);
            ++itMsg;
			LeaveCriticalSection(&m_cs);
			nDispatchedCount++;
            CurrentDispatchID( CurrentDispatchID()+1 ); // m_nCurrentDispatchID += 1;

            // this next line fixes a crash when, after we set the flag to
            // not dirty at the top of this function, we would then process all
            // messages in the queue.  If the incoming thread packed more messages
            // into the queue and set the flag back to dirty while we were still
            // processing, we would process the new messages too, but never
            // set the flag back to non-dirty.  Then, on the next attempt to
            // dispatch the messages, we would have a dirty flag but no messages
            // to process.  This fixes that problem by setting the flag to
            // non-dirty at the end of the message processing routine.
            Dirty(FALSE);
        }

		// send the OnFinishUpdate message
		itDest = DestinationList().Beginning();
		while( itDest.IsValid() )
		{
			itDest->OnFinishUpdate();
			++itDest;
		}

		// send the OnShutdownServer message if shutdown message was received
		if( bServerShuttingDown == TRUE )
		{
			itDest = DestinationList().Beginning();
			while( itDest.IsValid() )
			{
				itDest->OnShutDownServer();
				++itDest;
			}
		}

		// remove the sent messages from the list.
		EnterCriticalSection(&m_cs);
		itMsg = MessageList().Beginning();
		for( INT i=0; i<nDispatchedCount; i++ )
		{
		    delete itMsg.Current();
			MessageList().RemoveCurrent(itMsg);
			itMsg = MessageList().Beginning();

		}
		LeaveCriticalSection(&m_cs);

		CurrentlyOutputting(FALSE);
    }

	// shutdown the server if shutdown message was received
	if( bServerShuttingDown == TRUE && UsingWindow() == FALSE )
		MainWindow()->PostMessage(WM_CLOSE);
	else if( bServerShuttingDown == TRUE && UsingWindow() == TRUE )
	{
		// change the log window name
		// . Our pipe will die due to its ServerShutdowm message, but
		//   the window will remain open
		// . We change the window name so that if this one is still open
		//   when the logging program is run next, a new one with the
		//   same name will be created and a fresh pipe will too.
		CString csWindow;
		MainWindow()->GetWindowText(csWindow);
		csWindow += _T(" -- Pipe Closed");
		MainWindow()->SetWindowText(csWindow);
	}

    return br;
}

BOOL SSLogOutput::PushMessage(LOGMESSAGE* pMsg)
{
    BOOL br = TRUE;

	EnterCriticalSection(&m_cs);
    MessageList().InsertEnd(pMsg);
    Dirty(TRUE);
	LeaveCriticalSection(&m_cs);

    return br;
}

/*
BOOL SSLogOutput::InitializeDestinations(LOGMESSAGE* pMsg)
{
	BOOL br = TRUE;

	DWORD dwDestNeeded = pMsg->Filter() & LOGTYPEGROUP_DESTINATIONS;

	// determine which destinations we have previously created
	DWORD dwDestCurrent = 0;
	DestListIT itDest = DestinationList().Beginning();
	while( itDest.IsValid() )
	{
		dwDestCurrent |= itDest->DestinationType();
		++itDest;
	}

	// now see if we have all necessary destinations
	if( dwDestCurrent != dwDestNeeded )
	{
		// create our missing destinations
		INT nCount = SSLogDestination::GetFiltersCount();
		for( INT i=0; i<nCount; i++ )
		{
			DESTINATION_TYPE nType = (DESTINATION_TYPE)(g_Filters[i].nType & LOGTYPEGROUP_DESTINATIONS);
			if( nType & dwDestNeeded )
			{
				if( !(nType & dwDestCurrent) )
				{
					CreateDestination(nType);
				}
			}
		}
	}

	return br;
}
*/

BOOL SSLogOutput::InitializeDestinations()
{
	BOOL br = TRUE;

	INT nCount = GetFiltersCount();
	for( INT i=0; i<nCount; i++ )
	{
		DESTINATION_TYPE nType = (DESTINATION_TYPE)(g_Filters[i].nType & LOGTYPEGROUP_DESTINATIONS);
		if( nType != NULL )
		{
			CreateDestination(nType);
		}
	}

	return br;
}

BOOL SSLogOutput::CreateDestination(DESTINATION_TYPE nType)
{
	BOOL br = TRUE;

	switch( nType )
	{
	    case LOGTYPE_LOGTOWINDOW:
			DestinationList().InsertEnd(new SSLogDestinationWindow);
			break;

	    case LOGTYPE_LOGTOFILE:
			DestinationList().InsertEnd(new SSLogDestinationFile);
			break;

		case LOGTYPE_LOGTOEVENTLOG:
			DestinationList().InsertEnd(new SSLogDestinationNTEventLog);
			break;

	    default:
			// You should add the creation of any new destination type
			// in a new "case" statement here.  Derive your new destination
			// class from SSLogDestination.
			ASSERT(!"Add your LOGTYPE_MYDESTINATION flag here, and create an instance!");
		break;
	}

	return br;
}
