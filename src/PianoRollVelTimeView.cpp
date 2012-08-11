//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ピアノロールベロシティタイムビュークラス
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
#include <afxcmn.h>
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
#include "SekaijuToolBar.h"
#include "ChildFrame.h"
#include "PianoRollFrame.h"
#include "SekaijuView.h"
#include "PianoRollVelTimeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE (CPianoRollVelTimeView, CSekaijuView)

// メッセージマップ
BEGIN_MESSAGE_MAP (CPianoRollVelTimeView, CSekaijuView)
	ON_WM_CREATE ()
	ON_WM_DESTROY ()
	ON_WM_TIMER ()
	ON_WM_KEYDOWN ()
	ON_WM_LBUTTONDOWN ()
	ON_WM_RBUTTONDOWN ()
	ON_WM_LBUTTONUP ()
	ON_WM_RBUTTONUP ()
	ON_WM_MOUSEMOVE ()
	ON_WM_MOUSEWHEEL40 ()
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CPianoRollVelTimeView::CPianoRollVelTimeView () {
	m_lCurTime = m_lOldTime = 0;
	m_lOldY1 = m_lOldY2 = 0;
	m_bOldDraw = TRUE;
	m_pTempEvent = NULL;
	m_lTempMode = 0;
	m_pLastEvent = NULL; //TODO:20080802
	m_lLastInsertedTempoBPM = -1;
	m_lLastInsertedPitchBend = -1;
	m_lLastInsertedValue = -1;
}

// デストラクタ
CPianoRollVelTimeView::~CPianoRollVelTimeView () {
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// 旧位置の縦線消去
void CPianoRollVelTimeView::EraseOldLine (CDC* pDC) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pPianoRollFrame->GetTimeScrollPos (), pPianoRollFrame->GetVelScrollPos ());
	if (rcClient.left <= m_lOldX && m_lOldX <= rcClient.right && m_bOldDraw == TRUE) {
		pDC->SetROP2 (R2_NOT);
		pDC->MoveTo (m_lOldX, m_lOldY1);
		pDC->LineTo (m_lOldX, m_lOldY2);
	}
}

// 現在位置の縦線描画
void CPianoRollVelTimeView::DrawCurLine (CDC* pDC) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pPianoRollFrame->GetTimeScrollPos (), pPianoRollFrame->GetVelScrollPos ());
	long x = pPianoRollFrame->TimetoX (m_lCurTime);
	if (rcClient.left <= x && x <= rcClient.right) {
		pDC->SetROP2 (R2_NOT);
		pDC->MoveTo (x, rcClient.top);
		pDC->LineTo (x, rcClient.bottom);
		m_bOldDraw = TRUE;
		m_lOldTime = m_lCurTime;
		m_lOldX = x;
		m_lOldY1 = rcClient.top;
		m_lOldY2 = rcClient.bottom;
	}
	else {
		m_bOldDraw = FALSE;
	}
}



// 原点の移動
void CPianoRollVelTimeView::OnPrepareDC (CDC* pDC, CPrintInfo* pInfo) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	pDC->SetWindowOrg (pPianoRollFrame->GetTimeScrollPos (), pPianoRollFrame->GetVelScrollPos ());
}

// 描画
void CPianoRollVelTimeView::OnDraw (CDC* pDC) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pPianoRollFrame->GetTimeScrollPos (), pPianoRollFrame->GetVelScrollPos ());

	CPen penMeasure (PS_SOLID, 1, RGB (128, 128, 255));
	CPen penBeat (PS_SOLID, 1, RGB (192, 192, 192));
	CPen penKey (PS_SOLID, 1, RGB (192, 192, 192));
	CPen penOctave (PS_SOLID, 1, RGB (128, 128, 255));
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;

	long x, y, lTime;
	long lVisibleLeftTime = pPianoRollFrame->GetVisibleLeftTime ();
	long lVisibleRightTime = pPianoRollFrame->GetVisibleRightTime ();
	long lLeftMeasure, lLeftBeat, lLeftTick;
	long lRightMeasure, lRightBeat, lRightTick;
	long lnn, ldd, lcc, lbb;
	long lUnitTick;


	// 横線の描画
	CPen* pOldPen = pDC->SelectObject (&penKey);
	long i, j;
	for (i = 0; i <= 8; i++) {
		if (i != 4) {
			pDC->SelectObject (&penKey);
		}
		else {
			pDC->SelectObject (&penOctave);
		}
		y = pPianoRollFrame->VeltoY (i * 16);
		pDC->MoveTo (rcClient.left, y);
		pDC->LineTo (rcClient.right, y);
	}

	// 縦線の描画
	long lTimeMode, lTimeResolution;
	MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);
	if (lTimeMode == MIDIDATA_TPQNBASE) {
		MIDIData_BreakTime (GetDocument()->m_pMIDIData, lVisibleLeftTime, &lLeftMeasure, &lLeftBeat, &lLeftTick);
		MIDIData_BreakTime (GetDocument()->m_pMIDIData, lVisibleRightTime, &lRightMeasure, &lRightBeat, &lRightTick);
		for (i = lLeftMeasure; i <= lRightMeasure; i++) {
			// 小節線の描画
			MIDIData_MakeTimeEx (GetDocument()->m_pMIDIData, i, 0, 0, &lTime, &lnn, &ldd, &lcc, &lbb);
			x = pPianoRollFrame->TimetoX (lTime);
			pDC->SelectObject (&penMeasure);
			pDC->MoveTo (x, rcClient.top);
			pDC->LineTo (x, rcClient.bottom);
			// 拍線の描画
			pDC->SelectObject (&penBeat);
			lUnitTick = lTimeResolution * 4 / (1 << ldd);
			for (j = 1; j < lnn; j++) {
				x = pPianoRollFrame->TimetoX (lTime + j * lUnitTick);
				pDC->MoveTo (x, rcClient.top);
				pDC->LineTo (x, rcClient.bottom);
			}
		}
	}
	else {
		long lLeftFrameNumber = lVisibleLeftTime / lTimeResolution;
		long lRightFrameNumber = lVisibleRightTime / lTimeResolution;
		for (i = lLeftFrameNumber; i <= lRightFrameNumber; i++) {
			// フレーム境界線の描画
			lTime = i * lTimeResolution;
			x = pPianoRollFrame->TimetoX (lTime);
			pDC->SelectObject (&penMeasure);
			pDC->MoveTo (x, rcClient.top);
			pDC->LineTo (x, rcClient.bottom);
		}
		pDC->SelectObject (pOldPen);
	}
	pDC->SelectObject (pOldPen);

	// グラフの描画
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	long lValue;
	i = 0;
	// すべてのトラックに対して
	forEachTrack (pMIDIData, pMIDITrack) {
		// そのトラックが可視状態であれば
		if (pPianoRollFrame->IsTrackVisible (i)) {
			long lOldTempo = 60000000 / 100;
			long lOldTempoTime = 0;
			CPen penControl (PS_SOLID, 1, MIDITrack_GetForeColor (pMIDITrack));
			CPen penSelected (PS_SOLID, 1, RGB(0,0,0));
			CPen* pOldPen = NULL;
			CPen* pLastTempoPen = NULL;
			// そのトラック内のすべてのイベントに対して
			forEachEvent (pMIDITrack, pMIDIEvent) {
				lTime = MIDIEvent_GetTime (pMIDIEvent);
				// イベント時刻がクライアント領域内にあれば
				if (lVisibleLeftTime <= lTime && lTime < lVisibleRightTime) {
					// ベロシティの描画
					if (MIDIEvent_IsNoteOn (pMIDIEvent)) {
						if (pPianoRollFrame->IsGraphVisible (1)) {
							pOldPen = pDC->SelectObject 
								(pSekaijuDoc->IsEventSelected (pMIDIEvent) ? &penSelected : &penControl);
							x = pPianoRollFrame->TimetoX (lTime);
							lValue = MIDIEvent_GetVelocity (pMIDIEvent);
							y = pPianoRollFrame->VeltoY (lValue);
							long y0 = pPianoRollFrame->VeltoY (0);
							pDC->MoveTo (x, y);
							pDC->LineTo (x, y0);
							pDC->SelectObject (pOldPen);
						}
					}
					// チャンネルアフタータッチの描画
					else if (MIDIEvent_IsChannelAftertouch (pMIDIEvent)) {
						if (pPianoRollFrame->IsGraphVisible (2)) {
							pOldPen = pDC->SelectObject 
								(pSekaijuDoc->IsEventSelected (pMIDIEvent) ? &penSelected : &penControl);
							x = pPianoRollFrame->TimetoX (lTime);
							lValue = MIDIEvent_GetValue (pMIDIEvent);
							y = pPianoRollFrame->VeltoY (lValue);
							long y0 = pPianoRollFrame->VeltoY (0);
							pDC->MoveTo (x, y);
							pDC->LineTo (x, y0);
							if (lValue == 0) {
								pDC->Ellipse (x - 2, y0 - 2, x + 2, y0 + 2);
							}
							pDC->SelectObject (pOldPen);
						}
					}
					// ピッチベンドの描画
					else if (MIDIEvent_IsPitchBend (pMIDIEvent)) {
						if (pPianoRollFrame->IsGraphVisible (3)) {
							pOldPen = pDC->SelectObject 
								(pSekaijuDoc->IsEventSelected (pMIDIEvent) ? &penSelected : &penControl);
							x = pPianoRollFrame->TimetoX (lTime);
							lValue = MIDIEvent_GetValue (pMIDIEvent);
							y = pPianoRollFrame->PitchBendtoY (lValue);
							long yc = pPianoRollFrame->PitchBendtoY (8192);
							pDC->MoveTo (x, y);
							pDC->LineTo (x, yc);
							if (lValue == 8192) {
								pDC->Ellipse (x - 2, yc - 2, x + 2, yc + 2);
							}
							pDC->SelectObject (pOldPen);
						}
					}
					// コントロールチェンジの描画
					else if (MIDIEvent_IsControlChange (pMIDIEvent)) {
						long lNumber = MIDIEvent_GetNumber (pMIDIEvent);
						if (pPianoRollFrame->IsGraphVisible (4 + lNumber)) {
							pOldPen = pDC->SelectObject 
								(pSekaijuDoc->IsEventSelected (pMIDIEvent) ? &penSelected : &penControl);
							x = pPianoRollFrame->TimetoX (lTime);
							lValue = MIDIEvent_GetValue (pMIDIEvent);
							y = pPianoRollFrame->VeltoY (lValue);
							long y0 = pPianoRollFrame->VeltoY (0);
							pDC->MoveTo (x, y);
							pDC->LineTo (x, y0);
							if (lValue == 0) {
								pDC->Ellipse (x - 2, y0 - 2, x + 2, y0 + 2);
							}
							pDC->SelectObject (pOldPen);
						}
					}
				}
				// テンポの描画(トラック0のみ)
				if (MIDIEvent_IsTempo (pMIDIEvent) && i == 0) {
					if (pPianoRollFrame->IsGraphVisible (0)) {
						pOldPen = pDC->SelectObject 
							(pSekaijuDoc->IsEventSelected (pMIDIEvent) ? &penSelected : &penControl);
						x = pPianoRollFrame->TimetoX (lTime);
						long xold = pPianoRollFrame->TimetoX (lOldTempoTime);
						lValue = MIDIEvent_GetTempo (pMIDIEvent);
						lValue = CLIP (1, lValue, 60000000);
						y = pPianoRollFrame->TempoBPMtoY (60000000 / lValue);
						long yold = pPianoRollFrame->TempoBPMtoY (60000000 / lOldTempo);
						pDC->MoveTo (xold, yold);
						pDC->LineTo (x, yold);
						pDC->LineTo (x, y);
						lOldTempo = lValue;
						lOldTempoTime = lTime;
						pLastTempoPen = pDC->SelectObject (pOldPen);
					}
				}
			}
			// テンポの最終横線を描画する(トラック0のみ)
			if (pPianoRollFrame->IsGraphVisible (0) && i == 0) {
				if (pLastTempoPen) {
					pOldPen = pDC->SelectObject (pLastTempoPen);
				}
				else {
					pOldPen = pDC->SelectObject (&penControl);
				}
				lTime = MIDIData_GetEndTime (pMIDIData);
				x = pPianoRollFrame->TimetoX (lTime);
				long xold = pPianoRollFrame->TimetoX (lOldTempoTime);
				long yold = pPianoRollFrame->TempoBPMtoY (60000000 / lOldTempo);
				pDC->MoveTo (xold, yold);
				pDC->LineTo (x, yold);
				pDC->SelectObject (pOldPen);
			}
		}
		i++;
	}

	// 現在位置の描画
	DrawCurLine (pDC);
	
	// キャプター中の場合のみ
	if (GetCapture () == this) {
		// マウストラッカーが必要な場合、マウストラッカーの描画
		if (m_lTempMode == 0x0401) {
			pDC->SetROP2 (R2_NOT);
			pDC->MoveTo (m_ptMouseDown.x, m_ptMouseDown.y);
			pDC->LineTo (m_ptMouseMove.x, m_ptMouseDown.y);
			pDC->MoveTo (m_ptMouseDown.x, m_ptMouseMove.y);
			pDC->LineTo (m_ptMouseMove.x, m_ptMouseMove.y);
			pDC->MoveTo (m_ptMouseDown.x, m_ptMouseDown.y);
			pDC->LineTo (m_ptMouseDown.x, m_ptMouseMove.y);
			pDC->MoveTo (m_ptMouseMove.x, m_ptMouseDown.y);
			pDC->LineTo (m_ptMouseMove.x, m_ptMouseMove.y);
			pDC->SetROP2 (R2_COPYPEN);
		}
		// ライントラッカーが必要な場合、ライントラッカーの描画
		else if (m_lTempMode == 0x0201) {
			pDC->SetROP2 (R2_NOT);
			pDC->MoveTo (m_ptMouseDown.x, m_ptMouseDown.y);
			pDC->LineTo (m_ptMouseMove.x, m_ptMouseMove.y);
			pDC->SetROP2 (R2_COPYPEN);
		}
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	Sleep (0);
}

