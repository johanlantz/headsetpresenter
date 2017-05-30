//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include ".\applicationcontrol.h"
#include "SS_Log.h"
#include "defines.h"
//#include "InfoDialog.h"  //compile errors something with the includes
#include ".\pushkeys\Header Files\PushDLL.h"


//#include "comutil.h"

char ApplicationControl::szTitle[100];

char ApplicationControl::szKeys[100];
BOOL ApplicationControl::useStar = FALSE;
//DWORD ApplicationControl::dwSent = 0;




ApplicationControl::ApplicationControl(SettingsControl* iSettingsControl)
{
	mySettingsControl = iSettingsControl;
	forwardButtonVK = "VK_N";
	backwardButtonVK = "VK_P";
	connectButtonVK = "VK_N";
	CString playButtonVK = "VK_N";
	CString stopButtonVK = "VK_N";
	CString pauseButtonVK = "VK_N";
	CString ffwdButtonVK = "VK_N";
	CString rewButtonVK = "VK_N";
	grammarFileName = "NA";
	forcedVGSEnabled = false;
	forwardOnly = false;
	inWizardMode = true;  //assume we are in wizard mode until we get a ConnectionCompleted
	lastVGS = 0; // can this cause trouble?
	lastVGSTime = 0;
	windowMatch = wmPKPartialMatch;
	SetHomeDirPath();
	CreateLicenseManagerProxyAndCheckLicense();
}

ApplicationControl::~ApplicationControl(void)
{
	Log("APC:About to do CoUninitialize");
	::CoUninitialize();
}

