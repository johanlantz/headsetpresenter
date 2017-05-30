//LicenseManager by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2005
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "securitymanager.h"


SecurityManager::SecurityManager(void)
{
	    rawKey[0] = '7';
		rawKey[1] = '5';
		rawKey[2] = 'A';
		rawKey[3] = '1';
		rawKey[4] = 'E';
		rawKey[5] = 'E';
		rawKey[6] = '1';
		rawKey[7] = '8';
		rawKey[8] = 'D';
		rawKey[9] = 'C';
		rawKey[10] = 'F';
		rawKey[11] = 'E';
		rawKey[12] = 'A';
		rawKey[13] = '5';
		rawKey[14] = 'B';
		rawKey[15] = 'D';
		rawKey[16] = 'D';
		rawKey[17] = '0';
		rawKey[18] = '8';
		rawKey[19] = 'B';
		rawKey[20] = '3';
		rawKey[21] = '5';
		rawKey[22] = '5';
		rawKey[23] = '4';
		rawKey[24] = '2';
		rawKey[25] = '6';
		rawKey[26] = 'D';
		rawKey[27] = '7';
		rawKey[28] = '6';
		rawKey[29] = 'F';
		rawKey[30] = '8';
		rawKey[31] = 'A';
		rawKey[32] = '2';
		rawKey[33] = 'F';
		rawKey[34] = '2';
		rawKey[35] = '9';
		rawKey[36] = '2';
		rawKey[37] = 'E';
		rawKey[38] = '4';
		rawKey[39] = 'C';
		rawKey[40] = 'D';
		rawKey[41] = '5';
		rawKey[42] = '8';  //SALT BYTE switch 8 for a B in encrypt and decrypt
		rawKey[43] = '3';
		rawKey[44] = '1';
		rawKey[45] = '5';
		rawKey[46] = '8';
		rawKey[47] = 'F';
		rawKey[48] = '5';
		rawKey[49] = 'A';
		rawKey[50] = '7';
		rawKey[51] = '6';
		rawKey[52] = '2';
		rawKey[53] = '1';
		rawKey[54] = '1';
		rawKey[55] = 'A';
		rawKey[56] = 'B';
		rawKey[57] = '1';
		rawKey[58] = 'A';
		rawKey[59] = '6';
		rawKey[60] = 'F';
		rawKey[61] = 'D';
		rawKey[62] = '2';
		rawKey[63] = '3';

		minAllowedVersion = MAX_VERSION;  //init to dummy value
}

SecurityManager::~SecurityManager(void)
{
}

//Changes the rawKey so that LicensManager CRC check lic file does not use the same key as other applications 
//If the CRC is known the key can be broken 
//Most likely this will only be called by the app that generates the CRC for LicenseManager.dll
void SecurityManager::SetKey()
{
		rawKey[0] = '6';
		rawKey[1] = '1';
		rawKey[2] = 'B';
		rawKey[3] = '4';
		rawKey[4] = 'F';
		rawKey[5] = 'F';
		rawKey[6] = '5';
		rawKey[7] = '4';
		rawKey[8] = '9';
		rawKey[9] = 'B';
		rawKey[10] = 'D';
		rawKey[11] = 'A';
		rawKey[12] = '5';
		rawKey[13] = '1';
		rawKey[14] = '2';
		rawKey[15] = '7';
		rawKey[16] = 'F';
		rawKey[17] = 'D';
		rawKey[18] = 'B';
		rawKey[19] = '3';
		rawKey[20] = '2';
		rawKey[21] = '9';
		rawKey[22] = '8';
		rawKey[23] = '1';
		rawKey[24] = '7';
		rawKey[25] = 'D';
		rawKey[26] = 'C';
		rawKey[27] = 'F';
		rawKey[28] = '2';
		rawKey[29] = '2';
		rawKey[30] = 'A';
		rawKey[31] = '3';
		rawKey[32] = '1';
		rawKey[33] = '9';
		rawKey[34] = '2';
		rawKey[35] = '6';
		rawKey[36] = '9';
		rawKey[37] = 'B';
		rawKey[38] = '5';
		rawKey[39] = 'C';
		rawKey[40] = '8';
		rawKey[41] = '9';
		rawKey[42] = '8';  //SALT BYTE switch 8 for a B in encrypt and decrypt
		rawKey[43] = '2';
		rawKey[44] = '6';
		rawKey[45] = '1';
		rawKey[46] = '9';
		rawKey[47] = 'B';
		rawKey[48] = '7';
		rawKey[49] = 'B';
		rawKey[50] = '3';
		rawKey[51] = '4';
		rawKey[52] = '2';
		rawKey[53] = '8';
		rawKey[54] = '2';
		rawKey[55] = 'B';
		rawKey[56] = 'F';
		rawKey[57] = 'E';
		rawKey[58] = '6';
		rawKey[59] = '9';
		rawKey[60] = '1';
		rawKey[61] = '0';
		rawKey[62] = '2';
		rawKey[63] = 'F';
}

