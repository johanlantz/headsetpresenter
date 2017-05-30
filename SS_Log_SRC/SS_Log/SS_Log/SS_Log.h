// ----------------------------------------------------------------------- //
///
/// @file       SS_Log.h
/// @author     Steve Schaneville
/// @date       23 Oct 2001, 16:23
/// @brief      Header file for the SS_Log class.
///
/// This is the only file you need to include in your projects to
/// use the SS_Log features.  All macros, the SS_Log class, and any
/// other definitions needed are either in this file or included
/// by this file.
///
/// Copyright (c) 2001 - 2003\n
/// z_web_master@yahoo.com\n
/// http://schaneville.webhop.net/SS_Utils
/// Version 3.02
///
// ----------------------------------------------------------------------- //
#ifndef __SS_Log__
#define __SS_Log__

// ------------------[       Pre-Include Defines       ]------------------ //
// ------------------[          Include Files          ]------------------ //
#include <wtypes.h>
#include <assert.h>
#include <tchar.h>

#include "SS_Log_Defines.h"

// ------------------[      Macros/Constants/Types     ]------------------ //

/// For internal use only.  The CREATION_STATUSes are used to keep track of
/// whether or not we tried to start the SS_Log_Server process for receiving
/// messages, and what the result of the creation attempt was.
typedef enum SSLogCreationStatus
{
    SSLOG_CS_CREATE_NOT_YET_ATTEMPTED,  ///< SS_Log_Server.exe has not been started.
    SSLOG_CS_CREATE_SUCCEEDED,          ///< SS_Log_Server.exe was successfully started.
    SSLOG_CS_CREATE_FAILED,             ///< Startup of SS_Log_Server.exe failed.
    SSLOG_CS_COUNT // THIS MUST BE THE LAST ITEM

} CREATION_STATUS;

// ------------------[       Forward Declarations      ]------------------ //

class SS_Log;
class SSLogEnterFunction;

// ------------------[         Global Variables        ]------------------ //
// ------------------[         Global Functions        ]------------------ //
// ------------------[             Classes             ]------------------ //

// ----------------------------------------------------------------------- //
/// @class		SSLogEnterFunction
/// @author		Steve Schaneville
/// @brief		Helper class used for tracking when a function was entered
///             and exited.
///
/// The idea behind this class is simple: Create an instance and send
/// a logged message in the constructor.  Then, send another logged message
/// in the destructor.  This way, the user can place a stack instance at
/// the beginning of any function and the log will tell us when the function
/// was entered and exited.
///
/// @note It is not intended for the user to use this class directly.
/// Instead, use the LogEnterFunction and LocalLogEnterFunction macros.
// ----------------------------------------------------------------------- //
class SSLogEnterFunction
{
public:

    SSLogEnterFunction(TCHAR* szFile, int nLine, SS_Log* pLog, LPCTSTR szFunctionName);
    virtual ~SSLogEnterFunction();

    LPTSTR m_szFunctionName;
    SS_Log* m_pLog;
    INT m_nLocalID;
    INT m_nLineNumber;
    LPTSTR m_szFilename;
    static INT m_nIDCount;
};


// ----------------------------------------------------------------------- //
/// @class		SS_Log
/// @author		Steve Schaneville
/// @brief		This class that helps us keep track of our logging parameters
///             and handles sending messages to the SS_Log_Server process,
///             including startup and shutdown messages.
///
///
// ----------------------------------------------------------------------- //
class SS_Log
{
public:

    /// @{ @name Construction, Destruction, Assignment, Copy, Initialization
    SS_Log                                  (LPCTSTR szServerName);
    SS_Log                                  (LPCTSTR szServerName,
                                             LPCTSTR szRegKey,
                                             LPCTSTR szDestinationName = SSLOG_DEFAULT_DESTINATION_NAME,
                                             DWORD dwOutputSizeLimitKB = SSLOG_MAX_LOG_SIZE_DEFAULT,
                                             BOOL bUseServer = TRUE);
    ~SS_Log                                 ();

	SS_Log							        (SS_Log& rhs);

	SS_Log&			    operator =		    (SS_Log& rhs);

protected:

