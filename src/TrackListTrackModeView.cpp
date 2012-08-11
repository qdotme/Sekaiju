//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// トラックリストトラックモードビュークラス
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
#include <afxmt.h>
#include "common.h"
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "mousewheel.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDoc.h"
#include "SekaijuView.h"
#include "SekaijuToolbar.h"
#include "ChildFrame.h"
#include "TrackListFrame.h"
#include "TrackListTrackModeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define IDC_TEXTBOX 3939

IMPLEMENT_DYNCREATE (CTrackListTrackModeView, CSekaijuView)

// メッセージマップ
BEGIN_MESSAGE_MAP (CTrackListTrackModeView, CSekaijuView)
	ON_WM_CREATE ()
	ON_WM_KILLFOCUS ()
	ON_WM_KEYDOWN ()
	ON_WM_KEYUP ()
	ON_WM_CHAR ()
	ON_WM_LBUTTONDOWN ()	
	ON_WM_RBUTTONDOWN ()	
	ON_WM_LBUTTONUP ()	
	ON_WM_RBUTTONUP ()	
	ON_WM_MOUSEMOVE ()
	ON_WM_LBUTTONDBLCLK ()
	ON_WM_RBUTTONDBLCLK ()
	ON_WM_TIMER ()
	ON_WM_MOUSEWHEEL40 ()
END_MESSAGE_MAP()

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CTrackListTrackModeView::CTrackListTrackModeView () {
	m_lCurRow = 0;
	m_lCurColumn = 0;
	m_lCurButtonState = 0x00;
	m_lCurButtonInterval = 200;
	m_bSettingCellString = 0;
}

// デストラクタ
CTrackListTrackModeView::~CTrackListTrackModeView () {
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// (X,Y)座標からセル番号を取得
BOOL CTrackListTrackModeView::GetCellFromPoint 
(CPoint pt, long* pRow, long* pColumn) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	_ASSERT (pRow);
	_ASSERT (pColumn);
	*pRow = pTrackListFrame->YtoRow (pt.y);
	*pColumn = pTrackListFrame->XtoColumn (pt.x);
	return TRUE;
}

// 指定セルがビューからはみ出した場合のオートスクロール処理
BOOL CTrackListTrackModeView::AutoScrolltoShowCell (long lRow, long lColumn) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	long lNewPos = 0;
	long lRowZoom = pTrackListFrame->GetRowZoom ();
	// 指定セルがビューの左にはみ出した場合の処理
	if (pTrackListFrame->GetColumnLeft (lColumn) < rcClient.left) {
		lNewPos = pTrackListFrame->GetColumnLeft (lColumn);
		pTrackListFrame->SetColumnScrollPos (lNewPos);
	}
	// 指定セルがビューの右にはみ出した場合の処理
	else if (pTrackListFrame->GetColumnLeft (lColumn) + 
		pTrackListFrame->GetColumnWidth (lColumn) > rcClient.right) {
		lNewPos = pTrackListFrame->GetColumnLeft (lColumn) + 
			pTrackListFrame->GetColumnWidth (lColumn) - rcClient.Width ();
		pTrackListFrame->SetColumnScrollPos (lNewPos);
	}
	// 指定セルがビューの上にはみ出した場合の処理
	if (lRow * lRowZoom < rcClient.top) {
		lNewPos = lRow * lRowZoom;
		pTrackListFrame->SetRowScrollPos (lNewPos);		
	}
	// 指定セルがビューの下にはみ出した場合の処理
	else if ((lRow + 1) * lRowZoom > rcClient.bottom) {
		lNewPos = (lRow + 1) * lRowZoom - rcClient.Height ();
		pTrackListFrame->SetRowScrollPos (lNewPos);
	}
	return TRUE;
}

// 現在テキストボックスで編集中かどうか返す。
BOOL CTrackListTrackModeView::IsTextEditing () {
	return (m_theTextBox.GetStyle () & WS_VISIBLE) ? TRUE : FALSE;
}

// テキストボックスでの編集を開始する。
BOOL CTrackListTrackModeView::BeginTextEditing () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lFormat = MIDIData_GetFormat (pMIDIData);
	ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
	//ASSERT (lFormat == 0 && m_lCurRow == 0 || lFormat == 1 && m_lCurRow != 0);
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	pSekaijuApp->m_bInplaceEditing = 1;
	CString strCellString = GetCellString (m_lCurRow, m_lCurColumn);
	m_theTextBox.SetWindowText (strCellString);
	m_theTextBox.SetSel (0, -1, TRUE);
	m_theTextBox.EmptyUndoBuffer ();
	m_theTextBox.ShowWindow (SW_SHOW);
	m_theTextBox.SetFocus ();
	m_theTextBox.UpdateWindow ();
	return TRUE;
}

// テキストボックスでの編集を終了し、新しい値を格納する。
BOOL CTrackListTrackModeView::EndTextEditingOK () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();

	// 新しい履歴の作成・不足トラックの追加・値の設定はSetCellStringがやる。

	// 現在のセルの編集テキストを反映
	CString strText;
	m_theTextBox.GetWindowText (strText);
	if (SetCellString (m_lCurRow, m_lCurColumn, strText)) {
		pSekaijuDoc->SetModifiedFlag (TRUE);
		pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIEVENTCHANGED);
	}

	// 編集終了
	m_theTextBox.ShowWindow (SW_HIDE);
	this->SetFocus ();
	pSekaijuApp->m_bInplaceEditing = 0;
	return TRUE;
}

// テキストボックスでの編集を終了し、新しい値を格納しない。
BOOL CTrackListTrackModeView::EndTextEditingCancel () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	m_theTextBox.ShowWindow (SW_HIDE);
	this->SetFocus ();
	pSekaijuApp->m_bInplaceEditing = 0;
	return TRUE;
}

// 値の増減を開始する
BOOL CTrackListTrackModeView::BeginValueUpDown () {
	_RPTF0 (_CRT_WARN, "BeginValueUpDown\n");
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	ASSERT (m_lCurButtonState == 0x0000);
	long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
	long lNumber = lColumnContent >> 16;
	long lFormat = MIDIData_GetFormat (pMIDIData);
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	//ASSERT (lFormat == 0 && m_lCurRow == 0 || lFormat == 1 && m_lCurRow != 0);
	MIDITrack* pTempTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	MIDITrack* pCloneTrack = NULL;
	MIDIEvent* pCloneEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	long lChannel = 0;


	// 項目別に
	switch (lColumnContent & 0xFFFF) {
	// 入力ポート番号、入力チャンネル、出力ポート番号、出力チャンネルの場合
	case TRACKLISTFRAME_INPUTPORT:
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_INPUTPORT));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックを取得
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// 既存のトラックを保持するために置き換える。
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack)); 
		break;
	case TRACKLISTFRAME_INPUTCHANNEL:
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_INPUTCHANNEL));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックを取得
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// 既存のトラックを保持するために置き換える。
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack)); 
		break;
	case TRACKLISTFRAME_OUTPUTPORT:
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_OUTPUTPORT));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックを取得
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// 既存のトラックを保持するために置き換える。
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack)); 
		break;
	case TRACKLISTFRAME_OUTPUTCHANNEL:
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_OUTPUTCHANNEL));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックを取得
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// 既存のトラックを保持するために置き換える。
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack)); 
		break;
	// コントロールチェンジの場合
	case TRACKLISTFRAME_CONTROLCHANGE:
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_CONTROLCHANGE));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックを取得
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// このトラックで最初のCC#lNumberイベントを探す。
		lChannel = MIDITrack_GetOutputChannel (pTempTrack);
		pTempEvent = MIDITrack_GetFirstKindEvent (pTempTrack, MIDIEVENT_CONTROLCHANGE | lChannel);
		while (pTempEvent) {
			if (MIDIEvent_GetNumber (pTempEvent) == lNumber) {
				break;
			}
			pTempEvent = pTempEvent->m_pNextSameKindEvent;
		}
		// なかった場合、コントロールチェンジイベントを新規作成してトラックに追加。
		if (pTempEvent == NULL) {
			lChannel = CLIP (0, lChannel, 15);
			VERIFY (pTempEvent = MIDIEvent_CreateControlChange (0, lChannel, lNumber, 0));
			VERIFY (MIDITrack_InsertEvent (pTempTrack, pTempEvent));
		}
		// あった場合、既存のものを保持し、新しいコントロールチェンジイベントに置き換える。
		else {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent));
		}
		break;
	// プログラムチェンジの場合
	case TRACKLISTFRAME_PROGRAMCHANGE:
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_PROGRAMCHANGE));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックを取得
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// このトラックで最初のプログラムチェンジイベントを探す。
		lChannel = MIDITrack_GetOutputChannel (pTempTrack);
		pTempEvent = MIDITrack_GetFirstKindEvent (pTempTrack, MIDIEVENT_PROGRAMCHANGE | lChannel);
		// なかった場合、プログラムチェンジイベントを新規作成してトラックに追加。
		if (pTempEvent == NULL) {
			lChannel = CLIP (0, lChannel, 15);
			VERIFY (pTempEvent = MIDIEvent_CreateProgramChange (0, lChannel, 0));
			VERIFY (MIDITrack_InsertEvent (pTempTrack, pTempEvent));
		}
		// あった場合、既存のものを保持し、新しいプログラムチェンジイベントに置き換える。
		else {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent));
		}
		break;
	// タイム+、キー+、ベロシティ+の場合(20091126追加)
	case TRACKLISTFRAME_TIMEPLUS:
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_TIMEPLUS));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックを取得
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// 既存のトラックを保持するために置き換える。
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack)); 
		break;
	case TRACKLISTFRAME_KEYPLUS:
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_KEYPLUS));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックを取得
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// 既存のトラックを保持するために置き換える。
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack)); 
		break;
	case TRACKLISTFRAME_VELOCITYPLUS:
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_VELPLUS));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックを取得
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// 既存のトラックを保持するために置き換える。
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack)); 
		break;
	}
	return TRUE;
}

// 値の増減を終了する
BOOL CTrackListTrackModeView::EndValueUpDown () {
	_RPTF0 (_CRT_WARN, "EndValueUpDown\n");
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
	long lNumber = lColumnContent >> 16;
	MIDITrack* pTempTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	// 新しい履歴の用意
	VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
	VERIFY (pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow));
	long lChannel = MIDITrack_GetOutputChannel (pTempTrack);
	lChannel = CLIP (0, lChannel, 15);
	// 項目別に
	switch (lColumnContent & 0xFFFF) {
	case TRACKLISTFRAME_INPUTPORT:
	case TRACKLISTFRAME_INPUTCHANNEL:
	case TRACKLISTFRAME_OUTPUTPORT:
	case TRACKLISTFRAME_OUTPUTCHANNEL:
		// 履歴記録
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pTempTrack));
		break;
	// コントロールチェンジ
	case TRACKLISTFRAME_CONTROLCHANGE:
		// このトラックで最初のCC#lNumberイベントを探す。
		pTempEvent = MIDITrack_GetFirstKindEvent (pTempTrack, MIDIEVENT_CONTROLCHANGE | lChannel);
		while (pTempEvent) {
			if (MIDIEvent_GetNumber (pTempEvent) == lNumber) {
				break;
			}
			pTempEvent = pTempEvent->m_pNextSameKindEvent;
		}
		// 注意：フォーマット0の最初以外のトラック又はフォーマット1の最初のトラックで設定してはならない。
		ASSERT (pTempEvent);
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
		break;
	// プログラムチェンジ
	case MIDIEVENT_PROGRAMCHANGE:
		// このトラックで最初のプログラムチェンジイベントを探す。
		pTempEvent = MIDITrack_GetFirstKindEvent (pTempTrack, MIDIEVENT_PROGRAMCHANGE | lChannel);
		// 注意：フォーマット0の最初以外のトラック又はフォーマット1の最初のトラックで設定してはならない。
		ASSERT (pTempEvent);
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
		break;
	// タイム+、キー+、ベロシティ+の場合(20091126追加)
	case TRACKLISTFRAME_TIMEPLUS:
	case TRACKLISTFRAME_KEYPLUS:
	case TRACKLISTFRAME_VELOCITYPLUS:
		// 履歴記録
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pTempTrack));
	}
	return TRUE;
}

