using System;
using System.IO;
using System.Text;
using Franson.BlueTools;

namespace Chat
{

	public delegate void ProfileChangedHandler(Participant participant);
	public delegate void MessageReceivedHandler(Participant participan, String message);
	public delegate void ParticipantLostHandler(Participant participant);

	/// <summary>
	/// A Participant instance represent a connection between a local and remote device. The
	/// connection can only be made if the remote device supports the ChatService, i.e. a 
	/// service with UUID ChatService.ChatServiceServiceType.
	/// </summary>
	public class Participant
	{

		/// <summary>
		/// Raised whenever the profile of the participant changes.
		/// </summary>
		public event ProfileChangedHandler ProfileChanged;

		/// <summary>
		/// Raised whenever a message is received from the participant.
		/// </summary>
		public event MessageReceivedHandler MessageReceived;

		/// <summary>
		/// Raised whenever the connection to the participant is lost.
		/// </summary>
		public event ParticipantLostHandler Lost;

		/// <summary>
		/// The profile of the participant.
		/// </summary>
		private Profile		profile;

		/// <summary>
		/// The link layer communication link.
		/// </summary>
		private ChatLink	chatLink;

		/// <summary>
		/// The address of the physical device represented by the participant.
		/// </summary>
		private Address		address;

		/// <summary>
		/// Creates a new participant with which communication will occure over the 
		/// provided stream.
		/// </summary>
		/// <param name="stream">The stream over which communication will occure.</param>
		public Participant(Address address, Stream stream)
		{
			this.chatLink = new ChatLink(stream);
			this.address = address;

			chatLink.OnPacketRead += new PacketReadHandler(chatLink_OnPacketRead);
			chatLink.LinkBroken += new LinkBrokenHandler(chatLink_LinkBroken);

			profile = new Profile();
			profile.Name = "Unnamed";
		}

		/// <summary>
		/// Returns the address of the physical device represented by the participant instance.
		/// </summary>
		public Address Address
		{
			get
			{
				return address;
			}
		}

		/// <summary>
		/// Closes the connection to the Participant. After this, no messages can be sent to
		/// the participant, or received from the participant.
		/// </summary>
		public void Close()
		{
			chatLink.Close();
		}

		/// <summary>
		/// Returns the profile of the participant.
		/// </summary>
		public Profile Profile
		{
			get
			{
				return profile;
			}

			set
			{
				profile = value;
			}
		}

		/// <summary>
		/// Sends the provided profile to the participant. This method returns immediatelly.
		/// </summary>
		/// <param name="profile">The profile to send to the participant.</param>
		public void SendProfile(Profile profile)
		{
			Packet packet = new Packet();
			Encoder encoder = Encoding.Unicode.GetEncoder();
			MemoryStream memoryStream = new MemoryStream();
			byte[] nameBuffer = new byte[encoder.GetByteCount(profile.Name.ToCharArray(), 0, profile.Name.Length, true)];

			encoder.GetBytes(profile.Name.ToCharArray(), 0, profile.Name.Length, nameBuffer, 0, true);

			// Write the length of the name
			//
			memoryStream.WriteByte((byte)((nameBuffer.Length >> 8) & 0xFF));
			memoryStream.WriteByte((byte)((nameBuffer.Length) & 0xFF));

			// Write the name buffer
			//
			memoryStream.Write(nameBuffer, 0, nameBuffer.Length);			

			// Configure the packet
			//
			packet.PacketType = PacketType.Profile;
			packet.Data = memoryStream.ToArray();

			chatLink.Write(packet);
		}

		/// <summary>
		/// Sends a message to the Participant. This method will return immediatelly.
		/// </summary>
		/// <param name="message">The message to send to the participant.</param>
		public void SendMessage(String message)
		{
			Packet			packet			= new Packet();
			Encoder			encoder			= Encoding.Unicode.GetEncoder();
			MemoryStream	memoryStream	= new MemoryStream();
			byte[]			messageBuffer	= new byte[encoder.GetByteCount(message.ToCharArray(), 0, message.Length, true)];

			encoder.GetBytes(message.ToCharArray(), 0, message.Length, messageBuffer, 0, true);

			// Write the length of the message
			//
			memoryStream.WriteByte((byte)((messageBuffer.Length >> 8) & 0xFF));
			memoryStream.WriteByte((byte)((messageBuffer.Length >> 0) & 0xFF));

			// Write the message
			//
			memoryStream.Write(messageBuffer, 0, messageBuffer.Length);

			// Configure the packet
			//
			packet.PacketType	= PacketType.Message;
			packet.Data			= memoryStream.ToArray();

			chatLink.Write(packet);
		}

		public override string ToString()
		{
			return Profile.Name;
		}


		#region Events

		private void chatLink_OnPacketRead(Packet packet)
		{
			Decoder decoder = Encoding.Unicode.GetDecoder();

			switch(packet.PacketType)
			{
				case PacketType.Profile:
					int nameLength = packet.Data[0] << 8 | packet.Data[1];
					char[] nameBuffer = new char[decoder.GetCharCount(packet.Data, 2, nameLength)];

					decoder.GetChars(packet.Data, 2, nameLength, nameBuffer, 0);
					profile.Name = new String(nameBuffer);

					if(ProfileChanged != null)
					{
						ProfileChanged(this);
					}

					break;

				case PacketType.Message:
					if(MessageReceived != null)
					{
						int messageLength = packet.Data[0] << 8 | packet.Data[1];
						char[] messageBuffer = new char[decoder.GetCharCount(packet.Data, 2, messageLength)];

						decoder.GetChars(packet.Data, 2, messageLength, messageBuffer, 0);
						String message = new String(messageBuffer);

						MessageReceived(this, message);
					}

					break;
			}
		}

		private void chatLink_LinkBroken(ChatLink chatLink)
		{
			if(Lost != null)
			{
				Lost(this);
			}
		}

		#endregion
	}
}
