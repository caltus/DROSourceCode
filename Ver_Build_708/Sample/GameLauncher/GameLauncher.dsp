# Microsoft Developer Studio Project File - Name="GameLauncher" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=GameLauncher - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GameLauncher.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GameLauncher.mak" CFG="GameLauncher - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GameLauncher - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "GameLauncher - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "GameLauncher - Win32 Release_ST" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GameLauncher - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 HspLLib.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"Release/GameL.exe" /libpath:"..\..\..\Build\Release"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=IF NOT EXIST ..\..\..\Build MD ..\..\..\Build	IF NOT EXIST ..\..\..\Build\Release MD ..\..\..\Build\Release	copy Release\GameL.map ..\..\..\Build\Release	copy Release\GameL.pdb ..\..\..\Build\Release	copy Release\GameL.exe ..\..\..\Build\Release
# End Special Build Tool

!ELSEIF  "$(CFG)" == "GameLauncher - Win32 Debug"

# PROP BASE Use_MFC 6
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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 HspLLib.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"Debug/GameL.exe" /pdbtype:sept /libpath:"..\..\..\Build\Debug"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=IF NOT EXIST ..\..\..\Build MD ..\..\..\Build	IF NOT EXIST ..\..\..\Build\Debug MD ..\..\..\Build\Debug	copy Debug\GameL.map ..\..\..\Build\Debug	copy Debug\GameL.pdb ..\..\..\Build\Debug	copy Debug\GameL.exe ..\..\..\Build\Debug
# End Special Build Tool

!ELSEIF  "$(CFG)" == "GameLauncher - Win32 Release_ST"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameLauncher___Win32_Release_ST"
# PROP BASE Intermediate_Dir "GameLauncher___Win32_Release_ST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_ST"
# PROP Intermediate_Dir "Release_ST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\..\..\Src\HShieldPro\HspLauncherLib\Release\HspLLib.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"Release/GameL.exe"
# ADD LINK32 HspLLib.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"Release_ST/GameL.exe" /libpath:"..\..\..\Build\Release_ST"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=IF NOT EXIST ..\..\..\Build MD ..\..\..\Build	IF NOT EXIST ..\..\..\Build\Release_ST MD ..\..\..\Build\Release_ST	copy Release_ST\GameL.map ..\..\..\Build\Release_ST	copy Release_ST\GameL.pdb ..\..\..\Build\Release_ST	copy Release_ST\GameL.exe ..\..\..\Build\Release_ST
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "GameLauncher - Win32 Release"
# Name "GameLauncher - Win32 Debug"
# Name "GameLauncher - Win32 Release_ST"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\GameLauncher.cpp
# End Source File
# Begin Source File

SOURCE=.\GameLauncher.rc
# End Source File
# Begin Source File

SOURCE=.\GameLauncherDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\GameLauncher.h
# End Source File
# Begin Source File

SOURCE=.\GameLauncherDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\GameLauncher.ico
# End Source File
# Begin Source File

SOURCE=.\res\GameLauncher.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