void SecurityManager::SetMinAllowedVersion(double minAllowedVersion)
{
		this->minAllowedVersion = minAllowedVersion;
}

CString SecurityManager::GetMAC()
{
 
  CString FullMACStringFromComputer = "";
  IP_ADAPTER_INFO AdapterInfo[16];       // Allocate information
                                         // for up to 16 NICs
  DWORD dwBufLen = sizeof(AdapterInfo);  // Save memory size of buffer

  DWORD dwStatus = GetAdaptersInfo(      // Call GetAdapterInfo
    AdapterInfo,                 // [out] buffer to receive data
    &dwBufLen);                  // [in] size of receive data buffer
 
  PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; // Contains pointer to
  CString tempMAC;                                          // current adapter info
  do {
		tempMAC.Format("%02X-%02X-%02X-%02X-%02X-%02X;", pAdapterInfo->Address[0],pAdapterInfo->Address[1],pAdapterInfo->Address[2],pAdapterInfo->Address[3],pAdapterInfo->Address[4],pAdapterInfo->Address[5]);
		FullMACStringFromComputer += tempMAC; 
    pAdapterInfo = pAdapterInfo->Next;    // Progress through
                                          // linked list
  }
  while(pAdapterInfo);                    // Terminate if last adapter
  Log("GetMAC returns: %s", FullMACStringFromComputer);
  return FullMACStringFromComputer;
}

