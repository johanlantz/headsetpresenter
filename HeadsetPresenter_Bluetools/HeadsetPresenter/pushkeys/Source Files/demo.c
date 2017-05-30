/*
    a simple demonstration program for PUSHKEYS 1.0 for C

    (C) by Alexander Frink (Alexander.Frink@Uni-Mainz.DE) January 1998.

    Usage: pushkeys <title> <keys>

    Example (open a Command Prompt window first!): 
           pushkeys "Command Prompt" "edit~{sleep 5}{esc}%()fx~"

           starts the DOS editor, waits 5 seconds and closes it again
           (this will only work with an English Windows version)
             
*/

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "pushkeys.h"

UCHAR szTitle[MAXLEN];
UCHAR szKeys[MAXLEN];
BOOL bWildcard=FALSE;
DWORD dwSent=0;

void PushToWindow(HWND hWnd, LPTSTR szKeys)
{
    HWND hWndActive=GetForegroundWindow();

    SetForegroundWindow(hWnd);
    PushKeys(szKeys);
    SetForegroundWindow(hWndActive);
}

BOOL CALLBACK ListWindows(HWND hWnd, LPARAM lparam)
{
    UCHAR szCurrentTitle[MAXLEN];
    GetWindowText(hWnd,szCurrentTitle,MAXLEN);
	if (bWildcard) {
	    if (strncmp(szCurrentTitle,szTitle,strlen(szTitle))==0) {
			PushToWindow(hWnd,szKeys);
			dwSent++;
		}
	} else {
		if (strcmp(szCurrentTitle,szTitle)==0) {
			PushToWindow(hWnd,szKeys);
			dwSent++;
		}
    }
    return TRUE;
}

int main(int argc, char *argv[])
{
    if (argc!=3) {
		printf("Usage: %s title keys\n",argv[0]);
		printf("       Pushes 'keys' to the window with the title 'title' (in the title bar).\n");
		printf("       'title' may end with a '*' as a wildcard.\n");
		printf("       'title' is case-sensitive.\n");
		printf("       Enclose 'title' and 'keys' with quotes (\"\") if they contain blanks.\n\n");
        printf("       Example (open a Command Prompt window first!):\n"); 
        printf("       %s \"Command Prompt\" \"edit~{sleep 5}{esc}%%()fx~\"\n",argv[0]);
        printf("            starts the DOS editor, waits 5 seconds and closes it again\n");
        printf("            (THIS example will only work with an English Windows version)\n\n");
		printf("This is free software under the GNU General Public License.\n");
		printf("Copyright (C) 1997 by Alexander Frink\n");
		return 1;
	}

    strcpy(szTitle,argv[1]);
 	if (szTitle[strlen(szTitle)-1]=='*') {
		bWildcard=TRUE;
		szTitle[strlen(szTitle)-1]='\0';
	}
	strcpy(szKeys,argv[2]);
	
	EnumWindows(ListWindows,0);

	if (dwSent==0) {
		printf("No matching windows found!\n");
		return 1;
	}
	printf("OK, successfully pushed message to %i window(s) with title '%s'.\n",dwSent,argv[1]);
    return 0;
}
