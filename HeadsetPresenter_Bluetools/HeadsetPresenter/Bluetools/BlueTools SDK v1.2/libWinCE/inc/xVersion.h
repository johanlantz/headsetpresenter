
// Franson C++ Simple Framework
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xObject.h"
#include "xString.h"

namespace xCore
{
	class XLIB xVersion : public xObject
	{
	public:
		xVersion(int major, int minor, int revision, int build);

		int GetMajor();
		int GetMinor();
		int GetRevision();
		int GetBuild();

	private:
		int m_major;
		int m_minor;
		int m_revision;
		int m_build;
	};
}
