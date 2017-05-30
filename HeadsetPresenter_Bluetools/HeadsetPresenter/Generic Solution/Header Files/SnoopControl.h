//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2005
//////////////////////////////////////////////////////////////////////
// SnoopControl.h: interface for the SnoopControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SNOOPCONTROL_H__ED10FA32_5C03_43D2_9782_096C3488E387__INCLUDED_)
#define AFX_SNOOPCONTROL_H__ED10FA32_5C03_43D2_9782_096C3488E387__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "devicemgr.h"
#include "SettingsControl.h"

class SnoopControl  
{
public:
	SnoopControl();
	virtual ~SnoopControl();

	bool InstallDriver();
	void UninstallDriver();
	void RestartDevice();
	void SetSettingsControl(SettingsControl* iSettingsControl);
private:
	
	//CDevice device;
	CString m_sFilterName;
    CString m_sLowerFilters;
	BOOL m_bIsWin2K;

	CString sDriver;
	SettingsControl* mySettingsControl;



};

#endif // !defined(AFX_SNOOPCONTROL_H__ED10FA32_5C03_43D2_9782_096C3488E387__INCLUDED_)
