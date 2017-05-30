using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.IO;
using Franson.Protocols.AT;
using Franson.Protocols.SMS;
using Franson.BlueTools.SDP;
using Franson.BlueTools;
using System.Globalization;



namespace SMSManagerCF
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class MainForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.TabPage Device;
		private System.Windows.Forms.TabPage View;
		private System.Windows.Forms.TabPage Send;
		private System.Windows.Forms.ListBox deviceList;

		private SMSTransporter m_SMSTrans;
		private Manager	m_manager;		
		private Network	m_network;		
		private RemoteService m_serviceCurrent;	
		private Stream m_streamCurrent;
		private Queue m_deviceQueue;
		private Stack m_sSerialPorts;
		private	ArrayList m_Devices;
		private int m_intReadCounter;
		private int m_intMessageCount;
		private int m_intMessagePos;
		private System.Windows.Forms.Label lbStatus;
		private System.Windows.Forms.Button btnRead;
		private System.Windows.Forms.Button btnDelete;
		private System.Windows.Forms.Label lbViewStatus;
		private System.Windows.Forms.Button btnDiscover;
		private ArrayList m_listStoreSMS;
		private System.Windows.Forms.ListBox listSMS;
		private System.Windows.Forms.Label lbMessage;
		private System.Windows.Forms.Label lbPhone;
		private System.Windows.Forms.Button btnSend;
		private System.Windows.Forms.Label lbSendStatus;
		private System.Windows.Forms.TextBox tbxNumber;
		private System.Windows.Forms.TextBox tbxMessage;
		private System.Windows.Forms.MainMenu mainMenu1;
		private System.Windows.Forms.MenuItem menuItem1;
		private Queue m_indexQueue;

		public MainForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			// Instantiate the device queue and the serial port arraylist. 
			m_deviceQueue = new Queue();
			m_sSerialPorts = new Stack();
			m_Devices = new ArrayList();
			m_listStoreSMS = new ArrayList();
			m_indexQueue = new Queue();

			// Innitialize network
			InitConnection();

			// Disable all buttons. 
			DisableAllBtns();
		}
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			base.Dispose( disposing );
		}
		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this.Device = new System.Windows.Forms.TabPage();
			this.lbStatus = new System.Windows.Forms.Label();
			this.btnDiscover = new System.Windows.Forms.Button();
			this.deviceList = new System.Windows.Forms.ListBox();
			this.View = new System.Windows.Forms.TabPage();
			this.listSMS = new System.Windows.Forms.ListBox();
			this.lbViewStatus = new System.Windows.Forms.Label();
			this.btnDelete = new System.Windows.Forms.Button();
			this.btnRead = new System.Windows.Forms.Button();
			this.Send = new System.Windows.Forms.TabPage();
			this.lbSendStatus = new System.Windows.Forms.Label();
			this.btnSend = new System.Windows.Forms.Button();
			this.lbPhone = new System.Windows.Forms.Label();
			this.lbMessage = new System.Windows.Forms.Label();
			this.tbxNumber = new System.Windows.Forms.TextBox();
			this.tbxMessage = new System.Windows.Forms.TextBox();
			this.mainMenu1 = new System.Windows.Forms.MainMenu();
			this.menuItem1 = new System.Windows.Forms.MenuItem();
			// 
			// tabControl1
			// 
			this.tabControl1.Controls.Add(this.Device);
			this.tabControl1.Controls.Add(this.View);
			this.tabControl1.Controls.Add(this.Send);
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(240, 256);
			this.tabControl1.SelectedIndexChanged += new System.EventHandler(this.tabControl1_SelectedIndexChanged);
			// 
			// Device
			// 
			this.Device.Controls.Add(this.lbStatus);
			this.Device.Controls.Add(this.btnDiscover);
			this.Device.Controls.Add(this.deviceList);
			this.Device.Location = new System.Drawing.Point(4, 4);
			this.Device.Size = new System.Drawing.Size(232, 230);
			this.Device.Text = "Device";
			// 
			// lbStatus
			// 
			this.lbStatus.Location = new System.Drawing.Point(11, 206);
			this.lbStatus.Size = new System.Drawing.Size(211, 20);
			this.lbStatus.TextAlign = System.Drawing.ContentAlignment.TopCenter;
			// 
			// btnDiscover
			// 
			this.btnDiscover.Location = new System.Drawing.Point(80, 176);
			this.btnDiscover.Size = new System.Drawing.Size(64, 20);
			this.btnDiscover.Text = "Discover";
			this.btnDiscover.Click += new System.EventHandler(this.discoverBtn_Click);
			// 
			// deviceList
			// 
			this.deviceList.Location = new System.Drawing.Point(8, 8);
			this.deviceList.Size = new System.Drawing.Size(216, 145);
			this.deviceList.SelectedIndexChanged += new System.EventHandler(this.deviceList_SelectedIndexChanged);
			// 
			// View
			// 
			this.View.Controls.Add(this.listSMS);
			this.View.Controls.Add(this.lbViewStatus);
			this.View.Controls.Add(this.btnDelete);
			this.View.Controls.Add(this.btnRead);
			this.View.Location = new System.Drawing.Point(4, 4);
			this.View.Size = new System.Drawing.Size(232, 230);
			this.View.Text = "View";
			// 
			// listSMS
			// 
			this.listSMS.Location = new System.Drawing.Point(12, 9);
			this.listSMS.Size = new System.Drawing.Size(207, 158);
			this.listSMS.SelectedIndexChanged += new System.EventHandler(this.listSMS_SelectedIndexChanged);
			// 
			// lbViewStatus
			// 
			this.lbViewStatus.Location = new System.Drawing.Point(10, 210);
			this.lbViewStatus.Size = new System.Drawing.Size(216, 20);
			this.lbViewStatus.TextAlign = System.Drawing.ContentAlignment.TopCenter;
			// 
			// btnDelete
			// 
			this.btnDelete.Location = new System.Drawing.Point(120, 184);
			this.btnDelete.Text = "Delete SMS";
			this.btnDelete.Click += new System.EventHandler(this.btnDelete_Click);
			// 
			// btnRead
			// 
			this.btnRead.Location = new System.Drawing.Point(40, 184);
			this.btnRead.Text = "Read SMS";
			this.btnRead.Click += new System.EventHandler(this.btnRead_Click);
			// 
			// Send
			// 
			this.Send.Controls.Add(this.lbSendStatus);
			this.Send.Controls.Add(this.btnSend);
			this.Send.Controls.Add(this.lbPhone);
			this.Send.Controls.Add(this.lbMessage);
			this.Send.Controls.Add(this.tbxNumber);
			this.Send.Controls.Add(this.tbxMessage);
			this.Send.Location = new System.Drawing.Point(4, 4);
			this.Send.Size = new System.Drawing.Size(232, 230);
			this.Send.Text = "Send";
			// 
			// lbSendStatus
			// 
			this.lbSendStatus.Location = new System.Drawing.Point(15, 189);
			this.lbSendStatus.Size = new System.Drawing.Size(203, 20);
			this.lbSendStatus.TextAlign = System.Drawing.ContentAlignment.TopCenter;
			// 
			// btnSend
			// 
			this.btnSend.Location = new System.Drawing.Point(64, 144);
			this.btnSend.Size = new System.Drawing.Size(94, 20);
			this.btnSend.Text = "Send Message";
			this.btnSend.Click += new System.EventHandler(this.btnSend_Click);
			// 
			// lbPhone
			// 
			this.lbPhone.Location = new System.Drawing.Point(32, 88);
			this.lbPhone.Text = "Phone Number:";
			// 
			// lbMessage
			// 
			this.lbMessage.Location = new System.Drawing.Point(32, 16);
			this.lbMessage.Text = "Message:";
			// 
			// tbxNumber
			// 
			this.tbxNumber.Location = new System.Drawing.Point(32, 112);
			this.tbxNumber.Size = new System.Drawing.Size(173, 20);
			this.tbxNumber.Text = "";
			// 
			// tbxMessage
			// 
			this.tbxMessage.Location = new System.Drawing.Point(32, 48);
			this.tbxMessage.Size = new System.Drawing.Size(173, 20);
			this.tbxMessage.Text = "";
			// 
			// mainMenu1
			// 
			this.mainMenu1.MenuItems.Add(this.menuItem1);
			// 
			// menuItem1
			// 
			this.menuItem1.Text = "";
			// 
			// MainForm
			// 
			this.ClientSize = new System.Drawing.Size(240, 235);
			this.Controls.Add(this.tabControl1);
			this.Menu = this.mainMenu1;
			this.MinimizeBox = false;
			this.Text = "MainForm";
			this.Closing += new System.ComponentModel.CancelEventHandler(this.MainForm_Closing);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>

		static void Main() 
		{
			Application.Run(new MainForm());
		}

		private void tabControl1_SelectedIndexChanged(object sender, System.EventArgs e)
		{
		
		}

		private void discoverBtn_Click(object sender, System.EventArgs e)
		{
			try
			{
				// Start looking for devices on the network
				// Use Network.DiscoverDevices() if you don't want to use events.
				if ( m_network != null ) 
				{
					this.lbStatus.Text = "Discovering devices..";
					this.btnDiscover.Enabled = false;
					this.Refresh();

					// Add events for device discovery
					m_network.DeviceDiscovered += new BlueToolsEventHandler(network_DeviceDiscovered);
					m_network.DeviceDiscoveryStarted += new BlueToolsEventHandler(network_DeviceDiscoveryStarted);
					m_network.DeviceDiscoveryCompleted += new BlueToolsEventHandler(network_DeviceDiscoveryCompleted);

					m_network.DiscoverDevicesAsync();
				}
				else
				{
					MessageBox.Show("Bluetooth network not available. Are you using an old license key?");
				}
			} 
			catch (BlueToolsException exc)
			{
				MessageBox.Show(exc.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button1);
			}
		}

		private void InitConnection()
		{

			try
			{
				m_manager	= Manager.GetManager();

				switch(Manager.StackID)
				{
					case StackID.STACK_MICROSOFT:
						break;
					case StackID.STACK_WIDCOMM:
						break;
				}

				// You can get a valid evaluation key for BlueTools at
				// http://franson.com/bluetools/
				// That key will be valid for 14 days. Just cut and paste that key into the statement below.
				// To get a key that do not expire you need to purchase a license
				Franson.BlueTools.License license = new Franson.BlueTools.License();
				license.LicenseKey = "HU5UZer022JOLX98oidhQkbRbvv0NRJbHXEc";
				
				// Call events in GUI thread
				m_manager.Parent = this;
				// Call events in new thread (multi-threading)
				// m_manager.Parent = null

				// Get first netowrk (BlueTools 1.0 only supports one network == one dongle)
				m_network = m_manager.Networks[0];

			} 
			catch (Exception exc)
			{
				MessageBox.Show(exc.Message);
			}
		}

		#region Discovery devices

		private void network_DeviceDiscoveryStarted(object sender, BlueToolsEventArgs eventArgs)
		{
			// Search for devices on the network started

			this.deviceList.Enabled = false;
			this.lbStatus.Text = "Discovering devices...";
		}

		private void network_DeviceDiscovered(object sender, BlueToolsEventArgs eventArgs)
		{
			// A new device was discovered!
			// Note that this event is only called for NEW devices on the network

			// Use Network.Devices for a complete list of discovered devices.

			RemoteDevice device = (RemoteDevice)((DiscoveryEventArgs)eventArgs).Discovery;
			// this.lbStatusText.Text = "Found new device: " + device.Name;

			// Add the device to the combobox. 
			this.deviceList.Items.Add(device);
			Refresh();

			m_deviceQueue.Enqueue(device);
		}

		private void network_DeviceDiscoveryCompleted(object sender, BlueToolsEventArgs eventArgs)
		{
			// Search for devices on the network ended.
			//

			// Remove the events for device discovery
			m_network.DeviceDiscovered -= new BlueToolsEventHandler(network_DeviceDiscovered);
			m_network.DeviceDiscoveryStarted -= new BlueToolsEventHandler(network_DeviceDiscoveryStarted);
			m_network.DeviceDiscoveryCompleted -= new BlueToolsEventHandler(network_DeviceDiscoveryCompleted);

			// Enable the device list
			this.deviceList.Enabled = true;
			m_network.CancelDeviceDiscovery();

			this.lbStatus.Text = "Select device.";
			this.btnDiscover.Enabled = true;
			this.Refresh();
		}


		private void device_ServiceDiscovered(object sender, BlueToolsEventArgs eventArgs)
		{
			
			try
			{

				// A new service found for device!
				// Note that this event is only called for NEW services found
				// Use Device.Services for a complete list of services for device

				// Get the service associated with the discovery event
				RemoteService service = (RemoteService)((DiscoveryEventArgs)eventArgs).Discovery;
				// Search for devices on the network ended.

				RemoteDevice device = (RemoteDevice)sender;
				
				// Add the serial port services to the list.  
				if ( (service.ServiceType.GUID == ServiceType.SerialPort.GUID) || ( service.ServiceType.GUID == ServiceType.DialupNetworking.GUID ) )
				{
					// Add the serial port service to the serial port list. 
					m_sSerialPorts.Push(service);

					// Show the service in the status field. 
				}
			}
			catch (Exception exc)
			{
				// Show an error message. 
				MessageBox.Show(exc.Message);
			}
		}

		private void device_ServiceDiscoveryCompleted(object sender, BlueToolsEventArgs eventArgs)
		{
			// All services for device discovered"

			// Get the remote device that raised the event
			RemoteDevice device = (RemoteDevice)sender;

			// Remove the event handlers.
			if ( device != null ) 
			{
				device.ServiceDiscovered -= new BlueToolsEventHandler(device_ServiceDiscovered);
				device.ServiceDiscoveryCompleted -= new BlueToolsEventHandler(device_ServiceDiscoveryCompleted);
			}
				
			// Disconnect any existing connection. 
				
			try 
			{
				if ( m_streamCurrent != null ) 
				{
					m_streamCurrent.Close();
				}
			}
			catch ( Exception )
			{
				// There might be an exception if the device that was connected has 
				// been turned off for example. We don't want to see any error messages
				// about a device that we are not using anymore. 
			}

			// Get the serial and/or dial up networking services for the device. 

			RemoteService rService = null;
			if(device != null)
			{	

				// Try to find to serial service first. 
				foreach ( RemoteService service in device.Services ) 
				{
					if ( service != null ) 
					{
						// Add the serial port services to the list.  
						if ( (service.ServiceType.GUID == ServiceType.SerialPort.GUID) )
						{
							// The nokia PC Suite service cannot be useed. 
							if ( service.Name != "Nokia PC Suite")
							{
								rService = service;
								break;
							}
						}
					}
				}

				// If no serial service is found, try to find a dial up network service. 
				// This works for example on Motoroal Rzr. 
				if ( rService == null ) 
				{
					foreach ( RemoteService service in device.Services ) 
					{
						if ( service != null ) 
						{
							// Add the dial up services to the list.  
							if ( service.ServiceType.GUID == ServiceType.DialupNetworking.GUID  )
							{
								rService = service;
								break;
							}
						}
					}
				}
			}	

			if ( rService == null ) 
			{
				MessageBox.Show("No serial or dial up services found on the device.");
				this.lbStatus.Text = "No serial or dial up services found on the device.";
				this.deviceList.Enabled = true;
				return;
			}
		
			this.lbStatus.Text = "Connecting..";
			this.Refresh();

			// Connect to the selected serial port. 
			ConnectToService(rService);

			// Connect to the serial port stream. 
			if( m_streamCurrent != null)
			{
				m_SMSTrans = new SMSTransporter();

				try 
				{
					m_SMSTrans.Connect(m_streamCurrent);

				}
				catch (Exception exc)
				{	
					MessageBox.Show(exc.Message);
					m_serviceCurrent = null;
					this.lbStatus.Text = "Failed to connect.";
				}

				this.lbStatus.Text = "Connected";

				// Enable all buttons
				EnableAllBtns();
			}
		}

		private void device_Error(object sender, BlueToolsEventArgs eventArgs)
		{
			Franson.BlueTools.ErrorEventArgs errorEventArgs = (Franson.BlueTools.ErrorEventArgs)eventArgs;
			RemoteDevice device = (RemoteDevice)sender;

			MessageBox.Show(errorEventArgs.ErrorCode + ": " + errorEventArgs.Message );
			
			// Empty the stack. 
			for ( int iCount = 0 ; iCount < m_sSerialPorts.Count; iCount++)
			{
				m_sSerialPorts.Pop();		
			}
			this.deviceList.Enabled = true;
			
		}

		#endregion

		private void ConnectToService(RemoteService service)
		{
			if(m_serviceCurrent != null)
			{
				if ( m_streamCurrent != null )
					m_streamCurrent.Close();

				m_serviceCurrent	= null;
				m_streamCurrent	= null;
			}
			
			// Attempt to connect to the service
			//

			try
			{
				m_serviceCurrent = service;
				
				// Connect to service by reading its Stream
				m_streamCurrent = m_serviceCurrent.Stream;
			} 
			catch (Exception exc)
			{
				MessageBox.Show(exc.Message);
				m_serviceCurrent = null;
				m_streamCurrent = null;
				this.lbStatus.Text = "Unable to connect";
				this.deviceList.Enabled = true;
			}
		 }

		private void deviceList_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			// Get the select device. 
			RemoteDevice device = (RemoteDevice) deviceList.SelectedItem;
			this.btnDiscover.Enabled = false;

			if(device != null)
			{	
				

				// Disable the device list so the user doesn't attempt to select a device while
				// the service lookup is pending
				deviceList.Enabled = false;

				// Add a DiscoveryListener so we get service discovery events
				device.ServiceDiscovered += new BlueToolsEventHandler(device_ServiceDiscovered);

				// Add a DiscoveryListener so we get service discovery completion events
				device.ServiceDiscoveryCompleted += new BlueToolsEventHandler(device_ServiceDiscoveryCompleted);

				// Add DiscoveryListener if there is an error. 
				device.Error += new BlueToolsEventHandler(device_Error);

				try
				{
					// Cancel any ongoing device discovery
					m_network.CancelDeviceDiscovery();

					// Discover all services offered by the device
					// Use e.g. ServiceType.SerialPort to list only serial port services
					// Use Device.DiscoverServices() if you don't want to use events.
					device.DiscoverServicesAsync(ServiceType.RFCOMM);

					this.lbStatus.Text = "Searching for serial services...";
					this.Refresh();
				} 
				catch (Exception exc)
				{
					this.lbStatus.Text = "Operation failed.";
					MessageBox.Show(exc.Message);
				}
			}
		}

		private void btnRead_Click(object sender, System.EventArgs e)
		{
			if ( m_SMSTrans == null ) 
			{
				MessageBox.Show("Error - Not connected to a device.");
				this.lbStatus.Text = "Not connected to a device.";
				this.Refresh();

				// Enable all buttons. 
				EnableAllBtns();
			}	

			m_intMessageCount = 0;

			// Disable the buttons.
			DisableAllBtns();

			this.lbStatus.Text = "Reading messages...";

			this.listSMS.Items.Clear();
			m_listStoreSMS.Clear();

			// Add the events. 
			m_SMSTrans.SMSReadMessageCompleted += new SMSEventHandler(SMSReadCompleted);
			m_SMSTrans.SMSSendMessageError += new SMSEventHandler(SMSReadMessageError);

			// Try first to get an index from the device because this will speed up the
			// reading process due to that some positions might be empty and then it might
			// take a very long time to read all the messages. 

			int[] iIndex = new int[0];
			try
			{
				// Get the index from the phone. 
				iIndex = m_SMSTrans.GetIndex();
			}
			catch ( SMSException exc )
			{
				if ( exc.ErrorCode == ErrorCodeType.TimeOut )
				{
					MessageBox.Show(exc.Message);
					this.lbStatus.Text = exc.Message;

					// Since the stream is closed after a timeout, the user needs to 
					// connect again or connect to another device. 
					this.btnDiscover.Enabled = true;
					this.deviceList.Enabled = true;
					return;
				}
				else
				{
					// If there is an exception, this device does not support 
					// index listing so dont't do anything with the exception. 
				}
			}

			if ( iIndex.Length > 0 )
			{
				// If the index length is > 0 we have an index so we can use it. 

				// Clear the queue.
				m_indexQueue.Clear();

				// Add the array to a queue. 
				for ( int iCounter = 0 ; iCounter < iIndex.Length; iCounter++)
				{
					m_indexQueue.Enqueue(iIndex[iCounter]);
				}

				// Read the first message. 
				int iPos = (int) m_indexQueue.Dequeue();

				try
				{
					m_SMSTrans.ReadMessageAsync(iPos);
				}
				catch ( SMSException exc )
				{
					if ( exc.ErrorCode == ErrorCodeType.TimeOut )
					{
						MessageBox.Show(exc.Message);
						this.lbStatus.Text = exc.Message;

						// Since the stream is closed after a timeout, the user needs to 
						// connect again or connect to another device. 
						this.btnDiscover.Enabled = true;
						this.deviceList.Enabled = true;

						// Remove the events. 
						m_SMSTrans.SMSReadMessageCompleted += new SMSEventHandler(SMSReadCompleted);
						m_SMSTrans.SMSSendMessageError += new SMSEventHandler(SMSReadMessageError);
						return;
					}
					else
					{

						MessageBox.Show(exc.Message);
					
						// Enable the buttons.
						EnableAllBtns();

						this.lbViewStatus.Text = "Operation failed.";

						// Remove the events. 
						m_SMSTrans.SMSReadMessageCompleted += new SMSEventHandler(SMSReadCompleted);
						m_SMSTrans.SMSSendMessageError += new SMSEventHandler(SMSReadMessageError);
						return;
					}
				}		
			}
			else
			{
				try
				{
					// Get the number of messages on the remote device. 
					m_intMessageCount = m_SMSTrans.NumberOfMessages();
				}
				catch ( SMSException exc )
				{
					if ( exc.ErrorCode == ErrorCodeType.TimeOut )
					{
						MessageBox.Show(exc.Message);
						this.lbStatus.Text = exc.Message;

						// Since the stream is closed after a timeout, the user needs to 
						// connect again or connect to another device. 
						this.btnDiscover.Enabled = true;
						this.deviceList.Enabled = true;

						// Remove the events. 
						m_SMSTrans.SMSReadMessageCompleted += new SMSEventHandler(SMSReadCompleted);
						m_SMSTrans.SMSSendMessageError += new SMSEventHandler(SMSReadMessageError);
						return;
					}
					else 
					{
						MessageBox.Show(exc.Message);

						// Enable the buttons.
						EnableAllBtns();

						this.lbViewStatus.Text = "Operation failed.";
					}
				}
				m_intReadCounter = 0;
				m_intMessagePos = 1;
			
				try
				{
					m_SMSTrans.ReadMessageAsync(m_intMessagePos);
				}
				catch ( SMSException exc )
				{
					if ( exc.ErrorCode == ErrorCodeType.TimeOut )
					{
						MessageBox.Show(exc.Message);
						this.lbStatus.Text = exc.Message;

						// Since the stream is closed after a timeout, the user needs to 
						// connect again or connect to another device. 
						this.btnDiscover.Enabled = true;
						this.deviceList.Enabled = true;

						// Remove the events. 
						m_SMSTrans.SMSReadMessageCompleted += new SMSEventHandler(SMSReadCompleted);
						m_SMSTrans.SMSSendMessageError += new SMSEventHandler(SMSReadMessageError);
						return;
					}
					else
					{
						MessageBox.Show(exc.Message);

						// Enable the buttons.
						EnableAllBtns();

						this.lbViewStatus.Text = "Operation failed.";
						
						// Remove the events. 
						m_SMSTrans.SMSReadMessageCompleted += new SMSEventHandler(SMSReadCompleted);
						m_SMSTrans.SMSSendMessageError += new SMSEventHandler(SMSReadMessageError);
					}
				}
			}
		}

		private void SMSReadCompleted(object sender, SMSEventArgs e) 
		{
			// Cast the event to the right type to get access to the SMSMessageRead property. 
			SMSMessageEventArgs messageEvent = (SMSMessageEventArgs) e;
			SMSMessage message = messageEvent.SMSMessageRead;
			
			if ( message == null ) 
			{
				// The ReadMeesage returns null if there is no message at the given position. 
				m_intMessagePos++;
			}
			else
			{
				// Don't add empty messages to the list box. 
				if ( message.Status != SMSStatus.Empty ) 
				{
					// Add the items to the ListView.
					this.listSMS.Items.Add(message.TextMessage);
					m_listStoreSMS.Add(message);
					this.Refresh();
				}
		
				// If we have an index, get the next
				// positon from the queue. 
				if ( m_indexQueue.Count > 0 )
				{
					int iPos = (int) m_indexQueue.Dequeue();

					try
					{
						m_SMSTrans.ReadMessageAsync(iPos);
					}
					catch ( SMSException exc)
					{
						if ( exc.ErrorCode == ErrorCodeType.TimeOut )
						{
							MessageBox.Show(exc.Message);
							this.lbStatus.Text = exc.Message;

							// Since the stream is closed after a timeout, the user needs to 
							// connect again or connect to another device. 
							this.btnDiscover.Enabled = true;
							this.deviceList.Enabled = true;

							// Remove the events. 
							m_SMSTrans.SMSReadMessageCompleted += new SMSEventHandler(SMSReadCompleted);
							m_SMSTrans.SMSSendMessageError += new SMSEventHandler(SMSReadMessageError);
							return;
						}
						else 
						{
							MessageBox.Show(exc.Message);

							// Enable the buttons.
							EnableAllBtns();

							this.lbViewStatus.Text = "Operation failed.";

							// Remove the events. 
							m_SMSTrans.SMSReadMessageCompleted += new SMSEventHandler(SMSReadCompleted);
							m_SMSTrans.SMSSendMessageError += new SMSEventHandler(SMSReadMessageError);
							return;
						}	
					}
				}

					// If there is no exit, read the next position. 
				else if (m_intReadCounter < m_intMessageCount)
				{
					m_intMessagePos++;

					try
					{
						m_SMSTrans.ReadMessageAsync(m_intMessagePos);
						
						// Another message has been read. 
						m_intReadCounter++;
					}
					catch ( SMSException exc )
					{
						if ( exc.ErrorCode == ErrorCodeType.TimeOut )
						{
							MessageBox.Show(exc.Message);
							this.lbStatus.Text = exc.Message;

							// Since the stream is closed after a timeout, the user needs to 
							// connect again or connect to another device. 
							this.btnDiscover.Enabled = true;
							this.deviceList.Enabled = true;

							// Remove the events. 
							m_SMSTrans.SMSReadMessageCompleted += new SMSEventHandler(SMSReadCompleted);
							m_SMSTrans.SMSSendMessageError += new SMSEventHandler(SMSReadMessageError);
							return;
						}
						else
						{
							MessageBox.Show(exc.Message);

							// Enable the buttons.
							EnableAllBtns();

							this.lbViewStatus.Text = "Operation failed.";

							// Remove the events. 
							m_SMSTrans.SMSReadMessageCompleted -= new SMSEventHandler(SMSReadCompleted);
							m_SMSTrans.SMSSendMessageError -= new SMSEventHandler(SMSReadMessageError);
						}	
					}
				}
				else 
				{
					// Remove the events. 
					m_SMSTrans.SMSReadMessageCompleted -= new SMSEventHandler(SMSReadCompleted);
					m_SMSTrans.SMSSendMessageError -= new SMSEventHandler(SMSReadMessageError);

					this.lbViewStatus.Text =  "All messages read.";
					// Enable the buttons.
					EnableAllBtns();
				}
			}
		}

		private void SMSReadMessageError(object sender, SMSEventArgs e)
		{
			this.lbViewStatus.Text = "Operation failed.";	

			// Enable the buttons.
			EnableAllBtns();;
			
			MessageBox.Show(e.Message);
		}

		private void btnDelete_Click(object sender, System.EventArgs e)
		{
			if ( m_SMSTrans == null ) 
			{
				MessageBox.Show("Error - Not connected to a device.");
				this.lbStatus.Text = "Not connected to a device.";
				this.Refresh();

				// Enable all buttons. 
				EnableAllBtns();
			}

			// Set up the events. 
			m_SMSTrans.SMSDeleteMessageCompleted += new SMSEventHandler(SMSDeleteCompleted);
			m_SMSTrans.SMSSendMessageError += new SMSEventHandler(SMSReadMessageError);

			// User has to wait until the delete operatin is finished. 
			// Disable the buttons.
			DisableAllBtns();
			
			// Get the message from the arraylist. 
			SMSMessage message = (SMSMessage) m_listStoreSMS[listSMS.SelectedIndex];

			try
			{
				if ( message != null ) 
				{
					m_SMSTrans.DeleteMessageAsync( message.Pos );

				}
			}
			catch ( SMSException exc )
			{
				if ( exc.ErrorCode == ErrorCodeType.TimeOut )
				{
					MessageBox.Show(exc.Message);
					this.lbStatus.Text = exc.Message;

					// Since the stream is closed after a timeout, the user needs to 
					// connect again or connect to another device. 
					this.btnDiscover.Enabled = true;
					this.deviceList.Enabled = true;

					// Remove the events. 
					m_SMSTrans.SMSReadMessageCompleted += new SMSEventHandler(SMSReadCompleted);
					m_SMSTrans.SMSSendMessageError += new SMSEventHandler(SMSReadMessageError);
					return;
				}
				else
				{
					this.lbViewStatus.Text = "Operation failed.";	

					// Remove the events. 
					m_SMSTrans.SMSDeleteMessageCompleted -= new SMSEventHandler(SMSDeleteCompleted);
					m_SMSTrans.SMSSendMessageError -= new SMSEventHandler(SMSReadMessageError);

					// Enable the buttons.
					EnableAllBtns();
					return;
				}
			}

			// Remove the message from the arraylist. 
			m_listStoreSMS.RemoveAt(listSMS.SelectedIndex);

			// Remove the message from the listbox. 
			listSMS.Items.RemoveAt(listSMS.SelectedIndex);
		}

		private void SMSDeleteCompleted(object sender, SMSEventArgs e) 
		{
			// Remove the event handlers. 
			m_SMSTrans.SMSDeleteMessageCompleted -= new SMSEventHandler(SMSDeleteCompleted);
			m_SMSTrans.SMSSendMessageError -= new SMSEventHandler(SMSReadMessageError);
				
			// Enable the buttons.
			EnableAllBtns();
			this.lbViewStatus.Text = "Message is deleted.";
		}

		private void listSMS_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			// Get the message from the list. 
			SMSMessage message = null;
			if ( listSMS.SelectedIndex < m_listStoreSMS.Count ) 
			{
				message = (SMSMessage) m_listStoreSMS[listSMS.SelectedIndex];
			}

			if ( message != null ) 
			{
				string strDate = "";
		
				// Get the date. 
				// Only show dates that are not empty ( != new DateTime() )
				if ( message.Date != new DateTime() ) 
				{
					strDate += message.Date.ToString()  + ", " ;
				}

				message.Date.ToString();
				MessageBox.Show(String.Format(strDate + message.Number.ToString() + ", " 
						+  ", " + "Status: " + message.Status +  ", " +  message.TextMessage));
			}
		}

		private void SMSMessageCompleted(object sender, SMSEventArgs e)
		{
			this.lbSendStatus.Text = "SMS message sent ok.";
			// Enable the buttons.
			EnableAllBtns();
		}

		private void btnSend_Click(object sender, System.EventArgs e)
		{
			if ( m_SMSTrans == null ) 
			{
				MessageBox.Show("Error - Not connected to a device.");
				this.lbSendStatus.Text = "Not connected to a device.";
				this.Refresh();
			}

			// Disable the buttons.
			DisableAllBtns();

			this.lbSendStatus.Text = "Sending SMS...";
			this.Refresh();

			SMSMessage Message = null;

			try
			{
				Message = new SMSMessage(this.tbxMessage.Text, new PhoneNumber( this.tbxNumber.Text ));
			}
			catch ( Exception exc)
			{
				MessageBox.Show(exc.Message);
				
				// Enable the buttons.
				EnableAllBtns();

				this.lbSendStatus.Text = "Operation failed...";
				return;
			}

			m_SMSTrans.SMSMessageCompleted += new SMSEventHandler(SMSMessageCompleted);
			m_SMSTrans.SMSSendMessageError += new SMSEventHandler(SMSMessageError);

			try
			{
				if ( Message != null ) 
				{
					m_SMSTrans.SendSMSAsync(Message);
				}
			}
			catch ( SMSException exc )
			{
				if ( exc.ErrorCode == ErrorCodeType.TimeOut )
				{
					MessageBox.Show(exc.Message);
					this.lbStatus.Text = exc.Message;

					// Since the stream is closed after a timeout, the user needs to 
					// connect again or connect to another device. 
					this.btnDiscover.Enabled = true;
					this.deviceList.Enabled = true;

					// Remove the events. 
					m_SMSTrans.SMSReadMessageCompleted += new SMSEventHandler(SMSReadCompleted);
					m_SMSTrans.SMSSendMessageError += new SMSEventHandler(SMSReadMessageError);
					return;
				}
			
				else 
				{
					// Remove the events. 
					m_SMSTrans.SMSMessageCompleted += new SMSEventHandler(SMSMessageCompleted);
					m_SMSTrans.SMSSendMessageError += new SMSEventHandler(SMSMessageError);

					MessageBox.Show(exc.Message);
				
					// Enable the buttons.
					EnableAllBtns();

					this.lbSendStatus.Text = "Operation failed...";
				}
			}
		}

		private void SMSMessageError(object sender, SMSEventArgs e)
		{
			this.lbSendStatus.Text = "Operation failed.";	
			MessageBox.Show(e.Message);
			EnableAllBtns();
		}

		/// <summary>
		/// Enable all the buttons on the form. Used after an operation has been completed for example. 
		/// </summary>
		private void EnableAllBtns()
		{
			this.btnDelete.Enabled = true;
			this.btnRead.Enabled = true;
			this.btnSend.Enabled = true;
		}

		/// <summary>
		/// Disable all the buttons on the form. Used during an operation.  
		/// </summary>
		private void DisableAllBtns()
		{
			this.btnDelete.Enabled = false;
			this.btnRead.Enabled = false;
			this.btnSend.Enabled = false;
		}

		private void MainForm_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			// Disconnect any connected device. 
			if ( m_SMSTrans != null ) 
			{
				try
				{
					m_SMSTrans.Disconnect();
				}
				catch ( Exception )
				{
				}
			}

			try
			{
				// m_manager must be disposed!
				Manager.GetManager().Dispose();
			} 
			catch ( Exception )
			{
			}
		}
	}
}
