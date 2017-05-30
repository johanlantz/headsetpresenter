//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
// GenericController.cpp: implementation of the GenericController class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "GenericController.h"
#include "devicemgr.h"
#include "HeadsetPresenterDlg.h"
#include "..\..\USBSnoop\Shared\Common.h"
#include "IoControl.h"
#include "InfoDialog.h"


#include "SS_Log.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

GenericController::GenericController()
{
	myFileParser = new FileParser(this);
	defaultWebAddress = "http://www.headsetpresenter.com/content/help.asp";
	defaultVisitSiteMsg = "\n\nMore information about this might be available on the HeadsetPresenter website.\nWould you like to be directed there";
	SetHomeDirPath();
	controllerMode = ADVANCED_MODE;
}

GenericController::~GenericController()
{
	Stop();
	delete myFileParser;
	
}

bool GenericController::IsRadioAvailable()
{
	return TRUE; //Not used by generic controller
}

void GenericController::SetReplugMode(int iReplugMode)
{
	myFileParser->SetReplugMode(iReplugMode);
}

void GenericController::SetHomeDirPath()
{
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
		applicationHomeDir = fullPathToDLL;
		Log("CT:ApplicationHomeDir is: %s", applicationHomeDir);
	}
	
}
bool GenericController::Start()
{
	LogEnterFunction("CT:Start");
	
	if(myFileParser->IsThreadRunning())  //if thread is running and we press the HP button, restart thread, it is a new pres.
	{
		Log("CT:Trying to start when alredy running");
		//MessageBox(NULL,"The HeadsetPresenter is already active.\n\nPlease start your presentation with F5 or via the menu as usual","Information",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		return true;
	}
	else  //Thread is not running, this is the first time user presses start button, install driver.
	{
		CheckService();
		mySnoopControl.SetSettingsControl(mySettingsControl);
		if(mySnoopControl.InstallDriver())
		{
			if(mySettingsControl->GetRestartDeviceHandling() & RESTART_DEVICE_ALWAYS)
			{
				Log("CT:RESTART_DEVICE_ALWAYS => Restarting device");
				PreHardRestart();
				mySnoopControl.RestartDevice();
				//PostHardRestart();
			}
			else  //Soft restart
			{
				/*if(mySettingsControl->GetOSId() < WINDOWS_SERVER_2003)
				{
					MessageBox(NULL,"Restart mode never is not supported on OS earlier then Windows Server 2003\n\nPlease change to Restart mode Always in the Settings dialog","",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
					Log("GC:Unplug mode never is not supported on OS earlier then Windows Server 2003");
					//return false;
				}*/
				Log("CT:RESTART_DEVICE_NEVER => NOT restarting device");
				if(!DoSoftRestart())
				{
					return false;  //First time started on non Widcomm, restart windows needed to init
				}
			}
			myFileParser->StartParseThread();
			return true;
		}
		else
		{
			MessageBox(NULL,"Driver installation failed, contact support","",MB_OK);
			Log(LOGTYPE_CRITICAL,"CT:Installation of driver failed");
			return false;
		}
		
	}
}


//Called by onDisconnect invoked by PP 
bool GenericController::Stop()
{
	LogEnterFunction("CT:Stop");
	myFileParser->StopParseThread();
	
	//The filter is never removed if we use RESTART_DEVICE_NEVER
	//It is not needed to remove it here either since it will not take effect until 
	//we restart.
	//This is for precaution if there are problems with HP or USBSnoop for instance a leak
	//then this will disapper if the device or computer is restarted.
	QuitLogging();
	if(mySettingsControl->GetRestartDeviceHandling() & RESTART_DEVICE_ALWAYS)
	{
		Log("CT:RESTART_DEVICE_ALWAYS => filter is removed");
		mySnoopControl.UninstallDriver();
	}
	else
	{
		Log("CT:RESTART_DEVICE_NEVER => filter is NOT removed");
	}
	
	return true;
}

bool GenericController::isActive()
{
	
	return myFileParser->IsThreadRunning();
	
}