// ビューの更新
void CPianoRollVelTimeView::OnUpdate (CView* pSender, LPARAM lHint, CObject* pHint) {
	// クリティカルセクションはロックされているものとする。
	if ((lHint & SEKAIJUDOC_PLAYSTARTED) || 
		(lHint & SEKAIJUDOC_RECORDSTARTED) ||
		(lHint & SEKAIJUDOC_POSITIONCHANGED)) {
		PostMessage (WM_TIMER, 0x11, NULL);
	}
	CSekaijuView::OnUpdate (pSender, lHint, pHint);
}

// ウィンドウクラスの変更
BOOL CPianoRollVelTimeView::PreCreateWindow (CREATESTRUCT& cs) {
	CView::PreCreateWindow (cs);
	cs.lpszClass = AfxRegisterWndClass 
		(CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS,
		NULL, // デフォルトカーソル無し
		(HBRUSH)::GetStockObject (WHITE_BRUSH), // デフォルト背景ブラシ
		NULL); // デフォルトアイコン無し
	return TRUE;
}



//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// ウィンドウ生成時
int CPianoRollVelTimeView::OnCreate (LPCREATESTRUCT lpCreateStruct) {
	SetTimer (0x11, 55, NULL);
	return CSekaijuView::OnCreate (lpCreateStruct);

}

// ウィンドウ破棄時
void CPianoRollVelTimeView::OnDestroy () {
	KillTimer (0x11);
	CSekaijuView::OnDestroy ();
}

// タイマー呼び出し時
void CPianoRollVelTimeView::OnTimer (UINT nIDEvent) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	// 現在演奏位置縦線の描画処理
	if (nIDEvent == 0x11) {
		if (pSekaijuDoc->m_pMIDIData && pSekaijuDoc->m_pMIDIClock) {
			pSekaijuDoc->m_theCriticalSection.Lock ();
			CDC* pDC = GetDC ();
			OnPrepareDC (pDC, NULL);
			CRect rcClient;
			GetClientRect (&rcClient);
			rcClient += CSize (pPianoRollFrame->GetTimeScrollPos (), pPianoRollFrame->GetVelScrollPos ());
			m_lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
			// 前回描画した時刻と異なっている場合のみ
			if (m_lOldTime != m_lCurTime) {
				EraseOldLine (pDC);
				DrawCurLine (pDC);
			}
			ReleaseDC (pDC);
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			::Sleep (0);
		}
	}
	// マウスキャプター中にクライアント領域をはみ出した場合の自動スクロール処理
	else if (nIDEvent == 0x21) {
		if (GetCapture () == this) {
			CClientDC dc (this);
			OnPrepareDC (&dc, NULL);
			CRect rcClient;
			GetClientRect (&rcClient);
			rcClient += CSize (pPianoRollFrame->GetTimeScrollPos (), pPianoRollFrame->GetVelScrollPos ());
			if (!rcClient.PtInRect (m_ptMouseMove)) {
				long lOldTimeScrollPos = pPianoRollFrame->GetTimeScrollPos ();
				long lOldVelScrollPos = pPianoRollFrame->GetVelScrollPos ();
				pSekaijuDoc->m_theCriticalSection.Lock ();
				if (m_ptMouseMove.x < rcClient.left) {
					pPianoRollFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_LINEUP,
						(LPARAM)(pPianoRollFrame->m_wndTimeScroll.GetSafeHwnd ()));
				}
				else if (m_ptMouseMove.x > rcClient.right) {
					pPianoRollFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_LINEDOWN,
						(LPARAM)(pPianoRollFrame->m_wndTimeScroll.GetSafeHwnd ()));
				}
				if (m_ptMouseMove.y < rcClient.top) {
					pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEUP,
						(LPARAM)(pPianoRollFrame->m_wndVelScroll.GetSafeHwnd ()));
				}
				else if (m_ptMouseMove.y > rcClient.bottom) {
					pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEDOWN,
						(LPARAM)(pPianoRollFrame->m_wndVelScroll.GetSafeHwnd ()));
				}
				WORD wX = (WORD)(m_ptMouseMove.x - lOldTimeScrollPos);
				WORD wY = (WORD)(m_ptMouseMove.y - lOldVelScrollPos);
				PostMessage (WM_MOUSEMOVE, (WPARAM)m_nMouseMoveFlags, (LPARAM)((wY << 16) | wX));
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				::Sleep (0);
			}
		}
	}
}

