using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

using Franson.BlueTools;

namespace FindAndConnect_2008
{
    public partial class MainForm : Form
    {
        private Manager manager = null;
        private Network m_network = null;
        private RemoteService currentService = null;
        private Stream currentStream = null;
        private byte[] m_buffer = new byte[25];	// Buffer to read data into

        public MainForm()
        {
            InitializeComponent();

            this.FormClosing += new FormClosingEventHandler(MainForm_FormClosing);

            try
            {
                manager = Manager.GetManager();

                switch (Manager.StackID)
                {
                    case StackID.STACK_MICROSOFT:
                        lStackID.Text = "Microsoft Bluetooth Stack";
                        break;
                    case StackID.STACK_WIDCOMM:
                        lStackID.Text = "WidComm Bluetooth Stack";
                        break;
                }

                // You can get a valid evaluation key for BlueTools at
                // http://franson.com/bluetools/
                // That key will be valid for 14 days. Just cut and paste that key into the statement below.
                // To get a key that do not expire you need to purchase a license
                Franson.BlueTools.License license = new Franson.BlueTools.License();
                license.LicenseKey = "WoK6KL354ACCNOKQRWcZRjrRFQmtQXKYWsKB";

                // Call events in GUI thread
                manager.Parent = this;
                // Call events in new thread (multi-threading)
                // manager.Parent = null

                // Get first netowrk (BlueTools 1.0 only supports one network == one dongle)
                m_network = manager.Networks[0];

                // Add events for device discovery
                m_network.DeviceDiscovered += new BlueToolsEventHandler(m_network_DeviceDiscovered);
                m_network.DeviceDiscoveryStarted += new BlueToolsEventHandler(m_network_DeviceDiscoveryStarted);
                m_network.DeviceDiscoveryCompleted += new BlueToolsEventHandler(m_network_DeviceDiscoveryCompleted);
                m_network.DeviceLost += new BlueToolsEventHandler(m_network_DeviceLost);

                // Stack version
                Version versionStack = m_network.StackVersion;
                lStackID.Text += " " + versionStack.ToString();
            }
            catch (Exception exc)
            {
                MessageBox.Show(exc.Message);
                discoverDevices.Enabled = false;
            }
        }

        void m_network_DeviceLost(object sender, BlueToolsEventArgs eventArgs)
        {
            RemoteDevice device = (RemoteDevice)((DiscoveryEventArgs)eventArgs).Discovery;
            deviceList.Items.Remove(device);
        }

        void m_network_DeviceDiscoveryCompleted(object sender, BlueToolsEventArgs eventArgs)
        {
            // Search for devices on the network ended.
            lStatus.Text = "Click on a Device!";
            discoverDevices.Enabled = true;
        }

        void m_network_DeviceDiscoveryStarted(object sender, BlueToolsEventArgs eventArgs)
        {
            // Search for devices on the network started

            serviceList.Items.Clear();
            discoverDevices.Enabled = false;

            lStatus.Text = "Discovering devices...";
        }

        void m_network_DeviceDiscovered(object sender, BlueToolsEventArgs eventArgs)
        {
            // A new device was discovered!
            // Note that this event is only called for NEW devices on the network

            // Use Network.Devices for a complete list of discovered devices.

            RemoteDevice device = (RemoteDevice)((DiscoveryEventArgs)eventArgs).Discovery;
            deviceList.Items.Add(device);
        }

        private void discoverDevices_Click(object sender, EventArgs e)
        {
            try
            {
                // Start looking for devices on the network
                // Use Network.DiscoverDevices() if you don't want to use events.
                m_network.DiscoverDevicesAsync();
            }
            catch (BlueToolsException exc)
            {
                MessageBox.Show(exc.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button1);
            }
        }

        private void closeConnection_Click(object sender, EventArgs e)
        {
            if (currentService != null)
            {
                currentStream.Close();
                currentService = null;
                currentStream = null;
            }

            closeConnection.Enabled = false;
            serviceList.Enabled = true;
            lStatus.Text = "OK";
        }

        private void bBond_Click(object sender, EventArgs e)
        {
            RemoteDevice device = (RemoteDevice)deviceList.SelectedItem;
            if (device == null)
            {
                MessageBox.Show("Select a device first!");
            }
            else
            {
                try
                {
                    // Use RemoteDevice.BondAsync() for async version
                    device.Bond(txtPIN.Text);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.ToString());
                }
            }
        }

        private void serviceList_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Close any connections that are open
			//

			if(currentService != null)
			{
				currentStream.Close();

				currentService	= null;
				currentStream	= null;
			}
			
			// Attempt to connect to the service
			//

			// Get service object from list box
			currentService = (RemoteService) serviceList.SelectedItem;

