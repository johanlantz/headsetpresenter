
// Franson C++ BlueTools
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xService.h"

namespace xBlueTools
{
	using namespace xCore;

	class xServer;
	class xSession;
	class xLocalService;
	class xServiceList;

	// Callback interface for events
	// Implement this interface in the class that handles events from xLocalService
	class xILocalServiceEvents
	{
	public:
		virtual void OnAdvertised(xLocalService* source, xServer* xServer) = 0;
		virtual void OnClientConnected(xLocalService* source, xSession* sessionConnected) = 0;
		virtual void OnClientDisconnected(xLocalService* source, xSession* sessionDisconnected) = 0;
		virtual void OnDeadvertised(xLocalService* source, xServer* xServer) = 0;
	};

	class XLIB xLocalService : public xService
	{
		friend xServiceList;
		friend xSession;

	public:
		// ** Note all methods derived from BlueTools::xService **

		xLocalService(GUID serviceType, xString* strName, xString* strComment);

		virtual void SetEvents(xILocalServiceEvents*);		
		
		// Internal
		virtual void DispatchEvent(void*);

	private:
		xLocalService(void* internal);
		xILocalServiceEvents* m_events;
	};
}
