
// Franson C++ BlueTools
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xObject.h"

namespace xBlueTools
{
	class xServiceType
	{
	public:
		static GUID SDP();
		static GUID UDP();
		static GUID RFCOMM();
		static GUID TCP();
		static GUID TCS_BIN();
		static GUID TCS_AT();
		static GUID OBEX();
		static GUID IP();
		static GUID FTP();
		static GUID HTTP();
		static GUID WSP();
		static GUID BNEP();
		static GUID UPNP();
		static GUID HIDP();
		static GUID HardcopyControlChannel();
		static GUID HardcopyDataChannel();
		static GUID HardcopyNotification();
		static GUID AVCTP();
		static GUID AVDTP();
		static GUID CMTP();
		static GUID UDI_C_PLANE();
		static GUID L2CAP();
		static GUID ServiceDiscoveryServer();
		static GUID BrowseGroupDescriptor();
		static GUID PublicBrowseGroup();
		static GUID SerialPort();
		static GUID LANAccessUsingPPP();
		static GUID DialupNetworking();
		static GUID IrMCSync();
		static GUID OBEXObjectPush();
		static GUID OBEXFileTransfer();
		static GUID IrMCSyncCommand();
		static GUID Headset();
		static GUID CordlessTelephony();
		static GUID AudioSource();
		static GUID AudioSink();
		static GUID AVRemoteControlTarget();
		static GUID AdvancedAudioDistribution();
		static GUID AVRemoteControl();
		static GUID VideoConferencing();
		static GUID Intercom();
		static GUID Fax();
		static GUID HeadsetAudioGateway();
		static GUID WAP();
		static GUID WAPClient();
		static GUID PANU();
		static GUID NAP();
		static GUID GN();
		static GUID DirectPrinting();
		static GUID ReferencePrinting();
		static GUID Imaging();
		static GUID ImagingResponder();
		static GUID ImagingReferencedObjects();
		static GUID Handsfree();
		static GUID HandsfreeAudioGateway();
		static GUID DirectPrintingReferenceObjectsService();
		static GUID ReflectedUI();
		static GUID BasicPrinting();
		static GUID PrintingStatus();
		static GUID HumanInterfaceDeviceService();
		static GUID HardcopyCableReplacement();
		static GUID HCR_Print();
		static GUID HCR_Scan();
		static GUID Common_ISDN_Access();
		static GUID VideoConferencingGW();
		static GUID UDI_MT();
		static GUID UDI_TA();
		static GUID Audio_Video();
		static GUID SIM_Access();
		static GUID Phonebook_Access_PCE();
		static GUID Phonebook_Access_PSE();
		static GUID PnPInformation();
		static GUID GenericNetworking();
		static GUID GenericFileTransfer();
		static GUID GenericAudio();
		static GUID GenericTelephony();
		static GUID UPNP_Service();
		static GUID UPNP_IP_Service();
		static GUID ESDP_UPNP_IP_PAN();
		static GUID ESDP_UPNP_IP_LAP();
		static GUID ESDP_UPNP_L2CAP();
		static GUID VideoSource();
		static GUID VideoSink();
		static GUID VideoDistribution();


	};
}
