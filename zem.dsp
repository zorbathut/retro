# Microsoft Developer Studio Project File - Name="zem" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=zem - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "zem.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "zem.mak" CFG="zem - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "zem - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "zem - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "zem - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 libpngr.lib d3dx8.lib d3d8.lib kernel32.lib user32.lib GDI32.lib /nologo /subsystem:windows /pdb:none /machine:I386
# SUBTRACT LINK32 /verbose

!ELSEIF  "$(CFG)" == "zem - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libpngd.lib d3dx8.lib d3d8.lib kernel32.lib user32.lib GDI32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /verbose /pdb:none /nodefaultlib

!ENDIF 

# Begin Target

# Name "zem - Win32 Release"
# Name "zem - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\errlog.cpp
# End Source File
# Begin Source File

SOURCE=.\kDescribable.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\config.h
# End Source File
# Begin Source File

SOURCE=.\errlog.h
# End Source File
# Begin Source File

SOURCE=.\iostream_mini.h
# End Source File
# Begin Source File

SOURCE=.\kControls.h
# End Source File
# Begin Source File

SOURCE=.\kDescribable.h
# End Source File
# Begin Source File

SOURCE=.\kGlobalStorage.h
# End Source File
# Begin Source File

SOURCE=.\kInterface.h
# End Source File
# Begin Source File

SOURCE=.\kPoint.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "port-targets"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\kInterfaceWin32.cpp
# End Source File
# Begin Source File

SOURCE=.\null.h
# End Source File
# Begin Source File

SOURCE=.\types.h
# End Source File
# End Group
# Begin Group "zutil"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\kFunctor.h
# End Source File
# Begin Source File

SOURCE=.\kString.cpp
# End Source File
# Begin Source File

SOURCE=.\kString.h
# End Source File
# Begin Source File

SOURCE=.\minmax.h
# End Source File
# Begin Source File

SOURCE=.\miscfunc.h
# End Source File
# End Group
# Begin Group "libpng"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\libpng\png.h
# End Source File
# Begin Source File

SOURCE=.\libpng\pngconf.h
# End Source File
# Begin Source File

SOURCE=.\libpng\zconf.h
# End Source File
# Begin Source File

SOURCE=.\libpng\zlib.h
# End Source File
# End Group
# Begin Group "grfx"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\kColor.h
# End Source File
# Begin Source File

SOURCE=.\kGrfxFont.cpp
# End Source File
# Begin Source File

SOURCE=.\kGrfxFont.h
# End Source File
# Begin Source File

SOURCE=.\kGrfxFontFile.cpp
# End Source File
# Begin Source File

SOURCE=.\kGrfxFontFile.h
# End Source File
# Begin Source File

SOURCE=.\kGrfxFontNull.cpp
# End Source File
# Begin Source File

SOURCE=.\kGrfxFontNull.h
# End Source File
# Begin Source File

SOURCE=.\kGrfxRenderable.cpp
# End Source File
# Begin Source File

SOURCE=.\kGrfxRenderable.h
# End Source File
# Begin Source File

SOURCE=.\kGrfxRenderableFinite.cpp
# End Source File
# Begin Source File

SOURCE=.\kGrfxRenderableFinite.h
# End Source File
# Begin Source File

SOURCE=.\kGrfxRenderableFiniteNull.cpp
# End Source File
# Begin Source File

SOURCE=.\kGrfxRenderableFiniteNull.h
# End Source File
# Begin Source File

SOURCE=.\kGrfxRenderableOwnedraster.cpp
# End Source File
# Begin Source File

SOURCE=.\kGrfxRenderableOwnedraster.h
# End Source File
# Begin Source File

SOURCE=.\kGrfxRenderablePNG.cpp
# End Source File
# Begin Source File

SOURCE=.\kGrfxRenderablePNG.h
# End Source File
# Begin Source File

SOURCE=.\kGrfxRenderableRaster.cpp
# End Source File
# Begin Source File

SOURCE=.\kGrfxRenderableRaster.h
# End Source File
# Begin Source File

SOURCE=.\kGrfxUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\kGrfxUtil.h
# End Source File
# Begin Source File

SOURCE=.\kGrfxWritable.cpp
# End Source File
# Begin Source File

SOURCE=.\kGrfxWritable.h
# End Source File
# Begin Source File

SOURCE=.\kGrfxWritable16bpp.cpp
# End Source File
# Begin Source File

SOURCE=.\kGrfxWritable16bpp.h
# End Source File
# Begin Source File

SOURCE=.\kGrfxWritable16bpp565.cpp
# End Source File
# Begin Source File

SOURCE=.\kGrfxWritable16bpp565.h
# End Source File
# Begin Source File

SOURCE=.\kGrfxWritableOwnedraster.cpp
# End Source File
# Begin Source File

SOURCE=.\kGrfxWritableOwnedraster.h
# End Source File
# Begin Source File

SOURCE=.\kGrfxWritableRaster.cpp
# End Source File
# Begin Source File

SOURCE=.\kGrfxWritableRaster.h
# End Source File
# Begin Source File

SOURCE=.\kRaster.cpp
# End Source File
# Begin Source File

SOURCE=.\kRaster.h
# End Source File
# End Group
# Begin Group "file"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\kFileBase.cpp
# End Source File
# Begin Source File

SOURCE=.\kFileBase.h
# End Source File
# Begin Source File

SOURCE=.\kFileBaseNull.cpp
# End Source File
# Begin Source File

SOURCE=.\kFileBaseNull.h
# End Source File
# Begin Source File

SOURCE=.\kFileHandle.h
# End Source File
# Begin Source File

SOURCE=.\kFileManager.cpp
# End Source File
# Begin Source File

SOURCE=.\kFileManager.h
# End Source File
# Begin Source File

SOURCE=.\kFileModule.h
# End Source File
# Begin Source File

SOURCE=.\kFileModuleHandle.h
# End Source File
# Begin Source File

SOURCE=.\kFileModuleRawhandle.h
# End Source File
# Begin Source File

SOURCE=.\kFileRawhandle.h
# End Source File
# Begin Source File

SOURCE=.\kFileWrapped.cpp
# End Source File
# Begin Source File

SOURCE=.\kFileWrapped.h
# End Source File
# End Group
# Begin Group "mainshell"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\kMainShell.cpp
# End Source File
# Begin Source File

SOURCE=.\kMainShell.h
# End Source File
# Begin Source File

SOURCE=.\kMainShellMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\kMainShellMenu.h
# End Source File
# Begin Source File

SOURCE=.\kMainShellStackhead.cpp
# End Source File
# Begin Source File

SOURCE=.\kMainShellStackhead.h
# End Source File
# End Group
# Begin Group "boost"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\butility.h
# End Source File
# End Group
# Begin Group "modules"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\kMGrfxFont.cpp
# End Source File
# Begin Source File

SOURCE=.\kMGrfxFont.h
# End Source File
# Begin Source File

SOURCE=.\kMGrfxRenderableFinite.cpp
# End Source File
# Begin Source File

SOURCE=.\kMGrfxRenderableFinite.h
# End Source File
# Begin Source File

SOURCE=.\kMGrfxRenderableFiniteRaw.cpp
# End Source File
# Begin Source File

SOURCE=.\kMGrfxRenderableFiniteRaw.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\changelog.txt
# End Source File
# Begin Source File

SOURCE=.\datadog\errlog.txt
# End Source File
# Begin Source File

SOURCE=.\notes.txt
# End Source File
# End Target
# End Project
