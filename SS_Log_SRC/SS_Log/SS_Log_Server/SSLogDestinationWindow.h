// ----------------------------------------------------------------------- //
//
//  FILENAME:	SSLogDestinationWindow.h
//  AUTHOR:		Steve Schaneville
//  CREATED:	15 Feb 2003, 18:24
//
//  PURPOSE:
//
//  Copyright (c) 2003
//  Version 3.02
//
// ----------------------------------------------------------------------- //
#ifndef __SSLogDestinationWindow_h__
#define __SSLogDestinationWindow_h__

// ------------------[       Pre-Include Defines       ]------------------ //
// ------------------[          Include Files          ]------------------ //
#include "SSLogDestination.h"

// ------------------[          Macros/Defines         ]------------------ //
// ------------------[      Constants/Enumerations     ]------------------ //
// ------------------[       Forward Declarations      ]------------------ //
// ------------------[         Global Variables        ]------------------ //
// ------------------[         Global Functions        ]------------------ //
// ------------------[             Classes             ]------------------ //
class SS_Log_ServerDoc;

// ----------------------------------------------------------------------- //
//  Class:			SSLogDestinationWindow
//  Author:			Steve Schaneville
//  Notes:
// ----------------------------------------------------------------------- //
class SSLogDestinationWindow : public SSLogDestination
{
public:

    // construction, destruction
	SSLogDestinationWindow								();
	virtual ~SSLogDestinationWindow						();

    // assignment, copy
	SSLogDestinationWindow				    		    (SSLogDestinationWindow& rhs);
	SSLogDestinationWindow&		operator =				(SSLogDestinationWindow& rhs);

protected:

    // initialization
    virtual VOID				InitObject				();

public:

	// accessor functions

	// overrides
    virtual BOOL				OnWriteMessage          (LOGMESSAGE* pMsg);
    virtual BOOL				OnEraseLog              (LOGMESSAGE* pMsg);
    virtual BOOL				OnFinishUpdate			();
    virtual BOOL				OnBeginUpdate			();
	virtual BOOL				OnSetRegistryKey		(LOGMESSAGE* pMsg);
    virtual BOOL				OnRegistryFiltersChanged(LOGMESSAGE* pMsg);
	//virtual BOOL				WillProcessMessage		(LOGMESSAGE* pMsg);

    // utilities

protected:

	SS_Log_ServerDoc*			Document				();
	VOID						Document				(SS_Log_ServerDoc* pDoc);

private:

	SS_Log_ServerDoc*			m_pDocument;

};


// ----------------------------------------------------------------------- //
//  SSLogDestinationWindow Inline Functions
// ----------------------------------------------------------------------- //

inline SS_Log_ServerDoc* SSLogDestinationWindow::Document()
{ return m_pDocument; }

inline VOID SSLogDestinationWindow::Document(SS_Log_ServerDoc* pDoc)
{ m_pDocument = pDoc; }


#endif // __SSLogDestinationWindow_h__
