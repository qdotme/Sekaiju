/******************************************************************************/
/*                                                                            */
/*  Sekaiju3.1  read me first                             (C)2002-2012  kuzu  */
/*                                                                            */
/******************************************************************************/

Thank you for downloading Sekaiju3.1. This file includes basic information and
basic notice. Please read this file first. For detail, see also "Sekaiju_en.pdf"
in the "docs" foleder.

### Features ###

"Sekaiju" is free, open source MIDI sequencer software for creating and editing
MIDI data. This software enables you to create music with your MIDI modules or 
synthesizers. Sekaiju also can be used as a powerful MIDI editor.

 * Multiple MIDI data can be opened.
   This is a MDI (Multi Document Interface) application.
 * Can use track list window, piano roll window, event list window, and musical score window, 
   and they are printable.
 * All kind of MIDI events including meta event and system-exclusive event are editable.
 * Unlimited undo and redo.
 * Both TPQN base at the resolution (time-base) from 1 to 960 and 
   SMPTE24/25/29.97/30 base at the resolution 1 to 255 is supported. 
   Time mode and resolution can be changed at any time.
 * Sekaiju sequence file(*.skj) import and export.
 * Cherry sequence file(*.chy) import and export.
 * Standard MIDI file(*.mid) format 0/1/2 import and export.
 * MIDICSV file(*.csv) import and export.
 * Auto back up(from 1 minute to 120 minutes interval).
 * 16 MIDI input ports and 16 MIDI output ports can be used at the same time.
 * Real time recording with metronome, which can be inputted to multiple track.
 * Auto repeat from CC#111 position, which is used as a repeat marker in "RPG Tsukuru" software.
 * Intelligent playback which prevents garbled sound, wherever position you started playing.
 * Synchronized recording which starts at your keyboard playing.
 * MIDI Timing clock for syncronizing may be received and sent.
 * Instrument definitions are compatible with Cakewalk's instrument definition file(*.ins).
 * Only extracting zip file is required to execute. No need to install.
 * All GUI and docoumentations are bilingual (Japanese and English).

### Requirement ###

  * OS            : Microsoft Windows 2000/XP/Vista/7
  * CPU           : about Pentium1GHz(Windows2000/XP), about Core2Duo(WindowsVista/7)
  * Memory        : about 512MB (Windows2000/XP), about 2GB(WindowsVista/7)
  * Hard Disk     : about 20MB
  * Printer       : A4 or A3 color printer (option).
  * Monitor       : Full color display whose area is 1024 * 768 pixel or larger.
  * MIDI Module   : Hardware MIDI module or software MIDI module (VSTi is impossible).
  * MIDI Keyboard : Hardware MIDI keyboard (option).
  * others        : Sound environment (Amplifier, loudspeakers, or headphones).


### Contents ###

