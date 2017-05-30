using System;
using Franson.BlueTools;
using System.Windows.Forms;

namespace NetworkScanner
{
	/// <summary>
	/// Summary description for NetworkEnumerator.
	/// </summary>
	public class NetworkScanner : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TreeView deviceList;
		private System.Windows.Forms.Label deviceListLabel;
		private System.Windows.Forms.StatusBar statusBar;

		private Network[]	networks;
		private System.Windows.Forms.CheckBox autoDiscover;
		private System.Windows.Forms.Button discover;
		private Manager		manager;

		public NetworkScanner()
		{
			InitializeComponent();

			try
			{
				// Fetch the bluetooth manager instance
				manager	= Manager.GetManager();
			} 
			catch (Exception exc)
			{
				MessageBox.Show(exc.Message);
				return;
			}

			try
			{
				// You can get a valid evaluation key at
				// http://franson.com/bluetools/
				// That key will be valid for 14 days. Just cut and paste that key into the statement below.
				// To get a key that do not expire you need to purchase a license
				Franson.BlueTools.License license = new Franson.BlueTools.License();
				license.LicenseKey = "WoK6HM24B9ECLOPQSXVZPixRDRfuIYHYWrT9";

				// Fetch all network instances
				networks = manager.Networks;
			} 
			catch (BlueToolsException exc)
			{
				MessageBox.Show(exc.Message);
				return;
			}

			// Set the parent of the manager to this control. This is necessary if
			// we want to invoke methods on controls from within the bluetools
			// event handlers.
			manager.Parent = this;

			// Attach event listeners to all network instances
			foreach(Network network in networks)
			{
				// This allows us to get notified whenever a device has been discovered
				network.DeviceDiscovered += new BlueToolsEventHandler(network_DeviceDiscovered);

				// This allows us to get notified whenever a device has been lost
				network.DeviceLost += new BlueToolsEventHandler(network_DeviceLost);

				// This allows us get get notified whenever a device discovery
				// has completed. This will happen event if no new devices have
				// been discovered; it will deliver the current list of devices
				// available on the network. This can be used e.g. to update the
				// names of the devices in the list.
				network.DeviceDiscoveryCompleted += new BlueToolsEventHandler(network_DeviceDiscoveryCompleted);
				network.DeviceDiscoveryStarted += new BlueToolsEventHandler(network_DeviceDiscoveryStarted);
				
				// This allows us to get notified whenever an error occures in
				// the network. 
				network.Error += new BlueToolsEventHandler(network_Error);
			}

			Closing += new System.ComponentModel.CancelEventHandler(NetworkEnumerator_Closing);
		}

		private void InitializeComponent()
		{
			this.discover = new System.Windows.Forms.Button();
			this.deviceList = new System.Windows.Forms.TreeView();
			this.deviceListLabel = new System.Windows.Forms.Label();
			this.statusBar = new System.Windows.Forms.StatusBar();
			this.autoDiscover = new System.Windows.Forms.CheckBox();
			this.SuspendLayout();
			// 
			// discover
			// 
			this.discover.Location = new System.Drawing.Point(8, 280);
			this.discover.Name = "discover";
			this.discover.TabIndex = 0;
			this.discover.Text = "Discover";
			this.discover.Click += new System.EventHandler(this.discover_Click);
			// 
			// deviceList
			// 
			this.deviceList.ImageIndex = -1;
			this.deviceList.Location = new System.Drawing.Point(8, 24);
			this.deviceList.Name = "deviceList";
			this.deviceList.SelectedImageIndex = -1;
			this.deviceList.Size = new System.Drawing.Size(392, 248);
			this.deviceList.TabIndex = 2;
			// 
			// deviceListLabel
			// 
			this.deviceListLabel.Location = new System.Drawing.Point(8, 8);
			this.deviceListLabel.Name = "deviceListLabel";
			this.deviceListLabel.Size = new System.Drawing.Size(112, 16);
			this.deviceListLabel.TabIndex = 3;
			this.deviceListLabel.Text = "Devices in proximity";
			// 
			// statusBar
			// 
			this.statusBar.Location = new System.Drawing.Point(0, 304);
			this.statusBar.Name = "statusBar";
			this.statusBar.Size = new System.Drawing.Size(408, 22);
			this.statusBar.TabIndex = 4;
			// 
			// autoDiscover
			// 
			this.autoDiscover.Location = new System.Drawing.Point(88, 280);
			this.autoDiscover.Name = "autoDiscover";
			this.autoDiscover.Size = new System.Drawing.Size(120, 24);
			this.autoDiscover.TabIndex = 5;
			this.autoDiscover.Text = "Automic Discovery";
			this.autoDiscover.CheckedChanged += new System.EventHandler(this.autoDiscover_CheckedChanged);
			// 
			// NetworkScanner
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(408, 326);
			this.Controls.Add(this.autoDiscover);
			this.Controls.Add(this.statusBar);
			this.Controls.Add(this.deviceListLabel);
			this.Controls.Add(this.deviceList);
			this.Controls.Add(this.discover);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "NetworkScanner";
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.Text = "Network Scanner";
			this.Load += new System.EventHandler(this.NetworkScanner_Load);
			this.ResumeLayout(false);

		}


