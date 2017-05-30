using System;
using System.Windows.Forms;
using System.IO;

namespace Chat
{
	public class MessageStream : TextWriter
	{

		private Control control;
		private String text;
		private EventHandler eventHandler = null;

		public MessageStream(Control control)
		{
			this.control = control;
			eventHandler = new EventHandler(UpdateControl);
		}
	
		public override System.Text.Encoding Encoding
		{
			get
			{
				return System.Text.Encoding.Unicode;
			}
		}

		public override void Write(char[] buffer, int index, int count)
		{
			text = new String(buffer, index, count);
			control.Invoke(eventHandler);
		}

		private void UpdateControl(object sender, EventArgs eventArgs)
		{
			control.Text = text;
		}

	};
}
