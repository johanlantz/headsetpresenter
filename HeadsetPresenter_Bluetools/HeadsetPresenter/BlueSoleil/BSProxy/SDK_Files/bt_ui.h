/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* Copyright (c) 1999-2006 IVT Corporation
*
* All rights reserved.
* 
---------------------------------------------------------------------------*/

/////////////////////////////////////////////////////////////////////////////
// Module Name:
//  bt_ui.h
// Abstract:
//	Included in this file are the BlueSoleil's Bluetooth function interfaces
//  exported for other applications.
// 
// Author:
//	TD, LLH, HZJ, LZC, ZL
// Revision History:
//  10/30/2004 16:05:10 PM	LZC	Created
/////////////////////////////////////////////////////////////////////////////
//
// API version: 0.83
//
/////////////////////////////////////////////////////////////////////////////
#ifndef		__BT_UI_H__
#define		__BT_UI_H__

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BTEXPORT

/////////////////////////////////////////////////////////////////////////////
// General constants
#define DEVICE_ADDRESS_LENGTH			6
#define DEVICE_CLASS_LENGTH				3
#define MAX_DEVICE_NAME_LENGTH			64
#define MAX_PIN_CODE_LENGTH          16
	
#define MAX_SERVICE_NAME_LENGTH		128
	
//Connection state
#define STATE_CONNECTED				1
#define STATE_DISCONNECTED			0
	
//Inquiry mode
#define INQUIRY_GENERAL_MODE				0x00
#define INQUIRY_LIMITED_MODE				0x01
#define INQUIRY_PAIRED				0x02
#define INQUIRY_GENERAL_REFRESH		0x03
	
	
//API calling status code
#define BTSTATUS_FAIL					0x00000000	//General fail
#define BTSTATUS_SUCCESS				0x00000001	//General success
#define BTSTATUS_SYSTEM_ERROR			0x00000002
#define BTSTATUS_BT_NOT_READY			0x00000003
#define BTSTATUS_ALREADY_PAIRED			0x00000004	//BlueSoleil is already paired with the device
#define BTSTATUS_AUTHENTICATE_FAILED	0x00000005	//Authentication fails. 
#define BTSTATUS_BT_BUSY				0x00000006	//Bluetooth is busy with browsing services or connecting to a device.
#define BTSTATUS_CONNECTION_EXIST		0x00000007	//The connection to the service is already established.
#define BTSTATUS_CONNECTION_NOT_EXIST	0x00000008	//The connection does not exist or is released.
#define BTSTATUS_PARAMETER_ERROR		0x00000009
#define BTSTATUS_SERVICE_NOT_EXIST		0x0000000a
#define BTSTATUS_DEVICE_NOT_EXIST		0x0000000b
	
/////////////////////////////////////////////////////////////////////////////
// DUN
#define DUN_MAX_NAME_LENGTH			64
#define DUN_MAX_PASSWORD_LENGTH		64
#define DUN_MAX_DIAL_NUMBER_LENGTH	64
	
#define DUN_SET_NONE				0x0000
#define DUN_SET_USER_NAME			0x0001
#define DUN_SET_PASSWORD			0x0002
#define DUN_SET_DIAL_NUMBER			0x0004
	
/////////////////////////////////////////////////////////////////////////////
// OPP
#define OPP_COMMAND_PUSH			0x0001
#define OPP_COMMAND_PULL			0x0002
	
	
/////////////////////////////////////////////////////////////////////////////
// SYNC
#define SYNC_VCARD					0x0001
#define SYNC_VCAL					0x0002
#define SYNC_NOTE					0x0004
#define SYNC_VMESSAGE				0x0008
	
/////////////////////////////////////////////////////////////////////////////
// Service class 16bits UUIDs
#define CLS_SERIAL_PORT				0x1101		//Serial Port
#define CLS_LAN_ACCESS				0x1102		//LAN Access
#define CLS_DIALUP_NET				0x1103		//Dial-Up Networking
#define CLS_IRMC_SYNC				0x1104
#define CLS_OBEX_OBJ_PUSH			0x1105		//Object Push
#define CLS_OBEX_FILE_TRANS			0x1106		//File Transfer
#define CLS_IRMC_SYNC_CMD			0x1107		//Information Synchronization
#define CLS_HEADSET					0x1108		//Headset AG
#define CLS_CORDLESS_TELE			0x1109
#define CLS_AUDIO_SOURCE			0x110A	
#define CLS_AUDIO_SINK				0x110B
#define CLS_AVRCP_TG				0x110C
#define CLS_ADV_AUDIO_DISTRIB		0x110D
#define CLS_AVRCP_CT				0x110E
#define CLS_VIDEO_CONFERENCE		0x110F
#define CLS_INTERCOM				0x1110
#define CLS_FAX						0x1111
#define CLS_HEADSET_AG				0x1112
#define CLS_WAP						0x1113
#define CLS_WAP_CLIENT				0x1114
	
