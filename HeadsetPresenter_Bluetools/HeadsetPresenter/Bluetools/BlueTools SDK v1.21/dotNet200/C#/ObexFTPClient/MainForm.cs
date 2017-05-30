using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.Data;
using Franson.BlueTools;
using System.IO;
using System.Text;

using Franson.Protocols.Obex;
using Franson.Protocols.Obex.FTPClient;
using Franson.Protocols.Obex.ObjectPush;

namespace ObexFTPClientSample200
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	/// 	
	public class MainForm : System.Windows.Forms.Form
	{

		private System.Windows.Forms.OpenFileDialog openFileDialog;	
		private System.Windows.Forms.ImageList imageList;
		private System.ComponentModel.IContainer components;
		private System.Windows.Forms.ToolBarButton discoverBtn;
		private System.Windows.Forms.StatusBar statusBar;
		private System.Windows.Forms.StatusBarPanel statusBarPanel;
		private System.Windows.Forms.ToolBar toolBar;
		private System.Windows.Forms.ProgressBar progressBar;
		private System.Windows.Forms.ToolBarButton abortBtn;
		private System.Windows.Forms.StatusBarPanel stackPanel;
		private System.Windows.Forms.ToolBarButton disconnectBtn;
		private System.Windows.Forms.ToolBarButton delBtn;
        private System.Windows.Forms.ToolBarButton refreshBtn;
		private System.Windows.Forms.SaveFileDialog saveFileDialog;
		private System.Windows.Forms.ToolBarButton createDirBtn;

		// - User defined variables -

		// Application
		private AddressBook m_addressbook = null;
		private FileBrowser fb = null;

		// BlueTools
		private Manager			m_manager		 = null;
		private Network			m_network		 = null;
		private RemoteService	m_serviceCurrent = null;
		private Stream			m_streamCurrent	 = null;
		private RemoteDevice	m_deviceCurrent	 = null;

		// stores how many unrecoverable error has occurred
		private int m_iUnrecoverableError = 0;
		private System.Windows.Forms.ToolBarButton getBtn;
		private System.Windows.Forms.ImageList toolbarImageList;
		private System.Windows.Forms.ToolBarButton putBtn;

		// stores if the last transfer was denied
		private bool m_boolDenied = false;
		private System.Windows.Forms.TreeView folderTree;
		private System.Windows.Forms.ListView fileListView;
		private System.Windows.Forms.ColumnHeader nameHeader;
		private System.Windows.Forms.ColumnHeader sizeHeader;
		private System.Windows.Forms.ColumnHeader modifiedHeader;
		private System.Windows.Forms.Panel panel;
		// stores if the last transfer was aborted
		private bool m_boolAborted = false;

		public MainForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			// TODO: remove these
			AppDomain currentDomain = AppDomain.CurrentDomain;
			currentDomain.UnhandledException += new UnhandledExceptionEventHandler(currentDomain_UnhandledException);

			try
			{
				m_manager	= Manager.GetManager();

				// show which stack application is using
				switch (Manager.StackID)
				{
					case StackID.STACK_MICROSOFT:
					{
						stackPanel.Text = "Microsoft Bluetooth Stack";
						break;
					}

					case StackID.STACK_WIDCOMM:
					{
						stackPanel.Text = "WidComm Bluetooth Stack";
						break;
					}
				}

				// marshal events in this class thread
				m_manager.Parent = this;

                // get your trial license or buy BlueTools at franson.com
				Franson.BlueTools.License license = new Franson.BlueTools.License();
                license.LicenseKey = "WoK6IL448AACKORQWXWZTjvRHRfuLYDXWsK6";
				
				// Get first network (BlueTools 1.0 only supports one network == one dongle)
				m_network = m_manager.Networks[0];

				// Add events for device discovery
				m_network.DeviceDiscoveryStarted	+= new BlueToolsEventHandler(network_DeviceDiscoveryStarted);
				m_network.DeviceDiscoveryCompleted	+= new BlueToolsEventHandler(network_DeviceDiscoveryCompleted);

				// load address book
				m_addressbook = new AddressBook();
				m_addressbook.Load();

				// create FileBrowser object
				fb = new FileBrowser();

				// marshal events to this class thread.
				// this is necessary if you for example (like this sample does) want to display the progress
				// of a copy operation with a progress bar. without this marshalling you will end up calling
				// the event method from an internal FileBrowser thread. This can cause a lot of problems
				// with synchronization and deadlocks. By passing this class thread to FileBrowser it will
				// invoke the events in this thread and thus not cause the problems stated above.
				// If you want to know more read up on Invoke and BeginInvoke.
				fb.Parent = this;

				// set event handlers for PutFile
				fb.PutFileBegin		+= new ObexEventHandler(fb_CopyBegin);
				fb.PutFileEnd		+= new ObexEventHandler(fb_CopyEnd);
				fb.PutFileProgress	+= new ObexEventHandler(fb_CopyProgress);
				fb.GetPathEnd		+= new ObexEventHandler(fb_GetPathEnd);
				fb.SetPathEnd		+= new ObexEventHandler(fb_SetPathEnd);

				// set event handlers for GetFile
				// note that they are set the same as PutFile. 
				// They look pretty much the same and should display pretty much the same
				// so we re-use them!
				fb.GetFileBegin		+= new ObexEventHandler(fb_CopyBegin);
				fb.GetFileEnd		+= new ObexEventHandler(fb_CopyEnd);
				fb.GetFileProgress	+= new ObexEventHandler(fb_CopyProgress);
				fb.CreateDirectoryEnd += new ObexEventHandler(fb_CreateDirectoryEnd);

				// set event handler for possible errors 
				// asynchronous errors can't be trapped with try..catch
				// they are forwarded to this function instead
				fb.Error			+= new ObexEventHandler(fb_Error);

				// set event handler to be notified when ConnectAsync has finished
				fb.ConnectEnd		+= new ObexEventHandler(fb_ConnectEnd);

				fb.DisconnectEnd += new ObexEventHandler(fb_DisconnectEnd);

				ListDevices();
			} 
			catch (Exception exc)
			{
				MessageBox.Show(exc.Message);
			}
		}
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose(bool disposing)
		{
			if (disposing)
			{
                Manager.GetManager().Dispose();

				if (components != null) 
				{
					// force dispose of components
					components.Dispose();
				}
			}
				
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.imageList = new System.Windows.Forms.ImageList(this.components);
            this.toolBar = new System.Windows.Forms.ToolBar();
            this.discoverBtn = new System.Windows.Forms.ToolBarButton();
            this.disconnectBtn = new System.Windows.Forms.ToolBarButton();
            this.createDirBtn = new System.Windows.Forms.ToolBarButton();
            this.delBtn = new System.Windows.Forms.ToolBarButton();
            this.refreshBtn = new System.Windows.Forms.ToolBarButton();
            this.putBtn = new System.Windows.Forms.ToolBarButton();
            this.getBtn = new System.Windows.Forms.ToolBarButton();
            this.abortBtn = new System.Windows.Forms.ToolBarButton();
            this.toolbarImageList = new System.Windows.Forms.ImageList(this.components);
            this.statusBar = new System.Windows.Forms.StatusBar();
            this.statusBarPanel = new System.Windows.Forms.StatusBarPanel();
            this.stackPanel = new System.Windows.Forms.StatusBarPanel();
            this.progressBar = new System.Windows.Forms.ProgressBar();
            this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.folderTree = new System.Windows.Forms.TreeView();
            this.fileListView = new System.Windows.Forms.ListView();
            this.nameHeader = new System.Windows.Forms.ColumnHeader();
            this.sizeHeader = new System.Windows.Forms.ColumnHeader();
            this.modifiedHeader = new System.Windows.Forms.ColumnHeader();
            this.panel = new System.Windows.Forms.Panel();
            ((System.ComponentModel.ISupportInitialize)(this.statusBarPanel)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.stackPanel)).BeginInit();
            this.panel.SuspendLayout();
            this.SuspendLayout();
            // 
            // openFileDialog
            // 
            this.openFileDialog.AddExtension = false;
            this.openFileDialog.CheckPathExists = false;
            this.openFileDialog.Filter = "All files|*.*";
            this.openFileDialog.Multiselect = true;
            this.openFileDialog.Title = "Select file to send";
            // 
            // imageList
            // 
            this.imageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList.ImageStream")));
            this.imageList.TransparentColor = System.Drawing.Color.Black;
            this.imageList.Images.SetKeyName(0, "");
            this.imageList.Images.SetKeyName(1, "");
            this.imageList.Images.SetKeyName(2, "");
            // 
            // toolBar
            // 
            this.toolBar.Appearance = System.Windows.Forms.ToolBarAppearance.Flat;
            this.toolBar.AutoSize = false;
            this.toolBar.Buttons.AddRange(new System.Windows.Forms.ToolBarButton[] {
            this.discoverBtn,
            this.disconnectBtn,
            this.createDirBtn,
            this.delBtn,
            this.refreshBtn,
            this.putBtn,
            this.getBtn,
            this.abortBtn});
            this.toolBar.ButtonSize = new System.Drawing.Size(32, 32);
            this.toolBar.DropDownArrows = true;
            this.toolBar.ImageList = this.toolbarImageList;
            this.toolBar.Location = new System.Drawing.Point(0, 0);
            this.toolBar.Name = "toolBar";
            this.toolBar.ShowToolTips = true;
            this.toolBar.Size = new System.Drawing.Size(712, 48);
            this.toolBar.TabIndex = 31;
            this.toolBar.ButtonClick += new System.Windows.Forms.ToolBarButtonClickEventHandler(this.toolBar_ButtonClick);
            // 
            // discoverBtn
            // 
            this.discoverBtn.ImageIndex = 1;
            this.discoverBtn.Name = "discoverBtn";
            this.discoverBtn.ToolTipText = "Button scans for devices - menu connects you to found devices.";
            // 
            // disconnectBtn
            // 
            this.disconnectBtn.Enabled = false;
            this.disconnectBtn.ImageIndex = 2;
            this.disconnectBtn.Name = "disconnectBtn";
            this.disconnectBtn.ToolTipText = "Disconnect from remote device.";
            // 
            // createDirBtn
            // 
            this.createDirBtn.Enabled = false;
            this.createDirBtn.ImageIndex = 7;
            this.createDirBtn.Name = "createDirBtn";
            this.createDirBtn.ToolTipText = "Create new folder";
            // 
            // delBtn
            // 
            this.delBtn.Enabled = false;
            this.delBtn.ImageIndex = 5;
            this.delBtn.Name = "delBtn";
            this.delBtn.ToolTipText = "Delete selected file.";
            // 
            // refreshBtn
            // 
            this.refreshBtn.Enabled = false;
            this.refreshBtn.ImageIndex = 6;
            this.refreshBtn.Name = "refreshBtn";
            // 
            // putBtn
            // 
            this.putBtn.Enabled = false;
            this.putBtn.ImageIndex = 4;
            this.putBtn.Name = "putBtn";
            this.putBtn.ToolTipText = "Upload selected file(s)";
            // 
            // getBtn
            // 
            this.getBtn.Enabled = false;
            this.getBtn.ImageIndex = 8;
            this.getBtn.Name = "getBtn";
            this.getBtn.ToolTipText = "Download selected file(s)";
            // 
            // abortBtn
            // 
            this.abortBtn.Enabled = false;
            this.abortBtn.ImageIndex = 3;
            this.abortBtn.Name = "abortBtn";
            this.abortBtn.ToolTipText = "Abort operation";
            // 
            // toolbarImageList
            // 
            this.toolbarImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("toolbarImageList.ImageStream")));
            this.toolbarImageList.TransparentColor = System.Drawing.Color.Transparent;
            this.toolbarImageList.Images.SetKeyName(0, "");
            this.toolbarImageList.Images.SetKeyName(1, "");
            this.toolbarImageList.Images.SetKeyName(2, "");
            this.toolbarImageList.Images.SetKeyName(3, "");
            this.toolbarImageList.Images.SetKeyName(4, "");
            this.toolbarImageList.Images.SetKeyName(5, "");
            this.toolbarImageList.Images.SetKeyName(6, "");
            this.toolbarImageList.Images.SetKeyName(7, "");
            this.toolbarImageList.Images.SetKeyName(8, "");
            // 
            // statusBar
            // 
            this.statusBar.Location = new System.Drawing.Point(0, 391);
            this.statusBar.Name = "statusBar";
            this.statusBar.Panels.AddRange(new System.Windows.Forms.StatusBarPanel[] {
            this.statusBarPanel,
            this.stackPanel});
            this.statusBar.ShowPanels = true;
            this.statusBar.Size = new System.Drawing.Size(712, 22);
            this.statusBar.TabIndex = 32;
            this.statusBar.Text = "asaf";
            // 
            // statusBarPanel
            // 
            this.statusBarPanel.AutoSize = System.Windows.Forms.StatusBarPanelAutoSize.Spring;
            this.statusBarPanel.Name = "statusBarPanel";
            this.statusBarPanel.Width = 605;
            // 
            // stackPanel
            // 
            this.stackPanel.AutoSize = System.Windows.Forms.StatusBarPanelAutoSize.Contents;
            this.stackPanel.Name = "stackPanel";
            this.stackPanel.Text = "Unknown stack";
            this.stackPanel.Width = 91;
            // 
            // progressBar
            // 
            this.progressBar.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.progressBar.Location = new System.Drawing.Point(0, 383);
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(712, 8);
            this.progressBar.TabIndex = 42;
            this.progressBar.Visible = false;
            // 
            // saveFileDialog
            // 
            this.saveFileDialog.Title = "Select where to store file";
            // 
            // folderTree
            // 
            this.folderTree.Dock = System.Windows.Forms.DockStyle.Left;
            this.folderTree.FullRowSelect = true;
            this.folderTree.HideSelection = false;
            this.folderTree.ImageIndex = 1;
            this.folderTree.ImageList = this.imageList;
            this.folderTree.Location = new System.Drawing.Point(0, 0);
            this.folderTree.Name = "folderTree";
            this.folderTree.SelectedImageIndex = 1;
            this.folderTree.Size = new System.Drawing.Size(144, 343);
            this.folderTree.TabIndex = 47;
            this.folderTree.DoubleClick += new System.EventHandler(this.folderTree_DoubleClick);
            this.folderTree.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.folderTree_AfterSelect);
            // 
            // fileListView
            // 
            this.fileListView.Activation = System.Windows.Forms.ItemActivation.OneClick;
            this.fileListView.AllowDrop = true;
            this.fileListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.nameHeader,
            this.sizeHeader,
            this.modifiedHeader});
            this.fileListView.Dock = System.Windows.Forms.DockStyle.Right;
            this.fileListView.Location = new System.Drawing.Point(144, 0);
            this.fileListView.Name = "fileListView";
            this.fileListView.Size = new System.Drawing.Size(568, 343);
            this.fileListView.SmallImageList = this.imageList;
            this.fileListView.TabIndex = 36;
            this.fileListView.UseCompatibleStateImageBehavior = false;
            this.fileListView.View = System.Windows.Forms.View.Details;
            this.fileListView.ItemActivate += new System.EventHandler(this.fileListView_ItemActivate);
            this.fileListView.DragDrop += new System.Windows.Forms.DragEventHandler(this.fileListView_DragDrop);
            this.fileListView.DoubleClick += new System.EventHandler(this.fileListView_DoubleClick);
            this.fileListView.DragOver += new System.Windows.Forms.DragEventHandler(this.fileListView_DragOver);
            this.fileListView.MouseUp += new System.Windows.Forms.MouseEventHandler(this.fileListView_MouseUp);
            this.fileListView.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.fileListView_KeyPress);
            // 
            // nameHeader
            // 
            this.nameHeader.Text = "Name";
            this.nameHeader.Width = 150;
            // 
            // sizeHeader
            // 
            this.sizeHeader.Text = "Size";
            this.sizeHeader.Width = 80;
            // 
            // modifiedHeader
            // 
            this.modifiedHeader.Text = "Date modified";
            this.modifiedHeader.Width = 120;
            // 
            // panel
            // 
            this.panel.Controls.Add(this.folderTree);
            this.panel.Controls.Add(this.fileListView);
            this.panel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel.Location = new System.Drawing.Point(0, 48);
            this.panel.Name = "panel";
            this.panel.Size = new System.Drawing.Size(712, 343);
            this.panel.TabIndex = 33;
            // 
            // MainForm
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(712, 413);
            this.Controls.Add(this.progressBar);
            this.Controls.Add(this.panel);
            this.Controls.Add(this.statusBar);
            this.Controls.Add(this.toolBar);
            this.Name = "MainForm";
            this.Text = "Obex FTP Client Sample";
            this.Closing += new System.ComponentModel.CancelEventHandler(this.MainForm_Closing);
            ((System.ComponentModel.ISupportInitialize)(this.statusBarPanel)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.stackPanel)).EndInit();
            this.panel.ResumeLayout(false);
            this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>

		private void network_DeviceDiscoveryStarted(object sender, BlueToolsEventArgs eventArgs)
		{
			// Search for devices on the network started

			statusBarPanel.Text = "Searching for bluetooth devices...";

			// disable folderTree. You can't attempt to connect to anything now
			folderTree.Enabled = false;
		}

		private void network_DeviceDiscoveryCompleted(object sender, BlueToolsEventArgs eventArgs)
		{
            // add all found devices to addressbook
            foreach (Device device in m_network.Devices)
            {
                // add device to AddressBook
                m_addressbook.Add(device.Address, device.Name);
            }

            // disable discover button
            discoverBtn.Enabled = false;

			// list all found and stored devices
			ListDevices();

			// Search for devices on the network ended.
			statusBarPanel.Text = m_network.Devices.Length + " device(s) in your vicinity.";
			discoverBtn.Enabled = true;
			
			// re-enable the folder tree
			folderTree.Enabled = true;
		}

		private void device_ServiceDiscoveryCompleted(object sender, BlueToolsEventArgs eventArgs)
		{
			try
			{
				// reset error status - there has been no errors on this session
				m_iUnrecoverableError = 0;

				// if we have no streams or services already connected
				if (m_streamCurrent == null)
				{
					// Get the remote device that raised the event
					m_deviceCurrent = (RemoteDevice)sender;

					// You find an array of all found services here - the list is complete since this is ServiceDiscoverCompleted
					Service[] services = (Service[])((DiscoveryEventArgs)eventArgs).Discovery;

					// if services.Length is above 0 it means we found an Obex FTP service
					if (services.Length > 0)
					{
						// store current service
						m_serviceCurrent = (RemoteService)services[0];
						
						// get the stream to the Obex FTP service
						m_streamCurrent  = m_serviceCurrent.Stream;

						if (m_streamCurrent != null)
						{					
							// attempt to connect asynchronously to Obex FTP server
							fb.ConnectAsync(m_streamCurrent);
						}					
					}
					else
					{
                        // enable discover button if no appropriate service was found
                        discoverBtn.Enabled = true;

						// if this device did not support OBEX FTP display message in status bar
						statusBarPanel.Text = "No service for Obex FTP available on device.";

						// Update device list
						ListDevices();
					}
				}

				// remove event handlers
				m_deviceCurrent.ServiceDiscoveryCompleted -= new BlueToolsEventHandler(device_ServiceDiscoveryCompleted);
				m_deviceCurrent.Error					  -= new Franson.BlueTools.BlueToolsEventHandler(device_Error);
			}
			catch (Exception exc)
			{
				m_serviceCurrent = null;
				m_streamCurrent  = null;
				m_deviceCurrent	   = null;

				// display error message in window
				statusBarPanel.Text = exc.Message;

				// go back to showing devices
				ListDevices();
			}
		}

		private void device_Error(object sender, BlueToolsEventArgs eventArgs)
		{			
			Franson.BlueTools.ErrorEventArgs errorEventArgs = (Franson.BlueTools.ErrorEventArgs)eventArgs;

			MessageBox.Show(errorEventArgs.ErrorCode + ": " + errorEventArgs.Message);
		}

		private void MainForm_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			if (fb != null && fb.Connected)
			{
				fb.DisconnectEnd -= new ObexEventHandler(fb_DisconnectEnd);
                fb.DisconnectEnd += new ObexEventHandler(fb_ApplicationExit);

                fb.DisconnectAsync();

                e.Cancel = true;

                // don't continue to execute this method
				return;
			}

            if (m_addressbook != null)
            {
                m_addressbook.Close();
            }

			// cancel any pending device discovery operations
			if (m_network != null)
			{
				m_network.CancelDeviceDiscovery();
			}
		}

        private void fb_ApplicationExit(object sender, ObexEventArgs e)
        {
   			m_addressbook.Close();

			// cancel any pending device discovery operations
			if (m_network != null)
			{
				m_network.CancelDeviceDiscovery();
			}

            Dispose();

            // won't call mainform_closing so no risk for dead-lock
            Application.Exit();
        }

		void fb_CopyBegin(Object source, ObexEventArgs args)
		{
			// this transfer hasn't been denied (yet!)
			m_boolDenied  = false;
			// this transfer hasn't been aborted (yet!)
			m_boolAborted = false;

			progressBar.Visible = true;
			ObexCopyEventArgs copyArgs = (ObexCopyEventArgs)args;

            // must see if Size is -1, that means it is unknown
            progressBar.Maximum = (copyArgs.Size != -1) ? (int)copyArgs.Size : 0;
            abortBtn.Enabled = true;
            refreshBtn.Enabled = false;
            createDirBtn.Enabled = false;
            putBtn.Enabled = false;
            getBtn.Enabled = false;
            delBtn.Enabled = false;
            progressBar.Value = 0;
		}

		/// <summary>
		/// This method is called while a copy operation is commencing. It updates the statusbar to show the byte flow.
		/// </summary>
		/// <param name="source"></param>
		/// <param name="args"></param>
		private void fb_CopyProgress(Object source, ObexEventArgs args)
		{
			ObexCopyEventArgs copyArgs = (ObexCopyEventArgs)args;

			// if size is unknown, ObexCopyEventArgs.Size will return -1
			if (copyArgs.Size != -1)
			{
				if (copyArgs.Size != progressBar.Maximum)
				{
					progressBar.Maximum = (int)copyArgs.Size;	
				}		

				progressBar.Value   = (int)copyArgs.Position;				

				statusBarPanel.Text = Convert.ToString(copyArgs.Position) + "/" + Convert.ToString(copyArgs.Size);
			}
			else
			{
				// show user that we are copying but we don't know the full size
				statusBarPanel.Text = Convert.ToString(copyArgs.Position) + "/ ?";
			}
		}

		/// <summary>
		/// This method is called when a copy operation is finished.
		/// </summary>
		/// <param name="source"></param>
		/// <param name="args"></param>
		void fb_CopyEnd(Object source, ObexEventArgs args)
		{
			ObexCopyEventArgs copyArgs = (ObexCopyEventArgs)args;

			// check if Stream is available before closing
			if (copyArgs.Stream != null)
			{
				copyArgs.Stream.Close();
			}

			progressBar.Visible = false;

			if (args.Event == Franson.Protocols.Obex.EventType.PutFileEnd || 
				args.Event == Franson.Protocols.Obex.EventType.GetFileEnd)
			{
				// if sending was denied
				if (m_boolDenied)
				{
					statusBarPanel.Text = "File transfer denied by device.";
				}
                // ..if sending was aborted
				else if (m_boolAborted)
				{
					statusBarPanel.Text = "Copy was aborted.";
				}
                // ...else if there has been no unrecoverable errors
                else if (m_iUnrecoverableError == 0)
                {
                    statusBarPanel.Text = "Copy complete.";
                }
				// .. or if there has been unrecoverable errors
				else
				{
					statusBarPanel.Text = "There was an error with the copy and the connection was lost.";
				}
			}

            // Upload button should always be re-enabled (Download should only be re-enabled when a file is selected)
            putBtn.Enabled = true;
            delBtn.Enabled = true;

            // if this was a putfile operation, there is a new file on the remote device
            // if there was an unrecoverable error we can't access the device though...
            if ((args.Event == Franson.Protocols.Obex.EventType.PutFileEnd) && (m_iUnrecoverableError == 0))
            {
                abortBtn.Enabled = false;
                refreshBtn.Enabled = true;
                createDirBtn.Enabled = true;
                delBtn.Enabled = false;

                UpdateUI();
            }
		}

		/// <summary>
		/// This methos is called when user is double-clicking the TreeView. 
		/// If connected it changes the current folder on the remote device.
		/// If not connected, it connects you to the device you clicked on.
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void folderTree_DoubleClick(object sender, System.EventArgs e)
		{
			TreeNode node = folderTree.SelectedNode;

			if (fb.Connected && m_iUnrecoverableError == 0)
			{
				// this checks if there is a folder selected..
				if (node != null)
				{	
					// check so it is a node that we have ourselves added (with a FileFolder attached) that is being double-clicked
					if (node.Tag != null)
					{
						FileFolder ff = (FileFolder)node.Tag;
	
						fb.SetPathAsync(ff.Name);					
					}
					else
					{
						// All nods that do not have a FileFolder attached is treated as step-back
						fb.SetPathAsync("..");
					}
				}
			}
			else
			{
				if (node != null)
				{
                    // disable discover button
                    discoverBtn.Enabled = false;

					// get stored device
					ObexDevice oDevice = (ObexDevice)folderTree.SelectedNode.Tag;

					// empty treeview
					folderTree.Nodes.Clear();

					// set new device
					m_deviceCurrent = (RemoteDevice)m_network.ConnectDevice(oDevice.Address, oDevice.Name);

					// try to connec to Obex FTP
					FindServiceAndConnect();
				}
			}
		}

		/// <summary>
		/// This method updates the user interface. It calls GetPathAsync.
		/// </summary>
		private void UpdateUI()
		{
            // enable download & delete button if any file is selected
            getBtn.Enabled = fileListView.SelectedItems.Count > 0;
            delBtn.Enabled = fileListView.SelectedItems.Count > 0;

			// only call GetPath if there hasn't been an unrecoverable error
			if (m_iUnrecoverableError == 0)
			{
				fb.GetPathAsync();
			}
		}

		/// <summary>
		/// This method is called when a user is clicking any of the toolbar buttons.
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void toolBar_ButtonClick(object sender, System.Windows.Forms.ToolBarButtonClickEventArgs e)
		{
			// check which button was clicked and act appropriately...

			// discover button
			if (e.Button == discoverBtn)
			{
                // empty addressbook
                m_addressbook.Clear();

                // disable discover button
				discoverBtn.Enabled = false;

                // scan for devices
				m_network.DiscoverDevicesAsync();
			}
				// delete button
			else if (e.Button == delBtn)
			{
				// if a foldertree is selected
				if (folderTree.SelectedNode != null && folderTree.SelectedNode.Tag != null)
				{
					FileFolder ff = (FileFolder)folderTree.SelectedNode.Tag;

					fb.DeleteAsync(ff.Name);

					UpdateUI();
				}
				else if (fileListView.SelectedItems.Count > 0 && fileListView.SelectedItems[0].Tag != null)
				{
					FileFolder ff = (FileFolder)fileListView.SelectedItems[0].Tag;

					fb.DeleteAsync(ff.Name);

					UpdateUI();
				}
				else
				{
					MessageBox.Show("No file or folder selected.");
				}
			}
			else if (e.Button == putBtn)
			{
				if (openFileDialog.ShowDialog() == DialogResult.OK)
				{
					foreach (string filename in openFileDialog.FileNames)
					{
						try
						{
							// open a stream to the file so we can read the data from it
							FileStream fs = new FileStream(filename, FileMode.Open, FileAccess.Read);

							// store the file! (only the filename, not the path)
							fb.PutFileAsync(fs, Path.GetFileName(filename));
						}
						catch (Exception exc)
						{
							// show some error if we failed to open the FileStream
							MessageBox.Show(exc.Message);
						}
					}
				}
			}
			else if (e.Button == getBtn)
			{
				// if user wants to download files

				// check that there are selected files to download
				if (fileListView.SelectedItems.Count > 0)
				{
					// iterate all files
					foreach (ListViewItem lvItem in fileListView.SelectedItems)
					{
						// check that we have a FileFolder with info
						if (lvItem.Tag != null)
						{
							// get the FileFolder object
							FileFolder ff = (FileFolder)lvItem.Tag;

							// propose the same name
							saveFileDialog.FileName = ff.Name;
			
							// ask where to store the file
							if (saveFileDialog.ShowDialog() == DialogResult.OK)
							{
								// open a file stream to where the local store should be kept
								FileStream fs = new FileStream(saveFileDialog.FileName, FileMode.Create);

								// ask Obex to download it!
								fb.GetFileAsync(fs, ff.Name);
							}
						}
					}
				}
			}
			else if (e.Button == abortBtn)
			{
				fb.Abort();
			}
			else if (e.Button == refreshBtn)
			{
				UpdateUI();

				refreshBtn.Enabled = false;
			}
			else if (e.Button == disconnectBtn)
			{
				fb.DisconnectAsync();
			}
			else if (e.Button == createDirBtn)
			{
				InputForm input = new InputForm();
				if (input.ShowDialog(this) == DialogResult.OK)
				{
					fb.CreateDirectoryAsync(input.FolderName);
				}
			}
		}

		/// <summary>
		/// Takes the current device and attempts to find the Obex FTP Service
		/// </summary>
		private void FindServiceAndConnect()
		{
			if (m_deviceCurrent != null)
			{	
				try
				{
					// Add a DiscoveryListener so we get service discovery events
					m_deviceCurrent.ServiceDiscoveryCompleted += new BlueToolsEventHandler(device_ServiceDiscoveryCompleted);
					m_deviceCurrent.Error					   += new Franson.BlueTools.BlueToolsEventHandler(device_Error);
	
					// Cancel any on-going device discovery (not that there should be any going on)
					m_network.CancelDeviceDiscovery();
				
					// only find OBEX FTP Services - this is done in UI-thread so let's do it asynchronously
					m_deviceCurrent.DiscoverServicesAsync(ServiceType.OBEXFileTransfer);
	
					statusBarPanel.Text = "Trying to connect...";
				} 
				catch (Exception exc)
				{
                    // enable discover button if something happened
                    discoverBtn.Enabled = true;
					MessageBox.Show(exc.Message);
				}
			}		
		}

		/// <summary>
		/// This method is called when GetPathAsync is finished.
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void fb_GetPathEnd(object sender, ObexEventArgs e)
		{
			if (m_iUnrecoverableError == 0)
			{
				// when get path finished - clear controls
				folderTree.Nodes.Clear();
				fileListView.Items.Clear();

				// step through entire cache
				for (int i = 0; i < fb.Items.Count; i++)
				{
					// get FileFolder object at current index position
					FileFolder ff = fb.Items[i];
				
					// declare variables for nodes
					TreeNode	 TreeNode = null;
					ListViewItem ListNode = null;

					// if FileFolder is a folder
					if (ff.IsFolder)
					{
						// create a TreeView node
						TreeNode = new TreeNode(ff.Name, 0, 0);

						// store the FileFolder object if we want to access some of its properties in the UI
						TreeNode.Tag = ff;

						// add the object to the treeview
						folderTree.Nodes.Add(TreeNode);
					}
					else
					{
						// if FileFolder is not a folder it is by definition a file...
					
						// create a ListView item
						ListNode = new ListViewItem(ff.Name, 1);

						// add size and modification date to sub-columns (we could add other properties here too - like last access etc.)
						// if size is unknown it will return -1, and if modification date is unknown it will return DateTime.MinValue
						ListNode.SubItems.Add((ff.FileSize > 0)?(ff.FileSize > 1024)?Convert.ToString(ff.FileSize / 1024) + " kb":"1 kb":"0 kb");
						ListNode.SubItems.Add((ff.FileModified != DateTime.MinValue)?ff.FileModified.ToString():"n/a");

						// store the FileFolder object if we want to access some of its properties in the UI
						ListNode.Tag = ff;
					
						// add the item to the listview instead
						fileListView.Items.Add(ListNode);
					}						
				}

				// insert step back (..)
				folderTree.Nodes.Insert(0, new TreeNode("..", 0, 0));

                // disable download & delete button
                getBtn.Enabled = false;
                delBtn.Enabled = false;

				// enable the refresh button
				refreshBtn.Enabled = true;
			}
		}

		/// <summary>
		/// This method is called when ConnectAsync is finished.
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void fb_ConnectEnd(object sender, ObexEventArgs e)
		{
			// when connection is established

			// update UI to display any files and folders in the current directory
			// if there was a unrecoverable error there's no point in trying to talk to device though
			if (m_deviceCurrent != null && m_iUnrecoverableError == 0)
			{
				UpdateUI();
				statusBarPanel.Text = "Connected to " + ((Device)m_deviceCurrent).Name;

				// enable all buttons
				disconnectBtn.Enabled = true;
				putBtn.Enabled		  = true;
				delBtn.Enabled		  = true;
				refreshBtn.Enabled	  = true;
				createDirBtn.Enabled  = true;
			}
			else
			{
				statusBarPanel.Text = "Some error has occured.";

				ListDevices();
			}
		}
	
		private void currentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
		{
			MessageBox.Show("Unhandled exception. If you think this is due to the Franson.Protocols.Obex component " +
				"please report it, along with information on how to reproduce it.");
		}

		private void fb_Error(object sender, ObexEventArgs eventArgs)
		{
			ObexErrorEventArgs errorArgs = (ObexErrorEventArgs)eventArgs;
		
			switch (errorArgs.Error)
			{
				case Franson.Protocols.Obex.ErrorCode.StreamError:
				{
					// count one more unrecoverable error
					m_iUnrecoverableError++;

					// if there was an error we should clear this stream, it's not valid anymore
					if (m_streamCurrent != null)
					{
						m_streamCurrent.Close();
					}

					break;
				}
				case Franson.Protocols.Obex.ErrorCode.Forbidden:
				{
					MessageBox.Show(errorArgs.Message);

					break;
				}

				case Franson.Protocols.Obex.ErrorCode.NoConnection:
				{
					m_iUnrecoverableError++;
					MessageBox.Show("Could not establish a connection or the connection has been lost to the Obex device.");
					break;
				}
				case Franson.Protocols.Obex.ErrorCode.NotFound:
				{
					MessageBox.Show("File or folder not found.");
					break;
				}
				case Franson.Protocols.Obex.ErrorCode.SendDenied:
				{
					// this transfer was denied!
					m_boolDenied = true;

					break;
				}
                // this requires a disconnect, so we disconnect and connect again
                case Franson.Protocols.Obex.ErrorCode.SecurityAbort:
                {
                    m_boolAborted = true;

                    // this is done since SecurityAbort expects a disconnect
                    // but this FTP client wants to stay connected, so we disconnect and connect again
                    // if you want to do this really nice, you'll keep track of the path too, and restore it
                    // as well. this solution does put us back to root folder (the inbox)
                    fb.DisconnectEnd -= new ObexEventHandler(fb_DisconnectEnd);
                    fb.DisconnectAsync();
                    fb.ConnectAsync(m_streamCurrent);

                    break;
                }
				case Franson.Protocols.Obex.ErrorCode.Aborted:
				{
					m_boolAborted = true;
					break;
				}
				default:
				{
					MessageBox.Show(errorArgs.Message);
					break;
				}
			}
				
			// only need to inform user once
			if (m_iUnrecoverableError == 1)
			{
				MessageBox.Show(errorArgs.Message);

				// disable UI
				DisableUI();
		
				// restore devices in the tree
				ListDevices();

				m_serviceCurrent = null;
				m_streamCurrent  = null;
			}
		}

		private void fileListView_DoubleClick(object sender, System.EventArgs e)
		{
			if (fileListView.Items.Count > 0)
			{
				ListViewItem lvi = fileListView.SelectedItems[0];
				
				FileFolder ff = (FileFolder)lvi.Tag;

				if (ff != null)
				{
					saveFileDialog.FileName = ff.Name;

					if (saveFileDialog.ShowDialog() == DialogResult.OK)
					{
						FileStream fs = new FileStream(saveFileDialog.FileName, FileMode.Create, FileAccess.Write);

						fb.GetFileAsync(fs, ff.Name);
					}
				}
			}
		}

		private void fileListView_DragDrop(object sender, System.Windows.Forms.DragEventArgs e)
		{
			if (e.Data.GetDataPresent(DataFormats.FileDrop))
			{
				string[] filenames = (string[])e.Data.GetData(DataFormats.FileDrop);;

				foreach (string filename in filenames)
				{
					FileStream fs = new FileStream(filename, FileMode.Open, FileAccess.Read);

					fb.PutFileAsync(fs, Path.GetFileName(filename));
				}
			}
		}

		private void fileListView_DragOver(object sender, System.Windows.Forms.DragEventArgs e)
		{
			e.Effect = DragDropEffects.Copy;
		}

		private void fb_SetPathEnd(object sender, ObexEventArgs e)
		{
			// called when setpath is finished
			UpdateUI();
		}

		private void fb_CreateDirectoryEnd(object sender, ObexEventArgs e)
		{
			UpdateUI();
		}

		/// <summary>
		/// This method is called when a disconnect has finished as a result of the user clicking on the Disconnect button.
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void fb_DisconnectEnd(object sender, ObexEventArgs e)
		{
			// disable user interface
			DisableUI();

			// update status bar
			statusBarPanel.Text = "Disconnected.";
			
			// close stream if open
			try
			{
				if (m_serviceCurrent != null && m_streamCurrent != null)
					m_streamCurrent.Close();
			}
			finally
			{
				m_serviceCurrent = null;
				m_streamCurrent  = null;
				m_deviceCurrent	   = null;						
			}

			ListDevices();

            // enable discover button so we can connect again
            discoverBtn.Enabled = true;
		}

		/// <summary>
		/// This method lists all devices found in addressbook
		/// </summary>
		private void ListDevices()
		{
			// empty tree
			folderTree.Nodes.Clear();

			// add all found in address book
			foreach (ObexDevice oDevice in m_addressbook.Devices)
			{
				// add nodes with device icon to treeview
				TreeNode deviceNode = new TreeNode(oDevice.Name, 2, 2);
				deviceNode.Tag = oDevice;
				folderTree.Nodes.Add(deviceNode);
			}
		}

		/// <summary>
		/// This method disables the UI when there is an error or user disconnects.
		/// </summary>
		private void DisableUI()
		{
			// disable buttons
			disconnectBtn.Enabled = false;
			putBtn.Enabled		  = false;
			getBtn.Enabled		  = false;
			delBtn.Enabled		  = false;
			refreshBtn.Enabled	  = false;
			createDirBtn.Enabled  = false;
			abortBtn.Enabled	  = false;
			
			// clear controls since we can't access the contents anymore
			folderTree.Nodes.Clear();
			fileListView.Items.Clear();
		}

		private void fileListView_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
		{
            // enable download & delete button if any file is selected by keyboard
            getBtn.Enabled = fileListView.SelectedItems.Count > 0;
            delBtn.Enabled = fileListView.SelectedItems.Count > 0;
		}

		private void fileListView_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
            // enable download & delete button if any file is selected by mouse
            getBtn.Enabled = fileListView.SelectedItems.Count > 0;
            delBtn.Enabled = fileListView.SelectedItems.Count > 0;
		}

        private void folderTree_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if (fb.Connected)
            {
                delBtn.Enabled = folderTree.SelectedNode != null && folderTree.SelectedNode.Tag != null;
            }
        }

        private void fileListView_ItemActivate(object sender, EventArgs e)
        {
            // enable download & delete button if files are selected
            getBtn.Enabled = fileListView.SelectedItems.Count > 0;
            delBtn.Enabled = fileListView.SelectedItems.Count > 0;
        }
	}
}