// 現在のセルの値を増減させる
BOOL CTrackListTrackModeView::AddValueOfCurCell (long lDeltaValue) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	long lFormat = MIDIData_GetFormat (pMIDIData);
	// 現在のトラックへのポインタを取得
	// (該当するトラックは BeginValueUpDownで追加されているはずである)
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	VERIFY (pMIDITrack = pSekaijuDoc->GetTrack (m_lCurRow));
	// 値の反映
	long lValue = 0;
	long lNumber = 0;
	ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
	long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
	switch (lColumnContent & 0xFFFF) {
	// コントロールチェンジ(0～127)の場合
	case TRACKLISTFRAME_CONTROLCHANGE:
		// 注意：フォーマット0の最初以外のトラック又はフォーマット1の最初のトラックでは設定してはならない。
		lNumber = pTrackListFrame->GetColumnContent (m_lCurColumn) >> 16;
		VERIFY (pMIDIEvent = pSekaijuDoc->GetTrackFirstControlChange (pMIDITrack, lNumber));
		lValue = MIDIEvent_GetValue (pMIDIEvent);
		// (Bank Select MSB (CC#0) 又は Bank Select LSB (CC#32)) の場合かつパッチサーチONのとき
		if ((lNumber == 0 || lNumber == 32) && pSekaijuApp->m_theGeneralOption.m_bPatchSearch) {
			long lRet;
			MIDIEvent* pCC0Event = pSekaijuDoc->FindBankMSB (pMIDIEvent);
			long lBankMSB = 0;
			if (pCC0Event) {
				lBankMSB = MIDIEvent_GetValue (pCC0Event);
			}
			MIDIEvent* pCC32Event = pSekaijuDoc->FindBankLSB (pMIDIEvent);
			long lBankLSB = 0;
			if (pCC32Event) {
				lBankLSB = MIDIEvent_GetValue (pCC32Event);
			}
			MIDIEvent* pPCEvent = pSekaijuDoc->FindProgramChange (pMIDIEvent);
			long lProgramChange = 0;
			if (pPCEvent) {
				lProgramChange = MIDIEvent_GetNumber (pPCEvent);
			}
			long lTrackOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
			long lTrackViewMode = MIDITrack_GetViewMode (pMIDITrack);
			MIDIInstrumentDefinition* pMIDIInstDef = lTrackViewMode ?
				pSekaijuApp->m_pMIDIInstDefDrum[lTrackOutputPort] :
				pSekaijuApp->m_pMIDIInstDefNorm[lTrackOutputPort];
			// インストゥルメント定義が設定されている場合
			if (pMIDIInstDef) {
				MIDIPatchNameTable* pMIDIPatchNameTable = NULL;
				long lBank = 0;
				TCHAR szBuf[256];
				long lNewValue = lValue + (lDeltaValue > 0 ? 1 : -1);
				long lTempNewValue = lValue; // 最後に音色が見つかった新しい値
				while (0 <= lNewValue && lNewValue <= 127) {
					if (lNumber == 0) {
						lBank = (lNewValue << 7) | lBankLSB;
					}
					else if (lNumber == 32) {
						lBank = (lBankMSB << 7) | lNewValue;
					}
					// 音色があるか調べる
					pMIDIPatchNameTable = 
					MIDIInstrumentDefinition_GetPatchNameTable (pMIDIInstDef, lBank);				
					lRet = 0;
					if (pMIDIPatchNameTable) {
						lRet = MIDIPatchNameTable_GetName 
							(pMIDIPatchNameTable, lProgramChange, szBuf, TSIZEOF (szBuf));
					}
					// 音色があった場合
					if (lRet > 0) {
						lTempNewValue = lNewValue;
						if (lNewValue - lValue >= lDeltaValue && lDeltaValue > 0 ||
							lNewValue - lValue <= lDeltaValue && lDeltaValue < 0) {
							break;
						}
					}
					lNewValue += (lDeltaValue > 0 ? 1 : -1);
				}
				VERIFY (MIDIEvent_SetValue (pMIDIEvent, CLIP (0, lTempNewValue, 127)));
			}
			// インストゥルメント定義が設定されていない場合(通常の場合と同じ処理)
			else {
				VERIFY (MIDIEvent_SetValue (pMIDIEvent, CLIP (0, lValue + lDeltaValue, 127)));
			}
		}
		// 通常の場合
		else {
			VERIFY (MIDIEvent_SetValue (pMIDIEvent, CLIP (0, lValue + lDeltaValue, 127)));
		}
		break;
	// プログラムチェンジ(0～127)の場合
	case TRACKLISTFRAME_PROGRAMCHANGE:
		// 注意：フォーマット0の最初以外のトラック又はフォーマット1の最初のトラックでは設定してはならない。
		VERIFY (pMIDIEvent = pSekaijuDoc->GetTrackFirstProgramChange (pMIDITrack));
		lValue = MIDIEvent_GetValue (pMIDIEvent);
		// パッチサーチONのとき
		if (pSekaijuApp->m_theGeneralOption.m_bPatchSearch) {
			long lRet;
			MIDIEvent* pCC0Event = pSekaijuDoc->FindBankMSB (pMIDIEvent);
			long lBankMSB = 0;
			if (pCC0Event) {
				lBankMSB = MIDIEvent_GetValue (pCC0Event);
			}
			MIDIEvent* pCC32Event = pSekaijuDoc->FindBankLSB (pMIDIEvent);
			long lBankLSB = 0;
			if (pCC32Event) {
				lBankLSB = MIDIEvent_GetValue (pCC32Event);
			}
			MIDIEvent* pPCEvent = pMIDIEvent;
			long lProgramChange = 0;
			if (pPCEvent) {
				lProgramChange = MIDIEvent_GetNumber (pPCEvent);
			}
			long lTrackOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
			long lTrackViewMode = MIDITrack_GetViewMode (pMIDITrack);
			MIDIInstrumentDefinition* pMIDIInstDef = lTrackViewMode ?
				pSekaijuApp->m_pMIDIInstDefDrum[lTrackOutputPort] :
				pSekaijuApp->m_pMIDIInstDefNorm[lTrackOutputPort];
			// インストゥルメント定義が設定されている場合
			if (pMIDIInstDef) {
				MIDIPatchNameTable* pMIDIPatchNameTable = NULL;
				long lBank = 0;
				TCHAR szBuf[256];
				long lNewValue = lValue + (lDeltaValue > 0 ? 1 : -1);
				long lTempNewValue = lValue; // 最後に音色が見つかった新しい値
				while (0 <= lNewValue && lNewValue <= 127) {
					lBank = (lBankMSB << 7) | lBankLSB;
					// 音色があるか調べる
					pMIDIPatchNameTable = 
					MIDIInstrumentDefinition_GetPatchNameTable (pMIDIInstDef, lBank);				
					lRet = 0;
					if (pMIDIPatchNameTable) {
						lRet = MIDIPatchNameTable_GetName 
							(pMIDIPatchNameTable, lNewValue, szBuf, TSIZEOF (szBuf));
					}
					// 音色があった場合
					if (lRet > 0) {
						lTempNewValue = lNewValue;
						if (lNewValue - lValue >= lDeltaValue && lDeltaValue > 0 ||
							lNewValue - lValue <= lDeltaValue && lDeltaValue < 0) {
							break;
						}
					}
					lNewValue += (lDeltaValue > 0 ? 1 : -1);
				}
				VERIFY (MIDIEvent_SetValue (pMIDIEvent, CLIP (0, lTempNewValue, 127)));
			}
			// インストゥルメント定義が設定されていない場合(通常の場合と同じ処理)
			else {
				VERIFY (MIDIEvent_SetValue (pMIDIEvent, CLIP (0, lValue + lDeltaValue, 127)));
			}
		}
		// 通常の場合
		else {
			VERIFY (MIDIEvent_SetValue (pMIDIEvent, CLIP (0, lValue + lDeltaValue, 127)));
		}
		break;
	// 入力ポート(0～15)の場合
	case TRACKLISTFRAME_INPUTPORT:
		lValue = MIDITrack_GetInputPort (pMIDITrack);
		VERIFY (MIDITrack_SetInputPort (pMIDITrack, CLIP (0, lValue + lDeltaValue, 15)));
		break;
	// 入力チャンネル(0～15)の場合
	case TRACKLISTFRAME_INPUTCHANNEL:
		lValue = MIDITrack_GetInputChannel (pMIDITrack);
		VERIFY (MIDITrack_SetInputChannel (pMIDITrack, CLIP (-1, lValue + lDeltaValue, 15)));
		break;
	// 出力ポート(0～15)の場合
	case TRACKLISTFRAME_OUTPUTPORT:
		lValue = MIDITrack_GetOutputPort (pMIDITrack);
		VERIFY (MIDITrack_SetOutputPort (pMIDITrack, CLIP (0, lValue + lDeltaValue, 15)));
		break;
	// 出力チャンネル(0～15)の場合
	case TRACKLISTFRAME_OUTPUTCHANNEL:
		lValue = MIDITrack_GetOutputChannel (pMIDITrack);
		VERIFY (MIDITrack_SetOutputChannel (pMIDITrack, CLIP (-1, lValue + lDeltaValue, 15)));
		break;
	// タイム+(-127～127)の場合
	case TRACKLISTFRAME_TIMEPLUS:
		lValue = MIDITrack_GetTimePlus (pMIDITrack);
		VERIFY (MIDITrack_SetTimePlus (pMIDITrack, CLIP (-127, lValue + lDeltaValue, 127)));
		break;
	// キー+(-127～127)の場合
	case TRACKLISTFRAME_KEYPLUS:
		lValue = MIDITrack_GetKeyPlus (pMIDITrack);
		VERIFY (MIDITrack_SetKeyPlus (pMIDITrack, CLIP (-127, lValue + lDeltaValue, 127)));
		break;
	// ベロシティ+(-127～127)の場合
	case TRACKLISTFRAME_VELOCITYPLUS:
		lValue = MIDITrack_GetVelocityPlus (pMIDITrack);
		VERIFY (MIDITrack_SetVelocityPlus (pMIDITrack, CLIP (-127, lValue + lDeltaValue, 127)));
		break;
	}

	return TRUE;
}

