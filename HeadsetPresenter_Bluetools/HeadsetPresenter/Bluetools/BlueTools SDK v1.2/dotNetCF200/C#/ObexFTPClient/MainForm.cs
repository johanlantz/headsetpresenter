using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.Data;
using System.IO;

using Franson.BlueTools;
using Franson.Protocols.Obex;
using Franson.Protocols.Obex.FTPClient;

namespace ObexFTPClientSample200CF
{
	/// <summary>
	/// Summary description for MainForm.
	/// </summary>
	public class MainForm : System.Windows.Forms.Form
	{

		// BlueTools Fields
		private Manager			m_manager			= null;
		private Network			m_network		    = null;
		private RemoteService	m_serviceCurrent	= null;
		private Stream			m_streamCurrent	    = null;
		private FileBrowser		fileBrowser		    = null;
		private System.Windows.Forms.SaveFileDialog saveFileDialog;
        private System.Windows.Forms.TabControl tabControl;
		private System.Windows.Forms.TabPage deviceTab;
		private System.Windows.Forms.ProgressBar progressBar;
		private System.Windows.Forms.ListBox fileList;
		private System.Windows.Forms.Button downloadBtn;
		private System.Windows.Forms.Button discoverBtn;
		private System.Windows.Forms.Button disconnectBtn;
		private System.Windows.Forms.Button deleteBtn;
		private System.Windows.Forms.Button uploadBtn;
		private System.Windows.Forms.OpenFileDialog openFileDialog;
		private System.Windows.Forms.Button createBtn;
		private System.Windows.Forms.Button helpBtn;
		private System.Windows.Forms.Button connectBtn;
		private System.Windows.Forms.Button deviceHelpBtn;
		private System.Windows.Forms.TabPage fileTab;
		private System.Windows.Forms.CheckBox allowSelectCheckBox;
        private ListBox deviceList;		

		// count errors
		private int m_iUnrecoverableError = 0;

