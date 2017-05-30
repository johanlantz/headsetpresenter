// ----------------------------------------------------------------------- //
//
//  FILENAME:	sslogdestination.h
//  AUTHOR:		Steve Schaneville
//  CREATED:	13 Feb 2003, 13:00
//
//  PURPOSE:
//
//  Copyright (c) 2003
//  Version 3.02
//
// ----------------------------------------------------------------------- //
#ifndef __sslogdestination_h__
#define __sslogdestination_h__

// ------------------[       Pre-Include Defines       ]------------------ //
// ------------------[          Include Files          ]------------------ //
// ------------------[          Macros/Defines         ]------------------ //
// ------------------[      Constants/Enumerations     ]------------------ //
// ------------------[       Forward Declarations      ]------------------ //
// ------------------[         Global Variables        ]------------------ //
extern SSLogFilter g_Filters[];

// ------------------[         Global Functions        ]------------------ //
// ------------------[             Classes             ]------------------ //

// ----------------------------------------------------------------------- //
//  Class:			SSLogDestination
//  Author:			Steve Schaneville
//  Notes:
// ----------------------------------------------------------------------- //
class SSLogDestination
{
public:

    // construction, destruction
	SSLogDestination                                (DESTINATION_TYPE nType);
	virtual ~SSLogDestination                       ();

    // assignment, copy
	SSLogDestination			    		        (SSLogDestination& rhs);
	SSLogDestination&		operator =		        (SSLogDestination& rhs);

protected:

    // initialization
    virtual VOID	        InitObject		        ();

public:

	// accessor functions
    const DWORD				RegFilter				() const;
    VOID					RegFilter				(const DWORD dwRegFilter);
    LPCTSTR					RegKey					() const;
    VOID					RegKey					(LPCTSTR szRegistryKey);
    const DWORD				DefaultFlags			() const;
    VOID					DefaultFlags			(const DWORD dwFlags);
    DESTINATION_TYPE        DestinationType         ();
    VOID                    DestinationType         (DESTINATION_TYPE nType);

    // required overrides
    virtual BOOL            OnWriteMessage          (LOGMESSAGE* pMsg) = 0;

	// optional overrides
    virtual BOOL            OnSetDestinationName    (LOGMESSAGE* pMsg);
    virtual BOOL            OnEraseLog              (LOGMESSAGE* pMsg);
    virtual BOOL			OnLimitOutputSize		(LOGMESSAGE* pMsg);
    virtual BOOL            OnSetRegistryKey		(LOGMESSAGE* pMsg);
    virtual BOOL            OnShutDownServer        ();
	virtual BOOL			OnBeginUpdate			();
	virtual BOOL			OnFinishUpdate			();
    virtual BOOL			OnRegistryFiltersChanged(LOGMESSAGE* pMsg);
    virtual BOOL            WillProcessMessage      (LOGMESSAGE* pMsg);

protected:

    LONG                    GetFilterFromReg        ();

private:

    DESTINATION_TYPE    m_nDestinationType;
    DWORD               m_dwRegFilter;			// the currently specified reg filter
    TCHAR				m_szRegKey[MAX_PATH];	// regkey sub path (start from HKEY_CURRENT_USER\Software\)
    DWORD               m_dwDefaultFlags;		// the currently specified reg filter

};


// ----------------------------------------------------------------------- //
//  SSLogDestination Inline Functions
// ----------------------------------------------------------------------- //

inline DESTINATION_TYPE SSLogDestination::DestinationType()
{ return m_nDestinationType; }

inline VOID SSLogDestination::DestinationType(DESTINATION_TYPE nType)
{ m_nDestinationType = nType; }

inline const DWORD SSLogDestination::RegFilter() const
{ return m_dwRegFilter; }

inline VOID SSLogDestination::RegFilter(const DWORD dwFilter)
{ m_dwRegFilter = dwFilter; }

inline const DWORD SSLogDestination::DefaultFlags() const
{ return m_dwDefaultFlags; }

inline VOID SSLogDestination::DefaultFlags(const DWORD dwFlags)
{ m_dwDefaultFlags = dwFlags; }

inline LPCTSTR SSLogDestination::RegKey() const
{ return (LPCTSTR)m_szRegKey; }

inline VOID SSLogDestination::RegKey(LPCTSTR szRegKey)
{
	ASSERT(szRegKey);
	SS_String sKey;
	sKey = SSLOG_REG_HIVE_SUBKEY;
	sKey ^= szRegKey;
	_tcscpy(m_szRegKey, sKey);
	GetFilterFromReg();
}


#endif // __sslogdestination_h__