// セルの値のOn/Offを切り替える。
BOOL CTrackListTrackModeView::ToggleValueOfCurCell () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	CHistoryUnit* pCurHistoryUnit = NULL;
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	long lFormat = MIDIData_GetFormat (pMIDIData);
	long lValue = 0;
	MIDITrack* pTempTrack = NULL;
	MIDITrack* pCloneTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	CString strHistoryName;
	// 設定する項目の値の反映
	long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
	switch (lColumnContent & 0xFFFF) {
	// (未使用項目)
	case TRACKLISTFRAME_VISIBLE:
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_VISIBLE_ON_OFF));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックへのポインタを取得
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// 履歴保持のため現状トラックを差し替え
		ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack));
		// Visible ON/OFF切り替え
		lValue = pSekaijuDoc->GetTrackVisible (pCloneTrack);
		pSekaijuDoc->SetTrackVisible (pCloneTrack, !lValue);
		// 履歴記録
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
		break;
	// (未使用項目)
	case TRACKLISTFRAME_ENABLE:
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_ENABLE_ON_OFF));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックへのポインタを取得
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// 履歴保持のため現状トラックを差し替え
		ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack));
		// Enable ON/OFF切り替え
		lValue = pSekaijuDoc->GetTrackEnable (pCloneTrack);
		pSekaijuDoc->SetTrackEnable (pCloneTrack, !lValue);
		// 履歴記録
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
		break;
	// MIDI入力ON/OFF
	case TRACKLISTFRAME_INPUTON:
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_INPUT_ON_OFF));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックへのポインタを取得
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// 履歴保持のため現状トラックを差し替え
		ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack));
		// 入力ON/OFF切り替え
		lValue = MIDITrack_GetInputOn (pCloneTrack);
		MIDITrack_SetInputOn (pCloneTrack, !lValue);
		// 履歴記録
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
		break;
	// MIDI出力ON/OFF
	case TRACKLISTFRAME_OUTPUTON:
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_OUTPUT_ON_OFF));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックへのポインタを取得
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// 履歴保持のため現状トラックを差し替え
		ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack));
		// 出力ON/OFF切り替え
		lValue = MIDITrack_GetOutputOn (pCloneTrack);
		MIDITrack_SetOutputOn (pCloneTrack, !lValue);
		lValue = MIDITrack_GetOutputOn (pCloneTrack);
		// 履歴記録
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
		// 出力ON→OFFになった場合
		if (lValue == 0) {
			long lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
			pSekaijuApp->m_theCriticalSection.Lock ();
			pSekaijuApp->ResetTempMIDIStatusArray ();
			pSekaijuDoc->TimeMIDIStatus (lCurTime, pSekaijuApp->m_pTempMIDIStatus);
			long lFlags;
			// 再生中ならばノートのみ復元
			if (pSekaijuApp->m_bPlaying) {
				lFlags = SDS_NOTE;
				pSekaijuApp->m_bIgnoreNoteEvent = 1;
			}
			// 停止中ならば何も復元しない 
			else {
				lFlags = 0;
				pSekaijuApp->m_bIgnoreNoteEvent = 0;
			}
			pSekaijuApp->SendDifferentStatus (lFlags);
			pSekaijuApp->m_theCriticalSection.Unlock ();
		}
		// 出力OFF→ONになった場合
		else {
			long lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
			pSekaijuApp->m_theCriticalSection.Lock ();
			pSekaijuApp->ResetTempMIDIStatusArray ();
			pSekaijuDoc->TimeMIDIStatus (lCurTime, pSekaijuApp->m_pTempMIDIStatus);
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
		}
		break;
	// 表示モード(通常/ドラム)
	case TRACKLISTFRAME_VIEWMODE:
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_VIEWMODE));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックへのポインタを取得
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// 履歴保持のため現状トラックを差し替え
		ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack));
		// 通常/ドラム切り替え
		lValue = MIDITrack_GetViewMode (pCloneTrack);
		MIDITrack_SetViewMode (pCloneTrack, !lValue);
		// 履歴記録
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
		break;	
	}
	return TRUE;
}

// 色の選択
BOOL CTrackListTrackModeView::DoColorEditing () {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	long lFormat = MIDIData_GetFormat (pMIDIData);
	MIDITrack* pTempTrack = NULL;
	MIDITrack* pCloneTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	// 履歴の用意
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_MODIFY_COLOR));
	VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
	// 現在のトラックへのポインタを取得
	pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
	pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
	if (pTempTrack == NULL) {
		return FALSE;
	}
	// 色設定ダイアログの表示
	long lColor = 0;
	long lRet = 0;
	CColorDialog theDlg;
	ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
	long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
	switch (lColumnContent & 0xFFFF) {
	// 前景色
	case TRACKLISTFRAME_FORECOLOR:
		lColor = MIDITrack_GetForeColor (pTempTrack);
		theDlg.m_cc.rgbResult = lColor;
		theDlg.m_cc.Flags |= (CC_FULLOPEN | CC_RGBINIT);
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		lRet = theDlg.DoModal ();
		pSekaijuDoc->m_theCriticalSection.Lock ();
		if (lRet == IDOK) {
			lColor = (theDlg.GetColor () & 0x00FFFFFF);
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
			VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack));
			MIDITrack_SetForeColor (pCloneTrack, lColor);
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
			pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIEVENTCHANGED);
			return TRUE;
		}
		break;
	// 背景色(未使用)
	case TRACKLISTFRAME_BACKCOLOR:
		break;
	}
	return FALSE;
}

// セルの文字列を取得
CString CTrackListTrackModeView::GetCellString (long lRow, long lColumn) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	ASSERT (0 <= lRow && lRow < MAXMIDITRACKNUM);
	ASSERT (0 <= lColumn && lColumn < TRACKLISTFRAME_NUMCOLUMN);
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lRow);
	MIDIEvent* pMIDIEvent = NULL;
	CString strText;
	long lFormat = MIDIData_GetFormat (pMIDIData);
	long lValue = 0;
	long lNumber = 0;
	MIDIIn* pMIDIIn = NULL;
	MIDIOut* pMIDIOut = NULL;
	CString strNone;
	VERIFY (strNone.LoadString (IDS_NONE));
	if (pMIDITrack) {
		long lColumnContent = pTrackListFrame->GetColumnContent (lColumn);
		TCHAR szBuf2[2048];
		memset (szBuf2, 0, sizeof (szBuf2));
		switch (lColumnContent & 0xFFFF) {
		// トラック名
		case TRACKLISTFRAME_TRACKNAME:
			strText = pSekaijuDoc->GetTrackName (pMIDITrack);
			codestr2str ((LPTSTR)(LPCTSTR)strText, strText.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
			strText = szBuf2;
			break;
		// 可視(未使用)
		case TRACKLISTFRAME_VISIBLE:
			if (pSekaijuDoc->GetTrackVisible (pMIDITrack)) {
				VERIFY (strText.LoadString (IDS_SHOW));
			}
			else {
				VERIFY (strText.LoadString (IDS_HIDE));	
			}
			break;
		// イネーブル(未使用)
		case TRACKLISTFRAME_ENABLE:
			if (pSekaijuDoc->GetTrackEnable (pMIDITrack)) {
				VERIFY (strText.LoadString (IDS_ENABLE));
			}
			else {
				VERIFY (strText.LoadString (IDS_LOCK));	
			}
			break;
		// 入力ON
		case TRACKLISTFRAME_INPUTON:
			if (MIDITrack_GetInputOn (pMIDITrack)) {
				VERIFY (strText.LoadString (IDS_ON));
			}
			else {
				VERIFY (strText.LoadString (IDS_OFF));	
			}
			break;
		// 出力ON
		case TRACKLISTFRAME_OUTPUTON:
			if (MIDITrack_GetOutputOn (pMIDITrack)) {			
				VERIFY (strText.LoadString (IDS_ON));
			}
			else {
				VERIFY (strText.LoadString (IDS_OFF));	
			}
			break;
		// 入力ポート番号
		case TRACKLISTFRAME_INPUTPORT:
			lValue = MIDITrack_GetInputPort (pMIDITrack);
			ASSERT (0 <= lValue && lValue < MAXMIDIINDEVICENUM);
			pSekaijuApp->m_theCriticalSection.Lock ();
			pMIDIIn = pSekaijuApp->m_pMIDIIn[lValue];
			if (pMIDIIn) {
				strText.Format (_T("%3d-%s"), lValue + 1, pMIDIIn->m_pDeviceName);
			}
			else {
				strText.Format (_T("%3d-%s"), lValue + 1, strNone);
			}
			pSekaijuApp->m_theCriticalSection.Unlock ();
			break;
		// 入力チャンネル
		case TRACKLISTFRAME_INPUTCHANNEL:
			lValue = MIDITrack_GetInputChannel (pMIDITrack);
			if (0 <= lValue && lValue <= 15) {
				strText.Format (_T("%3d"), lValue + 1);
			}
			else {
				VERIFY (strText.LoadString (IDS_N_A));
			}
			break;
		// 出力ポート番号
		case TRACKLISTFRAME_OUTPUTPORT:
			lValue = MIDITrack_GetOutputPort (pMIDITrack);
			ASSERT (0 <= lValue && lValue < MAXMIDIOUTDEVICENUM);
			pSekaijuApp->m_theCriticalSection.Lock ();
			pMIDIOut = pSekaijuApp->m_pMIDIOut[lValue];
			if (pMIDIOut) {
				strText.Format (_T("%3d-%s"), lValue + 1, pMIDIOut->m_pDeviceName);
			}
			else {
				strText.Format (_T("%3d-%s"), lValue + 1, strNone);
			}
			pSekaijuApp->m_theCriticalSection.Unlock ();
			break;
		// 出力チャンネル
		case TRACKLISTFRAME_OUTPUTCHANNEL:
			lValue = MIDITrack_GetOutputChannel (pMIDITrack);
			if (0 <= lValue && lValue <= 15) {
				strText.Format (_T("%3d"), lValue + 1);
			}
			else {
				VERIFY (strText.LoadString (IDS_N_A));
			}
			break;
		// 表示モード(通常／ドラム)
		case TRACKLISTFRAME_VIEWMODE:
			if (MIDITrack_GetViewMode (pMIDITrack)) {
				VERIFY (strText.LoadString (IDS_DRUM));
			}
			else {
				VERIFY (strText.LoadString (IDS_NORM));
			}
			break;
		// コントロールチェンジ
		case TRACKLISTFRAME_CONTROLCHANGE:
			lValue = -1;
			lNumber = pTrackListFrame->GetColumnContent (lColumn) >> 16;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsControlChange (pMIDIEvent)) {
					if (MIDIEvent_GetNumber (pMIDIEvent) == lNumber) {
						lValue = MIDIEvent_GetValue (pMIDIEvent);
						break;
					}
				}
			}
			if (0 <= lValue && lValue <= 127) {
				strText.Format (_T("%3d"), lValue);
			}
			else {
				strText = _T("---");
			}
			break;
		// プログラムチェンジ
		case TRACKLISTFRAME_PROGRAMCHANGE:
			lValue = -1;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsProgramChange (pMIDIEvent)) {
					lValue = MIDIEvent_GetNumber (pMIDIEvent);
					break;
				}
			}
			if (0 <= lValue && lValue <= 127 && pMIDIEvent) {
				long lTrackOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
				ASSERT (0 <= lTrackOutputPort && lTrackOutputPort < 16);
				long lTrackOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
				ASSERT (-1 <= lTrackOutputChannel && lTrackOutputChannel < 16);
				if (lTrackOutputChannel == -1) {
					lTrackOutputChannel = MIDIEvent_GetChannel (pMIDIEvent);
				}
				ASSERT (0 <= lTrackOutputChannel && lTrackOutputChannel < 16);
				long lTrackViewMode  = MIDITrack_GetViewMode (pMIDITrack);
				TCHAR szBuf[256];
				memset (szBuf, 0, sizeof (szBuf));
				long lBankMSB = MIDIEvent_GetBankMSB (pMIDIEvent);
				long lBankLSB = MIDIEvent_GetBankLSB (pMIDIEvent);
				long lBank = (lBankMSB << 7) | lBankLSB; //MIDIEvent_GetBank (pMIDIEvent);
				MIDIInstrumentDefinition* pInstDef = NULL;
				// このトラックの表示モードが「通常」の場合
				if (lTrackViewMode == 0) {
					pInstDef = pSekaijuApp->m_pMIDIInstDefNorm[lTrackOutputPort];
				}
				// このトラックの表示モードが「ドラム」の場合
				else {
					pInstDef = pSekaijuApp->m_pMIDIInstDefDrum[lTrackOutputPort];
				}
				// このトラックのインストゥルメント定義が見つかった
				if (pInstDef) {
					MIDIPatchNameTable* pPatchNameTable =
						MIDIInstrumentDefinition_GetPatchNameTable (pInstDef, lBank);
					// このインストゥルメント定義の指定バンクのPatchNameTableが見つかった
					if (pPatchNameTable) {
						TCHAR szBuf[256];
						memset (szBuf, 0, 256);
						MIDIPatchNameTable_GetName 
							(pPatchNameTable, lValue, szBuf, 255);
						strText.Format (_T("%d-%s"), lValue, szBuf);
					}
					else {
						strText.Format (_T("%d"), lValue);
					}
				}
				else {
					strText.Format (_T("%d"), lValue);
				}
			}
			else {
				strText = _T("---");
			}
			break;
		// タイム+
		case TRACKLISTFRAME_TIMEPLUS:
			if (lFormat == 1 && lRow == 0) {
				strText.Format (_T("---"));
			}
			else {
				lValue = MIDITrack_GetTimePlus (pMIDITrack);
				strText.Format (_T("%3d"), lValue);
			}
			break;
		// キー+
		case TRACKLISTFRAME_KEYPLUS:
			if (lFormat == 1 && lRow == 0) {
				strText.Format (_T("---"));
			}
			else {
				lValue = MIDITrack_GetKeyPlus (pMIDITrack);
				strText.Format (_T("%3d"), lValue);
			}
			break;
		// ベロシティ+
		case TRACKLISTFRAME_VELOCITYPLUS:
			if (lFormat == 1 && lRow == 0) {
				strText.Format (_T("---"));
			}
			else {
				lValue = MIDITrack_GetVelocityPlus (pMIDITrack);
				strText.Format (_T("%3d"), lValue);
			}
			break;
		// イベント数
		case TRACKLISTFRAME_NUMEVENT:
			lValue = MIDITrack_CountEvent (pMIDITrack);
			strText.Format (_T("%8d"), lValue);
			break;
		}
	}
	return strText;
}

