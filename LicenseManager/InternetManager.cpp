// InternetManager.cpp : implementation file
//

#include "stdafx.h"
#include "LicenseManager.h"
#include "InternetManager.h"
#include <Wininet.h>

// InternetManager

IMPLEMENT_DYNAMIC(InternetManager, CWnd)
InternetManager::InternetManager()
{
	httpAddress = "www.licensemanager.se";  //if site changes check if path to requestLicense.asp also has changed
	Log("Setting httpAddress to: %s",httpAddress);
}

InternetManager::~InternetManager()
{
	Log("Destroy InternetManager");
}

/*Handles connection to server. WinInet classes used due to problem with proxys demanding 
authentication on the MFC classes and openURL, MFC can probably be used remember to use KEEP_CONNECTION to get through proxys*/
CString InternetManager::SendRequestToServer(CString queryString)
{
	LogEnterFunction("SendRequestToServer");
	CString retVal = "SERVER_ERROR_OR_NO_CONNECTION";

	HINTERNET hOpenHandle,  hConnectHandle, hResourceHandle;
	DWORD dwError, dwErrorCode;
	BOOL sendStatus;
	
	hOpenHandle = InternetOpen("Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)", 
							INTERNET_OPEN_TYPE_PRECONFIG, 
							NULL, NULL, 0);

	hConnectHandle = InternetConnect(hOpenHandle,
									httpAddress, 
									INTERNET_INVALID_PORT_NUMBER,
									NULL,
									NULL, 
									INTERNET_SERVICE_HTTP,
									0,0);
	
	hResourceHandle = HttpOpenRequest(hConnectHandle, "GET",
									queryString,
									NULL, NULL, NULL, 
									INTERNET_FLAG_KEEP_CONNECTION|INTERNET_FLAG_RELOAD|INTERNET_FLAG_NO_CACHE_WRITE , 0);

	resend:
	
	sendStatus = HttpSendRequest(hResourceHandle, NULL, 0, NULL, 0);

	// dwErrorCode stores the error code associated with the call to
	// HttpSendRequest.  

	dwErrorCode = hResourceHandle ? ERROR_SUCCESS : GetLastError();

	dwError = InternetErrorDlg(NULL, hResourceHandle, dwErrorCode, 
							FLAGS_ERROR_UI_FILTER_FOR_ERRORS | 
							FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS |
							FLAGS_ERROR_UI_FLAGS_GENERATE_DATA,
							NULL);

	if (dwError == ERROR_INTERNET_FORCE_RETRY)
	{
		Log("ERROR_INTERNET_FORCE_RETRY");	
		goto resend;
	}
	// Insert code to read the data from the hResourceHandle
	// at this point.
	char buf[500];  //2008-03 Johan changed buffer from 100 to 300 to capture error messages on webpage
	DWORD bytesRead = 0;
	//only perform readfile if SendRequest was successful
	if(sendStatus)
	{
		InternetReadFile(hResourceHandle,buf,499,&bytesRead);
		buf[bytesRead] = '\0';
		retVal = buf;
		Log("sendStatus ok, read: %s", buf);
		//MessageBox(buf,"",MB_OK);
	}
	
	//Cleanup
	if(hResourceHandle != NULL)
	{
		InternetCloseHandle(hResourceHandle);
	}
	if(hConnectHandle != NULL)
	{
		InternetCloseHandle(hConnectHandle);
	}
	if(hOpenHandle != NULL)
	{
		InternetCloseHandle(hOpenHandle);
	}
	//End Cleanup
	Log("Returning: %s",retVal);
	return retVal;
}


BEGIN_MESSAGE_MAP(InternetManager, CWnd)
END_MESSAGE_MAP()



// InternetManager message handlers

