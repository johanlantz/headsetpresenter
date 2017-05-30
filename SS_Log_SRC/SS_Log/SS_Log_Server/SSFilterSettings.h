// ----------------------------------------------------------------------- //
//
//  FILENAME:	SSFilterSettings.h
//  AUTHOR:		Steve Schaneville
//  CREATED:	26 Jun 2002, 03:52
//
//  PURPOSE:	
//
//  Copyright (c) 2002
//
// ----------------------------------------------------------------------- //
#ifndef __SSFilterSettings_h__
#define __SSFilterSettings_h__

/////////////////////////////////////////////////////////////////////////////
// ------------------[       Pre-Include Defines       ]------------------ //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ------------------[          Include Files          ]------------------ //
/////////////////////////////////////////////////////////////////////////////
#include "SS_String.h"

/////////////////////////////////////////////////////////////////////////////
// ------------------[          Macros/Defines         ]------------------ //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ------------------[      Constants/Enumerations     ]------------------ //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ------------------[       Forward Declarations      ]------------------ //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ------------------[         Global Variables        ]------------------ //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ------------------[         Global Functions        ]------------------ //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ------------------[             Classes             ]------------------ //
/////////////////////////////////////////////////////////////////////////////
struct SSFilter
{
    SSFilter() { pButton = NULL; bIsChecked = FALSE; }
	
    SS_String   sFilterName;
    CButton*    pButton;
    BOOL        bIsChecked;
};

struct SSFilterList
{
    SSFilterList() { nCount = 0; }
	
    INT         nCount;
    SSFilter*   pFilterList[32]; // 32 because SS_Log only supports up to 32 filters
};


// ----------------------------------------------------------------------- //
//  Class:			SSFilterSettings
//  Author:			Steve Schaneville
//  Notes:			
// ----------------------------------------------------------------------- //
class SSFilterSettings
{
public:

    // construction, destruction
	SSFilterSettings								(LPCTSTR szRegKey,
													 BOOL bUseExtendedOptions);
	virtual ~SSFilterSettings						();

    // assignment, copy
	SSFilterSettings		    		            (SSFilterSettings& rhs);
	SSFilterSettings&		operator =		        (SSFilterSettings& rhs);

protected:

    // initialization
    virtual VOID			InitObject		        ();

public:

	// accessor functions
	VOID					RegKey					(SS_String sRegKey);
	SS_String				RegKey					();
	VOID					UseTextInMessage		(DWORD dwUseTextInMessage);
	DWORD					UseTextInMessage		();
	VOID					TextInMessage			(SS_String sTextInMessage);
	SS_String				TextInMessage			();
	VOID					UseFilename				(DWORD dwUseFilename);
	DWORD					UseFilename				();
	VOID					Filename				(SS_String sFilename);
	SS_String				Filename				();
	VOID					UseIDBetween			(DWORD dwUseIDBetween);
	DWORD					UseIDBetween			();
	VOID					IDLower					(DWORD dwIDLower);
	DWORD					IDLower					();
	VOID					IDUpper					(DWORD dwIDUpper);
	DWORD					IDUpper					();
	VOID					FilterList				(SSFilterList& filterList );
	SSFilterList&			FilterList				();
	VOID					UseExtendedOptions		(BOOL bUseExtendedOptions);
	BOOL					UseExtendedOptions		();
	VOID					RequireAllFilters		(DWORD dwRequireAllFilters);
	DWORD					RequireAllFilters		();
	VOID					Filter					(DWORD dwFilter);
	DWORD					Filter					();
    VOID					AutoApply				(DWORD dwAutoApply);
    DWORD					AutoApply				();

    // utilities
	BOOL					WriteSettingsToReg		(LPCTSTR szRegKey = NULL);
	BOOL					ReadSettingsFromReg		(LPCTSTR szRegKey = NULL);
	BOOL					InitFilters				(LPCTSTR szRegKeyToCopyFrom);

protected:

