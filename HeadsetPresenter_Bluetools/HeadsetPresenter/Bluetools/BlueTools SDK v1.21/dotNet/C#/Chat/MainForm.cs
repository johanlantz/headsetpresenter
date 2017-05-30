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

		private ChatManager	chatManager;
		private System.Windows.Forms.MenuItem menuItem5;
		private System.Windows.Forms.MenuItem enableService;
		private System.Windows.Forms.MenuItem disableService;
		private System.Windows.Forms.ComboBox participantList;
		private System.Windows.Forms.TextBox discussion;
		private System.Windows.Forms.TextBox message;
		private System.Windows.Forms.Button send;
		private System.Windows.Forms.StatusBar statusBar;

		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private MessageStream messageStream;

		public MainForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			try
			{
				// You can get a valid evaluation key at
				// http://franson.com/bluetools/
				// That key will be valid for 14 days. Just cut and paste that key into the statement below.
				// To get a key that do not expire you need to purchase a license
				Franson.BlueTools.License license = new Franson.BlueTools.License();
				license.LicenseKey ="WoK6HM338A6BRNTQYXVZOiyRERkuKYGYWsKA";
			}
			catch(BlueToolsException exc)
			{
				MessageBox.Show(exc.Message);
				return;
			}
			
			messageStream = new MessageStream(statusBar);
			System.Console.SetOut(messageStream);

			this.Load += new EventHandler(MainForm_Load);
			this.Closing += new CancelEventHandler(MainForm_Closing);
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
			this.participantList = new System.Windows.Forms.ComboBox();
			this.discussion = new System.Windows.Forms.TextBox();
			this.message = new System.Windows.Forms.TextBox();
			this.send = new System.Windows.Forms.Button();
			this.statusBar = new System.Windows.Forms.StatusBar();
			this.SuspendLayout();
			// 
			// mainMenu
			// 
			this.mainMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					 this.menuItem3,
																					 this.menuItem4});
			// 
			// menuItem3
			// 
			this.menuItem3.Index = 0;
			this.menuItem3.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.exitApplication});
			this.menuItem3.Text = "File";
			// 
			// exitApplication
			// 
			this.exitApplication.Index = 0;
			this.exitApplication.Text = "Exit";
			this.exitApplication.Click += new System.EventHandler(this.exitApplication_Click);
			// 
			// menuItem4
			// 
			this.menuItem4.Index = 1;
			this.menuItem4.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.setup,
																					  this.menuItem5});
			this.menuItem4.Text = "Tools";
			// 
			// setup
			// 
			this.setup.Index = 0;
			this.setup.Text = "Setup";
			this.setup.Click += new System.EventHandler(this.setup_Click);
			// 
			// menuItem5
			// 
			this.menuItem5.Index = 1;
			this.menuItem5.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.enableService,
																					  this.disableService});
			this.menuItem5.Text = "Service";
			// 
			// enableService
			// 
			this.enableService.Index = 0;
			this.enableService.Text = "Enabled";
			this.enableService.Click += new System.EventHandler(this.enableService_Click);
			// 
			// disableService
			// 
			this.disableService.Enabled = false;
			this.disableService.Index = 1;
			this.disableService.Text = "Disabled";
			this.disableService.Click += new System.EventHandler(this.disableService_Click);
			// 
			// menuItem1
			// 
			this.menuItem1.Index = -1;
			this.menuItem1.Text = "File";
			// 
			// menuItem2
			// 
			this.menuItem2.Index = -1;
			this.menuItem2.Text = "Tools";
			// 
			// participantList
			// 
			this.participantList.Font = new System.Drawing.Font("Tahoma", 9F);
			this.participantList.Location = new System.Drawing.Point(8, 8);
			this.participantList.Name = "participantList";
			this.participantList.TabIndex = 4;
			// 
			// discussion
			// 
			this.discussion.Location = new System.Drawing.Point(8, 32);
			this.discussion.Multiline = true;
			this.discussion.Name = "discussion";
			this.discussion.Size = new System.Drawing.Size(328, 152);
			this.discussion.TabIndex = 5;
			this.discussion.Text = "";
			// 
			// message
			// 
			this.message.Location = new System.Drawing.Point(8, 192);
			this.message.Name = "message";
			this.message.Size = new System.Drawing.Size(224, 20);
			this.message.TabIndex = 6;
			this.message.Text = "";
			// 
			// send
			// 
			this.send.Location = new System.Drawing.Point(240, 192);
			this.send.Name = "send";
			this.send.Size = new System.Drawing.Size(96, 24);
			this.send.TabIndex = 7;
			this.send.Text = "Send";
			this.send.Click += new System.EventHandler(this.send_Click);
			// 
			// statusBar
			// 
			this.statusBar.Location = new System.Drawing.Point(0, 219);
			this.statusBar.Name = "statusBar";
			this.statusBar.Size = new System.Drawing.Size(352, 22);
			this.statusBar.TabIndex = 8;
			// 
			// MainForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(352, 241);
			this.Controls.Add(this.statusBar);
			this.Controls.Add(this.participantList);
			this.Controls.Add(this.discussion);
			this.Controls.Add(this.message);
			this.Controls.Add(this.send);
			this.Menu = this.mainMenu;
			this.Name = "MainForm";
			this.Text = "Blue Chat";
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

		private void setup_Click(object sender, System.EventArgs e)
		{
			SetupForm setupForm = new SetupForm();
			if(setupForm.ShowDialog() == DialogResult.OK)
			{
				chatManager.Profile = setupForm.Profile;
				Console.WriteLine("Changed name to '" + chatManager.Profile.Name + "'");
			}
		}

		private void exitApplication_Click(object sender, System.EventArgs e)
		{
			Close();
			Dispose();
		}

		private void MainForm_Load(object sender, EventArgs e)
		{
			Manager.GetManager().Parent = this;
		}

		private void send_Click(object sender, System.EventArgs e)
		{
			Console.WriteLine("Sending message...");
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
			Console.WriteLine("Message received from " + participant.Profile.Name);

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
				Participant old = null;
				foreach(Participant existing in participantList.Items)
				{
					if(existing.Address.Equals(participant))
					{
						participantList.Items.Remove(existing);
						old = existing;
						break;
					}
				}

				if(old != null)
				{
					statusBar.Text = "Participant '" + old.Profile.Name + "' changed name";
				}
				else
				{
					statusBar.Text = "Participant '" + participant.Profile.Name + "' connected";
				}

				participantList.Items.Add(participant);
			}
		}

		private void enableService_Click(object sender, System.EventArgs e)
		{
			if(chatManager == null)
			{
				try
				{
					chatManager	= new ChatManager();

					chatManager.ProfileChanged		+= new ProfileChangedHandler(chatManager_ProfileChanged);
					chatManager.MessageReceived		+= new MessageReceivedHandler(chatManager_MessageReceived);
					chatManager.ParticipantDisconnected += new ParticipantDisconnectedHandler(chatManager_ParticipantDisconnected);

					enableService.Enabled = false;
					disableService.Enabled = true;
				} 
				catch (Exception exc)
				{
					MessageBox.Show(this, exc.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button1);
				}
			}
		}

		private void disableService_Click(object sender, System.EventArgs e)
		{
		
		}

		private void MainForm_Closing(object sender, CancelEventArgs e)
		{
			Dispose();
		}

		private void chatManager_ParticipantDisconnected(Participant participant)
		{
			this.participantList.Items.Remove(participant);
			Console.WriteLine("Participant '" + participant.Profile.Name + "' disconnected");
		}
	}
}