// キーが押された時
void CPianoRollVelTimeView::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	switch (nChar) {
	// ↑
	case VK_UP:
		pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEUP, 
			(LPARAM)(pPianoRollFrame->m_wndVelScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// ↓
	case VK_DOWN:
		pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEDOWN, 
			(LPARAM)(pPianoRollFrame->m_wndVelScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// PageUp
	case VK_PRIOR:
		pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_PAGEUP, 
			(LPARAM)(pPianoRollFrame->m_wndVelScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// PageDown
	case VK_NEXT:
		pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_PAGEDOWN, 
			(LPARAM)(pPianoRollFrame->m_wndVelScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// ←
	case VK_LEFT:
		pPianoRollFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_LINEUP, 
			(LPARAM)(pPianoRollFrame->m_wndTimeScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// →
	case VK_RIGHT:
		pPianoRollFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_LINEDOWN, 
			(LPARAM)(pPianoRollFrame->m_wndTimeScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// Home
	case VK_HOME:
		pPianoRollFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_PAGEUP, 
			(LPARAM)(pPianoRollFrame->m_wndTimeScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// End
	case VK_END:
		pPianoRollFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_PAGEDOWN, 
			(LPARAM)(pPianoRollFrame->m_wndTimeScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// ESC
	case VK_ESCAPE:
		// キャプター中の場合
		if (GetCapture () == this) {
			if (m_lTempMode = 0x0201) {
				m_pTempEvent = NULL;
				m_lTempMode = 0;
				KillTimer (0x21);
				ReleaseCapture ();
				//::SetCursor (pSekaijuApp->m_hCursorArrow);
				pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
			}
		}
		break;
	// Deleteキー
	case VK_DELETE:
		// 『編集(E)』-『削除』実行 (20090823追加)
		PostMessage (WM_COMMAND, ID_EDIT_DELETE, NULL);
		break;
	}
	return;
}

// マウス左ボタン押された時  
void CPianoRollVelTimeView::OnLButtonDown (UINT nFlags, CPoint point) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
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

	CClientDC dc (this);
	OnPrepareDC (&dc, NULL);
	CSize sizWndOrg (pPianoRollFrame->GetTimeScrollPos (), pPianoRollFrame->GetVelScrollPos ());
	point += sizWndOrg;
	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += sizWndOrg;
	ASSERT (m_pTempEvent == NULL);
	ASSERT (m_lTempMode == 0);
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lFormat = MIDIData_GetFormat (pMIDIData);
	m_lTempTrackIndex = pPianoRollFrame->GetCurTrackIndex ();
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (m_lTempTrackIndex);
	m_lTempTime = pPianoRollFrame->XtoTime (point.x);
	m_lTempTool = pPianoRollFrame->m_lCurTool;
	m_lTempChannel = pPianoRollFrame->GetCurChannel ();
	m_lTempChannel = CLIP (0, m_lTempChannel, 15);
	m_lTempVelocity = pPianoRollFrame->GetCurVelocity ();
	m_lTempVelocity = CLIP (1, m_lTempVelocity, 127);
	m_lTempSnap = pPianoRollFrame->GetCurSnap ();
	m_lTempSnap = CLIP (1, m_lTempSnap, MIDIData_GetTimeResolution (pMIDIData));
	m_lTempGraphKind = pPianoRollFrame->GetCurGraphKind ();
	m_lTempGraphSnap = pPianoRollFrame->GetCurGraphSnap ();
	m_lTempPortIndex = MIDITrack_GetOutputPort (pMIDITrack);
	m_pTempEvent = NULL;
	m_pLastEvent = NULL;
	MIDITrack* pTempTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	MIDIEvent* pLastEvent = NULL;
	MIDIEvent* pPrevEvent = NULL;
	MIDITrack* pCloneTrack = NULL;
	MIDIEvent* pCloneEvent = NULL;
	MIDIOut* pMIDIOut = NULL;
	MIDIStatus* pMIDIStatus = NULL;
	long lNewVel = pPianoRollFrame->YtoVel (point.y);
	long lNewValue = pPianoRollFrame->YtoVel (point.y);
	long lNewPitchBend = pPianoRollFrame->YtoPitchBend (point.y);
	long lNewTempoBPM = pPianoRollFrame->YtoTempoBPM (point.y);
	long lNewTime = pPianoRollFrame->XtoTime (point.x);
	CRect rcRegion;
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	switch (m_lTempTool) {
	// ペン
	case ID_PIANOROLL_PEN:
		// フォーマット1のMIDIデータで最初のトラックにグラフを置くのを禁止
		if (lFormat == 1 && m_lTempTrackIndex == 0 && m_lTempGraphKind != 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_GRAPHEVENT_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			break;
		}
		VERIFY (strHistoryName.LoadString (IDS_INSERT_GRAPH));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// グラフの種類がテンポの場合は強制的にトラック0に描画させる。
		if (m_lTempGraphKind == 0) {
			pTempTrack = pSekaijuDoc->GetTrack (0);
			if (pPianoRollFrame->IsTrackVisible (0) == FALSE) {
				pPianoRollFrame->SetTrackVisible (0);
				pPianoRollFrame->m_pKeyScaleView->Invalidate ();
				pPianoRollFrame->m_pKeyTimeView->Invalidate ();
				pPianoRollFrame->m_pVelScaleView->Invalidate ();
				pPianoRollFrame->m_pVelTimeView->Invalidate ();
			}
		}
		// グラフの種類がテンポ以外の場合は、現在選択中のトラックに描画させる。
		else {
			pTempTrack = pSekaijuDoc->GetTrack (m_lTempTrackIndex);
			if (pPianoRollFrame->IsTrackVisible (m_lTempTrackIndex) == FALSE) {
				pPianoRollFrame->SetTrackVisible (m_lTempTrackIndex);
				pPianoRollFrame->m_pKeyScaleView->Invalidate ();
				pPianoRollFrame->m_pKeyTimeView->Invalidate ();
				pPianoRollFrame->m_pVelScaleView->Invalidate ();
				pPianoRollFrame->m_pVelTimeView->Invalidate ();
			}
		}
		// 描画するグラフ種類の表示がOFF担っている場合ONにする。
		if (pPianoRollFrame->IsGraphVisible (m_lTempGraphKind) == FALSE) {
			pPianoRollFrame->SetGraphVisible (m_lTempGraphKind);
			pPianoRollFrame->m_pKeyScaleView->Invalidate ();
			pPianoRollFrame->m_pKeyTimeView->Invalidate ();
			pPianoRollFrame->m_pVelScaleView->Invalidate ();
			pPianoRollFrame->m_pVelTimeView->Invalidate ();
		}
		// マウス設置点に既に同種グラフがある場合
		ASSERT (pTempTrack);
		pTempEvent = pTempTrack->m_pFirstEvent;
		while (pTempEvent) {
			long lTempTime = MIDIEvent_GetTime (pTempEvent);
			// このイベントは時刻が一致している
			if (lTempTime == lNewTime) {
				// テンポの削除
				if (MIDIEvent_IsTempo (pTempEvent) && 
					m_lTempGraphKind == 0) {
					pPrevEvent = pTempEvent->m_pPrevEvent;
					pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
					MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
					pTempEvent = pPrevEvent ? pPrevEvent->m_pNextEvent : pTempTrack->m_pFirstEvent;
				}
				// ベロシティの変更
				else if (MIDIEvent_IsNoteOn (pTempEvent) && 
					MIDIEvent_IsNote (pTempEvent) &&
					m_lTempGraphKind == 1) {
					pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
					pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent);
					lNewVel = CLIP (1, lNewVel, 127);
					MIDIEvent_SetVelocity (pCloneEvent, lNewVel);
					pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent);
					pTempEvent = pCloneEvent->m_pNextEvent;
				}
				// チャンネルアフタータッチの削除
				else if (MIDIEvent_IsChannelAftertouch (pTempEvent) &&
					m_lTempGraphKind == 2) {
					pPrevEvent = pTempEvent->m_pPrevEvent;
					pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
					MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
					pTempEvent = pPrevEvent ? pPrevEvent->m_pNextEvent : pTempTrack->m_pFirstEvent;
				}
				// ピッチベンドの削除
				else if (MIDIEvent_IsPitchBend (pTempEvent) &&
					m_lTempGraphKind == 3) {
					pPrevEvent = pTempEvent->m_pPrevEvent;
					pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
					MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
					pTempEvent = pPrevEvent ? pPrevEvent->m_pNextEvent : pTempTrack->m_pFirstEvent;
				}
				// コントロールチェンジの削除
				else if (MIDIEvent_IsControlChange (pTempEvent) &&
					m_lTempGraphKind >= 4) {
					if (MIDIEvent_GetNumber (pTempEvent) ==
						m_lTempGraphKind - 4) {
						pPrevEvent = pTempEvent->m_pPrevEvent;
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
						MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
						pTempEvent = pPrevEvent ? pPrevEvent->m_pNextEvent : pTempTrack->m_pFirstEvent;
					}
					else {
						pTempEvent = pTempEvent->m_pNextEvent;
					}
				}
				// どれにも該当しない。
				else {
					pTempEvent = pTempEvent->m_pNextEvent;
				}
			}
			// このイベントは時刻が一致していない
			else {
				pTempEvent = pTempEvent->m_pNextEvent;
			}
		}
		pTempEvent = NULL;
		pLastEvent = NULL;
		
		// テンポの挿入
		if (m_lTempGraphKind == 0) {
			pLastEvent = MIDITrack_GetLastEvent (pTempTrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent);
					m_pLastEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent);
				}
			}
			if (m_lTempGraphSnap == 1) {
				lNewTime = CLIP (0, lNewTime, 0x7FFF0000);
				lNewTempoBPM = CLIP (1, lNewTempoBPM, 60000000);
				VERIFY (pTempEvent = MIDIEvent_CreateTempo (lNewTime, 60000000 / lNewTempoBPM));
				VERIFY (MIDITrack_InsertEvent (pTempTrack, pTempEvent));
				pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent);
				m_lLastInsertedTempoBPM = lNewTempoBPM;
			}
		}
		// チャンネルアフタータッチの挿入
		else if (m_lTempGraphKind == 2) {
			pLastEvent = MIDITrack_GetLastEvent (pTempTrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent);
					m_pLastEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent);
				}
			}
			if (m_lTempGraphSnap == 1) {
				lNewTime = CLIP (0, lNewTime, 0x7FFF0000);
				lNewValue = CLIP (0, lNewValue, 127);
				VERIFY (pTempEvent = MIDIEvent_CreateChannelAftertouch (lNewTime, m_lTempChannel, lNewValue));
				VERIFY (MIDITrack_InsertEvent (pTempTrack, pTempEvent));
				pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent);
				m_lLastInsertedValue = lNewValue;
			}
		}
		// ピッチベンドの挿入
		else if (m_lTempGraphKind == 3) {
			pLastEvent = MIDITrack_GetLastEvent (pTempTrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent);
					m_pLastEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent);
				}
			}
			if (m_lTempGraphSnap == 1) {
				lNewTime = CLIP (0, lNewTime, 0x7FFF0000);
				lNewPitchBend = CLIP (0, lNewPitchBend, 16383);
				VERIFY (pTempEvent = MIDIEvent_CreatePitchBend (lNewTime, m_lTempChannel, lNewPitchBend));
				VERIFY (MIDITrack_InsertEvent (pTempTrack, pTempEvent));
				pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent);
				m_lLastInsertedPitchBend = lNewPitchBend;
			}
		}
		// コントロールチェンジの挿入
		else if (m_lTempGraphKind >= 4) {
			pLastEvent = MIDITrack_GetLastEvent (pTempTrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent);
					m_pLastEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent);
				}
			}
			if (m_lTempGraphSnap == 1) {
				lNewTime = CLIP (0, lNewTime, 0x7FFF0000);
				lNewValue = CLIP (0, lNewValue, 127);
				VERIFY (pTempEvent = MIDIEvent_CreateControlChange 
					(lNewTime, m_lTempChannel, m_lTempGraphKind - 4, lNewValue));
				VERIFY (MIDITrack_InsertEvent (pTempTrack, pTempEvent));
				pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent);
				m_lLastInsertedValue = lNewValue;
			}
		}
		rcRegion = CRect (point.x, rcClient.top, point.x + 1, rcClient.bottom);
		rcRegion.NormalizeRect ();
		rcRegion.InflateRect (1, 1);
		InvalidateRect (rcRegion - sizWndOrg);
		m_lTempMode = 0x0101;
		::SetCursor (pSekaijuApp->m_hCursorDraw);
		pPianoRollFrame->m_bAutoPageUpdate = FALSE;
		SetTimer (0x21, 55, NULL);
		SetCapture ();
		break;
	// 線
	case ID_PIANOROLL_LINE:
		// フォーマット1のMIDIデータで最初のトラックにグラフを置くのを禁止
		if (lFormat == 1 && m_lTempTrackIndex == 0 && m_lTempGraphKind != 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_GRAPHEVENT_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			break;
		}
		VERIFY (strHistoryName.LoadString (IDS_INSERT_GRAPH));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// グラフの種類がテンポの場合は強制的にトラック0に描画させる。
		if (m_lTempGraphKind == 0) {
			pTempTrack = pSekaijuDoc->GetTrack (0);
			if (pPianoRollFrame->IsTrackVisible (0) == FALSE) {
				pPianoRollFrame->SetTrackVisible (0);
				pPianoRollFrame->m_pKeyScaleView->Invalidate ();
				pPianoRollFrame->m_pKeyTimeView->Invalidate ();
				pPianoRollFrame->m_pVelScaleView->Invalidate ();
				pPianoRollFrame->m_pVelTimeView->Invalidate ();
			}
		}
		// グラフの種類がテンポ以外の場合は、現在選択中のトラックに描画させる。
		else {
			pTempTrack = pSekaijuDoc->GetTrack (m_lTempTrackIndex);
			if (pPianoRollFrame->IsTrackVisible (m_lTempTrackIndex) == FALSE) {
				pPianoRollFrame->SetTrackVisible (m_lTempTrackIndex);
				pPianoRollFrame->m_pKeyScaleView->Invalidate ();
				pPianoRollFrame->m_pKeyTimeView->Invalidate ();
				pPianoRollFrame->m_pVelScaleView->Invalidate ();
				pPianoRollFrame->m_pVelTimeView->Invalidate ();
			}
		}
		// 描画するグラフ種類の表示がOFF担っている場合ONにする。
		if (pPianoRollFrame->IsGraphVisible (m_lTempGraphKind) == FALSE) {
			pPianoRollFrame->SetGraphVisible (m_lTempGraphKind);
			pPianoRollFrame->m_pKeyScaleView->Invalidate ();
			pPianoRollFrame->m_pKeyTimeView->Invalidate ();
			pPianoRollFrame->m_pVelScaleView->Invalidate ();
			pPianoRollFrame->m_pVelTimeView->Invalidate ();
		}
		m_lTempMode = 0x0201;
		::SetCursor (pSekaijuApp->m_hCursorLine);
		pPianoRollFrame->m_bAutoPageUpdate = FALSE;
		SetTimer (0x21, 55, NULL);
		SetCapture ();
		break;

	// 消しゴム
	case ID_PIANOROLL_ERASER:
		// フォーマット1のMIDIデータで最初のトラックにグラフを消すのを禁止
		if (lFormat == 1 && m_lTempTrackIndex == 0 && m_lTempGraphKind != 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_DELETE_GRAPHEVENT_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			break;
		}
		VERIFY (strHistoryName.LoadString (IDS_DELETE_GRAPH));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// グラフの種類がテンポの場合は強制的にトラック0に描画させる。
		if (m_lTempGraphKind == 0) {
			pTempTrack = pSekaijuDoc->GetTrack (0);
			if (pPianoRollFrame->IsTrackVisible (0) == FALSE) {
				pPianoRollFrame->SetTrackVisible (0);
				pPianoRollFrame->m_pKeyScaleView->Invalidate ();
				pPianoRollFrame->m_pKeyTimeView->Invalidate ();
				pPianoRollFrame->m_pVelScaleView->Invalidate ();
				pPianoRollFrame->m_pVelTimeView->Invalidate ();
			}
		}
		// グラフの種類がテンポ以外の場合は、現在選択中のトラックに描画させる。
		else {
			pTempTrack = pSekaijuDoc->GetTrack (m_lTempTrackIndex);
			if (pPianoRollFrame->IsTrackVisible (m_lTempTrackIndex) == FALSE) {
				pPianoRollFrame->SetTrackVisible (m_lTempTrackIndex);
				pPianoRollFrame->m_pKeyScaleView->Invalidate ();
				pPianoRollFrame->m_pKeyTimeView->Invalidate ();
				pPianoRollFrame->m_pVelScaleView->Invalidate ();
				pPianoRollFrame->m_pVelTimeView->Invalidate ();
			}
		}
		// 描画するグラフ種類の表示がOFF担っている場合ONにする。
		if (pPianoRollFrame->IsGraphVisible (m_lTempGraphKind) == FALSE) {
			pPianoRollFrame->SetGraphVisible (m_lTempGraphKind);
			pPianoRollFrame->m_pKeyScaleView->Invalidate ();
			pPianoRollFrame->m_pKeyTimeView->Invalidate ();
			pPianoRollFrame->m_pVelScaleView->Invalidate ();
			pPianoRollFrame->m_pVelTimeView->Invalidate ();
		}
		// マウス設置点に既に同種グラフがある場合
		ASSERT (pTempTrack);
		pTempEvent = pTempTrack->m_pFirstEvent;
		while (pTempEvent) {
			long lTempTime = MIDIEvent_GetTime (pTempEvent);
			// このイベントは時刻が一致している
			if (lTempTime == lNewTime) {
				// テンポの削除
				if (MIDIEvent_IsTempo (pTempEvent) && 
					m_lTempGraphKind == 0) {
					pPrevEvent = pTempEvent->m_pPrevEvent;
					pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
					MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
					pTempEvent = pPrevEvent ? pPrevEvent->m_pNextEvent : pTempTrack->m_pFirstEvent;
				}
				// ベロシティの変更
				else if (MIDIEvent_IsNoteOn (pTempEvent) && 
					MIDIEvent_IsNote (pTempEvent) &&
					m_lTempGraphKind == 1) {
					pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
					pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent);
					lNewVel = CLIP (1, lNewVel, 127);
					MIDIEvent_SetVelocity (pTempEvent, lNewVel);
					pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent);
					pTempEvent = pCloneEvent->m_pNextEvent;
				}
				// チャンネルアフタータッチの削除
				else if (MIDIEvent_IsChannelAftertouch (pTempEvent) &&
					m_lTempGraphKind == 2) {
					pPrevEvent = pTempEvent->m_pPrevEvent;
					pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
					MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
					pTempEvent = pPrevEvent ? pPrevEvent->m_pNextEvent : pTempTrack->m_pFirstEvent;
				}
				// ピッチベンドの削除
				else if (MIDIEvent_IsPitchBend (pTempEvent) &&
					m_lTempGraphKind == 3) {
					pPrevEvent = pTempEvent->m_pPrevEvent;
					pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
					MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
					pTempEvent = pPrevEvent ? pPrevEvent->m_pNextEvent : pTempTrack->m_pFirstEvent;
				}
				// コントロールチェンジの削除
				else if (MIDIEvent_IsControlChange (pTempEvent) &&
					m_lTempGraphKind >= 4) {
					if (MIDIEvent_GetNumber (pTempEvent) ==
						m_lTempGraphKind - 4) {
						pPrevEvent = pTempEvent->m_pPrevEvent;
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
						MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
						pTempEvent = pPrevEvent ? pPrevEvent->m_pNextEvent : pTempTrack->m_pFirstEvent;
					}
					else {
						pTempEvent = pTempEvent->m_pNextEvent;
					}
				}
				// いずれにも該当しない
				else {
					pTempEvent = pTempEvent->m_pNextEvent;
				}
			}
			// このイベントは時刻が一致していない
			else {
				pTempEvent = pTempEvent->m_pNextEvent;
			}
		}
		m_lTempMode = 0x0301;
		::SetCursor (pSekaijuApp->m_hCursorEraser);
		pPianoRollFrame->m_bAutoPageUpdate = FALSE;
		SetTimer (0x21, 55, NULL);
		SetCapture ();
		break;

	// 選択
	case ID_PIANOROLL_SELECT:
		VERIFY (strHistoryName.LoadString (IDS_SELECT_DESELECT_GRAPH));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// 旧選択イベントの選択解除(Shiftが押されていない場合かつCtrlが押されていない場合のみ)
		if ((nFlags & MK_SHIFT) == 0 && (nFlags & MK_CONTROL) == 0) {
			pSekaijuDoc->SelectNoObject (pCurHistoryUnit);
		}
		Invalidate ();
		m_lTempMode = 0x0401;
		dc.SetROP2 (R2_NOT);
		dc.SetPixel (point, RGB (0, 0, 0));
		dc.SetROP2 (R2_COPYPEN);
		::SetCursor (pSekaijuApp->m_hCursorSelect);
		pPianoRollFrame->m_bAutoPageUpdate = FALSE;
		SetTimer (0x21, 55, NULL);
		SetCapture ();
		break;

	// スクラブ
	case ID_PIANOROLL_SPEAKER:
		m_lTempMode = 0x0501;
		if (pSekaijuApp->m_bPlaying) {
			pPianoRollFrame->SendMessage (WM_COMMAND, (WPARAM)ID_CONTROL_PLAY, (LPARAM)0);
		}
		pSekaijuApp->SetPlayPosition (pSekaijuDoc, m_lTempTime);
		pSekaijuApp->SendDifferentStatus (SDS_ALL);
		pPianoRollFrame->m_bAutoPageUpdate = FALSE;
		::SetCursor (pSekaijuApp->m_hCursorSpeaker);
		SetTimer (0x21, 55, NULL);
		SetCapture ();
		break;
	}
	m_ptMouseDown = m_ptMouseMove = point;
	m_nMouseDownFlags = m_nMouseMoveFlags = nFlags;
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// マウス右ボタン押された時  
void CPianoRollVelTimeView::OnRButtonDown (UINT nFlags, CPoint point) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
}

