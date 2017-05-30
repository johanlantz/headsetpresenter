
// Franson C++ BlueTools
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xObject.h"
#include "xAddress.h"

namespace xBlueTools
{
	using namespace xCore;

	class XLIB xEntity : public xCore::xObject
	{
	public:
		virtual void GetAddress(xAddress** retVal);
		virtual void GetName(xCore::xString** retVal);

		virtual bool Remote();

		virtual bool Equals(xCore::xObject*);
		virtual void ToString(xCore::xString** retVal);

	protected:
		xEntity(void* internal);
	};
}
