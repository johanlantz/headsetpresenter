using System;

namespace Chat
{

	public enum PacketType : byte
	{
		Ack,
		Nak,
		Profile,
		Message
	}

	/// <summary>
	/// A Packet represents a packet of data that is transmitted over the bluetooth link. Besides the data,
	/// it also contains start and stop bytes, a packet sequence number, a packet type, a length and
	/// a checksum. The layout is:
	/// 
	/// 0x10 | 0x03 | PacketType | Sequence | Length | Data | Checksum | 0x03
	/// </summary>
	public class Packet
	{

		private PacketType	packetType;
		private byte		sequenceNumber;
		private byte[]		data;
		private int			checksum;

		public Packet()
		{
		}

		public PacketType PacketType
		{
			get
			{
				return packetType;
			}

			set
			{
				packetType = value;
			}
		}

		public byte SequenceNumber
		{
			get
			{
				return sequenceNumber;
			}

			set
			{
				sequenceNumber = value;
			}
		}

		public byte[] Data
		{
			get
			{
				return data;
			}

			set
			{
				data = value;
			}
		}

		public int Checksum
		{
			get
			{
				return checksum;
			}

			set
			{
				checksum = value;
			}
		}
	}
}
