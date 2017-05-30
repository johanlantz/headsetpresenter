// ----------------------------------------------------------------------- //
///
/// @file       SS_Log.cpp
/// @author     Steve Schaneville
/// @date       23 Oct 2001, 16:23
/// @brief      Implementation file for the SS_Log class.
///
/// Copyright (c) 2001 - 2003\n
/// z_web_master@yahoo.com\n
/// http://schaneville.webhop.net/SS_Utils
/// Version 3.02
///
// ----------------------------------------------------------------------- //

// ------------------[       Pre-Include Defines       ]------------------ //
// ------------------[          Include Files          ]------------------ //
#include <wtypes.h>
#include <tchar.h>
#include <stdio.h>
#include <assert.h>
#include "SS_RegistryKey.h"

// we #define _SS_LOG_DO_NOT_INCLUDE_MACROS so the SS_Log.h file does not
// include the pre-processor macros intended only for use by the user of
// this class.
#define _SS_LOG_DO_NOT_INCLUDE_MACROS
#include "SS_Log.h"
#undef _SS_LOG_DO_NOT_INCLUDE_MACROS

// ------------------[      Macros/Constants/Types     ]------------------ //

// just a struct for passing around a window name and handle together, used
// by the EnumWindowsProc function.
typedef struct SSFindWindow
{
    TCHAR   szWindowName[MAX_PATH];
    HWND    hWnd;
} SS_FIND_WINDOW;

// ------------------[         Global Variables        ]------------------ //
#ifdef _SS_LOG_ACTIVE

/// The global log is just an instance of SS_Log that will be sending its
/// messages to a server named in the SSLOG_DEFAULT_LOGSERVER #define.
SS_Log g_Log(SSLOG_DEFAULT_LOGSERVER);

#endif

// ------------------[         Global Functions        ]------------------ //

#ifdef _SS_LOG_ACTIVE

// the following functions are called by the logging macros.  These functions
// are basically make up the calls for sending messages.
INT WriteLog(SS_Log* pLog, DWORD dwFilter, TCHAR* pMsg, va_list* args);
INT WriteLog(SS_Log* pLog, DWORD dwFilter, TCHAR* pMsg, ...);
INT WriteLog(SS_Log* pLog, TCHAR* pMsg, ...);
INT WriteLog(DWORD dwFilter, TCHAR* pMsg, ...);
INT WriteLog(TCHAR* pMsg, ...);
INT WriteLogID(SS_Log* pLog, DWORD dwFilter, UINT nResourceID, va_list* args );
INT WriteLogID(SS_Log* pLog, DWORD dwFilter, UINT nResourceID, ...);
INT WriteLogID(SS_Log* pLog, UINT nResourceID, ...);
INT WriteLogID(DWORD dwFilter, UINT nResourceID, ...);
INT WriteLogID(UINT nResourceID, ...);
VOID LogStoreFileLine(TCHAR* szFile, int nLine);
#endif // _SS_LOG_ACTIVE

// A global function that will look for a previously opened SS_Log_Server.
// If one is found with the name provided in the szWindowName parameter
// (from the struct in lParam), we return the handle to that window in the
// hWnd parameter (also in the struct in lParam).
BOOL CALLBACK EnumWindowsProc( HWND hwnd, LPARAM lParam )
{
    SS_FIND_WINDOW* pFindWindow = (SS_FIND_WINDOW*)lParam;

    TCHAR szText[256];
    DWORD dwResult = 0;
    LRESULT status;
    status = SendMessageTimeout(hwnd, WM_GETTEXT, 256, (LPARAM) &szText[0], SMTO_ABORTIFHUNG, 1000, &dwResult); // new
    //GetWindowText(hwnd, szText, 256);
    if( !_tcscmp(szText, pFindWindow->szWindowName) )
    {
        pFindWindow->hWnd = hwnd;
        return FALSE;
    }

    return TRUE;
}

// ------------------[    Class Function Definitions   ]------------------ //

// ----------------------------------------------------------------------- //
/// Construct an SS_Log instance and initialize the server name.  The server
/// will not be started until needed.
///
/// @param		szServerName [in] a null-terminated string used to
///                             initialize our new server name.
/// @return     none
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_Log::SS_Log(LPCTSTR szServerName)
{
    ASSERT(szServerName);

    // initialize
    InitObject();

    // set log window name
    ServerName(szServerName);
}

// ----------------------------------------------------------------------- //
/// Construct an SS_Log instance and initialize the server name.  The server
/// will not be started until needed.
///
/// @param		szServerName [in] a null-terminated string used to
///                 initialize our new server name.
/// @param      szRegKey [in] registry key in which we will store all settings.
///                 See SetRegistryKey() for more details.
/// @param      szDestinationName [in] name of the output destination.  See
///                 SetDestinationName() for details.
/// @param      dwOutputSizeLimitKB [in] size in KiloBytes to limit the output
///                 destinations.  See LimitOutpuSize() for details.
/// @param      bUseServer [in] specified whether or not to use the destination
///                 server or simply write out to a file locally.  This has
///                 serious performance implications.  See UseServer() for
///                 details.
/// @return     none
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_Log::SS_Log(LPCTSTR szServerName, LPCTSTR szRegKey,
               LPCTSTR szDestinationName, DWORD dwOutputSizeLimitKB,
               BOOL bUseServer)
{
    ASSERT(szServerName);
    ASSERT(szRegKey);
    ASSERT(szDestinationName);
    ASSERT(dwOutputSizeLimitKB >= 0);

    // initialize
    InitObject();

    // are we using the server? We *must* set this before setting other params
    // so we don't accidentally launch the server if it isn't wanted.
    UseServer(bUseServer);

    // set log window name
    ServerName(szServerName);

    // set other parameters
//    SendSSLogMessage(MSGTYPE_DUMMY_NO_FUNCTION, _T("")); // just to wake up server
//                                                         // so that registry keys
//                                                         // get created properly
    SetRegistryKey(szRegKey);
    SetDestinationName(szDestinationName);
    LimitOutputSize(dwOutputSizeLimitKB);
}

// ----------------------------------------------------------------------- //
/// SS_Log destructor.
///
/// @return     none
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_Log::~SS_Log()
{
    ReleaseConnection();

    DeleteCriticalSection(&m_cs);
    CloseHandle(FileHandle());

    delete [] m_szServerName;
    m_szServerName = NULL;
    delete [] m_szDestinationName;
    m_szDestinationName = NULL;
    delete [] m_szRegistryKey;
    m_szRegistryKey = NULL;
}

// ----------------------------------------------------------------------- //
/// SS_Log copy constructor.  User specifiable data is copied over, but
/// operational parameters get the default value of a new SS_Log.
///
/// @param		rhs [in] an SS_Log instance to copy
/// @return     none
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_Log::SS_Log( SS_Log& rhs )
{
    *this = rhs;
}

