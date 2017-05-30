//LicenseManager by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2005
//////////////////////////////////////////////////////////////////////

#pragma once



#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <ctype.h>
#include <time.h>
#include <afx.h>  //needed for CString
#include <Iphlpapi.h>

using namespace std;

extern "C" 
{
#include "rijndael-api-fst.h"
}

#define BLOCKSIZE  (64 * 1024) // the optimal buffer size for sequential I/O on Windows NT/2k/XP
#define MAX_VERSION 1000000


class SecurityManager
{
public:
	SecurityManager(void);
	~SecurityManager(void);
	
	CString GetMAC();
	
	int DecryptLicenseFile(char* src,char* decryptedLicense);
	int EncryptLicenseFile(string dataToEncrypt, char *dst);
	bool IsLicenseValidForThisHardware(CString pathToLicenseFile, CString productVersion, bool allowAnyValidLicenseVersion);
	void SetMinAllowedVersion(double minAllowedVersion);
	void SetKey();
private:
	void gen_iv(unsigned char *buf, int size);
	unsigned char rawKey[MAX_KEY_SIZE];
	double minAllowedVersion;
	
};
