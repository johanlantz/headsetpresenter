//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include ".\settingscontrol.h"
#include "SS_Log.h"
#include "defines.h"

SettingsControl::SettingsControl(void)
{
	//Hardcoded productVersion number must be enumerated if needed
	//No logging can be done before LoadSettings has set the disable logging flag
	productVersion = CString("3.5");
	minAllowedProductVersion = CString("3.0");
	//end
	OSId = 0;
	RestoreDefaults();
	logDisabled = LOG_DISABLED;
	LogDisableLogging(logDisabled);  //changed this in SS_Log
	
	//lastUsedApplicationName = "PowerPoint*";  //maybe remove this
	LoadSettings();
}

SettingsControl::~SettingsControl(void)
{
	SaveSettings();
}

void SettingsControl::RestoreDefaults()
{
	lastDeviceName = "NA";  //if this is changed from NA then HasLastDevice must be changed as well
	lastDeviceServiceName = "NA";
	hasLastDevice = false;
	lastDeviceSCN = 0; //Set default values
	lastDeviceAddress ="NA";
	forcedVGSActive = false;
	forwardOnly = false;
	lastControllerMode = ERROR_MODE;
	replugMode = REPLUG_AUTOMATIC;
	speechSettings = 0 | LOG_DISABLED;
	stackName = NO_STACK;
	lastControllerName = "NA";
	audioDevice = "NA";
	restartDeviceHandling = RESTART_DEVICE_NEVER; //ControllerFactory changes to always if < Server_2003
	serviceInstalled = false;
	infoDialogCheckBoxFlags = 0;
	keepAliveTimerVal = 1000;
	letWidcommQueryForPin = 0; //false use own pairing
	volSupportInNormalMode = 0;
	blockDoubleClicks = 0; //Do not block double clicks
}

