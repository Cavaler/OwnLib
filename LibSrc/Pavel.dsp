# Microsoft Developer Studio Project File - Name="Pavel" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Pavel - Win32 Debug MT
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Pavel.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Pavel.mak" CFG="Pavel - Win32 Debug MT"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Pavel - Win32 Debug ML" (based on "Win32 (x86) Static Library")
!MESSAGE "Pavel - Win32 Debug MT" (based on "Win32 (x86) Static Library")
!MESSAGE "Pavel - Win32 Debug MD" (based on "Win32 (x86) Static Library")
!MESSAGE "Pavel - Win32 Release ML" (based on "Win32 (x86) Static Library")
!MESSAGE "Pavel - Win32 Release MT" (based on "Win32 (x86) Static Library")
!MESSAGE "Pavel - Win32 Release MD" (based on "Win32 (x86) Static Library")
!MESSAGE "Pavel - Win32 Release ML Intel" (based on "Win32 (x86) Static Library")
!MESSAGE "Pavel - Win32 Release MT Intel" (based on "Win32 (x86) Static Library")
!MESSAGE "Pavel - Win32 Release MD Intel" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Pavel - Win32 Debug ML"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir ".\DebugML"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /Zp2 /W3 /GX /Z7 /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "FAR_USE_NAMESPACE" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Lib\PavelMLd.lib"

!ELSEIF  "$(CFG)" == "Pavel - Win32 Debug MT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Pavel___Win32_Debug_MT"
# PROP BASE Intermediate_Dir "Pavel___Win32_Debug_MT"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir ".\DebugMT"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Zp2 /W3 /GX /Z7 /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "FAR_USE_NAMESPACE" /FR /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G5 /Zp2 /MTd /W3 /GX /Z7 /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "FAR_USE_NAMESPACE" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Lib\PavelD.lib"
# ADD LIB32 /nologo /out:"..\Lib\PavelMTd.lib"

!ELSEIF  "$(CFG)" == "Pavel - Win32 Debug MD"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\DebugDLL"
# PROP BASE Intermediate_Dir ".\DebugDLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir ".\DebugMD"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /Zp2 /MDd /W3 /GX /Z7 /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "FAR_USE_NAMESPACE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Lib\PavelMDd.lib"

!ELSEIF  "$(CFG)" == "Pavel - Win32 Release ML"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir ".\ReleaseML"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /Zp2 /W3 /GX /Ot /Og /Oi /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "FAR_USE_NAMESPACE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Lib\PavelML.lib"

!ELSEIF  "$(CFG)" == "Pavel - Win32 Release MT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Pavel___Win32_Release_MT"
# PROP BASE Intermediate_Dir "Pavel___Win32_Release_MT"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir ".\ReleaseMT"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Zp2 /W3 /GX /Ot /Og /Oi /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "FAR_USE_NAMESPACE" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G5 /Zp2 /MT /W3 /GX /Ot /Og /Oi /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "FAR_USE_NAMESPACE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Lib\PavelMT.lib"

!ELSEIF  "$(CFG)" == "Pavel - Win32 Release MD"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Pavel___Win32_ReleaseDLL"
# PROP BASE Intermediate_Dir "Pavel___Win32_ReleaseDLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir ".\ReleaseMD"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /W3 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G5 /Zp2 /MD /W3 /GX /Ot /Og /Oi /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "FAR_USE_NAMESPACE" /FD /c
# SUBTRACT CPP /Z<none> /YX
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Lib\PavelMD.lib"

!ELSEIF  "$(CFG)" == "Pavel - Win32 Release ML Intel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Pavel___Win32_Release_Intel"
# PROP BASE Intermediate_Dir "Pavel___Win32_Release_Intel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir ".\ReleaseML_Intel"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Zp2 /W3 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G6 /Zp2 /W3 /GX /Ob2 /X /I "E:\CPP\STLport-4.5.3\stlport" /I "D:\MSDEV\VC98\Include" /I "\CPP\Include" /D "FAR_USE_NAMESPACE" /D "_USE_INTEL_COMPILER" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /FD /O3 /c
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Lib\PavelMLI.lib"

