//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2004
//////////////////////////////////////////////////////////////////////
// ppControl.cpp: implementation of the ppControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Header Files\ppControl.h"

//THIS FILE IS EXCLUDED FROM THE BUILD 
#include "shellapi.h"
#include "..\..\LicenseManager\CRC\LicManCRC.h"

#include "SS_Log.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ppControl::ppControl(bool isMSVersion, bool isStandAlone)
{
	runningStandAlone = isStandAlone;
	runningMSVersion = isMSVersion;
	myLicManProxy = NULL;
	SetHomeDirPath();
//	CreateLicenseManagerProxyAndCheckLicense();
}

/*ppControl::ppControl()
{
	Log("PPC:Creating Generic version");
	runningMSVersion = false;
	SetHomeDirPath();
	CreateLicenseManagerProxyAndCheckLicense();
}*/


void ppControl::CreateLicenseManagerProxyAndCheckLicense()
{
	LogEnterFunction("PPC:CreateLicenseManagerProxyAndCheckLicense");
	//Doing this here just so I do not have to do it twice in constructor
	Log("PPC:CoInitialise");
	::CoInitialize(NULL);  //do uninit somewhere , does not work in destructor, used in other places ?
	
	demoCounter = 0;
	// end init
	myLicManProxy = new LicenseManagerProxy();
	if(runningMSVersion)
	{
		myLicManProxy->SetRegistrationComment(L"Microsoft");
	}
	else
	{

		myLicManProxy->SetRegistrationComment(L"Widcomm");  
	}
	myLicManProxy->SetMinAllowedVersion(3.0);
	if(!myLicManProxy->CheckLicense(L"HeadsetPresenter",L"3.0"))
	{
		runInDemoMode = true;
	}
	else
	{
		runInDemoMode = false;
	}
	
	//If CRC does not match allow only demo mode for now maybe move this later so PP will not even start
	//Make sure that we crc check the correct file, the exe if it is stand alone
	_bstr_t temp;
	if(!runningStandAlone)
	{
		Log("PPC:Not Running standAlone, CRC check on HeadsetPresenter.dll");
		strcat(applicationHomeDir,"HeadsetPresenter.dll");
	}
	else
	{
		Log("PPC:Running standAlone, CRC check on HeadsetPresenter.exe");
		strcat(applicationHomeDir,"HeadsetPresenter.exe");
	}
	temp = applicationHomeDir;
	
	if(!myLicManProxy->CRCCheckSelfCRC(temp,L""))
	{
		Log("PPC:CRCCheckSelf failed"); //if this happens make sure that crc has really been appended to the dll or exe file, sometimes the second row in postbuild has not been copied
		CRCCheckOK = false;
	}
	else
	{
		Log("PPC:CRCCheckSelf OK");
		CRCCheckOK = true;
	}
}
ppControl::~ppControl()
{
	//ppControl may have been created without CreateLicenseManagerProxyAndCheckLicense()
	//have beeing called, therefor ppControl should always be deleted by the proxy BUT
	//we must check if we should delete myLicManProxy
	//If no commands has been sent to HeadsetPresenterDlg->CommandCallback, the proxy is only created not started.
	if(myLicManProxy != NULL)
	{
		delete myLicManProxy;	
	}
	delete[] applicationHomeDir;
}

void ppControl::SetHomeDirPath()
{
	applicationHomeDir = new char[512];
	HMODULE phModule;
	TCHAR fullPathToDLL[512];
	phModule = GetModuleHandle("HeadsetPresenter.dll");
	if(phModule == NULL)
	{
		phModule = GetModuleHandle("HeadsetPresenter.exe");  //When running stand alone
		if(phModule == NULL)
		{
			Log("CT:Failed to get instance path of HeadsetPresenter");
			MessageBox(NULL,"Failed to get instance path. Please contact support","HeadsetPresenter",MB_OK);
		}
	}
	if(phModule != NULL)  //We got instance for either .dll or .exe
	{
		//Get the dir path only and set \0 after the final \ in the path to remove the filename of the dll
		DWORD dwResult = ::GetModuleFileName(phModule,fullPathToDLL, sizeof(fullPathToDLL));
		char* t = _tcsrchr(fullPathToDLL,'\\');
		t++;
		*t = '\0';
		//applicationHomeDir = fullPathToDLL;
		strcpy(applicationHomeDir,fullPathToDLL);
		Log("CT:ApplicationHomeDir is: %s", applicationHomeDir);
	}
	
}

