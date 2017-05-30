//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2004
//////////////////////////////////////////////////////////////////////
// ppControl.h: interface for the ppControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PPCONTROL_H__7BBBA29A_BEF6_44C0_B4C5_C1EF4AA6ADA5__INCLUDED_)
#define AFX_PPCONTROL_H__7BBBA29A_BEF6_44C0_B4C5_C1EF4AA6ADA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//Temporary Common Files and Office files for XP are in the C:\program files folder
//Office 2k is not installed here
#import "C:\Program Files\Common Files\Microsoft Shared\Office10\mso.dll" rename("RGB","RGBy") 
#import "C:\Program Files\Common Files\Microsoft Shared\VBA\VBA6\VBE6EXT.OLB" 
#import "C:\Program Files\Microsoft Office\Office10\msppt.olb" rename("RGB","RGBt")

#include ".\LicenseManager\Header Files\LicenseManagerProxy.h"



class btcontrol;
class Controller;


class ppControl  
{
public:
	ppControl(bool isMSVersion, bool isStandAlone);
	//ppControl();  // for generic stack
	virtual ~ppControl();

	int NextSlide2();
	int PrevSlide2();

	bool GetActivePresentation();
	bool GetRunningPresentation();
	bool StartPowerpoint2();

	void StartActivePresentation();
	
	void CreateLicenseManagerProxyAndCheckLicense();
	
private:
	
	
	PowerPoint::SlideShowWindowPtr swp;

	PowerPoint::_ApplicationPtr curApp;
	PowerPoint::_ApplicationPtr startApp;
	PowerPoint::_PresentationPtr curPres;
	PowerPoint::PresentationsPtr activePresentations;
	btcontrol* myBtControl;
	bool runInDemoMode;
	LicenseManagerProxy* myLicManProxy;
	int demoCounter;
	bool runningMSVersion;
	bool CRCCheckOK;
	void SetHomeDirPath();
	char* applicationHomeDir;
	bool runningStandAlone;
};

#endif 
