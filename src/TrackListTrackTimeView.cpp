//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// トラックリストトラックタイムビュークラス
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
#include "SekaijuToolbar.h"
#include "ChildFrame.h"
#include "TrackListFrame.h"
#include "SekaijuView.h"
#include "TrackListTrackTimeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE (CTrackListTrackTimeView, CSekaijuView)

// メッセージマップ
BEGIN_MESSAGE_MAP (CTrackListTrackTimeView, CSekaijuView)
	ON_WM_CREATE ()
	ON_WM_DESTROY ()
	ON_WM_TIMER ()
	ON_WM_TIMER ()
	ON_WM_KEYDOWN ()
	ON_WM_KEYUP ()
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
CTrackListTrackTimeView::CTrackListTrackTimeView () {
	m_lCurTime = m_lOldTime = 0;
	m_lOldY1 = m_lOldY2 = 0;
	m_bOldDraw = TRUE;
	m_lTempMode = 0;
}

// デストラクタ
CTrackListTrackTimeView::~CTrackListTrackTimeView () {
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// 旧位置の縦線消去
void CTrackListTrackTimeView::EraseOldLine (CDC* pDC) {
	CRect rcClient;
	GetClientRect (&rcClient);
	pDC->DPtoLP (&rcClient);
	if (rcClient.left <= m_lOldX && m_lOldX <= rcClient.right && m_bOldDraw == TRUE) {
		pDC->SetROP2 (R2_NOT);
		pDC->MoveTo (m_lOldX, m_lOldY1);
		pDC->LineTo (m_lOldX, m_lOldY2);
	}
}

// 現在位置の縦線描画
void CTrackListTrackTimeView::DrawCurLine (CDC* pDC) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CRect rcClient;
	GetClientRect (&rcClient);
	pDC->DPtoLP (&rcClient); // TODO
	long x = pTrackListFrame->TimetoX (m_lCurTime);
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


//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// 原点の移動
void CTrackListTrackTimeView::OnPrepareDC (CDC* pDC, CPrintInfo* pInfo) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	pDC->SetWindowOrg (pTrackListFrame->GetTimeScrollPos (), pTrackListFrame->GetRowScrollPos ());
}

