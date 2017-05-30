// ----------------------------------------------------------------------- //
///
/// @file       SS_Log_Defines.cpp
/// @author     Steve Schaneville
/// @date       1 Mar 2003, 16:23
/// @brief      Global implementation file for the SS_Log class.
///
/// Copyright (c) 2001 - 2003\n
/// z_web_master@yahoo.com\n
/// http://schaneville.webhop.net/SS_Utils
/// Version 3.02
///
// ----------------------------------------------------------------------- //

#include <wtypes.h>
#include <tchar.h>
#include <assert.h>
#include "SS_Log_Defines.h"

/// This global array maps the log flags to the display text and to the
/// registry text.  For each log type you add, make sure to place an entry in
/// this array for your type.  See SS_LogFilterType and SSLogFilter for
/// details.
SSLogFilter g_Filters[] =
{
    // nType,                   szText,                 szReg,
    LOGTYPE_DEBUG,              _T("D"),                 _T("Build Debug (D)"),
	LOGTYPE_RELEASE,            _T("R"),                 _T("Build Release (R)"),

	LOGTYPE_LOGTOWINDOW,        _T("W"),                 _T("Log to Window (W)"),
	LOGTYPE_LOGTOFILE,          _T("F"),                 _T("Log to File (F)"),
	LOGTYPE_LOGTOEVENTLOG,      _T("E"),                 _T("Log to NT Event Log (E)"),

	LOGTYPE_CRITICAL,           _T("CRITICAL"),         _T("Status Critical"),
	LOGTYPE_WARNING,            _T("WARNING"),          _T("Status Warning"),
	LOGTYPE_NORMAL,             _T("NORMAL"),           _T("Status Normal"),
	LOGTYPE_TRACE,              _T("TRACE"),            _T("Status Trace"),
	LOGTYPE_TRACE_FAIL,         _T("TRACE FAIL"),       _T("Status Trace Fail"),

	LOGTYPE_INOUT,              _T("IN-OUT"),           _T("In && Out"),

	// user-defined types...
	LOGTYPE_ADD_YOUR_TYPE_HERE, _T("EXAMPLE"),          _T("Example") // this line can be safely removed (adjust your commas)
};

/// This function returns the number of entries in the global
/// filters list.  Useful if looping through the list.
INT GetFiltersCount()
{
	return sizeof(g_Filters) / sizeof(g_Filters[0]);
}

/// Will convert flags passed in into the corresponding text for display
/// in a message.  For multiple flags, each flag's text will be separated
/// by a '/'.
BOOL GetFlagsText(DWORD dwFlags, LPSTR szTextA, INT cchBufferSize)
{
	ASSERT(szTextA);
	ASSERT(cchBufferSize > 0);
	BOOL br = TRUE;
	char szTextTmpA[256];
    char szFilterTextA[256];
	strcpy(szTextTmpA, "");
    strcpy(szFilterTextA, "");

	// prep the levels text
	INT nCount = GetFiltersCount();
	for( INT i=0; i<nCount; i++ )
	{
		if( dwFlags & g_Filters[i].nType )
		{
            // make sure we are using an ANSI string
#if defined(UNICODE) || defined(_UNICODE)
            wcstombs(szFilterTextA, g_Filters[i].szText, 256);
#else
            strcpy(szFilterTextA, g_Filters[i].szText);
#endif

            ASSERT((strlen(szTextTmpA)+1+strlen(szFilterTextA)) < 256);
			if( (strlen(szTextTmpA)+1+strlen(szFilterTextA)) >= 256 )
			{
				br = FALSE;
				break;
			}
			if( strcmp(szTextTmpA, "") )
				strcat(szTextTmpA, "/");
			strcat(szTextTmpA, szFilterTextA);
		}
	}

	strncpy(szTextA, szTextTmpA, cchBufferSize);

	return br;
}

/// Will convert the flags text (as displayed in a message) back to the
/// original form of the on/off bits in a DWORD.  This function is typically
/// used for loading a message file that has the text version of the
/// flags.
BOOL GetFlagsFromText(LPCSTR szTextA, LPDWORD pdwFlags)
{
    BOOL br = TRUE;

    char szFilterTextA[256];
    strcpy(szFilterTextA, "");
    char* pszFlagsA = NULL;
    char szFlagsA[1024];
    strncpy( szFlagsA, szTextA, 1024 );
    *pdwFlags = 0;
    INT nCount = GetFiltersCount();

    pszFlagsA = strtok(szFlagsA, "/");
    while( pszFlagsA != NULL )
    {
        for( INT i=0; i<nCount; i++ )
        {
#if defined(UNICODE) || defined(_UNICODE)
            wcstombs(szFilterTextA, g_Filters[i].szText, 256);
#else
            strcpy(szFilterTextA, g_Filters[i].szText);
#endif
            if( !strcmp(szFilterTextA, pszFlagsA) )
                *pdwFlags |= (DWORD)g_Filters[i].nType;
        }
        pszFlagsA = strtok(NULL, "/");
    }

    return br;
}


