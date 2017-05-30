
// Franson C++ BlueTools
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xEntity.h"
#include "xServiceType.h"
#include "xSecurityPolicy.h"

namespace xBlueTools
{
	using namespace xCore;

	class xLocalService;
	class xRemoteService;

	class XLIB xService : public xEntity
	{
		friend xLocalService;
		friend xRemoteService;

	public:

		// ** Note all methods derived class from BlueTools::xEntity **

		virtual xSecurityPolicy GetSecurityPolicy();
		virtual void SetSecurityPolicy(xSecurityPolicy sp);

		virtual GUID GetServiceID();

		// Not supported in this C++ BlueTools version
		// virtual void GetServiceRecord();

		GUID GetServiceType();

	protected:
		xService(void* internal);
	};
}
