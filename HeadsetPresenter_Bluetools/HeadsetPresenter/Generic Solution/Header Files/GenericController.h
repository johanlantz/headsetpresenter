//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2005
//////////////////////////////////////////////////////////////////////
// GenericController.h: interface for the Controller class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ONTROLLER_H__A2E8AC3F_F926_455C_B634_DC6E0B5EF051__INCLUDED_)
#define AFX_ONTROLLER_H__A2E8AC3F_F926_455C_B634_DC6E0B5EF051__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SuperController.h"
#include "SnoopControl.h"
#include "FileParser.h"
#include "ManualConnectDialog.h"
class CHeadsetPresenterDlg;



class GenericController : public SuperController
{
public:
	GenericController();
	 ~GenericController();

	void SetHomeDirPath();
	bool Start();
	bool Stop();
	bool isActive();
	void getHelp(int codeID);


	bool ConnectToLastUsedDevice();
	bool ShowDeviceSearchDialog();
	void DiscoverDevices(bool refresh);
	void DeviceSelected(int deviceIndex);
	CStringList* GetServiceListAsCStringList();
	bool ConnectToSelectedDevice(int SCN, CString serviceName );
	void DiscoverServicesAsync();
	bool DiscoverServicesSync();
	int GetNoOfServicesDiscovered();
	int GetSCNOfService(CString serviceNameToFind);
	bool IsRadioAvailable();
	bool ShouldHPQueryForPIN(){return false;}
	void UnbondDevice() {}
	bool BondDevice() {return true;}
	void Write(unsigned char* data){}
	void CancelDeviceDiscovery() {};

	void OnReadCompleted(char* data);

	void SetReplugMode(int iReplugMode);
	int GetMode();
private:
	SnoopControl mySnoopControl;
	FileParser* myFileParser;
	
	CString FullMACString;
	CString defaultWebAddress;
	CString defaultVisitSiteMsg;
	CString applicationHomeDir;
	ManualConnectDialog myManualConnectDlg;
	void InitLocalSettings();
	
	void CheckService();
	void OnStartService();
	void OnStopService();
	void OnDeleteService();
	void OnCreateService();

	ULONG m_LoggingState;
	void ResumeLogging();
	void PauseLogging();
	void CloseLogFile();
	void LogDelete();

	void PrepareForHardRestart();
	void PreHardRestart();
	void PostHardRestart();
	bool DoSoftRestart();
	void QuitLogging();
	void CheckLogFile();

};

#endif // !defined(AFX_ONTROLLER_H__A2E8AC3F_F926_455C_B634_DC6E0B5EF051__INCLUDED_)
