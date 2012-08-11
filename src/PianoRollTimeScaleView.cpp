//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ピアノロールタイムスケールビュークラス
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
#include "PianoRollFrame.h"
#include "PianoRollTimeScaleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE (CPianoRollTimeScaleView, CSekaijuView)

// メッセージマップ
BEGIN_MESSAGE_MAP (CPianoRollTimeScaleView, CSekaijuView)
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
CPianoRollTimeScaleView::CPianoRollTimeScaleView () {
}

// デストラクタ
CPianoRollTimeScaleView::~CPianoRollTimeScaleView () {
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// 指定時刻にフラグとテキストを描画
void CPianoRollTimeScaleView::DrawFlagAndText 
(CDC* pDC, long lTime, LPCTSTR lpszText, long lColor) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	long x = pPianoRollFrame->TimetoX (lTime);
	CRect rcClient;
	GetClientRect (&rcClient);
	CRect rcBack (x, 0, x + 1024, 16);
	CRect rcFlag (x, 3, x + 3, 12);
	CRect rcText (x + 5, 0, x + 1024, 16);
	CPen pen;
	pen.CreatePen (PS_SOLID, 1, lColor);
	CPen* pOldPen = pDC->SelectObject (&pen);
	pDC->FillSolidRect (&rcBack, ::GetSysColor (COLOR_BTNFACE));
	pDC->FillSolidRect (&rcFlag, lColor);
	pDC->MoveTo (x, 3);
	pDC->LineTo (x, 16);
	pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextColor (lColor);
	pDC->DrawText (lpszText, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	pDC->SelectObject (pOldPen);
}


//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// 原点の移動をオーバーライド
void CPianoRollTimeScaleView::OnPrepareDC (CDC* pDC, CPrintInfo* pInfo) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	pDC->SetWindowOrg (pPianoRollFrame->GetTimeScrollPos (), 0);
}

