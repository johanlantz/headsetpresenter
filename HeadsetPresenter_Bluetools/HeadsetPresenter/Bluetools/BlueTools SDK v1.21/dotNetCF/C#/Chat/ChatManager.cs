using System;
using System.IO;
using System.Collections;
using Franson.BlueTools;

namespace Chat
{
	
	/// <summary>
	/// The ChatManager manages the spontanuous networks that occure when devices locate
	/// eachother.
	/// </summary>
	public class ChatManager
	{

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
		private ArrayList		participants	= new ArrayList();

		/// <summary>
		/// The profile of this device.
		/// </summary>
		private Profile			profile			= new Profile();

		/// <summary>
		/// The ChatService that allows devices to connect to this device.
		/// </summary>
		private ChatService		chatService		= new ChatService();

		private ArrayList newDevices = new ArrayList();

		/// <summary>
		/// Creates 
		/// </summary>
		public ChatManager()
		{
			// Initialize the profile
			//
			profile.Name = "Unnamed";

			Network[] networks = Manager.GetManager().Networks;
			foreach(Network network in networks)
			{
				network.DeviceDiscovered += new BlueToolsEventHandler(network_DeviceDiscovered);
				network.DeviceLost += new BlueToolsEventHandler(network_DeviceLost);

				network.Server.Advertise(chatService);
				//network.AutoDiscovery = true;
				network.DiscoverDevicesAsync();

				network.DeviceDiscoveryCompleted += new BlueToolsEventHandler(network_DeviceDiscoveryCompleted);
			}

			chatService.ParticipantConnected += new ParticipantConnectedHandler(chatService_ParticipantConnected);
		}

		/// <summary>
		/// Closes the ChatManager and all connections managed by it.
		/// </summary>
		public void Close()
		{
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
			newDevices.Add(device);
		}

		private void network_DeviceLost(object sender, BlueToolsEventArgs eventArgs)
		{
			RemoteDevice device = (RemoteDevice)((DiscoveryEventArgs)eventArgs).Discovery;
			device.ServiceDiscovered -= new BlueToolsEventHandler(device_ServiceDiscovered);

			System.Windows.Forms.MessageBox.Show("Lost " + device.Name);
		}

		private void device_ServiceDiscovered(object sender, BlueToolsEventArgs eventArgs)
		{
			Device device = (Device)sender;
			RemoteService service = (RemoteService)((DiscoveryEventArgs)eventArgs).Discovery;
			if(service.ServiceType.Equals(ChatService.ChatServiceServiceType))
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

		private void network_DeviceDiscoveryCompleted(object sender, BlueToolsEventArgs eventArgs)
		{
			while(newDevices.Count > 0)
			{
				RemoteDevice device = (RemoteDevice)newDevices[0];
				newDevices.RemoveAt(0);

				device.ServiceDiscovered += new BlueToolsEventHandler(device_ServiceDiscovered);

				try
				{
					device.DiscoverServicesAsync(ServiceType.RFCOMM);
				} 
				catch (Exception exc)
				{
					System.Windows.Forms.MessageBox.Show(exc.Message);
				}
			}
		}

		#endregion
	}
}