    VOID			    InitObject		    ();
    /// @}

public:

    /// @{ @name Flag Manipulation
    const DWORD         DefaultFlags		() const;
    const DWORD         DefaultFlags        (DWORD dwFlags);
    const DWORD         GetFlags			();
    const DWORD         SetFlags			(const DWORD nType);
    const DWORD         AddFlags			(const DWORD nType);
    const DWORD         RemoveFlags			(const DWORD nType);
    BOOL                SetRegistryFilters  (DWORD dwRegFilters);
    DWORD               GetRegistryFilters  ();
    /// @}

    /// @{ @name Error Handling
    INT					GetLastError		() const;
    VOID				SetLastError        (INT nLastError);
    BOOL				Succeeded			();
    /// @}

    /// @{ @name Process Instance Accessors
    HINSTANCE           InstanceHandle      ();
    VOID                InstanceHandle      (HINSTANCE hInst);
    /// @}

    VOID				UseServer			(BOOL bUseServer);
    BOOL				UseServer			();

    /// @{ @name Message and Log Manipulation
    INT                 WriteLog            (MSGTYPE nType, TCHAR* szFile,
                                             int nLine, DWORD dwFilter,
                                             const TCHAR* pMsg, va_list* args);
    INT                 WriteLogID          (TCHAR* szFile, int nLine,
                                             DWORD dwFilter, UINT nResourceID,
                                             va_list* args);
    INT                 EraseLog            ();
	INT 				SetDestinationName	(LPCTSTR szName);
    INT                 LimitOutputSize     (const DWORD dwSizeLimit);
	INT					SetRegistryKey		(LPCTSTR szRegKey);
	 VOID                LoggingDisabled     (BOOL bLoggingDisabled);  //Johan this was protected before
    /// @}

protected:

    /// @{ @name Internal Accessors
    HANDLE				FileHandle			();
    VOID                FileHandle			(const HANDLE hFileHandle);
    const CREATION_STATUS CreateServerStatus () const;
    VOID                CreateServerStatus  (const CREATION_STATUS nStatus);
    LPCTSTR             ServerName          () const;
    VOID                ServerName          (LPCTSTR szServerName);
    LPCTSTR             DestinationName     () const;
    VOID                DestinationName     (LPCTSTR szDestinationName);
    LPCTSTR             RegistryKey         () const;
    VOID                RegistryKey         (LPCTSTR szRegistryKey);
    INT					LogCount			() const;
    VOID				LogCount			(INT nLogCount);
    //VOID                LoggingDisabled     (BOOL bLoggingDisabled);
    BOOL                LoggingDisabled     ();
    const BOOL          ConnectionAdded		() const;
    VOID                ConnectionAdded		(const BOOL bConnectionAdded);
    DWORD				MaxFileSizeKb		();
    VOID				MaxFileSizeKb		(DWORD dwMaxFileSizeKb);
    /// @}

    /// @{ @name Internal Utilities
    BOOL                WriteToServer       (LPCSTR szMessageA);
    BOOL                OpenLogServer       ();
    INT                 WriteToFile         (LPCSTR szBufferA);
    INT                 SendSSLogMessage    (MSGTYPE nType, LPCTSTR szMessage);
    INT                 AddConnection       ();
    INT                 ReleaseConnection   ();
    BOOL                ReadRegDefaultSettings();
    VOID				SetFlagsInternal    (LPDWORD pdwFilter);
    /// @}

private:

    DWORD               m_dwFlags;			// the filter used when not otherwise specified
    LPTSTR              m_szServerName;     // the name of the server to write to
    LPTSTR              m_szDestinationName;     // the name of the server to write to
    LPTSTR              m_szRegistryKey;     // the name of the server to write to
    CRITICAL_SECTION    m_cs;           // so we can avoid threading issues
    CREATION_STATUS 	m_nCreateServerStatus;  // don't try to create log server every time if it fails first time
    HINSTANCE           m_hInstance;        // application instance
    BOOL                m_bConnectionAdded;
	INT					m_nLastError;
	BOOL				m_bUseServer;
	HANDLE				m_hFile;
	INT					m_nLogCount;
	DWORD				m_dwMaxFileSizeKb;
    BOOL                m_bLoggingDisabled;

public:

