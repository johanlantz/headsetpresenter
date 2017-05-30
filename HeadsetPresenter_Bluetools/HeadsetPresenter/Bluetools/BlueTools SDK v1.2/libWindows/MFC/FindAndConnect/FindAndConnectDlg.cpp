// FindAndConnectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FindAndConnect.h"
#include "FindAndConnectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindAndConnectDlg dialog

CFindAndConnectDlg::CFindAndConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindAndConnectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindAndConnectDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_network = NULL;
	m_manager = NULL;
	m_device = NULL;

	m_listDevices = new xList();
	m_listServices = new xList();
}

void CFindAndConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindAndConnectDlg)
	DDX_Control(pDX, IDC_LABEL_STATUS, m_labelStatus);
	DDX_Control(pDX, IDC_DATA_WRITE, m_textDataWrite);
	DDX_Control(pDX, IDC_DATA_READ, m_textDataRead);
	DDX_Control(pDX, IDC_SERVICES, m_boxServices);
	DDX_Control(pDX, IDC_DEVICES, m_boxDevices);
	DDX_Control(pDX, IDC_WRITE, m_buttonWrite);
	DDX_Control(pDX, IDC_OPEN, m_buttonOpen);
	DDX_Control(pDX, IDC_CLOSE, m_buttonClose);
	DDX_Control(pDX, IDC_DISCOVER, m_buttonDiscover);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFindAndConnectDlg, CDialog)
	//{{AFX_MSG_MAP(CFindAndConnectDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_DISCOVER, OnDiscover)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	ON_BN_CLICKED(IDC_OPEN, OnOpen)
	ON_BN_CLICKED(IDC_WRITE, OnWrite)
	ON_LBN_SELCHANGE(IDC_DEVICES, OnSelchangeDevices)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindAndConnectDlg message handlers

BOOL CFindAndConnectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Do not use exceptions
	xObject::SetUseExceptions(false);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_manager = NULL;
	xManager::GetManager(&m_manager);

	if(m_manager == NULL)
	{
		MessageBox(_T("Bluetooth not found!"), NULL, NULL);
	}
	else
	{
		xString* strKey = new xString("WoK6IL564A8COORQTXaYUjwRIQluJXKYWsLA");
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
				}
			}

			X_RELEASE(networkList);
		}

		X_RELEASE(strKey);
		X_RELEASE(license);
	}
	m_buttonClose.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFindAndConnectDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFindAndConnectDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// Form event handlers
//

void CFindAndConnectDlg::OnDiscover() 
{
	if(m_network)
	{
		if(m_network->GetDiscoveryPending())
		{
			// Cancel pending discovery
			m_network->CancelDeviceDiscovery();

			m_labelStatus.SetWindowText(_T("Discovery canceled!"));
		}
		else
		{
			// Start new discovery
			m_network->DiscoverDevicesAsync();

			m_labelStatus.SetWindowText(_T("Discovers devices!"));
		}
	}
	else
	{
		// Network has not been found
	}
}

void CFindAndConnectDlg::OnSelchangeDevices() 
{
	// Cancel any pending device discovery
	m_network->CancelDeviceDiscovery();

	// Do not receive events from last device
	if(m_device)
	{
		m_device->SetEvents(NULL);
		X_RELEASE(m_device);
	}

	xObject* objDummy = NULL;

	// Index of selected device in list box
	int iDeviceIndex = m_boxDevices.GetCurSel();

	// Get that device from our list of devices
	m_listDevices->Get(iDeviceIndex, &objDummy);
	m_device = (xRemoteDevice*) objDummy;

	if(m_device == NULL)
	{
		// UNknown error
	}
	else if(m_device->GetDiscoveryPending() == false)
	{
		// Receive events from this device
		m_device->SetEvents(this);

		// Clear contents of service list box, to display services of selected devices
		m_boxServices.ResetContent();

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
		m_labelStatus.SetWindowText(_T("Service discovery pending"));
	}
}

void CFindAndConnectDlg::OnOK() 
{
	m_cleanup();
	
	CDialog::OnOK();
}

void CFindAndConnectDlg::OnCancel() 
{
	m_cleanup();
	
	CDialog::OnCancel();
}

void CFindAndConnectDlg::OnClose() 
{
	int iServiceIndex = m_boxServices.GetCurSel();
	
	if(iServiceIndex == -1)
	{
		// No service selected
	}
	else
	{
		xObject* objDummy = NULL;
		xRemoteService* service = NULL;

		m_listServices->Get(iServiceIndex, &objDummy);
		service = (xRemoteService*) objDummy;

		if(service)
		{
			xStream* stream = NULL;

			service->GetStream(&stream);
			if(stream)
			{
				stream->Close();
			}

			X_RELEASE(stream);
		}
		X_RELEASE(service);
	}

	m_boxDevices.EnableWindow(TRUE);
	m_boxServices.EnableWindow(TRUE);
	m_buttonClose.EnableWindow(FALSE);
	m_buttonOpen.EnableWindow(TRUE);
}

