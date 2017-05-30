// ----------------------------------------------------------------------- //
//
//  FILENAME:	SSLogDestinationFile.cpp
//  AUTHOR:		Steve Schaneville
//  CREATED:	15 Feb 2003, 18:25
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
#include "SSLogDestinationFile.h"
#include "SS_String.h"

// ------------------[      Macros/Constants/Types     ]------------------ //
// ------------------[         Global Variables        ]------------------ //
// ------------------[         Global Functions        ]------------------ //
// ------------------[    Class Function Definitions   ]------------------ //

// Standard Constructor
SSLogDestinationFile::SSLogDestinationFile() :
	SSLogDestination(LOGTYPE_LOGTOFILE)
{
    InitObject();
}

// Standard Destructor
SSLogDestinationFile::~SSLogDestinationFile()
{
    FileHandle(NULL);
}

// Object Initialization
VOID SSLogDestinationFile::InitObject()
{
	m_hFile = NULL;
	MaxFileSizeKb(SSLOG_MAX_LOG_SIZE_DEFAULT);
	Filename(SSLOG_DEFAULT_DESTINATION_NAME);
}

// Copy Constructor
SSLogDestinationFile::SSLogDestinationFile( SSLogDestinationFile& rhs ) :
	SSLogDestination(rhs.DestinationType())
{
    InitObject();
    *this = rhs;
}

// Assignment Operator
SSLogDestinationFile& SSLogDestinationFile::operator = ( SSLogDestinationFile& rhs )
{
    InitObject();

	FileHandle(rhs.FileHandle());
	Filename(rhs.Filename());
	MaxFileSizeKb(rhs.MaxFileSizeKb());

    return *this;
}

// ----------------------------------------------------------------------- //
//  Function:		SSLogDestinationFile::Filename
//  Author:			Steve Schaneville
//  Parameters:		szFilename - name of file that the message will be
//                               written to.
//  Return Values:	none
//  Notes:			We will append ".log" if no extention exists
// ----------------------------------------------------------------------- //
VOID SSLogDestinationFile::Filename(LPCTSTR szFilename)
{
    ASSERT(szFilename);
    ASSERT(_tcslen(szFilename) < MAX_PATH);

    // add ".log" if no extention is there, then save the filename into
    // our member variable m_szLogFile
    TCHAR szFile[MAX_PATH];
    _tcscpy(szFile, szFilename);
    _tcstok( szFile, _T(".") );

    if( !_tcscmp(szFile,szFilename) )
    {
        _tcscat(szFile, _T(".log") );
        _tcscpy(m_szFilename, szFile);
    }
    else
        _tcscpy(m_szFilename, szFilename);

    // make sure that we don't use the old file handle in the future
    FileHandle(NULL);
}

// ----------------------------------------------------------------------- //
//  Function:		SSLogDestinationFile::WriteToFile
//  Author:			Steve Schaneville
//  Parameters:		szBufferA [in] - the buffer to be written to the file
//  Return Values:	none
//  Notes:
// ----------------------------------------------------------------------- //
VOID SSLogDestinationFile::WriteToFile(LOGMESSAGE* pMsg)
{
    // open the log file if not already open
    if( FileHandle() == NULL || FileHandle() == INVALID_HANDLE_VALUE )
    {
        HANDLE hLog = ::CreateFile( Filename(), GENERIC_WRITE,
            FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
            OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL,
            NULL );
        FileHandle(hLog);
    }

    if( FileHandle() == NULL || FileHandle() == INVALID_HANDLE_VALUE )
        return;

	// trim our log file if necessary
	HANDLE hFileNew = NULL;
	TrimLogFile(FileHandle(), &hFileNew, MaxFileSizeKb(), Filename());
	if( hFileNew != NULL && hFileNew != INVALID_HANDLE_VALUE )
		FileHandle(hFileNew);

    // write the log and flush!
    if( INVALID_HANDLE_VALUE != FileHandle() && NULL != FileHandle() )
    {
        DWORD cbWritten;
        DWORD dwResult;
		SS_String sMessage;
		SS_String sFlagsText;
		GetFlagsText(pMsg->Flags(), sFlagsText.GetBuffer(256), 256);
        sFlagsText.ReleaseBuffer();

		sMessage = pMsg->MessageID();
		sMessage += _T("\t");
		sMessage += pMsg->DateTime();
		sMessage += _T("\t");
		sMessage += sFlagsText;
		sMessage += _T("\t");
		sMessage += pMsg->Filename();
		sMessage += _T("\t");
		sMessage += pMsg->LineNumber();
		sMessage += _T("\t");
		sMessage += pMsg->Message();

        // SetFilePointer is necessary for cases where we have two logs
        // writing to the same file.  We must make sure that we
        // move to the end of the file in case the other log has written
        // to it since we last did... we don't want to set on that log's
        // information.
        dwResult = ::SetFilePointer( FileHandle(), 0, NULL, FILE_END );
        ::WriteFile( FileHandle(), (LPCVOID)sMessage.GetBufferConst(),
            sMessage.Length(),
            &cbWritten, NULL );
        ::WriteFile( FileHandle(), "\n",
            strlen("\n"),
            &cbWritten, NULL );
    }
}

VOID SSLogDestinationFile::LimitFileSize(const DWORD dwMaxFileSize)
{
    if( dwMaxFileSize == SSLOG_MAX_LOG_SIZE_NO_LIMIT )
        MaxFileSizeKb(dwMaxFileSize);
    else
        MaxFileSizeKb(dwMaxFileSize*1020); // file size in KB on disk
}

BOOL SSLogDestinationFile::OnWriteMessage(LOGMESSAGE* pMsg)
{
	UNREFERENCED_PARAMETER(pMsg);
	BOOL br = TRUE;

	WriteToFile(pMsg);

	return br;
}

BOOL SSLogDestinationFile::OnEraseLog(LOGMESSAGE* pMsg)
{
	UNREFERENCED_PARAMETER(pMsg);
	BOOL br = TRUE;

    FileHandle(NULL);
    ::DeleteFile(Filename());

	return br;
}

BOOL SSLogDestinationFile::OnSetDestinationName(LOGMESSAGE* pMsg)
{
	UNREFERENCED_PARAMETER(pMsg);
	ASSERT(pMsg->Message());
	BOOL br = TRUE;

	Filename(pMsg->Message());

	return br;
}

BOOL SSLogDestinationFile::OnLimitOutputSize(LOGMESSAGE* pMsg)
{
    UNREFERENCED_PARAMETER(pMsg);
    BOOL br = TRUE;
    ASSERT(pMsg->Message());

    SS_String sSizeLimit = pMsg->Message();
    LimitFileSize(sSizeLimit);

    return br;
}

BOOL SSLogDestinationFile::OnShutDownServer(LOGMESSAGE* pMsg)
{
	UNREFERENCED_PARAMETER(pMsg);
	BOOL br = TRUE;

	return br;
}

BOOL SSLogDestinationFile::OnFinishUpdate()
{
	BOOL br = TRUE;
	br = ::FlushFileBuffers(FileHandle());
	return br;
}