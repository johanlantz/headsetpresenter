// ----------------------------------------------------------------------- //
//
//  FILENAME:	SSLogDestinationFile.h
//  AUTHOR:		Steve Schaneville
//  CREATED:	15 Feb 2003, 18:25
//
//  PURPOSE:	
//
//  Copyright (c) 2003
//
// ----------------------------------------------------------------------- //
#ifndef __SSLogDestinationFile_h__
#define __SSLogDestinationFile_h__

// ------------------[       Pre-Include Defines       ]------------------ //
// ------------------[          Include Files          ]------------------ //
#include "SSLogDestination.h"

// ------------------[          Macros/Defines         ]------------------ //
// ------------------[      Constants/Enumerations     ]------------------ //
// ------------------[       Forward Declarations      ]------------------ //
// ------------------[         Global Variables        ]------------------ //
// ------------------[         Global Functions        ]------------------ //
// ------------------[             Classes             ]------------------ //

// ----------------------------------------------------------------------- //
//  Class:			SSLogDestinationFile
//  Author:			Steve Schaneville
//  Notes:			
// ----------------------------------------------------------------------- //
class SSLogDestinationFile : public SSLogDestination
{
public:

    // construction, destruction
	SSLogDestinationFile								();
	virtual ~SSLogDestinationFile						();

    // assignment, copy
	SSLogDestinationFile		    		            (SSLogDestinationFile& rhs);
	SSLogDestinationFile&		operator =		        (SSLogDestinationFile& rhs);

protected:

    // initialization
    virtual VOID				InitObject		        ();

public:

	// accessor functions
	LPTSTR						Filename				();
	VOID						Filename				(LPCTSTR szFilename);

    // utilities
    VOID						LimitFileSize			(const DWORD dwMaxFileSize);

	// overrides
    virtual BOOL				OnWriteMessage          (LOGMESSAGE* pMsg);
    virtual BOOL				OnEraseLog              (LOGMESSAGE* pMsg);
    virtual BOOL				OnLimitOutputSize       (LOGMESSAGE* pMsg);
    virtual BOOL				OnSetDestinationName    (LOGMESSAGE* pMsg);
    virtual BOOL				OnShutDownServer        (LOGMESSAGE* pMsg);
	virtual BOOL				OnFinishUpdate			();

protected:

	// accessor functions
	const HANDLE				FileHandle				() const;
	VOID						FileHandle				(const HANDLE hFile);
    const DWORD					MaxFileSizeKb			() const;
    VOID						MaxFileSizeKb			(DWORD dwMaxFileSizeKb);

    // utilities
	VOID						WriteToFile				(LOGMESSAGE* pMsg);

private:

	HANDLE		m_hFile;
    DWORD       m_dwMaxFileSizeKb;		// don't let files grow beyond this level (in Kilobytes)
    TCHAR		m_szFilename[MAX_PATH]; // the filename to write to

};


// ----------------------------------------------------------------------- //
//  SSLogDestinationFile Inline Functions
// ----------------------------------------------------------------------- //

// return the handle to the log file
inline const HANDLE SSLogDestinationFile::FileHandle() const
{ return m_hFile; }

// set the handle to the log file
inline VOID SSLogDestinationFile::FileHandle(const HANDLE hFile)
{
    if( hFile == NULL && FileHandle() != NULL ) 
        CloseHandle(FileHandle());
    m_hFile = hFile; 
}

inline const DWORD SSLogDestinationFile::MaxFileSizeKb() const
{ return m_dwMaxFileSizeKb; }

inline VOID SSLogDestinationFile::MaxFileSizeKb(const DWORD dwMaxFileSizeKb)
{ m_dwMaxFileSizeKb = dwMaxFileSizeKb; }

// return the log window's name
inline LPTSTR SSLogDestinationFile::Filename()
{ return m_szFilename; }


#endif // __SSLogDestinationFile_h__