void SettingsControl::LoadSettings()
{
	//No logging can be done before LoadSettings has set the disable logging flag
	try
	{
		CString path = getPath();
		path = path + "HPSettings.txt";
		CFile myFile(path, CFile::modeCreate | CFile::modeNoTruncate| CFile::shareDenyNone);
		int bytesRead=0;
		char s[1000];
		CString settings;
		//bytesRead = myFile.Read(settings.GetBufferSetLength(50),50);
		bytesRead = myFile.Read(s,1000);
		
		if(bytesRead > 0)
		{
			s[bytesRead] = 0;
			settings = s;

			//Step 0 See if we should log, no Log commands may be introduced before this has executed otherwise the log will start anyway
			//CString tempLogAsString;
			//tempLogAsString = GetDataFromIdentifier("Log=",settings);
			//logDisabled = atoi(tempLogAsString);
			//LogDisableLogging(logDisabled);  //changed this in SS_Log
			//Log("CTRL:logDisabled = %d",logDisabled);
			
			//Step 0 Get Speech Settings, done first to get the Log flag embedded
			CString tempSpeechAsString;
			tempSpeechAsString = GetDataFromIdentifier("SpeechSettings=",settings);
			speechSettings = atoi(tempSpeechAsString);
			LogDisableLogging(speechSettings & LOG_DISABLED); //Any non 0 value will disable logging
			Log("CTRL:SpeechSettings = %d",speechSettings);
			
			//Step 1 Get DeviceName
			lastDeviceName = GetDataFromIdentifier("DeviceName=",settings);
			Log("CTRL: LastDeviceName = %s",lastDeviceName);
			
			//Step 2 Get DeviceAddress
			lastDeviceAddress = GetDataFromIdentifier("DeviceAddress=",settings);
			Log("CTRL: LastDeviceAddress = %s",lastDeviceAddress);

			//Step 3 Get ServiceName
			lastDeviceServiceName = GetDataFromIdentifier("ServiceName=",settings);
			Log("CTRL: LastDeviceServiceName = %s",lastDeviceServiceName);

			//Step 4 Get SCN
			CString tempSCNAsString;
			tempSCNAsString = GetDataFromIdentifier("SCN=",settings);
			//tempSCNAsString = settings.GetAt(0);
			lastDeviceSCN = atoi(tempSCNAsString);
			Log("CTRL:LastDeviceSCN = %d",lastDeviceSCN);
			

			//Step 5 Get forced VGS setting
			if(GetDataFromIdentifier("ForceVGS=",settings) == "1")
			{
				Log("CTRL:Read forcedVGS = true");
				forcedVGSActive = true;
			}
			else
			{
				Log("CTRL:Read forcedVGS = false");
			}

			//Step 6 Get lastUsedApplication
			lastUsedApplicationName = GetDataFromIdentifier("LastUsedApplicationName=",settings);
			Log("CTRL: LastUsedApplicationName = %s",lastUsedApplicationName);

			//Step 7 Get ForwardOnly setting
			if(GetDataFromIdentifier("ForwardOnly=",settings) == "1")
			{
				Log("CTRL:Read ForwardOnly = true");
				forwardOnly = true;
			}
			else
			{
				Log("CTRL:Read forwardOnly = false");
			}

			//Step 8 GetLastControllerMode
			CString tempControllerModeAsString;
			tempControllerModeAsString = GetDataFromIdentifier("LastControllerMode=",settings);
			lastControllerMode = atoi(tempControllerModeAsString);
			Log("CTRL:LastControllerMode = %d",lastControllerMode);

			//Step 9 GetLastReplugMode
			CString tempReplugModeAsString;
			tempReplugModeAsString = GetDataFromIdentifier("LastReplugMode=",settings);
			replugMode = atoi(tempReplugModeAsString);
			Log("CTRL:LastReplugMode = %d",replugMode);

			//Step 10 GetStackName
			CString tempStackNameAsString;
			tempStackNameAsString = GetDataFromIdentifier("StackName=",settings);
			stackName = atoi(tempStackNameAsString);
			Log("CTRL:StackName = %d",stackName);

			//Step 11 GetOSId
			CString tempOSIdAsString;
			tempOSIdAsString = GetDataFromIdentifier("OSId=",settings);
			OSId = atoi(tempOSIdAsString);
			Log("CTRL:OSId = %d",OSId);

			//Step 12 GetLastControllerName
			lastControllerName = GetDataFromIdentifier("LastControllerName=",settings);
			Log("CTRL: LastControllerName = %s",lastControllerName);

			//Moved to first since log flag is included hidden
			//Step 13 Get speech settings
			//CString tempSpeechAsString;
			//tempSpeechAsString = GetDataFromIdentifier("SpeechSettings=",settings);
			//speechSettings = atoi(tempSpeechAsString);
			//Log("CTRL:SpeechSettings = %d",speechSettings);

			//Step 14 GetAudioDeviceName
			audioDevice = GetDataFromIdentifier("AudioDevice=",settings);
			Log("CTRL: AudioDevice = %s",audioDevice);
			
			//Step 15 GetRestartDeviceHandling
			CString tempRestartDevAsString;
			tempRestartDevAsString = GetDataFromIdentifier("RestartDeviceHandling=",settings);
			restartDeviceHandling = atoi(tempRestartDevAsString);
			Log("CTRL:RestartDeviceHandling = %d",restartDeviceHandling);

			//Step 16 Get serviceInstalled setting
			if(GetDataFromIdentifier("ServiceInstalled=",settings) == "1")
			{
				Log("CTRL:Read serviceInstalled = true");
				serviceInstalled = true;
			}
			else
			{
				Log("CTRL:Read serviceInstalled = false");
			}

			//Step 17 GetDialogBoxFlags
			CString tempBoxFlagsAsString;
			tempBoxFlagsAsString = GetDataFromIdentifier("BoxFlags=",settings);
			infoDialogCheckBoxFlags = atoi(tempBoxFlagsAsString);
			Log("CTRL:BoxFlags = %d",tempBoxFlagsAsString);

			//Step 18 Get KeepAliveTimerVal
			CString tempKAAsString;
			tempKAAsString = GetDataFromIdentifier("KeepAliveTimer=",settings);
			keepAliveTimerVal = atoi(tempKAAsString);
			Log("CTRL:KeepAliveTimer = %d",keepAliveTimerVal);

			//Step 19 Get letWidcommGetPin
			CString tempUseWidcommPinAsString;
			tempUseWidcommPinAsString = GetDataFromIdentifier("LetWidcommQueryForPin=",settings);
			letWidcommQueryForPin = atoi(tempUseWidcommPinAsString);
			Log("CTRL:LetWidcommQueryForPin = %d",lastDeviceSCN);
			
			//Step 20 GetVolsupportNormal , only used when registering
			CString tempVolSupportAsString;
			tempVolSupportAsString = GetDataFromIdentifier("VolSupportInNormalMode=",settings);
			volSupportInNormalMode = atoi(tempVolSupportAsString);
			Log("CTRL:VolSupportInNormal = %d",volSupportInNormalMode);

			//Step 21 BlockDoubleClicks, default is 0
			CString tempBlockDoubleClicksAsString;
			tempBlockDoubleClicksAsString = GetDataFromIdentifier("BlockDoubleClicks=",settings);
			blockDoubleClicks = atoi(tempBlockDoubleClicksAsString);
			Log("CTRL:BlockDoubleClicks = %d",blockDoubleClicks);
		}
		myFile.Close();
	}
   catch(CFileException* pEx)
   {
	   pEx->Delete();
      // Simply show an error message to the user.
   }
}

