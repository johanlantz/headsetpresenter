using System;
using Franson.BlueTools;

namespace Chat
{

	public delegate void ParticipantConnectedHandler(Participant participant);

	/// <summary>
	/// Summary description for ChatService.
	/// </summary>
	public class ChatService : LocalService
	{

		public event ParticipantConnectedHandler ParticipantConnected;

		public ChatService() : base(ChatService.ChatServiceServiceType, "Chat Service", "A Simple Chat Service")
		{
		}

		protected override void OnClientConnected(Session session)
		{
			if(ParticipantConnected != null)
			{
				ParticipantConnected(new Participant(session.RemoteDevice.Address, session.Stream));
			}
		}

		public static ServiceType ChatServiceServiceType
		{
			get
			{
				return new ServiceType(new Guid(0x15646874, 0x0000, 0x1000, 0x80, 0x00, 0x00, 0x95, 0x5C, 0xAB, 0x31, 0xF0), "TCP");
			}
		}

	}
}