// 描画
void CPianoRollTimeScaleView::OnDraw (CDC* pDC) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;

	CRect rcClient;
	GetClientRect (&rcClient);
	pDC->DPtoLP (&rcClient);

	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);
	long lColorBlack = RGB (0, 0, 0);
	long lColorWhite = RGB (255, 255, 255);

	pDC->FillSolidRect (&rcClient, lColorBtnFace);
	CFont* pOldFont = pDC->SelectObject (&(pPianoRollFrame->m_theFont));
	
	long x, xold;
	long lVisibleLeftTime = pPianoRollFrame->GetVisibleLeftTime ();
	long lVisibleRightTime = pPianoRollFrame->GetVisibleRightTime ();
	TCHAR szBuf[256];
	long lTimeMode, lTimeResolution;
	MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);

	// 上段に拍子記号・調性記号・マーカーの描画
	long lTime = 0;
	long lOldTime = 0;
	CString strText1;
	CString strText2;
	pMIDITrack = MIDIData_GetFirstTrack (pMIDIData);
	if (pMIDITrack) {
		long lColorTrack1 = MIDITrack_GetForeColor (pMIDITrack);
		forEachEvent (pMIDITrack, pMIDIEvent) {
			lTime = MIDIEvent_GetTime (pMIDIEvent);
			if (0 <= lTime && lTime <= lVisibleRightTime) {
				// テンポ
				if (MIDIEvent_IsTempo (pMIDIEvent)) {
					long lTempo;
					lTempo = MIDIEvent_GetTempo (pMIDIEvent);
					strText1.Format (_T("%1.2lf"), (double)60000000 / (double)lTempo);
					if (lOldTime != lTime) {
						if (strText2 != _T("")) {
							DrawFlagAndText (pDC, lOldTime, strText2, lColorTrack1);
						}
						strText2 = _T("");
					}
					if (strText2 != _T("")) {
						strText2 += _T(", ");
					}
					strText2 += strText1;
					lOldTime = lTime;
				}
				// 拍子記号
				else if (MIDIEvent_IsTimeSignature (pMIDIEvent)) {
					long lnn, ldd, lcc, lbb;
					MIDIEvent_GetTimeSignature (pMIDIEvent, &lnn, &ldd, &lcc, &lbb);
					strText1.Format (_T("%d/%d"), lnn, 1 << ldd);
					if (lOldTime != lTime) {
						if (strText2 != _T("")) {
							DrawFlagAndText (pDC, lOldTime, strText2, lColorTrack1);
						}
						strText2 = _T("");
					}
					if (strText2 != _T("")) {
						strText2 += _T(", ");
					}
					strText2 += strText1;
					lOldTime = lTime;
				}
				// 調性記号
				else if (MIDIEvent_IsKeySignature (pMIDIEvent)) {
					long lsf, lmi;
					MIDIEvent_GetKeySignature (pMIDIEvent, &lsf, &lmi);
					strText1.Format (_T("%d%s"), labs (lsf), lsf >= 0 ? _T("#") : _T("b"));
					if (lOldTime != lTime) {
						if (strText2 != _T("")) {
							DrawFlagAndText (pDC, lOldTime, strText2, lColorTrack1);
						}
						strText2 = _T("");
					}
					if (strText2 != _T("")) {
						strText2 += _T(", ");
					}
					strText2 += strText1;
					lOldTime = lTime;
				}
				// マーカー
				else if (MIDIEvent_IsMarker (pMIDIEvent)) {
					memset (szBuf, 0, sizeof (szBuf));
					MIDIEvent_GetText (pMIDIEvent, szBuf, TSIZEOF (szBuf) - 1);
					if (lOldTime != lTime) {
						if (strText2 != _T("")) {
							DrawFlagAndText (pDC, lOldTime, strText2, lColorTrack1);
						}
						strText2 = _T("");
					}
					if (strText2 != _T("")) {
						strText2 += _T(", ");
					}
					strText2 += szBuf;
					lOldTime = lTime;
				}
			}
		}
		if (strText2 != _T("")) {
			DrawFlagAndText (pDC, lOldTime, strText2, lColorTrack1);
		}
	}
	
	// 下段に小節ボタン描画
	if (lTimeMode == MIDIDATA_TPQNBASE) {
		long lLeftMeasure, lLeftBeat, lLeftTick;
		long lRightMeasure, lRightBeat, lRightTick;
		MIDIData_BreakTime (pMIDIData, lVisibleLeftTime, &lLeftMeasure, &lLeftBeat, &lLeftTick);
		MIDIData_BreakTime (pMIDIData, lVisibleRightTime, &lRightMeasure, &lRightBeat, &lRightTick);
		xold = pPianoRollFrame->TimetoX (lVisibleLeftTime) - 1;
		CRect theRect;
		pDC->SetBkMode (TRANSPARENT);
		for (long i = lLeftMeasure; i <= lRightMeasure + 1; i++) {
			MIDIData_MakeTime (pMIDIData, i, 0, 0, &lTime);
			x = pPianoRollFrame->TimetoX (lTime);
			pDC->MoveTo (x, 16);
			pDC->LineTo (x, 32);
			theRect = CRect (xold, 16, x, 32);
			pDC->Draw3dRect (xold, 16, x - xold, 16, lColorBtnHighlight, lColorBtnShadow);
			memset (szBuf, 0, sizeof (szBuf));
			_sntprintf (szBuf, 255, _T("%d"), i);
			pDC->SetTextColor (lColorBtnText);
			pDC->DrawText (szBuf, -1, &theRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			xold = x;
		}
	}
	else {
		long lLeftFrameNumber = lVisibleLeftTime / lTimeResolution;
		long lRightFrameNumber = lVisibleRightTime / lTimeResolution;
		xold = pPianoRollFrame->TimetoX (lVisibleLeftTime) - 1;
		CRect theRect;
		pDC->SetBkMode (TRANSPARENT);
		for (long i = lLeftFrameNumber; i <= lRightFrameNumber + 1; i++) {
			lTime = i * lTimeResolution;
			x = pPianoRollFrame->TimetoX (lTime);
			pDC->MoveTo (x, 16);
			pDC->LineTo (x, 32);
			theRect = CRect (xold, 16, x, 32);
			pDC->Draw3dRect (xold, 16, x - xold, 16, lColorBtnHighlight, lColorBtnShadow);
			memset (szBuf, 0, sizeof (szBuf));
			_sntprintf (szBuf, 255, _T("%d"), i);
			pDC->DrawText (szBuf, -1, &theRect,	DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			xold = x;
		}
	}
	pDC->SelectObject (pOldFont);

	if (GetCapture () == this) {
		long lDownX = pPianoRollFrame->TimetoX (m_lDownTime);
		long lCurX = pPianoRollFrame->TimetoX (m_lCurTime);
		long lTop = rcClient.top;
		long lBottom = rcClient.bottom;
		CRect rect (lDownX, lBottom / 2, lCurX, lBottom);
		pDC->SetROP2 (R2_NOT);
		pDC->Rectangle (&rect);
		pDC->SetROP2 (R2_COPYPEN);
	}	

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// ビューの更新
void CPianoRollTimeScaleView::OnUpdate (CView* pSender, LPARAM lHint, CObject* pHint) {
	// クリティカルセクションはロックされているものとする。
	CSekaijuView::OnUpdate (pSender, lHint, pHint);
}




//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// ウィンドウ生成時
BOOL CPianoRollTimeScaleView::OnCreate (LPCREATESTRUCT lpcs) {
	return CSekaijuView::OnCreate (lpcs);
}

// キーが押された時
void CPianoRollTimeScaleView::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	switch (nChar) {
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
	// Deleteキー
	case VK_DELETE:
		// 『編集(E)』-『削除』実行 (20090823追加)
		PostMessage (WM_COMMAND, ID_EDIT_DELETE, NULL);
		break;

	}
	return;
}

// マウス左ボタン押された時 
void CPianoRollTimeScaleView::OnLButtonDown (UINT nFlags, CPoint point) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	// 録音中は何もしない
	if (pSekaijuApp->m_bRecording) {
		return;
	}
	// MIDIデータがロックされている場合は何もしない
	if (pSekaijuDoc->m_bEditLocked) {
		return;
	}
	
	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pPianoRollFrame->GetTimeScrollPos (), 0);
	point += CSize (pPianoRollFrame->GetTimeScrollPos (), 0);

	// 上半分をクリックしたとき(演奏位置移動)
	if (point.y < rcClient.Height () / 2) {
		pSekaijuDoc->m_theCriticalSection.Lock ();
		long lTime = pPianoRollFrame->XtoTime (point.x);
		pSekaijuApp->SetPlayPosition (pSekaijuDoc, lTime);
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);		
	}

	// 下半分(小節番号部)をクリックしたとき(小節選択)
	else {

		pSekaijuDoc->m_theCriticalSection.Lock ();
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		
		// 履歴の記録
		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName; // 20110103追加
		VERIFY (strHistoryName.LoadString (IDS_SELECT_DESELECT)); // 20110103追加
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName)); // 20110103修正
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());

		// 旧選択イベントの選択解除(Shiftが押されていない場合かつCtrlが押されていない場合のみ)
		if ((nFlags & MK_SHIFT) == 0 && (nFlags & MK_CONTROL) == 0) {
			pSekaijuDoc->SelectNoObject (pCurHistoryUnit);
		}

		m_lTempSnap = pPianoRollFrame->GetCurSnap ();
		m_lTempSnap = CLIP (1, m_lTempSnap, MIDIData_GetTimeResolution (pMIDIData));
		m_lOldTime = pPianoRollFrame->XtoTime (m_ptMouseDown.x) / m_lTempSnap * m_lTempSnap;
		m_lDownTime = pPianoRollFrame->XtoTime (point.x) / m_lTempSnap * m_lTempSnap;
		m_lCurTime = pPianoRollFrame->XtoTime (point.x) / m_lTempSnap * m_lTempSnap;

		SetCapture ();
		SetTimer (0x21, 55, NULL);
		pPianoRollFrame->m_bAutoPageUpdate = FALSE;
		Invalidate ();

		m_ptMouseDown = m_ptMouseMove = point;
		m_nMouseDownFlags = m_nMouseMoveFlags = nFlags;

		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}
}

