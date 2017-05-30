# Microsoft Developer Studio Project File - Name="UsbSnoop" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=UsbSnoop - Win32 Release win98
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "UsbSnoop.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "UsbSnoop.mak" CFG="UsbSnoop - Win32 Release win98"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UsbSnoop - Win32 Debug win2k" (based on "Win32 (x86) Application")
!MESSAGE "UsbSnoop - Win32 Debug win98" (based on "Win32 (x86) Application")
!MESSAGE "UsbSnoop - Win32 Release win2k" (based on "Win32 (x86) Application")
!MESSAGE "UsbSnoop - Win32 Release win98" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "UsbSnoop - Win32 Debug win2k"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "UsbSnoop___Win32_Debug_win2k"
# PROP BASE Intermediate_Dir "UsbSnoop___Win32_Debug_win2k"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug2k"
# PROP Intermediate_Dir "debug2k"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Gz /W3 /Z7 /Oi /Gy /I "c:\98ddk\inc\win98" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D _X86_=1 /D i386=1 /D "STD_CALL" /D CONDITION_HANDLING=1 /D NT_UP=1 /D NT_INST=0 /D WIN32=100 /D _NT1X_=100 /D WINNT=1 /D _WIN32_WINNT=0x0500 /D _WIN32_IE=0x0400 /D WIN32_LEAN_AND_MEAN=1 /D DBG=1 /D DEVL=1 /D FPO=0 /D "NDEBUG" /D _DLL=1 /D "DRIVER" /D "_IDWBUILD" /D "RDRDBG" /D "SRVDBG" /FR /Yu"stddcls.h" /FD /GZ /Zel -cbstring /QIfdiv- /QI0f /GF /QIf /c
# ADD CPP /nologo /Gz /W3 /GX /Zi /Od /I "$(NTDDK)\inc" /D "WIN32" /D "_DEBUG" /D _X86_=1 /D i386=1 /D "STD_CALL" /D CONDITION_HANDLING=1 /D NT_UP=1 /D NT_INST=0 /D WIN32=100 /D _NT1X_=100 /D WINNT=1 /D _WIN32_WINNT=0x0500 /D _WIN32_IE=0x0501 /D WIN32_LEAN_AND_MEAN=1 /D DBG=1 /D DEVL=1 /D FPO=0 /D "NDEBUG" /D _DLL=1 /D "DRIVER" /D "_IDWBUILD" /D "RDRDBG" /D "SRVDBG" /D "WIN2K" /D "NDIS50" /FR /FD /Zel -cbstring /QIfdiv- /QI0f /GF /QIf /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wdm.lib /nologo /base:"0x10000" /version:4.0 /entry:"DriverEntry@8" /subsystem:windows /pdb:none /debug /debugtype:both /machine:I386 /nodefaultlib /out:"debug\UsbSnoop.sys" /libpath:"c:\98ddk\lib\i386\checked" -MERGE:_PAGE=PAGE -MERGE:_TEXT=.text -MERGE:.rdata=.text -SECTION:INIT,d -OPT:REF -FORCE:MULTIPLE -RELEASE -FULLBUILD -IGNORE:4001,4037,4039,4065,4070,4078,4087,4089,4096 -osversion:4.00 -optidata -driver -align:0x20 -subsystem:native,4.00 -debug:notmapped,FULL
# ADD LINK32 wdm.lib /nologo /base:"0x10000" /version:1.5 /entry:"DriverEntry@8" /subsystem:windows /pdb:none /debug /debugtype:both /machine:I386 /nodefaultlib /out:"debug2k\usbsnoop.sys" /libpath:"$(NTDDK)\libchk\i386" -MERGE:_PAGE=PAGE -MERGE:_TEXT=.text -MERGE:.rdata=.text -SECTION:INIT,d -OPT:REF -FORCE:MULTIPLE -RELEASE -FULLBUILD -IGNORE:4001,4037,4039,4065,4070,4078,4087,4089,4096 -osversion:4.00 -optidata -driver -align:0x20 -subsystem:native,4.00 -debug:notmapped,FULL