CString SettingsControl::GetDataFromIdentifier(CString dataToGet, CString settings)
{
	int startPos = settings.Find(dataToGet,0) + dataToGet.GetLength(); 
	int endPos = settings.Find("\r\n",startPos);

	if(settings.Find(dataToGet,0) == -1)
	{
		MessageBox(NULL,"HPSettings.txt has been corrupted, the wizard must be rerun",dataToGet,MB_OK);
		Log("CTRL:HPSettings.txt is corrupted for element %s. Saved settings could not be restored.\nThis will be automatically corrected after the wizard has been rerun.", dataToGet);
		hasLastDevice = false;
	}
	
	return settings.Mid(startPos, endPos-startPos);
}

void SettingsControl::SaveSettings()
{
	try
	{
		CString path = getPath();
		path = path + "HPSettings.txt";
		CFile myFile(path, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone);	

		CString dataToWrite;

		//Step 0 Write logDisabled, handled by SpeechSettings now
		//CString logDisabledAsString;
		//itoa(logDisabled,logDisabledAsString.GetBufferSetLength(1),10);
		//dataToWrite = "Log=" + logDisabledAsString + CString("\r\n");
		//myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
		//Log("CTRL: Saving: %s", dataToWrite);
		

		//Step 1 Write DeviceName
		dataToWrite = "DeviceName=" + GetLastDeviceName() + CString("\r\n");
		myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
		Log("CTRL: Saving: %s", dataToWrite);

		//Step 2 Write DeviceAddress
		dataToWrite = "DeviceAddress=" + GetLastDeviceAddress() + CString("\r\n");
		myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
		Log("CTRL: Saving: %s", dataToWrite);

		//Step 3 Write ServiceName
		dataToWrite = "ServiceName=" + GetLastDeviceServiceName() + CString("\r\n");
		myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
		Log("CTRL: Saving: %s", dataToWrite);

		//Step 4 Write SCN
		CString lastSCNAsString;
		itoa(lastDeviceSCN,lastSCNAsString.GetBufferSetLength(1),10);
		dataToWrite = "SCN=" + lastSCNAsString + CString("\r\n");
		myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
		Log("CTRL: Saving: %s", dataToWrite);
		
		//Step 5 Write forcedVGS mode
		if(forcedVGSActive)
		{
			dataToWrite = "ForceVGS=" + CString("1") + CString("\r\n");
			myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
			Log("CTRL: Saving: %s", dataToWrite);
		}
		else
		{
			dataToWrite = "ForceVGS=" + CString("0") + CString("\r\n");
			myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
			Log("CTRL: Saving: %s", dataToWrite);
		}
		
		//Step 6 Save last used application
		dataToWrite = "LastUsedApplicationName=" + lastUsedApplicationName + CString("\r\n");
		myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
		Log("CTRL: Saving: %s", dataToWrite);

		//Step 7 Write forcedVGS mode
		if(forwardOnly)
		{
			dataToWrite = "ForwardOnly=" + CString("1") + CString("\r\n");
			myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
			Log("CTRL: Saving: %s", dataToWrite);
		}
		else
		{
			dataToWrite = "ForwardOnly=" + CString("0") + CString("\r\n");
			myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
			Log("CTRL: Saving: %s", dataToWrite);
		}

		//Step 8 Write LastControllerMode
		char LastControllerModeAsString[5];
		itoa(lastControllerMode,LastControllerModeAsString,10);  //If we put 2 here it seems as the atoi in Load becomes confused there might be a whitespace after the digit that causes the atoi to fail
		dataToWrite = "LastControllerMode=" + CString(LastControllerModeAsString) + CString("\r\n");
		myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
		Log("CTRL: Saving: %s", dataToWrite);

		//Step 9 Save ReplugMode
		char LastReplugModeAsString[5];
		itoa(replugMode,LastReplugModeAsString,10);  //If we put 2 here it seems as the atoi in Load becomes confused there might be a whitespace after the digit that causes the atoi to fail
		dataToWrite = "LastReplugMode=" + CString(LastReplugModeAsString) + CString("\r\n");
		myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
		Log("CTRL: Saving: %s", dataToWrite);

		//Step 10 Save StackName
		char tempStackNameAsString[5];
		itoa(stackName,tempStackNameAsString,10);  
		dataToWrite = "StackName=" + CString(tempStackNameAsString) + CString("\r\n");
		myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
		Log("CTRL: Saving: %s", dataToWrite);

		//Step 11 Save OSId
		char tempOSIdAsString[5];
		itoa(OSId,tempOSIdAsString,10);  
		dataToWrite = "OSId=" + CString(tempOSIdAsString) + CString("\r\n");
		myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
		Log("CTRL: Saving: %s", dataToWrite);

		//Step 12 Save last controllerName
		dataToWrite = "LastControllerName=" + lastControllerName + CString("\r\n");
		myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
		Log("CTRL: Saving: %s", dataToWrite);

		//Step 13 Write SpeechSettings
		char speechAsString[5];
		itoa(speechSettings,speechAsString,10);
		dataToWrite = "SpeechSettings=" + CString(speechAsString) + CString("\r\n");
		myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
		Log("CTRL: Saving: %s", dataToWrite);

		//Step 14 Save last controllerName
		dataToWrite = "AudioDevice=" + audioDevice + CString("\r\n");
		myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
		Log("CTRL: Saving: %s", dataToWrite);

		//Step 15 Save RestartDeviceHandling
		char tempRestartDevAsString[5];
		itoa(restartDeviceHandling,tempRestartDevAsString,10);  
		dataToWrite = "RestartDeviceHandling=" + CString(tempRestartDevAsString) + CString("\r\n");
		myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
		Log("CTRL: Saving: %s", dataToWrite);

		//Step 16 serviceInstalled
		if(serviceInstalled)
		{
			dataToWrite = "ServiceInstalled=" + CString("1") + CString("\r\n");
			myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
			Log("CTRL: Saving: %s", dataToWrite);
		}
		else
		{
			dataToWrite = "ServiceInstalled=" + CString("0") + CString("\r\n");
			myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
			Log("CTRL: Saving: %s", dataToWrite);
		}

		//Step 17 Save DialogBoxFlags
		char tempBoxFlagsAsString[5];
		itoa(infoDialogCheckBoxFlags,tempBoxFlagsAsString,10);  
		dataToWrite = "BoxFlags=" + CString(tempBoxFlagsAsString) + CString("\r\n");
		myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
		Log("CTRL: Saving: %s", dataToWrite);

		//Step 17 Save KeepAliveTimerVal
		char tempKAAsString[10];
		itoa(keepAliveTimerVal,tempKAAsString,10);  
		dataToWrite = "KeepAliveTimer=" + CString(tempKAAsString) + CString("\r\n");
		myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
		Log("CTRL: Saving: %s", dataToWrite);

		//Step 18 Save WidcommPin
		char useWidcommPinAsString[5];
		itoa(letWidcommQueryForPin,useWidcommPinAsString,10);  
		dataToWrite = "LetWidcommQueryForPin=" + CString(useWidcommPinAsString) + CString("\r\n");
		myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
		Log("CTRL: Saving: %s", dataToWrite);

		//Step 20 Save VolSupportNormal
		char volSupportAsString[5];
		itoa(volSupportInNormalMode,volSupportAsString,10);  
		dataToWrite = "VolSupportInNormalMode=" + CString(volSupportAsString) + CString("\r\n");
		myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
		Log("CTRL: Saving: %s", dataToWrite);

		//Step 21 Save BlockDoubleClicks
		char blockDoubleClicksAsString[5];
		itoa(blockDoubleClicks,blockDoubleClicksAsString,10);  
		dataToWrite = "BlockDoubleClicks=" + CString(blockDoubleClicksAsString) + CString("\r\n");
		myFile.Write((LPCTSTR)dataToWrite,dataToWrite.GetLength());
		Log("CTRL: Saving: %s", dataToWrite);

		myFile.Close();	
	}
   catch(CFileException* pEx)
   {
	   pEx->Delete();
      // Simply show an error message to the user.
   }
}

