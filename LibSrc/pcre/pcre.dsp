# Microsoft Developer Studio Project File - Name="pcre" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=pcre - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "pcre.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pcre.mak" CFG="pcre - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pcre - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "pcre - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "pcre - Win32 Release Intel" (based on "Win32 (x86) Static Library")
!MESSAGE "pcre - Win32 Debug NET" (based on "Win32 (x86) Static Library")
!MESSAGE "pcre - Win32 Release NET" (based on "Win32 (x86) Static Library")
!MESSAGE "pcre - Win32 Release WDebug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/CPP/LibSrc/pcre", NLAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "pcre - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\DebugDLL"
# PROP BASE Intermediate_Dir ".\DebugDLL"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Lib"
# PROP Intermediate_Dir "..\DebugMD"
# PROP Target_Dir "."
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /Zp2 /MDd /W3 /GX /Z7 /Od /Gy /D "_DEBUG" /D "STATIC" /D "WIN32" /D "_WINDOWS" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /FR /FD /GF /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"..\DebugDLL\pcre.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Lib\pcreMDd.lib"

!ELSEIF  "$(CFG)" == "pcre - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseDLL"
# PROP BASE Intermediate_Dir "ReleaseDLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Lib"
# PROP Intermediate_Dir "..\ReleaseMD"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /W3 /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "STATIC" /YX /FD /c
# ADD CPP /nologo /G6 /Zp2 /MD /W3 /GX /Ot /Oa /Og /Oi /Oy /Ob2 /Gy /D "NDEBUG" /D "STATIC" /D "WIN32" /D "_WINDOWS" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /FD /GF /c
# SUBTRACT CPP /Ox /YX
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Lib\pcreMD.lib"

!ELSEIF  "$(CFG)" == "pcre - Win32 Release Intel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\Lib"
# PROP BASE Intermediate_Dir "..\ReleaseDLL_Intel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Lib"
# PROP Intermediate_Dir "..\ReleaseMD_Intel"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Zp2 /MD /W3 /GX /O2 /D "NDEBUG" /D "STATIC" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G6 /Zp2 /MD /W3 /GX /Ot /Oa /Og /Oi /Oy /Ob2 /Gy /X /I "\CPP\STLport-4.5.3\stlport" /I "D:\MSDEV\VC98\Include" /I "\CPP\Include" /D "STATIC" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /D "_USE_INTEL_COMPILER" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /FD /GF /c
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:"..\..\Lib\pcreMDI.lib"

!ELSEIF  "$(CFG)" == "pcre - Win32 Debug NET"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "pcre___Win32_Debug_MD_NET"
# PROP BASE Intermediate_Dir "pcre___Win32_Debug_MD_NET"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Lib"
# PROP Intermediate_Dir "..\DebugMD7"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Zp2 /MDd /W3 /GX /Z7 /Od /D "_DEBUG" /D "STATIC" /D "WIN32" /D "_WINDOWS" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /FR /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G5 /Zp2 /MDd /W3 /GX /Z7 /Od /Gy /D "STATIC" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D _USE_COMPILER=VC7 /FR /FD /GF /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"..\DebugDLL\pcre.bsc"
# ADD BSC32 /nologo /o"..\DebugDLL\pcre.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\Lib\pcreMDd.lib"
# ADD LIB32 /nologo /out:"..\..\Lib\pcreMDd7.lib"

!ELSEIF  "$(CFG)" == "pcre - Win32 Release NET"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "pcre___Win32_Release_MD_NET"
# PROP BASE Intermediate_Dir "pcre___Win32_Release_MD_NET"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Lib"
# PROP Intermediate_Dir "..\ReleaseMD7"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /Zp2 /MD /W3 /GX /Ot /Og /Oi /Oy /Ob2 /D "NDEBUG" /D "STATIC" /D "WIN32" /D "_WINDOWS" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G6 /Zp2 /MD /W3 /GX /Ot /Oa /Og /Oi /Oy /Ob2 /Gy /D "STATIC" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D _USE_COMPILER=VC7 /FD /GF /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\Lib\pcreMD.lib"
# ADD LIB32 /nologo /out:"..\..\Lib\pcreMD7.lib"

!ELSEIF  "$(CFG)" == "pcre - Win32 Release WDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "pcre___Win32_Release_WDebug"
# PROP BASE Intermediate_Dir "pcre___Win32_Release_WDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Lib"
# PROP Intermediate_Dir "..\ReleaseMD+d"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /Zp2 /MD /W3 /GX /Ot /Oa /Og /Oi /Oy /Ob2 /Gy /D "NDEBUG" /D "STATIC" /D "WIN32" /D "_WINDOWS" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /FD /GF /c
# SUBTRACT BASE CPP /Ox /YX
# ADD CPP /nologo /G6 /Zp2 /MD /W3 /GX /Ot /Oa /Og /Oi /Oy /Ob2 /Gy /D "NDEBUG" /D "STATIC" /D "WIN32" /D "_WINDOWS" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /FD /GF /c
# SUBTRACT CPP /Ox /YX
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\Lib\pcreMD.lib"
# ADD LIB32 /nologo /out:"..\..\Lib\pcreMD+d.lib"

!ENDIF 

# Begin Target

# Name "pcre - Win32 Debug"
# Name "pcre - Win32 Release"
# Name "pcre - Win32 Release Intel"
# Name "pcre - Win32 Debug NET"
# Name "pcre - Win32 Release NET"
# Name "pcre - Win32 Release WDebug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\CRegExp.cpp
# End Source File
# Begin Source File

SOURCE=.\get.c
# End Source File
# Begin Source File

SOURCE=.\maketables.c
# End Source File
# Begin Source File

SOURCE=.\pcre.c
# End Source File
# Begin Source File

SOURCE=.\pcreposix.c
# End Source File
# Begin Source File

SOURCE=.\study.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\chartables.c
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\Include\CRegExp.h
# End Source File
# Begin Source File

SOURCE=.\DOS866.c

!IF  "$(CFG)" == "pcre - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Release Intel"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Debug NET"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Release NET"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Release WDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\internal.h
# End Source File
# Begin Source File

SOURCE=.\OEM.c

!IF  "$(CFG)" == "pcre - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Release Intel"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Debug NET"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Release NET"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Release WDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Include\pcre\pcre.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\pcre\pcreposix.h
# End Source File
# End Group
# End Target
# End Project
