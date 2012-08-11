//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ピアノロールベロシティスケールビュークラス
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
#include "PianoRollVelScaleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CPianoRollVelScaleView, CView)

// メッセージマップ
BEGIN_MESSAGE_MAP (CPianoRollVelScaleView, CView)
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
CPianoRollVelScaleView::CPianoRollVelScaleView () {
}

// デストラクタ
CPianoRollVelScaleView::~CPianoRollVelScaleView () {
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// 原点の移動
void CPianoRollVelScaleView::OnPrepareDC (CDC* pDC, CPrintInfo* pInfo) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	pDC->SetWindowOrg (0, pPianoRollFrame->GetVelScrollPos ());
}

// 描画
void CPianoRollVelScaleView::OnDraw (CDC* pDC) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	
	CRect rcClient;
	GetClientRect (&rcClient);
	pDC->DPtoLP (&rcClient);


	// 背景の塗りつぶし
	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	pDC->FillSolidRect (&rcClient, lColorBtnFace);

	// 目盛り横線の描画
	CPen penKey (PS_SOLID, 1, RGB (0, 0, 0));
	CPen penOctave (PS_SOLID, 1, RGB (0, 0, 0));
	CPen* pOldPen = pDC->SelectObject (&penKey);
	long i;
	for (i = 0; i <= 8; i++) {
		if (i != 4) {
			pDC->SelectObject (&penKey);
		}
		else {
			pDC->SelectObject (&penOctave);
		}
		long y = pPianoRollFrame->VeltoY (i * 16);
		pDC->MoveTo (rcClient.left, y);
		pDC->LineTo (rcClient.right, y);
	}

	// ベロシティ・チャンネルアフター・コントロールチェンジ値の描画
	BOOL bControlChangeVisible = FALSE;
	CString strText;
	CRect rcText (rcClient);
	pDC->SelectObject (&(pPianoRollFrame->m_theFont));
	for (i = 4; i < 4 + 127; i++) {
		if (pPianoRollFrame->IsGraphVisible (i)) {
			bControlChangeVisible = TRUE;
			break;
		}
	}
	// ベロシティ値又はチャンネルアフタータッチ値又はコントロールチェンジ値の描画(0～64～127)
	if (pPianoRollFrame->GetCurGraphKind () == 1 ||
		pPianoRollFrame->GetCurGraphKind () == 2 ||
		pPianoRollFrame->GetCurGraphKind () >= 4) {
		for (i = 0; i <= 8; i++) {
			strText.Format (_T("%d"), CLIP (0, (i * 16), 127));
			long y = pPianoRollFrame->VeltoY (i * 16);
			rcText.top = y - 8;
			rcText.bottom = y + 8;
			pDC->DrawText (strText, rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
	// ピッチベンド値の描画(-8192～0～8191)
	else if (pPianoRollFrame->GetCurGraphKind () == 3) {
		for (i = 0; i <= 8; i++) {
			strText.Format (_T("%d"), CLIP (-8192, (i - 4) *  2048, 8191));
			long y = pPianoRollFrame->VeltoY (i * 16);
			rcText.top = y - 8;
			rcText.bottom = y + 8;
			pDC->DrawText (strText, rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
	// テンポの描画(0～256)
	else if (pPianoRollFrame->GetCurGraphKind () == 0) {
		for (i = 0; i <= 8; i++) {
			strText.Format (_T("%d"), CLIP (0, (i * 32), 256));
			long y = pPianoRollFrame->VeltoY (i * 16);
			rcText.top = y - 8;
			rcText.bottom = y + 8;
			pDC->DrawText (strText, rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}

	// 選択範囲の反転(キャプタ－中のみ)
	if (GetCapture () == this) {
		long lDownY = m_ptMouseDown.y;//pPianoRollFrame->KeytoY (m_lDownKey);
		long lCurY = m_ptMouseMove.y;//pPianoRollFrame->KeytoY (m_lCurKey);
		long lTop = __min (lDownY, lCurY) - 1;
		long lBottom = __max (lDownY, lCurY);
		long w = rcClient.right;
		CRect rect (0, lTop, w, lBottom);
		pDC->SetROP2 (R2_NOT);
		pDC->Rectangle (&rect);
		pDC->SetROP2 (R2_COPYPEN);
	}
	pDC->SelectObject (pOldPen);

}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// ウィンドウ生成時
BOOL CPianoRollVelScaleView::OnCreate (LPCREATESTRUCT lpcs) {
	return CSekaijuView::OnCreate (lpcs);
}

// キーが押された時
void CPianoRollVelScaleView::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
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
	// Deleteキー
	case VK_DELETE:
		// 『編集(E)』-『削除』実行 (20090823追加)
		PostMessage (WM_COMMAND, ID_EDIT_DELETE, NULL);
		break;
	}
	return;
}

// マウス左ボタン押された時 
void CPianoRollVelScaleView::OnLButtonDown (UINT nFlags, CPoint point) {
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
	

	pSekaijuDoc->m_theCriticalSection.Lock ();

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (0, pPianoRollFrame->GetVelScrollPos ());
	point += CSize (0, pPianoRollFrame->GetVelScrollPos ());

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

	m_ptMouseDown = m_ptMouseMove = point;
	m_nMouseDownFlags = m_nMouseMoveFlags = nFlags;

	m_lOldTempoBPM = pPianoRollFrame->YtoTempoBPM (m_ptMouseDown.y);
	m_lDownTempoBPM = pPianoRollFrame->YtoTempoBPM (point.y);
	m_lCurTempoBPM = pPianoRollFrame->YtoTempoBPM (point.y);
	m_lOldVel = pPianoRollFrame->YtoVel (m_ptMouseDown.y);
	m_lDownVel = pPianoRollFrame->YtoVel (point.y);
	m_lCurVel = pPianoRollFrame->YtoVel (point.y);
	m_lOldPitchBend = pPianoRollFrame->YtoPitchBend (m_ptMouseDown.y);
	m_lDownPitchBend = pPianoRollFrame->YtoPitchBend (point.y);
	m_lCurPitchBend = pPianoRollFrame->YtoPitchBend (point.y);


	SetTimer (0x21, 55, NULL);
	SetCapture ();
	Invalidate ();

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// マウス右ボタン押された時  
void CPianoRollVelScaleView::OnRButtonDown (UINT nFlags, CPoint point) {
}

// マウス左ボタン離されたとき
void CPianoRollVelScaleView::OnLButtonUp (UINT nFlags, CPoint point) {
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
	rcClient += CSize (0, pPianoRollFrame->GetVelScrollPos ());
	point += CSize (0, pPianoRollFrame->GetVelScrollPos ());

	if (GetCapture () == this) {
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		
		pSekaijuDoc->m_theCriticalSection.Lock ();
		KillTimer (0x21);
		ReleaseCapture ();

		long lMinTempoBPM = __min (m_lDownTempoBPM, m_lCurTempoBPM);
		long lMaxTempoBPM = __max (m_lDownTempoBPM, m_lCurTempoBPM);
		long lMinVel = __min (m_lDownVel, m_lCurVel);
		long lMaxVel = __max (m_lDownVel, m_lCurVel);
		long lMinPitchBend = __min (m_lDownPitchBend, m_lCurPitchBend);
		long lMaxPitchBend = __max (m_lDownPitchBend, m_lCurPitchBend);
		long i = 0;
		forEachTrack (pMIDIData, pMIDITrack) {
			// このトラックが可視の場合
			if (pPianoRollFrame->IsTrackVisible (i)) {
				forEachEvent (pMIDITrack, pMIDIEvent) {
					// テンポイベントの場合
					if (MIDIEvent_IsTempo (pMIDIEvent)) {
						// テンポが可視の場合
						if (pPianoRollFrame->IsGraphVisible (0)) {
							long lTempo = MIDIEvent_GetTempo (pMIDIEvent);
							long lTempoBPM = 60000000 / lTempo;
							if (lMinTempoBPM <= lTempoBPM && lTempoBPM <= lMaxTempoBPM) {
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
							long lVel = MIDIEvent_GetVelocity (pMIDIEvent);
							if (lMinVel <= lVel && lVel <= lMaxVel) {
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
							long lVel = MIDIEvent_GetValue (pMIDIEvent);
							if (lMinVel <= lVel && lVel <= lMaxVel) {
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
							long lPitchBend = MIDIEvent_GetValue (pMIDIEvent);
							if (lMinPitchBend <= lPitchBend && lPitchBend <= lMaxPitchBend) {
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
							long lVel = MIDIEvent_GetValue (pMIDIEvent);
							if (lMinVel <= lVel && lVel <= lMaxVel) {
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
void CPianoRollVelScaleView::OnRButtonUp (UINT nFlags, CPoint point) {
}

// マウスが動かされたとき
void CPianoRollVelScaleView::OnMouseMove (UINT nFlags, CPoint point) {
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
	rcClient += CSize (0, pPianoRollFrame->GetVelScrollPos ());
	point += CSize (0, pPianoRollFrame->GetVelScrollPos ());

	if (GetCapture () == this) {
		pSekaijuDoc->m_theCriticalSection.Lock ();
		m_lCurTempoBPM = pPianoRollFrame->YtoTempoBPM (point.y);
		m_lDownTempoBPM = pPianoRollFrame->YtoTempoBPM (m_ptMouseDown.y);
		m_lOldTempoBPM = pPianoRollFrame->YtoTempoBPM (m_ptMouseMove.y);
		m_lCurVel = pPianoRollFrame->YtoVel (point.y);
		m_lDownVel = pPianoRollFrame->YtoVel (m_ptMouseDown.y);
		m_lOldVel = pPianoRollFrame->YtoVel (m_ptMouseMove.y);
		m_lCurPitchBend = pPianoRollFrame->YtoPitchBend (point.y);
		m_lDownPitchBend = pPianoRollFrame->YtoPitchBend (m_ptMouseDown.y);
		m_lOldPitchBend = pPianoRollFrame->YtoPitchBend (m_ptMouseMove.y);
		if (m_lOldTempoBPM != m_lCurTempoBPM ||
			m_lOldVel != m_lCurVel ||
			m_lOldPitchBend != m_lCurPitchBend) {
			Invalidate (TRUE);
		}
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}
	m_ptMouseMove = point;
	m_nMouseMoveFlags = nFlags;


}

// マウス左ボタンがダブルクリックされたとき
void CPianoRollVelScaleView::OnLButtonDblClk (UINT nFlags, CPoint point) {
}

// マウス右ボタンがダブルクリックされたとき
void CPianoRollVelScaleView::OnRButtonDblClk (UINT nFlags, CPoint point) {
}


// タイマー時
void CPianoRollVelScaleView::OnTimer (UINT nIDEvent) {	

	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	if (nIDEvent == 0x21) {
		if (GetCapture () == this) {
			CRect rcClient;
			GetClientRect (&rcClient);
			rcClient += CSize (0, pPianoRollFrame->GetVelScrollPos ());
			if (!rcClient.PtInRect (m_ptMouseMove)) {
				pSekaijuDoc->m_theCriticalSection.Lock ();
				long lOldVelScrollPos = pPianoRollFrame->GetVelScrollPos ();
				if (m_ptMouseMove.y < rcClient.top) {
					pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEUP,
						(LPARAM)(pPianoRollFrame->m_wndVelScroll.GetSafeHwnd ()));
				}
				else if (m_ptMouseMove.y >= rcClient.bottom) {
					pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEDOWN,
						(LPARAM)(pPianoRollFrame->m_wndVelScroll.GetSafeHwnd ()));
				}
				WORD wX = (WORD)(m_ptMouseMove.x);
				WORD wY = (WORD)(m_ptMouseMove.y - lOldVelScrollPos);
				PostMessage (WM_MOUSEMOVE, (WPARAM)m_nMouseMoveFlags, (LPARAM)((wY << 16) | wX));
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				::Sleep (0);
			}
		}
	}
}


// マウスホイールが回された時
void CPianoRollVelScaleView::OnMouseWheel40 (UINT nFlags, CPoint point) {
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

