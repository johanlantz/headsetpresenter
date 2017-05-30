using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace ObexFTPClientSample
{
	/// <summary>
	/// Summary description for InputForm.
	/// </summary>
	public class InputForm : System.Windows.Forms.Form
	{
		internal System.Windows.Forms.Button okBtn;
		internal System.Windows.Forms.Button cancelBtn;
		internal System.Windows.Forms.TextBox textBox;
		internal System.Windows.Forms.Label informLabel;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public InputForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		public string FolderName
		{
			get
			{
				return textBox.Text;
			}
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
			this.okBtn = new System.Windows.Forms.Button();
			this.cancelBtn = new System.Windows.Forms.Button();
			this.textBox = new System.Windows.Forms.TextBox();
			this.informLabel = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// okBtn
			// 
			this.okBtn.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.okBtn.Location = new System.Drawing.Point(152, 64);
			this.okBtn.Name = "okBtn";
			this.okBtn.TabIndex = 9;
			this.okBtn.Text = "Ok";
			// 
			// cancelBtn
			// 
			this.cancelBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.cancelBtn.Location = new System.Drawing.Point(72, 64);
			this.cancelBtn.Name = "cancelBtn";
			this.cancelBtn.TabIndex = 8;
			this.cancelBtn.Text = "Cancel";
			// 
			// textBox
			// 
			this.textBox.Location = new System.Drawing.Point(8, 32);
			this.textBox.Name = "textBox";
			this.textBox.Size = new System.Drawing.Size(216, 20);
			this.textBox.TabIndex = 7;
			this.textBox.Text = "New folder";
			// 
			// informLabel
			// 
			this.informLabel.Location = new System.Drawing.Point(8, 8);
			this.informLabel.Name = "informLabel";
			this.informLabel.Size = new System.Drawing.Size(158, 23);
			this.informLabel.TabIndex = 6;
			this.informLabel.Text = "Input name of new sub-folder:";
			// 
			// InputForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(232, 93);
			this.Controls.Add(this.okBtn);
			this.Controls.Add(this.cancelBtn);
			this.Controls.Add(this.textBox);
			this.Controls.Add(this.informLabel);
			this.Name = "InputForm";
			this.Text = "InputForm";
			this.ResumeLayout(false);

		}
		#endregion
	}
}
