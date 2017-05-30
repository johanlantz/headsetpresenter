
// Franson C++ BlueTools
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xServer.h"
#include "xLocalService.h"
#include "xRemoteDevice.h"
#include "xStream.h"

namespace xBlueTools
{
	using namespace xCore;

	class xLocalService;

	class XLIB xSession : public xObject
	{
		friend xLocalService;

	public:
		virtual void GetLocalHost(xServer** retVal);
		virtual void GetLocalService(xLocalService** retVal);
		virtual void GetRemoteDevice(xRemoteDevice** retVal);
		virtual void GetStream(xStream** retVal);
		virtual void Close();
		virtual bool Equals(xObject*);

	protected:
		xSession(void* internal);
	};
}
