
// Franson C++ BlueTools
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xDevice.h"
#include "xRemoteService.h"
#include "xServiceList.h"

namespace xBlueTools
{
	using namespace xCore;

	class xRemoteDevice;
	class xNetwork;
	class xDeviceList;
	class xSession;

	// Interface to implement for async callcbacks from xRemoteDevice
	class xIRemoteDeviceEvents
	{
	public:
		virtual void OnServiceDiscoveryStarted(xRemoteDevice* source) = 0;
		virtual void OnServiceDiscovered(xRemoteDevice* source, xRemoteService* discoveredService) = 0;
		virtual void OnServiceDiscoveryCompleted(xRemoteDevice* source, xServiceList* discoveredServices) = 0;
		virtual void OnError(xRemoteDevice* source, int errorCode, xString* errorText) = 0;
		virtual void OnBondStarted(xRemoteDevice* source) = 0;
		virtual void OnBondCompleted(xRemoteDevice* source) = 0;
		virtual void OnUnbondStarted(xRemoteDevice* source) = 0;
		virtual void OnUnbondCompleted(xRemoteDevice* source) = 0;
	};

	class XLIB xRemoteDevice : public xDevice
	{
		friend xNetwork;
		friend xDeviceList;
		friend xSession;

	public:
		// ** Note all methods inherited from BlueTools::xDevice **

		virtual bool GetBonded();
		virtual bool GetDiscoveryPending();
		virtual bool IsBondingPending();
		virtual int GetSignalStrength();

		virtual void Bond(xString* pincode);
		virtual void BondAsync(xString* pincode);

		virtual void ConnectService(int serviceChannelNumber, xString* serviceName, xRemoteService** retVal);
		virtual void DiscoverServices(GUID serviceType, xServiceList** retVal);
		virtual void DiscoverServicesAsync(GUID serviceType);

		virtual void Unbond();
		virtual void UnbondAsync();

		virtual void SetEvents(xIRemoteDeviceEvents* events);

		// Internal
		virtual void DispatchEvent(void*);

	private:
		xRemoteDevice(void* internal);
		xIRemoteDeviceEvents* m_events;
	};
}
