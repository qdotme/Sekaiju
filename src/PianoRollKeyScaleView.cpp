//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ピアノロールキースケールビュークラス
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
#include "SekaijuView.h"
#include "SekaijuToolBar.h"
#include "ChildFrame.h"
#include "PianoRollFrame.h"
#include "PianoRollKeyScaleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE (CPianoRollKeyScaleView, CSekaijuView)

// メッセージマップ
BEGIN_MESSAGE_MAP (CPianoRollKeyScaleView, CSekaijuView)
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
CPianoRollKeyScaleView::CPianoRollKeyScaleView () {
}

// デストラクタ
CPianoRollKeyScaleView::~CPianoRollKeyScaleView () {
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// 原点の移動をオーバーライド
void CPianoRollKeyScaleView::OnPrepareDC (CDC* pDC, CPrintInfo* pInfo) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	pDC->SetWindowOrg (0, pPianoRollFrame->GetKeyScrollPos ());
}

// 描画
void CPianoRollKeyScaleView::OnDraw (CDC* pDC) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect rcClient;
	GetClientRect (&rcClient);
	int w = rcClient.Width ();
	int hh;
	int i;
	long lKeyZoom = pPianoRollFrame->GetKeyZoom ();
	//pDC->Rectangle (0, 0, w, m_nKeyZoom * 128);
	pDC->MoveTo (0, 0);
	pDC->LineTo (w, 0);
	// 10オクターブループ
	for (i = 0; i < 11; i++) {
		hh = (128 - 12 * i) * lKeyZoom ;
		// 白鍵と白鍵の間の線
		pDC->MoveTo (0, hh - lKeyZoom * 0);
		pDC->LineTo (w, hh - lKeyZoom * 0);
		pDC->MoveTo (0, hh - lKeyZoom * 3 / 2);
		pDC->LineTo (w, hh - lKeyZoom * 3 / 2);
		pDC->MoveTo (0, hh - lKeyZoom * 7 / 2);
		pDC->LineTo (w, hh - lKeyZoom * 7 / 2);
		pDC->MoveTo (0, hh - lKeyZoom * 5);
		pDC->LineTo (w, hh - lKeyZoom * 5);
		pDC->MoveTo (0, hh - lKeyZoom * 13 / 2);
		pDC->LineTo (w, hh - lKeyZoom * 13 / 2);
		pDC->MoveTo (0, hh - lKeyZoom * 17 / 2);
		pDC->LineTo (w, hh - lKeyZoom * 17 / 2);
		pDC->MoveTo (0, hh - lKeyZoom * 21 / 2);
		pDC->LineTo (w, hh - lKeyZoom * 21 / 2);
		// 黒鍵部分塗りつぶし
		pDC->FillSolidRect 
			(0, hh - lKeyZoom * 2, w * 2 / 3, lKeyZoom, RGB (0, 0, 0));
		pDC->FillSolidRect 
			(0, hh - lKeyZoom * 4, w * 2 / 3, lKeyZoom, RGB (0, 0, 0));
		pDC->FillSolidRect 
			(0, hh - lKeyZoom * 7, w * 2 / 3, lKeyZoom, RGB (0, 0, 0));
		pDC->FillSolidRect 
			(0, hh - lKeyZoom * 9, w * 2 / 3, lKeyZoom, RGB (0, 0, 0));
		pDC->FillSolidRect 
			(0, hh - lKeyZoom * 11, w * 2 / 3, lKeyZoom, RGB (0, 0, 0));
	}
	//pDC->MoveTo (0, 0);
	//pDC->LineTo (0, m_nKeyZoom * 128);
	//pDC->MoveTo (w - 1, 0);
	//pDC->LineTo (w - 1, m_nKeyZoom * 128);
	
	// 文字
	pDC->FillSolidRect 
		(0, 0, 0, 0, ::GetSysColor (COLOR_WINDOW));
	CRect rcText (rcClient);
	rcText.left = w * 2 / 3;
	CFont* pOldFont = pDC->SelectObject (&(pPianoRollFrame->m_theFont));
	for (i = 0; i < 11; i++) {
		hh = (128 - 12 * i) * lKeyZoom;
		rcText.top = hh - lKeyZoom / 2 - 6;
		rcText.bottom = hh - lKeyZoom / 2 + 6;
		CString strText;
		strText.Format (_T("%d"), i * 12);
		pDC->DrawText (strText, rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}	
	pDC->SelectObject (pOldFont);
	
	// 選択部反転
	if (GetCapture () == this) {
		long lDownY = pPianoRollFrame->KeytoY (m_lDownKey);
		long lCurY = pPianoRollFrame->KeytoY (m_lCurKey);
		long lTop = __min (lDownY, lCurY) - lKeyZoom;
		long lBottom = __max (lDownY, lCurY);
		CRect rect (0, lTop, w, lBottom);
		pDC->SetROP2 (R2_NOT);
		pDC->Rectangle (&rect);
		pDC->SetROP2 (R2_COPYPEN);
	}
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// ウィンドウ生成時
BOOL CPianoRollKeyScaleView::OnCreate (LPCREATESTRUCT lpcs) {
	return CSekaijuView::OnCreate (lpcs);
}

// キーが押された時
void CPianoRollKeyScaleView::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	switch (nChar) {
	// ↑
	case VK_UP:
		pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEUP, 
			(LPARAM)(pPianoRollFrame->m_wndKeyScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// ↓
	case VK_DOWN:
		pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEDOWN, 
			(LPARAM)(pPianoRollFrame->m_wndKeyScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// PageUp
	case VK_PRIOR:
		pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_PAGEUP, 
			(LPARAM)(pPianoRollFrame->m_wndKeyScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// PageDown
	case VK_NEXT:
		pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_PAGEDOWN, 
			(LPARAM)(pPianoRollFrame->m_wndKeyScroll.GetSafeHwnd ()));
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
void CPianoRollKeyScaleView::OnLButtonDown (UINT nFlags, CPoint point) {
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
	rcClient += CSize (0, pPianoRollFrame->GetKeyScrollPos ());
	point += CSize (0, pPianoRollFrame->GetKeyScrollPos ());

	// 履歴の記録
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_SELECT_DESELECT));
	VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());

	// 旧選択イベントの選択解除(Shiftが押されていない場合かつCtrlが押されていない場合のみ)
	if ((nFlags & MK_SHIFT) == 0 && (nFlags & MK_CONTROL) == 0) {
		pSekaijuDoc->SelectNoObject (pCurHistoryUnit);
	}

	m_lOldKey = pPianoRollFrame->YtoKey (m_ptMouseDown.y);
	m_lDownKey = pPianoRollFrame->YtoKey (point.y);
	m_lCurKey = pPianoRollFrame->YtoKey (point.y);

	// 発音
	long lCurTrackIndex = pPianoRollFrame->GetCurTrackIndex ();
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lCurTrackIndex);
	if (pMIDITrack) {
		long lTrackOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
		long lTrackOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
		if (0 <= lTrackOutputPort && lTrackOutputPort < MAXMIDIOUTDEVICENUM) {
			MIDIOut* pMIDIOut = pSekaijuApp->m_pMIDIOut[lTrackOutputPort];
			MIDIStatus* pMIDIStatus = pSekaijuApp->m_pMIDIStatus[lTrackOutputPort];
			long lChannel = lTrackOutputChannel;
			long lVelocity = pPianoRollFrame->GetCurVelocity ();
			if (lChannel < 0 || lChannel >= 16) {
				lChannel = pPianoRollFrame->GetCurChannel ();
			}
			BYTE byMsg[3];
			byMsg[0] = 0x90 | (BYTE)CLIP(0, lChannel, 15);
			byMsg[1] = (BYTE)CLIP (0, m_lDownKey + MIDITrack_GetKeyPlus (pMIDITrack), 127);
			byMsg[2] = (BYTE)CLIP (0, lVelocity + MIDITrack_GetVelocityPlus (pMIDITrack), 127);
			if (pMIDIOut) {
				MIDIOut_PutMIDIMessage (pMIDIOut, byMsg, 3);
			}
			if (pMIDIStatus) {
				MIDIStatus_PutMIDIMessage (pMIDIStatus, byMsg, 3);
			}
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
void CPianoRollKeyScaleView::OnRButtonDown (UINT nFlags, CPoint point) {
}

// マウス左ボタン離されたとき
void CPianoRollKeyScaleView::OnLButtonUp (UINT nFlags, CPoint point) {
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
	rcClient += CSize (0, pPianoRollFrame->GetKeyScrollPos ());
	point += CSize (0, pPianoRollFrame->GetKeyScrollPos ());

	if (GetCapture () == this) {
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		
		pSekaijuDoc->m_theCriticalSection.Lock ();
		KillTimer (0x21);
		ReleaseCapture ();

		long lMinKey = __min (m_lDownKey, m_lCurKey);
		long lMaxKey = __max (m_lDownKey, m_lCurKey);

		// 古いキーを消音
		long lCurTrackIndex = pPianoRollFrame->GetCurTrackIndex ();
		pMIDITrack = pSekaijuDoc->GetTrack (lCurTrackIndex);
		if (pMIDITrack) {
			long lTrackOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
			long lTrackOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
			if (0 <= lTrackOutputPort && lTrackOutputPort < MAXMIDIOUTDEVICENUM) {
				MIDIOut* pMIDIOut = pSekaijuApp->m_pMIDIOut[lTrackOutputPort];
				MIDIStatus* pMIDIStatus = pSekaijuApp->m_pMIDIStatus[lTrackOutputPort];
				long lChannel = lTrackOutputChannel;
				if (lChannel < 0 || lChannel >= 16) {
					lChannel = pPianoRollFrame->GetCurChannel ();
				}
				BYTE byMsg[3];
				byMsg[0] = 0x90 | (BYTE)CLIP(0, lChannel, 15);
				byMsg[1] = (BYTE)CLIP (0, m_lCurKey + MIDITrack_GetKeyPlus (pMIDITrack), 127);
				byMsg[2] = 0;
				if (pMIDIOut) {
					MIDIOut_PutMIDIMessage (pMIDIOut, byMsg, 3);
				}
				if (pMIDIStatus) {
					MIDIStatus_PutMIDIMessage (pMIDIStatus, byMsg, 3);
				}
			}
		}
		
		// 該当範囲にあるノートイベントの選択
		long i = 0;
		forEachTrack (pMIDIData, pMIDITrack) {
			if (pPianoRollFrame->IsTrackVisible (i)) {
				forEachEvent (pMIDITrack, pMIDIEvent) {
					if ((MIDIEvent_IsNoteOn (pMIDIEvent) ||
						MIDIEvent_IsNoteOff (pMIDIEvent)) &&
						pMIDIEvent->m_pPrevCombinedEvent == NULL) {
						long lKey = MIDIEvent_GetKey (pMIDIEvent);
						if (lMinKey <= lKey && lKey <= lMaxKey) {
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
			i++;
		}
	
		pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED |
			SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED); 
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}

}

// マウス右ボタン離されたとき
void CPianoRollKeyScaleView::OnRButtonUp (UINT nFlags, CPoint point) {
}

// マウスが動かされたとき
void CPianoRollKeyScaleView::OnMouseMove (UINT nFlags, CPoint point) {
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
	rcClient += CSize (0, pPianoRollFrame->GetKeyScrollPos ());
	point += CSize (0, pPianoRollFrame->GetKeyScrollPos ());

	if (GetCapture () == this) {
		pSekaijuDoc->m_theCriticalSection.Lock ();

		m_lCurKey = pPianoRollFrame->YtoKey (point.y);
		m_lDownKey = pPianoRollFrame->YtoKey (m_ptMouseDown.y);
		m_lOldKey = pPianoRollFrame->YtoKey (m_ptMouseMove.y);
		// 前回のキーと今回のキーが異なる場合のみ
		if (m_lOldKey != m_lCurKey) {
			// 古いキーを消音、新しいキーを発音
			long lCurTrackIndex = pPianoRollFrame->GetCurTrackIndex ();
			MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lCurTrackIndex);
			if (pMIDITrack) {
				long lTrackOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
				long lTrackOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
				if (0 <= lTrackOutputPort && lTrackOutputPort < MAXMIDIOUTDEVICENUM) {
					MIDIOut* pMIDIOut = pSekaijuApp->m_pMIDIOut[lTrackOutputPort];
					MIDIStatus* pMIDIStatus = pSekaijuApp->m_pMIDIStatus[lTrackOutputPort];
					long lChannel = lTrackOutputChannel;
					long lVelocity = pPianoRollFrame->GetCurVelocity ();
					if (lChannel < 0 || lChannel >= 16) {
						lChannel = pPianoRollFrame->GetCurChannel ();
					}
					BYTE byMsg[3];
					byMsg[0] = 0x90 | (BYTE)CLIP(0, lChannel, 15);
					byMsg[1] = (BYTE)CLIP (0, m_lOldKey + MIDITrack_GetKeyPlus (pMIDITrack), 127);
					byMsg[2] = (BYTE)0;
					if (pMIDIOut) {
						MIDIOut_PutMIDIMessage (pMIDIOut, byMsg, 3);
					}
					if (pMIDIStatus) {
						MIDIStatus_PutMIDIMessage (pMIDIStatus, byMsg, 3);
					}
					byMsg[1] = (BYTE)CLIP (0, m_lCurKey + MIDITrack_GetKeyPlus (pMIDITrack), 127);
					byMsg[2] = (BYTE)CLIP (1, lVelocity + MIDITrack_GetVelocityPlus (pMIDITrack), 127);
					if (pMIDIOut) {
						MIDIOut_PutMIDIMessage (pMIDIOut, byMsg, 3);
					}
					if (pMIDIStatus) {
						MIDIStatus_PutMIDIMessage (pMIDIStatus, byMsg, 3);
					}
				}
			}
			// 鍵盤再描画
			Invalidate (TRUE);
		}
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}
	m_ptMouseMove = point;
	m_nMouseMoveFlags = nFlags;


}

// マウス左ボタンがダブルクリックされたとき
void CPianoRollKeyScaleView::OnLButtonDblClk (UINT nFlags, CPoint point) {
}

// マウス右ボタンがダブルクリックされたとき
void CPianoRollKeyScaleView::OnRButtonDblClk (UINT nFlags, CPoint point) {
}


// タイマー時
void CPianoRollKeyScaleView::OnTimer (UINT nIDEvent) {	

	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	if (nIDEvent == 0x21) {
		if (GetCapture () == this) {
			CRect rcClient;
			GetClientRect (&rcClient);
			rcClient += CSize (0, pPianoRollFrame->GetKeyScrollPos ());
			if (!rcClient.PtInRect (m_ptMouseMove)) {
				pSekaijuDoc->m_theCriticalSection.Lock ();
				long lOldKeyScrollPos = pPianoRollFrame->GetKeyScrollPos ();
				if (m_ptMouseMove.y < rcClient.top) {
					pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEUP,
						(LPARAM)(pPianoRollFrame->m_wndKeyScroll.GetSafeHwnd ()));
				}
				else if (m_ptMouseMove.y >= rcClient.bottom) {
					pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEDOWN,
						(LPARAM)(pPianoRollFrame->m_wndKeyScroll.GetSafeHwnd ()));
				}
				WORD wX = (WORD)(m_ptMouseMove.x);
				WORD wY = (WORD)(m_ptMouseMove.y - lOldKeyScrollPos);
				PostMessage (WM_MOUSEMOVE, (WPARAM)m_nMouseMoveFlags, (LPARAM)((wY << 16) | wX));
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				::Sleep (0);
			}
		}
	}
}

// マウスホイールが回された時
void CPianoRollKeyScaleView::OnMouseWheel40 (UINT nFlags, CPoint point) {
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