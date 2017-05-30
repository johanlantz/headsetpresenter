// ----------------------------------------------------------------------- //
//
//  FILENAME:	SSFilterSettings.cpp
//  AUTHOR:		Steve Schaneville
//  CREATED:	26 Jun 2002, 03:53
//
//  PURPOSE:
//
//  Copyright (c) 2002
//  Version 3.02
//
// ----------------------------------------------------------------------- //

/////////////////////////////////////////////////////////////////////////////
// ------------------[       Pre-Include Defines       ]------------------ //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ------------------[          Include Files          ]------------------ //
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SSFilterSettings.h"
#include "SS_RegistryKey.h"

/////////////////////////////////////////////////////////////////////////////
// ------------------[      Macros/Constants/Types     ]------------------ //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ------------------[         Global Variables        ]------------------ //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ------------------[         Global Functions        ]------------------ //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ------------------[    Class Function Definitions   ]------------------ //
/////////////////////////////////////////////////////////////////////////////

// Standard Constructor
SSFilterSettings::SSFilterSettings(LPCTSTR szRegKey, BOOL bUseExtendedOptions)
{
    InitObject();
	RegKey(szRegKey);
	UseExtendedOptions(bUseExtendedOptions);
	ReadSettingsFromReg(RegKey());
}

// Standard Destructor
SSFilterSettings::~SSFilterSettings()
{
	CleanFilters();
}

// Object Initialization
VOID SSFilterSettings::InitObject()
{
	UseTextInMessage(FALSE);
	UseFilename(FALSE);
	UseIDBetween(FALSE);
	IDLower(0);
	IDUpper(0);
	Filter(0);
	RequireAllFilters(FALSE);
}

// Copy Constructor
SSFilterSettings::SSFilterSettings( SSFilterSettings& rhs )
{
    InitObject();
    *this = rhs;
}

// Assignment Operator
SSFilterSettings& SSFilterSettings::operator = ( SSFilterSettings& rhs )
{
    InitObject();

	UseTextInMessage(rhs.UseTextInMessage());
	UseFilename(rhs.UseFilename());
	UseIDBetween(rhs.UseIDBetween());
	TextInMessage(rhs.TextInMessage());
	Filename(rhs.Filename());
	IDUpper(rhs.IDUpper());
	IDLower(rhs.IDLower());
	RequireAllFilters(rhs.RequireAllFilters());

	RegKey(rhs.RegKey());
	FilterList(rhs.FilterList());

    return *this;
}

BOOL SSFilterSettings::ReadSettingsFromReg(LPCTSTR szRegKey)
{
	BOOL br = TRUE;

	CleanFilters();

	SS_String sKeyPath;
	if( szRegKey != NULL )
		sKeyPath = szRegKey;
	else
		sKeyPath = RegKey();

    LONG lResult;
	SS_RegistryKey sKey(SSLOG_REG_DEFAULT_HIVE);
    lResult = sKey.OpenKey(sKeyPath);
    INT nValueIndex = 0;
    SS_String sValueName;
    INT nSize = 256;
    DWORD dwValueNameSize = nSize;
    DWORD dwValue = 0;
    DWORD dwValueSize = sizeof(DWORD);
    DWORD dwType = 0;

    while( lResult == ERROR_SUCCESS )
    {
        dwValueNameSize = nSize;
        dwValueSize = sizeof(DWORD);
        lResult = sKey.EnumValue(nValueIndex, sValueName.GetBuffer(dwValueNameSize),
			&dwValueNameSize, &dwType,
			(LPBYTE)&dwValue, &dwValueSize);
        sValueName.ReleaseBuffer();

        // fill out list
        if( lResult == ERROR_SUCCESS )
        {
            // SS_Log does not support more than 32 filters, so we should not
            // be reading in more than 32 of them.
            ASSERT(m_FilterList.nCount <= 32);

            m_FilterList.pFilterList[nValueIndex] = new SSFilter;
            m_FilterList.pFilterList[nValueIndex]->sFilterName = sValueName;
            m_FilterList.pFilterList[nValueIndex]->bIsChecked = dwValue > 0 ? TRUE : FALSE;
            m_FilterList.nCount++;
        }

        // increment our index
        nValueIndex++;
    }

	if( UseExtendedOptions() )
	{
		SS_RegistryKey sKeyExt(SSLOG_REG_DEFAULT_HIVE);
		sKeyPath ^= _T("Extended");
		lResult = sKeyExt.CreateKey(sKeyPath);
		lResult = sKeyExt.OpenKey(sKeyPath);

		LPTSTR szValue = NULL;

        dwValue = 0;
		sKeyExt.QueryValue(_T("UseTextInMessage"), &dwValue);
		UseTextInMessage(dwValue);
        szValue = NULL;
		sKeyExt.QueryValue(_T("TextInMessage"), &szValue);
		TextInMessage(szValue);
		dwValue = 0;
		sKeyExt.QueryValue(_T("UseFilename"), &dwValue);
		UseFilename(dwValue);
        szValue = NULL;
		sKeyExt.QueryValue(_T("Filename"), &szValue);
		Filename(szValue);
        dwValue = 0;
		sKeyExt.QueryValue(_T("UseIDBetween"), &dwValue);
		UseIDBetween(dwValue);
        dwValue = 0;
		sKeyExt.QueryValue(_T("IDLower"), &dwValue);
		IDLower(dwValue);
		dwValue = 0;
		sKeyExt.QueryValue(_T("IDUpper"), &dwValue);
		IDUpper(dwValue);
		dwValue = 1;
		lResult = sKeyExt.QueryValue(_T("RequireAllFilters"), &dwValue);
        if( lResult == ERROR_FILE_NOT_FOUND )
            dwValue = 1; // the default
		RequireAllFilters(dwValue);
        dwValue = 0;
		lResult = sKeyExt.QueryValue(_T("AutoApply"), &dwValue);
		AutoApply(dwValue);
	}

	UpdateFilterMask();

	return br;
}