	BOOL					CleanFilters			();
	BOOL					UpdateFilterMask		();

private:

	SS_String		m_sRegKey;

	DWORD			m_dwTextInMsg;
	SS_String		m_sTextInMsg;
	DWORD			m_dwFilename;
	SS_String		m_sFilename;
	DWORD			m_dwIDBetween;
	DWORD			m_dwIDLower;
	DWORD			m_dwIDUpper;
    DWORD           m_dwAutoApply;
    SSFilterList    m_FilterList;
	DWORD			m_dwRequireAllFilters;
	BOOL			m_bUseExtendedOptions;

	DWORD			m_dwFilter;

};


// ----------------------------------------------------------------------- //
//  SSFilterSettings Inline Functions
// ----------------------------------------------------------------------- //

inline VOID	SSFilterSettings::RegKey(SS_String sRegKey)
{ m_sRegKey = sRegKey; }

inline SS_String SSFilterSettings::RegKey()
{ return m_sRegKey; }

inline VOID SSFilterSettings::UseTextInMessage(DWORD dwUseTextInMessage)
{ m_dwTextInMsg = dwUseTextInMessage; }

inline DWORD SSFilterSettings::UseTextInMessage()
{ return m_dwTextInMsg; }

inline VOID SSFilterSettings::TextInMessage(SS_String sTextInMessage)
{ m_sTextInMsg = sTextInMessage; }

inline SS_String SSFilterSettings::TextInMessage()
{ return m_sTextInMsg; }

inline VOID SSFilterSettings::UseFilename(DWORD dwUseFilename)
{ m_dwFilename = dwUseFilename; }

inline DWORD SSFilterSettings::UseFilename()
{ return m_dwFilename; }

inline VOID SSFilterSettings::Filename(SS_String sFilename)
{ m_sFilename = sFilename; }

inline SS_String SSFilterSettings::Filename()
{ return m_sFilename; }

inline VOID SSFilterSettings::UseIDBetween(DWORD dwUseIDBetween)
{ m_dwIDBetween = dwUseIDBetween; }

inline DWORD SSFilterSettings::UseIDBetween()
{ return m_dwIDBetween; }

inline VOID SSFilterSettings::IDLower(DWORD dwIDLower)
{ m_dwIDLower = dwIDLower; }

inline DWORD SSFilterSettings::IDLower()
{ return m_dwIDLower; }

inline VOID SSFilterSettings::IDUpper(DWORD dwIDUpper)
{ m_dwIDUpper = dwIDUpper; }

inline DWORD SSFilterSettings::IDUpper()
{ return m_dwIDUpper; }

inline VOID	SSFilterSettings::FilterList(SSFilterList& filterList)
{ m_FilterList = filterList; }

inline SSFilterList& SSFilterSettings::FilterList()
{ return m_FilterList; }

inline VOID SSFilterSettings::UseExtendedOptions(BOOL bUseExtendedOptions)
{ m_bUseExtendedOptions = bUseExtendedOptions; }

inline BOOL SSFilterSettings::UseExtendedOptions()
{ return m_bUseExtendedOptions; }

inline VOID SSFilterSettings::RequireAllFilters(DWORD dwRequireAllFilters)
{ m_dwRequireAllFilters = dwRequireAllFilters; }

inline DWORD SSFilterSettings::RequireAllFilters()
{ return m_dwRequireAllFilters; }

inline VOID SSFilterSettings::Filter(DWORD dwFilter)
{ m_dwFilter = dwFilter; }

inline DWORD SSFilterSettings::Filter()
{ return m_dwFilter; }

inline VOID SSFilterSettings::AutoApply(DWORD dwAutoApply)
{ m_dwAutoApply = dwAutoApply; }

inline DWORD SSFilterSettings::AutoApply()
{ return m_dwAutoApply; }


#endif // __SSFilterSettings_h__
