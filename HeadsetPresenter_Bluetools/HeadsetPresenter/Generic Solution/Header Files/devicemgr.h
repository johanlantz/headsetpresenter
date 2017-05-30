//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2005
//////////////////////////////////////////////////////////////////////
// devicemgr.h: interface for the CDeviceMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVICEMGR_H__3D8E001E_8F97_4FDF_A282_95C8D77F56D3__INCLUDED_)
#define AFX_DEVICEMGR_H__3D8E001E_8F97_4FDF_A282_95C8D77F56D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "device.h"
#include "SettingsControl.h"

#include <setupapi.h>
#include <afxtempl.h>

/*
	To loop over all USB devices, use the following code:

  	CDeviceMgr mgr;
	POSITION pos;

	pos = mgr.GetHeadPosition();
	while (pos != NULL)
	{
		CDevice& device = mgr.GetNext(pos);

		... use device ...
	}
*/


class CDeviceMgr  
{
public:
	void Dump();
	POSITION FindDevice(CString hardwareID);
	POSITION FindDeviceBasedOnDriver(CString hardwareID);  //Johan 
	CDevice& GetFoundDevice(POSITION pos);

	inline POSITION GetHeadPosition() { return deviceList.GetHeadPosition(); };
	inline CDevice& GetNext(POSITION& pos) { return deviceList.GetNext(pos); };

	CDeviceMgr();
	virtual ~CDeviceMgr();
	CString EliminateDevices();
	CDevice& GetFirstBTDevice();
	void SetSettingsControl(SettingsControl* iSettingsControl);
	CDevice d;
	

private:
	CList<CDevice,const CDevice&> deviceList;
	CList<CDevice,const CDevice&> deviceListOnlyBT;
	CString SearchForKnownDevice(CStringList& knownDevices);
	CString TryToIdentifyOneUnknownDevice();
	
	void BuildKnownDeviceList(CStringList& knownDevices);
	void CheckFileForDevice(CStringList& knownDevices);
	void WriteUnknownDeviceToFile();
	void SetHomeDirPath();
	CString applicationHomeDirPath;
	HDEVINFO m_hDev;
	SettingsControl* mySettingsControl;
	
};

#endif // !defined(AFX_DEVICEMGR_H__3D8E001E_8F97_4FDF_A282_95C8D77F56D3__INCLUDED_)