void ApplicationControl::SendToWindow()
{
		//setup PushDLL PushInfo structure
	PushInfo PI;
	PI.WindowMatch=(WindowMatch)windowMatch;
	PI.WindowTitle=szTitle;
	//not using ANSI substitution
	PI.UseANSI=FALSE;
	//we will NOT return to this application after key presses
	PI.ReturnFocus=FALSE;
	//ensure target window is always in the foreground
	PI.TrackTarget=FALSE;  //Changed this from true
	//send keys as quickly as possible, might need to change this if sending sequence, better if user adds Sleep himself and this is kept to 0
	PI.KeyDelay=0;
	//key press text to pass to dll
	PI.Keys=szKeys;
	//callback keyevent handler (set this to NULL if no handler provided)
	//PI.KeyEvent=KeyEventHandler;
	PI.KeyEvent=NULL;
	//send keys to target application, handle of target window returned
	HWND tw=PushKeysEx(&PI);

	//Fallback solutions for PowerPoint in other languages, special handling
	if(tw == NULL && strstr(szTitle, "PowerPoint"))
	{
		Log("APC:Failed to find Window name trying to mofify to known alternatives");
		PI.WindowTitle="Presentazione di PowerPoint";
		tw=PushKeysEx(&PI);
	}
	//more special if cases could be added here
	
	if (tw==NULL)
	{
		Log("APC: The target window %s was not found, wildcard was %d",szTitle,(WindowMatch)windowMatch);
		//InfoDialog myInfoDlg;
		//myInfoDlg.SetSettingsControl(mySettingsControl);
		//myInfoDlg.SetBoxId(INFO_DLG_APP_NOT_FOUND);
		CString msg = "HeadsetPresenter detected that a button was pressed but:\nThe window with a name ";
		if(PI.WindowMatch == wmPKExactMatch)
		{
			msg += CString("exactly matching ");
		}
		else if(PI.WindowMatch == wmPKStartMatch)
		{
			msg += CString("starting with ");
		}
		else if(PI.WindowMatch == wmPKPartialMatch)
		{
			msg += CString("containing ");
		}
		else
		{
			Log("APC:Errounous windowmatch param");
			MessageBox(NULL,"Non valid window match param","",MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
		}
		msg += CString("\"") + CString(szTitle) + CString("\"");
		msg += CString(" could not be found.\n\n");
		msg += CString("Most likely this is because:\n1. The window is not active, for instance if you are controlling PowerPoint the presentation window is not active.\nThis is not an error, just start your presentation and click again.\nIf this message pops up even when you have the slideshow running please see 2. below.");
		msg += CString("\n\n2. The window name is not correct. The most common problem is that a local language version of the application is used and the window is named something different then in the preconfigured english version.\nFor instance the PowerPoint presentation window is sometimes renamed in non english speaking countries. This is very simple to change and it is described in detail in chapter 5.1 in the documentation");
		MessageBox(NULL,msg,"Window not found",MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
		/*InfoDialog myInfoDlg;
		myInfoDlg.SetSettingsControl(mySettingsControl);
		myInfoDlg.SetBoxId(INFO_DLG_WINDOW_NOT_FOUND);
		myInfoDlg.SetText("Window not found",msg); 
		if(myInfoDlg.ShowBox())
		{
			myInfoDlg.DoModal();
		}*/

	}
}

void ApplicationControl::SetSettingsControl(SettingsControl* iSettingsControl)
{
	mySettingsControl = iSettingsControl;
}

void ApplicationControl::SetInWizardMode(bool iWizardMode)
{
	inWizardMode = iWizardMode;
}


void ApplicationControl::CreateLicenseManagerProxyAndCheckLicense()
{
	LogEnterFunction("APC:CreateLicenseManagerProxyAndCheckLicense");
	Log("APC:CoInitialise");
	::CoInitialize(NULL);  //do uninit somewhere , does not work in destructor, used in other places ?
	
	demoCounter = 0;
	
	myLicManProxy = new LicenseManagerProxy();
	
	//myLicManProxy->SetRegistrationComment(GetRegistrationComment()); //moving this to before CreateLicFile so that as much info as possible has been collected
	myLicManProxy->SetMinAllowedVersion(mySettingsControl->GetMinAllowedProductVersionAsDouble());  //reuse product version here
	if(!myLicManProxy->CheckLicense(L"HeadsetPresenter",(bstr_t)mySettingsControl->GetProductVersion()))
	{
		Log("APC:No license found");
		runInDemoMode = true;
	}
	else
	{
		Log("APC:Valid license found"); //if running from VS this can happen if installer has run and lic file is available in Program Files folder. LicenseFile will be checked for in the folder where LicenceManager.dll was registered? Potential problem with multiple products?
		runInDemoMode = false;
	}
	
	//If CRC does not match allow only demo mode for now maybe move this later so PP will not even start
	//Make sure that we crc check the correct file, the exe if it is stand alone
	_bstr_t temp;
	//applicationHomeDir = getPath().GetBuffer();
	Log("APC:CRC check on HeadsetPresenter.exe");
	strcat(applicationHomeDir,"HeadsetPresenter.exe");
	
	temp = applicationHomeDir;
	
	if(!myLicManProxy->CRCCheckSelfCRC(temp,L""))
	{
		Log("APC:CRCCheckSelf failed"); //if this happens make sure that crc has really been appended to the dll or exe file, sometimes the second row in postbuild has not been copied
		CRCCheckOK = false;
	}
	else
	{
		Log("APC:CRCCheckSelf OK");
		CRCCheckOK = true;
	}
}

bstr_t ApplicationControl::GetRegistrationComment()
{
	CString regComment;
	regComment += GetRegistrationCommentSegment(CString("ver"),mySettingsControl->GetProductVersion());
	regComment += GetRegistrationCommentSegment(CString("stack"),"",mySettingsControl->GetStackName());
	regComment += GetRegistrationCommentSegment(CString("hsname"),mySettingsControl->GetLastDeviceName());
	regComment += GetRegistrationCommentSegment(CString("fonly"),"",(int)mySettingsControl->GetForwardOnly());
	regComment += GetRegistrationCommentSegment(CString("fvgs"),"",(int)mySettingsControl->GetForcedVGS());
	regComment += GetRegistrationCommentSegment(CString("os"),"",(int)mySettingsControl->GetOSId());
	regComment += GetRegistrationCommentSegment(CString("usbdev"),mySettingsControl->GetLastControllerName());
	regComment += GetRegistrationCommentSegment(CString("volnorm"),"",(int)mySettingsControl->GetVolSupportInNormalMode());
	Log("APC:RegComment = %s",regComment);
	return (bstr_t)regComment;
}

CString ApplicationControl::GetRegistrationCommentSegment(CString key, CString valueAsString, int valueAsInt)
{
	CString temp;
	temp = key + CString(":=**");
	if(valueAsInt == NO_VALUE)  //Value is a string
	{
		temp = temp + valueAsString;
	}
	else
	{	
		char buf[10];
		itoa(valueAsInt,buf,10);
		temp = temp + CString(buf);
	}
	temp = temp + CString("**");
	Log("APC:Built segment : %s",temp);
	return temp;
}

void ApplicationControl::GetMappings(CString appName)
{
	try
	{
		CString path = getPath();
		path = path + "ButtonMappings.txt";
		CFile myFile(path, CFile::modeCreate | CFile::modeNoTruncate| CFile::shareDenyNone);
		//int fileSize = (int)myFile.GetLength();  /need to create dynamic array for this to work
		int bytesRead=0;
		char s[10000];  //Perhaps this should be dynamic
		CString settings;
		bytesRead = myFile.Read(s,10000);
		
		if(bytesRead == 10000)
		{
			MessageBox(NULL,"Static file size could be exceded for ButtonMappings.txt.\nIf you really have a need for a configuration this big please contact support.","",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		}

		if(bytesRead > 0)
		{
			s[bytesRead] = 0;
			settings = s;
			//Log("APC:ButtonMappings.txt read as : %s",settings);
			Log("APC:GetMappings for: %s",appName);
			int startAt = settings.Find(appName,0);
			if(startAt == APPLICATION_NOT_FOUND)
			{
				Log("APC:Application %s was not found in settings %s", appName, settings);
				Log("APC:Check so the ButtonMapping is not > 5000 chars");
				MessageBox(NULL,"There is an error in ButtonMappings.txt.\nThe application " + appName + " could not be found.\nPlease check your ButtonMapping.txt file and contact support if necessary.","Error: application not found", MB_OK|MB_ICONINFORMATION);
			}
			else  //Get key mappings for appName
			{
					SetWindowMatch(GetDataFromIdentifierWithOffset("WindowMatch : ", settings, startAt));
					forwardButtonVK = GetDataFromIdentifierWithOffset("VolumeUpButton : ", settings, startAt);
					backwardButtonVK = GetDataFromIdentifierWithOffset("VolumeDownButton : ", settings, startAt);
					connectButtonVK = GetDataFromIdentifierWithOffset("ConnectButton : ", settings, startAt);
					playButtonVK = GetDataFromIdentifierWithOffset("PlayButton : ", settings, startAt);
					stopButtonVK = GetDataFromIdentifierWithOffset("StopButton : ", settings, startAt);
					pauseButtonVK = GetDataFromIdentifierWithOffset("PauseButton : ", settings, startAt);
					ffwdButtonVK = GetDataFromIdentifierWithOffset("FFWDButton : ", settings, startAt);
					rewButtonVK = GetDataFromIdentifierWithOffset("REWButton : ", settings, startAt);
					grammarFileName = GetDataFromIdentifierWithOffset("GrammarFile : ", settings, startAt);	
			}
		}
		
		myFile.Close();
	}
	catch(CFileException* pEx)
	{
		pEx->Delete();
		// Simply show an error message to the user.
	}
}

CString ApplicationControl::GetGrammarName()
{
	if(grammarFileName == "NA" || grammarFileName == "" || grammarFileName.Right(4) != ".xml")
	{
		MessageBox(NULL, "Invalid grammar name or no grammar defined for this application, using HPGrammar.xml","currentAppGrammar",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		Log("SAPI:Invalid grammar name %s",grammarFileName);
		grammarFileName = "HPGrammar.xml";
	}
	return grammarFileName;
}


void ApplicationControl::SetWindowMatch(CString windowMatchFromFile)
{
	if(windowMatchFromFile == CString("ExactMatch"))
	{
		windowMatch = (WindowMatch)wmPKExactMatch;
	}
	else if(windowMatchFromFile == CString("PartialMatch"))
	{
		windowMatch = (WindowMatch)wmPKPartialMatch;
	}
	else if(windowMatchFromFile == CString("StartMatch"))
	{
		windowMatch = (WindowMatch)wmPKStartMatch;
	}
	else
	{
		MessageBox(NULL,"An errounous WindowMatch parameter was discovered.\nPlease check the spelling of element:\n" + windowMatchFromFile + CString("\nIt must match either of these options exactly:\n1. StartMatch\n2. PartialMatch\n3. ExactMatch\nHeadsetPresenter will default to PartialMatch.\n\nUse the Add button in the main window to edit the settings and correct this error."),"Error in WindowMatch parameter",MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
		windowMatch = (WindowMatch)wmPKPartialMatch;
	}
}

CString ApplicationControl::GetDataFromIdentifierWithOffset(CString dataToGet, CString settings, int startAt)
{
	int startPos = settings.Find(dataToGet,startAt) + dataToGet.GetLength(); 
	int endPos = settings.Find("\r\n",startPos);

	if((startPos - dataToGet.GetLength()) == -1 )
	{
		Log("APC:Syntax error for element %s", dataToGet);
		MessageBox(NULL,"There is a possible syntax error in ButtonMappings.txt for the element " + dataToGet + "\nMake sure that all mappings for each application is available in the ButtonMappings file.\nAlso make sure that the : sign is preceeded and followed by a whitespace.\n\nAll correct mappings will be usable regardless of this error.","Error: Syntax error in ButtonMappings.txt", MB_OK|MB_ICONINFORMATION);
		return CString("EOF");
	}
	Log("APC: Returning ButtonMap : %s for %s", settings.Mid(startPos, endPos-startPos), dataToGet); 
	return settings.Mid(startPos, endPos-startPos);
}

void ApplicationControl::SetForcedVGSMode(bool VGSMode)
{
	Log("APC:SetForcedVGS");
	forcedVGSEnabled = VGSMode;
}

void ApplicationControl::SetForwardOnlyMode(bool newForwardOnly)
{
	Log("APC:SetForwardOnly");
	forwardOnly = newForwardOnly;
}


bool ApplicationControl::IncomingCommand(CString Command, int commandType)
{
	bool commandOkToSend = true;
	Log("APC:Incoming command: %s", Command);
	
	if(!CheckCRCAndDemo())  
	{
		return false;
	}

	if(commandType == AT_COMMAND)
	{
		if(Command.Find("VGS") != -1)
		{
			int newVGS = GetVGS(Command);
			
			if(mySettingsControl->GetBlockDoubleClicks()) //should be a setting
			{
				Log("APC:Double movement check enabled");
				time(&thisVGSTime);
				if(thisVGSTime - lastVGSTime < 2)
				{
					Log("APC:Possible double movement thisVGSTime=%d, lastVGSTime=%d, command will be ignored", thisVGSTime, lastVGSTime);
					return false;
				}
				time(&lastVGSTime);
			}

			//1. If forward only do not care about VGS number
			if(forwardOnly)
			{
				strcpy(szKeys,forwardButtonVK.GetBuffer());
			}
			//2. Not using forced VGS must check also if we are hitting bottom otherwise we could move forward when hitting 1 or 0
			else if(!forcedVGSEnabled)
			{
				Log("APC: Not using forcedVGS");
				if(newVGS > lastVGS)
				{
					Log("APC:Forward");
					strcpy(szKeys,forwardButtonVK.GetBuffer());
					lastVGS = newVGS;
				}
				else if(newVGS < lastVGS)
				{
					
					strcpy(szKeys,backwardButtonVK.GetBuffer());
					lastVGS =newVGS;
				}
				else  //VGS are equal
				{
					Log("APC:VGS are equal, check to see direction");
					if(newVGS > 7)
					{
						Log("APC:Forward");
						strcpy(szKeys,forwardButtonVK.GetBuffer());
					}
					else
					{
						Log("APC:Backward");
						strcpy(szKeys,backwardButtonVK.GetBuffer());
					}
					lastVGS =newVGS;
				}
			}
			//3. Using forced VGS just check if higher or lower, unsure if this can apply to headset profile, Nokia supports it? Should be implemented also for Headset
			else 
			{
				Log("APC: Using forcedVGS");
				if(newVGS > 1)  //Reserve 0 and 1 for backward moving if newVGS=lastVGS
				{
					strcpy(szKeys,forwardButtonVK.GetBuffer());
					Log("PPP:Forward");
				}
				else
				{
					strcpy(szKeys,backwardButtonVK.GetBuffer());
					Log("PPP:Backward");
				}
			}
		}
		else if(Command.Find("CKPD") != -1)
		{
			if(!CheckCRCAndDemo())
			{
				Log("APC:CheckCRCAndDemo returned false, exiting");
				return false;
			}
			strcpy(szKeys,connectButtonVK.GetBuffer());
			Log("APC:Forward");
			//MessageBox(NULL,"Forward","",MB_OK);
		}
		else if(Command.Find("BVRA") != -1)  //BVRA not impl in wizard yet
		{
			if(!CheckCRCAndDemo())
			{
				return false;
			}
			strcpy(szKeys,connectButtonVK.GetBuffer());
			Log("APC:Forward");
		}
		else
		{
			//demoCounter--; //if no action was performed, decrease demoCounter
		}
	}
	else if(commandType == AVRCP_COMMAND)
	{
		Log("APC:AVRCP command recieved");
		int opIdPos = Command.Find("00 48 7c");
		CString opId;
		unsigned long i;
		if(opIdPos != -1)
		{
			opIdPos += 9;
			opId = Command.Mid(opIdPos,2);
			Log("APC:Extracted op_id as :%s",opId);
			i = strtoul(opId,'\0',16);
			Log("APC:Converted opId to %h", i);
			switch(i)
			{
				case 0x44 :
					Log("APC:Play %s",playButtonVK);
					strcpy(szKeys,playButtonVK.GetBuffer());
				break;
				case 0x45 :
					Log("APC:Stop %s",stopButtonVK);
					strcpy(szKeys,stopButtonVK.GetBuffer());
				break;
				case 0x46 :
					Log("APC:Pause %s", pauseButtonVK);
					strcpy(szKeys,pauseButtonVK.GetBuffer());
				break;
				case 0x4b :
					Log("APC:Ffwd",ffwdButtonVK);
					strcpy(szKeys,ffwdButtonVK.GetBuffer());
				break;
				case 0x4c :
					Log("APC:Rew %s",rewButtonVK);
					strcpy(szKeys,rewButtonVK.GetBuffer());
				break;
				default:
					Log("An unknown or doublet AVRCP command was received %d",opId);
					demoCounter--;
					commandOkToSend = false;
			}
		}
		else
		{
			Log("An unhandled AVRCPcommand was received %d",opId);
			commandOkToSend = false;
		}
	}
	else if(commandType == SAPI_COMMAND)
	{
		strcpy(szKeys,Command.GetBuffer());
	}
	else
	{
		MessageBox(NULL,"An unknown	commandType was received this should never happen, please contact support to resolve this","error",MB_OK);
		Log("APC:Unknown command %d was reveived, should never happen",commandType);
		commandOkToSend = false;
	}

	if(commandOkToSend)
	{
		Log("APC:Command to send is %s",szKeys);
		Log("APC:about to enum windows");
		SendToWindow();
		
		/*EnumWindows(ListWindows,0);
		
		if (dwSent==0) {
			//MessageBox(NULL,"No window","",MB_OK);
			Log("APC:Failed to send message to %s",szTitle);
		}
		else
		{
			//MessageBox(NULL,"MSG OK","",MB_OK);
			Log("APC:Sent message to %s",szTitle);
		}*/
	}
	return true;
}

void ApplicationControl::SetApplicationName(CString appName)
{
	Log("APC:SetApplicationName = %s",appName);
	strcpy(szTitle,appName.GetBuffer());
	/*if (szTitle[strlen(szTitle)-1]=='*') //ends with star, compare only left part
	{
		Log("APC: Start match");
		wildcardMode = wmPKStartMatch;
		//useStar=TRUE;
		szTitle[strlen(szTitle)-1]='\0';
	}
	else 
	{
		wildcardMode = wmPKPartialMatch; //Partial match, Full match not implemented
		Log("APC: Partial match");
	}*/
	GetMappings(appName);
}

int ApplicationControl::GetVGS(CString ATCommand)
{
	int VGSVal = -1;
	int pos = ATCommand.Find("VGS",0);
	CString volSub = 0;
	if(ATCommand.GetAt(pos+4) == '0')
	{
		Log("APC:VGS has 0 in front");
		volSub = ATCommand.Mid(pos+5,1);
		VGSVal = atoi(volSub);
		Log("APC:VGS set to %d",VGSVal);
	}
	else
	{
		
		volSub = ATCommand.Mid(pos+4,2);
		Log("PPP:Extracted vol as: %s", volSub);
		VGSVal = atoi(volSub);
		Log("APC:VGS set to %d",VGSVal);
	}
	return VGSVal;
}


CStringList& ApplicationControl::GetApplicationNames()
{	
	try
	{
		CString path = getPath();
		path = path + "ButtonMappings.txt";
		CFile myFile(path, CFile::modeCreate | CFile::modeNoTruncate| CFile::shareDenyNone);
		int bytesRead=0;
		char s[10000];
		CString settings;
		//bytesRead = myFile.Read(settings.GetBufferSetLength(50),50);
		bytesRead = myFile.Read(s,10000);
		
		if(bytesRead > 0)
		{
			s[bytesRead] = 0;
			settings = s;
			//Log("APC:ButtonMappings.txt read as : %s",settings);
			
			int initPos = 0;
			CString localAppName;
			do
			{
				localAppName = GetDataFromIdentifier("ApplicationName : ", settings);
				if(localAppName != CString("EOF"))
				{
					myAppNameList.AddTail(localAppName);
				}

			}while(localAppName != CString("EOF"));
		}
		
		myFile.Close();
	}
	catch(CFileException* pEx)
	{
		pEx->Delete();
		// Simply show an error message to the user.
	}
	return myAppNameList;
}

/*This routine is used in 2 ways:
1. When getting all application names to display in the listBox. Then initPos is statically = 0
this is valid for the entire duration of the application. Calling this function a second time 
would fail since it would start looking at the end of the file. It is not dynamically updatable.
2. When identifying mappings for a specific application, then the search starts at startAt since
all applications have the same structure in ButtonMappings.txt*/
CString ApplicationControl::GetDataFromIdentifier(CString dataToGet, CString settings)
{
	static initPos = 0; //Will only be 0 the first time, IT WILL NEVER WORK TO CALL THIS FUNCTION TWICE WITHOUT RESET OF initPos
	
	int startPos = settings.Find(dataToGet,initPos) + dataToGet.GetLength(); 
	int endPos = settings.Find("\r\n",startPos);
	initPos = endPos;

	if((startPos - dataToGet.GetLength()) == -1 )
	{
		Log("APC:EOF");
		return CString("EOF");
	}
	Log("APC: Returning ApplicationName: %s", settings.Mid(startPos, endPos-startPos)); 
	return settings.Mid(startPos, endPos-startPos);
}

//This is a mess due to inheritance from ppControl, appears to work fine though*/
bool ApplicationControl::CheckCRCAndDemo()
{
	//Allow any number of commands while in wizardMode
	if(inWizardMode)
	{
		return true;
	}

	if(!runInDemoMode)
	{
		return true;
	}
	if(!CRCCheckOK)
	{
		MessageBox(NULL,"A CRC check has failed.\n\nPowerpoint will not move forward","CRC check failed",MB_OK);
		Log("APC:CRC has failed, not moving forward");
		return false;
	}
	
	if(demoCounter <= 5)
	{
		Log("APC:Demo counter <= 5: %d",demoCounter);
		demoCounter++;
		return true;
	}
	else
	{
		Log("APC:Democounter = 5: %d", demoCounter);
		int boxres;
		boxres = MessageBox(NULL,"This HeadsetPresenter is not yet licensed and has is a five click limitation.\n\nIf you have purchased the HeadsetPresenter and received a registration code press the YES button below\nto complete the activation of the HeadsetPresenter and remove this message.\n\nDo you want to activate the HeadsetPresenter with your registration code?", "Bluetooth Headset Presenter", MB_YESNO|MB_TOPMOST|MB_ICONINFORMATION|MB_SETFOREGROUND);
		//needed a special version of getHElp here since there was an MFC problem when passing Controller on to ppControl
		if(boxres == IDYES)
		{
			bool licenseOK;
			myLicManProxy->SetRegistrationComment(GetRegistrationComment());
			licenseOK = myLicManProxy->CreateLicFile(L"HeadsetPresenter",L"3.5");
			if(licenseOK)
			{
				runInDemoMode = false;	
			}
						
		}
		else
		{ //http://www.headsetpresenter.com\content\download_info.asp
			boxres = MessageBox(NULL,"A HeadsetPresenter license can be aquired quickly and securly from the HeadsetPresenter website.\n\nDo you want to be forwarded to the HeadsetPresenter webpage for more information?","License key info",MB_YESNO|MB_TOPMOST|MB_ICONINFORMATION|MB_SETFOREGROUND);
			if(boxres == IDYES)
			{
				ShellExecute(NULL, "open", "http://www.headsetpresenter.com\\content\\download_info.asp", NULL, NULL, SW_SHOWNORMAL);
			}
		}
	}
	return !runInDemoMode;	
}
void ApplicationControl::OpenBuyWebpage()
{
	
}

CString ApplicationControl::getPath()
{
	/*Get path to Configurator.exe = path to HeadsetPresenter.dll*/
	TCHAR fullPathToEXE[512];
	//Get the dir path only and set \0 after the final \ in the path to remove the filename of the exe
	DWORD dwResult = ::GetModuleFileName(NULL,fullPathToEXE, sizeof(fullPathToEXE));
	char* t = _tcsrchr(fullPathToEXE,'\\');
	t++;
	*t = '\0';
	//end get path
	return CString(fullPathToEXE);
}

//This is a mess due to inheritance from ppControl, appears to work fine though*/
void ApplicationControl::SetHomeDirPath()
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
