@echo off

rem * tell user about info
echo We will try to copy 14 files.

rem * the include files
copy SS_Utils.h 		"\Program Files\Microsoft Visual Studio\VC98\SS_Utils"
copy SS_Log.h 			"\Program Files\Microsoft Visual Studio\VC98\SS_Utils"
copy SS_RegistryKey.h		"\Program Files\Microsoft Visual Studio\VC98\SS_Utils"
copy SS_Result.h		"\Program Files\Microsoft Visual Studio\VC98\SS_Utils"
copy SS_Result_Enum.h		"\Program Files\Microsoft Visual Studio\VC98\SS_Utils"
copy SS_String.h		"\Program Files\Microsoft Visual Studio\VC98\SS_Utils"
copy version.h			"\Program Files\Microsoft Visual Studio\VC98\SS_Utils"
copy PasswordToReg.h		"\Program Files\Microsoft Visual Studio\VC98\SS_Utils"


rem * the libraries
copy Debug\UtilsD.lib		"\Program Files\Microsoft Visual Studio\VC98\SS_Utils"
copy Release\Utils.lib		"\Program Files\Microsoft Visual Studio\VC98\SS_Utils"
copy Unicode_Debug\UtilsUD.lib	"\Program Files\Microsoft Visual Studio\VC98\SS_Utils"
copy Unicode_Release\UtilsU.lib	"\Program Files\Microsoft Visual Studio\VC98\SS_Utils"

rem * the SS_Log_Window.exe application
copy SS_Log\SS_Log_Window\release\SS_Log_Window.exe %SYSTEMROOT%

rem * the SS_Log_AddIn MSDEV add-in.  register it too.
copy SS_Log\SS_Log_AddIn\release\SS_Log_AddIn.dll %SYSTEMROOT%
regsvr32 %SYSTEMROOT%\SS_Log_AddIn.dll