!ELSEIF  "$(CFG)" == "UsbSnoop - Win32 Debug win98"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "UsbSnoop___Win32_Debug_win98"
# PROP BASE Intermediate_Dir "UsbSnoop___Win32_Debug_win98"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug98"
# PROP Intermediate_Dir "debug98"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Gz /W3 /Z7 /Oi /Gy /I "c:\98ddk\inc\win98" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D _X86_=1 /D i386=1 /D "STD_CALL" /D CONDITION_HANDLING=1 /D NT_UP=1 /D NT_INST=0 /D WIN32=100 /D _NT1X_=100 /D WINNT=1 /D _WIN32_WINNT=0x0500 /D _WIN32_IE=0x0400 /D WIN32_LEAN_AND_MEAN=1 /D DBG=1 /D DEVL=1 /D FPO=0 /D "NDEBUG" /D _DLL=1 /D "DRIVER" /D "_IDWBUILD" /D "RDRDBG" /D "SRVDBG" /FR /Yu"stddcls.h" /FD /GZ /Zel -cbstring /QIfdiv- /QI0f /GF /QIf /c
# ADD CPP /nologo /Gz /W3 /Z7 /Gy /I "$(98DDK)\inc\win98" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D _X86_=1 /D i386=1 /D "STD_CALL" /D CONDITION_HANDLING=1 /D NT_UP=1 /D NT_INST=0 /D WIN32=100 /D _NT1X_=100 /D WINNT=1 /D _WIN32_WINNT=0x0500 /D _WIN32_IE=0x0400 /D WIN32_LEAN_AND_MEAN=1 /D DBG=1 /D DEVL=1 /D FPO=0 /D "NDEBUG" /D _DLL=1 /D "DRIVER" /D "_IDWBUILD" /D "RDRDBG" /D "SRVDBG" /D "WIN98" /FR /FD /QIfdiv- /QI0f /GF /QIf /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wdm.lib /nologo /base:"0x10000" /version:4.0 /entry:"DriverEntry@8" /subsystem:windows /pdb:none /debug /debugtype:both /machine:I386 /nodefaultlib /out:"debug\UsbSnoop.sys" /libpath:"c:\98ddk\lib\i386\checked" -MERGE:_PAGE=PAGE -MERGE:_TEXT=.text -MERGE:.rdata=.text -SECTION:INIT,d -OPT:REF -FORCE:MULTIPLE -RELEASE -FULLBUILD -IGNORE:4001,4037,4039,4065,4070,4078,4087,4089,4096 -osversion:4.00 -optidata -driver -align:0x20 -subsystem:native,4.00 -debug:notmapped,FULL
# ADD LINK32 wdm.lib /nologo /base:"0x10000" /version:1.5 /entry:"DriverEntry@8" /subsystem:windows /pdb:none /debug /debugtype:both /machine:I386 /nodefaultlib /out:"debug98\usbsnoop.sys" /libpath:"$(98DDK)\lib\i386\checked" -MERGE:_PAGE=PAGE -MERGE:_TEXT=.text -MERGE:.rdata=.text -SECTION:INIT,d -OPT:REF -FORCE:MULTIPLE -RELEASE -FULLBUILD -IGNORE:4001,4037,4039,4065,4070,4078,4087,4089,4096 -osversion:4.00 -optidata -driver -align:0x20 -subsystem:native,4.00 -debug:notmapped,FULL

