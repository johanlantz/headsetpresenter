#pragma once
#include "C:\Projects\LicenseManager\LicenseManager.h"

class Authenticator
{
public:
	Authenticator(ILicenseHandler* imyLicenseHandler);
	~Authenticator(void);
private:
	ILicenseHandler* myLicenseHandler;
public:
	bool AuthenticateComponent(void);
	int CK;
};