		public MainForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			try
			{
                // get your trial license or buy BlueTools at franson.com
				Franson.BlueTools.License license = new Franson.BlueTools.License();
                license.LicenseKey = "HU5UZer022JOLX98oidhQkbRbvv0NRJbHXEc";

				// get bluetools manager
                m_manager = Manager.GetManager();

				// Call events in GUI thread
				m_manager.Parent = this;

				// Get first netowrk (BlueTools 1.0 only supports one network == one dongle)
                m_network = m_manager.Networks[0];

				// Add events for device discovery
				m_network.DeviceDiscovered			+= new BlueToolsEventHandler(network_DeviceDiscovered);
				m_network.DeviceDiscoveryStarted	+= new BlueToolsEventHandler(network_DeviceDiscoveryStarted);
				m_network.DeviceDiscoveryCompleted	+= new BlueToolsEventHandler(network_DeviceDiscoveryCompleted);
			} 
			catch (Exception exc)
			{
				MessageBox.Show(exc.Message);				
			}

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.tabControl = new System.Windows.Forms.TabControl();
            this.deviceTab = new System.Windows.Forms.TabPage();
            this.deviceHelpBtn = new System.Windows.Forms.Button();
            this.connectBtn = new System.Windows.Forms.Button();
            this.disconnectBtn = new System.Windows.Forms.Button();
            this.discoverBtn = new System.Windows.Forms.Button();
            this.fileTab = new System.Windows.Forms.TabPage();
            this.allowSelectCheckBox = new System.Windows.Forms.CheckBox();
            this.helpBtn = new System.Windows.Forms.Button();
            this.createBtn = new System.Windows.Forms.Button();
            this.deleteBtn = new System.Windows.Forms.Button();
            this.uploadBtn = new System.Windows.Forms.Button();
            this.downloadBtn = new System.Windows.Forms.Button();
            this.progressBar = new System.Windows.Forms.ProgressBar();
            this.fileList = new System.Windows.Forms.ListBox();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.deviceList = new System.Windows.Forms.ListBox();
            this.tabControl.SuspendLayout();
            this.deviceTab.SuspendLayout();
            this.fileTab.SuspendLayout();
            this.SuspendLayout();
            // 
            // saveFileDialog
            // 
            this.saveFileDialog.FileName = "doc1";
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.deviceTab);
            this.tabControl.Controls.Add(this.fileTab);
            this.tabControl.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.tabControl.Location = new System.Drawing.Point(0, -8);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(240, 302);
            this.tabControl.TabIndex = 0;
            // 
            // deviceTab
            // 
            this.deviceTab.Controls.Add(this.deviceList);
            this.deviceTab.Controls.Add(this.deviceHelpBtn);
            this.deviceTab.Controls.Add(this.connectBtn);
            this.deviceTab.Controls.Add(this.disconnectBtn);
            this.deviceTab.Controls.Add(this.discoverBtn);
            this.deviceTab.Location = new System.Drawing.Point(0, 0);
            this.deviceTab.Name = "deviceTab";
            this.deviceTab.Size = new System.Drawing.Size(240, 279);
            this.deviceTab.Text = "Devices";
            // 
            // deviceHelpBtn
            // 
            this.deviceHelpBtn.Location = new System.Drawing.Point(129, 103);
            this.deviceHelpBtn.Name = "deviceHelpBtn";
            this.deviceHelpBtn.Size = new System.Drawing.Size(88, 20);
            this.deviceHelpBtn.TabIndex = 0;
            this.deviceHelpBtn.Text = "Help";
            this.deviceHelpBtn.Click += new System.EventHandler(this.deviceHelpBtn_Click);
            // 
            // connectBtn
            // 
            this.connectBtn.Enabled = false;
            this.connectBtn.Location = new System.Drawing.Point(25, 129);
            this.connectBtn.Name = "connectBtn";
            this.connectBtn.Size = new System.Drawing.Size(88, 20);
            this.connectBtn.TabIndex = 1;
            this.connectBtn.Text = "Connect";
            this.connectBtn.Click += new System.EventHandler(this.connectBtn_Click);
            // 
            // disconnectBtn
            // 
            this.disconnectBtn.Enabled = false;
            this.disconnectBtn.Location = new System.Drawing.Point(129, 129);
            this.disconnectBtn.Name = "disconnectBtn";
            this.disconnectBtn.Size = new System.Drawing.Size(88, 20);
            this.disconnectBtn.TabIndex = 2;
            this.disconnectBtn.Text = "Disconnect";
            this.disconnectBtn.Click += new System.EventHandler(this.disconnectBtn_Click);
            // 
            // discoverBtn
            // 
            this.discoverBtn.Location = new System.Drawing.Point(25, 103);
            this.discoverBtn.Name = "discoverBtn";
            this.discoverBtn.Size = new System.Drawing.Size(88, 20);
            this.discoverBtn.TabIndex = 3;
            this.discoverBtn.Text = "Scan";
            this.discoverBtn.Click += new System.EventHandler(this.discoverBtn_Click);
            // 
            // fileTab
            // 
            this.fileTab.Controls.Add(this.allowSelectCheckBox);
            this.fileTab.Controls.Add(this.helpBtn);
            this.fileTab.Controls.Add(this.createBtn);
            this.fileTab.Controls.Add(this.deleteBtn);
            this.fileTab.Controls.Add(this.uploadBtn);
            this.fileTab.Controls.Add(this.downloadBtn);
            this.fileTab.Controls.Add(this.progressBar);
            this.fileTab.Controls.Add(this.fileList);
            this.fileTab.Location = new System.Drawing.Point(0, 0);
            this.fileTab.Name = "fileTab";
            this.fileTab.Size = new System.Drawing.Size(240, 279);
            this.fileTab.Text = "Files";
            // 
            // allowSelectCheckBox
            // 
            this.allowSelectCheckBox.Location = new System.Drawing.Point(17, 192);
            this.allowSelectCheckBox.Name = "allowSelectCheckBox";
            this.allowSelectCheckBox.Size = new System.Drawing.Size(130, 20);
            this.allowSelectCheckBox.TabIndex = 0;
            this.allowSelectCheckBox.Text = "Allow folder select";
            // 
            // helpBtn
            // 
            this.helpBtn.Location = new System.Drawing.Point(16, 166);
            this.helpBtn.Name = "helpBtn";
            this.helpBtn.Size = new System.Drawing.Size(89, 20);
            this.helpBtn.TabIndex = 1;
            this.helpBtn.Text = "Help";
            this.helpBtn.Click += new System.EventHandler(this.helpBtn_Click);
            // 
            // createBtn
            // 
            this.createBtn.Enabled = false;
            this.createBtn.Location = new System.Drawing.Point(17, 140);
            this.createBtn.Name = "createBtn";
            this.createBtn.Size = new System.Drawing.Size(88, 20);
            this.createBtn.TabIndex = 2;
            this.createBtn.Text = "Create dir";
            this.createBtn.Click += new System.EventHandler(this.createBtn_Click);
            // 
            // deleteBtn
            // 
            this.deleteBtn.Enabled = false;
            this.deleteBtn.Location = new System.Drawing.Point(128, 140);
            this.deleteBtn.Name = "deleteBtn";
            this.deleteBtn.Size = new System.Drawing.Size(88, 20);
            this.deleteBtn.TabIndex = 3;
            this.deleteBtn.Text = "Delete";
            this.deleteBtn.Click += new System.EventHandler(this.deleteBtn_Click);
            // 
            // uploadBtn
            // 
            this.uploadBtn.Enabled = false;
            this.uploadBtn.Location = new System.Drawing.Point(16, 114);
            this.uploadBtn.Name = "uploadBtn";
            this.uploadBtn.Size = new System.Drawing.Size(88, 20);
            this.uploadBtn.TabIndex = 4;
            this.uploadBtn.Text = "Upload";
            this.uploadBtn.Click += new System.EventHandler(this.uploadBtn_Click);
            // 
            // downloadBtn
            // 
            this.downloadBtn.Enabled = false;
            this.downloadBtn.Location = new System.Drawing.Point(128, 114);
            this.downloadBtn.Name = "downloadBtn";
            this.downloadBtn.Size = new System.Drawing.Size(88, 20);
            this.downloadBtn.TabIndex = 5;
            this.downloadBtn.Text = "Download";
            this.downloadBtn.Click += new System.EventHandler(this.downloadBtn_Click);
            // 
            // progressBar
            // 
            this.progressBar.Location = new System.Drawing.Point(10, 101);
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(214, 8);
            // 
            // fileList
            // 
            this.fileList.Location = new System.Drawing.Point(12, 12);
            this.fileList.Name = "fileList";
            this.fileList.Size = new System.Drawing.Size(216, 86);
            this.fileList.TabIndex = 7;
            this.fileList.SelectedIndexChanged += new System.EventHandler(this.fileList_SelectedIndexChanged);
            // 
            // deviceList
            // 
            this.deviceList.Location = new System.Drawing.Point(12, 12);
            this.deviceList.Name = "deviceList";
            this.deviceList.Size = new System.Drawing.Size(225, 86);
            this.deviceList.TabIndex = 5;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.ClientSize = new System.Drawing.Size(240, 294);
            this.Controls.Add(this.tabControl);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "MainForm";
            this.Text = "Obex FTPClient Sample CF";
            this.Closing += new System.ComponentModel.CancelEventHandler(this.MainForm_Closing);
            this.tabControl.ResumeLayout(false);
            this.deviceTab.ResumeLayout(false);
            this.fileTab.ResumeLayout(false);
            this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>	
		protected override void Dispose(bool disposing)
		{
			base.Dispose(disposing);

            // Dispose must be called for the application to exit!

            if (m_manager != null)
            {
                m_manager.Dispose();
            }
		}

