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

namespace SMSManager
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class MainForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.GroupBox gboxSendSMS;
		private System.Windows.Forms.TextBox tbxMessage;
		private System.Windows.Forms.TextBox tbxPhone;
		private System.Windows.Forms.Label lbMessage;
		private System.Windows.Forms.Label lbPhone;

		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private System.Windows.Forms.Label lbStatus;
		private System.Windows.Forms.Label lbStatusText;
		private System.Windows.Forms.GroupBox gboxView;
		private System.Windows.Forms.ListView lviewSMS;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ColumnHeader columnHeader3;
		private System.Windows.Forms.GroupBox gboxDevice;
		internal System.Windows.Forms.ComboBox cboxDevice;
		private System.Windows.Forms.Button btnDiscover;
		private System.Windows.Forms.Button btnSendSync;
		private System.Windows.Forms.Button btnSendAsync;
		private System.Windows.Forms.Button btnReadSync;
		private System.Windows.Forms.Button btnReadSMSAsync;
		private System.Windows.Forms.Button btnDelSync;
		private System.Windows.Forms.Button btnDeleteAsync;
		private System.Windows.Forms.ColumnHeader chPhone;

		/// <summary>
		/// The transporter object that is used for all communication with the remote device. 
		/// </summary>
		private SMSTransporter m_SMSTrans = null;
		private SMSMessage m_Message = null;
		private Manager	m_manager = null;		
		private Network	m_network = null;		
		private RemoteService m_serviceCurrent = null;	
		private Stream m_streamCurrent = null;
		private Queue m_deviceQueue = null;
		private	ArrayList m_Devices = null;
		private int m_intReadCounter = 0;
		private int m_intMessageCount = 0;
		private int m_intMessagePos = 0;
		private Stack m_sDelList = null;
		private Queue m_indexQueue = null;

		public MainForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			// Instantiate the device queue and the serial port arraylist. 
			m_deviceQueue = new Queue();
			m_Devices = new ArrayList();
			m_indexQueue = new Queue();

			// Disable all buttons. 
			DisableAllBtns();


			// Innitialize network
			InitConnection();
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{	
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.gboxSendSMS = new System.Windows.Forms.GroupBox();
			this.btnSendAsync = new System.Windows.Forms.Button();
			this.lbPhone = new System.Windows.Forms.Label();
			this.lbMessage = new System.Windows.Forms.Label();
			this.tbxPhone = new System.Windows.Forms.TextBox();
			this.tbxMessage = new System.Windows.Forms.TextBox();
			this.btnSendSync = new System.Windows.Forms.Button();
			this.gboxView = new System.Windows.Forms.GroupBox();
			this.btnDeleteAsync = new System.Windows.Forms.Button();
			this.btnDelSync = new System.Windows.Forms.Button();
			this.lviewSMS = new System.Windows.Forms.ListView();
			this.chPhone = new System.Windows.Forms.ColumnHeader();
			this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
			this.btnReadSync = new System.Windows.Forms.Button();
			this.btnReadSMSAsync = new System.Windows.Forms.Button();
			this.lbStatus = new System.Windows.Forms.Label();
			this.lbStatusText = new System.Windows.Forms.Label();
			this.gboxDevice = new System.Windows.Forms.GroupBox();
			this.btnDiscover = new System.Windows.Forms.Button();
			this.cboxDevice = new System.Windows.Forms.ComboBox();
			this.gboxSendSMS.SuspendLayout();
			this.gboxView.SuspendLayout();
			this.gboxDevice.SuspendLayout();
			this.SuspendLayout();
			// 
			// gboxSendSMS
			// 
			this.gboxSendSMS.Controls.Add(this.btnSendAsync);
			this.gboxSendSMS.Controls.Add(this.lbPhone);
			this.gboxSendSMS.Controls.Add(this.lbMessage);
			this.gboxSendSMS.Controls.Add(this.tbxPhone);
			this.gboxSendSMS.Controls.Add(this.tbxMessage);
			this.gboxSendSMS.Controls.Add(this.btnSendSync);
			this.gboxSendSMS.Location = new System.Drawing.Point(280, 24);
			this.gboxSendSMS.Name = "gboxSendSMS";
			this.gboxSendSMS.Size = new System.Drawing.Size(544, 104);
			this.gboxSendSMS.TabIndex = 0;
			this.gboxSendSMS.TabStop = false;
			this.gboxSendSMS.Text = "Send SMS";
			// 
			// btnSendAsync
			// 
			this.btnSendAsync.Location = new System.Drawing.Point(400, 64);
			this.btnSendAsync.Name = "btnSendAsync";
			this.btnSendAsync.Size = new System.Drawing.Size(104, 23);
			this.btnSendAsync.TabIndex = 5;
			this.btnSendAsync.Text = "Send SMS ASync";
			this.btnSendAsync.Click += new System.EventHandler(this.btnSendAsync_Click);
			// 
			// lbPhone
			// 
			this.lbPhone.Location = new System.Drawing.Point(16, 64);
			this.lbPhone.Name = "lbPhone";
			this.lbPhone.Size = new System.Drawing.Size(88, 16);
			this.lbPhone.TabIndex = 4;
			this.lbPhone.Text = "Phone Number:";
			// 
			// lbMessage
			// 
			this.lbMessage.Location = new System.Drawing.Point(16, 24);
			this.lbMessage.Name = "lbMessage";
			this.lbMessage.Size = new System.Drawing.Size(88, 23);
			this.lbMessage.TabIndex = 3;
			this.lbMessage.Text = "Message:";
			// 
			// tbxPhone
			// 
			this.tbxPhone.Location = new System.Drawing.Point(104, 64);
			this.tbxPhone.Name = "tbxPhone";
			this.tbxPhone.Size = new System.Drawing.Size(152, 20);
			this.tbxPhone.TabIndex = 2;
			this.tbxPhone.Text = "";
			// 
			// tbxMessage
			// 
			this.tbxMessage.Location = new System.Drawing.Point(104, 24);
			this.tbxMessage.Name = "tbxMessage";
			this.tbxMessage.Size = new System.Drawing.Size(400, 20);
			this.tbxMessage.TabIndex = 1;
			this.tbxMessage.Text = "";
			// 
			// btnSendSync
			// 
			this.btnSendSync.Location = new System.Drawing.Point(296, 64);
			this.btnSendSync.Name = "btnSendSync";
			this.btnSendSync.Size = new System.Drawing.Size(96, 23);
			this.btnSendSync.TabIndex = 0;
			this.btnSendSync.Text = "Send SMS Sync";
			this.btnSendSync.Click += new System.EventHandler(this.btnSend_Click);
			// 
			// gboxView
			// 
			this.gboxView.Controls.Add(this.btnDeleteAsync);
			this.gboxView.Controls.Add(this.btnDelSync);
			this.gboxView.Controls.Add(this.lviewSMS);
			this.gboxView.Controls.Add(this.btnReadSync);
			this.gboxView.Controls.Add(this.btnReadSMSAsync);
			this.gboxView.Location = new System.Drawing.Point(24, 136);
			this.gboxView.Name = "gboxView";
			this.gboxView.Size = new System.Drawing.Size(800, 224);
			this.gboxView.TabIndex = 1;
			this.gboxView.TabStop = false;
			this.gboxView.Text = "View SMS";
			// 
			// btnDeleteAsync
			// 
			this.btnDeleteAsync.Location = new System.Drawing.Point(520, 184);
			this.btnDeleteAsync.Name = "btnDeleteAsync";
			this.btnDeleteAsync.Size = new System.Drawing.Size(112, 24);
			this.btnDeleteAsync.TabIndex = 8;
			this.btnDeleteAsync.Text = "Delete SMS Async.";
			this.btnDeleteAsync.Click += new System.EventHandler(this.btnDeleteAsync_Click);
			// 
			// btnDelSync
			// 
			this.btnDelSync.Location = new System.Drawing.Point(408, 184);
			this.btnDelSync.Name = "btnDelSync";
			this.btnDelSync.Size = new System.Drawing.Size(104, 24);
			this.btnDelSync.TabIndex = 7;
			this.btnDelSync.Text = "Delete SMS Sync.";
			this.btnDelSync.Click += new System.EventHandler(this.btnDelSync_Click);
			// 
			// lviewSMS
			// 
			this.lviewSMS.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
																					   this.chPhone,
																					   this.columnHeader1,
																					   this.columnHeader2,
																					   this.columnHeader3});
			this.lviewSMS.FullRowSelect = true;
			this.lviewSMS.GridLines = true;
			this.lviewSMS.Location = new System.Drawing.Point(24, 32);
			this.lviewSMS.Name = "lviewSMS";
			this.lviewSMS.Size = new System.Drawing.Size(752, 144);
			this.lviewSMS.TabIndex = 6;
			this.lviewSMS.View = System.Windows.Forms.View.Details;
			// 
			// chPhone
			// 
			this.chPhone.Text = "Phone Number";
			this.chPhone.Width = 100;
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "Date";
			this.columnHeader1.Width = 100;
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "Status";
			this.columnHeader2.Width = 100;
			// 
			// columnHeader3
			// 
			this.columnHeader3.Text = "Message";
			this.columnHeader3.Width = 1000;
			// 
			// btnReadSync
			// 
			this.btnReadSync.Location = new System.Drawing.Point(168, 184);
			this.btnReadSync.Name = "btnReadSync";
			this.btnReadSync.Size = new System.Drawing.Size(112, 24);
			this.btnReadSync.TabIndex = 0;
			this.btnReadSync.Text = "Read SMS Sync.";
			this.btnReadSync.Click += new System.EventHandler(this.btnRefresh_Click);
			// 
			// btnReadSMSAsync
			// 
			this.btnReadSMSAsync.Location = new System.Drawing.Point(288, 184);
			this.btnReadSMSAsync.Name = "btnReadSMSAsync";
			this.btnReadSMSAsync.Size = new System.Drawing.Size(112, 24);
			this.btnReadSMSAsync.TabIndex = 6;
			this.btnReadSMSAsync.Text = "Read SMS Async.";
			this.btnReadSMSAsync.Click += new System.EventHandler(this.btnReadSMSAsync_Click);
			// 
			// lbStatus
			// 
			this.lbStatus.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.lbStatus.Location = new System.Drawing.Point(32, 368);
			this.lbStatus.Name = "lbStatus";
			this.lbStatus.Size = new System.Drawing.Size(72, 24);
			this.lbStatus.TabIndex = 3;
			this.lbStatus.Text = "Status:";
			// 
			// lbStatusText
			// 
			this.lbStatusText.Location = new System.Drawing.Point(96, 368);
			this.lbStatusText.Name = "lbStatusText";
			this.lbStatusText.Size = new System.Drawing.Size(680, 24);
			this.lbStatusText.TabIndex = 4;
			// 
			// gboxDevice
			// 
			this.gboxDevice.Controls.Add(this.btnDiscover);
			this.gboxDevice.Controls.Add(this.cboxDevice);
			this.gboxDevice.Location = new System.Drawing.Point(24, 24);
			this.gboxDevice.Name = "gboxDevice";
			this.gboxDevice.Size = new System.Drawing.Size(240, 104);
			this.gboxDevice.TabIndex = 5;
			this.gboxDevice.TabStop = false;
			this.gboxDevice.Text = "Device";
			// 
			// btnDiscover
			// 
			this.btnDiscover.Location = new System.Drawing.Point(72, 64);
			this.btnDiscover.Name = "btnDiscover";
			this.btnDiscover.TabIndex = 4;
			this.btnDiscover.Text = "Discover";
			this.btnDiscover.Click += new System.EventHandler(this.btnDiscover_Click);
			// 
			// cboxDevice
			// 
			this.cboxDevice.AllowDrop = true;
			this.cboxDevice.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cboxDevice.Enabled = false;
			this.cboxDevice.Location = new System.Drawing.Point(24, 24);
			this.cboxDevice.Name = "cboxDevice";
			this.cboxDevice.Size = new System.Drawing.Size(184, 21);
			this.cboxDevice.TabIndex = 3;
			this.cboxDevice.SelectedIndexChanged += new System.EventHandler(this.cboxDevice_SelectedIndexChanged);
			// 
			// MainForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(848, 397);
			this.Controls.Add(this.gboxDevice);
			this.Controls.Add(this.gboxView);
			this.Controls.Add(this.gboxSendSMS);
			this.Controls.Add(this.lbStatusText);
			this.Controls.Add(this.lbStatus);
			this.Name = "MainForm";
			this.Text = "SMSManger";
			this.Closing += new System.ComponentModel.CancelEventHandler(this.MainForm_Closing);
			this.gboxSendSMS.ResumeLayout(false);
			this.gboxView.ResumeLayout(false);
			this.gboxDevice.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new MainForm());
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
				license.LicenseKey = "WoK6IL44AABBRONPZXZZSjtRGRjuJYHXWsL8";
				
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

		private void SMSMessageCompleted(object sender, SMSEventArgs e)
		{
			this.lbStatusText.Text = "SMS message sent ok.";
			EnableAllBtns();
		}

		private void SMSMessageError(object sender, SMSEventArgs e)
		{
			// Get the SMSErrorEvetnArgs.
			SMSErrorEventArgs eError = (SMSErrorEventArgs) e;
			if ( eError.ErrorCode == ErrorCodeType.TimeOut )
			{
				MessageBox.Show(eError.Message);
				this.lbStatusText.Text = eError.Message;

				// Since the stream is closed after a timeout, the user needs to 
				// connect again or connect to another device. 
				this.btnDiscover.Enabled = true;
				this.cboxDevice.Enabled = true;
			}
			else
			{
				this.lbStatusText.Text = "Operation failed.";	
				MessageBox.Show(e.Message);
				// Enable all buttons. 
				EnableAllBtns();
			}
		}

		private void SMSReadMessageError(object sender, SMSEventArgs e)
		{
			this.lbStatusText.Text = "Operation failed.";	
			EnableAllBtns();
			MessageBox.Show(e.Message);
		}

		private void btnSend_Click(object sender, System.EventArgs e)
		{
			if ( m_SMSTrans == null ) 
			{
				MessageBox.Show("Error - Not connected to a device.");
				this.lbStatusText.Text = "Not connected to a device.";
				this.Refresh();

				// Enable all buttons. 
				EnableAllBtns();
			
				return;
			}
			// Disable all button.
			DisableAllBtns();

			this.lbStatusText.Text = "Sending SMS...";
			this.Refresh();

			try
			{
				SMSMessage m_Message = new SMSMessage(this.tbxMessage.Text, new PhoneNumber(this.tbxPhone.Text)) ;
				m_SMSTrans.SendSMS(m_Message);
			}
			catch ( SMSException exc )
			{
				if ( exc.ErrorCode == ErrorCodeType.TimeOut )
				{
					MessageBox.Show(exc.Message);
					this.lbStatusText.Text = exc.Message;

					// Since the stream is closed after a timeout, the user needs to 
					// connect again or connect to another device. 
					this.btnDiscover.Enabled = true;
					this.cboxDevice.Enabled = true;
					return;
				}
				else 
				{
					MessageBox.Show(exc.Message);
					m_serviceCurrent = null;

					// Enable all buttons. 
					EnableAllBtns();
					return;
				}
					
			}
			this.lbStatusText.Text = "SMS message sent ok.";

			// Enable all buttons. 
			EnableAllBtns();
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
				catch (Exception)
				{
				}
			}

			try
			{
				// m_manager must be disposed!
				Manager.GetManager().Dispose();
			} 
			catch (Exception)
			{
			}	
		}

		private void btnRefresh_Click(object sender, System.EventArgs e)
		{
			if ( m_SMSTrans == null ) 
			{
				MessageBox.Show("Error - Not connected to a device.");
				this.lbStatusText.Text = "Not connected to a device.";
				this.Refresh();

				// Enable all buttons. 
				EnableAllBtns();
			}	
		
			// Disable all buttons. 
			DisableAllBtns();

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
					this.lbStatusText.Text = exc.Message;

					// Since the stream is closed after a timeout, the user needs to 
					// connect again or connect to another device. 
					this.btnDiscover.Enabled = true;
					this.cboxDevice.Enabled = true;
					return;
				}
				else
				{
					// If thers is a non timeout exception, this device does not support 
					// index listíng so dont'r do anyting wihth the exception. 
				}
			}

			if ( iIndex.Length > 0 )
			{
				// We have an index, so use it to know where to find the messages. 

				for ( int iCounter = 0 ; iCounter < iIndex.Length; iCounter++)
				{
					try 
					{
						// Read a message. 
						m_Message = m_SMSTrans.ReadMessage(iIndex[iCounter]);
					}
					catch ( SMSException exc)
					{
						if ( exc.ErrorCode == ErrorCodeType.TimeOut )
						{
							MessageBox.Show(exc.Message);
							this.lbStatusText.Text = exc.Message;

							// Since the stream is closed after a timeout, the user needs to 
							// connect again or connect to another device. 
							this.btnDiscover.Enabled = true;
							this.cboxDevice.Enabled = true;
							return;
						}
						else
						{
							// Enable all buttons. 
							EnableAllBtns();

							MessageBox.Show(exc.Message);
							return;
						}
					}

					if ( m_Message != null ) 
					{
						ListViewItem item1 = new ListViewItem(m_Message.Number.ToString() );
						
						// Add the time if it is not empty. 
						if ( m_Message.Date != new DateTime() ) 
						{
							item1.SubItems.Add(m_Message.Date.ToString() );
						}
						else
						{	
							// if the date is empty, add en empty filed. 
							item1.SubItems.Add("");
						}

						item1.SubItems.Add(m_Message.Status.ToString() );
						item1.SubItems.Add(m_Message.TextMessage);

						// Add the message to the tag for so that we can access it 
						// during delete etc. 
						item1.Tag = m_Message;

						// Add the items to the ListView.
						this.lviewSMS.Items.Add(item1);
						this.Refresh();
					}
				}
			}

				// If it was not possibel to get an index, read the number of messages 
				// on the device instead and then go through all the memory for messages. 
			else if ( iIndex.Length == 0 )
			{
				int intMessageCount = 0;
				try
				{
					// Get the number of messages on the remote device. 
					intMessageCount = m_SMSTrans.NumberOfMessages();
				}
				catch ( SMSException exc)
				{
					// Enable all buttons. 
					EnableAllBtns();

					MessageBox.Show(exc.Message);
					return;
				}
			
				// The message index starts at position 1. 
				int intMessageNumber = 1;
				int intReadCounter = 0;

				// Update form. 
				this.lbStatusText.Text = "Reading SMS from device...";
				this.lviewSMS.Items.Clear();
				this.Refresh();

				// Read all message from the device. 
				while ( intReadCounter < intMessageCount)
				{
					try 
					{
						m_Message = m_SMSTrans.ReadMessage(intMessageNumber);
					}
					catch ( SMSException exc)
					{
						if ( exc.ErrorCode == ErrorCodeType.TimeOut )
						{
							MessageBox.Show(exc.Message);
							this.lbStatusText.Text = exc.Message;

							// Since the stream is closed after a timeout, the user needs to 
							// connect again or connect to another device. 
							this.btnDiscover.Enabled = true;
							this.cboxDevice.Enabled = true;
							return;
						}
						else
						{
							// Enable all buttons. 
							EnableAllBtns();

							MessageBox.Show(exc.Message);
							return;
						}
					}

					if ( m_Message == null ) 
					{
						// The ReadMeesage returns null if there is no mwssage at the given position. 
						intMessageNumber++;
					}
					else
					{
						ListViewItem item1 = new ListViewItem(m_Message.Number.ToString() );
						
						// Add the time if it is not empty. 
						if ( m_Message.Date != new DateTime() ) 
						{
							item1.SubItems.Add(m_Message.Date.ToString() );
						}
						else
						{	
							// if the date is empty, add en empty filed. 
							item1.SubItems.Add("");
						}

						item1.SubItems.Add(m_Message.Status.ToString() );
						item1.SubItems.Add(m_Message.TextMessage);

						
						// Add the message to the tag for so that we can access it 
						// during delete etc. 
						item1.Tag = m_Message;

						// Add the items to the ListView.
						this.lviewSMS.Items.Add(item1);
						this.Refresh();

						intReadCounter++;
						intMessageNumber++;
					}
				}
				this.lbStatusText.Text = "All messages read.";
				this.Refresh();

				// Enable all buttons. 
				EnableAllBtns();
			}	

		}

		private void btnDiscover_Click(object sender, System.EventArgs e)
		{

			try
			{
				// Start looking for devices on the network
				// Use Network.DiscoverDevices() if you don't want to use events.
				if ( m_network != null ) 
				{
					this.lbStatusText.Text = "Discovering devices..";
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
				this.lbStatusText.Text = "Discovery failed.";
				this.btnDiscover.Enabled = true;
			}
		
		}

		#region Discovery devices

		private void network_DeviceDiscoveryStarted(object sender, BlueToolsEventArgs eventArgs)
		{
			// Search for devices on the network started

			this.cboxDevice.Enabled = false;
			this.lbStatusText.Text = "Discovering devices...";
		}

		private void network_DeviceDiscovered(object sender, BlueToolsEventArgs eventArgs)
		{
			// A new device was discovered!
			// Note that this event is only called for NEW devices on the network

			// Use Network.Devices for a complete list of discovered devices.

			RemoteDevice device = (RemoteDevice)((DiscoveryEventArgs)eventArgs).Discovery;
			this.lbStatusText.Text = "Found new device: " + device.Name;

			// Add the device to the combobox. 
			this.cboxDevice.Items.Add(device);

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

			// Reenable the device list
			this.cboxDevice.Enabled = true;
			m_network.CancelDeviceDiscovery();

			this.lbStatusText.Text = "Discovery completed";
			this.btnDiscover.Enabled = true;
		}


		private void device_ServiceDiscovered(object sender, BlueToolsEventArgs eventArgs)
		{
			// A new serive found for device!
			// Note that this event is only called for NEW services found
			// Use Device.Services for a complete list of services for device

			RemoteService service = null;
			RemoteDevice device = null;

			try
			{
				// Get the service associated with the discovery event
				service = (RemoteService)((DiscoveryEventArgs)eventArgs).Discovery;
				// Search for devices on the network ended.

				device = (RemoteDevice)sender;
			}
			catch (Exception exc)
			{
				// Show an error message. 
				MessageBox.Show(exc.Message);
			}

			/*
				
			if ( service != null ) 
			{
				// Add the serial port services to the list.  
				if ( (service.ServiceType.GUID == ServiceType.SerialPort.GUID) || ( service.ServiceType.GUID == ServiceType.DialupNetworking.GUID ) )
				{
					// Add the serial port service to the serial port list. 
					m_sSerialPorts.Push(service);

					// Show the service in the staus field. 
					this.lbStatusText.Text = device.Name + " " + service.Name;
				}
			}
			*/
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
							// Add the serial port services to the list.  
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
				this.lbStatusText.Text = "No serial or dial up services found on the device.";
				this.cboxDevice.Enabled = true;
				return;
			}
		
			this.lbStatusText.Text = "Connecting..";
			this.Refresh();

			// Connect to the selected serial port. 
			ConnectToService(rService);

			// Connect to the serial port stream. 
			if(m_streamCurrent != null)
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
					this.lbStatusText.Text = "Failed to connect.";
				}

				this.lbStatusText.Text = "Connected";

				// Enable all buttons. 
				EnableAllBtns();

				this.cboxDevice.Enabled = false;
			}
		}

		private void device_Error(object sender, BlueToolsEventArgs eventArgs)
		{
			Franson.BlueTools.ErrorEventArgs errorEventArgs = (Franson.BlueTools.ErrorEventArgs)eventArgs;
			RemoteDevice device = (RemoteDevice)sender;

			MessageBox.Show(errorEventArgs.ErrorCode + ": " + errorEventArgs.Message );
			
			// Empty the stack. 			
			this.cboxDevice.Enabled = true;
			
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
				
				// Reset connections. 
				m_serviceCurrent = null;
				m_streamCurrent = null;

				this.lbStatusText.Text = "Unable to connect";
				this.btnDiscover.Enabled = true;
			}
		}

		private void btnSendAsync_Click(object sender, System.EventArgs e)
		{
			if (  m_SMSTrans == null )
			{
				MessageBox.Show("Not connected to a device.");
				m_serviceCurrent = null;
				
				// Enable all buttons. 
				EnableAllBtns();

				this.lbStatusText.Text = "Not connected to a device.";
				this.Refresh();
			}
 
			// Disable all buttons. 
			DisableAllBtns();

			this.lbStatusText.Text = "Sending SMS...";
			this.Refresh();

			try
			{
				SMSMessage Message = new SMSMessage(this.tbxMessage.Text, new PhoneNumber( this.tbxPhone.Text ));
				m_SMSTrans.SendSMSAsync(Message);

				m_SMSTrans.SMSMessageCompleted += new SMSEventHandler(SMSMessageCompleted);
				m_SMSTrans.SMSSendMessageError += new SMSEventHandler(SMSMessageError);
			}
			catch ( SMSException exc )
			{
				if ( exc.ErrorCode == ErrorCodeType.TimeOut )
				{
					MessageBox.Show(exc.Message);
					this.lbStatusText.Text = exc.Message;

					// Since the stream is closed after a timeout, the user needs to 
					// connect again or connect to another device. 
					this.btnDiscover.Enabled = true;
					this.cboxDevice.Enabled = true;
					return;
				}
				else
				{
					MessageBox.Show(exc.Message);
					m_serviceCurrent = null;
				
					// Enable all buttons. 
					EnableAllBtns();

					this.lbStatusText.Text = "Operation failed...";
					this.Refresh();
				}
			}
		}

		private void btnReadSMSAsync_Click(object sender, System.EventArgs e)
		{
			m_intMessageCount = 0;

			this.lviewSMS.Items.Clear();

			if ( m_SMSTrans == null ) 
			{
				// Enable all buttons. 
				EnableAllBtns();

				MessageBox.Show("Error - Not connected to a device.");
				this.lbStatusText.Text = "Not connected to a device.";
				this.Refresh();
			}

			// Disable all buttons. 
			DisableAllBtns();
			this.lbStatusText.Text = "Reading messages...";

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
			catch ( SMSException exc) 
			{
				if ( exc.ErrorCode == ErrorCodeType.TimeOut )
				{
					MessageBox.Show(exc.Message);
					this.lbStatusText.Text = exc.Message;

					// Since the stream is closed after a timeout, the user needs to 
					// connect again or connect to another device. 
					this.btnDiscover.Enabled = true;
					this.cboxDevice.Enabled = true;
					return;
				}
				else
				{
					// If thers is a non timeout exception, this device does not support 
					// index listíng so dont'r do anyting wihth the exception. 
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
				catch ( SMSException exc) 
				{
					if ( exc.ErrorCode == ErrorCodeType.TimeOut )
					{
						MessageBox.Show(exc.Message);
						this.lbStatusText.Text = exc.Message;

						// Since the stream is closed after a timeout, the user needs to 
						// connect again or connect to another device. 
						this.btnDiscover.Enabled = true;
						this.cboxDevice.Enabled = true;

						// Remove the events. 
						m_SMSTrans.SMSReadMessageCompleted += new SMSEventHandler(SMSReadCompleted);
						m_SMSTrans.SMSSendMessageError += new SMSEventHandler(SMSReadMessageError);

						return;
					}
					else
					{

						MessageBox.Show(exc.Message);
						
						// Disable all buttons. 
						EnableAllBtns();

						this.lbStatusText.Text = "Operation failed.";

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
				catch ( SMSException exc)
				{
					MessageBox.Show(exc.Message);
						
					// Disable all buttons. 
					EnableAllBtns();

					this.lbStatusText.Text = "Operation failed.";
				}

				m_intReadCounter = 0;
				m_intMessagePos = 1;
			
				try
				{
					m_SMSTrans.ReadMessageAsync(m_intMessagePos);
				}
				catch ( SMSException exc)
				{
					if ( exc.ErrorCode == ErrorCodeType.TimeOut )
					{
						MessageBox.Show(exc.Message);
						this.lbStatusText.Text = exc.Message;

						// Since the stream is closed after a timeout, the user needs to 
						// connect again or connect to another device. 
						this.btnDiscover.Enabled = true;
						this.cboxDevice.Enabled = true;

						// Remove the events. 
						m_SMSTrans.SMSReadMessageCompleted += new SMSEventHandler(SMSReadCompleted);
						m_SMSTrans.SMSSendMessageError += new SMSEventHandler(SMSReadMessageError);

						return;
					}
					else
					{

						MessageBox.Show(exc.Message);
						
						// Disable all buttons. 
						EnableAllBtns();

						this.lbStatusText.Text = "Operation failed.";

						// Remove the events. 
						m_SMSTrans.SMSReadMessageCompleted += new SMSEventHandler(SMSReadCompleted);
						m_SMSTrans.SMSSendMessageError += new SMSEventHandler(SMSReadMessageError);
						return;
					}
				}
			}
		}

		private void SMSReadCompleted(object sender, SMSEventArgs e) 
		{
			// Cast the event to the rigth type to get access to the SMSMessageRead property. 
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
					// Creat a new item and add the phone number. 		
					ListViewItem item1 = new ListViewItem( message.Number.ToString() );
					
					// Add the time if it is not empty. 
					if ( message.Date != new DateTime() ) 
					{
						item1.SubItems.Add(message.Date.ToString() );
					}		
					else
					{	
						// if the date is empty, add en empty filed. 
						item1.SubItems.Add("");
					}
					
					// Add the status. 
					item1.SubItems.Add(message.Status.ToString() );
					
					// Add the text message. 
					item1.SubItems.Add(message.TextMessage) ;

					// Add the message to the list box if we want to access it later. 
					item1.Tag = message;
				
					// A message has been read. Add the ReadCounter. 
					m_intReadCounter++;

					// Add the items to the ListView.
					this.lviewSMS.Items.Add(item1);
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
						MessageBox.Show(exc.Message);
						
						// Enable all buttons. 
						EnableAllBtns();

						this.lbStatusText.Text = "Operation failed.";

						// Remove the events. 
						m_SMSTrans.SMSReadMessageCompleted += new SMSEventHandler(SMSReadCompleted);
						m_SMSTrans.SMSSendMessageError += new SMSEventHandler(SMSReadMessageError);
						return;
					}	
				}

					// If there is no exit, read the next position. 
				else if (m_intReadCounter < m_intMessageCount)
				{
					m_intMessagePos++;

					try
					{
						m_SMSTrans.ReadMessageAsync(m_intMessagePos);
					}
					catch ( SMSException exc) 
					{
						MessageBox.Show(exc.Message);
						
						// Enable all buttons. 
						EnableAllBtns();

						this.lbStatusText.Text = "Operation failed.";

						// Remove the events. 
						m_SMSTrans.SMSReadMessageCompleted += new SMSEventHandler(SMSReadCompleted);
						m_SMSTrans.SMSSendMessageError += new SMSEventHandler(SMSReadMessageError);
						return;
					}	
				}
				else 
				{
					// Remove the events. 
					m_SMSTrans.SMSReadMessageCompleted -= new SMSEventHandler(SMSReadCompleted);
					m_SMSTrans.SMSSendMessageError -= new SMSEventHandler(SMSReadMessageError);

					this.lbStatusText.Text =  "All messages read.";
					
					// Enable all buttons. 
					EnableAllBtns();
				}
			}
		}

		private void btnDeleteAsync_Click(object sender, System.EventArgs e)
		{
			if ( m_SMSTrans == null ) 
			{
				// Enable all buttons. 
				EnableAllBtns();

				MessageBox.Show("Error - Not connected to a device.");
				this.lbStatusText.Text = "Not connected to a device.";
				this.Refresh();
			}

			ListView.SelectedListViewItemCollection selected = lviewSMS.SelectedItems;
    
			// int iCount = lviewSMS.SelectedItems.Count;
			m_sDelList = new Stack();

			SMSMessage smsMessage;

			// Disable all buttons. 
			DisableAllBtns();

			foreach ( ListViewItem item in selected )
			{
				smsMessage = (SMSMessage) item.Tag;

				if ( smsMessage != null ) 
				{
					m_sDelList.Push( smsMessage.Pos );
					item.Remove();
				}
			}

			// Delete the first message selected. 

			if ( m_sDelList.Count > 0 )
			{
				// Add event listerners. 
				m_SMSTrans.SMSDeleteMessageCompleted += new SMSEventHandler(SMSDeleteCompleted);
				m_SMSTrans.SMSSendMessageError += new SMSEventHandler(SMSReadMessageError);

				int iMsgPos = (int) m_sDelList.Pop();
				try
				{
					m_SMSTrans.DeleteMessageAsync( iMsgPos );
				}
				catch ( SMSException exc )
				{
					// Remove event listeners. 
					m_SMSTrans.SMSDeleteMessageCompleted += new SMSEventHandler(SMSDeleteCompleted);
					m_SMSTrans.SMSSendMessageError += new SMSEventHandler(SMSReadMessageError);

					if ( exc.ErrorCode == ErrorCodeType.TimeOut )
					{
						MessageBox.Show(exc.Message);
						this.lbStatusText.Text = exc.Message;

						// Since the stream is closed after a timeout, the user needs to 
						// connect again or connect to another device. 
						this.btnDiscover.Enabled = true;
						this.cboxDevice.Enabled = true;
						return;
					}
					else
					{
						MessageBox.Show(exc.Message);	
					}
				}
			}
			else
			{
				this.lbStatusText.Text = "Nothing to delete.";
				
				// Enable all buttons. 
				EnableAllBtns();
			}
		}

		
		private void SMSDeleteCompleted(object sender, SMSEventArgs e) 
		{

			if ( m_sDelList.Count == 0 ) 
			{
				m_SMSTrans.SMSDeleteMessageCompleted -= new SMSEventHandler(SMSDeleteCompleted);
				m_SMSTrans.SMSSendMessageError -= new SMSEventHandler(SMSReadMessageError);
				
				this.lbStatusText.Text = "Message(s) deleted.";
				
				// Enable all buttons. 
				EnableAllBtns();
			}
			else
			{
				// Delete the next message
				int iMsgPos = (int) m_sDelList.Pop();
				
				try
				{
					m_SMSTrans.DeleteMessageAsync( iMsgPos );
				}
				catch ( SMSException exc)
				{	
					this.lbStatusText.Text = "Operation failed.";
				
					// Enable all buttons. 
					EnableAllBtns();

					if ( exc.ErrorCode == ErrorCodeType.TimeOut )
					{
						MessageBox.Show(exc.Message);
						this.lbStatusText.Text = exc.Message;

						// Since the stream is closed after a timeout, the user needs to 
						// connect again or connect to another device. 
						this.btnDiscover.Enabled = true;
						this.cboxDevice.Enabled = true;
						return;
					}
					else
					{
						MessageBox.Show(exc.Message);	
					}
				}
			}
		}

		private void cboxDevice_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			// Get the selcted device. 
			RemoteDevice device = (RemoteDevice) cboxDevice.SelectedItem;
			
			try
			{
				
				// Disable the device list so the user doesn't attempt to select a device while
				// the service lookup is pending
				//
	
				// Add a DiscoveryListener so we get service discovery events
				device.ServiceDiscovered += new BlueToolsEventHandler(device_ServiceDiscovered);

				// Add a DiscoveryListener so we get service discovery completion events
				device.ServiceDiscoveryCompleted += new BlueToolsEventHandler(device_ServiceDiscoveryCompleted);

				// Add a DiscoveryListener so we get discovery error events. 
				device.Error += new BlueToolsEventHandler(device_Error);

				// Cancel any ongoing device discovery
				m_network.CancelDeviceDiscovery();

				// Discover all services offered by the device
				// Use e.g. ServiceType.SerialPort to list only serial port services
				// Use Device.DiscoverServices() if you don't want to use events.
				device.DiscoverServicesAsync(ServiceType.RFCOMM);

				this.lbStatusText.Text = "Searching for serial services...";
				this.Refresh();
				this.btnDiscover.Enabled = false;
					
					
			}
			catch (Exception exc)
			{
				MessageBox.Show(exc.Message);
			}
		}
	

		private void btnDelSync_Click(object sender, System.EventArgs e)
		{
			if ( m_SMSTrans == null ) 
			{
				// Enable all buttons. 
				EnableAllBtns();

				MessageBox.Show("Error - Not connected to a device.");
				this.lbStatusText.Text = "Not connected to a device.";
				this.Refresh();
			}

			ListView.SelectedListViewItemCollection selected = this.lviewSMS.SelectedItems;
			SMSMessage smsMessage = null;

			// Disable all buttons. 
			DisableAllBtns();

			bool msgDeleted = false;
			foreach ( ListViewItem item in selected )
			{
				smsMessage = (SMSMessage) item.Tag;
				if ( smsMessage != null ) 
				{
					int iMsgNumber = smsMessage.Pos;
					try
					{
						m_SMSTrans.DeleteMessage(iMsgNumber);
						item.Remove();
						msgDeleted = true;
					}
					catch ( SMSException exc )
					{
						if ( exc.ErrorCode == ErrorCodeType.TimeOut )
						{
							MessageBox.Show(exc.Message);
							this.lbStatusText.Text = exc.Message;

							// Since the stream is closed after a timeout, the user needs to 
							// connect again or connect to another device. 
							this.btnDiscover.Enabled = true;
							this.cboxDevice.Enabled = true;
							return;
						}
						else
						{
							MessageBox.Show(exc.Message);	
						}
					}
				}
			}
			if ( msgDeleted == false)
			{
				this.lbStatusText.Text = "Nothing to delete.";
			}
			else
			{
				this.lbStatusText.Text = "Message(s) deleted.";
			}

			// Enable all buttons. 
			EnableAllBtns();
		}

		/// <summary>
		/// Enable all buttons after an operation. 
		/// </summary>
		public void EnableAllBtns()
		{
			this.btnReadSMSAsync.Enabled = true;
			this.btnReadSync.Enabled = true;
			this.btnSendAsync.Enabled = true;
			this.btnSendSync.Enabled = true;
			this.btnDelSync.Enabled = true;
			this.btnDeleteAsync.Enabled = true;
		}

		/// <summary>
		/// Disble all button during an operation. 
		/// </summary>
		public void DisableAllBtns()
		{
			this.btnReadSMSAsync.Enabled = false;
			this.btnReadSync.Enabled = false;
			this.btnSendAsync.Enabled = false;
			this.btnSendSync.Enabled = false;
			this.btnDelSync.Enabled = false;
			this.btnDeleteAsync.Enabled = false;
		}
	}
}

