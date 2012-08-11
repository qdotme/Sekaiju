# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Sekaiju - Win32 DebugU
!MESSAGE 構成が指定されていません。ﾃﾞﾌｫﾙﾄの Sekaiju - Win32 DebugU を設定します。
!ENDIF 

!IF "$(CFG)" != "Sekaiju - Win32 Release" && "$(CFG)" !=\
 "Sekaiju - Win32 Debug" && "$(CFG)" != "Sekaiju - Win32 DebugU" && "$(CFG)" !=\
 "Sekaiju - Win32 ReleaseU"
!MESSAGE 指定された ﾋﾞﾙﾄﾞ ﾓｰﾄﾞ "$(CFG)" は正しくありません。
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛ 'CFG' を定義することによって
!MESSAGE NMAKE 実行時にﾋﾞﾙﾄﾞ ﾓｰﾄﾞを指定できます。例えば:
!MESSAGE 
!MESSAGE NMAKE /f "Sekaiju.mak" CFG="Sekaiju - Win32 DebugU"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Sekaiju - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "Sekaiju - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE "Sekaiju - Win32 DebugU" ("Win32 (x86) Application" 用)
!MESSAGE "Sekaiju - Win32 ReleaseU" ("Win32 (x86) Application" 用)
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
# PROP Target_Last_Scanned "Sekaiju - Win32 Debug"
RSC=rc.exe
MTL=mktyplib.exe
CPP=cl.exe

!IF  "$(CFG)" == "Sekaiju - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Sekaiju_"
# PROP BASE Intermediate_Dir "Sekaiju_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\Sekaiju.exe"

CLEAN : 
	-@erase ".\Release\Sekaiju.exe"
	-@erase ".\Release\EditBeatScanDlg.obj"
	-@erase ".\Release\TrackListModeScaleView.obj"
	-@erase ".\Release\FilePropertyDlg.obj"
	-@erase ".\Release\SekaijuDoc.obj"
	-@erase ".\Release\InplaceListBox.obj"
	-@erase ".\Release\PianoRollVelTimeView.obj"
	-@erase ".\Release\PianoRollPrintView.obj"
	-@erase ".\Release\EditVelocityDlg.obj"
	-@erase ".\Release\EditRemoveMeasureDlg.obj"
	-@erase ".\Release\TrackListOption2Page.obj"
	-@erase ".\Release\MetronomeDlg.obj"
	-@erase ".\Release\ChildFrame.obj"
	-@erase ".\Release\MIDIInstDefNormPage.obj"
	-@erase ".\Release\PianoRollKeyTimeView.obj"
	-@erase ".\Release\PianoRollScaleView.obj"
	-@erase ".\Release\EditBreakupAndTrillDlg.obj"
	-@erase ".\Release\SekaijuApp.obj"
	-@erase ".\Release\MIDIOutSyncModePage.obj"
	-@erase ".\Release\EditDurationDlg.obj"
	-@erase ".\Release\ColorfulCheckListBox.obj"
	-@erase ".\Release\HistoryUnit.obj"
	-@erase ".\Release\MusicalScoreTimeScaleView.obj"
	-@erase ".\Release\MIDIInSyncModePage.obj"
	-@erase ".\Release\EditTrackDlg.obj"
	-@erase ".\Release\HistoryRecord.obj"
	-@erase ".\Release\SekaijuToolBar.obj"
	-@erase ".\Release\OptionSheet.obj"
	-@erase ".\Release\EditValueDlg.obj"
	-@erase ".\Release\TrackListPrintView.obj"
	-@erase ".\Release\EventListPropertyScaleView.obj"
	-@erase ".\Release\PianoRollVelScaleView.obj"
	-@erase ".\Release\EditInsertMeasureDlg.obj"
	-@erase ".\Release\TrackListOption1Page.obj"
	-@erase ".\Release\SekaijuDocTemplate.obj"
	-@erase ".\Release\TrackListScaleView.obj"
	-@erase ".\Release\PianoRollOptionPage.obj"
	-@erase ".\Release\AutoSaveDlg.obj"
	-@erase ".\Release\EventListIndexPropertyView.obj"
	-@erase ".\Release\MusicalScoreOptionPage.obj"
	-@erase ".\Release\MusicalScorePrintView.obj"
	-@erase ".\Release\SekaijuView.obj"
	-@erase ".\Release\PianoRollFrame.obj"
	-@erase ".\Release\EventListFrame.obj"
	-@erase ".\Release\TrackListTrackScaleView.obj"
	-@erase ".\Release\EditTimeDlg.obj"
	-@erase ".\Release\MusicalScoreTrackScaleView.obj"
	-@erase ".\Release\MusicalScoreScaleView.obj"
	-@erase ".\Release\common.obj"
	-@erase ".\Release\MIDIInDevicePage.obj"
	-@erase ".\Release\MusicalScoreTrackTimeView.obj"
	-@erase ".\Release\MIDIDeviceSheet.obj"
	-@erase ".\Release\AboutDlg.obj"
	-@erase ".\Release\EventListIndexScaleView.obj"
	-@erase ".\Release\MainFrame.obj"
	-@erase ".\Release\MusicalScoreFrame.obj"
	-@erase ".\Release\GeneralOptionPage.obj"
	-@erase ".\Release\TrackListFrame.obj"
	-@erase ".\Release\EventListOptionPage.obj"
	-@erase ".\Release\EditKeyDlg.obj"
	-@erase ".\Release\ColorfulComboBox.obj"
	-@erase ".\Release\MIDIInstDefDrumPage.obj"
	-@erase ".\Release\EditChannelDlg.obj"
	-@erase ".\Release\TrackListTrackModeView.obj"
	-@erase ".\Release\MIDISyncModeSheet.obj"
	-@erase ".\Release\MIDIOutDevicePage.obj"
	-@erase ".\Release\EditTimeSmpDlg.obj"
	-@erase ".\Release\InplaceEdit.obj"
	-@erase ".\Release\EventListPrintView.obj"
	-@erase ".\Release\PropertyNoteDlg.obj"
	-@erase ".\Release\EditQuantizeDlg.obj"
	-@erase ".\Release\EventListScaleView.obj"
	-@erase ".\Release\TrackListTrackTimeView.obj"
	-@erase ".\Release\LanguageDlg.obj"
	-@erase ".\Release\PianoRollKeyScaleView.obj"
	-@erase ".\Release\SekaijuFileDlg.obj"
	-@erase ".\Release\PianoRollTimeScaleView.obj"
	-@erase ".\Release\SekaijuDocManager.obj"
	-@erase ".\Release\TrackListTimeScaleView.obj"
	-@erase ".\Release\Sekaiju.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /c
CPP_PROJ=/nologo /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL"\
 /D "_MBCS" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/Sekaiju.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Sekaiju.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\MIDIData\Release\MIDIData.lib ..\MIDIClock\Release\MIDIClock.lib ..\MIDIIO\Release\MIDIIO.lib ..\MIDIStatus\Release\MIDIStatus.lib ..\MIDIInstrument\Release\MIDIInstrument.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=..\MIDIData\Release\MIDIData.lib\
 ..\MIDIClock\Release\MIDIClock.lib ..\MIDIIO\Release\MIDIIO.lib\
 ..\MIDIStatus\Release\MIDIStatus.lib\
 ..\MIDIInstrument\Release\MIDIInstrument.lib /nologo /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)/Sekaiju.pdb" /machine:I386\
 /out:"$(OUTDIR)/Sekaiju.exe" 
LINK32_OBJS= \
	"$(INTDIR)/EditBeatScanDlg.obj" \
	"$(INTDIR)/TrackListModeScaleView.obj" \
	"$(INTDIR)/FilePropertyDlg.obj" \
	"$(INTDIR)/SekaijuDoc.obj" \
	"$(INTDIR)/InplaceListBox.obj" \
	"$(INTDIR)/PianoRollVelTimeView.obj" \
	"$(INTDIR)/PianoRollPrintView.obj" \
	"$(INTDIR)/EditVelocityDlg.obj" \
	"$(INTDIR)/EditRemoveMeasureDlg.obj" \
	"$(INTDIR)/TrackListOption2Page.obj" \
	"$(INTDIR)/MetronomeDlg.obj" \
	"$(INTDIR)/ChildFrame.obj" \
	"$(INTDIR)/MIDIInstDefNormPage.obj" \
	"$(INTDIR)/PianoRollKeyTimeView.obj" \
	"$(INTDIR)/PianoRollScaleView.obj" \
	"$(INTDIR)/EditBreakupAndTrillDlg.obj" \
	"$(INTDIR)/SekaijuApp.obj" \
	"$(INTDIR)/MIDIOutSyncModePage.obj" \
	"$(INTDIR)/EditDurationDlg.obj" \
	"$(INTDIR)/ColorfulCheckListBox.obj" \
	"$(INTDIR)/HistoryUnit.obj" \
	"$(INTDIR)/MusicalScoreTimeScaleView.obj" \
	"$(INTDIR)/MIDIInSyncModePage.obj" \
	"$(INTDIR)/EditTrackDlg.obj" \
	"$(INTDIR)/HistoryRecord.obj" \
	"$(INTDIR)/SekaijuToolBar.obj" \
	"$(INTDIR)/OptionSheet.obj" \
	"$(INTDIR)/EditValueDlg.obj" \
	"$(INTDIR)/TrackListPrintView.obj" \
	"$(INTDIR)/EventListPropertyScaleView.obj" \
	"$(INTDIR)/PianoRollVelScaleView.obj" \
	"$(INTDIR)/EditInsertMeasureDlg.obj" \
	"$(INTDIR)/TrackListOption1Page.obj" \
	"$(INTDIR)/SekaijuDocTemplate.obj" \
	"$(INTDIR)/TrackListScaleView.obj" \
	"$(INTDIR)/PianoRollOptionPage.obj" \
	"$(INTDIR)/AutoSaveDlg.obj" \
	"$(INTDIR)/EventListIndexPropertyView.obj" \
	"$(INTDIR)/MusicalScoreOptionPage.obj" \
	"$(INTDIR)/MusicalScorePrintView.obj" \
	"$(INTDIR)/SekaijuView.obj" \
	"$(INTDIR)/PianoRollFrame.obj" \
	"$(INTDIR)/EventListFrame.obj" \
	"$(INTDIR)/TrackListTrackScaleView.obj" \
	"$(INTDIR)/EditTimeDlg.obj" \
	"$(INTDIR)/MusicalScoreTrackScaleView.obj" \
	"$(INTDIR)/MusicalScoreScaleView.obj" \
	"$(INTDIR)/common.obj" \
	"$(INTDIR)/MIDIInDevicePage.obj" \
	"$(INTDIR)/MusicalScoreTrackTimeView.obj" \
	"$(INTDIR)/MIDIDeviceSheet.obj" \
	"$(INTDIR)/AboutDlg.obj" \
	"$(INTDIR)/EventListIndexScaleView.obj" \
	"$(INTDIR)/MainFrame.obj" \
	"$(INTDIR)/MusicalScoreFrame.obj" \
	"$(INTDIR)/GeneralOptionPage.obj" \
	"$(INTDIR)/TrackListFrame.obj" \
	"$(INTDIR)/EventListOptionPage.obj" \
	"$(INTDIR)/EditKeyDlg.obj" \
	"$(INTDIR)/ColorfulComboBox.obj" \
	"$(INTDIR)/MIDIInstDefDrumPage.obj" \
	"$(INTDIR)/EditChannelDlg.obj" \
	"$(INTDIR)/TrackListTrackModeView.obj" \
	"$(INTDIR)/MIDISyncModeSheet.obj" \
	"$(INTDIR)/MIDIOutDevicePage.obj" \
	"$(INTDIR)/EditTimeSmpDlg.obj" \
	"$(INTDIR)/InplaceEdit.obj" \
	"$(INTDIR)/EventListPrintView.obj" \
	"$(INTDIR)/PropertyNoteDlg.obj" \
	"$(INTDIR)/EditQuantizeDlg.obj" \
	"$(INTDIR)/EventListScaleView.obj" \
	"$(INTDIR)/TrackListTrackTimeView.obj" \
	"$(INTDIR)/LanguageDlg.obj" \
	"$(INTDIR)/PianoRollKeyScaleView.obj" \
	"$(INTDIR)/SekaijuFileDlg.obj" \
	"$(INTDIR)/PianoRollTimeScaleView.obj" \
	"$(INTDIR)/SekaijuDocManager.obj" \
	"$(INTDIR)/TrackListTimeScaleView.obj" \
	"$(INTDIR)/Sekaiju.res"