        private void network_DeviceDiscovered(object sender, BlueToolsEventArgs eventArgs)
        {
            DiscoveryEventArgs discoveryEventArgs = (DiscoveryEventArgs)eventArgs;
            Device device = (Device)discoveryEventArgs.Discovery;

            deviceList.Items.Add(device);
        }

        /// <summary>
        /// Method called when bluetools begins to scan for devices.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="eventArgs"></param>
        private void network_DeviceDiscoveryStarted(object sender, BlueToolsEventArgs eventArgs)
        {
            // empty the control
            deviceList.Items.Clear();

            foreach (RemoteDevice device in m_network.Devices)
            {
                deviceList.Items.Add(device);
            }

            // disable the button - can only run one device discovery at the time after all
            discoverBtn.Enabled = false;

            // disable the listbox
            deviceList.Enabled = false;
        }

        /// <summary>
        /// Method called when bluetools has finished scanning for devices.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="eventArgs"></param>
        private void network_DeviceDiscoveryCompleted(object sender, BlueToolsEventArgs eventArgs)
        {
            // enable the button - user might want to scan devices again!
            discoverBtn.Enabled = true;

            // enable the listbox so we can connect to devices.
            deviceList.Enabled = true;
        }

        private void discoverBtn_Click(object sender, System.EventArgs e)
        {
            if (!m_network.DiscoveryPending)
            {
                // find devices in the vicinity
                m_network.DiscoverDevicesAsync();
            }
        }

