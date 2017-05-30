
// Franson C++ Simple Framework
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xObject.h"
#include "xString.h"

namespace xCore
{
	class XLIB xException : public xObject
	{
	public:
		xException(int iErrorCode, xString* strErrroMessage);
		xException(int iErrorCode, char* strErrorMessage);
		// Sets iErrorCode to Win32 GetLastError()
		xException(char* strErrorMessage);

		~xException();

		int GetErrorCode();
		void GetErrorMessage(xString** retVal);

	private:
		int m_iErrorCode;
		xString* m_strErrorMessage;
	};
}