
// Franson C++ BlueTools
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xNetwork.h"

namespace xBlueTools
{
	using namespace xCore;

	class xManager;

	class XLIB xNetworkList : public xObject
	{
		friend xManager;

	public:
		virtual int GetNetworkCount();

		virtual void GetNetwork(int iIndex, xNetwork** retVal);

	protected:
		xNetworkList(void* internal);
	};
}
