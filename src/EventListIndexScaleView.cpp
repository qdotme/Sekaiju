//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// イベントリストインデックススケールビュークラス
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
#include "ChildFrame.h"
#include "EventListFrame.h"
#include "EventListIndexScaleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE (CEventListIndexScaleView, CSekaijuView)

// メッセージマップ
BEGIN_MESSAGE_MAP (CEventListIndexScaleView, CSekaijuView)
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
CEventListIndexScaleView::CEventListIndexScaleView () {
}

// デストラクタ
CEventListIndexScaleView::~CEventListIndexScaleView () {
}


//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// セルの長方形を取得
CRect CEventListIndexScaleView::GetRectFromRow (long lRow) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CRect rcClient;
	GetClientRect (&rcClient);
	CRect rcCell (0, 0, 0 ,0);
	rcCell.left = 0;
	rcCell.right = rcCell.left + rcClient.Width ();
	long lRowZoom = pEventListFrame->GetRowZoom ();
	rcCell.top = lRow * lRowZoom;
	rcCell.bottom = (lRow + 1) * lRowZoom;
	ASSERT (rcCell.left < rcCell.right);
	ASSERT (rcCell.top < rcCell.bottom);
	return rcCell;
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// 原点の移動をオーバーライド
void CEventListIndexScaleView::OnPrepareDC (CDC* pDC, CPrintInfo* pInfo) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	pDC->SetWindowOrg (0, pEventListFrame->GetRowScrollPos ());
}

// 描画
void CEventListIndexScaleView::OnDraw (CDC* pDC) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	BOOL bEventZeroOrigin = pSekaijuApp->m_theGeneralOption.m_bEventZeroOrigin;
	
	CRect rcClient;
	GetClientRect (&rcClient);
	pDC->DPtoLP (&rcClient);
	pDC->SetBkMode (TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject (pEventListFrame->GetParentFont ());
	long lColorBlack = RGB (0, 0, 0);
	long lColorWhite = RGB (255, 255, 255);
	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);

	long lTopIndex = pEventListFrame->GetVisibleTopRow ();
	long lBottomIndex = pEventListFrame->GetVisibleBottomRow ();
	long y = 0;
	long yold = 0;
	TCHAR szBuf[256];
	
	long lRowZoom = pEventListFrame->GetRowZoom ();
	for (long i = lTopIndex; i <= lBottomIndex; i++) {
		MIDIEvent* pMIDIEvent = pEventListFrame->GetVisibleEvent (i);
		long bEventSelected = FALSE;
		if (pMIDIEvent) {
			bEventSelected = pSekaijuDoc->IsEventSelected (pMIDIEvent);
		}
		y = i * lRowZoom;
		CRect theRect (0, y, rcClient.Width (), y + lRowZoom);
		memset (szBuf, 0, sizeof (szBuf));
		_sntprintf (szBuf, 255, _T("%d"), i + (bEventZeroOrigin ? 0 : 1));
		if (bEventSelected) {
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
		yold = y;
	}
	pDC->SelectObject (pOldFont);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}


// ビューの更新
void CEventListIndexScaleView::OnUpdate (CView* pSender, LPARAM lHint, CObject* pHint) {
	// クリティカルセクションはロックされているものとする。
	CSekaijuView::OnUpdate (pSender, lHint, pHint);
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// ウィンドウ生成時
BOOL CEventListIndexScaleView::OnCreate (LPCREATESTRUCT lpcs) {
	return CSekaijuView::OnCreate (lpcs);
}

// キー押し下げ時
void CEventListIndexScaleView::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
}

