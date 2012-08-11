//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 世界樹ツールバークラス
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
#include "Resource.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuToolBar.h"
#include "MainFrame.h"
#include "SekaijuDoc.h"




// デバッグ用
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC (CSekaijuToolBar, CToolBar)

// メッセージマップ
BEGIN_MESSAGE_MAP (CSekaijuToolBar, CToolBar)
	ON_WM_NCPAINT()
	ON_WM_HSCROLL ()
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CSekaijuToolBar::CSekaijuToolBar () {
}

// デストラクタ
CSekaijuToolBar::~CSekaijuToolBar () {
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// http://support.microsoft.com/kb/843490/jaの問題に対応
void CSekaijuToolBar::EraseNonClient () {
	// Get window DC that is clipped to the non-client area.
	CWindowDC dc(this);
	CRect rectClient;
	GetClientRect(rectClient);
	CRect rectWindow;
	GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);
	rectClient.OffsetRect(-rectWindow.left, -rectWindow.top);
	dc.ExcludeClipRect(rectClient);

	// Draw the borders in the non-client area.
	rectWindow.OffsetRect(-rectWindow.left, -rectWindow.top);
	DrawBorders(&dc, rectWindow);

	// Erase the parts that are not drawn.
	dc.IntersectClipRect(rectWindow);
	SendMessage(WM_ERASEBKGND, (WPARAM)dc.m_hDC);

	// Draw the gripper in the non-client area.
	DrawGripper(&dc, rectWindow);
}

// http://support.microsoft.com/kb/843490/jaの問題に対応
void CSekaijuToolBar::DoPaint (CDC* pDC) {
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	// Paint inside the client area.
	CRect rect;
	GetClientRect(rect);
	DrawBorders(pDC, rect);
	DrawGripper(pDC, rect);
}

// http://support.microsoft.com/kb/843490/jaの問題に対応
void CSekaijuToolBar::DrawGripper (CDC* pDC, const CRect& rect) {
	pDC->FillSolidRect( &rect, ::GetSysColor(COLOR_BTNFACE)); // Fill in the background.
	//CToolBar::DrawGripper(pDC,rect); //VC++4.0std(MFC4.0)では未対応
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// http://support.microsoft.com/kb/843490/jaの問題に対応
void CSekaijuToolBar::OnNcPaint() {
	Invalidate (TRUE); // VC++4.0std(MFC4.0)でツールバーが再描画されないのを防止
	EraseNonClient();
}

// 位置スクロールバーを動かしたときの挙動
void CSekaijuToolBar::OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if (pMainFrame == NULL) {
		return;
	}
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame ()->GetActiveDocument());
	if (pSekaijuDoc == NULL) {
		return;
	}
	if (pScrollBar == &(pMainFrame->m_wndPositionScroll)) {
		switch (nSBCode) {
		case SB_LINELEFT:
			pMainFrame->SendMessage (WM_COMMAND, ID_CONTROL_PREVBEAT, 0);
			break;
		case SB_LINERIGHT:
			pMainFrame->SendMessage (WM_COMMAND, ID_CONTROL_NEXTBEAT, 0);
			break;
		case SB_PAGELEFT:
			pMainFrame->SendMessage (WM_COMMAND, ID_CONTROL_PREVMEASURE, 0);
			break;
		case SB_PAGERIGHT:
			pMainFrame->SendMessage (WM_COMMAND, ID_CONTROL_NEXTMEASURE, 0);
			break;
		case SB_LEFT: // 20090220追加
			pMainFrame->SendMessage (WM_COMMAND, ID_CONTROL_TOBEGIN, 0);
			break;
		case SB_RIGHT: // 20090220追加
			pMainFrame->SendMessage (WM_COMMAND, ID_CONTROL_TOEND, 0);
			break;
		case SB_THUMBTRACK:
			if (!pSekaijuApp->m_bRecording) {
				pSekaijuDoc->m_theCriticalSection.Lock ();
				pSekaijuApp->SendAllNoteOff ();
				pSekaijuApp->SendAllHold1Off ();
				pSekaijuApp->SendAllSostenutoOff ();
				pSekaijuApp->SendAllHold2Off ();
				pSekaijuApp->SendAllSoundOff ();
				pSekaijuApp->SetPlayPosition (pSekaijuDoc, nPos);
				pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED);
				pSekaijuDoc->m_theCriticalSection.Unlock ();
			}
			break;
		}

	}
}

