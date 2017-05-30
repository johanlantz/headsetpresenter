
// Franson C++ BlueTools
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xObject.h"
#include "xDeviceList.h"
#include "xServer.h"
#include "xRemoteDevice.h"
#include "xDeviceList.h"
#include "xVersion.h"

namespace xBlueTools
{
	using namespace xCore;

	class xNetwork;
	class xNetworkList;

	// Callback interface for events
	// Implement this interface in the class that handles events from xNetwork
	class xINetworkEvents
	{
	public:
		virtual void OnDeviceDiscoveryStarted(xNetwork* source) = 0;
		virtual void OnDeviceDiscovered(xNetwork* source, xRemoteDevice* discoveredDevice) = 0;
		virtual void OnDeviceLost(xNetwork* source, xRemoteDevice* lostDevice) = 0;
		virtual void OnDeviceDiscoveryCompleted(xNetwork* source, xDeviceList* discoveredDevices) = 0;
		virtual void OnError(xNetwork* source, int errorCode, xString* errorText) = 0;
	};

	class XLIB xNetwork : public xObject
	{
		friend xNetworkList;

	public:
		virtual bool GetAutoDiscovery();
		virtual void SetAutoDiscovery(bool bAuto);

		virtual int GetAutoRefreshInterval();
		virtual void SetAutoRefreshInterval(int iInterval);

		virtual void GetDevices(xDeviceList** retVal);

		virtual bool GetDiscoveryPending();

		virtual void GetServer(xServer** retVal);

		virtual void GetStackVersion(xVersion** retVal);

		virtual void CancelDeviceDiscovery();

		virtual void ConnectDevice(xAddress* deviceAddress, xString* deviceName, xRemoteDevice** retVal);

		virtual void DiscoverDevicesAsync();

		virtual void DiscoverDevices(xDeviceList** retVal);

		virtual void SetEvents(xINetworkEvents*);

		virtual bool IsHardwareAvailable();

		// Internal
		virtual void DispatchEvent(void*);

	private:
		xNetwork(void* internal);
		xINetworkEvents* m_events;
	};
}
