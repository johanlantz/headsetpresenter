#include "StdAfx.h"
#include "..\header files\powerpointproxy.h"
#include "SS_Log.h"



PowerPointProxy::PowerPointProxy(void)
{
	started = false;
	forcedVGSEnabled = false;
	lastVGS = 0;
	myPPControl = new ppControl(false,true);
}

PowerPointProxy::~PowerPointProxy(void)
{
	delete myPPControl;
}
bool PowerPointProxy::isStarted()
{
	return started;
}

bool PowerPointProxy::Start()
{
	myPPControl->CreateLicenseManagerProxyAndCheckLicense();
	if(myPPControl->StartPowerpoint2())
	{
		started = true;	
	}
	else
	{
		int boxres;
		do
		{
			boxres = MessageBox(NULL, "Powerpoint is not started. \nPlease start PowerPoint before activating the HeadsetPresenter?","Error",MB_RETRYCANCEL|MB_ICONINFORMATION);
			if(boxres == IDRETRY)
			{
				started = myPPControl->StartPowerpoint2();
			}
		}while(boxres == IDRETRY && started == false);
	}
	return started;
}

void PowerPointProxy::SetForcedVGSMode(bool VGSMode)
{
	forcedVGSEnabled = VGSMode;
}
void PowerPointProxy::IncomingCommand(CString ATCommand)
{
	Log("PPP:Incoming command: %s", ATCommand);
	if(ATCommand.Find("VGS") != -1)
	{
		
		int newVGS = GetVGS(ATCommand);

		if(!forcedVGSEnabled)
		{
			Log("PPP: Not using forcedVGS");
			if(newVGS >= lastVGS)
			{
				Log("PPP:Forward");
				//MessageBox(NULL,"Forward","",MB_OK);
				myPPControl->NextSlide2();
				lastVGS = newVGS;
			}
			else
			{
				Log("PPP:Backward");
				//MessageBox(NULL,"Backward","",MB_OK);
				myPPControl->PrevSlide2();
				lastVGS =newVGS;
			}
		}
		else //using forced VGS=7 mode
		{
			Log("PPP: Using forcedVGS");
			if(newVGS > 7)
			{
				myPPControl->NextSlide2();
				Log("PPP:Forward");
			}
			else
			{
				myPPControl->PrevSlide2();
				Log("PPP:Backward");
			}
		}
	}
	else if(ATCommand.Find("CKPD") != -1)
	{
		myPPControl->NextSlide2();
		Log("PPP:Forward");
		//MessageBox(NULL,"Forward","",MB_OK);
	}
}

int PowerPointProxy::GetVGS(CString ATCommand)
{
	int VGSVal = -1;
	int pos = ATCommand.Find("VGS",0);
	CString volSub = 0;
	if(ATCommand.GetAt(pos+4) == '0')
	{
		Log("PPP:VGS has 0 in front");
		volSub = ATCommand.Mid(pos+5,1);
		VGSVal = atoi(volSub);
		Log("PPP:VGS set to %d",VGSVal);
	}
	else
	{
		
		volSub = ATCommand.Mid(pos+4,2);
		Log("PPP:Extracted vol as: %s", volSub);
		VGSVal = atoi(volSub);
		Log("PPP:VGS set to %d",VGSVal);
	}
	return VGSVal;
}