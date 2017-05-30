using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.IO;

using Franson.BlueTools;
using Franson.Protocols.Obex;
using Franson.Protocols.Obex.ObjectPush;

namespace ObjectPushSample
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class MainForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.OpenFileDialog openFileDialog;
		private System.Windows.Forms.ListBox deviceList;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button discoverBtn;
		private System.Windows.Forms.Button sendBtn;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private System.Windows.Forms.ProgressBar progressBar;
		private System.Windows.Forms.StatusBarPanel statusBarPanel;
		private System.Windows.Forms.StatusBar statusBar;
		private System.Windows.Forms.StatusBarPanel copyPanel;

		private Manager m_manager = null;
		private Network m_network = null;
		private ObexObjectPush m_objectPush = null;

		// these objects should be class global to prevent them from being taken by the garbage collector
		private RemoteService m_serviceCurrent = null;
		private Stream		  m_streamCurrent  = null;
		private System.Windows.Forms.Label informLabel;

		// this is to control that the stream is'nt closed before all transfers is done. 
		private int m_intTransfers = 0;

		// this is status booleans
		private bool m_boolAborted = false;
		private bool m_boolDenied  = false;
		private bool m_boolUnrecoverableError = false;
		private bool m_boolTimeout = false;
		

		public MainForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();


			// catch all BlueTools exceptions - like e.g. license expired
			try
			{
				// get your trial license or buy BlueTools at franson.com
				Franson.BlueTools.License license = new Franson.BlueTools.License();
				license.LicenseKey = "WoK6IL944A9CIONQRXaYUjpRJRiuHYFYWrT7";

				// get bluetools manager
				m_manager = Manager.GetManager();

				// get first network dongle - bluetools 1.0 only supports one!
				m_network = m_manager.Networks[0];

				// marshal events into this class thread.
				m_manager.Parent = this;

				// update statusbar panel with name of currently used stack
				switch (Manager.StackID)
				{
					case StackID.STACK_MICROSOFT: 
					{
						statusBarPanel.Text = "Microsoft Stack";
						break;
					}

					case StackID.STACK_WIDCOMM:
					{
						statusBarPanel.Text = "Widcomm Stack";

						break;
					}

					default:
					{
						statusBarPanel.Text = "Unknown stack";

						break;
					}
				}
			}
			catch (Exception exc)
			{
				// display BlueTools errors here
				MessageBox.Show(exc.Message);
			}
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				// BlueTools manager must be disposed, otherwise you can't exit application!
				Manager.GetManager().Dispose();

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
			this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
			this.deviceList = new System.Windows.Forms.ListBox();
			this.label1 = new System.Windows.Forms.Label();
			this.discoverBtn = new System.Windows.Forms.Button();
			this.sendBtn = new System.Windows.Forms.Button();
			this.progressBar = new System.Windows.Forms.ProgressBar();
			this.statusBar = new System.Windows.Forms.StatusBar();
			this.statusBarPanel = new System.Windows.Forms.StatusBarPanel();
			this.copyPanel = new System.Windows.Forms.StatusBarPanel();
			this.informLabel = new System.Windows.Forms.Label();
			((System.ComponentModel.ISupportInitialize)(this.statusBarPanel)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.copyPanel)).BeginInit();
			this.SuspendLayout();
			// 
			// openFileDialog
			// 
			this.openFileDialog.Multiselect = true;
			// 
			// deviceList
			// 
			this.deviceList.Location = new System.Drawing.Point(16, 32);
			this.deviceList.Name = "deviceList";
			this.deviceList.Size = new System.Drawing.Size(160, 160);
			this.deviceList.TabIndex = 0;
			this.deviceList.SelectedIndexChanged += new System.EventHandler(this.deviceList_SelectedIndexChanged);
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(16, 8);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(100, 16);
			this.label1.TabIndex = 1;
			this.label1.Text = "Devices";
			// 
			// discoverBtn
			// 
			this.discoverBtn.Location = new System.Drawing.Point(16, 216);
			this.discoverBtn.Name = "discoverBtn";
			this.discoverBtn.TabIndex = 2;
			this.discoverBtn.Text = "Discover";
			this.discoverBtn.Click += new System.EventHandler(this.discoverBtn_Click);
			// 
			// sendBtn
			// 
			this.sendBtn.Enabled = false;
			this.sendBtn.Location = new System.Drawing.Point(104, 216);
			this.sendBtn.Name = "sendBtn";
			this.sendBtn.TabIndex = 3;
			this.sendBtn.Text = "Push";
			this.sendBtn.Click += new System.EventHandler(this.sendBtn_Click);
			// 
			// progressBar
			// 
			this.progressBar.Location = new System.Drawing.Point(16, 200);
			this.progressBar.Name = "progressBar";
			this.progressBar.Size = new System.Drawing.Size(160, 8);
			this.progressBar.TabIndex = 4;
			// 
			// statusBar
			// 
			this.statusBar.Location = new System.Drawing.Point(0, 295);
			this.statusBar.Name = "statusBar";
			this.statusBar.Panels.AddRange(new System.Windows.Forms.StatusBarPanel[] {
																						 this.statusBarPanel,
																						 this.copyPanel});
			this.statusBar.ShowPanels = true;
			this.statusBar.Size = new System.Drawing.Size(192, 22);
			this.statusBar.SizingGrip = false;
			this.statusBar.TabIndex = 5;
			// 
			// statusBarPanel
			// 
			this.statusBarPanel.AutoSize = System.Windows.Forms.StatusBarPanelAutoSize.Spring;
			this.statusBarPanel.Width = 182;
			// 
			// copyPanel
			// 
			this.copyPanel.AutoSize = System.Windows.Forms.StatusBarPanelAutoSize.Contents;
			this.copyPanel.Width = 10;
			// 
			// informLabel
			// 
			this.informLabel.Location = new System.Drawing.Point(16, 248);
			this.informLabel.Name = "informLabel";
			this.informLabel.Size = new System.Drawing.Size(160, 40);
			this.informLabel.TabIndex = 6;
			this.informLabel.Text = "Click Discover to find nearby Bluetooth devices.";
			// 
			// MainForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(192, 317);
			this.Controls.Add(this.informLabel);
			this.Controls.Add(this.statusBar);
			this.Controls.Add(this.progressBar);
			this.Controls.Add(this.sendBtn);
			this.Controls.Add(this.discoverBtn);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.deviceList);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Name = "MainForm";
			this.Text = "ObjectPush Sample";
			this.Closing += new System.ComponentModel.CancelEventHandler(this.MainForm_Closing);
			((System.ComponentModel.ISupportInitialize)(this.statusBarPanel)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.copyPanel)).EndInit();
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

		private void discoverBtn_Click(object sender, System.EventArgs e)
		{
			m_network.DeviceDiscovered			+= new BlueToolsEventHandler(m_network_DeviceDiscovered);
			m_network.DeviceDiscoveryStarted	+= new BlueToolsEventHandler(m_network_DeviceDiscoveryStarted);
			m_network.DeviceDiscoveryCompleted	+= new BlueToolsEventHandler(m_network_DeviceDiscoveryCompleted);

			// Start looking for devices on the network
			// Use Network.DiscoverDevices() if you don't want to use events.
			m_network.DiscoverDevicesAsync();
		}

		private void MainForm_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			// if currentService is available, close the stream
			if (m_serviceCurrent != null)
			{
				if (m_streamCurrent != null)
				{
					m_streamCurrent.Close();
				}

				// set these objects to null to mark them for GC
				m_streamCurrent  = null;
				m_serviceCurrent = null;
			}

			// dispose objects - also makes sure that Manager are disposed.
			Dispose();
		}

		private void m_network_DeviceDiscovered(object sender, BlueToolsEventArgs eventArgs)
		{
			// add every device found
			RemoteDevice device = (RemoteDevice)((DiscoveryEventArgs)eventArgs).Discovery;
			deviceList.Items.Add(device);
		}

		private void m_network_DeviceDiscoveryStarted(object sender, BlueToolsEventArgs eventArgs)
		{
			// disable the Discover button when we begin device discovery
			discoverBtn.Enabled = false;

			// disable the send button since you can't send while device discovering
			sendBtn.Enabled = false;

			// disable the device list while searching for devices
			deviceList.Enabled = false;

			// inform the user what we're doing
			informLabel.Text = "Scanning for devices...";
		}

		private void m_network_DeviceDiscoveryCompleted(object sender, BlueToolsEventArgs eventArgs)
		{
			Device[] devices = (Device[])((DiscoveryEventArgs)eventArgs).Discovery;

			// enable Discover button again since device discovery is complete
			discoverBtn.Enabled = true;

			// enable the device list again
			deviceList.Enabled = true;

			// inform the user what to do
			informLabel.Text = "Click on the device you wish to push a file to.";

			m_network.DeviceDiscovered			-= new BlueToolsEventHandler(m_network_DeviceDiscovered);
			m_network.DeviceDiscoveryStarted	-= new BlueToolsEventHandler(m_network_DeviceDiscoveryStarted);
			m_network.DeviceDiscoveryCompleted	-= new BlueToolsEventHandler(m_network_DeviceDiscoveryCompleted);
		}

		private void deviceList_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			// get selected item (a remote device)
			RemoteDevice selectedDevice = (RemoteDevice)deviceList.SelectedItem;

			selectedDevice.ServiceDiscoveryStarted += new BlueToolsEventHandler(selectedDevice_ServiceDiscoveryStarted);
			selectedDevice.ServiceDiscoveryCompleted += new BlueToolsEventHandler(selectedDevice_ServiceDiscoveryCompleted);
			// detect Object Push service (OPP) on this device

			deviceList.Enabled = false;
			selectedDevice.DiscoverServicesAsync(ServiceType.OBEXObjectPush);
		}

		private void selectedDevice_ServiceDiscoveryStarted(object sender, BlueToolsEventArgs eventArgs)
		{
			// when beginning to search for services - disable the Discover button
			// we shouldn't scan for devices while attempting to scan for services.
			discoverBtn.Enabled = false;
			sendBtn.Enabled		= false;
		}

		private void selectedDevice_ServiceDiscoveryCompleted(object sender, BlueToolsEventArgs eventArgs)
		{
			// when service discovery is complete - let us re-enable the discover button. 
			// It is okay to try to update the device list
			discoverBtn.Enabled = true;

			// re-enable the device list
			deviceList.Enabled  = true;

			// set error event handler for the device
			RemoteDevice device = (RemoteDevice)sender;
			device.Error += new BlueToolsEventHandler(device_Error);

			// get all services found
			Service[] services = (Service[])((DiscoveryEventArgs)eventArgs).Discovery;

			// if we have a service since before, close the stream
			if (m_streamCurrent != null)
			{
				m_streamCurrent.Close();
				m_streamCurrent = null;
			}
			
			// and remove the service
			if (m_serviceCurrent != null)
			{
				m_serviceCurrent = null;
			}

			// if we found a new service...
			if (services.Length > 0)
			{				
				// ...get OPP service object
				m_serviceCurrent = (RemoteService)services[0];
								
				try
				{				
					// create an ObexObjectPush object connected to the ServiceStream
					m_objectPush = new ObexObjectPush(1000); // wait forever

					// marshal event to this class thread
					m_objectPush.Parent = this;

					// setup event handlers
					m_objectPush.Error				+= new ObexEventHandler(m_objectPush_Error);
					m_objectPush.PutFileBegin		+= new ObexEventHandler(m_objectPush_PutFileBegin);
					m_objectPush.PutFileProgress	+= new ObexEventHandler(m_objectPush_PutFileProgress);
					m_objectPush.PutFileEnd			+= new ObexEventHandler(m_objectPush_PutFileEnd);
					m_objectPush.DisconnectEnd		+= new ObexEventHandler(m_objectPush_DisconnectEnd);

					// enable the push button
					sendBtn.Enabled		= true;

					// inform the user of the next step
					informLabel.Text = "Click on Push to select file(s) to push to device.";
				}
				catch (Exception exc)
				{
					sendBtn.Enabled		= false;

					MessageBox.Show(exc.Message);
				}
			}			
		}

		private void sendBtn_Click(object sender, System.EventArgs e)
		{
			if (sendBtn.Text.Equals("Push"))
			{
				try
				{
					// if we have a service to send to
					if (m_serviceCurrent != null)
					{
						// make sure ObexObjectPush is initialized and select file to push to device
						if (m_objectPush != null && openFileDialog.ShowDialog() == DialogResult.OK)
						{
							

							foreach(string filename in openFileDialog.FileNames)
							{
								m_streamCurrent = m_serviceCurrent.Stream;

								FileStream fileStream = new FileStream(filename, FileMode.Open, FileAccess.Read);
								m_objectPush.PushFileAsync(fileStream, Path.GetFileName(filename), m_streamCurrent);

								//Increment number of transfers.
								m_intTransfers++;
							}
						}
					}
				}
				catch (Exception)
				{
					// disable Push button if there was an error
					sendBtn.Enabled = false;

					informLabel.Text = "Failed to get a socket.\nClick on the device you wish to push a file to.";
				}
			}
			else
			{
				// if button text isn't Send it will be Cancel - 
				// if user click it, we tell ObexObjectPush that we want to abort
				m_objectPush.Abort();

				// only need to press cancel once - won't help you to press it more
				sendBtn.Enabled = false;
			}
		}

		private void m_objectPush_PutFileProgress(object sender, ObexEventArgs eventArgs)
		{
			ObexCopyEventArgs copyArgs = (ObexCopyEventArgs)eventArgs;

			// if size isn't unknown
			if (copyArgs.Size != -1)
			{
				// if max value on progressbar isn't set, set it
				if (copyArgs.Size != progressBar.Maximum)
				{
					progressBar.Maximum = (int)copyArgs.Size;
				}

				// set position of file copy
				progressBar.Value = (int)copyArgs.Position;

				// update copy panel to show progress in kb
				copyPanel.Text = Convert.ToString(copyArgs.Position / 1024) + " kb / " + Convert.ToString(copyArgs.Size / 1024) + " kb";
			}
			else
			{
				// update copy panel to show progress in kb
				copyPanel.Text = Convert.ToString(copyArgs.Position / 1024) + " kb / ? kb";
			}
		}

		private void m_objectPush_PutFileEnd(object sender, ObexEventArgs eventArgs)
		{
			// when finished copying...

			// ...close file stream
			ObexCopyEventArgs copyArgs = (ObexCopyEventArgs)eventArgs;

			// Close Stream opened by ObexObjectPush
			copyArgs.Stream.Close();

			// ...enable discover button
			discoverBtn.Enabled = true;

			// ...change text caption back to Send
			sendBtn.Text = "Push";

			// set progressbar position back to beginning - just for show, doesn't really matter
			progressBar.Value = 0;

			// set copy panel to nothing to make it invisible again - it's auto-resizing :)
			copyPanel.Text = "";

			// inform the user what is happening
			if (m_boolAborted)
			{
				// ...if transfer was aborted..
				informLabel.Text = "Push aborted.\nClick on Push to select file(s) to push to device.";
			}
			else if (m_boolDenied)
			{
				// .. if push was denied by receiving device
				informLabel.Text = "Push denied by device.\nClick on Push to select file(s) to push to device.";		
			}
			else if (m_boolTimeout)
			{
				// ..if stream was lost because of a timeout
				informLabel.Text = "Stream lost to device due to device not responding within timeout interval." + 
								   "\nClick on the device you wish to push file(s) to.";
			}
			else if (m_boolUnrecoverableError)
			{
				// ..if stream was lost
				informLabel.Text = "Stream lost to device.\nClick on the device you wish to push file(s) to.";
			}
			else
			{
				// ...transfer completed
				informLabel.Text = "File(s) sent to device.\nClick on Push to select file(s) to push to device.";			
			}

			// enable device list again when finished
			deviceList.Enabled = true;

			// we enable the Send button again if it was disabled when it was a Cancel button
			sendBtn.Enabled = true;				 
		}

		private void m_objectPush_PutFileBegin(object sender, ObexEventArgs eventArgs)
		{
			// this transfer hasn't been aborted (yet!)
			m_boolAborted = false;

			// this transfer hasn't been denied (yet!)
			m_boolDenied = false;

			// there hasn't been any unrecoverable error (yet!)
			m_boolUnrecoverableError = false;

			// no timeout (yet!)
			m_boolTimeout = false;

			// while copying, disable the discover button
			discoverBtn.Enabled = false;

			// change the text so the user can Cancel the transfer
			sendBtn.Text = "Cancel";

			// inform the user what is happening
			informLabel.Text = "Sending file(s) to device...";

			// disable device list while pushing file
			deviceList.Enabled = false;
		}

		private void m_objectPush_Error(object sender, ObexEventArgs eventArgs)
		{
			ObexErrorEventArgs errorArgs = (ObexErrorEventArgs)eventArgs;
			
			if (errorArgs.Error == Franson.Protocols.Obex.ErrorCode.Timeout)
			{
				m_boolTimeout = true;

				// the Push button
				sendBtn.Enabled = false;

				// the device list
				deviceList.Enabled = true;
			}
			else if (errorArgs.Error == Franson.Protocols.Obex.ErrorCode.StreamError)
			{
				// since the stream was lost, re-enable the UI

				// the Push button
				sendBtn.Enabled = false;

				// the device list
				deviceList.Enabled = true;

				// this error can't be recovered from
				m_boolUnrecoverableError = true;

				// show this error in a box - it's pretty serious
				MessageBox.Show(errorArgs.Message);
			}
			else
			{
				// if the error is transfer was aborted, set boolean so we can display this to user
				if (errorArgs.Error == Franson.Protocols.Obex.ErrorCode.Aborted ||
					errorArgs.Error == Franson.Protocols.Obex.ErrorCode.SecurityAbort)
				{
					m_boolAborted = true;
				}

				// some devices return Forbidden when canceling (like Sony Ericsson phones) so let's tell 
				// user what really happened
				if (errorArgs.Error == Franson.Protocols.Obex.ErrorCode.SendDenied)
				{
					m_boolDenied = true;
				}
			}
		}

		private void device_Error(object sender, BlueToolsEventArgs eventArgs)
		{
			Franson.BlueTools.ErrorEventArgs errorArgs = (Franson.BlueTools.ErrorEventArgs)eventArgs;

			// show bluetools errors in boxes - they are serious, can't use obex without bluetools in this sample
			MessageBox.Show(errorArgs.Message);
		}

		private void m_objectPush_DisconnectEnd(object sender, ObexEventArgs e)
		{
			if (m_streamCurrent != null)
			{
				// Count down disconnections
				m_intTransfers--;

				// When we have had same number of disconnections as transfers we are done and can safely close the stream.
				if(m_intTransfers == 0)
				{
					m_streamCurrent.Close();
				}
			}
		}
	}
}
