// LicenseHandler.cpp : Implementation of CLicenseHandler
//////////////////////////////////////////////////////////////////////
//LicenseManager by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2005
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include ".\licensehandler.h"
#include "EmailDialog.h"
#include "InternetManager.h"
#include "crc32static.h"
#include "ActivationKey.h"

#include <sys/timeb.h>
#include <time.h>


CLicenseHandler::CLicenseHandler()
{
	Log("LicenseHandler Constructor");
	CK=13291;
	activated = false;
	mySecurityManager = new SecurityManager();
	myRegKeyDialog = new CRegKeyDialog();
	allowAnyValidLicenseVersion = false; //do not allow free updates
	SetHomeDirPath();
	registrationComment = "";
	CRCCheckInternal(applicationHomeDir + "LicenseManager.dll", "");  //CRC check the lic man
}

CLicenseHandler::~CLicenseHandler()
{
	LogEnterFunction("LicenseHandler destructor");
	delete mySecurityManager;
	delete myRegKeyDialog;
}

void CLicenseHandler::SetHomeDirPath()
{
	HMODULE phModule;
	TCHAR fullPathToDLL[512];
	phModule = GetModuleHandle("LicenseManager.dll");
	if(phModule == NULL)
	{
		MessageBox(NULL,"Failed to get instance path. Please contact support","LicenseManger",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
	}
	else
	{
		//Get the dir path only and set \0 after the final \ in the path to remove the filename of the dll
		DWORD dwResult = ::GetModuleFileName(phModule,fullPathToDLL, sizeof(fullPathToDLL));
		char* t = _tcsrchr(fullPathToDLL,'\\');
		t++;
		*t = '\0';
		applicationHomeDir = fullPathToDLL;
		Log("Application home dir =%s",applicationHomeDir);
		//MessageBox(NULL,fullPathToDLL,"",MB_OK);
	}
}
STDMETHODIMP CLicenseHandler::CreateLicFile(BSTR productName, BSTR productVersion, BSTR* statusAsString)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(!activated)
	{
		MessageBox(NULL,"not activated","",MB_OK);
		return S_FALSE;
	}
	Log("CreateLicFile");
	CString licenseStatus;
	
	myRegKeyDialog->SetProductInfo(CString(productName), CString(productVersion));
	INT_PTR keyFormStatus;
	int retryMsgBoxResult = IDRETRY;
	bool breakLoop = false;
	do
	{
		keyFormStatus = myRegKeyDialog->DoModal();
		if(keyFormStatus == IDOK)
		{
			licenseStatus = GetLicenseStringFromServer(myRegKeyDialog->GetKeyString(),CString(productName),CString(productVersion));
			Log("licenseStatus=%s", licenseStatus);
			if(licenseStatus == "OK")
			{
				GenerateLicenseAndWriteToFile(CString(productName), CString(productVersion));
			}
			else if(licenseStatus == "INVALIDIDENTIFIER")  //just to allow the user to retype the code if it is a typing error
			{
				retryMsgBoxResult = MessageBox(NULL,"The supplied activation code is invalid.","LicenseManager",MB_RETRYCANCEL|MB_TOPMOST|MB_ICONINFORMATION);	
			}
			else if(licenseStatus.Left(13) == "NOLICENSELEFT" && licenseStatus.GetLength() > 18 )  //if there is keyData attached recreate license
			{
				RecreateLicense(CString(productName), CString(productVersion), licenseStatus.Mid(14,licenseStatus.GetLength()-1));
				licenseStatus = licenseStatus.Left(13);
				CString msg = "The license for this product has already been delivered.\n\nThe original license file has been reconstructed and if you have not changed hardware you can use " + CString(productName) +" as usual.\n\nIf you have questions regarding the license policy of " + CString(productName) +" please contact support.\n\nPlease restart the application to activate the license.";
				MessageBox(NULL,msg,"LicenseManager",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
				breakLoop = true;  //otherwise we will show the enter identifier field again
			}
			else if(licenseStatus == "WRONGPRODUCT")
			{
				CString msg = "The application " + CString(productName) + " is not valid for this license key.";
				MessageBox(NULL,msg,"Unknown application",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
				breakLoop = true;
			}
			else if(licenseStatus == "WRONGVERSION")
			{
				CString msg = "The supplied activation code is not valid for this version of " + CString(productName) + ".\n\n";
				msg = msg + "Application version = " + CString(productVersion) + "\n\n";
				msg = msg + "License key version = " + licenseStatus.Mid(13,licenseStatus.GetLength()-1) + "\n\n";
				msg = msg + "If you have any questions concering you license please contact support.";
				MessageBox(NULL,msg,"License key not valid for this version",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
				licenseStatus = licenseStatus.Left(12);
				breakLoop = true;
			}
			else if(licenseStatus == "EXPIRED")
			{
				MessageBox(NULL,"The supplied license key has expired.\n\nThe application could not be licensed","License key has expired",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
				breakLoop = true;
			}
			else
			{
				MessageBox(NULL,"The license server did not respond or an Internet Connection was not available.\n\nPlease contact support.","Server Error",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
				breakLoop = true;
			}
		}
	}while(keyFormStatus == IDOK && licenseStatus != "OK" && retryMsgBoxResult == IDRETRY && breakLoop == false );
	
	//explicit handle if the user presses cancel in the dialog
	if(keyFormStatus == IDCANCEL)
	{
		Log("User pressed cancel");
		licenseStatus = "USERCANCEL";
	}

	*statusAsString = licenseStatus.AllocSysString();
	

	return S_OK;
}

CString CLicenseHandler::GetLicenseStringFromServer(CString identifier, CString productName, CString productVersion)
{
	LogEnterFunction("GetLicenseStringFromServer");
	CString returnData;
	CString fullMACString = mySecurityManager->GetMAC();
	InternetManager myInternetManager;
	CString queryString;
	queryString = "/access/requestLicense.asp";
	queryString += "?identifier=" + identifier + "&keyData=" + fullMACString;
	queryString += "&prodName=" + productName + "&prodVersion=" + productVersion + "&registrationComment=" + registrationComment;
	//We should most likely send minVersionAllowed here or the version number should be returned from the net, otherwise we will approve even old license codes. MinAllowedVersion is only used to verify an old license file
	Log("QueryString: %s",queryString);
	int retVal = IDOK;
	//This do while, will handle the scenario that the user has the setting "Never dial a dial up connection
	do
	{
		returnData = myInternetManager.SendRequestToServer(queryString);
		Log("SendRequestToServer returned:%s",returnData);
		if(returnData == "SERVER_ERROR_OR_NO_CONNECTION")
		{
			retVal = MessageBox(NULL,"The license server could not be contacted.\n\nPlease make sure that:\n1. The computer is connected to the Internet.\n2. You do not have a firewall blocking the HeadsetPresenter from contacting the license server.\nIf you run for instance McAfee or ZoneAlarm you have to temporary disable them while activating your license.\n\nCheck your connection or firewall settings then press Retry.\n\nIf this does not work contact support to get your license.", "No Internet Connection",MB_RETRYCANCEL|MB_ICONQUESTION |MB_TOPMOST);
		}
		else
		{
			retVal = IDOK;  //To break loop in a controlled way if it failed the first time returnData is IDRETRY
		}
	}while(retVal == IDRETRY);
	Log("Returning: %s",returnData);
	return returnData;
	/*CHttpFile* httpFile=NULL;
	CString errorMsg ="";
	CString returnData;
	CString fullMACString = mySecurityManager->GetMAC();
	CString httpRequestString = httpAddress; //base address to w2solutions
	httpRequestString = httpRequestString + "requestLicense.asp";
	httpRequestString = httpRequestString + "?identifier=" + identifier + "&keyData=" + fullMACString;
	httpRequestString = httpRequestString + "&prodName=" + productName + "&prodVersion=" + productVersion;
	try
	{
		httpFile = (CHttpFile*)iSession->OpenURL(httpRequestString);
		//MessageBox(NULL,"openURL succeded","",MB_OK|MB_TOPMOST);
		if(httpFile == NULL)
		{
			MessageBox(NULL,"Http could not be created.\n","SERVERERROR",MB_OK|MB_TOPMOST);
			returnData = "SERVERERROR";
		}
	}
	catch(CInternetException* ex)
	{
		TCHAR  szCause[255];
		ex->GetErrorMessage(szCause, 255);
		
		errorMsg.Format(_T("The License Activation requires an Internet connection to verify your license.\n\nPlease connect to the Internet and try again.\n\nError: #%u."),ex->m_dwError);
		errorMsg += szCause;
		MessageBox(NULL,(LPCTSTR)errorMsg,"No Internet Connection",MB_OK|MB_TOPMOST);
		returnData = "USERCANCEL"; //this happens if the user cancels a dial up or if no internet connection could be established. For instance if the Connection setting is set to never dial a connection AND there is no Ethernet connected.
	}
	

	if(httpFile != NULL)
	{
		try
		{
			httpFile->ReadString(returnData);
			//MessageBox(NULL,returnData,"server returned",MB_OK|MB_TOPMOST);
		}
		catch(CInternetException* ex)
		{
			MessageBox(NULL,"Read failed","",MB_OK|MB_TOPMOST);
			returnData = "READFAILED";
		}
		try
		{
			httpFile->Close();
		}
		catch(CInternetException* ex){}
	}
	return returnData;
	*/
}



/* This method works in two ways:
1. Normal case productName and version is supplied since no license file exist on computer and server returned ok 
   to create license.
2. Special case, user requested a license but licensesLeft == 0 so we recreate a license with the info
   sent from the server
*/
bool CLicenseHandler::GenerateLicenseAndWriteToFile(CString productName, CString productVersion, CString noLicenseLeftSuppliedMAC)
{
	LogEnterFunction("GenerateLicenseAndWriteToFile");
	productName.Insert(0,applicationHomeDir);
	/*productName.Append(productVersion);*/
	productName.Append(".lic");
	CString macAddressesConcatenated;
	CString encryptionString;
	if(noLicenseLeftSuppliedMAC == "")
	{
		macAddressesConcatenated = mySecurityManager->GetMAC();
		Log("macAddressesConcatenated: %s", macAddressesConcatenated);
	}
	else
	{
		Log("noLicenseLeftSuppliedMAC used: ", macAddressesConcatenated);
		macAddressesConcatenated = noLicenseLeftSuppliedMAC;
	}
	char LI[10];
	itoa(productVersion.GetLength(),LI,10);
	encryptionString = LI + productVersion + macAddressesConcatenated;
	//MessageBox(NULL,encryptionString,"",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
	mySecurityManager->EncryptLicenseFile(encryptionString.GetBuffer(),productName.GetBuffer());
	return true;
}

//not used at this time
STDMETHODIMP CLicenseHandler::get_status(LONG* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*pVal = 1;
	return S_OK;
}

void CLicenseHandler::RecreateLicense(CString productName, CString productVersion, CString noLicenseLeftSuppliedMAC)
{
	LogEnterFunction("RecreateLicense");
	CString licenseFilePath = applicationHomeDir+productName /*+ productVersion*/ + ".lic";
	try
	{
		CFile myFile(licenseFilePath, CFile::modeCreate | CFile::modeNoTruncate| CFile::shareDenyNone | CFile::modeReadWrite );
		if(myFile.GetLength() > 0)
		{	
			MessageBox(NULL,"A license has been requested AND a license file exist.\n\nThis should not happen.\n\nPlease contact the support for " + productName,"NoLicenseLeft AND licenseExist", MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		}
		else
		{
			myFile.Close();  // Avoid conflict with license creator
			GenerateLicenseAndWriteToFile(CString(productName), CString(productVersion));
		}
	}
	catch(CFileException* pEx)
    {
	  TCHAR  szCause[255];
	  pEx->GetErrorMessage(szCause, 255);
	  CString errorMsg = "Exception in File open when recreating license, error msg is "; 
	  errorMsg += szCause;
	  Log("%s",errorMsg);
	  MessageBox(NULL,"Exception thrown by File open in recreate license\n\nPlease contact support if this issue persist","",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
	  pEx->Delete();
	}
}

STDMETHODIMP CLicenseHandler::CheckLicense(BSTR productName, BSTR productVersion, BSTR* resultAsString)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(!activated)
	{
		MessageBox(NULL,"not activated","",MB_OK);
		return S_FALSE;
	}
	Log("CheckLicense");
	CString res;
	CString productNameAsCString = CString(productName);
	productNameAsCString.Insert(0,applicationHomeDir);
	/*productNameAsCString.Append(CString(productVersion));*/
	productNameAsCString.Append(".lic");
	Log("Check license file for: %s",productNameAsCString);
	CFile myLicFile;
	bool licFileExists = false;
	try
	{
		if(myLicFile.Open(productNameAsCString,CFile::modeRead,NULL))
		{
			licFileExists = true;
			Log("License file exists");
		}
		myLicFile.Close();	
	}
   catch(CFileException* pEx)
   {
	   pEx->Delete();
      // Simply show an error message to the user.
   }
	
	if(licFileExists)
	{
		if(mySecurityManager->IsLicenseValidForThisHardware(productNameAsCString, CString(productVersion),allowAnyValidLicenseVersion))
		{
			Log("LicenseFile existed and was ok");
			res = "OK";
		}
		else
		{
			Log("An invalid license file existed");
			MessageBox(NULL, "The license file " + productNameAsCString + " is not valid for this computer.\n\nIf you have questions regarding the licensing policy of " + CString(productName) + " please contact support.\n\n" + CString(productName)+ " will run in limited mode.","License not valid",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
			res = "NOK";
		}
	}
	else
	{
		res = "NOK";	
	}
	Log("CheckLicense returns %s",res);
	*resultAsString = res.AllocSysString();
	return S_OK;
}


STDMETHODIMP CLicenseHandler::AllowAnyValidLicenseVersion(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(!activated)
	{
		MessageBox(NULL,"not activated","",MB_OK);
		return S_FALSE;
	}
	MessageBox(NULL,"The function AllowAnyValidVersion is deprecated, use SetMinVersionAllowed instead","",MB_OK);

	return S_OK;
}

STDMETHODIMP CLicenseHandler::QueryForEmail(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(!activated)
	{
		MessageBox(NULL,"not activated","",MB_OK);
		return S_FALSE;
	}
	INT_PTR emailFormStatus;
	CEmailDialog myEmailDialog;
	myEmailDialog.SetServerPath(httpAddress);
	myEmailDialog.SetKeyString(myRegKeyDialog->GetKeyString());
	myEmailDialog.SetRegistrationComment(registrationComment);
	emailFormStatus = myEmailDialog.DoModal();
	if(emailFormStatus == IDOK)
	{
		MessageBox(NULL,"Thank you for registering.\n\nThe application is now licensed.","Registration complete",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(NULL,"The application has been successfully licensed.","License activation complete",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
	}
	return S_OK;
}

STDMETHODIMP CLicenseHandler::SetRegistrationComment(BSTR iregistrationComment)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(!activated)
	{
		MessageBox(NULL,"not activated","",MB_OK);
		return S_FALSE;
	}
	Log("Set registration comment to: %s", registrationComment);
	registrationComment = CString(iregistrationComment);
	
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CLicenseHandler::CRCCheckSelf(BSTR fullPathToApplication, BSTR fullPathToNonAppendedCRC, BSTR* resultAsString)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(!activated)
	{
		MessageBox(NULL,"not activated","",MB_OK);
		return S_FALSE;
	}
	CString returnResult = "NOK";
	
	if(CRCCheckInternal(CString(fullPathToApplication), CString(fullPathToNonAppendedCRC)) == true)
	{
		returnResult = "OK";
	}
	
	*resultAsString = returnResult.AllocSysString();
	return S_OK;
}

bool CLicenseHandler::CRCCheckInternal(CString pathToApplication, CString pathToNonAppendedCRC)
{
	LogEnterFunction("LH:CRCCheckInternal");
	//Log("LH:Changing");
	//mySecurityManager->SetKey(); //This does not work since after CRC has been calc the key changes, then if a lic file is created it is created with the CRC key
	bool crcIsAppended = true;

	CString pathToFileWithCRC;
	if(pathToNonAppendedCRC == "")
	{
		pathToFileWithCRC = pathToApplication;
		Log("LH:CRC is appended, path to decrypt is :%s", pathToFileWithCRC);
	}
	else
	{
		pathToFileWithCRC = pathToNonAppendedCRC;
		Log("LH:CRC is not appended, path to decrypt is :%s", pathToFileWithCRC);
		crcIsAppended = false;
	}
	Log("LM:FilePath=%s",pathToFileWithCRC);
	CString CRC;
	mySecurityManager->DecryptLicenseFile(pathToFileWithCRC.GetBuffer(), CRC.GetBufferSetLength(500));
	DWORD dwCrc32, dwErrorCode = NO_ERROR;
	CString CRCResult, returnResult;
	Log("LH:CRCCheckInternal, pathToFileWithCRC is:%s",pathToFileWithCRC);
	
	if(crcIsAppended)
	{
		dwErrorCode = CCrc32Static::FileCrc32Win32Mod(pathToFileWithCRC,dwCrc32);
		Log("LH:CRC is appended, calling modified routine");
	}
	else
	{
		dwErrorCode = CCrc32Static::FileCrc32Win32(pathToApplication,dwCrc32);
		Log("LH:CRC is not appended, calling standard routine");
	}
	
	if(dwErrorCode == NO_ERROR)
	{
		Log("LH:CRC returned NO_ERROR");
		CRCResult.Format(_T("0x%08x"), dwCrc32);
		//Log("LH:CRC from decrypted file : %s",CRC);
		//Log("LH:CRC from calc : %s",CRCResult);
		if(CRCResult.CompareNoCase(CString(CRC)) == 0)
		{
			Log("LH:CRC is a match");
			return true;
		}
		else
		{
			Log("LH:CRC is not a match");
			MessageBox(NULL,"The CRC check for " + pathToApplication + " failed.\n\nThe application has been tampered with and might not be fully usable.","LicenseManager CRC Error",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
			return false;
		}
	}
	else
	{
		CRCResult.Format(_T("CRC returned Error:[0x%08x]"), dwErrorCode);
		Log("LH:%s",CRCResult);
		MessageBox(NULL,CRCResult,"CRC error",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
		return false;
	}
}
STDMETHODIMP CLicenseHandler::SetMinVersionAllowed(DOUBLE minVersionAllowed, BSTR* resultAsString)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(!activated)
	{
		MessageBox(NULL,"not activated","",MB_OK);
		return S_FALSE;
	}
	mySecurityManager->SetMinAllowedVersion(minVersionAllowed);
	CString returnResult = "OK";
	*resultAsString = returnResult.AllocSysString();
	return S_OK;
}

STDMETHODIMP CLicenseHandler::Authenticate(INT challenge, INT* response)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(!activated)
	{
		MessageBox(NULL,"not activated","",MB_OK);
		return S_FALSE;
	}
	*response = challenge ^ CK;
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CLicenseHandler::Activate(INT secretKey)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if(secretKey == ACTIVATION_KEY)//to simple change later
	{
		activated = true;
	}
	else
	{
		MessageBox(NULL,"Invalid activation key","",MB_OK);
		Sleep(1000);
		return S_FALSE;
	}

	return S_OK;
}