// マウス左ボタン押された時 
void CEventListIndexScaleView::OnLButtonDown (UINT nFlags, CPoint point) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();

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
	rcClient += CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());
	point += CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());

	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDITrack* pCloneTrack = NULL;
	MIDIEvent* pCloneEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	VERIFY (pSekaijuDoc->AddHistoryUnit (_T("選択／選択解除")));
	VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());

	// 旧選択イベントの選択解除(Shiftが押されていない場合かつCtrlが押されていない場合のみ)
	if ((nFlags & MK_SHIFT) == 0 && (nFlags & MK_CONTROL) == 0) {
		pSekaijuDoc->SelectNoObject (pCurHistoryUnit);
		pEventListFrame->MakeVisibleEventArray (); // TODO 他のフレームウィンドウが不正になる
	}

	// イベントの選択
	long lOldRow = pEventListFrame->YtoRow (m_ptMouseDown.y);
	long lCurRow = pEventListFrame->YtoRow (point.y);
	// シフトが押されている場合
	if (nFlags & MK_SHIFT) {
		long lMinRow = __min (lOldRow, lCurRow);
		long lMaxRow = __max (lOldRow, lCurRow);
		long i;
		for (i = lMinRow; i <= lMaxRow; i++) {
			pMIDIEvent = pEventListFrame->GetVisibleEvent (i);
			if (pMIDIEvent) {
				MIDIEvent* pFirstCombinedEvent = MIDIEvent_GetFirstCombinedEvent (pMIDIEvent);
				pCloneEvent = pSekaijuDoc->SelectEvent (pFirstCombinedEvent, 1, pCurHistoryUnit);
				if (pCloneEvent) {
					pEventListFrame->ReplaceVisibleEvent (pFirstCombinedEvent, pCloneEvent);
				}
			}
		}
	}
	// シフトが押されていない場合
	else {
		pMIDIEvent = pEventListFrame->GetVisibleEvent (lCurRow);
		if (pMIDIEvent) {
			MIDIEvent* pFirstCombinedEvent = MIDIEvent_GetFirstCombinedEvent (pMIDIEvent);
			pCloneEvent = pSekaijuDoc->SelectEvent (pFirstCombinedEvent, 1, pCurHistoryUnit);
			if (pCloneEvent) {
				pEventListFrame->ReplaceVisibleEvent (pFirstCombinedEvent, pCloneEvent);
			}
		}
	}

	SetCapture ();
	SetTimer (0x21, 55, NULL);
	Invalidate ();

	m_ptMouseDown = m_ptMouseMove = point;
	m_nMouseDownFlags = m_nMouseMoveFlags = nFlags;
	pEventListFrame->m_bAutoPageUpdate = FALSE;

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// マウス右ボタン押された時  
void CEventListIndexScaleView::OnRButtonDown (UINT nFlags, CPoint point) {
}

// マウス左ボタン離されたとき
void CEventListIndexScaleView::OnLButtonUp (UINT nFlags, CPoint point) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();

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

	if (GetCapture () == this) {
		pSekaijuDoc->m_theCriticalSection.Lock ();
		ReleaseCapture ();
		KillTimer (0x21);
		pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIDATACHANGED | 
			SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIEVENTCHANGED);
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}

}

// マウス右ボタン離されたとき
void CEventListIndexScaleView::OnRButtonUp (UINT nFlags, CPoint point) {
}