!ELSEIF  "$(CFG)" == "UsbSnoop - Win32 Release win2k"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "UsbSnoop___Win32_Release_win2k"
# PROP BASE Intermediate_Dir "UsbSnoop___Win32_Release_win2k"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "release2k"
# PROP Intermediate_Dir "release2k"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Gz /W3 /GX /Zi /Od /I "c:\ntddk\inc" /D "WIN32" /D "_DEBUG" /D _X86_=1 /D i386=1 /D "STD_CALL" /D CONDITION_HANDLING=1 /D NT_UP=1 /D NT_INST=0 /D WIN32=100 /D _NT1X_=100 /D WINNT=1 /D _WIN32_WINNT=0x0500 /D _WIN32_IE=0x0501 /D WIN32_LEAN_AND_MEAN=1 /D DBG=1 /D DEVL=1 /D FPO=0 /D "NDEBUG" /D _DLL=1 /D "DRIVER" /D "_IDWBUILD" /D "RDRDBG" /D "SRVDBG" /D "WIN2K" /D "NDIS50" /FR /FD /Zel -cbstring /QIfdiv- /QI0f /GF /QIf /c
# ADD CPP /nologo /Gz /MD /W3 /O2 /I "$(NTDDK)\inc" /D "WIN32" /D "_DEBUG" /D _X86_=1 /D i386=1 /D "STD_CALL" /D CONDITION_HANDLING=1 /D NT_UP=1 /D NT_INST=0 /D WIN32=100 /D _NT1X_=100 /D WINNT=1 /D _WIN32_WINNT=0x0500 /D _WIN32_IE=0x0501 /D WIN32_LEAN_AND_MEAN=1 /D DBG=1 /D DEVL=1 /D FPO=0 /D "NDEBUG" /D _DLL=1 /D "DRIVER" /D "_IDWBUILD" /D "RDRDBG" /D "SRVDBG" /D "WIN2K" /D "NDIS50" /FR /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wdm.lib /nologo /base:"0x10000" /version:4.0 /entry:"DriverEntry@8" /subsystem:windows /pdb:none /debug /debugtype:both /machine:I386 /nodefaultlib /out:"debug2k\UsbSnoop.sys" /libpath:"c:\ntddk\libchk\i386" -MERGE:_PAGE=PAGE -MERGE:_TEXT=.text -MERGE:.rdata=.text -SECTION:INIT,d -OPT:REF -FORCE:MULTIPLE -RELEASE -FULLBUILD -IGNORE:4001,4037,4039,4065,4070,4078,4087,4089,4096 -osversion:4.00 -optidata -driver -align:0x20 -subsystem:native,4.00 -debug:notmapped,FULL
# ADD LINK32 wdm.lib /nologo /base:"0x10000" /version:1.5 /entry:"DriverEntry@8" /subsystem:windows /pdb:none /machine:I386 /nodefaultlib /out:"release2k\usbsnoop.sys" /libpath:"$(NTDDK)\libfre\i386" -MERGE:_PAGE=PAGE -MERGE:_TEXT=.text -MERGE:.rdata=.text -SECTION:INIT,d -OPT:REF -FORCE:MULTIPLE -RELEASE -FULLBUILD -IGNORE:4001,4037,4039,4065,4070,4078,4087,4089,4096 -osversion:4.00 -optidata -driver -align:0x20 -subsystem:native,4.00 -debug:notmapped,FULL
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "UsbSnoop - Win32 Release win98"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "UsbSnoop___Win32_Release_win98"
# PROP BASE Intermediate_Dir "UsbSnoop___Win32_Release_win98"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "release98"
# PROP Intermediate_Dir "release98"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Gz /W3 /Z7 /Gy /I "c:\98ddk\inc\win98" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D _X86_=1 /D i386=1 /D "STD_CALL" /D CONDITION_HANDLING=1 /D NT_UP=1 /D NT_INST=0 /D WIN32=100 /D _NT1X_=100 /D WINNT=1 /D _WIN32_WINNT=0x0500 /D _WIN32_IE=0x0400 /D WIN32_LEAN_AND_MEAN=1 /D DBG=1 /D DEVL=1 /D FPO=0 /D "NDEBUG" /D _DLL=1 /D "DRIVER" /D "_IDWBUILD" /D "RDRDBG" /D "SRVDBG" /D "WIN98" /FR /FD /GZ /Zel -cbstring /QIfdiv- /QI0f /GF /QIf /c
# SUBTRACT BASE CPP /Oi /YX /Yc /Yu
# ADD CPP /nologo /Gz /MD /W3 /O2 /I "$(98DDK)\inc\win98" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D _X86_=1 /D i386=1 /D "STD_CALL" /D CONDITION_HANDLING=1 /D NT_UP=1 /D NT_INST=0 /D WIN32=100 /D _NT1X_=100 /D WINNT=1 /D _WIN32_WINNT=0x0500 /D _WIN32_IE=0x0400 /D WIN32_LEAN_AND_MEAN=1 /D DBG=1 /D DEVL=1 /D FPO=0 /D "NDEBUG" /D _DLL=1 /D "DRIVER" /D "_IDWBUILD" /D "RDRDBG" /D "SRVDBG" /D "WIN98" /FR /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wdm.lib /nologo /base:"0x10000" /version:4.0 /entry:"DriverEntry@8" /subsystem:windows /pdb:none /debug /debugtype:both /machine:I386 /nodefaultlib /out:"debug98\UsbSnoop.sys" /libpath:"c:\98ddk\lib\i386\checked" -MERGE:_PAGE=PAGE -MERGE:_TEXT=.text -MERGE:.rdata=.text -SECTION:INIT,d -OPT:REF -FORCE:MULTIPLE -RELEASE -FULLBUILD -IGNORE:4001,4037,4039,4065,4070,4078,4087,4089,4096 -osversion:4.00 -optidata -driver -align:0x20 -subsystem:native,4.00 -debug:notmapped,FULL
# ADD LINK32 wdm.lib /nologo /base:"0x10000" /version:1.5 /entry:"DriverEntry@8" /subsystem:windows /pdb:none /machine:I386 /nodefaultlib /out:"release98\usbsnoop.sys" /libpath:"$(98DDK)\lib\i386\free" -MERGE:_PAGE=PAGE -MERGE:_TEXT=.text -MERGE:.rdata=.text -SECTION:INIT,d -OPT:REF -FORCE:MULTIPLE -RELEASE -FULLBUILD -IGNORE:4001,4037,4039,4065,4070,4078,4087,4089,4096 -osversion:4.00 -optidata -driver -align:0x20 -subsystem:native,4.00 -debug:notmapped,FULL
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "UsbSnoop - Win32 Debug win2k"
# Name "UsbSnoop - Win32 Debug win98"
# Name "UsbSnoop - Win32 Release win2k"
# Name "UsbSnoop - Win32 Release win98"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\buffer.c