        /// <summary>
        /// Method called when user selects an item in the list box.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void deviceList_SelectedIndexChanged(object sender, System.EventArgs e)
        {
            connectBtn.Enabled = true;
        }

        /// <summary>
        /// Method called when Bluetools has finished searching for services on a remote device.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="eventArgs"></param>
        private void device_ServiceDiscoveryCompleted(object sender, BlueToolsEventArgs eventArgs)
        {
            DiscoveryEventArgs discoveryArgs = (DiscoveryEventArgs)eventArgs;

            Service[] services = (Service[])discoveryArgs.Discovery;

            // if services is above 0 we found an Obex FTP service.
            if (services.Length > 0)
            {
                try
                {
                    m_serviceCurrent = (RemoteService)services[0];
                    m_streamCurrent = m_serviceCurrent.Stream;

                    fileBrowser = new FileBrowser();

                    // same event handlers for both put & get
                    fileBrowser.GetFileProgress += new Franson.Protocols.Obex.ObexEventHandler(fileBrowser_PutFileProgress);
                    fileBrowser.GetFileEnd += new ObexEventHandler(fileBrowser_PutFileEnd);
                    fileBrowser.PutFileEnd += new ObexEventHandler(fileBrowser_PutFileEnd);
                    fileBrowser.PutFileProgress += new ObexEventHandler(fileBrowser_PutFileProgress);

                    fileBrowser.CreateDirectoryEnd += new ObexEventHandler(fileBrowser_CreateDirectoryEnd);
                    // marshal events into this class thread
                    fileBrowser.Parent = this;

                    // event handlers for connect, disconnect and getpath 
                    fileBrowser.DisconnectEnd += new ObexEventHandler(fileBrowser_DisconnectEnd);
                    fileBrowser.GetPathEnd += new ObexEventHandler(fileBrowser_GetPathEnd);
                    fileBrowser.SetPathEnd += new ObexEventHandler(fileBrowser_SetPathEnd);
                    fileBrowser.ConnectEnd += new ObexEventHandler(fileBrowser_ConnectEnd);
                    fileBrowser.DeleteEnd += new ObexEventHandler(fileBrowser_DeleteEnd);
                    fileBrowser.Error += new ObexEventHandler(fileBrowser_Error);

                    fileBrowser.ConnectAsync(m_streamCurrent);
                    fileBrowser.GetPathAsync();
                }
                catch (Exception exc)
                {
                    // if connection fails we must handle this too and restore access to ui

                    discoverBtn.Enabled = true;
                    deviceList.Enabled = true;

                    MessageBox.Show(exc.Message);
                }
            }
            else
            {
                // if there was no Obex FTP service found we must enable parts of the UI again so user
                // can select another device
                deviceList.Enabled = true;
                discoverBtn.Enabled = true;

                MessageBox.Show("No Obex FTP service found on selected device.");
            }

            // remove event handler for service discovery
            ((RemoteDevice)sender).ServiceDiscoveryCompleted -= new BlueToolsEventHandler(device_ServiceDiscoveryCompleted);
        }

