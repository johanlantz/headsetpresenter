using System;
using System.Threading;
using Franson.BlueTools;
using SerialNET;
using System.IO;
using System.Text;

namespace SerialService
{
	/// <summary>
	/// 
	/// </summary>
	public class SerialService : LocalService
	{

		private Port		port;
		private Session		session;
		private bool		opened;

		public SerialService(Port port) : base(ServiceType.SerialPort, "Serial Service", "Simple BlueTools Serial Service")
		{
			this.port		= port;
		}

		protected override void OnClientConnected(Session session)
		{
			if(opened)
			{
				session.Close();
			}
			else
			{
				this.session	= session;
				this.opened		= true;

				new Thread(new ThreadStart(HandleClient)) .Start();
			}
		}

		protected override void OnDeadvertised(Server server)
		{
		}
		
		private void HandleClient()
		{
			Stream	stream	= session.Stream;
			Encoder	encoder	= Encoding.ASCII.GetEncoder();

			port.Enabled = true;

			while(opened)
			{
				String	data	= port.Read(0, 10000);
				char[]	chars	= data.ToCharArray();
                int		count	= encoder.GetByteCount(chars, 0, chars.Length, true);
				byte[]	buffer	= new byte[count];

				encoder.GetBytes(chars, 0, chars.Length, buffer, 0, true);
				
				try
				{
					stream.Write(buffer, 0, buffer.Length);
				} 
				catch (Exception)
				{
					break;
				}
			}

			session.Close();

			this.opened = false;
			this.session = null;
		}

	}
}