void GenericController::getHelp(int codeID)
{
/*
	CString helpURL = defaultWebAddress;
	helpURL += "?compMAC=";
	
	helpURL += FullMACString;
	helpURL += "&appMAC";
	helpURL += applicationMAC;
	helpURL += "&codeID=";
	char codeAsString[5];
	itoa(codeID,codeAsString,10);
	helpURL += codeAsString;
	ShellExecute(NULL, "open", helpURL.GetBuffer(10), NULL, NULL, SW_SHOWNORMAL);
	*/
}



//Service mangagment functions
//Not sure if start and stop is necessary


/******************************************************************
*
* Make sure that the defines in Common.h are correct if updating the USBSnoop
*
*******************************************************************/
#define USBSNOOP_SERVICE "HeadsetPresenter"

void GenericController::CheckService()
{
	
// check if "usbsnoop" service is installed
	Log("CT:CheckService called");
	//if(!mySettingsControl->GetIsServiceInstalled())
	//{
		SC_HANDLE hManager = OpenSCManager(NULL,NULL,
			SC_MANAGER_CREATE_SERVICE|SC_MANAGER_ENUMERATE_SERVICE);

		if (hManager == NULL)
		{
			//if(mySettingsControl->GetOSId() & WINDOWS_VISTA)
			//{
			//	MessageBox(NULL,"Can't open service manager. Advanced mode on Vista requires HeadsetPresenter to run with admin rights the first time it is started.\nStart HeadsetPresenter by rightclicking on HeadsetPresenter.exe and select Run As Administrator.\n\nThis message should actually never appear, please let support know that this happened.","Vista",MB_OK|MB_ICONINFORMATION);
			//}
			//else
			//{
			//	MessageBox(NULL,"Can't open service manager","Report to support",MB_OK|MB_ICONINFORMATION);
			//}
			//MessageBox(NULL,"Can't open service manager","Report to support",MB_OK|MB_ICONINFORMATION);
			//return ;
		}

		SC_HANDLE hService = OpenService(hManager,"HeadsetPresenter",DELETE);
		if (hService == NULL)
		{
			if (GetLastError() == ERROR_SERVICE_DOES_NOT_EXIST)
			{
					hService = CreateService(hManager,"HeadsetPresenter","HeadsetPresenter (display)",
						0,SERVICE_KERNEL_DRIVER,SERVICE_DEMAND_START,SERVICE_ERROR_NORMAL,
						_T("system32\\drivers\\hspf.sys"),
						NULL,NULL,NULL,NULL,NULL);

				if (hService == NULL)
				{
					MessageBox(NULL,"Can't create service","Report to support",MB_OK|MB_ICONINFORMATION);
				}
				else
				{
					//mySettingsControl->SetServiceInstalled(true);
					Log("GC:Installing service");
					CloseServiceHandle(hService);
				}
			}
		}
		else
		{

	//		if (!DeleteService(hService))
	//			MessageBox("Cannot remove existing hspf service");
			Log("GC:Service is installed not installing");
			CloseServiceHandle(hService);
		}


		CloseServiceHandle(hManager);
	//}
		
}

