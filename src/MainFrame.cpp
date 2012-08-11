//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MDI親フレームウィンドウクラス
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
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuToolBar.h"
#include "MainFrame.h"
#include "SekaijuDoc.h"


#ifndef TBSTYLE_ALTDRAG
#define TBSTYLE_ALTDRAG 0x0400
#endif
#ifndef TBSTYLE_FLAT
#define TBSTYLE_FLAT    0x0800
#endif
#ifndef TBSTYLE_LIST
#define TBSTYLE_LIST    0x1000
#endif


// デバッグ用
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC (CMainFrame, CMDIFrameWnd)

// メッセージマップ
BEGIN_MESSAGE_MAP (CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE ()
	ON_WM_DESTROY ()
	ON_WM_TIMER ()
	ON_WM_HSCROLL ()
	ON_WM_MOUSEWHEEL40 ()
	ON_COMMAND (ID_VIEW_TOOLBAR1, OnViewToolbar1)
	ON_UPDATE_COMMAND_UI (ID_VIEW_TOOLBAR1, OnUpdateViewToolbar1UI)
	ON_COMMAND (ID_VIEW_TOOLBAR2, OnViewToolbar2)
	ON_UPDATE_COMMAND_UI (ID_VIEW_TOOLBAR2, OnUpdateViewToolbar2UI)
	ON_MESSAGE (WM_COMMANDWAKEUP, OnCommandWakeUp)
	ON_MESSAGE (WM_COMMANDREADSHM, OnCommandReadShm)
	ON_MESSAGE (WM_COMMANDFILEOPEN, OnCommandFileOpen)
END_MESSAGE_MAP ()

// ステータス ライン インジケータ
static UINT indicators[] = {
	ID_SEPARATOR,           
	ID_INDICATOR_KANA,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CMainFrame::CMainFrame () {
	m_strWndClass = AfxGetAppName ();
	m_strTempFileName = _T("");
}

// デストラクタ
CMainFrame::~CMainFrame () {
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// 再生位置スクロールバー(ツールバー2上)の範囲設定
void CMainFrame::SetPositionScrollRange (long lStartTime, long lEndTime, BOOL bRedraw) {
	ASSERT (m_wndPositionScroll.GetSafeHwnd ());
	m_wndPositionScroll.SetScrollRange (lStartTime, lEndTime, bRedraw);
}

// SetWindowTextのテキストが異なる場合のみ更新するヴァージョン
int CMainFrame::SetWindowTextWhenDifferent (CWnd* pWnd, LPCTSTR lpszText) {
	ASSERT (pWnd);
	CString strCurText;
	pWnd->GetWindowText (strCurText);
	if (_tcscmp (strCurText, lpszText) != 0) {
		pWnd->SetWindowText (lpszText);
	}
	return 1;
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// ウィンドウ作成直前
BOOL CMainFrame::PreCreateWindow (CREATESTRUCT& cs) {
	// 注意：このPreCreateWindowを有効に働かせるためには、
	// CMainFrameをLoadFrameではなくCreate関数で作成すること。

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();

	// デフォルトのMDIフレーム用WNDCLASSを作成し、登録する。
	BOOL bRet = CMDIFrameWnd::PreCreateWindow (cs);
	// cs.lpszClassが"AfxMDIFrame40(d)"になる.

	// メインウィンドウのクラス名をm_strWndClassにする。
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
	// m_strWndClassがまだ登録されていない場合
	if (!::GetClassInfo(hInst, m_strWndClass, &wndcls)) {
		// デフォルトのWNDCLASSを取得
		bRet = ::GetClassInfo(hInst, cs.lpszClass, &wndcls);
		if (bRet) {
			// デフォルトのWNDCLASSを固定クラス名に改造して登録
			wndcls.lpszClassName = m_strWndClass;
			wndcls.hIcon = ::LoadIcon (hInst, MAKEINTRESOURCE(IDR_MAINFRAME));
			if (::RegisterClass(&wndcls)) {
				cs.lpszClass = m_strWndClass;
	 	    }
			else {
				bRet = FALSE;
			}
		}
	}

	// csの各メンバ変数を更新する。
	//cs.hInstance;
	//cs.hMenu;
	//cs.hwndParent;
	// 起動時に前回のウィンドウ位置を復元する場合
	if (pSekaijuApp->m_theGeneralOption.m_bRestoreWindowPlacement) {
		// 最小化
		if (pSekaijuApp->m_theWindowPlacement.m_bIconic ||
			pSekaijuApp->m_nCmdShow == SW_SHOWMINIMIZED) {
			cs.cy = CW_USEDEFAULT;
			cs.cx = CW_USEDEFAULT;
			cs.y = CW_USEDEFAULT;
			cs.x = CW_USEDEFAULT;
			cs.style |= WS_MINIMIZE;
		}
		// 最大化
		else if (pSekaijuApp->m_theWindowPlacement.m_bZoomed ||
			pSekaijuApp->m_nCmdShow == SW_SHOWMAXIMIZED) {
			cs.cy = CW_USEDEFAULT;
			cs.cx = CW_USEDEFAULT;
			cs.y = CW_USEDEFAULT;
			cs.x = CW_USEDEFAULT;
			cs.style |= WS_MAXIMIZE;
		}
		// その他
		else {
			cs.cy = pSekaijuApp->m_theWindowPlacement.m_nHeight;
			cs.cx = pSekaijuApp->m_theWindowPlacement.m_nWidth;
			cs.y = pSekaijuApp->m_theWindowPlacement.m_nY;
			cs.x = pSekaijuApp->m_theWindowPlacement.m_nX;
		}
	}
	// デフォルトのウィンドウ位置を用いる場合
	else {
		cs.cy = CW_USEDEFAULT;
		cs.cx = CW_USEDEFAULT;
		cs.y = CW_USEDEFAULT;
		cs.x = CW_USEDEFAULT;
	}
	//cs.lpszName;
	//cs.lpszClass;
	//cs.dwExStyle;

	// 注意：最小化と最大化は、ShowWindow時のm_nCmdShowの
	// 値によって最終的に決まるので、ShowWindowの直前で
	// m_nCmdShowの値も変更しておかなければならない。

	// return直後のCreateWindowExにcsの各メンバ変数が渡される。

	return bRet;
}

// 現在アクティブな子フレームウィンドウを取得(なければthis)
CFrameWnd* CMainFrame::GetActiveFrame () {
	// check first for MDI client window not created
	if (m_hWndMDIClient == NULL) {
		return this;
	}
	// MDI client has been created, get active MDI child
	HWND hWnd = (HWND)::SendMessage(m_hWndMDIClient, WM_MDIGETACTIVE, 0, NULL);
	if (!::IsWindow (hWnd)) {
		return this;
	}
	CMDIChildWnd* pWnd = (CMDIChildWnd*)CWnd::FromHandle(hWnd);
	if (pWnd == NULL) {
		return this;
	}
	if (!pWnd->IsKindOf (RUNTIME_CLASS (CMDIChildWnd))) {
		return this;
	}

	// check for special pseudo-inactive state
	if (pWnd != NULL && pWnd->m_bPseudoInactive &&
		(pWnd->GetStyle () & WS_VISIBLE) == 0) {
		// Window is hidden, active, but m_bPseudoInactive -- return NULL
		return this;
	}
	return pWnd;
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// ウィンドウ作成時
int CMainFrame::OnCreate (LPCREATESTRUCT lpCreateStruct) {
	CRect rcTemp;
	
	if (CMDIFrameWnd::OnCreate (lpCreateStruct) == -1) {
		return -1;
	}

	// ツールバー1の作成
	if (!m_wndToolBar1.Create (this) ||
		!m_wndToolBar1.LoadToolBar (IDR_MAINFRAME1)) {
		TRACE0 ("Failed to create toolbar\n");
		return -1;
	}
	m_wndToolBar1.SetBarStyle (m_wndToolBar1.GetBarStyle () |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);
	m_wndToolBar1.EnableDocking (CBRS_ALIGN_ANY);
	EnableDocking (CBRS_ALIGN_ANY);
	DockControlBar (&m_wndToolBar1);
	//m_wndToolBar1.ModifyStyle( 0, TBSTYLE_FLAT );

	// ツールバー2の作成
	if (!m_wndToolBar2.Create (this) ||
		!m_wndToolBar2.LoadToolBar (IDR_MAINFRAME2)) {
		TRACE0 ("Failed to create toolbar\n");
		return -1;
	}
	m_wndToolBar2.SetBarStyle (m_wndToolBar2.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndToolBar2.EnableDocking (CBRS_ALIGN_ANY);
	EnableDocking (CBRS_ALIGN_ANY);
	DockControlBar (&m_wndToolBar2);
	//m_wndToolBar2.ModifyStyle( 0, TBSTYLE_FLAT );

	// 時:分:秒:ミリ秒の作成
	m_wndToolBar2.SetButtonInfo (0, IDC_MILLISECEDIT, TBBS_SEPARATOR, 100);
	m_wndToolBar2.GetItemRect (0, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 22;
	if (!m_wndMillisecEdit.CreateEx (
		WS_EX_CLIENTEDGE, _T("EDIT"), NULL,
		WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_CENTER | ES_READONLY,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar2.GetSafeHwnd (), (HMENU)IDC_MILLISECEDIT)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndMillisecEdit.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));

	// 小節:拍:ティックの作成
	m_wndToolBar2.SetButtonInfo (2, IDC_TIMEEDIT, TBBS_SEPARATOR, 100);
	m_wndToolBar2.GetItemRect (2, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 22;
	if (!m_wndTimeEdit.CreateEx (
		WS_EX_CLIENTEDGE, _T("EDIT"), NULL,
		WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_CENTER | ES_READONLY,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar2.GetSafeHwnd (), (HMENU)IDC_TIMEEDIT)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndTimeEdit.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));

	// 再生位置スクロールバーの作成
	m_wndToolBar2.SetButtonInfo (9, IDC_POSITIONSCROLL, TBBS_SEPARATOR, 120);
	m_wndToolBar2.GetItemRect (9, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 22;
	if (!m_wndPositionScroll.CreateEx (
		0, _T("SCROLLBAR"), NULL,
		WS_VISIBLE | WS_TABSTOP | WS_CHILD,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar2.GetSafeHwnd (), (HMENU)IDC_POSITIONSCROLL)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}

	// 拍子･調性の作成
	m_wndToolBar2.SetButtonInfo (19, IDC_MILLISECEDIT, TBBS_SEPARATOR, 60);
	m_wndToolBar2.GetItemRect (19, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 22;
	if (!m_wndMeasureEdit.CreateEx (
		WS_EX_CLIENTEDGE, _T("EDIT"), NULL,
		WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_CENTER | ES_READONLY,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar2.GetSafeHwnd (), (HMENU)IDC_MEASUREEDIT)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndMeasureEdit.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));

	// テンポ[BPM]の作成
	m_wndToolBar2.SetButtonInfo (21, IDC_TIMEEDIT, TBBS_SEPARATOR, 60);
	m_wndToolBar2.GetItemRect (21, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 22;
	if (!m_wndTempoEdit.CreateEx (
		WS_EX_CLIENTEDGE, _T("EDIT"), NULL,
		WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_CENTER | ES_READONLY,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar2.GetSafeHwnd (), (HMENU)IDC_TEMPOEDIT)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndTempoEdit.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));

	// ステータスバーの作成
	if (!m_wndStatusBar.Create (this) ||
		!m_wndStatusBar.SetIndicators (indicators, sizeof(indicators)/sizeof(UINT))) {
		TRACE0 ("Failed to create status bar\n");
		return -1;
	}

	// ツールバーのテキスト更新用タイマー起動
	SetTimer (0x03, 55, NULL);

	// ファイルのドロップを許可
	DragAcceptFiles (TRUE);

	return TRUE;
}

// ウィンドウ破壊時
void CMainFrame::OnDestroy () {
	// ツールバーのテキスト更新用タイマー終了
	KillTimer (0x03);
	// ウィンドウ位置の保持
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CRect rcWindow;
	GetWindowRect (&rcWindow);
	pSekaijuApp->m_theWindowPlacement.m_bIconic = this->IsIconic ();
	pSekaijuApp->m_theWindowPlacement.m_bZoomed = this->IsZoomed ();
	pSekaijuApp->m_theWindowPlacement.m_nX = rcWindow.left;
	pSekaijuApp->m_theWindowPlacement.m_nY = rcWindow.top;
	pSekaijuApp->m_theWindowPlacement.m_nWidth = rcWindow.Width ();
	pSekaijuApp->m_theWindowPlacement.m_nHeight = rcWindow.Height ();

}

// タイマー処理時
void CMainFrame::OnTimer (UINT nIDEvent) {
	// ツールバーのテキスト更新用タイマー(0x03)
	if (nIDEvent == 0x03) {
		CFrameWnd* pActiveFrame = this->GetActiveFrame ();
		//CMDIChildWnd* pActiveFrame = this->MDIGetActive ();
		if (pActiveFrame == NULL || pActiveFrame == this) {
		//if (pActiveFrame == NULL) { // || pActiveFrame == this) {
			/* 現在の時：分：秒：ミリ秒表示 */
			SetWindowTextWhenDifferent (&m_wndTimeEdit, _T(""));
			/* 現在の小節：拍：ティック値表示 */
			SetWindowTextWhenDifferent (&m_wndMillisecEdit, _T(""));
			/* スクロールポジションの更新 */
			m_wndPositionScroll.SetScrollPos (0, TRUE);
			/* 現在の拍子及び調性記号の表示 */
			SetWindowTextWhenDifferent (&m_wndMeasureEdit, _T(""));
			/* 現在のテンポ[BPM]の表示 */
			SetWindowTextWhenDifferent (&m_wndTempoEdit, _T(""));
		}
		else {
			CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pActiveFrame->GetActiveDocument ());
			if (pSekaijuDoc) {
				TCHAR szText[1024];
				if (pSekaijuDoc->m_pMIDIData && pSekaijuDoc->m_pMIDIClock) {
					pSekaijuDoc->m_theCriticalSection.Lock ();
					long lCurTime, lMeasure, lBeat, lTick, lFrameNumber;
					long lCurMillisec, lHour, lMinute, lSec, lMillisec;
					long lCurTempo;
					long nn, dd, cc, bb;
					long sf, mi;
					lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
					lCurMillisec = MIDIClock_GetMillisec (pSekaijuDoc->m_pMIDIClock);
					/* 現在の時：分：秒：ミリ秒表示 */
					lHour = (lCurMillisec / 3600000);
					lMinute = (lCurMillisec / 60000) % 60;
					lSec = (lCurMillisec / 1000) % 60;
					lMillisec = lCurMillisec % 1000;
					memset (szText, 0, sizeof (szText));
					_sntprintf (szText, 1023, _T("%02d:%02d:%02d:%03d"), lHour, lMinute, lSec, lMillisec);
					SetWindowTextWhenDifferent (&m_wndMillisecEdit, szText);
					/* 現在の小節：拍：ティック値表示 */
					MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
					long lTimeMode, lTimeResolution;
					MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);
					if (lTimeMode == MIDIDATA_TPQNBASE) {
						MIDIData_BreakTimeEx (pSekaijuDoc->m_pMIDIData, lCurTime, &lMeasure, &lBeat, &lTick,
							&nn, &dd, &cc, &bb);
						memset (szText, 0, sizeof (szText));
						_sntprintf (szText, 1023, _T("%05d:%02d:%03d"), lMeasure + 1, lBeat + 1, lTick);
						SetWindowTextWhenDifferent (&m_wndTimeEdit, szText);
					}
					else if (lTimeMode == MIDIDATA_SMPTE24BASE ||
						lTimeMode == MIDIDATA_SMPTE25BASE ||
						lTimeMode == MIDIDATA_SMPTE29BASE ||
						lTimeMode == MIDIDATA_SMPTE30BASE) {
						lFrameNumber = lCurTime / lTimeResolution;
						lTick = lCurTime % lTimeResolution;
						memset (szText, 0, sizeof (szText));
						_sntprintf (szText, 1023, _T("%08d:%03d"), lFrameNumber, lTick);
						SetWindowTextWhenDifferent (&m_wndTimeEdit, szText);
					}
					/* スクロールポジションの更新 */
					m_wndPositionScroll.SetScrollPos (lCurTime, TRUE);
					/* 現在の拍子記号及び調性記号の表示 */
					MIDIData_FindTimeSignature (pSekaijuDoc->m_pMIDIData, lCurTime, &nn, &dd, &cc, &bb);
					MIDIData_FindKeySignature (pSekaijuDoc->m_pMIDIData, lCurTime, &sf, &mi);
					memset (szText, 0, sizeof (szText));
					_sntprintf (szText, 1023, _T("%d/%d, %d%s"), nn, 1 << dd, abs(sf), 
						(sf > 0 ? _T("#") : (sf < 0 ? _T("b") : _T(" ")))); // 20100613修正
					SetWindowTextWhenDifferent (&m_wndMeasureEdit, szText);
					/* 現在のテンポ[BPM]の表示 */
					MIDIData_FindTempo (pSekaijuDoc->m_pMIDIData, lCurTime, &lCurTempo);
					memset (szText, 0, sizeof (szText));
					_sntprintf (szText, 1023, _T("%1.2lf"), (double)60000000 / (double)lCurTempo);
					SetWindowTextWhenDifferent (&m_wndTempoEdit, szText);
					pSekaijuDoc->m_theCriticalSection.Unlock ();
				}
				else {
					/* 現在の時：分：秒：ミリ秒表示 */
					memset (szText, 0, sizeof (szText));
					_sntprintf (szText, 1023, _T("%05d:%02d:%03d"), 1, 1, 0);
					SetWindowTextWhenDifferent (&m_wndTimeEdit, szText);
					/* 現在の小節：拍：ティック値表示 */
					memset (szText, 0, sizeof (szText));
					_sntprintf (szText, 1023, _T("%02d:%02d:%02d:%03d"), 0, 0, 0, 0);
					SetWindowTextWhenDifferent (&m_wndMillisecEdit, szText);
					/* スクロールポジションの更新 */
					m_wndPositionScroll.SetScrollPos (0, TRUE);
					/* 現在の拍子記号及び調性記号の表示 */
					memset (szText, 0, sizeof (szText));
					_sntprintf (szText, 1023, _T("%d/%d, %d%s"), 4, 4, 0, _T(" "));
					SetWindowTextWhenDifferent (&m_wndMeasureEdit, szText);
					/* 現在のテンポ[BPM]の表示 */
					memset (szText, 0, sizeof (szText));
					_sntprintf (szText, 1023, _T("%1.2lf"), (double)60000000 / (double)600000);
					SetWindowTextWhenDifferent (&m_wndTempoEdit, szText);
				}
			}
			else {
				/* 現在の時：分：秒：ミリ秒表示 */
				SetWindowTextWhenDifferent (&m_wndTimeEdit, _T(""));
				/* 現在の小節：拍：ティック値表示 */
				SetWindowTextWhenDifferent (&m_wndMillisecEdit, _T(""));
				/* スクロールポジションの更新 */
				m_wndPositionScroll.SetScrollPos (0, TRUE);
				/* 現在の拍子記号及び調性記号の表示 */
				SetWindowTextWhenDifferent (&m_wndMeasureEdit, _T(""));
				/* 現在のテンポ[BPM]の表示 */
				SetWindowTextWhenDifferent (&m_wndTempoEdit, _T(""));
			}
		}
	}
}

// ツールバー2上の位置スクロールバーの操作メッセージはここにはこない。
void CMainFrame::OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	// ツールバー上のスクロールメッセージは、CSekaijuToolBar::OnHScrollで処理せよ。
}

// マウスホイールが回された時
void CMainFrame::OnMouseWheel40 (UINT nFlags, CPoint point) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	long lDelta = (short)HIWORD (nFlags);
	long lFlags = LOWORD (nFlags);
	if (lFlags & MK_CONTROL) {
		if (lDelta > 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel == 0 ||
			lDelta < 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel != 0) {
			this->PostMessage (WM_COMMAND, ID_CONTROL_PREVMEASURE);
		}
		else {
			this->PostMessage (WM_COMMAND, ID_CONTROL_NEXTMEASURE);
		}
	}
}


// 『表示』-『ツールバー1』
void CMainFrame::OnViewToolbar1 () {
	ShowControlBar (&m_wndToolBar1, (m_wndToolBar1.GetStyle () & WS_VISIBLE) == 0, FALSE);
}

// 『表示』-『ツールバー1』
void CMainFrame::OnUpdateViewToolbar1UI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck ((m_wndToolBar1.GetStyle () & WS_VISIBLE) ? 1 : 0);
}

// 『表示』-『ツールバー2』
void CMainFrame::OnViewToolbar2 () {
	ShowControlBar (&m_wndToolBar2, (m_wndToolBar2.GetStyle () & WS_VISIBLE) == 0, FALSE);
}

// 『表示』-『ツールバー2』
void CMainFrame::OnUpdateViewToolbar2UI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck ((m_wndToolBar2.GetStyle () & WS_VISIBLE) ? 1 : 0);
}

// このウィンドウを最前面に出すことを要求
long CMainFrame::OnCommandWakeUp (WPARAM wParam, LPARAM lParam) {
	// メイン・ウィンドウが最小化されていれば元に戻す
	if (this->IsIconic ()) {
		::ShowWindowAsync (this->GetSafeHwnd (), SW_RESTORE);
	}
	// メイン・ウィンドウを最前面に表示する
	SetForegroundWindow ();
	return 1;
}

// 共有メモリが変更されたので読み取ることを要求
long CMainFrame::OnCommandReadShm (WPARAM wParam, LPARAM lParam) {
	CString strMsg;
	HANDLE hShare = CreateFileMapping
		(INVALID_HANDLE_VALUE, NULL, FILE_MAP_READ, 0, 1024, AfxGetAppName ());
	if (hShare == INVALID_HANDLE_VALUE) {
		// 共有メモリ(受信側)オープンエラー
		strMsg.LoadString (IDS_SHAREMEMORY_FOR_RECV_OPEN_ERROR);
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return 0;
	}
	TCHAR *pShareMem = (TCHAR*)MapViewOfFile (hShare, FILE_MAP_READ, 0, 0, 1024);
	if (pShareMem == NULL) {
		// 共有メモリ(受信側)マッピングエラー
		::CloseHandle (hShare);
		strMsg.LoadString (IDS_SHAREMEMORY_FOR_RECV_MAPPING_ERROR);
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return 0;
	}
	m_strTempFileName = CString (pShareMem);
	::UnmapViewOfFile (pShareMem);
	::CloseHandle (hShare);
	return 1;
}

// ファイルを開くことを要求(開くファイル名はm_strTempFileName)
long CMainFrame::OnCommandFileOpen (WPARAM wParam, LPARAM lParam) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp();
	CDocTemplate* pSekaijuDocTemplate = pSekaijuApp->m_pSekaijuDocTemplate;
	CSekaijuDoc* pSekaijuDoc = 
		(CSekaijuDoc*)(pSekaijuDocTemplate->OpenDocumentFile (m_strTempFileName));
	if (pSekaijuDoc == NULL) {
		//MessageBox (m_strTempFileName, APPNAME, MB_ICONINFORMATION);
		_RPT1 (_CRT_WARN, "新しいドキュメントを開けませんでした。-%s.", m_strTempFileName);
	}

	return 1;
}