// マウス左ボタン離されたとき
void CPianoRollVelTimeView::OnLButtonUp (UINT nFlags, CPoint point) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
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

	CClientDC dc (this);
	OnPrepareDC (&dc, NULL);
	CSize sizWndOrg (pPianoRollFrame->GetTimeScrollPos (), pPianoRollFrame->GetVelScrollPos ());
	point += sizWndOrg;
	long lDownVel = pPianoRollFrame->YtoVel (m_ptMouseDown.y);
	long lOldVel = pPianoRollFrame->YtoVel (m_ptMouseMove.y);
	long lDownValue = pPianoRollFrame->YtoVel (m_ptMouseDown.y);
	long lOldValue = pPianoRollFrame->YtoVel (m_ptMouseMove.y);
	long lDownTempoBPM = pPianoRollFrame->YtoTempoBPM (m_ptMouseDown.y);
	long lOldTempoBPM = pPianoRollFrame->YtoTempoBPM (m_ptMouseMove.y);
	long lDownPitchBend = pPianoRollFrame->YtoPitchBend (m_ptMouseDown.y);
	long lOldPitchBend = pPianoRollFrame->YtoPitchBend (m_ptMouseMove.y);
	long lDownTime = pPianoRollFrame->XtoTime (m_ptMouseDown.x);
	long lOldTime = pPianoRollFrame->XtoTime (m_ptMouseMove.x);
	CRect rcRegion;
	// キャプター中
	if (GetCapture () == this) {
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (m_lTempTrackIndex);
		pSekaijuDoc->m_theCriticalSection.Lock ();
		long lTempTime = 0;
		long lMinTime = __min (lDownTime, lOldTime);
		long lMaxTime = __max (lDownTime, lOldTime);
		long lTempX = 0;
		long lMinX = __min (m_ptMouseDown.x, m_ptMouseMove.x);
		long lMaxX = __max (m_ptMouseDown.x, m_ptMouseMove.x);
		long lMinValue = __min (lDownValue, lOldValue);
		long lMaxValue = __max (lDownValue, lOldValue);
		long lMinTempoBPM = __min (lDownTempoBPM, lOldTempoBPM);
		long lMaxTempoBPM = __max (lDownTempoBPM, lOldTempoBPM);
		long lMinPitchBend = __min (lDownPitchBend, lOldPitchBend);
		long lMaxPitchBend = __max (lDownPitchBend, lOldPitchBend);
		long i = 0;
		long j = 0;
		long jMin = lMinTime / m_lTempGraphSnap;
		long jMax = lMaxTime / m_lTempGraphSnap;
	
		MIDITrack* pTempTrack = NULL;
		MIDIEvent* pTempEvent = NULL;
		MIDIEvent* pLastEvent = NULL;
		MIDIEvent* pPrevEvent = NULL;
		MIDIEvent* pCloneEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		switch (m_lTempTool) {
		// ペン
		case ID_PIANOROLL_PEN:
			if (m_pLastEvent) { // TODO:20080802 m_pLastEvent==0xcdcdcdcd
				if (MIDIEvent_IsEndofTrack (m_pLastEvent)) {
					pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, m_pLastEvent);
				}
			}
			pSekaijuDoc->SetModifiedFlag (TRUE);
			break;
		// 線
		case ID_PIANOROLL_LINE:
			pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
			if (m_lTempGraphKind == 0) {
				pTempTrack = pSekaijuDoc->GetTrack (0);
			}
			else {
				pTempTrack = pSekaijuDoc->GetTrack (m_lTempTrackIndex);
			}
			ASSERT (pTempTrack);
			// グラフの種類がベロシティ以外の場合EOTの履歴記録準備
			m_pLastEvent = NULL;
			if (m_lTempGraphKind != 1) {
				pLastEvent = MIDITrack_GetLastEvent (pTempTrack);
				if (pLastEvent) {
					if (MIDIEvent_IsEndofTrack (pLastEvent)) {
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent);
						m_pLastEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent);
					}
				}
			}
			// 開始点～終了点にある同種のグラフを削除
			//pTempEvent = pTempTrack->m_pLastEvent;	
			// すべてのイベントに対して(後方から評価)
			pTempEvent = pTempTrack->m_pFirstEvent;
			while (pTempEvent) {
				pPrevEvent = pTempEvent->m_pPrevEvent;
				lTempTime = MIDIEvent_GetTime (pTempEvent);
				// このイベントは時刻が開始点～終了点間にある場合
				if (lMinTime <= lTempTime && lTempTime <= lMaxTime) {
					// テンポの削除
					if (MIDIEvent_IsTempo (pTempEvent) && 
						m_lTempGraphKind == 0) {
						pPrevEvent = pTempEvent->m_pPrevEvent;
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
						MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
						pTempEvent = pPrevEvent ? pPrevEvent->m_pNextEvent : pTempTrack->m_pFirstEvent;
					}
					// チャンネルアフタータッチの削除
					else if (MIDIEvent_IsChannelAftertouch (pTempEvent) && 
						m_lTempGraphKind == 2) {
						pPrevEvent = pTempEvent->m_pPrevEvent;
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
						MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
						pTempEvent = pPrevEvent ? pPrevEvent->m_pNextEvent : pTempTrack->m_pFirstEvent;
					}
					// ピッチベンドの削除
					else if (MIDIEvent_IsPitchBend (pTempEvent) &&
						m_lTempGraphKind == 3) {
						pPrevEvent = pTempEvent->m_pPrevEvent;
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
						MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
						pTempEvent = pPrevEvent ? pPrevEvent->m_pNextEvent : pTempTrack->m_pFirstEvent;
					}
					// コントロールチェンジの削除
					else if (MIDIEvent_IsControlChange (pTempEvent) &&
						m_lTempGraphKind >= 4) {
						if (MIDIEvent_GetNumber (pTempEvent) ==
							m_lTempGraphKind - 4) {
							pPrevEvent = pTempEvent->m_pPrevEvent;
							pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
							MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
							pTempEvent = pPrevEvent ? pPrevEvent->m_pNextEvent : pTempTrack->m_pFirstEvent;
						}
						else {
							pTempEvent = pTempEvent->m_pNextEvent;
						}
					}
					// いずれにも該当しない
					else {
						pTempEvent = pTempEvent->m_pNextEvent;
					}
				}
				// このイベントは時刻が開始点～終了点の外側にある場合
				else {
					pTempEvent = pTempEvent->m_pNextEvent;
				}
			}
			// テンポの直線追加
			if (m_lTempGraphKind == 0) {
				long lTempTempoBPM = 0;
				long lTempOldTempoBPM = -1;
				if (m_lTempGraphSnap == 1) {
					for (lTempX = lMinX; lTempX <= lMaxX; lTempX++) {
						lTempTime = pPianoRollFrame->XtoTime (lTempX);
						if (lOldTime == lDownTime) {
							lTempTempoBPM = lOldTempoBPM;
						}
						else {
							lTempTempoBPM = lDownTempoBPM + (lTempTime - lDownTime) * 
								(lOldTempoBPM - lDownTempoBPM) / (lOldTime - lDownTime);
						}
						lTempTempoBPM = CLIP (1, lTempTempoBPM, 60000000);
						if (lTempTempoBPM != lTempOldTempoBPM) {
							VERIFY (pTempEvent = MIDIEvent_CreateTempo (lTempTime, 60000000 / lTempTempoBPM));
							VERIFY (MIDITrack_InsertEvent (pTempTrack, pTempEvent));
							pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent);
						}
						lTempOldTempoBPM = lTempOldTempoBPM;
					}
				}
				else if (m_lTempGraphSnap >= 2) {
					for (j = jMin; j <= jMax + 1; j++) {
						lTempTime = j * m_lTempGraphSnap;
						if (lMinTime <= lTempTime && lTempTime <= lMaxTime) {
							if (lOldTime == lDownTime) {
								lTempTempoBPM = lOldTempoBPM;
							}
							else {
								lTempTempoBPM = lDownTempoBPM + (lTempTime - lDownTime) * 
									(lOldTempoBPM - lDownTempoBPM) / (lOldTime - lDownTime);
							}
							lTempTempoBPM = CLIP (1, lTempTempoBPM, 60000000);
							if (lTempTempoBPM != lTempOldTempoBPM) {
								VERIFY (pTempEvent = MIDIEvent_CreateTempo (lTempTime, 60000000 / lTempTempoBPM));
								VERIFY (MIDITrack_InsertEvent (pTempTrack, pTempEvent));
								pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent);
							}
							lTempOldTempoBPM = lTempOldTempoBPM;
						}
					}
				}
				if (m_pLastEvent) {
					if (MIDIEvent_IsEndofTrack (m_pLastEvent)) {
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, m_pLastEvent);
					}
				}
			}
			// ベロシティの直線修正
			else if (m_lTempGraphKind == 1) {
				forEachEvent (pTempTrack, pTempEvent) {
					long lTempTime = MIDIEvent_GetTime (pTempEvent);
					if (lMinTime <= lTempTime && lTempTime <= lMaxTime) {
						//if (MIDIEvent_IsNoteOn (pTempEvent) && MIDIEvent_IsNote (pTempEvent)) {
						if (MIDIEvent_IsNoteOn (pTempEvent) && pTempEvent->m_pPrevCombinedEvent == NULL) {
							long lTempVel = 0;
							if (lOldTime == lDownTime) {
								lTempVel = lOldVel;
							}
							else {
								lTempVel = lDownVel + (lTempTime - lDownTime) * 
								(lOldVel - lDownVel) / (lOldTime - lDownTime);
							}
							lTempVel = CLIP (1, lTempVel, 127);
							pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
							VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent));
							MIDIEvent_SetVelocity (pCloneEvent, lTempVel);
							pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent);
							pTempEvent = pCloneEvent;
						}
					}
				}
			}
			// チャンネルアフタータッチの直線追加 
			else if (m_lTempGraphKind == 2) {
				long lTempValue = 0;
				long lTempOldValue = -1;
				if (m_lTempGraphSnap == 1) {
					for (lTempX = lMinX; lTempX <= lMaxX; lTempX++) {
						lTempTime = pPianoRollFrame->XtoTime (lTempX);
						if (lOldTime == lDownTime) {
							lTempValue = lOldValue;
						}
						else {
							lTempValue = lDownValue + (lTempTime - lDownTime) * 
								(lOldValue - lDownValue) / (lOldTime - lDownTime);
						}
						lTempValue = CLIP (0, lTempValue, 127);
						if (lTempValue != lTempOldValue) {
							VERIFY (pTempEvent = MIDIEvent_CreateChannelAftertouch 
								(lTempTime, m_lTempChannel, lTempValue));
							VERIFY (MIDITrack_InsertEvent (pTempTrack, pTempEvent));
							pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent);
						}
						lTempOldValue = lTempValue;
					}
				}
				else if (m_lTempGraphSnap >= 2) {
					for (j = jMin; j <= jMax + 1; j++) {
						lTempTime = j * m_lTempGraphSnap;
						if (lMinTime <= lTempTime && lTempTime <= lMaxTime) {
							if (lOldTime == lDownTime) {
								lTempValue = lOldValue;
							}
							else {
								lTempValue = lDownValue + (lTempTime - lDownTime) * 
									(lOldValue - lDownValue) / (lOldTime - lDownTime);
							}
							lTempValue = CLIP (0, lTempValue, 127);
							if (lTempValue != lTempOldValue) {
								VERIFY (pTempEvent = MIDIEvent_CreateChannelAftertouch 
									(lTempTime, m_lTempChannel, lTempValue));
								VERIFY (MIDITrack_InsertEvent (pTempTrack, pTempEvent));
								pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent);
							}
							lTempOldValue = lTempValue;
						}
					}
				}
				if (m_pLastEvent) {
					if (MIDIEvent_IsEndofTrack (m_pLastEvent)) {
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, m_pLastEvent);
					}
				}
			}
			// ピッチベンドの直線追加 
			else if (m_lTempGraphKind == 3) {
				long lTempPitchBend = 0;
				long lTempOldPitchBend = -1;
				if (m_lTempGraphSnap == 1) {
					for (lTempX = lMinX; lTempX <= lMaxX; lTempX++) {
						lTempTime = pPianoRollFrame->XtoTime (lTempX);
						if (lOldTime == lDownTime) {
							lTempPitchBend = lOldPitchBend;
						}
						else {
							lTempPitchBend = lDownPitchBend + (lTempTime - lDownTime) * 
								(lOldPitchBend - lDownPitchBend) / (lOldTime - lDownTime);
						}
						lTempPitchBend = CLIP (0, lTempPitchBend, 16483);
						if (lTempPitchBend != lTempOldPitchBend) {
							VERIFY (pTempEvent = MIDIEvent_CreatePitchBend
								(lTempTime, m_lTempChannel, lTempPitchBend));
							VERIFY (MIDITrack_InsertEvent (pTempTrack, pTempEvent));
							pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent);
						}
						lTempOldPitchBend = lTempPitchBend;
					}
				}
				else if (m_lTempGraphSnap >= 2) {
					for (j = jMin; j <= jMax + 1; j++) {
						lTempTime = j * m_lTempGraphSnap;
						if (lMinTime <= lTempTime && lTempTime <= lMaxTime) {
							if (lOldTime == lDownTime) {
								lTempPitchBend = lOldPitchBend;
							}
							else {
								lTempPitchBend = lDownPitchBend + (lTempTime - lDownTime) * 
									(lOldPitchBend - lDownPitchBend) / (lOldTime - lDownTime);
							}
							lTempPitchBend = CLIP (0, lTempPitchBend, 16483);
							if (lTempPitchBend != lTempOldPitchBend) {
								VERIFY (pTempEvent = MIDIEvent_CreatePitchBend
									(lTempTime, m_lTempChannel, lTempPitchBend));
								VERIFY (MIDITrack_InsertEvent (pTempTrack, pTempEvent));
								pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent);
							}
							lTempOldPitchBend = lTempPitchBend;
						}
					}
				}
				if (m_pLastEvent) {
					if (MIDIEvent_IsEndofTrack (m_pLastEvent)) {
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, m_pLastEvent);
					}
				}
			}
			// CCイベントの直線追加 
			else if (m_lTempGraphKind >= 4) {
				long lTempValue = 0;
				long lTempOldValue = -1;
				if (m_lTempGraphSnap == 1) {
					for (lTempX = lMinX; lTempX <= lMaxX; lTempX++) {
						lTempTime = pPianoRollFrame->XtoTime (lTempX);
						if (lOldTime == lDownTime) {
							lTempValue = lOldValue;
						}
						else {
							lTempValue = lDownValue + (lTempTime - lDownTime) * 
								(lOldValue - lDownValue) / (lOldTime - lDownTime);
						}
						lTempValue = CLIP (0, lTempValue, 127);
						if (lTempValue != lTempOldValue) {
							VERIFY (pTempEvent = MIDIEvent_CreateControlChange
								(lTempTime, m_lTempChannel, m_lTempGraphKind - 4, lTempValue));
							VERIFY (MIDITrack_InsertEvent (pTempTrack, pTempEvent));
							pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent);
						}
						lTempOldValue = lTempValue;
					}
				}
				else if (m_lTempGraphSnap >= 2) {
					for (j = jMin; j <= jMax + 1; j++) {
						lTempTime = j * m_lTempGraphSnap;
						if (lMinTime <= lTempTime && lTempTime <= lMaxTime) {
							if (lOldTime == lDownTime) {
								lTempValue = lOldValue;
							}
							else {
								lTempValue = lDownValue + (lTempTime - lDownTime) * 
									(lOldValue - lDownValue) / (lOldTime - lDownTime);
							}
							lTempValue = CLIP (0, lTempValue, 127);
							if (lTempValue != lTempOldValue) {
								VERIFY (pTempEvent = MIDIEvent_CreateControlChange
									(lTempTime, m_lTempChannel, m_lTempGraphKind - 4, lTempValue));
								VERIFY (MIDITrack_InsertEvent (pTempTrack, pTempEvent));
								pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent);
							}
							lTempOldValue = lTempValue;
						}
					} 
				}
				if (m_pLastEvent) {
					if (MIDIEvent_IsEndofTrack (m_pLastEvent)) {
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, m_pLastEvent);
					}
				}
			}
			pSekaijuDoc->SetModifiedFlag (TRUE);
			break;
		// 消しゴム
		case ID_PIANOROLL_ERASER:
			pSekaijuDoc->SetModifiedFlag (TRUE);
			break;
		// 選択
		case ID_PIANOROLL_SELECT:
			pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
			// 新規矩形内部選択(左→右ドラッグ)
			if (m_ptMouseMove.x >= m_ptMouseDown.x) {
				forEachTrack (pMIDIData, pTempTrack) {
					if (pPianoRollFrame->IsTrackVisible (i)) {
						forEachEvent (pTempTrack, pTempEvent) {
							long lTime = MIDIEvent_GetTime (pTempEvent);
							if (lMinTime <= lTime && lTime <= lMaxTime) {
								if (MIDIEvent_IsTempo (pTempEvent)) {
									if (pPianoRollFrame->IsGraphVisible (0)) {
										long lTempoBPM = 60000000 / MIDIEvent_GetTempo (pTempEvent);
										if (lMinTempoBPM <= lTempoBPM && lTempoBPM <= lMaxTempoBPM) {
											pCloneEvent = pSekaijuDoc->SelectEvent 
												(pTempEvent, 1, pCurHistoryUnit);
											if (pCloneEvent) {
												pTempEvent = pCloneEvent;
											}
										}
									}
								}
								else if (MIDIEvent_IsNoteOn (pTempEvent)) {
									if (pPianoRollFrame->IsGraphVisible (1)) {
										long lValue = MIDIEvent_GetVelocity (pTempEvent);
										if (lMinValue <= lValue && lValue <= lMaxValue) {
											pCloneEvent = pSekaijuDoc->SelectEvent 
												(pTempEvent, 1, pCurHistoryUnit);
											if (pCloneEvent) {
												pTempEvent = pCloneEvent;
											}
										}
									}
								}
								else if (MIDIEvent_IsChannelAftertouch (pTempEvent)) {
									if (pPianoRollFrame->IsGraphVisible (2)) {
										long lValue = MIDIEvent_GetValue (pTempEvent);
										if (lMinValue <= lValue && lValue <= lMaxValue) {
											pCloneEvent = pSekaijuDoc->SelectEvent 
												(pTempEvent, 1, pCurHistoryUnit);
											if (pCloneEvent) {
												pTempEvent = pCloneEvent;
											}
										}
									}
								}
								else if (MIDIEvent_IsPitchBend (pTempEvent)) {
									if (pPianoRollFrame->IsGraphVisible (3)) {
										long lPitchBend = MIDIEvent_GetValue (pTempEvent);
										if (lMinPitchBend <= lPitchBend && lPitchBend <= lMaxPitchBend) {
											pCloneEvent = pSekaijuDoc->SelectEvent 
												(pTempEvent, 1, pCurHistoryUnit);
											if (pCloneEvent) {
												pTempEvent = pCloneEvent;
											}
										}
									}
								}
								else if (MIDIEvent_IsControlChange (pTempEvent)) {
									long lNumber = MIDIEvent_GetNumber (pTempEvent);
									if (pPianoRollFrame->IsGraphVisible (lNumber + 4)) {
										long lValue = MIDIEvent_GetValue (pTempEvent);
										if (lMinValue <= lValue && lValue <= lMaxValue) {
											pCloneEvent = pSekaijuDoc->SelectEvent 
												(pTempEvent, 1, pCurHistoryUnit);
											if (pCloneEvent) {
												pTempEvent = pCloneEvent;
											}
										}
									}
								}
							}
						}
					}
					i++;
				}
			}
			// 新規矩形接触選択(右→左ドラッグ) // 20100503追加
			else {
				forEachTrack (pMIDIData, pTempTrack) {
					if (pPianoRollFrame->IsTrackVisible (i)) {
						forEachEvent (pTempTrack, pTempEvent) {
							long lTime = MIDIEvent_GetTime (pTempEvent);
							if (lMinTime <= lTime && lTime <= lMaxTime) {
								if (MIDIEvent_IsTempo (pTempEvent)) {
									if (pPianoRollFrame->IsGraphVisible (0)) {
										long lTempoBPM = 60000000 / MIDIEvent_GetTempo (pTempEvent);
										if (lTempoBPM >= lMinTempoBPM) {
											pCloneEvent = pSekaijuDoc->SelectEvent 
												(pTempEvent, 1, pCurHistoryUnit);
											if (pCloneEvent) {
												pTempEvent = pCloneEvent;
											}
										}
									}
								}
								else if (MIDIEvent_IsNoteOn (pTempEvent)) {
									if (pPianoRollFrame->IsGraphVisible (1)) {
										long lValue = MIDIEvent_GetVelocity (pTempEvent);
										if (lValue >= lMinValue) {
											pCloneEvent = pSekaijuDoc->SelectEvent 
												(pTempEvent, 1, pCurHistoryUnit);
											if (pCloneEvent) {
												pTempEvent = pCloneEvent;
											}
										}
									}
								}
								else if (MIDIEvent_IsChannelAftertouch (pTempEvent)) {
									if (pPianoRollFrame->IsGraphVisible (2)) {
										long lValue = MIDIEvent_GetValue (pTempEvent);
										if (lValue >= lMinValue) {
											pCloneEvent = pSekaijuDoc->SelectEvent 
												(pTempEvent, 1, pCurHistoryUnit);
											if (pCloneEvent) {
												pTempEvent = pCloneEvent;
											}
										}
									}
								}
								else if (MIDIEvent_IsPitchBend (pTempEvent)) {
									if (pPianoRollFrame->IsGraphVisible (3)) {
										long lPitchBend = MIDIEvent_GetValue (pTempEvent);
										if (lPitchBend >= lMinPitchBend) {
											pCloneEvent = pSekaijuDoc->SelectEvent 
												(pTempEvent, 1, pCurHistoryUnit);
											if (pCloneEvent) {
												pTempEvent = pCloneEvent;
											}
										}
									}
								}
								else if (MIDIEvent_IsControlChange (pTempEvent)) {
									long lNumber = MIDIEvent_GetNumber (pTempEvent);
									if (pPianoRollFrame->IsGraphVisible (lNumber + 4)) {
										long lValue = MIDIEvent_GetValue (pTempEvent);
										if (lValue >= lMinValue) {
											pCloneEvent = pSekaijuDoc->SelectEvent 
												(pTempEvent, 1, pCurHistoryUnit);
											if (pCloneEvent) {
												pTempEvent = pCloneEvent;
											}
										}
									}
								}
							}
						}
					}
					i++;
				}
			}
			break;
		// スクラブ
		case ID_PIANOROLL_SPEAKER:
			pSekaijuApp->SendAllNoteOff ();
			pSekaijuApp->SendAllSoundOff ();
			break;
		}
		m_pTempEvent = NULL;
		m_lTempMode = 0;
		KillTimer (0x21);
		ReleaseCapture ();
		//::SetCursor (pSekaijuApp->m_hCursorArrow);
		pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}
}

