//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include ".\BluetoolsController.h"
#include "SS_Log.h"
//#include "InfoDialog.h"
//#include "ManualPinDialog.h"
//#include "PreparePairingDlg.h"
#include "AlreadyConnectedToAGDialog.h"


#include "HeadsetPresenterDlg.h"  //Must include here for all sub controllers
bool BluetoolsController::isDisconnecting = false;

BluetoolsController::BluetoolsController()
{
	Log("BTC:Enter BT constr");
	controllerMode = 0;  //change this when we know if we run MS or Widcomm
	lastPairingFailed = false;  
	m_network = NULL;
	m_manager = NULL;
	m_device = NULL;

	m_listDevices = new xList();
	m_listServices = new xList();

	xObject::SetUseExceptions(true);  //JJ changed from false
	
	//isReconnecting = false;
	//forceVGS=false;
	//JCHANGE2 Moved from ShowDeviceDiscoveryDialog
	deviceDiscoveryDialog = new DeviceDiscoveryDialog(); //Should use the same declare in constructor?
	serviceDiscoveryDialog = new ServiceDiscoveryDialog();
	//wizardDialog = new WizardDialog();
	deviceDiscoveryDialog->SetController(this);  //Can this be done in the superclass?????
	//Always create a serviceDiscoveryDialog as well so we can use it both from here and from the wizard if needed
	//serviceDiscoveryDialog = new ServiceDiscoveryDialog();
	serviceDiscoveryDialog->SetController(this);
	//wizardDialog->SetController(this); //DO not SetController for wizard here, needs supercontroller
	//End moved

	h_WaitForDisconnect = CreateEvent(NULL,FALSE,FALSE,NULL);
	ResetEvent(h_WaitForDisconnect);
}

bool BluetoolsController::Start()
{
	InitBluetooth();
	return true;
}

bool BluetoolsController::IsRadioAvailable()
{
  return TRUE; //TBI for widcomm, currently checked in registry 
}

BluetoolsController::~BluetoolsController(void)
{
	myHeadsetPresenterCallback->StopTimer();
	Log("BTC:Hiding main window");
	myHeadsetPresenterCallback->ShowWindow(SW_HIDE);
	DisconnectRemoteDevice();
	
	Log("BTC:Will wait for disconnectEvent in OnReadComplete");
	DWORD res = WaitForSingleObject(h_WaitForDisconnect,8000);
	switch(res)
	{
		case WAIT_ABANDONED:
			Log("BTC:Continue,WAIT_ABANDONED ");
			break;
		case WAIT_OBJECT_0:
			Log("BTC:Continue OK,WAIT_OBJECT_0 ");
			break;
		case WAIT_TIMEOUT:
			Log("BTC:Continue WAIT_TIMEOUT ");
			break;
		case WAIT_FAILED:
			Log("BTC:Continue NOK, WAIT_FAILED ");
			break;
		default:
			Log("BTC:Unknown result from wait");
	}

	/*if(deviceDiscoveryDialog)
	{
		delete deviceDiscoveryDialog;
	}

	if(wizardDialog)
	{
		delete wizardDialog;
	}
	if(serviceDiscoveryDialog)
	{
		delete serviceDiscoveryDialog;
	}*/
	
	//Workaround to handle that dispose hangs after failed pairing
	//This is not that nice since it do work in the FindAndConnect example
	//For some reason the manager->dispose hangs when a pairing has failed in HeadsetPresenter
	if(!lastPairingFailed)
	{
		m_cleanup();
	}
	else
	{
		Log("BTC:WARNING a pairing failed, full dispose has not been done");
	}
}


int BluetoolsController::GetMode()
{
	return controllerMode;
}