//Maybe this will never be used
void GenericController::OnStartService()
{	
	Log("CT:OnStartService called");
	MessageBox(NULL,"Onstartservice","",MB_OK);
	SC_HANDLE hManager = OpenSCManager(NULL,NULL,
		SC_MANAGER_CREATE_SERVICE|SC_MANAGER_ENUMERATE_SERVICE);

	if (hManager == NULL)
	{
		MessageBox(NULL,"Can't open service manager","Report to support",MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		SC_HANDLE hService = OpenService(hManager,USBSNOOP_SERVICE,DELETE);
		if (hService == NULL)
		{
			MessageBox(NULL,"Can't open service","Report to support",MB_OK|MB_ICONINFORMATION);
		}
		else
		{
			if (!StartService(hService,NULL,0))
				MessageBox(NULL,"Can't start service","Report to support",MB_OK|MB_ICONINFORMATION);

			CloseServiceHandle(hService);
		}

		CloseServiceHandle(hManager);
	}
}

//Maybe this will never be used
void GenericController::OnStopService()
{	
	MessageBox(NULL,"Onstopservice","",MB_OK);
	Log("CT:OnStopService called");
	SC_HANDLE hManager = OpenSCManager(NULL,NULL,
		SC_MANAGER_CREATE_SERVICE|SC_MANAGER_ENUMERATE_SERVICE);

	if (hManager == NULL)
	{
		MessageBox(NULL,"Can't open service manager","Report to support",MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		SC_HANDLE hService = OpenService(hManager,USBSNOOP_SERVICE,DELETE);
		if (hService == NULL)
		{
			MessageBox(NULL,"Can't open service","Report to support",MB_OK|MB_ICONINFORMATION);
		}
		else
		{
			SERVICE_STATUS sStatus;

			memset(&sStatus, 0, sizeof(sStatus));

			if (!ControlService(hService,SERVICE_CONTROL_STOP,&sStatus))
				MessageBox(NULL,"Can't stop service","Report to support",MB_OK|MB_ICONINFORMATION);

			CloseServiceHandle(hService);
		}

		CloseServiceHandle(hManager);
	}
}


void GenericController::OnCreateService()
{	
	Log("CT:OnCreateService called");
	SC_HANDLE hManager = OpenSCManager(NULL,NULL,
		SC_MANAGER_CREATE_SERVICE|SC_MANAGER_ENUMERATE_SERVICE);

	if (hManager == NULL)
	{
		MessageBox(NULL,"Can't open service manager","Report to support",MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		SC_HANDLE hService = CreateService(hManager,USBSNOOP_SERVICE,"HeadsetPresenter (display)",
			0,SERVICE_KERNEL_DRIVER,SERVICE_DEMAND_START,SERVICE_ERROR_NORMAL,
			"System32\\DRIVERS\\HSPF.SYS",
			NULL,NULL,NULL,NULL,NULL);

		if (hService == NULL)
		{
			MessageBox(NULL,"Can't create service","Report to support",MB_OK|MB_ICONINFORMATION);
		}
		else
		{
			CloseServiceHandle(hService);
		}

		CloseServiceHandle(hManager);
	}
}

//Maybe this will never be used
void GenericController::OnDeleteService()
{	
	MessageBox(NULL,"OnDeleteservice","",MB_OK);
	Log("CT:OnDeleteService called");
	SC_HANDLE hManager = OpenSCManager(NULL,NULL,
		SC_MANAGER_CREATE_SERVICE|SC_MANAGER_ENUMERATE_SERVICE);

	if (hManager == NULL)
	{
		MessageBox(NULL,"Can't open service manager","Report to support",MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		SC_HANDLE hService = OpenService(hManager,USBSNOOP_SERVICE,DELETE);
		if (hService == NULL)
		{
			MessageBox(NULL,"Can't open service","Report to support",MB_OK|MB_ICONINFORMATION);
		}
		else
		{
			if (!DeleteService(hService))
				MessageBox(NULL,"Can't delete service","Report to support",MB_OK|MB_ICONINFORMATION);

			CloseServiceHandle(hService);
		}

		CloseServiceHandle(hManager);
	}
}



//Inherited functions from SuperController
bool GenericController::ConnectToLastUsedDevice()
{	
	INT_PTR res = myManualConnectDlg.DoModal();
	if(res == IDOK)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

bool GenericController::ShowDeviceSearchDialog()
{
	INT_PTR res = myManualConnectDlg.DoModal();
	if(res == IDOK)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GenericController::DiscoverDevices(bool refresh)
{
	MessageBox(NULL,"discover dev","",MB_OK);
}

void GenericController::DeviceSelected(int deviceIndex)
{
	MessageBox(NULL,"dev sel","",MB_OK);
	
}

CStringList* GenericController::GetServiceListAsCStringList()
{
	CStringList* fakeList = new CStringList();
	return fakeList;
}

bool GenericController::ConnectToSelectedDevice(int SCN, CString serviceName )
{
	return true;
}

void GenericController::DiscoverServicesAsync()
{
}

bool GenericController::DiscoverServicesSync()
{
	return true;
	//No action here, synch call will return immidiatly
}

int GenericController::GetNoOfServicesDiscovered()
{
	return GENERIC_MODE_SERVICE_COUNT;
}



int GenericController::GetSCNOfService(CString serviceNameToFind)
{
	return 0;
}

void GenericController::InitLocalSettings()
{
	//Get controller specific settings
	this->SetReplugMode(mySettingsControl->GetLastReplugMode());
	//ForcedVGS will be false for generic controller
}

int GenericController::GetMode()
{
	return controllerMode;
}
//Called by FileParser when a data string is read 
void GenericController::OnReadCompleted(char* data)
{
	//bool dataIsATCommand = false;
	int commandType = ERROR_COMMAND;
	char inBuffer[500];
	strcpy(inBuffer,data); //make local copy incase FileParser thread overwrite the old buffer
	Log("GC:data read and copied as : %s",inBuffer);
	char* hitInBuffer = 0;
	CString dataString;
	//1. Check if it is an AT command
	if((hitInBuffer = strstr(inBuffer,"41 54 2b")) != NULL )
	{
		Log("GC:Data is AT command");
		//dataIsATCommand = true;
		commandType = AT_COMMAND;
		char seps[]   = " ";
		char *token;
		unsigned long i;
		char t;
		/* Establish string and get the first token: Set the pointer to the AT+ position*/
		token = strtok( hitInBuffer, seps );
		while( token != NULL )
		{     
			i = strtoul(token,'\0',16);
			t = (char)i;
			dataString.Append(CString(t)); 
			token = strtok( NULL, seps );
		}
		Log("GC:AT command extracted as : %s",dataString);
	}
	//Not AT command, AVRCP
	else
	{
		//dataIsATCommand = false;
		commandType = AVRCP_COMMAND;
		dataString = CString(inBuffer);
		Log("GC:Data is not AT command, sending : %s",dataString);
	}
	//For the generic controller we need to inform the manual connect dlg that a command is received
	if(state == STATE_NOT_CONNECTED)
	{
		Log("GC:Sending command to manual connect dialog");
		myManualConnectDlg.GotCommand();	
		//Must call func. otherwise we will not enter STATE_CONNECTED, ForwardCommand=false so only state change
		HandleStatesAndForwardCommand(dataString, commandType, false);
	}
	//Do not forward commands if the ManualConnectDlg is open then the window not found msg will be displayed
	if(!myManualConnectDlg.IsDialogActive())
	{
		HandleStatesAndForwardCommand(dataString, commandType, true);
	}
	/*
	if(wizardDialog->GetWizardState() == STATE_WIZARD_TRY_HFP || wizardDialog->GetWizardState() == STATE_WIZARD_TRY_HSP || wizardDialog->GetWizardState() == STATE_WIZARD_TRY_UNKNOWN)
	{
		Log("GC:Sending command to wizard");
		wizardDialog->CommandReceived(dataString,commandType);
	}
	else if(state == STATE_CONNECTED) //Actually if the mode dialog is displayed commands could be sent to the app here, but that only occurs if the user does not press Next
	{
		Log("GC:Sending command to HPCallback");
		myHeadsetPresenterCallback->CommandCallback(dataString, commandType);
	}
	else if(state == STATE_NOT_CONNECTED)
	{
		Log("GC:Sending command to manual connect dialog");
		myManualConnectDlg.GotCommand();
		SetState(STATE_CONNECTED);
	}*/
}

void GenericController::CheckLogFile()
{
	// this function check the log file presence and size and
	// update the two field in the dialog box
CString	m_LogFileName;
int		m_LogSize;
	TCHAR sLogPath[MAX_PATH];
	HANDLE hFile;

	// compute the name of the log file

	if (GetWindowsDirectory(sLogPath, sizeof(sLogPath)) == 0)
	{
		// failure
		return ;
	}

	_tcscat(sLogPath, _T("\\") );
	_tcscat(sLogPath, USBSNOOP_LOGFILE_NAME);

	m_LogFileName = sLogPath;
	m_LogSize = -1;

	hFile = CreateFile(sLogPath,GENERIC_READ,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		m_LogSize = GetFileSize(hFile, NULL);

		//  in case of errors, dwSize is -1

		CloseHandle(hFile);
	}
	else
	{
		Log("GC:Failed to check log");
	}


}
/*Snoop IOCTL funtions*/
void GenericController::ResumeLogging()
{
	Log("GC:Resuming logging");
    ULONG LoggingState = LoggingEnabled;

    if( !UsbSnoopSetLoggingState( LoggingState ) )
	{
		Log("GC:Can't resume logging");
        //MessageBox(NULL,"Can't resume logging","",MB_OK);
	}
	//CheckLogFile();
}

void GenericController::PauseLogging()
{
	Log("GC:Pausing logging");
    ULONG LoggingState = 0;

    if( !UsbSnoopSetLoggingState( LoggingState ) )
	{
		Log("GC:Can't pause logging");
        //MessageBox(NULL,"Can't pause logging","",MB_OK);
	}
	//CheckLogFile();
}

void GenericController::CloseLogFile()
{
	Log("GC:Close log");
    if( !UsbSnoopCloseLogFile() )
	{
		Log("GC:Can't close log file");
        //MessageBox(NULL,"Can't close log file","",MB_OK);
	}
}

bool GenericController::DoSoftRestart()
{
	LogEnterFunction("GC:DoSoftRestart");
	if(mySettingsControl->GetOSId() < WINDOWS_SERVER_2003) //RESTART_NEVER is a prereq to even get here so we do not have to check on that. 
	{
		InfoDialog myInfoDlg;
		myInfoDlg.SetSettingsControl(mySettingsControl);
		myInfoDlg.SetBoxId(INFO_DLG_SOFT_ON_OLD_DRIVER);
		myInfoDlg.SetText("Important information","A restart might be needed","HeadsetPresenter is currently configured to use \"Restart mode NEVER\". This feature is a Windows XP/Vista feature but it can work on Windows 2000 as well. However you need to reboot you computer once in order for the initialisation of HeadsetPresenter for Windows 2000 to finish.\n\nIf this is the first time HeadsetPresenter is started in Advanced mode, please exit the application and restart Windows.\n\nThis is only needed once and this reminder can be deactivated after restart."); 
		if(myInfoDlg.ShowBox())
		{
			myInfoDlg.DoModal();
		}
		Log("GC:DoSoftRestart on old os, return true");
		return true;
	}
	if( UsbSnoopIsDriverStarted() )
    {
		Log("GC:Driver is started");
		UsbSnoopQueryLoggingState( &m_LoggingState );
		Log("GC:Logstate is %lu",m_LoggingState);
        if( m_LoggingState & LoggingEnabled )
        {
			//It would be better to pause close and delete but this is simpler for now
			Log("GC:Logging is enabled, just continue FP will forward to correct pos");
        }
        else
        {
			//Resume does not seem to work if there is no log file
			Log("GC:Logging is not enabled, resuming (starting)");
			ResumeLogging();
        }
    }
    else  //Driver not started ie installation done but not restarted for non widcomm
    {
		Log("GC:Driver not started, reboot or restart needed");
		if(!(myHeadsetPresenterCallback->GetSupportedModes() & WIDCOMM))
		{
			//InfoDialog myRestartInfoDlg
			//MessageBox(NULL,"HeadsetPresenter is not fully installed.\n\nBefore the first time HeadsetPresenter is run in Advanced mode, Windows needs to be restarted.\n\nPlease restart Windows, this is only needed once.","Restart needed",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
			InfoDialog myRestartInfoDlg;
			myRestartInfoDlg.SetText("Advanced Mode Info","Completing installation","Before the first time Advanced mode is used Windows must be restarted to complete the installation of HeadsetPresenter.\n\nPlease exit the application and restart Windows.\n\nThis is only needed once");
			myRestartInfoDlg.DoModal();
			Log("GC:Driver not started AND NOT Widcomm, request manual windows restart");
			Log("GC:This should only happen once!!!!");
			return false;
		}
		else
		{
			//Restart device only once, we want to use SoftRestart but the driver is not yet active
			//If widcomm do the restart without asking for the other ones request a restart
			//This restart will also start the logging
			Log("GC:Driver not started AND Widcomm, restart automatically");
			Log("GC:This restart should only happen once!!!!");
			mySnoopControl.RestartDevice();
			Sleep(500);
			UsbSnoopQueryLoggingState( &m_LoggingState );
			Log("GC:Logstate is %lu",m_LoggingState);
			if( m_LoggingState & LoggingEnabled )
			{
				//It would be better to pause close and delete but this is simpler for now
				Log("GC:Logging is enabled, just continue FP will forward to correct pos");
			}
			else
			{
				//Resume does not seem to work if there is no log file
				Log("GC:Logging is not enabled, resuming (starting)");
				ResumeLogging();
			}
		}
    }
	return true;
}

void GenericController::QuitLogging()
{
	LogEnterFunction("GC:QuitLogging");
	if( UsbSnoopIsDriverStarted() )
    {
		Log("Driver is started");
        UsbSnoopQueryLoggingState( &m_LoggingState );
		Log("GC:Logstate is %lu",m_LoggingState);

        if( m_LoggingState & LoggingEnabled )
        {
			Log("GC:Logging is enabled");
			PauseLogging();
			Sleep(1000);
			CloseLogFile();
			Sleep(500);
            /*GetDlgItem( IDC_BUTTON_START )->EnableWindow( FALSE );
            GetDlgItem( IDC_BUTTON_STOP )->EnableWindow( TRUE );
            GetDlgItem( IDC_DELETE )->EnableWindow( FALSE );
            GetDlgItem( IDC_BUTTON_CLOSE_FILE )->EnableWindow( FALSE );*/
        }
        else
        {
			Log("GC:Logging is not enabled");
            //GetDlgItem( IDC_BUTTON_START )->EnableWindow( TRUE );
            //GetDlgItem( IDC_BUTTON_STOP )->EnableWindow( FALSE );

            if( m_LoggingState & LogFileOpen )
            {
				Log("GC:Logfile is open closing");
				CloseLogFile();
                //GetDlgItem( IDC_BUTTON_CLOSE_FILE )->EnableWindow( TRUE );
                //GetDlgItem( IDC_DELETE )->EnableWindow( FALSE );
            }
            else
            {
				Log("GC:Logfile is not open, no action");
                //GetDlgItem( IDC_BUTTON_CLOSE_FILE )->EnableWindow( FALSE );
                //GetDlgItem( IDC_DELETE )->EnableWindow( TRUE );
            }
        }
    }
    else
    {
        /*GetDlgItem( IDC_BUTTON_START )->EnableWindow( FALSE );
        GetDlgItem( IDC_BUTTON_STOP )->EnableWindow( FALSE );
        GetDlgItem( IDC_DELETE )->EnableWindow( TRUE );
        GetDlgItem( IDC_BUTTON_CLOSE_FILE )->EnableWindow( FALSE );*/
    }
	LogDelete();
}

void GenericController::PreHardRestart()
{
	LogEnterFunction("GC:PreHardRestart");
	if(mySettingsControl->GetOSId() < WINDOWS_SERVER_2003)
	{
		Log("GC:PreHardrestart on old os, ignoring");
		return;
	}
	if( UsbSnoopIsDriverStarted() )
    {
		Log("GC:Driver is started");
        UsbSnoopQueryLoggingState( &m_LoggingState );
		Log("GC:Logstate is %lu",m_LoggingState);
        if( m_LoggingState & LoggingEnabled )
        {
			Log("GC:Logging is enabled, do nothing");
        }
        else
        {
			Log("GC:Logging is not enabled, resume it");
			ResumeLogging();
			Sleep(500); //After this restart will be made
        }
    }
    else  //If driver was not started, the restart should have fixed this.
    {
       Log("GC:Driver was not started the restart will fix this");
    }
}

void GenericController::PostHardRestart()
{
	LogEnterFunction("GC:PostHardRestart");
	if(mySettingsControl->GetOSId() < WINDOWS_SERVER_2003)
	{
		Log("GC:Post hardrestart on old os, ignoring");
		return;
	}
	Sleep(1000);  //not sure if the driver is loaded, just for security here
	if( UsbSnoopIsDriverStarted() )
    {
		Log("GC:Driver is started");
        UsbSnoopQueryLoggingState( &m_LoggingState );
		Log("GC:Logstate is %lu",m_LoggingState);
        if( m_LoggingState & LoggingEnabled )
        {
			Log("GC:Logging is enabled, do nothing");
        }
        else
        {
			Log("GC:Logging is not enabled, resume it");
			ResumeLogging();
        }
    }
    else  //If driver was not started, the restart should have fixed this
    {
       Log("GC:Driver was not started after a replug, this should not happen");
	   MessageBox(NULL,"Driver was not started after a replug, this should not happen.\n\nContact support if this happens","",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
    }
}


void GenericController::PrepareForHardRestart()
{
	LogEnterFunction("GC:PrepareForHardRestart");
	if(mySettingsControl->GetOSId() < WINDOWS_SERVER_2003)
	{
		Log("GC:Prepare for hard restart on old os, ignoring");
		return;
	}
	if( UsbSnoopIsDriverStarted() )
    {
		Log("GC:Driver is started");
        UsbSnoopQueryLoggingState( &m_LoggingState );
		Log("GC:Logstate is %lu",m_LoggingState);
        if( m_LoggingState & LoggingEnabled )
        {
			/*Log("GC:Logging is enabled, close down so we get a fresh file");
			PauseLogging();
			Sleep(500);
			CloseLogFile();
			Sleep(500);
            GetDlgItem( IDC_BUTTON_START )->EnableWindow( FALSE );
            GetDlgItem( IDC_BUTTON_STOP )->EnableWindow( TRUE );
            GetDlgItem( IDC_DELETE )->EnableWindow( FALSE );
            GetDlgItem( IDC_BUTTON_CLOSE_FILE )->EnableWindow( FALSE );*/
        }
        else
        {
			ResumeLogging();
          /*  GetDlgItem( IDC_BUTTON_START )->EnableWindow( TRUE );
            GetDlgItem( IDC_BUTTON_STOP )->EnableWindow( FALSE );

            if( m_LoggingState & LogFileOpen )
            {
                GetDlgItem( IDC_BUTTON_CLOSE_FILE )->EnableWindow( TRUE );
                GetDlgItem( IDC_DELETE )->EnableWindow( FALSE );
            }
            else
            {
                GetDlgItem( IDC_BUTTON_CLOSE_FILE )->EnableWindow( FALSE );
                GetDlgItem( IDC_DELETE )->EnableWindow( TRUE );
            }*/
        }
    }
    else  //If logging is not enabled, the restart will fix this
    {
        /*GetDlgItem( IDC_BUTTON_START )->EnableWindow( FALSE );
        GetDlgItem( IDC_BUTTON_STOP )->EnableWindow( FALSE );
        GetDlgItem( IDC_DELETE )->EnableWindow( TRUE );
        GetDlgItem( IDC_BUTTON_CLOSE_FILE )->EnableWindow( FALSE );*/
    }
}

void GenericController::LogDelete()
{
	if(mySettingsControl->GetOSId() < WINDOWS_SERVER_2003)
	{
		Log("GC:LogDelete on old os, ignoring");
		return;
	}
	if( UsbSnoopIsDriverStarted() )
    {
        // Ask Driver to Delete File
	    if (!UsbSnoopDeleteLogFile())
		{
			Log("GC:Failed to delete log file, if 2k its expected");
		    //MessageBox(NULL,"Failed to delete log file","",MB_OK);
		}
    }
    else
    {
        // User-Mode File Delete
		TCHAR sLogPath[MAX_PATH];
		if (GetWindowsDirectory(sLogPath, sizeof(sLogPath)) == 0)
		{
				MessageBox(NULL,"Failed to get dir","",MB_OK);
		}
		_tcscat(sLogPath, _T("\\") );
		_tcscat(sLogPath, _T("hspf.log"));

		
	    if (!DeleteFile(sLogPath))
		{
			//MessageBox(NULL,"Failed to delete log file","",MB_OK);
		}
		    
    }
}