// ----------------------------------------------------------------------- //
/// SS_Log assignment operator.  User specifiable data is copied over, but
/// operational parameters get the default value of a new SS_Log.
///
/// @param		rhs [in] an SS_Log instance to copy
/// @return     This instance of the SS_Log.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_Log&	SS_Log::operator = (SS_Log& rhs)
{
	InitObject();

    DefaultFlags( rhs.DefaultFlags() );
    ServerName( rhs.ServerName() );
    DestinationName( rhs.DestinationName() );
    UseServer( rhs.UseServer() );
    MaxFileSizeKb( rhs.MaxFileSizeKb() );

    return *this;
}

// ----------------------------------------------------------------------- //
/// SS_Log initializor.  Call this function from ALL constructors and
/// assignment operators before any initialization of variables is performed.
///
/// @return     none
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
VOID SS_Log::InitObject()
{
    InitializeCriticalSection(&m_cs);

	m_hFile = NULL;
	ConnectionAdded(FALSE);
    InstanceHandle(NULL);
    FileHandle(NULL);
	CreateServerStatus(SSLOG_CS_CREATE_NOT_YET_ATTEMPTED);
    m_szServerName = new TCHAR[MAX_PATH];
	ServerName(_T(""));
    m_szDestinationName = new TCHAR[MAX_PATH];
	DestinationName(_T(""));
    m_szRegistryKey = new TCHAR[MAX_PATH];
    LoggingDisabled(FALSE);
	UseServer(TRUE);
    ServerName(SSLOG_DEFAULT_LOGSERVER);
	SetLastError(SSLOG_ERROR_NO_ERROR);
	LogCount(0);
	MaxFileSizeKb(INVALID_FILE_SIZE);

    TCHAR szRegKey[MAX_PATH];
    _tcscpy(szRegKey, SSLOG_REG_DEFAULT_APP_KEY);
    _tcscat(szRegKey, _T("\\"));
    _tcscat(szRegKey, SSLOG_REG_DEFAULT_APP_SUBKEY);
    RegistryKey(szRegKey);
    ReadRegDefaultSettings();

#ifdef _DEBUG
    DefaultFlags( LOGTYPE_DEBUG | LOGTYPE_TRACE | LOGTYPE_LOGTOFILE | LOGTYPE_LOGTOWINDOW ); // we don't want to add LOGTYPE_LOGTOSYSTEMLOG by default
#else // _DEBUG
    DefaultFlags( LOGTYPE_RELEASE | LOGTYPE_TRACE | LOGTYPE_LOGTOFILE | LOGTYPE_LOGTOWINDOW ); // we don't want to add LOGTYPE_LOGTOSYSTEMLOG by default
#endif // _DEBUG
}

// ----------------------------------------------------------------------- //
/// WriteLog is the main function to be called when sending any message to the
/// server.  Note that even though this function is public, it is NOT intended
/// for the user to ever call this function.  Use the macros in the
/// @link PLF Primary Logging Functions @endlink and
/// @link GLF Global Log Functions @endlink
/// sections to send your logs.
///
/// WriteLog will gather the contents of the message into a tab delimited
/// string and forward the string on the WriteToServer (or WriteToFile if
/// not using the server) function.  The final message string will be of the
/// form:
///
/// <tt>\<message type\>\\t\<time\>\\t\<filename\>\\t\<line #\>\\t\<filter flags\>\\t\<message text\></tt>
///
/// That text will be parsed out into the individual components in the
/// server.
///
/// Before the message is sent we determine what type of build we are
/// in and compare that to the LOGTYPE_BUILDS message flags.  We will
/// filter out messages (no send them) if the appropriate flag is not
/// present in the dwFilterLog parameter for this build type.  All other
/// filtering will be done by the server.
///
/// @param		nType [in] one of the MSGTYPE values.  MSGTYPE_LOG_MESSAGE
///					send a message to be logged, and all other message
///					type do various server configurations.
/// @param		szFile [in] filename where the message originated.
///	@param		nLine [in] line number where the message originated.
/// @param		dwFilterLog [in] flags attached to this message.  Any
///					combination of the flags in the SS_LogFilterType enum
///					is valid.
/// @param		pMsg [in] the sprintf-style format text for the message to
///					be sent, or if this is a server config message, the
///					configuration parameter(s).
/// @param		args [in] a va_list of arguments to be formatted into the
///					pMsg format string.
/// @return     An error value found in the SSLogError enum.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_Log::WriteLog( MSGTYPE nType, TCHAR* szFile, int nLine, DWORD dwFilterLog,
					   const TCHAR* pMsg, va_list* args )
{
    ASSERT(pMsg);
	INT			nr = SSLOG_ERROR_UNKNOWN_ERROR;

#ifdef _SS_LOG_ACTIVE

	// local variable declarations
    BOOL        bLogThisMessage = TRUE;
    int         nchars;
    SYSTEMTIME  ust;
    SYSTEMTIME  lst;
    TCHAR       szMessage[SSLOG_MAX_MESSAGE_LENGTH];
    TCHAR       szFinalBuffer[SSLOG_MAX_MESSAGE_LENGTH];
    char        szFinalBufferA[SSLOG_MAX_MESSAGE_LENGTH];
    TCHAR       szTimeFormat[256] = _T("%02d:%02d:%02d, %02d.%02d.%02d");
    TCHAR       szFinalFormat[256] = _T("%d\t%s\t%s\t%d\t%d\t%s");
    TCHAR       szTime[256];

	// determine our build type
#ifdef _DEBUG
    DWORD		dwCurrentBuildType = LOGTYPE_DEBUG;
#else // _DEBUG
    DWORD		dwCurrentBuildType = LOGTYPE_RELEASE;
#endif // _DEBUG

    // we only process messages if logging is not disabled
    if( LoggingDisabled() == FALSE )
    {
        // prep the message text
        // . if nchars == 0, there is no message, so nothing to log, so just return.
        // . if args == NULL, this is an SS_Log-specific message.  Continue without
        //   using any params
        if( args != NULL )
        {
            nchars = _vsntprintf(szMessage, SSLOG_MAX_MESSAGE_LENGTH, pMsg, *args);
            if( 0 == nchars )
                bLogThisMessage = FALSE;
        }
        else
            _tcscpy(szMessage, pMsg);

	    // make sure we add our default filter flags.
	    SetFlagsInternal(&dwFilterLog);

	    // We filter out the build type here.  All other filters are handled on
	    // the server side.  We have to handle the build type on this side because
	    // the server has no idea how this app was built.
	    if( !(dwCurrentBuildType & dwFilterLog) )
		    bLogThisMessage = FALSE;

        if( bLogThisMessage == TRUE )
        {
            // prep message time
            GetSystemTime(&ust);
            SystemTimeToTzSpecificLocalTime( NULL, &ust, &lst );
            _stprintf(szTime, szTimeFormat, lst.wHour, lst.wMinute, lst.wSecond,
                lst.wDay, lst.wMonth, lst.wYear);

            // prep the final output buffer
            _stprintf(szFinalBuffer, szFinalFormat,
                nType, szTime, szFile, nLine, dwFilterLog, szMessage);

		    // convert to ANSI string if we are in a UNICODE build.
    #if defined(UNICODE) || defined(_UNICODE)
            wcstombs(szFinalBufferA, szFinalBuffer, SSLOG_MAX_MESSAGE_LENGTH);
    #else
            strcpy(szFinalBufferA, szFinalBuffer);
    #endif

            // send the message if there is something to send
            if( bLogThisMessage == TRUE )
		    {
			    EnterCriticalSection(&m_cs);
			    if( UseServer() == TRUE )
				    nr = WriteToServer(szFinalBufferA);
			    else
				    // only send MSGTYPE_LOG_MESSAGE type messages
				    // because other message types are for server
				    // conficuration only.
				    if( nType == MSGTYPE_LOG_MESSAGE )
					    nr = WriteToFile(szFinalBufferA);

			    LeaveCriticalSection(&m_cs);
		    }
        }
    }
#endif // _SS_LOG_ACTIVE

	return nr;
}

