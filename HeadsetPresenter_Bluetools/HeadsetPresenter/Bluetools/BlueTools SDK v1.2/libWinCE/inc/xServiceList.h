
// Franson C++ BlueTools
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xService.h"

namespace xBlueTools
{
	using namespace xCore;

	class xDevice;
	class xRemoteDevice;

	class XLIB xServiceList : public xObject
	{
		friend xDevice;
		friend xRemoteDevice;

	public:
		virtual int GetServiceCount();

		virtual void GetService(int iIndex, xService** retVal);

	private:
		xServiceList(void* internal);
	};
}
