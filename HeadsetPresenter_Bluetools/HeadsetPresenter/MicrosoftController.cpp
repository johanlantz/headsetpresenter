
#include "StdAfx.h"
#ifdef MS_STACK  //Must have this one below stdafx.h probably there is a double endif somewhere in the .h files inclued by stdafx.h
#include ".\microsoftcontroller.h"
#include "HeadsetPresenterDlg.h"
#include "SS_Log.h"


DWORD WINAPI ThreadFuncM( LPVOID lpParam ) 
{ 
	MicrosoftController* t = (MicrosoftController*)lpParam;
	t->RunSocketReceive();
  return 0; 
} 

MicrosoftController::MicrosoftController(void)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );
 
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) 
	{
		MessageBox(NULL, "Could not init sockets", "Error", MB_OK);
		exit(0);
	}

	hThread = 0;
	myDevice = 0;
}

MicrosoftController::~MicrosoftController(void)
{
	int socketShutdown = shutdown(s,SD_SEND);
}


bool MicrosoftController::Start()
{
	return true;
}

bool MicrosoftController::IsRadioAvailable()
{
	bool foundRadio = false;
	HANDLE hRadio;
    BLUETOOTH_FIND_RADIO_PARAMS btfrp = { sizeof(btfrp) };

    HBLUETOOTH_RADIO_FIND hFind = BluetoothFindFirstRadio( &btfrp, &hRadio );
    if ( NULL != hFind )
    {
  
		Log("MSC:Found a Microsoft Radio");
		CloseHandle( hRadio );
        //BluetoothFindRadioClose( hFind );
		foundRadio = true;
    }
	else
	{
		Log("MSC: No Microsoft Radio was found");
	}
	return foundRadio;
}

void MicrosoftController::StartReceiveThread()
{
	if(hThread !=0)	//If there is a thread running wait for it to stop before starting a new one
	{
		StopThread();
		while(CMD_QUIT ==true)
		{
			Sleep(100);		
		}
	}
	
	DWORD  dwThrdParam = 1; 
	CMD_QUIT = false;   
	hThread = CreateThread( 
        NULL,                        // default security attributes 
        0,                           // use default stack size  
        ThreadFuncM,                  // thread function 
        this,                // argument to thread function 
        0,                           // use default creation flags 
        &dwThreadId);                // returns the thread identifier 
 
   // Check the return value for success. 
	
   if (hThread == NULL) 
   {
      MessageBox( NULL, "Create thread failed. Please contact support if this error persist.", "main", MB_OK );
   } 	
}

void MicrosoftController::StopThread()
{	
	Log("MSC:Stopping thread");
	CMD_QUIT = true;
	Sleep(200);
	CloseHandle( hThread );
	hThread = 0;
}

void MicrosoftController::StoreHeadsetAddress(BTH_ADDR headsetAddress)
{
	char buf[100];
	_ui64toa(headsetAddress,buf,16);
	mySettingsControl->SetLastDeviceAddress(buf);
	Log("BTC:Stored Headset Address: %s",buf); 
}


BTH_ADDR MicrosoftController::RestoreHeadsetAddress()
{
	BTH_ADDR oldAddress = 0;
	oldAddress = _strtoui64(mySettingsControl->GetLastDeviceAddress(),'\0',16);
	Log("Restored lastDeviceAddress as %d",oldAddress);
	return oldAddress;
}