bool SettingsControl::HasLastDevice()
{
	if(lastDeviceName != "NA")
	{
		hasLastDevice = true;
	}
	return hasLastDevice;
}

void SettingsControl::SetLastDeviceAddress(CString iAddress)
{
	Log("CTRL:lastDeviceAddress = %s",iAddress);
	lastDeviceAddress = iAddress;
}

void SettingsControl::SetLastDeviceSCN(int iSCN)
{
	Log("CTRL:lastDeviceSCN = %d",iSCN);
	lastDeviceSCN = iSCN;
}

CString SettingsControl::GetLastDeviceAddress()
{
	Log("CTRL:GetLastDeviceAddress = %s",lastDeviceAddress);
	return lastDeviceAddress;
}

int SettingsControl::GetLastDeviceSCN()
{
	//Log("CTRL:lastDeviceSCN = %d",lastDeviceSCN);
	return lastDeviceSCN;
}

CString SettingsControl::GetLastDeviceName()
{
	Log("CTRL:GetLastDeviceName = %s",lastDeviceName);
	return lastDeviceName;
}

void SettingsControl::SetLastDeviceName(CString devName)
{
	lastDeviceName = devName;
	Log("CTRL:SetLastDeviceAddress = %s",lastDeviceName);
}


CString SettingsControl::GetLastDeviceServiceName()
{
	return lastDeviceServiceName;
}