// 描画
void CTrackListTrackTimeView::OnDraw (CDC* pDC) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	long lVisibleLeftTime = pTrackListFrame->GetVisibleLeftTime ();
	long lVisibleRightTime = pTrackListFrame->GetVisibleRightTime ();
	long lVisibleTopRow = pTrackListFrame->GetVisibleTopRow ();
	long lVisibleBottomRow = pTrackListFrame->GetVisibleBottomRow ();
	long i, j;

	CRect rcClient;
	GetClientRect (&rcClient);
	pDC->DPtoLP (&rcClient); //TODO

	CPen penMeasure (PS_SOLID, 1, RGB (128, 128, 255));
	CPen penBeat (PS_SOLID, 1, RGB (192, 192, 192));
	CPen penTrack (PS_SOLID, 1, RGB (128, 128, 128));

	// 小節境界又はフレーム境界を配列に列挙及び、各セルの選択状態把握
	long lLeftMeasure, lLeftBeat, lLeftTick;
	long lRightMeasure, lRightBeat, lRightTick;
	MIDIData_BreakTime (pMIDIData, lVisibleLeftTime, &lLeftMeasure, &lLeftBeat, &lLeftTick);
	MIDIData_BreakTime (pMIDIData, lVisibleRightTime, &lRightMeasure, &lRightBeat, &lRightTick);
	long* pBorderTime = (long*)calloc (lRightMeasure - lLeftMeasure + 2, sizeof (long));
	for (i = lLeftMeasure; i <= lRightMeasure + 1; i++) {
		long lTime = 0;
		MIDIData_MakeTime (pMIDIData, i, 0, 0, &lTime);
		*(pBorderTime + i - lLeftMeasure) = lTime;
	}

	// 背景色と横線の描画
	CPen* pOldPen = pDC->SelectObject (&penTrack);
	// 一部選択されている小節の背景ブラシ作成(20100517追加)
	// WORD HatchBits[8] = { 0x11, 0x22, 0x44, 0x88, 0x11, 0x22, 0x44, 0x88 };
	WORD HatchBits[8] = { 0x33, 0x66, 0xCC, 0x99, 0x33, 0x66, 0xCC, 0x99 };
	CBitmap bm;
	bm.CreateBitmap (8, 8, 1, 1, HatchBits);
	CBrush brush;
	brush.CreatePatternBrush (&bm);
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&brush);

	long lRowZoom = pTrackListFrame->GetRowZoom ();
	long lNormalColor = RGB(255,255,255);
	long lSelectedColor = RGB(255,255,255);
	i = 0;
	for (i = lVisibleTopRow; i <= lVisibleBottomRow; i++) {
		// 奇数行(1,3,5...)(20100517追加)
		if (i % 2) { 
			lNormalColor = RGB(255,255,192);
			lSelectedColor = RGB(224,224,192);
		}
		// 偶数行(0,2,4...)(20100517追加)
		else {
			lNormalColor = RGB(255,255,255);
			lSelectedColor = RGB(224,224,255);
		}
		long y = i * lRowZoom;
		pMIDITrack = pSekaijuDoc->GetTrack (i);
		for (j = lLeftMeasure; j <= lRightMeasure; j++) {
			long lTime1 = *(pBorderTime + j - lLeftMeasure);
			long lTime2 = *(pBorderTime + j + 1 - lLeftMeasure);
			long x1 = pTrackListFrame->TimetoX (lTime1);
			long x2 = pTrackListFrame->TimetoX (lTime2);
			long lOffset = 
				(i - lVisibleTopRow) * (lRightMeasure - lLeftMeasure + 2) + 
				(j - lLeftMeasure);
			if (pMIDITrack) {
				long lTrackMeasureSelected = pSekaijuDoc->IsTrackMeasureSelected (pMIDITrack, j);
				switch (lTrackMeasureSelected) {
				case 0:	// 小節内にイベントはない
				case 1: // 小節内のイベントはひとつも選択されていない(20100517調整)
					pDC->FillSolidRect (x1, y, x2 - x1, lRowZoom, lNormalColor);
					break;
				case 2: // 小節内の一部のイベントが選択されている(20100517追加)
					pDC->SetTextColor (lSelectedColor); // いったんFillSolidRectすると戻ってしまうので
					pDC->SetBkColor (lNormalColor); // brushの前景色と背景色は毎回設定する必要あり。
					pDC->FillRect (CRect (x1, y, x2, y + lRowZoom), &brush); 
					break;
				case 3: // 小節内の全てのイベントが選択されている(20100517調整)
					pDC->FillSolidRect (x1, y, x2 - x1, lRowZoom, lSelectedColor); 
					break;
				}
			}
			// トラックは存在しない(20100517調整)
			else {
				pDC->FillSolidRect 
					(x1, y, x2 - x1, lRowZoom, lNormalColor);
			}
		}
		pDC->MoveTo (rcClient.left, y - 1);
		pDC->LineTo (rcClient.right, y - 1);
	}

	pDC->SelectObject(pOldBrush);


	// 縦線(小節境界線又はフレーム境界線)の描画
	for (i = lLeftMeasure; i <= lRightMeasure; i++) {
		long lTime = *(pBorderTime + i - lLeftMeasure);
		long x = pTrackListFrame->TimetoX (lTime);
		pDC->SelectObject (&penMeasure);
		pDC->MoveTo (x, rcClient.top);
		pDC->LineTo (x, rcClient.bottom);
	}
	pDC->SelectObject (pOldPen);

	// データ部の描画
	i = 0;
	forEachTrack (pMIDIData, pMIDITrack) {
		if (lVisibleTopRow <= i && i <= lVisibleBottomRow) {
			long y = i * lRowZoom;
			long lTrackColor = MIDITrack_GetForeColor (pMIDITrack);
			long lSelectedColor = RGB (0, 0, 0);
			CPen theTrackPen (PS_SOLID, 1, lTrackColor);
			CPen theSelectedPen (PS_SOLID, 1, lSelectedColor);
			forEachEvent (pMIDITrack, pMIDIEvent) {
				long lIsEventSelected = pSekaijuDoc->IsEventSelected (pMIDIEvent);
				long lColor = lIsEventSelected ? lSelectedColor : lTrackColor;
				// ノートイベントの描画
				if (MIDIEvent_IsNote (pMIDIEvent)) {
					long lTime = MIDIEvent_GetTime (pMIDIEvent);
					long lKey = MIDIEvent_GetKey (pMIDIEvent);
					long lDur = MIDIEvent_GetDuration (pMIDIEvent);
					long x1 = pTrackListFrame->TimetoX (lTime);
					long x2 = pTrackListFrame->TimetoX (lTime + lDur) + 1;
					long y1 = y + lRowZoom * (127 - lKey) / 128;
					long y2 = y1 + 1;
					CRect rcNote (x1, y1, x2, y2);
					pDC->FillSolidRect (&rcNote, lColor);
				}
				// キーアフタータッチの描画
				else if (MIDIEvent_IsKeyAftertouch (pMIDIEvent)) {
					long lTime = MIDIEvent_GetTime (pMIDIEvent);
					long lValue = MIDIEvent_GetValue (pMIDIEvent);
					long x1 = pTrackListFrame->TimetoX (lTime);
					long x2 = x1 + 1;
					long y1 = (i + 1) * lRowZoom;
					long y2 = y1 - lRowZoom  * (lValue) / 128;
					CRect rcNote (x1, y1, x2, y2);
					pDC->FillSolidRect (&rcNote, lColor);
					if (lValue == 0) {
						pDC->SelectObject (lIsEventSelected ? &theSelectedPen : &theTrackPen);
						pDC->Ellipse (x1 - 2, y1 - 2, x1 + 2, y1 + 2); 
						pDC->SelectObject (pOldPen);
					}
				}
				// コントロールチェンジイベントの描画
				else if (MIDIEvent_IsControlChange (pMIDIEvent)) {
					long lTime = MIDIEvent_GetTime (pMIDIEvent);
					long lValue = MIDIEvent_GetValue (pMIDIEvent);
					long x1 = pTrackListFrame->TimetoX (lTime);
					long x2 = x1 + 1;
					long y1 = (i + 1) * lRowZoom;
					long y2 = y1 - lRowZoom  * (lValue) / 128;
					CRect rcNote (x1, y1, x2, y2);
					pDC->FillSolidRect (&rcNote, lColor);
					if (lValue == 0) {
						pDC->SelectObject (lIsEventSelected ? &theSelectedPen : &theTrackPen);
						pDC->Ellipse (x1 - 2, y1 - 2, x1 + 2, y1 + 2); 
						pDC->SelectObject (pOldPen);
					}
				}
				// プログラムチェンジイベントの描画
				else if (MIDIEvent_IsProgramChange (pMIDIEvent)) {
					long lTime = MIDIEvent_GetTime (pMIDIEvent);
					long lValue = MIDIEvent_GetValue (pMIDIEvent);
					long x1 = pTrackListFrame->TimetoX (lTime);
					long x2 = x1 + 1;
					long y1 = (i + 1) * lRowZoom;
					long y2 = y1 - lRowZoom  * (lValue) / 128;
					CRect rcNote (x1, y1, x2, y2);
					pDC->FillSolidRect (&rcNote, lColor);
					if (lValue == 0) {
						pDC->SelectObject (lIsEventSelected ? &theSelectedPen : &theTrackPen);
						pDC->Ellipse (x1 - 2, y1 - 2, x1 + 2, y1 + 2); 
						pDC->SelectObject (pOldPen);
					}
				}
				// チャンネルアフタータッチの描画
				else if (MIDIEvent_IsChannelAftertouch (pMIDIEvent)) {
					long lTime = MIDIEvent_GetTime (pMIDIEvent);
					long lValue = MIDIEvent_GetValue (pMIDIEvent);
					long x1 = pTrackListFrame->TimetoX (lTime);
					long x2 = x1 + 1;
					long y1 = (i + 1) * lRowZoom;
					long y2 = y1 - lRowZoom  * (lValue) / 128;
					CRect rcNote (x1, y1, x2, y2);
					pDC->FillSolidRect (&rcNote, lColor);
					if (lValue == 0) {
						pDC->SelectObject (lIsEventSelected ? &theSelectedPen : &theTrackPen);
						pDC->Ellipse (x1 - 2, y1 - 2, x1 + 2, y1 + 2); 
						pDC->SelectObject (pOldPen);
					}
				}
				// ピッチベンドの描画
				else if (MIDIEvent_IsPitchBend (pMIDIEvent)) {
					long lTime = MIDIEvent_GetTime (pMIDIEvent);
					long lValue = MIDIEvent_GetValue (pMIDIEvent);
					long x1 = pTrackListFrame->TimetoX (lTime);
					long x2 = x1 + 1;
					long y1 = (i + 1) * lRowZoom - lRowZoom / 2;
					long y2 = y1 - lRowZoom  * (lValue - 8192) / 16384; // 20110508修正
					CRect rcNote (x1, y1, x2, y2);
					pDC->FillSolidRect (&rcNote, lColor);
					if (lValue == 8192) {
						pDC->SelectObject (lIsEventSelected ? &theSelectedPen : &theTrackPen);
						pDC->Ellipse (x1 - 2, y1 - 2, x1 + 2, y1 + 2); 
						pDC->SelectObject (pOldPen);
					}
				}
			}
		}
		i++;
	}

	// 現在位置の縦線描画
	DrawCurLine (pDC);

	// マウストラッカーが必要な場合、マウストラッカーの描画
	if (GetCapture () == this) {
		if (m_lTempMode == 0x0401) {
			pDC->SetROP2 (R2_NOT);
			pDC->MoveTo (m_lMinX, m_lMinY);
			pDC->LineTo (m_lMinX, m_lMaxY);
			pDC->LineTo (m_lMaxX, m_lMaxY);
			pDC->LineTo (m_lMaxX, m_lMinY);
			pDC->LineTo (m_lMinX, m_lMinY);
			pDC->SetROP2 (R2_COPYPEN);
		}
	}

	free (pBorderTime);

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// ビューの更新
void CTrackListTrackTimeView::OnUpdate (CView* pSender, LPARAM lHint, CObject* pHint) {
	// クリティカルセクションはロックされているものとする。
	if ((lHint & SEKAIJUDOC_PLAYSTARTED) || 
		(lHint & SEKAIJUDOC_RECORDSTARTED) ||
		(lHint & SEKAIJUDOC_POSITIONCHANGED)) {
		PostMessage (WM_TIMER, 0x11, NULL);
	}
	CSekaijuView::OnUpdate (pSender, lHint, pHint);
}

