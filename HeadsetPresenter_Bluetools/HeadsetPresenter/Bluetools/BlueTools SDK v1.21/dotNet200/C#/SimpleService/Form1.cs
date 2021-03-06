using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using Franson.BlueTools;

namespace SimpleService
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button bAdvertise;
		private System.Windows.Forms.Button bDeadvertise;
		private System.Windows.Forms.TextBox txtWriteData;
		private System.Windows.Forms.Button bWrite;
		private System.Windows.Forms.TextBox txtRead;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label lServiceStatus;
		private System.Windows.Forms.Label lStackID;
		private System.Windows.Forms.ListBox listSessions;
		private System.Windows.Forms.Label label3;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
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
			this.bAdvertise = new System.Windows.Forms.Button();
			this.bDeadvertise = new System.Windows.Forms.Button();
			this.txtWriteData = new System.Windows.Forms.TextBox();
			this.bWrite = new System.Windows.Forms.Button();
			this.txtRead = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.lServiceStatus = new System.Windows.Forms.Label();
			this.lStackID = new System.Windows.Forms.Label();
			this.listSessions = new System.Windows.Forms.ListBox();
			this.label3 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// bAdvertise
			// 
			this.bAdvertise.Location = new System.Drawing.Point(24, 24);
			this.bAdvertise.Name = "bAdvertise";
			this.bAdvertise.Size = new System.Drawing.Size(96, 24);
			this.bAdvertise.TabIndex = 0;
			this.bAdvertise.Text = "Advertise";
			this.bAdvertise.Click += new System.EventHandler(this.bAdvertise_Click);
			// 
			// bDeadvertise
			// 
			this.bDeadvertise.Location = new System.Drawing.Point(24, 56);
			this.bDeadvertise.Name = "bDeadvertise";
			this.bDeadvertise.Size = new System.Drawing.Size(96, 24);
			this.bDeadvertise.TabIndex = 1;
			this.bDeadvertise.Text = "Deadvertise";
			this.bDeadvertise.Click += new System.EventHandler(this.bDeadvertise_Click);
			// 
			// txtWriteData
			// 
			this.txtWriteData.Location = new System.Drawing.Point(32, 136);
			this.txtWriteData.Name = "txtWriteData";
			this.txtWriteData.Size = new System.Drawing.Size(216, 20);
			this.txtWriteData.TabIndex = 2;
			this.txtWriteData.Text = "some data";
			// 
			// bWrite
			// 
			this.bWrite.Location = new System.Drawing.Point(256, 136);
			this.bWrite.Name = "bWrite";
			this.bWrite.Size = new System.Drawing.Size(56, 24);
			this.bWrite.TabIndex = 3;
			this.bWrite.Text = "Write";
			this.bWrite.Click += new System.EventHandler(this.bWrite_Click);
			// 
			// txtRead
			// 
			this.txtRead.Location = new System.Drawing.Point(32, 184);
			this.txtRead.Name = "txtRead";
			this.txtRead.Size = new System.Drawing.Size(216, 20);
			this.txtRead.TabIndex = 4;
			this.txtRead.Text = "";
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(32, 168);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(152, 16);
			this.label1.TabIndex = 5;
			this.label1.Text = "Data from client";
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(32, 120);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(136, 16);
			this.label2.TabIndex = 6;
			this.label2.Text = "Data to client";
			// 
			// lServiceStatus
			// 
			this.lServiceStatus.Location = new System.Drawing.Point(152, 32);
			this.lServiceStatus.Name = "lServiceStatus";
			this.lServiceStatus.Size = new System.Drawing.Size(136, 16);
			this.lServiceStatus.TabIndex = 7;
			this.lServiceStatus.Text = "Service not active";
			// 
			// lStackID
			// 
			this.lStackID.Location = new System.Drawing.Point(32, 232);
			this.lStackID.Name = "lStackID";
			this.lStackID.Size = new System.Drawing.Size(232, 16);
			this.lStackID.TabIndex = 8;
			// 
			// listSessions
			// 
			this.listSessions.Location = new System.Drawing.Point(320, 136);
			this.listSessions.Name = "listSessions";
			this.listSessions.Size = new System.Drawing.Size(152, 82);
			this.listSessions.TabIndex = 9;
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(320, 112);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(88, 16);
			this.label3.TabIndex = 10;
			this.label3.Text = "Sessions:";
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(504, 253);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.listSessions);
			this.Controls.Add(this.lStackID);
			this.Controls.Add(this.lServiceStatus);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.txtRead);
			this.Controls.Add(this.bWrite);
			this.Controls.Add(this.txtWriteData);
			this.Controls.Add(this.bDeadvertise);
			this.Controls.Add(this.bAdvertise);
			this.Name = "Form1";
			this.Text = "SimpleService C#";
			this.Load += new System.EventHandler(this.Form1_Load);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Form1());
		}

		Manager m_manager = null;
		Network m_network = null;
		LocalService m_service = null;

		private void Form1_Load(object sender, System.EventArgs e)
		{
			// Clean up when form closes
			this.Closing += new System.ComponentModel.CancelEventHandler(Form1_Closing);

			try
			{
				m_manager	= Manager.GetManager();

				switch(Manager.StackID)
				{
					case StackID.STACK_MICROSOFT:
						lStackID.Text = "Microsoft stack";
						break;
					case StackID.STACK_WIDCOMM:
						lStackID.Text = "WidComm stack";
						break;
				}

				// You can get a valid evaluation key for BlueTools at
				// http://franson.com/bluetools/
				// That key will be valid for 14 days. Just cut and paste that key into the statement below.
				// To get a key that do not expire you need to purchase a license
				Franson.BlueTools.License license = new Franson.BlueTools.License();
				license.LicenseKey = "WoK6IL358ACCJORQUXVZQjpRERguNYEYWrT7";
				
				// Call events in GUI thread
				m_manager.Parent = this;
				// Call events in new thread (multi-threading)
				// manager.Parent = null

				// Get first netowrk (BlueTools 1.0 only supports one network == one dongle)
				m_network = m_manager.Networks[0];

				// Create local service
				m_service = new LocalService(ServiceType.SerialPort, "SimpleService", "Sample");

				// Called when client connected to service
				m_service.ClientConnected += new BlueToolsEventHandler(m_service_ClientConnected);
				// Called when client disconnected from service
				m_service.ClientDisconnected += new BlueToolsEventHandler(m_service_ClientDisconnected);

				// Called when service successfully is advertised to server
				m_service.Advertised += new BlueToolsEventHandler(m_service_Advertised);
				// Called when all connection to service closed, and service removed from server
				m_service.Deadvertised += new BlueToolsEventHandler(m_service_Deadvertised);

				// Buttons
				bAdvertise.Enabled = true;
				bDeadvertise.Enabled = false;
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.ToString());
			}
		}

		private void m_service_ClientConnected(object sender, BlueToolsEventArgs eventArgs)
		{
			ConnectionEventArgs connectionEvent = (ConnectionEventArgs) eventArgs;

			Session connectedSession = connectionEvent.Session;

			// Add new session to list
			listSessions.Items.Add(connectedSession);

			// Read data from stream
			System.IO.Stream connectedStream = connectedSession.Stream;
			byte[] buffer = new byte[20];
			connectedStream.BeginRead(buffer, 0, buffer.Length, new AsyncCallback(readCallback), connectedStream);

			lServiceStatus.Text = "Client connected";
		}

		private void m_service_ClientDisconnected(object sender, BlueToolsEventArgs eventArgs)
		{
			ConnectionEventArgs connectionEvent = (ConnectionEventArgs) eventArgs;

			// Remove session from list
			listSessions.Items.Remove(connectionEvent.Session);

			lServiceStatus.Text = "Client disconnected";
		}

		private void bAdvertise_Click(object sender, System.EventArgs e)
		{
			try
			{
				m_network.Server.Advertise(m_service);
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.ToString());
			}
		}

		private void bDeadvertise_Click(object sender, System.EventArgs e)
		{
			try
			{
				m_network.Server.Deadvertise(m_service);
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.ToString());
			}
		}

		private void m_service_Advertised(object sender, BlueToolsEventArgs eventArgs)
		{
			lServiceStatus.Text = "Service advertised";

			// Buttons
			bAdvertise.Enabled = false;
			bDeadvertise.Enabled = true;
		}

		private void m_service_Deadvertised(object sender, BlueToolsEventArgs eventArgs)
		{
			lServiceStatus.Text = "Service not active";

			// Buttons
			bAdvertise.Enabled = true;
			bDeadvertise.Enabled = false;
		}

		private void Form1_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			// Dispose must be called for the application to exit!
			Manager.GetManager().Dispose();
		}

		private void bWrite_Click(object sender, System.EventArgs e)
		{
			Session selectedSession = (Session) listSessions.SelectedItem;

			if(selectedSession != null)
			{
				System.IO.Stream selectedStream = selectedSession.Stream;

				char[] charWrite = txtWriteData.Text.ToCharArray();
				byte[] byteWrite = new byte[charWrite.Length];

				for(int inx = 0; inx < charWrite.Length; inx++)
				{
					byteWrite[inx] = (byte) charWrite[inx];
				}
				selectedStream.BeginWrite(byteWrite, 0, byteWrite.Length, new AsyncCallback(writeCallback), selectedStream);

			}
			else
			{
				MessageBox.Show("Select a session first");
			}
		}

		private void writeCallback(IAsyncResult result)
		{
			System.IO.Stream selectedStream = (System.IO.Stream) result.AsyncState;

			try
			{
				// EndWrite() must always be called if BegineWrite() was used!
				selectedStream.EndWrite(result);
			}
			catch(ObjectDisposedException ex)
			{
				// Thrown if stream has been closed.
				lServiceStatus.Text = ex.Message;
			}
		}

		private void readCallback(IAsyncResult result)
		{
			// Receives data from all connected devices!

			// IAsyncResult argument is of type BlueToolsAsyncResult
			BlueToolsAsyncResult blueResults = (BlueToolsAsyncResult) result;

			System.IO.Stream currentStream = (System.IO.Stream) blueResults.AsyncState;
			
			byte[] buffer = blueResults.Buffer;

			try
			{
				// EndRead() must always be called!
				int len = currentStream.EndRead(result);

				System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding(); 
				string str = enc.GetString(buffer, 0, len);

				txtRead.Text = str;

				// Start new async read
				currentStream.BeginRead(buffer, 0, buffer.Length, new AsyncCallback(readCallback), currentStream);
			}
			catch(ObjectDisposedException ex)
			{
				// Thrown if stream has been closed.
				lServiceStatus.Text = ex.Message;
			}
		}

	}
}