!ELSEIF  "$(CFG)" == "Pavel - Win32 Release MT Intel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Pavel___Win32_Release_MT_Intel"
# PROP BASE Intermediate_Dir "Pavel___Win32_Release_MT_Intel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir ".\ReleaseMT_Intel"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /Zp2 /W3 /GX /Ob2 /D "FAR_USE_NAMESPACE" /D "ICL" /D "_USE_INTEL_COMPILER" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /FD /O3 /c
# ADD CPP /nologo /G6 /Zp2 /MT /W3 /GX /Ob2 /X /I "E:\CPP\STLport-4.5.3\stlport" /I "D:\MSDEV\VC98\Include" /I "\CPP\Include" /D "FAR_USE_NAMESPACE" /D "_USE_INTEL_COMPILER" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /FD /O3 /c
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Lib\PavelI.lib"
# ADD LIB32 /nologo /out:"..\Lib\PavelMTI.lib"

!ELSEIF  "$(CFG)" == "Pavel - Win32 Release MD Intel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Pavel___Win32_ReleaseDLL_Intel"
# PROP BASE Intermediate_Dir "Pavel___Win32_ReleaseDLL_Intel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir ".\ReleaseMD_Intel"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Zp2 /MD /W3 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G6 /Zp2 /MD /W3 /GX /Ob2 /X /I "E:\CPP\STLport-4.5.3\stlport" /I "D:\MSDEV\VC98\Include" /I "\CPP\Include" /D "FAR_USE_NAMESPACE" /D "_USE_INTEL_COMPILER" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /FD /O3 /c
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"E:\CPP\Lib\PavelRD.lib"
# ADD LIB32 /nologo /out:"..\Lib\PavelMDI.lib"

!ENDIF 

# Begin Target

# Name "Pavel - Win32 Debug ML"
# Name "Pavel - Win32 Debug MT"
# Name "Pavel - Win32 Debug MD"
# Name "Pavel - Win32 Release ML"
# Name "Pavel - Win32 Release MT"
# Name "Pavel - Win32 Release MD"
# Name "Pavel - Win32 Release ML Intel"
# Name "Pavel - Win32 Release MT Intel"
# Name "Pavel - Win32 Release MD Intel"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Ansi.cpp
# End Source File
# Begin Source File

SOURCE=.\BASE64.cpp
# End Source File
# Begin Source File

SOURCE=.\CMapping.cpp
# End Source File
# Begin Source File

SOURCE=.\Collect.cpp
# End Source File
# Begin Source File

SOURCE=.\CString.cpp
# End Source File
# Begin Source File

SOURCE=.\Directory.cpp
# End Source File
# Begin Source File

SOURCE=.\EasyReg.cpp
# End Source File
# Begin Source File

SOURCE=.\Far.cpp
# End Source File
# Begin Source File

SOURCE=.\FarDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FarDlgItems.cpp
# End Source File
# Begin Source File

SOURCE=.\FarDlgValidators.cpp
# End Source File
# Begin Source File

SOURCE=.\FarGetMsgEx.cpp
# End Source File
# Begin Source File

SOURCE=.\HMAC.cpp
# End Source File
# Begin Source File

SOURCE=.\MD5.cpp
# End Source File
# Begin Source File

SOURCE=.\StringEx.cpp
# End Source File
# Begin Source File

SOURCE=.\UTF8.cpp
# End Source File
# Begin Source File

SOURCE=.\XLat.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\Include\Ansi.h
# End Source File
# Begin Source File

SOURCE=..\Include\BASE64.h
# End Source File
# Begin Source File

SOURCE=..\Include\CMapping.h
# End Source File
# Begin Source File

SOURCE=..\Include\Collect.h
# End Source File
# Begin Source File

SOURCE=..\Include\CString.h
# End Source File
# Begin Source File

SOURCE=..\Include\Directory.h
# End Source File
# Begin Source File

SOURCE=..\Include\EasyReg.h
# End Source File
# Begin Source File

SOURCE=..\Include\FAR.h
# End Source File
# Begin Source File

SOURCE=..\Include\FarDlg.h
# End Source File
# Begin Source File

SOURCE=..\Include\HMAC.h
# End Source File
# Begin Source File

SOURCE=..\Include\MD5.h
# End Source File
# Begin Source File

SOURCE=..\Include\plugin.hpp
# End Source File
# Begin Source File

SOURCE=..\Include\ptr.h
# End Source File
# Begin Source File

SOURCE=..\Include\StringEx.h
# End Source File
# Begin Source File

SOURCE=..\Include\UTF8.h
# End Source File
# Begin Source File

SOURCE=..\Include\XLat.h
# End Source File
# End Group
# End Target
# End Project
