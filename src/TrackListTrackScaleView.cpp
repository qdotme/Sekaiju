//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// トラックリストトラックスケールビュークラス
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
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDoc.h"
#include "SekaijuView.h"
#include "SekaijuToolbar.h"
#include "ChildFrame.h"
#include "TrackListFrame.h"
#include "TrackListTrackScaleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE (CTrackListTrackScaleView, CSekaijuView)

// メッセージマップ
BEGIN_MESSAGE_MAP (CTrackListTrackScaleView, CSekaijuView)
	ON_WM_CREATE ()
	ON_WM_KEYDOWN ()
	ON_WM_LBUTTONDOWN ()
	ON_WM_RBUTTONDOWN ()
	ON_WM_LBUTTONUP ()
	ON_WM_RBUTTONUP ()
	ON_WM_MOUSEMOVE ()
	ON_WM_LBUTTONDBLCLK ()
	ON_WM_RBUTTONDBLCLK ()
	ON_WM_TIMER ()
	ON_WM_MOUSEWHEEL40 ()
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CTrackListTrackScaleView::CTrackListTrackScaleView () {
}

// デストラクタ
CTrackListTrackScaleView::~CTrackListTrackScaleView () {
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// 原点の移動をオーバーライド
void CTrackListTrackScaleView::OnPrepareDC (CDC* pDC, CPrintInfo* pInfo) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	pDC->SetWindowOrg (0, pTrackListFrame->GetRowScrollPos ());
}

// 描画
void CTrackListTrackScaleView::OnDraw (CDC* pDC) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();

	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack;
	BOOL bTrackZeroOrigin = pSekaijuApp->m_theGeneralOption.m_bTrackZeroOrigin;

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (0, pTrackListFrame->GetRowScrollPos ());
	pDC->SetBkMode (TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject (&(pTrackListFrame->m_theFont));
	long lVisibleTopRow = pTrackListFrame->GetVisibleTopRow ();
	long lVisibleBottomRow = pTrackListFrame->GetVisibleBottomRow ();
	long y = 0;
	TCHAR szBuf[256];
	long i = 0;
	long lRowZoom = pTrackListFrame->GetRowZoom ();
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);
	long lColorBlack = RGB (0, 0, 0);
	long lColorWhite = RGB (255, 255, 255);

	for (i = lVisibleTopRow; i <= lVisibleBottomRow; i++) {
		y = i * lRowZoom;
		memset (szBuf, 0, sizeof (szBuf));
		_sntprintf (szBuf, 255, _T("%d"), i + (bTrackZeroOrigin ? 0 : 1));
		BOOL bTrackSelected = FALSE;
		if (0 <= i && i < lTrackCount) { // 20080903位置修正
			pMIDITrack = pSekaijuDoc->GetTrack (i);
			bTrackSelected = pSekaijuDoc->IsTrackSelected (pMIDITrack);
			if (GetCapture () == this) {
				long lMinRow = __min (m_lDownRow, m_lCurRow);
				long lMaxRow = __max (m_lDownRow, m_lCurRow);
				if (lMinRow <= i && i <= lMaxRow) {
					bTrackSelected = TRUE;
				}
			}
		}
		else { // 20080903位置修正
			pMIDITrack = NULL;
			bTrackSelected = FALSE;
		}
		CRect theRect (0, y, rcClient.Width (), y + lRowZoom);
		if (bTrackSelected) {
			pDC->FillSolidRect (&theRect, lColorBlack);
			pDC->Draw3dRect (&theRect, lColorBlack, lColorBtnShadow);
			pDC->SetTextColor (lColorWhite);
			pDC->DrawText (szBuf, -1, &theRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
		else {
			pDC->FillSolidRect (&theRect, lColorBtnFace);
			pDC->Draw3dRect (&theRect, lColorBtnHighlight, lColorBtnShadow);
			pDC->SetTextColor (lColorBtnText);
			pDC->DrawText (szBuf, -1, &theRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
	}
	pDC->SelectObject (pOldFont);

	pSekaijuDoc->m_theCriticalSection.Unlock ();

}

// ビューの更新
void CTrackListTrackScaleView::OnUpdate (CView* pSender, LPARAM lHint, CObject* pHint) {
	// クリティカルセクションはロックされているものとする。
	CSekaijuView::OnUpdate (pSender, lHint, pHint);
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// ウィンドウ生成時
BOOL CTrackListTrackScaleView::OnCreate (LPCREATESTRUCT lpcs) {
	return CSekaijuView::OnCreate (lpcs);
}

// キー押し下げ時
void CTrackListTrackScaleView::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	switch (nChar) {
	// Deleteキー
	case VK_DELETE:
		// 『編集(E)』-『削除』実行 (20090823追加)
		PostMessage (WM_COMMAND, ID_EDIT_DELETE, NULL);
		break;
	}
}

// マウス左ボタン押された時 
void CTrackListTrackScaleView::OnLButtonDown (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
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

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (0, pTrackListFrame->GetRowScrollPos ());
	point += CSize (0, pTrackListFrame->GetRowScrollPos ());

	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDITrack* pCloneTrack = NULL;
	MIDIEvent* pCloneEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_SELECT_DESELECT));
	VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());

	// 旧選択イベントの選択解除(Shiftが押されていない場合かつCtrlが押されていない場合のみ)
	if ((nFlags & MK_SHIFT) == 0 && (nFlags & MK_CONTROL) == 0) {
		pSekaijuDoc->SelectNoObject (pCurHistoryUnit);
	}


	// 該当トラック内の全イベントの選択
	m_lOldRow = pTrackListFrame->YtoRow (m_ptMouseDown.y);
	m_lDownRow = pTrackListFrame->YtoRow (point.y);
	m_lCurRow = pTrackListFrame->YtoRow (point.y);
	if (nFlags & MK_SHIFT) {
		long lMinRow = __min (m_lOldRow, m_lCurRow);
		long lMaxRow = __max (m_lOldRow, m_lCurRow);
		long i = 0;
		forEachTrack (pMIDIData, pMIDITrack) {
			if (lMinRow <= i && i <= lMaxRow) {
				pSekaijuDoc->SelectTrack (pMIDITrack, 1, pCurHistoryUnit);
			}
			i++;
		}
	}
	else {
		pMIDITrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pMIDITrack) {
			pSekaijuDoc->SelectTrack (pMIDITrack, 1, pCurHistoryUnit);
		}
	}
	
	SetTimer (0x21, 55, NULL);
	SetCapture ();
	Invalidate ();

	m_ptMouseDown = m_ptMouseMove = point;
	m_nMouseDownFlags = m_nMouseMoveFlags = nFlags;

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// マウス右ボタン押された時  
void CTrackListTrackScaleView::OnRButtonDown (UINT nFlags, CPoint point) {
}

