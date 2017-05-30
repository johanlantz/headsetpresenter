/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* Copyright (c) 1999-2005 IVT Corporation
*
* All rights reserved.
* 
---------------------------------------------------------------------------*/

/////////////////////////////////////////////////////////////////////////////
// Module Name:
//  bt_def.h
// Abstract:
//      This file contains Bluetooth common definitions
// 
// Author:
//	LZC
// Revision History:
//  04/07/2005 16:05:10 PM	LZC	Created
/////////////////////////////////////////////////////////////////////////////
#ifndef		__BT_DEF_H__
#define		__BT_DEF_H__

/*Mask for 3-bytes class of device*/
#define COD_FORMAT_MASK					(0x000003)
#define COD_MINOR_DEVICE_CLASS_MASK		(0x0000FC)
#define COD_MAJOR_DEVICE_CLASS_MASK		(0x001F00)
#define COD_SERVICE_CLASS_MASK			(0xFFE000)
#define COD_DEVICE_CLASS_MASK			(COD_MAJOR_DEVICE_CLASS_MASK | COD_MINOR_DEVICE_CLASS_MASK)

/*major service classes*/
#define COD_SRVCLS_LIMITED				(0x002000)
#define COD_SRVCLS_POSITION				(0x010000)
#define COD_SRVCLS_NETWORK				(0x020000)
#define COD_SRVCLS_RENDER				(0x040000)
#define COD_SRVCLS_CAPTURE				(0x080000)
#define COD_SRVCLS_OBJECT				(0x100000)
#define COD_SRVCLS_AUDIO				(0x200000)
#define COD_SRVCLS_TELEPHONE			(0x400000)
#define COD_SRVCLS_INFOR				(0x800000)

/*major device classes*/			                                    
#define COD_DEVCLS_MISC					(0x000000)
#define COD_DEVCLS_COMPUTER				(0x000100)
#define COD_DEVCLS_PHONE				(0x000200)
//#define COD_DEVCLS_PHONE				(0x001000)
#define COD_DEVCLS_LAP					(0x000300)
#define COD_DEVCLS_AUDIO				(0x000400)
#define COD_DEVCLS_PERIPHERAL			(0x000500)
#define COD_DEVCLS_IMAGE              	(0x000600)
#define COD_DEVCLS_UNCLASSIFIED			(0x001F00)

/*the minor device class field - computer major class */
#define COD_COMPCLS_UNCLASSIFIED     	(COD_DEVCLS_COMPUTER | 0x000000) 
#define COD_COMPCLS_DESKTOP          	(COD_DEVCLS_COMPUTER | 0x000004)
#define COD_COMPCLS_SERVER            	(COD_DEVCLS_COMPUTER | 0x000008)
#define COD_COMPCLS_LAPTOP            	(COD_DEVCLS_COMPUTER | 0x00000C)
#define COD_COMPCLS_HANDHELD        	(COD_DEVCLS_COMPUTER | 0x000010)
#define COD_COMPCLS_PALMSIZED        	(COD_DEVCLS_COMPUTER | 0x000014)
#define COD_COMPCLS_WEARABLE        	(COD_DEVCLS_COMPUTER | 0x000018)

/*the minor device class field - phone major class*/
#define COD_PHONECLS_UNCLASSIFIED   	(COD_DEVCLS_PHONE | 0x000000) 
#define COD_PHONECLS_CELLULAR         	(COD_DEVCLS_PHONE | 0x000004)
#define COD_PHONECLS_CORDLESS        	(COD_DEVCLS_PHONE | 0x000008)
#define COD_PHONECLS_SMARTPHONE     	(COD_DEVCLS_PHONE | 0x00000C)
#define COD_PHONECLS_WIREDMODEM    		(COD_DEVCLS_PHONE | 0x000010)
#define COD_PHONECLS_COMMONISDNACCESS	(COD_DEVCLS_PHONE | 0x000014)
#define COD_PHONECLS_SIMCARDREADER		(COD_DEVCLS_PHONE | 0x000018)

