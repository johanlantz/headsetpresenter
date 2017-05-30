// ----------------------------------------------------------------------- //
//
//  FILENAME:	SSLogDestinationWindow.cpp
//  AUTHOR:		Steve Schaneville
//  CREATED:	15 Feb 2003, 18:23
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
#include "SSLogDestinationWindow.h"
#include "SS_Log_ServerDoc.h"

// ------------------[      Macros/Constants/Types     ]------------------ //
// ------------------[         Global Variables        ]------------------ //
extern SS_Log_ServerDoc* g_pDoc;

// ------------------[         Global Functions        ]------------------ //
// ------------------[    Class Function Definitions   ]------------------ //

// Standard Constructor
SSLogDestinationWindow::SSLogDestinationWindow() :
	SSLogDestination(LOGTYPE_LOGTOWINDOW)
{
    InitObject();
}

// Standard Destructor
SSLogDestinationWindow::~SSLogDestinationWindow()
{
}

// Copy Constructor
SSLogDestinationWindow::SSLogDestinationWindow( SSLogDestinationWindow& rhs ) :
	SSLogDestination(rhs.DestinationType())
{
    InitObject();
    *this = rhs;
}

// Assignment Operator
SSLogDestinationWindow& SSLogDestinationWindow::operator = ( SSLogDestinationWindow& rhs )
{
	UNREFERENCED_PARAMETER(rhs);
    InitObject();

    return *this;
}

// Object Initialization
VOID SSLogDestinationWindow::InitObject()
{
	Document(g_pDoc);
}

BOOL SSLogDestinationWindow::OnWriteMessage(LOGMESSAGE* pMsg)
{
	UNREFERENCED_PARAMETER(pMsg);
	BOOL br = TRUE;

	Document()->AddMessage(pMsg);

	return br;
}

BOOL SSLogDestinationWindow::OnEraseLog(LOGMESSAGE* pMsg)
{
	UNREFERENCED_PARAMETER(pMsg);
	BOOL br = TRUE;

	Document()->EraseLog();

	return br;
}

BOOL SSLogDestinationWindow::OnBeginUpdate()
{
	BOOL br = TRUE;

	Document()->BeginUpdate();

	return br;
}

BOOL SSLogDestinationWindow::OnFinishUpdate()
{
	BOOL br = TRUE;

	Document()->FinishUpdate();

	return br;
}

BOOL SSLogDestinationWindow::OnSetRegistryKey(LOGMESSAGE* pMsg)
{
	BOOL br = TRUE;
	SSLogDestination::OnSetRegistryKey(pMsg);

	Document()->SetRegistryKey(pMsg);

	return br;
}

BOOL SSLogDestinationWindow::OnRegistryFiltersChanged(LOGMESSAGE* pMsg)
{
    BOOL br = TRUE;
    SSLogDestination::OnRegistryFiltersChanged(pMsg);

    Document()->RegistryFiltersChanged(pMsg);

    return br;
}

/*
BOOL SSLogDestinationWindow::WillProcessMessage(LOGMESSAGE* pMsg)
{
    // m_nDestinationType should have been set on construction.
	// Pass this into this base class' constructor from the
	// constructor of your overloaded class
    ASSERT(DestinationType() != -1);
	ASSERT(pMsg != NULL);

	// make sure this message is intended for our destination type
    BOOL bDestinationTypePasses;
	DWORD dwRegFilter = RegFilter();
    bDestinationTypePasses = dwRegFilter & pMsg->Flags() & LOGTYPEGROUP_DESTINATIONS & DestinationType();

	return bDestinationTypePasses;
}
*/