			try
			{
				// Connect to service by reading its Stream
				currentStream = currentService.Stream;

	            currentStream.BeginRead(m_buffer, 0, m_buffer.Length, new AsyncCallback(readCallback), currentStream);

				closeConnection.Enabled		= true;
				serviceList.Enabled			= false;
				lStatus.Text = "Receiving data...";
			} 
			catch (Exception exc)
			{
				MessageBox.Show(exc.Message);
				currentService = null;
			}
        }

        private void deviceList_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Get the remote device that is currently selected
            RemoteDevice device = (RemoteDevice)deviceList.SelectedItem;

            if (device != null)
            {
                try
                {
                    // Disable the device list so the user doesn't attempt to select a device while
                    // the service lookup is pending
                    deviceList.Enabled = false;

                    // Remove the existing items from the service list
                    serviceList.Items.Clear();

                    // Add a DiscoveryListener so we get service discovery events
                    device.ServiceDiscovered += new BlueToolsEventHandler(device_ServiceDiscovered);

                    // Add a DiscoveryListener so we get service discovery completion events
                    device.ServiceDiscoveryCompleted += new BlueToolsEventHandler(device_ServiceDiscoveryCompleted);

                    device.Error += new BlueToolsEventHandler(device_Error);

                    // Cancel any ongoing device discovery
                    m_network.CancelDeviceDiscovery();

                    // Discover all services offered by the device
                    // Use e.g. ServiceType.SerialPort to list only serial port services
                    // Use Device.DiscoverServices() if you don't want to use events.
                    // device.DiscoverServicesAsync(ServiceType.SerialPort);
                    device.DiscoverServicesAsync(ServiceType.RFCOMM);

                    // Add already discovered services to list box (for this device) 
                    serviceList.Items.Clear();
                    Service[] services = device.Services;
                    foreach (RemoteService service in services)
                    {
                        serviceList.Items.Add(service);
                    }

                    lStatus.Text = "Searching for Services...";
                }
                catch (Exception exc)
                {
                    MessageBox.Show(exc.Message);
                }
            }
        }

        void device_Error(object sender, BlueToolsEventArgs eventArgs)
        {
            Franson.BlueTools.ErrorEventArgs errorEventArgs = (Franson.BlueTools.ErrorEventArgs)eventArgs;
            MessageBox.Show(errorEventArgs.ErrorCode + ": " + errorEventArgs.Message);
        }

        void device_ServiceDiscoveryCompleted(object sender, BlueToolsEventArgs eventArgs)
        {
            // All services for device discovered"

            // Get the remote device that raised the event
            RemoteDevice device = (RemoteDevice)sender;

            // You find an array of all found services here.
            // Or by using Device.Services
            // Note that all services are RemoteService
            Service[] services = (Service[])((DiscoveryEventArgs)eventArgs).Discovery;

            // Remove event handlers
            device.ServiceDiscovered -= new BlueToolsEventHandler(device_ServiceDiscovered);
            device.ServiceDiscoveryCompleted -= new BlueToolsEventHandler(device_ServiceDiscoveryCompleted);

            // Reenable the device list
            deviceList.Enabled = true;

            lStatus.Text = "Click on a Service";
        }

        void device_ServiceDiscovered(object sender, BlueToolsEventArgs eventArgs)
        {
            // A new serive found for device!
            // Note that this event is only called for NEW services found
            // Use Device.Services for a complete list of services for device

            // Get the service associated with the discovery event
            Service service = (Service)((DiscoveryEventArgs)eventArgs).Discovery;
            // Add the newly discovered service to the service list
            serviceList.Items.Add(service);
        }

        private void readCallback(IAsyncResult result)
        {
            byte[] buffer = null;

            // result is always a BlueToolsAsyncResult object
            BlueToolsAsyncResult blueAsyncResult = (BlueToolsAsyncResult)result;

            // We passed on buffer as custom object, you can pass on any object here. We passed the stream object
            ServiceStream stream = (ServiceStream)blueAsyncResult.AsyncState;

            // The buffer used for reading can be found in the result object.
            buffer = blueAsyncResult.Buffer;

            try
            {
                // EndRead() must always be called!
                // If stream has been closed due to an error, we'll have an excpetion here
                int len = stream.EndRead(result);

                System.Text.ASCIIEncoding enc = new ASCIIEncoding();
                string str = enc.GetString(buffer, 0, len);

                serviceOutput.Text = str;

                // Start new async read
                stream.BeginRead(m_buffer, 0, m_buffer.Length, new AsyncCallback(readCallback), stream);
            }
            catch (ObjectDisposedException ex)
            {
                // Thrown if stream has been closed.
                closeConnection.Enabled = false;
                serviceList.Enabled = true;

                lStatus.Text = ex.Message;
            }
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            Dispose();
        }
    }
}