// マウス左ボタン離されたとき
void CTrackListTrackScaleView::OnLButtonUp (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
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

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (0, pTrackListFrame->GetRowScrollPos ());
	point += CSize (0, pTrackListFrame->GetRowScrollPos ());

	if (GetCapture () == this) {
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		
		pSekaijuDoc->m_theCriticalSection.Lock ();
		KillTimer (0x21);
		ReleaseCapture ();

		if (m_lDownRow != m_lCurRow) {
			long lMinRow = __min (m_lDownRow, m_lCurRow);
			long lMaxRow = __max (m_lDownRow, m_lCurRow);
			long i = 0;
			//if (lDownRow <= lOldRow && lOldRow <= lCurRow ||
			//	lDownRow >= lOldRow && lOldRow >= lCurRow) {
			forEachTrack (pMIDIData, pMIDITrack) {
				if (lMinRow <= i && i <= lMaxRow) {
					pSekaijuDoc->SelectTrack (pMIDITrack, 1, pCurHistoryUnit);
				}
				i++;
			}
		}
		
		pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED |
			SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED); 
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}

}

// マウス右ボタン離されたとき
void CTrackListTrackScaleView::OnRButtonUp (UINT nFlags, CPoint point) {
}

// マウスが動かされたとき
void CTrackListTrackScaleView::OnMouseMove (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
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

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (0, pTrackListFrame->GetRowScrollPos ());
	point += CSize (0, pTrackListFrame->GetRowScrollPos ());

	if (GetCapture () == this) {
		pSekaijuDoc->m_theCriticalSection.Lock ();

		m_lCurRow = pTrackListFrame->YtoRow (point.y);
		m_lDownRow = pTrackListFrame->YtoRow (m_ptMouseDown.y);
		m_lOldRow = pTrackListFrame->YtoRow (m_ptMouseMove.y);
		if (m_lOldRow != m_lCurRow) {
			Invalidate (FALSE);
		}
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}
	m_ptMouseMove = point;
	m_nMouseMoveFlags = nFlags;


}

// マウス左ボタンがダブルクリックされたとき
void CTrackListTrackScaleView::OnLButtonDblClk (UINT nFlags, CPoint point) {
}

// マウス右ボタンがダブルクリックされたとき
void CTrackListTrackScaleView::OnRButtonDblClk (UINT nFlags, CPoint point) {
}


// タイマー時
void CTrackListTrackScaleView::OnTimer (UINT nIDEvent) {	

	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	if (nIDEvent == 0x21) {
		if (GetCapture () == this) {
			CRect rcClient;
			GetClientRect (&rcClient);
			rcClient += CSize (0, pTrackListFrame->GetRowScrollPos ());
			if (!rcClient.PtInRect (m_ptMouseMove)) {
				pSekaijuDoc->m_theCriticalSection.Lock ();
				long lOldRowScrollPos = pTrackListFrame->GetRowScrollPos ();
				if (m_ptMouseMove.y < rcClient.top) {
					pTrackListFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEUP,
						(LPARAM)(pTrackListFrame->m_wndRowScroll.GetSafeHwnd ()));
				}
				else if (m_ptMouseMove.y >= rcClient.bottom) {
					pTrackListFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEDOWN,
						(LPARAM)(pTrackListFrame->m_wndRowScroll.GetSafeHwnd ()));
				}
				WORD wX = (WORD)(m_ptMouseMove.x);
				WORD wY = (WORD)(m_ptMouseMove.y - lOldRowScrollPos);
				PostMessage (WM_MOUSEMOVE, (WPARAM)m_nMouseMoveFlags, (LPARAM)((wY << 16) | wX));
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				::Sleep (0);
			}
		}
	}
}

// マウスホイールが回された時
void CTrackListTrackScaleView::OnMouseWheel40 (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lDelta = (short)HIWORD (nFlags);
	long lFlags = LOWORD (nFlags);
	if (lFlags & MK_CONTROL) {
		if (lDelta > 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel == 0 ||
			lDelta < 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel != 0) {
			pTrackListFrame->PostMessage (WM_COMMAND, ID_CONTROL_PREVMEASURE);
		}
		else {
			pTrackListFrame->PostMessage (WM_COMMAND, ID_CONTROL_NEXTMEASURE);
		}
	}
	else {
		long lRowScrollPos = pTrackListFrame->GetRowScrollPos ();
		long lRowZoom = pTrackListFrame->GetRowZoom ();
		lRowScrollPos -= lRowZoom * lDelta / WHEELDELTA;
		pTrackListFrame->SetRowScrollPos (lRowScrollPos);
	}
}