    /// @{ @name Deprecated Functions
    /// The following functions have been DEPRECATED.  Please don't use them
	/// any more, as they may not be supported in the future.

    VOID	Filename		(LPCTSTR szName)	{ SetDestinationName(szName); } ///< @deprecated Maps to SetDestinationName
	VOID	WindowName		(LPCTSTR szName)	{ SetDestinationName(szName); } ///< @deprecated Maps to SetDestinationName
	VOID	Filter			(DWORD dwFlags)		{ SetFlags(dwFlags); }          ///< @deprecated Maps to SetFlags
	VOID	AddFilters		(DWORD dwFlags)		{ AddFlags(dwFlags); }          ///< @deprecated Maps to AddFlags
	VOID	RemoveFilters	(DWORD dwFlags)		{ RemoveFlags(dwFlags); }       ///< @deprecated Maps to RemoveFlags
	DWORD	Filter			()					{ return GetFlags(); }          ///< @deprecated Maps to GetFlags

	/// @}
};


// ----------------------------------------------------------------------- //
//  SS_Log Inline Functions
// ----------------------------------------------------------------------- //

/// Returns the current set of flags that will be passed with all messages
/// by default.
inline const DWORD SS_Log::DefaultFlags() const
{ return m_dwFlags; }

/// Sets the list of flags that will be passed with all messages
/// by default.
inline const DWORD SS_Log::DefaultFlags(DWORD dwFlags)
{ DWORD dwOldFlags = m_dwFlags; SetFlagsInternal(&dwFlags); m_dwFlags = dwFlags; return dwOldFlags; }

/// Returns the current set of flags that will be passed with all messages
/// by default.  Identical to the DefaultFlags function.
inline const DWORD SS_Log::GetFlags ()
{ return DefaultFlags(); }

/// Sets the list of flags that will be passed with all messages
/// by default.  Identical to the DefaultFlags function.
inline const DWORD SS_Log::SetFlags (const DWORD nType)
{ return DefaultFlags(nType); }

/// Adds the flags specified in nType to the current list of flags that will
/// be passed with all messages by default.
inline const DWORD SS_Log::AddFlags (const DWORD nType)
{ DWORD dwOld = DefaultFlags(); DefaultFlags(DefaultFlags() | nType); return dwOld; }

/// Removes the flags specified in nType to the current list of flags that
/// will be passed with all messages by default.
inline const DWORD SS_Log::RemoveFlags (const DWORD nType)
{ DWORD dwOld = DefaultFlags(); m_dwFlags = DefaultFlags() & ~nType; return dwOld; }

/// Returns a null-terminated string containing the name of the server we
/// are currently sending our messages to.
inline LPCTSTR SS_Log::ServerName() const
{ return m_szServerName; }

/// Sets the name of the server we are going to send our messages to.
inline VOID SS_Log::ServerName(LPCTSTR szServerName)
{ ASSERT(szServerName); _tcscpy(m_szServerName, szServerName); }

/// Returns a null-terminated string containing the current destination name.
/// See SS_Log::DestinationName(LPCTSTR szDestinationName) for more details.
inline LPCTSTR SS_Log::DestinationName() const
{ return m_szDestinationName; }

/// Sets the name of the log destination.  The DestinationName affects each
/// log destination type differently.  Current destination types are as
/// follows:
///
/// -# LOGTYPE_LOGTOFILE (file) - sets the filename of the log file to be
///      written out to disk.
/// -# LOGTYPE_LOGTOWINDOW (window) - no affect.
/// -# LOGTYPE_LOGTONTEVENTLOG (NT Event Log) - sets the text that will appear
///      in the "source" column in the Windows Event Viewer.
inline VOID SS_Log::DestinationName(LPCTSTR szDestinationName)
{ ASSERT(szDestinationName); _tcscpy(m_szDestinationName, szDestinationName); }

/// Returns a null-terminated string containing the current registry key that
/// was passed in by the user.
/// See SS_Log::SetRegistryKey() for more details.
inline LPCTSTR SS_Log::RegistryKey() const
{ return m_szRegistryKey; }

