# Microsoft Developer Studio Project File - Name="pcre" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=pcre - Win32 Debug MD NET
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "pcre.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pcre.mak" CFG="pcre - Win32 Debug MD NET"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pcre - Win32 Debug ML" (based on "Win32 (x86) Static Library")
!MESSAGE "pcre - Win32 Debug MT" (based on "Win32 (x86) Static Library")
!MESSAGE "pcre - Win32 Debug MD" (based on "Win32 (x86) Static Library")
!MESSAGE "pcre - Win32 Release ML" (based on "Win32 (x86) Static Library")
!MESSAGE "pcre - Win32 Release MT" (based on "Win32 (x86) Static Library")
!MESSAGE "pcre - Win32 Release MD" (based on "Win32 (x86) Static Library")
!MESSAGE "pcre - Win32 Release ML Intel" (based on "Win32 (x86) Static Library")
!MESSAGE "pcre - Win32 Release MT Intel" (based on "Win32 (x86) Static Library")
!MESSAGE "pcre - Win32 Release MD Intel" (based on "Win32 (x86) Static Library")
!MESSAGE "pcre - Win32 Debug MD NET" (based on "Win32 (x86) Static Library")
!MESSAGE "pcre - Win32 Release MD NET" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/CPP/LibSrc/pcre", NLAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "pcre - Win32 Debug ML"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Lib"
# PROP Intermediate_Dir "..\DebugML"
# PROP Target_Dir "."
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /Zp2 /W3 /GX /Z7 /Od /D "_DEBUG" /D "STATIC" /D "WIN32" /D "_WINDOWS" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"..\Debug\pcre.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Lib\pcreMLd.lib"

!ELSEIF  "$(CFG)" == "pcre - Win32 Debug MT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "pcre___Win32_Debug_MT"
# PROP BASE Intermediate_Dir "pcre___Win32_Debug_MT"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Lib"
# PROP Intermediate_Dir "..\DebugMT"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Zp2 /W3 /GX /Z7 /Od /D "_DEBUG" /D "STATIC" /D "WIN32" /D "_WINDOWS" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /FR /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G5 /Zp2 /MTd /W3 /GX /Z7 /Od /D "_DEBUG" /D "STATIC" /D "WIN32" /D "_WINDOWS" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"..\Debug\pcre.bsc"
# ADD BSC32 /nologo /o"..\Debug\pcre.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\Lib\pcreD.lib"
# ADD LIB32 /nologo /out:"..\..\Lib\pcreMTd.lib"

!ELSEIF  "$(CFG)" == "pcre - Win32 Debug MD"

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
# ADD CPP /nologo /G5 /Zp2 /MDd /W3 /GX /Z7 /Od /D "_DEBUG" /D "STATIC" /D "WIN32" /D "_WINDOWS" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"..\DebugDLL\pcre.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Lib\pcreMDd.lib"

!ELSEIF  "$(CFG)" == "pcre - Win32 Release ML"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Lib"
# PROP Intermediate_Dir "..\ReleaseML"
# PROP Target_Dir "."
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G6 /Zp2 /W3 /GX /Ot /Og /Oi /Oy /Ob2 /D "NDEBUG" /D "STATIC" /D "WIN32" /D "_WINDOWS" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /FD /c
# SUBTRACT CPP /Ox /YX
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Lib\pcreML.lib"

!ELSEIF  "$(CFG)" == "pcre - Win32 Release MT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "pcre___Win32_Release_MT"
# PROP BASE Intermediate_Dir "pcre___Win32_Release_MT"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Lib"
# PROP Intermediate_Dir "..\ReleaseMT"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /Zp2 /W3 /GX /Ot /Og /Oi /Oy /Ob2 /D "NDEBUG" /D "STATIC" /D "WIN32" /D "_WINDOWS" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /FD /c
# SUBTRACT BASE CPP /Ox /YX
# ADD CPP /nologo /G6 /Zp2 /MT /W3 /GX /Ot /Og /Oi /Oy /Ob2 /D "NDEBUG" /D "STATIC" /D "WIN32" /D "_WINDOWS" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /FD /c
# SUBTRACT CPP /Ox /YX
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Lib\pcreMT.lib"

!ELSEIF  "$(CFG)" == "pcre - Win32 Release MD"

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
# ADD CPP /nologo /G6 /Zp2 /MD /W3 /GX /Ot /Og /Oi /Oy /Ob2 /D "NDEBUG" /D "STATIC" /D "WIN32" /D "_WINDOWS" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Lib\pcreMD.lib"

