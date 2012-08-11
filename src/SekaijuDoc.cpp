//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 世界樹ドキュメントクラス
// (C)2002-2012 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This library is free software; you can redistribute it and/or */
/* modify it under the terms of the GNU Lesser General Public */
/* License as published by the Free Software Foundation; either */
/* version 2.1 of the License, or (at your option) any later version. */

/* This library is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU */
/* Lesser General Public License for more details. */

/* You should have received a copy of the GNU Lesser General Public */
/* License along with this library; if not, write to the Free Software */
/* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#include "winver.h"
#include <afxwin.h>
#include <afxext.h>
#include <afxcmn.h>
#include <afxmt.h>
#include "common.h"
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "ColorfulComboBox.h"
#include "ColorfulCheckListBox.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDocTemplate.h"
#include "SekaijuDoc.h"
#include "SekaijuView.h"
#include "SekaijuToolBar.h"
#include "MainFrame.h"
#include "ChildFrame.h"
#include "FilePropertyDlg.h"
#include "EditTrackDlg.h"
#include "EditTimeDlg.h"
#include "EditTimeSmpDlg.h"
#include "EditChannelDlg.h"
#include "EditKeyDlg.h"
#include "EditVelocityDlg.h"
#include "EditDurationDlg.h"
#include "EditValueDlg.h"
#include "EditBreakupAndTrillDlg.h"
#include "EditQuantizeDlg.h"
#include "EditBeatScanDlg.h"
#include "EditInsertMeasureDlg.h"
#include "EditRemoveMeasureDlg.h"


#include "HistoryRecord.h"
#include "HistoryUnit.h"

#include "TrackListFrame.h"
#include "PianoRollFrame.h"
#include "EventListFrame.h"
#include "MusicalScoreFrame.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE (CSekaijuDoc, CDocument)

// メッセージマップ
BEGIN_MESSAGE_MAP (CSekaijuDoc, CDocument)
	ON_UPDATE_COMMAND_UI (ID_FILE_SAVE, OnUpdateFileSaveUI)
	ON_UPDATE_COMMAND_UI (ID_FILE_SAVE_AS,OnUpdateFileSaveAsUI)
	ON_COMMAND (ID_FILE_PROPERTY, OnFileProperty)
	ON_UPDATE_COMMAND_UI (ID_FILE_PROPERTY,OnUpdateFilePropertyUI)
	ON_COMMAND (ID_EDIT_UNDO, OnEditUndo)

	ON_UPDATE_COMMAND_UI (ID_EDIT_UNDO, OnUpdateEditUndoUI)
	ON_COMMAND (ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI (ID_EDIT_REDO, OnUpdateEditRedoUI)
	ON_COMMAND (ID_EDIT_INITHISTORY, OnEditInitHistory)
	ON_UPDATE_COMMAND_UI (ID_EDIT_INITHISTORY, OnUpdateEditInitHistoryUI)
	ON_COMMAND (ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI (ID_EDIT_CUT, OnUpdateEditCutUI)
	ON_COMMAND (ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI (ID_EDIT_COPY, OnUpdateEditCopyUI)
	ON_COMMAND (ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI (ID_EDIT_PASTE, OnUpdateEditPasteUI)
	ON_COMMAND (ID_EDIT_DELETE, OnEditDelete)
	ON_UPDATE_COMMAND_UI (ID_EDIT_DELETE, OnUpdateEditDeleteUI)
	ON_COMMAND (ID_EDIT_SELECTALL, OnEditSelectAll)
	ON_UPDATE_COMMAND_UI (ID_EDIT_SELECTALL, OnUpdateEditSelectAllUI)
	ON_COMMAND (ID_EDIT_SELECTNONE,OnEditSelectNone)
	ON_UPDATE_COMMAND_UI (ID_EDIT_SELECTNONE, OnUpdateEditSelectNoneUI)
	ON_COMMAND (ID_EDIT_SELECTBEFORE,OnEditSelectBefore)
	ON_UPDATE_COMMAND_UI (ID_EDIT_SELECTBEFORE, OnUpdateEditSelectBeforeUI)
	ON_COMMAND (ID_EDIT_DESELECTBEFORE,OnEditDeselectBefore)
	ON_UPDATE_COMMAND_UI (ID_EDIT_DESELECTBEFORE, OnUpdateEditDeselectBeforeUI)
	ON_COMMAND (ID_EDIT_SELECTAFTER,OnEditSelectAfter)
	ON_UPDATE_COMMAND_UI (ID_EDIT_SELECTAFTER, OnUpdateEditSelectAfterUI)
	ON_COMMAND (ID_EDIT_DESELECTAFTER,OnEditDeselectAfter)
	ON_UPDATE_COMMAND_UI (ID_EDIT_DESELECTAFTER, OnUpdateEditDeselectAfterUI)

	ON_COMMAND (ID_EDIT_TRACK, OnEditTrack)
	ON_UPDATE_COMMAND_UI (ID_EDIT_TRACK, OnUpdateEditTrackUI)
	ON_COMMAND (ID_EDIT_TIME, OnEditTime)
	ON_UPDATE_COMMAND_UI (ID_EDIT_TIME, OnUpdateEditTimeUI)
	ON_COMMAND (ID_EDIT_CHANNEL, OnEditChannel)
	ON_UPDATE_COMMAND_UI (ID_EDIT_CHANNEL, OnUpdateEditChannelUI)
	ON_COMMAND (ID_EDIT_KEY, OnEditKey)
	ON_UPDATE_COMMAND_UI (ID_EDIT_KEY, OnUpdateEditKeyUI)
	ON_COMMAND (ID_EDIT_VELOCITY, OnEditVelocity)
	ON_UPDATE_COMMAND_UI (ID_EDIT_VELOCITY, OnUpdateEditVelocityUI)
	ON_COMMAND (ID_EDIT_DURATION, OnEditDuration)
	ON_UPDATE_COMMAND_UI (ID_EDIT_DURATION, OnUpdateEditDurationUI)
	ON_COMMAND (ID_EDIT_VALUE, OnEditValue)
	ON_UPDATE_COMMAND_UI (ID_EDIT_VALUE, OnUpdateEditValueUI)
	ON_COMMAND (ID_EDIT_QUANTIZE, OnEditQuantize)
	ON_UPDATE_COMMAND_UI (ID_EDIT_QUANTIZE, OnUpdateEditQuantizeUI)
	ON_COMMAND (ID_EDIT_BREAKUPANDTRILL, OnEditBreakupAndTrill)
	ON_UPDATE_COMMAND_UI (ID_EDIT_BREAKUPANDTRILL, OnUpdateEditBreakupAndTrillUI)
	ON_COMMAND (ID_EDIT_BEATSCAN, OnEditBeatScan)
	ON_UPDATE_COMMAND_UI (ID_EDIT_BEATSCAN, OnUpdateEditBeatScanUI)

	ON_COMMAND (ID_EDIT_INSERTMEASURE, OnEditInsertMeasure) // 20100728追加
	ON_UPDATE_COMMAND_UI (ID_EDIT_INSERTMEASURE, OnUpdateEditInsertMeasureUI) // 20100728追加
	ON_COMMAND (ID_EDIT_REMOVEMEASURE, OnEditRemoveMeasure) // 20100728追加
	ON_UPDATE_COMMAND_UI (ID_EDIT_REMOVEMEASURE, OnUpdateEditRemoveMeasureUI) // 20100728追加

	ON_COMMAND (ID_VIEW_REDRAW, OnViewRedraw)
	ON_COMMAND (ID_VIEW_TRACKLIST, OnViewTrackList)
	ON_COMMAND (ID_VIEW_PIANOROLL, OnViewPianoRoll)
	ON_COMMAND (ID_VIEW_EVENTLIST, OnViewEventList)
	ON_COMMAND (ID_VIEW_MUSICALSCORE, OnViewMusicalScore) // 20090629使用不可状態に設定(仮)

	ON_COMMAND (ID_POPUP_SHOWTRACKLIST, OnPopupShowTrackList)
	ON_UPDATE_COMMAND_UI (ID_POPUP_SHOWTRACKLIST, OnUpdatePopupShowTrackListUI) // 20100429追加
	ON_COMMAND (ID_POPUP_SHOWPIANOROLL, OnPopupShowPianoRoll)
	ON_UPDATE_COMMAND_UI (ID_POPUP_SHOWPIANOROLL, OnUpdatePopupShowPianoRollUI) // 20100429追加
	ON_COMMAND (ID_POPUP_SHOWEVENTLIST, OnPopupShowEventList)
	ON_UPDATE_COMMAND_UI (ID_POPUP_SHOWEVENTLIST, OnUpdatePopupShowEventListUI) // 20100429追加
	ON_COMMAND (ID_POPUP_SHOWMUSICALSCORE, OnPopupShowMusicalScore) // 20090629使用不可状態に設定(仮)
	ON_UPDATE_COMMAND_UI (ID_POPUP_SHOWMUSICALSCORE, OnUpdatePopupShowMusicalScoreUI) // 20100429追加 // 20110104修正
	
	ON_COMMAND (ID_POPUP_TRACKINPUTON, OnPopupTrackInputOn)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKINPUTON, OnUpdatePopupTrackInputOnUI)
	ON_COMMAND (ID_POPUP_TRACKINPUTOFF, OnPopupTrackInputOff)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKINPUTOFF, OnUpdatePopupTrackInputOffUI)
	ON_COMMAND (ID_POPUP_TRACKINPUTALL, OnPopupTrackInputAll)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKINPUTALL, OnUpdatePopupTrackInputAllUI)
	ON_COMMAND (ID_POPUP_TRACKOUTPUTON, OnPopupTrackOutputOn)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKOUTPUTON, OnUpdatePopupTrackOutputOnUI)
	ON_COMMAND (ID_POPUP_TRACKOUTPUTOFF, OnPopupTrackOutputOff)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKOUTPUTOFF, OnUpdatePopupTrackOutputOffUI)
	ON_COMMAND (ID_POPUP_TRACKOUTPUTALL, OnPopupTrackOutputAll)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKOUTPUTALL, OnUpdatePopupTrackOutputAllUI)

	ON_COMMAND (ID_POPUP_CUT, OnPopupCut)
	ON_UPDATE_COMMAND_UI (ID_POPUP_CUT, OnUpdatePopupCutUI)
	ON_COMMAND (ID_POPUP_COPY, OnPopupCopy)
	ON_UPDATE_COMMAND_UI (ID_POPUP_COPY, OnUpdatePopupCopyUI)
	ON_COMMAND (ID_POPUP_PASTE, OnPopupPaste)
	ON_UPDATE_COMMAND_UI (ID_POPUP_PASTE, OnUpdatePopupPasteUI)
	
END_MESSAGE_MAP ()


//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CSekaijuDoc::CSekaijuDoc () {
	m_pMIDIClock = NULL;
	m_pMIDIData = NULL;
	m_lOldTime = 0L;
	m_lNewTime = 0L;
	m_bEditLocked = FALSE;
	m_bSaveLocked = FALSE;
	m_lCurHistoryPosition = 0L;
	m_tmFileOpenTime = 0L;
	m_tmLastAutoSaveTime = 0L;
	m_lTempTime = 0;
	m_pTempTrack = NULL;
	m_pTempEvent = NULL;
}

// デストラクタ
CSekaijuDoc::~CSekaijuDoc() {
	// クリティカルセクションのロック
	m_theCriticalSection.Lock ();
	// 履歴の削除
	DeleteAllHistoryUnit ();	
	// MIDIデータの削除
	if (m_pMIDIData) {
		MIDIData_Delete (m_pMIDIData);
	}
	m_pMIDIData = NULL;
	// MIDIクロックの削除
	if (m_pMIDIClock) {
		MIDIClock_Stop (m_pMIDIClock);
		MIDIClock_Delete (m_pMIDIClock);
	}
	m_pMIDIClock = NULL;
	// アプリケーションに旧ドキュメントが消滅したことを伝える。
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	pSekaijuApp->m_pOldDocument = NULL;
	// クリティカルセクションの解除
	m_theCriticalSection.Unlock ();
	// ビューの更新
	UpdateAllViews (NULL);
}



//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// ドキュメントを新規作成するときのオーバーライド
BOOL CSekaijuDoc::OnNewDocument () {
	if (!CDocument::OnNewDocument()) {
		return FALSE;
	}
	// OnNewDocumentがFALSEを返したときの反応テスト
	// TODO:FALSEを返したときでも正常に動作するようにせよ。
	// return FALSE;

	BeginWaitCursor ();
	CString strMsg;
	// クリティカルセクションのロック
	m_theCriticalSection.Lock ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	// 既存MIDIデータの削除
	if (m_pMIDIData != NULL) {
		MIDIData_Delete (m_pMIDIData);
		m_pMIDIData = NULL;
	}
	// 既存MIDIクロックの削除
	if (m_pMIDIClock != NULL) {
		MIDIClock_Delete (m_pMIDIClock);
		m_pMIDIClock = NULL;
	}
	// 新規MIDIデータの作成
	m_pMIDIData = MIDIData_Create (MIDIDATA_FORMAT1, 17, MIDIDATA_TPQNBASE, 120);
	if (m_pMIDIData == NULL) {
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
		VERIFY (strMsg.LoadString (IDS_MIDIDATA_CREATE_FAILED));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return FALSE;
	}
	// 各トラックのデフォルトの属性設定
	long i = 0;
	MIDITrack* pMIDITrack;
	MIDIEvent* pMIDIEvent;
	m_pMIDIData->m_lUser1 = m_lOldTime;
	m_pMIDIData->m_lUser2 = MIDIData_GetFormat (m_pMIDIData) == 1 ? 1 : 0;
	m_pMIDIData->m_lUser3 = 0;
	m_pMIDIData->m_lUserFlag = MIDIDATA_VISIBLE | MIDITRACK_ENABLE;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		MIDITrack_SetInputOn (pMIDITrack, 1);
		MIDITrack_SetInputPort (pMIDITrack, 0);
		MIDITrack_SetInputChannel (pMIDITrack, i == 0 ? - 1 : ((i + 15) % 16));	
		MIDITrack_SetOutputOn (pMIDITrack, 1);
		MIDITrack_SetOutputPort (pMIDITrack, 0);
		MIDITrack_SetOutputChannel (pMIDITrack, i == 0 ? -1 : ((i + 15) % 16));	
		MIDITrack_SetViewMode (pMIDITrack, (i == 10 ? 1 : 0));
		MIDITrack_SetForeColor (pMIDITrack, pSekaijuApp->m_lDefTrackColor[i % 16]);
		MIDITrack_SetBackColor (pMIDITrack, 0x00FFFFFF);
		pMIDITrack->m_lUser1 = 0;
		pMIDITrack->m_lUser2 = 0;
		pMIDITrack->m_lUser3 = 0;
		pMIDITrack->m_lUserFlag = MIDITRACK_VISIBLE | MIDITRACK_ENABLE;
		this->AddDefaultEventToTrack (pMIDITrack, i == 0 ? 0x000B : 0x0007, NULL);
		i++;
	}

	// フラグの初期化
	m_pMIDIData->m_lUserFlag |= MIDIDATA_ALIVE;
	m_pMIDIData->m_lUserFlag &= ~MIDIDATA_DEAD;
	m_pMIDIData->m_lUserFlag &= ~MIDIDATA_RESISTEREDASINSERTED;
	m_pMIDIData->m_lUserFlag &= ~MIDIDATA_RESISTEREDASREMOVED;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		pMIDITrack->m_lUserFlag |= MIDITRACK_ALIVE;
		pMIDITrack->m_lUserFlag &= ~MIDITRACK_DEAD;
		pMIDITrack->m_lUserFlag &= ~MIDITRACK_RESISTEREDASINSERTED;
		pMIDITrack->m_lUserFlag &= ~MIDITRACK_RESISTEREDASREMOVED;
		forEachEvent (pMIDITrack, pMIDIEvent) {
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_RESISTEREDASINSERTED;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_RESISTEREDASREMOVED;
		}
	}
	
	long lTimeMode, lTimeResolution;
	MIDIData_GetTimeBase (m_pMIDIData, &lTimeMode, &lTimeResolution);
	// MIDIクロックの生成
	m_pMIDIClock = MIDIClock_Create (MIDICLOCK_MASTERTPQNBASE, 120, 60000000 / 120);
	if (m_pMIDIClock == NULL) {
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
		VERIFY (strMsg.LoadString (IDS_MIDICLOCK_CREATE_FAILED));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return FALSE;
	}
	// MIDIクロックのタイムモードと速度をアプリケーションに合わせる(20090625追加)
	ApplyAppCurSpeedIndex ();

	// 状態の初期化
	m_lOldTime = 0L;
	m_lNewTime = 0L;
	time (&m_tmFileOpenTime);
	time (&m_tmLastAutoSaveTime);
	m_lCurHistoryPosition = -1;
	// クリティカルセクションの解除
	m_theCriticalSection.Unlock ();
	// ビューの更新
	UpdateAllViews (NULL, 0xFFFFFFFF);
	EndWaitCursor ();
	::Sleep (0);
	return TRUE;
}

// ドキュメントを開く時のオーバーライド
BOOL CSekaijuDoc::OnOpenDocument (LPCTSTR lpszPathName) {
	BeginWaitCursor ();
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	// クリティカルセクションのロック
	m_theCriticalSection.Lock ();
	// 旧MIDIDataはあるはずがないが、もしあれば削除。
	if (m_pMIDIData) {
		MIDIData_Delete (m_pMIDIData);
		m_pMIDIData = NULL;
	}
	// 旧MIDIClockはあるはずもないが、もしあれば削除。
	if (m_pMIDIClock) {
		MIDIClock_Delete (m_pMIDIClock);
		m_pMIDIClock = NULL;
	}
	// アプリケーションオブジェクトへのポインタ取得
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	// MIDIデータの読み込み
	CString strPathName = lpszPathName;
	CString strExt = strPathName.Right (4);
	// 世界樹シーケンスファイル(*.skj)の場合
	if (strExt.CompareNoCase (_T(".skj")) == 0) {
		m_pMIDIData = MIDIData_LoadFromBinary (lpszPathName);
		if (m_pMIDIData == NULL) {
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			CString strFormat;
			VERIFY (strFormat.LoadString (IDS_S_N_FILE_LOAD_FAILED));
			CString strMsg;
			strMsg.Format (strFormat, lpszPathName);
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			return FALSE;
		}
		// フラグの初期化
		m_pMIDIData->m_lUser1 = m_lOldTime;
		m_pMIDIData->m_lUser2 = MIDIData_GetFormat (m_pMIDIData) == 1 ? 1 : 0;
		m_pMIDIData->m_lUser3 = 0;
		m_pMIDIData->m_lUserFlag |= MIDIDATA_VISIBLE | MIDITRACK_ENABLE;
		m_pMIDIData->m_lUserFlag |= MIDIDATA_ALIVE;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_DEAD;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_RESISTEREDASINSERTED;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_RESISTEREDASREMOVED;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			pMIDITrack->m_lUserFlag |= MIDITRACK_ALIVE;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_DEAD;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_RESISTEREDASINSERTED;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_RESISTEREDASREMOVED;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_RESISTEREDASINSERTED;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_RESISTEREDASREMOVED;
			}
		}
	}
	// Cherryシーケンスファイルの場合(*.chy)の場合
	else if (strExt.CompareNoCase (_T(".chy")) == 0) {
		m_pMIDIData = MIDIData_LoadFromCherry (lpszPathName);
		if (m_pMIDIData == NULL) {
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			CString strFormat;
			VERIFY (strFormat.LoadString (IDS_S_N_FILE_LOAD_FAILED));
			CString strMsg;
			strMsg.Format (strFormat, lpszPathName);
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			return FALSE;
		}
		// 各トラックのデフォルトの属性設定
		long i = 0;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDITrack_SetForeColor (pMIDITrack, pSekaijuApp->m_lDefTrackColor[i % 16]);
			pMIDITrack->m_lUser1 = 0;
			pMIDITrack->m_lUser2 = 0;
			pMIDITrack->m_lUser3 = 0;
			pMIDITrack->m_lUserFlag = MIDITRACK_VISIBLE | MIDITRACK_ENABLE;
			i++;
		}
		// フラグの初期化
		m_pMIDIData->m_lUser1 = m_lOldTime;
		m_pMIDIData->m_lUser2 = MIDIData_GetFormat (m_pMIDIData) == 1 ? 1 : 0;
		m_pMIDIData->m_lUser3 = 0;
		m_pMIDIData->m_lUserFlag |= MIDIDATA_VISIBLE | MIDITRACK_ENABLE;
		m_pMIDIData->m_lUserFlag |= MIDIDATA_ALIVE;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_DEAD;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_RESISTEREDASINSERTED;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_RESISTEREDASREMOVED;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			pMIDITrack->m_lUserFlag |= MIDITRACK_ALIVE;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_DEAD;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_RESISTEREDASINSERTED;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_RESISTEREDASREMOVED;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_RESISTEREDASINSERTED;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_RESISTEREDASREMOVED;
			}
		}
	}
	// スタンダードMIDIファイル(*.mid)の場合
	else if (strExt.CompareNoCase (_T(".mid")) == 0 ||
		strExt.CompareNoCase (_T(".vsq")) == 0) {
		m_pMIDIData = MIDIData_LoadFromSMF (lpszPathName);
		if (m_pMIDIData == NULL) {
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			CString strFormat;
			VERIFY (strFormat.LoadString (IDS_S_N_FILE_LOAD_FAILED));
			CString strMsg;
			strMsg.Format (strFormat, lpszPathName);
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			return FALSE;
		}
		// フォーマット0のMIDIデータの場合、フォーマット1に変換する。
		if (MIDIData_GetFormat (m_pMIDIData) == 0) {
			m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_THIS_MIDIDATA_IS_FORMAT0_N_CONVERT_TO_FORMAT1));
			long lRet = AfxMessageBox (strMsg, MB_ICONQUESTION | MB_YESNO);
			m_theCriticalSection.Lock ();
			if (lRet == IDYES) {
				VERIFY (MIDIData_SetFormat (m_pMIDIData, 1));
			}
		}
		// 各トラックのデフォルトの属性設定
		long i = 0;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDITrack_SetForeColor (pMIDITrack, pSekaijuApp->m_lDefTrackColor[i % 16]);
			pMIDITrack->m_lUser1 = 0;
			pMIDITrack->m_lUser2 = 0;
			pMIDITrack->m_lUser3 = 0;
			pMIDITrack->m_lUserFlag = MIDITRACK_VISIBLE | MIDITRACK_ENABLE;
			i++;
		}
		// ノートイベントの結合
		forEachTrack (m_pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsNoteOn (pMIDIEvent)) {
					MIDIEvent_Combine (pMIDIEvent);
				}
			}
		}
		// フラグの初期化
		m_pMIDIData->m_lUser1 = m_lOldTime;
		m_pMIDIData->m_lUser2 = MIDIData_GetFormat (m_pMIDIData) == 1 ? 1 : 0;
		m_pMIDIData->m_lUser3 = 0;
		m_pMIDIData->m_lUserFlag |= MIDIDATA_VISIBLE | MIDITRACK_ENABLE;
		m_pMIDIData->m_lUserFlag |= MIDIDATA_ALIVE;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_DEAD;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_RESISTEREDASINSERTED;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_RESISTEREDASREMOVED;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			pMIDITrack->m_lUserFlag |= MIDITRACK_ALIVE;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_DEAD;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_RESISTEREDASINSERTED;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_RESISTEREDASREMOVED;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_RESISTEREDASINSERTED;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_RESISTEREDASREMOVED;
			}
		}
	}
	// MIDICSVファイル(*.csv)の場合
	else if (strExt.CompareNoCase (_T(".csv")) == 0) {
		m_pMIDIData = MIDIData_LoadFromMIDICSV (lpszPathName);
		if (m_pMIDIData == NULL) {
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			CString strFormat;
			VERIFY (strFormat.LoadString (IDS_S_N_FILE_LOAD_FAILED));
			CString strMsg;
			strMsg.Format (strFormat, lpszPathName);
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			return FALSE;
		}
		// フォーマット0のMIDIデータの場合、フォーマット1に変換する。
		if (MIDIData_GetFormat (m_pMIDIData) == 0) {
			m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_THIS_MIDIDATA_IS_FORMAT0_N_CONVERT_TO_FORMAT1));
			long lRet = AfxMessageBox (strMsg, MB_ICONQUESTION | MB_YESNO);
			m_theCriticalSection.Lock ();
			if (lRet == IDYES) {
				VERIFY (MIDIData_SetFormat (m_pMIDIData, 1));
			}
		}
		// 各トラックのデフォルトの属性設定
		long i = 0;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDITrack_SetForeColor (pMIDITrack, pSekaijuApp->m_lDefTrackColor[i % 16]);
			pMIDITrack->m_lUser1 = 0;
			pMIDITrack->m_lUser2 = 0;
			pMIDITrack->m_lUser3 = 0;
			pMIDITrack->m_lUserFlag = MIDITRACK_VISIBLE | MIDITRACK_ENABLE;
			i++;
		}
		// ノートイベントの結合
		forEachTrack (m_pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsNoteOn (pMIDIEvent)) {
					MIDIEvent_Combine (pMIDIEvent);
				}
			}
		}
		// フラグの初期化
		m_pMIDIData->m_lUser1 = m_lOldTime;
		m_pMIDIData->m_lUser2 = MIDIData_GetFormat (m_pMIDIData) == 1 ? 1 : 0;
		m_pMIDIData->m_lUser3 = 0;
		m_pMIDIData->m_lUserFlag |= MIDIDATA_VISIBLE | MIDITRACK_ENABLE;
		m_pMIDIData->m_lUserFlag |= MIDIDATA_ALIVE;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_DEAD;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_RESISTEREDASINSERTED;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_RESISTEREDASREMOVED;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			pMIDITrack->m_lUserFlag |= MIDITRACK_ALIVE;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_DEAD;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_RESISTEREDASINSERTED;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_RESISTEREDASREMOVED;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_RESISTEREDASINSERTED;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_RESISTEREDASREMOVED;
			}
		}
	}
	// 未定義のフォーマットの場合
	else {
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		CString strFormat;
		VERIFY (strFormat.LoadString (IDS_S_N_INVALID_EXT_N_FILE_LOAD_FAILED));
		CString strMsg;
		strMsg.Format (strFormat, lpszPathName);
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return FALSE;
	}

	long lTimeMode, lTimeResolution;
	MIDIData_GetTimeBase (m_pMIDIData, &lTimeMode, &lTimeResolution);
	// MIDIクロックの生成
	m_pMIDIClock = MIDIClock_Create (lTimeMode, lTimeResolution, 60000000 / 120);
	if (m_pMIDIClock == NULL) {
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_MIDICLOCK_CREATE_FAILED));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return FALSE;
	}
	// MIDIクロックのタイムモードと速度をアプリケーションに合わせる(20090625追加)
	ApplyAppCurSpeedIndex ();

	// 状態の初期化
	m_lOldTime = 0L;
	m_lNewTime = 0L;
	time (&m_tmFileOpenTime);
	time (&m_tmLastAutoSaveTime);
	m_lCurHistoryPosition = -1;

	// フォーマット1の場合のMIDIデータの正当性チェック(1)(20100128追加)
	if (MIDIData_GetFormat (m_pMIDIData) == 1) {
		long lNumErrorEvent = 0;
		CPtrArray theErrorEventArray;
		// 最初のトラックにMIDIチャンネルイベントが混入していないかチェック
		pMIDITrack = MIDIData_GetFirstTrack (m_pMIDIData);
		if (pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsMIDIEvent (pMIDIEvent)) {
					lNumErrorEvent++;
					if (pMIDIEvent->m_pPrevCombinedEvent == NULL) {
						theErrorEventArray.Add (pMIDIEvent);
					}
				}
			}
		}
		// 最初のトラックにMIDIチャンネルイベントが混入していた場合
		if (lNumErrorEvent > 0) {
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			CString strFormat;
			VERIFY (strFormat.LoadString (IDS_D_MIDICHANNELEVENTS_ARE_IN_THE_FIRST_TRACK_ARE_YOU_SURE_TO_REPAIR_THEM));
			CString strMsg;
			strMsg.Format (strFormat, lNumErrorEvent);
			long lRet = AfxMessageBox (strMsg, MB_ICONQUESTION | MB_YESNO);
			BeginWaitCursor ();
			m_theCriticalSection.Lock ();
			// 異常なMIDIチャンネルイベントを他のトラックに移動
			if (lRet == IDYES) {
				long i = 0;
				long j = 0;
				MIDITrack* pNewTrack[16] = {NULL};
				for (i = 0; i < 16; i++) {
					VERIFY (pNewTrack[i] = MIDITrack_Create ());
				}
				for (j = 0; j < theErrorEventArray.GetSize (); j++) {
					MIDIEvent* pErrorEvent =  (MIDIEvent*)(theErrorEventArray.GetAt (j));
					VERIFY (MIDITrack_RemoveEvent (pMIDITrack, pErrorEvent));
					long lChannel = MIDIEvent_GetChannel (pErrorEvent);
					if (0 <= lChannel && lChannel < 16) {
						VERIFY (MIDITrack_InsertEvent (pNewTrack[lChannel], pErrorEvent));
					}
				}
				MIDITrack* pPrevTrack = MIDIData_GetFirstTrack (m_pMIDIData);
				for (i = 0; i < 16; i++) {
					MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pNewTrack[i]);
					if (pLastEvent) {
						if (!MIDIEvent_IsEndofTrack (pLastEvent)) {
							long lLastTime = MIDIEvent_GetTime (pLastEvent);
							VERIFY (MIDITrack_InsertEndofTrack (pNewTrack[i], lLastTime));
						}
						VERIFY (MIDITrack_SetForeColor (pNewTrack[i], pSekaijuApp->m_lDefTrackColor[(i + 1) % 8]));
						VERIFY (MIDITrack_SetInputPort (pNewTrack[i], 0)); // 20120109追加
						VERIFY (MIDITrack_SetInputChannel (pNewTrack[i], i)); // 20120109追加
						VERIFY (MIDITrack_SetOutputPort (pNewTrack[i], 0)); // 20120109追加
						VERIFY (MIDITrack_SetOutputChannel (pNewTrack[i], i)); // 20120109追加
						pNewTrack[i]->m_lUser1 = 0;
						pNewTrack[i]->m_lUser2 = 0;
						pNewTrack[i]->m_lUser3 = 0;
						pNewTrack[i]->m_lUserFlag = MIDITRACK_VISIBLE | MIDITRACK_ENABLE;
						VERIFY (MIDIData_InsertTrackAfter (m_pMIDIData, pNewTrack[i], pPrevTrack));
						pPrevTrack = pNewTrack[i];
					}
				}
				VERIFY (MIDITrack_SetInputPort (pMIDITrack, 0)); // 20120109追加
				VERIFY (MIDITrack_SetInputChannel (pMIDITrack, -1)); // 20120109追加
				VERIFY (MIDITrack_SetOutputPort (pMIDITrack, 0)); // 20120109追加
				VERIFY (MIDITrack_SetOutputChannel (pMIDITrack, -1)); // 20120109追加
			}
			// 異常なまま読み込む場合、編集・保存とも不可とする
			else {
				m_bEditLocked = TRUE;
				m_bSaveLocked = TRUE;
				m_theCriticalSection.Unlock ();
				EndWaitCursor ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_THIS_MIDIDATA_CANT_BE_EDITED_OR_SAVED_BECAUSE_IT_IS_ABNORMAL));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				BeginWaitCursor ();
				m_theCriticalSection.Lock ();
			}
		}
	}

	// フォーマット1の場合のMIDIデータの正当性チェック(2)(20100128追加)
	if (MIDIData_GetFormat (m_pMIDIData) == 1) {
		long i = 0;
		long lNumErrorEvent = 0;
		CPtrArray theErrorEventArray;
		// 2番目以降のトラックにテンポ・SMPTEオフセット・拍子記号・調性記号イベントが混入していないかチェック
		forEachTrack (m_pMIDIData, pMIDITrack) {
			if (i >= 1) {
				forEachEvent (pMIDITrack, pMIDIEvent) {
					if (MIDIEvent_IsTempo (pMIDIEvent) ||
						MIDIEvent_IsSMPTEOffset (pMIDIEvent) ||
						MIDIEvent_IsTimeSignature (pMIDIEvent) ||
						MIDIEvent_IsKeySignature (pMIDIEvent)) {
						lNumErrorEvent++;
						if (pMIDIEvent->m_pPrevCombinedEvent == NULL) {
							theErrorEventArray.Add (pMIDIEvent);
						}
					}
				}
			}
			i++;
		}
		// 2番目以降のトラックにテンポ・SMPTEオフセット・拍子記号・調性記号イベントが混入していた場合
		if (lNumErrorEvent > 0) {
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			CString strFormat;
			VERIFY (strFormat.LoadString (IDS_D_TEMPOEVENTS_ARE_IN_THE_SECOND_TRACK_ARE_YOU_SURE_TO_REPAIR_THEM));
			CString strMsg;
			strMsg.Format (strFormat, lNumErrorEvent);
			long lRet = AfxMessageBox (strMsg, MB_ICONQUESTION | MB_YESNO);
			BeginWaitCursor ();
			m_theCriticalSection.Lock ();
			// 異常なMIDIチャンネルイベントを他のトラックに移動
			if (lRet == IDYES) {
				long j = 0;
				MIDITrack* pMIDITrack = MIDIData_GetFirstTrack (m_pMIDIData);
				for (j = 0; j < theErrorEventArray.GetSize (); j++) {
					MIDIEvent* pErrorEvent = (MIDIEvent*)(theErrorEventArray.GetAt (j));
					MIDITrack* pErrorTrack = (MIDITrack*)MIDIEvent_GetParent (pErrorEvent);
					VERIFY (MIDITrack_RemoveEvent (pErrorTrack, pErrorEvent));
					VERIFY (MIDITrack_InsertEvent (pMIDITrack, pErrorEvent));
				}
			}
			// 異常なまま読み込む場合、編集・保存とも不可とする
			else {
				m_bEditLocked = TRUE;
				m_bSaveLocked = TRUE;
				m_theCriticalSection.Unlock ();
				EndWaitCursor ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_THIS_MIDIDATA_CANT_BE_EDITED_OR_SAVED_BECAUSE_IT_IS_ABNORMAL));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				BeginWaitCursor ();
				m_theCriticalSection.Lock ();
			}
		}
	}

	// ビューの更新
	UpdateAllViews (NULL, 0xFFFFFFFF);

	// 最後に開いたファイル名の更新
	pSekaijuApp->m_strLastOpenFileName[0] = lpszPathName;

	// 自動演奏開始
	if (pSekaijuApp->m_theGeneralOption.m_bOpenPlay == TRUE &&
		pSekaijuApp->m_bPlaying == FALSE) {
		AfxGetMainWnd ()->PostMessage (WM_COMMAND, ID_CONTROL_PLAY, 0);
	}
	
	// クリティカルセクションの解除
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);
	return TRUE;
}

// ドキュメントを保存するときのオーバーライド
BOOL CSekaijuDoc::OnSaveDocument (LPCTSTR lpszPathName) {
	BeginWaitCursor ();
	CString strPathName (lpszPathName);
	CString strExt = strPathName.Right (4);
	CString strMsg1;
	CString strMsg2;
	long lRet = 0;

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	m_theCriticalSection.Lock ();
	// 録音中の場合は録音停止 
	if (pSekaijuApp->m_bRecording) {
		pSekaijuApp->StopRecording (this);
	}

	if (m_bSaveLocked) {
		return FALSE;
	}


	// 世界樹シーケンスファイル(*.skj)の場合
	if (strExt.CompareNoCase (_T(".skj")) == 0) {
		lRet = MIDIData_SaveAsBinary (m_pMIDIData, lpszPathName);
		if (lRet == 0) {
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			::Sleep (0);
			VERIFY (strMsg1.LoadString (IDS_S_N_MIDIDATA_SAVE_FAILED));
			strMsg2.Format (strMsg1, lpszPathName);
			AfxMessageBox (strMsg2, MB_ICONEXCLAMATION);
			return FALSE;
		}
	}
	// Cherryシーケンスファイル(*.chy)の場合
	else if (strExt.CompareNoCase (_T(".chy")) == 0) {
		lRet = MIDIData_SaveAsCherry (m_pMIDIData, lpszPathName);
		if (lRet == 0) {
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			::Sleep (0);
			VERIFY (strMsg1.LoadString (IDS_S_N_MIDIDATA_SAVE_FAILED));
			strMsg2.Format (strMsg1, lpszPathName);
			AfxMessageBox (strMsg2, MB_ICONEXCLAMATION);
			return FALSE;
		}
	}
	// スタンダードMIDIファイル(*.mid)の場合
	else if (strExt.CompareNoCase (_T(".mid")) == 0 ||
		strExt.CompareNoCase (_T(".vsq")) == 0) {
		// MIDIイベントのチャンネルチェック
		long lCount = 0;
		MIDITrack* pMIDITrack;
		MIDIEvent* pMIDIEvent;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
			// 出力先トラック番号が1～16のいずれかに指定されているならば
			if (0 <= lOutputChannel && lOutputChannel < 16) {
				forEachEvent (pMIDITrack, pMIDIEvent) {
					if (MIDIEvent_IsMIDIEvent (pMIDIEvent)) {
						long lChannel = MIDIEvent_GetChannel (pMIDIEvent);
						if (lOutputChannel != lChannel) {
							lCount++;
						}
					}
				}
			}
		}
		if (lCount > 0) {
			VERIFY (strMsg1.LoadString (IDS_D_MIDIEVENTS_CHANNEL_IS_CONFLICTED_N_CONVERT_TO_MIDITRACKS_CHANNEL));
			strMsg2.Format (strMsg1, lCount);
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			lRet = AfxMessageBox (strMsg2, MB_ICONINFORMATION | MB_YESNO);
			BeginWaitCursor ();
			m_theCriticalSection.Lock ();
			if (lRet == IDYES) {
				MIDIEvent* pCloneEvent = NULL;
				CHistoryUnit* pCurHistoryUnit = NULL;
				CString strHistoryName;
				VERIFY (strHistoryName.LoadString (IDS_MODIFY_MIDICHANNEL));
				VERIFY (this->AddHistoryUnit (strHistoryName));
				VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());
				forEachTrack (m_pMIDIData, pMIDITrack) {
					long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
					// 出力先トラック番号が1～16のいずれかに指定されているならば
					if (0 <= lOutputChannel && lOutputChannel < 16) {
						forEachEvent (pMIDITrack, pMIDIEvent) {
							// MIDIチャンネルイベントで前方向に非結合イベントならば
							if (MIDIEvent_IsMIDIEvent (pMIDIEvent) && 
								pMIDIEvent->m_pPrevCombinedEvent == NULL) {
								// トラックの出力チャンネルとMIDIチャンネルが異なっているならば
								if (MIDIEvent_GetChannel (pMIDIEvent) != lOutputChannel) {
									VERIFY (pCurHistoryUnit->AddHistoryRecord 
										(HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
									pCloneEvent = ReplaceMIDIEvent (pMIDIEvent);
									MIDIEvent_SetChannel (pCloneEvent, lOutputChannel);
									VERIFY (pCurHistoryUnit->AddHistoryRecord 
										(HISTORYRECORD_INSERTEVENT, pCloneEvent));
									pMIDIEvent = pCloneEvent;
								}
							}
						}
					}
				}
			}
		}
		// ノートイベントの長さ>0であることのチェック
		lCount = 0;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsNoteOn (pMIDIEvent) && MIDIEvent_IsNote (pMIDIEvent)) {
					long lDuration = MIDIEvent_GetDuration (pMIDIEvent);
					if (lDuration <= 0) {
						lCount++;
					}
				}
			}
		}
		if (lCount > 0) {
			VERIFY (strMsg1.LoadString (IDS_D_MIDIEVENTS_DURATION_IS_LESS_THAN_0_N_DELETE_THESE_MIDIEVENTS));
			strMsg2.Format (strMsg1,lCount);
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			lRet = AfxMessageBox (strMsg2, MB_ICONINFORMATION | MB_YESNO);
			BeginWaitCursor ();
			m_theCriticalSection.Lock ();
			if (lRet == IDYES) {
				CHistoryUnit* pCurHistoryUnit = NULL;
				CString strHistoryName;
				VERIFY (strHistoryName.LoadString (IDS_DELETE_NOTEEVENT));
				VERIFY (this->AddHistoryUnit (strHistoryName));
				VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());
				forEachTrack (m_pMIDIData, pMIDITrack) {
					CPtrArray theEventArray;
					theEventArray.RemoveAll ();
					forEachEvent (pMIDITrack, pMIDIEvent) {
						if (MIDIEvent_IsNoteOn (pMIDIEvent) && 
							MIDIEvent_IsNote (pMIDIEvent)) {
							long lDuration = MIDIEvent_GetDuration (pMIDIEvent);
							if (lDuration <= 0) {
								theEventArray.Add (pMIDIEvent);
							}
						}
					}
					long j;
					for (j = 0; j < theEventArray.GetSize (); j++) {
						pMIDIEvent = (MIDIEvent*)theEventArray.GetAt (j);
						VERIFY (pCurHistoryUnit->AddHistoryRecord
							(HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						MIDITrack_RemoveEvent (pMIDITrack, pMIDIEvent);
					}
				}
			}
		}
		// トラックの最後ではないエンドオブトラックイベントのチェック
		lCount = 0;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsEndofTrack (pMIDIEvent) && pMIDIEvent != pLastEvent) {
					lCount++;
				}
			}
		}
		if (lCount > 0) {
			VERIFY (strMsg1.LoadString (IDS_D_ENDOFTRACKEVENT_IS_NOT_LAST_PLACE_N_DELETE_THESE_ENDOFTRACKEVENTS));
			strMsg2.Format (strMsg1, lCount);
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			lRet = AfxMessageBox (strMsg2, MB_ICONINFORMATION | MB_YESNO);
			BeginWaitCursor ();
			m_theCriticalSection.Lock ();
			if (lRet == IDYES) {
				CHistoryUnit* pCurHistoryUnit = NULL;
				CString strHistoryName;
				VERIFY (strHistoryName.LoadString (IDS_DELETE_ENDOFTRACKEVENT));
				VERIFY (this->AddHistoryUnit (strHistoryName));
				VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());
				forEachTrack (m_pMIDIData, pMIDITrack) {
					MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
					forEachEvent (pMIDITrack, pMIDIEvent) {
						if (MIDIEvent_IsEndofTrack (pMIDIEvent) && pMIDIEvent != pLastEvent) {
							MIDIEvent* pTempEvent = MIDIEvent_GetPrevEvent (pMIDIEvent);
							VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
							VERIFY (MIDITrack_RemoveEvent (pMIDITrack, pMIDIEvent));
							pMIDIEvent = pTempEvent;
						}
					}
				}
			}
		}
		// トラックの最後のエンドオブトラックイベントの付け忘れチェック
		lCount = 0;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			if (!MIDIEvent_IsEndofTrack (pLastEvent)) {
				lCount++;
			}
		}
		if (lCount > 0) {
			VERIFY (strMsg1.LoadString (IDS_D_MIDITRACK_DOESNT_HAVE_ENDOFTRACKEVENT_N_INSERT_ENDOFTRACKEVENTS));
			strMsg2.Format (strMsg1, lCount);
			EndWaitCursor ();
			m_theCriticalSection.Unlock ();
			lRet = AfxMessageBox (strMsg2, MB_ICONINFORMATION | MB_YESNO);
			m_theCriticalSection.Lock ();
			BeginWaitCursor ();
			if (lRet == IDYES) {
				CHistoryUnit* pCurHistoryUnit = NULL;
				CString strHistoryName;
				VERIFY (strHistoryName.LoadString (IDS_ADD_ENDOFTRACKEVENT));
				VERIFY (this->AddHistoryUnit (strHistoryName));
				VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());
				forEachTrack (m_pMIDIData, pMIDITrack) {
					MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
					if (!MIDIEvent_IsEndofTrack (pLastEvent)) {
						long lTime = (pLastEvent ? MIDIEvent_GetTime (pLastEvent) : 0);
						VERIFY (pMIDIEvent = MIDIEvent_CreateEndofTrack (lTime));
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pMIDIEvent));
						VERIFY (MIDITrack_InsertEvent (pMIDITrack, pMIDIEvent));
					}
				}
			}
		}

		// 保存
		lRet = MIDIData_SaveAsSMF (m_pMIDIData, lpszPathName);
		if (lRet == 0) {
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			::Sleep (0);
			VERIFY (strMsg1.LoadString (IDS_S_N_MIDIDATA_SAVE_FAILED));
			strMsg2.Format (strMsg1, lpszPathName);
			AfxMessageBox (strMsg2, MB_ICONEXCLAMATION);
			return FALSE;
		}
	}

	// MIDICSVファイル(*.csv)の場合
	else if (strExt.CompareNoCase (_T(".csv")) == 0) {
		// MIDIイベントのチャンネルチェック
		long lCount = 0;
		MIDITrack* pMIDITrack;
		MIDIEvent* pMIDIEvent;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
			// 出力先トラック番号が1～16のいずれかに指定されているならば
			if (0 <= lOutputChannel && lOutputChannel < 16) {
				forEachEvent (pMIDITrack, pMIDIEvent) {
					if (MIDIEvent_IsMIDIEvent (pMIDIEvent)) {
						long lChannel = MIDIEvent_GetChannel (pMIDIEvent);
						if (lOutputChannel != lChannel) {
							lCount++;
						}
					}
				}
			}
		}
		if (lCount > 0) {
			VERIFY (strMsg1.LoadString (IDS_D_MIDIEVENTS_CHANNEL_IS_CONFLICTED_N_CONVERT_TO_MIDITRACKS_CHANNEL));
			strMsg2.Format (strMsg1, lCount);
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			lRet = AfxMessageBox (strMsg2, MB_ICONINFORMATION | MB_YESNO);
			BeginWaitCursor ();
			m_theCriticalSection.Lock ();
			if (lRet == IDYES) {
				MIDIEvent* pCloneEvent = NULL;
				CHistoryUnit* pCurHistoryUnit = NULL;
				CString strHistoryName;
				VERIFY (strHistoryName.LoadString (IDS_MODIFY_MIDICHANNEL));
				VERIFY (this->AddHistoryUnit (strHistoryName));
				VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());
				forEachTrack (m_pMIDIData, pMIDITrack) {
					long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
					// 出力先トラック番号が1～16のいずれかに指定されているならば
					if (0 <= lOutputChannel && lOutputChannel < 16) {
						forEachEvent (pMIDITrack, pMIDIEvent) {
							// MIDIチャンネルイベントで前方向に非結合イベントならば
							if (MIDIEvent_IsMIDIEvent (pMIDIEvent) && 
								pMIDIEvent->m_pPrevCombinedEvent == NULL) {
								// トラックの出力チャンネルとMIDIチャンネルが異なっているならば
								if (MIDIEvent_GetChannel (pMIDIEvent) != lOutputChannel) {
									VERIFY (pCurHistoryUnit->AddHistoryRecord 
										(HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
									pCloneEvent = ReplaceMIDIEvent (pMIDIEvent);
									MIDIEvent_SetChannel (pCloneEvent, lOutputChannel);
									VERIFY (pCurHistoryUnit->AddHistoryRecord 
										(HISTORYRECORD_INSERTEVENT, pCloneEvent));
									pMIDIEvent = pCloneEvent;
								}
							}
						}
					}
				}
			}
		}
		// ノートイベントの長さ>0であることのチェック
		lCount = 0;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsNoteOn (pMIDIEvent) && MIDIEvent_IsNote (pMIDIEvent)) {
					long lDuration = MIDIEvent_GetDuration (pMIDIEvent);
					if (lDuration <= 0) {
						lCount++;
					}
				}
			}
		}
		if (lCount > 0) {
			VERIFY (strMsg1.LoadString (IDS_D_MIDIEVENTS_DURATION_IS_LESS_THAN_0_N_DELETE_THESE_MIDIEVENTS));
			strMsg2.Format (strMsg1,lCount);
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			lRet = AfxMessageBox (strMsg2, MB_ICONINFORMATION | MB_YESNO);
			BeginWaitCursor ();
			m_theCriticalSection.Lock ();
			if (lRet == IDYES) {
				CHistoryUnit* pCurHistoryUnit = NULL;
				CString strHistoryName;
				VERIFY (strHistoryName.LoadString (IDS_DELETE_NOTEEVENT));
				VERIFY (this->AddHistoryUnit (strHistoryName));
				VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());
				forEachTrack (m_pMIDIData, pMIDITrack) {
					CPtrArray theEventArray;
					theEventArray.RemoveAll ();
					forEachEvent (pMIDITrack, pMIDIEvent) {
						if (MIDIEvent_IsNoteOn (pMIDIEvent) && 
							MIDIEvent_IsNote (pMIDIEvent)) {
							long lDuration = MIDIEvent_GetDuration (pMIDIEvent);
							if (lDuration <= 0) {
								theEventArray.Add (pMIDIEvent);
							}
						}
					}
					long j;
					for (j = 0; j < theEventArray.GetSize (); j++) {
						pMIDIEvent = (MIDIEvent*)theEventArray.GetAt (j);
						VERIFY (pCurHistoryUnit->AddHistoryRecord
							(HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						MIDITrack_RemoveEvent (pMIDITrack, pMIDIEvent);
					}
				}
			}
		}
		// トラックの最後ではないエンドオブトラックイベントのチェック
		lCount = 0;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsEndofTrack (pMIDIEvent) && pMIDIEvent != pLastEvent) {
					lCount++;
				}
			}
		}
		if (lCount > 0) {
			VERIFY (strMsg1.LoadString (IDS_D_ENDOFTRACKEVENT_IS_NOT_LAST_PLACE_N_DELETE_THESE_ENDOFTRACKEVENTS));
			strMsg2.Format (strMsg1, lCount);
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			lRet = AfxMessageBox (strMsg2, MB_ICONINFORMATION | MB_YESNO);
			BeginWaitCursor ();
			m_theCriticalSection.Lock ();
			if (lRet == IDYES) {
				CHistoryUnit* pCurHistoryUnit = NULL;
				CString strHistoryName;
				VERIFY (strHistoryName.LoadString (IDS_DELETE_ENDOFTRACKEVENT));
				VERIFY (this->AddHistoryUnit (strHistoryName));
				VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());
				forEachTrack (m_pMIDIData, pMIDITrack) {
					MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
					forEachEvent (pMIDITrack, pMIDIEvent) {
						if (MIDIEvent_IsEndofTrack (pMIDIEvent) && pMIDIEvent != pLastEvent) {
							MIDIEvent* pTempEvent = MIDIEvent_GetPrevEvent (pMIDIEvent);
							VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
							VERIFY (MIDITrack_RemoveEvent (pMIDITrack, pMIDIEvent));
							pMIDIEvent = pTempEvent;
						}
					}
				}
			}
		}
		// トラックの最後のエンドオブトラックイベントの付け忘れチェック
		lCount = 0;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			if (!MIDIEvent_IsEndofTrack (pLastEvent)) {
				lCount++;
			}
		}
		if (lCount > 0) {
			VERIFY (strMsg1.LoadString (IDS_D_MIDITRACK_DOESNT_HAVE_ENDOFTRACKEVENT_N_INSERT_ENDOFTRACKEVENTS));
			strMsg2.Format (strMsg1, lCount);
			EndWaitCursor ();
			m_theCriticalSection.Unlock ();
			lRet = AfxMessageBox (strMsg2, MB_ICONINFORMATION | MB_YESNO);
			m_theCriticalSection.Lock ();
			BeginWaitCursor ();
			if (lRet == IDYES) {
				CHistoryUnit* pCurHistoryUnit = NULL;
				CString strHistoryName;
				VERIFY (strHistoryName.LoadString (IDS_ADD_ENDOFTRACKEVENT));
				VERIFY (this->AddHistoryUnit (strHistoryName));
				VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());
				forEachTrack (m_pMIDIData, pMIDITrack) {
					MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
					if (!MIDIEvent_IsEndofTrack (pLastEvent)) {
						long lTime = (pLastEvent ? MIDIEvent_GetTime (pLastEvent) : 0);
						VERIFY (pMIDIEvent = MIDIEvent_CreateEndofTrack (lTime));
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pMIDIEvent));
						VERIFY (MIDITrack_InsertEvent (pMIDITrack, pMIDIEvent));
					}
				}
			}
		}

		// 保存
		lRet = MIDIData_SaveAsMIDICSV (m_pMIDIData, lpszPathName);
		if (lRet == 0) {
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			::Sleep (0);
			VERIFY (strMsg1.LoadString (IDS_S_N_MIDIDATA_SAVE_FAILED));
			strMsg2.Format (strMsg1, lpszPathName);
			AfxMessageBox (strMsg2, MB_ICONEXCLAMATION);
			return FALSE;
		}

	}
	// 未定義のフォーマットの場合
	else {
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
		VERIFY (strMsg1.LoadString (IDS_S_N_INVALID_EXT_N_FILE_SAVE_FAILED));
		strMsg2.Format (strMsg1, lpszPathName);
		AfxMessageBox (strMsg2, MB_ICONEXCLAMATION);
		return FALSE;
	}
	SetModifiedFlag (0);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
	UpdateAllViews (NULL, 
		SEKAIJUDOC_MIDIDATACHANGED | SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIEVENTCHANGED);
	EndWaitCursor ();
	return TRUE;
}

// ドキュメントを閉じる時のオーバーライド
void CSekaijuDoc::OnCloseDocument () {
	_RPTF0 (_CRT_WARN, ("CSekaijuDoc::OnCloseDocument ()\n"));
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	m_theCriticalSection.Lock ();
	// 録音中の場合は録音停止 
	// 注意：録音中に突然閉じられた場合、先にCSekaijuDoc::OnSaveDocumentが呼び出されることがある。
	// よって、CSekaijuDoc::OnSaveDocumentの冒頭でStopRecordingを呼び出しておくこと。
	if (pSekaijuApp->m_bRecording) {
		pSekaijuApp->StopRecording (this);
	}
	m_theCriticalSection.Unlock ();
	CDocument::OnCloseDocument ();
	// CDocument::OnCloseDocument ()内でdelete thisされ、その際デストラクタが呼び出される。
}






//------------------------------------------------------------------------------
// CSekaijuDoc クラスの診断
//------------------------------------------------------------------------------

#ifdef _DEBUG
void CSekaijuDoc::AssertValid () const {
	CDocument::AssertValid ();
}

void CSekaijuDoc::Dump (CDumpContext& dc) const {
	CDocument::Dump (dc);
}
#endif


//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// アプリケーションのスピードインデックスを適用する
BOOL CSekaijuDoc::ApplyAppCurSpeedIndex () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	// 同期モードの取得
	long lSyncInputPort = 0;
	long lSyncInputMode = 0;
	pSekaijuApp->GetCurSyncInputPortAndMode (&lSyncInputPort, &lSyncInputMode);
	// タイムベースの取得
	long lTimeMode, lTimeResolution;
	MIDIData_GetTimeBase (m_pMIDIData, &lTimeMode, &lTimeResolution);
	// MIDIClockが動作している場合は強制停止
	MIDIClock_Stop (m_pMIDIClock);
	// MIDIクロックのタイムモードと速度をアプリケーションに合わせる(20090625追加)
	switch (pSekaijuApp->m_lCurSpeedIndex) {
	case 0: // スピード=静止
		MIDIClock_SetTimeBase (m_pMIDIClock, lTimeMode, lTimeResolution);
		MIDIClock_SetSpeed (m_pMIDIClock, 0);
		MIDIClock_SetMIDIInSyncMode (m_pMIDIClock, MIDICLOCK_MASTER);
		break;
	case 1: // スピード=低速
		MIDIClock_SetTimeBase (m_pMIDIClock, lTimeMode, lTimeResolution);
		MIDIClock_SetSpeed (m_pMIDIClock, pSekaijuApp->m_theGeneralOption.m_lSpeedSlow);
		MIDIClock_SetMIDIInSyncMode (m_pMIDIClock, MIDICLOCK_MASTER);
		break;
	case 2: // スピード=標準
		MIDIClock_SetTimeBase (m_pMIDIClock, lTimeMode, lTimeResolution);
		MIDIClock_SetSpeed (m_pMIDIClock, pSekaijuApp->m_theGeneralOption.m_lSpeedNormal);
		MIDIClock_SetMIDIInSyncMode (m_pMIDIClock, MIDICLOCK_MASTER);
		break;
	case 3: // スピード=高速
		MIDIClock_SetTimeBase (m_pMIDIClock, lTimeMode, lTimeResolution);
		MIDIClock_SetSpeed (m_pMIDIClock, pSekaijuApp->m_theGeneralOption.m_lSpeedFast);
		MIDIClock_SetMIDIInSyncMode (m_pMIDIClock, MIDICLOCK_MASTER);
		break;
	case 4: // スピード=他機器にスレーブ
		if (lSyncInputMode == 1) { // MIDIタイミングクロックにスレーブ
			MIDIClock_SetTimeBase (m_pMIDIClock, MIDICLOCK_SLAVEMIDITIMINGCLOCK, lTimeResolution);
			MIDIClock_SetMIDIInSyncMode (m_pMIDIClock, MIDICLOCK_SLAVEMIDITIMINGCLOCK);
		}
		else if (lSyncInputMode == 2) { // SMPTE/MTCにスレーブ
			MIDIClock_SetTimeBase (m_pMIDIClock, MIDICLOCK_SLAVESMPTEMTC, lTimeResolution);
			MIDIClock_SetMIDIInSyncMode (m_pMIDIClock, MIDICLOCK_SLAVESMPTEMTC);
		}
		else {
			ASSERT (FALSE);
			// TODO:
		}
		MIDIClock_SetSpeed (m_pMIDIClock, 10000);
		break;
	}
	return TRUE;
}



// 指定時刻のMIDIステータス(状態)を計算し、引数で指定されたpMIDIStatus[]に格納する。
void CSekaijuDoc::TimeMIDIStatus (long lTargetTime, MIDIStatus* pMIDIStatus[]) {
	MIDITrack* pMIDITrack;
	MIDIEvent* pMIDIEvent;
	lTargetTime = CLIP (0, lTargetTime, 0x7FFFFFFF);
	// 20120106 システムエクスクルーシヴイベントはどこのトラックに挿入されるか未定なので、
	// MIDIチャンネルイベントに先立って計算するように修正。
	// システムエクスクルーシヴイベントによる状態の計算
	forEachTrack (m_pMIDIData, pMIDITrack) {
		if (MIDITrack_GetOutputOn (pMIDITrack)) {
			long lOutputOn = MIDITrack_GetOutputOn (pMIDITrack);
			long lOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
			long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
			if (lOutputOn) {
				forEachEvent (pMIDITrack, pMIDIEvent) {
					long lTime = MIDIEvent_GetTime (pMIDIEvent);
					lTime = CLIP (0, lTime + MIDITrack_GetTimePlus (pMIDITrack), 0x7FFFFFFF);
					if (lTime > lTargetTime) { // 20111010符号
						break;
					}
					// システムエクスクルーシブイベントの場合
					if (MIDIEvent_IsSysExEvent (pMIDIEvent)) {
						BYTE byMsg[256];
						memset (byMsg, 0, sizeof (byMsg));
						long lLen = MIDIEvent_GetLen (pMIDIEvent);
						MIDIEvent_GetData (pMIDIEvent, byMsg, __min (lLen, sizeof (byMsg)));
						// 出力ポートが存在する場合のみ
						if (pMIDIStatus[lOutputPort] != NULL) {
							MIDIStatus_PutMIDIMessage (pMIDIStatus[lOutputPort], byMsg, lLen);
						}
					}
				}
			}
		}
	}
	// MIDIチャンネルイベントによる状態の計算
	forEachTrack (m_pMIDIData, pMIDITrack) {
		if (MIDITrack_GetOutputOn (pMIDITrack)) {
			long lOutputOn = MIDITrack_GetOutputOn (pMIDITrack);
			long lOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
			long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
			if (lOutputOn) {
				forEachEvent (pMIDITrack, pMIDIEvent) {
					long lTime = MIDIEvent_GetTime (pMIDIEvent);
					lTime = CLIP (0, lTime + MIDITrack_GetTimePlus (pMIDITrack), 0x7FFFFFFF);
					if (lTime > lTargetTime) { // 20111010符号
						break;
					}
					// MIDIチャンネルイベントの場合
					if (MIDIEvent_IsMIDIEvent (pMIDIEvent)) {
						BYTE byMsg[256];
						memset (byMsg, 0, sizeof (byMsg));
						long lLen = MIDIEvent_GetLen (pMIDIEvent);
						MIDIEvent_GetData (pMIDIEvent, byMsg, __min (lLen, sizeof (byMsg)));
						// トラックで設定された出力チャンネルの反映
						if (0 <= lOutputChannel && lOutputChannel < 16) {
							byMsg[0] &= 0xF0;
							byMsg[0] |= (BYTE)lOutputChannel;
						}
						// キー+とベロシティ+の反映
						if (MIDIEvent_IsNoteOn (pMIDIEvent)) {
							byMsg[1] = (BYTE)CLIP (0, byMsg[1] + MIDITrack_GetKeyPlus (pMIDITrack), 127);
							byMsg[2] = (BYTE)CLIP (1, byMsg[2] + MIDITrack_GetVelocityPlus (pMIDITrack), 127);
						}
						else if (MIDIEvent_IsNoteOff (pMIDIEvent) || MIDIEvent_IsKeyAftertouch (pMIDIEvent)) {
							byMsg[1] = (BYTE)CLIP (0, byMsg[1] + MIDITrack_GetKeyPlus (pMIDITrack), 127);
						}
						// 出力ポートが存在する場合のみ
						if (pMIDIStatus[lOutputPort] != NULL) {
							MIDIStatus_PutMIDIMessage (pMIDIStatus[lOutputPort], byMsg, lLen);
						}
					}
				}
			}
		}
	}
}

// クリップボードに独自テキストを設定
BOOL CSekaijuDoc::SetClipboardTextPrivate9 (CString& strData) {
	TCHAR* p = NULL;
	HGLOBAL hGlobalMem = NULL;
	if ((hGlobalMem = GlobalAlloc (GHND, strData.GetLength() + 32)) == NULL) {
		//_RPTF0 (_CRT_WARN, "ｸﾞﾛｰﾊﾞﾙﾒﾓﾘ確保不能。ｺﾋﾟｰは失敗しました。\n");
		return FALSE;
	}
	if ((p = (TCHAR*)GlobalLock(hGlobalMem)) == NULL) {
		//_RPTF0 (_CRT_WARN, "ｸﾞﾛｰﾊﾞﾙﾒﾓﾘ固定不能。ｺﾋﾟｰは失敗しました。\n");
		GlobalFree(hGlobalMem);
		return FALSE;
	}
	_tcscpy (p, (LPCTSTR)strData);
	if (GlobalUnlock(hGlobalMem) == TRUE) {
		//まだﾛｯｸされてればTRUE,解放されたorｴﾗｰはFALSE。
		//_RPTF0 (_CRT_WARN, "ｸﾞﾛｰﾊﾞﾙﾒﾓﾘ固定異常。ｺﾋﾟｰは失敗しました。\n");
		GlobalFree(hGlobalMem);
		return FALSE;
	}
	if(::OpenClipboard (AfxGetMainWnd()->m_hWnd) == FALSE) {
		//_RPTF0 (_CRT_WARN, "ｸﾘｯﾌﾟﾎﾞｰﾄﾞｵｰﾌﾟﾝ不能。ｺﾋﾟｰは失敗しました。\n");
		GlobalFree (hGlobalMem);
		return FALSE;
	}
	if (::EmptyClipboard() == FALSE) {
		::CloseClipboard();//先に閉じないとﾀﾞｲｱﾛｸﾞ表示時にいじられる。
		//_RPTF0 (_CRT_WARN, "ｸﾘｯﾌﾟﾎﾞｰﾄﾞ初期化不能。ｺﾋﾟｰは失敗しました。\n");
		GlobalFree (hGlobalMem);
		return FALSE;
	}
	if (::SetClipboardData(CF_PRIVATEFIRST + 9, hGlobalMem) == FALSE) {
		::CloseClipboard();
		//_RPTF0 (_CRT_WARN, "ｸﾘｯﾌﾟﾎﾞｰﾄﾞ転送不能。ｺﾋﾟｰは失敗しました。\n");
		GlobalFree (hGlobalMem);
	}
	CloseClipboard();
	// hGlobalMemはもはやこのﾌﾟﾛｸﾞﾗﾑのものではない。
	// 従って、GlobalFree(hGlobalMem)をかけない。
	return TRUE;
}

// クリップボードの独自テキストを取得
BOOL CSekaijuDoc::GetClipboardTextPrivate9 (CString& strText) {
	if (IsClipboardFormatAvailable (CF_PRIVATEFIRST + 9) == FALSE) {
		//_RPTF0 (_CRT_WARN, "ｸﾘｯﾌﾟﾎﾞｰﾄﾞが有効な形式ではありません。貼り付けに失敗しました。\n");
		return FALSE;
	}
	if (::OpenClipboard (AfxGetMainWnd()->m_hWnd) == FALSE) {
		//_RPTF0 (_CRT_WARN, "ｸﾘｯﾌﾟﾎﾞｰﾄﾞが開けません。貼り付けに失敗しました。\n");
		return FALSE;
	}
	HGLOBAL hData = NULL;
	if ((hData = GetClipboardData(CF_PRIVATEFIRST + 9)) == NULL) {
		CloseClipboard (); // 先に閉じないとﾀﾞｲｱﾛｸﾞ表示中にいじられる。
		//_RPTF0 (_CRT_WARN, "ｸﾘｯﾌﾟﾎﾞｰﾄﾞから転送不能。貼り付けに失敗しました。\n");
		return FALSE;
	}
	TCHAR* p = NULL; // 転送元ﾊﾞｯﾌｧ。
	if ((p = (TCHAR*)GlobalLock(hData)) == NULL) {
		CloseClipboard ();
		//_RPTF0 (_CRT_WARN, "ｸﾞﾛｰﾊﾞﾙﾒﾓﾘ固定不能。貼り付けに失敗しました。\n");
		return FALSE;
	}
	CString strData = p;
	if ((GlobalUnlock (hData)) == TRUE) {
		// まだﾛｯｸされてればTRUE,解放されたorｴﾗｰはFALSE。
		CloseClipboard();
		//_RPTF0 (_CRT_WARN, "ｸﾞﾛｰﾊﾞﾙﾒﾓﾘ固定異常(致命的)。貼り付けに失敗しました。\n");
		return FALSE;
	}
	//重要！プログラミングWindows95(アスキー) P.929　に以下のように記述。
	//『ｸﾘｯﾌﾟﾎﾞｰﾄﾞをｸﾛｰｽﾞする前にﾃﾞｰﾀﾌﾞﾛｯｸのﾛｯｸをはずす。』
	if (::CloseClipboard () == FALSE) {
		//_RPTF0 (_CRT_WARN, "ｸﾘｯﾌﾟﾎﾞｰﾄﾞを閉じられません。しかし処理を続けます。\n");
	}
	strText = strData;
	return TRUE;
}

// クリップボードが独自テキストかどうか調べる
BOOL CSekaijuDoc::IsClipboardTextPrivate9 () {
	if (IsClipboardFormatAvailable (CF_PRIVATEFIRST + 9) == FALSE) {
		return FALSE;
	}
	return TRUE;
}

// コピー用文字列を作成
long CSekaijuDoc::MakeCopyString (CString& strText, long lTempCopyMode, CHistoryUnit* pHistoryUnit) {
	long i, j, k;
	long lFormat;
	long lTimeMode;
	long lTimeResolution;
	long lBeginTime = 0x7FFFFFFF;
	long lEndTime = 0;
	long lBeginTrack = 0x7FFFFFFF;
	long lEndTrack = 0;
	long lCopiedEventCount = 0;
	MIDITrack* pMIDITrack;
	MIDIEvent* pMIDIEvent;

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();

	//m_theCriticalSection.Lock ();
	lFormat = MIDIData_GetFormat (m_pMIDIData);
	MIDIData_GetTimeBase (m_pMIDIData, &lTimeMode, &lTimeResolution);
	i = 0;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		pMIDITrack->m_lUserFlag &= ~MIDITRACK_SELECTED;
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				pMIDITrack->m_lUserFlag |= MIDITRACK_SELECTED;
				if (i < lBeginTrack) {
					lBeginTrack = i;
				}
				lEndTrack = i;
				long lTime = MIDIEvent_GetTime (pMIDIEvent);
				if (lTime < lBeginTime) {
					lBeginTime = lTime;
				}
				if (lTime > lEndTime) {
					lEndTime = lTime;
				}
			}
		}
		i++;
	}
	if (lBeginTime == 0x7FFFFFFF || lBeginTrack == 0x7FFFFFFF) {
		//m_theCriticalSection.Unlock ();
		//AfxMessageBox ("イベントが何も選択されていません。", MB_ICONEXCLAMATION);
		//m_theCriticalSection.Lock ();
		ASSERT (FALSE);
		return 0;
	}
	long lMeasure, lBeat, lTick;
	MIDIData_BreakTime (m_pMIDIData, lBeginTime, &lMeasure, &lBeat, &lTick);
	MIDIData_MakeTime (m_pMIDIData, lMeasure, 0, 0, &lBeginTime);

	// MIDIデータ情報をstrDataに設定
	CString strData;
	CString strTextLine;
	CString strTextTemp;
	
	strTextLine.Format (_T("MDa2 %X %X %X %X %X %X %X %X %X %X\n"),
		(m_pMIDIData->m_lFormat == 0 ? 0 : 2),
		(lTempCopyMode ? lEndTrack - lBeginTrack + 1 : lEndTrack + 1),
		m_pMIDIData->m_lTimeBase,
		m_pMIDIData->m_lReserved1,
		m_pMIDIData->m_lReserved2,
		m_pMIDIData->m_lReserved3,
		m_pMIDIData->m_lReserved4,
		m_pMIDIData->m_lUser1,
		m_pMIDIData->m_lUser2,
		m_pMIDIData->m_lUser3,
		m_pMIDIData->m_lUserFlag);
	strData += strTextLine;

	forEachTrack (m_pMIDIData, pMIDITrack) {
		if (pMIDITrack->m_lUserFlag & MIDITRACK_SELECTED) {
			MIDITrack_CountEvent (pMIDITrack);
			strTextLine.Format (_T("MTr2 %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X\n"),
				pMIDITrack->m_lTempIndex - (lTempCopyMode ? lBeginTrack : 0),
				pMIDITrack->m_lNumEvent,
				pMIDITrack->m_lInputOn,
				pMIDITrack->m_lInputPort,
				pMIDITrack->m_lInputChannel,
				pMIDITrack->m_lOutputOn,
				pMIDITrack->m_lOutputPort,
				pMIDITrack->m_lOutputChannel,
				pMIDITrack->m_lViewMode,
				pMIDITrack->m_lForeColor,
				pMIDITrack->m_lBackColor,
				pMIDITrack->m_lTimePlus,
				pMIDITrack->m_lKeyPlus,
				pMIDITrack->m_lVelocityPlus,
				pMIDITrack->m_lReserved1,
				pMIDITrack->m_lReserved2,
				pMIDITrack->m_lReserved3,
				pMIDITrack->m_lReserved4,
				pMIDITrack->m_lUser1,
				pMIDITrack->m_lUser2,
				pMIDITrack->m_lUser3,
				pMIDITrack->m_lUserFlag);
			j = 0;
			strData += strTextLine;

			forEachEvent (pMIDITrack, pMIDIEvent) {
				strTextLine = _T("");
				if (IsEventSelected (pMIDIEvent)) {
					strTextTemp.Format (_T("MEvt %X %X %X %X"),
						pMIDIEvent->m_lTempIndex,
						pMIDIEvent->m_lTime - lBeginTime,
						pMIDIEvent->m_lKind,
						pMIDIEvent->m_lLen);
					strTextLine += strTextTemp;
					for (k = 0; k < pMIDIEvent->m_lLen; k++) {
						strTextTemp.Format (_T(" %X"), *(pMIDIEvent->m_pData + k));
						strTextLine += strTextTemp;
					}
					strTextTemp.Format (_T(" %X %X %X %X %X %X\n"),
						pMIDIEvent->m_pPrevCombinedEvent ? pMIDIEvent->m_pPrevCombinedEvent->m_lTempIndex : -1,
						pMIDIEvent->m_pNextCombinedEvent ? pMIDIEvent->m_pNextCombinedEvent->m_lTempIndex : -1,
						pMIDIEvent->m_lUser1,
						pMIDIEvent->m_lUser2,
						pMIDIEvent->m_lUser3,
						pMIDIEvent->m_lUserFlag);
					strTextLine += strTextTemp;
					strData += strTextLine;
					lCopiedEventCount++;
				}
				j++;
			}
		}
		i++;
	}

	strText = strData;
	return lCopiedEventCount;
}

// 独自テキストを展開して貼り付け
long CSekaijuDoc::ParsePasteString
(CString& strData, long lBeginTrackIndex, long lBeginTime, CHistoryUnit* pHistoryUnit) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CString strMsg;
	CString strTextLine;
	long lFormat;
	long lNumTrack;
	long lTimeBase;
	long lTimeMode;
	long lTimeResolution;
	long lNumEvent;
	long lTrackIndex;
	long lEventIndex;
	long lTime;
	long lKind;
	long lLen;
	BYTE byData[1024];
	long lPrevCombinedEvent;
	long lNextCombinedEvent;
	long lInputOn;
	long lInputPort;
	long lInputChannel;
	long lOutputOn;
	long lOutputPort;
	long lOutputChannel;
	long lViewMode;
	long lForeColor;
	long lBackColor;
	long lTimePlus;
	long lKeyPlus;
	long lVelocityPlus;
	long lReserved1;
	long lReserved2;
	long lReserved3;
	long lReserved4;
	long lUser1;
	long lUser2;
	long lUser3;
	long lUserFlag;
	TCHAR szType[256];
	long i = 0;
	long j = 0;
	long k = 0;
	long ret = 0;
	long lErrorNo = 0;
	long lDeletedEventCount = 0;
	long lInsertedEventCount = 0;
	MIDITrack* pClipTrack = NULL;
	MIDIEvent* pClipEvent = NULL;
	MIDIData* pClipData = NULL;

	// クリップボードの内容から新しいMIDIDataオブジェクトを作成
	while (1) {
		int n = strData.Find(_T('\n'));
		if (n < 0) {
			break;
		}
		strTextLine = strData.Left (n + 1);
		strData = strData.Mid (n + 1);
		// MDa2行の場合
		if (strTextLine.Left (4) == _T("MDa2")) {
			// 2回以上MDatが現れた場合
			if (pClipData != NULL) {
				break;
			}
			// MDat情報の読み取り
			_stscanf (strTextLine, _T("%s %x %x %x %x %x %x %x %x\n"),
				szType,
				&lFormat, 
				&lNumTrack, 
				&lTimeBase,
				&lReserved1,
				&lReserved2,
				&lReserved3,
				&lReserved4,
				&lUser1, 
				&lUser2, 
				&lUser3, 
				&lUserFlag);
			if (lFormat < 0 || lFormat > 2) {
				VERIFY (strMsg.LoadString (IDS_ABNORMAL_FORMAT_MIDIDATA_IN_THE_CLIPBOARD));
				m_theCriticalSection.Unlock ();
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				m_theCriticalSection.Lock ();
				return -1;
			}
			if (lTimeBase & 0x00008000) {
				lTimeMode = 256 - ((lTimeBase & 0x0000FF00) >> 8);
				lTimeResolution = lTimeBase & 0x000000FF;
			}
			else {
				lTimeMode =  MIDIDATA_TPQNBASE;
				lTimeResolution = lTimeBase & 0x00007FFF;
			}
			if (lTimeMode != MIDIDATA_TPQNBASE &&
				lTimeMode != MIDIDATA_SMPTE24BASE &&
				lTimeMode != MIDIDATA_SMPTE25BASE &&
				lTimeMode != MIDIDATA_SMPTE29BASE &&
				lTimeMode != MIDIDATA_SMPTE30BASE) {
				VERIFY (strMsg.LoadString (IDS_TIMEMODE_OF_CLIPBOARD_IS_ABNORMAL));
				m_theCriticalSection.Unlock ();
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				m_theCriticalSection.Lock ();
				return -1;
			}
			// MIDIDataの生成
			pClipData = MIDIData_Create 
				(2, lNumTrack, lTimeMode, lTimeResolution);
			if (pClipData == NULL) {
				VERIFY (strMsg.LoadString (IDS_INSUFFICIENT_MEMORY));
				m_theCriticalSection.Unlock ();
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				m_theCriticalSection.Lock ();
				return -1;
			}
		}
		// MTr2行の場合
		else if (strTextLine.Left (4) == _T("MTr2")) {
			_stscanf (strTextLine, _T("%s %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x\n"),
				szType, 
				&lTrackIndex, 
				&lNumEvent,
				&lInputOn,
				&lInputPort,
				&lInputChannel,
				&lOutputOn,
				&lOutputPort,
				&lOutputChannel,
				&lViewMode,
				&lForeColor,
				&lBackColor,
				&lTimePlus,
				&lKeyPlus,
				&lVelocityPlus,
				&lReserved1,
				&lReserved2,
				&lReserved3,
				&lReserved4,
				&lUser1,
				&lUser2,
				&lUser3,
				&lUserFlag);
			// 挿入トラックをpClipTrackに決定
			long i = 0;
			forEachTrack (pClipData, pClipTrack) {
				if (i == lTrackIndex) {
					break;
				}
				i++;
			}
		}
		// MEvt行の場合
		else if (strTextLine.Left (4) == _T("MEvt")) {
			if (pClipTrack == NULL) {
				break;
			}
			TCHAR* p = (TCHAR*)(LPCTSTR)strTextLine;
			_stscanf (p, _T("%s %x %x %x %x"),
				szType, &lEventIndex, &lTime, &lKind, &lLen);
			memset (byData, 0, sizeof (byData));
			for (k = 0; k < 5; k++) {
				p = _tcschr (p, _T(' '));
				if (p == NULL) {
					break;
				}
				while (*p == _T(' ')) {
					p++;
				}
			}
			for (k = 0; k < lLen; k++) {
				_stscanf (p, _T("%x"), &byData[k]);
				p = _tcschr (p, ' ');
				if (p == NULL) {
					break;
				}
				while (*p == ' ') {
					p++;
				}
			}

			_stscanf (p, _T("%x %x %x %x %x %d\n"), 
				&lPrevCombinedEvent, &lNextCombinedEvent,
				&lUser1, &lUser2, &lUser3, &lUserFlag);

			// 新しいMIDIイベントの生成
			pClipEvent = MIDIEvent_Create (lTime, lKind, byData, lLen);
			if (pClipEvent == NULL) {
				continue;
			}
			pClipEvent->m_lUserFlag |= MIDIEVENT_SELECTED;
			pClipEvent->m_lTempIndex = lEventIndex;

			// トラックにイベントを挿入
			ret = MIDITrack_InsertEvent (pClipTrack, pClipEvent);
			if (ret <= 0) {
				MIDIEvent_Delete (pClipEvent);
				continue;
			}
			
			// 結合イベントである場合の結合処理(1)
			if (lPrevCombinedEvent != -1) {
				MIDIEvent* pTempEvent;
				forEachEvent (pClipTrack, pTempEvent) {
					if (pTempEvent->m_lTempIndex == lPrevCombinedEvent) {
						pClipEvent->m_pPrevCombinedEvent = pTempEvent;
						pTempEvent->m_pNextCombinedEvent = pClipEvent;
						break;
					}
				}
			}

			// 結合イベントである場合の結合処理(2)
			if (lNextCombinedEvent != -1) {
				MIDIEvent* pTempEvent;
				forEachEvent (pClipTrack, pTempEvent) {
					if (pTempEvent->m_lTempIndex == lNextCombinedEvent) {
						pClipEvent->m_pNextCombinedEvent = pTempEvent;
						pTempEvent->m_pPrevCombinedEvent = pClipEvent;
						break;
					}
				}
			}
		}
	}

	// 現在のMIDIデータのすべてのMIDIイベント選択解除
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pNewMIDIEvent = NULL;
	MIDIEvent* pLastEvent = NULL;
	MIDIEvent* pNewLastEvent = NULL;
	SelectNoObject (pHistoryUnit);

	// 現在のMIDIデータにクリップボードMIDIデータをマージ
	//long lCurTime = MIDIClock_GetTickCount (m_pMIDIClock);
	long lNumClipTrack = MIDIData_CountTrack (pClipData);
	
	// マージ用MIDIデータのフォーマットをこのMIDIデータのフォーマットに合わせる。
	if(m_pMIDIData->m_lFormat== 0) {
		MIDIData_SetFormat (pClipData, 0);
	}

	// マージ用MIDIデータのタイムベースをこのMIDIデータのタイムベースに合わせる。
	MIDIData_GetTimeBase (m_pMIDIData, &lTimeMode, &lTimeResolution);
	if (m_pMIDIData->m_lTimeBase != pClipData->m_lTimeBase) {
		MIDIData_SetTimeBase (pClipData, lTimeMode, lTimeResolution);
	}

	// 必要な数だけトラックを追加する
	if (MIDIData_GetFormat (m_pMIDIData) != 0) {
		long lClipTrackCount = MIDIData_CountTrack (pClipData);
		long lNeedTrackCount = lBeginTrackIndex + lClipTrackCount;
		this->AddTrack (lNeedTrackCount, 0x0002, pHistoryUnit);
	}

	// すべてのトラックのEOTイベントの履歴記録
	forEachTrack (m_pMIDIData, pMIDITrack) {
		pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
		if (pLastEvent) {
			if (MIDIEvent_IsEndofTrack (pLastEvent)) {
				pHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent);
				pNewLastEvent = this->ReplaceMIDIEvent (pLastEvent);
			}
		}
	}

	// マージ用MIDIデータからイベントを除去し、このMIDIデータに挿入
	if (MIDIData_GetFormat (m_pMIDIData) == 0) {
		pMIDITrack = MIDIData_GetFirstTrack (m_pMIDIData);
	}
	else {
		pMIDITrack = GetTrack (lBeginTrackIndex);
	}
	forEachTrack (pClipData, pClipTrack) {
		ASSERT (pMIDITrack);
		pClipEvent = pClipTrack->m_pFirstEvent;
		while (pClipEvent) {
			ret = MIDITrack_RemoveEvent (pClipTrack, pClipEvent);
			ret = MIDIEvent_SetTime (pClipEvent, pClipEvent->m_lTime + lBeginTime);
			ret = MIDITrack_InsertEvent (pMIDITrack, pClipEvent);
			if (ret > 0) {
				VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pClipEvent));
			}
			lInsertedEventCount += ret;
			pClipEvent = pClipTrack->m_pFirstEvent;
		}
		pMIDITrack = pMIDITrack->m_pNextTrack;
	}

	// すべてのトラックのEOTイベントの履歴記録
	forEachTrack (m_pMIDIData, pMIDITrack) {
		pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
		if (pLastEvent) {
			if (MIDIEvent_IsEndofTrack (pLastEvent)) {
				pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pLastEvent);
			}
		}
	}

	// クリップMIDIデータの削除
	MIDIData_Delete (pClipData);
	pClipData = NULL;
	return lInsertedEventCount;
}

// 選択されているイベントの削除
BOOL CSekaijuDoc::DeleteSelectedEvent (CHistoryUnit* pHistoryUnit) {
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDIEvent* pPrevEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		// 削除するので後方から
		pMIDIEvent = MIDITrack_GetLastEvent (pMIDITrack);
		while (pMIDIEvent) {
			pPrevEvent = MIDIEvent_GetPrevEvent (pMIDIEvent);
			// 選択されているならば
			if (this->IsEventSelected (pMIDIEvent)) {
				// 結合イベントの途中でないならば
				if (pMIDIEvent->m_pPrevCombinedEvent == NULL) {
					// 最後のEndofTrackイベントでないならば
					if (!(MIDIEvent_IsEndofTrack (pMIDIEvent) && 
						MIDIEvent_GetNextEvent (pMIDIEvent) == NULL)) {
						pHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent);
						MIDITrack_RemoveEvent (pMIDITrack, pMIDIEvent);
					}
				}
			}
			pMIDIEvent = pPrevEvent;
		}
	}
	return TRUE;
}

// MIDIイベントを選択又は非選択状態にする。
long CSekaijuDoc::SetEventSelected (MIDIEvent* pMIDIEvent, BOOL bSelected) {
	ASSERT (pMIDIEvent);
	ASSERT (bSelected == 0 || bSelected == 1);
	long lCount = 0;
	MIDIEvent* pTempEvent = MIDIEvent_GetFirstCombinedEvent (pMIDIEvent);
	while (pTempEvent) {
		if (bSelected) {
			pTempEvent->m_lUserFlag |= MIDIEVENT_SELECTED;
		}
		else {
			pTempEvent->m_lUserFlag &= ~MIDIEVENT_SELECTED;
		}
		pTempEvent = pTempEvent->m_pNextCombinedEvent;
		lCount++;
	}
	return lCount;
}

// MIDIイベントが選択状態か調べる
long CSekaijuDoc::IsEventSelected (MIDIEvent* pMIDIEvent) {
	MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
	ASSERT (pMIDITrack);
	return pMIDIEvent->m_lUserFlag & MIDIEVENT_SELECTED ? 1 : 0;
}

// すべてのMIDIイベントを非選択状態にし、履歴に記録する。
long CSekaijuDoc::SelectNoObject (CHistoryUnit* pHistoryUnit) {
	long lCount = 0;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDITrack* pCloneTrack = NULL;
	MIDIEvent* pCloneEvent = NULL;
	ASSERT (pHistoryUnit);	

	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (pMIDIEvent->m_pPrevCombinedEvent == NULL) {
				pCloneEvent = this->SelectEvent (pMIDIEvent, 0, pHistoryUnit);
				if (pCloneEvent) {
					pMIDIEvent = pCloneEvent;
					lCount++;
				}
			}
		}
	}
	_RPTF1 (_CRT_WARN, "SelectNoObject returns %d.\n", lCount);
	return lCount;
}

// すべてのMIDIイベントを選択状態にし、履歴に記録する。
long CSekaijuDoc::SelectAllObject (CHistoryUnit* pHistoryUnit) {
	return 1; // TODO;
}

// 指定のMIDIイベントを選択状態又は非選択状態にし、履歴に記録する。
// 選択状態が変化した場合、新しいMIDIイベントを返す。変化しなかった場合、NULLを返す。
MIDIEvent* CSekaijuDoc::SelectEvent (MIDIEvent* pMIDIEvent, long bSelected,
									 CHistoryUnit* pHistoryUnit) {
	ASSERT (pMIDIEvent);
	MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
	ASSERT (pMIDITrack);
	MIDIEvent* pCloneEvent = NULL;
	MIDIEvent* pFirstCombinedEvent = NULL;
	ASSERT (pHistoryUnit);
	// 現状と異なる選択状態を指定をした場合のみ
	if (this->IsEventSelected (pMIDIEvent) != bSelected) {// &&
		//pMIDIEvent->m_pPrevCombinedEvent == NULL) {
		VERIFY (pFirstCombinedEvent = MIDIEvent_GetFirstCombinedEvent (pMIDIEvent));
		VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pFirstCombinedEvent));
		VERIFY (pCloneEvent = this->ReplaceMIDIEvent (pFirstCombinedEvent));
		// 結合イベントを先頭から順に選択状態又は非選択状態にする。
		MIDIEvent* pTempEvent = pCloneEvent;
		while (pTempEvent) {
			if (bSelected) {
				pTempEvent->m_lUserFlag |= MIDIEVENT_SELECTED;
			}
			else {
				pTempEvent->m_lUserFlag &= ~MIDIEVENT_SELECTED;
			}
			pTempEvent = pTempEvent->m_pNextCombinedEvent;
		}
		VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
		return pCloneEvent;
	}
	return NULL;
}

// MIDIトラックを選択状態又は非選択状態にし、履歴に記録する
MIDITrack* CSekaijuDoc::SelectTrack (MIDITrack* pMIDITrack, long bSelected,
									 CHistoryUnit* pHistoryUnit) {
	ASSERT (pMIDITrack);
	ASSERT (pHistoryUnit);
	MIDIEvent* pMIDIEvent = NULL;
	MIDIEvent* pCloneEvent = NULL;
	// トラック内のすべてのイベントを選択状態又は非選択状態にする。
	forEachEvent (pMIDITrack, pMIDIEvent) {
		pCloneEvent = this->SelectEvent (pMIDIEvent, bSelected, pHistoryUnit);
		if (pCloneEvent) {
			pMIDIEvent = pCloneEvent;
		}
	}
	return NULL;
}

// MIDIトラックが選択状態か調べる
long CSekaijuDoc::IsTrackSelected (MIDITrack* pMIDITrack) {
	ASSERT (pMIDITrack);
	// トラック内のすべてのイベントが選択されていれば1、そうでなければ0を返す。
	MIDIEvent* pMIDIEvent = NULL;
	forEachEvent (pMIDITrack, pMIDIEvent) {
		if ((pMIDIEvent->m_lUserFlag & MIDIEVENT_SELECTED) == 0) {
			return 0;
		}
	}
	return 1;
}


// 指定トラックの指定小節を選択状態又は非選択状態にし、履歴に記録する
long CSekaijuDoc::SelectTrackMeasure (MIDITrack* pMIDITrack, long lMeasure, long bSelected,
									  CHistoryUnit* pHistoryUnit) {
	ASSERT (pMIDITrack);
	MIDIEvent* pMIDIEvent;
	MIDIEvent* pCloneEvent;
	// 指定小節の開始タイムと終了タイムを計算
	long lMinTime, lMaxTime;
	MIDIData_MakeTime (m_pMIDIData, lMeasure, 0, 0, &lMinTime);
	MIDIData_MakeTime (m_pMIDIData, lMeasure + 1, 0, 0, &lMaxTime);
	ASSERT (pHistoryUnit);
	// 指定トラックの指定小節内のすべてのMIDIイベントが選択状態又は非選択状態にする
	forEachEvent (pMIDITrack, pMIDIEvent) {
		long lTime = MIDIEvent_GetTime (pMIDIEvent);
		if (lMinTime <= lTime && lTime < lMaxTime) {
			if (pMIDIEvent->m_pPrevCombinedEvent == NULL) { // 20081102:前小節からのノートオフは除外
				pCloneEvent = this->SelectEvent (pMIDIEvent, bSelected, pHistoryUnit);
				if (pCloneEvent) {
					pMIDIEvent = pCloneEvent;
				}
			}
		}
	}
	return 1;
}

// 指定トラックの指定小節が選択状態か調べる(20100517動作仕様修正)
long CSekaijuDoc::IsTrackMeasureSelected (MIDITrack* pMIDITrack, long lMeasure) {
	ASSERT (pMIDITrack);
	MIDIEvent* pMIDIEvent;
	// 指定小節の開始タイムと終了タイムを計算
	long lMinTime, lMaxTime;
	MIDIData_MakeTime (m_pMIDIData, lMeasure, 0, 0, &lMinTime);
	MIDIData_MakeTime (m_pMIDIData, lMeasure + 1, 0, 0, &lMaxTime);
	long lEventNum = 0; // 20100517追加
	long lSelectedEventNum = 0; // 20100517追加
	forEachEvent (pMIDITrack, pMIDIEvent) {
		long lTime = MIDIEvent_GetTime (pMIDIEvent);
		if (lMinTime <= lTime && lTime < lMaxTime) {
			if (pMIDIEvent->m_pPrevCombinedEvent == NULL) { // 20081102:前小節からのノートオフは除外
				lEventNum++;
				if ((pMIDIEvent->m_lUserFlag & MIDIEVENT_SELECTED) != 0) {
					lSelectedEventNum++;
				}
			}
		}
	}
	// イベントが存在しない場合は0を返す。
	if (lEventNum == 0) {
		return 0;
	}
	// 選択されているイベントが存在しない場合は1を返す。
	if (lSelectedEventNum == 0) {
		return 1;
	}
	// 一部のイベントが選択されている場合は2を返す。
	else if (lEventNum > lSelectedEventNum) {
		return 2;
	}
	// すべてのイベントが選択されている場合は3を返す。
	else { // if (lEventNum == lSelectedEventNum) {
		return 3;
	}
}


// 指定インデックスのトラックを取得
MIDITrack* CSekaijuDoc::GetTrack (long lIndex) {
	ASSERT (0 <= lIndex && lIndex < MAXMIDITRACKNUM);
	long i = 0;
	MIDITrack* pMIDITrack = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		if (i == lIndex) {
			return pMIDITrack;
		}
		i++;
	}
	return NULL;	
}

// トラックにデフォルトのイベントを追加する。
// lModeは以下の値をOR
// 0001:トラックにトラック名イベントを挿入
// 0002:トラックにEOTイベントを挿入
// 0004:トラックにコントロールチェンジ・プログラムチェンジを挿入
// 0008:トラックにテンポ・拍子記号・調性記号を挿入(すべてタイム0)
long CSekaijuDoc::AddDefaultEventToTrack (MIDITrack* pMIDITrack, long lMode, CHistoryUnit* pHistoryUnit) {
	ASSERT (pMIDITrack);
	long lCurTrackCount = MIDIData_CountTrack (m_pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (m_pMIDIData);
	long lChannel = MIDITrack_GetOutputChannel (pMIDITrack);
	MIDIEvent* pNewEvent = NULL;
	// トラック名イベントの挿入
	if (lMode & 0x0001) {
		VERIFY (pNewEvent = MIDIEvent_CreateTrackName (lTimeResolution * 0 / 120, _T("")));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
	}
	// テンポ・拍子記号・調性記号
	if (lMode & 0x0008) {
		VERIFY (pNewEvent = MIDIEvent_CreateTempo (0, 60000000 / 120));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateTimeSignature (0, 4, 2, 24, 8));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateKeySignature (0, 0, 0));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
	}
	// コントロールチェンジ・プログラムチェンジの挿入
	if (lMode & 0x0004 && 0 <= lChannel && lChannel <= 15) {
		VERIFY (pNewEvent = MIDIEvent_CreateControlChange (lTimeResolution * 120 / 120, lChannel, 0, 0));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateControlChange (lTimeResolution * 130 / 120, lChannel, 32, 0));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateProgramChange (lTimeResolution * 140 / 120, lChannel, 0));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateControlChange (lTimeResolution * 150 / 120, lChannel, 1, 0));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateControlChange (lTimeResolution * 160 / 120, lChannel, 7, 100));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateControlChange (lTimeResolution * 170 / 120, lChannel, 10, 64));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateControlChange (lTimeResolution * 180 / 120, lChannel, 11, 127));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateControlChange (lTimeResolution * 190 / 120, lChannel, 64, 0));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateControlChange (lTimeResolution * 200 / 120, lChannel, 91, 40));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateControlChange (lTimeResolution * 210 / 120, lChannel, 93, 0));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateControlChange (lTimeResolution * 220 / 120, lChannel, 94, 0));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
	}
	// エンドオブトラックイベントの挿入
	if (lMode & 0x0002) {
		VERIFY (pNewEvent = MIDIEvent_CreateEndofTrack (lTimeResolution * 4));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
	}
	return 1;
}

// 必要な数だけトラックを追加し、履歴に記録する。
// lNeedTrackCountは最終的なトラックの総数。追加されたトラックの数を返す。
// lModeは以下の値をOR
// 0001:追加されたトラックにトラック名イベントを挿入
// 0002:追加されたトラックにEOTイベントを挿入
// 0004:追加されたトラックにコントロールチェンジ・プログラムチェンジを挿入
// 0008:追加されたトラックにテンポ・拍子記号・調整記号を挿入(タイム0)
long CSekaijuDoc::AddTrack (long lNeedTrackCount, long lMode, CHistoryUnit* pHistoryUnit) {
	ASSERT (0 <= lNeedTrackCount && lNeedTrackCount < MAXMIDITRACKNUM);
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	long lFormat = MIDIData_GetFormat (m_pMIDIData);
	// フォーマット1又はフォーマット2の場合以外はトラックを追加できない。
	if (!(lFormat == 1 || lFormat == 2)) {
		return 0;
	}
	long i = 0;
	long lCount = 0;
	MIDITrack* pTempTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	long lCurTrackCount = MIDIData_CountTrack (m_pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (m_pMIDIData);
	long lMode2 = lMode;
	for (i = lCurTrackCount; i < lNeedTrackCount; i++) {
		VERIFY (pTempTrack = MIDITrack_Create ());
		pTempTrack->m_lTempIndex = i;
		MIDITrack_SetViewMode (pTempTrack, 0);
		MIDITrack_SetForeColor (pTempTrack, pSekaijuApp->m_lDefTrackColor[i % 16]);
		MIDITrack_SetBackColor (pTempTrack, 0x00FFFFFF);
		MIDITrack_SetInputOn (pTempTrack, 1);
		MIDITrack_SetInputPort (pTempTrack, 0);
		MIDITrack_SetInputChannel (pTempTrack, (i + 15) % 16);
		MIDITrack_SetOutputOn (pTempTrack, 1);
		MIDITrack_SetOutputPort (pTempTrack, 0);
		MIDITrack_SetOutputChannel (pTempTrack, (i + 15) % 16);
		VERIFY (MIDIData_AddTrack (m_pMIDIData, pTempTrack));
		VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pTempTrack));
		// フォーマット1の場合lModeを調整
		if (lFormat == 1 && i == 0) {
			lMode2 = lMode & ~0x0004; // 最初のトラックにコントロールチェンジ・プログラムチェンジを禁止
		}
		else if (lFormat == 1 && i != 0) {
			lMode2 = lMode & ~0x0008; // 2番目以降のトラックにテンポ・拍子記号・調性記号を禁止
		}
		VERIFY (this->AddDefaultEventToTrack (pTempTrack, lMode2, pHistoryUnit));
		lCount++;
	}
	return lCount;
}


// トラックの名前取得
CString CSekaijuDoc::GetTrackName (MIDITrack* pMIDITrack) {
	TCHAR szBuf[1024];
	memset (szBuf, 0, sizeof (szBuf));
	MIDITrack_GetName (pMIDITrack, szBuf, TSIZEOF (szBuf) - 1);
	return CString (szBuf);
}

// トラックの名前設定
BOOL CSekaijuDoc::SetTrackName (MIDITrack* pMIDITrack, CString strTrackName) {
	CHistoryUnit* pCurHistoryUnit = NULL;
	VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());
	MIDIEvent* pTempEvent = MIDITrack_GetFirstKindEvent (pMIDITrack, MIDIEVENT_TRACKNAME);
	MIDIEvent* pCloneEvent = NULL;
	// 既にトラック名イベントがある場合
	if (pTempEvent) {
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
		VERIFY (pCloneEvent = ReplaceMIDIEvent (pTempEvent));
		VERIFY (MIDIEvent_SetText (pCloneEvent, (LPCTSTR)strTrackName));
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent);
	}
	// このトラックにトラック名イベントがない場合
	else {
		VERIFY (pCloneEvent = MIDIEvent_CreateTrackName (0, strTrackName));
		MIDITrack_InsertEvent (pMIDITrack, pCloneEvent);
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent);
	}
	return MIDITrack_SetName (pMIDITrack, (LPCTSTR)strTrackName);
}


// トラックのインデックスを取得
long CSekaijuDoc::GetTrackIndex (MIDITrack* pMIDITrack) {
	ASSERT (pMIDITrack);
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	MIDITrack* pTempTrack;
	long i = 0;
	forEachTrack (m_pMIDIData, pTempTrack) {
		if (pTempTrack == pMIDITrack) {
			return i;
		}
		i++;
	}
	ASSERT (FALSE);
	return -1;
}


// トラックの可視性取得
long CSekaijuDoc::GetTrackVisible (MIDITrack* pMIDITrack) {
	ASSERT (pMIDITrack);
	return pMIDITrack->m_lUserFlag & MIDITRACK_VISIBLE ? 1 : 0;
}

// トラックの可視性設定
BOOL CSekaijuDoc::SetTrackVisible (MIDITrack* pMIDITrack, long lVisible) {
	ASSERT (pMIDITrack);
	ASSERT (0 <= lVisible && lVisible <= 1);
	if (lVisible) {
		pMIDITrack->m_lUserFlag |= MIDITRACK_VISIBLE;
	}
	else {
		pMIDITrack->m_lUserFlag &= ~MIDITRACK_VISIBLE;
	}
	return TRUE;
}

// トラックのイネーブルON/OFF取得
long CSekaijuDoc::GetTrackEnable (MIDITrack* pMIDITrack) {
	ASSERT (pMIDITrack);
	return pMIDITrack->m_lUserFlag & MIDITRACK_ENABLE ? 1 : 0;
}

// トラックのイネーブルON/OFF設定
BOOL CSekaijuDoc::SetTrackEnable (MIDITrack* pMIDITrack, long lEnable) {
	ASSERT (pMIDITrack);
	ASSERT (0 <= lEnable && lEnable <= 1);
	if (lEnable) {
		pMIDITrack->m_lUserFlag |= MIDITRACK_ENABLE;
	}
	else {
		pMIDITrack->m_lUserFlag &= ~MIDITRACK_ENABLE;
	}
	return TRUE;
}

// トラックの選択状態の取得
long CSekaijuDoc::GetTrackSelected (MIDITrack* pMIDITrack) {
	ASSERT (pMIDITrack);
	return pMIDITrack->m_lUserFlag & MIDITRACK_SELECTED ? 1 : 0;
}

// トラックの選択状態の設定
BOOL CSekaijuDoc::SetTrackSelected (MIDITrack* pMIDITrack, long lSelected) {
	ASSERT (pMIDITrack);
	ASSERT (0 <= lSelected && lSelected <= 1);
	if (lSelected) {
		pMIDITrack->m_lUserFlag |= MIDITRACK_SELECTED;
	}
	else {
		pMIDITrack->m_lUserFlag &= ~MIDITRACK_SELECTED;
	}
	return TRUE;
}


// 最初のコントロールチェンジイベントの取得
MIDIEvent* CSekaijuDoc::GetTrackFirstControlChange (MIDITrack* pMIDITrack, long lNumber) {
	_ASSERT (pMIDITrack);
	_ASSERT (0 <= lNumber && lNumber < 128);
	MIDIEvent* pMIDIEvent = NULL;
	forEachEvent (pMIDITrack, pMIDIEvent) {
		if (MIDIEvent_IsControlChange (pMIDIEvent)) {
			if (MIDIEvent_GetNumber (pMIDIEvent) == lNumber) {
				return pMIDIEvent;
			}
		}
	}
	return NULL;
}

// 最初のプログラムチェンジイベントの取得
MIDIEvent* CSekaijuDoc::GetTrackFirstProgramChange (MIDITrack* pMIDITrack) {
	ASSERT (pMIDITrack);
	MIDIEvent* pMIDIEvent = NULL;
	forEachEvent (pMIDITrack, pMIDIEvent) {
		if (MIDIEvent_IsProgramChange (pMIDIEvent)) {
			return pMIDIEvent;
		}
	}
	return NULL;
}

MIDIEvent* CSekaijuDoc::FindBankMSB (MIDIEvent* pMIDIEvent) {
	ASSERT (pMIDIEvent);
	MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
	ASSERT (pMIDITrack);
	if (MIDIEvent_IsControlChange (pMIDIEvent)) {
		long lNumber = MIDIEvent_GetNumber (pMIDIEvent);
		if (lNumber == 32) {
			while (pMIDIEvent) { // 20110508修正
				if (MIDIEvent_IsControlChange (pMIDIEvent)) {
					if (MIDIEvent_GetNumber (pMIDIEvent) == 0) {
						return pMIDIEvent;
					}
				}
				pMIDIEvent = MIDIEvent_GetPrevEvent (pMIDIEvent); // 20110508追加
			}
		}
	}
	else if (MIDIEvent_IsProgramChange (pMIDIEvent)) {
		while (pMIDIEvent) { // 20110508修正
			if (MIDIEvent_IsControlChange (pMIDIEvent)) {
				if (MIDIEvent_GetNumber (pMIDIEvent) == 0) {
					return pMIDIEvent;
				}
			}
			pMIDIEvent = MIDIEvent_GetPrevEvent (pMIDIEvent); // 20110508追加
		}
	}
	return NULL;
}

MIDIEvent* CSekaijuDoc::FindBankLSB (MIDIEvent* pMIDIEvent) {
	ASSERT (pMIDIEvent);
	MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
	ASSERT (pMIDITrack);
	if (MIDIEvent_IsControlChange (pMIDIEvent)) {
		long lNumber = MIDIEvent_GetNumber (pMIDIEvent);
		if (lNumber == 0) {
			while (pMIDIEvent) { // 20110508修正
				if (MIDIEvent_IsControlChange (pMIDIEvent)) {
					if (MIDIEvent_GetNumber (pMIDIEvent) == 32) {
						return pMIDIEvent;
					}
				}
				pMIDIEvent = MIDIEvent_GetNextEvent (pMIDIEvent); // 20110508追加
			}
		}
	}
	else if (MIDIEvent_IsProgramChange (pMIDIEvent)) {
		while (pMIDIEvent) { // 20110508修正
			if (MIDIEvent_IsControlChange (pMIDIEvent)) {
				if (MIDIEvent_GetNumber (pMIDIEvent) == 32) {
					return pMIDIEvent;
				}
			}
			pMIDIEvent = MIDIEvent_GetPrevEvent (pMIDIEvent); // 20110508追加
		}
	}
	return NULL;
}

MIDIEvent* CSekaijuDoc::FindProgramChange (MIDIEvent* pMIDIEvent) {
	ASSERT (pMIDIEvent);
	MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
	ASSERT (pMIDITrack);
	if (MIDIEvent_IsControlChange (pMIDIEvent)) {
		long lNumber = MIDIEvent_GetNumber (pMIDIEvent);
		if (lNumber == 0 || lNumber == 32) {
			while (pMIDIEvent) { // 20110508修正
				if (MIDIEvent_IsProgramChange (pMIDIEvent)) {
					return pMIDIEvent;
				}
				pMIDIEvent = MIDIEvent_GetNextEvent (pMIDIEvent); // 20110508追加
			}
		}
	}
	return NULL;
}

// 指定トラックのlTimeにおけるlKeyの名称(例："C#6")を取得 // 20090501追加
CString CSekaijuDoc::GetKeyName (MIDITrack* pMIDITrack, long lTime, long lKey) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	long lTrackViewMode = MIDITrack_GetViewMode (pMIDITrack);
	// このトラックの表示モードは「通常」の場合
	if (lTrackViewMode == 0) {
		long lsf, lmi;
		long lOctaveSignature = pSekaijuApp->m_theGeneralOption.m_lOctaveSignature;
		MIDIData_FindKeySignature (m_pMIDIData, lTime, &lsf, &lmi);
		long lIndex = (lsf & 0x0000000F) * 16 + (lKey % 12);
		CString strKeyName;
		strKeyName.Format (_T("%s%d"), 
			pSekaijuApp->m_strNoteKeyName[lIndex], 
			lKey / 12 + lOctaveSignature - 5);
		return strKeyName;
	}
	// このトラックの表示モードは「ドラム」の場合
	else {
		long lTrackOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
		MIDIInstrumentDefinition* pMIDIInstDef = pSekaijuApp->m_pMIDIInstDefDrum[lTrackOutputPort];
		// このトラックの指定するポートのインストゥルメント定義が見つかった
		if (pMIDIInstDef) {
			long lTrackCC0, lTrackCC32, lTrackPC;
			MIDIEvent* pCC0Event = GetTrackFirstControlChange (pMIDITrack, 0);
			if (pCC0Event) {
				lTrackCC0 = MIDIEvent_GetValue (pCC0Event);
			}
			else {
				lTrackCC0 = 0;
			}
			MIDIEvent* pCC32Event = GetTrackFirstControlChange (pMIDITrack, 32);
			if (pCC32Event) {
				lTrackCC32 = MIDIEvent_GetValue (pCC32Event);
			}
			else {
				lTrackCC32 = 0;
			}
			MIDIEvent* pPCEvent = GetTrackFirstProgramChange (pMIDITrack);
			if (pPCEvent) {
				lTrackPC = MIDIEvent_GetNumber (pPCEvent);
			}
			else {
				lTrackPC = 0;
			}
			MIDINoteNameTable* pNoteNameTable = 
				MIDIInstrumentDefinition_GetNoteNameTable 
				(pSekaijuApp->m_pMIDIInstDefDrum[lTrackOutputPort], 
				(lTrackCC0 << 7) | (lTrackCC32), lTrackPC);
			// このインストゥルメント定義は指定バンク/プログラムの
			// NoteNameTableを持っている
			if (pNoteNameTable) {
				TCHAR szKeyName[256];
				memset (szKeyName, 0, sizeof (szKeyName));
				MIDINoteNameTable_GetName 
					(pNoteNameTable, lKey, szKeyName, TSIZEOF (szKeyName) - 1);
				CString strKeyName (szKeyName);
				return strKeyName;
			}
			// このインストゥルメント定義は指定バンク/プログラムの
			// NoteNameTableを持っていない
			else {
				return _T("");
			}
		}
		// このトラックの指定するポートのインストゥルメント定義が見つからなかった
		else {
			return _T("");
		}
	}
}

// タイムを時:分:秒:ミリ秒文字列に変換 // 20100502追加
BOOL CSekaijuDoc::LongTimeToStringMillisec (MIDIData* pMIDIData, long lTime, CString* pstrText) {
	long lMilliSec, lHour, lMinute, lSec, lMSec;
	lMilliSec = MIDIData_TimeToMillisec (pMIDIData, lTime);
	lHour = lMilliSec / 3600000;
	lMinute = (lMilliSec % 3600000) / 60000;
	lSec = (lMilliSec % 60000) / 1000;
	lMSec = (lMilliSec % 1000);
	pstrText->Format (_T("%02d:%02d:%02d:%03d"), lHour, lMinute, lSec, lMSec);
	return TRUE;
}

// タイムを小節:拍:ティック 又は フレーム番号:ティック文字列に変換 // 20100502追加
BOOL CSekaijuDoc::LongTimeToStringTime (MIDIData* pMIDIData, long lTime, CString* pstrText) {
	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	// TPQNベースの場合(標準)…long→"小節:拍:ティック"
	if (lTimeMode == MIDIDATA_TPQNBASE) {
		long lMeasure, lBeat, lTick;
		MIDIData_BreakTime (pMIDIData, lTime, &lMeasure, &lBeat, &lTick);
		pstrText->Format (_T("%05d:%02d:%03d"), lMeasure + 1, lBeat + 1, lTick);
		return TRUE;
	}
	// SMPTE24ベース、SMPTE25ベース、SMPTE29.97ベース、SMPTE30ベースの場合…long→"フレーム:サブフレーム"
	else if (lTimeMode == MIDIDATA_SMPTE24BASE || lTimeMode == MIDIDATA_SMPTE25BASE ||
		lTimeMode == MIDIDATA_SMPTE29BASE || lTimeMode == MIDIDATA_SMPTE30BASE) {
		long lFrameNumber, lTick;
		lFrameNumber = lTime / lTimeResolution;
		lTick = lTime % lTimeResolution;
		pstrText->Format (_T("%08d:%03d"), lFrameNumber, lTick);
		return TRUE;
	}
	return FALSE;
}

// 小節:拍:ティック 又は フレーム番号:ティック文字列をタイムに変換 // 20100502追加
// 戻り値：正常=0, 異常=エラーメッセージ文字列リソースID(1～)
long CSekaijuDoc::StringTimeToLongTime (MIDIData* pMIDIData, CString strText, long* pTime) {
	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	long lTemp = 0;
	CString strToken;
	// TPQNベースの場合(標準)…"小節:拍:ティック"→long
	if (lTimeMode == MIDIDATA_TPQNBASE) {
		long lMeasure, lBeat, lTick;
		// 小節と拍の間のセパレータチェック
		lTemp = strText.Find (_T(":"));
		if (lTemp < 0) {
			return IDS_A_SEPARATOR_BETWEEN_MEASURE_AND_BEAT_IS_NOT_FOUND;
		}
		// 小節文字チェック
		strToken = strText.Left (lTemp);
		if (IsNumeric (strToken) <= 0) {
			return IDS_MEASURE_MUST_BE_HALF_WIDTH_NUMBER;
		}
		// 小節値チェック
		lMeasure = _ttol (strToken) - 1;
		if (lMeasure < 0 || lMeasure >= 65535) {
			return IDS_MEASURE_VALUE_IS_OUT_OF_RANGE;
		}
		// 拍とティックの間のセパレータチェック
		strText = strText.Mid (lTemp + 1);
		lTemp = strText.Find (_T(":"));
		if (lTemp < 0) {
			return IDS_A_SEPARATOR_BETWEEN_BEAT_AND_TICK_IS_NOT_FOUND;
		}
		// 拍文字チェック
		strToken = strText.Left (lTemp);
		if (IsNumeric (strToken) <= 0) {
			return IDS_BEAT_MUST_BE_HALF_WIDTH_NUMBER;
		}
		// 拍値チェック
		lBeat = _ttol (strToken) - 1;
		if (lBeat < 0 || lBeat >= 100) {
			return IDS_BEAT_VALUE_IS_OUT_OF_RANGE;
		}
		// ティック文字チェック
		strText = strText.Mid (lTemp + 1);
		if (IsNumeric (strToken) <= 0) {
			return IDS_TICK_MUST_BE_HALF_WIDTH_NUMBER;
		}
		// ティック値チェック
		lTick = _ttol (strText);
		if (lTick < 0 || lTick > 960) {
			return IDS_TICK_VALUE_IS_OUT_OF_RANGE;
		}
		MIDIData_MakeTime (pMIDIData, lMeasure, lBeat, lTick, pTime);
		*pTime = CLIP (0x00000000, *pTime, 0x7FFFFFFF);
		return 0;
	}
	// SMPTE24ベース、SMPTE25ベース、SMPTE29.97ベース、SMPTE30ベースの場合…"フレーム:サブフレーム"→long
	else if (lTimeMode == MIDIDATA_SMPTE24BASE || lTimeMode == MIDIDATA_SMPTE25BASE ||
		lTimeMode == MIDIDATA_SMPTE29BASE || lTimeMode == MIDIDATA_SMPTE30BASE) {
		long lFrame, lSubFrame;
		// フレームとサブフレームの間のセパレータチェック
		lTemp = strText.Find (_T(":"));
		if (lTemp < 0) {
			return IDS_A_SEPARATOR_BETWEEN_FRAME_AND_TICK_IS_NOT_FOUND;
		}
		// フレーム文字チェック
		strToken = strText.Left (lTemp);
		if (IsNumeric (strToken) <= 0) {
			return IDS_FRAME_MUST_BE_HALF_WIDTH_NUMBER;
		}
		// フレーム値チェック
		lFrame = _ttol (strToken);
		if (lFrame < 0 || lFrame > 99999999) {
			return IDS_FRAME_VALUE_IS_OUT_OF_RANGE;
		}
		// サブフレーム文字チェック
		strText = strText.Mid (lTemp + 1);
		if (IsNumeric (strToken) <= 0) {
			return IDS_SUBFRAME_MUST_BE_HALF_WIDTH_NUMBER;
		}
		// サブフレーム値チェック
		lSubFrame = _ttol (strText);
		if (lSubFrame < 0 || lSubFrame > 960) {
			return IDS_SUBFRAME_VALUE_IS_OUT_OF_RANGE;
		}
		*pTime = lFrame * lTimeResolution + lSubFrame;
		*pTime = CLIP (0x00000000, *pTime, 0x7FFFFFFF);
		return 0;
	}
	else {
		return 0;
	}
}

// 新規トラックリスト表示
void CSekaijuDoc::ShowTrackListFrame () {
	CSekaijuDocTemplate* pDocTemplate = (CSekaijuDocTemplate*)GetDocTemplate ();
	CTrackListFrame* pTrackListFrame = 
		(CTrackListFrame*)(pDocTemplate->CreateTrackListFrame (this));
	pDocTemplate->InitialUpdateFrame (pTrackListFrame, this);
}

// 新規ピアノロール表示
void CSekaijuDoc::ShowPianoRollFrame () {
	CSekaijuDocTemplate* pDocTemplate = (CSekaijuDocTemplate*)GetDocTemplate ();
	CPianoRollFrame* pPianoRollFrame = 
		(CPianoRollFrame*)(pDocTemplate->CreatePianoRollFrame (this));
	pDocTemplate->InitialUpdateFrame (pPianoRollFrame, this);

}

// 新規イベントリスト表示
void CSekaijuDoc::ShowEventListFrame () {
	CSekaijuDocTemplate* pDocTemplate = (CSekaijuDocTemplate*)GetDocTemplate ();
	CEventListFrame* pEventListFrame = 
		(CEventListFrame*)(pDocTemplate->CreateEventListFrame (this));
	pDocTemplate->InitialUpdateFrame (pEventListFrame, this);

}

// 新規譜面表示
void CSekaijuDoc::ShowMusicalScoreFrame () {
	CSekaijuDocTemplate* pDocTemplate = (CSekaijuDocTemplate*)GetDocTemplate ();
	CMusicalScoreFrame* pMusicalScoreFrame = 
		(CMusicalScoreFrame*)(pDocTemplate->CreateMusicalScoreFrame (this));
	pDocTemplate->InitialUpdateFrame (pMusicalScoreFrame, this);

}





// MIDIEventを同じMIDIイベントで置き換え、新しい方のMIDIイベントを返す。
// MIDIEventのプロパティを変更する際には必ずこの関数を呼び出し、旧状態を保存する。
// (1)pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent);
// (2)pCloneEvent = ReplaceMIDIevent (pMIDITrack, pMIDIEvent);
// (3)MIDIEvent_Set* (pCloneEvent, *);
// (4)pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent);
MIDIEvent* CSekaijuDoc::ReplaceMIDIEvent (MIDIEvent* pMIDIEvent) {
	long lCount = 0;
	long lPosition = 0;
	ASSERT (pMIDIEvent);
	MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
	ASSERT (pMIDITrack);
	MIDIEvent* pTargetEvent = MIDIEvent_GetFirstCombinedEvent (pMIDIEvent);
	MIDIEvent* pCloneEvent = MIDIEvent_CreateClone (pMIDIEvent);
	if (pCloneEvent == NULL) {
		return NULL;
	}
	MIDIEvent* pInsertEvent = MIDIEvent_GetFirstCombinedEvent (pCloneEvent);
	while (pTargetEvent && pInsertEvent) {
		MIDIEvent* pTargetPrevEvent = pTargetEvent->m_pPrevEvent;
		MIDIEvent* pTargetNextEvent = pTargetEvent->m_pNextEvent;
		pInsertEvent->m_lUserFlag &= ~MIDIEVENT_ALIVE; // フラグはAddHistoryRecord内で決定
		pInsertEvent->m_lUserFlag &= ~MIDIEVENT_DEAD; // フラグはAddHistoryRecord内で決定
		VERIFY (MIDITrack_RemoveSingleEvent (pMIDITrack, pTargetEvent));
		VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pInsertEvent, pTargetPrevEvent));
		pTargetEvent = pTargetEvent->m_pNextCombinedEvent;
		pInsertEvent = pInsertEvent->m_pNextCombinedEvent;
		lCount++;
	}
	ASSERT (pTargetEvent == NULL && pInsertEvent == NULL);
	return pCloneEvent;
}

// MIDIイベントの複製を行う。複製したイベントは複製元イベントの直後に挿入されている。
// 注意：EOTなどを複製してもNULLが返る。
MIDIEvent* CSekaijuDoc::DuplicateMIDIEvent (MIDIEvent* pMIDIEvent) {
	ASSERT (pMIDIEvent);
	MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
	ASSERT (pMIDITrack);
	long lCount = 0;
	long lRet = 0;
	MIDIEvent* pTargetEvent = MIDIEvent_GetFirstCombinedEvent (pMIDIEvent);
	MIDIEvent* pCloneEvent = MIDIEvent_CreateClone (pMIDIEvent);
	if (pCloneEvent == NULL) {
		return NULL;
	}
	MIDIEvent* pInsertEvent = pCloneEvent;
	while (pTargetEvent && pInsertEvent) {
		pInsertEvent->m_lUserFlag &= ~MIDIEVENT_ALIVE;
		pInsertEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
		lRet = MIDITrack_InsertSingleEventAfter (pMIDITrack, pInsertEvent, pTargetEvent);
		if (lRet == 0) {
			MIDIEvent_Delete (pCloneEvent);
			pCloneEvent = NULL;
			return NULL;
		}
		pTargetEvent = pTargetEvent->m_pNextCombinedEvent;
		pInsertEvent = pInsertEvent->m_pNextCombinedEvent;
		lCount++;
	}
	ASSERT (pTargetEvent == NULL && pInsertEvent == NULL);
	return pCloneEvent;
}

// MIDITrackオブジェクトを差し替え、新しいMIDITrackオブジェクトを返す。
// 内部のMIDIEventオブジェクトは、新しいMIDITrackの所有物となる。
MIDITrack* CSekaijuDoc::ReplaceMIDITrack (MIDITrack* pMIDITrack) {
	ASSERT (pMIDITrack);
	MIDIData* pMIDIData = MIDITrack_GetParent (pMIDITrack);
	ASSERT (pMIDIData);
	// イベント数0の空のMIDITrackを作成
	MIDITrack* pCloneTrack = NULL;
	pCloneTrack = MIDITrack_Create ();
	if (pCloneTrack == NULL) {
		return NULL;
	}
	pCloneTrack->m_lInputOn = pMIDITrack->m_lInputOn;
	pCloneTrack->m_lInputPort = pMIDITrack->m_lInputPort;
	pCloneTrack->m_lInputChannel = pMIDITrack->m_lInputChannel;
	pCloneTrack->m_lOutputOn = pMIDITrack->m_lOutputOn;
	pCloneTrack->m_lOutputPort = pMIDITrack->m_lOutputPort;
	pCloneTrack->m_lOutputChannel = pMIDITrack->m_lOutputChannel;
	pCloneTrack->m_lTimePlus = pMIDITrack->m_lTimePlus;
	pCloneTrack->m_lKeyPlus = pMIDITrack->m_lKeyPlus;
	pCloneTrack->m_lVelocityPlus = pMIDITrack->m_lVelocityPlus;
	pCloneTrack->m_lViewMode = pMIDITrack->m_lViewMode;
	pCloneTrack->m_lForeColor = pMIDITrack->m_lForeColor;
	pCloneTrack->m_lBackColor = pMIDITrack->m_lBackColor;
	pCloneTrack->m_lUser1 = pMIDITrack->m_lUser1;
	pCloneTrack->m_lUser2 = pMIDITrack->m_lUser2;
	pCloneTrack->m_lUser3 = pMIDITrack->m_lUser3;
	pCloneTrack->m_lUserFlag = pMIDITrack->m_lUserFlag;
	// 前のトラックと次のトラックを保持しておく(なければNULL)。
	MIDITrack* pTargetPrevTrack = pMIDITrack->m_pPrevTrack;
	MIDITrack* pTargetNextTrack = pMIDITrack->m_pNextTrack;
	// 差し替え元MIDIトラック内のMIDIイベントをすべて差し替え先MIDIトラックに移動する。
	MIDITrack_CountEvent (pMIDITrack);
	MIDIEvent* pMIDIEvent = MIDITrack_GetFirstEvent (pMIDITrack);
	while (pMIDIEvent) {
		VERIFY (MIDITrack_RemoveEvent (pMIDITrack, pMIDIEvent));
		VERIFY (MIDITrack_InsertEvent (pCloneTrack, pMIDIEvent));
		pMIDIEvent = MIDITrack_GetFirstEvent (pMIDITrack);
	}
	// MIDIデータから古いMIDITrackを除去、新しいMIDITrackを追加
	VERIFY (MIDIData_RemoveTrack (pMIDIData, pMIDITrack));
	VERIFY (MIDIData_InsertTrackAfter (pMIDIData, pCloneTrack, pTargetPrevTrack));
	return pCloneTrack;
}

// MIDIDataオブジェクトを差し替え、新しいMIDIDataオブジェクトを返す。
// 内部のMIDITrackオブジェクトは、新しいMIDIDataの所有物となる。
MIDIData* CSekaijuDoc::ReplaceMIDIData (MIDIData* pMIDIData) {
	ASSERT (pMIDIData);
	// トラック数0の空のMIDIデータを作成
	MIDIData* pCloneData = NULL;
	long lFormat = MIDIData_GetFormat (pMIDIData);
	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pCloneData = MIDIData_Create (lFormat, 0, lTimeMode, lTimeResolution);
	if (pCloneData == NULL) {
		return NULL;
	}
	pCloneData->m_lUser1 = pMIDIData->m_lUser1;
	pCloneData->m_lUser2 = pMIDIData->m_lUser2;
	pCloneData->m_lUser3 = pMIDIData->m_lUser3;
	pCloneData->m_lUserFlag = pMIDIData->m_lUserFlag;
	// 差し替え元MIDIデータ内のMIDIトラックをすべて差し替え先MIDIデータに移動する。
	MIDIData_CountTrack (pMIDIData);
	MIDITrack* pMIDITrack = MIDIData_GetFirstTrack (pMIDIData);
	while (pMIDITrack) {
		VERIFY (MIDIData_RemoveTrack (pMIDIData, pMIDITrack));
		VERIFY (MIDIData_AddTrack (pCloneData, pMIDITrack));
		pMIDITrack = MIDIData_GetFirstTrack (pMIDIData);
	}
	// 古いMIDIDataを除去、新しいMIDIDataを設定
	this->m_pMIDIData = pCloneData;
	return pCloneData;
}

// 新規履歴ユニットの追加(履歴ユニット名)
BOOL CSekaijuDoc::AddHistoryUnit (CString strName) {
	// 現在の履歴ユニット以降にある不要な履歴ユニットを削除
	long lNumHistoryUnit = m_theHistoryUnitArray.GetSize ();
	for (long i = lNumHistoryUnit - 1; i > m_lCurHistoryPosition; i--) {
		CHistoryUnit* pHistoryUnit = (CHistoryUnit*)m_theHistoryUnitArray.GetAt (i);
		m_theHistoryUnitArray.RemoveAt (i);
		delete pHistoryUnit;
	}
	// 新規履歴ユニット作成
	CHistoryUnit* pHistoryUnit = new CHistoryUnit ();
	pHistoryUnit->m_strName = strName;
	pHistoryUnit->m_theTime = CTime::GetCurrentTime ();
	// 履歴ユニット配列に追加
	m_theHistoryUnitArray.Add (pHistoryUnit);
	// この履歴ユニットのインデックスを現在の履歴ユニットとする。
	m_lCurHistoryPosition = m_theHistoryUnitArray.GetSize () - 1;

	//for Debug
	//FILE* pFile = fopen ("history.txt", "at");
	//fprintf (pFile, "// AddHistoryUnit Index=%d, Name=%s, Time=%d\n", i+1, strName, pHistoryUnit->m_theTime);
	//fclose (pFile);
	
	return TRUE;
}

// 現在の履歴ユニットを取得する
CHistoryUnit* CSekaijuDoc::GetCurHistoryUnit () {
	// 現在の履歴ユニット番号はm_lCurHistoryPositionが保持している。
	// この値は、「元に戻す」でデクリメント、「やり直し」でインクリメントする。
	long lNumHistoryUnit = m_theHistoryUnitArray.GetSize ();
	if (0 <= m_lCurHistoryPosition && m_lCurHistoryPosition < lNumHistoryUnit) {
		return (CHistoryUnit*)m_theHistoryUnitArray.GetAt (m_lCurHistoryPosition);
	}
	ASSERT (FALSE);
	return NULL;
}

// 履歴ユニットの完全消去
BOOL CSekaijuDoc::DeleteAllHistoryUnit () {
	long lNumHistoryUnit = m_theHistoryUnitArray.GetSize ();
	for (long i = lNumHistoryUnit - 1; i >= 0; i--) {
		CHistoryUnit* pHistoryUnit = (CHistoryUnit*)m_theHistoryUnitArray.GetAt (i);
		m_theHistoryUnitArray.RemoveAt (i);
		delete pHistoryUnit;
	}
	m_lCurHistoryPosition = -1;
	return TRUE;
}





//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// 『ファイル(&F)』-『上書き保存(&S)』
void CSekaijuDoc::OnUpdateFileSaveUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

}

// 『ファイル(&F)』-『名前を付けて保存(&A)』
void CSekaijuDoc::OnUpdateFileSaveAsUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
}


// 『ファイル(&F)』-『プロパティ(&R)』
void CSekaijuDoc::OnFileProperty () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	// 20120107 '\t''\r''\n''\\'の混入に対応
	CString strFormat;
	CFilePropertyDlg theDlg;
	TCHAR szBuf1[2048];
	TCHAR szBuf2[2048];
	memset (szBuf1, 0, sizeof (szBuf1));
	memset (szBuf2, 0, sizeof (szBuf2));
	MIDIData_GetTitle (m_pMIDIData, szBuf1, TSIZEOF (szBuf1) - 1);
	codestr2str (szBuf1, TCSLEN (szBuf1), szBuf2, TSIZEOF (szBuf2) - 1);
	theDlg.m_strTitle = szBuf2;
	memset (szBuf1, 0, sizeof (szBuf1));
	memset (szBuf2, 0, sizeof (szBuf2));
	MIDIData_GetSubTitle (m_pMIDIData, szBuf1, TSIZEOF (szBuf1) - 1);
	codestr2str (szBuf1, TCSLEN (szBuf1), szBuf2, TSIZEOF (szBuf2) - 1);
	theDlg.m_strSubTitle = szBuf2;
	memset (szBuf1, 0, sizeof (szBuf1));
	memset (szBuf2, 0, sizeof (szBuf2));
	MIDIData_GetCopyright (m_pMIDIData, szBuf1, TSIZEOF (szBuf1) - 1);
	codestr2str (szBuf1, TCSLEN (szBuf1), szBuf2, TSIZEOF (szBuf2) - 1);
	theDlg.m_strCopyright = szBuf2;
	memset (szBuf1, 0, sizeof (szBuf1));
	memset (szBuf2, 0, sizeof (szBuf2));
	MIDIData_GetComment (m_pMIDIData, szBuf1, TSIZEOF (szBuf1) - 1);
	codestr2str (szBuf1, TCSLEN (szBuf1), szBuf2, TSIZEOF (szBuf2) - 1);
	theDlg.m_strComment1 = szBuf2;
	long lNumTrack = MIDIData_CountTrack (m_pMIDIData);
	theDlg.m_strNumTrack.Format (_T("%d"), lNumTrack);
	long lNumEvent = 0;
	MIDITrack* pMIDITrack = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		lNumEvent += MIDITrack_CountEvent (pMIDITrack);
	}
	theDlg.m_strNumEvent.Format (_T("%d"), lNumEvent);
	
	long lSMFFormat = MIDIData_GetFormat (m_pMIDIData);
	theDlg.m_nSMFFormat = (int)lSMFFormat;	
	long lTimeMode = MIDIData_GetTimeMode (m_pMIDIData);
	switch (lTimeMode) {
	case MIDIDATA_TPQNBASE:
		theDlg.m_nTimeMode = 0;
		break;
	case MIDIDATA_SMPTE24BASE:
		theDlg.m_nTimeMode = 1;
		break;
	case MIDIDATA_SMPTE25BASE:
		theDlg.m_nTimeMode = 2;
		break;
	case MIDIDATA_SMPTE29BASE:
		theDlg.m_nTimeMode = 3;
		break;
	case MIDIDATA_SMPTE30BASE:
		theDlg.m_nTimeMode = 4;
		break;
	}
	long lTimeResolution = MIDIData_GetTimeResolution (m_pMIDIData);
	theDlg.m_nResolution = (int)lTimeResolution;

	long lEndTime = MIDIData_GetEndTime (m_pMIDIData);
	if (lTimeMode == MIDIDATA_TPQNBASE) {
		long lMeasure, lBeat, lTick;
		MIDIData_BreakTime (m_pMIDIData, lEndTime, &lMeasure, &lBeat, &lTick);
		VERIFY (strFormat.LoadString (IDS_05D_02D_03D_MEASURE_BEAT_TICK));
		theDlg.m_strEndTime.Format (strFormat, lMeasure + 1, lBeat + 1, lTick);
	}
	else if (lTimeMode == MIDIDATA_SMPTE24BASE ||
		lTimeMode == MIDIDATA_SMPTE25BASE ||
		lTimeMode == MIDIDATA_SMPTE29BASE ||
		lTimeMode == MIDIDATA_SMPTE30BASE) {
		long lFrameNumber, lTick;
		lFrameNumber = lEndTime / lTimeResolution;
		lTick = lEndTime % lTimeResolution;
		VERIFY (strFormat.LoadString (IDS_08D_03D_FRAME_TICK));
		theDlg.m_strEndTime.Format (strFormat, lFrameNumber, lTick);
	}
	long lEndMillisec = MIDIData_TimeToMillisec (m_pMIDIData, lEndTime);
	long lHour = (lEndMillisec / 3600000) % 3600000;
	long lMinute = (lEndMillisec / 60000) % 60;
	long lSec = (lEndMillisec / 1000) % 60;
	long lMillisec = lEndMillisec % 1000;
	VERIFY (strFormat.LoadString (IDS_02D_02D_02D_03D_HOUR_MINUTE_SECOND_MILLISEC));
	theDlg.m_strEndMillisec.Format (strFormat, lHour, lMinute, lSec, lMillisec);


	
	if (theDlg.DoModal () == IDOK) {

		if (m_bEditLocked) {
			return;
		}
		
		BeginWaitCursor ();
		m_theCriticalSection.Lock ();
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		MIDIEvent* pTempEvent = NULL;
		MIDIEvent* pCloneEvent = NULL;
		MIDITrack* pCloneTrack = NULL;
		MIDIData* pCloneData = NULL;

		// 新しい履歴ユニットの用意
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_FILE_MODIFY_PROPERTY));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		CHistoryUnit* pCurHistoryUnit = NULL;
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		VERIFY (pMIDITrack = MIDIData_GetFirstTrack (m_pMIDIData));
		
		// タイトル設定
		// 最初のトラック名イベントを探索
		forEachEvent (pMIDITrack, pTempEvent) {
			if (MIDIEvent_GetKind (pTempEvent) == MIDIEVENT_TRACKNAME) {
				break;
			}
		}
		// 最初のトラック名イベントが見つからなかった場合
		if (pTempEvent == NULL) {
			memset (szBuf2, 0, sizeof (szBuf2));
			str2codestr ((LPTSTR)(LPCTSTR)theDlg.m_strTitle, 
				theDlg.m_strTitle.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
			VERIFY (pTempEvent = MIDIEvent_CreateTrackName (0, szBuf2));
			VERIFY (MIDITrack_InsertEvent (pMIDITrack, pTempEvent));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
		}
		// 最初のトラック名が見つかった場合
		else {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
			VERIFY (pCloneEvent = this->ReplaceMIDIEvent (pTempEvent));
			memset (szBuf2, 0, sizeof (szBuf2));
			str2codestr ((LPTSTR)(LPCTSTR)theDlg.m_strTitle, 
				theDlg.m_strTitle.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
			MIDIEvent_SetText (pCloneEvent, szBuf2);
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
		}

		// サブタイトルの設定(空文字列時はサブタイトル削除)
		// 2番目のトラック名イベントを探索
		long lCount = 0;
		forEachEvent (pMIDITrack, pTempEvent) {
			if (MIDIEvent_GetKind (pTempEvent) == MIDIEVENT_TRACKNAME) {
				if (lCount == 1) {
					break;
				}
				lCount++;
			}
		}
		// サブタイトルを設定する場合
		if (theDlg.m_strSubTitle != _T("")) {
			// 2番目のトラック名イベントが見つからなかった場合
			if (pTempEvent == NULL) {
				memset (szBuf2, 0, sizeof (szBuf2));
				str2codestr ((LPTSTR)(LPCTSTR)theDlg.m_strSubTitle, 
					theDlg.m_strSubTitle.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
				VERIFY (pTempEvent = MIDIEvent_CreateTrackName (0, szBuf2));
				VERIFY (MIDITrack_InsertEvent (pMIDITrack, pTempEvent));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
			}
			// 2番目のトラック名が見つかった場合
			else {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
				VERIFY (pCloneEvent = this->ReplaceMIDIEvent (pTempEvent));
				memset (szBuf2, 0, sizeof (szBuf2));
				str2codestr ((LPTSTR)(LPCTSTR)theDlg.m_strSubTitle, 
					theDlg.m_strSubTitle.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
				MIDIEvent_SetText (pCloneEvent, szBuf2);
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			}
		}
		// サブタイトルを削除する場合
		else {
			// 2番目のトラック名が見つかった場合
			if (pTempEvent) {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
				VERIFY (MIDITrack_RemoveEvent (pMIDITrack, pTempEvent));
			}
		}

		// 著作権設定(空文字時は著作権イベント削除)
		// 最初の著作権イベントを探索
		forEachEvent (pMIDITrack, pTempEvent) {
			if (MIDIEvent_GetKind (pTempEvent) == MIDIEVENT_COPYRIGHTNOTICE) {
				break;
			}
		}
		// 著作権を設定する場合
		if (theDlg.m_strCopyright != _T("")) {
			// 最初の著作権イベントが見つからなかった場合
			if (pTempEvent == NULL) {
				memset (szBuf2, 0, sizeof (szBuf2));
				str2codestr ((LPTSTR)(LPCTSTR)theDlg.m_strCopyright, 
					theDlg.m_strCopyright.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
				VERIFY (pTempEvent = MIDIEvent_CreateCopyrightNotice (0, szBuf2));
				VERIFY (MIDITrack_InsertEvent (pMIDITrack, pTempEvent));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
			}
			// 最初の著作権イベントが見つかった場合
			else {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
				VERIFY (pCloneEvent = this->ReplaceMIDIEvent (pTempEvent));
				memset (szBuf2, 0, sizeof (szBuf2));
				str2codestr ((LPTSTR)(LPCTSTR)theDlg.m_strCopyright, 
					theDlg.m_strCopyright.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
				MIDIEvent_SetText (pCloneEvent, szBuf2);
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			}
		}
		// 著作権を削除する場合
		else {
			// 最初の著作権イベントが見つかった場合
			if (pTempEvent) {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
				VERIFY (MIDITrack_RemoveEvent (pMIDITrack, pTempEvent));
			}
		}

		// コメント1設定(空文字時は最初のテキストイベント削除)
		// 最初のテキストイベントを探索
		forEachEvent (pMIDITrack, pTempEvent) {
			if (MIDIEvent_GetKind (pTempEvent) == MIDIEVENT_TEXTEVENT) {
				break;
			}
		}
		// コメント1を設定する場合
		if (theDlg.m_strComment1 != _T("")) {
			// 最初のテキストイベントが見つからなかった場合
			if (pTempEvent == NULL) {
				memset (szBuf2, 0, sizeof (szBuf2));
				str2codestr ((LPTSTR)(LPCTSTR)theDlg.m_strComment1, 
					theDlg.m_strComment1.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
				VERIFY (pTempEvent = MIDIEvent_CreateTextEvent (0, szBuf2));
				VERIFY (MIDITrack_InsertEvent (pMIDITrack, pTempEvent));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
			}
			// 最初のテキストイベントが見つかった場合
			else {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
				VERIFY (pCloneEvent = this->ReplaceMIDIEvent (pTempEvent));
				memset (szBuf2, 0, sizeof (szBuf2));
				str2codestr ((LPTSTR)(LPCTSTR)theDlg.m_strComment1, 
					theDlg.m_strComment1.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
				MIDIEvent_SetText (pCloneEvent, szBuf2);
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			}
		}
		// コメント1を削除する場合
		else {
			// 最初のテキストイベントが見つかった場合
			if (pTempEvent) {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
				VERIFY (MIDITrack_RemoveEvent (pMIDITrack, pTempEvent));
			}
		}

		// 新しいSMFフォーマットを検出
		long lNewSMFFormat = theDlg.m_nSMFFormat;
		// SMFフォーマットが変更された場合
		if (lNewSMFFormat != lSMFFormat) {
			// 全オブジェクトの履歴記録(20090116履歴記録方法修正)
			forEachTrackInverse (m_pMIDIData, pMIDITrack) {
				if (MIDITrack_GetFirstEvent (pMIDITrack)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord 
					(HISTORYRECORD_REMOVEEVENTALL, MIDITrack_GetFirstEvent (pMIDITrack)));
				}
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack));
			}
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEDATA, m_pMIDIData));

			// 全オブジェクトの交換
			forEachTrackInverse (m_pMIDIData, pMIDITrack) {
				forEachEventInverse (pMIDITrack, pMIDIEvent) {
					if (pMIDIEvent->m_pPrevCombinedEvent == NULL) {
						VERIFY (pCloneEvent = this->ReplaceMIDIEvent (pMIDIEvent));
						pMIDIEvent = pCloneEvent;
					}
				}
				VERIFY (pCloneTrack = this->ReplaceMIDITrack (pMIDITrack));
				pMIDITrack = pCloneTrack;
			}
			VERIFY (m_pMIDIData = this->ReplaceMIDIData (m_pMIDIData));

			// フォーマット0/1/2変換
			MIDIData_SetFormat (m_pMIDIData, lNewSMFFormat);
			MIDIData_CountTrack (m_pMIDIData);
			long i = 0;
			forEachTrack (m_pMIDIData, pMIDITrack) {
				MIDITrack_SetForeColor (pMIDITrack, pSekaijuApp->m_lDefTrackColor[i % 16]);
				i++;
			}

			// 全オブジェクトの履歴記録(20090116履歴記録方法修正)
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTDATA, m_pMIDIData));
			forEachTrack (m_pMIDIData, pMIDITrack) {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pMIDITrack));
				if (MIDITrack_GetFirstEvent (pMIDITrack)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENTALL, 
					MIDITrack_GetFirstEvent (pMIDITrack)));
				}
			}
		}
		
		// 新しいタイムモードと分解能を検出
		long lNewTimeMode = 0;
		switch (theDlg.m_nTimeMode) {
		case 0:
			lNewTimeMode = MIDIDATA_TPQNBASE;
			break;
		case 1:
			lNewTimeMode = MIDIDATA_SMPTE24BASE;
			break;
		case 2:
			lNewTimeMode = MIDIDATA_SMPTE25BASE;
			break;
		case 3:
			lNewTimeMode = MIDIDATA_SMPTE29BASE;
			break;
		case 4:
			lNewTimeMode = MIDIDATA_SMPTE30BASE;
			break;
		}
		long lNewTimeResolution = theDlg.m_nResolution;
		// タイムモード又は分解能が変更された場合
		if (lNewTimeMode != lTimeMode ||
			lNewTimeResolution != lTimeResolution) {
			CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
			
			// 演奏中の場合、演奏の一時停止
			pSekaijuApp->SendAllNoteOff ();
			pSekaijuApp->SendAllHold1Off ();
			pSekaijuApp->SendAllSostenutoOff ();
			pSekaijuApp->SendAllHold2Off ();
			if (pSekaijuApp->m_bPlaying) {
				MIDIClock_Stop (m_pMIDIClock);
			}
			long lTempo = MIDIEVENT_DEFTEMPO;
			VERIFY (MIDIData_FindTempo (m_pMIDIData, 0, &lTempo));

			// 全オブジェクトの履歴記録(20090116履歴記録方法修正)
			forEachTrackInverse (m_pMIDIData, pMIDITrack) {
				if (MIDITrack_GetFirstEvent (pMIDITrack)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord 
					(HISTORYRECORD_REMOVEEVENTALL, MIDITrack_GetFirstEvent (pMIDITrack)));
				}
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack));
			}
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEDATA, m_pMIDIData));

			// 全オブジェクトの交換
			forEachTrackInverse (m_pMIDIData, pMIDITrack) {
				forEachEventInverse (pMIDITrack, pMIDIEvent) {
					if (pMIDIEvent->m_pPrevCombinedEvent == NULL) {
						VERIFY (pCloneEvent = this->ReplaceMIDIEvent (pMIDIEvent));
						pMIDIEvent = pCloneEvent;
					}
				}
				VERIFY (pCloneTrack = this->ReplaceMIDITrack (pMIDITrack));
				pMIDITrack = pCloneTrack;
			}
			VERIFY (m_pMIDIData = this->ReplaceMIDIData (m_pMIDIData));
			
			// タイムベース(タイムモード及び分解能)の変換
			MIDIData_SetTimeBase (m_pMIDIData, lNewTimeMode, lNewTimeResolution);

			// 全オブジェクトの履歴記録(20090116履歴記録方法修正)
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTDATA, m_pMIDIData));
			forEachTrack (m_pMIDIData, pMIDITrack) {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pMIDITrack));
				if (MIDITrack_GetFirstEvent (pMIDITrack)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord 
					(HISTORYRECORD_INSERTEVENTALL, MIDITrack_GetFirstEvent (pMIDITrack)));
				}
			}
			
			// MIDIクロックのタイムモードと速度をアプリケーションに合わせる(20090625追加)
			ApplyAppCurSpeedIndex ();

			MIDIClock_Reset (m_pMIDIClock);
			MIDIClock_SetTempo (m_pMIDIClock, lTempo);

			// 演奏中だった場合、演奏の続行
			if (pSekaijuApp->m_bPlaying) {
				MIDIClock_Start (m_pMIDIClock);
			}
		}
		SetModifiedFlag (TRUE);
		UpdateAllViews (NULL, SEKAIJUDOC_MIDIDATACHANGED | SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
	}
}

// 『ファイル(&F)』-『プロパティ(&R)』
void CSekaijuDoc::OnUpdateFilePropertyUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}

}


// 『編集(&E)』-『元に戻す(&U)』
void CSekaijuDoc::OnEditUndo () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	CString strMsg;
	this->m_theCriticalSection.Lock ();
	long lNumHistoryUnit = m_theHistoryUnitArray.GetSize ();
	// 現在の履歴ユニットがない場合
	if (m_lCurHistoryPosition <= -1) {
		this->m_theCriticalSection.Unlock ();
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_UNDO_ANYMORE));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	
	BeginWaitCursor ();
	// 現在の履歴ユニットを取得
	CHistoryUnit* pHistoryUnit = 
		(CHistoryUnit*)m_theHistoryUnitArray.GetAt (m_lCurHistoryPosition);
	long lHistoryRecordCount = pHistoryUnit->m_theHistoryRecordArray.GetSize ();
	long i;
	MIDIData* pMIDIData = NULL;
	MIDITrack* pPrevTrack = NULL;
	MIDITrack* pNextTrack = NULL;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pPrevEvent = NULL;
	MIDIEvent* pNextEvent = NULL;
	MIDIEvent* pFirstEvent = NULL;
	MIDIEvent* pLastEvent = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDITrack* pTempTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	MIDITrack* pFirstTrack = NULL;
	MIDITrack* pLastTrack = NULL;
	MIDIData* pTempData = NULL;
	
	// 履歴ユニット内にあるすべての記録について、後方から評価
	for (i = lHistoryRecordCount - 1; i >= 0; i--) {
		CHistoryRecord* pHistoryRecord = 
			(CHistoryRecord*)(pHistoryUnit->m_theHistoryRecordArray.GetAt (i));
		switch (pHistoryRecord->m_lType) {
		// MIDIEventオブジェクトの挿入記録→MIDIEventオブジェクトの排除
		case HISTORYRECORD_INSERTEVENT:
			pMIDIEvent = (MIDIEvent*)(pHistoryRecord->m_pObject);
			pPrevEvent = (MIDIEvent*)(pHistoryRecord->m_pPrevObject);
			pNextEvent = (MIDIEvent*)(pHistoryRecord->m_pNextObject);
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pParent);
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_DEAD;
			VERIFY (MIDITrack_RemoveSingleEvent (pMIDITrack, pMIDIEvent));
			break;
		// MIDIEventオブジェクトの除去記録→MIDIEventオブジェクトの復元
		case HISTORYRECORD_REMOVEEVENT:
			pMIDIEvent = (MIDIEvent*)(pHistoryRecord->m_pObject);
			pPrevEvent = (MIDIEvent*)(pHistoryRecord->m_pPrevObject);
			pNextEvent = (MIDIEvent*)(pHistoryRecord->m_pNextObject);
			pFirstEvent = (MIDIEvent*)(pHistoryRecord->m_pFirstChild);
			pLastEvent = (MIDIEvent*)(pHistoryRecord->m_pLastChild);
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pParent);
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
			// 20090116 MIDIEventの挿入位置決定方法変更
			if (pNextEvent == NULL) {
				VERIFY (MIDITrack_InsertSingleEventBefore (pMIDITrack, pMIDIEvent, pNextEvent));
			}
			else if (MIDIEvent_GetParent (pNextEvent) != NULL) {
				VERIFY (MIDITrack_InsertSingleEventBefore (pMIDITrack, pMIDIEvent, pNextEvent));
			}
			else if (pPrevEvent == NULL) {
				VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pPrevEvent));
			}
			else if (MIDIEvent_GetParent (pPrevEvent) != NULL) {
				VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pPrevEvent));
			}
			else {
				long lInsertTime = MIDIEvent_GetTime (pMIDIEvent);
				pTempEvent = pMIDITrack->m_pLastEvent;
				// トラックの後方から挿入位置を探索
				while (1) {
					// トラックにデータがない、又はトラックの先頭入れてよい
					if (pTempEvent == NULL) {
						VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, NULL));
						break;
					}
					// pTempEventの直後に入れてよい
					else {
						// 挿入するものがノートオフイベントの場合(ベロシティ0のノートオンを含む)
						if (MIDIEvent_IsNoteOff (pMIDIEvent)) {
							// 対応するノートオンイベントより前には絶対に来れない (20090111追加)
							if (pTempEvent == pMIDIEvent->m_pPrevCombinedEvent) {
								VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pTempEvent));
								break;
							}
							// 同時刻のイベントがある場合は同時刻の他のノートオフの直後に挿入
							else if (pTempEvent->m_lTime == lInsertTime && MIDIEvent_IsNoteOff (pTempEvent)) {
								VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pTempEvent));
								break;
							}
							else if (pTempEvent->m_lTime < lInsertTime) {
								VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pTempEvent));
								break;
							}
						}
						// その他のイベントの場合
						else {
							if (pTempEvent->m_lTime <= lInsertTime) {
								VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pTempEvent));
								break;
							}
						}
					}
					pTempEvent = pTempEvent->m_pPrevEvent;
				}
			}
			break;
		// MIDIEventオブジェクトの全挿入記録→全MIDIEventオブジェクトの排除
		case HISTORYRECORD_INSERTEVENTALL:
			pMIDIEvent = (MIDIEvent*)(pHistoryRecord->m_pObject);
			pPrevEvent = (MIDIEvent*)(pHistoryRecord->m_pPrevObject);
			pNextEvent = (MIDIEvent*)(pHistoryRecord->m_pNextObject);
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pParent);
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_DEAD;
			VERIFY (MIDITrack_RemoveSingleEvent (pMIDITrack, pMIDIEvent));
			break;
		// MIDIEventオブジェクトの全除去記録→全MIDIEventオブジェクトの復元
		case HISTORYRECORD_REMOVEEVENTALL:
			pMIDIEvent = (MIDIEvent*)(pHistoryRecord->m_pObject);
			pPrevEvent = (MIDIEvent*)(pHistoryRecord->m_pPrevObject);
			pNextEvent = (MIDIEvent*)(pHistoryRecord->m_pNextObject);
			pFirstEvent = (MIDIEvent*)(pHistoryRecord->m_pFirstChild);
			pLastEvent = (MIDIEvent*)(pHistoryRecord->m_pLastChild);
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pParent);
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
			// 20090116 MIDIEventの挿入位置決定方法変更
			if (pNextEvent == NULL) {
				VERIFY (MIDITrack_InsertSingleEventBefore (pMIDITrack, pMIDIEvent, pNextEvent));
			}
			else if (MIDIEvent_GetParent (pNextEvent) != NULL) {
				VERIFY (MIDITrack_InsertSingleEventBefore (pMIDITrack, pMIDIEvent, pNextEvent));
			}
			else if (pPrevEvent == NULL) {
				VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pPrevEvent));
			}
			else if (MIDIEvent_GetParent (pPrevEvent) != NULL) {
				VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pPrevEvent));
			}
			else {
				ASSERT (FALSE);
			}
			break;
		// MIDITrackオブジェクトの挿入記録→MIDITrackオブジェクトの排除
		case HISTORYRECORD_INSERTTRACK:
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pObject);
			pPrevTrack = (MIDITrack*)(pHistoryRecord->m_pPrevObject);
			pNextTrack = (MIDITrack*)(pHistoryRecord->m_pNextObject);
			pMIDIData = (MIDIData*)(pHistoryRecord->m_pParent);
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_ALIVE;
			pMIDITrack->m_lUserFlag |= MIDITRACK_DEAD;
			VERIFY (MIDIData_RemoveTrack (pMIDIData, pMIDITrack));
			break;
		// MIDITrackオブジェクトの除去記録→MIDITrackオブジェクトの復元
		case HISTORYRECORD_REMOVETRACK:
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pObject);
			pPrevTrack = (MIDITrack*)(pHistoryRecord->m_pPrevObject);
			pNextTrack = (MIDITrack*)(pHistoryRecord->m_pNextObject);
			pFirstEvent = (MIDIEvent*)(pHistoryRecord->m_pFirstChild);
			pLastEvent = (MIDIEvent*)(pHistoryRecord->m_pLastChild);
			pMIDIData = (MIDIData*)(pHistoryRecord->m_pParent);
			pMIDITrack->m_lUserFlag |= MIDITRACK_ALIVE;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_DEAD;
			if (pFirstEvent || pLastEvent) {
				VERIFY (pTempEvent = pFirstEvent);
				pTempTrack = MIDIEvent_GetParent (pFirstEvent);
				if (pTempTrack != NULL && pTempTrack != pMIDITrack) {
					while (pTempEvent) {
						VERIFY (MIDITrack_RemoveEvent (pTempTrack, pTempEvent));
						VERIFY (MIDITrack_InsertEvent (pMIDITrack, pTempEvent));
						pTempEvent = MIDITrack_GetFirstEvent (pTempTrack);
					}
				}
			}
			// 20090116 MIDITrackの挿入位置決定方法変更
			if (pNextTrack == NULL) {
				VERIFY (MIDIData_InsertTrackBefore (pMIDIData, pMIDITrack, pNextTrack));
			}
			else if (MIDITrack_GetParent (pNextTrack) == pMIDIData) {
				VERIFY (MIDIData_InsertTrackBefore (pMIDIData, pMIDITrack, pNextTrack));
			}
			else if (pPrevTrack == NULL) {
				VERIFY (MIDIData_InsertTrackAfter (pMIDIData, pMIDITrack, pPrevTrack));
			}
			else if (MIDITrack_GetParent (pPrevTrack) == pMIDIData) {
				VERIFY (MIDIData_InsertTrackAfter (pMIDIData, pMIDITrack, pPrevTrack));
			}
			else {
				ASSERT (FALSE);
			}
			break;
		// MIDIDataオブジェクトの挿入記録→MIDIDataオブジェクトの排除
		case HISTORYRECORD_INSERTDATA:
			pMIDIData = (MIDIData*)(pHistoryRecord->m_pObject);
			pMIDIData->m_lUserFlag &= ~MIDIDATA_ALIVE;
			pMIDIData->m_lUserFlag |= MIDIDATA_DEAD;
			this->m_pMIDIData = NULL;
			break;
		// MIDIDataオブジェクトの除去記録→MIDIDataオブジェクトの復元
		case HISTORYRECORD_REMOVEDATA:
			pMIDIData = (MIDIData*)(pHistoryRecord->m_pObject);
			pFirstTrack = (MIDITrack*)(pHistoryRecord->m_pFirstChild);
			pLastTrack = (MIDITrack*)(pHistoryRecord->m_pLastChild);
			pMIDIData->m_lUserFlag |= MIDIDATA_ALIVE;
			pMIDIData->m_lUserFlag &= ~MIDIDATA_DEAD;
			if (pFirstTrack || pLastTrack) {
				VERIFY (pTempTrack = pFirstTrack);
				pTempData = MIDITrack_GetParent (pFirstTrack);
				if (pTempData != NULL && pTempData != pMIDIData) {
					while (pTempTrack) {
						VERIFY (MIDIData_RemoveTrack (pTempData, pTempTrack));
						VERIFY (MIDIData_AddTrack (pMIDIData, pTempTrack));
						pTempTrack = MIDIData_GetFirstTrack (pTempData);
					}
				}
			}
			this->m_pMIDIData = pMIDIData;
			break;
		}
	}

	// 必要に応じてMIDIClockの補正
	long lMIDIClockTimeMode;
	long lMIDIClockTimeResolution;
	MIDIClock_GetTimeBase (m_pMIDIClock, &lMIDIClockTimeMode, &lMIDIClockTimeResolution);
	long lMIDIDataTimeMode;
	long lMIDIDataTimeResolution;
	MIDIData_GetTimeBase (m_pMIDIData, &lMIDIDataTimeMode, &lMIDIDataTimeResolution);
	if (lMIDIClockTimeMode != lMIDIDataTimeMode || 
		lMIDIClockTimeResolution != lMIDIDataTimeResolution) {
		// 演奏中の場合、演奏の一時停止
		pSekaijuApp->SendAllNoteOff ();
		pSekaijuApp->SendAllHold1Off ();
		pSekaijuApp->SendAllSostenutoOff ();
		pSekaijuApp->SendAllHold2Off ();
		if (pSekaijuApp->m_bPlaying) {
			MIDIClock_Stop (m_pMIDIClock);
		}
		long lTempo = MIDIEVENT_DEFTEMPO;
		VERIFY (MIDIData_FindTempo (m_pMIDIData, 0, &lTempo));
		MIDIClock_Reset (m_pMIDIClock);
		MIDIClock_SetTimeBase (m_pMIDIClock, lMIDIDataTimeMode, lMIDIDataTimeResolution);
		MIDIClock_SetTempo (m_pMIDIClock, lTempo);
		if (pSekaijuApp->m_bPlaying) {
			MIDIClock_Start (m_pMIDIClock);
		}
	}

	// 現在の履歴ユニット番号をデクリメント
	this->m_lCurHistoryPosition--;
	this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIDATACHANGED | 
		SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIEVENTCHANGED);
	this->SetModifiedFlag (TRUE);
	this->m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	return;
}

// 『編集(&E)』-『元に戻す(&U)』
void CSekaijuDoc::OnUpdateEditUndoUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	m_theCriticalSection.Lock ();
	long lNumHistoryUnit = m_theHistoryUnitArray.GetSize ();
	// 元に戻せない場合
	if (m_lCurHistoryPosition <= -1) {
		CString strText;
		VERIFY (strText.LoadString (IDS_UNABLE_TO_UNDO_T_CTRL_Z));
		pCmdUI->SetText (strText);
		pCmdUI->Enable (FALSE);
	}
	// 元に戻せる場合
	else {
		CHistoryUnit* pHistoryUnit = 
			(CHistoryUnit*)m_theHistoryUnitArray.GetAt (m_lCurHistoryPosition);
		CString strFormat;
		CString strText;
		VERIFY (strFormat.LoadString (IDS_UNDO_02D_02D_02D_S_T_CTRL_Z));
		strText.Format (strFormat,
			pHistoryUnit->m_theTime.GetHour (),
			pHistoryUnit->m_theTime.GetMinute (),
			pHistoryUnit->m_theTime.GetSecond (),
			pHistoryUnit->m_strName);
		pCmdUI->SetText (strText);
		pCmdUI->Enable (TRUE);
	}
	m_theCriticalSection.Unlock ();
}


// 『編集(&E)』-『やり直し(&R)』
void CSekaijuDoc::OnEditRedo () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}
	
	this->m_theCriticalSection.Lock ();
	long lNumHistoryUnit = m_theHistoryUnitArray.GetSize ();
	// 次の履歴ユニットがない場合
	if (m_lCurHistoryPosition >= lNumHistoryUnit - 1) {
		this->m_theCriticalSection.Unlock ();
		CString strText;
		VERIFY (strText.LoadString (IDS_UNABLE_TO_REDO_ANYMORE));
		AfxMessageBox (strText, MB_ICONEXCLAMATION);
		return;
	}
	
	BeginWaitCursor ();
	// 次の履歴ユニットを取得
	CHistoryUnit* pHistoryUnit = 
		(CHistoryUnit*)m_theHistoryUnitArray.GetAt (m_lCurHistoryPosition + 1);

	long lHistoryRecordCount = pHistoryUnit->m_theHistoryRecordArray.GetSize ();
	long i;
	MIDIData* pMIDIData = NULL;
	MIDITrack* pPrevTrack = NULL;
	MIDITrack* pNextTrack = NULL;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pPrevEvent = NULL;
	MIDIEvent* pNextEvent = NULL;
	MIDIEvent* pFirstEvent = NULL;
	MIDIEvent* pLastEvent = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDITrack* pTempTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	MIDITrack* pFirstTrack = NULL;
	MIDITrack* pLastTrack = NULL;
	MIDIData* pTempData = NULL;

	// 履歴ユニット内にあるすべての記録について、前方から評価
	for (i = 0; i < lHistoryRecordCount; i++) {
		CHistoryRecord* pHistoryRecord = 
			(CHistoryRecord*)(pHistoryUnit->m_theHistoryRecordArray.GetAt (i));
		switch (pHistoryRecord->m_lType) {
		// MIDIEventオブジェクトの挿入記録→MIDIEventオブジェクトの復元
		case HISTORYRECORD_INSERTEVENT:
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pParent);
			pMIDIEvent = (MIDIEvent*)(pHistoryRecord->m_pObject);
			pPrevEvent = (MIDIEvent*)(pHistoryRecord->m_pPrevObject);
			pNextEvent = (MIDIEvent*)(pHistoryRecord->m_pNextObject);
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
			// 20090116 MIDIEventの挿入位置決定方法変更
			if (pPrevEvent == NULL) {
				VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pPrevEvent));
			}
			else if (MIDIEvent_GetParent (pPrevEvent) != NULL) {
				VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pPrevEvent));
			}
			else if (pNextEvent == NULL) {
				VERIFY (MIDITrack_InsertSingleEventBefore (pMIDITrack, pMIDIEvent, pNextEvent));
			}
			else if (MIDIEvent_GetParent (pNextEvent) != NULL) {
				VERIFY (MIDITrack_InsertSingleEventBefore (pMIDITrack, pMIDIEvent, pNextEvent));
			}
			else {
				long lInsertTime = MIDIEvent_GetTime (pMIDIEvent);
				pTempEvent = pMIDITrack->m_pLastEvent;
				// トラックの後方から挿入位置を探索
				while (1) {
					// トラックにデータがない、又はトラックの先頭入れてよい
					if (pTempEvent == NULL) {
						VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, NULL));
						break;
					}
					// pTempEventの直後に入れてよい
					else {
						// 挿入するものがノートオフイベントの場合(ベロシティ0のノートオンを含む)
						if (MIDIEvent_IsNoteOff (pMIDIEvent)) {
							// 対応するノートオンイベントより前には絶対に来れない (20090111追加)
							if (pTempEvent == pMIDIEvent->m_pPrevCombinedEvent) {
								VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pTempEvent));
								break;
							}
							// 同時刻のイベントがある場合は同時刻の他のノートオフの直後に挿入
							else if (pTempEvent->m_lTime == lInsertTime && MIDIEvent_IsNoteOff (pTempEvent)) {
								VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pTempEvent));
								break;
							}
							else if (pTempEvent->m_lTime < lInsertTime) {
								VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pTempEvent));
								break;
							}
						}
						// その他のイベントの場合
						else {
							if (pTempEvent->m_lTime <= lInsertTime) {
								VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pTempEvent));
								break;
							}
						}
					}
					pTempEvent = pTempEvent->m_pPrevEvent;
				}
			}
			//if (MIDIEvent_IsNoteOff (pMIDIEvent)) {
			//	if (pMIDIEvent->m_pPrevEvent) {
			//		if (pMIDIEvent->m_pPrevEvent->m_lTime == pMIDIEvent->m_lTime) {
			//			ASSERT (MIDIEvent_IsNoteOff (pMIDIEvent->m_pPrevEvent));
			//		}
			//	}
			//}
			break;
		// MIDIEventオブジェクトの除去記録→MIDIEventオブジェクトの排除
		case HISTORYRECORD_REMOVEEVENT:
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pParent);
			pMIDIEvent = (MIDIEvent*)(pHistoryRecord->m_pObject);
			pPrevEvent = (MIDIEvent*)(pHistoryRecord->m_pPrevObject);
			pNextEvent = (MIDIEvent*)(pHistoryRecord->m_pNextObject);
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_DEAD;
			VERIFY (MIDITrack_RemoveSingleEvent (pMIDITrack, pMIDIEvent));
			break;

		// MIDIEventオブジェクトの全挿入記録→全MIDIEventオブジェクトの復元
		case HISTORYRECORD_INSERTEVENTALL:
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pParent);
			pMIDIEvent = (MIDIEvent*)(pHistoryRecord->m_pObject);
			pPrevEvent = (MIDIEvent*)(pHistoryRecord->m_pPrevObject);
			pNextEvent = (MIDIEvent*)(pHistoryRecord->m_pNextObject);
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
			// 20090116 MIDIEventの挿入位置決定方法変更
			if (pPrevEvent == NULL) {
				VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pPrevEvent));
			}
			else if (MIDIEvent_GetParent (pPrevEvent) != NULL) {
				VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pPrevEvent));
			}
			else if (pNextEvent == NULL) {
				VERIFY (MIDITrack_InsertSingleEventBefore (pMIDITrack, pMIDIEvent, pNextEvent));
			}
			else if (MIDIEvent_GetParent (pNextEvent) != NULL) {
				VERIFY (MIDITrack_InsertSingleEventBefore (pMIDITrack, pMIDIEvent, pNextEvent));
			}
			else {
				ASSERT (FALSE);
			}
			break;
		// MIDIEventオブジェクトの全除去記録→全MIDIEventオブジェクトの排除
		case HISTORYRECORD_REMOVEEVENTALL:
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pParent);
			pMIDIEvent = (MIDIEvent*)(pHistoryRecord->m_pObject);
			pPrevEvent = (MIDIEvent*)(pHistoryRecord->m_pPrevObject);
			pNextEvent = (MIDIEvent*)(pHistoryRecord->m_pNextObject);
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_DEAD;
			VERIFY (MIDITrack_RemoveSingleEvent (pMIDITrack, pMIDIEvent));
			break;

		// MIDITrackオブジェクトの挿入記録→MIDITrackオブジェクトの復元
		case HISTORYRECORD_INSERTTRACK:
			pMIDIData = (MIDIData*)(pHistoryRecord->m_pParent);
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pObject);
			pPrevTrack = (MIDITrack*)(pHistoryRecord->m_pPrevObject);
			pNextTrack = (MIDITrack*)(pHistoryRecord->m_pNextObject);
			pFirstEvent = (MIDIEvent*)(pHistoryRecord->m_pFirstChild);
			pLastEvent = (MIDIEvent*)(pHistoryRecord->m_pLastChild);
			pMIDITrack->m_lUserFlag |= MIDITRACK_ALIVE;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_DEAD;
			if (pFirstEvent || pLastEvent) {
				VERIFY (pTempEvent = pFirstEvent);
				pTempTrack = MIDIEvent_GetParent (pFirstEvent);
				if (pTempTrack != NULL && pTempTrack != pMIDITrack) {
					while (pTempEvent) {
						VERIFY (MIDITrack_RemoveEvent (pTempTrack, pTempEvent));
						VERIFY (MIDITrack_InsertEvent (pMIDITrack, pTempEvent));
						pTempEvent = MIDITrack_GetFirstEvent (pTempTrack);
					}
				}
			}
			// 20090116 MIDITrackの挿入位置決定方法変更
			if (pPrevTrack == NULL) {
				VERIFY (MIDIData_InsertTrackAfter (pMIDIData, pMIDITrack, pPrevTrack));
			}
			else if (MIDITrack_GetParent (pPrevTrack) == pMIDIData) {
				VERIFY (MIDIData_InsertTrackAfter (pMIDIData, pMIDITrack, pPrevTrack));
			}
			else if (pNextTrack == NULL) {
				VERIFY (MIDIData_InsertTrackBefore (pMIDIData, pMIDITrack, pNextTrack));
			}
			else if (MIDITrack_GetParent (pNextTrack) == pMIDIData) {
				VERIFY (MIDIData_InsertTrackBefore (pMIDIData, pMIDITrack, pNextTrack));
			}
			else {
				ASSERT (FALSE);
			}
			break;
		// MIDITrackオブジェクトの除去記録→MIDITrackオブジェクトの排除
		case HISTORYRECORD_REMOVETRACK:
			pMIDIData = (MIDIData*)(pHistoryRecord->m_pParent);
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pObject);
			pPrevTrack = (MIDITrack*)(pHistoryRecord->m_pPrevObject);
			pNextTrack = (MIDITrack*)(pHistoryRecord->m_pNextObject);
			pFirstEvent = (MIDIEvent*)(pHistoryRecord->m_pFirstChild);
			pLastEvent = (MIDIEvent*)(pHistoryRecord->m_pLastChild);
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_ALIVE;
			pMIDITrack->m_lUserFlag |= MIDITRACK_DEAD;
			VERIFY (MIDIData_RemoveTrack (pMIDIData, pMIDITrack));
			break;
		// MIDIDataオブジェクトの挿入記録→MIDIDataオブジェクトの復元
		case HISTORYRECORD_INSERTDATA:
			pMIDIData = (MIDIData*)(pHistoryRecord->m_pObject);
			pFirstTrack = (MIDITrack*)(pHistoryRecord->m_pFirstChild);
			pLastTrack = (MIDITrack*)(pHistoryRecord->m_pLastChild);
			pMIDIData->m_lUserFlag |= MIDIDATA_ALIVE;
			pMIDIData->m_lUserFlag &= ~MIDIDATA_DEAD;
			if (pFirstTrack || pLastTrack) {
				VERIFY (pTempTrack = pFirstTrack);
				pTempData = MIDITrack_GetParent (pFirstTrack);
				if (pTempData != NULL && pTempData != pMIDIData) {
					while (pTempTrack) {
						VERIFY (MIDIData_RemoveTrack (pTempData, pTempTrack));
						VERIFY (MIDIData_AddTrack (pMIDIData, pTempTrack));
						pTempTrack = MIDIData_GetFirstTrack (pTempData);
					}
				}
			}
			this->m_pMIDIData = pMIDIData;
			break;
		// MIDIDataオブジェクトの除去記録→MIDIDataオブジェクトの排除
		case HISTORYRECORD_REMOVEDATA:
			pMIDIData = (MIDIData*)(pHistoryRecord->m_pObject);
			pFirstTrack = (MIDITrack*)(pHistoryRecord->m_pFirstChild);
			pLastTrack = (MIDITrack*)(pHistoryRecord->m_pLastChild);
			pMIDIData->m_lUserFlag &= ~MIDIDATA_ALIVE;
			pMIDIData->m_lUserFlag |= MIDIDATA_DEAD;
			this->m_pMIDIData = NULL;
			break;
		}
	}

	// 必要に応じてMIDIClockの補正
	long lMIDIClockTimeMode;
	long lMIDIClockTimeResolution;
	MIDIClock_GetTimeBase (m_pMIDIClock, &lMIDIClockTimeMode, &lMIDIClockTimeResolution);
	long lMIDIDataTimeMode;
	long lMIDIDataTimeResolution;
	MIDIData_GetTimeBase (m_pMIDIData, &lMIDIDataTimeMode, &lMIDIDataTimeResolution);
	if (lMIDIClockTimeMode != lMIDIDataTimeMode || 
		lMIDIClockTimeResolution != lMIDIDataTimeResolution) {
		// 演奏中の場合、演奏の一時停止
		pSekaijuApp->SendAllNoteOff ();
		pSekaijuApp->SendAllHold1Off ();
		pSekaijuApp->SendAllSostenutoOff ();
		pSekaijuApp->SendAllHold2Off ();
		if (pSekaijuApp->m_bPlaying) {
			MIDIClock_Stop (m_pMIDIClock);
		}
		long lTempo = MIDIEVENT_DEFTEMPO;
		VERIFY (MIDIData_FindTempo (m_pMIDIData, 0, &lTempo));
		MIDIClock_Reset (m_pMIDIClock);
		MIDIClock_SetTimeBase (m_pMIDIClock, lMIDIDataTimeMode, lMIDIDataTimeResolution);
		MIDIClock_SetTempo (m_pMIDIClock, lTempo);
		if (pSekaijuApp->m_bPlaying) {
			MIDIClock_Start (m_pMIDIClock);
		}
	}
	
	// 現在の履歴ユニット番号をインクリメント
	this->m_lCurHistoryPosition++;
	this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIDATACHANGED | 
		SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIEVENTCHANGED);
	this->SetModifiedFlag (TRUE);
	this->m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	return;
}

// 『編集(&E)』-『やり直し(&R)』
void CSekaijuDoc::OnUpdateEditRedoUI (CCmdUI* pCmdUI) {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	m_theCriticalSection.Lock ();
	long lNumHistoryUnit = m_theHistoryUnitArray.GetSize ();
	// やり直しできない場合
	if (m_lCurHistoryPosition >= lNumHistoryUnit - 1) {
		CString strText;
		VERIFY (strText.LoadString (IDS_UNABLE_TO_REDO_T_CTRL_Y));
		pCmdUI->SetText (strText);
		pCmdUI->Enable (FALSE);
	}
	// やり直しできる場合
	else {
		CHistoryUnit* pHistoryUnit = 
			(CHistoryUnit*)m_theHistoryUnitArray.GetAt (m_lCurHistoryPosition + 1);
		CString strFormat;
		CString strText;
		VERIFY (strFormat.LoadString (IDS_REDO_02D_02D_02D_S_T_CTRL_Y));
		strText.Format (strFormat,
			pHistoryUnit->m_theTime.GetHour (),
			pHistoryUnit->m_theTime.GetMinute (),
			pHistoryUnit->m_theTime.GetSecond (),
			pHistoryUnit->m_strName);
		pCmdUI->SetText (strText);
		pCmdUI->Enable (TRUE);
	}
	m_theCriticalSection.Unlock ();
}


// 『編集(&E)』-『作業履歴の初期化』
void CSekaijuDoc::OnEditInitHistory () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}
	
	CString strMsg;
	VERIFY (strMsg.LoadString (IDS_INITIALIZE_HISTORY));
	int nRet = AfxMessageBox (strMsg, MB_ICONINFORMATION | MB_YESNO);

	if (nRet == IDYES) {
		BeginWaitCursor ();
		this->m_theCriticalSection.Lock ();
		this->DeleteAllHistoryUnit ();
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIDATACHANGED | 
			SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIEVENTCHANGED);
		this->SetModifiedFlag (TRUE);
		this->m_theCriticalSection.Unlock ();
		EndWaitCursor ();
	}

}



// 『編集(&E)』-『作業履歴の初期化』
void CSekaijuDoc::OnUpdateEditInitHistoryUI (CCmdUI* pCmdUI) {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	long lNumHistoryUnit = m_theHistoryUnitArray.GetSize ();
	if (lNumHistoryUnit <= 0) {
		pCmdUI->Enable (FALSE);
	}
}




// 『編集(E)』-『切り取り(T)』
void CSekaijuDoc::OnEditCut () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	long lRet = 0;
	long lCopiedEventCount = 0;
	BeginWaitCursor ();

	// クリップボードに転送する文字列を作成
	CString strText;
	m_theCriticalSection.Lock ();
	lCopiedEventCount = this->MakeCopyString (strText, 0x0000, NULL);
	UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
	if (lCopiedEventCount == 0) {
		EndWaitCursor ();
		return;
	}

	// クリップボードに独自テキストを転送
	lRet = this->SetClipboardTextPrivate9 (strText);
	if (lRet == FALSE) {
		EndWaitCursor ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_WRITE_TO_CLIPBOARD));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}

	// 選択イベントを削除
	CHistoryUnit* pCurHistoryUnit = NULL;
	m_theCriticalSection.Lock ();
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_EDIT_CUT));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());

	this->DeleteSelectedEvent (pCurHistoryUnit);
	
	SetModifiedFlag (TRUE);
	UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);

}

// 『編集(E)』-『切り取り(T)』
void CSekaijuDoc::OnUpdateEditCutUI (CCmdUI* pCmdUI) {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}


	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack;
	MIDIEvent* pMIDIEvent;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		if (this->IsTrackSelected (pMIDITrack)) {
			lEnable = 1;
		}
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}


// 『編集(E)』-『コピー(C)』
void CSekaijuDoc::OnEditCopy () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}


	long lRet = 0;
	long lCopiedEventCount = 0;
	BeginWaitCursor ();

	// クリップボードに転送する文字列を作成
	CString strText;
	m_theCriticalSection.Lock ();
	lCopiedEventCount = this->MakeCopyString (strText, 0x0000, NULL);
	UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
	if (lCopiedEventCount == 0) {
		EndWaitCursor ();
		return;
	}

	// クリップボードに独自テキストを転送
	lRet = this->SetClipboardTextPrivate9 (strText);
	if (lRet == FALSE) {
		EndWaitCursor ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_WRITE_TO_CLIPBOARD));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	EndWaitCursor ();
}

// 『編集(E)』-『コピー(C)』
void CSekaijuDoc::OnUpdateEditCopyUI (CCmdUI* pCmdUI) {
	
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	// ひとつでもMIDIイベントが選択されていればEnable
	// そうでなければDisableとする
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack;
	MIDIEvent* pMIDIEvent;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);

}


// 『編集(E)』-『貼り付け(P)』
void CSekaijuDoc::OnEditPaste () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}


	BeginWaitCursor ();
	CString strMsg;

	// クリップボードのテキストを取得
	CString strText;
	long lRet = this->GetClipboardTextPrivate9 (strText);
	if (lRet == FALSE) {
		EndWaitCursor ();
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_READ_FROM_CLIPBOARD));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	
	// テキストフォーマット検査
	if (strText.Left(5) != _T("MDa2 ")) {
		EndWaitCursor ();
		VERIFY (strMsg.LoadString (IDS_NO_AVAILABLE_MIDIDATA_IN_THE_CLIPBOARD));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	
	// 貼り付け処理
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_EDIT_PASTE));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	m_theCriticalSection.Lock ();
	m_lTempTrackIndex = 0;
	long lMeasure, lBeat, lTick;
	long lTime = m_lNewTime;
	MIDIData_BreakTime (m_pMIDIData, lTime, &lMeasure, &lBeat, &lTick);
	MIDIData_MakeTime (m_pMIDIData, lMeasure, 0, 0, &lTime);
	m_lTempTime = lTime;
	long lInsertedEventCount = this->ParsePasteString (strText, m_lTempTrackIndex, m_lTempTime, pCurHistoryUnit);
	this->SetModifiedFlag (TRUE);
	this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED |
		SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED);
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);

	// 結果のメッセージボックス表示
	if (lInsertedEventCount == -1) {
		VERIFY (strMsg.LoadString (IDS_PASTE_FAILED));
	}
	else {
		CString strFormat;
		VERIFY (strFormat.LoadString (IDS_D_MIDIEVENTS_WERE_INSERTED));
		strMsg.Format (strFormat, lInsertedEventCount);
	}
	AfxMessageBox (strMsg, MB_ICONINFORMATION);
}

// 『編集(E)』-『貼り付け(P)』
void CSekaijuDoc::OnUpdateEditPasteUI (CCmdUI* pCmdUI) {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	
	// クリップボードの形式が独自テキストか調べる
	pCmdUI->Enable (this->IsClipboardTextPrivate9 ());
}

// 『編集(E)』-『削除(D)』
void CSekaijuDoc::OnEditDelete () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	
	BeginWaitCursor ();
	CHistoryUnit* pCurHistoryUnit = NULL;
	m_theCriticalSection.Lock ();
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_EDIT_DELETE));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());

	this->DeleteSelectedEvent (pCurHistoryUnit);

	SetModifiedFlag (TRUE);
	UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);
}

// 『編集(E)』-『削除(D)』
void CSekaijuDoc::OnUpdateEditDeleteUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}


	// MIDIイベントがひとつでも選択状態ならEnable
	// そうでなければDisable
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}


// 『編集(E)』-『すべて選択(A)』
void CSekaijuDoc::OnEditSelectAll () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	
	// すべてのMIDIイベントを選択状態にする
	BeginWaitCursor ();
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDIEvent* pCloneEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	m_theCriticalSection.Lock ();
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_EDIT_SELECT_ALL));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (pMIDIEvent->m_pPrevCombinedEvent == NULL) {
				pCloneEvent = this->SelectEvent (pMIDIEvent, 1, pCurHistoryUnit);
				if (pCloneEvent) {
					pMIDIEvent = pCloneEvent;
				}
			}
		}
	}
	this->SetModifiedFlag (TRUE);
	this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED | 
		SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED);
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);
}

// 『編集(E)』-『すべて選択(A)』
void CSekaijuDoc::OnUpdateEditSelectAllUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}


	// すべてのイベントが選択されていればこの項目をチェックする
	m_theCriticalSection.Lock ();
	long lCheck = 1;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent) == 0) {
				lCheck = 0;
				break;
			}
		}
		if (lCheck == 0) {
			break;
		}
	}
	pCmdUI->SetCheck (lCheck);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
	
}

// 『編集(E)』-『すべて選択解除(N)』
void CSekaijuDoc::OnEditSelectNone () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}


	
	// すべてのMIDIイベントを非選択状態にする
	BeginWaitCursor ();
	CHistoryUnit* pCurHistoryUnit = NULL;
	m_theCriticalSection.Lock ();
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_EDIT_DESELECT_ALL));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	this->SelectNoObject (pCurHistoryUnit);
	this->SetModifiedFlag (TRUE);
	this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED | 
		SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED);
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);
}

// 『編集(E)』-『すべて選択解除(N)』
void CSekaijuDoc::OnUpdateEditSelectNoneUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	// すべてのイベントが非選択であればこの項目をチェックする
	m_theCriticalSection.Lock ();
	long lCheck = 1;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent) != 0) {
				lCheck = 0;
				break;
			}
		}
		if (lCheck == 0) {
			break;
		}
	}
	pCmdUI->SetCheck (lCheck);
	m_theCriticalSection.Unlock ();
	::Sleep (0);

}

// 『編集(E)』-『現在位置より前のイベントを追加選択』
void CSekaijuDoc::OnEditSelectBefore () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	// 現在位置より前のイベントを選択状態にする
	BeginWaitCursor ();
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDIEvent* pCloneEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	m_theCriticalSection.Lock ();
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_EDIT_SELECT_BEFORE));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (pMIDIEvent->m_pPrevCombinedEvent == NULL) {
				long lTime = MIDIEvent_GetTime (pMIDIEvent);
				if (lTime < m_lNewTime) {
					pCloneEvent = this->SelectEvent (pMIDIEvent, 1, pCurHistoryUnit);
					if (pCloneEvent) {
						pMIDIEvent = pCloneEvent;
					}
				}
			}
		}
	}
	this->SetModifiedFlag (TRUE);
	this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED | 
		SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED);
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);
}

// 『編集(E)』-『現在位置より前のイベントを追加選択』
void CSekaijuDoc::OnUpdateEditSelectBeforeUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
}

// 『編集(E)』-『現在位置より前のイベントを選択解除』
void CSekaijuDoc::OnEditDeselectBefore () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}
	
	// 現在位置より前のイベントを選択解除にする
	BeginWaitCursor ();
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDIEvent* pCloneEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	m_theCriticalSection.Lock ();
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_EDIT_DESELECT_BEFORE));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (pMIDIEvent->m_pPrevCombinedEvent == NULL) {
				long lTime = MIDIEvent_GetTime (pMIDIEvent);
				if (lTime < m_lNewTime) {
					pCloneEvent = this->SelectEvent (pMIDIEvent, 0, pCurHistoryUnit);
					if (pCloneEvent) {
						pMIDIEvent = pCloneEvent;
					}
				}
			}
		}
	}
	this->SetModifiedFlag (TRUE);
	this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED | 
		SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED);
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);}

// 『編集(E)』-『現在位置より前のイベントを選択解除』
void CSekaijuDoc::OnUpdateEditDeselectBeforeUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
}

// 『編集(E)』-『現在位置より後のイベントを追加選択』
void CSekaijuDoc::OnEditSelectAfter () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	// 現在位置より後のイベントを選択状態にする
	BeginWaitCursor ();
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDIEvent* pCloneEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	m_theCriticalSection.Lock ();
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_EDIT_SELECT_AFTER));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (pMIDIEvent->m_pPrevCombinedEvent == NULL) {
				long lTime = MIDIEvent_GetTime (pMIDIEvent);
				if (lTime >= m_lNewTime) {
					pCloneEvent = this->SelectEvent (pMIDIEvent, 1, pCurHistoryUnit);
					if (pCloneEvent) {
						pMIDIEvent = pCloneEvent;
					}
				}
			}
		}
	}
	this->SetModifiedFlag (TRUE);
	this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED | 
		SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED);
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);
}

// 『編集(E)』-『現在位置より後のイベントを追加選択』
void CSekaijuDoc::OnUpdateEditSelectAfterUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
}

// 『編集(E)』-『現在位置より後のイベントを選択解除』
void CSekaijuDoc::OnEditDeselectAfter () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}
	
	// 現在位置より後のイベントを選択解除にする
	BeginWaitCursor ();
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDIEvent* pCloneEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	m_theCriticalSection.Lock ();
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_EDIT_DESELECT_AFTER));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (pMIDIEvent->m_pPrevCombinedEvent == NULL) {
				long lTime = MIDIEvent_GetTime (pMIDIEvent);
				if (lTime >= m_lNewTime) {
					pCloneEvent = this->SelectEvent (pMIDIEvent, 0, pCurHistoryUnit);
					if (pCloneEvent) {
						pMIDIEvent = pCloneEvent;
					}
				}
			}
		}
	}
	this->SetModifiedFlag (TRUE);
	this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED | 
		SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED);
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);
}

// 『編集(E)』-『現在位置より後のイベントを選択解除』
void CSekaijuDoc::OnUpdateEditDeselectAfterUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
}

// 『編集(E)』-『トラックの変更...』
void CSekaijuDoc::OnEditTrack () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}	
	if (m_bEditLocked) {
		return;
	}

	long lTimeMode, lTimeResolution;
	MIDIData_GetTimeBase (m_pMIDIData, &lTimeMode, &lTimeResolution);

	CEditTrackDlg theDlg;
	theDlg.m_nAmount = pSekaijuApp->m_theEditTrackDlgStatus.m_nAmount;
	theDlg.m_nUnit = CLIP (0, pSekaijuApp->m_theEditTrackDlgStatus.m_nUnit, 1);
	if (theDlg.DoModal () == IDOK) {
		BeginWaitCursor ();
		m_theCriticalSection.Lock ();
		long i, j;
		long lErrorCount = 0;
		long lFormat = MIDIData_GetFormat (m_pMIDIData);
		long lTrackCount = MIDIData_CountTrack (m_pMIDIData);
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		MIDIEvent* pCloneEvent = NULL;
		BOOL bTrackZeroOrigin = pSekaijuApp->m_theGeneralOption.m_bTrackZeroOrigin;
		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_EDIT_MODIFY_TRACKINDEX));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		
		// 同じイベントを2回移動しないように、選択されているイベントを一時配列に登録する。
		CPtrArray theTempSelectedEventArray;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				// 選択されているイベントのみ
				if (this->IsEventSelected (pMIDIEvent)) {
					// 非結合イベントのみ、結合イベントの場合は前非結合のみ、最後のEOTは除外
					if (MIDIEvent_IsCombined (pMIDIEvent) && pMIDIEvent->m_pPrevCombinedEvent == NULL ||
						!MIDIEvent_IsCombined (pMIDIEvent) &&
						!(MIDIEvent_IsEndofTrack (pMIDIEvent) &&
						MIDITrack_GetLastEvent (pMIDITrack) == pMIDIEvent)) {
						theTempSelectedEventArray.Add (pMIDIEvent);
					}
				}
			}
		}

		// すべての移動可能な選択されているイベントについて
		long lTempSelectedEventCount = theTempSelectedEventArray.GetSize ();
		for (j = 0; j < lTempSelectedEventCount; j++) {
			pMIDIEvent = (MIDIEvent*)(theTempSelectedEventArray.GetAt (j));
			long lKind = MIDIEvent_GetKind (pMIDIEvent);
			pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
			ASSERT (pMIDITrack);

			// 移動先トラックを計算
			long lTargetTrackIndex = 0;
			if (theDlg.m_nUnit == 0) {
				lTargetTrackIndex = (theDlg.m_nAmount - (bTrackZeroOrigin ? 0 : 1));
			}
			else {
				long lTrackIndex = GetTrackIndex (pMIDITrack);
				lTargetTrackIndex = lTrackIndex + theDlg.m_nAmount;
			}
			lTargetTrackIndex = CLIP (0, lTargetTrackIndex, (MAXMIDITRACKNUM - 1));
			MIDITrack* pTargetTrack = this->GetTrack (lTargetTrackIndex);

			// 移動可能なイベントの場合のみ
			if (lFormat == 1 && !MIDIEvent_IsMIDIEvent (pMIDIEvent) && lTargetTrackIndex == 0 ||
				lFormat == 1 && (lKind & 0xF0) != 0x50 && lTargetTrackIndex != 0 ||
				lFormat == 2) {
	
				// トラックがない場合、トラックの追加及びEOTの追加
				MIDITrack* pNewTrack = NULL;
				MIDIEvent* pNewEvent = NULL;
				long lTrackCount = MIDIData_CountTrack (m_pMIDIData);
				for (i = lTrackCount; i <= lTargetTrackIndex; i++) { 
					VERIFY (pNewTrack = MIDITrack_Create ());
					VERIFY (MIDITrack_SetForeColor (pNewTrack, pSekaijuApp->m_lDefTrackColor[i % 16]));
					VERIFY (MIDITrack_SetInputOn (pNewTrack, TRUE)); 
					VERIFY (MIDITrack_SetInputChannel (pNewTrack, (i + 15) % 16)); 
					VERIFY (MIDITrack_SetOutputOn (pNewTrack, TRUE)); 
					VERIFY (MIDITrack_SetOutputChannel (pNewTrack, (i + 15) % 16)); 
					VERIFY (MIDIData_AddTrack (m_pMIDIData, pNewTrack));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pNewTrack));
					VERIFY (pNewEvent = MIDIEvent_CreateEndofTrack (lTimeResolution * 4));
					VERIFY (MIDITrack_InsertEvent (pNewTrack, pNewEvent));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
				}
				if (pNewEvent) {
					VERIFY (this->SelectEvent (pNewEvent, TRUE, pCurHistoryUnit));
				}

				// 現在トラックから除去し移動先トラックに挿入
				VERIFY (pTargetTrack = this->GetTrack (lTargetTrackIndex));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
				VERIFY (pCloneEvent = this->ReplaceMIDIEvent (pMIDIEvent));
				VERIFY (MIDITrack_RemoveEvent (pMIDITrack, pCloneEvent));
				VERIFY (MIDITrack_InsertEvent (pTargetTrack, pCloneEvent));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
				// 注意: EndofTrackイベントはRemoveすることはできるが,二重にInsertすることはできない。
				// TODO: EndofTrackイベントの親ﾄﾗｯｸがなくなってしまうバグがまだあり。
			}
			else {
				lErrorCount++;
			}
		}
		
		theTempSelectedEventArray.RemoveAll ();
		this->SetModifiedFlag (TRUE);
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
		if (lErrorCount > 0) {
			CString strMsg;
			CString strFormat;
			VERIFY (strFormat.LoadString (IDS_D_MIDIEVENTS_TRACKINDEX_ISNT_CHANGED));
			strMsg.Format (strFormat, lErrorCount);
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		}
		pSekaijuApp->m_theEditTrackDlgStatus.m_nAmount = theDlg.m_nAmount;
		pSekaijuApp->m_theEditTrackDlgStatus.m_nUnit = theDlg.m_nUnit;
	}

}

// 『編集(E)』-『トラックの変更...』
void CSekaijuDoc::OnUpdateEditTrackUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	// すべてのイベントが非選択であればこの項目をディスエーブルする
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	long lFormat = MIDIData_GetFormat (m_pMIDIData);
	if (lFormat == 0) {
		pCmdUI->Enable (FALSE);
	}
	else {
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (this->IsEventSelected (pMIDIEvent)) {
					lEnable = 1;
					break;
				}
			}
			if (lEnable == 1) {
				break;
			}
		}
		pCmdUI->Enable (lEnable);
	}
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}



// 『編集(E)』-『タイムの変更...』
void CSekaijuDoc::OnEditTime () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	long lTimeMode, lTimeResolution;
	MIDIData_GetTimeBase (m_pMIDIData, &lTimeMode, &lTimeResolution);

	// TPQNベース
	if (lTimeMode == 0) {
		CEditTimeDlg theDlg;
		theDlg.m_nAmount = pSekaijuApp->m_theEditTimeDlgStatus.m_nAmount;
		theDlg.m_nUnit = CLIP (0, pSekaijuApp->m_theEditTimeDlgStatus.m_nUnit, 3);
		if (theDlg.DoModal () == IDOK) {
			BeginWaitCursor ();
			m_theCriticalSection.Lock ();
			MIDITrack* pMIDITrack = NULL;
			MIDIEvent* pMIDIEvent = NULL;
			MIDIEvent* pCloneEvent = NULL;
			CHistoryUnit* pCurHistoryUnit = NULL;
			CString strHistoryName;
			VERIFY (strHistoryName.LoadString (IDS_EDIT_MODIFY_TIME));
			VERIFY (this->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
			
			// 選択されているイベントのうち、最初のイベントの時刻を求める
			long lFirstTime = 0x7FFFFFFF;
			forEachTrack (m_pMIDIData, pMIDITrack) {
				forEachEvent (pMIDITrack, pMIDIEvent) {
					if (this->IsEventSelected (pMIDIEvent)) {
						long lTime = MIDIEvent_GetTime (pMIDIEvent);
						if (lTime < lFirstTime) {
							lFirstTime = lTime;
							break;
						}
					}
				}
			}
			long lnn, ldd, lbb, lcc;
			MIDIData_FindTimeSignature (m_pMIDIData, lFirstTime, &lnn, &ldd, &lbb, &lcc);
			// 同じイベントを2回移動しないように、選択されているイベントを一時配列に登録する。
			CPtrArray theTempSelectedEventArray;
			forEachTrack (m_pMIDIData, pMIDITrack) {
				pMIDITrack->m_lUser1 = 0;
				forEachEvent (pMIDITrack, pMIDIEvent) {
					// 選択されているイベントのみ
					if (this->IsEventSelected (pMIDIEvent)) {
						// 非結合イベントのみ、結合イベントの場合は前非結合のみ
						if (MIDIEvent_IsCombined (pMIDIEvent) && pMIDIEvent->m_pPrevCombinedEvent == NULL ||
							!MIDIEvent_IsCombined (pMIDIEvent)) {
							theTempSelectedEventArray.Add (pMIDIEvent);
							pMIDITrack->m_lUser1++;
						}
					}
				}
			}
			// EOTの履歴保持(EOTが選択されていない場合のみ)(20081102修正)
			forEachTrack (m_pMIDIData, pMIDITrack) {
				MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
				if (pLastEvent && pMIDITrack->m_lUser1 > 0) {
					if (MIDIEvent_IsEndofTrack (pLastEvent) && !this->IsEventSelected (pLastEvent)) {
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pLastEvent));
					}
				}
			}
			// 時刻の変更
			// 一時配列のイベント1個につき1回の移動をかける。
			long lTempSelectedEventArrayCount = theTempSelectedEventArray.GetSize ();
			if (theDlg.m_nAmount < 0) {
				// (EOTを最後に移動させるため、昇順に)(20081102修正)
				for (long j = 0; j < lTempSelectedEventArrayCount; j++) {
					pMIDIEvent = (MIDIEvent*)(theTempSelectedEventArray.GetAt (j));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
					VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
					long lTime = MIDIEvent_GetTime (pCloneEvent);
					if (theDlg.m_nUnit == 0) {
						lTime += theDlg.m_nAmount;
					}
					else if (theDlg.m_nUnit == 1) {
						lTime += theDlg.m_nAmount * lTimeResolution * 4 / (1 << ldd); // (20081110計算式修正)
					}
					else if (theDlg.m_nUnit == 2) {
						lTime += theDlg.m_nAmount * lTimeResolution * 4 * lnn / (1 << ldd);
					}
					else if (theDlg.m_nUnit == 3) {
						lTime = lFirstTime + (lTime - lFirstTime) * theDlg.m_nAmount / 100;
						// ノートオフに結合したノートオンイベントの場合は、音長さも変更する
						if (MIDIEvent_IsNoteOn (pCloneEvent) && MIDIEvent_IsCombined (pCloneEvent)) {
							long lDuration = MIDIEvent_GetDuration (pCloneEvent);
							lDuration = lDuration * theDlg.m_nAmount / 100;
							lDuration = CLIP (1, lDuration, 65535);
							MIDIEvent_SetDuration (pCloneEvent, lDuration);
						}
					}
					lTime = CLIP (0, lTime, 0x7FFFFFFF);
					VERIFY (MIDIEvent_SetTime (pCloneEvent, lTime));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
					pMIDIEvent = pCloneEvent;
				}
			}
			else {
				// (EOTを最初に移動させるため、逆順に)(20081102修正)
				for (long j = lTempSelectedEventArrayCount - 1; j >= 0; j--) {
					pMIDIEvent = (MIDIEvent*)(theTempSelectedEventArray.GetAt (j));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
					VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
					long lTime = MIDIEvent_GetTime (pCloneEvent);
					if (theDlg.m_nUnit == 0) {
						lTime += theDlg.m_nAmount;
					}
					else if (theDlg.m_nUnit == 1) {
						lTime += theDlg.m_nAmount * lTimeResolution * 4 / (1 << ldd); // (20081110計算式修正)
					}
					else if (theDlg.m_nUnit == 2) {
						lTime += theDlg.m_nAmount * lTimeResolution * 4 * lnn / (1 << ldd);
					}
					else if (theDlg.m_nUnit == 3) {
						lTime = lFirstTime + (lTime - lFirstTime) * theDlg.m_nAmount / 100;
						// ノートオフに結合したノートオンイベントの場合は、音長さも変更する
						if (MIDIEvent_IsNoteOn (pCloneEvent) && MIDIEvent_IsCombined (pCloneEvent)) {
							long lDuration = MIDIEvent_GetDuration (pCloneEvent);
							lDuration = lDuration * theDlg.m_nAmount / 100;
							lDuration = CLIP (1, lDuration, 65535);
							MIDIEvent_SetDuration (pCloneEvent, lDuration);
						}
					}
					lTime = CLIP (0, lTime, 0x7FFFFFFF);
					VERIFY (MIDIEvent_SetTime (pCloneEvent, lTime));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
					pMIDIEvent = pCloneEvent;
				}
			}
			theTempSelectedEventArray.RemoveAll ();
			// EOTの履歴保持(EOTが選択されていない場合のみ)(20081102修正)
			forEachTrack (m_pMIDIData, pMIDITrack) {
				MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
				if (pLastEvent && pMIDITrack->m_lUser1 > 0) {
					if (MIDIEvent_IsEndofTrack (pLastEvent) && !this->IsEventSelected (pLastEvent)) {
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pLastEvent));
					}
				}
				pMIDITrack->m_lUser1 = 0;
			}
			this->SetModifiedFlag (TRUE);
			this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			::Sleep (0);
			pSekaijuApp->m_theEditTimeDlgStatus.m_nAmount = theDlg.m_nAmount;
			pSekaijuApp->m_theEditTimeDlgStatus.m_nUnit = theDlg.m_nUnit;
		}
	}

	// SMPTEベース
	else {
		CEditTimeSmpDlg theDlg;
		theDlg.m_nAmount = pSekaijuApp->m_theEditTimeSmpDlgStatus.m_nAmount;
		theDlg.m_nUnit = CLIP (0, pSekaijuApp->m_theEditTimeSmpDlgStatus.m_nUnit, 2);
		if (theDlg.DoModal () == IDOK) {
			BeginWaitCursor ();
			m_theCriticalSection.Lock ();
			MIDITrack* pMIDITrack = NULL;
			MIDIEvent* pMIDIEvent = NULL;
			MIDIEvent* pCloneEvent = NULL;
			CHistoryUnit* pCurHistoryUnit = NULL;
			CString strHistoryName;
			VERIFY (strHistoryName.LoadString (IDS_EDIT_MODIFY_TIME));
			VERIFY (this->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
			
			// 選択されているイベントのうち、最初のイベントの時刻を求める
			long lFirstTime = 0x7FFFFFFF;
			forEachTrack (m_pMIDIData, pMIDITrack) {
				forEachEvent (pMIDITrack, pMIDIEvent) {
					if (this->IsEventSelected (pMIDIEvent)) {
						long lTime = MIDIEvent_GetTime (pMIDIEvent);
						if (lTime < lFirstTime) {
							lFirstTime = lTime;
							break;
						}
					}
				}
			}
			long lTimeMode, lTimeResolution;
			MIDIData_GetTimeBase (m_pMIDIData, &lTimeMode, &lTimeResolution);
			long lnn, ldd, lbb, lcc;
			MIDIData_FindTimeSignature (m_pMIDIData, lFirstTime, &lnn, &ldd, &lbb, &lcc);
			// 同じイベントを2回移動しないように、選択されているイベントを一時配列に登録する。
			CPtrArray theTempSelectedEventArray;
			forEachTrack (m_pMIDIData, pMIDITrack) {
				pMIDITrack->m_lUser1 = 0;
				forEachEvent (pMIDITrack, pMIDIEvent) {
					// 選択されているイベントのみ
					if (this->IsEventSelected (pMIDIEvent)) {
						// 非結合イベントのみ、結合イベントの場合は前非結合のみ
						if (MIDIEvent_IsCombined (pMIDIEvent) && pMIDIEvent->m_pPrevCombinedEvent == NULL ||
							!MIDIEvent_IsCombined (pMIDIEvent)) {
							theTempSelectedEventArray.Add (pMIDIEvent);
							pMIDITrack->m_lUser1++;
						}
					}
				}
			}
			// EOTの履歴保持(EOTが選択されていない場合のみ)(20081102修正)
			forEachTrack (m_pMIDIData, pMIDITrack) {
				MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
				if (pLastEvent && pMIDITrack->m_lUser1 > 0) {
					if (MIDIEvent_IsEndofTrack (pLastEvent) && !this->IsEventSelected (pLastEvent)) {
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pLastEvent));
					}
				}
			}
			// 一時配列のイベント1個につき1回の移動をかける。
			long lTempSelectedEventArrayCount = theTempSelectedEventArray.GetSize ();
			if (theDlg.m_nAmount < 0) {
				// (EOTを最後に移動させるため、昇順に)(20081102修正)
				for (long j = 0; j < lTempSelectedEventArrayCount; j++) {
					pMIDIEvent = (MIDIEvent*)(theTempSelectedEventArray.GetAt (j));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
					VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
					long lTime = MIDIEvent_GetTime (pCloneEvent);
					if (theDlg.m_nUnit == 0) {
						lTime += theDlg.m_nAmount;
					}
					else if (theDlg.m_nUnit == 1) {
						lTime += theDlg.m_nAmount * lTimeResolution;
					}
					else if (theDlg.m_nUnit == 2) {
						lTime = lFirstTime + (lTime - lFirstTime) * theDlg.m_nAmount / 100;
						// ノートオフに結合したノートオンイベントの場合は、音長さも変更する
						if (MIDIEvent_IsNoteOn (pCloneEvent) && MIDIEvent_IsCombined (pCloneEvent)) {
							long lDuration = MIDIEvent_GetDuration (pCloneEvent);
							lDuration = lDuration * theDlg.m_nAmount / 100;
							lDuration = CLIP (1, lDuration, 65535);
							MIDIEvent_SetDuration (pCloneEvent, lDuration);
						}
					}
					lTime = CLIP (0, lTime, 0x7FFFFFFF);
					VERIFY (MIDIEvent_SetTime (pCloneEvent, lTime));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
					pMIDIEvent = pCloneEvent;
				}
			}
			else {
				// (EOTを最初に移動させるため、逆順に)(20081102修正)
				for (long j = lTempSelectedEventArrayCount - 1; j >= 0; j--) {
					pMIDIEvent = (MIDIEvent*)(theTempSelectedEventArray.GetAt (j));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
					VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
					long lTime = MIDIEvent_GetTime (pCloneEvent);
					if (theDlg.m_nUnit == 0) {
						lTime += theDlg.m_nAmount;
					}
					else if (theDlg.m_nUnit == 1) {
						lTime += theDlg.m_nAmount * lTimeResolution;
					}
					else if (theDlg.m_nUnit == 2) {
						lTime = lFirstTime + (lTime - lFirstTime) * theDlg.m_nAmount / 100;
						// ノートオフに結合したノートオンイベントの場合は、音長さも変更する
						if (MIDIEvent_IsNoteOn (pCloneEvent) && MIDIEvent_IsCombined (pCloneEvent)) {
							long lDuration = MIDIEvent_GetDuration (pCloneEvent);
							lDuration = lDuration * theDlg.m_nAmount / 100;
							lDuration = CLIP (1, lDuration, 65535);
							MIDIEvent_SetDuration (pCloneEvent, lDuration);
						}
					}
					lTime = CLIP (0, lTime, 0x7FFFFFFF);
					VERIFY (MIDIEvent_SetTime (pCloneEvent, lTime));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
					pMIDIEvent = pCloneEvent;
				}
			}
			theTempSelectedEventArray.RemoveAll ();
			// EOTの履歴保持(EOTが選択されていない場合のみ)(20081102修正)
			forEachTrack (m_pMIDIData, pMIDITrack) {
				MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
				if (pLastEvent && pMIDITrack->m_lUser1 > 0) {
					if (MIDIEvent_IsEndofTrack (pLastEvent) && !this->IsEventSelected (pLastEvent)) {
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pLastEvent));
					}
				}
				pMIDITrack->m_lUser1 = 0;
			}
			this->SetModifiedFlag (TRUE);
			this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			::Sleep (0);
			pSekaijuApp->m_theEditTimeSmpDlgStatus.m_nAmount = theDlg.m_nAmount;
			pSekaijuApp->m_theEditTimeSmpDlgStatus.m_nUnit = theDlg.m_nUnit;
		}
	}
}

// 『編集(E)』-『タイムの変更...』
void CSekaijuDoc::OnUpdateEditTimeUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	// すべてのイベントが非選択であればこの項目をディスエーブルする
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable == 1) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// 『編集(E)』-『チャンネルの変更...』
void CSekaijuDoc::OnEditChannel () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	CEditChannelDlg theDlg;
	theDlg.m_nAmount = pSekaijuApp->m_theEditChannelDlgStatus.m_nAmount;
	theDlg.m_nUnit = CLIP (0, pSekaijuApp->m_theEditChannelDlgStatus.m_nUnit, 2);
	if (theDlg.DoModal () == IDOK) {
		BeginWaitCursor ();
		m_theCriticalSection.Lock ();
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		MIDIEvent* pCloneEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		long lTrackOutputChannel = 0;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_EDIT_MODIFY_CHANNEL));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		// チャンネルの変更
		forEachTrack (m_pMIDIData, pMIDITrack) {
			lTrackOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
			forEachEvent (pMIDITrack, pMIDIEvent) {
				// 選択されているイベントのみ
				if (this->IsEventSelected (pMIDIEvent)) {
					// MIDIチャンネルイベント
					// (ノートオフ・ノートオン・キーアフター・コントロールチェンジ・
					// プログラムチェンジ・チャンネルアフター・ピッチベンド)のみ
					if (MIDIEvent_IsMIDIEvent (pMIDIEvent) &&
						pMIDIEvent->m_pPrevCombinedEvent == NULL) {
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						long lChannel = MIDIEvent_GetChannel (pCloneEvent);
						if (theDlg.m_nUnit == 0) {
							if (0 <= lTrackOutputChannel && lTrackOutputChannel < 16) {
								lChannel = lTrackOutputChannel;
							}
						}
						else if (theDlg.m_nUnit == 1) {
							lChannel = theDlg.m_nAmount - 1;
						}
						else if (theDlg.m_nUnit == 2) {
							lChannel += theDlg.m_nAmount;
						}
						lChannel = CLIP (0, lChannel, 15);
						VERIFY (MIDIEvent_SetChannel (pCloneEvent, lChannel));
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
					}
				}
			}
		}
		this->SetModifiedFlag (TRUE);
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
		pSekaijuApp->m_theEditChannelDlgStatus.m_nAmount = theDlg.m_nAmount;
		pSekaijuApp->m_theEditChannelDlgStatus.m_nUnit = theDlg.m_nUnit;
	}
}

// 『編集(E)』-『チャンネルの変更...』
void CSekaijuDoc::OnUpdateEditChannelUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	// すべてのイベントが非選択であればこの項目をディスエーブルする
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable == 1) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// 『編集(E)』-『音程の変更...』
void CSekaijuDoc::OnEditKey () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}
	
	CEditKeyDlg theDlg;
	theDlg.m_nAmount = pSekaijuApp->m_theEditKeyDlgStatus.m_nAmount;
	theDlg.m_nUnit = CLIP (0, pSekaijuApp->m_theEditKeyDlgStatus.m_nUnit, 1);
	theDlg.m_nTargetNote = CLIP (0, pSekaijuApp->m_theEditKeyDlgStatus.m_nTargetNote, 1);
	theDlg.m_nTargetKeyAfter = CLIP (0, pSekaijuApp->m_theEditKeyDlgStatus.m_nTargetKeyAfter, 1);
	if (theDlg.DoModal () == IDOK) {
		BeginWaitCursor ();
		m_theCriticalSection.Lock ();
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		MIDIEvent* pCloneEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_EDIT_MODIFY_KEY));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		// 音程の変更
		forEachTrack (m_pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				// 選択されているイベントのみ
				if (this->IsEventSelected (pMIDIEvent)) {
					// ノートオン・ノートオフ(非結合)・キーアフタータッチのみ
					if ((MIDIEvent_IsNoteOn (pMIDIEvent) && pMIDIEvent->m_pPrevCombinedEvent == NULL ||
						MIDIEvent_IsNoteOff (pMIDIEvent) && pMIDIEvent->m_pPrevCombinedEvent == NULL) &&
						theDlg.m_nTargetNote == 1 ||
						MIDIEvent_IsKeyAftertouch (pMIDIEvent) &&
						theDlg.m_nTargetKeyAfter == 1) {
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						long lKey = MIDIEvent_GetKey (pCloneEvent);
						if (theDlg.m_nUnit == 0) {
							lKey += theDlg.m_nAmount;
						}
						else {
							lKey += theDlg.m_nAmount * 12;
						}
						lKey = CLIP (0, lKey, 127);
						VERIFY (MIDIEvent_SetKey (pCloneEvent, lKey));
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
					}
				}
			}
		}
		this->SetModifiedFlag (TRUE);
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
		pSekaijuApp->m_theEditKeyDlgStatus.m_nAmount = theDlg.m_nAmount;
		pSekaijuApp->m_theEditKeyDlgStatus.m_nUnit = theDlg.m_nUnit;
		pSekaijuApp->m_theEditKeyDlgStatus.m_nTargetNote = theDlg.m_nTargetNote;
		pSekaijuApp->m_theEditKeyDlgStatus.m_nTargetKeyAfter = theDlg.m_nTargetKeyAfter;
	}
}

// 『編集(E)』-『音程の変更...』
void CSekaijuDoc::OnUpdateEditKeyUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	// すべてのイベントが非選択であればこの項目をディスエーブルする
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable == 1) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);

}

// 『編集(E)』-『ベロシティの変更...』
void CSekaijuDoc::OnEditVelocity () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}	
	if (m_bEditLocked) {
		return;
	}
	
	CEditVelocityDlg theDlg;
	theDlg.m_nAmount = pSekaijuApp->m_theEditVelocityDlgStatus.m_nAmount;
	theDlg.m_nUnit = CLIP (0, pSekaijuApp->m_theEditVelocityDlgStatus.m_nUnit, 2);
	theDlg.m_nTargetNoteOn = CLIP (0, pSekaijuApp->m_theEditVelocityDlgStatus.m_nTargetNoteOn, 1);
	theDlg.m_nTargetNoteOff = CLIP (0, pSekaijuApp->m_theEditVelocityDlgStatus.m_nTargetNoteOff, 1);
	if (theDlg.DoModal () == IDOK) {
		BeginWaitCursor ();
		m_theCriticalSection.Lock ();
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		MIDIEvent* pCloneEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_EDIT_MODIFY_VELOCITY));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		// ベロシティの変更
		forEachTrack (m_pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				// 選択されているイベントのみ
				if (this->IsEventSelected (pMIDIEvent)) {
					// ノートオン(前非結合)・ノートオフ(前非結合)の場合
					if (MIDIEvent_IsNoteOn (pMIDIEvent) && 
						pMIDIEvent->m_pPrevCombinedEvent == NULL && theDlg.m_nTargetNoteOn ||
						MIDIEvent_IsNoteOff (pMIDIEvent) && 
						pMIDIEvent->m_pPrevCombinedEvent == NULL && theDlg.m_nTargetNoteOff) {
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						MIDIEvent* pTempEvent = pCloneEvent;
						while (pTempEvent) {
							if (MIDIEvent_IsNoteOn (pTempEvent) && theDlg.m_nTargetNoteOn ||
								MIDIEvent_IsNoteOff (pTempEvent) && theDlg.m_nTargetNoteOff) {
								long lVelocity = MIDIEvent_GetVelocity (pTempEvent);
								if (theDlg.m_nUnit == 0) {
									lVelocity = theDlg.m_nAmount;
								}
								else if (theDlg.m_nUnit == 1) {
									lVelocity += theDlg.m_nAmount;
								}
								else {
									lVelocity = lVelocity * theDlg.m_nAmount / 100;
								}
								lVelocity = CLIP (0, lVelocity, 127);
								MIDIEvent_SetVelocity (pTempEvent, lVelocity);
							}
							pTempEvent = pTempEvent->m_pNextCombinedEvent;
						}
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
					}
				}
			}
		}
		this->SetModifiedFlag (TRUE);
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
		pSekaijuApp->m_theEditVelocityDlgStatus.m_nAmount = theDlg.m_nAmount;
		pSekaijuApp->m_theEditVelocityDlgStatus.m_nUnit = theDlg.m_nUnit;
		pSekaijuApp->m_theEditVelocityDlgStatus.m_nTargetNoteOn = theDlg.m_nTargetNoteOn;
		pSekaijuApp->m_theEditVelocityDlgStatus.m_nTargetNoteOff = theDlg.m_nTargetNoteOff;
	}
}

// 『編集(E)』-『ベロシティの変更...』
void CSekaijuDoc::OnUpdateEditVelocityUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	// すべてのイベントが非選択であればこの項目をディスエーブルする
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable == 1) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// 『編集(E)』-『音長さの変更...』
void CSekaijuDoc::OnEditDuration () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}	
	if (m_bEditLocked) {
		return;
	}

	CEditDurationDlg theDlg;
	theDlg.m_nAmount = pSekaijuApp->m_theEditDurationDlgStatus.m_nAmount;
	theDlg.m_nUnit = CLIP (0, pSekaijuApp->m_theEditDurationDlgStatus.m_nUnit, 2);
	if (theDlg.DoModal () == IDOK) {
		BeginWaitCursor ();
		m_theCriticalSection.Lock ();
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		MIDIEvent* pCloneEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_EDIT_MODIFY_DURATION));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		// EOTの履歴保持
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
					VERIFY (pCloneEvent = ReplaceMIDIEvent (pLastEvent));
				}
			}
		}
		// 音の長さ変更
		forEachTrack (m_pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				// 選択されているイベントのみ
				if (this->IsEventSelected (pMIDIEvent)) {
					// ノートオフに結合されたノートオンイベントのみ
					if (MIDIEvent_IsNoteOn (pMIDIEvent) && MIDIEvent_IsCombined (pMIDIEvent)) {
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						long lDuration = MIDIEvent_GetDuration (pCloneEvent);
						if (theDlg.m_nUnit == 0) {
							lDuration = theDlg.m_nAmount;
						}
						else if (theDlg.m_nUnit == 1) {
							lDuration += theDlg.m_nAmount;
						}
						else {
							lDuration = lDuration * theDlg.m_nAmount / 100;
						}
						lDuration = CLIP (1, lDuration, 65535);
						VERIFY (MIDIEvent_SetDuration (pCloneEvent, lDuration));
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
					}
				}
			}
		}
		// EOTの履歴保持
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pLastEvent));
				}
			}
		}
		this->SetModifiedFlag (TRUE);
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
		pSekaijuApp->m_theEditDurationDlgStatus.m_nAmount = theDlg.m_nAmount;
		pSekaijuApp->m_theEditDurationDlgStatus.m_nUnit = theDlg.m_nUnit;
	}
}

// 『編集(E)』-『音長さの変更...』
void CSekaijuDoc::OnUpdateEditDurationUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	// すべてのイベントが非選択であればこの項目をディスエーブルする
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable == 1) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// 『編集(E)』-『値の変更...』
void CSekaijuDoc::OnEditValue () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}	
	if (m_bEditLocked) {
		return;
	}
	
	CEditValueDlg theDlg;
	theDlg.m_nAmount = pSekaijuApp->m_theEditValueDlgStatus.m_nAmount;
	theDlg.m_nUnit = CLIP (0, pSekaijuApp->m_theEditValueDlgStatus.m_nUnit, 2);
	theDlg.m_nTargetKeyAfter = CLIP (0, pSekaijuApp->m_theEditValueDlgStatus.m_nTargetKeyAfter, 1);
	theDlg.m_nTargetControlChange = CLIP (0, pSekaijuApp->m_theEditValueDlgStatus.m_nTargetControlChange, 1);
	theDlg.m_nTargetChannelAfter = CLIP (0, pSekaijuApp->m_theEditValueDlgStatus.m_nTargetChannelAfter, 1);
	theDlg.m_nTargetPitchBend = CLIP (0, pSekaijuApp->m_theEditValueDlgStatus.m_nTargetPitchBend, 1);
	if (theDlg.DoModal () == IDOK) {
		BeginWaitCursor ();
		m_theCriticalSection.Lock ();
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		MIDIEvent* pCloneEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_EDIT_MODIFY_VALUE));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		// 値の変更
		forEachTrack (m_pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				// 選択されているイベントのみ
				if (this->IsEventSelected (pMIDIEvent)) {
					// キーアフタータッチ・コントロールチェンジ・チャンネルアフタータッチ
					if (MIDIEvent_IsKeyAftertouch (pMIDIEvent) && theDlg.m_nTargetKeyAfter ||
						MIDIEvent_IsControlChange (pMIDIEvent) && theDlg.m_nTargetControlChange ||
						MIDIEvent_IsChannelAftertouch (pMIDIEvent) && theDlg.m_nTargetChannelAfter) {	
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						long lValue = MIDIEvent_GetValue (pMIDIEvent);
						if (theDlg.m_nUnit == 0) {
							lValue = theDlg.m_nAmount;
						}
						if (theDlg.m_nUnit == 1) {
							lValue += theDlg.m_nAmount;
						}
						else {
							lValue = lValue * theDlg.m_nAmount / 100;
						}
						lValue = CLIP (0, lValue, 127);
						VERIFY (MIDIEvent_SetValue (pCloneEvent, lValue));
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
					}
					// ピッチベンド
					else if (MIDIEvent_IsPitchBend (pMIDIEvent) && theDlg.m_nTargetPitchBend) {
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						long lValue = MIDIEvent_GetValue (pMIDIEvent);
						if (theDlg.m_nUnit == 0) {
							lValue = theDlg.m_nAmount;
						}
						if (theDlg.m_nUnit == 1) {
							lValue += theDlg.m_nAmount;
						}
						else {
							lValue = 8192 + (lValue - 8192) * theDlg.m_nAmount / 100;
						}
						lValue = CLIP (0, lValue, 16383);
						VERIFY (MIDIEvent_SetValue (pCloneEvent, lValue));
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
					}
				}
			}
		}
		this->SetModifiedFlag (TRUE);
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
		pSekaijuApp->m_theEditValueDlgStatus.m_nAmount = theDlg.m_nAmount;
		pSekaijuApp->m_theEditValueDlgStatus.m_nUnit = theDlg.m_nUnit;
		pSekaijuApp->m_theEditValueDlgStatus.m_nTargetKeyAfter = theDlg.m_nTargetKeyAfter;
		pSekaijuApp->m_theEditValueDlgStatus.m_nTargetControlChange = theDlg.m_nTargetControlChange;
		pSekaijuApp->m_theEditValueDlgStatus.m_nTargetChannelAfter = theDlg.m_nTargetChannelAfter;
		pSekaijuApp->m_theEditValueDlgStatus.m_nTargetPitchBend = theDlg.m_nTargetPitchBend;
	}
}

// 『編集(E)』-『値の変更...』
void CSekaijuDoc::OnUpdateEditValueUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	// すべてのイベントが非選択であればこの項目をディスエーブルする
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable == 1) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// 『編集(E)』-『クォンタイズ...』
void CSekaijuDoc::OnEditQuantize () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}
	

	CEditQuantizeDlg theDlg;
	theDlg.m_lTimeMode = MIDIData_GetTimeMode (m_pMIDIData);
	theDlg.m_lTimeResolution = MIDIData_GetTimeResolution (m_pMIDIData);
	theDlg.m_nSnapTimeIndex = CLIP (0, pSekaijuApp->m_theEditQuantizeDlgStatus.m_nSnapTimeIndex, 6);
	theDlg.m_nStrength = CLIP (0, pSekaijuApp->m_theEditQuantizeDlgStatus.m_nStrength, 100);
	theDlg.m_nTargetNoteOn = CLIP (0, pSekaijuApp->m_theEditQuantizeDlgStatus.m_nTargetNoteOn, 1);
	theDlg.m_nTargetNoteOff = CLIP (0, pSekaijuApp->m_theEditQuantizeDlgStatus.m_nTargetNoteOff, 1);
	if (theDlg.DoModal () == IDOK) {
		BeginWaitCursor ();
		long lDivision[7] = {1, 2, 3, 4, 6, 8, 12};
		long lSnapTimeIndex = CLIP (0, theDlg.m_nSnapTimeIndex, 6);
		long lSnapTime = __max (1, MIDIData_GetTimeResolution (m_pMIDIData) / lDivision[lSnapTimeIndex]);
		m_theCriticalSection.Lock ();
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		MIDIEvent* pCloneEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_EDIT_QUANTIZE));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		
		// EOTの履歴保持
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
					VERIFY (pCloneEvent = ReplaceMIDIEvent (pLastEvent));
				}
			}
		}

		// 同じイベントを2回移動しないように、選択されているイベントを一時配列に登録する。
		CPtrArray theTempSelectedEventArray;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				// 選択されているイベントのみ
				if (this->IsEventSelected (pMIDIEvent)) {
					// ノートオフに結合されたノートオンイベントのみ
					if (MIDIEvent_IsNoteOn (pMIDIEvent) && MIDIEvent_IsCombined (pMIDIEvent)) {
						theTempSelectedEventArray.Add (pMIDIEvent);
					}
				}
			}
		}

		// 一時配列のイベント1個につき1回の移動をかける。
		long lTempSelectedEventArrayCount = theTempSelectedEventArray.GetSize ();
		for (long j = 0; j < lTempSelectedEventArrayCount; j++) {
			pMIDIEvent = (MIDIEvent*)(theTempSelectedEventArray.GetAt (j));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));

			// 音符の開始位置移動(音長さは保持される)
			if (theDlg.m_nTargetNoteOn) {
				long lTime = MIDIEvent_GetTime (pCloneEvent);
				long lNewTime = (lTime + lSnapTime / 2) / lSnapTime * lSnapTime;
				long lDeltaTime = lNewTime - lTime;
				long lStDeltaTime = lDeltaTime * theDlg.m_nStrength / 100;
				long lStNewTime = CLIP (0, lTime + lStDeltaTime, 0x7FFFFFFF);
				VERIFY (MIDIEvent_SetTime (pCloneEvent, lStNewTime));
			}

			// 音符の終了位置移動(音長さを変更することによる)
			if (theDlg.m_nTargetNoteOff) {
				long lBeginTime = MIDIEvent_GetTime (pCloneEvent);
				long lDuration = MIDIEvent_GetDuration (pCloneEvent);
				long lEndTime = lBeginTime + lDuration;
				long lNewEndTime = (lEndTime + lSnapTime / 2) / lSnapTime * lSnapTime;
				long lDeltaTime = lNewEndTime - lEndTime;
				long lStDeltaTime = lDeltaTime * theDlg.m_nStrength / 100;
				long lStNewEndTime = CLIP (0, lEndTime + lStDeltaTime, 0x7FFFFFFF);
				long lStNewDuration = lStNewEndTime - lBeginTime;
				while (lStNewDuration <= 1) {
					lStNewDuration += lSnapTime;
				}
				VERIFY (MIDIEvent_SetDuration (pCloneEvent, lStNewDuration));
			}
	
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pMIDIEvent = pCloneEvent;
		}
		theTempSelectedEventArray.RemoveAll ();

		// EOTの履歴保持
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pLastEvent));
				}
			}
		}

		this->SetModifiedFlag (TRUE);
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
		pSekaijuApp->m_theEditQuantizeDlgStatus.m_nSnapTimeIndex = theDlg.m_nSnapTimeIndex;
		pSekaijuApp->m_theEditQuantizeDlgStatus.m_nStrength = theDlg.m_nStrength;
		pSekaijuApp->m_theEditQuantizeDlgStatus.m_nTargetNoteOn = theDlg.m_nTargetNoteOn;
		pSekaijuApp->m_theEditQuantizeDlgStatus.m_nTargetNoteOff = theDlg.m_nTargetNoteOff;
	}
}



// 『編集(E)』-『クォンタイズ...』
void CSekaijuDoc::OnUpdateEditQuantizeUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	// すべてのイベントが非選択であればこの項目をディスエーブルする
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable == 1) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// 『編集(E)』-『音符の細分化とトリル化...』
void CSekaijuDoc::OnEditBreakupAndTrill () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	CEditBreakupAndTrillDlg theDlg;
	theDlg.m_lTimeMode = MIDIData_GetTimeMode (m_pMIDIData);
	theDlg.m_lTimeResolution = MIDIData_GetTimeResolution (m_pMIDIData);
	theDlg.m_nDurationIndex = pSekaijuApp->m_theEditBreakupAndTrillDlgStatus.m_nDurationIndex;
	theDlg.m_nEnableTrill = pSekaijuApp->m_theEditBreakupAndTrillDlgStatus.m_nEnableTrill;
	theDlg.m_nKeyShift = pSekaijuApp->m_theEditBreakupAndTrillDlgStatus.m_nKeyShift;

	
	if (theDlg.DoModal () == IDOK) {
		pSekaijuApp->m_theEditBreakupAndTrillDlgStatus.m_nDurationIndex = theDlg.m_nDurationIndex;
		pSekaijuApp->m_theEditBreakupAndTrillDlgStatus.m_nEnableTrill = theDlg.m_nEnableTrill;
		pSekaijuApp->m_theEditBreakupAndTrillDlgStatus.m_nKeyShift = theDlg.m_nKeyShift;

		BeginWaitCursor ();
		m_theCriticalSection.Lock ();

		// 処理後の1音の長さ[tick]を計算
		long lDurationDiv[7] = {1, 2, 3, 4, 6, 8, 12};
		long lDuration = theDlg.m_lTimeResolution / lDurationDiv[theDlg.m_nDurationIndex];
		lDuration = CLIP (1, lDuration, 65535);
		long lKeyShift = CLIP (-127, theDlg.m_nKeyShift, 127);
		
		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_EDIT_BREAKUP_AND_TRILL));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		MIDIEvent* pCloneEvent = NULL;
		long i = 0;
		long j = 0;
		long lDeletedNoteEventCount = 0;
		long lInsertedNoteEventCount = 0;
		
		// EOTの履歴保持
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
					VERIFY (pCloneEvent = ReplaceMIDIEvent (pLastEvent));
				}
			}
		}
		
		// 配列に登録されたイベントを削除
		forEachTrack (m_pMIDIData, pMIDITrack) {
			// 選択されているノートオンイベントを配列に登録
			CPtrArray theEventArray;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (this->IsEventSelected (pMIDIEvent)) {
					if (MIDIEvent_IsNote (pMIDIEvent) && MIDIEvent_IsNoteOn (pMIDIEvent)) {
						theEventArray.Add (pMIDIEvent);
					}
				}
			}
			// それぞれの選択されているノートイベントについて
			for (i = 0; i < theEventArray.GetSize (); i++) {
				VERIFY (pMIDIEvent = (MIDIEvent*)theEventArray.GetAt (i));
				ASSERT (MIDIEvent_IsNote (pMIDIEvent) && MIDIEvent_IsNoteOn (pMIDIEvent));
				long lOldTime = MIDIEvent_GetTime (pMIDIEvent);
				long lOldChannel = MIDIEvent_GetChannel (pMIDIEvent);
				long lOldDuration = MIDIEvent_GetDuration (pMIDIEvent);
				long lOldKey = MIDIEvent_GetKey (pMIDIEvent);
				long lOldVelocity = MIDIEvent_GetVelocity (pMIDIEvent);
				// 既存のノートイベントを削除
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
				VERIFY (MIDITrack_RemoveEvent (pMIDITrack, pMIDIEvent));
				lDeletedNoteEventCount++;
				// 新しいノートイベントを挿入
				for (j = 0; j < 65536; j++) {
					long lNewTime = lOldTime + lDuration * j;
					if (lNewTime >= lOldTime + lOldDuration) {
						break;
					}
					long lNewDuration = lDuration;
					if (lNewTime + lNewDuration >= lOldTime + lOldDuration) {
						lNewDuration = lOldTime + lOldDuration - lNewTime;
						ASSERT (lNewDuration > 0);
					}
					long lNewKey = lOldKey;
					if (theDlg.m_nEnableTrill) {
						lNewKey = (j % 2 == 0 ? lOldKey : lOldKey + lKeyShift);
					}
					lNewKey = CLIP (1, lNewKey, 127);
					long lNewVelocity = lOldVelocity;
					long lNewChannel = lOldChannel;
					// 新しいノートイベントを作成
					MIDIEvent* pNewEvent = MIDIEvent_CreateNote 
						(lNewTime, lNewChannel, lNewKey, lNewVelocity, lNewDuration);
					ASSERT (pNewEvent);
					// 結合イベントを先頭から順に選択状態にする。
					MIDIEvent* pTempEvent = pNewEvent;
					while (pTempEvent) {
						pTempEvent->m_lUserFlag |= MIDIEVENT_SELECTED;
						pTempEvent = pTempEvent->m_pNextCombinedEvent;
					}
					// トラックに挿入、履歴記録
					VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
					lInsertedNoteEventCount++;
				}
			}
			theEventArray.RemoveAll ();
		}

		// EOTの履歴保持
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pLastEvent));
				}
			}
		}

		this->SetModifiedFlag (TRUE);
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);

	}
}

// 『編集(E)』-『音符の細分化とトリル化...』
void CSekaijuDoc::OnUpdateEditBreakupAndTrillUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}


	// すべてのイベントが非選択であればこの項目をディスエーブルする
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable == 1) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}



// 『編集(E)』-『ビート検出とテンポ自動挿入...』
void CSekaijuDoc::OnEditBeatScan () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}


	CString strMsg;
	
	// フォーマットチェック
	long lFormat = MIDIData_GetFormat (m_pMIDIData);
	if (lFormat == 0) {
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_BEATSCAN_WITH_FORMAT0_MIDIDATA));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}

	// タイムベースチェック
	long lTimeMode, lTimeResolution;
	MIDIData_GetTimeBase (m_pMIDIData, &lTimeMode, &lTimeResolution);
	long lTrackCount = MIDIData_CountTrack (m_pMIDIData);
	if (lTimeMode != MIDIDATA_TPQNBASE) {
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_BEATSCAN_WITH_SMPTEBASE_MIDIDATA));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	
	// 各トラックのトラック名をダイアログクラスの配列に代入
	long i = 0;
	long j = 0;
	TCHAR szTrackName[256];
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDIEvent* pCloneEvent = NULL;
	CEditBeatScanDlg theDlg;
	theDlg.m_lTimeMode = lTimeMode;
	theDlg.m_lTimeResolution = lTimeResolution;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		memset (szTrackName, 0, sizeof (szTrackName));
		MIDITrack_GetName (pMIDITrack, szTrackName, TSIZEOF (szTrackName));
		theDlg.m_theTrackNameArray.Add (szTrackName);
		i++;
	}

	theDlg.m_nBeatTrackIndex = pSekaijuApp->m_theEditBeatScanDlgStatus.m_nBeatTrackIndex;
	theDlg.m_nBeatTrackIndex = CLIP (0, theDlg.m_nBeatTrackIndex, lTrackCount - 1);
	theDlg.m_nBeatIntervalIndex = pSekaijuApp->m_theEditBeatScanDlgStatus.m_nBeatIntervalIndex;
	theDlg.m_nInsertTempo = pSekaijuApp->m_theEditBeatScanDlgStatus.m_nInsertTempo;

	// ダイアログ表示
	if (theDlg.DoModal () == IDOK) {

		pSekaijuApp->m_theEditBeatScanDlgStatus.m_nBeatTrackIndex = theDlg.m_nBeatTrackIndex;
		pSekaijuApp->m_theEditBeatScanDlgStatus.m_nBeatIntervalIndex = theDlg.m_nBeatIntervalIndex;
		pSekaijuApp->m_theEditBeatScanDlgStatus.m_nInsertTempo = theDlg.m_nInsertTempo;

		m_theCriticalSection.Lock ();
		CDWordArray theBeatTimeArray;
		CDWordArray theBeatMillisecArray;
		CDWordArray theTempoArray;

		// 処理後のビート間隔[tick]を計算
		long lBeatDiv[7] = {1, 2, 3, 4, 6, 8, 12};
		long lBeatInterval = lTimeResolution / lBeatDiv[theDlg.m_nBeatIntervalIndex];
		
		// 選択されている各ビートの時刻[tick]とミリ秒[msec]を抽出
		MIDITrack* pBeatTrack = this->GetTrack (theDlg.m_nBeatTrackIndex);
		ASSERT (pBeatTrack);
		long lOldTime = 0;
		long lNumBeatCount = 0;
		theBeatTimeArray.Add (0);
		theBeatMillisecArray.Add (0);
		lNumBeatCount++;
		forEachEvent (pBeatTrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				if (MIDIEvent_IsNoteOn (pMIDIEvent)) {
					long lTime = MIDIEvent_GetTime (pMIDIEvent);
					if (lTime > lOldTime) {
						theBeatTimeArray.Add (lTime);
						long lMillisec = MIDIData_TimeToMillisec (m_pMIDIData, lTime);
						theBeatMillisecArray.Add (lMillisec);
						lNumBeatCount++;
						lOldTime = lTime;
					}
				}
			}
		}
		ASSERT (lNumBeatCount == theBeatTimeArray.GetSize ());
		ASSERT (lNumBeatCount == theBeatMillisecArray.GetSize ());
		
		// 指定トラック内で選択されているビートの数が少なすぎる場合は却下
		if (lNumBeatCount <= 3) {
			m_theCriticalSection.Unlock ();
			VERIFY (strMsg.LoadString (IDS_SELECTED_NOTEEVENT_IS_TOO_FEW_IN_THE_SPECIFIED_TRACK));
			AfxMessageBox (strMsg);
			return;
		}

		// ビートスキャン開始時刻[tick]とビートスキャン終了時刻[tick]を抽出
		long lBeginTime = theBeatTimeArray.GetAt (1);
		long lEndTime = theBeatTimeArray.GetAt (lNumBeatCount - 1);
		long lBeginMeasure, lBeginBeat, lBeginTick;
		MIDIData_BreakTime (m_pMIDIData, lBeginTime, &lBeginMeasure, &lBeginBeat, &lBeginTick);
		long lEndMeasure, lEndBeat, lEndTick;
		MIDIData_BreakTime (m_pMIDIData, lEndTime, &lEndMeasure, &lEndBeat, &lEndTick);

		// ビートスキャン処理開始確認メッセージ
		CString strFormat;
		VERIFY (strFormat.LoadString (IDS_D_NOTEVENT_IS_DETECTED_IN_THE_SPECIFIED_TRACK_CONTINUE));
		strMsg.Format (strFormat, lNumBeatCount - 1,
			lBeginMeasure + 1, lBeginBeat + 1, lBeginTick,
			lEndMeasure + 1, lEndBeat + 1, lEndTick);
		m_theCriticalSection.Unlock ();
		long lRet = AfxMessageBox (strMsg, MB_ICONINFORMATION | MB_YESNOCANCEL);
		if (lRet == IDNO || lRet == IDCANCEL) {
			return;
		}
		m_theCriticalSection.Lock ();
		
		BeginWaitCursor ();
		
		// 各ビート位置における補正後テンポ[μ秒/4分音符]を計算
		long lTempo = 0;
		MIDIData_FindTempo (m_pMIDIData, 0, &lTempo);
		theTempoArray.Add (lTempo);
		for (j = 1; j < lNumBeatCount - 1; j++) {
			lTempo = 
				(theBeatMillisecArray.GetAt (j + 1) - theBeatMillisecArray.GetAt (j)) *
				1000 * lTimeResolution / lBeatInterval;
			theTempoArray.Add (lTempo);
		}
		MIDIData_FindTempo (m_pMIDIData, lEndTime, &lTempo);
		theTempoArray.Add (lTempo);
		ASSERT (lNumBeatCount == theTempoArray.GetSize ());

		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_EDIT_BEATSCAN));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		
		// EOTの履歴保持
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
					VERIFY (pCloneEvent = ReplaceMIDIEvent (pLastEvent));
				}
			}
		}
		
		// 既存のテンポイベントの削除
		long lDeletedTempoEventCount = 0;
		if (theDlg.m_nInsertTempo) {
			forEachTrack (m_pMIDIData, pMIDITrack) {
				// lBeginTime～lEndTimeにあるテンポイベントを配列に登録
				CPtrArray theEventArray;
				forEachEvent (pMIDITrack, pMIDIEvent) {
					if (MIDIEvent_IsTempo (pMIDIEvent)) {
						long lTime = MIDIEvent_GetTime (pMIDIEvent);
						if (lBeginTime <= lTime && lTime < lEndTime) {
							theEventArray.Add (pMIDIEvent);
						}
					}
				}
				// 配列に登録されたイベントを削除
				for (i = 0; i < theEventArray.GetSize (); i++) {
					VERIFY (pMIDIEvent = (MIDIEvent*)theEventArray.GetAt (i));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
					VERIFY (MIDITrack_RemoveEvent (pMIDITrack, pMIDIEvent));
					lDeletedTempoEventCount++;
				}
				theEventArray.RemoveAll ();
			}
		}

		// MIDIイベントの時刻移動
		long lMovedEventCount = 0;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			j = 0;
			// lBeginTime以降にあるイベントを配列に登録
			// (結合イベントの場合は最初のイベントのみ)
			// (エンドオブトラックイベントは自動移動するので除く)
			CPtrArray theEventArray;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				long lTime = MIDIEvent_GetTime (pMIDIEvent);
				if (lTime >= lBeginTime) {
					if (pMIDIEvent->m_pPrevCombinedEvent == NULL &&
						!MIDIEvent_IsEndofTrack (pMIDIEvent)) {
						theEventArray.Add (pMIDIEvent);
					}
				}
			}
			// 配列に登録されたイベントを位置移動
			for (i = 0; i < theEventArray.GetSize (); i++) { 
				VERIFY (pMIDIEvent = (MIDIEvent*)theEventArray.GetAt (i));
				ASSERT (pMIDIEvent->m_pPrevCombinedEvent == NULL);
				ASSERT (MIDIEvent_GetParent (pMIDIEvent) == pMIDITrack);
				// ノートイベント(ノートオン+ノートオフ)の場合
				if (MIDIEvent_IsNote (pMIDIEvent)) {
					long lTime1 = MIDIEvent_GetTime (pMIDIEvent);
					long lDuration = MIDIEvent_GetDuration (pMIDIEvent);
					long lTime2 = lTime1 + lDuration;
					while (j < lNumBeatCount - 1) {
						if ((long)theBeatTimeArray.GetAt (j) <= lTime1 &&
							lTime1 < (long)theBeatTimeArray.GetAt (j + 1)) {
							break;
						}
						j++;
					}
					long k = j;
					while (k < lNumBeatCount - 1) {
						if ((long)theBeatTimeArray.GetAt (k) <= lTime2 &&
							lTime2 < (long)theBeatTimeArray.GetAt (k + 1)) {
							break;
						}
						k++;
					}
					long lNewTime1 = 0;
					long lNewTime2 = 0;
					// ノートオン位置移動
					// 開始時刻or最初のビート時刻より前にあるイベント
					if (j == 0) {
						ASSERT (0 <= lTime1 && lTime1 < lBeginTime);
						ASSERT (FALSE);
					}
					// ビート時刻(j)とビート時刻(j+1)の間にあるイベント
					else if (1 <= j && j < lNumBeatCount - 1) {
						ASSERT (lBeginTime <= lTime1 && lTime1 < lEndTime);
						lNewTime1 = 
							lBeginTime +
							lBeatInterval * (j - 1) +
							lBeatInterval *
							(lTime1 - theBeatTimeArray.GetAt (j)) /
							(theBeatTimeArray.GetAt (j + 1) - theBeatTimeArray.GetAt(j));
						lNewTime1 = CLIP (0, lNewTime1, 0x7FFFFFFF);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						MIDIEvent_SetTime (pCloneEvent, lNewTime1);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
						lMovedEventCount++;
					}
					// 終了時刻or最後のビート時刻より後にあるイベント
					else {
						ASSERT (lTime1 >= lEndTime);
						lNewTime1 =
							lBeginTime +
							lBeatInterval * (lNumBeatCount - 2) +
							lTime1 - theBeatTimeArray.GetAt (lNumBeatCount - 1);
						lNewTime1 = CLIP (0, lNewTime1, 0x7FFFFFFF);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						MIDIEvent_SetTime (pCloneEvent, lNewTime1);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
						lMovedEventCount++;
					}
					// ノートオフ位置移動(音長さの変更による)
					// 開始時刻or最初のビート時刻より前にあるイベント
					if (k == 0) {
						ASSERT (0 <= lTime2 && lTime2 < lBeginTime);
						ASSERT (FALSE);
					}
					// ビート時刻(k)とビート時刻(k+1)の間にあるイベント
					else if (1 <= k && k < lNumBeatCount - 1) {
						ASSERT (lBeginTime <= lTime2 && lTime2 < lEndTime);
						long lNewTime2 = 
							lBeginTime +
							lBeatInterval * (k - 1) +
							lBeatInterval *
							(lTime2 - theBeatTimeArray.GetAt (k)) /
							(theBeatTimeArray.GetAt (k + 1) - theBeatTimeArray.GetAt(k));
						lNewTime2 = CLIP (0, lNewTime2, 0x7FFFFFFF);
						long lNewDuration = lNewTime2 - lNewTime1;
						lNewDuration = CLIP (0, lNewDuration, 0xFFFF);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						MIDIEvent_SetDuration (pCloneEvent, lNewDuration);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
						lMovedEventCount++;
					}
					// 終了時刻or最後のビート時刻より後にあるイベント
					else {
						ASSERT (lTime2 >= lEndTime);
						lNewTime2 =
							lBeginTime +
							lBeatInterval * (lNumBeatCount - 2) +
							lTime2 - theBeatTimeArray.GetAt (lNumBeatCount - 1);
						lNewTime2 = CLIP (0, lNewTime2, 0x7FFFFFFF);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						long lNewDuration = lNewTime2 - lNewTime1;
						lNewDuration = CLIP (0, lNewDuration, 0xFFFF);
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						MIDIEvent_SetDuration (pCloneEvent, lNewDuration);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
						lMovedEventCount++;
					}
				}
				// ノートイベント(ノートオン+ノートオフ)以外の場合
				else {
					long lTime = MIDIEvent_GetTime (pMIDIEvent);
					while (j < lNumBeatCount - 1) {
						if ((long)theBeatTimeArray.GetAt (j) <= lTime &&
							lTime < (long)theBeatTimeArray.GetAt (j + 1)) {
							break;
						}
						j++;
					}
					// 開始時刻or最初のビート時刻より前にあるイベント
					if (j == 0) {
						ASSERT (0 <= lTime && lTime < lBeginTime);
						ASSERT (FALSE);
					}
					// ビート時刻(j)とビート時刻(j+1)の間にあるイベント
					else if (1 <= j && j < lNumBeatCount - 1) {
						ASSERT (lBeginTime <= lTime && lTime < lEndTime);
						long lNewTime = 
							lBeginTime +
							lBeatInterval * (j - 1) +
							lBeatInterval *
							(lTime - theBeatTimeArray.GetAt (j)) /
							(theBeatTimeArray.GetAt (j + 1) - theBeatTimeArray.GetAt(j));
						lNewTime = CLIP (0, lNewTime, 0x7FFFFFFF);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						MIDIEvent_SetTime (pCloneEvent, lNewTime);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
						lMovedEventCount++;
					}
					// 終了時刻or最後のビート時刻より後にあるイベント
					else {
						ASSERT (lTime >= lEndTime);
						long lNewTime =
							lBeginTime +
							lBeatInterval * (lNumBeatCount - 2) +
							lTime - theBeatTimeArray.GetAt (lNumBeatCount - 1);
						lNewTime = CLIP (0, lNewTime, 0x7FFFFFFF);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						MIDIEvent_SetTime (pCloneEvent, lNewTime);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
						lMovedEventCount++;
					}
				}
			} // for
			theEventArray.RemoveAll ();
		} // forEachTrack

		// テンポイベントの挿入
		long lInsertedTempoEventCount = 0;
		if (theDlg.m_nInsertTempo) {
			VERIFY (pMIDITrack = MIDIData_GetFirstTrack (m_pMIDIData));
			for (i = 1; i < theTempoArray.GetSize (); i++) {
				lTempo = theTempoArray.GetAt (i);
				lTempo = CLIP (1, lTempo, 60000000);
				long lTime = lBeginTime + lBeatInterval * (i - 1);
				VERIFY (pMIDIEvent = MIDIEvent_CreateTempo (lTime, lTempo));
				VERIFY (MIDITrack_InsertEvent (pMIDITrack, pMIDIEvent));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pMIDIEvent));
				lInsertedTempoEventCount++;
			}
		}

		// EOTの履歴保持
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pLastEvent));
				}
			}
		}

		this->SetModifiedFlag (TRUE);
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
	}
}

// 『編集(E)』-『ビート検出とテンポ自動挿入...』
void CSekaijuDoc::OnUpdateEditBeatScanUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}


	// すべてのイベントが非選択であればこの項目をディスエーブルする
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable == 1) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}


// 『編集(E)』-『小節を挿入...』(20100728追加)
void CSekaijuDoc::OnEditInsertMeasure () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDIEvent* pCloneEvent = NULL;
	long lTimeMode = MIDIData_GetTimeMode (m_pMIDIData);

	CEditInsertMeasureDlg theDlg;
	theDlg.m_nPosition = pSekaijuApp->m_theEditInsertMeasureDlgStatus.m_nPosition;
	theDlg.m_nNumMeasure = pSekaijuApp->m_theEditInsertMeasureDlgStatus.m_nNumMeasure;
	theDlg.m_bZeroOrigin = (lTimeMode == MIDIDATA_TPQNBASE ? 0 : 1);
	if (theDlg.DoModal () == IDOK) {
		pSekaijuApp->m_theEditInsertMeasureDlgStatus.m_nPosition = theDlg.m_nPosition;
		pSekaijuApp->m_theEditInsertMeasureDlgStatus.m_nNumMeasure = theDlg.m_nNumMeasure;

		BeginWaitCursor ();
		m_theCriticalSection.Lock ();

		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_EDIT_INSERTMEASURE));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		
		long lTime1, lTime2, lMeasureTime, lSlideTime;
		if (lTimeMode == MIDIDATA_TPQNBASE) {
			MIDIData_MakeTime (m_pMIDIData, theDlg.m_nPosition - 1, 0, 0, &lTime1);
			MIDIData_MakeTime (m_pMIDIData, theDlg.m_nPosition , 0, 0, &lTime2);
		}
		else {
			MIDIData_MakeTime (m_pMIDIData, theDlg.m_nPosition, 0, 0, &lTime1);
			MIDIData_MakeTime (m_pMIDIData, theDlg.m_nPosition + 1, 0, 0, &lTime2);
		}
		lMeasureTime = MAX (0, lTime2 - lTime1);
		lSlideTime = lMeasureTime * theDlg.m_nNumMeasure;
		// 後続のMIDIイベントの時刻移動
		// 同じイベントを2回移動しないように、後続のイベントを一時配列に登録する。
		CPtrArray theTempFollowingEventArray;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			pMIDITrack->m_lUser1 = 0;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				// 小節挿入位置より後のイベントのみ
				if (MIDIEvent_GetTime (pMIDIEvent) >= lTime1) {
					// 非結合イベントのみ、結合イベントの場合は前非結合のみ
					if (MIDIEvent_IsCombined (pMIDIEvent) && pMIDIEvent->m_pPrevCombinedEvent == NULL ||
						!MIDIEvent_IsCombined (pMIDIEvent)) {
						theTempFollowingEventArray.Add (pMIDIEvent);
						pMIDITrack->m_lUser1++;
					}
				}
			}
		}
		// 一時配列のイベント1個につき1回の移動をかける。
		long lTempFollowingEventArrayCount = theTempFollowingEventArray.GetSize ();
		if (lSlideTime < 0) {
			// (EOTを最後に移動させるため、昇順に)(20081102修正)
			for (long j = 0; j < lTempFollowingEventArrayCount; j++) {
				pMIDIEvent = (MIDIEvent*)(theTempFollowingEventArray.GetAt (j));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
				VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
				long lTime = MIDIEvent_GetTime (pCloneEvent);
				lTime += lSlideTime;
				lTime = CLIP (0, lTime, 0x7FFFFFFF);
				VERIFY (MIDIEvent_SetTime (pCloneEvent, lTime));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
				pMIDIEvent = pCloneEvent;
			}
		}
		else {
			// (EOTを最初に移動させるため、逆順に)(20081102修正)
			for (long j = lTempFollowingEventArrayCount - 1; j >= 0; j--) {
				pMIDIEvent = (MIDIEvent*)(theTempFollowingEventArray.GetAt (j));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
				VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
				long lTime = MIDIEvent_GetTime (pCloneEvent);
				lTime += lSlideTime;
				lTime = CLIP (0, lTime, 0x7FFFFFFF);
				VERIFY (MIDIEvent_SetTime (pCloneEvent, lTime));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
				pMIDIEvent = pCloneEvent;
			}
		}
		theTempFollowingEventArray.RemoveAll ();
		this->SetModifiedFlag (TRUE);
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
	}
}

// 『編集(E)』-『小節を挿入...』(20100728追加)
void CSekaijuDoc::OnUpdateEditInsertMeasureUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
}

// 『編集(E)』-『小節を除去...』(20100728追加)
void CSekaijuDoc::OnEditRemoveMeasure () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDIEvent* pCloneEvent = NULL;
	long lTimeMode = MIDIData_GetTimeMode (m_pMIDIData);

	CEditRemoveMeasureDlg theDlg;
	theDlg.m_nPosition = pSekaijuApp->m_theEditRemoveMeasureDlgStatus.m_nPosition;
	theDlg.m_nNumMeasure = pSekaijuApp->m_theEditRemoveMeasureDlgStatus.m_nNumMeasure;
	theDlg.m_bZeroOrigin = (lTimeMode == MIDIDATA_TPQNBASE ? 0 : 1);
	if (theDlg.DoModal () == IDOK) {
		pSekaijuApp->m_theEditRemoveMeasureDlgStatus.m_nPosition = theDlg.m_nPosition;
		pSekaijuApp->m_theEditRemoveMeasureDlgStatus.m_nNumMeasure = theDlg.m_nNumMeasure;

		BeginWaitCursor ();
		m_theCriticalSection.Lock ();

		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_EDIT_INSERTMEASURE));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		
		long lTime1, lTime2, lSlideTime;
		if (lTimeMode == MIDIDATA_TPQNBASE) {
			MIDIData_MakeTime (m_pMIDIData, theDlg.m_nPosition - 1, 0, 0, &lTime1);
			MIDIData_MakeTime (m_pMIDIData, theDlg.m_nPosition - 1 + theDlg.m_nNumMeasure, 0, 0, &lTime2);
		}
		else {
			MIDIData_MakeTime (m_pMIDIData, theDlg.m_nPosition, 0, 0, &lTime1);
			MIDIData_MakeTime (m_pMIDIData, theDlg.m_nPosition + theDlg.m_nNumMeasure, 0, 0, &lTime2);
		}
		lSlideTime = MIN (0, lTime1 - lTime2);
		// lTime1～lTime2のイベントの削除
		// 同じイベントを2回移動しないように、lTime1～lTime2のイベントを一時配列に登録する。
		CPtrArray theTempDuringEventArray;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			pMIDITrack->m_lUser1 = 0;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				// lTime1～lTime2のイベントのみ
				long lTime = MIDIEvent_GetTime (pMIDIEvent);
				if (lTime1 <= lTime && lTime < lTime2) {
					// 非結合イベントのみ、結合イベントの場合は前非結合のみ
					if (MIDIEvent_IsCombined (pMIDIEvent) && pMIDIEvent->m_pPrevCombinedEvent == NULL ||
						!MIDIEvent_IsCombined (pMIDIEvent)) {
						theTempDuringEventArray.Add (pMIDIEvent);
						pMIDITrack->m_lUser1++;
					}
				}
			}
		}
		// 一時配列内のイベントを削除(EOTを除く)
		long lTempDuringEventArrayCount = theTempDuringEventArray.GetSize ();
		for (long j = 0; j < lTempDuringEventArrayCount; j++) {
			pMIDIEvent = (MIDIEvent*)(theTempDuringEventArray.GetAt (j));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			if (MIDIEvent_IsEndofTrack (pMIDIEvent)) {
				VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
				long lTime = CLIP (0, lTime1, 0x7FFFFFFF);
				MIDIEvent_SetTime (pCloneEvent, lTime);
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
				pMIDIEvent = pCloneEvent;
			}
			else {
				VERIFY (MIDITrack_RemoveEvent (MIDIEvent_GetParent (pMIDIEvent), pMIDIEvent));
			}
		}
		theTempDuringEventArray.RemoveAll ();
		
		// 後続のMIDIイベントの時刻移動
		// 同じイベントを2回移動しないように、後続のイベントを一時配列に登録する。
		CPtrArray theTempFollowingEventArray;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			pMIDITrack->m_lUser1 = 0;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				// 小節挿入位置より後のイベントのみ
				if (MIDIEvent_GetTime (pMIDIEvent) >= lTime1) {
					// 非結合イベントのみ、結合イベントの場合は前非結合のみ
					if (MIDIEvent_IsCombined (pMIDIEvent) && pMIDIEvent->m_pPrevCombinedEvent == NULL ||
						!MIDIEvent_IsCombined (pMIDIEvent)) {
						theTempFollowingEventArray.Add (pMIDIEvent);
						pMIDITrack->m_lUser1++;
					}
				}
			}
		}
		// 一時配列のイベント1個につき1回の移動をかける。
		long lTempFollowingEventArrayCount = theTempFollowingEventArray.GetSize ();
		if (lSlideTime < 0) {
			// (EOTを最後に移動させるため、昇順に)(20081102修正)
			for (long j = 0; j < lTempFollowingEventArrayCount; j++) {
				pMIDIEvent = (MIDIEvent*)(theTempFollowingEventArray.GetAt (j));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
				VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
				long lTime = MIDIEvent_GetTime (pCloneEvent);
				lTime += lSlideTime;
				lTime = CLIP (0, lTime, 0x7FFFFFFF);
				VERIFY (MIDIEvent_SetTime (pCloneEvent, lTime));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
				pMIDIEvent = pCloneEvent;
			}
		}
		else {
			// (EOTを最初に移動させるため、逆順に)(20081102修正)
			for (long j = lTempFollowingEventArrayCount - 1; j >= 0; j--) {
				pMIDIEvent = (MIDIEvent*)(theTempFollowingEventArray.GetAt (j));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
				VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
				long lTime = MIDIEvent_GetTime (pCloneEvent);
				lTime += lSlideTime;
				lTime = CLIP (0, lTime, 0x7FFFFFFF);
				VERIFY (MIDIEvent_SetTime (pCloneEvent, lTime));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
				pMIDIEvent = pCloneEvent;
			}
		}
		theTempFollowingEventArray.RemoveAll ();
		this->SetModifiedFlag (TRUE);
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
	}
}

// 『編集(E)』-『小節を除去...』(20100728追加)
void CSekaijuDoc::OnUpdateEditRemoveMeasureUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
}



// 『ビュー(&V)』-『再表示(&R)』
void CSekaijuDoc::OnViewRedraw () {
	//this->UpdateAllViews (NULL, 0xFFFFFFFF);
	this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIDATACHANGED | 
		SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIEVENTCHANGED);
	// TODO:上の2つのどちらが良いか。
}

// 『ビュー(&V)』-『新規トラックリスト(&T)』
void CSekaijuDoc::OnViewTrackList () {
	m_theCriticalSection.Lock ();
	m_lTempTime = m_lNewTime;
	m_pTempTrack = NULL;
	m_pTempEvent = NULL;
	m_theCriticalSection.Unlock ();
	ShowTrackListFrame ();
}

// 『ビュー(&V)』-『新規ピアノロール(&P)』
void CSekaijuDoc::OnViewPianoRoll () {
	m_theCriticalSection.Lock ();
	m_lTempTime = m_lNewTime;
	m_pTempTrack = NULL;
	m_pTempEvent = NULL;
	MIDITrack* pMIDITrack = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		pMIDITrack->m_lUserFlag |= MIDITRACK_VISIBLE;
	}
	m_theCriticalSection.Unlock ();
	ShowPianoRollFrame ();
}

// 『ビュー(&V)』-『新規イベントリスト(&E)』
void CSekaijuDoc::OnViewEventList () {
	m_theCriticalSection.Lock ();
	m_lTempTime = m_lNewTime;
	m_pTempTrack = NULL;
	m_pTempEvent = NULL;
	MIDITrack* pMIDITrack = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		pMIDITrack->m_lUserFlag |= MIDITRACK_VISIBLE;
	}
	m_theCriticalSection.Unlock ();
	ShowEventListFrame ();
}

// 『ビュー(&V)』-『新規譜面(&S)』
void CSekaijuDoc::OnViewMusicalScore () {
	m_theCriticalSection.Lock ();
	m_lTempTime = m_lNewTime;
	m_pTempTrack = NULL;
	m_pTempEvent = NULL;
	MIDITrack* pMIDITrack = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		pMIDITrack->m_lUserFlag |= MIDITRACK_VISIBLE;
	}
	m_theCriticalSection.Unlock ();
	ShowMusicalScoreFrame ();
}


// 『ポップアップ』-『このトラックのみトラックリスト表示(&T)』
void CSekaijuDoc::OnPopupShowTrackList () {
	if (m_pTempTrack != NULL) { // 20100429追加
		ShowTrackListFrame ();
	}
}

// 『ポップアップ』-『このトラックのみトラックリスト表示(&T)』 // 20100429追加
void CSekaijuDoc::OnUpdatePopupShowTrackListUI (CCmdUI* pCmdUI) {
	if (m_pTempTrack == NULL) { // 20100429追加
		pCmdUI->Enable (FALSE);
	}
}

// 『ポップアップ』-『このトラックのみピアノロール表示(&P)』
void CSekaijuDoc::OnPopupShowPianoRoll () {
	if (m_pTempTrack != NULL) { // 20100429追加
		ShowPianoRollFrame ();
	}
}

// 『ポップアップ』-『このトラックのみピアノロール表示(&P)』 // 20100429追加
void CSekaijuDoc::OnUpdatePopupShowPianoRollUI (CCmdUI* pCmdUI) {
	if (m_pTempTrack == NULL) { // 20100429追加
		pCmdUI->Enable (FALSE);
	}
}

// 『ポップアップ』-『このトラックのみイベントリスト表示(&E)』
void CSekaijuDoc::OnPopupShowEventList () {
	if (m_pTempTrack != NULL) { // 20100429追加
		ShowEventListFrame ();
	}
}

// 『ポップアップ』-『このトラックのみイベントリスト表示(&E)』 // 20100429追加
void CSekaijuDoc::OnUpdatePopupShowEventListUI (CCmdUI* pCmdUI) {
	if (m_pTempTrack == NULL) { // 20100429追加
		pCmdUI->Enable (FALSE);
	}
}

// 『ポップアップ』-『このトラックのみ譜面表示(&S)』
void CSekaijuDoc::OnPopupShowMusicalScore () {
	if (m_pTempTrack != NULL) { // 20100429追加
		ShowMusicalScoreFrame ();
	}
}

// 『ポップアップ』-『このトラックのみ譜面表示(&S)』 // 20100429追加
void CSekaijuDoc::OnUpdatePopupShowMusicalScoreUI (CCmdUI* pCmdUI) {
	if (m_pTempTrack == NULL) { // 20100429追加
		pCmdUI->Enable (FALSE);
	}
}


// 『ポップアップ』-『このトラックのみ入力ON』
void CSekaijuDoc::OnPopupTrackInputOn () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}
	if (m_pTempTrack == NULL) {  // 20100429追加
		return;
	}
	
	m_theCriticalSection.Lock ();

	// 入力ON/OFF変更処理(履歴記録)
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_ONLY_THIS_TRACK_INPUT_ON));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	MIDITrack* pMIDITrack = NULL;
	MIDITrack* pCloneTrack = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack));
		VERIFY (pCloneTrack = this->ReplaceMIDITrack (pMIDITrack));
		if (pMIDITrack == m_pTempTrack) {
			MIDITrack_SetInputOn (pCloneTrack, 1);
		}
		else {
			MIDITrack_SetInputOn (pCloneTrack, 0);
		}
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
		pMIDITrack = pCloneTrack;
	}

	UpdateAllViews (NULL, SEKAIJUDOC_MIDITRACKCHANGED);
	m_theCriticalSection.Unlock ();	
	::Sleep (0);
}

// 『ポップアップ』-『このトラックのみ入力ON』
void CSekaijuDoc::OnUpdatePopupTrackInputOnUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_pTempTrack == NULL) {  // 20100429追加
		pCmdUI->Enable (FALSE);
		return;
	}
}

// 『ポップアップ』-『このトラックのみ入力OFF』
void CSekaijuDoc::OnPopupTrackInputOff () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}
	if (m_pTempTrack == NULL) {  // 20100429追加
		return;
	}

	m_theCriticalSection.Lock ();

	// 入力ON/OFF変更処理(履歴記録)
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_ONLY_THIS_TRACK_INPUT_OFF));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	MIDITrack* pMIDITrack = NULL;
	MIDITrack* pCloneTrack = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack));
		VERIFY (pCloneTrack = this->ReplaceMIDITrack (pMIDITrack));
		if (pMIDITrack == m_pTempTrack) {
			MIDITrack_SetInputOn (pCloneTrack, 0);
		}
		else {
			MIDITrack_SetInputOn (pCloneTrack, 1);
		}
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
		pMIDITrack = pCloneTrack;
	}

	UpdateAllViews (NULL, SEKAIJUDOC_MIDITRACKCHANGED);
	m_theCriticalSection.Unlock ();	
	::Sleep (0);
}

// 『ポップアップ』-『このトラックのみ入力OFF』
void CSekaijuDoc::OnUpdatePopupTrackInputOffUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_pTempTrack == NULL) {  // 20100429追加
		pCmdUI->Enable (FALSE);
		return;
	}
}

// 『ポップアップ』-『全トラック入力ON』
void CSekaijuDoc::OnPopupTrackInputAll () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	m_theCriticalSection.Lock ();
	// 入力ON/OFF変更処理(履歴記録)
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_ALL_TRACK_INPUT_ON));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	MIDITrack* pMIDITrack = NULL;
	MIDITrack* pCloneTrack = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack));
		VERIFY (pCloneTrack = this->ReplaceMIDITrack (pMIDITrack));
		MIDITrack_SetInputOn (pCloneTrack, 1);
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
		pMIDITrack = pCloneTrack;
	}
	UpdateAllViews (NULL, SEKAIJUDOC_MIDITRACKCHANGED);
	m_theCriticalSection.Unlock ();	
	::Sleep (0);
}

// 『ポップアップ』-『全トラック入力ON』
void CSekaijuDoc::OnUpdatePopupTrackInputAllUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
}

// 『ポップアップ』-『このトラックのみ出力ON』
void CSekaijuDoc::OnPopupTrackOutputOn () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}
	if (m_pTempTrack == NULL) {  // 20100429追加
		return;
	}

	m_theCriticalSection.Lock ();
	// 出力ON/OFF変更処理(履歴記録)
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_ONLY_THIS_TRACK_OUTPUT_ON));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	MIDITrack* pMIDITrack = NULL;
	MIDITrack* pCloneTrack = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack));
		VERIFY (pCloneTrack = this->ReplaceMIDITrack (pMIDITrack));
		if (pMIDITrack == m_pTempTrack) {
			MIDITrack_SetOutputOn (pCloneTrack, 1);
		}
		else {
			MIDITrack_SetOutputOn (pCloneTrack, 0);
		}
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
		pMIDITrack = pCloneTrack;
	}

	// 演奏状態復元処理
	long lCurTime = MIDIClock_GetTickCount (m_pMIDIClock);
	pSekaijuApp->m_theCriticalSection.Lock ();
	pSekaijuApp->ResetTempMIDIStatusArray ();
	this->TimeMIDIStatus (lCurTime, pSekaijuApp->m_pTempMIDIStatus);
	long lFlags;
	// 再生中ならば(a)すべて又は(b)ノートのみを復元する
	if (pSekaijuApp->m_bPlaying) {
		lFlags = pSekaijuApp->m_theGeneralOption.m_bSearchUpdate ? SDS_ALL : SDS_NOTE;
		pSekaijuApp->m_bIgnoreNoteEvent = 1;
	}
	// 停止中ならば(a)ノートを除くすべてを復元するか(b)何も復元しない 
	else {
		lFlags = pSekaijuApp->m_theGeneralOption.m_bSearchUpdate ? (SDS_ALL & ~SDS_NOTE) : 0;
		pSekaijuApp->m_bIgnoreNoteEvent = 0;
	}
	pSekaijuApp->SendDifferentStatus (lFlags);
	pSekaijuApp->m_theCriticalSection.Unlock ();
	
	UpdateAllViews (NULL, SEKAIJUDOC_MIDITRACKCHANGED);
	m_theCriticalSection.Unlock ();	
	::Sleep (0);
}

// 『ポップアップ』-『このトラックのみ出力ON』
void CSekaijuDoc::OnUpdatePopupTrackOutputOnUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_pTempTrack == NULL) { // 20100429追加
		pCmdUI->Enable (FALSE);
		return;
	}
}

// 『ポップアップ』-『このトラックのみ出力OFF』
void CSekaijuDoc::OnPopupTrackOutputOff () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}
	if (m_pTempTrack == NULL) {  // 20100429追加
		return;
	}

	m_theCriticalSection.Lock ();
	// 出力ON/OFF変更処理(履歴記録)
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_ONLY_THIS_TRACK_OUTPUT_OFF));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	MIDITrack* pMIDITrack = NULL;
	MIDITrack* pCloneTrack = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack));
		VERIFY (pCloneTrack = this->ReplaceMIDITrack (pMIDITrack));
		if (pMIDITrack == m_pTempTrack) {
			MIDITrack_SetOutputOn (pCloneTrack, 0);
		}
		else {
			MIDITrack_SetOutputOn (pCloneTrack, 1);
		}
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
		pMIDITrack = pCloneTrack;
	}

	// 演奏状態復元処理
	long lCurTime = MIDIClock_GetTickCount (m_pMIDIClock);
	pSekaijuApp->m_theCriticalSection.Lock ();
	pSekaijuApp->ResetTempMIDIStatusArray ();
	this->TimeMIDIStatus (lCurTime, pSekaijuApp->m_pTempMIDIStatus);
	long lFlags;
	// 再生中ならば(a)すべて又は(b)ノートのみを復元する
	if (pSekaijuApp->m_bPlaying) {
		lFlags = pSekaijuApp->m_theGeneralOption.m_bSearchUpdate ? SDS_ALL : SDS_NOTE;
		pSekaijuApp->m_bIgnoreNoteEvent = 1;
	}
	// 停止中ならば(a)ノートを除くすべてを復元するか(b)何も復元しない 
	else {
		lFlags = pSekaijuApp->m_theGeneralOption.m_bSearchUpdate ? (SDS_ALL & ~SDS_NOTE) : 0;
		pSekaijuApp->m_bIgnoreNoteEvent = 0;
	}
	pSekaijuApp->SendDifferentStatus (lFlags);
	pSekaijuApp->m_theCriticalSection.Unlock ();
	
	
	UpdateAllViews (NULL, SEKAIJUDOC_MIDITRACKCHANGED);
	m_theCriticalSection.Unlock ();	
	::Sleep (0);
}

// 『ポップアップ』-『このトラックのみ出力OFF』
void CSekaijuDoc::OnUpdatePopupTrackOutputOffUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_pTempTrack == NULL) { // 20100429追加
		pCmdUI->Enable (FALSE);
		return;
	}
}

// 『ポップアップ』-『全トラック出力ON』
void CSekaijuDoc::OnPopupTrackOutputAll () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	m_theCriticalSection.Lock ();

	// 出力ON/OFF変更処理(履歴記録)
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_ALL_TRACK_OUTPUT_ON));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	MIDITrack* pMIDITrack = NULL;
	MIDITrack* pCloneTrack = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack));
		VERIFY (pCloneTrack = this->ReplaceMIDITrack (pMIDITrack));
		MIDITrack_SetOutputOn (pCloneTrack, 1);
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
		pMIDITrack = pCloneTrack;
	}

	// 演奏状態復元処理
	long lCurTime = MIDIClock_GetTickCount (m_pMIDIClock);
	pSekaijuApp->m_theCriticalSection.Lock ();
	pSekaijuApp->ResetTempMIDIStatusArray ();
	this->TimeMIDIStatus (lCurTime, pSekaijuApp->m_pTempMIDIStatus);
	long lFlags;
	// 再生中ならば(a)すべて又は(b)ノートのみを復元する
	if (pSekaijuApp->m_bPlaying) {
		lFlags = pSekaijuApp->m_theGeneralOption.m_bSearchUpdate ? SDS_ALL : SDS_NOTE;
		pSekaijuApp->m_bIgnoreNoteEvent = 1;
	}
	// 停止中ならば(a)ノートを除くすべてを復元するか(b)何も復元しない 
	else {
		lFlags = pSekaijuApp->m_theGeneralOption.m_bSearchUpdate ? (SDS_ALL & ~SDS_NOTE) : 0;
		pSekaijuApp->m_bIgnoreNoteEvent = 0;
	}
	pSekaijuApp->SendDifferentStatus (lFlags);
	pSekaijuApp->m_theCriticalSection.Unlock ();

	
	UpdateAllViews (NULL, SEKAIJUDOC_MIDITRACKCHANGED);
	m_theCriticalSection.Unlock ();	
	::Sleep (0);
}

// 『ポップアップ』-『全トラック出力ON』
void CSekaijuDoc::OnUpdatePopupTrackOutputAllUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
}

// 『ポップアップ』-『切り取り(&T)』
void CSekaijuDoc::OnPopupCut () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}


	long lRet = 0;
	long lCopiedEventCount = 0;
	BeginWaitCursor ();

	// クリップボードに転送する文字列を作成
	CString strText;
	m_theCriticalSection.Lock ();
	lCopiedEventCount = this->MakeCopyString (strText, 0x0001, NULL);
	UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
	if (lCopiedEventCount == 0) {
		EndWaitCursor ();
		return;
	}

	// クリップボードに独自テキストを転送
	lRet = this->SetClipboardTextPrivate9 (strText);
	if (lRet == FALSE) {
		EndWaitCursor ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_WRITE_TO_CLIPBOARD));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}

	// 選択イベントを削除
	CHistoryUnit* pCurHistoryUnit = NULL;
	m_theCriticalSection.Lock ();
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_EDIT_CUT));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());

	this->DeleteSelectedEvent (pCurHistoryUnit);
	
	SetModifiedFlag (TRUE);
	UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);
}

// 『ポップアップ』-『切り取り(&T)』
void CSekaijuDoc::OnUpdatePopupCutUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	// ひとつでもMIDIイベントが選択されていればEnable
	// そうでなければDisableとする
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack;
	MIDIEvent* pMIDIEvent;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);

}

// 『ポップアップ』-『コピー(&C)』
void CSekaijuDoc::OnPopupCopy () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}


	long lRet = 0;
	long lCopiedEventCount = 0;
	BeginWaitCursor ();

	// クリップボードに転送する文字列を作成
	CString strText;
	m_theCriticalSection.Lock ();
	lCopiedEventCount = this->MakeCopyString (strText, 0x0001, NULL);
	UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
	if (lCopiedEventCount == 0) {
		EndWaitCursor ();
		return;
	}

	// クリップボードに独自テキストを転送
	lRet = this->SetClipboardTextPrivate9 (strText);
	if (lRet == FALSE) {
		EndWaitCursor ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_WRITE_TO_CLIPBOARD));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	EndWaitCursor ();
}

// 『ポップアップ』-『コピー(&C)』
void CSekaijuDoc::OnUpdatePopupCopyUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}


	// ひとつでもMIDIイベントが選択されていればEnable
	// そうでなければDisableとする
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack;
	MIDIEvent* pMIDIEvent;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// 『ポップアップ』-『貼り付け(&T)』
void CSekaijuDoc::OnPopupPaste () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}


	BeginWaitCursor ();
	CString strMsg;

	// クリップボードのテキストを取得
	CString strText;
	long lRet = this->GetClipboardTextPrivate9 (strText);
	if (lRet == FALSE) {
		EndWaitCursor ();
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_READ_FROM_CLIPBOARD));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	
	// テキストフォーマット検査
	if (strText.Left(5) != _T("MDa2 ")) {
		EndWaitCursor ();
		VERIFY (strMsg.LoadString (IDS_NO_AVAILABLE_MIDIDATA_IN_THE_CLIPBOARD));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	
	// 貼り付け処理
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_EDIT_PASTE));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	m_theCriticalSection.Lock ();
	long lInsertedEventCount = this->ParsePasteString (strText, m_lTempTrackIndex, m_lTempTime, pCurHistoryUnit);
	this->SetModifiedFlag (TRUE);
	this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED |
		SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED);
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);

	// 結果のメッセージボックス表示
	if (lInsertedEventCount == -1) {
		VERIFY (strMsg.LoadString (IDS_PASTE_FAILED));
	}
	else {
		CString strFormat;
		VERIFY (strFormat.LoadString (IDS_D_MIDIEVENTS_WERE_INSERTED));
		strMsg.Format (strFormat, lInsertedEventCount);
	}
	AfxMessageBox (strMsg, MB_ICONINFORMATION);
}

// 『ポップアップ』-『貼り付け(&T)』
void CSekaijuDoc::OnUpdatePopupPasteUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
	
	// クリップボードの形式が独自テキストか調べる
	pCmdUI->Enable (this->IsClipboardTextPrivate9 ());

}




