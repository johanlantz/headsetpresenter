using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.Data;
using System.IO;

using Franson.BlueTools;
using Franson.Protocols.Obex;
using Franson.Protocols.Obex.ObjectPush;

namespace ObjectPushSample200CF
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class MainForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.ProgressBar progressBar;
		private System.Windows.Forms.Button sendBtn;
		private System.Windows.Forms.Button discoverBtn;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.ListBox deviceList;
		private System.Windows.Forms.OpenFileDialog openFileDialog;
		private System.Windows.Forms.Label informLabel;
		private System.Windows.Forms.StatusBar statusBar;

		private Manager m_manager = null;
		private Network m_network = null;
		private ObexObjectPush m_objectPush = null;
		private bool m_boolAborted = false;
		private bool m_boolDenied  = false;
		
		// these objects should be class global to prevent them from being taken by the garbage collector
		private RemoteService m_serviceCurrent	= null;
		private Stream m_streamCurrent			= null;
		private bool m_boolUnrecoverableError   = false;

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
				license.LicenseKey = "HU5UZexzz2JOHX68pijhSkaRWu00NRMbHY5X";

				// get bluetools manager
				m_manager = Manager.GetManager();

				// get first network dongle - bluetools 1.0 only supports one!
				m_network = m_manager.Networks[0];

				// marshal events into this class thread.
				m_manager.Parent = this;

				statusBar.Text = GetStack();
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
			// BlueTools manager must be disposed, otherwise you can't exit application!
			Manager.GetManager().Dispose();

			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.progressBar = new System.Windows.Forms.ProgressBar();
            this.sendBtn = new System.Windows.Forms.Button();
            this.discoverBtn = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.deviceList = new System.Windows.Forms.ListBox();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.statusBar = new System.Windows.Forms.StatusBar();
            this.informLabel = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // progressBar
            // 
            this.progressBar.Location = new System.Drawing.Point(40, 152);
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(160, 8);
            // 
            // sendBtn
            // 
            this.sendBtn.Enabled = false;
            this.sendBtn.Location = new System.Drawing.Point(128, 168);
            this.sendBtn.Name = "sendBtn";
            this.sendBtn.Size = new System.Drawing.Size(72, 20);
            this.sendBtn.TabIndex = 3;
            this.sendBtn.Text = "Push";
            this.sendBtn.Click += new System.EventHandler(this.sendBtn_Click);
            // 
            // discoverBtn
            // 
            this.discoverBtn.Location = new System.Drawing.Point(40, 168);
            this.discoverBtn.Name = "discoverBtn";
            this.discoverBtn.Size = new System.Drawing.Size(72, 20);
            this.discoverBtn.TabIndex = 4;
            this.discoverBtn.Text = "Discover";
            this.discoverBtn.Click += new System.EventHandler(this.discoverBtn_Click);
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(40, 8);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(100, 16);
            this.label1.Text = "Devices";
            // 
            // deviceList
            // 
            this.deviceList.Location = new System.Drawing.Point(40, 32);
            this.deviceList.Name = "deviceList";
            this.deviceList.Size = new System.Drawing.Size(160, 114);
            this.deviceList.TabIndex = 6;
            this.deviceList.SelectedIndexChanged += new System.EventHandler(this.deviceList_SelectedIndexChanged);
            // 
            // statusBar
            // 
            this.statusBar.Location = new System.Drawing.Point(0, 272);
            this.statusBar.Name = "statusBar";
            this.statusBar.Size = new System.Drawing.Size(240, 22);
            // 
            // informLabel
            // 
            this.informLabel.Location = new System.Drawing.Point(8, 200);
            this.informLabel.Name = "informLabel";
            this.informLabel.Size = new System.Drawing.Size(224, 40);
            this.informLabel.Text = "Click on discover to detect Bluetooth devices.";
            // 
            // MainForm
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Inherit;
            this.ClientSize = new System.Drawing.Size(240, 294);
            this.Controls.Add(this.informLabel);
            this.Controls.Add(this.statusBar);
            this.Controls.Add(this.progressBar);
            this.Controls.Add(this.sendBtn);
            this.Controls.Add(this.discoverBtn);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.deviceList);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "MainForm";
            this.Text = "Object Push Sample CF";
            this.Closing += new System.ComponentModel.CancelEventHandler(this.MainForm_Closing);
            this.ResumeLayout(false);

		}
		#endregion

		private void discoverBtn_Click(object sender, System.EventArgs e)
		{
			m_network.DeviceDiscovered			+= new BlueToolsEventHandler(m_network_DeviceDiscovered);
			m_network.DeviceDiscoveryStarted	+= new BlueToolsEventHandler(m_network_DeviceDiscoveryStarted);
			m_network.DeviceDiscoveryCompleted	+= new BlueToolsEventHandler(m_network_DeviceDiscoveryCompleted);

			// Start looking for devices on the network
			// Use Network.DiscoverDevices() if you don't want to use events.
			m_network.DiscoverDevicesAsync();
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

			// inform the user what we're doing
			informLabel.Text = "Scanning for devices...";

			// disable device list while scanning
			deviceList.Enabled = false;
		}

		private void m_network_DeviceDiscoveryCompleted(object sender, BlueToolsEventArgs eventArgs)
		{
			// enable Discover button again since device discovery is complete
			discoverBtn.Enabled = true;

			// inform the user what to do
			informLabel.Text = "Click on the device you wish to push a file to.";

			// enable device list again
			deviceList.Enabled = true;

			m_network.DeviceDiscovered			-= new BlueToolsEventHandler(m_network_DeviceDiscovered);
			m_network.DeviceDiscoveryStarted	-= new BlueToolsEventHandler(m_network_DeviceDiscoveryStarted);
			m_network.DeviceDiscoveryCompleted	-= new BlueToolsEventHandler(m_network_DeviceDiscoveryCompleted);
		}

		private void deviceList_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			// cancel any possible ongoing device discovery if user clicks on anything
			m_network.CancelDeviceDiscovery();

			// get selected item (a remote device)
			RemoteDevice selectedDevice = (RemoteDevice)deviceList.SelectedItem;

			selectedDevice.ServiceDiscoveryStarted += new BlueToolsEventHandler(selectedDevice_ServiceDiscoveryStarted);
			selectedDevice.ServiceDiscoveryCompleted += new BlueToolsEventHandler(selectedDevice_ServiceDiscoveryCompleted);
			// detect Object Push service (OPP) on this device
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
					// wait for 20 seconds for frozen devices
					m_objectPush = new ObexObjectPush(-1);

					// marshal event to this class thread
					m_objectPush.Parent = this;

					// setup event handlers
					m_objectPush.Error				+= new ObexEventHandler(m_objectPush_Error);
					m_objectPush.PutFileBegin		+= new ObexEventHandler(m_objectPush_PutFileBegin);
					m_objectPush.PutFileProgress	+= new ObexEventHandler(m_objectPush_PutFileProgress);
					m_objectPush.PutFileEnd			+= new ObexEventHandler(m_objectPush_PutFileEnd);
                    m_objectPush.DisconnectEnd      += new ObexEventHandler(m_objectPush_DisconnectEnd);

					sendBtn.Enabled		= true;

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
							sendBtn.Text = "Cancel";

							m_streamCurrent = m_serviceCurrent.Stream;

							FileStream fileStream = new FileStream(openFileDialog.FileName, FileMode.Open, FileAccess.Read);
							m_objectPush.PushFileAsync(fileStream, Path.GetFileName(openFileDialog.FileName), m_streamCurrent);
						}
					}
				}
				catch (Exception exc)
				{
					MessageBox.Show(exc.Message);
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

			// if max value on progressbar isn't set, set it
			if (copyArgs.Size != progressBar.Maximum)
            {
				progressBar.Maximum = (int)copyArgs.Size;
            }

			// set position of file copy
			progressBar.Value = (int)copyArgs.Position;

			// update copy panel to show progress in kb
			statusBar.Text = Convert.ToString(copyArgs.Position / 1024) + " kb / " + Convert.ToString(copyArgs.Size / 1024) + " kb";
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

			// ...change text caption back to Push
			sendBtn.Text = "Push";

			// set progressbar position back to beginning - just for show, doesn't really matter
			progressBar.Value = 0;

			// set copy panel to nothing to make it invisible again - it's auto-resizing :)
			statusBar.Text = GetStack();

			// inform the user what is happening
			if (m_boolAborted)
			{
				// ...if transfer was aborted..
				informLabel.Text = "Push aborted.";
			}
			else if (m_boolDenied)
			{
				// ... if transfer was impossible
				informLabel.Text = "Push denied by device.";
			}
			else if (m_boolUnrecoverableError)
			{
				informLabel.Text = "Stream was lost to device.";
			}
			else
			{
				// ...transfer completed
				informLabel.Text = "File(s) sent to device.";
			}

            // enable device list again when finished
            deviceList.Enabled = true;

			// show user what to do next
			informLabel.Text += "\nClick on Send to select file(s) to push to device.";

			// we enable the Send button again if it was disabled when it was a Cancel button
			sendBtn.Enabled = true;
		}

		private void m_objectPush_PutFileBegin(object sender, ObexEventArgs eventArgs)
		{
			// this transfer hasn't been aborted (yet!)
			m_boolAborted = false;

			// this transfer hasn't been denied (yet!)
			m_boolDenied = false;

			// this transfer has not had any errors (yet!)
			m_boolUnrecoverableError = false;

			// while copying, disable the discover button
			discoverBtn.Enabled = false;

            // disable device list while copying
            deviceList.Enabled = false;

			// inform the user what is happening
			informLabel.Text = "Sending file(s) to device...";
		}

		private void m_objectPush_Error(object sender, ObexEventArgs eventArgs)
		{
			ObexErrorEventArgs errorArgs = (ObexErrorEventArgs)eventArgs;
			
			// if the error was not an Obex error - we can assume the Stream is not available anymore
			if (errorArgs.Error == Franson.Protocols.Obex.ErrorCode.StreamError)
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

				if (errorArgs.Error == Franson.Protocols.Obex.ErrorCode.SendDenied)
				{
					m_boolDenied = true;
				}

				// show message to user
				MessageBox.Show(errorArgs.Message);
			}
		}

		private void device_Error(object sender, BlueToolsEventArgs eventArgs)
		{
			Franson.BlueTools.ErrorEventArgs errorArgs = (Franson.BlueTools.ErrorEventArgs)eventArgs;

			statusBar.Text = errorArgs.Message;
		}

        private void m_objectPush_DisconnectEnd(object sender, ObexEventArgs eventArgs)
        {
            // we close this stream to ensure that we get a fresh new one when pushing next time.
            // this is needed since some devices do not allow multiple pushes in one stream
            if (m_streamCurrent != null)
            {
                m_streamCurrent.Close();
            }
        }

		private string GetStack()
		{
			// update statusbar panel with name of currently used stack
			switch (Manager.StackID)
			{
				case StackID.STACK_MICROSOFT: 
				{
					return "Microsoft Stack";
				}

				case StackID.STACK_WIDCOMM:
				{
					return "Widcomm Stack";
				}

				default:
				{
					return "Unknown stack";
				}
			}
		}

		private void MainForm_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			// if m_serviceCurrent is available, close the stream
			if (m_serviceCurrent != null)
			{
				if (m_streamCurrent != null)
					m_streamCurrent.Close();

				// set these objects to null to mark them for GC
				m_streamCurrent  = null;
				m_serviceCurrent = null;
			}

			// dispose objects - also makes sure that Manager are disposed.
			Dispose();
		}
	}
}