Sekiaju3.1
|-license.txt           License text
|-MIDIIO.dll            MIDI Input / Output library DLL [Essential]
|-MIDIData.dll          MIDI Data creating and editing library DLL [Essential]
|-MIDIClock.dll         MIDI Clock measurement library DLL [Essetial]
|-MIDIStatus.dll        MIDI module status keeping library DLL [Essential]
|-MIDIInstrument.dll    MIDI instrument definition file load library DLL [Essential]
|-Sekaiju.exe           Sekaiju Application [Essetial]
|-Sekaiju.exe.manifest  Manifest file for WindowsXP / Vista visual style
|-Sekaiju.ini           Configuration text [Essetial]
|-Sekaiju.mdp           Project work space for Microsoft VisualC++4.0J
|-Sekaiju.mak           C/C++ make file
|-SekaijuEnu.dll        Sekaiju English resouce DLL [Essential]
|-SekaijuJpn.dll        Sekaiju Japanese resouce DLL [Essential]
|-readme.txt            Read me first (Japanese)
|-readme_en.txt         Read me first (English)
|-src                   C/C++ source files, header files, resouce script foleder
| |-AboutDlg.cpp
| |-AboutDlg.h
| |-AutoSaveDlg.cpp
| |-AutoSaveDlg.h
| |-BeatScanDlg.cpp
| |-BeatScanDlg.h
| |-BreakupAndTrillScanDlg.cpp
| |-BreakupAndTrillScanDlg.h
| |-ChildFrame.cpp
| |-ChildFrame.h
| |-ColorfulCheckListBox.cpp
| |-ColorfulCheckListBox.h
| |-ColorfulComboBox.cpp
| |-ColorfulComboBox.h
| |-common.c
| |-common.h
| |-EditChannelDlg.cpp
| |-EditChannelDlg.h
| |-EditDurationDlg.cpp
| |-EditDurationDlg.h
| |-EditInsertMeasureDlg.cpp
| |-EditInsertMeasureDlg.h
| |-EditKeyDlg.cpp
| |-EditKeyDlg.h
| |-EditQuantizeDlg.cpp
| |-EditQuantizeDlg.h
| |-EditRemoveMeasureDlg.cpp
| |-EditRemoveMeasureDlg.h
| |-EditTimeDlg.cpp
| |-EditTimeDlg.h
| |-EditTimeSmpDlg.cpp
| |-EditTimeSmpDlg.h
| |-EditTrackDlg.cpp
| |-EditTrackDlg.h
| |-EditValueDlg.cpp
| |-EditValueDlg.h
| |-EditVelocityDlg.cpp
| |-EditVelocityDlg.h
| |-EventListFrame.cpp
| |-EventListFrame.h
| |-EventListIndexPropertyView.cpp
| |-EventListIndexPropertyView.h
| |-EventListIndexScaleView.cpp
| |-EventListIndexScaleView.h
| |-EventListOptionPage.cpp
| |-EventListOptionPage.h
| |-EventListPrintView.cpp
| |-EventListPrintView.h
| |-EventListPropertyScaleView.cpp
| |-EventListPropertyScaleView.h
| |-EventListScaleView.cpp
| |-EventListScaleView.h
| |-FilePropertyDlg.cpp
| |-FilePropertyDlg.h
| |-HistoryRecord.cpp
| |-HistoryRecord.h
| |-HistoryUnit.cpp
| |-HistoryUnit.h
| |-InplaceEdit.cpp
| |-InplaceEdit.h
| |-InplaceListBox.cpp
| |-InplaceListBox.h
| |-LanguageDlg.cpp
| |-LanguageDlg.h
| |-MainFrame.cpp
| |-MainFrame.h
| |-MetronomeDlg.cpp
| |-MetronomeDlg.h
| |-MIDIDeviceSheet.cpp
| |-MIDIDeviceSheet.h
| |-MIDIInDevicePage.cpp
| |-MIDIInDevicePage.h
| |-MIDIInstDefDrumPage.cpp
| |-MIDIInstDefDrumPage.h
| |-MIDIInstDefNormPage.cpp
| |-MIDIInstDefNormPage.h
| |-MIDIInSyncModePage.cpp
| |-MIDIInSyncModePage.h
| |-MIDIOutDevicePage.cpp
| |-MIDIOutDevicePage.h
| |-MIDIOutSyncModePage.cpp
| |-MIDIOutSyncModePage.h
| |-MIDISyncModeSheet.cpp
| |-MIDISyncModeSheet.h
| |-mousewheel.h
| |-MusicalScoreFrame.cpp
| |-MusicalScoreFrame.h
| |-MusicalScoreOptionPage.cpp
| |-MusicalScoreOptionPage.h
| |-MusicalScorePrintView.cpp
| |-MusicalScorePrintView.h
| |-MusicalScoreTrackScaleView.cpp
| |-MusicalScoreTrackScaleView.h
| |-MusicalScoreTrackTimeView.cpp
| |-MusicalScoreTrackTimeView.h
| |-MusicalScoreScaleView.cpp
| |-MusicalScoreScaleView.h
| |-MusicalTimeScaleView.cpp
| |-MusicalTimeScaleView.h
| |-OptionSheet.cpp
| |-OptionSheet.h
| |-PianoRollFrame.cpp
| |-PianoRollFrame.h
| |-PianoRollKeyScaleView.cpp
| |-PianoRollKeyScaleView.h
| |-PianoRollKeyTimeView.cpp
| |-PianoRollKeyTimeView.h
| |-PianoRollOptionPage.cpp
| |-PianoRollOptionPage.h
| |-PianoRollPrintView.cpp
| |-PianoRollPrintView.h
| |-PianoRollScaleView.cpp
| |-PianoRollScaleView.h
| |-PianoRollTimeScaleView.cpp
| |-PianoRollTimeScaleView.h
| |-PianoRollVelScaleView.cpp
| |-PianoRollVelScaleView.h
| |-PianoRollVelTimeView.cpp
| |-PianoRollVelTimeView.h
| |-PropertyNote.cpp
| |-PropertyNote.h
| |-Resource.h
| |-Sekaiju.rc
| |-SekaijuApp.cpp
| |-SekaijuApp.h
| |-SekaijuDoc.cpp
| |-SekaijuDoc.h
| |-SekaijuDocManager.cpp
| |-SekaijuDocManager.h
| |-SekaijuDocTemplate.cpp
| |-SekaijuDocTemplate.h
| |-SekaijuFileDlg.cpp
| |-SekaijuFileDlg.h
| |-SekaijuToolBar.cpp
| |-SekaijuToolBar.h
| |-SekaijuView.cpp
| |-SekaijuView.h
| |-TrackListFrame.cpp
| |-TrackListFrame.h
| |-TrackListModeScaleView.cpp
| |-TrackListModeScaleView.h
| |-TrackListOption1Page.cpp
| |-TrackListOption1Page.h
| |-TrackListOption2Page.cpp
| |-TrackListOption2Page.h
| |-TrackListPrintView.cpp
| |-TrackListPrintView.h
| |-TrackListScaleView.cpp
| |-TrackListScaleView.h
| |-TrackListTimeScaleView.cpp
| |-TrackListTimeScaleView.h
| |-TrackListTrackModeView.cpp
| |-TrackListTrackModeView.h
| |-TrackListTrackScaleView.cpp
| |-TrackListTrackScaleView.h
| |-TrackListTrackTimeView.cpp
| |-TrackListTrackTimeView.h
| !-WinVer.h
|-res                   Resouce folder (bitmaps, icons, cursors and so on).
| |-Draw.cur
| |-Eraser.cur
| |-EvenList1.bmp
| |-Icon1.ico
| |-Icon2.ico
| |-Line.cur
| |-MainFrame.ico
| |-MusicalScore1.bmp
| |-Pan.cur
| |-PianoRoll1.bmp
| |-ResizeAll.cur
| |-ResizeNS.cur
| |-ResizeWE.cur
| |-Sekaiju.ico
| |-SekaijuType.ico
| |-Select.cur
| |-SelectAdd.cur
| |-SizeAllCopy.cur
| |-Speaker.cur
| |-Toolbar1.bmp
| |-Toolbar2.bmp
| |-TrackList1.bmp
| !-Zoom.cur
|-SekaijuRes            Sekaiju Language Resource folder
| |-SekaijuEnu.rc       Sekaiju Resource script for English
| |-SekaijuJpn.rc       Sekaiju Resource script for Japanese
| |-SekaijuRes.rc       Sekaiju Resource script for All
| |-SekaijuRes.mak      Sekaiju Resource make file
| !-SekaijuRes.mdp      Sekaiju Resource Project work space for Microsoft VisualC++4.0J
|-instrument            Instrument Definition files folder [Essetial]
| |-GM1_GM2.ins         GM1/GM2 instrument definition file
| |-MSGS.ins            Microsoft GS Wavetable Synth instrument definition file [Essetial]
| |-MU1000_MU2000.ins   YAMAHA MU1000/MU2000 instrument definition file
| !-SC-8850.ins         Roland SC-8850 instrument definition file
|-docs                  Documentations folder
| |-Sekaiju.odt         Japanese Documentation (for OpenOffice3.0)
| |-Sekaiju.pdf         Japanese Documentation (for Adobe Reader)
| |-Sekaiju.odt         English Documentation (for OpenOffice3.0)
| !-Sekaiju.pdf         English Documentation (for Adobe Reader)
|-autosave              Auto save folder

  [Essential] : Essential to execute Sekaiju application.


  !NOTE!  Windows's default setup will be configured not to show dll extensition 
  files in the "explorer" or "my computer". In that case, switch on "Show all 
  folders and files" in the "View" tab of the options dialog of the "explorer" or 
  "my computer", whose dialog may be opened from the "Tool" - "Options..." menu. 
  Also Switch off "Hide extension" in the same tab.

  !WARNING!  MSVCRT40.dll and MFC40.dll is essential, too. They are shipped with
  Windows, normally including in "c:\windows\system32\" or "c:\winNT\system32\"
  or "c:\windows\SysWOW64\" folder. If you may not find them, you must download 
  them from somewhere.