/*the minor device class field - LAN/Network access point major class*/
#define COD_LAP_FULL                  	(COD_DEVCLS_LAP | 0x000000)
#define COD_LAP_17                     	(COD_DEVCLS_LAP | 0x000002)
#define COD_LAP_33                     	(COD_DEVCLS_LAP | 0x000004)
#define COD_LAP_50                     	(COD_DEVCLS_LAP | 0x000006)
#define COD_LAP_67                     	(COD_DEVCLS_LAP | 0x000008)
#define COD_LAP_83                     	(COD_DEVCLS_LAP | 0x00000A)
#define COD_LAP_99                     	(COD_DEVCLS_LAP | 0x00000C)
#define COD_LAP_NOSRV                 	(COD_DEVCLS_LAP | 0x00000D)

/*the minor device class field - Audio/Video major class*/
#define COD_AV_UNCLASSIFIED           	(COD_DEVCLS_AUDIO | 0x000000)
#define COD_AV_HEADSET                	(COD_DEVCLS_AUDIO | 0x000004)
#define COD_AV_HANDSFREE             	(COD_DEVCLS_AUDIO | 0x000008)
#define COD_AV_HEADANDHAND           	(COD_DEVCLS_AUDIO | 0x00000C)
#define COD_AV_MICROPHONE            	(COD_DEVCLS_AUDIO | 0x000010) 
#define COD_AV_LOUDSPEAKER           	(COD_DEVCLS_AUDIO | 0x000014)
#define COD_AV_HEADPHONES            	(COD_DEVCLS_AUDIO | 0x000018)
#define COD_AV_PORTABLEAUDIO         	(COD_DEVCLS_AUDIO | 0x00001C)
#define COD_AV_CARAUDIO               	(COD_DEVCLS_AUDIO | 0x000020)
#define COD_AV_SETTOPBOX              	(COD_DEVCLS_AUDIO | 0x000024)
#define COD_AV_HIFIAUDIO               	(COD_DEVCLS_AUDIO | 0x000028)
#define COD_AV_VCR                     	(COD_DEVCLS_AUDIO | 0x00002C)
#define COD_AV_VIDEOCAMERA           	(COD_DEVCLS_AUDIO | 0x000030)
#define COD_AV_CAMCORDER             	(COD_DEVCLS_AUDIO | 0x000034)
#define COD_AV_VIDEOMONITOR          	(COD_DEVCLS_AUDIO | 0x000038)
#define COD_AV_VIDEODISPANDLOUDSPK   	(COD_DEVCLS_AUDIO | 0x00003C) 
#define COD_AV_VIDEOCONFERENCE       	(COD_DEVCLS_AUDIO | 0x000040)
#define COD_AV_GAMEORTOY             	(COD_DEVCLS_AUDIO | 0x000048)

/*the minor device class field - peripheral major class*/
#define COD_PERIPHERAL_KEYBOARD      	(COD_DEVCLS_PERIPHERAL | 0x000040)    
#define COD_PERIPHERAL_POINT           	(COD_DEVCLS_PERIPHERAL | 0x000080)
#define COD_PERIPHERAL_KEYORPOINT    	(COD_DEVCLS_PERIPHERAL | 0x0000C0)
#define COD_PERIPHERAL_UNCLASSIFIED  	(COD_DEVCLS_PERIPHERAL | 0x000000) 
#define COD_PERIPHERAL_JOYSTICK       	(COD_DEVCLS_PERIPHERAL | 0x000004)
#define COD_PERIPHERAL_GAMEPAD       	(COD_DEVCLS_PERIPHERAL | 0x000008)
#define COD_PERIPHERAL_REMCONTROL    	(COD_DEVCLS_PERIPHERAL | 0x00000C)
#define COD_PERIPHERAL_SENSE          	(COD_DEVCLS_PERIPHERAL | 0x000010)

/*the minor device class field - imaging major class*/
#define COD_IMAGE_DISPLAY             	(COD_DEVCLS_IMAGE | 0x000010)
#define COD_IMAGE_CAMERA             	(COD_DEVCLS_IMAGE | 0x000020)
#define COD_IMAGE_SCANNER            	(COD_DEVCLS_IMAGE | 0x000040)
#define COD_IMAGE_PRINTER              	(COD_DEVCLS_IMAGE | 0x000080)

#endif //__BT_DEF_H__