void SettingsControl::SetLastDeviceServiceName(CString servName)
{
	lastDeviceServiceName = servName;
}

void SettingsControl::SetForcedVGS(bool newMode)
{
	forcedVGSActive = newMode;
}

bool SettingsControl::GetForcedVGS()
{
	return forcedVGSActive;
}

void SettingsControl::SetForwardOnly(bool newMode)
{
	Log("CTRL:Forward only set to %d", newMode);
	forwardOnly = newMode;
}

bool SettingsControl::GetForwardOnly()
{
	Log("CTRL:Return forwardonly as %d",forwardOnly);
	return forwardOnly;
}

void SettingsControl::SetLastApplicationName(CString name)
{
	lastUsedApplicationName = name;
}

CString SettingsControl::GetLastApplicationName()
{
	return lastUsedApplicationName;
}

void SettingsControl::SetLastControllerMode(int controllerMode)
{
	lastControllerMode = controllerMode;
}

int SettingsControl::GetLastControllerMode()
{
	Log("CTRL:GetLastControllerMode returns %d",lastControllerMode);
	return lastControllerMode;
}

void SettingsControl::SetLastReplugMode(int iReplugMode)
{
	replugMode = iReplugMode;
}

int SettingsControl::GetLastReplugMode()
{
	return replugMode;
}

void SettingsControl::SetSpeechSettings(int iSpeechSettings)
{
	speechSettings = iSpeechSettings;
}

int SettingsControl::GetSpeechSettings()
{
	return speechSettings;
}