### How to install and execute ###

This software has no installer. You may install by following step.

(1) Please extract "Sekaiju3.1.zip" on your hard disk.

  !WARNING!  If you forget to extract, Sekaiju will not be executed.

(2) Please double click "Sekaiju.exe" on "explorer" or "my computer"
  and Sekaiju main window will be opend.

  !WARNING!  You should execute Sekaiju on your local computer.
  Execution from network computer will cause some trouble.

(3) First you must select MIDI In device, MIDI Out device, 
  MIDI Instrument (normal) and MIDI Instrument (drum) to fit
  your using MIDI device or MIDI instrument from "Setup" - "MIDI Device
  and Instrument" menu of Sekaiju.
  Default configuration is, "(None)", "MIDI Mapper", 
  "Microsft GS Wavetable Synth" and "Microsoft GS Wavetable Synth (Drum)". 

  !WARNING!  If you select "(None)" as MIDI Out device, nothing will be sound.

  !NOTE!  MIDI Instrument data is provided by instrument definition file
  in the "instrument" folder. Sekaiju's instrument definition file is compatible 
  with Cakewalk instrument definition file(*.ins). Sekaiju is shipped with 
  instrument definition files for GM1/GM2, YAMAHA MU-1000/2000, Roland SC-8850 
  and Microsoft GS Wavetable Synth. If you use other instruments, please google 
  "Cakewalk instrument definition file" and you will find suitable instrument 
  definition file. Please put it in "insturument" folder, restart Sekaiju, and
  you may select other instrument definition in the dialog.