// セルの文字列を設定
BOOL CTrackListTrackModeView::SetCellString (long lRow, long lColumn, CString strText) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	m_bSettingCellString = 1;
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pTempTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	MIDITrack* pCloneTrack = NULL;
	MIDIEvent* pCloneEvent = NULL;
	long lFormat = MIDIData_GetFormat (pMIDIData);
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	long lValue = 0;
	long lNumber = 0;
	MIDIIn* pMIDIIn = NULL;
	MIDIOut* pMIDIOut = NULL;
	ASSERT (0 <= lColumn && lColumn < TRACKLISTFRAME_NUMCOLUMN);
	ASSERT (0 <= lRow && lRow < MAXMIDITRACKNUM);
	long lColumnContent = pTrackListFrame->GetColumnContent (lColumn);
	switch (lColumnContent & 0xFFFF) {
	// トラック名
	case TRACKLISTFRAME_TRACKNAME:
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_TRACKNAME));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックへのポインタを取得
		pSekaijuDoc->AddTrack (lRow + 1, 0x000F, pCurHistoryUnit);
		pMIDITrack = pSekaijuDoc->GetTrack (lRow);
		if (pMIDITrack == NULL) {
			return FALSE;
		}
		// 最初のトラック名イベントを探索
		forEachEvent (pMIDITrack, pTempEvent) {
			if (MIDIEvent_GetKind (pTempEvent) == MIDIEVENT_TRACKNAME) {
				break;
			}
		}
		// 最初のトラック名イベントが見つからなかった場合
		if (pTempEvent == NULL) {
			TCHAR szBuf2[2048];
			memset (szBuf2, 0, sizeof (szBuf2));
			str2codestr ((LPTSTR)(LPCTSTR)(strText), strText.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
			VERIFY (pTempEvent = MIDIEvent_CreateTrackName (0, szBuf2));
			VERIFY (MIDITrack_InsertEvent (pMIDITrack, pTempEvent));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
		}
		// 最初のトラック名が見つかった場合
		else {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent));
			TCHAR szBuf2[2048];
			memset (szBuf2, 0, sizeof (szBuf2));
			str2codestr ((LPTSTR)(LPCTSTR)(strText), strText.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
			MIDIEvent_SetText (pCloneEvent, szBuf2);
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
		}
		break;
	// 入力ポート番号
	case TRACKLISTFRAME_INPUTPORT:
		strText.TrimLeft ();
		if (IsNumeric (strText) == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_1_TO_16));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		lValue = _ttol (strText);
		if (lValue <= 0 || lValue > 16) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_1_TO_16));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		pSekaijuApp->m_theCriticalSection.Lock ();
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_INPUTPORT));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックへのポインタを取得
		pSekaijuDoc->AddTrack (lRow + 1, 0x000F, pCurHistoryUnit);
		pMIDITrack = pSekaijuDoc->GetTrack (lRow);
		if (pMIDITrack == NULL) {
			return FALSE;
		}
		// 入力ポート番号の設定
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack);
		pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pMIDITrack);
		MIDITrack_SetInputPort (pCloneTrack, CLIP (0, lValue - 1, 15));
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack);
		pSekaijuApp->m_theCriticalSection.Unlock ();
		m_bSettingCellString = 0;
		return TRUE;
	// 入力チャンネル
	case TRACKLISTFRAME_INPUTCHANNEL:
		strText.TrimLeft ();
		if (strText == _T("n/a")) {
			strText = _T("0");
		}
		if (IsNumeric (strText) == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_0_TO_16));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		lValue = _ttol (strText);
		if (lValue <= -1 || lValue > 16) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_0_TO_16));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		pSekaijuApp->m_theCriticalSection.Lock ();
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_INPUTCHANNEL));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックへのポインタを取得
		pSekaijuDoc->AddTrack (lRow + 1, 0x000F, pCurHistoryUnit);
		pMIDITrack = pSekaijuDoc->GetTrack (lRow);
		if (pMIDITrack == NULL) {
			return FALSE;
		}
		// 入力チャンネルの設定
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack);
		pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pMIDITrack);
		MIDITrack_SetInputChannel (pCloneTrack, CLIP (-1, lValue - 1, 15));
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack);
		pSekaijuApp->m_theCriticalSection.Unlock ();
		m_bSettingCellString = 0;
		return TRUE;
	// 出力ポート番号
	case TRACKLISTFRAME_OUTPUTPORT:
		strText.TrimLeft ();
		if (IsNumeric (strText) == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_1_TO_16));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		lValue = _ttol (strText);
		if (lValue <= 0 || lValue > 16) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_1_TO_16));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		pSekaijuApp->m_theCriticalSection.Lock ();
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_OUTPUTPORT));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックへのポインタを取得
		pSekaijuDoc->AddTrack (lRow + 1, 0x000F, pCurHistoryUnit);
		pMIDITrack = pSekaijuDoc->GetTrack (lRow);
		if (pMIDITrack == NULL) {
			return FALSE;
		}
		// 出力ポート番号の設定
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack);
		pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pMIDITrack);
		MIDITrack_SetOutputPort (pCloneTrack, CLIP (0, lValue - 1, 15));
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack);
		pSekaijuApp->m_theCriticalSection.Unlock ();
		m_bSettingCellString = 0;
		return TRUE;
	// 出力チャンネル
	case TRACKLISTFRAME_OUTPUTCHANNEL:
		strText.TrimLeft ();
		if (strText == _T("n/a")) {
			strText = _T("0");
		}
		if (IsNumeric (strText) == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_0_TO_16));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		lValue = _ttol (strText);
		if (lValue <= -1 || lValue > 16) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_0_TO_16));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		pSekaijuApp->m_theCriticalSection.Lock ();
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_OUTPUTCHANNEL));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックへのポインタを取得
		pSekaijuDoc->AddTrack (lRow + 1, 0x000F, pCurHistoryUnit);
		pMIDITrack = pSekaijuDoc->GetTrack (lRow);
		if (pMIDITrack == NULL) {
			return FALSE;
		}
		// 出力チャンネルの設定
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack);
		pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pMIDITrack);
		MIDITrack_SetOutputChannel (pCloneTrack, CLIP (-1, lValue - 1, 15));
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack);
		pSekaijuApp->m_theCriticalSection.Unlock ();
		m_bSettingCellString = 0;
		return TRUE;
	// コントロールチェンジ(CC#0, CC#32, ボリューム, パン, リバーブ, コーラス, ディレイなど)
	case TRACKLISTFRAME_CONTROLCHANGE:
		if (lFormat == 1 && lRow == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_CONTROLCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		lNumber = pTrackListFrame->GetColumnContent (lColumn) >> 16;
		strText.TrimLeft ();
		if (IsNumeric (strText) == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_0_TO_127));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		lValue = _ttol (strText);
		if (lValue < 0 || lValue > 127) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_0_TO_127));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_CONTROLCHANGE));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックへのポインタを取得
		pSekaijuDoc->AddTrack (lRow + 1, 0x000F, pCurHistoryUnit);
		pMIDITrack = pSekaijuDoc->GetTrack (lRow);
		if (pMIDITrack == NULL) {
			return FALSE;
		}
		// 最初のCC#lNumberイベントを探索
		forEachEvent (pMIDITrack, pTempEvent) {
			if (MIDIEvent_IsControlChange (pTempEvent)) {
				if (MIDIEvent_GetNumber (pTempEvent) == lNumber) {
					break;
				}
			}
		}
		// 最初のCC#lNumberイベントが見つからなかった場合
		if (pTempEvent == NULL) {
			long lChannel = MIDITrack_GetOutputChannel (pMIDITrack);
			VERIFY (pTempEvent = MIDIEvent_CreateControlChange (0, lChannel, lNumber, lValue));
			VERIFY (MIDITrack_InsertEvent (pMIDITrack, pTempEvent));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
		}
		// 最初のCC#lNumberが見つかった場合
		else {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent));
			VERIFY (MIDIEvent_SetValue (pCloneEvent, lValue));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
		}
		m_bSettingCellString = 0;
		return TRUE;
	// プログラムチェンジ
	case TRACKLISTFRAME_PROGRAMCHANGE:
		if (lFormat == 1 && lRow == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_PROGRAMCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		strText.TrimLeft ();
		if (IsNumeric (strText) == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_0_TO_127));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		lValue = _ttol (strText);
		if (lValue < 0 || lValue > 127) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_0_TO_127));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_PROGRAMCHANGE));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックへのポインタを取得
		pMIDITrack = pSekaijuDoc->GetTrack (lRow);
		// 操作対象のトラックがない場合は、必要な数だけトラックを追加する。
		pSekaijuDoc->AddTrack (lRow + 1, 0x000F, pCurHistoryUnit);
		pMIDITrack = pSekaijuDoc->GetTrack (lRow);
		if (pMIDITrack == NULL) {
			return FALSE;
		}
		// 最初のプログラムチェンジイベントを探索
		forEachEvent (pMIDITrack, pTempEvent) {
			if (MIDIEvent_IsProgramChange (pTempEvent)) {
				break;
			}
		}
		// 最初のプログラムチェンジイベントが見つからなかった場合
		if (pTempEvent == NULL) {
			long lChannel = MIDITrack_GetOutputChannel (pMIDITrack);
			VERIFY (pTempEvent = MIDIEvent_CreateProgramChange (0, lChannel, lValue));
			VERIFY (MIDITrack_InsertEvent (pMIDITrack, pTempEvent));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
		}
		// 最初のプログラムチェンジイベントが見つかった場合
		else {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent));
			VERIFY (MIDIEvent_SetValue (pCloneEvent, lValue));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
		}
		m_bSettingCellString = 0;
		return TRUE;

	// タイム+
	case TRACKLISTFRAME_TIMEPLUS:
		strText.TrimLeft ();
		if (IsNumeric (strText) == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_M127_TO_127));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		lValue = _ttol (strText);
		if (lValue < -127 || lValue > 127) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_M127_TO_127));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_TIMEPLUS));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックへのポインタを取得
		pSekaijuDoc->AddTrack (lRow + 1, 0x000F, pCurHistoryUnit);
		pMIDITrack = pSekaijuDoc->GetTrack (lRow);
		if (pMIDITrack == NULL) {
			return FALSE;
		}
		// タイム+の設定
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack);
		pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pMIDITrack);
		MIDITrack_SetTimePlus (pCloneTrack, CLIP (-127, lValue, 127));
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack);
		m_bSettingCellString = 0;
		return TRUE;
	// キー+
	case TRACKLISTFRAME_KEYPLUS:
		strText.TrimLeft ();
		if (IsNumeric (strText) == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_M127_TO_127));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		lValue = _ttol (strText);
		if (lValue < -127 || lValue > 127) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_M127_TO_127));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_KEYPLUS));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックへのポインタを取得
		pSekaijuDoc->AddTrack (lRow + 1, 0x000F, pCurHistoryUnit);
		pMIDITrack = pSekaijuDoc->GetTrack (lRow);
		if (pMIDITrack == NULL) {
			return FALSE;
		}
		// キー+の設定
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack);
		pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pMIDITrack);
		MIDITrack_SetKeyPlus (pCloneTrack, CLIP (-127, lValue, 127));
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack);
		m_bSettingCellString = 0;
		return TRUE;
	// ベロシティ+
	case TRACKLISTFRAME_VELOCITYPLUS:
		strText.TrimLeft ();
		if (IsNumeric (strText) == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_M127_TO_127));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		lValue = _ttol (strText);
		if (lValue < -127 || lValue > 127) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_M127_TO_127));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		// 新しい履歴の用意
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_VELPLUS));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 現在のトラックへのポインタを取得
		pSekaijuDoc->AddTrack (lRow + 1, 0x000F, pCurHistoryUnit);
		pMIDITrack = pSekaijuDoc->GetTrack (lRow);
		if (pMIDITrack == NULL) {
			return FALSE;
		}
		// ベロシティ+の設定
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack);
		pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pMIDITrack);
		MIDITrack_SetVelocityPlus (pCloneTrack, CLIP (-127, lValue, 127));
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack);
		m_bSettingCellString = 0;
		return TRUE;
	// イベント数
	case TRACKLISTFRAME_NUMEVENT:
		return FALSE;
	}
	m_bSettingCellString = 0;
	return TRUE;
}