// マウス右ボタン押された時  
void CPianoRollTimeScaleView::OnRButtonDown (UINT nFlags, CPoint point) {
}

// マウス左ボタン離されたとき
void CPianoRollTimeScaleView::OnLButtonUp (UINT nFlags, CPoint point) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pPianoRollFrame->GetTimeScrollPos (), 0);
	point += CSize (pPianoRollFrame->GetTimeScrollPos (), 0);

	// 録音中は何もしない
	if (pSekaijuApp->m_bRecording) {
		return;
	}
	// MIDIデータがロックされている場合は何もしない
	if (pSekaijuDoc->m_bEditLocked) {
		return;
	}

	// キャプタ－中の場合
	if (GetCapture () == this) {
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		
		pSekaijuDoc->m_theCriticalSection.Lock ();
		KillTimer (0x21);
		ReleaseCapture ();

		long lMinTime = __min (m_lDownTime, m_lCurTime);
		long lMaxTime = __max (m_lDownTime, m_lCurTime);

		// 該当範囲にあるノートイベントの及び各種グラフイベントの選択
		long i = 0;
		forEachTrack (pMIDIData, pMIDITrack) {
			if (pPianoRollFrame->IsTrackVisible (i)) {
				forEachEvent (pMIDITrack, pMIDIEvent) {
					// ノートイベントの場合
					if ((MIDIEvent_IsNoteOn (pMIDIEvent) ||
						MIDIEvent_IsNoteOff (pMIDIEvent)) &&
						pMIDIEvent->m_pPrevCombinedEvent == NULL) {
						long lTime = MIDIEvent_GetTime (pMIDIEvent);
						if (lMinTime <= lTime && lTime <= lMaxTime) {
							if (pSekaijuDoc->IsEventSelected (pMIDIEvent) == 0) {
								MIDIEvent* pCloneEvent = 
									pSekaijuDoc->SelectEvent 
									(pMIDIEvent, 1, pCurHistoryUnit);
								ASSERT (pCloneEvent);
								pMIDIEvent = pCloneEvent;
							}
						}
					}
					// テンポイベントの場合
					else if (MIDIEvent_IsTempo (pMIDIEvent)) {
						// テンポが可視の場合
						if (pPianoRollFrame->IsGraphVisible (0)) {
							long lTime = MIDIEvent_GetTime (pMIDIEvent);
							if (lMinTime <= lTime && lTime <= lMaxTime) {
								if (pSekaijuDoc->IsEventSelected (pMIDIEvent) == 0) {
									MIDIEvent* pCloneEvent = 
										pSekaijuDoc->SelectEvent 
										(pMIDIEvent, 1, pCurHistoryUnit);
									ASSERT (pCloneEvent);
									pMIDIEvent = pCloneEvent;
								}
							}
						}
					}
					// ノートイベントの場合
					else if ((MIDIEvent_IsNoteOn (pMIDIEvent) ||
						MIDIEvent_IsNoteOff (pMIDIEvent)) &&
						pMIDIEvent->m_pPrevCombinedEvent == NULL) {
						// ベロシティが可視の場合
						if (pPianoRollFrame->IsGraphVisible (1)) {
							long lTime = MIDIEvent_GetTime (pMIDIEvent);
							if (lMinTime <= lTime && lTime <= lMaxTime) {
								if (pSekaijuDoc->IsEventSelected (pMIDIEvent) == 0) {
									MIDIEvent* pCloneEvent = 
										pSekaijuDoc->SelectEvent 
										(pMIDIEvent, 1, pCurHistoryUnit);
									ASSERT (pCloneEvent);
									pMIDIEvent = pCloneEvent;
								}
							}
						}
					}
					// チャンネルアフタータッチイベントの場合
					else if (MIDIEvent_IsChannelAftertouch (pMIDIEvent)) {
						// チャンネルアフタータッチが可視の場合
						if (pPianoRollFrame->IsGraphVisible (2)) {
							long lTime = MIDIEvent_GetTime (pMIDIEvent);
							if (lMinTime <= lTime && lTime <= lMaxTime) {
								if (pSekaijuDoc->IsEventSelected (pMIDIEvent) == 0) {
									MIDIEvent* pCloneEvent = 
										pSekaijuDoc->SelectEvent 
										(pMIDIEvent, 1, pCurHistoryUnit);
									ASSERT (pCloneEvent);
									pMIDIEvent = pCloneEvent;
								}
							}
						}
					}
					// ピッチベンドイベントの場合
					else if (MIDIEvent_IsPitchBend (pMIDIEvent)) {
						// ピッチベンドが可視の場合
						if (pPianoRollFrame->IsGraphVisible (3)) {
							long lTime = MIDIEvent_GetTime (pMIDIEvent);
							if (lMinTime <= lTime && lTime <= lMaxTime) {
								if (pSekaijuDoc->IsEventSelected (pMIDIEvent) == 0) {
									MIDIEvent* pCloneEvent = 
										pSekaijuDoc->SelectEvent 
										(pMIDIEvent, 1, pCurHistoryUnit);
									ASSERT (pCloneEvent);
									pMIDIEvent = pCloneEvent;
								}
							}
						}
					}
					// コントロールチェンジの場合
					else if (MIDIEvent_IsControlChange (pMIDIEvent)) {
						long lNumber = MIDIEvent_GetNumber (pMIDIEvent);
						// 指定ナンバー(CC#)のコントロールチェンジが可視の場合
						if (pPianoRollFrame->IsGraphVisible (4 + lNumber)) {
							long lTime = MIDIEvent_GetTime (pMIDIEvent);
							if (lMinTime <= lTime && lTime <= lMaxTime) {
								if (pSekaijuDoc->IsEventSelected (pMIDIEvent) == 0) {
									MIDIEvent* pCloneEvent = 
										pSekaijuDoc->SelectEvent 
										(pMIDIEvent, 1, pCurHistoryUnit);
									ASSERT (pCloneEvent);
									pMIDIEvent = pCloneEvent;
								}
							}
						}
					}
				}
			}
			i++;
		}

		pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED |
			SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED); 
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}

}

