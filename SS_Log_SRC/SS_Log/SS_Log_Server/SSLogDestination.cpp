// ----------------------------------------------------------------------- //
//
//  FILENAME:	sslogdestination.cpp
//  AUTHOR:		Steve Schaneville
//  CREATED:	13 Feb 2003, 13:00
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
#include "sslogdestination.h"
#include "SS_RegistryKey.h"
#include "SS_String.h"

// ------------------[      Macros/Constants/Types     ]------------------ //
// ------------------[         Global Variables        ]------------------ //
// ------------------[         Global Functions        ]------------------ //
// ------------------[    Class Function Definitions   ]------------------ //

// Standard Constructor
SSLogDestination::SSLogDestination(DESTINATION_TYPE nType)
{
    InitObject();
    DestinationType(nType);
}

// Standard Destructor
SSLogDestination::~SSLogDestination()
{
}

// Copy Constructor
SSLogDestination::SSLogDestination( SSLogDestination& rhs )
{
    InitObject();
    *this = rhs;
}

// Assignment Operator
SSLogDestination& SSLogDestination::operator = ( SSLogDestination& rhs )
{
    InitObject();

	DestinationType(rhs.DestinationType());
	RegFilter(rhs.RegFilter());
	RegKey(rhs.RegKey());
	DefaultFlags(rhs.DefaultFlags());

    return *this;
}

// Object Initialization
VOID SSLogDestination::InitObject()
{
    DestinationType( DESTINATIONTYPE_UNDEFINED );
	RegFilter( 0 );
	DefaultFlags( 0 );
    SS_String sKey = SSLOG_REG_DEFAULT_APP_KEY;
    sKey ^= SSLOG_REG_DEFAULT_APP_SUBKEY;
	RegKey(sKey);

	// read our filter from the registry
	GetFilterFromReg();
}

BOOL SSLogDestination::WillProcessMessage(LOGMESSAGE* pMsg)
{
    // m_nDestinationType should have been set on construction.
	// Pass this into this base class' constructor from the
	// constructor of your overloaded class
    ASSERT(DestinationType() != -1);
	ASSERT(pMsg != NULL);

    BOOL	bWillProcess = FALSE; // assume that we won't process this message
    BOOL    bLevelsPass;
    BOOL    bBuildTypePasses;
    BOOL    bDestinationTypePasses;
	DWORD	dwRegFilter = RegFilter();

    // LOGTYPE_LEVELS type filters:
    // . compare current message's filter type against the registry filter's
    // . if any of the LOGTYPE_LEVELS that are in dwFilterLog are in the
	//   registry, we pass.
    bLevelsPass = dwRegFilter & pMsg->Flags() & LOGTYPEGROUP_LEVELS;

    // now make sure that the current build type is in dwFilterLog AND in the
    // registry...
    bBuildTypePasses = dwRegFilter & pMsg->Flags() & LOGTYPEGROUP_BUILDS;

    // make sure this message is intended for our destination type
    bDestinationTypePasses = dwRegFilter & pMsg->Flags() & LOGTYPEGROUP_DESTINATIONS & DestinationType();

    // now return our result
    bWillProcess = (bLevelsPass && bBuildTypePasses && bDestinationTypePasses);

	// return the result
    return bWillProcess;
}

// ----------------------------------------------------------------------- //
//  Function:		SS_Log::GetFilterFromReg
//  Author:			Steve Schaneville
//  Parameters:		dwFilter (out) - the filter in the registry
//  Return Values:	0 if successful, non-zero if not.
//  Notes:			We simply look in the registry for each entry
//                  specified by the SS_LOG_REG_FILTER_* values.  If we
//                  don't find them, we create them and give them default
//                  values.  We place all filters into the provided dwFilter
//                  for returning to the calling function.
// ----------------------------------------------------------------------- //
LONG SSLogDestination::GetFilterFromReg()
{
    DWORD dwFilters;
    LONG lResult = ::GetFiltersFromRegistry(SSLOG_REG_DEFAULT_HIVE, RegKey(), &dwFilters);
    RegFilter(dwFilters);
    return lResult;
}

BOOL SSLogDestination::OnSetRegistryKey(LOGMESSAGE* pMsg)
{
	RegKey(pMsg->Message());
    GetFilterFromReg();
    return TRUE;
}

BOOL SSLogDestination::OnEraseLog(LOGMESSAGE* pMsg)
{
	UNREFERENCED_PARAMETER(pMsg);
    return TRUE;
}

BOOL SSLogDestination::OnLimitOutputSize(LOGMESSAGE* pMsg)
{
	UNREFERENCED_PARAMETER(pMsg);
    return TRUE;
}

BOOL SSLogDestination::OnSetDestinationName(LOGMESSAGE* pMsg)
{
	UNREFERENCED_PARAMETER(pMsg);
    return TRUE;
}

BOOL SSLogDestination::OnShutDownServer()
{
    return TRUE;
}

BOOL SSLogDestination::OnBeginUpdate()
{
	return TRUE;
}

BOOL SSLogDestination::OnFinishUpdate()
{
	return TRUE;
}

BOOL SSLogDestination::OnRegistryFiltersChanged(LOGMESSAGE* pMsg)
{
    GetFilterFromReg();
    return TRUE;
}
