// ----------------------------------------------------------------------- //
///
/// @file       SS_Log_Defines.h
/// @author     Steve Schaneville
/// @date       12 Feb 2003, 16:23
/// @brief      Global definition file for the SS_Log class.
///
/// This file and the corresponding cpp file contains everything you need
/// to define your own log levels/types.  There are only 3 steps you need
/// to take to do this.  See the SS_LogFilterType enum for details.
///
/// Copyright (c) 2001 - 2003\n
/// z_web_master@yahoo.com\n
/// http://schaneville.webhop.net/SS_Utils
/// Version 3.02
///
// ----------------------------------------------------------------------- //
#ifndef __ss_log_defines_h__
#define __ss_log_defines_h__

// ------------------[       Pre-Include Defines       ]------------------ //
#ifndef ASSERT
#define ASSERT assert
#endif


/// This enum contains all the defined log levels or types.  You can easily
/// define your own types by following the steps below.
///
/// Steps for Defining a new SS_LogFilterType:
///
/// -# Look for the definition of SS_LogFilterType (defined below as
///    "typedef enum SS_LogFilterType").  Add a LOGTYPE_* entry to the enum list
///    (i.e. "LOGTYPE_XYZ").  Just assign it the next bit available in line,
///    (i.e. "LOGTYPE_XYZ = (1<<9)")
/// -# Add your new filter type to the LOGTYPE_USERS #define just below the
///    SS_LogFilterType enum.  Use the bitwise OR ("|") operator.
/// -# In the SS_Log_Defines.cpp file, find the "SSLogFilter g_Filters[]" object
///    and add an entry to that array.  The entry should include 3 items:
///
///          - the LOGTYPE_XYZ enum value you just added to the SS_LogFilterType
///				enum
///          - a string that will be shown in the Filter column of the log window
///          - a string that will be written into the registry... this registry entry
///				can be used to turn that filter on and off (with a 1 or 0 value)
///
typedef enum SS_LogFilterType
{
    LOGTYPE_DEBUG                   = (1<<0),  //    1 - for debug builds
    LOGTYPE_RELEASE                 = (1<<1),  //    2 - for release builds

    LOGTYPE_LOGTOWINDOW             = (1<<2),  //    4 - write log to screen window
    LOGTYPE_LOGTOFILE               = (1<<3),  //    8 - write log to file
    LOGTYPE_LOGTOEVENTLOG           = (1<<4),  //   16 - write log to the NT system event log (works for Win9x too... see "LogEvent" in MSDN

    LOGTYPE_CRITICAL                = (1<<5),  //   32 - for critical errors
    LOGTYPE_WARNING                 = (1<<6),  //   64 - for warning errors
    LOGTYPE_NORMAL                  = (1<<7),  //  128 - non-error info (everything's ok)
    LOGTYPE_TRACE                   = (1<<8),  //  256 - for low priority info
	LOGTYPE_TRACE_FAIL				= (1<<9),  //  512 - for low priority info, failure situation

    LOGTYPE_INOUT                   = (1<<10), // 1024 - for entering/exiting functions

    // user defined log types... DO NOT EXCEED (1<<31) !!!  And remember to add
    // the types to the LOGTYPE_USERS #define below...
    //
    LOGTYPE_ADD_YOUR_TYPE_HERE      = (1<<11)  // 2048 - example.  This line can be safely removed (adjust your commas)

} SS_LOG_FILTER_TYPE;

/// Filter type groups.  When adding your own type, after adding it to the
/// SS_LogFilterType enum above, add your type to the LOGTYPEGROUP_USERS
/// group below using the bitwize OR operator ("|").
#define LOGTYPEGROUP_USERS              ( LOGTYPE_ADD_YOUR_TYPE_HERE )

#define LOGTYPEGROUP_BUILDS             ( LOGTYPE_DEBUG | LOGTYPE_RELEASE )
#define LOGTYPEGROUP_DESTINATIONS       ( LOGTYPE_LOGTOWINDOW     | \
                                          LOGTYPE_LOGTOFILE       | \
                                          LOGTYPE_LOGTOEVENTLOG )   \

#define LOGTYPEGROUP_LEVELS             ( LOGTYPE_CRITICAL		| \
                                          LOGTYPE_WARNING		| \
                                          LOGTYPE_NORMAL		| \
                                          LOGTYPE_TRACE			| \
										  LOGTYPE_TRACE_FAIL	| \
                                          LOGTYPE_INOUT			| \
                                          LOGTYPEGROUP_USERS )

// define the destination type to simply be one of the filter types
#define DESTINATION_TYPE                SS_LOG_FILTER_TYPE
#define DESTINATIONTYPE_UNDEFINED       ((DESTINATION_TYPE)-1)