"$(OUTDIR)\Sekaiju.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Sekaiju0"
# PROP BASE Intermediate_Dir "Sekaiju0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\Sekaiju.exe" "$(OUTDIR)\Sekaiju.bsc"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\Sekaiju.bsc"
	-@erase ".\Debug\OptionSheet.sbr"
	-@erase ".\Debug\InplaceEdit.sbr"
	-@erase ".\Debug\ColorfulComboBox.sbr"
	-@erase ".\Debug\TrackListTrackScaleView.sbr"
	-@erase ".\Debug\PropertyNoteDlg.sbr"
	-@erase ".\Debug\MIDIInSyncModePage.sbr"
	-@erase ".\Debug\EditQuantizeDlg.sbr"
	-@erase ".\Debug\MIDIInstDefNormPage.sbr"
	-@erase ".\Debug\AutoSaveDlg.sbr"
	-@erase ".\Debug\EventListPrintView.sbr"
	-@erase ".\Debug\TrackListFrame.sbr"
	-@erase ".\Debug\TrackListPrintView.sbr"
	-@erase ".\Debug\TrackListTrackModeView.sbr"
	-@erase ".\Debug\MIDIOutSyncModePage.sbr"
	-@erase ".\Debug\EventListIndexPropertyView.sbr"
	-@erase ".\Debug\EditTimeSmpDlg.sbr"
	-@erase ".\Debug\EventListScaleView.sbr"
	-@erase ".\Debug\TrackListScaleView.sbr"
	-@erase ".\Debug\PianoRollVelTimeView.sbr"
	-@erase ".\Debug\TrackListOption2Page.sbr"
	-@erase ".\Debug\MetronomeDlg.sbr"
	-@erase ".\Debug\MIDIDeviceSheet.sbr"
	-@erase ".\Debug\FilePropertyDlg.sbr"
	-@erase ".\Debug\PianoRollKeyTimeView.sbr"
	-@erase ".\Debug\EditVelocityDlg.sbr"
	-@erase ".\Debug\TrackListTrackTimeView.sbr"
	-@erase ".\Debug\PianoRollTimeScaleView.sbr"
	-@erase ".\Debug\MainFrame.sbr"
	-@erase ".\Debug\ColorfulCheckListBox.sbr"
	-@erase ".\Debug\PianoRollOptionPage.sbr"
	-@erase ".\Debug\TrackListTimeScaleView.sbr"
	-@erase ".\Debug\EditTrackDlg.sbr"
	-@erase ".\Debug\SekaijuDoc.sbr"
	-@erase ".\Debug\HistoryUnit.sbr"
	-@erase ".\Debug\MusicalScoreTimeScaleView.sbr"
	-@erase ".\Debug\EditDurationDlg.sbr"
	-@erase ".\Debug\TrackListModeScaleView.sbr"
	-@erase ".\Debug\InplaceListBox.sbr"
	-@erase ".\Debug\ChildFrame.sbr"
	-@erase ".\Debug\EventListFrame.sbr"
	-@erase ".\Debug\EditValueDlg.sbr"
	-@erase ".\Debug\EditKeyDlg.sbr"
	-@erase ".\Debug\EventListPropertyScaleView.sbr"
	-@erase ".\Debug\LanguageDlg.sbr"
	-@erase ".\Debug\EditInsertMeasureDlg.sbr"
	-@erase ".\Debug\SekaijuApp.sbr"
	-@erase ".\Debug\TrackListOption1Page.sbr"
	-@erase ".\Debug\EventListIndexScaleView.sbr"
	-@erase ".\Debug\MusicalScoreFrame.sbr"
	-@erase ".\Debug\GeneralOptionPage.sbr"
	-@erase ".\Debug\PianoRollVelScaleView.sbr"
	-@erase ".\Debug\EditBreakupAndTrillDlg.sbr"
	-@erase ".\Debug\MIDISyncModeSheet.sbr"
	-@erase ".\Debug\EventListOptionPage.sbr"
	-@erase ".\Debug\MIDIInstDefDrumPage.sbr"
	-@erase ".\Debug\HistoryRecord.sbr"
	-@erase ".\Debug\MIDIOutDevicePage.sbr"
	-@erase ".\Debug\EditChannelDlg.sbr"
	-@erase ".\Debug\EditBeatScanDlg.sbr"
	-@erase ".\Debug\PianoRollKeyScaleView.sbr"
	-@erase ".\Debug\SekaijuView.sbr"
	-@erase ".\Debug\MusicalScoreTrackScaleView.sbr"
	-@erase ".\Debug\MusicalScorePrintView.sbr"
	-@erase ".\Debug\AboutDlg.sbr"
	-@erase ".\Debug\EditTimeDlg.sbr"
	-@erase ".\Debug\SekaijuToolBar.sbr"
	-@erase ".\Debug\SekaijuDocTemplate.sbr"
	-@erase ".\Debug\EditRemoveMeasureDlg.sbr"
	-@erase ".\Debug\MusicalScoreTrackTimeView.sbr"
	-@erase ".\Debug\MusicalScoreScaleView.sbr"
	-@erase ".\Debug\common.sbr"
	-@erase ".\Debug\SekaijuDocManager.sbr"
	-@erase ".\Debug\PianoRollPrintView.sbr"
	-@erase ".\Debug\MIDIInDevicePage.sbr"
	-@erase ".\Debug\SekaijuFileDlg.sbr"
	-@erase ".\Debug\MusicalScoreOptionPage.sbr"
	-@erase ".\Debug\PianoRollFrame.sbr"
	-@erase ".\Debug\PianoRollScaleView.sbr"
	-@erase ".\Debug\Sekaiju.exe"
	-@erase ".\Debug\SekaijuDocTemplate.obj"
	-@erase ".\Debug\EditRemoveMeasureDlg.obj"
	-@erase ".\Debug\MusicalScoreTrackTimeView.obj"
	-@erase ".\Debug\MusicalScoreScaleView.obj"
	-@erase ".\Debug\common.obj"
	-@erase ".\Debug\SekaijuDocManager.obj"
	-@erase ".\Debug\PianoRollPrintView.obj"
	-@erase ".\Debug\MIDIInDevicePage.obj"
	-@erase ".\Debug\SekaijuFileDlg.obj"
	-@erase ".\Debug\MusicalScoreOptionPage.obj"
	-@erase ".\Debug\PianoRollFrame.obj"
	-@erase ".\Debug\PianoRollScaleView.obj"
	-@erase ".\Debug\OptionSheet.obj"
	-@erase ".\Debug\InplaceEdit.obj"
	-@erase ".\Debug\ColorfulComboBox.obj"
	-@erase ".\Debug\TrackListTrackScaleView.obj"
	-@erase ".\Debug\PropertyNoteDlg.obj"
	-@erase ".\Debug\MIDIInSyncModePage.obj"
	-@erase ".\Debug\EditQuantizeDlg.obj"
	-@erase ".\Debug\MIDIInstDefNormPage.obj"
	-@erase ".\Debug\AutoSaveDlg.obj"
	-@erase ".\Debug\EventListPrintView.obj"
	-@erase ".\Debug\TrackListFrame.obj"
	-@erase ".\Debug\TrackListPrintView.obj"
	-@erase ".\Debug\TrackListTrackModeView.obj"
	-@erase ".\Debug\MIDIOutSyncModePage.obj"
	-@erase ".\Debug\EventListIndexPropertyView.obj"
	-@erase ".\Debug\EditTimeSmpDlg.obj"
	-@erase ".\Debug\EventListScaleView.obj"
	-@erase ".\Debug\TrackListScaleView.obj"
	-@erase ".\Debug\PianoRollVelTimeView.obj"
	-@erase ".\Debug\TrackListOption2Page.obj"
	-@erase ".\Debug\MetronomeDlg.obj"
	-@erase ".\Debug\MIDIDeviceSheet.obj"
	-@erase ".\Debug\FilePropertyDlg.obj"
	-@erase ".\Debug\PianoRollKeyTimeView.obj"
	-@erase ".\Debug\EditVelocityDlg.obj"
	-@erase ".\Debug\TrackListTrackTimeView.obj"
	-@erase ".\Debug\PianoRollTimeScaleView.obj"
	-@erase ".\Debug\MainFrame.obj"
	-@erase ".\Debug\ColorfulCheckListBox.obj"
	-@erase ".\Debug\PianoRollOptionPage.obj"
	-@erase ".\Debug\TrackListTimeScaleView.obj"
	-@erase ".\Debug\EditTrackDlg.obj"
	-@erase ".\Debug\SekaijuDoc.obj"
	-@erase ".\Debug\HistoryUnit.obj"
	-@erase ".\Debug\MusicalScoreTimeScaleView.obj"
	-@erase ".\Debug\EditDurationDlg.obj"
	-@erase ".\Debug\TrackListModeScaleView.obj"
	-@erase ".\Debug\InplaceListBox.obj"
	-@erase ".\Debug\ChildFrame.obj"
	-@erase ".\Debug\EventListFrame.obj"
	-@erase ".\Debug\EditValueDlg.obj"
	-@erase ".\Debug\EditKeyDlg.obj"
	-@erase ".\Debug\EventListPropertyScaleView.obj"
	-@erase ".\Debug\LanguageDlg.obj"
	-@erase ".\Debug\EditInsertMeasureDlg.obj"
	-@erase ".\Debug\SekaijuApp.obj"
	-@erase ".\Debug\TrackListOption1Page.obj"
	-@erase ".\Debug\EventListIndexScaleView.obj"
	-@erase ".\Debug\MusicalScoreFrame.obj"
	-@erase ".\Debug\GeneralOptionPage.obj"
	-@erase ".\Debug\PianoRollVelScaleView.obj"
	-@erase ".\Debug\EditBreakupAndTrillDlg.obj"
	-@erase ".\Debug\MIDISyncModeSheet.obj"
	-@erase ".\Debug\EventListOptionPage.obj"
	-@erase ".\Debug\MIDIInstDefDrumPage.obj"
	-@erase ".\Debug\HistoryRecord.obj"
	-@erase ".\Debug\MIDIOutDevicePage.obj"
	-@erase ".\Debug\EditChannelDlg.obj"
	-@erase ".\Debug\EditBeatScanDlg.obj"
	-@erase ".\Debug\PianoRollKeyScaleView.obj"
	-@erase ".\Debug\SekaijuView.obj"
	-@erase ".\Debug\MusicalScoreTrackScaleView.obj"
	-@erase ".\Debug\MusicalScorePrintView.obj"
	-@erase ".\Debug\AboutDlg.obj"
	-@erase ".\Debug\EditTimeDlg.obj"
	-@erase ".\Debug\SekaijuToolBar.obj"
	-@erase ".\Debug\Sekaiju.res"
	-@erase ".\Debug\Sekaiju.ilk"
	-@erase ".\Debug\Sekaiju.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /FR"$(INTDIR)/" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/Sekaiju.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Sekaiju.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/OptionSheet.sbr" \
	"$(INTDIR)/InplaceEdit.sbr" \
	"$(INTDIR)/ColorfulComboBox.sbr" \
	"$(INTDIR)/TrackListTrackScaleView.sbr" \
	"$(INTDIR)/PropertyNoteDlg.sbr" \
	"$(INTDIR)/MIDIInSyncModePage.sbr" \
	"$(INTDIR)/EditQuantizeDlg.sbr" \
	"$(INTDIR)/MIDIInstDefNormPage.sbr" \
	"$(INTDIR)/AutoSaveDlg.sbr" \
	"$(INTDIR)/EventListPrintView.sbr" \
	"$(INTDIR)/TrackListFrame.sbr" \
	"$(INTDIR)/TrackListPrintView.sbr" \
	"$(INTDIR)/TrackListTrackModeView.sbr" \
	"$(INTDIR)/MIDIOutSyncModePage.sbr" \
	"$(INTDIR)/EventListIndexPropertyView.sbr" \
	"$(INTDIR)/EditTimeSmpDlg.sbr" \
	"$(INTDIR)/EventListScaleView.sbr" \
	"$(INTDIR)/TrackListScaleView.sbr" \
	"$(INTDIR)/PianoRollVelTimeView.sbr" \
	"$(INTDIR)/TrackListOption2Page.sbr" \
	"$(INTDIR)/MetronomeDlg.sbr" \
	"$(INTDIR)/MIDIDeviceSheet.sbr" \
	"$(INTDIR)/FilePropertyDlg.sbr" \
	"$(INTDIR)/PianoRollKeyTimeView.sbr" \
	"$(INTDIR)/EditVelocityDlg.sbr" \
	"$(INTDIR)/TrackListTrackTimeView.sbr" \
	"$(INTDIR)/PianoRollTimeScaleView.sbr" \
	"$(INTDIR)/MainFrame.sbr" \
	"$(INTDIR)/ColorfulCheckListBox.sbr" \
	"$(INTDIR)/PianoRollOptionPage.sbr" \
	"$(INTDIR)/TrackListTimeScaleView.sbr" \
	"$(INTDIR)/EditTrackDlg.sbr" \
	"$(INTDIR)/SekaijuDoc.sbr" \
	"$(INTDIR)/HistoryUnit.sbr" \
	"$(INTDIR)/MusicalScoreTimeScaleView.sbr" \
	"$(INTDIR)/EditDurationDlg.sbr" \
	"$(INTDIR)/TrackListModeScaleView.sbr" \
	"$(INTDIR)/InplaceListBox.sbr" \
	"$(INTDIR)/ChildFrame.sbr" \
	"$(INTDIR)/EventListFrame.sbr" \
	"$(INTDIR)/EditValueDlg.sbr" \
	"$(INTDIR)/EditKeyDlg.sbr" \
	"$(INTDIR)/EventListPropertyScaleView.sbr" \
	"$(INTDIR)/LanguageDlg.sbr" \
	"$(INTDIR)/EditInsertMeasureDlg.sbr" \
	"$(INTDIR)/SekaijuApp.sbr" \
	"$(INTDIR)/TrackListOption1Page.sbr" \
	"$(INTDIR)/EventListIndexScaleView.sbr" \
	"$(INTDIR)/MusicalScoreFrame.sbr" \
	"$(INTDIR)/GeneralOptionPage.sbr" \
	"$(INTDIR)/PianoRollVelScaleView.sbr" \
	"$(INTDIR)/EditBreakupAndTrillDlg.sbr" \
	"$(INTDIR)/MIDISyncModeSheet.sbr" \
	"$(INTDIR)/EventListOptionPage.sbr" \
	"$(INTDIR)/MIDIInstDefDrumPage.sbr" \
	"$(INTDIR)/HistoryRecord.sbr" \
	"$(INTDIR)/MIDIOutDevicePage.sbr" \
	"$(INTDIR)/EditChannelDlg.sbr" \
	"$(INTDIR)/EditBeatScanDlg.sbr" \
	"$(INTDIR)/PianoRollKeyScaleView.sbr" \
	"$(INTDIR)/SekaijuView.sbr" \
	"$(INTDIR)/MusicalScoreTrackScaleView.sbr" \
	"$(INTDIR)/MusicalScorePrintView.sbr" \
	"$(INTDIR)/AboutDlg.sbr" \
	"$(INTDIR)/EditTimeDlg.sbr" \
	"$(INTDIR)/SekaijuToolBar.sbr" \
	"$(INTDIR)/SekaijuDocTemplate.sbr" \
	"$(INTDIR)/EditRemoveMeasureDlg.sbr" \
	"$(INTDIR)/MusicalScoreTrackTimeView.sbr" \
	"$(INTDIR)/MusicalScoreScaleView.sbr" \
	"$(INTDIR)/common.sbr" \
	"$(INTDIR)/SekaijuDocManager.sbr" \
	"$(INTDIR)/PianoRollPrintView.sbr" \
	"$(INTDIR)/MIDIInDevicePage.sbr" \
	"$(INTDIR)/SekaijuFileDlg.sbr" \
	"$(INTDIR)/MusicalScoreOptionPage.sbr" \
	"$(INTDIR)/PianoRollFrame.sbr" \
	"$(INTDIR)/PianoRollScaleView.sbr"

"$(OUTDIR)\Sekaiju.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 ole32.lib ..\MIDIData\Debug\MIDIDatad.lib ..\MIDIClock\Debug\MIDIClockd.lib ..\MIDIIO\Debug\MIDIIOd.lib ..\MIDIStatus\Debug\MIDIStatusd.lib ..\MIDIInstrument\Debug\MIDIInstrumentd.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=ole32.lib ..\MIDIData\Debug\MIDIDatad.lib\
 ..\MIDIClock\Debug\MIDIClockd.lib ..\MIDIIO\Debug\MIDIIOd.lib\
 ..\MIDIStatus\Debug\MIDIStatusd.lib ..\MIDIInstrument\Debug\MIDIInstrumentd.lib\
 /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)/Sekaiju.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/Sekaiju.exe" 
LINK32_OBJS= \
	"$(INTDIR)/SekaijuDocTemplate.obj" \
	"$(INTDIR)/EditRemoveMeasureDlg.obj" \
	"$(INTDIR)/MusicalScoreTrackTimeView.obj" \
	"$(INTDIR)/MusicalScoreScaleView.obj" \
	"$(INTDIR)/common.obj" \
	"$(INTDIR)/SekaijuDocManager.obj" \
	"$(INTDIR)/PianoRollPrintView.obj" \
	"$(INTDIR)/MIDIInDevicePage.obj" \
	"$(INTDIR)/SekaijuFileDlg.obj" \
	"$(INTDIR)/MusicalScoreOptionPage.obj" \
	"$(INTDIR)/PianoRollFrame.obj" \
	"$(INTDIR)/PianoRollScaleView.obj" \
	"$(INTDIR)/OptionSheet.obj" \
	"$(INTDIR)/InplaceEdit.obj" \
	"$(INTDIR)/ColorfulComboBox.obj" \
	"$(INTDIR)/TrackListTrackScaleView.obj" \
	"$(INTDIR)/PropertyNoteDlg.obj" \
	"$(INTDIR)/MIDIInSyncModePage.obj" \
	"$(INTDIR)/EditQuantizeDlg.obj" \
	"$(INTDIR)/MIDIInstDefNormPage.obj" \
	"$(INTDIR)/AutoSaveDlg.obj" \
	"$(INTDIR)/EventListPrintView.obj" \
	"$(INTDIR)/TrackListFrame.obj" \
	"$(INTDIR)/TrackListPrintView.obj" \
	"$(INTDIR)/TrackListTrackModeView.obj" \
	"$(INTDIR)/MIDIOutSyncModePage.obj" \
	"$(INTDIR)/EventListIndexPropertyView.obj" \
	"$(INTDIR)/EditTimeSmpDlg.obj" \
	"$(INTDIR)/EventListScaleView.obj" \
	"$(INTDIR)/TrackListScaleView.obj" \
	"$(INTDIR)/PianoRollVelTimeView.obj" \
	"$(INTDIR)/TrackListOption2Page.obj" \
	"$(INTDIR)/MetronomeDlg.obj" \
	"$(INTDIR)/MIDIDeviceSheet.obj" \
	"$(INTDIR)/FilePropertyDlg.obj" \
	"$(INTDIR)/PianoRollKeyTimeView.obj" \
	"$(INTDIR)/EditVelocityDlg.obj" \
	"$(INTDIR)/TrackListTrackTimeView.obj" \
	"$(INTDIR)/PianoRollTimeScaleView.obj" \
	"$(INTDIR)/MainFrame.obj" \
	"$(INTDIR)/ColorfulCheckListBox.obj" \
	"$(INTDIR)/PianoRollOptionPage.obj" \
	"$(INTDIR)/TrackListTimeScaleView.obj" \
	"$(INTDIR)/EditTrackDlg.obj" \
	"$(INTDIR)/SekaijuDoc.obj" \
	"$(INTDIR)/HistoryUnit.obj" \
	"$(INTDIR)/MusicalScoreTimeScaleView.obj" \
	"$(INTDIR)/EditDurationDlg.obj" \
	"$(INTDIR)/TrackListModeScaleView.obj" \
	"$(INTDIR)/InplaceListBox.obj" \
	"$(INTDIR)/ChildFrame.obj" \
	"$(INTDIR)/EventListFrame.obj" \
	"$(INTDIR)/EditValueDlg.obj" \
	"$(INTDIR)/EditKeyDlg.obj" \
	"$(INTDIR)/EventListPropertyScaleView.obj" \
	"$(INTDIR)/LanguageDlg.obj" \
	"$(INTDIR)/EditInsertMeasureDlg.obj" \
	"$(INTDIR)/SekaijuApp.obj" \
	"$(INTDIR)/TrackListOption1Page.obj" \
	"$(INTDIR)/EventListIndexScaleView.obj" \
	"$(INTDIR)/MusicalScoreFrame.obj" \
	"$(INTDIR)/GeneralOptionPage.obj" \
	"$(INTDIR)/PianoRollVelScaleView.obj" \
	"$(INTDIR)/EditBreakupAndTrillDlg.obj" \
	"$(INTDIR)/MIDISyncModeSheet.obj" \
	"$(INTDIR)/EventListOptionPage.obj" \
	"$(INTDIR)/MIDIInstDefDrumPage.obj" \
	"$(INTDIR)/HistoryRecord.obj" \
	"$(INTDIR)/MIDIOutDevicePage.obj" \
	"$(INTDIR)/EditChannelDlg.obj" \
	"$(INTDIR)/EditBeatScanDlg.obj" \
	"$(INTDIR)/PianoRollKeyScaleView.obj" \
	"$(INTDIR)/SekaijuView.obj" \
	"$(INTDIR)/MusicalScoreTrackScaleView.obj" \
	"$(INTDIR)/MusicalScorePrintView.obj" \
	"$(INTDIR)/AboutDlg.obj" \
	"$(INTDIR)/EditTimeDlg.obj" \
	"$(INTDIR)/SekaijuToolBar.obj" \
	"$(INTDIR)/Sekaiju.res"

"$(OUTDIR)\Sekaiju.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Sekaiju_"
# PROP BASE Intermediate_Dir "Sekaiju_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Target_Dir ""
OUTDIR=.\DebugU
INTDIR=.\DebugU

ALL : "$(OUTDIR)\Sekaiju.exe" "$(OUTDIR)\Sekaiju.bsc"

