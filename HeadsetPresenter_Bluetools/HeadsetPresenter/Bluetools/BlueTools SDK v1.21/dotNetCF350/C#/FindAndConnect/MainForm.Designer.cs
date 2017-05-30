namespace FindAndConnect_2008
{
    using Franson.BlueTools;
    using System;

    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            try
            {
                //m_network.Dispose();
                // Manager must be disposed!
                Manager.GetManager().Dispose();
            }
            catch (Exception)
            {
            }

            if (disposing && (components != null))
            {
                components.Dispose();
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
            this.deviceLabel = new System.Windows.Forms.Label();
            this.serviceList = new System.Windows.Forms.ListBox();
            this.networkLabel = new System.Windows.Forms.Label();
            this.deviceList = new System.Windows.Forms.ListBox();
            this.txtPIN = new System.Windows.Forms.TextBox();
            this.serviceOutput = new System.Windows.Forms.TextBox();
            this.bBond = new System.Windows.Forms.Button();
            this.lStatus = new System.Windows.Forms.Label();
            this.closeConnection = new System.Windows.Forms.Button();
            this.discoverDevices = new System.Windows.Forms.Button();
            this.lStackID = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // deviceLabel
            // 
            this.deviceLabel.Location = new System.Drawing.Point(140, 9);
            this.deviceLabel.Name = "deviceLabel";
            this.deviceLabel.Size = new System.Drawing.Size(100, 16);
            this.deviceLabel.TabIndex = 6;
            this.deviceLabel.Text = "Device";
            // 
            // serviceList
            // 
            this.serviceList.Location = new System.Drawing.Point(140, 25);
            this.serviceList.Name = "serviceList";
            this.serviceList.Size = new System.Drawing.Size(120, 147);
            this.serviceList.TabIndex = 7;
            this.serviceList.SelectedIndexChanged += new System.EventHandler(this.serviceList_SelectedIndexChanged);
            // 
            // networkLabel
            // 
            this.networkLabel.Location = new System.Drawing.Point(12, 9);
            this.networkLabel.Name = "networkLabel";
            this.networkLabel.Size = new System.Drawing.Size(100, 16);
            this.networkLabel.TabIndex = 8;
            this.networkLabel.Text = "Network";
            // 
            // deviceList
            // 
            this.deviceList.Location = new System.Drawing.Point(12, 25);
            this.deviceList.Name = "deviceList";
            this.deviceList.Size = new System.Drawing.Size(120, 147);
            this.deviceList.TabIndex = 9;
            this.deviceList.SelectedIndexChanged += new System.EventHandler(this.deviceList_SelectedIndexChanged);
            // 
            // txtPIN
            // 
            this.txtPIN.Location = new System.Drawing.Point(12, 213);
            this.txtPIN.Name = "txtPIN";
            this.txtPIN.Size = new System.Drawing.Size(120, 20);
            this.txtPIN.TabIndex = 27;
            // 
            // serviceOutput
            // 
            this.serviceOutput.Location = new System.Drawing.Point(12, 277);
            this.serviceOutput.Multiline = true;
            this.serviceOutput.Name = "serviceOutput";
            this.serviceOutput.ReadOnly = true;
            this.serviceOutput.Size = new System.Drawing.Size(248, 40);
            this.serviceOutput.TabIndex = 23;
            // 
            // bBond
            // 
            this.bBond.Location = new System.Drawing.Point(12, 237);
            this.bBond.Name = "bBond";
            this.bBond.Size = new System.Drawing.Size(120, 24);
            this.bBond.TabIndex = 26;
            this.bBond.Text = "Bond";
            this.bBond.Click += new System.EventHandler(this.bBond_Click);
            // 
            // lStatus
            // 
            this.lStatus.Location = new System.Drawing.Point(12, 325);
            this.lStatus.Name = "lStatus";
            this.lStatus.Size = new System.Drawing.Size(248, 40);
            this.lStatus.TabIndex = 25;
            this.lStatus.Text = "Click Discover!";
            // 
            // closeConnection
            // 
            this.closeConnection.Enabled = false;
            this.closeConnection.Location = new System.Drawing.Point(140, 181);
            this.closeConnection.Name = "closeConnection";
            this.closeConnection.Size = new System.Drawing.Size(120, 23);
            this.closeConnection.TabIndex = 22;
            this.closeConnection.Text = "Close";
            this.closeConnection.Click += new System.EventHandler(this.closeConnection_Click);
            // 
            // discoverDevices
            // 
            this.discoverDevices.Location = new System.Drawing.Point(12, 181);
            this.discoverDevices.Name = "discoverDevices";
            this.discoverDevices.Size = new System.Drawing.Size(120, 23);
            this.discoverDevices.TabIndex = 24;
            this.discoverDevices.Text = "Discover";
            this.discoverDevices.Click += new System.EventHandler(this.discoverDevices_Click);
            // 
            // lStackID
            // 
            this.lStackID.Location = new System.Drawing.Point(12, 372);
            this.lStackID.Name = "lStackID";
            this.lStackID.Size = new System.Drawing.Size(248, 16);
            this.lStackID.TabIndex = 28;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(272, 397);
            this.Controls.Add(this.lStackID);
            this.Controls.Add(this.txtPIN);
            this.Controls.Add(this.serviceOutput);
            this.Controls.Add(this.bBond);
            this.Controls.Add(this.lStatus);
            this.Controls.Add(this.closeConnection);
            this.Controls.Add(this.discoverDevices);
            this.Controls.Add(this.deviceLabel);
            this.Controls.Add(this.serviceList);
            this.Controls.Add(this.networkLabel);
            this.Controls.Add(this.deviceList);
            this.Name = "MainForm";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label deviceLabel;
        private System.Windows.Forms.ListBox serviceList;
        private System.Windows.Forms.Label networkLabel;
        private System.Windows.Forms.ListBox deviceList;
        private System.Windows.Forms.TextBox txtPIN;
        private System.Windows.Forms.TextBox serviceOutput;
        private System.Windows.Forms.Button bBond;
        private System.Windows.Forms.Label lStatus;
        private System.Windows.Forms.Button closeConnection;
        private System.Windows.Forms.Button discoverDevices;
        private System.Windows.Forms.Label lStackID;
    }
}