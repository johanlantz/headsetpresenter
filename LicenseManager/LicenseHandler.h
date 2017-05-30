// LicenseHandler.h : Declaration of the CLicenseHandler
//LicenseManager by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2005
//////////////////////////////////////////////////////////////////////

#pragma once
#include "resource.h"       // main symbols

#include "LicenseManager.h"
#include "SecurityManager.h"
#include "RegKeyDialog.h"
#include <afxinet.h>

// CLicenseHandler

class ATL_NO_VTABLE CLicenseHandler : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLicenseHandler, &CLSID_LicenseHandler>,
	public IDispatchImpl<ILicenseHandler, &IID_ILicenseHandler, &LIBID_LicenseManagerLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CLicenseHandler();
	~CLicenseHandler();
	
	
	

DECLARE_REGISTRY_RESOURCEID(IDR_LICENSEHANDLER)


BEGIN_COM_MAP(CLicenseHandler)
	COM_INTERFACE_ENTRY(ILicenseHandler)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:

private:
	CString GetLicenseStringFromServer(CString identifier, CString productName, CString productVersion);
	void RecreateLicense(CString productName, CString productVersion, CString noLicenseLeftSuppliedMAC);
	bool GenerateLicenseAndWriteToFile(CString productName, CString productVersion, CString noLicenseLeftSuppliedMAC = "");
	CString httpAddress;
	SecurityManager* mySecurityManager;
	CRegKeyDialog* myRegKeyDialog;
	bool allowAnyValidLicenseVersion;
	void SetHomeDirPath();
	CString applicationHomeDir;
	CString registrationComment;
	bool CRCCheckInternal(CString pathToApplication, CString pathToNonAppendedCRC);
public:
	STDMETHOD(get_status)(LONG* pVal);
	STDMETHOD(CheckLicense)(BSTR productName, BSTR productVersion, BSTR* resultAsString);
	STDMETHOD(CreateLicFile)(BSTR productName, BSTR productVersion, BSTR* statusAsString);
	STDMETHOD(AllowAnyValidLicenseVersion)(void);
	STDMETHOD(QueryForEmail)(void);
	STDMETHOD(SetRegistrationComment)(BSTR iregistrationComment);
	STDMETHOD(CRCCheckSelf)(BSTR fullPathToApplication, BSTR fullPathToNonAppendedCRC, BSTR* resultAsString);
	STDMETHOD(SetMinVersionAllowed)(DOUBLE minVersionAllowed, BSTR* resultAsString);
	STDMETHOD(Authenticate)(INT challenge, INT* response);
	STDMETHOD(Activate)(INT secretKey);
private:
	bool activated;
	int CK;
};

OBJECT_ENTRY_AUTO(__uuidof(LicenseHandler), CLicenseHandler)