BOOL SSFilterSettings::WriteSettingsToReg(LPCTSTR szRegKey)
{
    BOOL br = TRUE;

	SS_String sKeyPath;
	if( szRegKey != NULL )
		sKeyPath = szRegKey;
	else
		sKeyPath = RegKey();

    LONG lResult;
    SS_RegistryKey sKey(SSLOG_REG_DEFAULT_HIVE);
    lResult = sKey.OpenKey(sKeyPath);

    for( INT i=0; i < m_FilterList.nCount; i++ )
    {
        sKey.SetValue(m_FilterList.pFilterList[i]->sFilterName,
			m_FilterList.pFilterList[i]->bIsChecked);
    }

    if( UseExtendedOptions() == TRUE )
    {
	    SS_RegistryKey sKeyExt(SSLOG_REG_DEFAULT_HIVE);
	    sKeyPath ^= _T("Extended");
        lResult = sKeyExt.OpenKey(sKeyPath);

	    sKeyExt.SetValue(_T("UseTextInMessage"), UseTextInMessage());
	    sKeyExt.SetValue(_T("TextInMessage"), (LPCTSTR)TextInMessage());
	    sKeyExt.SetValue(_T("UseFilename"), UseFilename());
	    sKeyExt.SetValue(_T("Filename"), (LPCTSTR)Filename());
	    sKeyExt.SetValue(_T("UseIDBetween"), UseIDBetween());
	    sKeyExt.SetValue(_T("IDLower"), IDLower());
	    sKeyExt.SetValue(_T("IDUpper"), IDUpper());
	    sKeyExt.SetValue(_T("RequireAllFilters"), RequireAllFilters());
        sKeyExt.SetValue(_T("AutoApply"), AutoApply());
    }

	UpdateFilterMask();

	return br;
}

BOOL SSFilterSettings::InitFilters(LPCTSTR szRegKeyToCopyFrom)
{
	BOOL br = TRUE;

	SS_RegistryKey sKey(SSLOG_REG_DEFAULT_HIVE);
	SS_String sKeyPath = RegKey();
	sKeyPath ^= _T("Extended");
	sKey.OpenKey(sKeyPath);
	DWORD dwValue;
	LONG lResult = sKey.QueryValue(_T("UseTextInMessage"), &dwValue);
	if( lResult != ERROR_SUCCESS )
	{
		ReadSettingsFromReg(szRegKeyToCopyFrom);
		WriteSettingsToReg(RegKey());
		ReadSettingsFromReg(RegKey());
	}

	return br;
}

BOOL SSFilterSettings::CleanFilters()
{
	BOOL br = TRUE;

	INT nCount = m_FilterList.nCount;
	for( INT i=0; i<nCount; i++ )
	{
		delete m_FilterList.pFilterList[i];
	}

	m_FilterList.nCount = 0;

	return br;
}

BOOL SSFilterSettings::UpdateFilterMask()
{
	BOOL br = TRUE;
	SS_String sName;
    Filter(0);

	INT nCount = GetFiltersCount();
	for( INT i=0; i<m_FilterList.nCount; i++ )
	{
		sName = g_Filters[i].szReg;
		for( INT j=0; j<nCount; j++ )
		{
			if( sName == m_FilterList.pFilterList[j]->sFilterName )
			{
				if( m_FilterList.pFilterList[j]->bIsChecked )
					Filter( Filter() | g_Filters[i].nType );
			}
		}
	}

	return br;
}