#define CLS_PAN_PANU				0x1115		//Personal Area Networking
#define CLS_PAN_NAP					0x1116		//Personal Area Networking
#define CLS_PAN_GN					0x1117		//Personal Area Networking
	
#define CLS_DIRECT_PRINT			0x1118
#define CLS_REF_PRINT				0x1119
#define CLS_IMAGING					0x111A		//Basic Image
#define CLS_IMAG_RESPONDER			0x111B		//Basic Image
#define CLS_IMAG_AUTO_ARCH			0x111C		//Basic Image
#define CLS_IMAG_REF_OBJ			0x111D		//Basic Image
#define CLS_HANDSFREE				0x111E
#define CLS_HANDSFREE_AG			0x111F
#define CLS_HID						0x1124
#define CLS_HCRP					0x1125
#define CLS_HCR_PRINT				0x1126
#define CLS_HCR_SCAN				0x1127
#define CLS_SIM_ACCESS				0x112D
#define CLS_PNP_INFO				0x1200
#define CLS_GENERIC_NET				0x1201
#define CLS_GENERIC_FILE_TRANS		0x1202
#define CLS_GENERIC_AUDIO			0x1203
#define CLS_GENERIC_TELE			0x1204


//Mask for device information
#define MASK_DEVICE_NAME		0x00000001
#define MASK_DEVICE_CLASS		0x00000002
#define MASK_DEVICE_ADDRESS		0x00000004
#define MASK_LMP_VERSION		0x00000008
#define MASK_GET_DATA_COUNT		0x00000010
#define MASK_CONNECT_STATUS		0x00000040
#define MASK_PAIR_STATUS		0x00000080
#define MASK_CLOCK_OFFSET		0x00000200
#define MASK_DATA_RATE			0x00000400
#define MASK_SIGNAL_STRENGTH	0x00000800

//Events to register
#define EVENT_CONNECTION_STATUS					0x03
#define EVENT_DUN_RAS_CALLBACK					0x04
#define EVENT_ERROR								0x05
#define EVENT_INQUIRY_DEVICE_REPORT				0x06
#define EVENT_SPPEX_CONNECTION_STATUS			0x07
#define EVENT_BLUETOOTH_STATUS					0x08

#define ISVALIDEVENT(event) ((event >= EVENT_CONNECTION_STATUS) \
							&&(event <= EVENT_BLUETOOTH_STATUS))

//For EVENT_CONNECTION_STATUS and EVENT_SPPEX_CONNECTION_STATUS
#define STATUS_INCOMING_CONNECT		0x01
#define STATUS_OUTGOING_CONNECT		0x02
#define STATUS_INCOMING_DISCONNECT		0x03
#define STATUS_OUTGOING_DISCONNECT		0x04

//For EVENT_BLUETOOTH_STATUS
#define STATUS_BLUETOOTH_STARTED	0x01
#define STATUS_BLUETOOTH_STOPED		0x02

//Structures
typedef struct _BLUETOOTH_DEVICE_INFO {
	DWORD dwSize;
	BYTE address[DEVICE_ADDRESS_LENGTH];
	BYTE classOfDevice[DEVICE_CLASS_LENGTH];
	CHAR szName[MAX_DEVICE_NAME_LENGTH];
	BOOL bPaired;
} BLUETOOTH_DEVICE_INFO, *PBLUETOOTH_DEVICE_INFO;

typedef struct _BLUETOOTH_DEVICE_INFO_EX {
	DWORD dwSize;
	BYTE address[DEVICE_ADDRESS_LENGTH];
	BYTE classOfDevice[DEVICE_CLASS_LENGTH];
	CHAR szName[MAX_DEVICE_NAME_LENGTH];
	BOOL bPaired;
	UCHAR ucLmpVersion;
	WORD wManuName;
	WORD wLmpSubversion;
	BYTE reserved[16];
	WORD wClockOffset;
	BOOL bConnected;
	DWORD dwDataRecvBytes;
	DWORD dwDataSentBytes;
	CHAR cSignalStrength;
} BLUETOOTH_DEVICE_INFO_EX, *PBLUETOOTH_DEVICE_INFO_EX;