/// Internal accessor function to set the registr key passed in by the
/// user to the SS_Log::SetRegistryKey() function.
inline VOID SS_Log::RegistryKey(LPCTSTR szRegistryKey)
{ ASSERT(szRegistryKey); _tcscpy(m_szRegistryKey, szRegistryKey); }

/// Returns the last error encountered by a logging operation.  See SSLogError
/// for details.
inline INT SS_Log::GetLastError() const
{ return m_nLastError; }

/// Sets the last error encountered by a logging operation.  See SSLogError
/// for details.
inline VOID SS_Log::SetLastError(INT nLastError)
{ m_nLastError = nLastError; }

/// Returns the current log ID that will be written to file if we are not
/// using the server.
inline INT SS_Log::LogCount() const
{ return m_nLogCount; }

/// Sets the current log ID that will be written to file if we are not
/// using the server.
inline VOID SS_Log::LogCount(INT nLogCount)
{ m_nLogCount = nLogCount; }

/// Specifies whether or not we should use the server to log messages.  This
/// has a serious impact on performance, as using the server is many times
/// faster than the alternative to writing straight to a file (which is what
/// happens if we specify not to use the server).  Note also that by not
/// using the server, the user eliminates the possibility to use some of the
/// additional destinations (NT Event Log, Window, and any other future
/// implementations).
inline VOID SS_Log::UseServer(BOOL bUseServer)
{ m_bUseServer = bUseServer; }

/// Return TRUE if we are currently using the server, or FALSE otherwise.
inline BOOL SS_Log::UseServer()
{ return m_bUseServer; }

/// Returns the file handle that is being used to write our log file.  This
/// handle will only be used if we are not using the server (See UseServer).
inline HANDLE SS_Log::FileHandle()
{ return m_hFile; }

/// Sets the file handle that will be used to write our log file.  This
/// handle will only be used when UseServer is true.
inline VOID SS_Log::FileHandle(const HANDLE hFile)
{
    if( hFile == NULL && FileHandle() != NULL )
        CloseHandle(FileHandle());
    m_hFile = hFile;
}

/// Returns the currently set maximum file size.  Returns
/// SSLOG_MAX_LOG_SIZE_NO_LIMIT if no limit is set.
inline DWORD SS_Log::MaxFileSizeKb()
{ return m_dwMaxFileSizeKb; }

/// Sets the maximum file size in Kb that the log file will be allowed to
/// reach.  Once this limit is reached, the oldest half of the log will be
/// erased so we can fit more messages.  This parameter is only used if
/// we are not using the server (See UseServer).  Pass in
/// SSLOG_MAX_LOG_SIZE_NO_LIMIT if you want to specify no limit (the default).
inline VOID SS_Log::MaxFileSizeKb(DWORD dwMaxFileSizeKb)
{
	if( dwMaxFileSizeKb == SSLOG_MAX_LOG_SIZE_NO_LIMIT )
        m_dwMaxFileSizeKb = dwMaxFileSizeKb;
    else
        m_dwMaxFileSizeKb = dwMaxFileSizeKb*1020; // file size in KB on disk
}

/// Returns TRUE if the last message sent succeeded, or FALSE in case of failure.
/// Call GetLastError() for more details.
inline BOOL SS_Log::Succeeded()
{ return GetLastError() == SSLOG_ERROR_NO_ERROR; }

/// Returns the status of the last server creation attempt.
inline const CREATION_STATUS SS_Log::CreateServerStatus() const
{ return m_nCreateServerStatus; }

/// Sets the status of the last server creation attempt.
inline VOID SS_Log::CreateServerStatus(const CREATION_STATUS nStatus)
{ m_nCreateServerStatus = nStatus; }

/// Return the process's instance handle.  Note that this parameter defaults
/// to NULL, and must be passed in by the user if logging by resource ID is
/// desired.
inline HINSTANCE SS_Log::InstanceHandle()
{ return m_hInstance; }

// Sets the process instance handle.  This is necessary if logging by resource
/// ID is desired.
inline VOID SS_Log::InstanceHandle(HINSTANCE hInstance)
{ m_hInstance = hInstance; }

