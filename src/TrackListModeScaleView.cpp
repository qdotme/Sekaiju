//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// トラックリストモードスケールビュークラス
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
#include "TrackListModeScaleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CTrackListModeScaleView, CSekaijuView)

// メッセージマップ
BEGIN_MESSAGE_MAP (CTrackListModeScaleView, CSekaijuView)
	ON_WM_CREATE ()
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
CTrackListModeScaleView::CTrackListModeScaleView () {
}

// デストラクタ
CTrackListModeScaleView::~CTrackListModeScaleView () {
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// 原点の移動
void CTrackListModeScaleView::OnPrepareDC (CDC* pDC, CPrintInfo* pInfo) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	pDC->SetWindowOrg (pTrackListFrame->GetColumnScrollPos (), 0);
}

// 描画
void CTrackListModeScaleView::OnDraw (CDC* pDC) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CRect rcClient;
	GetClientRect (&rcClient);
	pDC->DPtoLP (&rcClient);
	CFont* pOldFont = pDC->SelectObject (&(pTrackListFrame->m_theFont));
	CRect theRect (0, 0, 0, 0);
	long lColumnZoom = pTrackListFrame->GetColumnZoom ();
	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);
	long lColorBlack = RGB (0, 0, 0);
	long lColorWhite = RGB (255, 255, 255);
	// 背景の塗りつぶし
	pDC->FillSolidRect (&rcClient, lColorBtnFace);
	pDC->SetBkMode (TRANSPARENT);
	// 各列の描画
	for (int j = 0; j < TRACKLISTFRAME_NUMCOLUMN; j ++) {
		theRect.top = rcClient.top;
		theRect.bottom = rcClient.bottom;
		theRect.left = theRect.right; 
		theRect.right = theRect.right + pTrackListFrame->GetColumnBaseWidth (j) * lColumnZoom;
		pDC->Draw3dRect (&theRect, lColorBtnHighlight, lColorBtnShadow);
		pDC->DrawText (pTrackListFrame->GetColumnTitle (j), &theRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
	pDC->SelectObject (pOldFont);
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// ウィンドウ生成時
BOOL CTrackListModeScaleView::OnCreate (LPCREATESTRUCT lpcs) {
	return CSekaijuView::OnCreate (lpcs);
}

// マウス左ボタン押された時  
void CTrackListModeScaleView::OnLButtonDown (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	point += CSize (pTrackListFrame->GetColumnScrollPos (), 0);
	// カーソルが境界上にあるか調べる
	long j = 0;
	long lBorderX = 0;
	for (j = 0; j < TRACKLISTFRAME_NUMCOLUMN; j++) {
		lBorderX += pTrackListFrame->GetColumnBaseWidth (j) * pTrackListFrame->GetColumnZoom ();
		if (lBorderX - 2 <= point.x && point.x <= lBorderX + 2) {
			break;
		}
	}
	// カーソルが境界上にあった場合
	if (0 <= j && j < TRACKLISTFRAME_NUMCOLUMN) {
		SetCapture ();
		m_lTempColumnIndex = j;
		m_lTempColumnBaseWidth = pTrackListFrame->GetColumnBaseWidth (j);
		m_ptMouseDown = m_ptMouseMoveOld = point;
		::SetCursor (pSekaijuApp->m_hCursorSizeWE);
	}
}

// マウス右ボタン押された時  
void CTrackListModeScaleView::OnRButtonDown (UINT nFlags, CPoint point) {

}

// マウス左ボタン離されたとき
void CTrackListModeScaleView::OnLButtonUp (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (GetCapture () == this) {
		ReleaseCapture ();
		::SetCursor (pSekaijuApp->m_hCursorArrow);
		pTrackListFrame->RecalcColumnScrollInfo ();
		pTrackListFrame->m_pModeScaleView->Invalidate ();
		pTrackListFrame->m_pTrackModeView->Invalidate ();
	}
}

// マウス右ボタン離されたとき
void CTrackListModeScaleView::OnRButtonUp (UINT nFlags, CPoint point) {

}

// マウスが動かされたとき
void CTrackListModeScaleView::OnMouseMove (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	point += CSize (pTrackListFrame->GetColumnScrollPos (), 0);
	// キャプター中
	if (GetCapture () == this) {
		CRect rcClient;
		GetClientRect (&rcClient);
		CSize szMouseDelta = point.x - m_ptMouseDown.x;
		long lNewColumnBaseWidth = 
			(m_lTempColumnBaseWidth * pTrackListFrame->GetColumnZoom () + szMouseDelta.cx) / 
			pTrackListFrame->GetColumnZoom ();
		lNewColumnBaseWidth = CLIP (1, lNewColumnBaseWidth, 1024);
		if (lNewColumnBaseWidth != pTrackListFrame->GetColumnBaseWidth (m_lTempColumnIndex)) {
			CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
			pTrackListFrame->SetColumnBaseWidth (m_lTempColumnIndex, lNewColumnBaseWidth);
			pTrackListFrame->m_pModeScaleView->Invalidate ();
			pTrackListFrame->m_pTrackModeView->Invalidate ();
		}
		m_ptMouseMoveOld = point;
	}
	// 非キャプター中
	else {
		// カーソルが境界上にあるか調べる
		long j = 0;
		long lBorderX = 0;
		for (j = 0; j < TRACKLISTFRAME_NUMCOLUMN; j++) {
			lBorderX += pTrackListFrame->GetColumnBaseWidth (j) * pTrackListFrame->GetColumnZoom ();
			if (lBorderX - 2 <= point.x && point.x <= lBorderX + 2) {
				break;
			}
		}
		// カーソルが境界上にあった場合
		if (0 <= j && j < TRACKLISTFRAME_NUMCOLUMN) {
			::SetCursor (pSekaijuApp->m_hCursorSizeWE);
		}
		// カーソルが境界上にない場合
		else {
			::SetCursor (pSekaijuApp->m_hCursorArrow);
		}
	}
}

// マウスホイールが回された時
void CTrackListModeScaleView::OnMouseWheel40 (UINT nFlags, CPoint point) {
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