        /// <summary>
        /// Method called when FileBrowser instance has finished getting path.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void fileBrowser_GetPathEnd(object sender, Franson.Protocols.Obex.ObexEventArgs e)
        {
            fileList.Items.Clear();

            FileFolder[] contents = fileBrowser.Items.ToArray();

            foreach (FileFolder ff in contents)
            {
                if (ff.IsFolder)
                {
                    fileList.Items.Add("Folder: " + ff.Name);
                }
                else
                {
                    fileList.Items.Add("File: " + ff.Name);
                }
            }

            fileList.Items.Insert(0, "..");

            // enable the list box again
            fileList.Enabled = true;
            downloadBtn.Enabled = true;
            uploadBtn.Enabled = true;
            deleteBtn.Enabled = true;
            createBtn.Enabled = true;
        }

        private void downloadBtn_Click(object sender, System.EventArgs e)
        {
            if (downloadBtn.Text.Equals("Abort"))
            {
                fileBrowser.Abort();
            }
            else
            {
                // avoid the .. item - it has no FileFolder
                if (fileList.SelectedIndex > 0)
                {
                    // get the selected file from the cache
                    FileFolder ff = fileBrowser.Items[fileList.SelectedIndex - 1];

                    saveFileDialog.FileName = ff.Name;

                    // this must be added so CF will add the extension properly.
                    saveFileDialog.Filter = Path.GetExtension(ff.Name) + "-files|" + Path.GetExtension(ff.Name);

                    if (saveFileDialog.ShowDialog() == DialogResult.OK)
                    {
                        downloadBtn.Text = "Abort";

                        FileStream fs = new FileStream(saveFileDialog.FileName, FileMode.Create, FileAccess.Write);
                        fileBrowser.GetFileAsync(fs, ff.Name);
                    }
                }
            }
        }

        private void fileBrowser_PutFileProgress(object sender, Franson.Protocols.Obex.ObexEventArgs e)
        {
            ObexCopyEventArgs copyArgs = (ObexCopyEventArgs)e;

            if (copyArgs.Size != -1)
            {
                if (copyArgs.Size != (int)progressBar.Maximum)
                {
                    progressBar.Maximum = (int)copyArgs.Size;
                }

                progressBar.Value = (int)copyArgs.Position;
            }

            fileList.Enabled = false;
        }

        private void fileBrowser_PutFileEnd(object sender, ObexEventArgs e)
        {
            ObexCopyEventArgs copyArgs = (ObexCopyEventArgs)e;

            // close the stream we opened before
            copyArgs.Stream.Close();

            // return progressBar to beginning
            progressBar.Value = 0;

            fileList.Enabled = true;

            // restore button captions
            uploadBtn.Text = "Upload";
            downloadBtn.Text = "Download";

            if (m_iUnrecoverableError == 0)
            {
                // enable buttons
                uploadBtn.Enabled = true;
                downloadBtn.Enabled = true;

                // update current folder
                fileBrowser.GetPathAsync();
            }
        }

        private void disconnectBtn_Click(object sender, System.EventArgs e)
        {
            // click disconnect we can attempt to disconnect
            fileBrowser.DisconnectAsync();
        }

        private void fileBrowser_DisconnectEnd(object sender, ObexEventArgs e)
        {
            // we disconnected, so we can change target device
            deviceList.Enabled = true;
            fileList.Enabled = false;

            // empty file list
            fileList.Items.Clear();

            // we're not connected, we can't disconnect
            disconnectBtn.Enabled = false;

            // can't access files if we're not connected
            uploadBtn.Enabled = false;
            downloadBtn.Enabled = false;
            deleteBtn.Enabled = false;
            createBtn.Enabled = false;
            discoverBtn.Enabled = true;

            if (m_streamCurrent != null)
            {
                m_streamCurrent.Close();
                m_streamCurrent = null;
            }

            m_serviceCurrent = null;
            // remove this event handler - we'll re-add it on connect
            fileBrowser.DisconnectEnd -= new ObexEventHandler(fileBrowser_DisconnectEnd);
        }