/// Returns TRUE if we've already sent the AddConnection message to the server.
inline const BOOL SS_Log::ConnectionAdded() const
{ return m_bConnectionAdded; }

/// Sets whether or not we've already sent the AddConnection message to the
/// server.
inline VOID SS_Log::ConnectionAdded(const BOOL bConnectionAdded)
{ m_bConnectionAdded = bConnectionAdded; }

/// Internal accessor that returns whether or not we are sending messages out
/// or not.  See SS_Log::LoggingDisabled(BOOL bLoggingDisabled).
inline BOOL SS_Log::LoggingDisabled()
{ return m_bLoggingDisabled; }

/// Internal accessor that sets whether or not we are sending messages out messages
/// or not.  This is set on construction by the "LoggingDisabled" registry entry
/// in HKCU\Software\SS_Log key, and updated when the user calles
/// SS_Log::SetRegistryKey() in the key passed into this function.
inline VOID SS_Log::LoggingDisabled(BOOL bLoggingDisabled)
{ m_bLoggingDisabled = bLoggingDisabled; }

// ------------------[          Macros/Defines         ]------------------ //

// The _SS_LOG_DO_NOT_INCLUDE_MACROS define only exists so that we don't
// include the macros listed below in the SS_Log.cpp class.
#ifndef _SS_LOG_DO_NOT_INCLUDE_MACROS


#ifdef _SS_LOG_ACTIVE

/// @{ section GLF Global Log Functions
/// These functions are used to access the global log. They are the same
/// functions that are available from a local instance of the SS_Log class.
/// Use them as you would use the corresponding functions in SS_Log.

/// @def LogSetFlags
///     Calls SS_Log::SetFlags() for the global log.

/// @def LogGetFlags
///     Calls SS_Log::GetFlags() for the global log.

/// @def LogRemoveFlags
///     Calls SS_Log::RemoveFlags() for the global log.

/// @def LogAddFlags
///     Calls SS_Log::AddFlags() for the global log.

/// @def LogSetDestinationName
///     Calls SS_Log::SetDestinationName() for the global log.

/// @def LogEraseLog
///     Calls SS_Log::EraseLog() for the global log.

/// @def LogLimitOutputSize
///     Calls SS_Log::LimitOutputSize() for the global log.

/// @def LogGetLastError
///     Calls SS_Log::GetLastError() for the global log.

/// @def LogSetRegistryKey
///     Calls SS_Log::SetRegistryKey() for the global log.

/// @def LogUseServer
///     Calls SS_Log::UseServer() for the global log.

/// @def LogSucceeded
///     Calls SS_Log::Succeeded() for the global log.

/// @def LogInstanceHandle
///     Calls SS_Log::LogInstanceHandle() for the global log.

/// @def LogGetRegistryFilters
///     Calls SS_Log::GetRegistryFilters() for the global log.

/// @def LogSetRegistryFilters
///     Calls SS_Log::GetRegistryFilters() for the global log.

#define LogSetFlags				g_Log.SetFlags
#define LogGetFlags				g_Log.GetFlags
#define LogRemoveFlags		    g_Log.RemoveFlags
#define LogAddFlags			    g_Log.AddFlags
#define LogSetDestinationName	g_Log.SetDestinationName
#define LogEraseLog				g_Log.EraseLog
#define LogLimitOutputSize		g_Log.LimitOutputSize
#define LogGetLastError			g_Log.GetLastError
#define LogSetRegistryKey		g_Log.SetRegistryKey
#define LogUseServer			g_Log.UseServer
#define LogSucceeded			g_Log.Succeeded
#define LogInstanceHandle		g_Log.InstanceHandle
#define LogGetRegistryFilters	g_Log.GetRegistryFilters
#define LogSetRegistryFilters	g_Log.SetRegistryFilters
/// @}

#else

#define LogSetFlags				__noop
#define LogGetFlags				__noop
#define LogRemoveFlags  		__noop
#define LogAddFlags			    __noop
#define LogSetDestinationName	__noop
#define LogEraseLog				__noop
#define LogLimitOutputSize		__noop
#define LogGetLastError			__noop
#define LogSetRegistryKey		__noop
#define LogUseServer			__noop
#define LogSucceeded			__noop
#define LogInstanceHandle		__noop
#define LogGetRegistryFilters	__noop
#define LogSetRegistryFilters	__noop