// マウスが動かされたとき
void CEventListIndexScaleView::OnMouseMove (UINT nFlags, CPoint point) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();

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
	
	if (GetCapture () == this) {
		CRect rcClient;
		GetClientRect (&rcClient);
		rcClient += CSize (0, pEventListFrame->GetRowScrollPos ());
		point += CSize (0, pEventListFrame->GetRowScrollPos ());
		pSekaijuDoc->m_theCriticalSection.Lock ();

		// イベントの選択又は選択解除
		long lCurRow = pEventListFrame->YtoRow (point.y);
		long lDownRow = pEventListFrame->YtoRow (m_ptMouseDown.y);
		long lOldRow = pEventListFrame->YtoRow (m_ptMouseMove.y);
		if (lOldRow != lCurRow) {
			long lVisibleEventCount = pEventListFrame->GetVisibleEventCount ();
			long lMinRow = __min (lOldRow, lCurRow);
			long lMaxRow = __max (lOldRow, lCurRow);
			long i;
			MIDITrack* pMIDITrack = NULL;
			MIDIEvent* pMIDIEvent = NULL;
			MIDIEvent* pCloneEvent = NULL;
			CHistoryUnit* pCurHistoryUnit = NULL;
			VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
			if (lDownRow <= lOldRow && lOldRow <= lCurRow ||
				lDownRow >= lOldRow && lOldRow >= lCurRow) {
				for (i = lMinRow; i <= lMaxRow; i++) {
 					pMIDIEvent = pEventListFrame->GetVisibleEvent (i);
					if (pMIDIEvent) {
						MIDIEvent* pFirstCombinedEvent = MIDIEvent_GetFirstCombinedEvent (pMIDIEvent);
						pCloneEvent = pSekaijuDoc->SelectEvent (pFirstCombinedEvent, 1, pCurHistoryUnit);
						if (pCloneEvent) {
							pEventListFrame->ReplaceVisibleEvent (pFirstCombinedEvent, pCloneEvent);
						}
					}
				}
			}
			else if ((nFlags & MK_SHIFT) == 0 && (nFlags & MK_CONTROL) == 0) {
				for (i = lMinRow; i <= lMaxRow; i++) {
 					if (i != lCurRow) {
						pMIDIEvent = pEventListFrame->GetVisibleEvent (i);
						if (pMIDIEvent) {
							MIDIEvent* pFirstCombinedEvent = MIDIEvent_GetFirstCombinedEvent (pMIDIEvent);
							pCloneEvent = pSekaijuDoc->SelectEvent (pFirstCombinedEvent, 0, pCurHistoryUnit);
							if (pCloneEvent) {
								pEventListFrame->ReplaceVisibleEvent (pFirstCombinedEvent, pCloneEvent);
							}
						}
					}
				}
			}
			Invalidate (FALSE);
		}
		m_ptMouseMove = point;
		m_nMouseMoveFlags = nFlags;
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}


}

// マウス左ボタンがダブルクリックされたとき
void CEventListIndexScaleView::OnLButtonDblClk (UINT nFlags, CPoint point) {
}

// マウス右ボタンがダブルクリックされたとき
void CEventListIndexScaleView::OnRButtonDblClk (UINT nFlags, CPoint point) {
}


// タイマー時
void CEventListIndexScaleView::OnTimer (UINT nIDEvent) {	
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	
	// マウスキャプター中にクライアント領域をはみ出した場合の自動スクロール処理
	if (nIDEvent == 0x21) {
		if (GetCapture () == this) {
			CClientDC dc (this);
			OnPrepareDC (&dc, NULL);
			CRect rcClient;
			GetClientRect (&rcClient);
			rcClient += CSize (0, pEventListFrame->GetRowScrollPos ());
			if (!rcClient.PtInRect (m_ptMouseMove)) {
				long lOldRowScrollPos = pEventListFrame->GetRowScrollPos ();
				pSekaijuDoc->m_theCriticalSection.Lock ();
				if (m_ptMouseMove.y < rcClient.top) {
					pEventListFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEUP,
						(LPARAM)(pEventListFrame->m_wndRowScroll.GetSafeHwnd ()));
				}
				else if (m_ptMouseMove.y > rcClient.bottom) {
					pEventListFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEDOWN,
						(LPARAM)(pEventListFrame->m_wndRowScroll.GetSafeHwnd ()));
				}
				WORD wX = (WORD)(m_ptMouseMove.x);
				WORD wY = (WORD)(m_ptMouseMove.y - lOldRowScrollPos);
				PostMessage (WM_MOUSEMOVE, (WPARAM)m_nMouseMoveFlags, (LPARAM)((wY << 16) | wX));
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				::Sleep (0);
			}
		}
	}

	::Sleep (0);
}

// マウスホイールが回された時
void CEventListIndexScaleView::OnMouseWheel40 (UINT nFlags, CPoint point) {
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

