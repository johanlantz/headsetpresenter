; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ss_log_server.h"
LastPage=0

ClassCount=6
Class1=CMainFrame
Class2=SS_FilterSettingsDlg
Class3=SS_Log_ServerApp
Class4=CAboutDlg
Class5=SS_Log_ServerDoc
Class6=SS_Log_ServerView

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_FILTERSETTINGS

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
LastObject=CMainFrame
Filter=T
VirtualFilter=fWC

[CLS:SS_FilterSettingsDlg]
Type=0
BaseClass=CDialog
HeaderFile=SS_FilterSettingsDlg.h
ImplementationFile=SS_FilterSettingsDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=ID_APP_ABOUT

[CLS:SS_Log_ServerApp]
Type=0
BaseClass=CWinApp
HeaderFile=SS_Log_Server.h
ImplementationFile=SS_Log_Server.cpp
LastObject=ID_APP_ABOUT
Filter=N
VirtualFilter=AC

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=SS_Log_Server.cpp
ImplementationFile=SS_Log_Server.cpp

[CLS:SS_Log_ServerDoc]
Type=0
BaseClass=CDocument
HeaderFile=SS_Log_ServerDoc.h
ImplementationFile=SS_Log_ServerDoc.cpp
LastObject=SS_Log_ServerDoc

[CLS:SS_Log_ServerView]
Type=0
BaseClass=CListView
HeaderFile=SS_Log_ServerView.h
ImplementationFile=SS_Log_ServerView.cpp
LastObject=SS_Log_ServerView
Filter=C
VirtualFilter=VWC

[DLG:IDD_FILTERSETTINGS]
Type=1
Class=SS_FilterSettingsDlg
ControlCount=16
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_TEXTINMSG_CHECK,button,1342242819
Control4=IDC_TEXTINMSG_EDIT,edit,1350631552
Control5=IDC_FILENAME_CHECK,button,1342242819
Control6=IDC_FILENAME_EDIT,edit,1350631552
Control7=IDC_ID_CHECK,button,1342242819
Control8=IDC_ID_EDIT1,edit,1350631552
Control9=IDC_ID_EDIT2,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_APPLY,button,1342242816
Control12=IDC_REQ_ALL_FILTERS_RADIO,button,1342312457
Control13=IDC_REQ_ONE_FILTER_RADIO,button,1342181385
Control14=IDC_FILTERS_GROUP,button,1342177287
Control15=IDC_SEARCH_FILTERS_GROUP,button,1342177287
Control16=IDC_AUTO_APPLY_CHECK,button,1342242819

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=3
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDOK,button,1342373889

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_FILE_SAVE
Command3=ID_FILE_PRINT
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_EDIT_FILTERSETTINGS
Command8=ID_EDIT_LISTVIEW_FILTER
Command9=ID_APP_ABOUT
CommandCount=9

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_FILE_SAVE
Command3=ID_FILE_SAVE_AS
Command4=ID_FILE_PRINT
Command5=ID_FILE_PRINT_PREVIEW
Command6=ID_FILE_PRINT_SETUP
Command7=ID_FILE_MRU_FILE1
Command8=ID_APP_EXIT
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_EDIT_FILTERSETTINGS
Command14=ID_EDIT_LISTVIEW_FILTER
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_VIEW_REFRESH
Command18=ID_APP_ABOUT
CommandCount=18

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDR_MAINFRAME]
Type=1
Class=?
ControlCount=1
Control1=IDC_STATUS,static,1342308352