// 現在のセルの長方形を描画
BOOL CTrackListTrackModeView::DrawCurFrame (CDC* pDC) {
	CRect rcCurFrame = GetRectFromCell (m_lCurRow, m_lCurColumn);
	rcCurFrame.left -= 1;
	rcCurFrame.right -= 1;
	rcCurFrame.top -= 1;
	rcCurFrame.bottom -= 1;
	CPen thePen (PS_SOLID, 3, RGB (0, 0, 0));
	CPen* pOldPen = pDC->SelectObject (&thePen);
	int nOldDrawMode = pDC->SetROP2 (R2_NOT);
	pDC->MoveTo (rcCurFrame.left, rcCurFrame.top);
	pDC->LineTo (rcCurFrame.left, rcCurFrame.bottom);
	pDC->LineTo (rcCurFrame.right, rcCurFrame.bottom);
	pDC->LineTo (rcCurFrame.right, rcCurFrame.top);
	pDC->LineTo (rcCurFrame.left, rcCurFrame.top);
	pDC->SetROP2 (nOldDrawMode);
	pDC->SelectObject (pOldPen);
	return TRUE;

}

// セルの長方形を取得
CRect CTrackListTrackModeView::GetRectFromCell (long lRow, long lColumn) {
	long j;
	CRect rcCell (0, 0, 0 ,0);
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParentFrame ();
	for (j = 0; j < lColumn; j++) {
		rcCell.left += pTrackListFrame->GetColumnWidth (j);
	}
	rcCell.right = rcCell.left + pTrackListFrame->GetColumnWidth (j);
	rcCell.top = lRow * pTrackListFrame->GetRowZoom ();
	rcCell.bottom = (lRow + 1) * pTrackListFrame->GetRowZoom ();
	return rcCell;
}

// テキストボックスの移動(VISIBLE==FALSE時含む)
BOOL CTrackListTrackModeView::MoveTextBox (long lRow, long lColumn) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
	ASSERT (0 <= lRow && lRow < MAXMIDITRACKNUM);
	ASSERT (0 <= lColumn && lColumn < TRACKLISTFRAME_NUMCOLUMN);
	CRect rcNewCell = GetRectFromCell (lRow, lColumn);
	rcNewCell -= CSize (pTrackListFrame->GetColumnScrollPos(), pTrackListFrame->GetRowScrollPos ());
	long lButtonWidth = 12;//::GetSystemMetrics (SM_CXVSCROLL);
	ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
	long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
	if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
	}
	else {
		lButtonWidth = 1;
	}
	m_theTextBox.MoveWindow 
		(rcNewCell.left, rcNewCell.top + pTrackListFrame->GetRowZoom () / 2 - 6, 
		rcNewCell.Width () - lButtonWidth, 13);
	return TRUE;
}




//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// 原点の移動をオーバーライド
void CTrackListTrackModeView::OnPrepareDC (CDC* pDC, CPrintInfo* pInfo) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
	pDC->SetWindowOrg (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
}

// 描画
void CTrackListTrackModeView::OnDraw (CDC* pDC) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos());
	CFont* pOldFont = pDC->SelectObject (&(pTrackListFrame->m_theFont));

	long lButtonWidth = 12;//::GetSystemMetrics (SM_CXVSCROLL);
	long lColorBlack = RGB (0, 0, 0);
	long lColorWhite = RGB (255, 255, 255);
	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);
	long lColorGrayText = ::GetSysColor (COLOR_GRAYTEXT);

	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;

	long i, j;
	long lRowZoom = pTrackListFrame->GetRowZoom ();
	long lColumnZoom = pTrackListFrame->GetColumnZoom ();
	// 行の背景色描画
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	long lVisibleTopRow = pTrackListFrame->GetVisibleTopRow ();
	long lVisibleBottomRow = pTrackListFrame->GetVisibleBottomRow ();
	for (i = lVisibleTopRow; i <= lVisibleBottomRow; i++) {
		long lBackColor = (i % 2 ? RGB(255,255,192) : RGB (255,255,255));
		if (0 <= i && i < lTrackCount) {
			pMIDITrack = pSekaijuDoc->GetTrack (i);
		}
		else {
			pMIDITrack = NULL;
		}
		if (pMIDITrack) {
			if (pSekaijuDoc->IsTrackSelected (pMIDITrack)) {
				lBackColor = (i % 2 ? RGB(224,224,192) : RGB (224,224,255));
			}
		}
		pDC->FillSolidRect 
			(rcClient.left, i * lRowZoom, 
			rcClient.Width (), (i + 1) * lRowZoom, 
			lBackColor);
	}
	// 横線の描画
	CPen penTrack (PS_SOLID, 1, RGB (128, 128, 128));
	CPen* pOldPen = pDC->SelectObject (&penTrack);
	for (i = lVisibleTopRow; i <= lVisibleBottomRow; i++) {
		long y = i * lRowZoom;
		pDC->MoveTo (rcClient.left, y - 1);
		pDC->LineTo (rcClient.right, y - 1);
	}
	pDC->SelectObject (pOldPen);
	// 縦線の描画
	CPen penColumn (PS_SOLID, 1, RGB (128, 128, 128));
	pOldPen = pDC->SelectObject (&penColumn);
	long x = 0;
	for (j = 0; j < TRACKLISTFRAME_NUMCOLUMN; j++) {
		pDC->MoveTo (x - 1, rcClient.top);
		pDC->LineTo (x - 1, rcClient.bottom);
		x += pTrackListFrame->GetColumnBaseWidth (j) * lColumnZoom;
	}
	pDC->SelectObject (pOldPen);


	// 文字の描画
	CRect rcText (0, 0, 0, 0);
	pDC->SetBkMode (TRANSPARENT);
	i = 0;
	j = 0;
	// 描画すべき開始行と終了行の計算
	long lStartRow = __max (lVisibleTopRow, 0);
	long lEndRow = __min (MAXMIDITRACKNUM, lVisibleBottomRow + 1);
	// 各セルの文字などの描画
	for (i = lStartRow; i < lEndRow; i++) {
		if (i < lTrackCount) {
			pMIDITrack = pSekaijuDoc->GetTrack (i);
		}
		else {
			pMIDITrack = NULL;
		}
		if (pMIDITrack) {
			long lTrackForeColor = MIDITrack_GetForeColor (pMIDITrack);
			pDC->SetTextColor (lTrackForeColor);
			// 各列について
			for (j = 0; j < TRACKLISTFRAME_NUMCOLUMN; j++) {
				CRect rcCell = GetRectFromCell (i, j);
				CRect rcText = rcCell;
				long lColumnContent = pTrackListFrame->GetColumnContent (j);
				if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
					rcText.right -= 10;
				}
				// 各セルの文字描画
				CString strCellString;
				CRect rcColor;
				switch (lColumnContent & 0xFFFF) {
				case TRACKLISTFRAME_FORECOLOR:
					rcColor.left = rcCell.left + 2;
					rcColor.right = rcCell.right - 3;
					rcColor.top = rcCell.top + lRowZoom / 2 - 6;
					rcColor.bottom = rcCell.top + lRowZoom / 2 + 6;
					pDC->FillSolidRect (&rcColor, lTrackForeColor); 
					break;
				case TRACKLISTFRAME_TRACKNAME:
				case TRACKLISTFRAME_VISIBLE:
				case TRACKLISTFRAME_ENABLE:
				case TRACKLISTFRAME_INPUTON:
				case TRACKLISTFRAME_INPUTPORT:
				case TRACKLISTFRAME_INPUTCHANNEL:
				case TRACKLISTFRAME_OUTPUTON:
				case TRACKLISTFRAME_OUTPUTPORT:
				case TRACKLISTFRAME_OUTPUTCHANNEL:
				case TRACKLISTFRAME_VIEWMODE:
				case TRACKLISTFRAME_CONTROLCHANGE:
				case TRACKLISTFRAME_PROGRAMCHANGE:
				case TRACKLISTFRAME_TIMEPLUS:
				case TRACKLISTFRAME_KEYPLUS:
				case TRACKLISTFRAME_VELOCITYPLUS:
				case TRACKLISTFRAME_NUMEVENT:
					strCellString = GetCellString (i, j);
					pDC->DrawText (strCellString, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
					break;
				}
			}
		}
	}	

	// 各セルのボタン類の描画
	CPen penColorBtnText (PS_SOLID, 1, lColorBtnText);
	CPen penColorGrayText (PS_SOLID, 1, lColorGrayText);
	CBrush brsColorBtnText;
	CBrush brsColorGrayText;
	brsColorBtnText.CreateSolidBrush (lColorBtnText);
	brsColorGrayText.CreateSolidBrush (lColorGrayText);
	CBrush* pOldBrush = NULL;

	CRect rcButton;
	CRect rcCell;
	POINT pt[3];
	pOldPen = pDC->SelectObject (&penColorBtnText);
	pOldBrush = pDC->SelectObject (&brsColorBtnText);
	
	for (i = lVisibleTopRow; i <= lVisibleBottomRow; i++) {
		for (j = 0; j < TRACKLISTFRAME_NUMCOLUMN; j++) {
			long lColumnContent = pTrackListFrame->GetColumnContent (j);
			if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT ||
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL ||
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT ||
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL ||
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE ||
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE ||
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS ||
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS ||
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
				rcCell = GetRectFromCell (i, j);
				rcButton = rcCell;
				// 上ボタン
				rcButton.left = rcCell.right - lButtonWidth;
				rcButton.bottom = rcCell.top + lRowZoom / 2;
				pDC->FillSolidRect (&rcButton, ::GetSysColor (COLOR_3DFACE));
				if (i == m_lCurRow && j == m_lCurColumn && (m_lCurButtonState & 0x01) == 0x01) {
					pDC->Draw3dRect (&rcButton, lColorBtnShadow, lColorBtnHighlight);
				}
				else {
					pDC->Draw3dRect (&rcButton, lColorBtnHighlight, lColorBtnShadow);
				}
				pt[0].x = rcCell.right - lButtonWidth / 2;
				pt[0].y = rcCell.top + lRowZoom * 1 / 4 - 1;
				pt[1].x = pt[0].x - 2;
				pt[1].y = pt[0].y + 2;
				pt[2].x = pt[0].x + 2;
				pt[2].y = pt[0].y + 2;
				pDC->Polygon (pt, 3);
				// 下ボタン
				rcButton.top = rcCell.top + lRowZoom / 2;
				rcButton.bottom = rcCell.bottom;
				pDC->FillSolidRect (&rcButton, ::GetSysColor (COLOR_3DFACE));
				if (i == m_lCurRow && j == m_lCurColumn && (m_lCurButtonState & 0x02) == 0x02) {
					pDC->Draw3dRect (&rcButton, lColorBtnShadow, lColorBtnHighlight);
				}
				else {
					pDC->Draw3dRect (&rcButton, lColorBtnHighlight, lColorBtnShadow);
				}
				pt[0].x = rcCell.right - lButtonWidth / 2;
				pt[0].y = rcCell.top + lRowZoom * 3 / 4 + 1;
				pt[1].x = pt[0].x - 2;
				pt[1].y = pt[0].y - 2;
				pt[2].x = pt[0].x + 2;
				pt[2].y = pt[0].y - 2;
				pDC->Polygon (pt, 3);
			}
		}
	}
	pDC->SelectObject (pOldPen);
	pDC->SelectObject (pOldBrush);

	// カレントセルの枠描画
	DrawCurFrame (pDC);

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// コマンドルーティングのオーバーライド
BOOL CTrackListTrackModeView::OnCmdMsg (UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
	// 注意:pHandlerInfo==NULLの時はコマンドを実行し、NULLでないときは
	// コマンドは実行せずpHandlerInfoの中身を設定することを意味する。
	if (nCode == CN_COMMAND && pHandlerInfo == NULL) {
	}
	return CView::OnCmdMsg (nID, nCode, pExtra, pHandlerInfo);
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// ウィンドウ生成時
BOOL CTrackListTrackModeView::OnCreate (LPCREATESTRUCT lpcs) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParentFrame ();
	ASSERT (pTrackListFrame);
	m_theTextBox.Create (WS_CHILD /*| WS_VISIBLE | WS_BORDER*/ | ES_AUTOHSCROLL, 
		CRect (0, pTrackListFrame->GetRowZoom () / 2 - 6, 
		pTrackListFrame->GetColumnWidth (0), 13), this, IDC_TEXTBOX);
	m_theTextBox.SetFont (&(pTrackListFrame->m_theFont));
	return CSekaijuView::OnCreate (lpcs);
	
}

