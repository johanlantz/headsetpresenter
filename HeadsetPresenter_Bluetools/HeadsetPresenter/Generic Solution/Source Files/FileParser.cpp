//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
// FileParser.cpp: implementation of the FileParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "sniffusb.h"
#include "FileParser.h"
#include <time.h>  //test


#include "GenericController.h"


#include "SS_Log.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool FileParser::THREAD_STOP = true;
int FileParser::readDelay = 200;  //Changed from default value 400
int FileParser::fileAccessDelay = 2000;
bool FileParser::manualUnplug = false;




FileParser::FileParser(GenericController* imyController)
{
	this->myController = imyController;
	hThread = 0;
	h_WaitForThreadExit = CreateEvent(NULL,FALSE,FALSE,NULL);
	ResetEvent(h_WaitForThreadExit);
}

FileParser::~FileParser()
{

}

DWORD WINAPI ThreadFunc(LPVOID lpParam)
{
	FileParser* fp = (FileParser*)lpParam;
	fp->RunParseThread();
	return 0;
}

void FileParser::SetReadDelay(CString newDelay)
{
	readDelay = atoi( (LPCTSTR)newDelay );
	Log("FP:readDelay=%d",readDelay);
}

void FileParser::SetAccessFileDelay(CString newDelay)
{
	fileAccessDelay = atoi( (LPCTSTR)newDelay );
	Log("FP:FileAccessDelay=%d",fileAccessDelay);
}

void FileParser::SetReplugMode(int iReplugMode)
{
	if(iReplugMode == REPLUG_MANUAL)
	{
		manualUnplug = true;
	}
	else
	{
		manualUnplug = false;
	}
}



void FileParser::StartParseThread()
{
	

	if(hThread != 0)
	{
		StopParseThread();
	}

	DWORD dwThrdParam = 1;
	THREAD_STOP = false;
	hThread = CreateThread(NULL,0,ThreadFunc,this,0,&dwThreadId);

	if(hThread == NULL)
	{
		MessageBox(NULL,"Create thread failed","Error",MB_OK|MB_ICONINFORMATION);
	}
}

//This is called by GenericController even if HeadsetPresenter was never started
void FileParser::StopParseThread()
{
	LogEnterFunction("FP:StopThread");
	
	
	THREAD_STOP=true;
	/*while(THREAD_STOP == true)
	{
		Sleep(100);
	}*/
	DWORD res = WaitForSingleObject(h_WaitForThreadExit,5000);
	switch(res)
	{
		case WAIT_ABANDONED:
			Log("FP:Continue,WAIT_ABANDONED ");
			break;
		case WAIT_OBJECT_0:
			Log("FP:Continue OK,WAIT_OBJECT_0 ");
			break;
		case WAIT_TIMEOUT:
			Log("FP:Continue WAIT_TIMEOUT ");
			break;
		case WAIT_FAILED:
			Log("FP:Continue NOK, WAIT_FAILED ");
			break;
		default:
			Log("FP:Unknown result from wait");
	}
	
	if(hThread != NULL)
	{
		CloseHandle(hThread);
		hThread = 0;
	}

}

bool FileParser::IsThreadRunning()
{
	if(THREAD_STOP == false)
		return true;
	else
		return false;
}