CLEAN : 
	-@erase ".\DebugU\vc40.pdb"
	-@erase ".\DebugU\vc40.idb"
	-@erase ".\DebugU\Sekaiju.bsc"
	-@erase ".\DebugU\TrackListTimeScaleView.sbr"
	-@erase ".\DebugU\common.sbr"
	-@erase ".\DebugU\MainFrame.sbr"
	-@erase ".\DebugU\MIDIDeviceSheet.sbr"
	-@erase ".\DebugU\MIDIInstDefNormPage.sbr"
	-@erase ".\DebugU\EventListIndexPropertyView.sbr"
	-@erase ".\DebugU\TrackListModeScaleView.sbr"
	-@erase ".\DebugU\EditInsertMeasureDlg.sbr"
	-@erase ".\DebugU\TrackListOption1Page.sbr"
	-@erase ".\DebugU\HistoryUnit.sbr"
	-@erase ".\DebugU\PianoRollFrame.sbr"
	-@erase ".\DebugU\EventListOptionPage.sbr"
	-@erase ".\DebugU\EventListFrame.sbr"
	-@erase ".\DebugU\MIDIInstDefDrumPage.sbr"
	-@erase ".\DebugU\OptionSheet.sbr"
	-@erase ".\DebugU\EditBreakupAndTrillDlg.sbr"
	-@erase ".\DebugU\PropertyNoteDlg.sbr"
	-@erase ".\DebugU\PianoRollVelScaleView.sbr"
	-@erase ".\DebugU\EditQuantizeDlg.sbr"
	-@erase ".\DebugU\MIDIInDevicePage.sbr"
	-@erase ".\DebugU\AutoSaveDlg.sbr"
	-@erase ".\DebugU\EditRemoveMeasureDlg.sbr"
	-@erase ".\DebugU\TrackListOption2Page.sbr"
	-@erase ".\DebugU\EditChannelDlg.sbr"
	-@erase ".\DebugU\MusicalScoreTrackTimeView.sbr"
	-@erase ".\DebugU\EventListPropertyScaleView.sbr"
	-@erase ".\DebugU\EventListIndexScaleView.sbr"
	-@erase ".\DebugU\SekaijuDocTemplate.sbr"
	-@erase ".\DebugU\EditTimeDlg.sbr"
	-@erase ".\DebugU\SekaijuToolBar.sbr"
	-@erase ".\DebugU\MusicalScoreTimeScaleView.sbr"
	-@erase ".\DebugU\ColorfulCheckListBox.sbr"
	-@erase ".\DebugU\MusicalScoreOptionPage.sbr"
	-@erase ".\DebugU\SekaijuDoc.sbr"
	-@erase ".\DebugU\FilePropertyDlg.sbr"
	-@erase ".\DebugU\PianoRollPrintView.sbr"
	-@erase ".\DebugU\EditVelocityDlg.sbr"
	-@erase ".\DebugU\ChildFrame.sbr"
	-@erase ".\DebugU\SekaijuFileDlg.sbr"
	-@erase ".\DebugU\EditKeyDlg.sbr"
	-@erase ".\DebugU\PianoRollScaleView.sbr"
	-@erase ".\DebugU\MusicalScoreTrackScaleView.sbr"
	-@erase ".\DebugU\SekaijuApp.sbr"
	-@erase ".\DebugU\InplaceEdit.sbr"
	-@erase ".\DebugU\EditDurationDlg.sbr"
	-@erase ".\DebugU\MIDIOutSyncModePage.sbr"
	-@erase ".\DebugU\MIDIInSyncModePage.sbr"
	-@erase ".\DebugU\InplaceListBox.sbr"
	-@erase ".\DebugU\MetronomeDlg.sbr"
	-@erase ".\DebugU\TrackListTrackModeView.sbr"
	-@erase ".\DebugU\MusicalScoreFrame.sbr"
	-@erase ".\DebugU\GeneralOptionPage.sbr"
	-@erase ".\DebugU\EventListPrintView.sbr"
	-@erase ".\DebugU\LanguageDlg.sbr"
	-@erase ".\DebugU\PianoRollKeyScaleView.sbr"
	-@erase ".\DebugU\TrackListPrintView.sbr"
	-@erase ".\DebugU\MIDISyncModeSheet.sbr"
	-@erase ".\DebugU\PianoRollVelTimeView.sbr"
	-@erase ".\DebugU\HistoryRecord.sbr"
	-@erase ".\DebugU\TrackListFrame.sbr"
	-@erase ".\DebugU\MusicalScorePrintView.sbr"
	-@erase ".\DebugU\MIDIOutDevicePage.sbr"
	-@erase ".\DebugU\TrackListTrackScaleView.sbr"
	-@erase ".\DebugU\EventListScaleView.sbr"
	-@erase ".\DebugU\PianoRollKeyTimeView.sbr"
	-@erase ".\DebugU\TrackListScaleView.sbr"
	-@erase ".\DebugU\EditTimeSmpDlg.sbr"
	-@erase ".\DebugU\AboutDlg.sbr"
	-@erase ".\DebugU\MusicalScoreScaleView.sbr"
	-@erase ".\DebugU\EditBeatScanDlg.sbr"
	-@erase ".\DebugU\EditTrackDlg.sbr"
	-@erase ".\DebugU\PianoRollOptionPage.sbr"
	-@erase ".\DebugU\TrackListTrackTimeView.sbr"
	-@erase ".\DebugU\PianoRollTimeScaleView.sbr"
	-@erase ".\DebugU\SekaijuView.sbr"
	-@erase ".\DebugU\ColorfulComboBox.sbr"
	-@erase ".\DebugU\EditValueDlg.sbr"
	-@erase ".\DebugU\SekaijuDocManager.sbr"
	-@erase ".\DebugU\Sekaiju.exe"
	-@erase ".\DebugU\HistoryRecord.obj"
	-@erase ".\DebugU\TrackListFrame.obj"
	-@erase ".\DebugU\MusicalScorePrintView.obj"
	-@erase ".\DebugU\MIDIOutDevicePage.obj"
	-@erase ".\DebugU\TrackListTrackScaleView.obj"
	-@erase ".\DebugU\EventListScaleView.obj"
	-@erase ".\DebugU\PianoRollKeyTimeView.obj"
	-@erase ".\DebugU\TrackListScaleView.obj"
	-@erase ".\DebugU\EditTimeSmpDlg.obj"
	-@erase ".\DebugU\AboutDlg.obj"
	-@erase ".\DebugU\MusicalScoreScaleView.obj"
	-@erase ".\DebugU\EditBeatScanDlg.obj"
	-@erase ".\DebugU\EditTrackDlg.obj"
	-@erase ".\DebugU\PianoRollOptionPage.obj"
	-@erase ".\DebugU\TrackListTrackTimeView.obj"
	-@erase ".\DebugU\PianoRollTimeScaleView.obj"
	-@erase ".\DebugU\SekaijuView.obj"
	-@erase ".\DebugU\ColorfulComboBox.obj"
	-@erase ".\DebugU\EditValueDlg.obj"
	-@erase ".\DebugU\SekaijuDocManager.obj"
	-@erase ".\DebugU\TrackListTimeScaleView.obj"
	-@erase ".\DebugU\common.obj"
	-@erase ".\DebugU\MainFrame.obj"
	-@erase ".\DebugU\MIDIDeviceSheet.obj"
	-@erase ".\DebugU\MIDIInstDefNormPage.obj"
	-@erase ".\DebugU\EventListIndexPropertyView.obj"
	-@erase ".\DebugU\TrackListModeScaleView.obj"
	-@erase ".\DebugU\EditInsertMeasureDlg.obj"
	-@erase ".\DebugU\TrackListOption1Page.obj"
	-@erase ".\DebugU\HistoryUnit.obj"
	-@erase ".\DebugU\PianoRollFrame.obj"
	-@erase ".\DebugU\EventListOptionPage.obj"
	-@erase ".\DebugU\EventListFrame.obj"
	-@erase ".\DebugU\MIDIInstDefDrumPage.obj"
	-@erase ".\DebugU\OptionSheet.obj"
	-@erase ".\DebugU\EditBreakupAndTrillDlg.obj"
	-@erase ".\DebugU\PropertyNoteDlg.obj"
	-@erase ".\DebugU\PianoRollVelScaleView.obj"
	-@erase ".\DebugU\EditQuantizeDlg.obj"
	-@erase ".\DebugU\MIDIInDevicePage.obj"
	-@erase ".\DebugU\AutoSaveDlg.obj"
	-@erase ".\DebugU\EditRemoveMeasureDlg.obj"
	-@erase ".\DebugU\TrackListOption2Page.obj"
	-@erase ".\DebugU\EditChannelDlg.obj"
	-@erase ".\DebugU\MusicalScoreTrackTimeView.obj"
	-@erase ".\DebugU\EventListPropertyScaleView.obj"
	-@erase ".\DebugU\EventListIndexScaleView.obj"
	-@erase ".\DebugU\SekaijuDocTemplate.obj"
	-@erase ".\DebugU\EditTimeDlg.obj"
	-@erase ".\DebugU\SekaijuToolBar.obj"
	-@erase ".\DebugU\MusicalScoreTimeScaleView.obj"
	-@erase ".\DebugU\ColorfulCheckListBox.obj"
	-@erase ".\DebugU\MusicalScoreOptionPage.obj"
	-@erase ".\DebugU\SekaijuDoc.obj"
	-@erase ".\DebugU\FilePropertyDlg.obj"
	-@erase ".\DebugU\PianoRollPrintView.obj"
	-@erase ".\DebugU\EditVelocityDlg.obj"
	-@erase ".\DebugU\ChildFrame.obj"
	-@erase ".\DebugU\SekaijuFileDlg.obj"
	-@erase ".\DebugU\EditKeyDlg.obj"
	-@erase ".\DebugU\PianoRollScaleView.obj"
	-@erase ".\DebugU\MusicalScoreTrackScaleView.obj"
	-@erase ".\DebugU\SekaijuApp.obj"
	-@erase ".\DebugU\InplaceEdit.obj"
	-@erase ".\DebugU\EditDurationDlg.obj"
	-@erase ".\DebugU\MIDIOutSyncModePage.obj"
	-@erase ".\DebugU\MIDIInSyncModePage.obj"
	-@erase ".\DebugU\InplaceListBox.obj"
	-@erase ".\DebugU\MetronomeDlg.obj"
	-@erase ".\DebugU\TrackListTrackModeView.obj"
	-@erase ".\DebugU\MusicalScoreFrame.obj"
	-@erase ".\DebugU\GeneralOptionPage.obj"
	-@erase ".\DebugU\EventListPrintView.obj"
	-@erase ".\DebugU\LanguageDlg.obj"
	-@erase ".\DebugU\PianoRollKeyScaleView.obj"
	-@erase ".\DebugU\TrackListPrintView.obj"
	-@erase ".\DebugU\MIDISyncModeSheet.obj"
	-@erase ".\DebugU\PianoRollVelTimeView.obj"
	-@erase ".\DebugU\Sekaiju.res"
	-@erase ".\DebugU\Sekaiju.ilk"
	-@erase ".\DebugU\Sekaiju.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FR /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FR"$(INTDIR)/" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\DebugU/
CPP_SBRS=.\DebugU/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL" /d "_UNICODE" /d "UNICODE"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/Sekaiju.res" /d "_DEBUG" /d "_AFXDLL" /d\
 "_UNICODE" /d "UNICODE" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Sekaiju.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/TrackListTimeScaleView.sbr" \
	"$(INTDIR)/common.sbr" \
	"$(INTDIR)/MainFrame.sbr" \
	"$(INTDIR)/MIDIDeviceSheet.sbr" \
	"$(INTDIR)/MIDIInstDefNormPage.sbr" \
	"$(INTDIR)/EventListIndexPropertyView.sbr" \
	"$(INTDIR)/TrackListModeScaleView.sbr" \
	"$(INTDIR)/EditInsertMeasureDlg.sbr" \
	"$(INTDIR)/TrackListOption1Page.sbr" \
	"$(INTDIR)/HistoryUnit.sbr" \
	"$(INTDIR)/PianoRollFrame.sbr" \
	"$(INTDIR)/EventListOptionPage.sbr" \
	"$(INTDIR)/EventListFrame.sbr" \
	"$(INTDIR)/MIDIInstDefDrumPage.sbr" \
	"$(INTDIR)/OptionSheet.sbr" \
	"$(INTDIR)/EditBreakupAndTrillDlg.sbr" \
	"$(INTDIR)/PropertyNoteDlg.sbr" \
	"$(INTDIR)/PianoRollVelScaleView.sbr" \
	"$(INTDIR)/EditQuantizeDlg.sbr" \
	"$(INTDIR)/MIDIInDevicePage.sbr" \
	"$(INTDIR)/AutoSaveDlg.sbr" \
	"$(INTDIR)/EditRemoveMeasureDlg.sbr" \
	"$(INTDIR)/TrackListOption2Page.sbr" \
	"$(INTDIR)/EditChannelDlg.sbr" \
	"$(INTDIR)/MusicalScoreTrackTimeView.sbr" \
	"$(INTDIR)/EventListPropertyScaleView.sbr" \
	"$(INTDIR)/EventListIndexScaleView.sbr" \
	"$(INTDIR)/SekaijuDocTemplate.sbr" \
	"$(INTDIR)/EditTimeDlg.sbr" \
	"$(INTDIR)/SekaijuToolBar.sbr" \
	"$(INTDIR)/MusicalScoreTimeScaleView.sbr" \
	"$(INTDIR)/ColorfulCheckListBox.sbr" \
	"$(INTDIR)/MusicalScoreOptionPage.sbr" \
	"$(INTDIR)/SekaijuDoc.sbr" \
	"$(INTDIR)/FilePropertyDlg.sbr" \
	"$(INTDIR)/PianoRollPrintView.sbr" \
	"$(INTDIR)/EditVelocityDlg.sbr" \
	"$(INTDIR)/ChildFrame.sbr" \
	"$(INTDIR)/SekaijuFileDlg.sbr" \
	"$(INTDIR)/EditKeyDlg.sbr" \
	"$(INTDIR)/PianoRollScaleView.sbr" \
	"$(INTDIR)/MusicalScoreTrackScaleView.sbr" \
	"$(INTDIR)/SekaijuApp.sbr" \
	"$(INTDIR)/InplaceEdit.sbr" \
	"$(INTDIR)/EditDurationDlg.sbr" \
	"$(INTDIR)/MIDIOutSyncModePage.sbr" \
	"$(INTDIR)/MIDIInSyncModePage.sbr" \
	"$(INTDIR)/InplaceListBox.sbr" \
	"$(INTDIR)/MetronomeDlg.sbr" \
	"$(INTDIR)/TrackListTrackModeView.sbr" \
	"$(INTDIR)/MusicalScoreFrame.sbr" \
	"$(INTDIR)/GeneralOptionPage.sbr" \
	"$(INTDIR)/EventListPrintView.sbr" \
	"$(INTDIR)/LanguageDlg.sbr" \
	"$(INTDIR)/PianoRollKeyScaleView.sbr" \
	"$(INTDIR)/TrackListPrintView.sbr" \
	"$(INTDIR)/MIDISyncModeSheet.sbr" \
	"$(INTDIR)/PianoRollVelTimeView.sbr" \
	"$(INTDIR)/HistoryRecord.sbr" \
	"$(INTDIR)/TrackListFrame.sbr" \
	"$(INTDIR)/MusicalScorePrintView.sbr" \
	"$(INTDIR)/MIDIOutDevicePage.sbr" \
	"$(INTDIR)/TrackListTrackScaleView.sbr" \
	"$(INTDIR)/EventListScaleView.sbr" \
	"$(INTDIR)/PianoRollKeyTimeView.sbr" \
	"$(INTDIR)/TrackListScaleView.sbr" \
	"$(INTDIR)/EditTimeSmpDlg.sbr" \
	"$(INTDIR)/AboutDlg.sbr" \
	"$(INTDIR)/MusicalScoreScaleView.sbr" \
	"$(INTDIR)/EditBeatScanDlg.sbr" \
	"$(INTDIR)/EditTrackDlg.sbr" \
	"$(INTDIR)/PianoRollOptionPage.sbr" \
	"$(INTDIR)/TrackListTrackTimeView.sbr" \
	"$(INTDIR)/PianoRollTimeScaleView.sbr" \
	"$(INTDIR)/SekaijuView.sbr" \
	"$(INTDIR)/ColorfulComboBox.sbr" \
	"$(INTDIR)/EditValueDlg.sbr" \
	"$(INTDIR)/SekaijuDocManager.sbr"

"$(OUTDIR)\Sekaiju.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 ole32.lib ..\MIDIData\Debug\MIDIDatad.lib ..\MIDIClock\Debug\MIDIClockd.lib ..\MIDIIO\Debug\MIDIIOd.lib ..\MIDIStatus\Debug\MIDIStatusd.lib ..\MIDIInstrument\Debug\MIDIInstrumentd.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/Sekaiju.exe"
# ADD LINK32 ole32.lib ..\MIDIData\Debug\MIDIDatad.lib ..\MIDIClock\Debug\MIDIClockd.lib ..\MIDIIO\Debug\MIDIIOd.lib ..\MIDIStatus\Debug\MIDIStatusd.lib ..\MIDIInstrument\Debug\MIDIInstrumentd.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386
LINK32_FLAGS=ole32.lib ..\MIDIData\Debug\MIDIDatad.lib\
 ..\MIDIClock\Debug\MIDIClockd.lib ..\MIDIIO\Debug\MIDIIOd.lib\
 ..\MIDIStatus\Debug\MIDIStatusd.lib ..\MIDIInstrument\Debug\MIDIInstrumentd.lib\
 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/Sekaiju.pdb" /debug /machine:I386 /out:"$(OUTDIR)/Sekaiju.exe" 
