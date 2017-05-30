#pragma once

//include for licenseManagerComponent, needs to be distributed in SDK
//Change path for the include file in you local directory
#include "C:\Projects\LicenseManager\LicenseManager.h"
#include "C:\Projects\LicenseManagerAuthenticator\Authenticator.h"
class LicenseManagerProxy
{
public:
	LicenseManagerProxy(void);
	virtual ~LicenseManagerProxy(void);
	
	bool CreateLicFile(BSTR productName, BSTR productVersion);
	bool CheckLicense(BSTR productName, BSTR productVersion);
	void AllowAnyValidLicenseVersion();
	void SetRegistrationComment(BSTR comment);
	bool CRCCheckSelfCRC(BSTR fullPathToApplication, BSTR fullPathToNonAppendedCRC);
	void SetMinAllowedVersion(double minAllowedVersion);
private:
	ILicenseHandler* myLicenseHandler;
	Authenticator* myAuthenticator;
};
