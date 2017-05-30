// CRCBumper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CRCBumper.h"
#include "Crc32Static.h"
#include "SecurityManager.h"
#include <process.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

static void test()
{
	SecurityManager mySecMan;
	//CString toEncrypt = "sdkfjsdlkfjsdklfjsdklfjsdkljfsdklfklsdfkljsdkljsdfksdfjsdjfklsdjfsdklfjsdkljfsdkljsdfklfjsdkllfjksdklfjsdkfjsdkljfsdklfjsdklfsdlfjsdkljfklsdjfsdklfjsdklsdkljfsdkljfsdkljsdljsdkljsdkljsdklfjsdklflfsdjklfhsdklfklsdfhsdklfj";
	//mySecMan.EncryptLicenseFile(toEncrypt.GetBuffer(),"C:\\Projects\\crcbumper\\release\\TestFile.txt");
	CString t;
	mySecMan.DecryptLicenseFile("C:\\Projects\\LicenseManager\\Release\\licenseManager.dll",t.GetBufferSetLength(500));
	printf(t.GetBuffer());
	exit(0);
	/*if(t.Compare(toEncrypt) == 0)
	{
		printf("ok");
	}
	else
	{
		printf("nok");
	}*/
	//printf("Decrypted data is: %s",t.GetBuffer());
	//exit(0);
}



int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{	
	//test();
	if(argv[1] == NULL || argv[2] == NULL)
	{
		printf("Usage:\nCRCBumper inFilePath pathToEncryptedCRC \n\nSample:\nCRCBumper c:\\myTestApp\\Release\\myTest.exe c:\\myTestEncryptedCRC.txt\n\n");
		return 0;
	}
	printf("argv[0]%s\n\n",argv[0]);
	printf("argv[1]%s\n\n",argv[1]);
	printf("argv[2]%s\n\n",argv[2]);
	
	

	int nRetCode = 0;
	CString CRCToWrite;
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		DWORD dwCrc32, dwErrorCode = NO_ERROR;
		CString CRCResult, returnResult;
		CString pathToDLL = argv[1];
		dwErrorCode = CCrc32Static::FileCrc32Win32(pathToDLL,dwCrc32);
		if(dwErrorCode == NO_ERROR)
		{
			CRCResult.Format(_T("0x%08x"), dwCrc32);
			printf("CRC calc as:%s\n\n",CRCResult);
			SecurityManager mySecMan;
			//mySecMan.SetKey(); //This does not work well with the licensemanager, keys might differ
			mySecMan.EncryptLicenseFile(CRCResult.GetBuffer(),argv[2]);
		}
		else
		{
			printf("CRC gen failed\n\n");
		}

		
	}
	//char* t;
	//scanf(t);
	return nRetCode;
}