void SettingsControl::SetLogDisabled(int iLogDisabled)
{
	if(iLogDisabled == 1)
	{
		speechSettings |= LOG_DISABLED;
		Log("CTRL:Disabling logging %d", speechSettings);
	}
	else
	{
		speechSettings &= ~LOG_DISABLED;
		Log("CTRL:Enabling logging %d", speechSettings);
	}
	//logDisabled = iLogDisabled;
}

int SettingsControl::GetLogDisabled()
{
	Log("CTRL:GetLogDisabled will return %d", speechSettings & LOG_DISABLED); 
	return speechSettings & LOG_DISABLED;
}

/*Currently stackName is only used to inform user of stack specific issues.
For instance that Toshiba stack needs to disconnect all devices before activation of HP.*/
void SettingsControl::SetStackName(int iStackName)
{
	stackName = iStackName;
}

int SettingsControl::GetStackName()
{
	return stackName;
}


void SettingsControl::SetOSId(int iOSId)
{
	OSId = iOSId;
	if(OSId < WINDOWS_SERVER_2003)
	{
		//Log("CTRL:OS is < SERVER_2003, set restart device to always");
		//SetRestartDeviceHandling(RESTART_DEVICE_ALWAYS);
	}
}

int SettingsControl::GetOSId()
{
	return OSId;
}

CString SettingsControl::GetProductVersion()
{
	return productVersion;
}

double SettingsControl::GetProductVersionAsDouble()
{
	return atof(productVersion);
}

double SettingsControl::GetMinAllowedProductVersionAsDouble()
{
	return atof(minAllowedProductVersion);
}

CString SettingsControl::GetLastControllerName()
{
	return lastControllerName;
}

void SettingsControl::SetLastControllerName(CString iControllerName)
{
	lastControllerName = iControllerName;
}
	
void SettingsControl::SetAudioDevice(CString iAudioDevice)
{
	audioDevice = iAudioDevice;
}

CString SettingsControl::GetAudioDevice()
{
	return audioDevice;
}

void SettingsControl::SetRestartDeviceHandling(int iRestartDeviceHandling)
{
	restartDeviceHandling = iRestartDeviceHandling;
}

int SettingsControl::GetRestartDeviceHandling()
{
	return restartDeviceHandling;
}

void SettingsControl::SetServiceInstalled(bool iServiceInstalled)
{
	serviceInstalled = iServiceInstalled;
}

bool SettingsControl::GetIsServiceInstalled()
{
	Log("CTRL:Returning false for service");
	return false;
	//return serviceInstalled;
}

void SettingsControl::SetKeepAliveTimerVal(int timerVal)
{
	keepAliveTimerVal = timerVal;
}

int SettingsControl::GetKeepAliveTimerVal()
{
	return keepAliveTimerVal;
}

void SettingsControl::SetLetWidcommQueryForPin(int useWPin)
{
	letWidcommQueryForPin = useWPin;
}

int SettingsControl::GetLetWidcommQueryForPin()
{
	return letWidcommQueryForPin;
}

void SettingsControl::SetVolSupportInNormalMode(int vSupport)
{
	volSupportInNormalMode = vSupport;
}
int SettingsControl::GetVolSupportInNormalMode()
{
	return volSupportInNormalMode;
}

void SettingsControl::SetBlockDoubleClicks(int iBlock)
{
	blockDoubleClicks = iBlock;
}

bool SettingsControl::GetBlockDoubleClicks()
{
	return blockDoubleClicks == 1 ? true : false;
}

void SettingsControl::SetInfoBoxCheckBoxFlag(int flag, bool set)
{
	Log("CTRL: old check box flags %d", infoDialogCheckBoxFlags);
	if(set)
	{
		infoDialogCheckBoxFlags |= flag;	
	}
	else  //reset this flag
	{
		infoDialogCheckBoxFlags &= ~flag;
	}
	Log("CTRL: New check box flags %d",infoDialogCheckBoxFlags);
}

int SettingsControl::GetInfoBoxCheckBoxFlags()
{
	return infoDialogCheckBoxFlags;
}

CString SettingsControl::getPath()
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