// フォーカスを失ったとき
void CTrackListTrackModeView::OnKillFocus (CWnd* pNewWnd) {
	_RPTF1 (_CRT_WARN, "CTrackListTrackModeView::OnKillFocus (pNewWnd=0x%08x)\n", (long)pNewWnd);
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	// インプレーステキストボックスにフォーカスが移った場合を除き
	if (pNewWnd != &m_theTextBox && !m_bSettingCellString) {
		// テキスト編集中の場合はその内容を確定
		if (IsTextEditing ()) {
			EndTextEditingOK ();
		}
		// マウスで操作中の場合はその操作を停止
		if (m_lCurButtonState & 0x00F0) {
			ReleaseCapture ();
			KillTimer (1);
			EndValueUpDown ();
			m_lCurButtonState &= 0xFF00;
			pSekaijuDoc->SetModifiedFlag (TRUE);
			pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		}
		// キーボードで操作中の場合はその操作を停止
		if (m_lCurButtonState & 0x0F00) {
			ReleaseCapture ();
			EndValueUpDown ();
			m_lCurButtonState &= 0xF0F0;
			pSekaijuDoc->SetModifiedFlag (TRUE);
			pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		}
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CView::OnKillFocus (pNewWnd);
}

// キー押し下げ時
void CTrackListTrackModeView::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {

	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos());
	
	// 古いセルの再描画(20100128:ちらつき防止のため条件式追加)
	if (IsTextEditing () == FALSE && nChar != VK_CONTROL && nChar != VK_SHIFT && nChar != VK_MENU) {
		CRect rcOldCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
		rcOldCell -= CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos());
		rcOldCell.InflateRect (2, 2);
		InvalidateRect (&rcOldCell);
	}

	ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
	long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
	long lFormat = MIDIData_GetFormat (pMIDIData);

	switch (nChar) {
	// 上キー
	case VK_UP:
		// テキスト編集中の場合はその内容を確定
		if (IsTextEditing ()) {
			EndTextEditingOK ();
		}
		// マウスで操作中の場合はその操作を停止
		if (m_lCurButtonState & 0x00F0) {
			ReleaseCapture ();
			KillTimer (1);
			EndValueUpDown ();
			m_lCurButtonState &= 0xFF00;
		}
		// キーボードで操作中の場合はその操作を停止
		if (m_lCurButtonState & 0x0F00) {
			ReleaseCapture ();
			EndValueUpDown ();
			m_lCurButtonState &= 0xF0F0;
		}
		// カレントセルをひとつ上へ移動
		m_lCurRow = CLIP (0, m_lCurRow - 1, MAXMIDITRACKNUM - 1);
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
		break;
	// 左キー
	case VK_LEFT:
		// テキスト編集中の場合はその内容を確定
		if (IsTextEditing ()) {
			EndTextEditingOK ();
		}
		// マウスで操作中の場合はその操作を停止
		if (m_lCurButtonState & 0x00F0) {
			ReleaseCapture ();
			KillTimer (1);
			EndValueUpDown ();
			m_lCurButtonState &= 0xFF00;
		}
		// キーボードで操作中の場合はその操作を停止
		if (m_lCurButtonState & 0x0F00) {
			ReleaseCapture ();
			EndValueUpDown ();
			m_lCurButtonState &= 0xF0F0;
		}
		// カレントセルをひとつ左へ移動
		m_lCurColumn = CLIP (0, m_lCurColumn - 1, TRACKLISTFRAME_NUMCOLUMN - 1);
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
		break;
	// 右キー
	case VK_RIGHT:
		// テキスト編集中の場合はその内容を確定
		if (IsTextEditing ()) {
			EndTextEditingOK ();
		}
		// マウスで操作中の場合はその操作を停止
		if (m_lCurButtonState & 0x00F0) {
			ReleaseCapture ();
			KillTimer (1);
			EndValueUpDown ();
			m_lCurButtonState &= 0xFF00;
		}
		// キーボードで操作中の場合はその操作を停止
		if (m_lCurButtonState & 0x0F00) {
			ReleaseCapture ();
			EndValueUpDown ();
			m_lCurButtonState &= 0xF0F0;
		}
		// カレントセルをひとつ右へ移動
		m_lCurColumn = CLIP (0, m_lCurColumn + 1, TRACKLISTFRAME_NUMCOLUMN - 1);
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
		break;
	// 下キー
	case VK_DOWN:
		// テキスト編集中の場合はその内容を確定
		if (IsTextEditing ()) {
			EndTextEditingOK ();
		}
		// マウスで操作中の場合はその操作を停止
		if (m_lCurButtonState & 0x00F0) {
			ReleaseCapture ();
			KillTimer (1);
			EndValueUpDown ();
			m_lCurButtonState &= 0xFF00;
		}
		// キーボードで操作中の場合はその操作を停止
		if (m_lCurButtonState & 0x0F00) {
			ReleaseCapture ();
			EndValueUpDown ();
			m_lCurButtonState &= 0xF0F0;
		}
		// カレントセルをひとつ下へ移動
		m_lCurRow = CLIP (0, m_lCurRow + 1, MAXMIDITRACKNUM - 1);
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
		break;
	// 改行(Enter)キー
	case VK_RETURN:
		// マウスで操作中の場合はその操作を停止
		if (m_lCurButtonState & 0x00F0) {
			ReleaseCapture ();
			KillTimer (1);
			EndValueUpDown ();
			m_lCurButtonState &= 0xFF00;
		}
		// キーボードで操作中の場合はその操作を停止
		if (m_lCurButtonState & 0x0F00) {
			ReleaseCapture ();
			EndValueUpDown ();
			m_lCurButtonState &= 0xF0F0;
		}
		// テキスト編集中でない場合
		if (IsTextEditing () == FALSE) {
			// リアルタイム入力中は編集禁止
			if (pSekaijuApp->m_bRecording) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				return;
			}
			// ロックされている場合は編集禁止
			if (pSekaijuDoc->m_bEditLocked) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				return;
			}
			// フォーマット0の場合は最初のトラック以外操作不能
			if (lFormat == 0 && m_lCurRow >= 1) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_EDIT_THIS_TRACK_IN_FORMAT0_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでコントロールチェンジの使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_CONTROLCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_CONTROLCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでプログラムチェンジの使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_PROGRAMCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_PROGRAMCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでタイム+の使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_TIMEPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでキー+の使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_KEYPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでベロシティ+の使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_VELPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// 現在のセルを操作可能な場合
			else {
				// 文字列又は数値の列
				if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_TRACKNAME ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
					BeginTextEditing ();
				}
				// 色
				else if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_FORECOLOR ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_BACKCOLOR) {
					if (DoColorEditing () == TRUE) {
						pSekaijuDoc->SetModifiedFlag (TRUE);
						pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
					}
				}
				// BOOL型
				else if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_VISIBLE ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_ENABLE ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTON ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTON ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VIEWMODE) {
					ToggleValueOfCurCell ();
					pSekaijuDoc->SetModifiedFlag (TRUE);
					pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
				}
			}
		}
		// テキスト編集中の場合(CInplaceEditクラスからPostMessageされる。)
		else {
			EndTextEditingOK ();
			pSekaijuDoc->SetModifiedFlag (TRUE);
			pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		}
		break;
	// エスケープ(Esc)キー(CInplaceEditクラスからPostMessageされる。)
	case VK_ESCAPE:
		EndTextEditingCancel ();
		break;
	// +キー
	case VK_ADD:
	case 187:
		// マウスで操作中の場合はその操作を停止
		if (m_lCurButtonState & 0x00F0) {
			KillTimer (1);
			ReleaseCapture ();
			EndValueUpDown ();
			m_lCurButtonState &= 0xFF00;
		}
		// 項目が数値の場合のみ
		if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
			// リアルタイム入力中は編集禁止
			if (pSekaijuApp->m_bRecording) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				return;
			}
			// ロックされている場合は編集禁止
			if (pSekaijuDoc->m_bEditLocked) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				return;
			}
			// フォーマット0の場合は最初のトラック以外操作不能
			if (lFormat == 0 && m_lCurRow >= 1) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_EDIT_THIS_TRACK_IN_FORMAT0_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでコントロールチェンジの使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_CONTROLCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_CONTROLCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでプログラムチェンジの使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_PROGRAMCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_PROGRAMCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでタイム+の使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_TIMEPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでキー+の使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_KEYPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでベロシティ+の使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_VELPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// 現在のセルを操作可能な場合
			else { // 20091126elseを追加
				// 初回押し時のみキーボード操作開始
				if (m_lCurButtonState == 0x0000) {
					BeginValueUpDown ();
					SetCapture ();
					m_lCurButtonState |= 0x0101;
				}
				// 値の増減
				if (::GetKeyState (VK_SHIFT) < 0) {
					AddValueOfCurCell (10);
				}
				else {
					AddValueOfCurCell (1);
				}
			}
		}
		break;
	// -キー
	case VK_SUBTRACT:
	case 189:
		// マウスで操作中の場合はその操作を停止
		if (m_lCurButtonState & 0x00F0) {
			KillTimer (1);
			ReleaseCapture ();
			EndValueUpDown ();
			m_lCurButtonState &= 0xFF00;
		}
		// 項目が数値の場合のみ
		if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
			// リアルタイム入力中は編集禁止
			if (pSekaijuApp->m_bRecording) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				return;
			}
			// ロックされている場合は編集禁止
			if (pSekaijuDoc->m_bEditLocked) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				return;
			}
			// フォーマット0の場合は最初のトラック以外操作不能
			if (lFormat == 0 && m_lCurRow >= 1) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_EDIT_THIS_TRACK_IN_FORMAT0_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでコントロールチェンジの使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_CONTROLCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_CONTROLCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでプログラムチェンジの使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_PROGRAMCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_PROGRAMCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでタイム+の使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_TIMEPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでキー+の使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_KEYPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでベロシティ+の使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_VELPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// 現在のセルを操作可能な場合
			else { // 20091126elseを追加
				// 初回押し時のみキーボード操作開始
				if (m_lCurButtonState == 0x0000) {
					BeginValueUpDown ();
					SetCapture ();
					m_lCurButtonState |= 0x0202;
				}
				// 値の増減
				if (::GetKeyState (VK_SHIFT) < 0) {
					AddValueOfCurCell (-10);
				}
				else {
					AddValueOfCurCell (-1);
				}
			}
		}
		break;
	// Deleteキー (20090823追加)
	case VK_DELETE:
		pTrackListFrame->PostMessage (WM_COMMAND, ID_TRACKLIST_DELETETRACK, NULL);
		break;
	// Insertキー (20090823追加)
	case VK_INSERT:
		// Ctrlが押されている
		if (GetKeyState (VK_CONTROL) < 0) { 
			pTrackListFrame->PostMessage (WM_COMMAND, ID_TRACKLIST_DUPLICATETRACK, NULL);
		}
		// Ctrlが押されていない
		else {
			pTrackListFrame->PostMessage (WM_COMMAND, ID_TRACKLIST_INSERTTRACK, NULL);
		}
		break;
	}
	
	// 新しいセルの描画(20100128:ちらつき防止のため条件式追加)
	if (IsTextEditing () == FALSE && nChar != VK_CONTROL && nChar != VK_SHIFT && nChar != VK_MENU) {
		CRect rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
		rcNewCell -= CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
		rcNewCell.InflateRect (2, 2);
		InvalidateRect (&rcNewCell);
	}

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// キー押し上げ時
void CTrackListTrackModeView::OnKeyUp (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();

	switch (nChar) {
	// +キー
	case VK_ADD:
	case 187:
		// キーボードで操作中の場合
		if (m_lCurButtonState & 0x0F00) {
			// +キー操作のみ停止
			m_lCurButtonState &= ~0x0101;
			// もはや何の操作も行われていないならば
			if (m_lCurButtonState == 0x0000) {
				ReleaseCapture ();
				EndValueUpDown ();
				pSekaijuDoc->SetModifiedFlag (TRUE);
				pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
			}
		}
		break;
	// -キー
	case VK_SUBTRACT:
	case 189:
		// キーボードで操作中の場合
		if (m_lCurButtonState & 0x0F00) {
			// -キー操作のみ停止
			m_lCurButtonState &= ~0x0202;
			// もはや何の操作も行われていないならば
			if (m_lCurButtonState == 0x0000) {
				ReleaseCapture ();
				EndValueUpDown ();
				pSekaijuDoc->SetModifiedFlag (TRUE);
				pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
			}
		}
		break;
	}

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// 文字入力時
void CTrackListTrackModeView::OnChar (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	// 録音中は何もしない
	if (pSekaijuApp->m_bRecording) {
		::SetCursor (pSekaijuApp->m_hCursorNo);
		return;
	}
	// MIDIデータがロックされている場合は何もしない
	if (pSekaijuDoc->m_bEditLocked) {
		::SetCursor (pSekaijuApp->m_hCursorNo);
		return;
	}
	
	pSekaijuDoc->m_theCriticalSection.Lock ();

	ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
	long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
	if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_TRACKNAME && 32 <= nChar && nChar <= 126 ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT && 48 <= nChar && nChar <= 57 ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL && 48 <= nChar && nChar <= 57 ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT && 48 <= nChar && nChar <= 57 ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL && 48 <= nChar && nChar <= 57 ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE && 48 <= nChar && nChar <= 57 ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE && 48 <= nChar && nChar <= 57 ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS && (48 <= nChar && nChar <= 57 || nChar == '-') ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS && (48 <= nChar && nChar <= 57 || nChar == '-') ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS && (48 <= nChar && nChar <= 57 || nChar == '-')) {
		BeginTextEditing ();
		//m_theTextBox.PostMessage (WM_CHAR, nChar, (nRepCnt << 16) | nFlags);
		m_theTextBox.PostMessage (WM_CHAR, nChar, (nFlags & 0xFFFF0000) | (nRepCnt & 0x0000FFFF));
		// ↑20080722：下の行はWindows2000対応。
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();

	::Sleep (0);
}

// マウス左ボタン押された時  
void CTrackListTrackModeView::OnLButtonDown (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	pSekaijuDoc->m_theCriticalSection.Lock ();

	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	point += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());

	long lButtonWidth = 12;//::GetSystemMetrics (SM_CXVSCROLL);
	BOOL bOldTextEditing = FALSE;
	// テキスト編集の終了
	if (IsTextEditing ()) {
		EndTextEditingOK ();
		pSekaijuDoc->SetModifiedFlag (TRUE);
		bOldTextEditing = TRUE;
	}
	
	// 古いセルの描画
	CRect rcOldCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
	rcOldCell -= CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	rcOldCell.InflateRect (2, 2);
	InvalidateRect (&rcOldCell);

	// 新しいセルの設定
	if (m_lCurButtonState == 0x0000) {
		long i, j;
		GetCellFromPoint (point, &i, &j);
		if (0 <= i && i < MAXMIDITRACKNUM && 0 <= j && j < TRACKLISTFRAME_NUMCOLUMN) {
			m_lCurRow = i;
			m_lCurColumn = j;
		}
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
	}
	CRect rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
	
	// ボタンの押され具合を判別
	ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
	long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
	if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
		// 上又は下のボタンが押された場合
		if (point.x > rcNewCell.right - lButtonWidth) {
			long lFormat = MIDIData_GetFormat (pMIDIData);
			// 録音中は何もしない
			if (pSekaijuApp->m_bRecording) {
				;
			}
			// MIDIデータがロックされている場合は何もしない
			else if (pSekaijuDoc->m_bEditLocked) {
				;
			}
			// フォーマット0の場合は最初のトラック以外操作不能
			else if (lFormat == 0 && m_lCurRow >= 1) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_EDIT_THIS_TRACK_IN_FORMAT0_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでコントロールチェンジの使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_CONTROLCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_CONTROLCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでプログラムチェンジの使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_PROGRAMCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_PROGRAMCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでタイム+の使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_TIMEPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでキー+の使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_KEYPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでベロシティ+の使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_VELPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// 現在のセルを操作可能な場合
			else {
				// マウスによる操作を開始できる場合
				if (m_lCurButtonState == 0x0000) {
					BeginValueUpDown ();
					// 上のボタンが押された場合
					if (point.y < rcNewCell.top + pTrackListFrame->GetRowZoom () / 2) {
						m_lCurButtonState |= 0x0011;
						AddValueOfCurCell (1);
					}
					// 下のボタンが押された場合
					else {
						m_lCurButtonState |= 0x0012;
						AddValueOfCurCell (-1);
					}
					m_lCurButtonInterval = 200;
					SetTimer (1, m_lCurButtonInterval, NULL);
					SetCapture ();
				}
				// 既にマウス操作がなされている場合
				else if (m_lCurButtonState & 0x00F0) {
					m_lCurButtonState |= 0x0010;
				}
			}
		}
	}

	// 新しいセルの描画
	rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
	rcNewCell -= CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	rcNewCell.InflateRect (2, 2);
	InvalidateRect (&rcNewCell);

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);

}

