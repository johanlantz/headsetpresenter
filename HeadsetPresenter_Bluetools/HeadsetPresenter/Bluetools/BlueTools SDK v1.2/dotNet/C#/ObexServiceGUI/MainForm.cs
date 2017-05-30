using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using Franson.Protocols.Obex.OBEXService;
using Franson.BlueTools;


namespace ObexServerSample
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class MainForm : System.Windows.Forms.Form
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private System.Windows.Forms.ListBox listBoxConnections;
		private System.Windows.Forms.GroupBox groupBoxConnectedDevices;
		private System.Windows.Forms.GroupBox groupBoxCommands;
		private System.Windows.Forms.ListBox listBoxCommands;
		private System.Windows.Forms.Button buttonStart;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.RadioButton radioButtonPush;
		private System.Windows.Forms.Button buttonStop;
		private System.Windows.Forms.RadioButton radioButtonFTP;
		private System.Windows.Forms.MainMenu mainMenu;
		private System.Windows.Forms.MenuItem menuItem1;
		private System.Windows.Forms.MenuItem menuItem2;
		private System.Windows.Forms.MenuItem menuItemExit;
		private System.Windows.Forms.MenuItem menuItemSetRoot;
		private System.Windows.Forms.GroupBox groupBoxInfo;
		private System.Windows.Forms.Label labelRootFolder;
		private System.Windows.Forms.StatusBar statusBar;
		private System.Windows.Forms.Label labelRootText;
		
		private bool m_bIsRunning = false;
		private string m_sRootFolderPath;
		private ServiceExtended m_extendedService = null;
		private Manager m_manager = null;

		public MainForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			this.Closing += new System.ComponentModel.CancelEventHandler(MainForm_Closing);

			try
			{
				m_manager = Manager.GetManager();
				
				Franson.BlueTools.License license = new Franson.BlueTools.License();
				license.LicenseKey = "WoK6IL94B9ECLOPQSXVZPixRDRfuIYHYWsK6";

				// show which stack application is using
				switch (Manager.StackID)
				{
					case StackID.STACK_MICROSOFT:
					{
						statusBar.Text = "Microsoft Bluetooth Stack";
						break;
					}

					case StackID.STACK_WIDCOMM:
					{
						statusBar.Text = "WidComm Bluetooth Stack";
						break;
					}
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message);
			}

			buttonStart.Enabled = false;
			buttonStop.Enabled = false;
		}


		private void OnConnect(ObexSession obexSession)
		{
			listBoxCommands.Items.Add(obexSession.RemoteDevice.Name + ": Connecting");
			listBoxCommands.SelectedIndex = listBoxCommands.Items.Count - 1;

			listBoxConnections.Items.Add(obexSession.RemoteDevice);
		}

		private void OnDisconnect(ObexSession obexSession)
		{
			
			listBoxCommands.Items.Add(obexSession.RemoteDevice.Name + ": Disconnecting");
			listBoxCommands.SelectedIndex = listBoxCommands.Items.Count - 1;

			listBoxConnections.Items.Remove(obexSession.RemoteDevice);
		}

		private void OnAbort(ObexSession obexSession)
		{
			listBoxCommands.Items.Add(obexSession.RemoteDevice.Name + ": Aborting...");
			listBoxCommands.SelectedIndex = listBoxCommands.Items.Count - 1;
		}
		
		private void OnCreateFolder(ObexSession obexSession, ObexFolder folder)
		{
			listBoxCommands.Items.Add(obexSession.RemoteDevice.Name + ": Create folder " + folder.Path);
			listBoxCommands.SelectedIndex = listBoxCommands.Items.Count - 1;
		}

		private void OnDelete(ObexSession obexSession, ObexFolderItem folderItem)
		{
			listBoxCommands.Items.Add(obexSession.RemoteDevice.Name + ": Delete " + folderItem.Path);
			listBoxCommands.SelectedIndex = listBoxCommands.Items.Count - 1;
		}

		private void OnGet(ObexSession obexSession, ObexFile obexFile)
		{
			listBoxCommands.Items.Add(obexSession.RemoteDevice.Name + ": Get file " + obexFile.Path);
			listBoxCommands.SelectedIndex = listBoxCommands.Items.Count - 1;
		}

		private void OnPut(ObexSession obexSession, ObexFile obexFile)
		{
			listBoxCommands.Items.Add(obexSession.RemoteDevice.Name + ": Put file " + obexFile.Name + " To: " + obexFile.ShortPath);
			listBoxCommands.SelectedIndex = listBoxCommands.Items.Count - 1;
		}

		private void OnSetPath(ObexSession obexSession, ObexFolder obexFolder)
		{
			listBoxCommands.Items.Add(obexSession.RemoteDevice.Name + ": Set path " + obexFolder.Path);
			listBoxCommands.SelectedIndex = listBoxCommands.Items.Count - 1;
		}

		private void OnGetPath(ObexSession obexSession)
		{
			listBoxCommands.Items.Add(obexSession.RemoteDevice.Name + ": Get path " + obexSession.CurrentFolder.Path);
			listBoxCommands.SelectedIndex = listBoxCommands.Items.Count - 1;	
		}

		private void OnException(ObexSession obexSession, Exception exception)
		{
			listBoxCommands.Items.Add(exception.Message);
			listBoxCommands.SelectedIndex = listBoxCommands.Items.Count - 1;

			if(listBoxConnections.Items.Contains(obexSession.RemoteDevice))
			{
				listBoxConnections.Items.Remove(obexSession.RemoteDevice);
			}
		}
		
		/// <summary>
		/// TODO. Proper shutdown Manager.GetManager().Dispose();
		/// </summary>
		protected override void Dispose( bool disposing )
		{			
			if(m_bIsRunning)
				
				m_extendedService.Stop();
				m_bIsRunning = false;
			
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

		private void MainForm_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{

			Dispose();
		}



		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.listBoxConnections = new System.Windows.Forms.ListBox();
			this.groupBoxConnectedDevices = new System.Windows.Forms.GroupBox();
			this.groupBoxCommands = new System.Windows.Forms.GroupBox();
			this.listBoxCommands = new System.Windows.Forms.ListBox();
			this.buttonStart = new System.Windows.Forms.Button();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.radioButtonFTP = new System.Windows.Forms.RadioButton();
			this.radioButtonPush = new System.Windows.Forms.RadioButton();
			this.buttonStop = new System.Windows.Forms.Button();
			this.groupBoxInfo = new System.Windows.Forms.GroupBox();
			this.mainMenu = new System.Windows.Forms.MainMenu();
			this.menuItem1 = new System.Windows.Forms.MenuItem();
			this.menuItem2 = new System.Windows.Forms.MenuItem();
			this.menuItemExit = new System.Windows.Forms.MenuItem();
			this.menuItemSetRoot = new System.Windows.Forms.MenuItem();
			this.labelRootFolder = new System.Windows.Forms.Label();
			this.statusBar = new System.Windows.Forms.StatusBar();
			this.labelRootText = new System.Windows.Forms.Label();
			this.groupBoxConnectedDevices.SuspendLayout();
			this.groupBoxCommands.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.groupBoxInfo.SuspendLayout();
			this.SuspendLayout();
			// 
			// listBoxConnections
			// 
			this.listBoxConnections.Location = new System.Drawing.Point(8, 16);
			this.listBoxConnections.Name = "listBoxConnections";
			this.listBoxConnections.Size = new System.Drawing.Size(184, 95);
			this.listBoxConnections.TabIndex = 0;
			// 
			// groupBoxConnectedDevices
			// 
			this.groupBoxConnectedDevices.Controls.Add(this.listBoxConnections);
			this.groupBoxConnectedDevices.Location = new System.Drawing.Point(8, 8);
			this.groupBoxConnectedDevices.Name = "groupBoxConnectedDevices";
			this.groupBoxConnectedDevices.Size = new System.Drawing.Size(200, 120);
			this.groupBoxConnectedDevices.TabIndex = 3;
			this.groupBoxConnectedDevices.TabStop = false;
			this.groupBoxConnectedDevices.Text = "Connected Devices";
			// 
			// groupBoxCommands
			// 
			this.groupBoxCommands.Controls.Add(this.listBoxCommands);
			this.groupBoxCommands.Location = new System.Drawing.Point(8, 136);
			this.groupBoxCommands.Name = "groupBoxCommands";
			this.groupBoxCommands.Size = new System.Drawing.Size(520, 200);
			this.groupBoxCommands.TabIndex = 4;
			this.groupBoxCommands.TabStop = false;
			this.groupBoxCommands.Text = "Log";
			// 
			// listBoxCommands
			// 
			this.listBoxCommands.Location = new System.Drawing.Point(8, 16);
			this.listBoxCommands.Name = "listBoxCommands";
			this.listBoxCommands.Size = new System.Drawing.Size(504, 173);
			this.listBoxCommands.TabIndex = 0;
			// 
			// buttonStart
			// 
			this.buttonStart.Location = new System.Drawing.Point(216, 96);
			this.buttonStart.Name = "buttonStart";
			this.buttonStart.Size = new System.Drawing.Size(72, 23);
			this.buttonStart.TabIndex = 5;
			this.buttonStart.Text = "Start";
			this.buttonStart.Click += new System.EventHandler(this.buttonStart_Click);
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.radioButtonPush);
			this.groupBox1.Controls.Add(this.radioButtonFTP);
			this.groupBox1.Location = new System.Drawing.Point(216, 8);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(152, 80);
			this.groupBox1.TabIndex = 6;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Service Type";
			// 
			// radioButtonFTP
			// 
			this.radioButtonFTP.Checked = true;
			this.radioButtonFTP.Location = new System.Drawing.Point(8, 16);
			this.radioButtonFTP.Name = "radioButtonFTP";
			this.radioButtonFTP.Size = new System.Drawing.Size(128, 24);
			this.radioButtonFTP.TabIndex = 0;
			this.radioButtonFTP.TabStop = true;
			this.radioButtonFTP.Text = "OBEX File Transfer";
			// 
			// radioButtonPush
			// 
			this.radioButtonPush.Location = new System.Drawing.Point(8, 40);
			this.radioButtonPush.Name = "radioButtonPush";
			this.radioButtonPush.Size = new System.Drawing.Size(128, 24);
			this.radioButtonPush.TabIndex = 1;
			this.radioButtonPush.Text = "OBEX Object Push";
			// 
			// buttonStop
			// 
			this.buttonStop.Location = new System.Drawing.Point(296, 96);
			this.buttonStop.Name = "buttonStop";
			this.buttonStop.Size = new System.Drawing.Size(72, 23);
			this.buttonStop.TabIndex = 6;
			this.buttonStop.Text = "Stop";
			this.buttonStop.Click += new System.EventHandler(this.buttonStop_Click);
			// 
			// groupBoxInfo
			// 
			this.groupBoxInfo.Controls.Add(this.labelRootText);
			this.groupBoxInfo.Controls.Add(this.labelRootFolder);
			this.groupBoxInfo.Location = new System.Drawing.Point(376, 8);
			this.groupBoxInfo.Name = "groupBoxInfo";
			this.groupBoxInfo.Size = new System.Drawing.Size(152, 128);
			this.groupBoxInfo.TabIndex = 7;
			this.groupBoxInfo.TabStop = false;
			this.groupBoxInfo.Text = "Info";
			// 
			// mainMenu
			// 
			this.mainMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					 this.menuItem1,
																					 this.menuItem2});
			// 
			// menuItem1
			// 
			this.menuItem1.Index = 0;
			this.menuItem1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuItemExit});
			this.menuItem1.Text = "File";
			// 
			// menuItem2
			// 
			this.menuItem2.Index = 1;
			this.menuItem2.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuItemSetRoot});
			this.menuItem2.Text = "Settings";
			// 
			// menuItemExit
			// 
			this.menuItemExit.Index = 0;
			this.menuItemExit.Text = "Exit";
			this.menuItemExit.Click += new System.EventHandler(this.menuItemExit_Click);
			// 
			// menuItemSetRoot
			// 
			this.menuItemSetRoot.Index = 0;
			this.menuItemSetRoot.Text = "Root folder";
			this.menuItemSetRoot.Click += new System.EventHandler(this.menuItemSetRoot_Click);
			// 
			// labelRootFolder
			// 
			this.labelRootFolder.Location = new System.Drawing.Point(8, 32);
			this.labelRootFolder.Name = "labelRootFolder";
			this.labelRootFolder.Size = new System.Drawing.Size(136, 40);
			this.labelRootFolder.TabIndex = 0;
			this.labelRootFolder.Text = "No root folder is set!";
			// 
			// statusBar
			// 
			this.statusBar.Location = new System.Drawing.Point(0, 344);
			this.statusBar.Name = "statusBar";
			this.statusBar.Size = new System.Drawing.Size(536, 22);
			this.statusBar.TabIndex = 8;
			this.statusBar.Text = "statusBar";
			// 
			// labelRootText
			// 
			this.labelRootText.Location = new System.Drawing.Point(8, 16);
			this.labelRootText.Name = "labelRootText";
			this.labelRootText.Size = new System.Drawing.Size(100, 16);
			this.labelRootText.TabIndex = 1;
			this.labelRootText.Text = "Root folder:";
			// 
			// MainForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(536, 366);
			this.Controls.Add(this.statusBar);
			this.Controls.Add(this.groupBoxInfo);
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.groupBoxCommands);
			this.Controls.Add(this.groupBoxConnectedDevices);
			this.Controls.Add(this.buttonStart);
			this.Controls.Add(this.buttonStop);
			this.Menu = this.mainMenu;
			this.Name = "MainForm";
			this.Text = "Obex FTP Service";
			this.Load += new System.EventHandler(this.MainForm_Load);
			this.groupBoxConnectedDevices.ResumeLayout(false);
			this.groupBoxCommands.ResumeLayout(false);
			this.groupBox1.ResumeLayout(false);
			this.groupBoxInfo.ResumeLayout(false);
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
				m_extendedService.OnConnect			+= new ServiceExtended.ObexServiceEventHandler(OnConnect);
				m_extendedService.OnDisconnect		+= new ServiceExtended.ObexServiceEventHandler(OnDisconnect);
				m_extendedService.OnAbort			+= new ServiceExtended.ObexServiceEventHandler(OnAbort);
				m_extendedService.OnCreateFolder	+= new ServiceExtended.ObexServiceFolderOperationEventHandler(OnCreateFolder);
				m_extendedService.OnDelete			+= new ServiceExtended.ObexServiceFolderItemEventHandler(OnDelete);
				m_extendedService.OnGet				+= new ServiceExtended.ObexServiceFileTransferEventHandler(OnGet);
				m_extendedService.OnPut				+= new ServiceExtended.ObexServiceFileTransferEventHandler(OnPut);
				m_extendedService.OnSetPath			+= new ServiceExtended.ObexServiceFolderOperationEventHandler(OnSetPath);
				m_extendedService.OnGetPath			+= new  ServiceExtended.ObexServiceEventHandler(OnGetPath);
				m_extendedService.OnObexException	+= new ServiceExtended.ObexServiceExceptionEventHandler(OnException);
			
				//Start the service
				m_extendedService.Start();
				m_bIsRunning = true;
				
				buttonStop.Enabled		= true;
				buttonStart.Enabled		= false;
				radioButtonFTP.Enabled	= false;
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


			buttonStop.Enabled		= false;
			buttonStart.Enabled		= true;
			
			radioButtonFTP.Enabled	= true;
			radioButtonPush.Enabled = true;

		}

		private void menuItemExit_Click(object sender, System.EventArgs e)
		{
			Dispose();
		}

		private void menuItemSetRoot_Click(object sender, System.EventArgs e)
		{
			System.Windows.Forms.FolderBrowserDialog folderBrowserDialog= new FolderBrowserDialog();
			folderBrowserDialog.ShowDialog();

			m_sRootFolderPath = folderBrowserDialog.SelectedPath;
			labelRootFolder.Text = m_sRootFolderPath;

			buttonStart.Enabled = true;

		}
	}
}