// マウス右ボタン離されたとき
void CPianoRollVelTimeView::OnRButtonUp (UINT nFlags, CPoint point) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	
}

// マウスが動かされたとき
void CPianoRollVelTimeView::OnMouseMove (UINT nFlags, CPoint point) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
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

	CClientDC dc (this);
	OnPrepareDC (&dc, NULL);
	CSize sizWndOrg (pPianoRollFrame->GetTimeScrollPos (), pPianoRollFrame->GetVelScrollPos ());
	point += sizWndOrg;
	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += sizWndOrg;
	long lOldVel = pPianoRollFrame->YtoVel (m_ptMouseMove.y);
	long lNewVel = pPianoRollFrame->YtoVel (point.y);
	long lOldValue = pPianoRollFrame->YtoVel (m_ptMouseMove.y);
	long lNewValue = pPianoRollFrame->YtoVel (point.y);
	long lOldPitchBend = pPianoRollFrame->YtoPitchBend (m_ptMouseMove.y);
	long lNewPitchBend = pPianoRollFrame->YtoPitchBend (point.y);
	long lOldTempoBPM = pPianoRollFrame->YtoTempoBPM (m_ptMouseMove.y);
	long lNewTempoBPM = pPianoRollFrame->YtoTempoBPM (point.y);
	long lOldTime = pPianoRollFrame->XtoTime (m_ptMouseMove.x);
	long lNewTime = pPianoRollFrame->XtoTime (point.x);
	CRect rcRegion;
	// キャプター中
	if (GetCapture () == this) {
		pSekaijuDoc->m_theCriticalSection.Lock ();
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		MIDITrack* pTempTrack = NULL;
		MIDIEvent* pTempEvent = NULL;
		MIDIEvent* pPrevEvent = NULL;
		MIDIEvent* pCloneEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		MIDIOut* pMIDIOut = NULL;
		MIDIStatus* pMIDIStatus = NULL;
		if (0 <= m_lTempPortIndex && m_lTempPortIndex < MAXMIDIOUTDEVICENUM) {
			pMIDIOut = pSekaijuApp->m_pMIDIOut[m_lTempPortIndex];
			pMIDIStatus = pSekaijuApp->m_pMIDIStatus[m_lTempPortIndex];
		}
		long i = 0;
		long lTime = lNewTime;
		// ツール別の操作
		switch (m_lTempTool) {
		// ペン
		case ID_PIANOROLL_PEN:
			pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
			if (m_lTempGraphKind == 0) {
				pTempTrack = pSekaijuDoc->GetTrack (0);
			}
			else {
				pTempTrack = pSekaijuDoc->GetTrack (m_lTempTrackIndex);
			}
			ASSERT (pTempTrack);
			// すべてのイベントに対して
			pTempEvent = pTempTrack->m_pFirstEvent;
			while (pTempEvent) {
				long lTempTime = MIDIEvent_GetTime (pTempEvent);
				if (lNewTime > lOldTime  && 
					lOldTime < lTempTime && lTempTime <= lNewTime ||
					lNewTime < lOldTime  &&
					lNewTime <= lTempTime && lTempTime < lOldTime ||
					lNewTime == lOldTime && lTempTime == lNewTime) {
					// テンポの削除
					if (MIDIEvent_IsTempo (pTempEvent) && 
						m_lTempGraphKind == 0) {
						pPrevEvent = pTempEvent->m_pPrevEvent;
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
						MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
						pTempEvent = pPrevEvent ? pPrevEvent->m_pNextEvent : pTempTrack->m_pFirstEvent;
					}
					// ベロシティの変更
					else if (MIDIEvent_IsNoteOn (pTempEvent) && 
						MIDIEvent_IsNote (pTempEvent) &&
						m_lTempGraphKind == 1) {
						lNewVel = CLIP (1, lNewVel, 127);
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
						pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent);
						lNewVel = CLIP (1, lNewVel, 127);
						MIDIEvent_SetVelocity (pCloneEvent, lNewVel);
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent);
						pTempEvent = pCloneEvent->m_pNextEvent;
					}
					// チャンネルアフタータッチの削除
					else if (MIDIEvent_IsChannelAftertouch (pTempEvent) &&
						m_lTempGraphKind == 2) {
						pPrevEvent = pTempEvent->m_pPrevEvent;
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
						MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
						pTempEvent = pPrevEvent ? pPrevEvent->m_pNextEvent : pTempTrack->m_pFirstEvent;
					}
					// ピッチベンドの削除
					else if (MIDIEvent_IsPitchBend (pTempEvent) &&
						m_lTempGraphKind == 3) {
						pPrevEvent = pTempEvent->m_pPrevEvent;
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
						MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
						pTempEvent = pPrevEvent ? pPrevEvent->m_pNextEvent : pTempTrack->m_pFirstEvent;
					}
					// コントロールチェンジの削除
					else if (MIDIEvent_IsControlChange (pTempEvent) &&
						m_lTempGraphKind >= 4) {
						if (MIDIEvent_GetNumber (pTempEvent) ==
							m_lTempGraphKind - 4) {
							pPrevEvent = pTempEvent->m_pPrevEvent;
							pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
							MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
							pTempEvent = pPrevEvent ? pPrevEvent->m_pNextEvent : pTempTrack->m_pFirstEvent;
						}
						else {
							pTempEvent = pTempEvent->m_pNextEvent;
						}
					}
					// どれにも該当しない。
					else {
						pTempEvent = pTempEvent->m_pNextEvent;
					}
				}
				// このイベントは時刻が一致していない
				else {
					pTempEvent = pTempEvent->m_pNextEvent;
				}
			}
			// 単一イベントの挿入
			lOldTime = CLIP (0, lOldTime, 0x7FFF0000);
			lNewTime = CLIP (0, lNewTime, 0x7FFF0000);
			lOldTempoBPM = CLIP (1, lOldTempoBPM, 60000000);
			lNewTempoBPM = CLIP (1, lNewTempoBPM, 60000000);
			lOldValue = CLIP (0, lOldValue, 127);
			lNewValue = CLIP (0, lNewValue, 127);
			lOldPitchBend = CLIP (0, lOldPitchBend, 16383);
			lNewPitchBend = CLIP (0, lNewPitchBend, 16383);
			lTime = lNewTime;
			if (m_lTempGraphSnap >= 2) {
				lTime = (lNewTime / m_lTempGraphSnap) * m_lTempGraphSnap;
			}
			// 挿入時刻がマウス移動区間内にある場合のみ
			if (lOldTime < lTime && lTime <= lNewTime ||
				lNewTime <= lTime && lTime < lOldTime ||
				lNewTime == lTime && lOldTime == lTime)  {
				// テンポの挿入
				if (m_lTempGraphKind == 0) {
					if (lNewTempoBPM != m_lLastInsertedTempoBPM) {
						VERIFY (pTempEvent = MIDIEvent_CreateTempo (lTime, 60000000 / lNewTempoBPM));
						VERIFY (MIDITrack_InsertEvent (pTempTrack, pTempEvent));
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent);
						m_lLastInsertedTempoBPM = lNewTempoBPM;
					}
				}
				// チャンネルアフタータッチの挿入
				else if (m_lTempGraphKind == 2) {
					if (lNewValue != m_lLastInsertedValue) {
						VERIFY (pTempEvent = MIDIEvent_CreateChannelAftertouch (lTime, m_lTempChannel, lNewValue));
						VERIFY (MIDITrack_InsertEvent (pTempTrack, pTempEvent));
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent);
						m_lLastInsertedValue = lNewValue;
					}
				}
				// ピッチベンドの挿入
				else if (m_lTempGraphKind == 3) {
					if (lNewPitchBend != m_lLastInsertedPitchBend) {
						VERIFY (pTempEvent = MIDIEvent_CreatePitchBend (lTime, m_lTempChannel, lNewPitchBend));
						VERIFY (MIDITrack_InsertEvent (pTempTrack, pTempEvent));
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent);
						m_lLastInsertedPitchBend = lNewPitchBend;
					}
				}
				// コントロールチェンジの挿入
				else if (m_lTempGraphKind >= 4) {
					if (lNewValue != m_lLastInsertedValue) {
						VERIFY (pTempEvent = MIDIEvent_CreateControlChange 
							(lTime, m_lTempChannel, m_lTempGraphKind - 4, lNewValue));
						VERIFY (MIDITrack_InsertEvent (pTempTrack, pTempEvent));
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent);
						m_lLastInsertedValue = lNewValue;
					}
				}
			}
			rcRegion = CRect (m_ptMouseMove.x, rcClient.top, point.x, rcClient.bottom);
			rcRegion.NormalizeRect ();
			rcRegion.InflateRect (1, 1);
			InvalidateRect (rcRegion - sizWndOrg);
			break;

		// 線
		case ID_PIANOROLL_LINE:
			rcRegion = CRect (m_ptMouseDown, m_ptMouseMove);
			rcRegion.NormalizeRect ();
			rcRegion.InflateRect (1, 1);
			InvalidateRect (rcRegion - sizWndOrg);
			rcRegion = CRect (m_ptMouseDown, point);
			rcRegion.NormalizeRect ();
			rcRegion.InflateRect (1, 1);
			InvalidateRect (rcRegion - sizWndOrg);
			break;
		// 消しゴム
		case ID_PIANOROLL_ERASER:
			pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
			if (m_lTempGraphKind == 0) {
				pTempTrack = pSekaijuDoc->GetTrack (0);
			}
			else {
				pTempTrack = pSekaijuDoc->GetTrack (m_lTempTrackIndex);
			}
			ASSERT (pTempTrack);
			pTempEvent = pTempTrack->m_pFirstEvent;
			//pTempEvent = pTempTrack->m_pLastEvent;	
			// すべてのイベントに対して(後方から評価)
			while (pTempEvent) {
				//pPrevEvent = pTempEvent->m_pPrevEvent;
				long lTempTime = MIDIEvent_GetTime (pTempEvent);
				if (lNewTime > lOldTime  && 
					lOldTime < lTempTime && lTempTime <= lNewTime ||
					lNewTime < lOldTime  &&
					lNewTime <= lTempTime && lTempTime < lOldTime ||
					lNewTime == lOldTime && lTempTime == lNewTime) {
					// テンポの削除
					if (MIDIEvent_IsTempo (pTempEvent) && 
						m_lTempGraphKind == 0) {
						pPrevEvent = pTempEvent->m_pPrevEvent;
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
						MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
						pTempEvent = pPrevEvent ? pPrevEvent->m_pNextEvent : pTempTrack->m_pFirstEvent;
					}
					// チャンネルアフターの削除
					else if (MIDIEvent_IsChannelAftertouch (pTempEvent) &&
						m_lTempGraphKind == 2) {
						pPrevEvent = pTempEvent->m_pPrevEvent;
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
						MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
						pTempEvent = pPrevEvent ? pPrevEvent->m_pNextEvent : pTempTrack->m_pFirstEvent;
					}
					// ピッチベンドの削除
					else if (MIDIEvent_IsPitchBend (pTempEvent) &&
						m_lTempGraphKind == 3) {
						pPrevEvent = pTempEvent->m_pPrevEvent;
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
						MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
						pTempEvent = pPrevEvent ? pPrevEvent->m_pNextEvent : pTempTrack->m_pFirstEvent;
					}
					// コントロールチェンジの削除
					else if (MIDIEvent_IsControlChange (pTempEvent) &&
						m_lTempGraphKind >= 4) {
						if (MIDIEvent_GetNumber (pTempEvent) ==
							m_lTempGraphKind - 4) {
							pPrevEvent = pTempEvent->m_pPrevEvent;
							pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
							MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
							pTempEvent = pPrevEvent ? pPrevEvent->m_pNextEvent : pTempTrack->m_pFirstEvent;
						}
						else {
							pTempEvent = pTempEvent->m_pNextEvent;
						}
					}
					// いずれにも該当しない
					else {
						pTempEvent = pTempEvent->m_pNextEvent;
					}
				}
				// このイベントは時刻が一致していない
				else {
					pTempEvent = pTempEvent->m_pNextEvent;
				}
			}
			rcRegion = CRect (m_ptMouseMove.x, rcClient.top, point.x, rcClient.bottom);
			rcRegion.NormalizeRect ();
			rcRegion.InflateRect (1, 1);
			InvalidateRect (rcRegion - sizWndOrg);
			break;
		// 選択
		case ID_PIANOROLL_SELECT:
			// 案B(選択矩形はちらつくが内部はちらつかない)
			rcRegion = CRect (m_ptMouseDown.x, m_ptMouseDown.y, m_ptMouseMove.x, m_ptMouseDown.y);
			rcRegion.NormalizeRect ();
			rcRegion.InflateRect (1, 1);
			InvalidateRect (rcRegion - sizWndOrg);
			rcRegion = CRect (m_ptMouseDown.x, m_ptMouseMove.y, m_ptMouseMove.x, m_ptMouseMove.y);
			rcRegion.NormalizeRect ();
			rcRegion.InflateRect (1, 1);
			InvalidateRect (rcRegion - sizWndOrg);
			rcRegion = CRect (m_ptMouseDown.x, m_ptMouseDown.y, m_ptMouseDown.x, m_ptMouseMove.y);
			rcRegion.NormalizeRect ();
			rcRegion.InflateRect (1, 1);
			InvalidateRect (rcRegion - sizWndOrg);
			rcRegion = CRect (m_ptMouseMove.x, m_ptMouseDown.y, m_ptMouseMove.x, m_ptMouseMove.y);
			rcRegion.NormalizeRect ();
			rcRegion.InflateRect (1, 1);
			InvalidateRect (rcRegion - sizWndOrg);

			rcRegion = CRect (m_ptMouseDown.x, m_ptMouseDown.y, point.x, m_ptMouseDown.y);
			rcRegion.NormalizeRect ();
			rcRegion.InflateRect (1, 1);
			InvalidateRect (rcRegion - sizWndOrg);
			rcRegion = CRect (m_ptMouseDown.x, point.y, point.x, point.y);
			rcRegion.NormalizeRect ();
			rcRegion.InflateRect (1, 1);
			InvalidateRect (rcRegion - sizWndOrg);
			rcRegion = CRect (m_ptMouseDown.x, m_ptMouseDown.y, m_ptMouseDown.x, point.y);
			rcRegion.NormalizeRect ();
			rcRegion.InflateRect (1, 1);
			InvalidateRect (rcRegion - sizWndOrg);
			rcRegion = CRect (point.x, m_ptMouseDown.y, point.x, point.y);
			rcRegion.NormalizeRect ();
			rcRegion.InflateRect (1, 1);
			InvalidateRect (rcRegion - sizWndOrg);
			break;

		// スクラブ
		case ID_PIANOROLL_SPEAKER:
			m_lTempTime = pPianoRollFrame->XtoTime (point.x);
			pSekaijuApp->SetPlayPosition (pSekaijuDoc, m_lTempTime);
			pSekaijuApp->SendDifferentStatus (SDS_ALL);
			break;
		}
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}
	// 非キャプター中
	// 非キャプター中(カーソルの種類だけ変更)
	else {
		long i = 0;
		switch (pPianoRollFrame->m_lCurTool) {
		// 描画
		case ID_PIANOROLL_PEN:
			::SetCursor (pSekaijuApp->m_hCursorDraw);
			break;
		// 線
		case ID_PIANOROLL_LINE:
			::SetCursor (pSekaijuApp->m_hCursorLine);
			break;
		// 消しゴム
		case ID_PIANOROLL_ERASER:
			::SetCursor (pSekaijuApp->m_hCursorEraser);
			break;
		// 選択
		case ID_PIANOROLL_SELECT:
			::SetCursor (pSekaijuApp->m_hCursorSelect);
			break;
		// スピーカー
		case ID_PIANOROLL_SPEAKER:
			::SetCursor (pSekaijuApp->m_hCursorSpeaker);
			break;
		// 不明
		default:
			//::SetCursor (pSekaijuApp->m_hCursorArrow);
			break;
		}
	}
	m_ptMouseMove = point;
	m_nMouseMoveFlags = nFlags;
}

// マウスホイールが回された時
void CPianoRollVelTimeView::OnMouseWheel40 (UINT nFlags, CPoint point) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lDelta = (short)HIWORD (nFlags);
	long lFlags = LOWORD (nFlags);
	if (lFlags & MK_CONTROL) {
		if (lDelta > 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel == 0 ||
			lDelta < 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel != 0) {
			pPianoRollFrame->PostMessage (WM_COMMAND, ID_CONTROL_PREVMEASURE);
		}
		else {
			pPianoRollFrame->PostMessage (WM_COMMAND, ID_CONTROL_NEXTMEASURE);
		}
	}
	else {
		long lVelScrollPos = pPianoRollFrame->GetVelScrollPos ();
		long lVelZoom = pPianoRollFrame->GetKeyZoom ();
		lVelScrollPos -= lVelZoom * lDelta / WHEELDELTA;
		pPianoRollFrame->SetVelScrollPos (lVelScrollPos);
	}
}