// マウス右ボタン押された時  
void CTrackListTrackModeView::OnRButtonDown (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;

	CPoint ptMenu (point);
	ClientToScreen (&ptMenu);
	
	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	point += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());

	long lButtonWidth = 12;//::GetSystemMetrics (SM_CXVSCROLL);
	BOOL bOldTextEditing = FALSE;
	// テキスト編集の終了
	if (IsTextEditing ()) {
		EndTextEditingOK ();
		pSekaijuDoc->SetModifiedFlag (TRUE);
		bOldTextEditing = TRUE;
	}
	
	// 古いセルの描画
	CRect rcOldCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
	rcOldCell -= CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	rcOldCell.InflateRect (2, 2);
	InvalidateRect (&rcOldCell);
	
	// 新しいセルの設定
	if (m_lCurButtonState == 0x0000) {
		long i, j;
		GetCellFromPoint (point, &i, &j);
		if (0 <= i && i < MAXMIDITRACKNUM && 0 <= j && j < TRACKLISTFRAME_NUMCOLUMN) {
			m_lCurRow = i;
			m_lCurColumn = j;
		}
		MoveTextBox (m_lCurRow, m_lCurColumn);
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
	}
	CRect rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);

	// ボタンの押され具合を判別
	ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
	long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
	if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
		// 上又は下のボタンが押された場合
		if (point.x > rcNewCell.right - lButtonWidth) {
			long lFormat = MIDIData_GetFormat (pMIDIData);
			// 録音中は何もしない
			if (pSekaijuApp->m_bRecording) {
				;
			}
			// MIDIデータがロックされている場合は何もしない
			else if (pSekaijuDoc->m_bEditLocked) {
				;
			}
			// フォーマット0の場合は最初のトラック以外操作不能
			else if (lFormat == 0 && m_lCurRow >= 1) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_EDIT_THIS_TRACK_IN_FORMAT0_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでコントロールチェンジの使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_CONTROLCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_CONTROLCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでプログラムチェンジの使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_PROGRAMCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_PROGRAMCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでタイム+の使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_TIMEPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでキー+の使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_KEYPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでベロシティ+の使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_VELPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// 現在のセルを操作可能な場合
			else {
				// マウスによる操作を開始できる場合
				if (m_lCurButtonState == 0x0000) {
					BeginValueUpDown ();
					// 上のボタンが押された場合
					if (point.y < rcNewCell.top + pTrackListFrame->GetRowZoom () / 2) {
						m_lCurButtonState |= 0x0021;
						AddValueOfCurCell (10);
					}
					// 下のボタンが押された場合
					else {
						m_lCurButtonState |= 0x0022;
						AddValueOfCurCell (-10);
					}
					m_lCurButtonInterval = 200;
					SetTimer (1, m_lCurButtonInterval, NULL);
					SetCapture ();
				}
				// 既にマウス操作がなされている場合
				else if (m_lCurButtonState & 0x00F0) {
					m_lCurButtonState |= 0x0020;
				}
			}
		}
		// ボタンは押されていない場合
		else {
			long lTrackIndex = pTrackListFrame->YtoRow (point.y);
			pSekaijuDoc->m_lTempTime = pTrackListFrame->XtoTime (point.x);
			pSekaijuDoc->m_pTempTrack = pSekaijuDoc->GetTrack (lTrackIndex);
			pSekaijuDoc->m_pTempEvent = NULL;
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			::Sleep (0);

			CMenu theMenu;
			VERIFY (theMenu.LoadMenu (IDR_POPUPMENU01));
			CMenu* pContextMenu = theMenu.GetSubMenu (0);
			pContextMenu->TrackPopupMenu (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
				ptMenu.x, ptMenu.y, pTrackListFrame);
			
			pSekaijuDoc->m_theCriticalSection.Lock ();
		}
	}
	// ボタンのないセルの場合
	else {
		long lTrackIndex = pTrackListFrame->YtoRow (point.y);
		pSekaijuDoc->m_lTempTime = pTrackListFrame->XtoTime (point.x);
		pSekaijuDoc->m_pTempTrack = pSekaijuDoc->GetTrack (lTrackIndex);
		pSekaijuDoc->m_pTempEvent = NULL;
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
		
		CMenu theMenu;
		VERIFY (theMenu.LoadMenu (IDR_POPUPMENU01));
		CMenu* pContextMenu = theMenu.GetSubMenu (0);
		pContextMenu->TrackPopupMenu (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
			ptMenu.x, ptMenu.y, pTrackListFrame);
		
		pSekaijuDoc->m_theCriticalSection.Lock ();
	}

	// 新しいセルの描画
	rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
	rcNewCell -= CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	rcNewCell.InflateRect (2, 2);
	InvalidateRect (&rcNewCell);

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// マウス左ボタン離されたとき
void CTrackListTrackModeView::OnLButtonUp (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	// 録音中は何もしない
	if (pSekaijuApp->m_bRecording) {
		::SetCursor (pSekaijuApp->m_hCursorNo);
		return;
	}
	// MIDIデータがロックされている場合は何もしない
	if (pSekaijuDoc->m_bEditLocked) {
		::SetCursor (pSekaijuApp->m_hCursorNo);
		return;
	}
	
	point += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());

	pSekaijuDoc->m_theCriticalSection.Lock ();
	
	// 左マウスで操作中の場合のみ
	if (m_lCurButtonState & 0x0010) {
		// 左マウス操作の終了
		m_lCurButtonState &= ~0x0010;
		// もはやマウスで操作されていない場合
		if ((m_lCurButtonState & 0x00F0) == 0x0000) {
			EndValueUpDown ();
			m_lCurButtonState = 0x0000;
			KillTimer (1);
			ReleaseCapture ();
			pSekaijuDoc->SetModifiedFlag (TRUE);
			pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		}
	}

	// 新しいセルの描画
	CRect rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
	rcNewCell -= CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	rcNewCell.InflateRect (2, 2);
	InvalidateRect (&rcNewCell);

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// マウス右ボタン離されたとき
void CTrackListTrackModeView::OnRButtonUp (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	// 録音中は何もしない
	if (pSekaijuApp->m_bRecording) {
		::SetCursor (pSekaijuApp->m_hCursorNo);
		return;
	}
	// MIDIデータがロックされている場合は何もしない
	if (pSekaijuDoc->m_bEditLocked) {
		::SetCursor (pSekaijuApp->m_hCursorNo);
		return;
	}

	point += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	
	pSekaijuDoc->m_theCriticalSection.Lock ();

	// 右マウスで操作中の場合のみ
	if (m_lCurButtonState & 0x0020) {
		// 右マウス操作の終了
		m_lCurButtonState &= ~0x0020;
		// もはやマウスで操作されていない場合
		if ((m_lCurButtonState & 0x00F0) == 0x0000) {
			EndValueUpDown ();
			m_lCurButtonState = 0x0000;
			KillTimer (1);
			ReleaseCapture ();
			pSekaijuDoc->SetModifiedFlag (TRUE);
			pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		}
	}

	// 新しいセルの描画
	CRect rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
	rcNewCell -= CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	rcNewCell.InflateRect (2, 2);
	InvalidateRect (&rcNewCell);

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// マウスが動かされたとき
void CTrackListTrackModeView::OnMouseMove (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();

	// 録音中は何もしない
	if (pSekaijuApp->m_bRecording) {
		::SetCursor (pSekaijuApp->m_hCursorNo);
		return;
	}
	// MIDIデータがロックされている場合は何もしない
	if (pSekaijuDoc->m_bEditLocked) {
		::SetCursor (pSekaijuApp->m_hCursorNo);
		return;
	}

}

// マウス左ボタンがダブルクリックされたとき
void CTrackListTrackModeView::OnLButtonDblClk (UINT nFlags, CPoint point) {

	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();

	// 録音中は何もしない
	if (pSekaijuApp->m_bRecording) {
		::SetCursor (pSekaijuApp->m_hCursorNo);
		return;
	}
	// MIDIデータがロックされている場合は何もしない
	if (pSekaijuDoc->m_bEditLocked) {
		::SetCursor (pSekaijuApp->m_hCursorNo);
		return;
	}
	
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lFormat = MIDIData_GetFormat (pMIDIData);

	long lButtonWidth = 12;//::GetSystemMetrics (SM_CXVSCROLL);
	// OnLButtonDownを実行し、現在編集中のセルを取りやめ、新しいフォーカスにセルを移動
	SendMessage (WM_LBUTTONDOWN, nFlags, ((point.y  & 0x0000FFFF) << 16) | (point.x & 0x0000FFFF));
	if (IsTextEditing () == TRUE) {
		return;
	}
	if (m_lCurButtonState != 0x0000) {
		return;
	}

	pSekaijuDoc->m_theCriticalSection.Lock ();

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	point += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());

	// 新しいセルの取得
	long i, j;
	GetCellFromPoint (point, &i, &j);
	if (0 <= i && i < MAXMIDITRACKNUM && 0 <= j && j < TRACKLISTFRAME_NUMCOLUMN) {
		m_lCurRow = i;
		m_lCurColumn = j;
	}
	CRect rcCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
	
	ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
	long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
	if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_TRACKNAME ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
		// セル内のボタン以外の部分
		if (point.x < rcCell.right - lButtonWidth) {
			// フォーマット0の場合は最初のトラック以外操作不能
			if (lFormat == 0 && m_lCurRow >= 1) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_EDIT_THIS_TRACK_IN_FORMAT0_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでコントロールチェンジの使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_CONTROLCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_CONTROLCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでプログラムチェンジの使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_PROGRAMCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_PROGRAMCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでタイム+の使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_TIMEPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでキー+の使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_KEYPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでベロシティ+の使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_VELPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// 現在のセルを操作可能な場合
			else {
				// テキスト編集モードに突入
				BeginTextEditing ();
			}
		}
	}
	else if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_FORECOLOR ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_BACKCOLOR) {
		// フォーマット0の場合は最初のトラック以外操作不能
		if (lFormat == 0 && m_lCurRow >= 1) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_EDIT_THIS_TRACK_IN_FORMAT0_MIDIDATA));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
		}
		// 現在のセルを操作可能な場合
		else {
			// カラーダイアログ表示
			if (DoColorEditing () == TRUE) {
				pSekaijuDoc->SetModifiedFlag (TRUE);
				pSekaijuDoc->UpdateAllViews (NULL); // TODO
			}
		}
	}
	else if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_VISIBLE ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_ENABLE ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTON ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTON ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VIEWMODE) {
		// フォーマット0の場合は最初のトラック以外操作不能
		if (lFormat == 0 && m_lCurRow >= 1) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_EDIT_THIS_TRACK_IN_FORMAT0_MIDIDATA));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
		}
		// 現在のセルを操作可能な場合
		else {
			// 値の反転
			ToggleValueOfCurCell ();
			pSekaijuDoc->SetModifiedFlag (TRUE);
			pSekaijuDoc->UpdateAllViews (NULL); // TODO
		}
	}

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// マウス右ボタンがダブルクリックされたとき
void CTrackListTrackModeView::OnRButtonDblClk (UINT nFlags, CPoint point) {

}

