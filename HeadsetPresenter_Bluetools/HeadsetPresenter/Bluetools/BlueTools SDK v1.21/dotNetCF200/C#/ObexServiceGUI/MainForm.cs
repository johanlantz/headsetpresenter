using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.Data;
using Franson.BlueTools;
using Franson.Protocols.Obex.OBEXService;

namespace ObexServiceSampleCF
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class MainForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.MainMenu mainMenu1;
		private System.ComponentModel.Container components = null;
		private System.Windows.Forms.ListBox listBoxCommands;
		private System.Windows.Forms.ListBox listBoxConnections;
		private System.Windows.Forms.RadioButton radioButtonFTP;
		private System.Windows.Forms.RadioButton radioButtonPush;
		private System.Windows.Forms.Button buttonStart;
		private System.Windows.Forms.Button buttonStop;
		private ServiceExtended m_extendedService = null;
		private Manager m_manager; 

		private string m_sRootFolderPath = @"\My Documents";
		private System.Windows.Forms.MenuItem menuItem1;
		private System.Windows.Forms.MenuItem menuItemExit;
		private System.Windows.Forms.MenuItem menuItem2;
		private System.Windows.Forms.MenuItem menuItemRootFolder;
		private System.Windows.Forms.Label labelStackID;
		private System.Windows.Forms.Label labelActivityLog;
		private System.Windows.Forms.Label labelConnectedDevices;
		
        private bool m_bIsRunning = false;
        private string m_currentStringItem;
        private RemoteDevice m_currentRemotedevice;

		public MainForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			this.Closing += new System.ComponentModel.CancelEventHandler(MainForm_Closing);

			m_manager = Manager.GetManager();

			switch(Manager.StackID)
			{
				case StackID.STACK_MICROSOFT:
					labelStackID.Text = "Microsoft Bluetooth Stack";
					break;
				case StackID.STACK_WIDCOMM:
					labelStackID.Text = "WidComm Bluetooth Stack";
					break;
			}
				
			Franson.BlueTools.License license = new Franson.BlueTools.License();
			license.LicenseKey = "HU5UZev162LOGX68oifhNlTRXvz0MRObHXEb";

		}

        private void OnConnect(ObexSession obexSession)
        {
            AddItemToListBoxCommandAsync(obexSession.RemoteDevice.Name + ": Connecting");
            AddItemToListBoxConnectionsAsync(obexSession.RemoteDevice);
        }

        private void OnDisconnect(ObexSession obexSession)
        {
            AddItemToListBoxCommandAsync(obexSession.RemoteDevice.Name + ": Disconnecting");
            RemoveItemToListBoxConnectionsAsync(obexSession.RemoteDevice);
        }

        private void OnAbort(ObexSession obexSession)
        {
            AddItemToListBoxCommandAsync(obexSession.RemoteDevice.Name + ": Aborting...");
        }

        private void OnCreateFolder(ObexSession obexSession, ObexFolder folder)
        {
            AddItemToListBoxCommandAsync(obexSession.RemoteDevice.Name + ": Create folder " + folder.Path);
        }

        private void OnDelete(ObexSession obexSession, ObexFolderItem folderItem)
        {
            AddItemToListBoxCommandAsync(obexSession.RemoteDevice.Name + ": Delete " + folderItem.Path);
        }

        private void OnGet(ObexSession obexSession, ObexFile obexFile)
        {
            AddItemToListBoxCommandAsync(obexSession.RemoteDevice.Name + ": Get file " + obexFile.Path);
        }

        private void OnPut(ObexSession obexSession, ObexFile obexFile)
        {
            AddItemToListBoxCommandAsync(obexSession.RemoteDevice.Name + ": Put file " + obexFile.Name + " To: " + obexFile.ShortPath);
        }

        private void OnSetPath(ObexSession obexSession, ObexFolder obexFolder)
        {
            AddItemToListBoxCommandAsync(obexSession.RemoteDevice.Name + ": Set path " + obexFolder.Path);
        }

        private void OnGetPath(ObexSession obexSession)
        {
            AddItemToListBoxCommandAsync(obexSession.RemoteDevice.Name + ": Get path " + obexSession.CurrentFolder.Path);
        }

        private void OnException(ObexSession obexSession, Exception exception)
        {
            if(listBoxConnections.Items.Contains(obexSession.RemoteDevice))
            {
                RemoveItemToListBoxConnectionsAsync(obexSession.RemoteDevice);
            }
        }

        #region Methods to handle form controls in a thread safe manner

        //Thread safe way to add Items to the listbox
        private void AddItemToListBoxCommandAsync(string item)
        {

            this.m_currentStringItem = item;
            Invoke(new System.EventHandler(AddItemToListBoxCommands));
        }

        //Thread safe way to add Items to the listbox
        private void AddItemToListBoxCommands(object sender, EventArgs eventArgs)
        {
            listBoxCommands.Items.Add(m_currentStringItem);
            listBoxCommands.SelectedIndex = listBoxCommands.Items.Count - 1;
        }

        //Thread safe way to add Items to the listbox
        private void AddItemToListBoxConnectionsAsync(RemoteDevice remoteDevice)
        {

            this.m_currentRemotedevice = remoteDevice;
            Invoke(new System.EventHandler(AddItemToListBoxConnections));
        }

        //Thread safe way to add Items to the listbox
        private void AddItemToListBoxConnections(object sender, EventArgs eventArgs)
        {
            listBoxConnections.Items.Add(m_currentRemotedevice);
        }

        //Thread safe way to add Items to the listbox
        private void RemoveItemToListBoxConnectionsAsync(RemoteDevice remoteDevice)
        {
            this.m_currentRemotedevice = remoteDevice;
            Invoke(new System.EventHandler(RemoveItemToListBoxConnections));
        }

        //Thread safe way to add Items to the listbox
        private void RemoveItemToListBoxConnections(object sender, EventArgs eventArgs)
        {
            listBoxConnections.Items.Remove(m_currentRemotedevice);
        }

        #endregion

		private void MainForm_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			m_extendedService.Stop();
			Dispose();
		}


		/// <summary>
		/// TODO. Proper shutdown Manager.GetManager().Dispose();
		/// </summary>
		protected override void Dispose( bool disposing )
		{
            if (m_bIsRunning)
            {
                m_extendedService.Stop();
                m_bIsRunning = false;
            }

			m_manager.Dispose();

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
			this.mainMenu1 = new System.Windows.Forms.MainMenu();
			this.menuItem1 = new System.Windows.Forms.MenuItem();
			this.menuItemExit = new System.Windows.Forms.MenuItem();
			this.menuItem2 = new System.Windows.Forms.MenuItem();
			this.menuItemRootFolder = new System.Windows.Forms.MenuItem();
			this.listBoxCommands = new System.Windows.Forms.ListBox();
			this.listBoxConnections = new System.Windows.Forms.ListBox();
			this.radioButtonFTP = new System.Windows.Forms.RadioButton();
			this.radioButtonPush = new System.Windows.Forms.RadioButton();
			this.buttonStart = new System.Windows.Forms.Button();
			this.buttonStop = new System.Windows.Forms.Button();
			this.labelStackID = new System.Windows.Forms.Label();
			this.labelActivityLog = new System.Windows.Forms.Label();
			this.labelConnectedDevices = new System.Windows.Forms.Label();
			// 
			// mainMenu1
			// 
			this.mainMenu1.MenuItems.Add(this.menuItem1);
			this.mainMenu1.MenuItems.Add(this.menuItem2);
			// 
			// menuItem1
			// 
			this.menuItem1.MenuItems.Add(this.menuItemExit);
			this.menuItem1.Text = "File";
			// 
			// menuItemExit
			// 
			this.menuItemExit.Text = "Exit";
			this.menuItemExit.Click += new System.EventHandler(this.menuItemExit_Click);
			// 
			// menuItem2
			// 
			this.menuItem2.MenuItems.Add(this.menuItemRootFolder);
			this.menuItem2.Text = "Settings";
			// 
			// menuItemRootFolder
			// 
			this.menuItemRootFolder.Text = "Root folder";
			this.menuItemRootFolder.Click += new System.EventHandler(this.menuItemRootFolder_Click);
			// 
			// listBoxCommands
			// 
			this.listBoxCommands.Location = new System.Drawing.Point(8, 104);
			this.listBoxCommands.Size = new System.Drawing.Size(224, 132);
			// 
			// listBoxConnections
			// 
			this.listBoxConnections.Location = new System.Drawing.Point(128, 16);
			this.listBoxConnections.Size = new System.Drawing.Size(104, 67);
			// 
			// radioButtonFTP
			// 
			this.radioButtonFTP.Checked = true;
			this.radioButtonFTP.Location = new System.Drawing.Point(16, 8);
			this.radioButtonFTP.Size = new System.Drawing.Size(96, 20);
			this.radioButtonFTP.Text = "Obex FTP";
			// 
			// radioButtonPush
			// 
			this.radioButtonPush.Location = new System.Drawing.Point(16, 32);
			this.radioButtonPush.Size = new System.Drawing.Size(96, 20);
			this.radioButtonPush.Text = "Object Push";
			// 
			// buttonStart
			// 
			this.buttonStart.Location = new System.Drawing.Point(8, 64);
			this.buttonStart.Size = new System.Drawing.Size(48, 20);
			this.buttonStart.Text = "Start";
			this.buttonStart.Click += new System.EventHandler(this.buttonStart_Click);
			// 
			// buttonStop
			// 
			this.buttonStop.Location = new System.Drawing.Point(64, 64);
			this.buttonStop.Size = new System.Drawing.Size(48, 20);
			this.buttonStop.Text = "Stop";
			this.buttonStop.Click += new System.EventHandler(this.buttonStop_Click);
			// 
			// labelStackID
			// 
			this.labelStackID.Location = new System.Drawing.Point(8, 248);
			this.labelStackID.Size = new System.Drawing.Size(224, 20);
			this.labelStackID.Text = "labelStackID";
			// 
			// labelActivityLog
			// 
			this.labelActivityLog.Location = new System.Drawing.Point(8, 88);
			this.labelActivityLog.Size = new System.Drawing.Size(100, 16);
			this.labelActivityLog.Text = "Activity Log";
			// 
			// labelConnectedDevices
			// 
			this.labelConnectedDevices.Location = new System.Drawing.Point(128, 0);
			this.labelConnectedDevices.Size = new System.Drawing.Size(100, 16);
			this.labelConnectedDevices.Text = "Connected devices";
			// 
			// MainForm
			// 
			this.Controls.Add(this.labelConnectedDevices);
			this.Controls.Add(this.labelActivityLog);
			this.Controls.Add(this.labelStackID);
			this.Controls.Add(this.buttonStop);
			this.Controls.Add(this.buttonStart);
			this.Controls.Add(this.radioButtonPush);
			this.Controls.Add(this.radioButtonFTP);
			this.Controls.Add(this.listBoxConnections);
			this.Controls.Add(this.listBoxCommands);
			this.Text = "MainForm";
			this.Load += new System.EventHandler(this.MainForm_Load);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>

		static void Main() 
		{
			Application.Run(new MainForm());
		}



		private void MainForm_Load(object sender, System.EventArgs e)
		{
		
		}

		private void buttonStart_Click(object sender, System.EventArgs e)
		{
			try
			{
				//Create serbvce root folder ObexFolder("Name", "Path");
				ObexFolder serviceRootFolder = new ObexFolder(m_sRootFolderPath , m_sRootFolderPath);
			
				//Instantiate the service. (Type, "Name", "Description", rootFolder)
				m_extendedService = new ServiceExtended(radioButtonFTP.Checked ? ObexServiceType.ObexFTP : ObexServiceType.ObexObjectPush, "Obex Service", "Sample Obex Service", serviceRootFolder);
			
				//Adds event from the Extended service
				m_extendedService.OnConnect += new ServiceExtended.ObexServiceEventHandler(OnConnect);
				m_extendedService.OnDisconnect += new ServiceExtended.ObexServiceEventHandler(OnDisconnect);
				m_extendedService.OnAbort += new ServiceExtended.ObexServiceEventHandler(OnAbort);
				m_extendedService.OnCreateFolder += new ServiceExtended.ObexServiceFolderOperationEventHandler(OnCreateFolder);
				m_extendedService.OnDelete += new ServiceExtended.ObexServiceFolderItemEventHandler(OnDelete);
				m_extendedService.OnGet += new ServiceExtended.ObexServiceFileTransferEventHandler(OnGet);
				m_extendedService.OnPut += new ServiceExtended.ObexServiceFileTransferEventHandler(OnPut);
				m_extendedService.OnSetPath += new ServiceExtended.ObexServiceFolderOperationEventHandler(OnSetPath);
				m_extendedService.OnGetPath += new  ServiceExtended.ObexServiceEventHandler(OnGetPath);
				m_extendedService.OnObexException += new ServiceExtended.ObexServiceExceptionEventHandler(OnException);
			
				//Start the service
				m_extendedService.Start();
				m_bIsRunning = true;

				
				buttonStop.Enabled = true;
				buttonStart.Enabled = false;
				radioButtonFTP.Enabled = false;
				radioButtonPush.Enabled = false;
			}
			catch(Exception ex)
			{
				throw ex;
			}
		}

		private void buttonStop_Click(object sender, System.EventArgs e)
		{
			m_extendedService.Stop();
			m_bIsRunning = false;


			buttonStop.Enabled = false;
			buttonStart.Enabled = true;
			
			radioButtonFTP.Enabled = true;
			radioButtonPush.Enabled = true;
		}

		private void menuItemExit_Click(object sender, System.EventArgs e)
		{
			Dispose();
		}

		private void menuItemRootFolder_Click(object sender, System.EventArgs e)
		{
			//Form settings = new Form();
			
		}

	}
}