bool SecurityManager::IsLicenseValidForThisHardware(CString pathToLicenseFile, CString productVersion, bool allowAnyValidLicenseVersion)
{
  //1. Get all MAC addresses as concatenated string
  CString FullMACStringFromComputer = "";
  IP_ADAPTER_INFO AdapterInfo[16];       // Allocate information
                                         // for up to 16 NICs
  DWORD dwBufLen = sizeof(AdapterInfo);  // Save memory size of buffer

  DWORD dwStatus = GetAdaptersInfo(      // Call GetAdapterInfo
    AdapterInfo,                 // [out] buffer to receive data
    &dwBufLen);                  // [in] size of receive data buffer
 
  PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; // Contains pointer to
  CString tempMAC;                                          // current adapter info
  do {
		tempMAC.Format("%02X-%02X-%02X-%02X-%02X-%02X;", pAdapterInfo->Address[0],pAdapterInfo->Address[1],pAdapterInfo->Address[2],pAdapterInfo->Address[3],pAdapterInfo->Address[4],pAdapterInfo->Address[5]);
		FullMACStringFromComputer += tempMAC; 
    pAdapterInfo = pAdapterInfo->Next;    // Progress through
                                          // linked list
  }
  while(pAdapterInfo);                    // Terminate if last adapter


  CString MACFromLicenseFile;
  int decryptResult;
  decryptResult = DecryptLicenseFile(pathToLicenseFile.GetBuffer(),MACFromLicenseFile.GetBufferSetLength(500));  //need to alloc size for the buffer before sending it to decrypt
  //Log("SM:MACFromLicenseFile :%s",MACFromLicenseFile);
  if(decryptResult == 0)
  {
	Log("License file is corrupted");
	return false; //if decryption fails just terminate
  }
  //MessageBox(NULL,MACFromLicenseFile,"decrypted MAC",MB_OK);
  CString PartialMACString;

	//Get LI and move startpos to after productVersion
    //if LI = 3 for instance if version is 1.4 then we set startpos to 3 (LI is at 0, 0+3 = 3)
	char LIasChar = MACFromLicenseFile.GetAt(0);
    int LI = atoi(&LIasChar);
	CString productVersionFromLicenseFile = MACFromLicenseFile.Mid(1,LI);
	Log("SM:ProductVersion in License file = %s",productVersionFromLicenseFile);
	//
	//double productVersionFromLicenseFileAsDouble = atof(productVersionFromLicenseFile.GetBuffer());
	Log("SM:productVersionFromLicenseFileAsDouble=%f",atof(productVersionFromLicenseFile.GetBuffer()));
	Log("SM:productVersionAsDouble=%f",atof(productVersion.GetBuffer()));
	Log("SM:MinAllowedVersion=%f",minAllowedVersion);

	if(atof(productVersionFromLicenseFile) != atof(productVersion) && atof(productVersionFromLicenseFile) < minAllowedVersion)
	{
		if(minAllowedVersion == MAX_VERSION)
		{
			MessageBox(NULL,"The license file " + pathToLicenseFile + " is not valid for this version of the application.\n\nVersion in license file = " + productVersionFromLicenseFile + "  \nVersion active = " + productVersion,"Incorrect version in license file",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		}
		else
		{
			CString minAllowedVersionAsString;
			minAllowedVersionAsString.Format("%f",minAllowedVersion);
			MessageBox(NULL,"The license file " + pathToLicenseFile + " is not valid for this version of the application.\n\nVersion in license file = " + productVersionFromLicenseFile + "  \nVersion active = " + productVersion + "\n\nMin required version in license = " + minAllowedVersionAsString,"Incorrect version in license file",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		}
		return false;
	}
	else
	{
		Log("SM:License OK");
	}
	//
	/*if(productVersionFromLicenseFile != productVersion && allowAnyValidLicenseVersion == false)
	{
		MessageBox(NULL,"The license file " + pathToLicenseFile + " is not valid for this version of the application.\n\nVersion in license file = " + productVersionFromLicenseFile + "  \nVersion active = " + productVersion,"Incorrect version in license file",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		return false;
	}*/
	int startPos= LI+1;  
	// end init

	int hit = 0;
	int noOfHits = 0;
	//MessageBox(NULL,MACFromLicenseFile.Mid(startPos,MACFromLicenseFile.GetLength()),"",MB_OK);
	while((hit = MACFromLicenseFile.Find(";",startPos)) != -1)
	{
		PartialMACString = MACFromLicenseFile.Mid(startPos,hit-startPos);
		Log("Partial MAC String = %s",PartialMACString);
		//MessageBox(NULL,PartialMACString,"partial MAC",MB_OK);
		startPos = hit+1;
		if(FullMACStringFromComputer.Find(PartialMACString,0) != -1)
		{
			noOfHits++;
		}
	}

	//Error handling
	//If the user has forgotten to add a terminating : to a single MAC address this will allow it anyway
	
	if(noOfHits == 0 && (FullMACStringFromComputer.Find(MACFromLicenseFile,0) != -1))
	{
			noOfHits++;
	}
	
	return noOfHits > 0 ? true : false;
}


void SecurityManager::gen_iv(unsigned char *buf, int size)
{
    while(--size >= 0) buf[size] += rand();
}

//If encrypt, encrypt the collected computer info in src and write it to dst
//If decrypt, decrypt the provided src string and use it with getMAC 
int SecurityManager::DecryptLicenseFile(char *src, char* decryptedLicense)
{
	int encrypt = FALSE;
	int totalSizeOfEncryptedData = 0;
	int LI_SIZE = 0;
    keyInstance keyInst;
    cipherInstance cipherInst;
    FILE *fsrc;//, *fdst;
    unsigned char keyMaterial[MAX_KEY_SIZE];
	unsigned char membuf[BLOCKSIZE + 32];
	//unsigned char initiv[MAX_IV_SIZE];
	//unsigned char rawKey[MAX_KEY_SIZE] = {'7','5','A','1','E','E','1','8','D','C','F','E','A','5','B','D','D','0','8','B','3','5','5','4','2','6','D','7','6','F','8','A','2','F','2','9','2','E','4','C','D','5','8','3','1','5','8','F','5','A','7','6','2','1','1','A','B','1','A','6','F','D','2','3'};
//    int i;
	int fsize;
	int sread = 0;
	int status = 0;
	int round = 0;

	
	LI_SIZE = 3; //If there is an encrypted CRC tail the LI is always 3 chars 
	

	//If decrypt we have a licensefile and want to return it in plain text
	if(!encrypt)
	{
		if((fsrc = fopen(src, "rb")) == NULL)  //source file check
		{
			MessageBox(NULL,"Missing file",src,MB_OK);
			perror(src);
			goto quit;
		}
	}

	//Get the size of the encrypted data
	fseek(fsrc, - 3, SEEK_END);
	char LI[4];
	fread(LI, 1, 3, fsrc);
	LI[3] = '\0';
	totalSizeOfEncryptedData = atoi(LI);
	//MessageBox(NULL,LI,"",MB_OK);
	

	//Instead of creating key from file we hardcode the key into rawkey then memcpy it to keymaterial
	//to alter the code as little as possible
	memcpy(keyMaterial,rawKey,MAX_KEY_SIZE);
	keyMaterial[42] = 'B';  //SALT pos 42 with a B instead of an 8

    //Get IV as last 16 bytes of encrypted file
    fseek(fsrc, -LI_SIZE - MAX_IV_SIZE, SEEK_END);        //LI_SIZE = -3 if crc otherwise 0
    if(MAX_IV_SIZE != fread(cipherInst.IV, 1, MAX_IV_SIZE, fsrc))
    {
		printf("e1", src);  //max iv size is invalid
		Log("SM:e1");
        goto quit; 
    }

	fsize = totalSizeOfEncryptedData;  
	fseek(fsrc, -totalSizeOfEncryptedData - LI_SIZE, SEEK_END);  //goto start of encrypted data
	
	//fsize = ftell(fsrc);  //size of source file	
	//rewind(fsrc);  //goto start of file, if decrypt src is the encrypted file
	
    if(!encrypt && (fsize < 32 || fsize % 16))
    {
        printf("e2", src);  //filesize is invalid  if this happens make sure that crc has really been appended to the dll or exe file, sometimes the second row in postbuild has not been copied
		Log("SM:e2");
        goto quit;
    }


    if(0 >= makeKey(&keyInst, encrypt ? DIR_ENCRYPT : DIR_DECRYPT, MAX_KEY_SIZE * 4, (char*)keyMaterial))
    {
        printf("e3");  //make key failed
		Log("SM:e3");
        goto quit;
    }
    cipherInst.mode = MODE_CBC;
	
	bool roundOne = true;
    while((sread = fread(membuf, 1, BLOCKSIZE, fsrc)) > 0)
    {
		if(roundOne)
		{
			sread -= 3;
			roundOne = false; //Only remove 3 chars the first time around, LI larger then 999 cannot be handled anyway and BLOCKSIZE is largen then that
		}
        fsize -= sread;
        
        if(sread == BLOCKSIZE && fsize > 32)
            blockDecrypt(&cipherInst, &keyInst, membuf, sread, membuf);
        else
        {
            if(fsize)
            {
                if(fsize != fread(membuf + BLOCKSIZE, 1, fsize, fsrc))
                {
                    printf("e4", src);  
					Log("SM:e4");
                    goto quit;
                }
                sread += fsize;
            }
            if(0 >= (sread = padDecrypt(&cipherInst, &keyInst, membuf, sread - MAX_IV_SIZE, membuf)))
            {
                printf("e5", src);
				Log("SM:e5");
                goto quit;
            }
        }
    
		//decrypting just save the data to return later
		
		memcpy(decryptedLicense, membuf,sread);  //this works for decrypt encrypt also need the initv below
		decryptedLicense[sread] = '\0';
	
        round++;
	
        //if(!(round % 16)) printf(".");
    }

    status = 1; // SUCCESS

quit:
    if(fsrc) fclose(fsrc);
    
    if(!encrypt) memset(membuf, 0, sizeof(membuf)); // sensitive data memory cleanup
    memset(keyMaterial, 0, sizeof(keyMaterial));
    makeKey(&keyInst, DIR_ENCRYPT, MAX_KEY_SIZE * 4, (char*)keyMaterial);
    return status;
}

int SecurityManager::EncryptLicenseFile(string dataToEncrypt, char *dst)
{
	int encrypt = TRUE;
	int totalSizeOfEncryptedData = 0;   //Used as LI 
    keyInstance keyInst;
    cipherInstance cipherInst;
    FILE *fdst;
    unsigned char keyMaterial[MAX_KEY_SIZE], membuf[BLOCKSIZE + 32], initiv[MAX_IV_SIZE];
	//unsigned char rawKey[MAX_KEY_SIZE] = {'7','5','A','1','E','E','1','8','D','C','F','E','A','5','B','D','D','0','8','B','3','5','5','4','2','6','D','7','6','F','8','A','2','F','2','9','2','E','4','C','D','5','8','3','1','5','8','F','5','A','7','6','2','1','1','A','B','1','A','6','F','D','2','3'};
    int i;
	int fsize;
	int sread = 0;
	int status = 0;
	int round = 0;

    if((fdst = fopen(dst, "wb")) == NULL)  //Destfile check binary?
    {
        printf("e6");
		Log("SM:e6");
        goto quit;
    }

	//Copy raw key to key material
    memcpy(keyMaterial,rawKey,MAX_KEY_SIZE);
	keyMaterial[42] = 'B';  //SALT pos 42 with a B instead of an 8

	//generate IV
    for(i = (BLOCKSIZE / 4) - 1; i >= 0; i--) sread += ((int*)membuf)[i];
    srand(sread ^ time(NULL));
    gen_iv(cipherInst.IV, MAX_IV_SIZE);
    memcpy(initiv, cipherInst.IV, MAX_IV_SIZE);

    //Get size of the data to encrypt
	fsize = dataToEncrypt.length();
    
    if(0 >= makeKey(&keyInst, encrypt ? DIR_ENCRYPT : DIR_DECRYPT, MAX_KEY_SIZE * 4, (char*)keyMaterial))
    {
        printf("e7");   //make key failed
        goto quit;
    }
    cipherInst.mode = MODE_CBC;

	//Copy the string contents and convert to unsigned since we want it in binary format
	memcpy(membuf,(unsigned char*)dataToEncrypt.c_str(),dataToEncrypt.length());

    sread = dataToEncrypt.length();
	while(sread > 0)  //We will only loop once, using sread to change code as little as possible
    {
        fsize -= sread;
        if(encrypt)
        {
            if(fsize)  //there is more data left then blocksize we will never end up here
            {
                blockEncrypt(&cipherInst, &keyInst, membuf, sread, membuf);
                memcpy(cipherInst.IV, membuf + BLOCKSIZE - MAX_IV_SIZE, MAX_IV_SIZE);
            }
            else  //data is less then blocksize, encrypt and fill with padding
			{
				sread = padEncrypt(&cipherInst, &keyInst, membuf, sread, membuf); 
			}
        }
        
	
		
		//memcpy(encryptedData, membuf,sread);  //this works for decrypt encrypt also need the initv below
		//encryptedData[sread] = '\0';
		
        if(sread != fwrite(membuf, 1, sread, fdst))  //membuf contains all data write it to dest file. If encrypt this is the encrypted file, if decrypt this is the file to be restored
        {
            printf("e8");
			Log("SM:e8");

            goto quit;
        }
        round++;
        //if(!(round % 16)) printf(".");
		totalSizeOfEncryptedData += sread;  //Save for CRC tail LI
		sread = 0;  //jj to terminate this impose a limit on the length of the string to encrypt. Probably
    }


    if(encrypt)
    {
        if(MAX_IV_SIZE != fwrite(initiv, 1, MAX_IV_SIZE, fdst))
        {
            printf("e9");
			Log("SM:e9");
            goto quit;
        }
		totalSizeOfEncryptedData += MAX_IV_SIZE;  //For CRC tail LI
    }
    status = 1; // SUCCESS

	//Store the encrypted data LI in all messages
	
	char LI[10];
	itoa(totalSizeOfEncryptedData,LI,10);
	//Make sure that LI is always 3 digits
	if(strlen(LI) == 3)
	{
		//OK already 3 digits 
	}
	else if(strlen(LI) == 2)
	{
		LI[3] = LI[2];  // \0
		LI[2] = LI[1];
		LI[1] = LI[0];
		LI[0] = '0';
	}
	else if(strlen(LI) == 1)
	{
		LI[3] = '\0';
		LI[2] = LI[1];
		LI[1] = LI[0];
		LI[0] = '0';

	}
	else
	{
		printf("LI is larger then 3 digits, this cannot be handled");
		Log("SM:LI is larger then 3 digits, this cannot be handled");
		MessageBox(NULL,"LI is larger then 3 digits, this cannot be handled","",MB_OK);
	}
	fwrite(LI, 1, strlen(LI), fdst);

quit:
    if(fdst)
    {
        fclose(fdst);
    }
    memset(keyMaterial, 0, sizeof(keyMaterial));
    makeKey(&keyInst, DIR_ENCRYPT, MAX_KEY_SIZE * 4, (char*)keyMaterial);
    return status;
}
