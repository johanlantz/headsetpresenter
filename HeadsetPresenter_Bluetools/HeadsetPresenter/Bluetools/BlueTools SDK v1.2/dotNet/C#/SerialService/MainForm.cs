using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using SerialNET;
using Franson.BlueTools;

namespace SerialService
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class MainForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button startService;
		private System.Windows.Forms.Button stopService;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.ComboBox serialPortList;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		Manager			manager;
		Network[]		networks;
		LocalService	service;

		public MainForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			Closing += new CancelEventHandler(MainForm_Closing);
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			manager.Dispose();
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
			this.startService = new System.Windows.Forms.Button();
			this.stopService = new System.Windows.Forms.Button();
			this.serialPortList = new System.Windows.Forms.ComboBox();
			this.label1 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// startService
			// 
			this.startService.Location = new System.Drawing.Point(8, 56);
			this.startService.Name = "startService";
			this.startService.TabIndex = 0;
			this.startService.Text = "Start";
			this.startService.Click += new System.EventHandler(this.startService_Click);
			// 
			// stopService
			// 
			this.stopService.Location = new System.Drawing.Point(88, 56);
			this.stopService.Name = "stopService";
			this.stopService.TabIndex = 1;
			this.stopService.Text = "Stop";
			this.stopService.Click += new System.EventHandler(this.stopService_Click);
			// 
			// serialPortList
			// 
			this.serialPortList.Location = new System.Drawing.Point(8, 24);
			this.serialPortList.Name = "serialPortList";
			this.serialPortList.Size = new System.Drawing.Size(160, 21);
			this.serialPortList.TabIndex = 2;
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(8, 8);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(100, 16);
			this.label1.TabIndex = 3;
			this.label1.Text = "Serial Port";
			// 
			// MainForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(184, 102);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.serialPortList);
			this.Controls.Add(this.stopService);
			this.Controls.Add(this.startService);
			this.Name = "MainForm";
			this.Text = "Serial Service";
			this.Load += new System.EventHandler(this.MainForm_Load);
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
			startService.Enabled = true;
			stopService.Enabled = false;

			byte[] ports = Port.List;
			for(int i = 0; i < ports.Length; i++)
			{
				serialPortList.Items.Add("COM" + ports[i]);
			}

			// You can get a valid evaluation key for BlueTools at
			// http://franson.com/bluetools/
			// That key will be valid for 14 days. Just cut and paste that key into the statement below.
			// To get a key that do not expire you need to purchase a license
			Franson.BlueTools.License bluetoolsLicense = new Franson.BlueTools.License();
			bluetoolsLicense.LicenseKey = "WoK6HM24B9ECLOPQSXVZPixRDRfuIYHYWrT9";

			// You can get a valid evaluation key for SerialTools at
			// http://franson.com/serialtools/
			// That key will be valid for 14 days. Just cut and paste that key into the statement below.
			// To get a key that do not expire you need to purchase a license
			SerialNET.License serialNetLicense = new SerialNET.License();
			serialNetLicense.LicenseKey = "SfHBmVyklQfUSYkv63PXn4jZo4ndCTxZSjHB";

			try
			{
				manager	= Manager.GetManager();
				manager.Parent = this;

				networks = manager.Networks;
			} 
			catch (BlueToolsException exc)
			{
				MessageBox.Show(exc.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
			}
		}

		private void startService_Click(object sender, System.EventArgs e)
		{
			if(service != null)
			{
				MessageBox.Show("The service has already been started");
			}
			else
			{
				Port port = new Port();

				port.BaudRate	= 4800;
				port.ComPort	= Int32.Parse(serialPortList.SelectedItem.ToString().Substring(3));

				service = new SerialService(port);

				startService.Enabled = false;
				stopService.Enabled = true;

				try
				{
					foreach(Network network in networks)
					{
						network.Server.Advertise(service);
					}
				} 
				catch (Exception exc)
				{
					  MessageBox.Show(exc.Message);
				}
			}
		}

		private void MainForm_Closing(object sender, CancelEventArgs e)
		{
			Dispose();
		}

		private void stopService_Click(object sender, System.EventArgs e)
		{
			if(service == null)
			{
				MessageBox.Show("The service has not been started");
			}
			else
			{
				foreach(Network network in networks)
				{
					network.Server.Deadvertise(service);
				}

				service = null;
				startService.Enabled = true;
				stopService.Enabled = false;
			}
		}
	}
}