LINK32_OBJS= \
	"$(INTDIR)/HistoryRecord.obj" \
	"$(INTDIR)/TrackListFrame.obj" \
	"$(INTDIR)/MusicalScorePrintView.obj" \
	"$(INTDIR)/MIDIOutDevicePage.obj" \
	"$(INTDIR)/TrackListTrackScaleView.obj" \
	"$(INTDIR)/EventListScaleView.obj" \
	"$(INTDIR)/PianoRollKeyTimeView.obj" \
	"$(INTDIR)/TrackListScaleView.obj" \
	"$(INTDIR)/EditTimeSmpDlg.obj" \
	"$(INTDIR)/AboutDlg.obj" \
	"$(INTDIR)/MusicalScoreScaleView.obj" \
	"$(INTDIR)/EditBeatScanDlg.obj" \
	"$(INTDIR)/EditTrackDlg.obj" \
	"$(INTDIR)/PianoRollOptionPage.obj" \
	"$(INTDIR)/TrackListTrackTimeView.obj" \
	"$(INTDIR)/PianoRollTimeScaleView.obj" \
	"$(INTDIR)/SekaijuView.obj" \
	"$(INTDIR)/ColorfulComboBox.obj" \
	"$(INTDIR)/EditValueDlg.obj" \
	"$(INTDIR)/SekaijuDocManager.obj" \
	"$(INTDIR)/TrackListTimeScaleView.obj" \
	"$(INTDIR)/common.obj" \
	"$(INTDIR)/MainFrame.obj" \
	"$(INTDIR)/MIDIDeviceSheet.obj" \
	"$(INTDIR)/MIDIInstDefNormPage.obj" \
	"$(INTDIR)/EventListIndexPropertyView.obj" \
	"$(INTDIR)/TrackListModeScaleView.obj" \
	"$(INTDIR)/EditInsertMeasureDlg.obj" \
	"$(INTDIR)/TrackListOption1Page.obj" \
	"$(INTDIR)/HistoryUnit.obj" \
	"$(INTDIR)/PianoRollFrame.obj" \
	"$(INTDIR)/EventListOptionPage.obj" \
	"$(INTDIR)/EventListFrame.obj" \
	"$(INTDIR)/MIDIInstDefDrumPage.obj" \
	"$(INTDIR)/OptionSheet.obj" \
	"$(INTDIR)/EditBreakupAndTrillDlg.obj" \
	"$(INTDIR)/PropertyNoteDlg.obj" \
	"$(INTDIR)/PianoRollVelScaleView.obj" \
	"$(INTDIR)/EditQuantizeDlg.obj" \
	"$(INTDIR)/MIDIInDevicePage.obj" \
	"$(INTDIR)/AutoSaveDlg.obj" \
	"$(INTDIR)/EditRemoveMeasureDlg.obj" \
	"$(INTDIR)/TrackListOption2Page.obj" \
	"$(INTDIR)/EditChannelDlg.obj" \
	"$(INTDIR)/MusicalScoreTrackTimeView.obj" \
	"$(INTDIR)/EventListPropertyScaleView.obj" \
	"$(INTDIR)/EventListIndexScaleView.obj" \
	"$(INTDIR)/SekaijuDocTemplate.obj" \
	"$(INTDIR)/EditTimeDlg.obj" \
	"$(INTDIR)/SekaijuToolBar.obj" \
	"$(INTDIR)/MusicalScoreTimeScaleView.obj" \
	"$(INTDIR)/ColorfulCheckListBox.obj" \
	"$(INTDIR)/MusicalScoreOptionPage.obj" \
	"$(INTDIR)/SekaijuDoc.obj" \
	"$(INTDIR)/FilePropertyDlg.obj" \
	"$(INTDIR)/PianoRollPrintView.obj" \
	"$(INTDIR)/EditVelocityDlg.obj" \
	"$(INTDIR)/ChildFrame.obj" \
	"$(INTDIR)/SekaijuFileDlg.obj" \
	"$(INTDIR)/EditKeyDlg.obj" \
	"$(INTDIR)/PianoRollScaleView.obj" \
	"$(INTDIR)/MusicalScoreTrackScaleView.obj" \
	"$(INTDIR)/SekaijuApp.obj" \
	"$(INTDIR)/InplaceEdit.obj" \
	"$(INTDIR)/EditDurationDlg.obj" \
	"$(INTDIR)/MIDIOutSyncModePage.obj" \
	"$(INTDIR)/MIDIInSyncModePage.obj" \
	"$(INTDIR)/InplaceListBox.obj" \
	"$(INTDIR)/MetronomeDlg.obj" \
	"$(INTDIR)/TrackListTrackModeView.obj" \
	"$(INTDIR)/MusicalScoreFrame.obj" \
	"$(INTDIR)/GeneralOptionPage.obj" \
	"$(INTDIR)/EventListPrintView.obj" \
	"$(INTDIR)/LanguageDlg.obj" \
	"$(INTDIR)/PianoRollKeyScaleView.obj" \
	"$(INTDIR)/TrackListPrintView.obj" \
	"$(INTDIR)/MIDISyncModeSheet.obj" \
	"$(INTDIR)/PianoRollVelTimeView.obj" \
	"$(INTDIR)/Sekaiju.res"

"$(OUTDIR)\Sekaiju.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Sekaiju0"
# PROP BASE Intermediate_Dir "Sekaiju0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseU"
# PROP Intermediate_Dir "ReleaseU"
# PROP Target_Dir ""
OUTDIR=.\ReleaseU
INTDIR=.\ReleaseU

ALL : "$(OUTDIR)\Sekaiju.exe"

CLEAN : 
	-@erase ".\ReleaseU\Sekaiju.exe"
	-@erase ".\ReleaseU\TrackListTimeScaleView.obj"
	-@erase ".\ReleaseU\EditRemoveMeasureDlg.obj"
	-@erase ".\ReleaseU\TrackListFrame.obj"
	-@erase ".\ReleaseU\AboutDlg.obj"
	-@erase ".\ReleaseU\EditBeatScanDlg.obj"
	-@erase ".\ReleaseU\EventListPropertyScaleView.obj"
	-@erase ".\ReleaseU\EditTimeSmpDlg.obj"
	-@erase ".\ReleaseU\MainFrame.obj"
	-@erase ".\ReleaseU\SekaijuView.obj"
	-@erase ".\ReleaseU\EditTimeDlg.obj"
	-@erase ".\ReleaseU\EventListIndexScaleView.obj"
	-@erase ".\ReleaseU\MusicalScoreTimeScaleView.obj"
	-@erase ".\ReleaseU\SekaijuFileDlg.obj"
	-@erase ".\ReleaseU\PianoRollKeyScaleView.obj"
	-@erase ".\ReleaseU\MetronomeDlg.obj"
	-@erase ".\ReleaseU\MusicalScorePrintView.obj"
	-@erase ".\ReleaseU\MIDIInDevicePage.obj"
	-@erase ".\ReleaseU\HistoryRecord.obj"
	-@erase ".\ReleaseU\MusicalScoreTrackScaleView.obj"
	-@erase ".\ReleaseU\TrackListTrackTimeView.obj"
	-@erase ".\ReleaseU\MusicalScoreScaleView.obj"
	-@erase ".\ReleaseU\MIDISyncModeSheet.obj"
	-@erase ".\ReleaseU\SekaijuDocTemplate.obj"
	-@erase ".\ReleaseU\MIDIOutDevicePage.obj"
	-@erase ".\ReleaseU\InplaceListBox.obj"
	-@erase ".\ReleaseU\InplaceEdit.obj"
	-@erase ".\ReleaseU\PropertyNoteDlg.obj"
	-@erase ".\ReleaseU\EditTrackDlg.obj"
	-@erase ".\ReleaseU\EditQuantizeDlg.obj"
	-@erase ".\ReleaseU\MIDIOutSyncModePage.obj"
	-@erase ".\ReleaseU\PianoRollVelTimeView.obj"
	-@erase ".\ReleaseU\PianoRollPrintView.obj"
	-@erase ".\ReleaseU\MusicalScoreOptionPage.obj"
	-@erase ".\ReleaseU\TrackListOption2Page.obj"
	-@erase ".\ReleaseU\TrackListModeScaleView.obj"
	-@erase ".\ReleaseU\EditValueDlg.obj"
	-@erase ".\ReleaseU\PianoRollKeyTimeView.obj"
	-@erase ".\ReleaseU\LanguageDlg.obj"
	-@erase ".\ReleaseU\AutoSaveDlg.obj"
	-@erase ".\ReleaseU\PianoRollScaleView.obj"
	-@erase ".\ReleaseU\MusicalScoreTrackTimeView.obj"
	-@erase ".\ReleaseU\SekaijuDocManager.obj"
	-@erase ".\ReleaseU\ColorfulCheckListBox.obj"
	-@erase ".\ReleaseU\EditChannelDlg.obj"
	-@erase ".\ReleaseU\EditBreakupAndTrillDlg.obj"
	-@erase ".\ReleaseU\MIDIInSyncModePage.obj"
	-@erase ".\ReleaseU\SekaijuToolBar.obj"
	-@erase ".\ReleaseU\TrackListTrackScaleView.obj"
	-@erase ".\ReleaseU\PianoRollOptionPage.obj"
	-@erase ".\ReleaseU\MIDIDeviceSheet.obj"
	-@erase ".\ReleaseU\FilePropertyDlg.obj"
	-@erase ".\ReleaseU\PianoRollVelScaleView.obj"
	-@erase ".\ReleaseU\EditVelocityDlg.obj"
	-@erase ".\ReleaseU\TrackListTrackModeView.obj"
	-@erase ".\ReleaseU\EventListPrintView.obj"
	-@erase ".\ReleaseU\TrackListPrintView.obj"
	-@erase ".\ReleaseU\EventListIndexPropertyView.obj"
	-@erase ".\ReleaseU\SekaijuDoc.obj"
	-@erase ".\ReleaseU\EditInsertMeasureDlg.obj"
	-@erase ".\ReleaseU\TrackListOption1Page.obj"
	-@erase ".\ReleaseU\MIDIInstDefNormPage.obj"
	-@erase ".\ReleaseU\EventListScaleView.obj"
	-@erase ".\ReleaseU\TrackListScaleView.obj"
	-@erase ".\ReleaseU\ChildFrame.obj"
	-@erase ".\ReleaseU\EditKeyDlg.obj"
	-@erase ".\ReleaseU\PianoRollFrame.obj"
	-@erase ".\ReleaseU\EventListFrame.obj"
	-@erase ".\ReleaseU\EditDurationDlg.obj"
	-@erase ".\ReleaseU\HistoryUnit.obj"
	-@erase ".\ReleaseU\SekaijuApp.obj"
	-@erase ".\ReleaseU\common.obj"
	-@erase ".\ReleaseU\MusicalScoreFrame.obj"
	-@erase ".\ReleaseU\GeneralOptionPage.obj"
	-@erase ".\ReleaseU\PianoRollTimeScaleView.obj"
	-@erase ".\ReleaseU\EventListOptionPage.obj"
	-@erase ".\ReleaseU\MIDIInstDefDrumPage.obj"
	-@erase ".\ReleaseU\OptionSheet.obj"
	-@erase ".\ReleaseU\ColorfulComboBox.obj"
	-@erase ".\ReleaseU\Sekaiju.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /c
# ADD CPP /nologo /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /c
CPP_PROJ=/nologo /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL"\
 /D "_UNICODE" /D "UNICODE" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\ReleaseU/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL" /d "_UNICODE" /d "UNICODE"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/Sekaiju.res" /d "NDEBUG" /d "_AFXDLL" /d\
 "_UNICODE" /d "UNICODE" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Sekaiju.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 ..\MIDIData\Release\MIDIData.lib ..\MIDIClock\Release\MIDIClock.lib ..\MIDIIO\Release\MIDIIO.lib ..\MIDIStatus\Release\MIDIStatus.lib ..\MIDIInstrument\Release\MIDIInstrument.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\MIDIData\Release\MIDIData.lib ..\MIDIClock\Release\MIDIClock.lib ..\MIDIIO\Release\MIDIIO.lib ..\MIDIStatus\Release\MIDIStatus.lib ..\MIDIInstrument\Release\MIDIInstrument.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386
LINK32_FLAGS=..\MIDIData\Release\MIDIData.lib\
 ..\MIDIClock\Release\MIDIClock.lib ..\MIDIIO\Release\MIDIIO.lib\
 ..\MIDIStatus\Release\MIDIStatus.lib\
 ..\MIDIInstrument\Release\MIDIInstrument.lib /nologo\
 /entry:"wWinMainCRTStartup" /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/Sekaiju.pdb" /machine:I386 /out:"$(OUTDIR)/Sekaiju.exe" 
LINK32_OBJS= \
	"$(INTDIR)/TrackListTimeScaleView.obj" \
	"$(INTDIR)/EditRemoveMeasureDlg.obj" \
	"$(INTDIR)/TrackListFrame.obj" \
	"$(INTDIR)/AboutDlg.obj" \
	"$(INTDIR)/EditBeatScanDlg.obj" \
	"$(INTDIR)/EventListPropertyScaleView.obj" \
	"$(INTDIR)/EditTimeSmpDlg.obj" \
	"$(INTDIR)/MainFrame.obj" \
	"$(INTDIR)/SekaijuView.obj" \
	"$(INTDIR)/EditTimeDlg.obj" \
	"$(INTDIR)/EventListIndexScaleView.obj" \
	"$(INTDIR)/MusicalScoreTimeScaleView.obj" \
	"$(INTDIR)/SekaijuFileDlg.obj" \
	"$(INTDIR)/PianoRollKeyScaleView.obj" \
	"$(INTDIR)/MetronomeDlg.obj" \
	"$(INTDIR)/MusicalScorePrintView.obj" \
	"$(INTDIR)/MIDIInDevicePage.obj" \
	"$(INTDIR)/HistoryRecord.obj" \
	"$(INTDIR)/MusicalScoreTrackScaleView.obj" \
	"$(INTDIR)/TrackListTrackTimeView.obj" \
	"$(INTDIR)/MusicalScoreScaleView.obj" \
	"$(INTDIR)/MIDISyncModeSheet.obj" \
	"$(INTDIR)/SekaijuDocTemplate.obj" \
	"$(INTDIR)/MIDIOutDevicePage.obj" \
	"$(INTDIR)/InplaceListBox.obj" \
	"$(INTDIR)/InplaceEdit.obj" \
	"$(INTDIR)/PropertyNoteDlg.obj" \
	"$(INTDIR)/EditTrackDlg.obj" \
	"$(INTDIR)/EditQuantizeDlg.obj" \
	"$(INTDIR)/MIDIOutSyncModePage.obj" \
	"$(INTDIR)/PianoRollVelTimeView.obj" \
	"$(INTDIR)/PianoRollPrintView.obj" \
	"$(INTDIR)/MusicalScoreOptionPage.obj" \
	"$(INTDIR)/TrackListOption2Page.obj" \
	"$(INTDIR)/TrackListModeScaleView.obj" \
	"$(INTDIR)/EditValueDlg.obj" \
	"$(INTDIR)/PianoRollKeyTimeView.obj" \
	"$(INTDIR)/LanguageDlg.obj" \
	"$(INTDIR)/AutoSaveDlg.obj" \
	"$(INTDIR)/PianoRollScaleView.obj" \
	"$(INTDIR)/MusicalScoreTrackTimeView.obj" \
	"$(INTDIR)/SekaijuDocManager.obj" \
	"$(INTDIR)/ColorfulCheckListBox.obj" \
	"$(INTDIR)/EditChannelDlg.obj" \
	"$(INTDIR)/EditBreakupAndTrillDlg.obj" \
	"$(INTDIR)/MIDIInSyncModePage.obj" \
	"$(INTDIR)/SekaijuToolBar.obj" \
	"$(INTDIR)/TrackListTrackScaleView.obj" \
	"$(INTDIR)/PianoRollOptionPage.obj" \
	"$(INTDIR)/MIDIDeviceSheet.obj" \
	"$(INTDIR)/FilePropertyDlg.obj" \
	"$(INTDIR)/PianoRollVelScaleView.obj" \
	"$(INTDIR)/EditVelocityDlg.obj" \
	"$(INTDIR)/TrackListTrackModeView.obj" \
	"$(INTDIR)/EventListPrintView.obj" \
	"$(INTDIR)/TrackListPrintView.obj" \
	"$(INTDIR)/EventListIndexPropertyView.obj" \
	"$(INTDIR)/SekaijuDoc.obj" \
	"$(INTDIR)/EditInsertMeasureDlg.obj" \
	"$(INTDIR)/TrackListOption1Page.obj" \
	"$(INTDIR)/MIDIInstDefNormPage.obj" \
	"$(INTDIR)/EventListScaleView.obj" \
	"$(INTDIR)/TrackListScaleView.obj" \
	"$(INTDIR)/ChildFrame.obj" \
	"$(INTDIR)/EditKeyDlg.obj" \
	"$(INTDIR)/PianoRollFrame.obj" \
	"$(INTDIR)/EventListFrame.obj" \
	"$(INTDIR)/EditDurationDlg.obj" \
	"$(INTDIR)/HistoryUnit.obj" \
	"$(INTDIR)/SekaijuApp.obj" \
	"$(INTDIR)/common.obj" \
	"$(INTDIR)/MusicalScoreFrame.obj" \
	"$(INTDIR)/GeneralOptionPage.obj" \
	"$(INTDIR)/PianoRollTimeScaleView.obj" \
	"$(INTDIR)/EventListOptionPage.obj" \
	"$(INTDIR)/MIDIInstDefDrumPage.obj" \
	"$(INTDIR)/OptionSheet.obj" \
	"$(INTDIR)/ColorfulComboBox.obj" \
	"$(INTDIR)/Sekaiju.res"

"$(OUTDIR)\Sekaiju.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

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

# Name "Sekaiju - Win32 Release"
# Name "Sekaiju - Win32 Debug"
# Name "Sekaiju - Win32 DebugU"
# Name "Sekaiju - Win32 ReleaseU"

