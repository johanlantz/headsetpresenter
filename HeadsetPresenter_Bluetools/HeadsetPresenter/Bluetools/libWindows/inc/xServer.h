
// Franson C++ BlueTools
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xServiceList.h"

namespace xBlueTools
{
	using namespace xCore;

	class xNetwork;
	class xSession;
	class xLocalService;

	class XLIB xServer : public xDevice
	{
		friend xNetwork;
		friend xSession;
		friend xLocalService; 

	public:
		// ** Note all methods inherited from BlueTools::Device **

		virtual void Advertise(xService* service);
		virtual void Advertise(xService* service, int scn);

		virtual void Deadvertise(xService* service);
 
	protected:
		xServer(void* internal);
	};
}
