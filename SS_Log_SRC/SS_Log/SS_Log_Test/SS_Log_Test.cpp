// SS_Log_Test.cpp : Defines the entry point for the application.
// Version 3.02

#include "stdafx.h"
#include "SS_Log.h"


HANDLE g_hEvent = NULL;
DWORD WINAPI ThreadProc(LPVOID lpParameter);


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
//    LogSetDestinationName(_T("test.log"));
//    DWORD dw = LogGetRegistryFilters();
//    LogSetRegistryKey("SteveCo\\SteveApp");
//    dw = LogGetRegistryFilters();
//	Log(_T("Look how quick and easy it is to get started!!!"));
//
//    SS_Log sLog(_T("whole constructor server"), _T("ss_log_wcs"),
//        _T("ss_log_wcs.log"), 25);
//    for( INT k=0; k<500; k++ )
//    {
//        Log(&sLog, _T("test %d"), k);
//    }
//    Log(_T("test Steveaux"));
//
//    return 0;

	// TUTORIAL!!!
	//
	// read *ALL* the comments as you look through this file.  They
	// explain what is going on and how the SS_Log class is used.  If you
	// take the time to read everything, you should have a fairly good
	// understanding of the functionality of this system.


	// this first line will launch a server, connect to it and send this
	// message.  The message will be displayed after 5 seconds (the
	// server updates its window every 5 seconds).
	Log(_T("Look how quick and easy it is to get started!!!"));
	// make sure that we sent the message successfully
	ASSERT(LogSucceeded());


    // this probably should have been done as the first line in this function,
    // but we will do it here... we will call the "Entering Function" command
    // that will produce an "entering" and "exiting" log.
    LogEnterFunction(_T("WinMain"));


	// ok something a little more fun.  Lets start off with an sprintf-style
	// message
	Log(_T("We will place a number here <%d> and a string here <%s>."), 123, _T("my string"));
	ASSERT(LogSucceeded());


	// Now a message from a resource ID.  Don't forget to set your instance
	// handle first!!
	LogInstanceHandle(hInstance);
	LogID(IDS_STRING1, _T("a string"), 123);
	ASSERT(LogSucceeded());


	// All the messages above have had the following default flags:
	// LOGTYPE_DEBUG | LOGTYPE_TRACE | LOGTYPE_LOGTOFILE | LOGTYPE_LOGTOWINDOW
	// Lets change the default flags.  Note that we only affect the flags
	// that are in the same group as those that we pass into the SetFlags
	// function.  So here, for example, since we are only passing in a
	// LOGTYPE_CRITICAL flag, and since that flag is part of the
	// LOGTYPEGROUP_LEVELS group, we only change flags in that group.  This
	// means than the LOGTYPE_TRACE flag will get removed. (see the
	// SS_Log_Defines.h file for info on flags and flag groups).
	LogSetFlags(LOGTYPE_CRITICAL);
	Log(_T("Another log, now with the critical flag set"));
	ASSERT(LogSucceeded());


	// now we'll ADD a flag.  Note that by adding the LOGTYPE_TRACE flag back
	// in, both the LOGTYPE_TRACE and the LOGTYPE_CRITICAL are set by default.
	// Were we to call the LogSetFlags function instead of the LogAddFlags
	// function below, the LOGTYPE_CRITICAL would have been removed again.
	LogAddFlags(LOGTYPE_TRACE);
	Log(_T("Yet another log, with these flags set: %d"), LogGetFlags());
	ASSERT(LogSucceeded());


	// all of those previous messages went to the *global log*, a default
	// log that gets defined simply by including the SS_Log.h file.  Now
	// we will create a local log that behaves just like the global log.
	// This log's information will go to a different deatination server.
	SS_Log localLog1(_T("Local Log 1"));
	localLog1.AddFlags(LOGTYPE_CRITICAL);
	Log(&localLog1, _T("Our first message to the local log"));
	ASSERT(localLog1.Succeeded());


	// from this point on we will do the exact same functions with both the
	// local log and the global log.
	//
	// let's send a message with an override in it.  Both our local and global
	// logs currently have the critical and trace flags set.  Lets override
	// those flags with the warning flag.
	Log(LOGTYPE_WARNING, _T("here we are overriding the flags in the global log"));
	Log(&localLog1, LOGTYPE_WARNING, _T("here we are overriding the flags in the local log"));


	// note that the warning flags above only overrided the levels message
	// flags for that single message.  The log below will revert back
	// to the defaults of trace and critical.
	Log(_T("Some more logging"));
	Log(&localLog1, _T("Some more logging"));


	// Let's change the destination.  We will send these two messages
	// to the NT event log instead of the file and window that they have
	// been going to.  By calling SetFlags, we will remove the LOGTYPE_LOGTOFILE
	// and LOGTYPE_LOGTOWINDOW flags, replacing them with the
	// LOGTYPE_LOGTOEVENTLOG flag.
	//
	// Note that if you are on a Win9x machine, there is no
	// nt event log, but windows it can be configured to send these messages to
	// to a file (not sure how to do the configuring).
	LogSetFlags(LOGTYPE_LOGTOEVENTLOG);
	localLog1.SetFlags(LOGTYPE_LOGTOEVENTLOG);

	Log(_T("the global log talking to the NT event log"));
	Log(&localLog1, _T("the local log talking to the NT event log"));


	// let's not send any more to the event log, so use RemoveFlags
	LogRemoveFlags(LOGTYPE_LOGTOEVENTLOG);
	localLog1.RemoveFlags(LOGTYPE_LOGTOEVENTLOG);


	// now we are in an interesting state... we previously called SetFlags
	// passing in LOGTYPE_LOGTOEVENTLOG, then we just called RemoveFlags
	// passing in LOGTYPE_LOGTOEVENTLOG.  The SetFlags function removed
	// all destination types and replaced them with LOGTYPE_LOGTOEVENTLOG,
	// then the RemoveFlags removed that destination type.  There are now
	// NO deatination type flags in the log default filters.  This means
	// that the following 2 lines will not get logged to anywhere!
	Log(_T("a log that will not show up anywhere!"));
	Log(&localLog1, _T("a log that will not show up anywhere!"));


	// ok, set the destination type group flags to contain the window and file
	// destinations, and set the level type group to contain the "normal"
	// flag (removing the critical and trace flags).
	//
	// note that the message IDs for the messages below will look as if
	// we skipped some numbers in the log window.  Well, we didn't skip
	// these numbers at all.  The first of the two IDs that seem skipped was
	// sent to the nt event log, and the second had no destination.
	// None-the-less, we track the fact that these messages existed by
	// assigning the proper IDs.
	//
	LogSetFlags(LOGTYPE_LOGTOFILE|LOGTYPE_LOGTOWINDOW|LOGTYPE_NORMAL);
	localLog1.SetFlags(LOGTYPE_LOGTOFILE|LOGTYPE_LOGTOWINDOW|LOGTYPE_NORMAL);

	Log(_T("our first \"normal\" message!"));
	Log(&localLog1, _T("our first \"normal\" message!"));


	// ok, now for a little fun... we'll create a new local log, and send
	// 10000 messages to it!!!  We will log only to a window (the window is
	// a bit slow to display all these messages, but it is important to
	// note that the slowness does NOT affect the sending speed of the
	// messages, which remains fast, preventing performance degredation
	// of your app.  You can tell this because this test app will complete
	// and terminate long before the window is finished displaying the
	// messages).  The file will be named "Local Log #2.log", and can
	// be found in the same directory that you launched this test app in.
	SS_Log localLog2(_T("Any name here"));
	localLog2.SetDestinationName(_T("Local Log #2.log"));
	for( INT i=0; i<10000; i++ )
	{
	    Log(&localLog2, _T("Message #%d of 10000"), i+1);
	}


	// ok, yet further!  First we will spawn another thread.  We will pass
	// the localLog1 pointer to the thread so we can send messages to the same
	// log from 2 threads simultaneously.  Let's see what happens...
	// (we'll use an event to make sure we start and end the threads at the
	// same time
	g_hEvent = ::CreateEvent(NULL, FALSE, FALSE, _T("MyEvent"));
	ASSERT(g_hEvent != NULL);
	HANDLE hThread = ::CreateThread(NULL, 0, ThreadProc, &localLog1, 0, 0);
	ASSERT(hThread != NULL);
	WaitForSingleObject(g_hEvent, 1000);
	for( i=0; i<1000; i++ )
	{
		Log(&localLog1, _T("Simultaneous logging from localLog1: %d"), i);
	}
	WaitForSingleObject(g_hEvent, 5000);


	// some other functions are below.  See the documentation at
    // "http://sslog.webhop.net" for details.
	//
