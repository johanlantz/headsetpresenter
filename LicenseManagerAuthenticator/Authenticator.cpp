#include ".\authenticator.h"
#include "time.h"

Authenticator::Authenticator(ILicenseHandler* imyLicenseHandler)
{
	myLicenseHandler = imyLicenseHandler;
	myLicenseHandler->Activate(150);  //licman must be activated with a secret key
	CK=13291;
	AuthenticateComponent();
}

Authenticator::~Authenticator(void)
{
}

/*Note: If a lib is run in debug mode with the HeadsetPresenter as target then HeadsetPresenter
must be rebuilt for any changes in the lib to be effective. It is not enough to rebuild the lib, the 
lib is statically linked into HeadsetPresenter at compiletime not like the dll case where changes would be 
introduces at runtime*/
bool Authenticator::AuthenticateComponent(void)
{
	bool authenticationResult = false;
	int response;
	srand( (unsigned)time( NULL ) );
	
	int challenge = rand();
	myLicenseHandler->Authenticate(challenge,&response);
	
	if(response == (challenge ^ CK))
	{
		authenticationResult = true;
	}
	else
	{
		MessageBox(NULL,"Authentication failed, the application will terminate.\n\nThis should never happen unless you are hacking the LicenceManager. Contact support if this is not the case","",MB_OK);
		exit(0);
	}
	return authenticationResult;
}