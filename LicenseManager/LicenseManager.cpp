// LicenseManager.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"
#include "LicenseManager.h"

class CLicenseManagerModule : public CAtlDllModuleT< CLicenseManagerModule >
{
public :
	DECLARE_LIBID(LIBID_LicenseManagerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_LICENSEMANAGER, "{AAD05AB7-5837-4A4A-9BE5-B1D203F9DDC2}")
};

CLicenseManagerModule _AtlModule;

class CLicenseManagerApp : public CWinApp
{
public:

// Overrides
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CLicenseManagerApp, CWinApp)
END_MESSAGE_MAP()

CLicenseManagerApp theApp;

BOOL CLicenseManagerApp::InitInstance()
{
    return CWinApp::InitInstance();
}

int CLicenseManagerApp::ExitInstance()
{
    return CWinApp::ExitInstance();
}


// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return (AfxDllCanUnloadNow()==S_OK && _AtlModule.GetLockCount()==0) ? S_OK : S_FALSE;
}


// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}