		private void network_DeviceLost(object sender, BlueToolsEventArgs eventArgs)
		{
			// In a device lost event, the sender is always a Network and the event
			// type is always a DiscoveryEventArgs. The device that was lost is 
			// attached to the DiscoveryEventArgs through the Discovery property.

			RemoteDevice device = (RemoteDevice)((DiscoveryEventArgs)eventArgs).Discovery;
			statusBar.Text = "Lost device " + device.Name;
			// Remove the device from the device and service list
			//

			foreach(TreeNode node in deviceList.Nodes)
			{
				if(node.Tag.Equals(device.Address.ToString()))
				{
					deviceList.Nodes.Remove(node);
					break;
				}
			}

			// Remove the event handler from the device
			device.ServiceDiscovered -= new BlueToolsEventHandler(device_ServiceDiscovered);
		}

		private void network_DeviceDiscovered(object sender, BlueToolsEventArgs eventArgs)
		{
			// In a device discovered event, the sender is always a Network, and the event
			// is always a DiscoveryEventArgs. The device that was discovered is attached
			// to the DiscoveryEventArgs through the Discovery property.
			RemoteDevice device = (RemoteDevice)((DiscoveryEventArgs)eventArgs).Discovery;

			statusBar.Text = "Discovered device " + device.Name;

			// Check if the device is already present in the list
			//

			bool found = false;
			foreach(TreeNode node in deviceList.Nodes)
			{
				if(node.Tag.Equals(device.Address.ToString()))
				{
					found = true;
					break;
				}
			}

			if(!found)
			{
				// Add the device to the list
				//

				TreeNode node = new TreeNode(device.Name.Length == 0 ? device.Address.ToString() : device.Name);
				node.Tag = device.Address.ToString();
				deviceList.Nodes.Add(node);

				// Add a service discovery listener so we can discover the services
				// of the device.
				device.ServiceDiscovered += new BlueToolsEventHandler(device_ServiceDiscovered);

				// Discover the services of the device. Note that this will generally
				// not execute immediatelly but instead wait until the device and
				// any enqueued service discovery has completed. The reason for this
				// is that the bluetooth stacks generally can't handle several
				// discoveries.
				device.DiscoverServicesAsync(ServiceType.RFCOMM);
			}
		}

		private void NetworkEnumerator_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			Dispose();
		}

		private void device_ServiceDiscovered(object sender, BlueToolsEventArgs eventArgs)
		{
			// In a service discovered event, the sender is always a RemoteDevice instance,
			// and the event is always a DiscoveryEventArgs. The discovered service is 
			// attached to the DiscoveryEventArgs through the Discovery property.

			RemoteDevice device = (RemoteDevice)sender;
			RemoteService service = (RemoteService)((DiscoveryEventArgs)eventArgs).Discovery;

			// Add the service to the list
			//

			foreach(TreeNode node in deviceList.Nodes)
			{
				if(node.Tag.Equals(device.Address.ToString()))
				{
					TreeNode serviceNode = new TreeNode(service.Name);
					serviceNode.Tag = service.Address.ToString();

					node.Nodes.Add(serviceNode);
					node.Expand();
				}
			}

			statusBar.Text = "Discovered service " + service.Name + " of device " + device.Name;
		}

		private void network_DeviceDiscoveryCompleted(object sender, BlueToolsEventArgs eventArgs)
		{
			Device[] devices = (Device[])((DiscoveryEventArgs)eventArgs).Discovery;

			// Update each device in the device list. Some bluetooth stack doesn't
			// immediatelly return the name of the device, as that is more complicated
			// than delivering the address only. Therefore, the name might not 
			// be present at first. We compensate for that here.

			foreach(Device device in devices)
			{
				foreach(TreeNode deviceNode in deviceList.Nodes)
				{
					if(deviceNode.Tag.Equals(device.Address.ToString()))
					{
						deviceNode.Text = device.Name;
						break;
					}
				}
			}

			statusBar.Text = "Device discovery completed";
			discover.Enabled = true;
		}

		private void network_DeviceDiscoveryStarted(object sender, BlueToolsEventArgs eventArgs)
		{
			statusBar.Text = "Looking for devices";
		}

		private void ShowError(Exception exc)
		{
			System.Windows.Forms.MessageBox.Show(this, exc.Message, "Error", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Exclamation);
		}

		private void network_Error(object sender, BlueToolsEventArgs eventArgs)
		{
			ShowError(((ErrorEventArgs)eventArgs).Exception);
		}

		protected override void Dispose(bool disposing)
		{
			manager.Dispose();

			base.Dispose (disposing);
			if(disposing)
			{
			}
		}

		[STAThread]
		public static void Main(String[] args)
		{
			Application.Run(new NetworkScanner());
		}

		private void autoDiscover_CheckedChanged(object sender, System.EventArgs e)
		{
			try
			{
				foreach(Network network in networks)
				{
					// Enable auto discovery on the Network instance. This should
					// always be used with caution, as the bluetooth stack might
					// not handle communication properly if it is always busy
					// with discovering devices.
					network.AutoDiscovery = autoDiscover.Checked;

					// Run discoveries as quickly as possible
					network.AutoRefreshInterval = 0;
				}

				discover.Enabled = !autoDiscover.Checked;
			} 
			catch (Exception exc)
			{
				ShowError(exc);
			}
		}

		private void discover_Click(object sender, System.EventArgs e)
		{
			try
			{
				discover.Enabled = false;
				foreach(Network network in networks)
				{
					network.DiscoverDevicesAsync();
				}
			}
			catch (Exception exc)
			{
				ShowError(exc);
			}
		}

		private void NetworkScanner_Load(object sender, System.EventArgs e)
		{
		
		}
	}
}
