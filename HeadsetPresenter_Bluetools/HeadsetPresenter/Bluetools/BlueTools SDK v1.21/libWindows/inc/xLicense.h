
// Franson C++ BlueTools
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xObject.h"
#include "xString.h"

namespace xBlueTools
{
	using namespace xCore;

	class XLIB xLicense : public xObject
	{
	public:
		xLicense();

		virtual void SetLicenseKey(xString* strKey);

	private:
	};
}