// ----------------------------------------------------------------------- //
/// Will send a message from a string resource ID to be logged.  See
/// WriteLog() for more details.
///
/// @param		szFile [in] filename where the message originated.
///	@param		nLine [in] line number where the message originated.
/// @param		dwFilter [in] flags attached to this message.  Any
///					combination of the flags in the SS_LogFilterType enum
///					is valid.
/// @param		nResourceID [in] a string resource ID containing the
///					sprintf-style format text for the message to
///					be sent.
/// @param		args [in] a va_list of arguments to be formatted into the
///					pMsg format string.
/// @return     An error value found in the SSLogError enum.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_Log::WriteLogID(TCHAR* szFile, int nLine, DWORD dwFilter,
                        UINT nResourceID, va_list* args)
{
    // the user must pass in the instance handle (use the InstanceHandle
    // function) before trying to log by passing in a resource ID.
    ASSERT(InstanceHandle());

    // get our string from the resource id.
    TCHAR szString[SSLOG_MAX_MESSAGE_LENGTH];
    LoadString(InstanceHandle(), nResourceID, szString, 4096);

    return WriteLog(MSGTYPE_LOG_MESSAGE, szFile, nLine, dwFilter, szString, args);
}

// ----------------------------------------------------------------------- //
/// Send our message to the log server.  This function will do several
/// things to make sure have a server available to us.  The steps performed
/// are as follows:
///
/// -# Check to see if we have previously tried to start the server.  If
///			we have and the attempt failed, assume that all future attempts
///			will fail too and just return SSLOG_ERROR_LAUNCH_SERVER_FAILED.
/// -# Check to see if we've added our connection to the server.  This is
///			just like AddRef in COM.  The server will shutdown when all
///			references are release.  So if we've not yet added our connection,
///			we call AddConnection(), which essentially recursively calls this
///			function again to send the MSGTYPE_ADD_CONNECTION message.
/// -# Attempt to connect to the named pipe.  The name comes from the
///			text string passed into the constructor of the SS_Log class.
/// -# If the connection to the pipe fails, AND if we've not yet attempted
///			to start the server, call OpenLogServer() to create the
///			server process.  Record the success or failure of the
///			process creating for future calls to this function (see step 1 above).
/// -# If everything has gone well so far, we have either already made
///			connection to the named pipe, or we still need to because
///			the server process was just created.  If the latter is the
///			case, try again to connect to the named pipe.
/// -# If a successful connection has been made to the named pipe,
///			write our message to it.
///
/// These steps swiftly handle several senarios, such as an SS_Log trying to
/// connect to a server with the same name as one previously created by
/// another SS_Log instance, or an SS_Log instance that is unable to
/// start a server.  The main goal is NOT to hinder/slow the execution
/// of the application using the SS_Log class.  Note that if the UseServer
/// function is called with a FALSE parameter, we will not try to start
/// a server at all, and the application's performance can be seriously affected.
///
/// @param		szMessageA [in] the buffer to be written to the file.
/// @return     One of the following error values (found in the SSLogError enum):
///				SSLOG_ERROR_LAUNCH_SERVER_FAILED, SSLOG_ERROR_CONNECT_TO_PIPE_FAILED,
///				SSLOG_ERROR_WRITE_TO_PIPE_FAILED, SSLOG_ERROR_NO_ERROR.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_Log::WriteToServer(LPCSTR szMessageA)
{
#ifdef _SS_LOG_ACTIVE

    INT nr = SSLOG_ERROR_UNKNOWN_ERROR;

    // if we were not able to open the server, the SS_Log_Server.exe
    // program is probably missing.  We'll do nothing in that case... all
    // logging will be dropped.
    if( CreateServerStatus() != SSLOG_CS_CREATE_FAILED )
    {
        if( ConnectionAdded() == FALSE )
            AddConnection();

        // get the log window's pipe name
        HANDLE hPipe = INVALID_HANDLE_VALUE;
        BOOL bPipeAvailable = FALSE;
        TCHAR szPipeName[MAX_PATH];
        _stprintf( szPipeName, _T("\\\\.\\pipe\\%s"), ServerName() );

        hPipe = ::CreateFile(szPipeName, GENERIC_WRITE, 0, NULL,
                             OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        if( hPipe != INVALID_HANDLE_VALUE )
            CreateServerStatus(SSLOG_CS_CREATE_SUCCEEDED);
		else
			nr = SSLOG_ERROR_CONNECT_TO_PIPE_FAILED;

		// if the pipe connection failed and we've not yet started the
		// server, start it now.
        if( hPipe == INVALID_HANDLE_VALUE &&
            CreateServerStatus() == SSLOG_CS_CREATE_NOT_YET_ATTEMPTED )
        {
            if( CreateServerStatus() == SSLOG_CS_CREATE_NOT_YET_ATTEMPTED )
            {
                BOOL br = OpenLogServer();
                if( br == FALSE )
				{
                    CreateServerStatus(SSLOG_CS_CREATE_FAILED);
					nr = SSLOG_ERROR_LAUNCH_SERVER_FAILED;
				}
                else
                {
                    CreateServerStatus(SSLOG_CS_CREATE_SUCCEEDED);
                    // transfer the default values
                    SetDestinationName(DestinationName());
                    LimitOutputSize(MaxFileSizeKb());
                }
            }
			else
				nr = SSLOG_ERROR_LAUNCH_SERVER_FAILED;
        }

		if( hPipe == INVALID_HANDLE_VALUE &&
            CreateServerStatus() == SSLOG_CS_CREATE_SUCCEEDED )
		{
			bPipeAvailable = ::WaitNamedPipe(szPipeName, NMPWAIT_WAIT_FOREVER);

			if( bPipeAvailable == TRUE )
			{
				hPipe = ::CreateFile(szPipeName, GENERIC_WRITE, 0, NULL,
					OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		        if( hPipe == INVALID_HANDLE_VALUE )
					nr = SSLOG_ERROR_CONNECT_TO_PIPE_FAILED;
			}
			else
				nr = SSLOG_ERROR_CONNECT_TO_PIPE_FAILED;
		}

        if( hPipe != INVALID_HANDLE_VALUE )
        {
			DWORD dwBytesWritten;
            BOOL br = ::WriteFile( hPipe, szMessageA, strlen(szMessageA)+1,
                &dwBytesWritten, NULL );
			if( br == TRUE )
				nr = SSLOG_ERROR_NO_ERROR;
			else
				nr = SSLOG_ERROR_WRITE_TO_PIPE_FAILED;
        }

		::CloseHandle(hPipe);
    }
	else
		nr = SSLOG_ERROR_LAUNCH_SERVER_FAILED;

	SetLastError(nr);

#endif // _SS_LOG_ACTIVE

	return nr;
}

// ----------------------------------------------------------------------- //
/// When not using the server for logging messages, this functions writes
/// messages out to the filename specified in the SetDestination function.
/// Note that no filtering is available from this function.  You must
/// use the server to get filtering.  However, you can load the file output
/// by this function into the SS_Log_Server.exe application and do some
/// versatile filtering "after the fact".
///
/// @param		szBufferA [in] the buffer to be written to the file.
/// @return     An error value found in the SSLogError enum.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_Log::WriteToFile(LPCSTR szBufferA)
{
	INT nr = SSLOG_ERROR_UNKNOWN_ERROR;

    // open the log file if not already open
    if( !FileHandle() )
    {
        // if this assert fires, it is likely that you have not yet called
        // the SetDestinationName().  You must do so when you are not
        // using the server before logging any messages
		ASSERT( _tcscmp(DestinationName(), _T("")) ); // make sure DestinationName() != ""
        HANDLE hLog = ::CreateFile( DestinationName(), GENERIC_WRITE,
            FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
            OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL,
            NULL );
        FileHandle(hLog);
    }

	// check the size of our log file and half it if requested by user
	HANDLE hFileNew = NULL;
	TrimLogFile(FileHandle(), &hFileNew, MaxFileSizeKb(), DestinationName());
	if( hFileNew != NULL && hFileNew != INVALID_HANDLE_VALUE )
		FileHandle(hFileNew);

    // write the log and flush!
	// . first parse the text and drop our message type, as this should
	//   not be recorded
	// . find the flags parameter and convert it to the corresponding flags
	//   text
    if( INVALID_HANDLE_VALUE != FileHandle() && NULL != FileHandle() )
    {
		ASSERT(strlen(szBufferA) < SSLOG_MAX_MESSAGE_LENGTH);
		char szBufferCopyA[SSLOG_MAX_MESSAGE_LENGTH];
		strcpy(szBufferCopyA, szBufferA);

        DWORD cbWritten;
        DWORD dwResult;
		char szMessageA[SSLOG_MAX_MESSAGE_LENGTH];
		char szFlagsA[256];
		char szCount[32];
		INT nFlags = 0;

		char* pszBufferCopyA = szBufferCopyA;
		char* psz1Tab = NULL;
		char* psz4Tabs = NULL;
		char* psz5Tabs = NULL;
		INT nTabsFound = 0;

		// do a bit of parsing to find the delimiters surrounding
		// our message type and flags parameters.
		while( nTabsFound != 5 )
		{
			if( *pszBufferCopyA == '\t' )
			{
				nTabsFound++;
				if( nTabsFound == 1 )
					psz1Tab = pszBufferCopyA;
				if( nTabsFound == 4 )
					psz4Tabs = pszBufferCopyA;
				if( nTabsFound == 5 )
					psz5Tabs = pszBufferCopyA;
			}
			pszBufferCopyA++;
		}
		*psz1Tab = '\0';
		*psz4Tabs = '\0';
		*psz5Tabs = '\0';

		// create our message ID.
		_itoa(LogCount(), szCount, 10);
		LogCount(LogCount()+1); // m_nLogCount++

		// get our flags parameter and convert to text
		nFlags = atoi(psz4Tabs+1);
		GetFlagsText(nFlags, szFlagsA, 256);

		// reassemble our message
		strcpy(szMessageA, szCount);
		strcat(szMessageA, "\t");
		strcat(szMessageA, psz1Tab+1);
		strcat(szMessageA, "\t");
		strcat(szMessageA, szFlagsA);
		strcat(szMessageA, "\t");
		strcat(szMessageA, psz5Tabs+1);

        // SetFilePointer is necessary for cases where we have two logs
        // writing to the same file.  We must make sure that we
        // move to the end of the file in case the other log has written
        // to it since we last did... we don't want to step on that log's
        // information.
        dwResult = ::SetFilePointer( FileHandle(), 0, NULL, FILE_END );
        ::WriteFile( FileHandle(), (LPCVOID)szMessageA,
            strlen(szMessageA),
            &cbWritten, NULL );
        ::WriteFile( FileHandle(), "\n",
            strlen("\n"),
            &cbWritten, NULL );
		::FlushFileBuffers( FileHandle() );
    }

	return nr;
}

// ----------------------------------------------------------------------- //
/// Open a new SS_Log_Server.exe process if we don't already find one running
/// with the appropriate name.  We use the EnumWindows() API function to
/// compare the names of all open windows with the name passed into the
/// constructor of the SS_Log class. If we don't find a match, we create a
/// new instance of SS_Log_Server.exe and pass it the SS_Log::ServerName()
/// name as a commandline param.
///
/// IMPORTANT:  SS_Log_Server.exe must be somewhere in the path for this
/// function to succeed.
///
/// @return     TRUE if, upon returning from this function, there is a
///				server with the proper name running.  FALSE otherwise.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
BOOL SS_Log::OpenLogServer()
{
#ifdef _SS_LOG_ACTIVE

	if( UseServer() == FALSE )
	{
		// if we are here, then there is a bug in the SS_Log class.  We should
		// never attempt to open the server if the user has requested not
		// to use it.
		ASSERT(!"Bug in SS_Log.  We should never arrive here.");
		return FALSE;
	}

    // we must create the log window if is it not already running
#ifdef _LCS
    WNDENUMPROC lpEnumFunc = (int(_stdcall*)())EnumWindowsProc;
#else
    WNDENUMPROC lpEnumFunc = EnumWindowsProc;
#endif

    SS_FIND_WINDOW findWindow;
    _tcscpy( findWindow.szWindowName, ServerName() );
    findWindow.hWnd = NULL;

    EnterCriticalSection(&m_cs);

    // we search for the window twice so that we need not enter the
    // critical section every time we call this function.
    EnumWindows( lpEnumFunc, (LPARAM)&findWindow );
    if( !findWindow.hWnd )
    {
        PROCESS_INFORMATION pi;
        STARTUPINFO si;
        si.lpDesktop = NULL;
        si.lpTitle = NULL;
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = TRUE;
        si.cb = sizeof(si);
        si.cbReserved2 = 0;
        si.lpReserved = NULL;
        si.lpReserved2 = NULL;

        TCHAR szCommandLine[MAX_PATH];
        _stprintf( szCommandLine, _T("SS_Log_Server \"%s\""), ServerName() );
        INT nResult = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE,
            NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        if( !nResult )
        {
#ifdef _DEBUG
            ::MessageBox( NULL,
						  _T("Could not start the SS_Log_Server.exe application... ")
						     _T("does it reside in the system path?  (This message ")
							 _T("appears in debug builds only.  In release builds, ")
							 _T("we simply continue quietly without writing to a log ")
							 _T("server)"),
						  _T("Error"),
						  MB_OK|MB_ICONSTOP );
#endif
			LeaveCriticalSection(&m_cs);
            return FALSE;
        }

        findWindow.hWnd = NULL;
        while( !findWindow.hWnd )
            EnumWindows( lpEnumFunc, (LPARAM)&findWindow );

    }

    LeaveCriticalSection(&m_cs);

#endif // _SS_LOG_ACTIVE

    return TRUE;
}

// ----------------------------------------------------------------------- //
/// Send the MSGTYPE_SET_DESTINATION_NAME message to the server, passing
/// the server name in the message's "message text".  The SetDestinationName
/// has a different affect on each destination type.  See
/// DestinationName(LPCTSTR szDestinationName) for details.
///
/// @param		szName [in] null-terminated string containing the name
///					of the destination.
/// @return     An error message specified in the SSLogError enum.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_Log::SetDestinationName(LPCTSTR szName)
{
	INT nr = SSLOG_ERROR_NO_ERROR;

#ifdef _SS_LOG_ACTIVE

	ASSERT(szName);
    DestinationName(szName);

    // store info in registry
    TCHAR szRegKey[MAX_PATH];
    _tcscpy(szRegKey, SSLOG_REG_HIVE_SUBKEY);
    _tcscat(szRegKey, _T("\\"));
    _tcscat(szRegKey, RegistryKey());
    HKEY hKey;
    LONG lResult = RegOpenKeyEx( SSLOG_REG_DEFAULT_HIVE, szRegKey, NULL, KEY_ALL_ACCESS, &hKey );
    if( ERROR_SUCCESS != lResult )
    {
        lResult = RegCreateKeyEx( SSLOG_REG_DEFAULT_HIVE, szRegKey, NULL, NULL,
            REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,
            NULL, &hKey, NULL );
    }
    if( ERROR_SUCCESS == lResult )
    {
        DWORD dwSize = _tcslen(szName)*sizeof(TCHAR);
        lResult = RegSetValueEx( hKey, _T("DestinationName"), NULL, REG_SZ,
            (LPBYTE)szName, dwSize );
    }
    if( hKey )
        RegCloseKey(hKey);

    // notify the server
    if( UseServer() )
	    nr = SendSSLogMessage(MSGTYPE_SET_DESTINATION_NAME, szName);

#endif // _SS_LOG_ACTIVE

	return nr;
}

// ----------------------------------------------------------------------- //
/// Send the MSGTYPE_ERASE_LOG message to the server.
///
/// @return     An error message specified in the SSLogError enum.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_Log::EraseLog()
{
	INT nr = SSLOG_ERROR_UNKNOWN_ERROR;

#ifdef _SS_LOG_ACTIVE

	if( UseServer() )
	    nr = SendSSLogMessage(MSGTYPE_ERASE_LOG, _T(""));
	else
		if( ::DeleteFile(DestinationName()) != 0 )
			nr = SSLOG_ERROR_NO_ERROR;

#endif // _SS_LOG_ACTIVE

	return nr;
}

// ----------------------------------------------------------------------- //
/// Send the MSGTYPE_LIMIT_OUTPUT_SIZE message to the server.  Currently
/// the LimitOutputSize message only has an effect on the file destination
/// type (Window and NT Event Log are unaffected).  When set, the  file
/// will not exceed the size (in Kb) specified in the dwSizeLimit parameter.
/// Once the limit is reached, the server will remove the oldest half of
/// the log to make space for new incoming messages.  This also works
/// when not using the server (when you've called UseServer(FALSE)).
///
/// To specify an unlimited file size, call
/// LimitOutputSize(SSLOG_MAX_LOG_SIZE_NO_LIMIT);
///
/// @param		dwSizeLimit [in] maximum size that the file should not
///					exceed, specified in kilobytes.
/// @return     An error message specified in the SSLogError enum.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_Log::LimitOutputSize(const DWORD dwSizeLimit)
{
	INT nr = SSLOG_ERROR_NO_ERROR;

#ifdef _SS_LOG_ACTIVE

	MaxFileSizeKb(dwSizeLimit);

    // store info in registry
    TCHAR szRegKey[MAX_PATH];
    _tcscpy(szRegKey, SSLOG_REG_HIVE_SUBKEY);
    _tcscat(szRegKey, _T("\\"));
    _tcscat(szRegKey, RegistryKey());
    HKEY hKey;
    LONG lResult = RegOpenKeyEx( SSLOG_REG_DEFAULT_HIVE, szRegKey, NULL, KEY_ALL_ACCESS, &hKey );
    if( ERROR_SUCCESS != lResult )
    {
        lResult = RegCreateKeyEx( SSLOG_REG_DEFAULT_HIVE, szRegKey, NULL, NULL,
            REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,
            NULL, &hKey, NULL );
    }
    if( ERROR_SUCCESS == lResult )
    {
        DWORD dwSize = sizeof(DWORD);
        lResult = RegSetValueEx( hKey, _T("OutputSizeLimitKB"), NULL, REG_DWORD,
            (LPBYTE)&dwSizeLimit, dwSize );
    }
    if( hKey )
        RegCloseKey(hKey);

    // notify the server
	if( UseServer() == TRUE )
	{
		TCHAR szSizeLimit[64];
		_itot(dwSizeLimit, szSizeLimit, 10);
		nr = SendSSLogMessage(MSGTYPE_LIMIT_OUTPUT_SIZE, szSizeLimit);
	}

#endif // _SS_LOG_ACTIVE

	return nr;
}

// ----------------------------------------------------------------------- //
/// Send the MSGTYPE_SET_REGISTRY_KEY message to the server.  This message
/// will determine where the server will store filter settings in the
/// registry.  This is useful when you use the SS_Log class in more than
/// one project.  Each project can have maintain its own set of filters.
///
/// The parameter passed in should only be a single name, and should not
/// include complete path information.  The filters will be stored under:
///
/// <tt>HKEY_CURRENT_USER\\Software\\<b>YOUR KEY</b>\\SS_Log\\Filters</tt>
///
///	So a logical string to pass into this function would be your company
/// name, or your company name\\app name.  (ex. "My Company\\My App").  In
/// this example, the filters would be stored at:
///
/// <tt>HKEY_CURRENT_USER\\Software\\<b>My Company\\My App</b>\\SS_Log\\Filters</tt>
///
/// @param		szRegKey [in] null-terminated string containing the name
///					of a registry key where you want to store the registry
///					filters for this application.  DO NOT include leading
///                 or trailing "\\" characters in the key you pass into
///                 this function.
/// @return     An error message specified in the SSLogError enum.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_Log::SetRegistryKey(LPCTSTR szRegKey)
{
	INT nr = SSLOG_ERROR_NO_ERROR;

#ifdef _SS_LOG_ACTIVE

    ASSERT(szRegKey);
    TCHAR szRegKeyTmp[MAX_PATH];
    _tcscpy(szRegKeyTmp, szRegKey);
    _tcscat(szRegKeyTmp, _T("\\"));
    _tcscat(szRegKeyTmp, SSLOG_REG_DEFAULT_APP_SUBKEY);
    RegistryKey(szRegKeyTmp);

    // read the default settings, because the settings that are in the
    // new key could be different than those in the old key.
    ReadRegDefaultSettings();

    // send a message to the server
	if( UseServer() == TRUE )
		nr = SendSSLogMessage(MSGTYPE_SET_REGISTRY_KEY, szRegKeyTmp);

    // store this key in HKCU\Software\SS_Log\SS_Log for manual runs of SS_Log_Server.exe
    TCHAR szSSLogRegKey[MAX_PATH];
    _tcscpy(szSSLogRegKey, SSLOG_REG_HIVE_SUBKEY);
    _tcscat(szSSLogRegKey, _T("\\"));
    _tcscat(szSSLogRegKey, SSLOG_REG_DEFAULT_APP_KEY);
    _tcscat(szSSLogRegKey, _T("\\"));
    _tcscat(szSSLogRegKey, SSLOG_REG_DEFAULT_APP_SUBKEY);
    HKEY hKey;
    LONG lResult = RegOpenKeyEx( SSLOG_REG_DEFAULT_HIVE, szSSLogRegKey, NULL, KEY_ALL_ACCESS, &hKey );
    if( ERROR_SUCCESS != lResult )
    {
        lResult = RegCreateKeyEx( SSLOG_REG_DEFAULT_HIVE, szSSLogRegKey, NULL, NULL,
            REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,
            NULL, &hKey, NULL );
    }
    if( ERROR_SUCCESS == lResult )
    {
        DWORD dwSize = _tcslen(szRegKeyTmp)*sizeof(TCHAR);
        lResult = RegSetValueEx( hKey, _T("MostRecentRegKey"), NULL, REG_SZ,
            (LPBYTE)szRegKeyTmp, dwSize );
    }
    if( hKey )
        RegCloseKey(hKey);

#endif // _SS_LOG_ACTIVE

	return nr;
}

// ----------------------------------------------------------------------- //
/// Adds a connection to the server.  This is just like calling AddRef in
/// COM.  If multiple SS_Log instances attach to the same server by calling
/// this AddConnection() function, the server will not shut down until
///	all SS_Log instances have disconnected with ReleaseConnection().
///
/// @return     An error message specified in the SSLogError enum.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_Log::AddConnection()
{
    INT nr = SSLOG_ERROR_NO_ERROR;

#ifdef _SS_LOG_ACTIVE
	if( UseServer() == TRUE )
	{
		if( ConnectionAdded() == FALSE )
		{
			ConnectionAdded(TRUE);
			SendSSLogMessage(MSGTYPE_ADD_CONNECTION, _T(""));
		}
	}

#endif // _SS_LOG_ACTIVE

	return nr;
}

// ----------------------------------------------------------------------- //
/// Releases a connection from the server.  This is just like calling Release
/// in COM.  If multiple SS_Log instances attach to the same server by calling
/// the AddConnection() function, the server will not shut down until
///	all SS_Log instances have disconnected with this ReleaseConnection()
/// function.
///
/// @return     An error message specified in the SSLogError enum.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_Log::ReleaseConnection()
{
    INT nr = SSLOG_ERROR_NO_ERROR;

#ifdef _SS_LOG_ACTIVE

	if( UseServer() == TRUE )
	{
		if( ConnectionAdded() == TRUE )
		{
			SendSSLogMessage(MSGTYPE_RELEASE_CONNECTION, _T(""));
			ConnectionAdded(FALSE);
		}
	}

#endif // _SS_LOG_ACTIVE

	return nr;
}

// ----------------------------------------------------------------------- //
/// A generic helper command for sending all server configuration messages.
///
/// @param		nType [in] the message type to send.  Values in the
///					SSLogMessageType enum are valid.
/// @param		szMessage [in] the message parameter that the server
///					will use to do configuration based on the type
///					of message being sent.
/// @return     An error message specified in the SSLogError enum.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_Log::SendSSLogMessage(MSGTYPE nType, LPCTSTR szMessage)
{
    return WriteLog(nType, _T(""), 0, DefaultFlags(), szMessage, NULL);
}

// ----------------------------------------------------------------------- //
/// This function will add this instance's default flags to the passed in
/// dwFilter.  Default flags are added in the three categories:
/// LOGTYPE_OUTPUTS, LOGTYPE_BUILDS, and LOGTYPE_LEVELS.  Default flags will
/// only be added for each of the three groups in which no flags were
/// passed in as overrides.  For example, if the user passes in
/// LOGTYPE_CRITICAL as the only flag in the dwFilter (note that
/// LOGTYPE_CRITICAL is in the LOGTYPE_LEVELS category of flags), then the
/// current default flags for only the LOGTYPE_OUTPUTS and LOGTYPE_BUILDS
/// types will be added.
///
/// @param		pdwFilter [in, out]
///					- [in] This parameter should contain the filter flags that
///							are intended to override the current default flags
///							of this instance of the SS_Log.
///					- [out] Upon return, the parameter will contain default
///							flags in the areas that were not overridden (see
///							notes).
/// @return     none
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
VOID SS_Log::SetFlagsInternal(LPDWORD pdwFilter)
{

#ifdef _SS_LOG_ACTIVE

    if( !(*pdwFilter & LOGTYPEGROUP_DESTINATIONS) )
    {
        // no LOGTYPE_OUTPUTS were specified, so we add the default
        *pdwFilter |= DefaultFlags() & LOGTYPEGROUP_DESTINATIONS;
    }

    if( !(*pdwFilter & LOGTYPEGROUP_BUILDS) )
    {
        // no LOGTYPE_BUILDS were specified, so we add the default
        *pdwFilter |= DefaultFlags() & LOGTYPEGROUP_BUILDS;
    }

    if( !(*pdwFilter & LOGTYPEGROUP_LEVELS) )
    {
        // no LOGTYPE_LEVELS were specified, so we add the default
        *pdwFilter |= DefaultFlags() & LOGTYPEGROUP_LEVELS;
    }

#endif // _SS_LOG_ACTIVE

}


// ----------------------------------------------------------------------- //
/// Writes the passed-in filters to the registry.  This function is provided
/// here only to give the calling application the ability to create a UI for
/// these filters rather than making the user use the SS_Log_Server UI (or
/// making the user go edit the registry directly).
///
/// This function also sends a message to the SS_Log_Server app causing it
/// to re-read the registry filters.  This ensures that the SS_Log_Server
/// will immediately pick up any changes that were made to the filters.
///
/// @return     TRUE if the read was successful.
/// @return     FALSE if reading the registry failed.
/// @author     Steve Schaneville
/// @sa         GetRegistryFilters()
// ----------------------------------------------------------------------- //
BOOL SS_Log::SetRegistryFilters(DWORD dwRegFilters)
{
    TCHAR szKey[MAX_PATH];
    _tcscpy(szKey, SSLOG_REG_HIVE_SUBKEY);
    _tcscat(szKey, _T("\\"));
    _tcscat(szKey, RegistryKey());
    BOOL br = ::WriteFiltersToRegistry(SSLOG_REG_DEFAULT_HIVE, szKey, dwRegFilters);

    if( UseServer() )
        SendSSLogMessage(MSGTYPE_REGISTRY_FILTERS_CHANGED, _T(""));

    return br;
}


// ----------------------------------------------------------------------- //
/// Reads the current filters from the registry.  These filters are not
/// used in this class at all, but rather, they are used in the
/// SS_Log_Server app where they are used to destructively filter out unwanted
/// messages.  This function is provided here only to give the calling
/// application the ability to create a UI for these filters rather than
/// making the user use the SS_Log_Server UI (or making the user go
/// edit the registry directly).
///
/// This function also sends a message to the SS_Log_Server app causing it
/// to re-read the registry filters.  Call this function to pick up changes
/// that the user made to the filters during runtime and have those changes
/// obeyed without restarting the application.
///
/// @return     TRUE if the read was successful.
/// @return     FALSE if reading the registry failed.
/// @author     Steve Schaneville
/// @sa         SetRegistryFilters()
// ----------------------------------------------------------------------- //
DWORD SS_Log::GetRegistryFilters()
{
    DWORD dwFilters;

    TCHAR szKey[MAX_PATH];
    _tcscpy(szKey, SSLOG_REG_HIVE_SUBKEY);
    _tcscat(szKey, _T("\\"));
    _tcscat(szKey, RegistryKey());
    GetFiltersFromRegistry(SSLOG_REG_DEFAULT_HIVE, szKey, &dwFilters);

    if( UseServer() )
        SendSSLogMessage(MSGTYPE_REGISTRY_FILTERS_CHANGED, _T(""));

    return dwFilters;
}


// ----------------------------------------------------------------------- //
/// This function reads the default settings from the
/// "HKEY_CURRENT_USER\Software\<user specified>" registry key.  Currently,
/// settings stored are:
///
/// - "DisableLogging" -- determine whether or not to send out any messages.
///     The internal variable is updated to reflect the registry's value.
/// - "MostRecentRegKey" -- set by the SS_Log class, and used by the
///     SS_Log_Server app to determine which key to use for reading/storing
///     registry filters.
///
/// @return     TRUE if logging is enabled, and FALSE if not.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
BOOL SS_Log::ReadRegDefaultSettings()
{
    BOOL br = FALSE;
    LONG lr;
    HKEY hKey;

    TCHAR szKey[MAX_PATH];
    _tcscpy(szKey, SSLOG_REG_HIVE_SUBKEY);
    _tcscat(szKey, _T("\\"));
    _tcscat(szKey, RegistryKey());
    lr = RegOpenKeyEx(SSLOG_REG_DEFAULT_HIVE, szKey, 0, KEY_READ|KEY_WRITE, &hKey);
    if( ERROR_SUCCESS != lr )
    {
        lr = RegCreateKeyEx( SSLOG_REG_DEFAULT_HIVE, szKey, NULL, NULL,
            REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,
            NULL, &hKey, NULL );
    }

    if( ERROR_SUCCESS == lr )
    {
        DWORD dwType;
        DWORD dwValue = 0;
        DWORD dwSize;
        TCHAR szValue[MAX_PATH];

        // LoggingDisabled
        //
        dwType = REG_DWORD;
        dwSize = sizeof(dwValue);
        lr = RegQueryValueEx(hKey, _T("LoggingDisabled"), 0, &dwType, (LPBYTE)&dwValue, &dwSize);
        if( ERROR_SUCCESS == lr )
            LoggingDisabled(dwValue);
        else
        {
            dwType = REG_DWORD;
            dwValue = 0;
            dwSize = sizeof(dwValue);
            lr = RegSetValueEx(hKey, _T("LoggingDisabled"), 0, dwType, (LPBYTE)&dwValue, dwSize);
        }

        // DestinationName
        //
        dwType = REG_SZ;
        dwSize = MAX_PATH*sizeof(TCHAR);
        lr = RegQueryValueEx(hKey, _T("DestinationName"), 0, &dwType, (LPBYTE)&szValue, &dwSize);
        if( ERROR_SUCCESS == lr )
            DestinationName(szValue);
        else
        {
            dwType = REG_SZ;
            _tcscpy(szValue, SSLOG_DEFAULT_DESTINATION_NAME);
            dwSize = _tcslen(szValue);
            lr = RegSetValueEx(hKey, _T("DestinationName"), 0, dwType, (LPBYTE)szValue, dwSize);
        }

        // OutputSizeLimitKB
        //
        dwType = REG_DWORD;
        dwSize = sizeof(dwValue);
        lr = RegQueryValueEx(hKey, _T("OutputSizeLimitKB"), 0, &dwType, (LPBYTE)&dwValue, &dwSize);
        if( ERROR_SUCCESS == lr )
            MaxFileSizeKb(dwValue);
        else
        {
            dwType = REG_DWORD;
            dwValue = 0;
            dwSize = sizeof(dwValue);
            lr = RegSetValueEx(hKey, _T("OutputSizeLimitKB"), 0, dwType, (LPBYTE)&dwValue, dwSize);
        }
    }
    if( hKey )
        RegCloseKey(hKey);

    return br;
}

// ----------------------------------------------------------------------- //
/// SSLogEnterFunction constructor.  This function will write out a message
/// saying "Entering \<szFunctionName\>..." to the log specified in the
/// pLog parameter.
///
/// @param		szFile [in] the file from where this message originated.
/// @param		nLine [in] the line number from where this message originated.
/// @param		pLog [in] a pointer to the log to which the message will be
///					written.
/// @param		szFunctionName [in] name of the function that this call
///					originated from within.
/// @return     none
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SSLogEnterFunction::m_nIDCount = 0;
SSLogEnterFunction::SSLogEnterFunction(TCHAR* szFile, int nLine,
									   SS_Log* pLog, LPCTSTR szFunctionName)
{
#ifdef _SS_LOG_ACTIVE
	ASSERT(szFile);
    ASSERT(pLog);
    ASSERT(szFunctionName);
    m_szFunctionName = new TCHAR[_tcslen(szFunctionName)+1];
    _tcscpy(m_szFunctionName, szFunctionName);
    m_szFilename = new TCHAR[_tcslen(szFile)+1];
    _tcscpy(m_szFilename, szFile);
    m_nLineNumber = nLine;
    m_pLog = pLog;
    m_nIDCount++;
    m_nLocalID = m_nIDCount;
    LogStoreFileLine(szFile, nLine);
    WriteLog(pLog, LOGTYPE_INOUT, _T("+++ Entering %s.  ID: %d"), szFunctionName, m_nLocalID);
#endif
}

// ----------------------------------------------------------------------- //
/// SSLogEnterFunction destructor.  This function will write out a message
/// saying "Exiting \<szFunctionName\>..." to the log that was passed into
/// the constructor of this SSLogEnterFunction instance.
///
/// @return     none
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SSLogEnterFunction::~SSLogEnterFunction()
{
#ifdef _SS_LOG_ACTIVE
    LogStoreFileLine(m_szFilename, m_nLineNumber);
    WriteLog(m_pLog, LOGTYPE_INOUT, _T("------ Exiting %s.  ID: %d"), m_szFunctionName, m_nLocalID);
    delete m_szFunctionName;
    m_szFunctionName = NULL;
    delete m_szFilename;
    m_szFilename = NULL;
#endif
}




// ------------------[      Post Global Functions      ]------------------ //
#ifdef _SS_LOG_ACTIVE

/// the global variable where we temporarily hold the filename of each message
TCHAR   g_szFile[MAX_PATH];
/// the global variable where we temporarily hold the line number of each message
int     g_nLine;
/// the global variable used to prevent loss of filename/line number data due to threading issues
CRITICAL_SECTION g_cs;
/// the global variable used to determine if we need to initialize our g_cs global critical section
BOOL    g_bInitCs = TRUE;

/// @{ @name Global WriteLog Functions
/// These functions are called by the Log and LogID macros to send the
/// messages on to the SS_Log class.
INT WriteLog(SS_Log* pLog, DWORD dwFilter, TCHAR* pMsg, va_list* args)
{
    // let other threads access the WriteLog function...
    TCHAR szFile[MAX_PATH];
    _tcscpy(szFile, g_szFile);
    INT nLine = g_nLine;
    LeaveCriticalSection(&g_cs);

    // write the log
    return pLog->WriteLog(MSGTYPE_LOG_MESSAGE, szFile, nLine, dwFilter, pMsg, args);
}

INT WriteLog(SS_Log* pLog, DWORD dwFilter, TCHAR* pMsg, ...)
{
    va_list args;
    va_start(args, pMsg);

    INT br = WriteLog( pLog, dwFilter, pMsg, &args );

    va_end(args);
	return br;
}

INT WriteLog(SS_Log* pLog, TCHAR* pMsg, ...)
{
    va_list args;
    va_start(args, pMsg);

    INT br = WriteLog( pLog, pLog->DefaultFlags(), pMsg, &args );

    va_end(args);
	return br;
}

INT WriteLog(DWORD dwFilter, TCHAR* pMsg, ...)
{
    va_list args;
    va_start(args, pMsg);

    INT br = WriteLog( &g_Log, dwFilter, pMsg, &args );

    va_end(args);
	return br;
}

INT WriteLog(TCHAR* pMsg, ...)
{
    va_list args;
    va_start(args, pMsg);

    INT br = WriteLog( &g_Log, g_Log.DefaultFlags(), pMsg, &args );

    va_end(args);
	return br;
}

INT WriteLogID(SS_Log* pLog, DWORD dwFilter, UINT nResourceID, va_list* args)
{
    // let other threads access the WriteLogID function...
    TCHAR szFile[MAX_PATH];
    _tcscpy(szFile, g_szFile);
    INT nLine = g_nLine;
    LeaveCriticalSection(&g_cs);

    // write the log
    return pLog->WriteLogID(szFile, nLine, dwFilter, nResourceID, args);
}

INT WriteLogID(SS_Log* pLog, DWORD dwFilter, UINT nResourceID, ...)
{
    va_list args;
    va_start(args, nResourceID);

    INT br = WriteLogID( pLog, dwFilter, nResourceID, &args );

    va_end(args);
	return br;
}

INT WriteLogID(SS_Log* pLog, UINT nResourceID, ...)
{
    va_list args;
    va_start(args, nResourceID);

    INT br = WriteLogID( pLog, pLog->DefaultFlags(), nResourceID, &args );

    va_end(args);
	return br;
}

INT WriteLogID(DWORD dwFilter, UINT nResourceID, ...)
{
    va_list args;
    va_start(args, nResourceID);

    INT br = WriteLogID( &g_Log, dwFilter, nResourceID, &args );

    va_end(args);
	return br;
}

INT WriteLogID(UINT nResourceID, ...)
{
    va_list args;
    va_start(args, nResourceID);

    INT br = WriteLogID( &g_Log, g_Log.DefaultFlags(), nResourceID, &args );

    va_end(args);
	return br;
}

VOID LogStoreFileLine(TCHAR* szFile, int nLine)
{
    if( g_bInitCs )
    {
        g_bInitCs = FALSE;
        InitializeCriticalSection(&g_cs);
    }

    EnterCriticalSection(&g_cs);
    _tcscpy(g_szFile, szFile);
    g_nLine = nLine;
}
/// @}

#endif // _SS_LOG_ACTIVE



