
// Franson C++ Simple Framework
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xObject.h"

#include "wtypes.h"
#include "oleauto.h"

namespace xCore
{
	class XLIB xString : public xObject
	{
	public:
		// NULL terminated string
		xString(char* str, int iLength = -1);

		// If acquire is set to true the ownership of the BSTR is passed to the xString
		// and xString will call SysFreeString() for the BSTR in xString's destructor
		// If acquire set to false, a copy is made of the BSTR
		// and the caller keeps ownership of the BSTR
		xString(BSTR bstr, bool acquire = true);

		// Cleanup
		virtual ~xString();

		// Returns BSTR. If detach is set to true the ownership of the BSTR is passed on to the 
		// caller. The caller must in that case call SysFreeString() for the returned BSTR

		// If detach is false xString will keep the ownership and the caller should not delete the BSTR after usage.
		virtual BSTR GetBSTR(bool detach = false);

		// Returns pointer to inernal buffer
		virtual char* GetCharString();

		// Returns T string. (TCHAR* / LPTSTR)
		virtual LPTSTR GetTString();

		// Returns length of string (excluding any trailing zero)
		virtual int GetLength();

		// Returns true if this xString equals obj->ToString()
		// Case sensitive compare.
		virtual bool Equals(xObject* obj);

		// Returns itself
		virtual void ToString(xString** retVal);

	private:
		BSTR m_bstr;
		char* m_charString;
		int m_iLength;
	};
}