void CFindAndConnectDlg::OnOpen() 
{
	int iServiceIndex = m_boxServices.GetCurSel();
	
	if(iServiceIndex == -1)
	{
		MessageBox(_T("No service selected!"));
	}
	else
	{
		xObject* objDummy = NULL;
		xRemoteService* service = NULL;

		m_listServices->Get(iServiceIndex, &objDummy);
		service = (xRemoteService*) objDummy;

		if(service)
		{
			xStream* stream = NULL;

			service->GetStream(&stream);
			if(service->IsError())
			{
				// Error opening stream
				m_show_error(service);
			}
			else
			{
				stream->BeginRead(m_buffer, 0, 256, this, stream, NULL);
				if(stream->IsError())
				{
					m_show_error(stream);
				}
				else
				{
					m_boxDevices.EnableWindow(FALSE);
					m_boxServices.EnableWindow(FALSE);
					m_buttonClose.EnableWindow(TRUE);
					m_buttonOpen.EnableWindow(FALSE);

					m_labelStatus.SetWindowText(_T("Stream opened!"));
				}
			}

			X_RELEASE(stream);
		}
		X_RELEASE(service);
	}
}

void CFindAndConnectDlg::OnWrite() 
{
	// TODO: Add your control notification handler code here
	
}


// BlueTools: Network event interface

// NOTE!
// All callbacks will be called in a new thread.
// MFC is not thread safe, so take care! You might want to synchronize with the GUI thread
// before updating any controls.

void CFindAndConnectDlg::OnDeviceDiscoveryStarted(xNetwork* source)
{
	m_buttonDiscover.SetWindowText(_T("Stop"));
}

void CFindAndConnectDlg::OnDeviceDiscovered(xNetwork* source, xRemoteDevice* discoveredDevice)
{
	xString* strName = NULL;

	discoveredDevice->GetName(&strName);
	m_boxDevices.AddString(strName->GetTString());

	// Keep a list of all discovered devices
	m_listDevices->Add(discoveredDevice);

	X_RELEASE(strName);
}

void CFindAndConnectDlg::OnDeviceLost(xNetwork* source, xRemoteDevice* lostDevice)
{
	int inx = m_listDevices->Remove(lostDevice);
	m_boxDevices.DeleteString(inx);
}

void CFindAndConnectDlg::OnDeviceDiscoveryCompleted(xNetwork* source, xDeviceList* discoveredDevices)
{
	m_buttonDiscover.SetWindowText(_T("Discover"));
}

void CFindAndConnectDlg::OnError(xNetwork* source, int errorCode, xString* errorText)
{
	MessageBox(errorText->GetTString());
}

// BlueTools: xIRemoteDeviceEvents
void CFindAndConnectDlg::OnServiceDiscoveryStarted(xRemoteDevice* source)
{
	m_labelStatus.SetWindowText(_T("Discovering services!"));

	m_boxDevices.EnableWindow(FALSE);
	m_buttonDiscover.EnableWindow(FALSE);
}

void CFindAndConnectDlg::OnServiceDiscovered(xRemoteDevice* source, xRemoteService* discoveredService)
{
	xString* strName = NULL;

	discoveredService->GetName(&strName);
	m_boxServices.AddString(strName->GetTString());

	// Keep a list of all discovered devices
	m_listServices->Add(discoveredService);

	X_RELEASE(strName);
}

void CFindAndConnectDlg::OnServiceDiscoveryCompleted(xRemoteDevice* source, xServiceList* discoveredServices)
{
	m_boxDevices.EnableWindow(TRUE);
	m_buttonDiscover.EnableWindow(TRUE);

	m_labelStatus.SetWindowText(_T("Click Open to read data!"));
}

void CFindAndConnectDlg::OnError(xRemoteDevice* source, int errorCode, xString* errorText)
{
	MessageBox(errorText->GetTString());
}

// BlueTools: xIStreamEvents
void CFindAndConnectDlg::OnReadCompleted(xStream* source, xStreamResult* results)
{
	xObject* objDummy = NULL;
	results->GetState(&objDummy);
	xStream* stream = (xStream*) objDummy;

	// Always call EndRead() after BeginRead()
	int iLength = stream->EndRead(results);

	// NOTE! MFC is not thread safe and we are called in a new thread.
	// Updating controls can be unsafe
	xString* strData = new xString((char*) m_buffer, iLength);
	m_textDataRead.SetWindowText(strData->GetTString());
	X_RELEASE(strData);

	stream->BeginRead(m_buffer, 0, 256, this, stream, NULL);
	if(stream->IsError())
	{
		m_boxDevices.EnableWindow(TRUE);
		m_boxServices.EnableWindow(TRUE);
		m_buttonClose.EnableWindow(FALSE);
		m_buttonOpen.EnableWindow(TRUE); 

		m_show_error(stream);
	}

	X_RELEASE(stream);
}


// Helpers
//

void CFindAndConnectDlg::m_cleanup()
{
	X_RELEASE(m_network);
	X_RELEASE(m_device);
	X_RELEASE(m_listDevices);
	X_RELEASE(m_listServices);

	if(m_manager)
	{
		m_manager->Dispose();
		X_RELEASE(m_manager);
	}

	if(xObject::GetInstanceCount() != 0)
	{
		int q = xObject::GetInstanceCount();
   		MessageBox(_T("Memory leak in xObjects! (Not all xObjects deleted)."));
	}
}

void CFindAndConnectDlg::m_show_error(xObject* obj)
{
	if(obj && obj->IsError())
	{
		xException* ex = obj->GetException();
		xString* str = NULL;
		ex->GetErrorMessage(&str);

		m_labelStatus.SetWindowText(str->GetTString());

		X_RELEASE(ex);
		X_RELEASE(str);
	}
}

