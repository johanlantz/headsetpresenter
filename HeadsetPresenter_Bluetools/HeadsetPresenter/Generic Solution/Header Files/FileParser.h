//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2005
//////////////////////////////////////////////////////////////////////
// FileParser.h: interface for the FileParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEPARSER_H__C5B484F7_5495_4D2C_885D_5AA48C24B7A8__INCLUDED_)
#define AFX_FILEPARSER_H__C5B484F7_5495_4D2C_885D_5AA48C24B7A8__INCLUDED_



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class GenericController;

class FileParser  
{
public:
	FileParser(GenericController* imyController);
	virtual ~FileParser();

	HANDLE hThread;
	DWORD dwThreadId;

	void StartParseThread();
	void StopParseThread();
	void RunParseThread();
	bool IsThreadRunning();
	void SetReadDelay(CString newDelay);
	void SetAccessFileDelay(CString newDelay);
	void SetReplugMode(int iReplugMode);

private:
	static bool THREAD_STOP;
	static int readDelay;
	static int fileAccessDelay;
	static bool manualUnplug;

	GenericController* myController;  //just to get access to getHelp from the other classes
	HANDLE h_WaitForThreadExit;


};

#endif // !defined(AFX_FILEPARSER_H__C5B484F7_5495_4D2C_885D_5AA48C24B7A8__INCLUDED_)