### How to change language ###

Sekaiju is made in Japan, therefore default GUI language is Japanese.
You may select English language, there is two way to change language. 
(1) is from GUI, (2) is form text editor.

(1) Select "Setup"-"Language" from the menu and select language in the dialog, 
  and restart Sekaiju. If it seems to be character corruption, press [Alt]+[S] - 
  [Alt]+[L],  and you may open the dialog.

(2) Open "Sekaiju.ini" in your text editor, change "Language=Japanese" into
  "Language=English", and execute Sekaiju.

### How to uninstall ###

This software has no uninstaller. You may uninstall by following step.

(1) Delete "Sekaiju3.1" folder on "explorer" or "my computer".

### License ###

(1) This software is released under the terms of GNU LGPL (Lesser General Public 
  License).

(2) This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

(3) This software links following Dinamic Link Library (DLL).
 * SekaijuJpn.dll       (c)2012 kuzu / OpenMIDIProject  License = GNU LGPL
 * SekaijuEnu.dll       (c)2012 kuzu / OpenMIDIProject  License = GNU LGPL
 * MIDIIO.dll           (c)2009 kuzu / OpenMIDIProject  License = GNU LGPL
 * MIDIData.dll         (c)2012 kuzu / OpenMIDIProject  License = GNU LGPL
 * MIDIClock.dll        (c)2009 kuzu / OpenMIDIProject  License = GNU LGPL
 * MIDIStatus.dll       (c)2009 kuzu / OpenMIDIProject  License = GNU LGPL
 * MIDIInstrument.dll   (c)2009 kuzu / OpenMIDIProject  License = GNU LGPL
 * MSVCRT40.dll         (c)1995 Microsoft  License = Proprietary (including in Windows)
 * MFC40.dll            (c)1995 Microsoft  License = Proprietary (including in Windows)

  !Warning! If you lost dll, Sekaiju may not be executed.
  MSVCRT40.dll and MFC40.dll are shipped with Windows, 
  normally including in "c:\windows\system32\" or "c:\winNT\system32" or
  "c:\windows\SysWOW64\" folder. If you may not find them, you must download
  them from somewhere.

### History ###

2007/12/31 Sekaiju0.1alpha (in the development) Release.
 * Released in the meantime, with many bug.

2008/03/31 Sekaiju0.2alpha (in the development) Release.
 * "Edit" - "Cut", "Edit" - "Copy", "Edit" - "Paste", and "Edit - Delete"
   menues are added.
 * Realtime recording is added.
 * Select, Move, Duplicate function is added in the track view window.
 * Select, Move, Duplicate function is added in the piano roll window.
 * Select function is added in the event list window.
 * A bug that Sekaiju forgets to close MIDIDevice is fixed.
 * Released in the meantime, with many bug.