!ELSEIF  "$(CFG)" == "pcre - Win32 Release ML Intel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\Lib"
# PROP BASE Intermediate_Dir "..\Release_Intel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Lib"
# PROP Intermediate_Dir "..\ReleaseML_Intel"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Zp2 /W3 /GX /O2 /D "NDEBUG" /D "STATIC" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G6 /Zp2 /W3 /GX /Ob2 /X /I "E:\CPP\STLport-4.5.3\stlport" /I "D:\MSDEV\VC98\Include" /I "\CPP\Include" /D "STATIC" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /D "_USE_INTEL_COMPILER" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /FD /O3 /c
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Lib\pcreMLI.lib"

!ELSEIF  "$(CFG)" == "pcre - Win32 Release MT Intel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "pcre___Win32_Release_MT_Intel"
# PROP BASE Intermediate_Dir "pcre___Win32_Release_MT_Intel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Lib"
# PROP Intermediate_Dir "..\ReleaseMT_Intel"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /Zp2 /W3 /GX /Ob2 /D "STATIC" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /D "ICL" /D "_USE_INTEL_COMPILER" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /FD /O3 /c
# ADD CPP /nologo /G6 /Zp2 /MT /W3 /GX /Ob2 /X /I "E:\CPP\STLport-4.5.3\stlport" /I "D:\MSDEV\VC98\Include" /I "\CPP\Include" /D "STATIC" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /D "_USE_INTEL_COMPILER" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /FD /O3 /c
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\Lib\pcreI.lib"
# ADD LIB32 /nologo /out:"..\..\Lib\pcreMTI.lib"

!ELSEIF  "$(CFG)" == "pcre - Win32 Release MD Intel"

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
# ADD CPP /nologo /G6 /Zp2 /MD /W3 /GX /Ob2 /X /I "E:\CPP\STLport-4.5.3\stlport" /I "D:\MSDEV\VC98\Include" /I "\CPP\Include" /D "STATIC" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /D "_USE_INTEL_COMPILER" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /FD /O3 /c
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:"..\..\Lib\pcreMDI.lib"

!ELSEIF  "$(CFG)" == "pcre - Win32 Debug MD NET"

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
# ADD CPP /nologo /G5 /Zp2 /MDd /W3 /GX /Z7 /Od /D "STATIC" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D _USE_COMPILER=VC7 /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"..\DebugDLL\pcre.bsc"
# ADD BSC32 /nologo /o"..\DebugDLL\pcre.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\Lib\pcreMDd.lib"
# ADD LIB32 /nologo /out:"..\..\Lib\pcreMDd7.lib"

!ELSEIF  "$(CFG)" == "pcre - Win32 Release MD NET"

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
# ADD CPP /nologo /G6 /Zp2 /MD /W3 /GX /Ot /Og /Oi /Oy /Ob2 /D "STATIC" /D POSIX_MALLOC_THRESHOLD=16 /D "SUPPORT_UTF8" /D "PCRE_STATIC" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D _USE_COMPILER=VC7 /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\Lib\pcreMD.lib"
# ADD LIB32 /nologo /out:"..\..\Lib\pcreMD7.lib"

!ENDIF 

# Begin Target

# Name "pcre - Win32 Debug ML"
# Name "pcre - Win32 Debug MT"
# Name "pcre - Win32 Debug MD"
# Name "pcre - Win32 Release ML"
# Name "pcre - Win32 Release MT"
# Name "pcre - Win32 Release MD"
# Name "pcre - Win32 Release ML Intel"
# Name "pcre - Win32 Release MT Intel"
# Name "pcre - Win32 Release MD Intel"
# Name "pcre - Win32 Debug MD NET"
# Name "pcre - Win32 Release MD NET"
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

!IF  "$(CFG)" == "pcre - Win32 Debug ML"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Debug MT"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Debug MD"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Release ML"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Release MT"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Release MD"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Release ML Intel"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Release MT Intel"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Release MD Intel"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Debug MD NET"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Release MD NET"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\internal.h
# End Source File
# Begin Source File

SOURCE=.\OEM.c

!IF  "$(CFG)" == "pcre - Win32 Debug ML"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Debug MT"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Debug MD"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Release ML"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Release MT"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Release MD"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Release ML Intel"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Release MT Intel"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Release MD Intel"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Debug MD NET"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pcre - Win32 Release MD NET"

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
