using System;
using System.IO;
using System.Text;

namespace Chat
{

	/// <summary>
	/// Summary description for DataStream.
	/// </summary>
	public class DataStream : Stream
	{

		private Stream	source;
		private bool	computeInputChecksum;
		private bool	computeOutputChecksum;
		private int		inputChecksum;
		private int		outputChecksum;

		public DataStream(Stream source)
		{
			this.source = source;
		}

		/// <summary>
		/// Gets the source stream that provides data to this stream.
		/// </summary>
		public Stream Source
		{
			get
			{
				return source;
			}
		}

		public override void Close()
		{
			base.Close ();
			source.Close();
		}


		/// <summary>
		/// Awaits the provided byte sequence. If the sequence is found in the stream, true will be returned.
		/// If the stream is closed before the sequence was found, false will be returned. This is useful when
		/// delimiting packets with start bytes in packet based communication protocols.
		/// </summary>
		/// <param name="sequence">The byte sequence to a await.</param>
		/// <returns>Whether or not the sequence was found. If not, the stream has been closed.</returns>
		public bool Await(byte[] sequence)
		{
			for(int cursor = 0;;)
			{
				int data = ReadByte();
				if(data == -1)
				{
					return false;
				}
				else
				{
					if(data == sequence[cursor++])
					{
						if(cursor == sequence.Length)
						{
							return true;
						}
					}
					else
					{
						cursor = 0;
					}
				}
			}
		}

		#region Checksum

		/// <summary>
		/// Gets or sets whether or not to compute a checksum on incoming bytes. If set, each byte read will affect
		/// the internal input checksum. This is useful when comparing an incoming checksum against a 
		/// computed checksum.
		/// </summary>
		public bool ComputeInputChecksum
		{
			get
			{
				return computeInputChecksum;
			}

			set
			{
				ResetInputChecksum();
				computeInputChecksum = value;
			}
		}

		/// <summary>
		/// Gets or sets whether or not to compute a checksum on outgoing bytes. If set, each byte written will
		/// affect the internal output checksum. This is useful when appending a checksum on a packet.
		/// </summary>
		public bool ComputeOutputChecksum
		{
			get
			{
				return computeOutputChecksum;
			}

			set
			{
				ResetOutputChecksum();
				computeOutputChecksum = value;
			}
		}

		/// <summary>
		/// Gets the output checksum, i.e. the checksum that is computed on outgoing data.
		/// </summary>
		public int OutputChecksum
		{
			get
			{
				return outputChecksum;
			}
		}

		/// <summary>
		/// Returns the input checkum, i.e. the checksum that is computed on incoming data.
		/// </summary>
		public int InputChecksum
		{
			get
			{
				return inputChecksum;
			}
		}

		/// <summary>
		/// Resets the input checksum.
		/// </summary>
		public void ResetInputChecksum()
		{
			inputChecksum = 0;
		}

		/// <summary>
		/// Resets the output checksum.
		/// </summary>
		public void ResetOutputChecksum()
		{
			outputChecksum = 0;
		}

		#endregion

		#region Read and Write Data

		public void WriteShort(short data)
		{
			Write(new byte[] { (byte)((data >> 8) & 0xFF), (byte)((data) & 0xFF) }, 0, 2);
		}

		public void WriteInt(int data)
		{
			Write(new byte[] { (byte)((data >> 24) & 0xFF), (byte)((data >> 16) & 0xFF), (byte)((data >> 8) & 0xFF), (byte)((data) & 0xFF) }, 0, 4);
		}

		public void WriteUTF8(String str)
		{
			WriteString(str, Encoding.UTF8);
		}

		public void WriteUnicode(String str)
		{
			WriteString(str, Encoding.Unicode);
		}

		public void WriteString(String str, Encoding encoding)
		{
			Encoder	encoder		= encoding.GetEncoder();
			char[]	characters	= str.ToCharArray();
			byte[]	buffer		= new byte[encoder.GetByteCount(characters, 0, characters.Length, true)];

			encoder.GetBytes(characters, 0, characters.Length, buffer, 0, true);

			WriteShort((short)buffer.Length);
			Write(buffer, 0, buffer.Length);
		}

		public String ReadUTF8()
		{
			return ReadString(Encoding.UTF8);
		}

		public String ReadUnicode()
		{
			return ReadString(Encoding.Unicode);
		}

		public String ReadString(Encoding encoding)
		{
			// Create a new decoder that can decode encoded strings
			//
			Decoder decoder = encoding.GetDecoder();
			
			// Read the length of the encoded string
			//
			int length = ReadShort();

			// Allocate a new buffer for the encoded string
			//
			byte[] buffer = new byte[length];

			// Read the encoded string
			//
			Read(buffer, 0, length);

			// Allocate a buffer for the decoded string
			//
			char[] characters = new char[decoder.GetCharCount(buffer, 0, buffer.Length)];

			// Decode the string
			//
			decoder.GetChars(buffer, 0, length, characters, 0);

			return new String(characters);
		}

		public short ReadShort()
		{
			int high	= ReadByte();
			int low		= ReadByte();

			if(high == -1 || low == -1)
			{
				throw new IOException("End of file");
			}
			else
			{
				return (short)(high << 8 | low);
			}
		}

		public int ReadInt()
		{
			int xhigh	= ReadByte();
			int xlow	= ReadByte();
			int high	= ReadByte();
			int low		= ReadByte();

			if(xhigh == -1 || xlow == -1 || high == -1 || low == -1)
			{
				throw new IOException("End of file");
			}
			else
			{
				return xhigh << 24 | xlow << 16 | high << 8 | low;
			}
		}


		#endregion

		#region Stream Members

		public override long Length
		{
			get
			{
				return source.Length;
			}
		}

		public override bool CanRead
		{
			get
			{
				return source.CanRead;
			}
		}

		public override bool CanSeek
		{
			get
			{
				return source.CanSeek;
			}
		}

		public override bool CanWrite
		{
			get
			{
				return source.CanWrite;
			}
		}

		public override long Position
		{
			get
			{
				return source.Position;
			}
			set
			{
				source.Position = value;
			}
		}

		public override void Flush()
		{
			source.Flush();
		}

		public override int Read(byte[] buffer, int offset, int count)
		{
			int bytesRead = source.Read(buffer, offset, count);
			if(ComputeInputChecksum)
			{
				for(int i = offset; i < bytesRead; i++)
				{
					inputChecksum += inputChecksum ^ buffer[i];
				}
			}

			return bytesRead;
		}

		public override void Write(byte[] buffer, int offset, int count)
		{
			source.Write(buffer, offset, count);
			if(ComputeOutputChecksum)
			{
				for(int i = offset; i < count; i++)
				{
					outputChecksum += outputChecksum ^ buffer[i];
				}
			}
		}

		public override long Seek(long offset, SeekOrigin origin)
		{
			return source.Seek(offset, origin);
		}

		public override void SetLength(long value)
		{
			source.SetLength(value);
		}


		#endregion

	}
}