// ----------------------------------------------------------------------- //
/// This function reads from the registry the filters that are on or off
/// and sets the appropriate flags (as defined in the SS_LogFilterType enum)
/// in the pdwFilters out-parameter.
///
/// @param		hRegHive [in] HKEY_CURRENT_USER or another registry hive
/// @param		szKeyPath [in] path to the registry key where SS_Log
///                 registry entries are held.  Note that we will add the
///                 "Filters" key to the path passed in, so don't include
///                 this as part of the path.
/// @param		pdwFilters [out] a pointer to a DWORD buffer into which we
///                 will set the filters that are currently turned
///                 on in the registry.
/// @return		TRUE if successful, FALSE otherwise
/// @author		Steve Schaneville
// ----------------------------------------------------------------------- //
BOOL GetFiltersFromRegistry(HKEY hRegHive, LPCTSTR szKeyPath, LPDWORD pdwFilters)
{
    LONG lResult = 0;
    DWORD dwFilterIsOn = 0;
    DWORD dwFilter = 0;
    HKEY hKey = NULL;
    TCHAR szKey[MAX_PATH];
    _tcscpy(szKey, szKeyPath);
    _tcscat(szKey, _T("\\"));
    _tcscat(szKey, SSLOG_REG_FILTERS_SUBKEY);

    lResult = RegOpenKeyEx( hRegHive, szKey, NULL, KEY_ALL_ACCESS, &hKey );
    if( ERROR_SUCCESS != lResult )
    {
        lResult = RegCreateKeyEx( hRegHive, szKey, NULL, NULL,
            REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,
            NULL, &hKey, NULL );
    }

    if( ERROR_SUCCESS == lResult )
    {
        for( INT i=0; i<GetFiltersCount(); i++ )
        {
            DWORD dwType = 0;
            DWORD dwSize = sizeof(dwFilterIsOn);
            lResult = RegQueryValueEx( hKey, g_Filters[i].szReg, NULL, &dwType,
                (LPBYTE)&dwFilterIsOn, &dwSize );
            if( ERROR_SUCCESS != lResult  || dwSize == 0 )
            {
                dwFilterIsOn = 1;
                dwSize = sizeof(DWORD);
                lResult = RegSetValueEx( hKey, g_Filters[i].szReg, NULL,
                    REG_DWORD, (LPBYTE)&dwFilterIsOn, dwSize );
            }
            if( dwFilterIsOn )
                dwFilter |= g_Filters[i].nType;
        }
    }

    *pdwFilters = dwFilter;
    if( hKey )
        RegCloseKey(hKey);

    return lResult == ERROR_SUCCESS ? TRUE : FALSE;
}


// ----------------------------------------------------------------------- //
/// Stores the flags provided in the dwFilters parameter into the registry.
/// Note that this function updates ALL filters in the registry, so you
/// must pass all the filter flags that you want to be on.  Passing in
/// only a single filter flag to this function will cause all others
/// to be turned off.
///
/// @param		hRegHive [in] HKEY_CURRENT_USER or another registry hive
/// @param		szKeyPath [in] path to the registry key where SS_Log
///                 registry entries are held.  Note that we will add the
///                 "Filters" key to the path passed in, so don't include
///                 this as part of the path.
/// @param		dwFilters [in] the set of filters that we will write into
///                 the registry key that was passed in.
/// @return		TRUE if successful, FALSE otherwise
/// @author		Steve Schaneville
// ----------------------------------------------------------------------- //
BOOL WriteFiltersToRegistry(HKEY hRegHive, LPCTSTR szKeyPath, DWORD dwFilters)
{
    LONG lResult = 0;
    DWORD dwFilterIsOn = 0;
    DWORD dwSize = 0;
    HKEY hKey;
    TCHAR szKey[MAX_PATH];
    _tcscpy(szKey, szKeyPath);
    _tcscat(szKey, _T("\\"));
    _tcscat(szKey, SSLOG_REG_FILTERS_SUBKEY);

    lResult = RegOpenKeyEx( hRegHive, szKey, NULL, KEY_ALL_ACCESS, &hKey );
    if( ERROR_SUCCESS != lResult )
    {
        lResult = RegCreateKeyEx( hRegHive, szKey, NULL, NULL,
            REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,
            NULL, &hKey, NULL );
    }

    if( ERROR_SUCCESS == lResult )
    {
        for( INT i=0; i<GetFiltersCount(); i++ )
        {
            dwFilterIsOn = (dwFilters & g_Filters[i].nType) ? 1 : 0;
            dwSize = sizeof(DWORD);
            lResult = RegSetValueEx( hKey, g_Filters[i].szReg, NULL,
                REG_DWORD, (LPBYTE)&dwFilterIsOn, dwSize );
        }
    }

    return lResult == ERROR_SUCCESS ? TRUE : FALSE;
}


