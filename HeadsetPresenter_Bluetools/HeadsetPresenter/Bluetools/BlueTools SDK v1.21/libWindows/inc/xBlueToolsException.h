
// Franson C++ BlueTools
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xException.h"

namespace xBlueTools
{
	using namespace xCore;

	class XLIB xBlueToolsException : public xException
	{
	public:
		xBlueToolsException(int iErrorCode, xString* strErrroMessage);
		xBlueToolsException(int iErrorCode, char* strErrorMessage);
		xBlueToolsException(char* strErrorMessage);
	};
}