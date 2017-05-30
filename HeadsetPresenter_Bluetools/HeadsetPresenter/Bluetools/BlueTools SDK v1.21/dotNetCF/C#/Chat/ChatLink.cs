using System;
using System.IO;
using System.Threading;
using System.Collections;

namespace Chat
{

	public delegate void PacketReadHandler(Packet packet);
	public delegate void LinkBrokenHandler(ChatLink chatLink);

	/// <summary>
	/// ChatLink constitutes the link layer communication protocol and enables secure transmission
	/// of packets. Each packet sent through the ChatLink has a strict structure, and once sent,
	/// the ChatLink will block until an acknowledgement on the packet has been received.
	/// </summary>
	public class ChatLink
	{

		/// <summary>
		/// A DataStream instance that allows us to read primitives from the stream.
		/// </summary>
		private DataStream dataStream = null;

		/// <summary>
		/// We'll synchronize with this event when awaiting an acknowledgement
		/// </summary>
		private ManualResetEvent ackEvent = new ManualResetEvent(false);

		/// <summary>
		/// The latest ack that was received
		/// </summary>
		private Packet ack = null;

		/// <summary>
		/// The number of packets written. Used to compute a sequence number for outgoing packages.
		/// </summary>
		private byte packetsWritten	= 0;

		/// <summary>
		/// A queue for outgoing packages. We'll let a thread consume packets available on this queue.
		/// This is necessary since we don't want to block the main thread.
		/// </summary>
		private ArrayList writeQueue = new ArrayList();

		/// <summary>
		/// The TX thread will synchronize against this event.
		/// </summary>
		private ManualResetEvent writeEvent	= new ManualResetEvent(false);

		/// <summary>
		/// Whether or not the link is opened
		/// </summary>
		private bool opened = true;

		/// <summary>
		/// Event that is raised whenever a packet is read
		/// </summary>
		public event PacketReadHandler OnPacketRead;

		/// <summary>
		/// Event that is raised whenever the link is broken.
		/// </summary>
		public event LinkBrokenHandler LinkBroken;

		/// <summary>
		/// Creates a new chat link that operates on the provided stream.
		/// </summary>
		/// <param name="stream">The stream from which to read data from and write data to.</param>
		public ChatLink(Stream stream)
		{
			dataStream = new DataStream(stream);

			new Thread(new ThreadStart(ReadFromStream)).Start();
			new Thread(new ThreadStart(WriteToStream)).Start();
		}

		/// <summary>
		/// Write a packet to the stream. This method will write the packet and await an acknowledgement.
		/// </summary>
		/// <param name="packet">The packet to write.</param>
		public void Write(Packet packet)
		{
			writeQueue.Add(packet);
			writeEvent.Set();
		}

		/// <summary>
		/// Closes the link.
		/// </summary>
		public void Close()
		{
			opened = false;

			// Release any threads that might be blocked
			//
			writeEvent.Set();
			ackEvent.Set();

			dataStream.Close();
			
			if(LinkBroken != null)
			{
				LinkBroken(this);
			}
		}

		#region Private methods

		private void WriteToStream()
		{
			while(opened)
			{
				while(writeQueue.Count > 0)
				{
					Packet packet = (Packet)writeQueue[0];
					
					writeQueue.RemoveAt(0);

					try
					{
						Write0(packet);					
					} 
					catch (Exception)
					{
						Close();
						break;
					}
				}

				writeEvent.WaitOne();
				writeEvent.Reset();
			}
		}

		private void Write0(Packet packet)
		{
			bool acknowledged = false;
			while(opened && !acknowledged)
			{
				ackEvent.Reset();

				// Assign a sequence number
				//
				packet.SequenceNumber = packetsWritten++;

				lock(this)
				{
					// Write the start bytes
					//
					dataStream.WriteShort(0x1003);

					// Start computing checksum
					//
					dataStream.ComputeOutputChecksum = true;

					// Write the contents of the packet
					//

					dataStream.WriteByte((byte)packet.PacketType);
					dataStream.WriteByte(packet.SequenceNumber);
					dataStream.WriteShort((short)packet.Data.Length);
					dataStream.Write(packet.Data, 0, packet.Data.Length);

					// Write the checksum
					//
					dataStream.WriteInt(dataStream.OutputChecksum);

					// Write the end-of-packet bytes
					//
					dataStream.WriteByte(0x03);
				}

				dataStream.Flush();

				// Check if we're sending an ack
				//
				if(packet.PacketType == PacketType.Ack || packet.PacketType == PacketType.Nak)
				{
					break;
				}

				// Await an ack
				//
				ackEvent.WaitOne();
				ackEvent.Reset();

				acknowledged = (ack.PacketType == PacketType.Ack);
			}
		}

		private void SendAck(int sequenceNumber, bool positive)
		{
			Packet packet		= new Packet();
			packet.PacketType	= (positive ? PacketType.Ack : PacketType.Nak);
			packet.Data			= new byte[] {	 
												 (byte)(( sequenceNumber >> 24 ) & 0xFF), 
												 (byte)(( sequenceNumber >> 16 ) & 0xFF), 
												 (byte)(( sequenceNumber >> 8  ) & 0xFF), 
												 (byte)(( sequenceNumber >> 0  ) & 0xFF)
											 };

			Write0(packet);
		}

		private void ReadFromStream()
		{
			while(opened)
			{
				Packet packet = null;

				try
				{
					packet = Read0();
				} 
				catch (Exception)
				{
					Close();
					break;
				}

				if(packet == null)
				{
					break;
				} 
				else
				{
					if(packet.PacketType == PacketType.Ack || packet.PacketType == PacketType.Nak)
					{
						ack = packet;
						ackEvent.Set();
					} 
					else
					{
						if(OnPacketRead != null)
						{
							OnPacketRead(packet);
						}
					}
				}
			}
		}

		private Packet Read0()
		{
			while(opened && dataStream.Await(new byte[] { 0x10, 0x03 }))
			{
				Packet	packet	= new Packet();
				int		length	= 0;

				// Start computing checksum on the incoming data
				//
				dataStream.ComputeInputChecksum = true;

				// Read the contents of the packet
				//

				packet.PacketType = (PacketType)dataStream.ReadByte();
				packet.SequenceNumber = (byte)dataStream.ReadByte();
				length = dataStream.ReadShort();

				// Check that the length is less than the MTU
				//
				if(length <= 4096)
				{
					// Allocate a new buffer for the data
					//
					packet.Data = new byte[length];

					// Read the contents of the packet
					//
					dataStream.Read(packet.Data, 0, length);

					// Fetch the computed input checksum
					//
					int checksum = dataStream.InputChecksum;

					// Read the provided input checksum
					//
					packet.Checksum = dataStream.ReadInt();

					// Verify that the next byte is an EOP identifier
					//
					if(dataStream.ReadByte() == 0x03)
					{
						if(packet.Checksum == checksum)
						{
							if(packet.PacketType != PacketType.Ack && packet.PacketType != PacketType.Nak)
							{
								SendAck(packet.SequenceNumber, true);
							} 

							return packet;
						}
					}
				}

				if(packet.PacketType != PacketType.Ack && packet.PacketType != PacketType.Nak)
				{
					SendAck(packet.SequenceNumber, false);
				}
			}

			return null;
		}
	}

	#endregion
}