// ----------------------------------------------------------------------- //
/// This function will check the size of a log file, and trim it if it
/// exceeds the max size passed into the function.  The trip is performed
/// by removing the first half (oldest portion) of the file,
/// leaving only the second half in place with the original
/// filename.  The function will make sure that it trims
/// only at the beginning or end of a line (won't cut a
/// line in half).
///
/// @param		hFileIn [in] an open file handle of the file to trim.
/// @param		phFileOut [out] the new handle to the open file, since
///					the one passed into the hFileIn parameter will be
///					closed if the trim takes place.
/// @param		dwMaxFileSizeKb [in] the maximum size that the file
///					should not be allowed to exceed (in kilobytes).  If
///					the file is larger than this value, the file will be
///					trimmed as described above.  IF the file is smaller
///					than this value, this function will simply return.
/// @param		szFilename [in] the filename of the file that is open in
///					the hFileIn parameter.  If the trim takes place, the
///					original file will be closed, and a new file with
///					this name will be created.
//  @return		TRUE
//  @author		Steve Schaneville
// ----------------------------------------------------------------------- //
BOOL TrimLogFile(HANDLE hFileIn, HANDLE* phFileOut, DWORD dwMaxFileSizeKb, LPCTSTR szFilename)
{
    ASSERT( (hFileIn != NULL) && (hFileIn!=INVALID_HANDLE_VALUE) );

    // Check for log file's current size.  If > MaxFileSize(), trim half of it
    DWORD dwSize = 0;
    dwSize = ::GetFileSize(hFileIn, NULL);
    if( dwSize >= dwMaxFileSizeKb && dwMaxFileSizeKb != INVALID_FILE_SIZE )
    {
        const DWORD dwBytesToRead = SSLOG_MAX_MESSAGE_LENGTH*2;
        DWORD dwBytesRead;
        DWORD dwBytesWritten;
        TCHAR szTmpFile[MAX_PATH];
        char szBufferA[dwBytesToRead];
        HANDLE hLog;
        HANDLE hLogTmp;
        BOOL bContinue;
        DWORD dwHalfSize = dwSize / 2;
        DWORD dwStartRead;
        char* szBufferPtrA = NULL;

        // close the original handle
        CloseHandle(hFileIn);

        // rename the log file to a tmp file (delete the tmp file first if one is there)
        _tcscpy(szTmpFile, szFilename);
        _tcscat(szTmpFile, _T(".tmp"));
        ::DeleteFile(szTmpFile);
        ::MoveFile(szFilename, szTmpFile);

        // open the tmp file we just created
        hLogTmp = ::CreateFile( szTmpFile, GENERIC_READ, 0, NULL,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
            NULL );

        // create the new log file and save the handle
        hLog = ::CreateFile( szFilename, GENERIC_WRITE, 0, NULL,
            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
            NULL );
        *phFileOut = hLog;

        // find the beginning of the next line half way through the file
        ::SetFilePointer( hLogTmp, dwHalfSize, NULL, FILE_BEGIN );
        ::ReadFile( hLogTmp, szBufferA, dwBytesToRead, &dwBytesRead, NULL );
        szBufferPtrA = strchr(szBufferA, '\n') + 1;
        dwStartRead = dwHalfSize + (DWORD)(szBufferPtrA-szBufferA);
        ::SetFilePointer( hLogTmp, dwStartRead, NULL, FILE_BEGIN );

        // write from tmp file to new file until we read the end
        bContinue = TRUE;
        while( bContinue )
        {
            ::ReadFile(hLogTmp, szBufferA, dwBytesToRead, &dwBytesRead, NULL);
            if(dwBytesRead)
                ::WriteFile(hFileIn, szBufferA, dwBytesRead, &dwBytesWritten, NULL);
            else
                bContinue = FALSE;
        }

        ::CloseHandle(hLogTmp);
        ::DeleteFile(szTmpFile);
    }

    return TRUE;
}
