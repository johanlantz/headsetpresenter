; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "LicenseManager SDK"
!define PRODUCT_VERSION "0.9"
!define PRODUCT_PUBLISHER "w2solutions"
!define PRODUCT_WEB_SITE "http://www.w2solutions.se"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\LicenseManagerSample.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!define MUI_LICENSEPAGE_RADIOBUTTONS
!insertmacro MUI_PAGE_LICENSE "license_LicenseManager.txt"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\ReadMeFirst.doc"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "Setup_LicManSDK.exe"
InstallDir "$PROGRAMFILES\LicenseManager SDK"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section "MainSection" SEC01
  SetOutPath "$INSTDIR\bin"
  SetOverwrite on
  File "..\LicenseManager\Release\LicenseManager.dll"
  File "..\LicenseManager\Release\LicenseManager.tlb"
  SetOutPath "$INSTDIR\include"
  File "..\LicenseManager\LicenseManager.h"
  SetOutPath "$INSTDIR\Samples\CPP\Release"
  File "..\LicenseManager SDK\Samples\CPP\Release\LicenseManagerSample.exe"
  CreateDirectory "$SMPROGRAMS\LicenseManager SDK"
  CreateShortCut "$SMPROGRAMS\LicenseManager SDK\LicenseManager C++ Sample.lnk" "$INSTDIR\Samples\CPP\Release\LicenseManagerSample.exe"
  SetOutPath "$INSTDIR\Samples\CPP"
  File "..\HS_gen\LicenseManagerProxy.h"
  File "..\HS_gen\LicenseManagerProxy.cpp"
  File "..\LicenseManager SDK\Samples\CPP\LicenseManagerSample"
  File "..\LicenseManager SDK\Samples\CPP\LicenseManagerSample.cpp"
  File "..\LicenseManager SDK\Samples\CPP\LicenseManagerSample.ncb"
  File "..\LicenseManager SDK\Samples\CPP\LicenseManagerSample.sln"
  File "..\LicenseManager SDK\Samples\CPP\LicenseManagerSample.suo"
  File "..\LicenseManager SDK\Samples\CPP\LicenseManagerSample.vcproj"
  File "..\LicenseManager SDK\Samples\CPP\ReadMe.txt"
  File "..\LicenseManager SDK\Samples\CPP\stdafx.cpp"
  File "..\LicenseManager SDK\Samples\CPP\stdafx.h"
  SetOutPath "$INSTDIR\Samples\VB.NET"
  File "..\LicenseManager SDK\Samples\VB.NET\AssemblyInfo.vb"
  SetOutPath "$INSTDIR\Samples\VB.NET\bin"
  File "..\LicenseManager SDK\Samples\VB.NET\bin\Interop.LicenseManagerLib.dll"
  File "..\LicenseManager SDK\Samples\VB.NET\bin\LicenseManagerSample.exe"
  File "..\LicenseManager SDK\Samples\VB.NET\bin\LicenseManagerSample.pdb"
  SetOutPath "$INSTDIR\Samples\VB.NET"
  File "..\LicenseManager SDK\Samples\VB.NET\Form1.resx"
  File "..\LicenseManager SDK\Samples\VB.NET\Form1.vb"
  File "..\LicenseManager SDK\Samples\VB.NET\LicenseManagerSample.sln"
  File "..\LicenseManager SDK\Samples\VB.NET\LicenseManagerSample.suo"
  File "..\LicenseManager SDK\Samples\VB.NET\LicenseManagerSample.vbproj"
  File "..\LicenseManager SDK\Samples\VB.NET\LicenseManagerSample.vbproj.user"
  SetOutPath "$INSTDIR\Samples\VB.NET\obj\Debug"
  File "..\LicenseManager SDK\Samples\VB.NET\obj\Debug\LicenseManagerSample.exe"
  File "..\LicenseManager SDK\Samples\VB.NET\obj\Debug\LicenseManagerSample.Form1.resources"
  File "..\LicenseManager SDK\Samples\VB.NET\obj\Debug\LicenseManagerSample.pdb"
  SetOutPath "$INSTDIR\Samples\VB.NET\obj"
  File "..\LicenseManager SDK\Samples\VB.NET\obj\Interop.LicenseManagerLib.dll"
  SetOutPath "$INSTDIR\Samples\VB6"
  File "..\LicenseManager SDK\Samples\VB6\lictestappvb.frm"
  File "..\LicenseManager SDK\Samples\VB6\MSSCCPRJ.SCC"
  File "..\LicenseManager SDK\Samples\VB6\Project1.exe"
  File "..\LicenseManager SDK\Samples\VB6\Project1.opt"
  File "..\LicenseManager SDK\Samples\VB6\Project1.vbp"
  File "..\LicenseManager SDK\Samples\VB6\Project1.vbw"
  SetOutPath "$INSTDIR"
  SetOverwrite on
  File "..\Documentation\ReadMeFirst.doc"
  SetOutPath "$INSTDIR\doc"
  File "..\Documentation\Using_the_LicenseManager_Component.pdf"
  SetOutPath "$INSTDIR\Tools"
  File "..\CRCBumper\Release\CRCBumper.exe"
  SetOutPath "$INSTDIR\CRC"
  File "..\LicenseManager\CRC\LicManCRC.h"
  File "..\LicenseManager\CRC\LicManCRC.txt"
