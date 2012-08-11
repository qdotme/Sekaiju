//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// イベントリストスケールビュークラス
// (C)2002-2010 おーぷんMIDIぷろじぇくと／くず
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
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "mousewheel.h"
#include "ColorfulComboBox.h"
#include "ColorfulCheckListBox.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDoc.h"
#include "SekaijuView.h"
#include "SekaijuToolBar.h"
#include "MainFrame.h"
#include "ChildFrame.h"
#include "EventListFrame.h"
#include "EventListScaleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE (CEventListScaleView, CSekaijuView)

// メッセージマップ
BEGIN_MESSAGE_MAP (CEventListScaleView, CSekaijuView)
	ON_WM_MOUSEWHEEL40 ()
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CEventListScaleView::CEventListScaleView () {
}

// デストラクタ
CEventListScaleView::~CEventListScaleView () {
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// 描画
void CEventListScaleView::OnDraw (CDC* pDC) {
	CSekaijuDoc* pSekaijuDoc = GetDocument();
	CRect rcClient;
	GetClientRect (&rcClient);
	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	pDC->FillSolidRect (&rcClient, lColorBtnFace);
}

// ビューの更新
void CEventListScaleView::OnUpdate (CView* pSender, LPARAM lHint, CObject* pHint) {
	// クリティカルセクションはロックされているものとする。
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	// 演奏開始した場合、リアルタイム入力開始した場合、位置移動した場合
	if ((lHint & SEKAIJUDOC_PLAYSTARTED) || 
		(lHint & SEKAIJUDOC_RECORDSTARTED) ||
		(lHint & SEKAIJUDOC_POSITIONCHANGED)) {
		if (pSekaijuApp->m_theGeneralOption.m_bEnableAutoPageUpdate) { // 20091224追加
			pEventListFrame->m_bAutoPageUpdate = TRUE;
		}
	}
	// MIDIデータが変更された場合
	if (lHint & SEKAIJUDOC_MIDIDATACHANGED) {
		// タイトル文字列を更新(20091201:CEventListPropertyScaleView::OnDrawから引越し)
		long lTimeMode = 0;
		long lTimeResolution = 120;
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);
		if (lTimeMode == MIDIDATA_TPQNBASE) {
			CString strTitle;
			VERIFY (strTitle.LoadString (IDS_MEASURE_BEAT_TICK));
			pEventListFrame->SetColumnTitle (2, strTitle);
		}
		else if (lTimeMode == MIDIDATA_SMPTE24BASE ||
			lTimeMode == MIDIDATA_SMPTE25BASE ||
			lTimeMode == MIDIDATA_SMPTE29BASE ||
			lTimeMode == MIDIDATA_SMPTE30BASE) {
			CString strTitle;
			VERIFY (strTitle.LoadString (IDS_FRAME_SUBFRAME));
			pEventListFrame->SetColumnTitle (2, strTitle);
		}
	}
	// MIDIトラックが変更された場合
	if (lHint & SEKAIJUDOC_MIDITRACKCHANGED) {
		pEventListFrame->UpdateTrackList ();
		pEventListFrame->UpdateTrackCombo ();
	}
	// MIDIデータ又はMIDIトラック又はMIDIイベントが変更された場合
	if ((lHint & SEKAIJUDOC_MIDIDATACHANGED) || 
		(lHint & SEKAIJUDOC_MIDITRACKCHANGED) ||
		(lHint & SEKAIJUDOC_MIDIEVENTCHANGED)) {
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		long lEndTime = MIDIData_GetEndTime (pMIDIData);
		pMainFrame->SetPositionScrollRange (0, lEndTime, TRUE);
		pEventListFrame->MakeVisibleEventArray ();
		pEventListFrame->RecalcRowScrollInfo ();
	}
	CSekaijuView::OnUpdate (pSender, lHint, pHint);
}

// マウスホイールが回された時
void CEventListScaleView::OnMouseWheel40 (UINT nFlags, CPoint point) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lDelta = (short)HIWORD (nFlags);
	long lFlags = LOWORD (nFlags);
	if (lFlags & MK_CONTROL) {
		if (lDelta > 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel == 0 ||
			lDelta < 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel != 0) {
			pEventListFrame->PostMessage (WM_COMMAND, ID_CONTROL_PREVMEASURE);
		}
		else {
			pEventListFrame->PostMessage (WM_COMMAND, ID_CONTROL_NEXTMEASURE);
		}
	}
	else {
		long lRowScrollPos = pEventListFrame->GetRowScrollPos ();
		long lRowZoom = pEventListFrame->GetRowZoom ();
		lRowScrollPos -= lRowZoom * lDelta / WHEELDELTA;
		pEventListFrame->SetRowScrollPos (lRowScrollPos);
		pEventListFrame->m_bAutoPageUpdate = FALSE;
	}
}

