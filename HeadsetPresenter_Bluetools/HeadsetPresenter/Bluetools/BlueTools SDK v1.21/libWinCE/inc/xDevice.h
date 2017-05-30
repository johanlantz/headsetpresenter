
// Franson C++ BlueTools
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xEntity.h"
#include "xServiceList.h"

namespace xBlueTools
{
	using namespace xCore;

	class xDeviceList;

	class XLIB xDevice : public xEntity
	{
		friend xDeviceList;

	public:
		// ** Note all methods inherited from BlueTools::xEntity **

		// Not supported in BlueTools 1.10
		// virtual DeviceClass GetDeviceClass();

		virtual void GetServices(xServiceList** retVal);

	protected:
		xDevice(void* internal);
	};
}