2008/07/26 Sekaiju0.3alpha (in the development) Release.
 * "Edit" - "Undo" and "Edit" - "Redo" menues are added.
 * A bug that Sekaiju plays SMPTE29.97 Base MIDIData abnormally is fixed.
 * A bug that Sekaiju crashes when no MIDIDevice is selected.
 * Released in the meantime, with many bug.

2008/08/14 Sekaiju0.4alpha (in the development) Release.
 * "Edit" - "Modify Event's Time", "Edit" - "Modify Event's Channel", 
   "Edit" - "Modify Event's Key", "Edit" - "Modify Event's Velocity", 
   "Edit" - "Modify Event's Duration", "Edit" - "Modify Event's Value"
   menues are added.
 * "Help" - "readme.txt", "Help" - "License", "Help" - "Documentation", and 
   "Help" - "Project Home Page" menues are added. 
 * A bug that short cut key works during inputting in the text box is fixed.
 * A bug that inputted SysEx message in the event list window causes abnormal 
   analysis and abnormal error message is fixed.
 * A bug that modifying MIDI channel causes destroy MIDIData is fixed.
 * Released in the meantime, with many bug.


2008/08/23 Sekaiju0.5alpha (in the development) Release.
 * "Edi"t - "Modify Event's Track" menu is added.
 * A bug that track view window does not show 256 or larger tracks is fixed.
 * A bug that mouse up on the track view window causes sound panic is fixed.
 * Released in the meantime, with many bug.

2008/10/04 Sekaiju0.6alpha (in the development) Release.
 * Metronome function is added.
 * A bug that Jump to Begin does not work during recording is fixed.
 * Automatic open MIDIData when Sekaiju is executed is added in the option dialog.
 * Automatic start playing when MIDIData is opened is added in the option dialog.
 * Search only available tone during up or down the value of CC#0, CC#32
   and program change is added in the option dialog.
 * Auto save function is added in the option dialog ("autosave" folder is added).
 * Auto save interval may be selected from 1 minutes to 120 minutes 
   in the option dialog.
 * Speed = Slow, Speed = Normal, Speed = Fast may be scaleble as percent 
   in the option dialog.
 * MIDIData playing and recording thread loop interval may be adjustable
   from 1 msec to 100 msec in the option dialog.
 * Dragging on the piano roll window's left keyboard works as key range selector.
 * Dragging on the piano roll window's left scale works as value range selector.
 * Dragging on the piano roll window's above time works as time range selector.
 * Edit - Quantize is added.
 * "Setup" - "Metronome..." menu and "Metronome" dialog are added.
 * "Setup" - "Option..." and "Option" dialog are added.
 * Released in the meantime, with many bug.

2008/11/03 Sekaiju0.7alpha (in the development) Release.
 * Cherry MIDI sequence file (*.chy) load / save is available.
 * Allow or Avoid multiple exec is selectable in the option dialog.
 * Multi doucment interface (MDI) or Single document interface (SDI) is selectable
   in the option dialog.
 * Restore window position.
 * In case that MIDIData(*.skj)(*.mid) is related with Sekaiju application in windows, 
   by double clicking MIDIData on "explorer" or "my computer", Sekaiju will be 
   executed and opened the file.
 * Octave number is selectable, Center 'C'(key=60) is C3 or C4 or C5.
 * Default controll change events and program change events in the new file is added.
 * Selecting measure in the track view window is improved.
 * Selecting track in the piano roll window will be change channel automatically.
 * Piano roll window's left keyboard shows octave number.
 * Selecting track in the event list window will be change channel automatically.
 * A bug that enter key does not works when selecting track or event's kind is fixed.
 * A bug that converting SMF format 0/1/2 causes undo crash and redo crash is fixed.
 * A bug that changing Time mode causes undo crash and redo crash is fixed.
 * A bug that Edit - Time does not works correctly is fixed.
 * Released in the meantime, with many bug.
 