// ウィンドウクラスの変更
BOOL CTrackListTrackTimeView::PreCreateWindow (CREATESTRUCT& cs) {
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
int CTrackListTrackTimeView::OnCreate (LPCREATESTRUCT lpCreateStruct) {
	SetTimer (0x11, 55, NULL);
	return CSekaijuView::OnCreate (lpCreateStruct);
}

// ウィンドウ破棄時
void CTrackListTrackTimeView::OnDestroy () {
	KillTimer (0x11);
	CSekaijuView::OnDestroy ();
}

// タイマー呼び出し時
void CTrackListTrackTimeView::OnTimer (UINT nIDEvent) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	if (nIDEvent == 0x11) {
		if (pSekaijuDoc->m_pMIDIData && pSekaijuDoc->m_pMIDIClock) {
			// クリティカルセクションロック
			pSekaijuDoc->m_theCriticalSection.Lock ();
			CDC* pDC = GetDC ();
			OnPrepareDC (pDC, NULL);
			CRect rcClient;
			GetClientRect (&rcClient);
			pDC->DPtoLP (&rcClient);
			m_lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
			// 現在位置縦線の更新
			if (m_lOldTime != m_lCurTime) {
				CPen thePen (PS_SOLID, 1, RGB (255, 0, 0));
				CPen* pOldPen = pDC->SelectObject (&thePen);
				EraseOldLine (pDC);
				DrawCurLine (pDC);
				pDC->SelectObject (pOldPen);
			}

			// ページの更新およびオートスクロール(手動スクロール時はしない)
			if (pTrackListFrame->m_bAutoPageUpdate == TRUE) {
				MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
				long lTimeMode, lTimeResolution;
				MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);
				if (lTimeMode == MIDIDATA_TPQNBASE) {
					// 現在位置が右側へはみ出した場合
					long lRightMeasure; long lRightBeat; long lRightTick; long lTempRightTime;
					MIDIData_BreakTime (pMIDIData, pTrackListFrame->GetVisibleRightTime (), 
						&lRightMeasure, &lRightBeat, &lRightTick);
					MIDIData_MakeTime (pMIDIData, lRightMeasure, 0, 0, &lTempRightTime);
					if (m_lCurTime >= lTempRightTime) {
						long lNewMeasure; long lNewBeat; long lNewTick;
						MIDIData_BreakTime (pMIDIData, m_lCurTime, &lNewMeasure, &lNewBeat, &lNewTick);
						MIDIData_MakeTime (pMIDIData, lNewMeasure, 0, 0, &lTempRightTime);
						long x = pTrackListFrame->TimetoX (lTempRightTime);
						pTrackListFrame->SetTimeScrollPos (x);
					}
					// 現在位置が左側へはみ出した場合
					long lLeftMeasure; long lLeftBeat; long lLeftTick; long lTempLeftTime;
					MIDIData_BreakTime (pMIDIData, pTrackListFrame->GetVisibleLeftTime (), 
						&lLeftMeasure, &lLeftBeat, &lLeftTick);
					MIDIData_MakeTime (pMIDIData, lLeftMeasure, 0, 0, &lTempLeftTime);
					if (m_lCurTime < lTempLeftTime) {
						long lNewMeasure; long lNewBeat; long lNewTick;
						MIDIData_BreakTime (pMIDIData, m_lCurTime, &lNewMeasure, &lNewBeat, &lNewTick);
						MIDIData_MakeTime (pMIDIData, lNewMeasure, 0, 0, &lTempLeftTime);
						long x = pTrackListFrame->TimetoX (lTempLeftTime);
						pTrackListFrame->SetTimeScrollPos (x);
					}
				}
				else {
					// 現在位置が右側へはみ出した場合
					long lRightFrameNumber = pTrackListFrame->GetVisibleRightTime () / lTimeResolution;
					long lTempRightTime = lRightFrameNumber * lTimeResolution;
					if (m_lCurTime >= lTempRightTime) {
						lTempRightTime = (m_lCurTime / lTimeResolution) * lTimeResolution;
						long x = pTrackListFrame->TimetoX (lTempRightTime);
						pTrackListFrame->SetTimeScrollPos (x);
					}
					// 現在位置が左側へはみ出した場合
					long lLeftFrameNumber = pTrackListFrame->GetVisibleLeftTime () / lTimeResolution;
					long lTempLeftTime = lLeftFrameNumber * lTimeResolution;
					if (m_lCurTime < lTempLeftTime) {
						lTempLeftTime = (m_lCurTime / lTimeResolution) * lTimeResolution;
						long x = pTrackListFrame->TimetoX (lTempLeftTime);
						pTrackListFrame->SetTimeScrollPos (x);
					}
				}
			}
			
			ReleaseDC (pDC);
			// クリティカルセクション解除
			pSekaijuDoc->m_theCriticalSection.Unlock ();
		}
	}
	// マウスキャプター中にクライアント領域をはみ出した場合の自動スクロール処理
	else if (nIDEvent == 0x21) {
		if (GetCapture () == this) {
			CClientDC dc (this);
			OnPrepareDC (&dc, NULL);
			CRect rcClient;
			GetClientRect (&rcClient);
			rcClient += CSize (pTrackListFrame->GetTimeScrollPos (), pTrackListFrame->GetRowScrollPos ());
			if (!rcClient.PtInRect (m_ptMouseMove)) {
				pSekaijuDoc->m_theCriticalSection.Lock ();
				long lOldTimeScrollPos = pTrackListFrame->GetTimeScrollPos ();
				long lOldRowScrollPos = pTrackListFrame->GetRowScrollPos ();
				if (m_ptMouseMove.x < rcClient.left) {
					pTrackListFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_LINEUP,
						(LPARAM)(pTrackListFrame->m_wndTimeScroll.GetSafeHwnd ()));
				}
				else if (m_ptMouseMove.x > rcClient.right) {
					pTrackListFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_LINEDOWN,
						(LPARAM)(pTrackListFrame->m_wndTimeScroll.GetSafeHwnd ()));
				}
				if (m_ptMouseMove.y < rcClient.top) {
					pTrackListFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEUP,
						(LPARAM)(pTrackListFrame->m_wndRowScroll.GetSafeHwnd ()));
				}
				else if (m_ptMouseMove.y > rcClient.bottom) {
					pTrackListFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEDOWN,
						(LPARAM)(pTrackListFrame->m_wndRowScroll.GetSafeHwnd ()));
				}
				WORD wX = (WORD)(m_ptMouseMove.x - lOldTimeScrollPos);
				WORD wY = (WORD)(m_ptMouseMove.y - lOldRowScrollPos);
				PostMessage (WM_MOUSEMOVE, (WPARAM)m_nMouseMoveFlags, (LPARAM)((wY << 16) | wX));
				pSekaijuDoc->m_theCriticalSection.Unlock ();
			}
		}
	}
}



