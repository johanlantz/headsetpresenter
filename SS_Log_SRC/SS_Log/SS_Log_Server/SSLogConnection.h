// ----------------------------------------------------------------------- //
//
//  FILENAME:	sslogconnection.h
//  AUTHOR:		Steve Schaneville
//  CREATED:	13 Feb 2003, 10:39
//
//  PURPOSE:
//
//  Copyright (c) 2003
//  Version 3.02
//
// ----------------------------------------------------------------------- //
#ifndef __sslogconnection_h__
#define __sslogconnection_h__

// ------------------[       Pre-Include Defines       ]------------------ //
// ------------------[          Include Files          ]------------------ //
// ------------------[          Macros/Defines         ]------------------ //
// ------------------[      Constants/Enumerations     ]------------------ //
// ------------------[       Forward Declarations      ]------------------ //
class SSLogOutput;

// ------------------[         Global Variables        ]------------------ //
// ------------------[         Global Functions        ]------------------ //
DWORD WINAPI PipeThread( LPVOID pParam );

// ------------------[             Classes             ]------------------ //

// ----------------------------------------------------------------------- //
//  Class:			SSLogConnection
//  Author:			Steve Schaneville
//  Notes:
// ----------------------------------------------------------------------- //
class SSLogConnection
{
public:

    // construction, destruction
	SSLogConnection                                 (SSLogOutput* pOutput);
	virtual ~SSLogConnection();

    // assignment, copy
	SSLogConnection			       		            (SSLogConnection& rhs);
	SSLogConnection&		operator =	            (SSLogConnection& rhs);

protected:

    // initialization
    virtual VOID	        InitObject		        ();

public:

	// accessor functions
    const LPCTSTR           ConnectionName          () const;
    VOID                    ConnectionName          (const LPCTSTR szConnectionName);
    SSLogOutput*            Output                  ();
    VOID                    Output					(SSLogOutput* pOutput);

    // utilities
    BOOL                    Listen                  ();
    BOOL                    CreateListeningThread   ();

protected:

    // utilities
    DWORD                   GetNextMessageID        ();
    DWORD                   GetNextProcessingID     ();
    BOOL                    DispatchMessage         (LOGMESSAGE* pMsg);
    INT                     AddConnection           ();
    INT                     ReleaseConnection       ();
    INT                     CurrentConnectionCount  ();
    BOOL                    HandleServerSpecificMessages (LOGMESSAGE* pMsg);

    // accessors
    const CWinThread*       ListeningThread         () const;
    VOID                    ListeningThread         (CWinThread* pThread);
    BOOL                    ShutdownSignaled        ();
    VOID                    ShutdownSignaled        (BOOL bShutdown);

private:

    TCHAR                   m_szConnectionName[MAX_PATH];

    CWinThread*             m_pListeningThread;
    SSLogOutput*            m_pOutput;

    DWORD                   m_nNextProcessingID;
    DWORD                   m_nNextMessageID;

    INT                     m_nCurrentConnections;
    BOOL                    m_bShutdownSignaled;

};


// ----------------------------------------------------------------------- //
//  SSLogConnection Inline Functions
// ----------------------------------------------------------------------- //

inline const CWinThread* SSLogConnection::ListeningThread() const
{ return m_pListeningThread; }

inline VOID SSLogConnection::ListeningThread(CWinThread* pThread)
{ m_pListeningThread = pThread; }

inline const LPCTSTR SSLogConnection::ConnectionName() const
{ return m_szConnectionName; }

inline VOID SSLogConnection::ConnectionName(const LPCTSTR szConnectionName)
{ ASSERT(szConnectionName); _tcscpy(m_szConnectionName, szConnectionName); }

inline VOID SSLogConnection::Output(SSLogOutput* pOutput)
{ m_pOutput = pOutput; }

inline SSLogOutput* SSLogConnection::Output()
{ return m_pOutput; }

inline DWORD SSLogConnection::GetNextMessageID()
{ DWORD dwID = m_nNextMessageID; m_nNextMessageID++; return dwID; }

inline DWORD SSLogConnection::GetNextProcessingID()
{ DWORD dwID = m_nNextProcessingID; m_nNextProcessingID++; return dwID; }

inline INT SSLogConnection::AddConnection()
{ m_nCurrentConnections++; return m_nCurrentConnections; }

inline INT SSLogConnection::ReleaseConnection()
{
    if(m_nCurrentConnections > 0)
        m_nCurrentConnections--;
    return m_nCurrentConnections;
}

inline INT SSLogConnection::CurrentConnectionCount()
{ return m_nCurrentConnections; }

inline BOOL SSLogConnection::ShutdownSignaled()
{ return m_bShutdownSignaled; }

inline VOID SSLogConnection::ShutdownSignaled(BOOL bShutdown)
{ m_bShutdownSignaled = bShutdown; }


#endif // __sslogconnection_h__
