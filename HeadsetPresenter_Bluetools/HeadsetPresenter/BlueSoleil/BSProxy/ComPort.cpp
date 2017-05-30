/************************************************************************************
// Sample code for calling IVT BlueSoleil APIs

  Copyright (c) 2004 -2005 , IVT Corporation. All rights reserved.

*******************************************************************************************/
#include "StdAfx.h"
#include "ComPort.h"
#include "SoleilProxy.h"
#include <stdio.h>

#include "SS_Log.h"

#define MAX_IO_THREAD 4

typedef struct _COM_IO{
	DWORD dwConnHandle;
	int nComPort;
	HANDLE hThread;
	BOOL bStopFlag;
}COM_IO;

COM_IO g_comIoList[MAX_IO_THREAD]={0};

HANDLE OpenPort(UCHAR nPort)
{
	HANDLE handle;
	char  buf[64];
	DWORD nread = 64;
	sprintf(buf,"\\\\.\\COM%d",nPort);
	//sprintf(buf,"\\\\.\\COM207");
	// open the com port.
	handle = CreateFile(buf, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if (handle == INVALID_HANDLE_VALUE){
		//Log("Failed to open COM port %d, last error=%d\n",nPort,GetLastError());
		Log("Failed to open COM port %d",nPort);
		LPVOID lpMsgBuf;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL );
		//MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
		Log("GetLastError returned %s",lpMsgBuf);
		CString msg = "HeadsetPresenter failed to open COM";
		msg.Format("HeadsetPresenter failed to open COM%d.\nThis is a BlueSoleil problem and details can be found in Chapter 5.2 in HeadsetPresenter for Beginners available in the HeadsetPresenter folder",nPort);
		MessageBox(NULL,msg,"COM error",MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
		//char buf[15] = "AT+VGS=10\0";
		//BS_DataReceivedOnCOM(buf);  //just used for testing
		return INVALID_HANDLE_VALUE;
	}

	DCB dcb;
	COMMTIMEOUTS commTimeouts;
	
	GetCommState((HANDLE)handle,&dcb);
	dcb.BaudRate = CBR_115200;
	dcb.Parity = NOPARITY;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;
	SetCommState((HANDLE)handle,&dcb);

	commTimeouts.ReadIntervalTimeout = MAXDWORD;
	commTimeouts.ReadTotalTimeoutConstant = 0;
	commTimeouts.ReadTotalTimeoutMultiplier = 0;
	commTimeouts.WriteTotalTimeoutConstant = 100;
	commTimeouts.WriteTotalTimeoutMultiplier = 10;
	SetCommTimeouts((HANDLE)handle,&commTimeouts);
	PurgeComm((HANDLE)handle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR );
	SetupComm((HANDLE)handle, 1024, 1024);
	EscapeCommFunction((HANDLE)handle,SETRTS);
	EscapeCommFunction((HANDLE)handle,SETDTR);

	return handle;
}
BOOL CheckActive(HANDLE hPort)
{
	// Write AT command to Modem.
	OVERLAPPED writeo, reado;	
	DWORD dwErrors;
	DWORD byte_sent;
	BOOL   ret = TRUE;
	BOOL  bWaitingOnWrite = TRUE;
	COMSTAT comStat;  
	DWORD nwritten,nread;
	CHAR buf[256]={0};
	nwritten = 0;

	memset(&writeo,0,sizeof(OVERLAPPED));
	writeo.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	memset(&reado,0,sizeof(OVERLAPPED));
	reado.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if ( !WriteFile(hPort, "AT\r", 3, &byte_sent, &writeo)){
		dwErrors=GetLastError();
		if ( dwErrors == ERROR_IO_PENDING) { 
			while(!GetOverlappedResult((HANDLE)hPort, &writeo, &byte_sent, TRUE)){
				if ( GetLastError() == ERROR_IO_INCOMPLETE){
              		nwritten += byte_sent;
					continue;
				}
				else{
					ClearCommError((HANDLE)hPort, &dwErrors, &comStat); 
					break;
				}
           	}
			nwritten += byte_sent;			
		}
		else{ 
			ClearCommError((HANDLE)hPort, &dwErrors, &comStat);
		} 
	}
	else{
		nwritten += byte_sent;
	}

	if ( nwritten < 3 ){
		CloseHandle(hPort);
		ret=FALSE;
		goto check_exit;
	}

	Log("Sent data to com port:AT\\r\n");

	// Read Response from Modem.
	DWORD nreturned;
	Sleep(500);
	nread=9;
	if(!ReadFile((HANDLE)hPort, buf, nread, &nreturned, &reado) ){
		dwErrors=GetLastError();
		if ( dwErrors== ERROR_IO_PENDING) { 
			DWORD dwRes;
			dwRes = WaitForSingleObject(reado.hEvent, 2000);
			 switch(dwRes)
			 {
				 case WAIT_OBJECT_0: 
					 GetOverlappedResult((HANDLE)hPort, &reado, &nreturned, FALSE);
					 break;
				 case WAIT_TIMEOUT:
			 		break;
				 default:					 
					break;
			 }
		}else{ 
			ClearCommError( (HANDLE)hPort, &dwErrors, &comStat); 
		}
	}
	else{
		DWORD error = GetLastError();
	}

	if((nreturned) && 
		((!strncmp((LPCTSTR)buf,"\r\nOK\r\n",6))) ||
		((!strncmp((LPCTSTR)buf,"AT\r\r\nOK\r\n",9))))
	{
		ret = TRUE;
	}
	else
		ret = FALSE;

	PurgeComm(hPort,PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	
	
	Log("Received data: %s\n",buf);
	BS_DataReceivedOnCOM(buf);
	//Maybe we should write OK here

check_exit:
	if(writeo.hEvent){
		CloseHandle(writeo.hEvent);
	}	
	if(reado.hEvent){
		CloseHandle(reado.hEvent);
	}
	return ret;
}

int HandleComPortData(HANDLE hComPort)
{
	DWORD nreturned,nread;
	OVERLAPPED writeo, reado;	
	DWORD dwErrors;
	DWORD byte_sent;
	BOOL  bWaitingOnWrite = TRUE;
	COMSTAT comStat;  
	DWORD nwritten;
	UCHAR buf[256]={0};

	if(hComPort==INVALID_HANDLE_VALUE){
		return -1;
	}

	memset(&writeo,0,sizeof(OVERLAPPED));
	writeo.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	memset(&reado,0,sizeof(OVERLAPPED));
	reado.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	nread=3;
	if(!ReadFile(hComPort, buf, nread, &nreturned, &reado) ){
		
		if (GetLastError() == ERROR_IO_PENDING) { 
			DWORD dwRes;
			dwRes = WaitForSingleObject(reado.hEvent, 2000);
			switch(dwRes)
			{
			case WAIT_OBJECT_0: 
				GetOverlappedResult(hComPort, &reado, &nreturned, FALSE);
				break;
			default:
				goto HandleComPortData_Exit;
			}
		}
		else{ 
			ClearCommError( hComPort, &dwErrors, &comStat); 
		}
	}
	Log("SPPEX Server Received data: %s\n",buf);
	
	if(nreturned&&strncmp((LPCTSTR)buf,"AT\r",3)==0){
		nwritten = 0;
		if ( !WriteFile(hComPort,"\r\nOK\r\n", 6, &byte_sent, &writeo)){
			if ( GetLastError() == ERROR_IO_PENDING) { 
				while(!GetOverlappedResult(hComPort, &writeo, &byte_sent, TRUE)){
					if ( GetLastError() == ERROR_IO_INCOMPLETE){
						nwritten += byte_sent;
						continue;
					}else{
						ClearCommError(hComPort, &dwErrors, &comStat); 
						break;
					}
				}
				nwritten += byte_sent;			
			}
			else{ 
				ClearCommError(hComPort, &dwErrors, &comStat); 
			} 
		}
		if(nwritten == 6)
			Log("SPPEX Server write data: \\r\\nOK\\r\\n\n");
	}



HandleComPortData_Exit:
	
	CloseHandle(writeo.hEvent);
	CloseHandle(reado.hEvent);

	return 0;
}



DWORD WINAPI SPPClientIOThread(LPVOID param)
{
	int nIndex = (int)param;

	HANDLE hComPort = OpenPort(g_comIoList[nIndex].nComPort);
	if(hComPort == INVALID_HANDLE_VALUE){
		CloseHandle(g_comIoList[nIndex].hThread);
		memset(&g_comIoList[nIndex],0, sizeof(COM_IO));
		return 0;
	}

	while(g_comIoList[nIndex].bStopFlag != TRUE) {
		CheckActive(hComPort);
		Sleep(100);  //Keep alive timer could replace this value?
	}
	
	CloseHandle(hComPort);

	return 0;
}

DWORD WINAPI SPPServerIOThread(LPVOID param)
{

	int nIndex = (int)param;

	HANDLE hComPort = OpenPort(g_comIoList[nIndex].nComPort);
	if(hComPort == INVALID_HANDLE_VALUE){
		CloseHandle(g_comIoList[nIndex].hThread);
		memset(&g_comIoList[nIndex],0, sizeof(COM_IO));
		return 0;
	}

	while(g_comIoList[nIndex].bStopFlag != TRUE) {
		HandleComPortData(hComPort);
		Sleep(500);
	}
	
	CloseHandle(hComPort);

	return 0;
}


BOOL StartIoOnComport(DWORD dwConnHandle,int nComport, BOOL bClientRole)
{
	int i;
	DWORD dwID;
	BOOL bRet = FALSE;

	for(i = 0; i<MAX_IO_THREAD; i++){
		if(g_comIoList[i].hThread == NULL){
			g_comIoList[i].nComPort = nComport;
			g_comIoList[i].dwConnHandle = dwConnHandle;
			if(bClientRole)
				g_comIoList[i].hThread = CreateThread(NULL,0,SPPClientIOThread,(LPVOID)i,0,&dwID);		
			else
				g_comIoList[i].hThread = CreateThread(NULL,0,SPPServerIOThread,(LPVOID)i,0,&dwID);
			if(g_comIoList[i].hThread == NULL){
				memset(&g_comIoList[i], 0, sizeof(COM_IO));
				bRet = FALSE;
			}else
				bRet = TRUE;
		   break;
		}
	}

	return bRet;
}

BOOL StopIoOnComport(DWORD dwConnHandle)
{
	int i;
	BOOL bRet = FALSE;
	
	for(i = 0; i<MAX_IO_THREAD; i++){
		if((dwConnHandle == 0  || dwConnHandle == g_comIoList[i].dwConnHandle) && g_comIoList[i].hThread != NULL){
			g_comIoList[i].bStopFlag = TRUE;
			if(WaitForSingleObject(g_comIoList[i].hThread, 3000) != WAIT_OBJECT_0)
				TerminateThread(g_comIoList[i].hThread, -1);
			CloseHandle(g_comIoList[i].hThread);
			memset(&g_comIoList[i], 0, sizeof(COM_IO));	
			bRet = TRUE;
			if(dwConnHandle != 0)
				break;
		}
	}
	
	return bRet;
	
}
