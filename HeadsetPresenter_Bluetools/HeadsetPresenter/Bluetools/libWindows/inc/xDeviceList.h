
// Franson C++ BlueTools
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xDevice.h"

namespace xBlueTools
{
	using namespace xCore;

	class xNetwork;

	class XLIB xDeviceList : public xObject
	{
		friend xNetwork;

	public:
		virtual int GetDeviceCount();

		virtual void GetDevice(int iIndex, xDevice** retVal);

	protected:
		xDeviceList(void* internal);
	};
}
