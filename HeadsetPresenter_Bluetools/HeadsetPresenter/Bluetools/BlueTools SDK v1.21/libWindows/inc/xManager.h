
// Franson C++ BlueTools
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xObject.h"
#include "xVersion.h"
#include "xSecurityPolicy.h"
#include "xNetworkList.h"
#include "xStackID.h"

namespace xBlueTools
{
	using namespace xCore;

	class XLIB xManager : public xObject
	{
	public:
		static void GetBlueToolsVersion(xVersion** retVal);
		static xStackID GetStackID();
		static void SetStackID(xStackID);
		static void GetManager(xManager** retVal);

		virtual void GetNetworks(xNetworkList** retVal);
		virtual xSecurityPolicy GetSecurityPolicy();
		virtual void SetSecurityPolicy(xSecurityPolicy);

		virtual void Dispose();
		
	private:
		xManager(void* internal);

		static xManager* m_singleton;
		static bool m_bDisposed;
	};
}