SectionEnd

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\LicenseManager SDK\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\LicenseManager SDK\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  ;RegDLL "$INSTDIR\bin\LicenseManager.dll"  Better if the user register manually
  ;WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\Samples\CPP\Debug\LicenseManagerSample.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\Samples\CPP\Debug\LicenseManagerSample.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\doc\Using_the_LicenseManager_Component.pdf"
  Delete "$INSTDIR\ReadMeFirst.doc"
  Delete "$INSTDIR\Samples\VB6\*.*"
  Delete "$INSTDIR\Samples\VB.NET\obj\Debug*.*"
  Delete "$INSTDIR\Samples\VB.NET\obj\*.*"
  Delete "$INSTDIR\Samples\VB.NET\*.*"
  Delete "$INSTDIR\Samples\VB.NET\bin\*.*"
  Delete "$INSTDIR\Samples\CPP\Release\*.*"
  Delete "$INSTDIR\Samples\CPP\*.*"
  Delete "$INSTDIR\include\LicenseManager.h"
  Delete "$INSTDIR\bin\LicenseManager.tlb"
  Delete "$INSTDIR\bin\LicenseManager.dll"
  Delete "$INSTDIR\Tools\*.*"
   Delete "$INSTDIR\CRC\*.*"

  Delete "$SMPROGRAMS\LicenseManager SDK\Uninstall.lnk"
  Delete "$SMPROGRAMS\LicenseManager SDK\Website.lnk"
  Delete "$SMPROGRAMS\LicenseManager SDK\LicenseManager C++ Sample.lnk"
  Delete "$DESKTOP\LicenseManager SDK.lnk"
  Delete "$SMPROGRAMS\LicenseManager SDK\LicenseManager SDK.lnk"

  RMDir "$SMPROGRAMS\LicenseManager SDK"
  RMDir "$INSTDIR\Samples\VB6"
  RMDir "$INSTDIR\Samples\VB.NET\obj\Debug"
  RMDir "$INSTDIR\Samples\VB.NET\obj"
  RMDir "$INSTDIR\Samples\VB.NET\bin"
  RMDir "$INSTDIR\Samples\VB.NET"
  RMDir "$INSTDIR\Samples\CPP\Debug"
  RMDir "$INSTDIR\Samples\CPP"
  RMDir "$INSTDIR\include"
  RMDir "$INSTDIR\doc"
  RMDir "$INSTDIR\bin"
  RMDir "$INSTDIR\Tools"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd