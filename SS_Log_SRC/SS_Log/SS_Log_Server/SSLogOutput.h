// ----------------------------------------------------------------------- //
//
//  FILENAME:	sslogoutput.h
//  AUTHOR:		Steve Schaneville
//  CREATED:	13 Feb 2003, 12:44
//
//  PURPOSE:	
//
//  Copyright (c) 2003
//
// ----------------------------------------------------------------------- //
#ifndef __sslogoutput_h__
#define __sslogoutput_h__

// ------------------[       Pre-Include Defines       ]------------------ //
// ------------------[          Include Files          ]------------------ //
// ------------------[       Forward Declarations      ]------------------ //
class SSLogDestination;

// ------------------[          Macros/Defines         ]------------------ //
typedef SS_List<SSLogDestination*>              DestList;
typedef SS_List<SSLogDestination*>::Iterator    DestListIT;

// ------------------[      Constants/Enumerations     ]------------------ //
// ------------------[         Global Variables        ]------------------ //
// ------------------[         Global Functions        ]------------------ //
VOID CALLBACK TimerProcOutput(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);

// ------------------[             Classes             ]------------------ //

// ----------------------------------------------------------------------- //
//  Class:			SSLogOutput
//  Author:			Steve Schaneville
//  Notes:			
// ----------------------------------------------------------------------- //
class SSLogOutput
{
public:

    // construction, destruction
	SSLogOutput                                     ();
	virtual ~SSLogOutput                            ();

    // assignment, copy
	SSLogOutput				    		            (SSLogOutput& rhs);

protected:

    // initialization
    virtual VOID	        InitObject		        ();

public:

	// accessor functions
    BOOL                    Dirty                   ();
    VOID                    Dirty                   (BOOL bDirty);
    UINT                    TimerID                 ();
    VOID                    TimerID                 (UINT nTimerID);
    VOID                    SetMainWindow			(CWnd* pWnd);

    DestList&               DestinationList         ();
    MsgList&                MessageList             ();

    // utilities
    BOOL                    CreateDispatchTimer     ();
    BOOL                    DispatchMessages        ();
    BOOL                    PushMessage             (LOGMESSAGE* pMsg);

protected:

    // accessor functions
    INT                     CurrentDispatchID       ();
    VOID                    CurrentDispatchID       (INT nCurrentDispatchID);
    BOOL                    UsingWindow				();
    VOID                    UsingWindow				(BOOL bUsingWindow);
    CWnd*                   MainWindow				();

	// utilities
	BOOL					InitializeDestinations	();
	BOOL					CreateDestination		(DESTINATION_TYPE nType);

    BOOL                    CurrentlyOutputting		();
    VOID                    CurrentlyOutputting		(BOOL bCurrentlyOutputting);


private:

	// make sure this operator is not called... our implementation is useless.
	SSLogOutput&			operator =		        (SSLogOutput& rhs);

	// member variables 
    BOOL				m_bDirty;
    UINT				m_nTimerID;
    BOOL				m_bUsingWindow;

    DestList			m_DestinationList;
    MsgList				m_MessageList;

    INT					m_nCurrentDispatchID;

    CWnd*				m_pMainWnd;
    CRITICAL_SECTION    m_cs;

	BOOL				m_bCurrentlyOutputting; // ;)

};


// ----------------------------------------------------------------------- //
//  SSLogOutput Inline Functions
// ----------------------------------------------------------------------- //

inline BOOL SSLogOutput::Dirty()
{ return m_bDirty; }

inline VOID SSLogOutput::Dirty(BOOL bDirty)
{ InterlockedExchange( (LPLONG)&m_bDirty, (LONG)bDirty ); }

inline UINT SSLogOutput::TimerID()
{ return m_nTimerID; }

inline VOID SSLogOutput::TimerID(UINT nTimerID)
{ m_nTimerID = nTimerID; }

inline DestList& SSLogOutput::DestinationList()
{ return m_DestinationList; }

inline MsgList& SSLogOutput::MessageList()
{ return m_MessageList; }

inline INT SSLogOutput::CurrentDispatchID()
{ return m_nCurrentDispatchID; }

inline VOID SSLogOutput::CurrentDispatchID(INT nCurrentDispatchID)
{ m_nCurrentDispatchID = nCurrentDispatchID; }

inline CWnd* SSLogOutput::MainWindow()
{ return m_pMainWnd; }

inline VOID SSLogOutput::SetMainWindow(CWnd* pWnd)
{ m_pMainWnd = pWnd; }

inline BOOL SSLogOutput::UsingWindow()
{ return m_bUsingWindow; }

inline VOID SSLogOutput::UsingWindow(BOOL bUsingWindow)
{ m_bUsingWindow = bUsingWindow; }

inline BOOL SSLogOutput::CurrentlyOutputting()
{ return m_bCurrentlyOutputting; }

inline VOID SSLogOutput::CurrentlyOutputting(BOOL bCurrentlyOutputting)
{ m_bCurrentlyOutputting = bCurrentlyOutputting; }


#endif // __sslogoutput_h__
