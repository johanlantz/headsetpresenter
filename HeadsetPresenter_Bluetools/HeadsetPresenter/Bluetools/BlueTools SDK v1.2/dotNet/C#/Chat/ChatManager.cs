using System;
using System.IO;
using System.Collections;
using Franson.BlueTools;
using System.Threading;

namespace Chat
{

	public delegate void ParticipantDisconnectedHandler(Participant participant);
	
	/// <summary>
	/// The ChatManager manages the spontanuous networks that occure when devices locate
	/// eachother.
	/// </summary>
	public class ChatManager
	{

		public event ParticipantDisconnectedHandler ParticipantDisconnected;

		/// <summary>
		/// Raised whenever a message is received from a device.
		/// </summary>
		public event MessageReceivedHandler MessageReceived;

		/// <summary>
		/// Fired whenever the profile of a device changes.
		/// </summary>
		public event ProfileChangedHandler ProfileChanged;

		/// <summary>
		/// List of all participants.
		/// </summary>
		private ArrayList participants = new ArrayList();

		/// <summary>
		/// The profile of this device.
		/// </summary>
		private Profile	profile	= new Profile();

		/// <summary>
		/// The ChatService that allows devices to connect to this device.
		/// </summary>
		private ChatService	chatService	= new ChatService();

		/// <summary>
		/// The thread in which we're discovering devices.
		/// </summary>
		private Thread discoveryThread = null;

		/// <summary>
		/// Whether or not the chat service should be enabled. This is only
		/// used to exit the threads upon exit.
		/// </summary>
		private bool opened = true;

		/// <summary>
		/// Creates 
		/// </summary>
		public ChatManager()
		{
			// Initialize the profile. We'll name the chat participant "Unnamed" when it
			// has not been set explicitly by the user.
			//
			profile.Name = "Unnamed";

			// Fetch the available networks. This will only return one network in BlueTools 1.0,
			// but always assume several networks might be returned, for compatibility with
			// future versions.
			//
			Network[] networks = Manager.GetManager().Networks;

			// Go through each network available and advertise the chat service so that is availble
			// on each network.
			//
			foreach(Network network in networks)
			{
				network.DeviceDiscoveryStarted += new BlueToolsEventHandler(network_DeviceDiscoveryStarted);

				// Attach a DeviceDiscovered event delegate to the current network. This will enable
				// us to handle device discoveries so we can negotiate a connection with the client.
				//
				network.DeviceDiscovered += new BlueToolsEventHandler(network_DeviceDiscovered);

				// Attach a DeviceList event handler to the current network. This will enable us
				// to handle lost devices. When a device is lost, we will want to disconnect 
				// and remove the device from the list of participants.
				//
				network.DeviceLost += new BlueToolsEventHandler(network_DeviceLost);

				// Advertise the chat service on the server of the current network.
				//

				try
				{
					network.Server.Advertise(chatService);
				} 
				catch (Exception exc)
				{
					System.Windows.Forms.MessageBox.Show("Could not start chat service: " + exc.Message);
				}
				
				// Enable auto discovery on the network. This will cause the network to continuosly
				// search for new devices. This is also necessary to get notified about lost devices.
				//
				network.AutoDiscovery = true;
			}

			// The chat service will negotiate connections with clients. Once a connection has
			// been negotiated, a participant has connected. This will cause the chat manager
			// to download the remote device profile. Once this is done, this event will be fired.
			//
			chatService.ParticipantConnected += new ParticipantConnectedHandler(chatService_ParticipantConnected);
		}


		public bool IsConnected(Address address)
		{
			foreach(Participant p in participants)
			{
				if(p.Address.HostAddress.Equals(address))
				{
					return true;
				}
			}

			return false;
		}

		/// <summary>
		/// Closes the ChatManager and all connections managed by it.
		/// </summary>
		public void Close()
		{
			Console.WriteLine("Closing chat manager");
			opened = false;
			while(participants.Count > 0)
			{
				Participant participant = (Participant)participants[0];
				participants.RemoveAt(0);
				participant.Close();
			}
		}