2008/12/13 Sekaiju0.8alpha (in the development) Release.
 * Mouse wheel may be used to scroll view up or down.
 * Ctrl + mouse wheel may be used to move playback position back and forward.
 * Repeat from CC#111 position when auto repeat is availabel ("RPG Tsukuru" method).
 * A bug that Edit-Time does not works correctly is fixed.
 * Track view window shows tempo, time signature, key signature above measure number.
 * Popup menu is available in the track view window by right clicking.
 * Duplicate button is added in the track view window.
 * Short cut key is added in the track view window.
 * Popup menu is available in the piano roll window by right clicking.
 * A bug that using line tool in the piano roll window causes crash is fixed.
 * Short cut key is added in the piano roll window.
 * Popup menu is available in the event list window by right clicking.
 * Duplicate button is added in the event list window.
 * Short cut key is added in the event list window.
 * Cherry sequence file (*.chy) compatibility is imporved (rocognize virtual CC#).
 * Released in the meantime, with many bug.

2008/12/20 Sekaiju0.9alpha (in the development) Release.
 * Track view window shows key after touch, program change, channel after touch and 
   pitch bend's graph.
 * Alt + mouse wheel may be used to up or down cell value.
 * When MIDIData playing at the end, Sekaiju sends not AllSoundOff(CC#120) 
   but AllNoteOff(CC#123) and HoldOff(CC#64).
 * Abolish updating CC#120 to CC#127 in the internal memory.
 * A bug that All output chanels becomes 1 when standard MIDI Files format 1 
   is opened is fixed.
 * Ctrl + mouse wheel may be used even if the focus is main window or toolbars.
 * Released in the meantime, with many bug.
 
2008/12/27 Sekaiju1.0 Release.
 * A bug that SMPTE offset, sequencer sepecific and SysEx (arbitrary) events are 
   not editable in the event list window is fixed.
 * A bug that snap does not works correctly in the piano roll window is fixed.

2009/01/17 Sekaiju1.1 Release.
 * A line tool may be used to input notes in the piano roll window.
 * XF data (extended SMF by YAMAHA)'s Standard MIDI File load / save available.
 * A bug that converting SMF format 0/1/2 causes moving NoteEvents away, undo
   error and redo error is fixed.
 * Optionally, track number may be counted from 0.
 * Optionally, event number may be counted from 0.
 * Microsoft GS Wavetable Synth' s instrument definition file is added and it
   is used as a default instrument definition file.
 * Sekaiju shows a wait cursor when busy.

2009/07/05 Sekaiju1.2 Release.
 * A bug that mouse cursor is blinking in the track view window is fixed.
 * A bug that mouse cursor is blinking in the piano roll window is fixed.
 * A pen or eraser tool is improved when touching overlapped notes in the 
   piano roll window.
 * A bug that Sekaiju sometimes sends double NoteOn when start playing is fixed.
 * MIDI Sync mode dialog is added.
 * Receiving MIDI Sync function is added.
 * Sending MIDI Sync function is added.
 * "Control" - "Speed = Slave to other machine" menu is added.

2009/07/14 Sekaiju1.3 Release.
 * A bug that moving or duplicating notes by using select tool in the piano roll 
   window causes sound panic is fixed.
 * A bug that editing note whose duration is 0 in the piano roll window causes
   some error is fixed.
 * A bug that editing end of track in the event list window causes crash is fixed.
 * A bug that Edit - Modify Event's Time causes crash if including note event whose 
   duration is 0 is fixed.
 * A bug that Edit - Modify Event's Duration causes crash if including note event 
   whose duration is 0 is fixed.
 * A bug that Opning Sekaiju sequence files including Note events whose duration
   is 0 causes some error is fixed.
 * A bug that Opning Standard MIDI files including Note events whose duration
   is 0 causes some error is fixed.
 * A warning dialog that notify including note event whose duration is 0 is added.

2009/09/06 Sekaiju1.4 Release.
 * Clicking above measure number in the track list window will move playback position.
 * Clicking above measure number in the piano roll window will move playback position.
 * Pressing del key will delete selected area in the track view window.
 * Pressing del key will delete selected notes or graphs in the piano roll window.
 * Track select control becomes colorful to fit track's color in the piano roll window.
 * Track select control becomes colorful to fit track's color in the event list window.
 * Default each column's width and each zoom scale of the track view window may be 
   changed in the option dialog.
 * Default each column's width and each zoom scale of the piano roll window may be 
   changed in the option dialog.
 * Default each column's width and each zoom scale of the event list window may be 
   changed in the option dialog.

2009/12/19 Sekaiju1.5 Release.
 * "Edit" - "Select Before current position" is added.
 * "Edit" - "Deselect Before current position" is added.
 * "Edit" - "Select After current position" is added.
 * "Edit" - "Deselect After current position" is added.
 * "Edit" - "Break up notes and make trill" is added.
 * "Edit" - "Scan beat and insert tempo" is added.
 * Auto save dialog is added, which may be opened from "Setup" - "Auto Save..." menu.
 * "Save as CSV or Text" button is added in the track list window, 
   which enables you to export current list as comma separated text(*.csv) button or 
   tab separated text(*.txt).
 * "Save as CSV or Text" button is added in the event list window, 
   which enables you to export current list as comma separated text(*.csv) button or 
   tab separated text(*.txt).
 * A bug that modifying Time+, Key+ and Vel+ causes undo error and redo error is fixed.
 * MIDIDataLibrary is updated to version 2.1.
 * Strings in the source code is moved to the resouce script.
 * Member variable's comments are added in the source code. 

2010/01/03 Sekaiju1.6 Release.
 * Intelligent play is imploved, master volume is also updated when start playing.
 * A view position is remained when zoom up or zoom down button is pressed 
   in the track list window, piano roll window, event list window.
 * Auto page update button is added to the track list window, piano roll window, and
   event list window. Auto page update may be turn on or off manually.
 * Turn on auto page update when start playing is added to the option dialog.
 * When modifying Note.Key by right clicking or by shift + [+][-] key pressing 
   in the event list window, the value will up or down not 10 but 12.
 * Key signature event in the event list window shows key signature name (e.g. C-Major).
 * MIDIStatusLibrary is updated to 0.6.
 * Member variable's comments are added in the source code. 

2010/01/31 Sekaiju1.7 Release.
 * When opening format1 MIDIData including MIDI channel event (note on, note off, 
   key after touch, control change, program change, channel after touch, or pitch bend) 
   in the first track, you may select move these event to the new other track, or
   open without modification but disable editing and saving.
 * When opening format1 MIDIData including tempo, SMPTE offset, time signature, 
   or key signature event in the second or later track, you may select move these
   event to the first track, or open without modification but disable editing and
   saving.
 * During recording, the popup menu item which that doesn't modify MIDIData may be used.
 * Resource is separated from exe file (Sekaiju.exe), 
   and becomes DLLs (SekaijuJpn.dll and SekaijuEnu.dll).
 * User interface may be selected from Japanese or English.
 * Language dialog is added, which may be opend from "Setup" - "Language..." menu.

2010/02/02 Sekaiju1.7 Release Again.
 * A bug that Sekaiju doen't exec in Windows XP SP3 is fixed.

2010/02/28 Sekaiju1.8 Release.
 * A horzontal or vertical scroll bar works by right clicking popup menu.
 * In the piano roll window, a bug that the sounding channel is diffrent from track's specified channel when put or move note is fixed.
 * In the event list window, insert, duplicate, delete event is improved.
 * Insert, duplicate, delete event mode option is added in the event list option dialog, which may be opened from "Setup" - "Option..." menu.
 * MIDIDataLibrary is updated to 2.2.
 * MIDIInstrumentLibary is udpated to 0.5.
   Memory consumption is reduced by a large margin, so more instrument definitions can be loaded safely.

2010/05/04 Sekaiju1.9 Release.

 * A bug that the dialog status of "Modify event's velocity" isn't remained is fixed.
 * A bug that the dialog status of "Modify event's duration" isn't remained is fixed.
 * A bug that the dialog status of "Modify event's value" isn't remained is fixed.
 * A bug that the duration combo box in the piano roll window 
   becomes old value when undo or redo is fixed.
 * A "Note Property" dialog is added to the piano roll window, 
   which can be opened from popup menu's "This event's property...".
 * In the piano roll window, the selecting tool becomes to select events
   inside the rectangle when dragging from left to right, or select events
   inside the rectangle and crossover to the rectangle when dragging from 
   right to left.

2010/06/06 Sekaiju2.0 Release.
 * In the track view window, A cell which part of events are selected is shown as hatching cell,
   and this cell can be moved or duplicated.
 * English version operation manual is added.

2010/07/30 Sekaiju2.1 Release.
 * MIDICSV File format (*.csv) load / save is supported.
 * "Edit" - "Insert measure" and "Remove measure" are added. 
 * A bug that the toolbar's measure signature is shown wrong is fixed.
 * A bug that in "Save As" Dialog the file type is slected diffrent from the file name's extention is fixed.
 * MIDIDataLibrary is updated to 2.3.

2011/02/05 Sekaiju2.2 Release.
 * Musical score window is implemented.
 * A bug that the history breaks when this event's property dialog is closed as OK
   in the piano roll window is fixed.
 * A bug that wrong event's kind is shown after only show this event's kind is turned off
   in the event list window is fixed.

2011/03/06 Sekaiju2.3 Release
 * Piano roll window becomes printable.
 * In the musical score window, some of right click popup menu becomes enabled 
   if the mouse cursor is not on the note.

2011/05/08 Sekaiju2.4 Release
 * Track list window becomes printable.
 * Event list window becomes printable.
 * A bug that wrong pitch bend graph is shown in the track list window is fixed.
 * A bug that program change value can't be incremented or decremented normally is fixed
   (in case patch search function is on).

2011/06/05 Sekaiju2.5 Release
 * Musical score window becomes printable.

2011/11/06 Sekaiju2.6 Release
 * In the musical score window, triplet quaver and triplet semiquaver becomes visible with character.
 * In the musical score window, a note that can't show as a normal note duration becomes shown exactly 
   as possible by using divide and tie.
 * In the event list window, a bug that when out of the available area clicked, 
   sekaiju works abnormally is fixed .
 * A bug that the view focus is not return after horizontal or vertical scroll bar of each window 
   is moved is fixed.
 * A bug that the patch (program change) is not changed correctly when start playing 
   in the middle of the data is fixed. 

2012/01/10 Sekaiju2.7 Release
 * In the musical score window, the buttons that enables to specify note duration directory 
   are added in the toolbar (whole note, half note, quarter note, quarver note, semiquarver note, 
   demisemiquarver note, dotted, triplet).
 * Text related events that includes TAB, CR or LF become visible, editable, and able to save 
   as "\t", "\r" or "\n".
   However, it is not recommended to include TAB, CR or LF in the text related event's strings.
 * On loading from MIDICSV file (*.csv), "\011","\015" and "\012" in the text related event 
   becomes to be inputed as CR, LF, and TAB.
 * On saving as MIDICSV file (*.csv), CR, LF, and TAB in the text related event
   becomes to be outputed as "\011","\015" and "\012".
   Also, LF is used as a line break. 
   Please use text editor or calc software that can recognize
   LF as a line break to see MIDICSV files of Sekaiju2.7 or later.
 * A bug that the patch (program change) is not changed correctly when start playing
   in the middle of the data which includes system exclusive events in the non-first track is fixed. 
 * In the each message box, the multiple-line-strings is arranged. 
   Also, a part of message are modified to easy to understand.

2012/02/10 Sekaiju2.8 Release
 * A bug that a part of Japanese character in the text related event can't be 
   edited or saved correctly is fixed.
 * '\' character in the text related event can be shown or edited as "\\".
 * MIDIDataLibrary is updated to 2.5.

2012/03/04 Sekaiju2.9 Release
 * A whole source code is refactoringed.
 * MIDIIOLibrary is updated to 0.7.
 * MIDIDataLibrary is updated to 2.6.
 * MIDIInstrumentLibrary is updated to 0.6.

2012/04/22 Sekaiju3.0 Release
 * A bug that Sekaiju ends abnormal immediately at the execution in some environment is fixed.
 * MIDIIOLibrary is updated to 0.8.

2012/07/07 Sekaiju3.1 Release
 * In the piano roll window's graph area, a graph snap feature is added, 
   which enables to draw graph at equal interval.
 * In the piano roll window's graph area, a top margin and bottom margin is added,
   which makes easy to input minimum(0) value or maximum(127) value. 
 * In the musical score window, track name can be shown or edited.
 * In the musical score window, some bugs are fixed.


### Contact ###

Mail to (temporary) :  ee65051@yahoo.co.jp
Project home page :  http://openmidiproject.sourceforge.jp/index.html