// マウス右ボタン離されたとき
void CPianoRollTimeScaleView::OnRButtonUp (UINT nFlags, CPoint point) {
}

// マウスが動かされたとき
void CPianoRollTimeScaleView::OnMouseMove (UINT nFlags, CPoint point) {
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

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pPianoRollFrame->GetTimeScrollPos (), 0);
	point += CSize (pPianoRollFrame->GetTimeScrollPos (), 0);

	// キャプタ－中の場合
	if (GetCapture () == this) {
		pSekaijuDoc->m_theCriticalSection.Lock ();

		m_lCurTime = pPianoRollFrame->XtoTime (point.x) / m_lTempSnap * m_lTempSnap;
		m_lDownTime = pPianoRollFrame->XtoTime (m_ptMouseDown.x) / m_lTempSnap * m_lTempSnap;
		m_lOldTime = pPianoRollFrame->XtoTime (m_ptMouseMove.x) / m_lTempSnap * m_lTempSnap;
		// 前回のタイムと今回のタイムが異なる場合のみ
		if (m_lOldTime != m_lCurTime) {
			// 目盛り再描画
			Invalidate (TRUE);
		}
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}
	m_ptMouseMove = point;
	m_nMouseMoveFlags = nFlags;

}

// マウス左ボタンがダブルクリックされたとき
void CPianoRollTimeScaleView::OnLButtonDblClk (UINT nFlags, CPoint point) {
}