bool MicrosoftController::Connect(bool reconnect){
	
	_SOCKADDR_BTH bthAddr = { sizeof(bthAddr) };
	int res;
	s = socket(AF_BTH, SOCK_STREAM,BTHPROTO_RFCOMM);
	
	/*Define socket timeout*/
	int timeout = 90;  //maybe this value is to small
	int err;
	err = setsockopt(s, SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout));
	if(err != NO_ERROR){ 
		int resbox = MessageBox( NULL, "Bluetooth Connection error.\n\nMost likely this is caused by using the Microsoft version of HeadsetPresenter\non a computer using a non Microsoft Bluetooth stack or that the Bluetooth adapter is not connected properly.\n\nRead the HeadsetPresenter for Beginners guide for to make sure that the correct version is running.\n\nIf the problem persist contact support at support@headsetpresenter.com.\n\nMore help might also be available on the HeadsetPresenter website.\n\n Do you want to be directed there?", "Bluetooth Connection Error", MB_YESNO|MB_ICONINFORMATION );
		/*Log("BTC:setsocketopt failed, using Widcomm with MS version?");
		if(resbox==IDYES)
		{
			getHelp(HELP_ID_WIDCOMM);
		}*/
	}

	//Test ejohros
	//if(err!=NO_ERROR && s!= INVALID_SOCKET)
	//{
	//	MessageBox( NULL, "This box is for debug purpose only. ERROR and VALID at the same time.", "Debug", MB_OK );
	//}

	/*end socket*/
	
	if(s != INVALID_SOCKET)
	{
		bthAddr.addressFamily = AF_BTH;
		bthAddr.serviceClassId = HeadsetServiceClass_UUID;
		if(reconnect)
		{
			bthAddr.btAddr = RestoreHeadsetAddress();
			Log("BTC:Reconnecting to:%l",bthAddr.btAddr);
		}
		else
		{
			bthAddr.btAddr = myDevice->Address.ullLong;
			Log("BTC:Connecting to:%h",bthAddr.btAddr);
		}
		res = connect(s,(sockaddr*)&bthAddr, sizeof(SOCKADDR_BTH));
		if(res != -1)
		{
			send(s,"\r\nRING\r\n", strlen("\r\nRING\r\n"),0);
			
			Log("BTC:Connection ok, sending RING");
		}
		else
		{
			if(!reconnect)  //The new connection failed. If oldAddr != 0 the reconnect fail and then we start the wizard
			{
				Log("BTC:Connection to new Headset failed");
				int resbox = MessageBox( NULL, "Unable to connect to headset.\nPlease make sure the the headset is not connected to your phone.\nEither select disconnect device in your Phone or switch\nBluetooth off in the Phone.\n\nMore help might be available on the HeadsetPresenter website. Would like like to go there? ", "Connection failed", MB_YESNO|MB_ICONINFORMATION );
				if(resbox == IDYES)
				{
					GetHelp(HELP_ID_CONNECT_ERROR);
				}
			}
			int t = WSAGetLastError();
		}
	}
  return res != -1 ? true : false; 
}



bool MicrosoftController::ConnectToLastUsedDevice()
{
	bool connectionOK = false;
	connectionOK = Connect(true);
	if(connectionOK)
	{
		StartReceiveThread(); //4. Reconnect succeded start thread
		Log("BTC:Connection to lastUsedHeadset succeeded");
	}
	return connectionOK;
}

