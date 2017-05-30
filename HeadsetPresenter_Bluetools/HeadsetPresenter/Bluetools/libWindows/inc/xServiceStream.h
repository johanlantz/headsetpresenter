
// Franson C++ BlueTools
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xStream.h"

namespace xBlueTools
{
	using namespace xCore;

	class xRemoteService;
	class xSession;

	class XLIB xServiceStream : public xStream
	{
		friend xRemoteService;
		friend xSession;

	public:
		// ** Note all methods inherited from Core::xStream**

	protected:
		xServiceStream(void* internal);
	};
}