        private void fileBrowser_ConnectEnd(object sender, ObexEventArgs e)
        {
            // there has been no errors (yet!)
            m_iUnrecoverableError = 0;

            // disable the deviceList while we're connected
            deviceList.Enabled = false;

            // we can attempt to disconnect
            disconnectBtn.Enabled = true;

            // can't scan when we're connected
            discoverBtn.Enabled = false;

            // change to tab page with files
            tabControl.SelectedIndex = 1;

            fileBrowser.DisconnectEnd += new ObexEventHandler(fileBrowser_DisconnectEnd);
        }

        private void fileList_SelectedIndexChanged(object sender, System.EventArgs e)
        {
            // if we don't allow selection of folders
            if (!allowSelectCheckBox.Checked)
            {
                // enable download button when we have a selected file
                downloadBtn.Enabled = fileList.SelectedIndex != 0;
                // enable delete button when we have a selected file
                deleteBtn.Enabled = fileList.SelectedIndex != 0;

                // if we click on the top, it's .. (back one step)
                if (fileList.SelectedIndex == 0)
                {
                    fileList.Enabled = false;

                    // tell file browser to step back one directory
                    fileBrowser.SetPathAsync("..");
                }
                // if we click on any other
                else if (fileList.SelectedIndex > 0)
                {
                    // get FileFolder object from cache
                    FileFolder ff = fileBrowser.Items[fileList.SelectedIndex - 1];

                    // check if it's a folder
                    if (ff != null && ff.IsFolder)
                    {
                        fileList.Enabled = false;

                        fileBrowser.SetPathAsync(ff.Name);
                    }
                }
            }
        }