bool ppControl::StartPowerpoint2()
{
	HRESULT hr= S_OK;
	
	hr = curApp.GetActiveObject("PowerPoint.Application");
	if(FAILED(hr))
	{
		//MessageBox(NULL, "Powerpoint not started. \nPlease start PowerPoint before activating the HeadsetPresenter?","Error",MB_OK|MB_ICONINFORMATION);
		Log("PPC:PowerPoint not active");
		return false;
	}
	else
	{
		Log("PPC:PowerPoint is active");
	}
	return true;		
}
/*Starts the loaded presentation*/
void ppControl::StartActivePresentation()
{
	LogEnterFunction("PPC:StartActivePresentation");
	swp = curPres->SlideShowSettings->Run();
}

bool ppControl::GetActivePresentation()
{
	
	LogEnterFunction("PPC:GetActivePresentation");
	bool foundActivePres = true;
	try{
		curPres = curApp->GetActivePresentation();
	}
	catch (_com_error t) {
		foundActivePres = false;
		Log("PPC:Could not get active presentation Error :%s",t.ErrorMessage());
		//MessageBox(NULL,"Could not get active presentation", "Information",MB_OK);
	}

	return foundActivePres;
}

bool ppControl::GetRunningPresentation()
{
	bool foundRunningPres = true;
	try
	{
		swp = curPres->GetSlideShowWindow();
		Log("PPC:Got runnning presentation");
	}
	catch (_com_error t) {
		foundRunningPres = false;
		Log("PPC:Could not find a running presentation");
		Log("PPC:Error: %s",t.ErrorMessage());
		
		//MessageBox(NULL,"Could not get active presentation", "Information",MB_OK);
	}

	return foundRunningPres;
}

int ppControl::NextSlide2()
{	
	LogEnterFunction("PPC:NextSlide2");
	if(!CRCCheckOK)
	{
		MessageBox(NULL,"A CRC check has failed.\n\nPowerpoint will not move forward","CRC check failed",MB_OK);
		Log("PPC:CRC has failed, not moving forward");
		return -1;
	}
	
	GetActivePresentation();
	GetRunningPresentation();
	
	if(runInDemoMode)
	{
		if(demoCounter <= 5)
		{
			Log("PPC:Demo counter <= 5: %d",demoCounter);
			demoCounter++;
		}
		else
		{
			Log("PPC:Democounter = 5: %d", demoCounter);
			int boxres;
			boxres = MessageBox(NULL,"This HeadsetPresenter is not yet licensed and has is a five click limitation.\n\nIf you have purchased the HeadsetPresenter and received a registration code press the YES button below\nto complete the activation of the HeadsetPresenter and remove this message.\n\nDo you want to activate the HeadsetPresenter with your registration code?", "Bluetooth Headset Presenter", MB_YESNO|MB_TOPMOST|MB_ICONINFORMATION);
			//needed a special version of getHElp here since there was an MFC problem when passing Controller on to ppControl
			if(boxres == IDYES)
			{
				bool licenseOK;
				licenseOK = myLicManProxy->CreateLicFile(L"HeadsetPresenter",L"1.5");
				if(licenseOK)
				{
					runInDemoMode = false;
					
				}
				
			}
			return 0;
		}
	}
	
	//This should happen anyway even if in demoMode. If there has been five clicks the return 0; will break before we move again anyway
	try{
		swp->View->Next();
		Log("PPC:Moving forward");
	}
	catch (_com_error t) 
	{
		Log("PPC:Failed to move forward Exception thrown: %s",t.Description);
		//MessageBox(NULL, t.Description(), "Com Error", MB_OK);
		//MessageBox(NULL, "End of presentation reached", "End of presentation", MB_OK);
	}	
	
	return 1;
}


int ppControl::PrevSlide2()
{	
	if(runInDemoMode)
	{
		if(demoCounter > 0)
		{
			demoCounter--;
		}
	}
	
	try{
		swp->View->Previous();
		Log("PPC:Moving backwards");
	}
	catch (_com_error t) 
	{
		Log("PPC:Failed to move backward Exception thrown: %s",t.Description);
		//MessageBox(NULL, t.Description(), "Com Error", MB_OK);
		//MessageBox(NULL, "Beginning of presentation reached", "Beginning of presentation", MB_OK);
	}	
	return 1;

}
