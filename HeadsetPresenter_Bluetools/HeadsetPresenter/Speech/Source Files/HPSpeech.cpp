// HPSpeech.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{D552FC04-2CC3-4FEE-AE16-41AA5010EBF2}", 
		 name = "HPSpeech", 
		 helpstring = "HPSpeech 1.0 Type Library",
		 resource_name = "IDR_HPSPEECH") ]
class CHPSpeechModule
{
public:
// Override CAtlDllModuleT members
};
		 
