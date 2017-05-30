using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.Data;
using Franson.BlueTools;
using System.IO;
using System.Text;
using Franson.BlueTools.SDP;

namespace FindAndConnect
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class MainForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.MainMenu mainMenu;
		private System.Windows.Forms.Button discoverDevices;
		private System.Windows.Forms.ListBox deviceList;
		private System.Windows.Forms.Label networkLabel;
		private System.Windows.Forms.ListBox serviceList;
		private System.Windows.Forms.TextBox serviceOutput;
		private System.Windows.Forms.Button closeConnection;
		private System.Windows.Forms.Label deviceLabel;
		private System.Windows.Forms.Label lStatus;
		private System.Windows.Forms.Label lStackID;

		private Manager			manager			= null;
		private Network			m_network		= null;
		private RemoteService	currentService	= null;
		private Stream			currentStream	= null;
		private System.Windows.Forms.Button bBond;
		private System.Windows.Forms.TextBox txtPIN;
		private byte[]			m_buffer		= new byte[25];	// Buffer to read data into

		public MainForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			this.Closing += new System.ComponentModel.CancelEventHandler(MainForm_Closing);

			try
			{
				manager	= Manager.GetManager();

				switch(Manager.StackID)
				{
					case StackID.STACK_MICROSOFT:
						lStackID.Text = "Microsoft Bluetooth Stack";
						break;
					case StackID.STACK_WIDCOMM:
						lStackID.Text = "WidComm Bluetooth Stack";
						break;
				}

				// You can get a valid evaluation key for BlueTools at
				// http://franson.com/bluetools/
				// That key will be valid for 14 days. Just cut and paste that key into the statement below.
				// To get a key that do not expire you need to purchase a license
				Franson.BlueTools.License license = new Franson.BlueTools.License();
				license.LicenseKey = "WoK6IL43AA7CJOMQTWbZNjvRCRhtPYEXWrSE";
				
				// Call events in GUI thread
				manager.Parent = this;
				// Call events in new thread (multi-threading)
				// manager.Parent = null

				// Get first netowrk (BlueTools 1.0 only supports one network == one dongle)
				m_network = manager.Networks[0];

				// Add events for device discovery
				m_network.DeviceDiscovered += new BlueToolsEventHandler(network_DeviceDiscovered);
				m_network.DeviceDiscoveryStarted += new BlueToolsEventHandler(network_DeviceDiscoveryStarted);
				m_network.DeviceDiscoveryCompleted += new BlueToolsEventHandler(network_DeviceDiscoveryCompleted);
				m_network.DeviceLost += new BlueToolsEventHandler(m_network_DeviceLost);

				// Stack version
				Version versionStack = m_network.StackVersion;
				lStackID.Text += " " + versionStack.ToString();
			} 
			catch (Exception exc)
			{
				MessageBox.Show(exc.Message);
				discoverDevices.Enabled = false;
			}
		}
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{

			try
			{
				//m_network.Dispose();
				// Manager must be disposed!
				Manager.GetManager().Dispose();
			} 
			catch (Exception)
			{
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
			this.mainMenu = new System.Windows.Forms.MainMenu();
			this.discoverDevices = new System.Windows.Forms.Button();
			this.deviceList = new System.Windows.Forms.ListBox();
			this.networkLabel = new System.Windows.Forms.Label();
			this.serviceList = new System.Windows.Forms.ListBox();
			this.serviceOutput = new System.Windows.Forms.TextBox();
			this.closeConnection = new System.Windows.Forms.Button();
			this.deviceLabel = new System.Windows.Forms.Label();
			this.lStatus = new System.Windows.Forms.Label();
			this.lStackID = new System.Windows.Forms.Label();
			this.bBond = new System.Windows.Forms.Button();
			this.txtPIN = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// discoverDevices
			// 
			this.discoverDevices.Location = new System.Drawing.Point(8, 192);
			this.discoverDevices.Name = "discoverDevices";
			this.discoverDevices.Size = new System.Drawing.Size(120, 23);
			this.discoverDevices.TabIndex = 6;
			this.discoverDevices.Text = "Discover";
			this.discoverDevices.Click += new System.EventHandler(this.discoverDevices_Click);
			// 
			// deviceList
			// 
			this.deviceList.Location = new System.Drawing.Point(8, 32);
			this.deviceList.Name = "deviceList";
			this.deviceList.Size = new System.Drawing.Size(120, 147);
			this.deviceList.TabIndex = 5;
			this.deviceList.SelectedIndexChanged += new System.EventHandler(this.deviceList_SelectedIndexChanged);
			// 
			// networkLabel
			// 
			this.networkLabel.Location = new System.Drawing.Point(8, 16);
			this.networkLabel.Name = "networkLabel";
			this.networkLabel.Size = new System.Drawing.Size(100, 16);
			this.networkLabel.TabIndex = 4;
			this.networkLabel.Text = "Network";
			// 
			// serviceList
			// 
			this.serviceList.Location = new System.Drawing.Point(136, 32);
			this.serviceList.Name = "serviceList";
			this.serviceList.Size = new System.Drawing.Size(120, 147);
			this.serviceList.TabIndex = 3;
			this.serviceList.SelectedIndexChanged += new System.EventHandler(this.serviceList_SelectedIndexChanged);
			// 
			// serviceOutput
			// 
			this.serviceOutput.Location = new System.Drawing.Point(8, 288);
			this.serviceOutput.Multiline = true;
			this.serviceOutput.Name = "serviceOutput";
			this.serviceOutput.ReadOnly = true;
			this.serviceOutput.Size = new System.Drawing.Size(248, 40);
			this.serviceOutput.TabIndex = 2;
			this.serviceOutput.Text = "";
			// 
			// closeConnection
			// 
			this.closeConnection.Enabled = false;
			this.closeConnection.Location = new System.Drawing.Point(136, 192);
			this.closeConnection.Name = "closeConnection";
			this.closeConnection.Size = new System.Drawing.Size(120, 23);
			this.closeConnection.TabIndex = 1;
			this.closeConnection.Text = "Close";
			this.closeConnection.Click += new System.EventHandler(this.closeConnection_Click);
			// 
			// deviceLabel
			// 
			this.deviceLabel.Location = new System.Drawing.Point(136, 16);
			this.deviceLabel.Name = "deviceLabel";
			this.deviceLabel.Size = new System.Drawing.Size(100, 16);
			this.deviceLabel.TabIndex = 0;
			this.deviceLabel.Text = "Device";
			// 
			// lStatus
			// 
			this.lStatus.Location = new System.Drawing.Point(8, 336);
			this.lStatus.Name = "lStatus";
			this.lStatus.Size = new System.Drawing.Size(248, 40);
			this.lStatus.TabIndex = 18;
			this.lStatus.Text = "Click Discover!";
			// 
			// lStackID
			// 
			this.lStackID.Location = new System.Drawing.Point(8, 376);
			this.lStackID.Name = "lStackID";
			this.lStackID.Size = new System.Drawing.Size(248, 16);
			this.lStackID.TabIndex = 19;
			// 
			// bBond
			// 
			this.bBond.Location = new System.Drawing.Point(8, 248);
			this.bBond.Name = "bBond";
			this.bBond.Size = new System.Drawing.Size(120, 24);
			this.bBond.TabIndex = 20;
			this.bBond.Text = "Bond";
			this.bBond.Click += new System.EventHandler(this.bBond_Click);
			// 
			// txtPIN
			// 
			this.txtPIN.Location = new System.Drawing.Point(8, 224);
			this.txtPIN.Name = "txtPIN";
			this.txtPIN.Size = new System.Drawing.Size(120, 20);
			this.txtPIN.TabIndex = 21;
			this.txtPIN.Text = "";
			// 
			// MainForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(272, 397);
			this.Controls.Add(this.txtPIN);
			this.Controls.Add(this.serviceOutput);
			this.Controls.Add(this.bBond);
			this.Controls.Add(this.lStackID);
			this.Controls.Add(this.lStatus);
			this.Controls.Add(this.deviceLabel);
			this.Controls.Add(this.closeConnection);
			this.Controls.Add(this.serviceList);
			this.Controls.Add(this.networkLabel);
			this.Controls.Add(this.deviceList);
			this.Controls.Add(this.discoverDevices);
			this.Menu = this.mainMenu;
			this.Name = "MainForm";
			this.Text = "Find And Discover";
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>

		static void Main() 
		{
			Application.Run(new MainForm());
		}

		private void discoverDevices_Click(object sender, System.EventArgs e)
		{
			try
			{
				// Start looking for devices on the network
				// Use Network.DiscoverDevices() if you don't want to use events.
				m_network.DiscoverDevicesAsync();
			} 
			catch (BlueToolsException exc)
			{
				MessageBox.Show(exc.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button1);
			}
		}

		private void network_DeviceDiscoveryStarted(object sender, BlueToolsEventArgs eventArgs)
		{
			// Search for devices on the network started

			serviceList.Items.Clear();
			discoverDevices.Enabled = false;

			lStatus.Text = "Discovering devices...";
		}

		private void network_DeviceDiscovered(object sender, BlueToolsEventArgs eventArgs)
		{
			// A new device was discovered!
			// Note that this event is only called for NEW devices on the network

			// Use Network.Devices for a complete list of discovered devices.

			RemoteDevice device = (RemoteDevice)((DiscoveryEventArgs)eventArgs).Discovery;
			deviceList.Items.Add(device);
		}

		private void network_DeviceDiscoveryCompleted(object sender, BlueToolsEventArgs eventArgs)
		{
			// Search for devices on the network ended.
			lStatus.Text = "Click on a Device!";
			discoverDevices.Enabled = true;
		}

		private void deviceList_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			// Get the remote device that is currently selected
			RemoteDevice device = (RemoteDevice)deviceList.SelectedItem;

			if(device != null)
			{	
				try
				{
					// Disable the device list so the user doesn't attempt to select a device while
					// the service lookup is pending
					deviceList.Enabled = false;

					// Remove the existing items from the service list
					serviceList.Items.Clear();

					// Add a DiscoveryListener so we get service discovery events
					device.ServiceDiscovered += new BlueToolsEventHandler(device_ServiceDiscovered);

					// Add a DiscoveryListener so we get service discovery completion events
					device.ServiceDiscoveryCompleted += new BlueToolsEventHandler(device_ServiceDiscoveryCompleted);

					device.Error += new BlueToolsEventHandler(device_Error);

					// Cancel any ongoing device discovery
					m_network.CancelDeviceDiscovery();

					// Discover all services offered by the device
		            // Use e.g. ServiceType.SerialPort to list only serial port services
					// Use Device.DiscoverServices() if you don't want to use events.
					// device.DiscoverServicesAsync(ServiceType.SerialPort);
					device.DiscoverServicesAsync(ServiceType.RFCOMM);

					// Add already discovered services to list box (for this device) 
					serviceList.Items.Clear();
					Service[] services = device.Services;
					foreach(RemoteService service in services)
					{
						serviceList.Items.Add(service);
					}

					lStatus.Text = "Searching for Services...";
				} 
				catch (Exception exc)
				{
					MessageBox.Show(exc.Message);
				}
			}
		}

		private void device_ServiceDiscovered(object sender, BlueToolsEventArgs eventArgs)
		{
			// A new serive found for device!
			// Note that this event is only called for NEW services found
			// Use Device.Services for a complete list of services for device

			// Get the service associated with the discovery event
			Service service = (Service)((DiscoveryEventArgs)eventArgs).Discovery;
			// Add the newly discovered service to the service list
			serviceList.Items.Add(service);
		}

		private void device_ServiceDiscoveryCompleted(object sender, BlueToolsEventArgs eventArgs)
		{
			// All services for device discovered"

			// Get the remote device that raised the event
			RemoteDevice device = (RemoteDevice)sender;

			// You find an array of all found services here.
			// Or by using Device.Services
			// Note that all services are RemoteService
			Service[] services = (Service[])((DiscoveryEventArgs)eventArgs).Discovery;

			// Remove event handlers
			device.ServiceDiscovered -= new BlueToolsEventHandler(device_ServiceDiscovered);
			device.ServiceDiscoveryCompleted -= new BlueToolsEventHandler(device_ServiceDiscoveryCompleted);

			// Reenable the device list
			deviceList.Enabled = true;

			lStatus.Text = "Click on a Service";
		}

		private void serviceList_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			// Close any connections that are open
			//

			if(currentService != null)
			{
				currentStream.Close();

				currentService	= null;
				currentStream	= null;
			}
			
			// Attempt to connect to the service
			//

			// Get service object from list box
			currentService = (RemoteService) serviceList.SelectedItem;

			try
			{
				// Connect to service by reading its Stream
				currentStream = currentService.Stream;

	            currentStream.BeginRead(m_buffer, 0, m_buffer.Length, new AsyncCallback(readCallback), currentStream);

				closeConnection.Enabled		= true;
				serviceList.Enabled			= false;
				lStatus.Text = "Receiving data...";
			} 
			catch (Exception exc)
			{
				MessageBox.Show(exc.Message);
				currentService = null;
			}
		}

		private void readCallback(IAsyncResult result)
		{
			byte[] buffer = null;

			// result is always a BlueToolsAsyncResult object
			BlueToolsAsyncResult blueAsyncResult = (BlueToolsAsyncResult) result;

			// We passed on buffer as custom object, you can pass on any object here. We passed the stream object
			ServiceStream stream = (ServiceStream) blueAsyncResult.AsyncState;
			
			// The buffer used for reading can be found in the result object.
			buffer = blueAsyncResult.Buffer;

			try
			{
				// EndRead() must always be called!
				// If stream has been closed due to an error, we'll have an excpetion here
				int len = stream.EndRead(result);

				System.Text.ASCIIEncoding enc = new ASCIIEncoding(); 
				string str = enc.GetString(buffer, 0, len);

				serviceOutput.Text = str;

				// Start new async read
				stream.BeginRead(m_buffer, 0, m_buffer.Length, new AsyncCallback(readCallback), stream);
			}
			catch(ObjectDisposedException ex)
			{
				// Thrown if stream has been closed.
				closeConnection.Enabled = false;
				serviceList.Enabled = true;

				lStatus.Text = ex.Message;
			}
		}
																																								  
		private void closeConnection_Click(object sender, System.EventArgs e)
		{
			if(currentService != null)
			{
				currentStream.Close();
				currentService = null;
				currentStream = null;
			}

			closeConnection.Enabled = false;
			serviceList.Enabled = true;
			lStatus.Text = "OK";
		}

		private void device_Error(object sender, BlueToolsEventArgs eventArgs)
		{
			Franson.BlueTools.ErrorEventArgs errorEventArgs = (Franson.BlueTools.ErrorEventArgs)eventArgs;
			MessageBox.Show(errorEventArgs.ErrorCode + ": " + errorEventArgs.Message);
		}

		private void MainForm_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			Dispose();
		}

		private void bBond_Click(object sender, System.EventArgs e)
		{
			RemoteDevice device = (RemoteDevice) deviceList.SelectedItem;
			if(device == null)
			{
				MessageBox.Show("Select a device first!");
			}
			else
			{
				try
				{
					// Use RemoteDevice.BondAsync() for async version
					device.Bond(txtPIN.Text);
				}
				catch(Exception ex)
				{
					MessageBox.Show(ex.ToString());
				}
			}
		}

		private void m_network_DeviceLost(object sender, BlueToolsEventArgs eventArgs)
		{
			RemoteDevice device = (RemoteDevice)((DiscoveryEventArgs)eventArgs).Discovery;
			deviceList.Items.Remove(device);
		}
	}
}
