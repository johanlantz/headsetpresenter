
// Franson C++ BlueTools
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xServiceStream.h"

namespace xBlueTools
{
	using namespace xCore;

	class xRemoteDevice;
	class xServiceList;

	class XLIB xRemoteService : public xService
	{
		friend xRemoteDevice;
		friend xServiceList;

	public:
		// Note all methods inherited from BlueTools::xService

		virtual void GetStream(xStream** retVal);

	protected:
		xRemoteService(void* internal);
	};
}