// マウス右ボタンがダブルクリックされたとき
void CPianoRollTimeScaleView::OnRButtonDblClk (UINT nFlags, CPoint point) {
}


// タイマー時
void CPianoRollTimeScaleView::OnTimer (UINT nIDEvent) {	
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	// マウスキャプター中にクライアント領域をはみ出した場合の自動スクロール処理
	if (nIDEvent == 0x21) {
		if (GetCapture () == this) {
			CRect rcClient;
			GetClientRect (&rcClient);
			rcClient += CSize (pPianoRollFrame->GetTimeScrollPos (), 0);
			if (!rcClient.PtInRect (m_ptMouseMove)) {
				pSekaijuDoc->m_theCriticalSection.Lock ();
				long lOldTimeScrollPos = pPianoRollFrame->GetTimeScrollPos ();
				if (m_ptMouseMove.x < rcClient.left) {
					pPianoRollFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_LINEUP,
						(LPARAM)(pPianoRollFrame->m_wndTimeScroll.GetSafeHwnd ()));
				}
				else if (m_ptMouseMove.x >= rcClient.right) {
					pPianoRollFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_LINEDOWN,
						(LPARAM)(pPianoRollFrame->m_wndTimeScroll.GetSafeHwnd ()));
				}
				WORD wX = (WORD)(m_ptMouseMove.x - lOldTimeScrollPos);
				WORD wY = (WORD)(m_ptMouseMove.y);
				PostMessage (WM_MOUSEMOVE, (WPARAM)m_nMouseMoveFlags, (LPARAM)((wY << 16) | wX));
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				::Sleep (0);
			}
		}
	}
}

// マウスホイールが回された時
void CPianoRollTimeScaleView::OnMouseWheel40 (UINT nFlags, CPoint point) {
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
		long lKeyScrollPos = pPianoRollFrame->GetKeyScrollPos ();
		long lKeyZoom = pPianoRollFrame->GetKeyZoom ();
		lKeyScrollPos -= lKeyZoom * lDelta / WHEELDELTA;
		pPianoRollFrame->SetKeyScrollPos (lKeyScrollPos);
	}
}
