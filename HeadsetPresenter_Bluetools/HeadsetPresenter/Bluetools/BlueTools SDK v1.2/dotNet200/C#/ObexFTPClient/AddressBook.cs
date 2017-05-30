using System;
using System.Collections;
using System.Xml;
using System.IO;
using Franson.BlueTools;

namespace ObexFTPClientSample200
{
	/// <summary>
	/// ObexDevice stores the address to a bluetooth supported device.
	/// </summary>
	public class ObexDevice
	{
		private Address m_address = null;
		private string m_name = "";
		
		/// <summary>
		/// Constructs new ObexDevice objects.
		/// </summary>
		/// <param name="address"></param>
		/// <param name="name"></param>
 
		public ObexDevice(Address address, string name)
		{
			// store an Address to the address stored in the xml file.
			m_address = address;

			// store name
			m_name = name;
		}

		/// <summary>
		/// Returns the name of the device stored by this instance of ObexDevice.
		/// </summary>
		public string Name
		{
			get
			{
				return m_name;
			}
		}

		/// <summary>
		/// Returns the Address stored by this instance of ObexDevice.
		/// </summary>
		public Address Address
		{
			get
			{
				return m_address;
			}
		}
	}

	/// <summary>
	/// AddressBook is used to store Bluetooth adresses so we can avoid doing a device discovery.
	/// </summary>
	public class AddressBook
	{
		private Hashtable m_hashAddressList = null;

		/// <summary>
		/// Creates a new instance of AddressBook. Use it to store devices you have already found. This way you can avoid doing
		/// device discovery.
		/// </summary>
		public AddressBook()
		{
			// create hash table to store addresses
			m_hashAddressList = new Hashtable();				
		}

		/// <summary>
		/// Load existing addresses from xml-file.
		/// </summary>
		public void Load()
		{
			XmlTextReader readerXML = new XmlTextReader("addressbook.xml");

			try
			{
				// check if AddressBook xml-file exists
				if (File.Exists("addressbook.xml"))
				{
					XmlDocument xmlDoc = new XmlDocument();

					try
					{
						xmlDoc.Load(readerXML);
					}
					catch (Exception exc)
					{
						Console.WriteLine(exc.Message);
					}

					XmlNodeList xmlDeviceList = xmlDoc.GetElementsByTagName("Devices");

					if (xmlDeviceList.Count > 0)
					{
						XmlNode xmlRootNode = xmlDeviceList.Item(0);
					
						foreach (XmlNode node in xmlRootNode.ChildNodes)
						{
							if (node.Attributes["address"] != null && node.Attributes["name"] != null)
							{
								m_hashAddressList.Add(node.Attributes["address"].Value, new ObexDevice(new Address(node.Attributes["address"].Value), node.Attributes["name"].Value));
							}

							
						}
					}
				}
			}
			catch
			{
				// all exceptions are treated as the file is corrupt.
				// thus we clean the address list.
				m_hashAddressList.Clear();
			}

			readerXML.Close();
		}

		/// <summary>
		/// Empties the address book.
		/// </summary>
		public void Clear()
		{
			m_hashAddressList.Clear();
		}

		/// <summary>
		/// Adds an Address object to the hash. If the object already exists, this will not create a duplicate.
		/// </summary>
		/// <param name="address">An address to store.</param>
		public void Add(Address address, string name)
		{
			// store address with string of address as key in hash
			// if the address already exists in the hash, the new address it not added
			// this is irrelevant though since they refer to the same device (Bluetooth addresses are unique!)

			if (!m_hashAddressList.ContainsKey(address.ToString()))
				m_hashAddressList.Add(address.ToString(), new ObexDevice(address, name));
		}

		/// <summary>
		/// Returns an Address object for the address given.
		/// </summary>
		/// <param name="address"></param>
		/// <returns>If address is known it returns an Address instance. Else it returns null.</returns>
		public Address Get(string address)
		{
			if (m_hashAddressList.ContainsKey(address))
				return (Address)m_hashAddressList[address];
			else
				return null;
		}

		/// <summary>
		/// Returns an ObexDevice[] with all devices stored in the AddressBook.
		/// </summary>
		public ObexDevice[] Devices
		{
			get
			{
				return (ObexDevice[])new ArrayList(m_hashAddressList.Values).ToArray(typeof(ObexDevice));
			}
		}

		/// <summary>
		/// Close is used when closing the AddressBook and storing the data to a xml-file.
		/// </summary>
		public void Close()
		{
			XmlTextWriter writer = new XmlTextWriter("addressbook.xml", null);

			writer.WriteStartDocument();

			writer.WriteStartElement("Devices");

			foreach(ObexDevice device in Devices)
			{
				writer.WriteStartElement("Device");
				writer.WriteAttributeString("name", device.Name);
				writer.WriteAttributeString("address", device.Address.ToString());
				writer.WriteEndElement();
			}

			writer.WriteEndElement();
			writer.WriteEndDocument();
			writer.Close();
		}
	}
}
