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
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Label label8;
		private System.Windows.Forms.TextBox txtRead2;
		private System.Windows.Forms.Button bWrite2;
		private System.Windows.Forms.Button bAdvertise2;
		private System.Windows.Forms.TextBox txtWriteData2;
		private System.Windows.Forms.ListBox listSessions2;
		private System.Windows.Forms.Label lServiceStatus2;
		private System.Windows.Forms.Button bDeadvertise2;
		private System.Windows.Forms.TextBox txtSCN;
		private System.Windows.Forms.TextBox txtSCN2;
		private System.Windows.Forms.CheckBox cbSerialPort2;
		private System.Windows.Forms.CheckBox cbSerialPort;
		private System.Windows.Forms.Button bCloseSession2;
		private System.Windows.Forms.Button bCloseSession1;
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
			this.label4 = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.txtRead2 = new System.Windows.Forms.TextBox();
			this.bWrite2 = new System.Windows.Forms.Button();
			this.bAdvertise2 = new System.Windows.Forms.Button();
			this.txtWriteData2 = new System.Windows.Forms.TextBox();
			this.bDeadvertise2 = new System.Windows.Forms.Button();
			this.lServiceStatus2 = new System.Windows.Forms.Label();
			this.listSessions2 = new System.Windows.Forms.ListBox();
			this.label8 = new System.Windows.Forms.Label();
			this.txtSCN = new System.Windows.Forms.TextBox();
			this.txtSCN2 = new System.Windows.Forms.TextBox();
			this.cbSerialPort = new System.Windows.Forms.CheckBox();
			this.cbSerialPort2 = new System.Windows.Forms.CheckBox();
			this.bCloseSession2 = new System.Windows.Forms.Button();
			this.bCloseSession1 = new System.Windows.Forms.Button();
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
			this.lServiceStatus.Location = new System.Drawing.Point(136, 24);
			this.lServiceStatus.Name = "lServiceStatus";
			this.lServiceStatus.Size = new System.Drawing.Size(344, 16);
			this.lServiceStatus.TabIndex = 7;
			this.lServiceStatus.Text = "Service not active";
			// 
			// lStackID
			// 
			this.lStackID.Location = new System.Drawing.Point(32, 488);
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
			this.listSessions.SelectedIndexChanged += new System.EventHandler(this.listSessions_SelectedIndexChanged);
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(320, 112);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(88, 16);
			this.label3.TabIndex = 10;
			this.label3.Text = "Sessions:";
			// 
			// label4
			// 
			this.label4.Location = new System.Drawing.Point(32, 368);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(136, 16);
			this.label4.TabIndex = 6;
			this.label4.Text = "Data to client";
			// 
			// label5
			// 
			this.label5.Location = new System.Drawing.Point(32, 416);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(152, 16);
			this.label5.TabIndex = 5;
			this.label5.Text = "Data from client";
			// 
			// txtRead2
			// 
			this.txtRead2.Location = new System.Drawing.Point(32, 432);
			this.txtRead2.Name = "txtRead2";
			this.txtRead2.Size = new System.Drawing.Size(216, 20);
			this.txtRead2.TabIndex = 4;
			this.txtRead2.Text = "";
			// 
			// bWrite2
			// 
			this.bWrite2.Location = new System.Drawing.Point(256, 384);
			this.bWrite2.Name = "bWrite2";
			this.bWrite2.Size = new System.Drawing.Size(56, 24);
			this.bWrite2.TabIndex = 3;
			this.bWrite2.Text = "Write";
			this.bWrite2.Click += new System.EventHandler(this.bWrite2_Click);
			// 
			// bAdvertise2
			// 
			this.bAdvertise2.Location = new System.Drawing.Point(24, 272);
			this.bAdvertise2.Name = "bAdvertise2";
			this.bAdvertise2.Size = new System.Drawing.Size(96, 24);
			this.bAdvertise2.TabIndex = 0;
			this.bAdvertise2.Text = "Advertise";
			this.bAdvertise2.Click += new System.EventHandler(this.bAdvertise2_Click);
			// 
			// txtWriteData2
			// 
			this.txtWriteData2.Location = new System.Drawing.Point(32, 384);
			this.txtWriteData2.Name = "txtWriteData2";
			this.txtWriteData2.Size = new System.Drawing.Size(216, 20);
			this.txtWriteData2.TabIndex = 2;
			this.txtWriteData2.Text = "some data2";
			// 
			// bDeadvertise2
			// 
			this.bDeadvertise2.Location = new System.Drawing.Point(24, 304);
			this.bDeadvertise2.Name = "bDeadvertise2";
			this.bDeadvertise2.Size = new System.Drawing.Size(96, 24);
			this.bDeadvertise2.TabIndex = 1;
			this.bDeadvertise2.Text = "Deadvertise";
			this.bDeadvertise2.Click += new System.EventHandler(this.bDeadvertise2_Click);
			// 
			// lServiceStatus2
			// 
			this.lServiceStatus2.Location = new System.Drawing.Point(136, 272);
			this.lServiceStatus2.Name = "lServiceStatus2";
			this.lServiceStatus2.Size = new System.Drawing.Size(352, 16);
			this.lServiceStatus2.TabIndex = 7;
			this.lServiceStatus2.Text = "Service not active";
			// 
			// listSessions2
			// 
			this.listSessions2.Location = new System.Drawing.Point(320, 384);
			this.listSessions2.Name = "listSessions2";
			this.listSessions2.Size = new System.Drawing.Size(152, 82);
			this.listSessions2.TabIndex = 9;
			// 
			// label8
			// 
			this.label8.Location = new System.Drawing.Point(320, 360);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(88, 16);
			this.label8.TabIndex = 10;
			this.label8.Text = "Sessions:";
			// 
			// txtSCN
			// 
			this.txtSCN.Location = new System.Drawing.Point(240, 56);
			this.txtSCN.Name = "txtSCN";
			this.txtSCN.Size = new System.Drawing.Size(40, 20);
			this.txtSCN.TabIndex = 11;
			this.txtSCN.Text = "0";
			// 
			// txtSCN2
			// 
			this.txtSCN2.Location = new System.Drawing.Point(240, 304);
			this.txtSCN2.Name = "txtSCN2";
			this.txtSCN2.Size = new System.Drawing.Size(40, 20);
			this.txtSCN2.TabIndex = 12;
			this.txtSCN2.Text = "0";
			// 
			// cbSerialPort
			// 
			this.cbSerialPort.Location = new System.Drawing.Point(144, 56);
			this.cbSerialPort.Name = "cbSerialPort";
			this.cbSerialPort.Size = new System.Drawing.Size(80, 16);
			this.cbSerialPort.TabIndex = 13;
			this.cbSerialPort.Text = "Serial Port";
			// 
			// cbSerialPort2
			// 
			this.cbSerialPort2.Location = new System.Drawing.Point(144, 304);
			this.cbSerialPort2.Name = "cbSerialPort2";
			this.cbSerialPort2.Size = new System.Drawing.Size(80, 16);
			this.cbSerialPort2.TabIndex = 13;
			this.cbSerialPort2.Text = "Serial Port";
			// 
			// bCloseSession2
			// 
			this.bCloseSession2.Location = new System.Drawing.Point(320, 472);
			this.bCloseSession2.Name = "bCloseSession2";
			this.bCloseSession2.Size = new System.Drawing.Size(64, 24);
			this.bCloseSession2.TabIndex = 14;
			this.bCloseSession2.Text = "Close";
			this.bCloseSession2.Click += new System.EventHandler(this.bCloseSession2_Click);
			// 
			// bCloseSession1
			// 
			this.bCloseSession1.Location = new System.Drawing.Point(320, 224);
			this.bCloseSession1.Name = "bCloseSession1";
			this.bCloseSession1.Size = new System.Drawing.Size(64, 24);
			this.bCloseSession1.TabIndex = 15;
			this.bCloseSession1.Text = "Close";
			this.bCloseSession1.Click += new System.EventHandler(this.bCloseSession1_Click);
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(504, 525);
			this.Controls.Add(this.bCloseSession1);
			this.Controls.Add(this.bCloseSession2);
			this.Controls.Add(this.cbSerialPort);
			this.Controls.Add(this.txtSCN2);
			this.Controls.Add(this.txtSCN);
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
			this.Controls.Add(this.label4);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.txtRead2);
			this.Controls.Add(this.bWrite2);
			this.Controls.Add(this.bAdvertise2);
			this.Controls.Add(this.txtWriteData2);
			this.Controls.Add(this.bDeadvertise2);
			this.Controls.Add(this.lServiceStatus2);
			this.Controls.Add(this.listSessions2);
			this.Controls.Add(this.label8);
			this.Controls.Add(this.cbSerialPort2);
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
		LocalService m_service2 = null;

		private void Form1_Load(object sender, System.EventArgs e)
		{
			// You can get a valid evaluation key for BlueTools at
			// http://franson.com/bluetools/
			// That key will be valid for 14 days. Just cut and paste that key into the statement below.
			// To get a key that do not expire you need to purchase a license
			Franson.BlueTools.License license = new Franson.BlueTools.License();
			license.LicenseKey = "WoK6IL758ACCJORQUXVZQjpRERguNYEYWrTB";

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

				// Call events in GUI thread
				m_manager.Parent = this;
				// Call events in new thread (multi-threading)
				// manager.Parent = null

				// Get first netowrk (BlueTools 1.0 only supports one network == one dongle)
				m_network = m_manager.Networks[0];

				// Clean up when form closes
				this.Closing += new System.ComponentModel.CancelEventHandler(Form1_Closing);

				// Buttons
				bAdvertise.Enabled = true;
				bDeadvertise.Enabled = false;

				bAdvertise2.Enabled = true;
				bDeadvertise2.Enabled = false;
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

			if(connectedSession.LocalService.Equals(m_service))
			{
				// Add new session to list
				listSessions.Items.Add(connectedSession);
				lServiceStatus.Text = "Client connected";
			}
			else if(connectedSession.LocalService.Equals(m_service2))
			{
				// Add new session to list
				listSessions2.Items.Add(connectedSession);
				lServiceStatus2.Text = "Client connected";
			}
			else
			{
				lServiceStatus2.Text = "ERROR!";
			}

			// Read data from stream
			System.IO.Stream connectedStream = connectedSession.Stream;
			byte[] buffer = new byte[20];
			connectedStream.BeginRead(buffer, 0, buffer.Length, new AsyncCallback(readCallback), connectedStream);
		}

		private void m_service_ClientDisconnected(object sender, BlueToolsEventArgs eventArgs)
		{
			ConnectionEventArgs connectionEvent = (ConnectionEventArgs) eventArgs;

			Session connectedSession = connectionEvent.Session;

			if(connectedSession.LocalService.Equals(m_service))
			{
				// Remove session from list
				listSessions.Items.Remove(connectedSession);
				lServiceStatus.Text = "Client disconnected";
			}
			else if(connectedSession.LocalService.Equals(m_service2))
			{
				listSessions2.Items.Remove(connectedSession);
				lServiceStatus2.Text = "Client disconnected";
			}
			else
			{
				lServiceStatus2.Text = "Client disconnected ERROR";
			}
		}

		private void bAdvertise_Click(object sender, System.EventArgs e)
		{
			try
			{
				// One more local service
				if(cbSerialPort.Checked == true)
				{
					m_service = new LocalService(ServiceType.SerialPort, "SerialPort", "");
				}
				else
				{
					m_service = new LocalService(ServiceType.RFCOMM, "RFCOMM", "");
				}

				// Called when client connected to service
				m_service.ClientConnected += new BlueToolsEventHandler(m_service_ClientConnected);
				// Called when client disconnected from service
				m_service.ClientDisconnected += new BlueToolsEventHandler(m_service_ClientDisconnected);

				// Called when service successfully is advertised to server
				m_service.Advertised += new BlueToolsEventHandler(m_service_Advertised);
				// Called when all connection to service closed, and service removed from server
				m_service.Deadvertised += new BlueToolsEventHandler(m_service_Deadvertised);

				m_network.Server.Advertise(m_service, int.Parse(txtSCN.Text));
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
			if(m_service == (LocalService) sender)
			{
				lServiceStatus.Text = "Service advertised SCN: " + m_service.Address.ServiceChannelNumber;
	
				// Buttons
				bAdvertise.Enabled = false;
				bDeadvertise.Enabled = true;
			}
			else if(m_service2 == (LocalService) sender)
			{
				lServiceStatus2.Text = "Service advertised SCN: " + m_service2.Address.ServiceChannelNumber;

				// Buttons
				bAdvertise2.Enabled = false;
				bDeadvertise2.Enabled = true;
			}
		}

		private void m_service_Deadvertised(object sender, BlueToolsEventArgs eventArgs)
		{
			if(m_service.Equals((LocalService) sender))
			{
				lServiceStatus.Text = "Service not active";

				// Buttons
				bAdvertise.Enabled = true;
				bDeadvertise.Enabled = false;
			}
			else if(m_service2.Equals((LocalService) sender))
			{
				lServiceStatus2.Text = "Service not active";

				// Buttons
				bAdvertise2.Enabled = true;
				bDeadvertise2.Enabled = false;
			}
		}

		private void Form1_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			// Dispose must be called for the application to exit!
			Manager.GetManager().Dispose();
		}

		private void bWrite_Click(object sender, System.EventArgs e)
		{
			writeToSession( (Session) listSessions.SelectedItem );
		}
			
		private void writeToSession(Session selectedSession)
		{
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

			// EndWrite() must always be called if BegineWrite() was used!
			selectedStream.EndWrite(result);
		}

		private void readCallback(IAsyncResult result)
		{
			// Receives data from all connected devices!

			// IAsyncResult argument is of type BlueToolsAsyncResult
			BlueToolsAsyncResult blueResults = (BlueToolsAsyncResult) result;

			System.IO.Stream currentStream = (System.IO.Stream) blueResults.AsyncState;
			
			byte[] buffer = blueResults.Buffer;

			// EndRead() must always be called!
			int len = currentStream.EndRead(result);

			System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding(); 
			string str = enc.GetString(buffer, 0, len);

			txtRead.Text = str;

			// Start new async read
			currentStream.BeginRead(buffer, 0, buffer.Length, new AsyncCallback(readCallback), currentStream);
		}

		private void bAdvertise2_Click(object sender, System.EventArgs e)
		{
			// One more local service
			if(cbSerialPort2.Checked == true)
			{
				m_service2 = new LocalService(ServiceType.SerialPort, "SerialPort2", "");
			}
			else
			{
				m_service2 = new LocalService(ServiceType.RFCOMM, "RFCOMM2", "");
			}

			m_service2.ClientConnected += new BlueToolsEventHandler(m_service_ClientConnected);
			m_service2.ClientDisconnected += new BlueToolsEventHandler(m_service_ClientDisconnected);
			m_service2.Advertised += new BlueToolsEventHandler(m_service_Advertised);
			m_service2.Deadvertised += new BlueToolsEventHandler(m_service_Deadvertised);

			try
			{
				m_network.Server.Advertise(m_service2, int.Parse(txtSCN2.Text));
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.ToString());
			}
		
		}

		private void bDeadvertise2_Click(object sender, System.EventArgs e)
		{
			try
			{
				m_network.Server.Deadvertise(m_service2);
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.ToString());
			}
		
		}

		private void bWrite2_Click(object sender, System.EventArgs e)
		{
			writeToSession( (Session) listSessions2.SelectedItem );
		}

		private void bCloseSession1_Click(object sender, System.EventArgs e)
		{
			closeSession( (Session) listSessions.SelectedItem );
		}

		private void closeSession(Session selectedSession)
		{
			if(selectedSession != null)
			{
				selectedSession.Close();
			}
			else
			{
				MessageBox.Show("Select a session first!");
			}
		}

		private void listSessions_SelectedIndexChanged(object sender, System.EventArgs e)
		{
		
		}

		private void bCloseSession2_Click(object sender, System.EventArgs e)
		{
			closeSession( (Session) listSessions2.SelectedItem );
		}
	}
}