#endif

// deprecated functions have been re-mapped.  Don't use these any more.
#ifdef _SS_LOG_ACTIVE

/// @{ @section DGLF Deprecated Global Log Functions
/// These functions are used to access the global log. They are the same
/// functions that are available from a local instance of the SS_Log class.
/// Use them as you would use the corresponding functions in SS_Log.

/// @def LogFilename
///     Maps to SetDestinationName()

/// @def LogWindowName
///     Maps to SetDestinationName()

/// @def LogRemoveFilters
///     Maps to RemoveFlags()

/// @def LogAddFilters
///     Maps to AddFlags()

/// @def LogFilter
///     Maps to SetFlags()

/// @def LogLimitFileSize
///     Maps to LimitOutputSize()
#define LogDisableLogging		g_Log.LoggingDisabled    //Johan added macro to disable logging
#define LogFilename				g_Log.SetDestinationName
#define LogWindowName			g_Log.SetDestinationName
#define LogRemoveFilters		g_Log.RemoveFlags
#define LogAddFilters			g_Log.AddFlags
#define LogFilter				g_Log.SetFlags
#define LogLimitFileSize		g_Log.LimitOutputSize
/// @}

#else

#define LogFilename				__noop
#define LogWindowName			__noop
#define LogRemoveFilters		__noop
#define LogAddFilters			__noop
#define LogFilter				__noop
#define LogLimitFileSize		__noop

#endif
// end deprecated functions

// ------------------[      Constants/Enumerations     ]------------------ //
// ------------------[       Forward Declarations      ]------------------ //
// ------------------[         Global Variables        ]------------------ //
#ifdef _SS_LOG_ACTIVE
extern SS_Log g_Log;
#endif

// ------------------[         Global Functions        ]------------------ //
#ifdef _SS_LOG_ACTIVE

/// @{ @section PLF Primary Logging Functions
/// These functions are the only ones that will actually produce a message in
/// a destination log (file, window, nt event log, other future destinations).
/// Note, however, that other messages are sent to the logging server for
/// configuration and user options.
///
/// For each of these calls, the global LogStoreFileLine function is called
/// first to store the filename and line number into global variables.  Then
/// one of the global WriteLog functions is called to process the logging.
/// Immediately after being called, the WriteLog functions make an local copy
/// of the filename and line number info so that the global variables can be
/// accessed by other threads without interfering with this message.

/// @def Log
/// @brief This is the function you will call for most all of your logging.
///
/// Four version of the function exist.  They are:
///
/// - Log(TCHAR* pMsg, ...) --- send a message
///       to the global log.  The parameters are:
///     @param pMsg [in] sprintf-style format string for the message.
///     @param ... [in] additional parameters to format into the format string.
///
/// - Log(DWORD dwFilter, TCHAR* pMsg, ...) --- send a message
///       to the global log.  The parameters are:
///     @param dwFilter [in] filter overrides for this message.
///     @param pMsg [in] sprintf-style format string for the message.
///     @param ... [in] additional parameters to format into the format string.
///
/// - Log(SS_Log* pLog, TCHAR* pMsg, ...) --- send a message
///       to a local log.  The parameters are:
///     @param pLog [in] pointer to the local to which we will send the message.
///     @param pMsg [in] sprintf-style format string for the message.
///     @param ... [in] additional parameters to format into the format string.
///
/// - Log(SS_Log* pLog, DWORD dwFilter, TCHAR* pMsg, ...) --- send a message
///       to a local log.  The parameters are:
///     @param pLog [in] pointer to the local to which we will send the message.
///     @param dwFilter [in] filter overrides for this message.
///     @param pMsg [in] sprintf-style format string for the message.
///     @param ... [in] additional parameters to format into the format string.
#define Log     LogStoreFileLine(_T(__FILE__), __LINE__), WriteLog

