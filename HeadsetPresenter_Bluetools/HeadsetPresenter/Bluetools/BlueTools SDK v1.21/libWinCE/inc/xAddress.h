
// Franson C++ BlueTools
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xObject.h"
#include "xString.h"

namespace xBlueTools
{
	using namespace xCore;

	class xEntity;

	class XLIB xAddress : public xObject
	{
		friend xEntity;

	public:
		xAddress(xAddress* host, int scn);
		xAddress(int nap, int sap, int scn);
		xAddress(int nap, int sap);
		xAddress(xString* xAddress);

		virtual void GetHostAddress(xAddress** retVal);
		virtual int GetNAP();
		virtual int GetSAP();
		virtual int GetServiceChannelNumber();

		// From Core::xObject
		virtual bool Equals(xObject*);
		virtual void ToString(xString** retVal);

	private:
		// Do not touch!
		xAddress(void* internal);
	};
}