void FileParser::RunParseThread()
{
	Log("FP:Parse Thread started");
	Sleep(fileAccessDelay);  //let the driver kick in, this is needed. If there is much info written to the log at startup or if the computer is slow more time might be needed.
	TCHAR logPath[255]; 
	
	if(GetWindowsDirectory(logPath,sizeof(logPath)) == 0)
	{
		MessageBox(NULL,"Could not init local directory.\n\nTrying default.","Error",MB_OK|MB_ICONINFORMATION);
		_tcscat(logPath, _T("c:\\winnt\\hsp.log"));
	}
	else
	{
		_tcscat(logPath, _T("\\hsp.log"));
	}
	Log("FP:Setting log path to: %s ", logPath);
	//HANDLE hFile = CreateFile("c:\\winnt\\hsp.log",GENERIC_ALL,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	
	//If the USB device can't handle a soft reset it might work with a manual unplug
	if(manualUnplug)
	{
		MessageBox(NULL,"Manual Unplug is needed for this device.\n\nPlease unplug and then replug your Bluetooth dongle now. Wait a second until it is active again. Then press OK","Manual unplug",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		Log("FP:Manual unplug done");
	}
	HANDLE hFile;
	bool fileOpenedOk = false;
	bool userAborted = false;
	bool aRetryWasDone = false;
	bool initialCreateFailed = false;
	INT_PTR boxRes;
	do
	{
		hFile = CreateFile(logPath,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if(hFile == INVALID_HANDLE_VALUE)  //File did not exist or : This error can pop up if the wrong .sys file is used. If to much data is passed to the usblog.sys it needs more then 2sec to start 
		{
			DWORD dwLastError = GetLastError();

			if (dwLastError != NO_ERROR)
			{
				char msg [100];
				sprintf(msg,"GetLastError() = %d",dwLastError);
				Log("FP:Critical, file open failed with code %s check GetLastError code in doc", msg);
			}
			Log(LOGTYPE_CRITICAL,"FP:Unable to open hsp.log");
			if(myController->GetSettingsControl()->GetRestartDeviceHandling() & RESTART_DEVICE_NEVER)
			{
				Log("FP:DeviceRestartHandling is set to NEVER");
				//Wait and try again, restart should never be needed here
				//Possibly the file is not created until a command is received
				Sleep(1000); 
				Log("FP:File not created, sleeping");
				initialCreateFailed = true;
			}
			else
			{
				Log("FP:DeviceRestartHandling is set to ALWAYS");
				if(!manualUnplug)
				{
					boxRes = MessageBox(NULL,"HeadsetPresenter could not initialize your Bluetooth device.\n\nA manual unplug of your device could solve this issue.\n\nClick Yes below to try this or No to quit","Startup Error",MB_YESNO|MB_TOPMOST|MB_ICONINFORMATION);
					if(boxRes == IDYES)
					{
						Log("FP:Will try a manual unplug");
						aRetryWasDone = true;
						MessageBox(NULL,"Manual Unplug might be needed to init this device.\n\nPlease unplug and then replug your Bluetooth dongle now.\n\nIf you are using a laptop it might be possible to turn the device on and off with the keyboard or through an application.\n\nWait a second until it is active again. Then press OK","Manual unplug",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
					}
					else
					{
						Log("FP:User has aborted");
						userAborted = true;
					}	
				}
				else
				{
					MessageBox(NULL,"HeadsetPresenter could not initialize you device in advanced mode.\n\nContact support for more assistance","Failed to init device",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
					userAborted = true;
				}
			}
		}
		else  //File opened ok
		{
			fileOpenedOk = true;
		}
	}while(fileOpenedOk == false && userAborted == false);
	
	if(aRetryWasDone && fileOpenedOk)
	{
		MessageBox(NULL,"The HeadsetPresenter has initialized successfully","Init ok",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
	}
	
	char inBuffer[500];
	inBuffer[499] = '\0';
	DWORD nBytesToRead = 499;
	DWORD nBytesRead;
	BOOL bResult;

	//Go to the end of the file
	//if initialCreate is true then the first pos in the file contains interesting info so dont forward
	if(!initialCreateFailed)
	{
		do
		{
			bResult = ReadFile(hFile, &inBuffer, nBytesToRead, &nBytesRead, NULL) ; 
			Log("FP:Initial read from file gave %d, bytes",nBytesRead);
		}while(nBytesRead > 0);
	}
	
	do
	{
		bResult = ReadFile(hFile, &inBuffer, nBytesToRead, &nBytesRead, NULL) ; 
		
		if(nBytesRead == 0)
		{
			::Sleep(readDelay);
		}
		else
		{
			inBuffer[nBytesRead] = '\0';
			Log("Read from buffer: %s", inBuffer);
			myController->OnReadCompleted(inBuffer);  //Got VGS
		}
	}
	while(THREAD_STOP == false);
	CloseHandle(hFile);  // is this the correct place ? should be
	Log("FP:Thread exit");
	THREAD_STOP = false;
	SetEvent(h_WaitForThreadExit);
}