!IF  "$(CFG)" == "Sekaiju - Win32 Release"

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\src\SekaijuView.cpp
DEP_CPP_SEKAI=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\SekaijuView.obj" : $(SOURCE) $(DEP_CPP_SEKAI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\SekaijuView.obj" : $(SOURCE) $(DEP_CPP_SEKAI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\SekaijuView.sbr" : $(SOURCE) $(DEP_CPP_SEKAI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\SekaijuView.obj" : $(SOURCE) $(DEP_CPP_SEKAI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\SekaijuView.sbr" : $(SOURCE) $(DEP_CPP_SEKAI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\SekaijuView.obj" : $(SOURCE) $(DEP_CPP_SEKAI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\ChildFrame.cpp
DEP_CPP_CHILD=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuToolBar.h"\
	".\src\MainFrame.h"\
	".\src\ChildFrame.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\ChildFrame.obj" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\ChildFrame.obj" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\ChildFrame.sbr" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\ChildFrame.obj" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\ChildFrame.sbr" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\ChildFrame.obj" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\MainFrame.cpp
DEP_CPP_MAINF=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\mousewheel.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuToolBar.h"\
	".\src\MainFrame.h"\
	".\src\SekaijuDoc.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\MainFrame.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MainFrame.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MainFrame.sbr" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MainFrame.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MainFrame.sbr" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\MainFrame.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\PianoRollFrame.cpp
DEP_CPP_PIANO=\
	".\src\common.h"\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\Resource.h"\
	".\src\ColorfulComboBox.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\MainFrame.h"\
	".\src\ChildFrame.h"\
	".\src\PianoRollFrame.h"\
	".\src\PianoRollPrintView.h"\
	".\src\PianoRollScaleView.h"\
	".\src\PianoRollTimeScaleView.h"\
	".\src\PianoRollKeyScaleView.h"\
	".\src\PianoRollKeyTimeView.h"\
	".\src\PianoRollVelScaleView.h"\
	".\src\PianoRollVelTimeView.h"\
	".\src\PropertyNoteDlg.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\PianoRollFrame.obj" : $(SOURCE) $(DEP_CPP_PIANO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\PianoRollFrame.obj" : $(SOURCE) $(DEP_CPP_PIANO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\PianoRollFrame.sbr" : $(SOURCE) $(DEP_CPP_PIANO) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\PianoRollFrame.obj" : $(SOURCE) $(DEP_CPP_PIANO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\PianoRollFrame.sbr" : $(SOURCE) $(DEP_CPP_PIANO) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\PianoRollFrame.obj" : $(SOURCE) $(DEP_CPP_PIANO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\PianoRollKeyScaleView.cpp
DEP_CPP_PIANOR=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\mousewheel.h"\
	".\src\ColorfulComboBox.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\ChildFrame.h"\
	".\src\PianoRollFrame.h"\
	".\src\PianoRollKeyScaleView.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\PianoRollKeyScaleView.obj" : $(SOURCE) $(DEP_CPP_PIANOR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\PianoRollKeyScaleView.obj" : $(SOURCE) $(DEP_CPP_PIANOR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\PianoRollKeyScaleView.sbr" : $(SOURCE) $(DEP_CPP_PIANOR) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\PianoRollKeyScaleView.obj" : $(SOURCE) $(DEP_CPP_PIANOR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\PianoRollKeyScaleView.sbr" : $(SOURCE) $(DEP_CPP_PIANOR) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\PianoRollKeyScaleView.obj" : $(SOURCE) $(DEP_CPP_PIANOR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\PianoRollKeyTimeView.cpp
DEP_CPP_PIANORO=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\mousewheel.h"\
	".\src\ColorfulComboBox.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\ChildFrame.h"\
	".\src\PianoRollFrame.h"\
	".\src\PianoRollKeyTimeView.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\PianoRollKeyTimeView.obj" : $(SOURCE) $(DEP_CPP_PIANORO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\PianoRollKeyTimeView.obj" : $(SOURCE) $(DEP_CPP_PIANORO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\PianoRollKeyTimeView.sbr" : $(SOURCE) $(DEP_CPP_PIANORO) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\PianoRollKeyTimeView.obj" : $(SOURCE) $(DEP_CPP_PIANORO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\PianoRollKeyTimeView.sbr" : $(SOURCE) $(DEP_CPP_PIANORO) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\PianoRollKeyTimeView.obj" : $(SOURCE) $(DEP_CPP_PIANORO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\PianoRollTimeScaleView.cpp
DEP_CPP_PIANOROL=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\mousewheel.h"\
	".\src\ColorfulComboBox.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\ChildFrame.h"\
	".\src\PianoRollFrame.h"\
	".\src\PianoRollTimeScaleView.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\PianoRollTimeScaleView.obj" : $(SOURCE) $(DEP_CPP_PIANOROL)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\PianoRollTimeScaleView.obj" : $(SOURCE) $(DEP_CPP_PIANOROL)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\PianoRollTimeScaleView.sbr" : $(SOURCE) $(DEP_CPP_PIANOROL)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\PianoRollTimeScaleView.obj" : $(SOURCE) $(DEP_CPP_PIANOROL)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\PianoRollTimeScaleView.sbr" : $(SOURCE) $(DEP_CPP_PIANOROL)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\PianoRollTimeScaleView.obj" : $(SOURCE) $(DEP_CPP_PIANOROL)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\PianoRollVelScaleView.cpp
DEP_CPP_PIANOROLL=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\mousewheel.h"\
	".\src\ColorfulComboBox.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\ChildFrame.h"\
	".\src\PianoRollFrame.h"\
	".\src\PianoRollVelScaleView.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\PianoRollVelScaleView.obj" : $(SOURCE) $(DEP_CPP_PIANOROLL)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\PianoRollVelScaleView.obj" : $(SOURCE) $(DEP_CPP_PIANOROLL)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\PianoRollVelScaleView.sbr" : $(SOURCE) $(DEP_CPP_PIANOROLL)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\PianoRollVelScaleView.obj" : $(SOURCE) $(DEP_CPP_PIANOROLL)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\PianoRollVelScaleView.sbr" : $(SOURCE) $(DEP_CPP_PIANOROLL)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\PianoRollVelScaleView.obj" : $(SOURCE) $(DEP_CPP_PIANOROLL)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\PianoRollVelTimeView.cpp
DEP_CPP_PIANOROLLV=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\mousewheel.h"\
	".\src\ColorfulComboBox.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuToolBar.h"\
	".\src\ChildFrame.h"\
	".\src\PianoRollFrame.h"\
	".\src\SekaijuView.h"\
	".\src\PianoRollVelTimeView.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\PianoRollVelTimeView.obj" : $(SOURCE) $(DEP_CPP_PIANOROLLV)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\PianoRollVelTimeView.obj" : $(SOURCE) $(DEP_CPP_PIANOROLLV)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\PianoRollVelTimeView.sbr" : $(SOURCE) $(DEP_CPP_PIANOROLLV)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\PianoRollVelTimeView.obj" : $(SOURCE) $(DEP_CPP_PIANOROLLV)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\PianoRollVelTimeView.sbr" : $(SOURCE) $(DEP_CPP_PIANOROLLV)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\PianoRollVelTimeView.obj" : $(SOURCE) $(DEP_CPP_PIANOROLLV)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\SekaijuApp.cpp

!IF  "$(CFG)" == "Sekaiju - Win32 Release"

DEP_CPP_SEKAIJ=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\Resource.h"\
	".\src\ColorfulComboBox.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDocManager.h"\
	".\src\SekaijuDocTemplate.h"\
	".\src\SekaijuToolBar.h"\
	".\src\MainFrame.h"\
	".\src\ChildFrame.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\PianoRollFrame.h"\
	".\src\TrackListFrame.h"\
	".\src\MIDIDeviceSheet.h"\
	".\src\MIDISyncModeSheet.h"\
	".\src\AutoSaveDlg.h"\
	".\src\LanguageDlg.h"\
	".\src\MetronomeDlg.h"\
	".\src\OptionSheet.h"\
	".\src\AboutDlg.h"\
	".\src\MIDIInDevicePage.h"\
	".\src\MIDIOutDevicePage.h"\
	".\src\MIDIInstDefNormPage.h"\
	".\src\MIDIInstDefDrumPage.h"\
	".\src\MIDIInSyncModePage.h"\
	".\src\MIDIOutSyncModePage.h"\
	".\src\GeneralOptionPage.h"\
	".\src\TrackListOption1Page.h"\
	".\src\TrackListOption2Page.h"\
	".\src\PianoRollOptionPage.h"\
	".\src\EventListOptionPage.h"\
	".\src\MusicalScoreOptionPage.h"\
	

"$(INTDIR)\SekaijuApp.obj" : $(SOURCE) $(DEP_CPP_SEKAIJ) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"

DEP_CPP_SEKAIJ=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\Resource.h"\
	".\src\ColorfulComboBox.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDocManager.h"\
	".\src\SekaijuDocTemplate.h"\
	".\src\SekaijuToolBar.h"\
	".\src\MainFrame.h"\
	".\src\ChildFrame.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\PianoRollFrame.h"\
	".\src\TrackListFrame.h"\
	".\src\MIDIDeviceSheet.h"\
	".\src\MIDISyncModeSheet.h"\
	".\src\AutoSaveDlg.h"\
	".\src\LanguageDlg.h"\
	".\src\MetronomeDlg.h"\
	".\src\OptionSheet.h"\
	".\src\AboutDlg.h"\
	".\src\MIDIInDevicePage.h"\
	".\src\MIDIOutDevicePage.h"\
	".\src\MIDIInstDefNormPage.h"\
	".\src\MIDIInstDefDrumPage.h"\
	".\src\MIDIInSyncModePage.h"\
	".\src\MIDIOutSyncModePage.h"\
	".\src\GeneralOptionPage.h"\
	".\src\TrackListOption1Page.h"\
	".\src\TrackListOption2Page.h"\
	".\src\PianoRollOptionPage.h"\
	".\src\EventListOptionPage.h"\
	".\src\MusicalScoreOptionPage.h"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\SekaijuApp.obj" : $(SOURCE) $(DEP_CPP_SEKAIJ) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\SekaijuApp.sbr" : $(SOURCE) $(DEP_CPP_SEKAIJ) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"

DEP_CPP_SEKAIJ=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\Resource.h"\
	".\src\ColorfulComboBox.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDocManager.h"\
	".\src\SekaijuDocTemplate.h"\
	".\src\SekaijuToolBar.h"\
	".\src\MainFrame.h"\
	".\src\ChildFrame.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\PianoRollFrame.h"\
	".\src\TrackListFrame.h"\
	".\src\MIDIDeviceSheet.h"\
	".\src\MIDISyncModeSheet.h"\
	".\src\AutoSaveDlg.h"\
	".\src\LanguageDlg.h"\
	".\src\MetronomeDlg.h"\
	".\src\OptionSheet.h"\
	".\src\AboutDlg.h"\
	".\src\MIDIInDevicePage.h"\
	".\src\MIDIOutDevicePage.h"\
	".\src\MIDIInstDefNormPage.h"\
	".\src\MIDIInstDefDrumPage.h"\
	".\src\MIDIInSyncModePage.h"\
	".\src\MIDIOutSyncModePage.h"\
	".\src\GeneralOptionPage.h"\
	".\src\TrackListOption1Page.h"\
	".\src\TrackListOption2Page.h"\
	".\src\PianoRollOptionPage.h"\
	".\src\EventListOptionPage.h"\
	".\src\MusicalScoreOptionPage.h"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\SekaijuApp.obj" : $(SOURCE) $(DEP_CPP_SEKAIJ) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\SekaijuApp.sbr" : $(SOURCE) $(DEP_CPP_SEKAIJ) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"

DEP_CPP_SEKAIJ=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\Resource.h"\
	".\src\ColorfulComboBox.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDocManager.h"\
	".\src\SekaijuDocTemplate.h"\
	".\src\SekaijuToolBar.h"\
	".\src\MainFrame.h"\
	".\src\ChildFrame.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\PianoRollFrame.h"\
	".\src\TrackListFrame.h"\
	".\src\MIDIDeviceSheet.h"\
	".\src\MIDISyncModeSheet.h"\
	".\src\AutoSaveDlg.h"\
	".\src\LanguageDlg.h"\
	".\src\MetronomeDlg.h"\
	".\src\OptionSheet.h"\
	".\src\AboutDlg.h"\
	".\src\MIDIInDevicePage.h"\
	".\src\MIDIOutDevicePage.h"\
	".\src\MIDIInstDefNormPage.h"\
	".\src\MIDIInstDefDrumPage.h"\
	".\src\MIDIInSyncModePage.h"\
	".\src\MIDIOutSyncModePage.h"\
	".\src\GeneralOptionPage.h"\
	

"$(INTDIR)\SekaijuApp.obj" : $(SOURCE) $(DEP_CPP_SEKAIJ) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\SekaijuDoc.cpp
DEP_CPP_SEKAIJU=\
	".\src\common.h"\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\ColorfulComboBox.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDocTemplate.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\MainFrame.h"\
	".\src\ChildFrame.h"\
	".\src\FilePropertyDlg.h"\
	".\src\EditTrackDlg.h"\
	".\src\EditTimeDlg.h"\
	".\src\EditTimeSmpDlg.h"\
	".\src\EditChannelDlg.h"\
	".\src\EditKeyDlg.h"\
	".\src\EditVelocityDlg.h"\
	".\src\EditDurationDlg.h"\
	".\src\EditValueDlg.h"\
	".\src\EditBreakupAndTrillDlg.h"\
	".\src\EditQuantizeDlg.h"\
	".\src\EditBeatScanDlg.h"\
	".\src\EditInsertMeasureDlg.h"\
	".\src\EditRemoveMeasureDlg.h"\
	".\src\TrackListFrame.h"\
	".\src\PianoRollFrame.h"\
	".\src\EventListFrame.h"\
	".\src\MusicalScoreFrame.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\SekaijuDoc.obj" : $(SOURCE) $(DEP_CPP_SEKAIJU) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\SekaijuDoc.obj" : $(SOURCE) $(DEP_CPP_SEKAIJU) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\SekaijuDoc.sbr" : $(SOURCE) $(DEP_CPP_SEKAIJU) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\SekaijuDoc.obj" : $(SOURCE) $(DEP_CPP_SEKAIJU) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\SekaijuDoc.sbr" : $(SOURCE) $(DEP_CPP_SEKAIJU) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\SekaijuDoc.obj" : $(SOURCE) $(DEP_CPP_SEKAIJU) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\AboutDlg.cpp
DEP_CPP_ABOUT=\
	".\src\Resource.h"\
	".\src\AboutDlg.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\AboutDlg.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\AboutDlg.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\AboutDlg.sbr" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\AboutDlg.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\AboutDlg.sbr" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\AboutDlg.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\PianoRollScaleView.cpp
DEP_CPP_PIANOROLLS=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\mousewheel.h"\
	".\src\ColorfulComboBox.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\MainFrame.h"\
	".\src\ChildFrame.h"\
	".\src\PianoRollFrame.h"\
	".\src\PianoRollScaleView.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\PianoRollScaleView.obj" : $(SOURCE) $(DEP_CPP_PIANOROLLS)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\PianoRollScaleView.obj" : $(SOURCE) $(DEP_CPP_PIANOROLLS)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\PianoRollScaleView.sbr" : $(SOURCE) $(DEP_CPP_PIANOROLLS)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\PianoRollScaleView.obj" : $(SOURCE) $(DEP_CPP_PIANOROLLS)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\PianoRollScaleView.sbr" : $(SOURCE) $(DEP_CPP_PIANOROLLS)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\PianoRollScaleView.obj" : $(SOURCE) $(DEP_CPP_PIANOROLLS)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\TrackListTrackTimeView.cpp
DEP_CPP_TRACK=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\mousewheel.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuToolBar.h"\
	".\src\ChildFrame.h"\
	".\src\TrackListFrame.h"\
	".\src\SekaijuView.h"\
	".\src\TrackListTrackTimeView.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\TrackListTrackTimeView.obj" : $(SOURCE) $(DEP_CPP_TRACK) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\TrackListTrackTimeView.obj" : $(SOURCE) $(DEP_CPP_TRACK) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\TrackListTrackTimeView.sbr" : $(SOURCE) $(DEP_CPP_TRACK) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\TrackListTrackTimeView.obj" : $(SOURCE) $(DEP_CPP_TRACK) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\TrackListTrackTimeView.sbr" : $(SOURCE) $(DEP_CPP_TRACK) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\TrackListTrackTimeView.obj" : $(SOURCE) $(DEP_CPP_TRACK) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\TrackListModeScaleView.cpp
DEP_CPP_TRACKL=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\mousewheel.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\ChildFrame.h"\
	".\src\TrackListFrame.h"\
	".\src\TrackListModeScaleView.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\TrackListModeScaleView.obj" : $(SOURCE) $(DEP_CPP_TRACKL)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\TrackListModeScaleView.obj" : $(SOURCE) $(DEP_CPP_TRACKL)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\TrackListModeScaleView.sbr" : $(SOURCE) $(DEP_CPP_TRACKL)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\TrackListModeScaleView.obj" : $(SOURCE) $(DEP_CPP_TRACKL)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\TrackListModeScaleView.sbr" : $(SOURCE) $(DEP_CPP_TRACKL)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\TrackListModeScaleView.obj" : $(SOURCE) $(DEP_CPP_TRACKL)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\TrackListScaleView.cpp
DEP_CPP_TRACKLI=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\mousewheel.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\MainFrame.h"\
	".\src\ChildFrame.h"\
	".\src\TrackListFrame.h"\
	".\src\TrackListScaleView.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\TrackListScaleView.obj" : $(SOURCE) $(DEP_CPP_TRACKLI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\TrackListScaleView.obj" : $(SOURCE) $(DEP_CPP_TRACKLI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\TrackListScaleView.sbr" : $(SOURCE) $(DEP_CPP_TRACKLI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\TrackListScaleView.obj" : $(SOURCE) $(DEP_CPP_TRACKLI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\TrackListScaleView.sbr" : $(SOURCE) $(DEP_CPP_TRACKLI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\TrackListScaleView.obj" : $(SOURCE) $(DEP_CPP_TRACKLI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\TrackListTimeScaleView.cpp
DEP_CPP_TRACKLIS=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\mousewheel.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\ChildFrame.h"\
	".\src\TrackListFrame.h"\
	".\src\TrackListTimeScaleView.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\TrackListTimeScaleView.obj" : $(SOURCE) $(DEP_CPP_TRACKLIS)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\TrackListTimeScaleView.obj" : $(SOURCE) $(DEP_CPP_TRACKLIS)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\TrackListTimeScaleView.sbr" : $(SOURCE) $(DEP_CPP_TRACKLIS)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\TrackListTimeScaleView.obj" : $(SOURCE) $(DEP_CPP_TRACKLIS)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\TrackListTimeScaleView.sbr" : $(SOURCE) $(DEP_CPP_TRACKLIS)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\TrackListTimeScaleView.obj" : $(SOURCE) $(DEP_CPP_TRACKLIS)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\TrackListTrackModeView.cpp
DEP_CPP_TRACKLIST=\
	".\src\common.h"\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\mousewheel.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\ChildFrame.h"\
	".\src\TrackListFrame.h"\
	".\src\TrackListTrackModeView.h"\
	".\src\Resource.h"\
	".\src\InplaceEdit.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\TrackListTrackModeView.obj" : $(SOURCE) $(DEP_CPP_TRACKLIST)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\TrackListTrackModeView.obj" : $(SOURCE) $(DEP_CPP_TRACKLIST)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\TrackListTrackModeView.sbr" : $(SOURCE) $(DEP_CPP_TRACKLIST)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\TrackListTrackModeView.obj" : $(SOURCE) $(DEP_CPP_TRACKLIST)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\TrackListTrackModeView.sbr" : $(SOURCE) $(DEP_CPP_TRACKLIST)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\TrackListTrackModeView.obj" : $(SOURCE) $(DEP_CPP_TRACKLIST)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\TrackListTrackScaleView.cpp
DEP_CPP_TRACKLISTT=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\mousewheel.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\ChildFrame.h"\
	".\src\TrackListFrame.h"\
	".\src\TrackListTrackScaleView.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\TrackListTrackScaleView.obj" : $(SOURCE) $(DEP_CPP_TRACKLISTT)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\TrackListTrackScaleView.obj" : $(SOURCE) $(DEP_CPP_TRACKLISTT)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\TrackListTrackScaleView.sbr" : $(SOURCE) $(DEP_CPP_TRACKLISTT)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\TrackListTrackScaleView.obj" : $(SOURCE) $(DEP_CPP_TRACKLISTT)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\TrackListTrackScaleView.sbr" : $(SOURCE) $(DEP_CPP_TRACKLISTT)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\TrackListTrackScaleView.obj" : $(SOURCE) $(DEP_CPP_TRACKLISTT)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\TrackListFrame.cpp
DEP_CPP_TRACKLISTF=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\Resource.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\SekaijuFileDlg.h"\
	".\src\MainFrame.h"\
	".\src\ChildFrame.h"\
	".\src\TrackListFrame.h"\
	".\src\TrackListPrintView.h"\
	".\src\TrackListScaleView.h"\
	".\src\TrackListModeScaleView.h"\
	".\src\TrackListTimeScaleView.h"\
	".\src\TrackListTrackScaleView.h"\
	".\src\TrackListTrackModeView.h"\
	".\src\TrackListTrackTimeView.h"\
	".\src\InplaceEdit.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\TrackListFrame.obj" : $(SOURCE) $(DEP_CPP_TRACKLISTF) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\TrackListFrame.obj" : $(SOURCE) $(DEP_CPP_TRACKLISTF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\TrackListFrame.sbr" : $(SOURCE) $(DEP_CPP_TRACKLISTF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\TrackListFrame.obj" : $(SOURCE) $(DEP_CPP_TRACKLISTF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\TrackListFrame.sbr" : $(SOURCE) $(DEP_CPP_TRACKLISTF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\TrackListFrame.obj" : $(SOURCE) $(DEP_CPP_TRACKLISTF) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\SekaijuDocTemplate.cpp
DEP_CPP_SEKAIJUD=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\ColorfulComboBox.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuDocTemplate.h"\
	".\src\SekaijuToolBar.h"\
	".\src\ChildFrame.h"\
	".\src\TrackListFrame.h"\
	".\src\PianoRollFrame.h"\
	".\src\EventListFrame.h"\
	".\src\MusicalScoreFrame.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\SekaijuDocTemplate.obj" : $(SOURCE) $(DEP_CPP_SEKAIJUD) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\SekaijuDocTemplate.obj" : $(SOURCE) $(DEP_CPP_SEKAIJUD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\SekaijuDocTemplate.sbr" : $(SOURCE) $(DEP_CPP_SEKAIJUD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\SekaijuDocTemplate.obj" : $(SOURCE) $(DEP_CPP_SEKAIJUD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\SekaijuDocTemplate.sbr" : $(SOURCE) $(DEP_CPP_SEKAIJUD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\SekaijuDocTemplate.obj" : $(SOURCE) $(DEP_CPP_SEKAIJUD) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\EventListPropertyScaleView.cpp
DEP_CPP_EVENT=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\mousewheel.h"\
	".\src\ColorfulComboBox.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\ChildFrame.h"\
	".\src\EventListFrame.h"\
	".\src\EventListPropertyScaleView.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\EventListPropertyScaleView.obj" : $(SOURCE) $(DEP_CPP_EVENT)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EventListPropertyScaleView.obj" : $(SOURCE) $(DEP_CPP_EVENT)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EventListPropertyScaleView.sbr" : $(SOURCE) $(DEP_CPP_EVENT)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EventListPropertyScaleView.obj" : $(SOURCE) $(DEP_CPP_EVENT)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EventListPropertyScaleView.sbr" : $(SOURCE) $(DEP_CPP_EVENT)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\EventListPropertyScaleView.obj" : $(SOURCE) $(DEP_CPP_EVENT)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\EventListIndexPropertyView.cpp
DEP_CPP_EVENTL=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\common.h"\
	".\src\mousewheel.h"\
	".\src\ColorfulComboBox.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\ChildFrame.h"\
	".\src\EventListFrame.h"\
	".\src\EventListIndexPropertyView.h"\
	".\src\Resource.h"\
	".\src\InplaceEdit.h"\
	".\src\InplaceListBox.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\EventListIndexPropertyView.obj" : $(SOURCE) $(DEP_CPP_EVENTL)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EventListIndexPropertyView.obj" : $(SOURCE) $(DEP_CPP_EVENTL)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EventListIndexPropertyView.sbr" : $(SOURCE) $(DEP_CPP_EVENTL)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EventListIndexPropertyView.obj" : $(SOURCE) $(DEP_CPP_EVENTL)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EventListIndexPropertyView.sbr" : $(SOURCE) $(DEP_CPP_EVENTL)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\EventListIndexPropertyView.obj" : $(SOURCE) $(DEP_CPP_EVENTL)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\EventListIndexScaleView.cpp
DEP_CPP_EVENTLI=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\mousewheel.h"\
	".\src\ColorfulComboBox.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\ChildFrame.h"\
	".\src\EventListFrame.h"\
	".\src\EventListIndexScaleView.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\EventListIndexScaleView.obj" : $(SOURCE) $(DEP_CPP_EVENTLI)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EventListIndexScaleView.obj" : $(SOURCE) $(DEP_CPP_EVENTLI)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EventListIndexScaleView.sbr" : $(SOURCE) $(DEP_CPP_EVENTLI)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EventListIndexScaleView.obj" : $(SOURCE) $(DEP_CPP_EVENTLI)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EventListIndexScaleView.sbr" : $(SOURCE) $(DEP_CPP_EVENTLI)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\EventListIndexScaleView.obj" : $(SOURCE) $(DEP_CPP_EVENTLI)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\EventListFrame.cpp
DEP_CPP_EVENTLIS=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\Resource.h"\
	".\src\ColorfulComboBox.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\SekaijuFileDlg.h"\
	".\src\MainFrame.h"\
	".\src\ChildFrame.h"\
	".\src\EventListFrame.h"\
	".\src\EventListPrintView.h"\
	".\src\EventListScaleView.h"\
	".\src\EventListIndexScaleView.h"\
	".\src\EventListPropertyScaleView.h"\
	".\src\EventListIndexPropertyView.h"\
	".\src\common.h"\
	".\src\InplaceEdit.h"\
	".\src\InplaceListBox.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\EventListFrame.obj" : $(SOURCE) $(DEP_CPP_EVENTLIS) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EventListFrame.obj" : $(SOURCE) $(DEP_CPP_EVENTLIS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EventListFrame.sbr" : $(SOURCE) $(DEP_CPP_EVENTLIS) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EventListFrame.obj" : $(SOURCE) $(DEP_CPP_EVENTLIS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EventListFrame.sbr" : $(SOURCE) $(DEP_CPP_EVENTLIS) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\EventListFrame.obj" : $(SOURCE) $(DEP_CPP_EVENTLIS) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\EventListScaleView.cpp
DEP_CPP_EVENTLIST=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\mousewheel.h"\
	".\src\ColorfulComboBox.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\MainFrame.h"\
	".\src\ChildFrame.h"\
	".\src\EventListFrame.h"\
	".\src\EventListScaleView.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\EventListScaleView.obj" : $(SOURCE) $(DEP_CPP_EVENTLIST) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EventListScaleView.obj" : $(SOURCE) $(DEP_CPP_EVENTLIST) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EventListScaleView.sbr" : $(SOURCE) $(DEP_CPP_EVENTLIST) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EventListScaleView.obj" : $(SOURCE) $(DEP_CPP_EVENTLIST) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EventListScaleView.sbr" : $(SOURCE) $(DEP_CPP_EVENTLIST) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\EventListScaleView.obj" : $(SOURCE) $(DEP_CPP_EVENTLIST) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\MIDIInDevicePage.cpp
DEP_CPP_MIDII=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	".\src\MIDIInDevicePage.h"\
	".\src\MIDIOutDevicePage.h"\
	".\src\MIDIInstDefNormPage.h"\
	".\src\MIDIInstDefDrumPage.h"\
	".\src\MIDIDeviceSheet.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\MIDIInDevicePage.obj" : $(SOURCE) $(DEP_CPP_MIDII) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MIDIInDevicePage.obj" : $(SOURCE) $(DEP_CPP_MIDII) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MIDIInDevicePage.sbr" : $(SOURCE) $(DEP_CPP_MIDII) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MIDIInDevicePage.obj" : $(SOURCE) $(DEP_CPP_MIDII) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MIDIInDevicePage.sbr" : $(SOURCE) $(DEP_CPP_MIDII) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\MIDIInDevicePage.obj" : $(SOURCE) $(DEP_CPP_MIDII) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\MIDIDeviceSheet.cpp
DEP_CPP_MIDID=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	".\src\MIDIInDevicePage.h"\
	".\src\MIDIOutDevicePage.h"\
	".\src\MIDIInstDefNormPage.h"\
	".\src\MIDIInstDefDrumPage.h"\
	".\src\MIDIDeviceSheet.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\MIDIDeviceSheet.obj" : $(SOURCE) $(DEP_CPP_MIDID) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MIDIDeviceSheet.obj" : $(SOURCE) $(DEP_CPP_MIDID) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MIDIDeviceSheet.sbr" : $(SOURCE) $(DEP_CPP_MIDID) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MIDIDeviceSheet.obj" : $(SOURCE) $(DEP_CPP_MIDID) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MIDIDeviceSheet.sbr" : $(SOURCE) $(DEP_CPP_MIDID) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\MIDIDeviceSheet.obj" : $(SOURCE) $(DEP_CPP_MIDID) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\MIDIOutDevicePage.cpp
DEP_CPP_MIDIO=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	".\src\MIDIInDevicePage.h"\
	".\src\MIDIOutDevicePage.h"\
	".\src\MIDIInstDefNormPage.h"\
	".\src\MIDIInstDefDrumPage.h"\
	".\src\MIDIDeviceSheet.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\MIDIOutDevicePage.obj" : $(SOURCE) $(DEP_CPP_MIDIO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MIDIOutDevicePage.obj" : $(SOURCE) $(DEP_CPP_MIDIO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MIDIOutDevicePage.sbr" : $(SOURCE) $(DEP_CPP_MIDIO) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MIDIOutDevicePage.obj" : $(SOURCE) $(DEP_CPP_MIDIO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MIDIOutDevicePage.sbr" : $(SOURCE) $(DEP_CPP_MIDIO) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\MIDIOutDevicePage.obj" : $(SOURCE) $(DEP_CPP_MIDIO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\SekaijuToolBar.cpp
DEP_CPP_SEKAIJUT=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\Resource.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuToolBar.h"\
	".\src\MainFrame.h"\
	".\src\SekaijuDoc.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\SekaijuToolBar.obj" : $(SOURCE) $(DEP_CPP_SEKAIJUT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\SekaijuToolBar.obj" : $(SOURCE) $(DEP_CPP_SEKAIJUT) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\SekaijuToolBar.sbr" : $(SOURCE) $(DEP_CPP_SEKAIJUT) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\SekaijuToolBar.obj" : $(SOURCE) $(DEP_CPP_SEKAIJUT) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\SekaijuToolBar.sbr" : $(SOURCE) $(DEP_CPP_SEKAIJUT) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\SekaijuToolBar.obj" : $(SOURCE) $(DEP_CPP_SEKAIJUT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\common.c
DEP_CPP_COMMO=\
	".\src\common.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\common.obj" : $(SOURCE) $(DEP_CPP_COMMO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\common.obj" : $(SOURCE) $(DEP_CPP_COMMO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\common.sbr" : $(SOURCE) $(DEP_CPP_COMMO) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\common.obj" : $(SOURCE) $(DEP_CPP_COMMO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\common.sbr" : $(SOURCE) $(DEP_CPP_COMMO) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\common.obj" : $(SOURCE) $(DEP_CPP_COMMO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\MIDIInstDefNormPage.cpp
DEP_CPP_MIDIIN=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	".\src\MIDIInDevicePage.h"\
	".\src\MIDIOutDevicePage.h"\
	".\src\MIDIInstDefNormPage.h"\
	".\src\MIDIInstDefDrumPage.h"\
	".\src\MIDIDeviceSheet.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\MIDIInstDefNormPage.obj" : $(SOURCE) $(DEP_CPP_MIDIIN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MIDIInstDefNormPage.obj" : $(SOURCE) $(DEP_CPP_MIDIIN) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MIDIInstDefNormPage.sbr" : $(SOURCE) $(DEP_CPP_MIDIIN) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MIDIInstDefNormPage.obj" : $(SOURCE) $(DEP_CPP_MIDIIN) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MIDIInstDefNormPage.sbr" : $(SOURCE) $(DEP_CPP_MIDIIN) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\MIDIInstDefNormPage.obj" : $(SOURCE) $(DEP_CPP_MIDIIN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\MIDIInstDefDrumPage.cpp
DEP_CPP_MIDIINS=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	".\src\MIDIInDevicePage.h"\
	".\src\MIDIOutDevicePage.h"\
	".\src\MIDIInstDefNormPage.h"\
	".\src\MIDIInstDefDrumPage.h"\
	".\src\MIDIDeviceSheet.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\MIDIInstDefDrumPage.obj" : $(SOURCE) $(DEP_CPP_MIDIINS) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MIDIInstDefDrumPage.obj" : $(SOURCE) $(DEP_CPP_MIDIINS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MIDIInstDefDrumPage.sbr" : $(SOURCE) $(DEP_CPP_MIDIINS) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MIDIInstDefDrumPage.obj" : $(SOURCE) $(DEP_CPP_MIDIINS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MIDIInstDefDrumPage.sbr" : $(SOURCE) $(DEP_CPP_MIDIINS) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\MIDIInstDefDrumPage.obj" : $(SOURCE) $(DEP_CPP_MIDIINS) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\SekaijuDocManager.cpp

!IF  "$(CFG)" == "Sekaiju - Win32 Release"

DEP_CPP_SEKAIJUDO=\
	".\src\Resource.h"\
	".\src\SekaijuDocManager.h"\
	".\src\SekaijuFileDlg.h"\
	

"$(INTDIR)\SekaijuDocManager.obj" : $(SOURCE) $(DEP_CPP_SEKAIJUDO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"

DEP_CPP_SEKAIJUDO=\
	".\src\Resource.h"\
	".\src\SekaijuDocManager.h"\
	".\src\SekaijuFileDlg.h"\
	
NODEP_CPP_SEKAIJUDO=\
	".\src\_WIN32_WINNT"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\SekaijuDocManager.obj" : $(SOURCE) $(DEP_CPP_SEKAIJUDO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\SekaijuDocManager.sbr" : $(SOURCE) $(DEP_CPP_SEKAIJUDO) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"

DEP_CPP_SEKAIJUDO=\
	".\src\Resource.h"\
	".\src\SekaijuDocManager.h"\
	".\src\SekaijuFileDlg.h"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\SekaijuDocManager.obj" : $(SOURCE) $(DEP_CPP_SEKAIJUDO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\SekaijuDocManager.sbr" : $(SOURCE) $(DEP_CPP_SEKAIJUDO) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"

DEP_CPP_SEKAIJUDO=\
	".\src\Resource.h"\
	".\src\SekaijuDocManager.h"\
	".\src\SekaijuFileDlg.h"\
	

"$(INTDIR)\SekaijuDocManager.obj" : $(SOURCE) $(DEP_CPP_SEKAIJUDO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\InplaceListBox.cpp
DEP_CPP_INPLA=\
	".\src\InplaceListBox.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\InplaceListBox.obj" : $(SOURCE) $(DEP_CPP_INPLA) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\InplaceListBox.obj" : $(SOURCE) $(DEP_CPP_INPLA) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\InplaceListBox.sbr" : $(SOURCE) $(DEP_CPP_INPLA) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\InplaceListBox.obj" : $(SOURCE) $(DEP_CPP_INPLA) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\InplaceListBox.sbr" : $(SOURCE) $(DEP_CPP_INPLA) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\InplaceListBox.obj" : $(SOURCE) $(DEP_CPP_INPLA) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\InplaceEdit.cpp
DEP_CPP_INPLAC=\
	".\src\InplaceEdit.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\InplaceEdit.obj" : $(SOURCE) $(DEP_CPP_INPLAC) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\InplaceEdit.obj" : $(SOURCE) $(DEP_CPP_INPLAC) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\InplaceEdit.sbr" : $(SOURCE) $(DEP_CPP_INPLAC) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\InplaceEdit.obj" : $(SOURCE) $(DEP_CPP_INPLAC) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\InplaceEdit.sbr" : $(SOURCE) $(DEP_CPP_INPLAC) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\InplaceEdit.obj" : $(SOURCE) $(DEP_CPP_INPLAC) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\FilePropertyDlg.cpp
DEP_CPP_FILEP=\
	".\src\Resource.h"\
	".\src\FilePropertyDlg.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\FilePropertyDlg.obj" : $(SOURCE) $(DEP_CPP_FILEP) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\FilePropertyDlg.obj" : $(SOURCE) $(DEP_CPP_FILEP) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\FilePropertyDlg.sbr" : $(SOURCE) $(DEP_CPP_FILEP) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\FilePropertyDlg.obj" : $(SOURCE) $(DEP_CPP_FILEP) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\FilePropertyDlg.sbr" : $(SOURCE) $(DEP_CPP_FILEP) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\FilePropertyDlg.obj" : $(SOURCE) $(DEP_CPP_FILEP) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\HistoryUnit.cpp
DEP_CPP_HISTO=\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuDoc.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\HistoryUnit.obj" : $(SOURCE) $(DEP_CPP_HISTO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\HistoryUnit.obj" : $(SOURCE) $(DEP_CPP_HISTO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\HistoryUnit.sbr" : $(SOURCE) $(DEP_CPP_HISTO) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\HistoryUnit.obj" : $(SOURCE) $(DEP_CPP_HISTO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\HistoryUnit.sbr" : $(SOURCE) $(DEP_CPP_HISTO) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\HistoryUnit.obj" : $(SOURCE) $(DEP_CPP_HISTO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\HistoryRecord.cpp
DEP_CPP_HISTOR=\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\HistoryRecord.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\HistoryRecord.obj" : $(SOURCE) $(DEP_CPP_HISTOR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\HistoryRecord.obj" : $(SOURCE) $(DEP_CPP_HISTOR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\HistoryRecord.sbr" : $(SOURCE) $(DEP_CPP_HISTOR) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\HistoryRecord.obj" : $(SOURCE) $(DEP_CPP_HISTOR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\HistoryRecord.sbr" : $(SOURCE) $(DEP_CPP_HISTOR) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\HistoryRecord.obj" : $(SOURCE) $(DEP_CPP_HISTOR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\EditVelocityDlg.cpp
DEP_CPP_EDITV=\
	".\src\Resource.h"\
	".\src\EditVelocityDlg.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\EditVelocityDlg.obj" : $(SOURCE) $(DEP_CPP_EDITV) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditVelocityDlg.obj" : $(SOURCE) $(DEP_CPP_EDITV) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditVelocityDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITV) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditVelocityDlg.obj" : $(SOURCE) $(DEP_CPP_EDITV) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditVelocityDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITV) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\EditVelocityDlg.obj" : $(SOURCE) $(DEP_CPP_EDITV) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\EditValueDlg.cpp
DEP_CPP_EDITVA=\
	".\src\Resource.h"\
	".\src\EditValueDlg.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\EditValueDlg.obj" : $(SOURCE) $(DEP_CPP_EDITVA) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditValueDlg.obj" : $(SOURCE) $(DEP_CPP_EDITVA) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditValueDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITVA) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditValueDlg.obj" : $(SOURCE) $(DEP_CPP_EDITVA) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditValueDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITVA) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\EditValueDlg.obj" : $(SOURCE) $(DEP_CPP_EDITVA) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\EditDurationDlg.cpp
DEP_CPP_EDITD=\
	".\src\Resource.h"\
	".\src\EditDurationDlg.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\EditDurationDlg.obj" : $(SOURCE) $(DEP_CPP_EDITD) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditDurationDlg.obj" : $(SOURCE) $(DEP_CPP_EDITD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditDurationDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditDurationDlg.obj" : $(SOURCE) $(DEP_CPP_EDITD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditDurationDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\EditDurationDlg.obj" : $(SOURCE) $(DEP_CPP_EDITD) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\EditTimeDlg.cpp
DEP_CPP_EDITT=\
	".\src\Resource.h"\
	".\src\EditTimeDlg.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\EditTimeDlg.obj" : $(SOURCE) $(DEP_CPP_EDITT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditTimeDlg.obj" : $(SOURCE) $(DEP_CPP_EDITT) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditTimeDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITT) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditTimeDlg.obj" : $(SOURCE) $(DEP_CPP_EDITT) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditTimeDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITT) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\EditTimeDlg.obj" : $(SOURCE) $(DEP_CPP_EDITT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\EditKeyDlg.cpp
DEP_CPP_EDITK=\
	".\src\Resource.h"\
	".\src\EditKeyDlg.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\EditKeyDlg.obj" : $(SOURCE) $(DEP_CPP_EDITK) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditKeyDlg.obj" : $(SOURCE) $(DEP_CPP_EDITK) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditKeyDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITK) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditKeyDlg.obj" : $(SOURCE) $(DEP_CPP_EDITK) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditKeyDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITK) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\EditKeyDlg.obj" : $(SOURCE) $(DEP_CPP_EDITK) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\EditChannelDlg.cpp
DEP_CPP_EDITC=\
	".\src\Resource.h"\
	".\src\EditChannelDlg.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\EditChannelDlg.obj" : $(SOURCE) $(DEP_CPP_EDITC) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditChannelDlg.obj" : $(SOURCE) $(DEP_CPP_EDITC) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditChannelDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITC) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditChannelDlg.obj" : $(SOURCE) $(DEP_CPP_EDITC) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditChannelDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITC) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\EditChannelDlg.obj" : $(SOURCE) $(DEP_CPP_EDITC) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\EditTimeSmpDlg.cpp
DEP_CPP_EDITTI=\
	".\src\Resource.h"\
	".\src\EditTimeSmpDlg.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\EditTimeSmpDlg.obj" : $(SOURCE) $(DEP_CPP_EDITTI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditTimeSmpDlg.obj" : $(SOURCE) $(DEP_CPP_EDITTI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditTimeSmpDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITTI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditTimeSmpDlg.obj" : $(SOURCE) $(DEP_CPP_EDITTI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditTimeSmpDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITTI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\EditTimeSmpDlg.obj" : $(SOURCE) $(DEP_CPP_EDITTI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\EditTrackDlg.cpp
DEP_CPP_EDITTR=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\Resource.h"\
	".\src\SekaijuApp.h"\
	".\src\EditTrackDlg.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\EditTrackDlg.obj" : $(SOURCE) $(DEP_CPP_EDITTR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditTrackDlg.obj" : $(SOURCE) $(DEP_CPP_EDITTR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditTrackDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITTR) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditTrackDlg.obj" : $(SOURCE) $(DEP_CPP_EDITTR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditTrackDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITTR) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\EditTrackDlg.obj" : $(SOURCE) $(DEP_CPP_EDITTR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\MetronomeDlg.cpp
DEP_CPP_METRO=\
	".\src\Resource.h"\
	".\src\MetronomeDlg.h"\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\MetronomeDlg.obj" : $(SOURCE) $(DEP_CPP_METRO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MetronomeDlg.obj" : $(SOURCE) $(DEP_CPP_METRO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MetronomeDlg.sbr" : $(SOURCE) $(DEP_CPP_METRO) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MetronomeDlg.obj" : $(SOURCE) $(DEP_CPP_METRO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MetronomeDlg.sbr" : $(SOURCE) $(DEP_CPP_METRO) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\MetronomeDlg.obj" : $(SOURCE) $(DEP_CPP_METRO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\EditQuantizeDlg.cpp
DEP_CPP_EDITQ=\
	".\src\Resource.h"\
	".\src\EditQuantizeDlg.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\EditQuantizeDlg.obj" : $(SOURCE) $(DEP_CPP_EDITQ) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditQuantizeDlg.obj" : $(SOURCE) $(DEP_CPP_EDITQ) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditQuantizeDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITQ) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditQuantizeDlg.obj" : $(SOURCE) $(DEP_CPP_EDITQ) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditQuantizeDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITQ) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\EditQuantizeDlg.obj" : $(SOURCE) $(DEP_CPP_EDITQ) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\SekaijuFileDlg.cpp
DEP_CPP_SEKAIJUF=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\Resource.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuFileDlg.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\SekaijuFileDlg.obj" : $(SOURCE) $(DEP_CPP_SEKAIJUF) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\SekaijuFileDlg.obj" : $(SOURCE) $(DEP_CPP_SEKAIJUF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\SekaijuFileDlg.sbr" : $(SOURCE) $(DEP_CPP_SEKAIJUF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\SekaijuFileDlg.obj" : $(SOURCE) $(DEP_CPP_SEKAIJUF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\SekaijuFileDlg.sbr" : $(SOURCE) $(DEP_CPP_SEKAIJUF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\SekaijuFileDlg.obj" : $(SOURCE) $(DEP_CPP_SEKAIJUF) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\MusicalScoreTimeScaleView.cpp
DEP_CPP_MUSIC=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\mousewheel.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\ChildFrame.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\ColorfulComboBox.h"\
	".\src\MusicalScoreFrame.h"\
	".\src\MusicalScoreTimeScaleView.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\MusicalScoreTimeScaleView.obj" : $(SOURCE) $(DEP_CPP_MUSIC)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MusicalScoreTimeScaleView.obj" : $(SOURCE) $(DEP_CPP_MUSIC)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MusicalScoreTimeScaleView.sbr" : $(SOURCE) $(DEP_CPP_MUSIC)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MusicalScoreTimeScaleView.obj" : $(SOURCE) $(DEP_CPP_MUSIC)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MusicalScoreTimeScaleView.sbr" : $(SOURCE) $(DEP_CPP_MUSIC)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\MusicalScoreTimeScaleView.obj" : $(SOURCE) $(DEP_CPP_MUSIC)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\MusicalScoreScaleView.cpp
DEP_CPP_MUSICA=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\mousewheel.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\MainFrame.h"\
	".\src\ChildFrame.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\ColorfulComboBox.h"\
	".\src\MusicalScoreFrame.h"\
	".\src\MusicalScoreScaleView.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\MusicalScoreScaleView.obj" : $(SOURCE) $(DEP_CPP_MUSICA) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MusicalScoreScaleView.obj" : $(SOURCE) $(DEP_CPP_MUSICA) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MusicalScoreScaleView.sbr" : $(SOURCE) $(DEP_CPP_MUSICA) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MusicalScoreScaleView.obj" : $(SOURCE) $(DEP_CPP_MUSICA) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MusicalScoreScaleView.sbr" : $(SOURCE) $(DEP_CPP_MUSICA) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\MusicalScoreScaleView.obj" : $(SOURCE) $(DEP_CPP_MUSICA) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\MusicalScoreFrame.cpp
DEP_CPP_MUSICAL=\
	".\src\common.h"\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\Resource.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\MainFrame.h"\
	".\src\ChildFrame.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\ColorfulComboBox.h"\
	".\src\MusicalScoreFrame.h"\
	".\src\MusicalScorePrintView.h"\
	".\src\MusicalScoreScaleView.h"\
	".\src\MusicalScoreTimeScaleView.h"\
	".\src\MusicalScoreTrackScaleView.h"\
	".\src\MusicalScoreTrackTimeView.h"\
	".\src\PropertyNoteDlg.h"\
	".\src\InplaceEdit.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\MusicalScoreFrame.obj" : $(SOURCE) $(DEP_CPP_MUSICAL) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MusicalScoreFrame.obj" : $(SOURCE) $(DEP_CPP_MUSICAL) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MusicalScoreFrame.sbr" : $(SOURCE) $(DEP_CPP_MUSICAL) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MusicalScoreFrame.obj" : $(SOURCE) $(DEP_CPP_MUSICAL) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MusicalScoreFrame.sbr" : $(SOURCE) $(DEP_CPP_MUSICAL) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\MusicalScoreFrame.obj" : $(SOURCE) $(DEP_CPP_MUSICAL) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\Sekaiju.rc
DEP_RSC_SEKAIJU_=\
	".\src\..\res\MainFrame.ico"\
	".\src\..\res\SekaijuType.ico"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\Sekaiju.res" : $(SOURCE) $(DEP_RSC_SEKAIJU_) "$(INTDIR)"
   $(RSC) /l 0x411 /fo"$(INTDIR)/Sekaiju.res" /i "src" /d "NDEBUG" /d "_AFXDLL"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


"$(INTDIR)\Sekaiju.res" : $(SOURCE) $(DEP_RSC_SEKAIJU_) "$(INTDIR)"
   $(RSC) /l 0x411 /fo"$(INTDIR)/Sekaiju.res" /i "src" /d "_DEBUG" /d "_AFXDLL"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


"$(INTDIR)\Sekaiju.res" : $(SOURCE) $(DEP_RSC_SEKAIJU_) "$(INTDIR)"
   $(RSC) /l 0x411 /fo"$(INTDIR)/Sekaiju.res" /i "src" /d "_DEBUG" /d "_AFXDLL"\
 /d "_UNICODE" /d "UNICODE" $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\Sekaiju.res" : $(SOURCE) $(DEP_RSC_SEKAIJU_) "$(INTDIR)"
   $(RSC) /l 0x411 /fo"$(INTDIR)/Sekaiju.res" /i "src" /d "NDEBUG" /d "_AFXDLL"\
 /d "_UNICODE" /d "UNICODE" $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\MIDISyncModeSheet.cpp
DEP_CPP_MIDIS=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	".\src\MIDIInSyncModePage.h"\
	".\src\MIDIOutSyncModePage.h"\
	".\src\MIDISyncModeSheet.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\MIDISyncModeSheet.obj" : $(SOURCE) $(DEP_CPP_MIDIS) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MIDISyncModeSheet.obj" : $(SOURCE) $(DEP_CPP_MIDIS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MIDISyncModeSheet.sbr" : $(SOURCE) $(DEP_CPP_MIDIS) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MIDISyncModeSheet.obj" : $(SOURCE) $(DEP_CPP_MIDIS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MIDISyncModeSheet.sbr" : $(SOURCE) $(DEP_CPP_MIDIS) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\MIDISyncModeSheet.obj" : $(SOURCE) $(DEP_CPP_MIDIS) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\MIDIOutSyncModePage.cpp
DEP_CPP_MIDIOU=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	".\src\MIDIInSyncModePage.h"\
	".\src\MIDIOutSyncModePage.h"\
	".\src\MIDISyncModeSheet.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\MIDIOutSyncModePage.obj" : $(SOURCE) $(DEP_CPP_MIDIOU) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MIDIOutSyncModePage.obj" : $(SOURCE) $(DEP_CPP_MIDIOU) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MIDIOutSyncModePage.sbr" : $(SOURCE) $(DEP_CPP_MIDIOU) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MIDIOutSyncModePage.obj" : $(SOURCE) $(DEP_CPP_MIDIOU) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MIDIOutSyncModePage.sbr" : $(SOURCE) $(DEP_CPP_MIDIOU) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\MIDIOutSyncModePage.obj" : $(SOURCE) $(DEP_CPP_MIDIOU) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\MIDIInSyncModePage.cpp
DEP_CPP_MIDIINSY=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	".\src\MIDIInSyncModePage.h"\
	".\src\MIDIOutSyncModePage.h"\
	".\src\MIDISyncModeSheet.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\MIDIInSyncModePage.obj" : $(SOURCE) $(DEP_CPP_MIDIINSY) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MIDIInSyncModePage.obj" : $(SOURCE) $(DEP_CPP_MIDIINSY) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MIDIInSyncModePage.sbr" : $(SOURCE) $(DEP_CPP_MIDIINSY) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MIDIInSyncModePage.obj" : $(SOURCE) $(DEP_CPP_MIDIINSY) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MIDIInSyncModePage.sbr" : $(SOURCE) $(DEP_CPP_MIDIINSY) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\MIDIInSyncModePage.obj" : $(SOURCE) $(DEP_CPP_MIDIINSY) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\ColorfulCheckListBox.cpp
DEP_CPP_COLOR=\
	".\src\ColorfulCheckListBox.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\ColorfulCheckListBox.obj" : $(SOURCE) $(DEP_CPP_COLOR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\ColorfulCheckListBox.obj" : $(SOURCE) $(DEP_CPP_COLOR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\ColorfulCheckListBox.sbr" : $(SOURCE) $(DEP_CPP_COLOR) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\ColorfulCheckListBox.obj" : $(SOURCE) $(DEP_CPP_COLOR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\ColorfulCheckListBox.sbr" : $(SOURCE) $(DEP_CPP_COLOR) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\ColorfulCheckListBox.obj" : $(SOURCE) $(DEP_CPP_COLOR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\ColorfulComboBox.cpp
DEP_CPP_COLORF=\
	".\src\ColorfulComboBox.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\ColorfulComboBox.obj" : $(SOURCE) $(DEP_CPP_COLORF) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\ColorfulComboBox.obj" : $(SOURCE) $(DEP_CPP_COLORF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\ColorfulComboBox.sbr" : $(SOURCE) $(DEP_CPP_COLORF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\ColorfulComboBox.obj" : $(SOURCE) $(DEP_CPP_COLORF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\ColorfulComboBox.sbr" : $(SOURCE) $(DEP_CPP_COLORF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\ColorfulComboBox.obj" : $(SOURCE) $(DEP_CPP_COLORF) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\OptionSheet.cpp

!IF  "$(CFG)" == "Sekaiju - Win32 Release"

DEP_CPP_OPTIO=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	".\src\GeneralOptionPage.h"\
	".\src\TrackListOption1Page.h"\
	".\src\TrackListOption2Page.h"\
	".\src\PianoRollOptionPage.h"\
	".\src\EventListOptionPage.h"\
	".\src\OptionSheet.h"\
	".\src\Resource.h"\
	".\src\MusicalScoreOptionPage.h"\
	

"$(INTDIR)\OptionSheet.obj" : $(SOURCE) $(DEP_CPP_OPTIO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"

DEP_CPP_OPTIO=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	".\src\GeneralOptionPage.h"\
	".\src\TrackListOption1Page.h"\
	".\src\TrackListOption2Page.h"\
	".\src\PianoRollOptionPage.h"\
	".\src\EventListOptionPage.h"\
	".\src\OptionSheet.h"\
	".\src\Resource.h"\
	".\src\MusicalScoreOptionPage.h"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\OptionSheet.obj" : $(SOURCE) $(DEP_CPP_OPTIO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\OptionSheet.sbr" : $(SOURCE) $(DEP_CPP_OPTIO) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"

DEP_CPP_OPTIO=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	".\src\GeneralOptionPage.h"\
	".\src\TrackListOption1Page.h"\
	".\src\TrackListOption2Page.h"\
	".\src\PianoRollOptionPage.h"\
	".\src\EventListOptionPage.h"\
	".\src\OptionSheet.h"\
	".\src\Resource.h"\
	".\src\MusicalScoreOptionPage.h"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\OptionSheet.obj" : $(SOURCE) $(DEP_CPP_OPTIO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\OptionSheet.sbr" : $(SOURCE) $(DEP_CPP_OPTIO) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"

DEP_CPP_OPTIO=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	".\src\GeneralOptionPage.h"\
	".\src\TrackListOption1Page.h"\
	".\src\TrackListOption2Page.h"\
	".\src\PianoRollOptionPage.h"\
	".\src\EventListOptionPage.h"\
	".\src\OptionSheet.h"\
	".\src\Resource.h"\
	

"$(INTDIR)\OptionSheet.obj" : $(SOURCE) $(DEP_CPP_OPTIO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\GeneralOptionPage.cpp

!IF  "$(CFG)" == "Sekaiju - Win32 Release"

DEP_CPP_GENER=\
	".\src\Resource.h"\
	".\src\GeneralOptionPage.h"\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	

"$(INTDIR)\GeneralOptionPage.obj" : $(SOURCE) $(DEP_CPP_GENER) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"

DEP_CPP_GENER=\
	".\src\Resource.h"\
	".\src\GeneralOptionPage.h"\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	
NODEP_CPP_GENER=\
	".\src\}"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\GeneralOptionPage.obj" : $(SOURCE) $(DEP_CPP_GENER) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\GeneralOptionPage.sbr" : $(SOURCE) $(DEP_CPP_GENER) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"

DEP_CPP_GENER=\
	".\src\Resource.h"\
	".\src\GeneralOptionPage.h"\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\GeneralOptionPage.obj" : $(SOURCE) $(DEP_CPP_GENER) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\GeneralOptionPage.sbr" : $(SOURCE) $(DEP_CPP_GENER) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"

DEP_CPP_GENER=\
	".\src\Resource.h"\
	".\src\GeneralOptionPage.h"\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	

"$(INTDIR)\GeneralOptionPage.obj" : $(SOURCE) $(DEP_CPP_GENER) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\PianoRollOptionPage.cpp
DEP_CPP_PIANOROLLO=\
	".\src\Resource.h"\
	".\src\PianoRollOptionPage.h"\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\PianoRollOptionPage.obj" : $(SOURCE) $(DEP_CPP_PIANOROLLO)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\PianoRollOptionPage.obj" : $(SOURCE) $(DEP_CPP_PIANOROLLO)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\PianoRollOptionPage.sbr" : $(SOURCE) $(DEP_CPP_PIANOROLLO)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\PianoRollOptionPage.obj" : $(SOURCE) $(DEP_CPP_PIANOROLLO)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\PianoRollOptionPage.sbr" : $(SOURCE) $(DEP_CPP_PIANOROLLO)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\PianoRollOptionPage.obj" : $(SOURCE) $(DEP_CPP_PIANOROLLO)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\EventListOptionPage.cpp
DEP_CPP_EVENTLISTO=\
	".\src\Resource.h"\
	".\src\EventListOptionPage.h"\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\EventListOptionPage.obj" : $(SOURCE) $(DEP_CPP_EVENTLISTO)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EventListOptionPage.obj" : $(SOURCE) $(DEP_CPP_EVENTLISTO)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EventListOptionPage.sbr" : $(SOURCE) $(DEP_CPP_EVENTLISTO)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EventListOptionPage.obj" : $(SOURCE) $(DEP_CPP_EVENTLISTO)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EventListOptionPage.sbr" : $(SOURCE) $(DEP_CPP_EVENTLISTO)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\EventListOptionPage.obj" : $(SOURCE) $(DEP_CPP_EVENTLISTO)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\TrackListOption2Page.cpp
DEP_CPP_TRACKLISTO=\
	".\src\Resource.h"\
	".\src\TrackListOption2Page.h"\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\TrackListOption2Page.obj" : $(SOURCE) $(DEP_CPP_TRACKLISTO)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\TrackListOption2Page.obj" : $(SOURCE) $(DEP_CPP_TRACKLISTO)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\TrackListOption2Page.sbr" : $(SOURCE) $(DEP_CPP_TRACKLISTO)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\TrackListOption2Page.obj" : $(SOURCE) $(DEP_CPP_TRACKLISTO)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\TrackListOption2Page.sbr" : $(SOURCE) $(DEP_CPP_TRACKLISTO)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\TrackListOption2Page.obj" : $(SOURCE) $(DEP_CPP_TRACKLISTO)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\TrackListOption1Page.cpp
DEP_CPP_TRACKLISTOP=\
	".\src\Resource.h"\
	".\src\TrackListOption1Page.h"\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\TrackListOption1Page.obj" : $(SOURCE) $(DEP_CPP_TRACKLISTOP)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\TrackListOption1Page.obj" : $(SOURCE) $(DEP_CPP_TRACKLISTOP)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\TrackListOption1Page.sbr" : $(SOURCE) $(DEP_CPP_TRACKLISTOP)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\TrackListOption1Page.obj" : $(SOURCE) $(DEP_CPP_TRACKLISTOP)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\TrackListOption1Page.sbr" : $(SOURCE) $(DEP_CPP_TRACKLISTOP)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\TrackListOption1Page.obj" : $(SOURCE) $(DEP_CPP_TRACKLISTOP)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\winver.h

!IF  "$(CFG)" == "Sekaiju - Win32 Release"

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\EditBeatScanDlg.cpp
DEP_CPP_EDITB=\
	".\src\Resource.h"\
	".\src\EditBeatScanDlg.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\EditBeatScanDlg.obj" : $(SOURCE) $(DEP_CPP_EDITB) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditBeatScanDlg.obj" : $(SOURCE) $(DEP_CPP_EDITB) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditBeatScanDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITB) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditBeatScanDlg.obj" : $(SOURCE) $(DEP_CPP_EDITB) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditBeatScanDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITB) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\EditBeatScanDlg.obj" : $(SOURCE) $(DEP_CPP_EDITB) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\EditBreakupAndTrillDlg.cpp
DEP_CPP_EDITBR=\
	".\src\Resource.h"\
	".\src\EditBreakupAndTrillDlg.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\EditBreakupAndTrillDlg.obj" : $(SOURCE) $(DEP_CPP_EDITBR)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditBreakupAndTrillDlg.obj" : $(SOURCE) $(DEP_CPP_EDITBR)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditBreakupAndTrillDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITBR)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditBreakupAndTrillDlg.obj" : $(SOURCE) $(DEP_CPP_EDITBR)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditBreakupAndTrillDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITBR)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\EditBreakupAndTrillDlg.obj" : $(SOURCE) $(DEP_CPP_EDITBR)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\AutoSaveDlg.cpp
DEP_CPP_AUTOS=\
	".\src\Resource.h"\
	".\src\AutoSaveDlg.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\AutoSaveDlg.obj" : $(SOURCE) $(DEP_CPP_AUTOS) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\AutoSaveDlg.obj" : $(SOURCE) $(DEP_CPP_AUTOS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\AutoSaveDlg.sbr" : $(SOURCE) $(DEP_CPP_AUTOS) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\AutoSaveDlg.obj" : $(SOURCE) $(DEP_CPP_AUTOS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\AutoSaveDlg.sbr" : $(SOURCE) $(DEP_CPP_AUTOS) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\AutoSaveDlg.obj" : $(SOURCE) $(DEP_CPP_AUTOS) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\MusicalScoreTrackTimeView.cpp
DEP_CPP_MUSICALS=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\mousewheel.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\ChildFrame.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\ColorfulComboBox.h"\
	".\src\MusicalScoreFrame.h"\
	".\src\MusicalScoreTrackTimeView.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\MusicalScoreTrackTimeView.obj" : $(SOURCE) $(DEP_CPP_MUSICALS)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MusicalScoreTrackTimeView.obj" : $(SOURCE) $(DEP_CPP_MUSICALS)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MusicalScoreTrackTimeView.sbr" : $(SOURCE) $(DEP_CPP_MUSICALS)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MusicalScoreTrackTimeView.obj" : $(SOURCE) $(DEP_CPP_MUSICALS)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MusicalScoreTrackTimeView.sbr" : $(SOURCE) $(DEP_CPP_MUSICALS)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\MusicalScoreTrackTimeView.obj" : $(SOURCE) $(DEP_CPP_MUSICALS)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\MusicalScoreTrackScaleView.cpp
DEP_CPP_MUSICALSC=\
	".\src\common.h"\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\mousewheel.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\ChildFrame.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\ColorfulComboBox.h"\
	".\src\MusicalScoreFrame.h"\
	".\src\MusicalScoreTrackScaleView.h"\
	".\src\Resource.h"\
	".\src\InplaceEdit.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\MusicalScoreTrackScaleView.obj" : $(SOURCE) $(DEP_CPP_MUSICALSC)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MusicalScoreTrackScaleView.obj" : $(SOURCE) $(DEP_CPP_MUSICALSC)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MusicalScoreTrackScaleView.sbr" : $(SOURCE) $(DEP_CPP_MUSICALSC)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MusicalScoreTrackScaleView.obj" : $(SOURCE) $(DEP_CPP_MUSICALSC)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MusicalScoreTrackScaleView.sbr" : $(SOURCE) $(DEP_CPP_MUSICALSC)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\MusicalScoreTrackScaleView.obj" : $(SOURCE) $(DEP_CPP_MUSICALSC)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\LanguageDlg.cpp
DEP_CPP_LANGU=\
	".\src\Resource.h"\
	".\src\LanguageDlg.h"\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\LanguageDlg.obj" : $(SOURCE) $(DEP_CPP_LANGU) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\LanguageDlg.obj" : $(SOURCE) $(DEP_CPP_LANGU) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\LanguageDlg.sbr" : $(SOURCE) $(DEP_CPP_LANGU) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\LanguageDlg.obj" : $(SOURCE) $(DEP_CPP_LANGU) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\LanguageDlg.sbr" : $(SOURCE) $(DEP_CPP_LANGU) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\LanguageDlg.obj" : $(SOURCE) $(DEP_CPP_LANGU) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\PropertyNoteDlg.cpp
DEP_CPP_PROPE=\
	".\src\Resource.h"\
	".\src\PropertyNoteDlg.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\PropertyNoteDlg.obj" : $(SOURCE) $(DEP_CPP_PROPE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\PropertyNoteDlg.obj" : $(SOURCE) $(DEP_CPP_PROPE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\PropertyNoteDlg.sbr" : $(SOURCE) $(DEP_CPP_PROPE) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\PropertyNoteDlg.obj" : $(SOURCE) $(DEP_CPP_PROPE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\PropertyNoteDlg.sbr" : $(SOURCE) $(DEP_CPP_PROPE) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\PropertyNoteDlg.obj" : $(SOURCE) $(DEP_CPP_PROPE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\EditRemoveMeasureDlg.cpp
DEP_CPP_EDITR=\
	".\src\Resource.h"\
	".\src\EditRemoveMeasureDlg.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\EditRemoveMeasureDlg.obj" : $(SOURCE) $(DEP_CPP_EDITR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditRemoveMeasureDlg.obj" : $(SOURCE) $(DEP_CPP_EDITR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditRemoveMeasureDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITR) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditRemoveMeasureDlg.obj" : $(SOURCE) $(DEP_CPP_EDITR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditRemoveMeasureDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITR) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\EditRemoveMeasureDlg.obj" : $(SOURCE) $(DEP_CPP_EDITR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\EditInsertMeasureDlg.cpp
DEP_CPP_EDITI=\
	".\src\Resource.h"\
	".\src\EditInsertMeasureDlg.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\EditInsertMeasureDlg.obj" : $(SOURCE) $(DEP_CPP_EDITI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditInsertMeasureDlg.obj" : $(SOURCE) $(DEP_CPP_EDITI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditInsertMeasureDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EditInsertMeasureDlg.obj" : $(SOURCE) $(DEP_CPP_EDITI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EditInsertMeasureDlg.sbr" : $(SOURCE) $(DEP_CPP_EDITI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\EditInsertMeasureDlg.obj" : $(SOURCE) $(DEP_CPP_EDITI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\MusicalScoreOptionPage.cpp
DEP_CPP_MUSICALSCO=\
	".\src\Resource.h"\
	".\src\MusicalScoreOptionPage.h"\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\SekaijuApp.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\MusicalScoreOptionPage.obj" : $(SOURCE) $(DEP_CPP_MUSICALSCO)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MusicalScoreOptionPage.obj" : $(SOURCE) $(DEP_CPP_MUSICALSCO)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MusicalScoreOptionPage.sbr" : $(SOURCE) $(DEP_CPP_MUSICALSCO)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MusicalScoreOptionPage.obj" : $(SOURCE) $(DEP_CPP_MUSICALSCO)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MusicalScoreOptionPage.sbr" : $(SOURCE) $(DEP_CPP_MUSICALSCO)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\MusicalScoreOptionPage.obj" : $(SOURCE) $(DEP_CPP_MUSICALSCO)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\PianoRollPrintView.cpp
DEP_CPP_PIANOROLLP=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\mousewheel.h"\
	".\src\ColorfulComboBox.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\MainFrame.h"\
	".\src\ChildFrame.h"\
	".\src\PianoRollFrame.h"\
	".\src\PianoRollPrintView.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\PianoRollPrintView.obj" : $(SOURCE) $(DEP_CPP_PIANOROLLP)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\PianoRollPrintView.obj" : $(SOURCE) $(DEP_CPP_PIANOROLLP)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\PianoRollPrintView.sbr" : $(SOURCE) $(DEP_CPP_PIANOROLLP)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\PianoRollPrintView.obj" : $(SOURCE) $(DEP_CPP_PIANOROLLP)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\PianoRollPrintView.sbr" : $(SOURCE) $(DEP_CPP_PIANOROLLP)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\PianoRollPrintView.obj" : $(SOURCE) $(DEP_CPP_PIANOROLLP)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\EventListPrintView.cpp
DEP_CPP_EVENTLISTP=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\common.h"\
	".\src\mousewheel.h"\
	".\src\ColorfulComboBox.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\ChildFrame.h"\
	".\src\EventListFrame.h"\
	".\src\EventListPrintView.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\EventListPrintView.obj" : $(SOURCE) $(DEP_CPP_EVENTLISTP)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EventListPrintView.obj" : $(SOURCE) $(DEP_CPP_EVENTLISTP)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EventListPrintView.sbr" : $(SOURCE) $(DEP_CPP_EVENTLISTP)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\EventListPrintView.obj" : $(SOURCE) $(DEP_CPP_EVENTLISTP)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\EventListPrintView.sbr" : $(SOURCE) $(DEP_CPP_EVENTLISTP)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\EventListPrintView.obj" : $(SOURCE) $(DEP_CPP_EVENTLISTP)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\TrackListPrintView.cpp
DEP_CPP_TRACKLISTP=\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\common.h"\
	".\src\mousewheel.h"\
	".\src\ColorfulComboBox.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\ChildFrame.h"\
	".\src\TrackListFrame.h"\
	".\src\TrackListPrintView.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\TrackListPrintView.obj" : $(SOURCE) $(DEP_CPP_TRACKLISTP)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\TrackListPrintView.obj" : $(SOURCE) $(DEP_CPP_TRACKLISTP)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\TrackListPrintView.sbr" : $(SOURCE) $(DEP_CPP_TRACKLISTP)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\TrackListPrintView.obj" : $(SOURCE) $(DEP_CPP_TRACKLISTP)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\TrackListPrintView.sbr" : $(SOURCE) $(DEP_CPP_TRACKLISTP)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\TrackListPrintView.obj" : $(SOURCE) $(DEP_CPP_TRACKLISTP)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\MusicalScorePrintView.cpp
DEP_CPP_MUSICALSCOR=\
	".\src\common.h"\
	".\src\..\\..\\MIDIIO\\MIDIIO.h"\
	".\src\..\\..\\MIDIData\\MIDIData.h"\
	".\src\..\\..\\MIDIClock\\MIDIClock.h"\
	".\src\..\\..\\MIDIStatus\\MIDIStatus.h"\
	".\src\..\\..\\MIDIInstrument\\MIDIInstrument.h"\
	".\src\mousewheel.h"\
	".\src\ColorfulComboBox.h"\
	".\src\ColorfulCheckListBox.h"\
	".\src\HistoryRecord.h"\
	".\src\HistoryUnit.h"\
	".\src\SekaijuApp.h"\
	".\src\SekaijuDoc.h"\
	".\src\SekaijuView.h"\
	".\src\SekaijuToolBar.h"\
	".\src\MainFrame.h"\
	".\src\ChildFrame.h"\
	".\src\MusicalScoreFrame.h"\
	".\src\MusicalScorePrintView.h"\
	".\src\Resource.h"\
	

!IF  "$(CFG)" == "Sekaiju - Win32 Release"


"$(INTDIR)\MusicalScorePrintView.obj" : $(SOURCE) $(DEP_CPP_MUSICALSCOR)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Sekaiju - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MusicalScorePrintView.obj" : $(SOURCE) $(DEP_CPP_MUSICALSCOR)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MusicalScorePrintView.sbr" : $(SOURCE) $(DEP_CPP_MUSICALSCOR)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 DebugU"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\MusicalScorePrintView.obj" : $(SOURCE) $(DEP_CPP_MUSICALSCOR)\
 "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MusicalScorePrintView.sbr" : $(SOURCE) $(DEP_CPP_MUSICALSCOR)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Sekaiju - Win32 ReleaseU"


"$(INTDIR)\MusicalScorePrintView.obj" : $(SOURCE) $(DEP_CPP_MUSICALSCOR)\
 "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