!IF  "$(CFG)" == "UsbSnoop - Win32 Debug win2k"

!ELSEIF  "$(CFG)" == "UsbSnoop - Win32 Debug win98"

!ELSEIF  "$(CFG)" == "UsbSnoop - Win32 Release win2k"

!ELSEIF  "$(CFG)" == "UsbSnoop - Win32 Release win98"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DriverEntry.cpp

!IF  "$(CFG)" == "UsbSnoop - Win32 Debug win2k"

!ELSEIF  "$(CFG)" == "UsbSnoop - Win32 Debug win98"

!ELSEIF  "$(CFG)" == "UsbSnoop - Win32 Release win2k"

!ELSEIF  "$(CFG)" == "UsbSnoop - Win32 Release win98"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MyKdPrint.c

!IF  "$(CFG)" == "UsbSnoop - Win32 Debug win2k"

!ELSEIF  "$(CFG)" == "UsbSnoop - Win32 Debug win98"

!ELSEIF  "$(CFG)" == "UsbSnoop - Win32 Release win2k"

!ELSEIF  "$(CFG)" == "UsbSnoop - Win32 Release win98"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RemoveLock.c

!IF  "$(CFG)" == "UsbSnoop - Win32 Debug win2k"

!ELSEIF  "$(CFG)" == "UsbSnoop - Win32 Debug win98"

!ELSEIF  "$(CFG)" == "UsbSnoop - Win32 Release win2k"

!ELSEIF  "$(CFG)" == "UsbSnoop - Win32 Release win98"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\stddcls.c

!IF  "$(CFG)" == "UsbSnoop - Win32 Debug win2k"

!ELSEIF  "$(CFG)" == "UsbSnoop - Win32 Debug win98"

!ELSEIF  "$(CFG)" == "UsbSnoop - Win32 Release win2k"

!ELSEIF  "$(CFG)" == "UsbSnoop - Win32 Release win98"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\buffer.h
# End Source File
# Begin Source File

SOURCE=.\Driver.h
# End Source File
# Begin Source File

SOURCE=.\MyKdPrint.h
# End Source File
# Begin Source File

SOURCE=.\stddcls.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