/// @def LogID
/// @brief Call this function to log a message from a string Resource ID.
///
/// Four version of the function exist.  They are:
///
/// - LogID(TCHAR* pMsg, ...) --- send a message
///       to the global log.  The parameters are:
///     @param pMsg [in] sprintf-style format string for the message.
///     @param ... [in] additional parameters to format into the format string.
///
/// - LogID(DWORD dwFilter, TCHAR* pMsg, ...) --- send a message
///       to the global log.  The parameters are:
///     @param dwFilter [in] filter overrides for this message.
///     @param pMsg [in] sprintf-style format string for the message.
///     @param ... [in] additional parameters to format into the format string.
///
/// - LogID(SS_Log* pLog, TCHAR* pMsg, ...) --- send a message
///       to a local log.  The parameters are:
///     @param pLog [in] pointer to the local to which we will send the message.
///     @param pMsg [in] sprintf-style format string for the message.
///     @param ... [in] additional parameters to format into the format string.
///
/// - LogID(SS_Log* pLog, DWORD dwFilter, TCHAR* pMsg, ...) --- send a message
///       to a local log.  The parameters are:
///     @param pLog [in] pointer to the local to which we will send the message.
///     @param dwFilter [in] filter overrides for this message.
///     @param pMsg [in] sprintf-style format string for the message.
///     @param ... [in] additional parameters to format into the format string.
#define LogID   LogStoreFileLine(_T(__FILE__), __LINE__), WriteLogID

/// @def LocalLogEnterFunction(pLog, szFunctionName)
/// @brief Will send a message upon entering and exiting a function.
///
/// Place this macro at the beginning of a function and pass in the
/// function name.  The macro will send a message to a local log each time the
/// function is called, and again when the function is returned.  Note that the
/// filename and line number info is sent for the entering message, but
/// unfortunately, I've not figured out how to gather that information
/// for the exiting message.
///
/// @param pLog [in] pointer to the local to which we will send the message.
/// @param szFunctionName [in] null-terminated string containing the name of
///     the function we are entering.
#define LocalLogEnterFunction(pLog, szFunctionName) \
    SSLogEnterFunction ssLogEnterFunction(_T(__FILE__), __LINE__, pLog, szFunctionName)

/// @def LogEnterFunction(szFunctionName)
/// @brief Will send a message upon entering and exiting a function.
///
/// Place this macro at the beginning of a function and pass in the
/// function name.  The macro will send a message to the global log each time
/// the function is called, and again when the function is returned.  Note that
/// the filename and line number info is sent for the entering message, but
/// unfortunately, I've not figured out how to gather that information
/// for the exiting message.
///
/// @param szFunctionName [in] name of the function this call was made from within.
#define LogEnterFunction(szFunctionName) \
    SSLogEnterFunction ssLogEnterFunction(_T(__FILE__), __LINE__, &g_Log, szFunctionName)

/// @}

extern INT WriteLog(SS_Log* pLog, DWORD dwFilter, TCHAR* pMsg, va_list* args );
extern INT WriteLog(SS_Log* pLog, DWORD dwFilter, TCHAR* pMsg, ...);
extern INT WriteLog(SS_Log* pLog, TCHAR* pMsg, ...);
extern INT WriteLog(DWORD dwFilter, TCHAR* pMsg, ...);
extern INT WriteLog(TCHAR* pMsg, ...);

extern INT WriteLogID(SS_Log* pLog, DWORD dwFilter, UINT nResourceID, va_list* args );
extern INT WriteLogID(SS_Log* pLog, DWORD dwFilter, UINT nResourceID, ...);
extern INT WriteLogID(SS_Log* pLog, UINT nResourceID, ...);
extern INT WriteLogID(DWORD dwFilter, UINT nResourceID, ...);
extern INT WriteLogID(UINT nResourceID, ...);

extern VOID LogStoreFileLine(TCHAR* szFile, int nLine);

#else // _SS_LOG_ACTIVE

#define Log                 __noop
#define LogEnterFunction    __noop
#define LogID               __noop
#define LogDisableLogging   __noop

#endif // _SS_LOG_ACTIVE

#endif // _SS_LOG_DO_NOT_INCLUDE_MACROS


#endif // __SS_Log__