		/// <summary>
		/// Gets or sets the local profile. Whenever this is set, the new profile will be
		/// sent to all participants.
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
				SendProfile();
			}
		}

		/// <summary>
		/// Returns a list of all participants connected to this device.
		/// </summary>
		public Participant[] Participants
		{
			get
			{
				return (Participant[])participants.ToArray(typeof(Participant));
			}
		}

		/// <summary>
		/// Adds a participant to the network.
		/// </summary>
		/// <param name="participant">The participant to add.</param>
		public void AddParticipant(Participant participant)
		{
			lock(this)
			{
				// This is the only negogation we have. Should be sufficient.
				//
				foreach(Participant other in participants)
				{
					if(other.Address.Equals(participant.Address))
					{
						participant.Close();
						return;
					}
				}

				Console.WriteLine("Adding participant '" + participant.Profile.Name + "'");
				participants.Add(participant);

				participant.MessageReceived += new MessageReceivedHandler(participant_MessageReceived);
				participant.ProfileChanged += new ProfileChangedHandler(participant_ProfileChanged);
				participant.Lost += new ParticipantLostHandler(participant_Lost);

				participant.SendProfile(profile);
			}
		}

		/// <summary>
		/// Sends the local profile to all participants.
		/// </summary>
		public void SendProfile()
		{
			foreach(Participant participant in participants)
			{
				Console.WriteLine("Sending profile to " + participant.Profile.Name);
				participant.SendProfile(profile);
			}
		}

		/// <summary>
		/// Sets a message to all participants.
		/// </summary>
		/// <param name="message">The message to send.</param>
		public void SendMessage(String message)
		{
			foreach(Participant participant in participants)
			{
				participant.SendMessage(message);
			}
		}

		#region Events

		private void participant_MessageReceived(Participant participant, String message)
		{
			if(MessageReceived != null)
			{
				MessageReceived(participant, message);
			}
		}

		private void participant_ProfileChanged(Participant participant)
		{
			if(ProfileChanged != null)
			{
				ProfileChanged(participant);
			}
		}

		private void chatService_ParticipantConnected(Participant participant)
		{
			AddParticipant(participant);
		}

		private void network_DeviceDiscovered(object sender, BlueToolsEventArgs eventArgs)
		{
			RemoteDevice device = (RemoteDevice)((DiscoveryEventArgs)eventArgs).Discovery;
			device.ServiceDiscovered += new BlueToolsEventHandler(device_ServiceDiscovered);

			try
			{
				device.DiscoverServicesAsync(ServiceType.RFCOMM);
			} 
			catch (Exception exc)
			{
				System.Windows.Forms.MessageBox.Show(exc.Message);
			}

			// Perhaps do this in discovery complete?
		}

		private void network_DeviceLost(object sender, BlueToolsEventArgs eventArgs)
		{
			RemoteDevice device = (RemoteDevice)((DiscoveryEventArgs)eventArgs).Discovery;
			device.ServiceDiscovered -= new BlueToolsEventHandler(device_ServiceDiscovered);
		}

		private void device_ServiceDiscovered(object sender, BlueToolsEventArgs eventArgs)
		{
			Device device = (Device)sender;
			RemoteService service = (RemoteService)((DiscoveryEventArgs)eventArgs).Discovery;
			Console.WriteLine("Discovered service " + service.Name + " of device " + device.Name);

			if(service.Name == "Chat Service")
			{
				// Check if we're already connected to the device
				AddParticipant(new Participant(device.Address, service.Stream));
			}
		}

		private void participant_Lost(Participant participant)
		{
			participants.Remove(participant);

			participant.MessageReceived -= new MessageReceivedHandler(participant_MessageReceived);
			participant.ProfileChanged -= new ProfileChangedHandler(participant_ProfileChanged);
			participant.Lost -= new ParticipantLostHandler(participant_Lost);
		}

		#endregion

		private void network_DeviceDiscoveryStarted(object sender, BlueToolsEventArgs eventArgs)
		{
			Console.WriteLine("Looking for devices...");
		}
	}
}
