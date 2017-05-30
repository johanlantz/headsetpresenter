using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace ObexFTPClientSample200
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class InputForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button okBtn;
		private System.Windows.Forms.TextBox textBox;
		private System.Windows.Forms.Label informLabel;
		private System.Windows.Forms.Button cancelBtn;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public string FolderName
		{
			get
			{
				return textBox.Text;
			}
		}

		public InputForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
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
            this.textBox = new System.Windows.Forms.TextBox();
            this.informLabel = new System.Windows.Forms.Label();
            this.okBtn = new System.Windows.Forms.Button();
            this.cancelBtn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // textBox
            // 
            this.textBox.Location = new System.Drawing.Point(16, 32);
            this.textBox.Name = "textBox";
            this.textBox.Size = new System.Drawing.Size(216, 20);
            this.textBox.TabIndex = 0;
            this.textBox.Text = "New folder";
            // 
            // informLabel
            // 
            this.informLabel.Location = new System.Drawing.Point(16, 8);
            this.informLabel.Name = "informLabel";
            this.informLabel.Size = new System.Drawing.Size(160, 23);
            this.informLabel.TabIndex = 1;
            this.informLabel.Text = "Input name of new sub-folder:";
            // 
            // okBtn
            // 
            this.okBtn.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.okBtn.Location = new System.Drawing.Point(78, 63);
            this.okBtn.Name = "okBtn";
            this.okBtn.Size = new System.Drawing.Size(75, 23);
            this.okBtn.TabIndex = 2;
            this.okBtn.Text = "Ok";
            // 
            // cancelBtn
            // 
            this.cancelBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelBtn.Location = new System.Drawing.Point(159, 63);
            this.cancelBtn.Name = "cancelBtn";
            this.cancelBtn.Size = new System.Drawing.Size(75, 23);
            this.cancelBtn.TabIndex = 3;
            this.cancelBtn.Text = "Cancel";
            // 
            // InputForm
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(240, 93);
            this.Controls.Add(this.cancelBtn);
            this.Controls.Add(this.okBtn);
            this.Controls.Add(this.informLabel);
            this.Controls.Add(this.textBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "InputForm";
            this.ShowInTaskbar = false;
            this.Text = "Input";
            this.TopMost = true;
            this.Activated += new System.EventHandler(this.InputForm_Activated);
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		#endregion

		private void InputForm_Activated(object sender, System.EventArgs e)
		{
			// set focus to ok button
			okBtn.Focus();
		}

	}
}
