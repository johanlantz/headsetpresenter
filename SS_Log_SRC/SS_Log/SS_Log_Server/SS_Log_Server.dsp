# Microsoft Developer Studio Project File - Name="SS_Log_Server" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SS_Log_Server - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SS_Log_Server.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SS_Log_Server.mak" CFG="SS_Log_Server - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SS_Log_Server - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SS_Log_Server - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SS_Log_Server - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /O2 /I "..\.." /I "include" /I "..\SS_Log" /D "NDEBUG" /D "_SS_LOG_ACTIVE" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "Include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Version.lib Utils.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib OLEAUT32.dll" /libpath:"..\..\release"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy release\SS_Log_Server.exe %SYSTEMROOT%
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SS_Log_Server - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\SS_Log" /I "..\.." /I "include" /D "_DEBUG" /D "_AFXDLL" /D "_SS_LOG_ACTIVE" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "Include" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Version.lib UtilsD.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBCMTD.lib" /pdbtype:sept /libpath:"..\..\debug"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy debug\SS_Log_Server.exe %SYSTEMROOT%
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "SS_Log_Server - Win32 Release"
# Name "SS_Log_Server - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CPage.cpp
# End Source File
# Begin Source File

SOURCE=.\CPrinter.cpp
# End Source File
# Begin Source File

SOURCE=.\Dib.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\SS_FilterSettingsDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\SS_Log\SS_Log_Defines.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\SS_Log_Server.cpp
# End Source File
# Begin Source File

SOURCE=.\SS_Log_Server.rc
# End Source File
# Begin Source File

SOURCE=.\SS_Log_ServerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\SS_Log_ServerView.cpp
# End Source File
# Begin Source File

SOURCE=.\SSCommandLineInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\SSFilterSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLogConnection.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLogDestination.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLogDestinationFile.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLogDestinationNTEventLog.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLogDestinationWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\SSLogOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SS_FilterSettingsDlg.h
# End Source File
# Begin Source File

SOURCE=..\SS_Log\SS_Log_Defines.h
# End Source File
# Begin Source File

SOURCE=.\SS_Log_Server.h
# End Source File
# Begin Source File

SOURCE=.\SS_Log_ServerDoc.h
# End Source File
# Begin Source File

SOURCE=.\SS_Log_ServerView.h
# End Source File
# Begin Source File

SOURCE=.\SSCommandLineInfo.h
# End Source File
# Begin Source File

SOURCE=.\SSFilterSettings.h
# End Source File
# Begin Source File

SOURCE=.\SSLogConnection.h
# End Source File
# Begin Source File

SOURCE=.\SSLogDestination.h
# End Source File
# Begin Source File

SOURCE=.\SSLogDestinationFile.h
# End Source File
# Begin Source File

SOURCE=.\SSLogDestinationNTEventLog.h
# End Source File
# Begin Source File

SOURCE=.\SSLogDestinationWindow.h
# End Source File
# Begin Source File

SOURCE=.\SSLogOutput.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=res\EventLogMsgs.mc

!IF  "$(CFG)" == "SS_Log_Server - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=res\EventLogMsgs.mc
InputName=EventLogMsgs

BuildCmds= \
	mc -cso -h Include -r Include res\$(InputName)

"$(ProjDir)\Include\$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(ProjDir)\Include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(ProjDir)\Include\MSG00001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "SS_Log_Server - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=res\EventLogMsgs.mc
InputName=EventLogMsgs

BuildCmds= \
	mc -cso -h Include -r Include res\$(InputName)

"$(ProjDir)\Include\$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(ProjDir)\Include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(ProjDir)\Include\MSG00001.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\res\SS_Log_Server.ico
# End Source File
# Begin Source File

SOURCE=.\res\SS_Log_Server.rc2
# End Source File
# Begin Source File

SOURCE=.\res\SS_Log_ServerDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\SS_Log_Server.reg
# End Source File
# End Target
# End Project
