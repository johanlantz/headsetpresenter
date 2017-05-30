#include "StdAfx.h"
#include ".\controllerfactory.h"
#include "ManualStackDialog.h"
#include "InfoDialog.h"

#include "SS_Log.h"

ControllerFactory::ControllerFactory(SettingsControl* iSettingsControl)
{
	manuallySelectedStack = NO_MANUAL_STACK_SELECTED;
	mySettingsControl = iSettingsControl;
	IsManualStackSettingsNeeded();

	myBluetoolsController = NULL;
#ifdef MS_STACK
	myMicrosoftController = NULL;
#endif
	myGenericController = NULL;
	myBSController = NULL;
}

ControllerFactory::~ControllerFactory(void)
{
	if(myBluetoolsController != NULL)
	{
		delete myBluetoolsController;
	}
#ifdef MS_STACK
	if(myMicrosoftController != NULL)
	{
		delete myMicrosoftController;
	}
#endif
	if(myGenericController != NULL)
	{
		delete myGenericController;
	}
	if(myBSController != NULL)
	{
		delete myBSController;
	}
}

/*Currently IsManualStackSettingsNeeded() does not stora any value, it just informs the user of Toshiba and
BlueSoleil specific things, this might need improvement.*/
void ControllerFactory::IsManualStackSettingsNeeded()
{
	if(mySettingsControl->GetLastControllerMode() == ERROR_MODE)
	{
		if(!(CheckRegistryForWidcomm() || CheckMicrosoftSupport() || CheckBlueSoleilSupport() || CheckToshibaSupport()))
		{
			ManualStackDialog myManualStackDialog;
			myManualStackDialog.SetText(TEXT_NO_KNOWN_STACK_FOUND);
			myManualStackDialog.DoModal();
			manuallySelectedStack = myManualStackDialog.GetSelectedStack();
			Log("CF:Manually selected stack with id= %d",manuallySelectedStack);
			switch(manuallySelectedStack) //Unsure if manuallySelectedStack will be used later or if this will just be used for setting manual unplug
			{
				case WIDCOMM :
					Log("CF:User selected Widcomm in Manual mode");
					//MessageBox(NULL,"If you are using Widcomm drivers HeadsetPresenter should have been able to detect that automatically.\n\nPlease contact support if any problems occur.","Microsoft",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
					break;
				case MICROSOFT :
					Log("CF:User selected Microsoft in Manual mode");
					//MessageBox(NULL,"If you are using Microsoft drivers HeadsetPresenter should have been able to detect that automatically.\n\nPlease contact support if any problems occur.","Microsoft",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
					break;
				case BLUESOLEIL : 
					{
						Log("CF:User selected Bluesoleil in Manual mode");
						InfoDialog myInfoDlg;
						myInfoDlg.SetSettingsControl(mySettingsControl);
						myInfoDlg.SetText("BlueSoleil","Important information","BlueSoleil has been successfully tested with HeadsetPresenter in Normal & Advanced mode.\n\nHowever please note the following:\n1. BlueSoleil offers a very limited and unstable interface for development. This could affect the stability in normal mode.\n2. After activating Advanced mode for the first time you will be requested to reboot Windows. Until that has been done HeadsetPresenter will not be fully installed.\n\nBlueSoleil might require a little tweaking the first time it is used and if there are any problems check the BlueSoleil chapter in the documentation or contact support."); 
						if(myInfoDlg.ShowBox())
						{
							myInfoDlg.DoModal();
						}
						//mySettingsControl->SetLastReplugMode(REPLUG_MANUAL);
					}
				break;
				case TOSHIBA :
					Log("CF:User selected Toshiba in Manual mode");
					MessageBox(NULL,"Toshiba drivers has been successfully tested with HeadsetPresenter in Advanced mode.\n\nHowever issues have been seen on older versions of the Toshiba drivers.\nIt is highly recommended that you update your Toshiba Bluetooth software to the latest version if you encounter any problems with the HeadsetPresenter.\n\nImportant information:\nSupport for Toshiba drivers in Normal mode is under development and will hopefully be available during Q3 2008.\nIf you are interested in this funtionality or have problems in Advanced mode please contact support and we will inform you when the Toshiba update is ready.","Toshiba",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
				break;
				case  AVM :
				break;
				default :
					MessageBox(NULL,"Your Bluetooth drivers has not been tested with the HeadsetPresenter.\n\nMost likely HeadsetPresenter will work in Advanced mode.\n\nContact support if there are any problems","Other driver",MB_OK);
			}
			mySettingsControl->SetStackName(manuallySelectedStack);
		}
	}
}



SuperController* ControllerFactory::GetController(int controllerMode)
{
	switch(controllerMode)
	{
		case NORMAL_MODE_WIDCOMM:
			if(myBluetoolsController == NULL)
			{
				myBluetoolsController = new BluetoolsController();
			}
			mySettingsControl->SetStackName(WIDCOMM);
			return myBluetoolsController;
		break;
#ifdef MS_STACK
		case NORMAL_MODE_MICROSOFT:
			if(myMicrosoftController == NULL)
			{
				myMicrosoftController = new MicrosoftController();
			}
			mySettingsControl->SetStackName(MICROSOFT);
			return myMicrosoftController;

		break;
#endif
		case NORMAL_MODE_BLUESOLEIL:
			if(myBSController == NULL)
			{
				Log("CF:Creating new BSController, should never happen");
				myBSController = new BSController();
			}
			mySettingsControl->SetStackName(BLUESOLEIL);
			return myBSController;  //Already created in CheckSupport, also Initialised already
		case ADVANCED_MODE:
			if(myBluetoolsController == NULL)
			{
				myGenericController = new GenericController();
			}
			//No stack name is set, keep old value if a manual select was performed
			return myGenericController;
		break;
		default:
			MessageBox(NULL,"Unknown controller, this should never happen please contact support","",MB_OK);
			Log("CF:Controller Error id:%d",controllerMode);
			return NULL; //to remove warning, will crash but should never happen
	}	
}

int ControllerFactory::GetSupportedModes()
{
	supportedModes = ERROR_MODE;
	int OSId = GetOSId();
	
	//1. Check if we have Widcomm
	if(CheckRegistryForWidcomm())
	{
		Log("CF:Widcomm set");
		supportedModes |= NORMAL_MODE_WIDCOMM;
	}

	if(CheckBlueSoleilSupport())
	{
		Log("CF:BlueSoleil set");
		supportedModes |= NORMAL_MODE_BLUESOLEIL;
	}

	
	//2. Check if we can use snoop driver
	if(OSId >= WINDOWS_2000)  
	{
		Log("CF:OSId>=WINDOWS_2000, allow advanced mode");
		supportedModes |= ADVANCED_MODE;
	}
	
	if(OSId & WINDOWS_VISTA)  //Currently unknown how snoop will run on vista
	{
		//Log("CF:Vista detected, disabling advanced mode");
		//supportedModes ^= ADVANCED_MODE; //it was set above when finding 2000, XOR to disable again
		//MessageBox(NULL,"Advanced mode is not tested on Windows Vista.\n\nAdvanced mode will be possible but status of functionality is unknown","",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
	}
	
	//3.Check Microsoft drivers
	if(OSId >= WINDOWS_XP_SP2)
	{
		if(CheckMicrosoftSupport())
		{
			Log("CF:OSId>=WINDOWS_XP_SP2 and MS support, activate MS, deactivate ADVANCED_MODE");
			supportedModes |= NORMAL_MODE_MICROSOFT;
			supportedModes ^= ADVANCED_MODE;  //Do not use advanced mode if we have Microsoft drivers available
		}
	}

	if(OSId < WINDOWS_SERVER_2003)//using old (1.8) snoop driver
	{
		//Forcing settings, the user can choose NEVER on Widcomm and 2k but it will be reset when wizard is run
		if(supportedModes & NORMAL_MODE_WIDCOMM)
		{
			Log("CF:OS>=Server2003 and widcomm, set restart_device_always");
			mySettingsControl->SetRestartDeviceHandling(RESTART_DEVICE_ALWAYS);
		}
		else
		{
			Log("CF:OS>=Server2003 and not widcomm, restart NEVER");
			mySettingsControl->SetRestartDeviceHandling(RESTART_DEVICE_NEVER);
		}
	}
	Log("CF:Returning supportedModes = %d", supportedModes);
	supportedModes = SanityCheckSupportedModes(supportedModes);
	return supportedModes;
}
/*This function handles the case when the user has more then one Bluetooth stack installed
*/
int ControllerFactory::SanityCheckSupportedModes(int supportedModes)
{
	int supportedNormalModes = supportedModes ^ ADVANCED_MODE;
	bool multipleStacksInstalled = false;
	Log("CF: SanityCheckSupportedModes %d", supportedModes);
	Log("CF:SupportedNormalModes are %d", supportedNormalModes);

	if(supportedModes & NORMAL_MODE_MICROSOFT)
	{
		if((supportedModes ^ NORMAL_MODE_MICROSOFT) & supportedNormalModes)
		{
			multipleStacksInstalled = true;
		}
	}
	else if(supportedModes & NORMAL_MODE_WIDCOMM)
	{
		if((supportedModes ^ NORMAL_MODE_WIDCOMM) & supportedNormalModes)
		{
			multipleStacksInstalled = true;
		}
	}
	else if(supportedModes & NORMAL_MODE_BLUESOLEIL)
	{
		if((supportedModes ^ NORMAL_MODE_BLUESOLEIL) & supportedNormalModes)
		{
			multipleStacksInstalled = true;
		}
	}
	else if(supportedModes & NORMAL_MODE_TOSHIBA)
	{
		if((supportedModes ^ NORMAL_MODE_TOSHIBA) & supportedNormalModes)
		{
			multipleStacksInstalled = true;
		}
	}

	if(multipleStacksInstalled)
	{
		Log("CF:User has multiple stacks installed");
		ManualStackDialog myManualStackDialog;
		myManualStackDialog.SetText(TEXT_MORE_THEN_ONE_STACK_FOUND);
		myManualStackDialog.DoModal();
		manuallySelectedStack = myManualStackDialog.GetSelectedStack();
		Log("CF:Manually selected stack with id= %d",manuallySelectedStack);
		switch(manuallySelectedStack) //Unsure if manuallySelectedStack will be used later or if this will just be used for setting manual unplug
		{
			case WIDCOMM :
				Log("CF:User selected Widcomm reactivate advanced mode");
				supportedModes = NORMAL_MODE_WIDCOMM | ADVANCED_MODE; 
				Log("CF:New supported modes are WIDCOMM|ADVANCED %d",supportedModes);
				break;
			case MICROSOFT :
				Log("CF:User selected Microsoft.");
				supportedModes = NORMAL_MODE_MICROSOFT;
				Log("CF:New supported mode should be 2 (Microsoft) and it is %d",supportedModes);
				break;
			case BLUESOLEIL :
				Log("CF:User selected BlueSoleil.");
				supportedModes = NORMAL_MODE_BLUESOLEIL | ADVANCED_MODE;
				Log("CF:New supported mode should be SOLEIL|ADVANCED and it is %d",supportedModes);
				break;
			case TOSHIBA :
				Log("CF:User selected Toshiba NOTE NORMAL MODE IS NOT ACTIVATED HERE.");
				supportedModes = /*NORMAL_MODE_TOSHIBA |*/ ADVANCED_MODE;
				MessageBox(NULL,"Toshiba drivers has been successfully tested with HeadsetPresenter in Advanced mode.\n\nHowever issues have been seen on older versions of the Toshiba drivers.\nIt is highly recommended that you update your Toshiba Bluetooth software to the latest version if you encounter any problems with the HeadsetPresenter.\n\nImportant information:\nSupport for Toshiba drivers in Normal mode is under development and will hopefully be available during Q3 2008.\nIf you are interested in this funtionality or have problems in Advanced mode please contact support and we will inform you when the Toshiba update is ready.","Toshiba",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
				Log("CF:New supported mode should be 16 (advanced no Tosh yet) and it is %d",supportedModes);
				break;
			default :
				MessageBox(NULL,"Your Bluetooth drivers has not been tested with the HeadsetPresenter.\n\nMost likely HeadsetPresenter will work in Advanced mode.\n\nContact support if there are any problems","Other driver",MB_OK);
				Log("CF:Untested stack, defaulting to ADVANCED_MODE");
				supportedModes = ADVANCED_MODE;
		}
		mySettingsControl->SetStackName(manuallySelectedStack);
	}

	//Old impl
	/*if(supportedModes & NORMAL_MODE_MICROSOFT)
	{
		if(supportedModes & NORMAL_MODE_WIDCOMM)
		{
			Log("CF:User has both Microsoft and Widcomm installed");
			ManualStackDialog myManualStackDialog;
			myManualStackDialog.SetText(TEXT_MORE_THEN_ONE_STACK_FOUND);
			myManualStackDialog.DoModal();
			manuallySelectedStack = myManualStackDialog.GetSelectedStack();
			Log("CF:Manually selected stack with id= %d",manuallySelectedStack);
			switch(manuallySelectedStack) //Unsure if manuallySelectedStack will be used later or if this will just be used for setting manual unplug
			{
				case WIDCOMM :
					Log("CF:User selected Widcomm when 2 stacks were available, reactivate advanced mode");
					supportedModes = supportedModes | ADVANCED_MODE; //advanced mode was deactivated when the MS stack was found
					supportedModes ^= NORMAL_MODE_MICROSOFT;
					Log("CF:New supported mode should be 9 and it is %d",supportedModes);
					break;
				case MICROSOFT :
					Log("CF:User selected Microsoft remove Widcomm support.");
					supportedModes ^= NORMAL_MODE_WIDCOMM;
					Log("CF:New supported mode should be 2 and it is %d",supportedModes);
					break;
				default :
					MessageBox(NULL,"Your Bluetooth drivers has not been tested with the HeadsetPresenter.\n\nMost likely HeadsetPresenter will work in Advanced mode.\n\nContact support if there are any problems","Other driver",MB_OK);
			}
			mySettingsControl->SetStackName(manuallySelectedStack);
		}
	}*/
	return supportedModes;
}

int ControllerFactory::GetOSId()
{
	int OSId;
	//#define BUFSIZE 80
   OSVERSIONINFOEX osvi;
   BOOL bOsVersionInfoEx;

   // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
   // If that fails, try using the OSVERSIONINFO structure.

   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

   if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
   {
      osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
      if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
         return FALSE;
   }

   switch (osvi.dwPlatformId)
   {
      // Test for the Windows NT product family.
      case VER_PLATFORM_WIN32_NT:
		// Test for the specific product family.
		
		if(osvi.dwMajorVersion == 6)
		{
			Log("CF:Windows Vista");
			OSId = WINDOWS_VISTA;
		}
		else if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
		{
			OSId = WINDOWS_SERVER_2003;
			Log("CF:Microsoft Windows Server2003 family");
		}
		else if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
		{
			if(osvi.wServicePackMajor == 2)
			{
				OSId = WINDOWS_XP_SP2;
				Log("CF:Microsoft Windows XP SP2 installed");
			}
			else
			{
				Log("CF:Microsoft Windows XP no SP installed");
				OSId = WINDOWS_XP;
			}
		}
		else if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
		{
			Log("CF:Microsoft Windows 2000 ");
			OSId = WINDOWS_2000;
		}
		else if ( osvi.dwMajorVersion <= 4 )
		{
			OSId = WINDOWS_NT;
			Log("CF:Microsoft Windows NT ");
		}
		break;

	case VER_PLATFORM_WIN32_WINDOWS:

        if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
        {
			Log("CF:Microsoft Windows 95 ");
			OSId = WINDOWS_95;
        } 

        else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
        {
			Log("CF:Microsoft Windows 98 ");
            OSId = WINDOWS_98;
        } 

        else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
        {
			Log("CF:Microsoft Windows Millennium Edition\n");
			OSId = WINDOWS_MILLENIUM;
		} 
	break;
	default:
		OSId = WINDOWS_2000;
   }
   Log("CF:Returning OSId = %d",OSId);
   mySettingsControl->SetOSId(OSId);
   return OSId;
}

bool ControllerFactory::CheckRegistryForWidcomm()
{
	HKEY hKey;
	DWORD dwBufferSize = 260;
	BYTE* buf;
	buf = new BYTE[MAX_PATH];
	long res;
	bool widcommOK = true;

	res = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Widcomm\\Install",0,KEY_READ,&hKey);
	if(res != ERROR_SUCCESS)
	{
		res = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Widcomm\\Version",0,KEY_READ,&hKey);
		if(res != ERROR_SUCCESS)
		{
			widcommOK = false;
		}
		else  //Path is Widcomm\Version\Software
		{
			res = RegQueryValueEx(hKey,"Software",NULL,NULL,buf,&dwBufferSize);
			if(res != ERROR_SUCCESS)
			{
				widcommOK=false;
			}
		}
	}
	else //Path is Widcomm/Install\VERSION
	{
		res = RegQueryValueEx(hKey,"VERSION",NULL,NULL,buf,&dwBufferSize);
		if(res != ERROR_SUCCESS)
		{
			widcommOK = false;
		}
	}
	if(widcommOK)  //Stack must be newer then 1.4.2.10
	{
		Log("CF:Widcomm stack found version = %s",buf);
		int major = 0;
		int minor = 0;
		int revision = 0;
		int build = 0;

		char *token;

		token = strtok( (char*)buf, "." );
		for(int i=0;i < 4; i++)
		{
			if(i==0)
			{
				major = atoi(token);
			}
			else if(i==1)
			{
				minor = atoi(token);
			}
			else if(i==2)
			{
				revision = atoi(token);
			}
			else if(i==3)
			{
				build = atoi(token);
			}
			token = strtok( NULL, "." );
		}
		if(major > 1)
		{
			widcommOK = true;
		}
		else if(major == 1)
		{
			if(minor > 4 )
			{
				widcommOK = true;
			}
			else if(minor == 4)
			{
				if(revision > 2)
				{
					widcommOK = true;
				}
				else if(revision == 2)
				{
					if(build > 9)
					{
						widcommOK = true;
					}
				}
			}
		}
		else
		{
			MessageBox(NULL,"HeadsetPresenter has detected an old Widcomm/ Broadcomm stack.\nTo be able to use all features of HeadsetPresenter 3.0 you must update your Bluetooth software to a Widcomm stack newer then 1.4.2.9.\n\nAdvanced mode will still be available without upgrading the drivers.","",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
			widcommOK = false;
		}
	}
	else
	{
		Log("CF:No Widcomm stack found");
		widcommOK = false;
	}
	RegCloseKey(hKey);
	delete buf;
	if(widcommOK)
	{
		Log("CF:Widcomm ok in registry");
	}
	else
	{
		Log("CF:Widcomm NOT in registry");
	}
	return widcommOK;
}

bool ControllerFactory::CheckRegistryForBlueSoleil()
{
	HKEY hKey;
	DWORD dwBufferSize = 260;
	BYTE* buf;
	buf = new BYTE[MAX_PATH];
	long res;
	bool bsOK = true;

	res = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\IVT Corporation\\BlueSoleil\\Install",0,KEY_READ,&hKey);
	if(res != ERROR_SUCCESS)
	{
		bsOK = false;
	}
	else 
	{
		res = RegQueryValueEx(hKey,"ProductVersion",NULL,NULL,buf,&dwBufferSize);
		if(res != ERROR_SUCCESS)
		{
			bsOK = false;
		}
		else
		{
			MessageBox(NULL,"CF:BlueSoleil","",MB_OK);
			Log("CF:Found BlueSoleil entry in registry");
		}
	}
	return bsOK;
}

bool ControllerFactory::CheckMicrosoftSupport()
{
	bool microsoftAvailable = false;
#ifdef MS_STACK
	int OSId = GetOSId();
	if(OSId >= WINDOWS_XP_SP2)
	{
		MicrosoftController myTempMicrosoftController;
		microsoftAvailable = myTempMicrosoftController.IsRadioAvailable();
	}
#endif
	
	return microsoftAvailable;
}

/*The BSController is always created here and if it is BlueSoleil it is reused when the controllerFactory returns*/
bool ControllerFactory::CheckBlueSoleilSupport()
{
	bool blueSoleilAvailable = false;
	
	myBSController = new BSController();
	blueSoleilAvailable = myBSController->IsRadioAvailable();	
		
	return blueSoleilAvailable;
}

bool ControllerFactory::CheckToshibaSupport()
{
	//Remember to fix
	//1. multipleStacks must enable NORMAL_MODE_TOSHIBA
	return false;
}