// キーが押された時
void CTrackListTrackTimeView::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	switch (nChar) {
	// ↑
	case VK_UP:
		pTrackListFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEUP, 
			(LPARAM)(pTrackListFrame->m_wndRowScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// ↓
	case VK_DOWN:
		pTrackListFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEDOWN, 
			(LPARAM)(pTrackListFrame->m_wndRowScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// PageUp
	case VK_PRIOR:
		pTrackListFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_PAGEUP, 
			(LPARAM)(pTrackListFrame->m_wndRowScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// PageDown
	case VK_NEXT:
		pTrackListFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_PAGEDOWN, 
			(LPARAM)(pTrackListFrame->m_wndRowScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// ←
	case VK_LEFT:
		pTrackListFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_LINEUP, 
			(LPARAM)(pTrackListFrame->m_wndTimeScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// →
	case VK_RIGHT:
		pTrackListFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_LINEDOWN, 
			(LPARAM)(pTrackListFrame->m_wndTimeScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// Home
	case VK_HOME:
		pTrackListFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_PAGEUP, 
			(LPARAM)(pTrackListFrame->m_wndTimeScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// End
	case VK_END:
		pTrackListFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_PAGEDOWN, 
			(LPARAM)(pTrackListFrame->m_wndTimeScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// Control
	case VK_CONTROL:
		if (1) {
			WORD wX = (WORD)(m_ptMouseMove.x - pTrackListFrame->GetTimeScrollPos ());
			WORD wY = (WORD)(m_ptMouseMove.y - pTrackListFrame->GetRowScrollPos ());
			UINT nFlags = m_nMouseMoveFlags | MK_CONTROL;
			PostMessage (WM_MOUSEMOVE, (WPARAM)nFlags, (LPARAM)((wY << 16) | wX));
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

// キーが離された時
void CTrackListTrackTimeView::OnKeyUp (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	switch (nChar) {
	// Control
	case VK_CONTROL:
		if (1) {
			WORD wX = (WORD)(m_ptMouseMove.x - pTrackListFrame->GetTimeScrollPos ());
			WORD wY = (WORD)(m_ptMouseMove.y - pTrackListFrame->GetRowScrollPos ());
			UINT nFlags = m_nMouseMoveFlags & (~MK_CONTROL);
			PostMessage (WM_MOUSEMOVE, (WPARAM)nFlags, (LPARAM)((wY << 16) | wX));
		}
		break;
	}
	return;
}


// マウス左ボタン押された時  
void CTrackListTrackTimeView::OnLButtonDown (UINT nFlags, CPoint point) {
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
	
	CClientDC dc (this);
	OnPrepareDC (&dc, NULL);
	CSize sizWndOrg (pTrackListFrame->GetTimeScrollPos (), pTrackListFrame->GetRowScrollPos ());
	point += sizWndOrg;
	ASSERT (m_lTempMode == 0);
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDITrack* pCloneTrack = NULL;
	MIDIEvent* pCloneEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	m_lTempTool = pTrackListFrame->m_lCurTool;
	m_lTempTime = pTrackListFrame->XtoTime (point.x);
	m_lTempTrackIndex = pTrackListFrame->YtoRow (point.y);
	CRect rcRegion;
	long lNewTrackIndex = pTrackListFrame->YtoRow (point.y);
	long lNewTime = pTrackListFrame->XtoTime (point.x);
	long lNewMeasure, lNewBeat, lNewTick;
	BOOL bPtInSelectedCell = FALSE;

	switch (m_lTempTool) {
	// 選択
	case ID_TRACKLIST_SELECT:
		// 既存の選択されているセル上にマウスカーソルがあるか調べる
		MIDIData_BreakTime (pMIDIData, lNewTime, &lNewMeasure, &lNewBeat, &lNewTick);
		pMIDITrack = pSekaijuDoc->GetTrack (lNewTrackIndex);
		if (pMIDITrack) {
			if (pSekaijuDoc->IsTrackMeasureSelected (pMIDITrack, lNewMeasure) >= 2) { // 20100517新規修正
				bPtInSelectedCell = TRUE;
			}
		}

		// 既存の選択されているセルの上にマウスが置かれた場合(コピー又は移動モード)
		if (bPtInSelectedCell) {
			long i = 0;
			long j = 0;
			MIDITrack* pTempTrack;
			MIDIEvent* pTempEvent;
			m_theTempSelectedEventArray.RemoveAll ();
			m_theTempEndofTrackEventArray.RemoveAll ();
			// 複写の場合(CONTROLが押されている)
			if (nFlags & MK_CONTROL) {
				VERIFY (strHistoryName.LoadString (IDS_DUPLICATE_MEASURE));
				VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
				VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
				// 選択されていないEOTイベントの履歴保持
				forEachTrack (pMIDIData, pMIDITrack) {
					MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
					if (pLastEvent) {
						if (MIDIEvent_IsEndofTrack (pLastEvent)) {
							if (pSekaijuDoc->IsEventSelected (pLastEvent) == 0) {
								pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent);
								pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent);
								m_theTempEndofTrackEventArray.Add (pCloneEvent);
							}
						}
					}
				}
				// イベントの複写
				forEachTrack (pMIDIData, pTempTrack) {
					forEachEvent (pTempTrack, pTempEvent) {
						if (pSekaijuDoc->IsEventSelected (pTempEvent) &&
							pTempEvent->m_pPrevCombinedEvent == NULL) {
							if (MIDIEvent_IsEndofTrack (pTempEvent)) {
								VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
								pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent);
								pCloneEvent->m_lUser1 = i;
								pCloneEvent->m_lUser2 = MIDIEvent_GetTime (pCloneEvent);
								m_theTempSelectedEventArray.Add (pCloneEvent);
								pTempEvent = pCloneEvent;
							}
							else {
								// 複写元イベントを非選択状態にする。
								pCloneEvent = pSekaijuDoc->SelectEvent (pTempEvent, 0, pCurHistoryUnit);
								if (pCloneEvent == NULL) {
									continue;
								}
								// イベントの複写をし、複写したイベントを選択状態にする。
								pCloneEvent = pSekaijuDoc->DuplicateMIDIEvent (pCloneEvent);
								if (pCloneEvent) { 
									pCloneEvent->m_lUser1 = i;
									pCloneEvent->m_lUser2 = MIDIEvent_GetTime (pCloneEvent);
									pSekaijuDoc->SetEventSelected (pCloneEvent, 1);
									m_theTempSelectedEventArray.Add (pCloneEvent);
									pTempEvent = pCloneEvent;
								}
							}
						}
					}
					i++;
				}
				m_lTempMode = 0x0403;
				::SetCursor (pSekaijuApp->m_hCursorSizeAllCopy);
			}
			// 移動の場合(CONTROLが押されていない)
			else {
				VERIFY (strHistoryName.LoadString (IDS_MOVE_MEASURE));
				VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
				VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
				// 選択されていないEOTイベントの履歴保持
				forEachTrack (pMIDIData, pMIDITrack) {
					MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
					if (pLastEvent) {
						if (MIDIEvent_IsEndofTrack (pLastEvent)) {
							if (pSekaijuDoc->IsEventSelected (pLastEvent) == 0) {
								pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent);
								pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent);
								m_theTempEndofTrackEventArray.Add (pCloneEvent);
							}
						}
					}
				}
				// 移動するイベントを履歴保持し、移動イベント用用配列に登録
				forEachTrack (pMIDIData, pTempTrack) {
					forEachEvent (pTempTrack, pTempEvent) {
						if (pSekaijuDoc->IsEventSelected (pTempEvent) &&
							pTempEvent->m_pPrevCombinedEvent == NULL) {
							VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
							pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent);
							pCloneEvent->m_lUser1 = i;
							pCloneEvent->m_lUser2 = MIDIEvent_GetTime (pCloneEvent);
							m_theTempSelectedEventArray.Add (pCloneEvent);
							pTempEvent = pCloneEvent;
						}
					}
					i++;
				}
				m_lTempMode = 0x0402;
				::SetCursor (pSekaijuApp->m_hCursorSizeAll);
			}
			//rcRegion = GetNoteRect (m_pTempEvent);
			//InvalidateRect (rcRegion - sizWndOrg);
			Invalidate ();
			pTrackListFrame->m_bAutoPageUpdate = FALSE;
			SetTimer (0x21, 55, NULL);
			SetCapture ();
		}

		// 選択されていないセルの上にマウスが置かれた場合(選択モード)
		else {
			VERIFY (strHistoryName.LoadString (IDS_SELECT_DESELECT));
			VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
			// 旧選択範囲の選択解除
			long i = 0;
			if ((nFlags & MK_SHIFT) == 0 && (nFlags & MK_CONTROL) == 0) {
				pSekaijuDoc->SelectNoObject (pCurHistoryUnit);
			}
			long lMinTime, lMinMeasure, lMinBeat, lMinTick;
			long lMaxTime, lMaxMeasure, lMaxBeat, lMaxTick;
			// 新しい選択範囲矩形を描画する
			m_lMinY = pTrackListFrame->RowtoY (lNewTrackIndex);
			m_lMaxY = pTrackListFrame->RowtoY (lNewTrackIndex + 1);
			MIDIData_BreakTime (pMIDIData, lNewTime, &lMinMeasure, &lMinBeat, &lMinTick);
			MIDIData_MakeTime (pMIDIData, lMinMeasure, 0, 0, &lMinTime);
			MIDIData_BreakTime (pMIDIData, lNewTime, &lMaxMeasure, &lMaxBeat, &lMaxTick);
			MIDIData_MakeTime (pMIDIData, lMaxMeasure + 1, 0, 0, &lMaxTime);
			m_lMinX = pTrackListFrame->TimetoX (lMinTime);
			m_lMaxX = pTrackListFrame->TimetoX (lMaxTime);
			//dc.MoveTo (lMinX, lMinY);
			//dc.LineTo (lMinX, lMaxY);
			//dc.LineTo (lMaxX, lMaxY);
			//dc.LineTo (lMaxX, lMinY);
			//dc.LineTo (lMinX, lMinY);
			//dc.SetROP2 (R2_COPYPEN);
			m_lTempMode = 0x0401;
			Invalidate ();
			if (nFlags & MK_CONTROL) {
				::SetCursor (pSekaijuApp->m_hCursorSelectAdd2);
			}
			else {
				::SetCursor (pSekaijuApp->m_hCursorSelect2);
			}
			pTrackListFrame->m_bAutoPageUpdate = FALSE;
			SetTimer (0x21, 55, NULL);
			SetCapture ();
		}
		break;

	// スクラブ
	case ID_TRACKLIST_SPEAKER:
		if (pSekaijuApp->m_bPlaying) {
			pTrackListFrame->SendMessage (WM_COMMAND, (WPARAM)ID_CONTROL_PLAY, (LPARAM)0);
		}
		pSekaijuApp->SetPlayPosition (pSekaijuDoc, m_lTempTime);
		pSekaijuApp->SendDifferentStatus (SDS_ALL);
		m_lTempMode = 0x0501;
		pTrackListFrame->m_bAutoPageUpdate = FALSE;
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
void CTrackListTrackTimeView::OnRButtonDown (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;

	CPoint ptMenu (point);
	ClientToScreen (&ptMenu);
	CSize sizWndOrg (pTrackListFrame->GetTimeScrollPos (), pTrackListFrame->GetRowScrollPos ());
	point += sizWndOrg;
	
	long lTrackIndex = pTrackListFrame->YtoRow (point.y);
	long lTime = pTrackListFrame->XtoTime (point.x);
	long lMeasure, lBeat, lTick;
	MIDIData_BreakTime (pMIDIData, lTime, &lMeasure, &lBeat, &lTick);
	MIDIData_MakeTime (pMIDIData, lMeasure, 0, 0, &lTime);
	pSekaijuDoc->m_lTempTime = lTime;
	pSekaijuDoc->m_lTempTrackIndex = lTrackIndex;
	pSekaijuDoc->m_pTempTrack = pSekaijuDoc->GetTrack (lTrackIndex);
	pSekaijuDoc->m_pTempEvent = NULL;
	
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);

	CMenu theMenu;
	VERIFY (theMenu.LoadMenu (IDR_POPUPMENU02));
	CMenu* pContextMenu = theMenu.GetSubMenu (0);
	pContextMenu->TrackPopupMenu (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
		ptMenu.x, ptMenu.y, pTrackListFrame);

}

// マウス左ボタン離されたとき
void CTrackListTrackTimeView::OnLButtonUp (UINT nFlags, CPoint point) {
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
	
	CClientDC dc (this);
	OnPrepareDC (&dc, NULL);
	CSize sizWndOrg (pTrackListFrame->GetTimeScrollPos (), pTrackListFrame->GetRowScrollPos ());
	point += sizWndOrg;

	long lDownTrackIndex = pTrackListFrame->YtoRow (m_ptMouseDown.y);
	long lOldTrackIndex = pTrackListFrame->YtoRow (m_ptMouseMove.y);
	long lNewTrackIndex = pTrackListFrame->YtoRow (point.y);
	long lDownTime = pTrackListFrame->XtoTime (m_ptMouseDown.x);
	long lOldTime = pTrackListFrame->XtoTime (m_ptMouseMove.x);
	long lNewTime = pTrackListFrame->XtoTime (point.x);
	CRect rcRegion;
	// キャプター中ならば
	if (GetCapture () == this) {
		pSekaijuDoc->m_theCriticalSection.Lock (); // 20080821位置修正
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		MIDITrack* pCloneTrack = NULL;
		MIDIEvent* pCloneEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		switch (m_lTempTool) {
		// 選択
		case ID_TRACKLIST_SELECT:
			VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
			// 移動中又は複写中
			if (m_lTempMode == 0x0402 || m_lTempMode == 0x0403) {
				long j;
				long lDownMeasure, lDownBeat, lDownTick;
				long lOldMeasure, lOldBeat, lOldTick;
				long lNewMeasure, lNewBeat, lNewTick;
				MIDIData_BreakTime (pMIDIData, lDownTime, &lDownMeasure, &lDownBeat, &lDownTick);
				MIDIData_BreakTime (pMIDIData, lOldTime, &lOldMeasure, &lOldBeat, &lOldTick);
				MIDIData_BreakTime (pMIDIData, lNewTime, &lNewMeasure, &lNewBeat, &lNewTick);
				long lTempSelectedEventCount = m_theTempSelectedEventArray.GetSize ();
				for (j = 0; j < lTempSelectedEventCount; j++) {
					MIDIEvent* pTempEvent = (MIDIEvent*)(m_theTempSelectedEventArray.GetAt (j));
					pTempEvent->m_lUser1 = 0;
					pTempEvent->m_lUser2 = 0;
				}
				// 複写の場合
				if (m_lTempMode == 0x0403) {
					// 元の位置と変わっていない場合、複写したイベントを削除し、履歴に登録しない。
					if (lNewTrackIndex == lDownTrackIndex &&
						lNewMeasure == lDownMeasure) {
						for (j = 0; j < lTempSelectedEventCount; j++) {
							pCloneEvent = (MIDIEvent*)(m_theTempSelectedEventArray.GetAt (j));
							MIDIEvent_Delete (pCloneEvent);
							pCloneEvent = NULL;
						}
					}
					// 元に位置と異なる場所に複写した場合、複写したイベントを履歴登録
					else {
						for (j = 0; j < lTempSelectedEventCount; j++) {
							pCloneEvent = (MIDIEvent*)(m_theTempSelectedEventArray.GetAt (j));
							pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent);
						}
					}
				}
				// 移動の場合
				else if (m_lTempMode == 0x0402) {
					// 移動したイベントを履歴登録
					for (j = 0; j < lTempSelectedEventCount; j++) {
						pCloneEvent = (MIDIEvent*)(m_theTempSelectedEventArray.GetAt (j));
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent);
					}
				}
				// 選択されていないEOTイベントの復元と履歴登録
				long lTempEndofTrackEventCount = m_theTempEndofTrackEventArray.GetSize ();
				for (j = 0; j < lTempEndofTrackEventCount; j++) {
					pCloneEvent = (MIDIEvent*)(m_theTempEndofTrackEventArray.GetAt (j));
					pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent);
				}
			}
			// 選択範囲変更中
			else {
				long i, j;
				long lMinMeasure, lMinBeat, lMinTick;
				long lMaxMeasure, lMaxBeat, lMaxTick;
				long lMinTrackIndex; 
				long lMaxTrackIndex;
				lMinTrackIndex = __min (lDownTrackIndex, lNewTrackIndex);
				lMaxTrackIndex = __max (lDownTrackIndex, lNewTrackIndex); 
				if (lDownTime < lNewTime) {
					MIDIData_BreakTime (pMIDIData, lDownTime, &lMinMeasure, &lMinBeat, &lMinTick);
					MIDIData_BreakTime (pMIDIData, lNewTime, &lMaxMeasure, &lMaxBeat, &lMaxTick);
				}
				else {
					MIDIData_BreakTime (pMIDIData, lNewTime, &lMinMeasure, &lMinBeat, &lMinTick);
					MIDIData_BreakTime (pMIDIData, lDownTime, &lMaxMeasure, &lMaxBeat, &lMaxTick);
				}
				i = 0;
				forEachTrack (pMIDIData, pMIDITrack) {
					if (lMinTrackIndex <= i && i <= lMaxTrackIndex) {
						for (j = lMinMeasure; j <= lMaxMeasure; j++) {
							pSekaijuDoc->SelectTrackMeasure (pMIDITrack, j, 1, pCurHistoryUnit);
						}
					}
					i++;
				}
			}
			pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIDATACHANGED | 
				SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIEVENTCHANGED);
			break;

		// スクラブ
		case ID_TRACKLIST_SPEAKER:
			pSekaijuApp->SendAllNoteOff ();
			pSekaijuApp->SendAllSoundOff ();
			pSekaijuDoc->UpdateAllViews (NULL);
			break;
		}
		m_lTempMode = 0;
		KillTimer (0x21);
		ReleaseCapture ();
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}
}

// マウス右ボタン離されたとき
void CTrackListTrackTimeView::OnRButtonUp (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
}

// マウスが動かされたとき
void CTrackListTrackTimeView::OnMouseMove (UINT nFlags, CPoint point) {
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
	
	CClientDC dc (this);
	OnPrepareDC (&dc, NULL);
	CSize sizWndOrg (pTrackListFrame->GetTimeScrollPos (), pTrackListFrame->GetRowScrollPos ());
	point += sizWndOrg;
	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += sizWndOrg;
	long lDownTrackIndex = pTrackListFrame->YtoRow (m_ptMouseDown.y);
	long lOldTrackIndex = pTrackListFrame->YtoRow (m_ptMouseMove.y);
	long lNewTrackIndex = pTrackListFrame->YtoRow (point.y);
	long lDownTime = pTrackListFrame->XtoTime (m_ptMouseDown.x);
	long lOldTime = pTrackListFrame->XtoTime (m_ptMouseMove.x);
	long lNewTime = pTrackListFrame->XtoTime (point.x);
	CRect rcRegion;

	// キャプター中ならば
	if (GetCapture () == this) {
		pSekaijuDoc->m_theCriticalSection.Lock ();
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		MIDITrack* pTempTrack = NULL;
		MIDIEvent* pTempEvent = NULL;
		MIDITrack* pNewTrack = NULL;
		MIDIEvent* pNewEvent = NULL;
		// ツール別の操作
		switch (m_lTempTool) {
		// 選択
		case ID_TRACKLIST_SELECT:
			// 移動又はコピー中
			if (m_lTempMode == 0x0402 || m_lTempMode == 0x0403) {
				long j;
				long lDownMeasure, lDownBeat, lDownTick;
				long lOldMeasure, lOldBeat, lOldTick;
				long lNewMeasure, lNewBeat, lNewTick;
				MIDIData_BreakTime (pMIDIData, lDownTime, &lDownMeasure, &lDownBeat, &lDownTick);
				MIDIData_BreakTime (pMIDIData, lOldTime, &lOldMeasure, &lOldBeat, &lOldTick);
				MIDIData_BreakTime (pMIDIData, lNewTime, &lNewMeasure, &lNewBeat, &lNewTick);
				// トラックが変更された場合
				if (lOldTrackIndex != lNewTrackIndex) {
					CHistoryUnit* pCurHistoryUnit = NULL;
					VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
					long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
					long lDeltaTrackIndex = lNewTrackIndex - lDownTrackIndex;
					long lSelectedEventCount = m_theTempSelectedEventArray.GetSize ();
					for (j = 0; j < lSelectedEventCount; j++) {
						pTempEvent = (MIDIEvent*)m_theTempSelectedEventArray.GetAt (j);
						pTempTrack = MIDIEvent_GetParent (pTempEvent);
						if (pTempTrack != MIDIData_GetFirstTrack (pMIDIData) &&
							pTempEvent->m_pPrevCombinedEvent == NULL &&
							!(MIDIEvent_IsEndofTrack (pTempEvent) &&
							MIDITrack_GetLastEvent (pTempTrack) == pTempEvent)) {
							long lTempTrackIndex = pTempEvent->m_lUser1;
							long lTargetTrackIndex = lTempTrackIndex + lDeltaTrackIndex;
							lTargetTrackIndex = CLIP (1, lTargetTrackIndex, (MAXMIDITRACKNUM - 1));
							MIDITrack* pTargetTrack = pSekaijuDoc->GetTrack (lTargetTrackIndex);
							// トラックが足りない場合はトラックを追加する
							//pSekaijuDoc->AddTrack (lTargetTrackIndex, 0x0007, pHistoryUnit);
							long lTrackCount = MIDIData_CountTrack (pMIDIData);
							long i;
							for (i = lTrackCount; i <= lTargetTrackIndex; i++) {
								VERIFY (pNewTrack = MIDITrack_Create ());
								VERIFY (MIDITrack_SetForeColor (pNewTrack, pSekaijuApp->m_lDefTrackColor[i % 16]));
								VERIFY (MIDITrack_SetInputOn (pNewTrack, TRUE)); 
								VERIFY (MIDITrack_SetInputChannel (pNewTrack, (i + 15) % 16)); 
								VERIFY (MIDITrack_SetOutputOn (pNewTrack, TRUE)); 
								VERIFY (MIDITrack_SetOutputChannel (pNewTrack, (i + 15) % 16)); 
								VERIFY (MIDIData_AddTrack (pMIDIData, pNewTrack));
								VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pNewTrack));
								VERIFY (pNewEvent = MIDIEvent_CreateEndofTrack (lTimeResolution * 4));
								VERIFY (MIDITrack_InsertEvent (pNewTrack, pNewEvent));
								//pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent);
								// EOTが挿入されたことはここでは履歴登録せず、LButtonUp時に登録する。
								//pTargetTrack = pSekaijuDoc->GetTrack (lTargetTrackIndex);
							}
							ASSERT (pTempTrack);
							VERIFY (pTargetTrack = pSekaijuDoc->GetTrack (lTargetTrackIndex));
							VERIFY (MIDITrack_RemoveEvent (pTempTrack, pTempEvent));
							VERIFY (MIDITrack_InsertEvent (pTargetTrack, pTempEvent));
							// 注意: EndofTrackイベントはRemoveすることはできるが,二重にInsertすることはできない。
							// TODO: EndofTrackイベントの親ﾄﾗｯｸがなくなってしまうバグがまだあり。
						}
					}
					Invalidate ();
				}

				// 小節が変更された場合
				if (lOldMeasure != lNewMeasure) {
					long lDownTime0 = 0;
					long lNewTime0 = 0;
					MIDIData_MakeTime (pMIDIData, lDownMeasure, 0, 0, &lDownTime0);
					MIDIData_MakeTime (pMIDIData, lNewMeasure, 0, 0, &lNewTime0);
					long lDeltaTime = lNewTime0 - lDownTime0; 
					long lSelectedEventCount = m_theTempSelectedEventArray.GetSize ();
					for (j = 0; j < lSelectedEventCount; j++) {
						pTempEvent = (MIDIEvent*)m_theTempSelectedEventArray.GetAt (j);
						if (pTempEvent->m_pPrevCombinedEvent == NULL) {
							long lTempTime = pTempEvent->m_lUser2;
							long lTargetTime = lTempTime + lDeltaTime;
							lTargetTime = CLIP (0, lTargetTime, 0x7FFFFFFF);
							MIDIEvent_SetTime (pTempEvent, lTargetTime);
						}
					}
					Invalidate ();
				}
			}

			// 選択範囲変更中
			else if (m_lTempMode == 0x0401) {
				long lMinTime, lMinMeasure, lMinBeat, lMinTick;
				long lMaxTime, lMaxMeasure, lMaxBeat, lMaxTick;
				long lMinTrackIndex; 
				long lMaxTrackIndex;

				// 古い選択範囲矩形を消す
				lMinTrackIndex = __min (lDownTrackIndex, lOldTrackIndex);
				lMaxTrackIndex = __max (lDownTrackIndex, lOldTrackIndex); 
				m_lMinY = pTrackListFrame->RowtoY (lMinTrackIndex);
				m_lMaxY = pTrackListFrame->RowtoY (lMaxTrackIndex + 1);
				if (lDownTime < lOldTime) {
					MIDIData_BreakTime (pMIDIData, lDownTime, &lMinMeasure, &lMinBeat, &lMinTick);
					MIDIData_MakeTime (pMIDIData, lMinMeasure, 0, 0, &lMinTime);
					MIDIData_BreakTime (pMIDIData, lOldTime, &lMaxMeasure, &lMaxBeat, &lMaxTick);
					MIDIData_MakeTime (pMIDIData, lMaxMeasure + 1, 0, 0, &lMaxTime);
				}
				else {
					MIDIData_BreakTime (pMIDIData, lOldTime, &lMinMeasure, &lMinBeat, &lMinTick);
					MIDIData_MakeTime (pMIDIData, lMinMeasure, 0, 0, &lMinTime);
					MIDIData_BreakTime (pMIDIData, lDownTime, &lMaxMeasure, &lMaxBeat, &lMaxTick);
					MIDIData_MakeTime (pMIDIData, lMaxMeasure + 1, 0, 0, &lMaxTime);
				}
				m_lMinX = pTrackListFrame->TimetoX (lMinTime);
				m_lMaxX = pTrackListFrame->TimetoX (lMaxTime);
				rcRegion = CRect (m_lMinX, m_lMinY, m_lMaxX, m_lMinY);
				rcRegion.NormalizeRect ();
				rcRegion.InflateRect (1, 1);
				InvalidateRect (rcRegion - sizWndOrg);
				rcRegion = CRect (m_lMinX, m_lMaxY, m_lMaxX, m_lMaxY);
				rcRegion.NormalizeRect ();
				rcRegion.InflateRect (1, 1);
				InvalidateRect (rcRegion - sizWndOrg);
				rcRegion = CRect (m_lMinX, m_lMinY, m_lMinX, m_lMaxX);
				rcRegion.NormalizeRect ();
				rcRegion.InflateRect (1, 1);
				InvalidateRect (rcRegion - sizWndOrg);
				rcRegion = CRect (m_lMaxX, m_lMinY, m_lMaxX, m_lMaxY);
				rcRegion.NormalizeRect ();
				rcRegion.InflateRect (1, 1);
				InvalidateRect (rcRegion - sizWndOrg);
				
				
				// 新しい選択範囲矩形を描画する
				lMinTrackIndex = __min (lDownTrackIndex, lNewTrackIndex);
				lMaxTrackIndex = __max (lDownTrackIndex, lNewTrackIndex); 
				m_lMinY = pTrackListFrame->RowtoY (lMinTrackIndex);
				m_lMaxY = pTrackListFrame->RowtoY (lMaxTrackIndex + 1);
				if (lDownTime < lNewTime) {
					MIDIData_BreakTime (pMIDIData, lDownTime, &lMinMeasure, &lMinBeat, &lMinTick);
					MIDIData_MakeTime (pMIDIData, lMinMeasure, 0, 0, &lMinTime);
					MIDIData_BreakTime (pMIDIData, lNewTime, &lMaxMeasure, &lMaxBeat, &lMaxTick);
					MIDIData_MakeTime (pMIDIData, lMaxMeasure + 1, 0, 0, &lMaxTime);
				}
				else {
					MIDIData_BreakTime (pMIDIData, lNewTime, &lMinMeasure, &lMinBeat, &lMinTick);
					MIDIData_MakeTime (pMIDIData, lMinMeasure, 0, 0, &lMinTime);
					MIDIData_BreakTime (pMIDIData, lDownTime, &lMaxMeasure, &lMaxBeat, &lMaxTick);
					MIDIData_MakeTime (pMIDIData, lMaxMeasure + 1, 0, 0, &lMaxTime);
				}
				m_lMinX = pTrackListFrame->TimetoX (lMinTime);
				m_lMaxX = pTrackListFrame->TimetoX (lMaxTime);
				rcRegion = CRect (m_lMinX, m_lMinY, m_lMaxX, m_lMinY);
				rcRegion.NormalizeRect ();
				rcRegion.InflateRect (1, 1);
				InvalidateRect (rcRegion - sizWndOrg);
				rcRegion = CRect (m_lMinX, m_lMaxY, m_lMaxX, m_lMaxY);
				rcRegion.NormalizeRect ();
				rcRegion.InflateRect (1, 1);
				InvalidateRect (rcRegion - sizWndOrg);
				rcRegion = CRect (m_lMinX, m_lMinY, m_lMinX, m_lMaxX);
				rcRegion.NormalizeRect ();
				rcRegion.InflateRect (1, 1);
				InvalidateRect (rcRegion - sizWndOrg);
				rcRegion = CRect (m_lMaxX, m_lMinY, m_lMaxX, m_lMaxY);
				rcRegion.NormalizeRect ();
				rcRegion.InflateRect (1, 1);
				InvalidateRect (rcRegion - sizWndOrg);
			}
	
			break;

		// スクラブ
		case ID_TRACKLIST_SPEAKER:
			m_lTempTime = pTrackListFrame->XtoTime (point.x);
			pSekaijuApp->SetPlayPosition (pSekaijuDoc, m_lTempTime);
			pSekaijuApp->SendDifferentStatus (SDS_ALL);
			break;
		}
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}
	// 非キャプター中
	else {
		
		// ツール別の操作
		switch (pTrackListFrame->m_lCurTool) {
		// 選択
		case ID_TRACKLIST_SELECT:		
			{
			pSekaijuDoc->m_theCriticalSection.Lock ();
			MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
			long lNewMeasure, lNewBeat, lNewTick;
			long lOldMeasure, lOldBeat, lOldTick;
			MIDIData_BreakTime (pMIDIData, lNewTime, &lNewMeasure, &lNewBeat, &lNewTick);
			MIDIData_BreakTime (pMIDIData, lOldTime, &lOldMeasure, &lOldBeat, &lOldTick);
			MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lNewTrackIndex);
			if (pMIDITrack) {
				if (pSekaijuDoc->IsTrackMeasureSelected (pMIDITrack, lNewMeasure) >= 2) { // 20100517新規修正
					if (nFlags & MK_CONTROL) {
						::SetCursor (pSekaijuApp->m_hCursorSizeAllCopy);
					}
					else {
						::SetCursor (pSekaijuApp->m_hCursorSizeAll);
					}
				}
				else {
					if (nFlags & MK_CONTROL) {
						::SetCursor (pSekaijuApp->m_hCursorSelectAdd2);
					}
					else {
						::SetCursor (pSekaijuApp->m_hCursorSelect2);
					}
				}
			}
			else {
				if (nFlags & MK_CONTROL) {
					::SetCursor (pSekaijuApp->m_hCursorSelectAdd2);
				}
				else {
					::SetCursor (pSekaijuApp->m_hCursorSelect2);
				}
			}
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			::Sleep (0);
			}
			break;
		case ID_TRACKLIST_SPEAKER:
			::SetCursor (pSekaijuApp->m_hCursorSpeaker);
			break;
		}
	}
	m_ptMouseMove = point;
	m_nMouseMoveFlags = nFlags;
}


// マウスホイールが回された時
void CTrackListTrackTimeView::OnMouseWheel40 (UINT nFlags, CPoint point) {
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