// タイマー時
void CTrackListTrackModeView::OnTimer (UINT nIDEvent) {	
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	
	if (nIDEvent == 1) {
		// タイマーの加速
		if (m_lCurButtonInterval > 50) {
			KillTimer (1);
			m_lCurButtonInterval = 50;
			SetTimer (1, m_lCurButtonInterval, NULL);
		}
		// 上ボタンが左クリックで押されている場合
		if ((m_lCurButtonState & 0x00FF) == 0x0011) {
			AddValueOfCurCell (1);
		}
		// 下ボタンが左クリックで押されている場合
		else if ((m_lCurButtonState & 0x00FF) == 0x0012) {
			AddValueOfCurCell (-1);
		}
		// 上ボタンが右クリックで押されている場合
		if ((m_lCurButtonState & 0x00FF) == 0x0021) {
			AddValueOfCurCell (10);
		}
		// 下ボタンが右クリックで押されている場合
		else if ((m_lCurButtonState & 0x00FF) == 0x0022) {
			AddValueOfCurCell (-10);
		}
		// 上ボタンが両クリックで押されている場合
		if ((m_lCurButtonState & 0x00FF) == 0x0031) {
			AddValueOfCurCell (10);
		}
		// 下ボタンが両クリックで押されている場合
		else if ((m_lCurButtonState & 0x00FF) == 0x0032) {
			AddValueOfCurCell (-10);
		}
	}
	// セルの再描画
	CRect rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
	rcNewCell -= CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	rcNewCell.InflateRect (2, 2);
	InvalidateRect (&rcNewCell, FALSE);

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);

}

// マウスホイールが回された時
void CTrackListTrackModeView::OnMouseWheel40 (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lDelta = (short)HIWORD (nFlags);
	long lFlags = LOWORD (nFlags);
	// 演奏位置の移動
	if (lFlags & MK_CONTROL) {
		if (lDelta > 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel == 0 ||
			lDelta < 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel != 0) {
			pTrackListFrame->PostMessage (WM_COMMAND, ID_CONTROL_PREVMEASURE);
		}
		else {
			pTrackListFrame->PostMessage (WM_COMMAND, ID_CONTROL_NEXTMEASURE);
		}
	}
	// カレントセルの値の増減
	else if (GetKeyState (VK_MENU) < 0) {
		// 録音中は何もしない
		if (pSekaijuApp->m_bRecording) {
			::SetCursor (pSekaijuApp->m_hCursorNo);
			return;
		}
		// MIDIデータがロックされている場合は何もしない
		if (pSekaijuDoc->m_bEditLocked) {
			::SetCursor (pSekaijuApp->m_hCursorNo);
			return;
		}

		pSekaijuDoc->m_theCriticalSection.Lock ();

		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;

		CRect rcClient;
		GetClientRect (&rcClient);
		rcClient += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
		point += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());

		// テキスト編集の終了
		if (IsTextEditing ()) {
			EndTextEditingOK ();
			pSekaijuDoc->SetModifiedFlag (TRUE);
		}
		
		// 古いセルの描画
		CRect rcOldCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
		rcOldCell -= CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
		rcOldCell.InflateRect (2, 2);
		InvalidateRect (&rcOldCell);

		// 新しいセルの設定
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
		CRect rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
		
		// ボタンの押され具合を判別
		ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
		long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
		if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
			long lFormat = MIDIData_GetFormat (pMIDIData);
			// フォーマット0の場合は最初のトラック以外操作不能
			if (lFormat == 0 && m_lCurRow >= 1) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_EDIT_THIS_TRACK_IN_FORMAT0_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでコントロールチェンジの使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_CONTROLCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_CONTROLCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでプログラムチェンジの使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_PROGRAMCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_PROGRAMCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでタイム+の使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_TIMEPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでキー+の使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_KEYPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// フォーマット1の場合は最初のトラックでベロシティ+の使用不可
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_VELPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// 現在のセルを操作可能な場合
			else {
				// マウスによる操作を開始できる場合
				if (m_lCurButtonState == 0x0000) {
					long lDeltaValue = GetKeyState (VK_SHIFT) < 0 ? 10 : 1;
					BeginValueUpDown ();
					// 上回しの場合
					if (lDelta > 0) {
						AddValueOfCurCell (lDeltaValue);
					}
					// 下回しの場合
					else {
						AddValueOfCurCell (-lDeltaValue);
					}
					EndValueUpDown ();
					pSekaijuDoc->SetModifiedFlag (TRUE);
					pSekaijuDoc->UpdateAllViews 
						(NULL, SEKAIJUDOC_MIDIEVENTCHANGED | SEKAIJUDOC_MIDITRACKCHANGED);
				}
			}
		}

		// 新しいセルの描画
		rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
		rcNewCell -= CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
		rcNewCell.InflateRect (2, 2);
		InvalidateRect (&rcNewCell);

		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}
	// 画面スクロール
	else {
		long lRowScrollPos = pTrackListFrame->GetRowScrollPos ();
		long lRowZoom = pTrackListFrame->GetRowZoom ();
		lRowScrollPos -= lRowZoom * lDelta / WHEELDELTA;
		pTrackListFrame->SetRowScrollPos (lRowScrollPos);
	}
}
