using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using Franson.BlueTools;

namespace Chat
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class MainForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.MainMenu mainMenu;
		private System.Windows.Forms.MenuItem menuItem1;
		private System.Windows.Forms.MenuItem menuItem2;
		private System.Windows.Forms.MenuItem menuItem3;
		private System.Windows.Forms.MenuItem exitApplication;
		private System.Windows.Forms.MenuItem menuItem4;
		private System.Windows.Forms.MenuItem setup;
		private System.Collections.ArrayList pendingMessages = new System.Collections.ArrayList();
		private System.Windows.Forms.TabControl tabControl;
		private System.Windows.Forms.TabPage chatPage;
		private System.Windows.Forms.TabPage connectPage;
		private System.Windows.Forms.TextBox discussion;
		private System.Windows.Forms.TextBox message;
		private System.Windows.Forms.Button send;

		private ChatManager	chatManager;
		private System.Windows.Forms.ComboBox participantList;
		private System.Windows.Forms.MenuItem menuItem5;
		private System.Windows.Forms.MenuItem enableService;
		private System.Windows.Forms.MenuItem disableService;

		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public MainForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			try
			{
				// You can get a valid evaluation key for BlueTools at
				// http://franson.com/bluetools/
				// That key will be valid for 14 days. Just cut and paste that key into the statement below.
				// To get a key that do not expire you need to purchase a license
				Franson.BlueTools.License license = new Franson.BlueTools.License();
				license.LicenseKey = "HU5UYfqz22OOHWB8qhmhMlVRYvz0MRMbHY5W";
				this.Load += new EventHandler(MainForm_Load);
			} 
			catch (BlueToolsException exc)
			{
				MessageBox.Show(exc.Message);
				return;
			}
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			Manager.GetManager().Dispose();

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
			this.mainMenu = new System.Windows.Forms.MainMenu();
			this.menuItem3 = new System.Windows.Forms.MenuItem();
			this.exitApplication = new System.Windows.Forms.MenuItem();
			this.menuItem4 = new System.Windows.Forms.MenuItem();
			this.setup = new System.Windows.Forms.MenuItem();
			this.menuItem5 = new System.Windows.Forms.MenuItem();
			this.enableService = new System.Windows.Forms.MenuItem();
			this.disableService = new System.Windows.Forms.MenuItem();
			this.menuItem1 = new System.Windows.Forms.MenuItem();
			this.menuItem2 = new System.Windows.Forms.MenuItem();
			this.tabControl = new System.Windows.Forms.TabControl();
			this.chatPage = new System.Windows.Forms.TabPage();
			this.participantList = new System.Windows.Forms.ComboBox();
			this.discussion = new System.Windows.Forms.TextBox();
			this.message = new System.Windows.Forms.TextBox();
			this.send = new System.Windows.Forms.Button();
			this.connectPage = new System.Windows.Forms.TabPage();
			// 
			// mainMenu
			// 
			this.mainMenu.MenuItems.Add(this.menuItem3);
			this.mainMenu.MenuItems.Add(this.menuItem4);
			// 
			// menuItem3
			// 
			this.menuItem3.MenuItems.Add(this.exitApplication);
			this.menuItem3.Text = "File";
			// 
			// exitApplication
			// 
			this.exitApplication.Text = "Exit";
			this.exitApplication.Click += new System.EventHandler(this.exitApplication_Click);
			// 
			// menuItem4
			// 
			this.menuItem4.MenuItems.Add(this.setup);
			this.menuItem4.MenuItems.Add(this.menuItem5);
			this.menuItem4.Text = "Tools";
			// 
			// setup
			// 
			this.setup.Text = "Setup";
			this.setup.Click += new System.EventHandler(this.setup_Click);
			// 
			// menuItem5
			// 
			this.menuItem5.MenuItems.Add(this.enableService);
			this.menuItem5.MenuItems.Add(this.disableService);
			this.menuItem5.Text = "Service";
			// 
			// enableService
			// 
			this.enableService.Text = "Enabled";
			this.enableService.Click += new System.EventHandler(this.enableService_Click);
			// 
			// disableService
			// 
			this.disableService.Enabled = false;
			this.disableService.Text = "Disabled";
			this.disableService.Click += new System.EventHandler(this.disableService_Click);
			// 
			// menuItem1
			// 
			this.menuItem1.Text = "File";
			// 
			// menuItem2
			// 
			this.menuItem2.Text = "Tools";
			// 
			// tabControl
			// 
			this.tabControl.Controls.Add(this.chatPage);
			this.tabControl.Controls.Add(this.connectPage);
			this.tabControl.SelectedIndex = 0;
			this.tabControl.Size = new System.Drawing.Size(234, 272);
			// 
			// chatPage
			// 
			this.chatPage.Controls.Add(this.participantList);
			this.chatPage.Controls.Add(this.discussion);
			this.chatPage.Controls.Add(this.message);
			this.chatPage.Controls.Add(this.send);
			this.chatPage.Location = new System.Drawing.Point(4, 4);
			this.chatPage.Size = new System.Drawing.Size(226, 246);
			this.chatPage.Text = "Chat";
			// 
			// participantList
			// 
			this.participantList.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Regular);
			this.participantList.Location = new System.Drawing.Point(16, 8);
			// 
			// discussion
			// 
			this.discussion.Location = new System.Drawing.Point(16, 32);
			this.discussion.Multiline = true;
			this.discussion.Size = new System.Drawing.Size(200, 152);
			this.discussion.Text = "";
			// 
			// message
			// 
			this.message.Location = new System.Drawing.Point(16, 192);
			this.message.Size = new System.Drawing.Size(200, 20);
			this.message.Text = "";
			// 
			// send
			// 
			this.send.Location = new System.Drawing.Point(16, 216);
			this.send.Size = new System.Drawing.Size(96, 24);
			this.send.Text = "Send";
			this.send.Click += new System.EventHandler(this.send_Click);
			// 
			// connectPage
			// 
			this.connectPage.Location = new System.Drawing.Point(4, 4);
			this.connectPage.Size = new System.Drawing.Size(602, 246);
			this.connectPage.Text = "Connect";
			// 
			// MainForm
			// 
			this.ClientSize = new System.Drawing.Size(234, 273);
			this.Controls.Add(this.tabControl);
			this.Menu = this.mainMenu;
			this.Text = "Blue Chat";

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		static void Main() 
		{
			Application.Run(new MainForm());
		}

		private void setup_Click(object sender, System.EventArgs e)
		{
			SetupForm setupForm = new SetupForm();
			if(setupForm.ShowDialog() == DialogResult.OK)
			{
				chatManager.Profile = setupForm.Profile;
			}
		}

		private void exitApplication_Click(object sender, System.EventArgs e)
		{
			chatManager.Close();
			Close();
			Dispose();
		}

		private void MainForm_Load(object sender, EventArgs e)
		{
			Manager.GetManager().Parent = this;
		}

		private void send_Click(object sender, System.EventArgs e)
		{
			chatManager.SendMessage(this.message.Text);
			discussion.Text += "Me - " + this.message.Text + "\r\n";
			this.message.Text = "";
		}

		private void chatManager_ProfileChanged(Participant participant)
		{
			// Marshal the event to the main thread
			//
			Invoke(new EventHandler(this.UpdateProfileList));
		}

		private void chatManager_MessageReceived(Participant participant, String message)
		{
			// Add the message to the list of pending messages
			//
			pendingMessages.Add(participant.Profile.Name + " - " + message + "\r\n");
			
			// Marshal the event to the main thread
			//
			Invoke(new EventHandler(this.UpdateDiscussion));
		}

		private void UpdateDiscussion(object sender, EventArgs eventArgs)
		{
			while(pendingMessages.Count > 0)
			{
				String message = (String)pendingMessages[0];
				pendingMessages.RemoveAt(0);
				discussion.Text += message;
			}
		}

		private void UpdateProfileList(object sender, EventArgs eventArgs)
		{
			participantList.Items.Clear();
			Participant[] participants = chatManager.Participants;
			foreach(Participant participant in participants)
			{
				participantList.Items.Add(participant);
			}
		}

		private void enableService_Click(object sender, System.EventArgs e)
		{
			if(chatManager == null)
			{
				chatManager	= new ChatManager();

				chatManager.ProfileChanged += new ProfileChangedHandler(chatManager_ProfileChanged);
				chatManager.MessageReceived += new MessageReceivedHandler(chatManager_MessageReceived);

				enableService.Enabled = false;
				disableService.Enabled = true;
			}
		}

		private void disableService_Click(object sender, System.EventArgs e)
		{
		
		}
	}
}