bool MicrosoftController::ShowDeviceSearchDialog()
{
	BLUETOOTH_SELECT_DEVICE_PARAMS btsdp =  {sizeof(btsdp)};
	btsdp.hwndParent = NULL;
	btsdp.fShowUnknown = TRUE;
	btsdp.fAddNewDeviceWizard = TRUE;
	  	
	btsdp.cNumOfClasses = 0;
	btsdp.prgClassOfDevices = NULL;

	btsdp.fForceAuthentication = FALSE;
	btsdp.fShowRemembered = TRUE;  //if this is set to false it seems as the wizard has problems finding the headset sometimes

	btsdp.fSkipServicesPage = FALSE;
	btsdp.pfnDeviceCallback = NULL;
	btsdp.pvParam = NULL;
	btsdp.cNumDevices = 0;

     BOOL deviceSelected = BluetoothSelectDevices( &btsdp );
     if ( deviceSelected )
     {
		 Log("BTC:A device has been selected");
        BLUETOOTH_DEVICE_INFO* btspi = btsdp.pDevices;  //pointer to the selected device
     
		for ( ULONG cDevice = 0; cDevice < btsdp.cNumDevices; cDevice ++ )
        {
          if ( btspi->fAuthenticated || btspi->fRemembered )
          {
			  Log("BTC:The selected device is authenticated");
            //if(IsHeadsetAllowed(btspi, CHECK_ADDRESS))
			//{
			StoreHeadsetAddress(btspi->Address.ullLong);	  
			myDevice = btspi;  //is there a copy constructor otherwise this will be killed by selcteddevicesfree
			USES_CONVERSION;
			//MessageBox(NULL, W2A(myDevice->szName), "Cancel", MB_OK);
			mySettingsControl->SetLastDeviceName(W2A(myDevice->szName));
			//}
          }
		  else	
		  {
		    Log("Device was not authenticated");
			MessageBox(NULL,"Device must be authenticated","Authenticate",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
    	  }
			btspi = (BLUETOOTH_DEVICE_INFO *) ((LPBYTE)btspi + btspi->dwSize);
	    } 
     }
	 else
	 {
		 int res = MessageBox(NULL, "Device selection canceled.\n\nIf connection failed please make sure the the headset is not connected to your phone.\nEither disconnect headset or turn Bluetooth off in the Phone.\n\nIf passkey exchange fails, make sure that headset is pairable.\n\nMore help might be available on the HeadsetPresenter website. Would like like to go there? " , "Device selection Cancel", MB_YESNO|MB_ICONINFORMATION);
		 if(res==IDYES)
		 { 
			GetHelp(HELP_ID_DEVICE_SELECTION_CANCELED);
		 }
	}
	bool connectionOK = false;
	if(myDevice != 0)
	{	
		connectionOK = Connect(false);
		if(connectionOK)
		{
			StartReceiveThread(); //3. Starts socket receive and powerpoint
			//MessageBox(NULL,"Connection to Headset established successfully.\n\nStart the presentation and move forward with the connect button.\n\nIf your Headset supports it you can also move forward and backward with the volume buttons.","HeadsetPresenter",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
			Log("BTC:Connect was ok");
		}
	}
    return connectionOK;
}

void MicrosoftController::DiscoverDevices(bool refresh){}

void MicrosoftController::DeviceSelected(int deviceIndex){}

CStringList* MicrosoftController::GetServiceListAsCStringList()
{
	MessageBox(NULL,"GetServiceListAs.. this must never happen","",MB_OK);
	return NULL;
}

bool MicrosoftController::ConnectToSelectedDevice(int SCN, CString serviceName )
{
	return true;
	//Should be handled by pairing guide
}

void MicrosoftController::DiscoverServicesAsync(){}

bool MicrosoftController::DiscoverServicesSync()
{
	return true;
}

int MicrosoftController::GetNoOfServicesDiscovered()
{
	return MICROSOFT_MODE_SERVICE_COUNT;
}

int MicrosoftController::GetSCNOfService(CString serviceNameToFind)
{
		MessageBox(NULL,"SCNofserv","",MB_OK);
		return 0;
}


void MicrosoftController::InitLocalSettings(){}

int MicrosoftController::GetMode()
{
	return NORMAL_MODE_MICROSOFT;
}

void MicrosoftController::RunSocketReceive()
{
	Log("BSC:Start RunSocketReceive");
	
	
	//myPP->StartActivePresentation(); //Start up the presentation that we know is loaded
	//time_t then, now;
	//time(&then);
	
	char buf[100];
	int status = -1;

	while(CMD_QUIT == false)
	{
		status = recv(s, &buf[0], 100,0);	

		if(status == -1)
		{
			//MessageBox(NULL,"Timeout while reading from socket","Timeout",MB_OK);  //First since it happens alot of times
			//other errors possible should use WSSAgetlasterror to investigate if necessary
		}
		else if(status > 0)
		{
			Log("MSC:Received %s",buf);
			OnReadCompleted(buf);
			send(s,"\r\nOK\r\n", strlen("\r\nOK\r\n"),0);
			//App control here, investigate time now and then, is it needed
		}
			/* && buf[4] == 'K' && buf[5] == 'P' && buf[6] == 'D')  //Check that we receive AT*CKPD
		{
			send(s,"\r\nOK\r\n", strlen("\r\nOK\r\n"),0);
			myPP->NextSlide2();
			time(&then);  
			Log("BTC:CKPD received");
		}
		else if(status > 0 && buf[3] == 'V' && buf[4] == 'G' && buf[5] == 'S') //Check for Volume up or down
		{
			send(s,"\r\nOK\r\n", strlen("\r\nOK\r\n"),0);  //Sending OK immidiatly to save milliseconds when moving fast forward
			time(&now);
			if((now-then) < 2 && currentVol == -1)  //Initial VGS message after a CKPD
			{
				currentVol = atoi(&buf[7]);
				Log("BTC:VGS received now-then<2 and currentVol==-1, init message. CurrentVol=%d",currentVol);
			}
			else
			{
				int newVolume = atoi(&buf[7]);
				
				if(newVolume >= currentVol && newVolume != 0)
				{
					myPP->NextSlide2();
					Log("BTC:VGS received: newVol=%d AND currentVol=%d. Moving forward",newVolume, currentVol);
				}
				else
				{
					myPP->PrevSlide2();
					Log("BTC:VGS received: newVol=%d AND currentVol=%d. Moving backwards",newVolume, currentVol);

				}
				currentVol = newVolume;
			}
			//send(s,"\r\nAT+VGS=15\r\n", strlen("\r\nAT+VGS=15\r\n"),0);
			//send(s,"\r\nOK\r\n", strlen("\r\nOK\r\n"),0); 
		}
		else if(status > 0) //We need to send ok for AT*ECBP to avoid delays
		{
			//MessageBox(NULL,buf,"",MB_OK);
			
			//ATLTRACE2( buf );
			Log("BTC:Received other command: %s",buf);
			send(s,"\r\nOK\r\n", strlen("\r\nOK\r\n"),0);  
		}
		*/
		else if(status == 0) 
		{
			MessageBox(NULL,"Connection closed by Headset or Headset out of range.","Connection closed",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);  //here we should handle a status that indicates that the connection is lost
			Log("MSC:Connection closed or lost");
			StopThread();	
		}
	}
	
	CMD_QUIT = false;
	Log("BTC:Exiting RunSocketReceive");
}

void MicrosoftController::OnReadCompleted(char* data)
{
	//bool dataIsATCommand = false;
	int commandType = 0;
	CString dataString;
	dataString = data;
	Log("GC:data read and copied as : %s",dataString);
	
	
	//1. Check if it is an AT command
	if(dataString.Find("AT+") != -1 )
	{
		Log("GC:Data is AT command");
		//dataIsATCommand = true;
		commandType = AT_COMMAND;
	}
	//Not AT command, AVRCP
	else
	{
		//dataIsATCommand = false;
		commandType = AVRCP_COMMAND;
		Log("GC:Data is not AT command, sending : %s",dataString);
	}
	HandleStatesAndForwardCommand(dataString, commandType, TRUE);
	
	/*if(wizardDialog->GetWizardState() == STATE_WIZARD_TRY_HFP || wizardDialog->GetWizardState() == STATE_WIZARD_TRY_HSP || wizardDialog->GetWizardState() == STATE_WIZARD_TRY_UNKNOWN)
	{
		wizardDialog->CommandReceived(dataString,commandType);
	}
	else if(state == STATE_CONNECTED)
	{
		myHeadsetPresenterCallback->CommandCallback(dataString, commandType);
	}
	else if(state == STATE_NOT_CONNECTED)
	{
		SetState(STATE_CONNECTED);
		myHeadsetPresenterCallback->CommandCallback(dataString, commandType);
	}*/
}
#endif