typedef struct _DUN_CLIENT_PARAM {
	DWORD dwSize;
	WORD wShortcutFlags;
	BOOL bAutoDial;
	CHAR szUserName[DUN_MAX_NAME_LENGTH];
	CHAR password[DUN_MAX_PASSWORD_LENGTH]; 
	CHAR dialNumber[DUN_MAX_DIAL_NUMBER_LENGTH]; 
}DUN_CLIENT_PARAM,*PDUN_CLIENT_PARAM;


typedef struct _OPP_CLIENT_PARAM {
	DWORD dwSize;
	WORD wCmdFlags;
	CHAR szObjectPath[MAX_PATH];
}OPP_CLIENT_PARAM,*POPP_CLIENT_PARAM;


typedef struct _SYNC_CLIENT_PARAM{
	DWORD dwSize;
	BOOL bShowSyncDlg;
	UCHAR ucSyncType;
}SYNC_CLIENT_PARAM,*PSYNC_CLIENT_PARAM;


typedef struct _SPP_CLIENT_PARAM{
	DWORD dwSize;
	UCHAR ucComPort;
}SPP_CLIENT_PARAM,*PSPP_CLIENT_PARAM;

typedef struct _GENERAL_SERVICE_INFO {
	DWORD dwSize;
	DWORD dwServiceHandle;
	WORD wServiceClassUuid16;
	CHAR szServiceName[MAX_SERVICE_NAME_LENGTH];
} GENERAL_SERVICE_INFO,*PGENERAL_SERVICE_INFO;

#ifndef UUID_DEFINED
#define UUID_DEFINED
typedef struct _UUID
{
    unsigned long Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[8];
} UUID;
#endif /* UUID_DEFINED */

typedef struct _SPPEX_SERVICE_INFO {
	DWORD dwSize;
	DWORD dwSDAPRecordHanlde;
	UUID serviceClassUuid128;  
	CHAR szServiceName[MAX_SERVICE_NAME_LENGTH];
	UCHAR ucServiceChannel;
	UCHAR ucComIndex;
} SPPEX_SERVICE_INFO,*PSPPEX_SERVICE_INFO;


typedef SPP_CLIENT_PARAM SPP_CONNECT_INFO;
typedef PSPP_CLIENT_PARAM PSPP_CONNECT_INFO;

typedef struct _GENERAL_CONNECT_INFO{
	DWORD dwHandle;
	BOOL bIsOutgoing;
	WORD wServiceClass;
	BYTE remoteBdAddr[DEVICE_ADDRESS_LENGTH];
}GENERAL_CONNECT_INFO,*PGENERAL_CONNECT_INFO;

//Function prototypes
BTEXPORT BOOL BT_InitializeLibrary();

BTEXPORT void BT_UninitializeLibrary();

BTEXPORT BOOL BT_IsBlueSoleilStarted(/* [in] */ DWORD dwSeconds );

BTEXPORT BOOL BT_IsBluetoothReady (/* [in] */ DWORD dwSeconds );

BTEXPORT DWORD BT_StartBluetooth();

BTEXPORT DWORD BT_StopBluetooth(BOOL bSwitch2HidMode);

BTEXPORT DWORD BT_GetLocalDeviceInfo (
							 /* [in] */ DWORD dwMask,
							 /* [out] */ PBLUETOOTH_DEVICE_INFO_EX pDevInfo
							 );

BTEXPORT DWORD BT_SetLocalDeviceInfo (
							 /* [in] */ DWORD dwMask,
							 /* [in] */ PBLUETOOTH_DEVICE_INFO pDevInfo
							 );

BTEXPORT DWORD BT_GetRemoteDeviceInfo (
							  /* [in] */ DWORD dwMask,
							  /* [in] [out] */ PBLUETOOTH_DEVICE_INFO_EX pDevInfo
							  );

BTEXPORT DWORD BT_SetRemoteDeviceInfo (
							  /* [in] */ DWORD dwMask,
							  /* [in] [out] */ PBLUETOOTH_DEVICE_INFO_EX pDevInfo
							  );

BTEXPORT DWORD BT_PairDevice(
					/* [in] */ PBLUETOOTH_DEVICE_INFO pDevInfo,
					/* [in] */ WORD wPinCodeLen,
					/* [in] */ BYTE* lpPinCode,
					/* [in] */ BOOL bKeepOldkeyOnFail,
					/* [in] */ BOOL bShowPincode
					);

BTEXPORT DWORD BT_UnpairDevice(/* [in] */ BYTE* lpBdAddr);

BTEXPORT DWORD BT_ConnectService(
					   /* [in] */ PBLUETOOTH_DEVICE_INFO pDevInfo, 
					   /* [in] */ PGENERAL_SERVICE_INFO pServiceInfo,
					   /* [in, out] */ BYTE* lpParam,
					   /* [out] */ DWORD* lpConnectionHandle
					   );