// define the registry entries for turning filters on and off
#define SSLOG_REG_DEFAULT_HIVE          (HKEY_CURRENT_USER)
#define SSLOG_REG_HIVE_SUBKEY           (_T("Software"))
#define SSLOG_REG_DEFAULT_APP_KEY		(_T("SS_Log"))
#define SSLOG_REG_DEFAULT_APP_SUBKEY	(_T("SS_Log"))
#define SSLOG_REG_FILTERS_SUBKEY        (_T("Filters"))
#define SSLOG_REG_LISTVIEW_SUBKEY		(_T("ListView"))
#define SSLOG_DEFAULT_LOGSERVER         (_T("Default Log Server"))
#define SSLOG_DEFAULT_DESTINATION_NAME  (_T("SS_Log.log"))

// the maximum message length
#define SSLOG_MAX_MESSAGE_LENGTH 4096

/// These are the errors that can be returned from some of SS_Log's
/// logging functions.
typedef enum SSLogError
{
	SSLOG_ERROR_NO_ERROR				= 0,	///< No Error.
	SSLOG_ERROR_SUCCESS					= SSLOG_ERROR_NO_ERROR, ///< No Error.
	SSLOG_ERROR_CREATE_PIPE_FAILED		= -1,	///< Could not create the named pipe.
	SSLOG_ERROR_LAUNCH_SERVER_FAILED	= -2,	///< Could not launch the SS_Log_Server.exe app.
	SSLOG_ERROR_CONNECT_TO_PIPE_FAILED	= -3,	///< Could not connect to the named pipe.
	SSLOG_ERROR_WRITE_TO_PIPE_FAILED	= -4,	///< Connected to pipe, but write failed.
	SSLOG_ERROR_NOTHING_TO_LOG			= -5,	///< User sent an empty message.
	SSLOG_ERROR_UNKNOWN_ERROR			= -100	///< Unknown error.
} SSLOGERROR;

// the maximum log file size.  The default is SSLOG_MAX_LOG_SIZE_NO_LIMIT,
// while the SSLOG_MAX_LOG_SIZE is provided simply so the user can
// ask for file limiting without having to specify a size.
#define SSLOG_MAX_LOG_SIZE_NO_LIMIT     (INVALID_FILE_SIZE)
#define SSLOG_MAX_LOG_SIZE_DEFAULT		(SSLOG_MAX_LOG_SIZE_NO_LIMIT)

// the registry path to the NT event log registered sources
#define SSLOG_EVENT_LOG_SOURCE_REG (_T("SYSTEM\\CurrentControlSet\\Services\\Eventlog\\Application\\"))

/// Defines special messages that will be sent from the SS_Log class to the
/// SSLogConnection class (the server) that signify an instruction or task
/// rather than a message to be logged.
typedef enum SSLogMessageType
{
	// destination messages
    MSGTYPE_LOG_MESSAGE,
    MSGTYPE_ERASE_LOG,
    MSGTYPE_SET_DESTINATION_NAME,
    MSGTYPE_SET_REGISTRY_KEY,
    MSGTYPE_LIMIT_OUTPUT_SIZE,

	// server specific messages
    MSGTYPE_DUMMY_NO_FUNCTION,
    MSGTYPE_ADD_CONNECTION,
    MSGTYPE_RELEASE_CONNECTION,
    MSGTYPE_SHUTDOWN_SERVER,
    MSGTYPE_REGISTRY_FILTERS_CHANGED,

	// this must be the last item in the list
    MSGTYPE_COUNT

} MSGTYPE;


/// Structure for passing filter types.
///
/// @param		nType -- the LOGTYPE_* entry to the SS_LogFilterType enum above.
/// @param		szText -- text to display in messages (such as in a file or window).
/// @param		szReg -- text to write to the registry.  This text will also be
///					displayed in the SS_Log_Server's filter settings dialog box.
struct SSLogFilter
{
    SS_LogFilterType    nType;
    TCHAR               szText[128];
    TCHAR               szReg[128];
};

extern SSLogFilter g_Filters[];

INT GetFiltersCount();
BOOL GetFlagsText(DWORD dwFlags, LPSTR szTextA, INT cchBufferSize);
BOOL GetFlagsFromText(LPCSTR szTextA, LPDWORD dwFlags);
BOOL GetFiltersFromRegistry(HKEY hRegHive, LPCTSTR szKeyPath, LPDWORD pdwFilters);
BOOL WriteFiltersToRegistry(HKEY hRegHive, LPCTSTR szKeyPath, DWORD dwFilters);
BOOL TrimLogFile(HANDLE hFileIn, HANDLE* phFileOut, DWORD dwMaxFileSizeKb, LPCTSTR szFilename);

#endif // __ss_log_defines_h__