//	LogLimitOutputSize(50);
//	LogEraseLog();
//	LogSetRegistryKey("My Company\\My App");
//
//	localLog.LimitOutputSize(50);
//	localLog.EraseLog();
//	localLog.SetRegistryKey("My Company\\My App");


	// one more function... the UseServer command, when set to FALSE, will
	// cause SS_Log not to launch a server, but instead, send everything
	// straight to a log file (set by SetDestinationName).  This option
	// provides no filtering, and can run VERY MUCH SLOWER than when
	// the server is being used
	//
//	LogUseServer(FALSE);		// this will cause eveything to be SLOW!
//	localLog.UseServer(FALSE);	// this will cause eveything to be SLOW!


	// more notes:
    // IMPORTANT: This next line won't compile because of a ambiguous function
    // call conflict... the problem is, the compiler can't tell which of
    // the following two functions to call:
    //
    //      VOID WriteLogID(SS_Log* pLog, UINT nResourceID, ...)
    //      VOID WriteLogID(SS_Log* pLog, DWORD dwFilter, UINT nResourceID, ...)
    //
    // I've not figured out how to fix this, so any suggestions are welcome.
    //
    //LogID(&logNTEvent, IDS_STRING2, 56, _T("something") ); // won't compile!!

	return 0;
}




DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	SS_Log* pLog = (SS_Log*)lpParameter;
    LocalLogEnterFunction(pLog, _T("ThreadProc"));

	SetEvent(g_hEvent);
	for( INT i=0; i<1000; i++ )
    {
	    Log(pLog, _T("Simultaneous logging from localLog3: %d"), i);
	}
	SetEvent(g_hEvent);

	return 0;

}