BTEXPORT DWORD BT_DisconnectService (/* [in] */ DWORD dwConnectionHandle);


BTEXPORT DWORD BT_InquireDevices(
						/* [in] */ UCHAR ucInqMode,
						/* [in] */ UCHAR ucInqTimeLen,
						/* [in, out] */ DWORD* lpDevsListLength,
						/* [out] */ PBLUETOOTH_DEVICE_INFO pDevsList
						);

BTEXPORT DWORD BT_CancelInquiry();

BTEXPORT DWORD BT_BrowseServices(
						/* [in] */ PBLUETOOTH_DEVICE_INFO pDevInfo,
						/* [in] */ BOOL bBrowseAllServices,
						/* [in][out] */ DWORD* lpServiceClassListLength,
						/* [in][out] */ PGENERAL_SERVICE_INFO pSeriveClassList
						);

BTEXPORT DWORD BT_GetConnectInfo(
							/* [in] */DWORD dwHandle,
							/* [out] */BOOL* lpIsOutgoing, 
							/* [out] */WORD* lpServiceClasss,
							/* [out] */BYTE* lpRemoteBdAddr,
							/* [in,out] */DWORD* lpConnInfoLen,
							/* [out] */BYTE* lpConnInfo
							);

BTEXPORT DWORD BT_StartSPPExService (
							/* [in][out] */ PSPPEX_SERVICE_INFO pServiceInfo,
							/* [out] */ DWORD* lpServerHandle
							);

BTEXPORT DWORD BT_StopSPPExService(/* [in] */ DWORD dwServerHandle);

BTEXPORT DWORD BT_SearchSPPExServices (
							  /* [in] */ PBLUETOOTH_DEVICE_INFO pDevInfo,
							  /* [in][out] */ DWORD* lpServiceClassListLength,
							  /* [in][out] */ PSPPEX_SERVICE_INFO pSeriveClassList
							  );

BTEXPORT DWORD BT_ConnectSPPExService (
							  /* [in] */ PBLUETOOTH_DEVICE_INFO pDevInfo,
							  /* [in][out] */ PSPPEX_SERVICE_INFO pServiceInfo,
							  /* [out] */ DWORD* lpConnectionHandle
							  );

BTEXPORT DWORD BT_DisconnectSPPExService(
								/* [in] */ DWORD dwConnectionHandle
								);

//Callback function prototypes
/*
#define EVENT_CONNECTION_STATUS			0x03
*/
typedef void (*PCALLBACK_CONNECTION_STATUS) (WORD wServiceClass, BYTE* lpBdAddr, UCHAR ucStatus, DWORD dwConnetionHandle);
/*
#define EVENT_DUN_RAS_CALLBACK			0x04
*/
typedef void (*PCALLBACK_RAS_CONNECTION_STATUS)(UCHAR ucStrLen, CHAR* szErrorString, DWORD dwError);

/*
#define EVENT_ERROR						0x05
*/
typedef void (*PCALLBACK_ERROR) (DWORD dwErrorCode);

/*
#define EVENT_INQUIRY_DEVICE_REPORT		0x06
*/
typedef void (*PCALLBACK_INQUIRY_DEVICE) (/*in*/PBLUETOOTH_DEVICE_INFO pDevInfo);

/*
#define EVENT_SPPEX_CONNECTION_STATUS	0x07
*/
typedef void (*PCALLBACK_SPPEX_CONNECTION_STATUS) (DWORD dwServerHandle, BYTE* lpBdAddr, UCHAR ucStatus, DWORD dwConnetionHandle);

/*
#define EVENT_BLUETOOTH_STATUS			0x08
*/
typedef void (*PCALLBACK_BLUETOOTH_STATUS) (UCHAR ucStatus);


BTEXPORT DWORD BT_RegisterCallback(
						  /* [in] */ UCHAR ucEvent,
						  /* [in] */ LPVOID pfnCbkFun
						  );

BTEXPORT DWORD BT_UnregisterCallback (/* [in] */ UCHAR ucEvent);


BTEXPORT DWORD BT_GetVersion();


BTEXPORT DWORD BT_GetBtSpecVersion(/* [out] */ DWORD* lpBtSpecVersion);

BTEXPORT DWORD BT_EnumConnections(/* [in, out] */ DWORD *lpBufferLen,
								 /* [out] */ PGENERAL_CONNECT_INFO lpBuffer
								 );

#ifdef __cplusplus
}
#endif

#endif //__BT_UI_H__
