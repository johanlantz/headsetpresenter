#include "stdafx.h"
#include "..\Header Files\licensemanagerproxy.h"

LicenseManagerProxy::LicenseManagerProxy(void)
{
	//::CoInitialize(NULL);  //is this not once per thread?
	HRESULT hr = CoCreateInstance(__uuidof(LicenseHandler),NULL,CLSCTX_ALL,__uuidof(ILicenseHandler),(void**)&myLicenseHandler);
	myAuthenticator = new Authenticator(myLicenseHandler);
}


LicenseManagerProxy::~LicenseManagerProxy(void)
{
	//myLicenseHandler->Release();  //Smart pointer ? if so do not release
	//::CoUninitialize();
}

bool LicenseManagerProxy::CRCCheckSelfCRC(BSTR fullPathToApplication, BSTR fullPathToNonAppendedCRC)
{
	BSTR status;
	myLicenseHandler->CRCCheckSelf(fullPathToApplication, fullPathToNonAppendedCRC,	&status);
	if(wcscmp(status,L"OK") == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
	SysFreeString(status);
}

void LicenseManagerProxy::SetMinAllowedVersion(double minAllowedVersion)
{
	BSTR status;
	myLicenseHandler->SetMinVersionAllowed(minAllowedVersion, &status);
	SysFreeString(status);
}

void LicenseManagerProxy::AllowAnyValidLicenseVersion()
{
	myLicenseHandler->AllowAnyValidLicenseVersion();
}

void LicenseManagerProxy::SetRegistrationComment(BSTR comment)
{
	myLicenseHandler->SetRegistrationComment(comment);
}

bool LicenseManagerProxy::CreateLicFile(BSTR productName, BSTR productVersion)
{
	BSTR status;
	myLicenseHandler->CreateLicFile(productName, productVersion, &status);
	bool res = false;
	if(wcscmp(status,L"OK") == 0)
	{
		myLicenseHandler->QueryForEmail();
		//MessageBox(NULL,"License activation OK.","Activation OK",MB_OK|MB_ICONINFORMATION);  //Here you might want to ask the user to reboot
		res = true;
	}
	else if(wcscmp(status,L"INVALIDIDENTIFIER")== 0)  //Identifier is invalid
	{
		//MessageBox(NULL,"The supplied activation code is invalid","LicenseManager",MB_OK|MB_ICONINFORMATION);
	}
	else if(wcscmp(status,L"NOLICENSELEFT")== 0) //The product is not registered for use with the licensing tool
	{
		//MessageBox(NULL,"The license for this product has already been delivered.\n\nThe original license file has been reconstructed","",MB_OK);
	}
	else if(wcscmp(status,L"SERVERERROR")== 0)  //Could not contact server
	{
		MessageBox(NULL,"The license server could not be contacted. Check you internet connection and try again.","LicenseManager",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
	}
	else if(wcscmp(status,L"USERCANCEL")== 0)  //User canceled the connection to internet or pressed cancel in the key code box
	{
	}
	else
	{
		//other unhandled return values, see documentation for all possible values
	}
	SysFreeString(status);  // We asked for it, we release it
	return res;
}

bool LicenseManagerProxy::CheckLicense(BSTR productName, BSTR productVersion)
{
	BSTR status;
	myLicenseHandler->CheckLicense(productName, productVersion,&status);
	bool res;
	if(wcscmp(status,L"OK")== 0)
	{
		res = true;
	}
	else
	{
		res = false;
	}
	SysFreeString(status); //We create, we destroy
	return res;
}