        private void uploadBtn_Click(object sender, System.EventArgs e)
        {
            if (uploadBtn.Text.Equals("Abort"))
            {
                fileBrowser.Abort();
            }
            else
            {
                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    uploadBtn.Text = "Abort";

                    FileStream fs = new FileStream(openFileDialog.FileName, FileMode.Open, FileAccess.Read);
                    fileBrowser.PutFileAsync(fs, Path.GetFileName(openFileDialog.FileName));
                }
            }
        }

        private void deleteBtn_Click(object sender, System.EventArgs e)
        {
            // if we click on any other than the first
            if (fileList.SelectedIndex > 0)
            {
                // get the selected file from the cache
                string filename = fileBrowser.Items[fileList.SelectedIndex - 1].Name;

                fileBrowser.DeleteAsync(filename);

                fileList.Enabled = false;
                uploadBtn.Enabled = false;
                downloadBtn.Enabled = false;
                deleteBtn.Enabled = false;
                createBtn.Enabled = false;
            }
        }

        private void fileBrowser_DeleteEnd(object sender, ObexEventArgs e)
        {
            // update file list box
            fileBrowser.GetPathAsync();
        }

        private void fileBrowser_Error(object sender, ObexEventArgs e)
        {
            ObexErrorEventArgs errorArgs = (ObexErrorEventArgs)e;

            if (errorArgs.Error == Franson.Protocols.Obex.ErrorCode.StreamError)
            {
                m_iUnrecoverableError++;
            }

            if (errorArgs.Error == Franson.Protocols.Obex.ErrorCode.Aborted)
            {
                MessageBox.Show("Copy was aborted");
            }
            else if (errorArgs.Error == Franson.Protocols.Obex.ErrorCode.SecurityAbort)
            {
                MessageBox.Show("Copy was aborted for security reasons");

                // this is done since SecurityAbort expects a disconnect
                // but this FTP client wants to stay connected, so we disconnect and connect again
                // if you want to do this really nice, you'll keep track of the path too, and restore it
                // as well. this solution does put us back to root folder (the inbox)
                fileBrowser.DisconnectEnd -= new ObexEventHandler(fileBrowser_DisconnectEnd);
                fileBrowser.DisconnectAsync();
                fileBrowser.ConnectAsync(m_streamCurrent);
            }
            else
            {
                MessageBox.Show(errorArgs.Message);
            }

            if (m_iUnrecoverableError == 1)
            {
                DisableUI();
            }
        }

        private void createBtn_Click(object sender, System.EventArgs e)
        {
            fileList.Enabled = false;
            fileBrowser.CreateDirectoryAsync("Test directory");
        }

        private void fileBrowser_CreateDirectoryEnd(object sender, ObexEventArgs e)
        {
            if (m_iUnrecoverableError == 0)
            {
                // when new directory is created, update file list box
                fileBrowser.GetPathAsync();
            }
        }

        private void fileBrowser_SetPathEnd(object sender, ObexEventArgs e)
        {
            if (m_iUnrecoverableError == 0)
            {
                fileBrowser.GetPathAsync();
            }
        }

        private void helpBtn_Click(object sender, System.EventArgs e)
        {
            // show help how to use FTP
            MessageBox.Show("Click on Upload or Download to transfer the selected file. (Only works on files.)\n\n" +
            "Click on Create dir to create a new sub-directory.\n\nClick Delete to delete the selected file or folder.\n\n" +
            "To Browse the FTP, click on an item marked \"Folder:\". A click on \"..\" steps back to the parent directory." +
            "To delete a folder, click \"Allow folder select\" to avoid stepping into it.");
        }

        private void connectBtn_Click(object sender, System.EventArgs e)
        {
            RemoteDevice device = (RemoteDevice)deviceList.SelectedItem;

            device.ServiceDiscoveryStarted += new BlueToolsEventHandler(device_ServiceDiscoveryStarted);
            device.ServiceDiscoveryCompleted += new BlueToolsEventHandler(device_ServiceDiscoveryCompleted);
            device.DiscoverServicesAsync(ServiceType.OBEXFileTransfer);

            discoverBtn.Enabled = false;
            connectBtn.Enabled = false;
            deviceList.Enabled = false;
        }

        private void deviceHelpBtn_Click(object sender, System.EventArgs e)
        {
            // show short manual how to use this client
            MessageBox.Show("Click on Scan to detect nearby Bluetooth devices.\n\nSelect a device from the list and Click on Connect to connect to the selected device.\n\nClick on disconnect to disconnect from the remote device.");
        }

        private void device_ServiceDiscoveryStarted(object sender, BlueToolsEventArgs eventArgs)
        {
            // disable the connect button - we are trying to connect already!
            connectBtn.Enabled = false;

            // remote event handler
            ((RemoteDevice)sender).ServiceDiscoveryStarted -= new BlueToolsEventHandler(device_ServiceDiscoveryStarted);
        }

        /// <summary>
        /// This method disables the UI when there is an unrecoverable error or user disconnects.
        /// </summary>
        private void DisableUI()
        {
            // disable buttons
            disconnectBtn.Enabled = false;
            uploadBtn.Enabled = false;
            downloadBtn.Enabled = false;
            deleteBtn.Enabled = false;
            createBtn.Enabled = false;

            // re-enable scan (if it was scan button was disabled)
            discoverBtn.Enabled = true;

            // clear controls since we can't access the contents anymore
            fileList.Items.Clear();

            // enable device list again - we must able to pick a new device if we want to
            deviceList.Enabled = true;

            // change to device tab page
            tabControl.SelectedIndex = 0;
        }

        private void MainForm_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if ((fileBrowser != null && fileBrowser.Connected))
            {
                fileBrowser.DisconnectEnd -= new ObexEventHandler(fileBrowser_DisconnectEnd);
                fileBrowser.DisconnectEnd += new ObexEventHandler(fileBrowser_ApplicationExit);

                fileBrowser.DisconnectAsync();

                e.Cancel = true;
            }
            else
            {
                if (m_network != null)
                {
                    // cancel device discovery if we're closing down
                    m_network.CancelDeviceDiscovery();
                }

                // dispose of (among other things) Bluetools Manager - must be done!		
                Dispose();
            }
        }

        private void fileBrowser_ApplicationExit(object sender, ObexEventArgs args)
        {
            m_network.CancelDeviceDiscovery();

            // dispose of (among other things) Bluetools Manager - must be done!
            Dispose();

            // won't call mainform_closing so no risk for dead-lock
            Application.Exit();
        }
    }
}
