# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=SekaijuRes - Win32 Jpn
!MESSAGE 構成が指定されていません。ﾃﾞﾌｫﾙﾄの SekaijuRes - Win32 Jpn を設定します。
!ENDIF 

!IF "$(CFG)" != "SekaijuRes - Win32 Jpn" && "$(CFG)" !=\
 "SekaijuRes - Win32 Enu"
!MESSAGE 指定された ﾋﾞﾙﾄﾞ ﾓｰﾄﾞ "$(CFG)" は正しくありません。
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛ 'CFG' を定義することによって
!MESSAGE NMAKE 実行時にﾋﾞﾙﾄﾞ ﾓｰﾄﾞを指定できます。例えば:
!MESSAGE 
!MESSAGE NMAKE /f "SekaijuRes.mak" CFG="SekaijuRes - Win32 Jpn"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "SekaijuRes - Win32 Jpn" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "SekaijuRes - Win32 Enu" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 
!ERROR 無効な構成が指定されています。
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
RSC=rc.exe
MTL=mktyplib.exe
CPP=cl.exe

!IF  "$(CFG)" == "SekaijuRes - Win32 Jpn"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Jpn"
# PROP BASE Intermediate_Dir "Jpn"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Jpn"
# PROP Intermediate_Dir "Jpn"
# PROP Target_Dir ""
OUTDIR=.\Jpn
INTDIR=.\Jpn

ALL : "$(OUTDIR)\SekaijuJpn.dll"

CLEAN : 
	-@erase ".\Jpn\SekaijuJpn.dll"
	-@erase ".\Jpn\SekaijuJpn.res"
	-@erase ".\Jpn\SekaijuJpn.lib"
	-@erase ".\Jpn\SekaijuJpn.exp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_OBJS=.\Jpn/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /fo"Jpn/SekaijuJpn.res" /d "_WIN32" /d "NDEBUG" /d "_JPN"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/SekaijuJpn.res" /d "_WIN32" /d "NDEBUG" /d\
 "_JPN" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/SekaijuRes.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Jpn/SekaijuJpn.dll" /NOENTRY
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)/SekaijuJpn.pdb"\
 /machine:I386 /out:"$(OUTDIR)/SekaijuJpn.dll"\
 /implib:"$(OUTDIR)/SekaijuJpn.lib" /NOENTRY 
LINK32_OBJS= \
	"$(INTDIR)/SekaijuJpn.res"

"$(OUTDIR)\SekaijuJpn.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SekaijuRes - Win32 Enu"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Enu"
# PROP BASE Intermediate_Dir "Enu"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Enu"
# PROP Intermediate_Dir "Enu"
# PROP Target_Dir ""
OUTDIR=.\Enu
INTDIR=.\Enu

ALL : "$(OUTDIR)\SekaijuEnu.dll"

CLEAN : 
	-@erase ".\Enu\SekaijuEnu.dll"
	-@erase ".\Enu\SekaijuEnu.res"
	-@erase ".\Enu\SekaijuEnu.lib"
	-@erase ".\Enu\SekaijuEnu.exp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_OBJS=.\Enu/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x409 /fo"Enu/SekaijuEnu.res" /d "_WIN32" /d "NDEBUG" /d "_ENU"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/SekaijuEnu.res" /d "_WIN32" /d "NDEBUG" /d\
 "_ENU" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/SekaijuRes.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Enu/SekaijuEnu.dll" /NOENTRY
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)/SekaijuEnu.pdb"\
 /machine:I386 /out:"$(OUTDIR)/SekaijuEnu.dll"\
 /implib:"$(OUTDIR)/SekaijuEnu.lib" /NOENTRY 
LINK32_OBJS= \
	"$(INTDIR)/SekaijuEnu.res"

"$(OUTDIR)\SekaijuEnu.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

MTL_PROJ=/nologo /D "NDEBUG" /win32 
CPP_PROJ=/nologo /MT /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/SekaijuRes.pch" /YX /Fo"$(INTDIR)/" /c 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "SekaijuRes - Win32 Jpn"
# Name "SekaijuRes - Win32 Enu"

!IF  "$(CFG)" == "SekaijuRes - Win32 Jpn"

!ELSEIF  "$(CFG)" == "SekaijuRes - Win32 Enu"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\SekaijuRes.rc
DEP_RSC_SEKAI=\
	".\..\src\winver.h"\
	".\..\src\resource.h"\
	".\SekaijuJpn.rc"\
	".\..\res\MainFrame.ico"\
	".\..\res\SekaijuType.ico"\
	".\..\res\Toolbar1.bmp"\
	".\..\res\Toolbar2.bmp"\
	".\..\res\TrackList1.bmp"\
	".\..\res\PianoRoll1.bmp"\
	".\..\res\EventList1.bmp"\
	".\..\res\MusicalScore1.bmp"\
	".\SekaijuEnu.rc"\
	

!IF  "$(CFG)" == "SekaijuRes - Win32 Jpn"


"$(INTDIR)\SekaijuJpn.res" : $(SOURCE) $(DEP_RSC_SEKAI) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SekaijuRes - Win32 Enu"


"$(INTDIR)\SekaijuEnu.res" : $(SOURCE) $(DEP_RSC_SEKAI) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