bool BluetoolsController::InitBluetooth()
{
	Log("BTC:If it crash here Bluetools lib is likely not available in the execution dir");
	m_manager = NULL;
	xManager::GetManager(&m_manager);

	if(m_manager == NULL)
	{
		MessageBox(NULL,_T("Bluetooth not found!"), "", MB_OK);
	}
	else
	{
		//If more then one stack is installed we should probably search for a Widcomm one
		switch(m_manager->GetStackID())
		{
			case STACK_WIDCOMM:
				Log("BTC:Widcomm detected");
				controllerMode = NORMAL_MODE_WIDCOMM;
				break;
			case STACK_MICROSOFT:
				Log("BTC:Microsoft detected");
				controllerMode = NORMAL_MODE_MICROSOFT;
				MessageBox(NULL,"Failed to detect a working Widcomm/Broadcomm stack. Please contact support.","",MB_OK);
				break;
			default:
				Log("BTC:Unknown detected");
				MessageBox(NULL,"An unknown stack was detected, this is not yet implemented. If you know that you have a Widcomm or Microsoft stack please contact support","",MB_OK);
		}
		xString* strKey = new xString("XSM6CR334An1tu8wL34IMLdFywLaH50FXtKF");								   
		xLicense* license = new xLicense();
		license->SetLicenseKey(strKey);

		if(license->IsError())
		{
			// Deal with license error
			m_show_error(license);
		}
		else
		{
			xNetworkList* networkList = NULL;
			m_manager->GetNetworks(&networkList);

			if(m_manager->IsError())
			{
				// Deal with network error
				m_show_error(m_manager);
			}
			else
			{
				networkList->GetNetwork(0, &m_network);
				if(m_network)
				{
					// Ask network object to use xINetworkEvents on this object as event callback
					m_network->SetEvents(this);
					xVersion* ver=NULL;
					m_network->GetStackVersion(&ver);
					
				}
			}

			X_RELEASE(networkList);
		}

		X_RELEASE(strKey);
		X_RELEASE(license);
	}
	//m_buttonClose.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void BluetoolsController::InitLocalSettings()
{
	//no local init for Blueools
	//here we can override settings frmo the settings file
}

bool BluetoolsController::ShouldHPQueryForPIN()
{
	return !mySettingsControl->GetLetWidcommQueryForPin(); 
}

void BluetoolsController::UnbondDevice()
{
	if(m_device->GetBonded())
	{
		Log("BTC:Device is bonded unbond before pairing.");
		m_device->Unbond();
	}
	else
	{
		Log("BTC:Device is not bonded, no unbond before pairing");
	}
}
bool BluetoolsController::ShowDeviceSearchDialog()
{
	return SuperShowDeviceSearchDialog(); //needed since Micsoroft stack does not use it
}
	/*
	//Done for non Microsoft stacks, implemented in SuperController for reuse
	if(!ShowPreparePairingDlg())
	{
		return false;
	}

	bool aDeviceWasSelected = false;
	LogEnterFunction("BTC:ShowDeviceSearchDialog");
	
	INT_PTR boxRes = deviceDiscoveryDialog->DoModal();
	if(boxRes == IDOK)
	{
		aDeviceWasSelected = true;  //The selected device is now m_device, no need to return address
		Log("BTC: A device was selected");
		//Try to unbond the device
		if(m_device->GetBonded())
		{
			Log("BTC:Device is bonded unbond before pairing.");
			m_device->Unbond();
		}
		else
		{
			Log("BTC:Device is not bonded, no unbond before pairing");
		}
		if(mySettingsControl->GetLetWidcommQueryForPin())
		{
			Log("BTC:Bonding is handled by native Widcomm GUI");
			InfoDialog myInfoDlg;
			myInfoDlg.SetSettingsControl(mySettingsControl);
			myInfoDlg.SetBoxId(INFO_DLG_PINCODE);
			myInfoDlg.SetText("Wizard help","A pin-code might be needed","HeadsetPresenter will now connect to the selected headset.\n\nA pin-code might be required to complete pairing. If a \"balloon\" appears in the bottom right corner of the screen, click on this message and enter the pin-code of your headset."); 
			if(myInfoDlg.ShowBox())
			{
				myInfoDlg.DoModal();
			}
		}
		else
		{
			Log("BTC:Bonding is handled by HeadsetPresenter");
			aDeviceWasSelected = BondDevice();
		}
	}
	else
	{
		Log("BTC: Device selection dialog was canceled");
	}
	return aDeviceWasSelected;
}
*/


CStringList* BluetoolsController::GetServiceListAsCStringList()
{
	int noOfServices;
	xServiceList* services = NULL;
	CStringList* servicesListAsCStringList = new CStringList(); //is this deleted?
	
	m_device->GetServices(&services);
	noOfServices = services->GetServiceCount();
	Log("SDD: No of services found is %d",noOfServices);
	
	for(int i=0;i<noOfServices;i++)
	{
		xObject* objDummy = NULL;
		xRemoteService* service = NULL;
		m_listServices->Get(i, &objDummy); 
		service = (xRemoteService*) objDummy;
		xString* strName = NULL;
		service->GetName(&strName);
		servicesListAsCStringList->AddTail(strName->GetCharString());
		Log("BTC:Adding service name:%s",strName->GetCharString());
	}

	X_RELEASE(services);
	return servicesListAsCStringList;
}

/*A device has been selected in the DeviceDiscoveryDialog, the index of that device is sent here*/
void BluetoolsController::DeviceSelected(int deviceIndex)
{
	LogEnterFunction("BTC:DeviceSelected");
	Log("BTC: Devices with index %d selected from list", deviceIndex);
	
	// Do not receive events from last device
	if(m_device)
	{
		m_device->SetEvents(NULL);
		X_RELEASE(m_device);
	}

	xObject* objDummy = NULL;
	
	// Get that device from our list of devices
	m_listDevices->Get(deviceIndex, &objDummy);
	m_device = (xRemoteDevice*) objDummy;
	Log("BTC: Got device with index %d from devicelist",deviceIndex);

}

/*void BluetoolsController::PopulateDeviceList()
{
	xObject* objDummy = NULL;

	for(int i=0;m_listDevices->GetLength()-1;i++)
	{
		xObject* objDummy = NULL;
		xRemoteDevice* m_Testdevice;
		xString* name = NULL;
		m_listDevices->Get(i, &objDummy);
		m_Testdevice = (xRemoteDevice*) objDummy;
		m_Testdevice->GetName(&name);
		Log("NISSE:%s",name->GetCharString());
	}
}*/

void BluetoolsController::DiscoverDevices(bool refresh) 
{
	LogEnterFunction("BTC:DiscoverDevices");
	

	if(m_network)
	{
		if(m_network->GetDiscoveryPending())
		{
			// Cancel pending discovery
			m_network->CancelDeviceDiscovery();
			//m_labelStatus.SetWindowText(_T("Discovery canceled!"));
		}
		else
		{
			
			// Start new discovery
			//m_network->DiscoverDevicesAsync();  //This is the normal case

			//xDeviceList* remoteDevices = NULL;  //synch code
			Log("BTC:If the Widcom stack is corrupted the application might crash here. Restart of Windows seems to resolve the problem");
			m_listDevices->Clear();
			try
			{
				//m_network->DiscoverDevices(&remoteDevices);
				m_network->DiscoverDevicesAsync(0);
			}
			catch (xException* ex) 
			{
				xString* s = NULL;
				ex->GetErrorMessage(&s);
				MessageBox(NULL,s->GetTString(),"",MB_OK);
				X_RELEASE(s);
			}
			// Code below is for synch device search
			/*int noOfDevs = remoteDevices->GetDeviceCount();
			for(int i = 0; i < remoteDevices->GetDeviceCount();i++)
			{
				xRemoteDevice* remoteDevice = NULL;
				xDevice* dev = NULL;
				xAddress* devAddr = NULL;
				xString* devAddrAsString = NULL;  //MUST init ALL strings to NULL, crash otherwise
				xString* strName = NULL;

				remoteDevices->GetDevice(i,&dev);
				remoteDevice = (xRemoteDevice*)dev;
				
				remoteDevice->GetAddress(&devAddr);
				devAddr->ToString(&devAddrAsString);
				TRACE(devAddrAsString->GetTString());
			
				remoteDevice->GetName(&strName);
				TRACE(strName->GetTString());
				Log("BTC:Adding device: %s to deviceList on index %d",strName->GetTString(),i); 
				m_listDevices->Add(remoteDevice);
				deviceDiscoveryDialog->AddDeviceToList(strName->GetTString());
	
				X_RELEASE(strName);			
				X_RELEASE(remoteDevice);
			}*/
			//X_RELEASE(remoteDevices);//Is this list needed? OnDeviceDiscovered is called 
		}
	}
	else
	{
		// Network has not been found
	}
}

void BluetoolsController::CancelDeviceDiscovery()
{
	m_network->CancelDeviceDiscovery();
}

void BluetoolsController::Write(unsigned char* data) 
{
	//LogEnterFunction("BTC:Write");
	if(state == STATE_CONNECTED)
	{
		unsigned char* m_buffer;
		m_buffer = data;
		//Log("BTC:About to write the following m_buffer :%s", data);
		xRemoteService* service = NULL;

	///Exception handling here??
		xObject* objDummy = NULL;

		m_listServices->Get(GetIndexOfConnectedService(), &objDummy);   
		service = (xRemoteService*) objDummy;

		if(service)
		{
			//Log("BTC:Got service");
			xStream* stream = NULL;

			try
			{
				service->GetStream(&stream);
			}catch(xException* ex){Log("BTC:Exception in write for GetStream");}

			if(stream)
			{
				Log("BTC:Writing data %s",data);
				try
				{
					stream->Write(m_buffer,0,strlen((const char*)m_buffer));
				}
				catch(xException* ex){Log("BTC:Exception in write, possibly periodical write?");}
			}
			else
			{
				Log("BTC:Failed to get stream");
			}
			TRACE("Writing %s:",m_buffer);
			
		}
		else
		{
			Log("BTC:Failed to get service");
		}
	}
}


// BlueTools: Network event interface

// NOTE!
// All callbacks will be called in a new thread.
// MFC is not thread safe, so take care! You might want to synchronize with the GUI thread
// before updating any controls.

void BluetoolsController::OnDeviceDiscoveryStarted(xNetwork* source)
{
	LogEnterFunction("BTC:DOnDeviceDiscoveryStarted");
}

void BluetoolsController::OnDeviceDiscovered(xNetwork* source, xRemoteDevice* discoveredDevice)
{
	LogEnterFunction("BTC:OnDeviceDiscovered");
	//new asynch code
	xString* strName = NULL;

	discoveredDevice->GetName(&strName);
	//m_boxDevices.AddString(strName->GetTString());
	deviceDiscoveryDialog->AddDeviceToList(strName->GetTString());

	// Keep a list of all discovered devices
	m_listDevices->Add(discoveredDevice);

	X_RELEASE(strName);
	// Keep a list of all discovered devices
	//m_listDevices->Add(discoveredDevice);

}

void BluetoolsController::OnDeviceLost(xNetwork* source, xRemoteDevice* lostDevice)
{
	int inx = m_listDevices->Remove(lostDevice);
	//m_boxDevices.DeleteString(inx);
}

void BluetoolsController::OnDeviceDiscoveryCompleted(xNetwork* source, xDeviceList* discoveredDevices)
{
	LogEnterFunction("BTC:OnDeviceDiscoveryCompleted");
	deviceDiscoveryDialog->DeviceDiscoveryComplete();
}

void BluetoolsController::OnError(xNetwork* source, int errorCode, xString* errorText)
{
	MessageBox(NULL,errorText->GetTString(),"",MB_OK);
}


bool BluetoolsController::DiscoverServicesSync()
{
	bool pairingOK = true;
	if(m_device == NULL)
	{
		MessageBox(NULL,"No remote device, please contact support","",MB_OK|MB_ICONINFORMATION);
	}
	else if(m_device->GetDiscoveryPending() == false)
	{
		// Receive events from this device
		m_device->SetEvents(this);

		// Add previously discovered services to list
		m_listServices->Clear();
		xServiceList* serviceList = NULL;
		try
		{
			m_device->DiscoverServices(xServiceType::RFCOMM(),&serviceList);
		}
		catch(xException* ex)
		{
			Log("BTC:Service discovery failed");
			//MessageBox(NULL,"Service discovery failed for the connected headset.\nIf this problem persists contact support to see if it can be resolved.\n\nThe next message box will try to display the error message given by Widcomm.","Service discovery failed",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
			MessageBox(NULL,"Service discovery failed for the connected headset.\nIf this problem persists contact support to see if it can be resolved.","Service discovery failed",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
			/*xString* str = NULL;
			ex->GetErrorMessage(&str);
			CString err;
			err.Format("Error msg = %s Error code = %d", str->GetTString(), ex->GetErrorCode());
			MessageBox(NULL,err,"Error info for support",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
			Log("BTC: Service discovery failed with msg %s and code %d", str->GetTString(), ex->GetErrorCode());*/
			pairingOK = false;
			lastPairingFailed = true;
		}
		
		for(int i = 0; i < serviceList->GetServiceCount();i++)
		{
			xService* serv = NULL;
			xRemoteService* remoteService = NULL;
			xString* strName = NULL;

			serviceList->GetService(i,&serv);
			remoteService = (xRemoteService*)serv;
			m_listServices->Add(remoteService);
			
			remoteService->GetName(&strName);
			Log("BTC:Found Service: %s",strName->GetCharString());
			X_RELEASE(serv);
			X_RELEASE(strName);

			//Clean up here
		}
		X_RELEASE(serviceList);
	}
	return pairingOK;
}

int BluetoolsController::GetNoOfServicesDiscovered()
{
	int noOfServiceDiscovered = m_listServices->GetLength();
	Log("BTC:GetNoOfServicesDiscovered returns %d",noOfServiceDiscovered);
	return noOfServiceDiscovered;
}
//Perhaps do sync service disc instead
void BluetoolsController::DiscoverServicesAsync()
{
	if(m_device == NULL)
	{
		MessageBox(NULL,"No remote device, please contract support","",MB_OK|MB_ICONINFORMATION);
	}
	else if(m_device->GetDiscoveryPending() == false)
	{
		// Receive events from this device
		m_device->SetEvents(this);

		// Clear contents of service list box, to display services of selected devices
		//m_boxServices.ResetContent();  //only in state manual

		// Add previously discovered services to list
		m_listServices->Clear();
		xServiceList* serviceList = NULL;

		m_device->GetServices(&serviceList);

		if(serviceList->GetServiceCount())
		{
			// Services already discovered
			// Fake discovery to populate m_listServices and list box

			for(int iServiceIndex = 0; iServiceIndex < serviceList->GetServiceCount(); iServiceIndex++)
			{
				xService* service = NULL;
				serviceList->GetService(iServiceIndex, &service);
					
				OnServiceDiscovered(m_device, (xRemoteService*) service);

				X_RELEASE(service);
			}
		}
		else
		{
			// Discover new services
			m_device->DiscoverServicesAsync(xServiceType::RFCOMM());
		}

		X_RELEASE(serviceList);
	}
	else
	{
		//only in manual mode
		//m_labelStatus.SetWindowText(_T("Service discovery pending"));
	}
	
}

// BlueTools: xIRemoteDeviceEvents
void BluetoolsController::OnServiceDiscoveryStarted(xRemoteDevice* source)
{
	LogEnterFunction("BTC:OnServiceDiscoveryStarted");

	//m_labelStatus.SetWindowText(_T("Discovering services!"));

	//m_boxDevices.EnableWindow(FALSE);
	//m_buttonDiscover.EnableWindow(FALSE);
}

void BluetoolsController::OnServiceDiscovered(xRemoteDevice* source, xRemoteService* discoveredService)
{
	LogEnterFunction("BTC:OnServiceDiscovered");
	/*xString* strName = NULL;
	CString serviceName;
	
	discoveredService->GetName(&strName);
	serviceName = strName->GetCharString();
	Log("BTC: Adding service: %s to serviceList", serviceName);

	
	// Keep a list of all discovered devices
	
	m_listServices->Add(discoveredService);

	X_RELEASE(strName);
	*/
}

void BluetoolsController::OnServiceDiscoveryCompleted(xRemoteDevice* source, xServiceList* discoveredServices)
{
	LogEnterFunction("BTC:OnServiceDiscoveryCompleted");
	Log("BTC: No of services found is %d",discoveredServices->GetServiceCount());
}

xRemoteDevice* BluetoolsController::GetRemoteDevice()
{
	return m_device;
}

/*xList* BluetoolsController::GetServiceListOfDiscoveredDevice()
{
	return m_listServices;
}*/

void BluetoolsController::OnError(xRemoteDevice* source, int errorCode, xString* errorText)
{
	MessageBox(NULL,errorText->GetTString(),"",MB_OK);
}

// BlueTools: xIStreamEvents
void BluetoolsController::OnReadCompleted(xStream* source, xStreamResult* results)
{
	LogEnterFunction("BTC:OnReadCompleted");

	
	xObject* objDummy = NULL;
	results->GetState(&objDummy);
	xStream* stream = (xStream*) objDummy;
	int iLength = -1;
	// Always call EndRead() after BeginRead()
	try
	{
		 iLength = stream->EndRead(results);
	}
	catch (xException* ex)
	{
		Log("Exception in onReadComplete, code=%d",ex->GetErrorCode());
		//MFC not thread safe, might not be possible to call ConnectionLost since it updates controls
		SetState(STATE_NOT_CONNECTED);
		if(isDisconnecting)
		{
			SetEvent(h_WaitForDisconnect);	
		}
		else
		{
			SuperConnectionLost();
			isDisconnecting = false;
			/*xString* str = NULL;
			ex->GetErrorMessage(&str);
			Log("Exception msg %s",str->GetCharString());*/
			//myHeadsetPresenterCallback->ConnectionLost();
		}
	}

	// NOTE! MFC is not thread safe and we are called in a new thread.
	// Updating controls can be unsafe
	if(iLength != -1) //If the exception occur we do not try to read data again, perhaps this is wrong?
	{
		xString* strData = new xString((char*) m_buffer, iLength);
		//m_textDataRead.SetWindowText(strData->GetTString());
		X_RELEASE(strData);

		stream->BeginRead(m_buffer, 0, 256, this, stream, NULL);
		if(stream->IsError())
		{
			m_show_error(stream);
		}

		unsigned char* msg;
		CString mb = CString(m_buffer);
		Log("BTC: Read: %s",mb);
		
		
		if(mb.Find("+VGS") != -1)
		{
			if(forceVGS)
			{
				Log("BTC:VGS received and forceVGS=true, sending 7");
				msg = (unsigned char*)"\r\n+VGS=7\r\n";
				stream->Write(msg,0,strlen((const char*)msg));  
			}
		}
		else if(mb.Find("AT+CIND=?") != -1)
		{
			TRACE("AT+CIND received");
			msg = (unsigned char*)"\r\n+CIND:(\"service\",(0-1)),(\"call\",(0-1)),(\"callsetup\",(0-3)),(\"callheld\",(0-2)),(\"signal\",(0-5)),(\"roam\",(0-5)),(\"battchg\",(0-5))\r\n";

			//MessageBox((LPCTSTR)msg);
			stream->Write(msg,0,strlen((const char*)msg));
			Log("BTC:Write %s",msg);

		}
		else if(mb.Find("AT+CIND?") != -1)
		{
			TRACE("AT+CIND? received");
			msg = (unsigned char*)"\r\n+CIND: 1,0,0,0,3,0,3\r\n";

			//MessageBox((LPCTSTR)msg);
			stream->Write(msg,0,strlen((const char*)msg));
			Log("BTC:Write %s",msg);
		}
		else if(mb.Find("AT+CMER") != -1)
		{
			TRACE("AT+CMER received");
			msg = (unsigned char*)"\r\nOK\r\n";

			//MessageBox((LPCTSTR)msg);
			stream->Write(msg,0,strlen((const char*)msg));
			Log("BTC:Write %s",msg);
		}
		else if(mb.Find("AT+CHLD") != -1)  //VGS does not work if we respond to CHLD
		{
			msg = (unsigned char*)"\r\n+CHLD: 1,2,2x\r\n";  //	+CHLD: (0,1,1x,2,2x,3)

			//MessageBox((LPCTSTR)msg);
			stream->Write(msg,0,strlen((const char*)msg));
			Log("BTC:Write %s",msg);
		}
		else if(mb.Find("AT+CCWA") != -1)  //VGS does not work if we respond to CHLD
		{
			TRACE("AT+CCWA received");
			msg = (unsigned char*)"\r\nOK\r\n"; 

			//MessageBox((LPCTSTR)msg);
			stream->Write(msg,0,strlen((const char*)msg));
			Log("BTC:Write %s",msg);
		}
		
		//This might be the problem with Nokia and Handsfree, Nokia may only support 0.96 therefor no BRSF is sent. Instead service discovery is used and the AG proably supports 3 way calling etc, perhaps something more is needed
		else if(mb.Find("AT+BRSF") != -1)
		{
			TRACE("BRSF received, will try to write only zeros, otherwise return error");
			msg = (unsigned char*)"\r\nERROR\r\n";  //Ver 0.96 of HFP 
			//msg = (unsigned char*)"\r\n+BRSF:55\r\n";  //BT135 halts when sending this. This setting activates Voice Reco so we could get AT+BVRA
			//MessageBox((LPCTSTR)msg);
			stream->Write(msg,0,strlen((const char*)msg));
			Log("BTC:Write %s",msg);
		}
		else if(mb.Find("AT+CKPD") != -1)
		{
			TRACE("CKPD received");
			msg = (unsigned char*)"\r\nOK\r\n";

			//MessageBox((LPCTSTR)msg);
			stream->Write(msg,0,strlen((const char*)msg));
			TRACE("Write %s",msg);
			static bool even = true;
			if(even)
			{
				//m_textDataRead.SetWindowText("even");
			}
			else
			{
				//m_textDataRead.SetWindowText("odd");
			}
			even = !even;

		}
		else  if(mb.Find("OK") != -1)
		{
			Log("BTC:OK received %s",mb);
		}
		else 
		{
			TRACE("unknown received %s",mb);
			msg = (unsigned char*)"\r\nOK\r\n";

			//MessageBox((LPCTSTR)msg);
			stream->Write(msg,0,strlen((const char*)msg));
			Log("BTC:\nWrite %s",msg);
		}

		HandleStatesAndForwardCommand(mb, AT_COMMAND, TRUE);
		/*
		//Currently ony AT commands are supported, always returning true
		if(wizardDialog->GetWizardState() == STATE_WIZARD_TRY_HFP || wizardDialog->GetWizardState() == STATE_WIZARD_TRY_HSP || wizardDialog->GetWizardState() == STATE_WIZARD_TRY_UNKNOWN)
		{
			wizardDialog->CommandReceived(mb, AT_COMMAND);
		}
		else if(state = STATE_CONNECTED)
		{
			myHeadsetPresenterCallback->CommandCallback(mb, AT_COMMAND);
		}*/
	}
	X_RELEASE(stream);  //moved from inner bracket ?
}


// Helpers
//

void BluetoolsController::m_cleanup()
{
	LogEnterFunction("BTC:m_cleanup");
	X_RELEASE(m_network);
	X_RELEASE(m_device);
	X_RELEASE(m_listDevices);
	X_RELEASE(m_listServices);

	if(m_manager)
	{
		m_manager->Dispose();  //if a pairing fails, this call hangs
		X_RELEASE(m_manager);
		
	}

	if(xObject::GetInstanceCount() != 0)
	{
		int q = xObject::GetInstanceCount();
   		//MessageBox(NULL,_T("Memory leak in xObjects! (Not all xObjects deleted)."),"",MB_OK);
	}
}

void BluetoolsController::m_show_error(xObject* obj)
{
	LogEnterFunction("BTC:m_show_error");
	if(obj && obj->IsError())
	{
		xException* ex = obj->GetException();
		xString* str = NULL;
		ex->GetErrorMessage(&str);

		//m_labelStatus.SetWindowText(str->GetTString());

		X_RELEASE(ex);
		X_RELEASE(str);
	}
}


/*
This function returns the SCN of a service named atleast part of serviceNameToFind
*/
int BluetoolsController::GetSCNOfService(CString serviceNameToFind)
{
	LogEnterFunction("BTC:GetSCNOfService");
	int noOfServices;
	int SCNOfWantedService = SCN_NOT_FOUND;
	CString serviceNameToFindLowerCase = serviceNameToFind;
	serviceNameToFindLowerCase.MakeLower();
	xServiceList* services = NULL;
	
	m_device->GetServices(&services);
	noOfServices = services->GetServiceCount();
	Log("No of services found is %d",noOfServices);
	Log("BTC: Will try to match with: %s or %s", serviceNameToFind,serviceNameToFindLowerCase);
	if(noOfServices > 0)
	{	
		for(int i=0;i<noOfServices;i++)
		{
			xAddress* remoteAddress = NULL;
			int remoteSCN = SCN_NOT_FOUND;
			xObject* objDummy = NULL;
			xRemoteService* service = NULL;
			m_listServices->Get(i, &objDummy); 
			service = (xRemoteService*) objDummy;
			xString* strName = NULL;
			CString serviceName;
			service->GetName(&strName);
			serviceName = strName->GetCharString();
			service->GetAddress(&remoteAddress);
			remoteSCN = remoteAddress->GetServiceChannelNumber();

			Log("BTC:Service name is:%s",serviceName);
			if(serviceName.Find(serviceNameToFind) != -1 || serviceName.Find(serviceNameToFindLowerCase) != -1)
			{
				SCNOfWantedService = remoteSCN;				
				//indexOfWantedService = i+1;  //servicesList is 0 based array but SCN numbers start on 1, therefor we must add 1
				Log("BTC: Match on SCN %d, listIndex: %d", SCNOfWantedService, i);
			}
			else
			{	
				Log("BTC: No match on SCN %d, listIndex: %d", remoteSCN, i);
			}

		}
	}
	else
	{
		MessageBox(NULL,"No Services found, contact support","",MB_OK);
		
	}
	X_RELEASE(services);
	return SCNOfWantedService;
}

bool BluetoolsController::ConnectToLastUsedDevice()
{
	ResetEvent(h_WaitForDisconnect);  //incase it is a reconnect retry
	bool connectionToLastDeviceOK = false;
	if(mySettingsControl->GetLastDeviceAddress() != "no address")
	{
		xAddress* remoteAddress = NULL;
		xString* remoteDeviceAddressAsString = NULL;
		xString* remoteDeviceName = NULL;
		xString* remoteDeviceServiceName = NULL;

		isReconnecting = true;
		remoteDeviceAddressAsString = new xString(mySettingsControl->GetLastDeviceAddress().GetBuffer());
		Log("Trying to reconnect to: %s on SCN: %d", remoteDeviceAddressAsString->GetCharString(), mySettingsControl->GetLastDeviceSCN());
	
		//Name is not stored in SettingsControl at this time
		remoteDeviceName = new xString(mySettingsControl->GetLastDeviceName().GetBuffer());
		remoteDeviceServiceName = new xString(mySettingsControl->GetLastDeviceName().GetBuffer());
		
		//ConnectToRemoteDevice do return a value but that only captures 2/3 exceptions
		//The one in OnReadComplete would not be handled, therefore we set state to Connected and 
		//ignore the return value when used in this spot, perhaps the function could be void
		//not sure how this would affect other calls, TBI
		
		SetState(STATE_CONNECTED);
		if(ConnectToRemoteDevice(remoteDeviceAddressAsString,mySettingsControl->GetLastDeviceSCN(), remoteDeviceServiceName, remoteDeviceName))
		{
			Log("BTC:Start RECONNECT_TIMER");
			Sleep(RECONNECT_TIMER);
			Log("BTC:Stop RECONNECT_TIMER");
			if(state == STATE_CONNECTED)
			{
				connectionToLastDeviceOK = true;
				
			}
			else 
			{
				connectionToLastDeviceOK = false;  //Reconnect failed, state was changed by exception in OnReadComplete
			}
		}
		else
		{
			connectionToLastDeviceOK = false;
		}
		
		isReconnecting = false;
		X_RELEASE(remoteAddress);
		X_RELEASE(remoteDeviceAddressAsString);
		X_RELEASE(remoteDeviceServiceName);
		X_RELEASE(remoteDeviceName);
	}
	return connectionToLastDeviceOK;
}

/*Called by Wizard when we have identified och manually selected a service
 ServiceName is never really used, handeling it because it is there.*/
bool BluetoolsController::ConnectToSelectedDevice(int SCN, CString serviceName)
{
	LogEnterFunction("BTC: ConnectToSelectedDevice");
	xAddress* remoteAddress = NULL;
	xString* remoteDeviceAddressAsString = NULL;
	xString* remoteDeviceName = NULL;
	xString* remoteDeviceServiceName = NULL;

	m_device->GetAddress(&remoteAddress);
	remoteAddress->ToString(&remoteDeviceAddressAsString);
	Log("BTC: Remote address is: %s",remoteDeviceAddressAsString->GetCharString());

	m_device->GetName(&remoteDeviceName);
	Log("BTC: Remote device name is : %s", remoteDeviceName->GetCharString());
	
	//Here serviceName should be stored
	remoteDeviceServiceName = new xString(serviceName.GetBuffer());
	bool connectionOK = ConnectToRemoteDevice(remoteDeviceAddressAsString,SCN, remoteDeviceServiceName, remoteDeviceName);
	if(!connectionOK)
	{
		MessageBox(NULL,"The pairing with your headset failed.\n\nMake sure that the headset is:\n1.In pairable mode\n2.That the correct PIN code is entered","Pairing failed",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
	}

	X_RELEASE(remoteAddress);
	X_RELEASE(remoteDeviceAddressAsString);
	X_RELEASE(remoteDeviceServiceName);
	X_RELEASE(remoteDeviceName);
	return connectionOK;
}

/*This function connects to a given address and Service Channel Number
DeviceName and ServiceName is probably not mandatory for the connection.
Calling function must start pairing dialog if connect fails, depending on state when calling ofcourse
*/
bool BluetoolsController::ConnectToRemoteDevice(xString* deviceAddress,int SCN, xString* serviceName, xString* deviceName)
{
	LogEnterFunction("BTC:ConnectToRemoteDevice");
	bool connectedOK = false;
	xRemoteService* service = NULL;
	xAddress* devAddr = new xAddress(deviceAddress);
	m_network->ConnectDevice(devAddr,deviceName,&m_device);  //Will always work even if device is not available
	
	m_device->ConnectService(SCN, serviceName, &service);  //1=headset 2=hands-free name is only for local service list
	
	if(service)
	{
		m_listServices->Add(service);
		xStream* stream = NULL;

		try
		{
			service->GetStream(&stream);
			Log("BTC: Got stream from remoteDevice");
		}
		catch (xException* ex)
		{
			MessageBox(NULL,"BTC: Exception in ConnectToRemoteDevice, contact support","",MB_OK);
			Log("Exception on GetStream");
			SetState(STATE_NOT_CONNECTED);
			//Connection could fail here
			return false;
		}
	
		if(service->IsError())
		{
			// Error opening stream
			m_show_error(service);
		}
		else
		{
			try
			{
				stream->BeginRead(m_buffer, 0, 256, this, stream, NULL);
				Log("BTC: Begin Read started ok");
			}
			catch (xException* ex)
			{
				//MessageBox(NULL,"BTC: Ex2","",MB_OK);
				Log("BTC:Exception on BeginRead");
				SetState(STATE_NOT_CONNECTED);
				if(!isReconnecting) //we are in pairing wizard
				{
					AlreadyConnectedToAGDialog myAGConnectedDialog;
					myAGConnectedDialog.DoModal();
				}
				//Connection could fail here
				return false;
			}
			if(stream->IsError())
			{
				m_show_error(stream);
			}
			else
			{
				connectedOK = true;
				unsigned char* msg;
				msg = (unsigned char*)"\r\nRING\r\n";
				try
				{
					Log("BTC:Write %s",msg);
					stream->Write(msg,0,strlen((const char*)msg));
				}
				catch(xException* ex)
				{
					Log("BTC:Exception on write in ConnectToRemoteDevice");
					if(!isReconnecting) //we are in pairing wizard
					{
						AlreadyConnectedToAGDialog myAGConnectedDialog;
						myAGConnectedDialog.DoModal();
					}
					SetState(STATE_NOT_CONNECTED);
					return false;
				}
				if(connectedOK)
				{
					mySettingsControl->SetLastDeviceName(deviceName->GetCharString());
					mySettingsControl->SetLastDeviceAddress(deviceAddress->GetCharString());
					mySettingsControl->SetLastDeviceServiceName(serviceName->GetCharString());
					mySettingsControl->SetLastDeviceSCN(SCN);
					myHeadsetPresenterCallback->StartTimer();
					Log("BTC:Connected to SCN:%d",SCN);
				}
			}
			X_RELEASE(stream);
		}		
		X_RELEASE(service);
	}
	return connectedOK;
}
void BluetoolsController::DisconnectRemoteDevice()
{
	LogEnterFunction("BTC:DisconnectRemoteDevice");
	xObject* objDummy = NULL;
	xRemoteService* service = NULL;
	
	if(state == STATE_CONNECTED) 
	{
		isDisconnecting = true;
		m_listServices->Get(GetIndexOfConnectedService(), &objDummy);  	
		service = (xRemoteService*) objDummy;
		if(service)
		{
			//xString* strName = NULL;  //Relase this one ?
			//service->GetName(&strName);
			Log("BTC:Disconnecting SCN %d at serviceIndex:%d",GetIndexOfConnectedService()+1,GetIndexOfConnectedService());
			
			xStream* stream = NULL;

			service->GetStream(&stream);
			if(stream)
			{
				Log("BTC:Got stream, closing");
				try
				{
					stream->Close();
				}
				catch(xException* ex)
				{
					Log("BTC:stream->Close threw an exception with id %d", ex->GetErrorCode());
				}
				
			}
			//X_RELEASE(stream);  //causes crash ????? TBI
		}
		X_RELEASE(service);
	}
}

int BluetoolsController::GetIndexOfConnectedService()
{
	int serviceIndex;
	int SCN;
	if(m_listServices->GetLength() == 1)  //either only one service or a reconnect
	{
		serviceIndex = 0;
	}
	else
	{
		SCN = mySettingsControl->GetLastDeviceSCN();
		serviceIndex = SCN - 1; //List is 0 based, services starts on 1, Major issue if SCN does not start with 1 on all devices
	}
	//Log("BTC:GetIndexOfConnectedService returns: %d",serviceIndex);
	return serviceIndex;
}

bool BluetoolsController::BondDevice()
{
	LogEnterFunction("BTC:BondDevice");
	bool bondOK = false;
	xString* pin = NULL;
	CString manualPin = GetPin();
	pin = new xString(manualPin.GetBuffer());
	try
	{
		Log("BTC:Trying to bond with pincode %s",manualPin);
		m_device->Bond(pin);
		bondOK = true;
	}
	catch (xException* ex)  //This ex does not contain an error message no use trying to extract it
	{
		Log("BTC:Bond failed");
		int res;
		res = MessageBox(NULL,"Pairing with the headset failed for reasons not known to HeadsetPresenter.\nIt is recommended that the headset is restarted and the wizard rerun.\nIf the problem persists please read chapter 5.4 in HeadsetPresenter for Beginners available in the HeadsetPresenter folder on the start menu.","Pairing failed",MB_OK|MB_ICONINFORMATION|MB_TOPMOST);  //here we need to show pin dialog
	}
	return bondOK;
}


