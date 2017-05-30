// ----------------------------------------------------------------------- //
//
//  FILENAME:	main.cpp
//  AUTHOR:		Steve Schaneville
//  CREATED:	13 Nov 2001, 09:04
//
//  DESCRIPTION:
//
//	    Before trying to figure this stuff out, you should probably read the
//      Help file located in the Project Workspace directory, "SS_Log.htm".
//
//      This file really shows the possibilities of the SS_Log project.  We're
//      using the global log two local logs.  
//
//      A debug build should, when run, produce two log windows, one with 30 
//      entries, the other with 11.  Also we should get a file named 
//      "\LocalLog1.log" with 20 entries.
//
//      The release build will produce 1 log windows with 20 entries, and 2 
//      log files ("\LocalLog1.log" and "SS_Log.log") each with 20 entries.
//
//      This is, of course, if you leave all the filters turned on in the 
//      registry.  Try turning off some of the values for different results.
//      (HKEY_CURRENT_USER\Software\SS_Log) (0=off, 1=on)
//
//
//  Copyright (c) 2001
//
// ----------------------------------------------------------------------- //

// ------------------[       Pre-Include Defines       ]------------------ //
// ------------------[          Include Files          ]------------------ //
#include "SS_Utils.h"

// ------------------[      Macros/Constants/Types     ]------------------ //
// ------------------[         Global Variables        ]------------------ //
// ------------------[         Global Functions        ]------------------ //

#ifndef _MAC
	int	WINAPI
#else
	int	CALLBACK
#endif
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nShowCmd)
//void main()
{

}
