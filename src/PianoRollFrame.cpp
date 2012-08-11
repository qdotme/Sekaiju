//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ピアノロールフレームウィンドウクラス
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
#include "common.h"
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "resource.h"
#include "ColorfulComboBox.h"
#include "ColorfulCheckListBox.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDoc.h"
#include "SekaijuView.h"
#include "SekaijuToolBar.h"
#include "MainFrame.h"
#include "ChildFrame.h"
#include "PianoRollFrame.h"
#include "PianoRollPrintView.h"
#include "PianoRollScaleView.h"
#include "PianoRollTimeScaleView.h"
#include "PianoRollKeyScaleView.h"
#include "PianoRollKeyTimeView.h"
#include "PianoRollVelScaleView.h"
#include "PianoRollVelTimeView.h"
#include "PropertyNoteDlg.h"

// アロケーションの監視
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 子ウィンドウのサイズ
#define PIANOROLLFRAME_SCALEHEIGHT 32
#define PIANOROLLFRAME_SCALEWIDTH 64
#define PIANOROLLFRAME_TIMEWIDTH 600
#define PIANOROLLFRAME_KEYHEIGHT 256


#define PIANOROLLFRAME_VELHEIGHT 128
#define PIANOROLLFRAME_SCROLLBARHEIGHT 16
#define PIANOROLLFRAME_SCROLLBARWIDTH 16
#define PIANOROLLFRAME_BORDERWIDTH 2
#define PIANOROLLFRAME_BORDERHEIGHT 2
#define PIANOROLLFRAME_SPLITTERWIDTH 4
#define PIANOROLLFRAME_SPLITTERHEIGHT 4
#define PIANOROLLFRAME_TRACKLISTWIDTH 120

// 子ウィンドウIDを定義
#define PIANOROLLFRAME_DUMMYVIEW      (AFX_IDW_PANE_FIRST + 0)
#define PIANOROLLFRAME_PRINTVIEW      (AFX_IDW_PANE_FIRST + 31)
#define PIANOROLLFRAME_SCALEVIEW      (AFX_IDW_PANE_FIRST + 32)
#define PIANOROLLFRAME_TIMESCALEVIEW  (AFX_IDW_PANE_FIRST + 33)
#define PIANOROLLFRAME_KEYSCALEVIEW   (AFX_IDW_PANE_FIRST + 34)
#define PIANOROLLFRAME_KEYTIMEVIEW    (AFX_IDW_PANE_FIRST + 35)
#define PIANOROLLFRAME_VELSCALEVIEW   (AFX_IDW_PANE_FIRST + 36)
#define PIANOROLLFRAME_VELTIMEVIEW    (AFX_IDW_PANE_FIRST + 37)
#define PIANOROLLFRAME_TIMESCROLL     (AFX_IDW_PANE_FIRST + 48)
#define PIANOROLLFRAME_KEYSCROLL      (AFX_IDW_PANE_FIRST + 49)
#define PIANOROLLFRAME_VELSCROLL      (AFX_IDW_PANE_FIRST + 50)
#define PIANOROLLFRAME_SIZEBOX        (AFX_IDW_PANE_FIRST + 51)
#define PIANOROLLFRAME_TIMEZOOMDOWN   (AFX_IDW_PANE_FIRST + 52)
#define PIANOROLLFRAME_TIMEZOOMUP     (AFX_IDW_PANE_FIRST + 53)
#define PIANOROLLFRAME_KEYZOOMDOWN    (AFX_IDW_PANE_FIRST + 54)
#define PIANOROLLFRAME_KEYZOOMUP      (AFX_IDW_PANE_FIRST + 55)
#define PIANOROLLFRAME_VELZOOMDOWN    (AFX_IDW_PANE_FIRST + 56)
#define PIANOROLLFRAME_VELZOOMUP      (AFX_IDW_PANE_FIRST + 57)
#define PIANOROLLFRAME_TRACKLIST      (AFX_IDW_PANE_FIRST + 58)
#define PIANOROLLFRAME_GRAPHKINDLIST  (AFX_IDW_PANE_FIRST + 59)

//
// ピアノロールフレームのクライアント領域に配置されたオブジェクト
//	　　　　　　　　　　　　　　　　　　　　　　　　   　　 m_lScr　┃    m_lTrack
//                  ┃←m_lScale→│←     m_lTime  　→│←ollBar→★←  ListWidth   →┃
//                      Width                Width     　　 Width   ┃
//        ─
//        ↑        ┏─────────────────────────────────┓
// m_lToolBar1Height┃CToolBar                                                          ┃
//	    　↓        ┃m_wndToolBar1                                                     ┃
//        ━        ┣━━━━━━┯━━━━━━━━━━━━━━┯━┳━━━━━━━┯━┫
//        ↑        ┃CView*      │CView*  　　                │↑┃CChcekListBox │↑┃
//  m_lScaleHeight  ┃m_pScaleView│m_pTimeScaleView      　　　├─┨m_wndTrackList├─┨
//        ↓        ┠──────┼──────────────┤　┃(With Scroll  │　┃
//        ─        ┃CView*　    │CView*    　　          　  │□★ Bar)         │□┃
//        ↑        ┃m_pKeyScale │m_pKeyTimeView     　　     │　┃              │　┃
//                  ┃View        │  　　　　　　　　　　　　　├─┨              │　┃
//   m_lKeyHeight   ┃　　　      │   CScrollBar m_wndKeyScroll│↓┃              │　┃
//                  ┃　　　      │　　　　　　　　　　　　　  ├─┨              ├─┨
//                  ┃　　　      │　  CButton m_wndKeyZoomDown│－┃              │↓┃
//                  ┃　　　      │　　　　　　　　　　　　　　├─┨─┬───┬─┼─┨
//        ↓        ┃　　　      │　 　 CButton m_wndKeyZoomUp│＋┃←│□　　│→│　┃
//      ━★━      ┣━━━━━━┿━━━━★━━━━━━━━━┿━╋━┷━★━┷━┿━┫								
//        ↑        ┃CView*　　　│CView*　　　　　　　　　　　│↑┃CCheckListBox │↑┃　
//                  ┃m_pVelScale │m_pVelTimeView              ├─┨m_wndGraph    ├─┨
//                  ┃View        │ 　　                       │□┃KindList      │□┃
//                  ┃            │  　　                      │　★(With Scroll  │　┃
//   m_lVelHeight   ┃            │  　　　　　　　　　　　　　├─┨ Bar)         │　┃
//                  ┃　　　      │　 CScrollBar m_wndVelScroll│↓┃              │　┃
//                  ┃　　　      │  　　　　　　　　　　　　  ├─┨              │　┃
//                  ┃　　　      │  　CButton m_wndVelZoomDown│－┃              │　┃
//        ↓        ┃　　　      │　　　　　　　　　　　　　　├─┨              ├─┨
//        ─　      ┃　　　      │　　　CButton m_wndVelZoomUp│＋┃              │↓┃
//        ↑        ┠─┬────┴────────┬─┬─┬─┼─┨─┬───┬─┼─┨
//m_lScrollBarHeight┃←│　　　□　　　　　　　　　│→│－│＋│⊿┃←│□　　│→│　┃　　
//        ↓        ┗━┷━━━━━━━━━━━━━┷━┷━┷━┷━┻━┷━━━┷━┷━┛
//        ━                    CScrollBar          CButton   CButton
//                              m_wndTimeScroll     m_wndTime m_wndTime
//                                                  ZoomDown  ZoomDown
//
// (あ)―――：単純な境界線(0px)。
// (い)━━━：太く立体的な境界線。BORDERWIDTH(2px)又はBORDERHEIGHT(2px)で示す幅を占領
// (う)━★━：スプリッター境界線。(い)*2+SPRITTERWIDTH(4px)又はSPRITTERHEIGHT(4px)で示す幅を占領。
//

// 関数マクロ
#define PIANOROLLFRAME_RANGE(A,B,C) ((A)>(B)?(A):((B)>(C)?(C):(B)))

// メッセージマップ
IMPLEMENT_DYNCREATE(CPianoRollFrame, CChildFrame)

BEGIN_MESSAGE_MAP(CPianoRollFrame, CChildFrame)
	ON_WM_CREATE ()
	ON_WM_DESTROY ()
	ON_WM_SIZE ()
	ON_WM_TIMER ()
	ON_WM_ERASEBKGND ()
	ON_WM_MDIACTIVATE ()
	ON_WM_PAINT ()	
	ON_WM_KEYDOWN ()
	ON_WM_LBUTTONDOWN ()	
	ON_WM_RBUTTONDOWN ()	
	ON_WM_LBUTTONUP ()	
	ON_WM_RBUTTONUP ()	
	ON_WM_MOUSEMOVE ()
	ON_WM_HSCROLL ()	
	ON_WM_VSCROLL ()
	ON_BN_CLICKED (PIANOROLLFRAME_TIMEZOOMDOWN, OnTimeZoomDown)
	ON_BN_CLICKED (PIANOROLLFRAME_TIMEZOOMUP, OnTimeZoomUp)
	ON_BN_CLICKED (PIANOROLLFRAME_KEYZOOMDOWN, OnKeyZoomDown)
	ON_BN_CLICKED (PIANOROLLFRAME_KEYZOOMUP, OnKeyZoomUp)
	ON_BN_CLICKED (PIANOROLLFRAME_VELZOOMDOWN, OnVelZoomDown)
	ON_BN_CLICKED (PIANOROLLFRAME_VELZOOMUP, OnVelZoomUp)
//	ON_WM_CHILDACTIVATE ()	
	ON_COMMAND (ID_PIANOROLL_PEN, OnPianoRollPen)
	ON_UPDATE_COMMAND_UI (ID_PIANOROLL_PEN, OnUpdatePianoRollPenUI)
	ON_COMMAND (ID_PIANOROLL_LINE, OnPianoRollLine)
	ON_UPDATE_COMMAND_UI (ID_PIANOROLL_LINE, OnUpdatePianoRollLineUI)
	ON_COMMAND (ID_PIANOROLL_ERASER, OnPianoRollEraser)
	ON_UPDATE_COMMAND_UI (ID_PIANOROLL_ERASER, OnUpdatePianoRollEraserUI)
	ON_COMMAND (ID_PIANOROLL_SELECT, OnPianoRollSelect)
	ON_UPDATE_COMMAND_UI (ID_PIANOROLL_SELECT, OnUpdatePianoRollSelectUI)
	ON_COMMAND (ID_PIANOROLL_SPEAKER, OnPianoRollSpeaker)
	ON_UPDATE_COMMAND_UI (ID_PIANOROLL_SPEAKER, OnUpdatePianoRollSpeakerUI)
	ON_COMMAND (ID_PIANOROLL_ONLYCURTRACK, OnPianoRollOnlyCurTrack)
	ON_UPDATE_COMMAND_UI (ID_PIANOROLL_ONLYCURTRACK, OnUpdatePianoRollOnlyCurTrackUI)
	ON_COMMAND (ID_PIANOROLL_SHOWALLTRACK, OnPianoRollShowAllTrack)
	ON_UPDATE_COMMAND_UI (ID_PIANOROLL_SHOWALLTRACK, OnUpdatePianoRollShowAllTrackUI)
	ON_COMMAND (ID_PIANOROLL_ONLYCURGRAPH, OnPianoRollOnlyCurGraph)
	ON_UPDATE_COMMAND_UI (ID_PIANOROLL_ONLYCURGRAPH, OnUpdatePianoRollOnlyCurGraphUI)
	ON_COMMAND (ID_PIANOROLL_SHOWALLGRAPH, OnPianoRollShowAllGraph)
	ON_UPDATE_COMMAND_UI (ID_PIANOROLL_SHOWALLGRAPH, OnUpdatePianoRollShowAllGraphUI)
	ON_COMMAND (ID_PIANOROLL_AUTOPAGEUPDATE, OnPianoRollAutoPageUpdate)
	ON_UPDATE_COMMAND_UI (ID_PIANOROLL_AUTOPAGEUPDATE, OnUpdatePianoRollAutoPageUpdateUI)

	ON_COMMAND (ID_POPUP_TRACKVISIBLEON, OnPopupTrackVisibleOn)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKVISIBLEON, OnUpdatePopupTrackVisibleOnUI) // 20100429追加
	ON_COMMAND (ID_POPUP_TRACKVISIBLEOFF, OnPopupTrackVisibleOff)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKVISIBLEOFF, OnUpdatePopupTrackVisibleOffUI) // 20100429追加
	ON_COMMAND (ID_POPUP_TRACKVISIBLEALL, OnPopupTrackVisibleAll)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKVISIBLEALL, OnUpdatePopupTrackVisibleAllUI) // 20100429追加
	ON_COMMAND (ID_POPUP_EVENTPROPERTY, OnPopupEventProperty) // 20100501追加
	ON_UPDATE_COMMAND_UI (ID_POPUP_EVENTPROPERTY, OnUpdatePopupEventPropertyUI) // 20100501追加


	ON_CBN_SELENDOK (IDC_TRACKCOMBO, OnTrackComboSelEndOK)	
	ON_CLBN_CHKCHANGE (PIANOROLLFRAME_TRACKLIST, OnTrackListChkChange)
	ON_LBN_SELCHANGE (PIANOROLLFRAME_TRACKLIST, OnTrackListSelChange)

	ON_CBN_SELENDOK (IDC_GRAPHKINDCOMBO, OnGraphKindComboSelEndOK)	
	ON_CLBN_CHKCHANGE (PIANOROLLFRAME_GRAPHKINDLIST, OnGraphKindListChkChange)
	ON_LBN_SELCHANGE (PIANOROLLFRAME_GRAPHKINDLIST, OnGraphKindListSelChange)
	
END_MESSAGE_MAP()

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CPianoRollFrame::CPianoRollFrame () {
	long i;
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	m_lKeyHeight = PIANOROLLFRAME_KEYHEIGHT;
	m_lScaleHeight = PIANOROLLFRAME_SCALEHEIGHT;
	m_lVelHeight = PIANOROLLFRAME_VELHEIGHT + 16;
	m_lScaleWidth = PIANOROLLFRAME_SCALEWIDTH;
	m_lTimeWidth = PIANOROLLFRAME_TIMEWIDTH;
	//m_lSplitterHeight = PIANOROLLFRAME_SPLITTERHEIGHT;
	//m_lSplitterWidth = PIANOROLLFRAME_SPLITTERWIDTH;
	m_lHScrollBarHeight = ::GetSystemMetrics (SM_CYHSCROLL);
	m_lVScrollBarWidth = ::GetSystemMetrics (SM_CXVSCROLL);
	m_lTrackListWidth = PIANOROLLFRAME_TRACKLISTWIDTH;
	m_lVelZoom = pSekaijuApp->m_thePianoRollOption.m_lDefVelZoom;
	m_lKeyZoom = pSekaijuApp->m_thePianoRollOption.m_lDefKeyZoom;
	m_lTimeZoom = pSekaijuApp->m_thePianoRollOption.m_lDefTimeZoom;
	m_lTimeScrollPos = 0;
	CString strDefaultFontName;
	VERIFY (strDefaultFontName.LoadString (IDS_DEFAULTFONTNAME));
	m_theFont.CreateFont (12, 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,  
		strDefaultFontName);
	m_lCurTool = ID_PIANOROLL_PEN;      // デフォルトのツール=ペン
	m_bAutoPageUpdate = FALSE;          // 自動ページ更新=ON
	m_bOnlyCurTrack = FALSE;            // 現在のトラックのみ表示=OFF
	m_bShowAllTrack = FALSE;            // すべてのトラックを表示=OFF
	for (i = 0; i < MAXMIDITRACKNUM; i++) {
		m_bTrackVisible[i] = TRUE;      // すべてのトラックを可視にする。
	}
	m_bOnlyCurGraph = FALSE;            // 現在のトラックのグラフのみ表示=OFF
	m_bShowAllGraph = FALSE;            // すべてのトラックのグラフを表示=OFF
	for (i = 0; i < 256; i++) {
		m_bGraphVisible[i] = FALSE;     // 全グラフを不可視にする。
	}
	m_bGraphVisible[1] = TRUE;          // ベロシティのグラフだけ可視にする。
}

// デストラクタ
CPianoRollFrame::~CPianoRollFrame () {
	m_theFont.DeleteObject ();
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// ドキュメントの取得
CSekaijuDoc* CPianoRollFrame::GetDocument () {
	ASSERT (m_pDummyView);
	return (CSekaijuDoc*)m_pDummyView->GetDocument ();
}

// 時間方向のズーム倍率取得
long CPianoRollFrame::GetTimeZoom () {
	return m_lTimeZoom;
}

// キー方向のズーム倍率取得
long CPianoRollFrame::GetKeyZoom () {
	return m_lKeyZoom;
}

// ベロシティ方向のズーム倍率取得
long CPianoRollFrame::GetVelZoom () {
	return m_lVelZoom;
}

// y座標をノートキーに変換
long CPianoRollFrame::YtoKey (long y) {
	return 127 - (y / GetKeyZoom ());
}

// ノートキーをy座標に変換
long CPianoRollFrame::KeytoY (long lKey) {
	return (128 - lKey) * GetKeyZoom ();
}

// y座標をベロシティに変換
long CPianoRollFrame::YtoVel (long y) {
	return (128 * m_lVelZoom + 8 - y) / m_lVelZoom;
}

// y座標をピッチベンドに変換
long CPianoRollFrame::YtoPitchBend (long y) {
	return (128 * m_lVelZoom + 8 - y) * 128 / m_lVelZoom;
}

// y座標をテンポ[BPM]に変換
long CPianoRollFrame::YtoTempoBPM (long y) {
	return (128 * m_lVelZoom + 8 - y) * 2 / m_lVelZoom; 
}

// ベロシティをy座標に変換
long CPianoRollFrame::VeltoY (long lVel) {
	return (128 - lVel) * m_lVelZoom + 8;
}

// ピッチベンドをy座標に変換
long CPianoRollFrame::PitchBendtoY (long lPitchBend) {
	return (16384 - lPitchBend) * m_lVelZoom / 128 + 8;
}

// テンポ[BPM]をy座標に変換
long CPianoRollFrame::TempoBPMtoY (long lTempoBPM) {
	return (256 - lTempoBPM) * m_lVelZoom / 2 + 8;
}

// x座標からタイムを取得
long CPianoRollFrame::XtoTime (long x) {
	long lTimeResolution = MIDIData_GetTimeResolution (GetDocument ()->m_pMIDIData);
	return x * lTimeResolution / 4 / m_lTimeZoom;
	// 注：ズーム倍率1倍のとき4分音符の長さを4ピクセルと定義している。
}

// タイムをx座標に変換
long CPianoRollFrame::TimetoX (long lTime) {
	long lTimeResolution = MIDIData_GetTimeResolution (GetDocument ()->m_pMIDIData);
	return lTime * 4 * m_lTimeZoom / lTimeResolution;
	// 注：ズーム倍率1倍のとき4分音符の長さを4ピクセルと定義している。
}



// 時間方向のスクロールポジション取得
long CPianoRollFrame::GetTimeScrollPos () {
	return m_lTimeScrollPos;
}

// キー方向のスクロールポジション取得
long CPianoRollFrame::GetKeyScrollPos () {
	return m_lKeyScrollPos;
}

// ベロシティ方向のスクロールポジション取得
long  CPianoRollFrame::GetVelScrollPos () {
	return m_lVelScrollPos;
}

// 時間方向のスクロールポジション設定
long CPianoRollFrame::SetTimeScrollPos (long lTimeScrollPos) {
	long lOldTimeScrollPos = m_lTimeScrollPos;
	m_wndTimeScroll.SetScrollPos (lTimeScrollPos);
	m_lTimeScrollPos = m_wndTimeScroll.GetScrollPos ();
	long lDeltaTimeScrollPos = m_lTimeScrollPos - lOldTimeScrollPos;
	m_pTimeScaleView->ScrollWindow (-lDeltaTimeScrollPos, 0);
	m_pKeyTimeView->ScrollWindow (-lDeltaTimeScrollPos, 0);
	m_pVelTimeView->ScrollWindow (-lDeltaTimeScrollPos, 0);
	m_pTimeScaleView->UpdateWindow ();
	m_pKeyTimeView->UpdateWindow ();
	m_pVelTimeView->UpdateWindow ();
	return m_lTimeScrollPos;
}

// キー方向のスクロールポジション設定
long CPianoRollFrame::SetKeyScrollPos (long lKeyScrollPos) {
	long lOldKeyScrollPos = m_lKeyScrollPos;
	m_wndKeyScroll.SetScrollPos (lKeyScrollPos);
	m_lKeyScrollPos = m_wndKeyScroll.GetScrollPos ();
	long lDeltaKeyScrollPos = m_lKeyScrollPos - lOldKeyScrollPos;
	m_pKeyScaleView->ScrollWindow (0, -lDeltaKeyScrollPos);
	m_pKeyTimeView->ScrollWindow (0, -lDeltaKeyScrollPos);
	m_pKeyScaleView->UpdateWindow ();
	m_pKeyTimeView->UpdateWindow ();
	return m_lKeyScrollPos;
}

// ベロシティ方向のスクロールポジション設定
long CPianoRollFrame::SetVelScrollPos (long lVelScrollPos) {
	long lOldVelScrollPos = m_lVelScrollPos;
	m_wndVelScroll.SetScrollPos (lVelScrollPos);
	m_lVelScrollPos = m_wndVelScroll.GetScrollPos ();
	long lDeltaVelScrollPos = m_lVelScrollPos - lOldVelScrollPos;
	m_pVelScaleView->ScrollWindow (0, -lDeltaVelScrollPos);
	m_pVelTimeView->ScrollWindow (0, -lDeltaVelScrollPos);
	m_pVelScaleView->UpdateWindow ();
	m_pVelTimeView->UpdateWindow ();
	return m_lVelScrollPos;
}



// 表示されているタイムの左端を求める
long CPianoRollFrame::GetVisibleLeftTime () {
	long lTimeResolution = MIDIData_GetTimeResolution (GetDocument ()->m_pMIDIData);
	return m_lTimeScrollPos * lTimeResolution / 4 /  m_lTimeZoom;
	// 注：ズーム倍率1倍のとき4分音符の長さを4ピクセルと定義している。
}

// 表示されているタイムの右端を求める
long CPianoRollFrame::GetVisibleRightTime () {
	CRect rcClient;
	m_pTimeScaleView->GetClientRect (&rcClient);
	long lTimeResolution = MIDIData_GetTimeResolution (GetDocument ()->m_pMIDIData);
	return (m_lTimeScrollPos + rcClient.Width ()) * lTimeResolution / 4 / m_lTimeZoom;
	// 注：ズーム倍率1倍のとき4分音符の長さを4ピクセルと定義している。
}

// 表示されているキーの上限を計算
long CPianoRollFrame::GetVisibleTopKey () {
	return 127 - (m_lKeyScrollPos / m_lKeyZoom);
}

// 表示されているキーの下限を計算
long CPianoRollFrame::GetVisibleBottomKey () {
	CRect rcClient;
	m_pKeyScaleView->GetClientRect (&rcClient);
	return 127 - (m_lKeyScrollPos + rcClient.Height ()) / m_lKeyZoom;
}

// 表示されているベロシティの上限を計算
long CPianoRollFrame::GetVisibleTopVel() {
	return 127 - (m_lVelScrollPos / m_lVelZoom);
}

// 表示されているベロシティの下限を計算
long CPianoRollFrame::GetVisibleBottomVel () {
	CRect rcClient;
	m_pVelScaleView->GetClientRect (&rcClient);	
	return 127 - (m_lVelScrollPos + rcClient.Height ()) / m_lVelZoom;
}



// スプリッターキャプターの描画
void CPianoRollFrame::DrawSplitterCaptor (CDC* pDC, CPoint pt) {
	CRect rcClient;
	GetClientRect (&rcClient);
	CPen pen;
	CPen* pOldPen;
	pen.CreatePen (PS_SOLID, 4, ::GetSysColor(COLOR_BTNSHADOW));
	pDC->SetROP2 (R2_XORPEN);
	pOldPen = pDC->SelectObject (&pen);
	if (m_bSplitterMovingH) {
		pDC->MoveTo (0, pt.y);
		pDC->LineTo (rcClient.Width (), pt.y);
	}
	if (m_bSplitterMovingV) {
		pDC->MoveTo (pt.x, 0);
		pDC->LineTo (pt.x, rcClient.Height ());
	}
	pDC->SelectObject (pOldPen);
}




// 現在のトラックのインデックスを取得
long CPianoRollFrame::GetCurTrackIndex () {
	return m_wndTrackList.GetCurSel ();
}

// 現在のチャンネルを取得
long CPianoRollFrame::GetCurChannel () {
	return m_wndChannelCombo.GetCurSel ();
}

// 現在のスナップ[ティック]を取得
long CPianoRollFrame::GetCurSnap () {
	CString strText;
	m_wndSnapCombo.GetWindowText (strText);
	return _ttol (strText);
}

// 現在のベロシティを取得
long CPianoRollFrame::GetCurVelocity () {
	CString strText;
	m_wndVelocityCombo.GetWindowText (strText);
	return _ttol (strText);
}

// 現在の(音符の)長さ[ティック]を取得
long CPianoRollFrame::GetCurDuration () {
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	return _ttol (strText);
}

// 現在のグラフの種類を取得
long CPianoRollFrame::GetCurGraphKind () {
	return m_wndGraphKindList.GetCurSel ();
}

// 現在のグラフのスナップ[ティック]を取得
long CPianoRollFrame::GetCurGraphSnap () {
	CString strText;
	m_wndGraphSnapCombo.GetWindowText (strText);
	return _ttol (strText);
}

// 現在のトラックのインデックスを設定
BOOL CPianoRollFrame::SetCurTrackIndex (long lCurTrackIndex) {
	ASSERT (0 <= lCurTrackIndex && lCurTrackIndex < MAXMIDITRACKNUM);
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	m_wndTrackCombo.SetCurSel (lCurTrackIndex);
	m_wndTrackList.SetCurSel (lCurTrackIndex);
	//m_pCurTrack = pSekaijuDoc->GetTrack (lCurTrackIndex);
	if (m_bOnlyCurTrack) {
		long lCount = m_wndTrackList.GetCount ();
		for (long i = 0; i < lCount; i++) {
			m_wndTrackList.SetCheck (i, (i == lCurTrackIndex ? 1 : 0));
		}
		m_pKeyTimeView->Invalidate ();
		m_pVelTimeView->Invalidate ();
	}
	return TRUE;
}

// 現在のチャンネルを設定
BOOL CPianoRollFrame::SetCurChannel (long lCurChannel) {
	ASSERT (0 <= lCurChannel && lCurChannel < 16);
	m_wndChannelCombo.SetCurSel (lCurChannel);
	return TRUE;
}

// 現在のスナップ[ティック]を設定
BOOL CPianoRollFrame::SetCurSnap (long lCurSnap) {
	ASSERT (1 <= lCurSnap);
	CString strText;
	strText.Format (_T("%d"), lCurSnap);
	m_wndSnapCombo.SetWindowText (strText);
	return TRUE;
}

// 現在のベロシティを設定
BOOL CPianoRollFrame::SetCurVelocity (long lCurVelocity) {
	ASSERT (0 <= lCurVelocity && lCurVelocity <= 127);
	CString strText;
	strText.Format (_T("%d"), lCurVelocity);
	m_wndVelocityCombo.SetWindowText (strText);
	return TRUE;
}

// 現在の(音符の)長さ[ティック]を設定
BOOL CPianoRollFrame::SetCurDuration (long lCurDuration) {
	ASSERT (0 <= lCurDuration);
	CString strText;
	strText.Format (_T("%d"), lCurDuration);
	m_wndDurationCombo.SetWindowText (strText);
	return TRUE;
}

// 現在のグラフの種類を設定
BOOL CPianoRollFrame::SetCurGraphKind (long lCurGraphKind) {
	ASSERT (0 <= lCurGraphKind && lCurGraphKind < 256);
	m_wndGraphKindCombo.SetCurSel (lCurGraphKind);
	m_wndGraphKindList.SetCurSel (lCurGraphKind);
	if (m_bOnlyCurGraph) {
		long lCount = m_wndGraphKindList.GetCount ();
		for (long i = 0; i < lCount; i++) {
			m_wndGraphKindList.SetCheck (i, (i == lCurGraphKind ? 1 : 0));
		}
		m_pKeyTimeView->Invalidate ();
		m_pVelTimeView->Invalidate ();
		m_pVelScaleView->Invalidate ();
	}
	return TRUE;
}

// 現在のグラフスナップ[ティック]を設定
BOOL CPianoRollFrame::SetCurGraphSnap (long lCurSnap) {
	ASSERT (1 <= lCurSnap);
	CString strText;
	strText.Format (_T("%d"), lCurSnap);
	m_wndGraphSnapCombo.SetWindowText (strText);
	return TRUE;
}

// 指定インデックスのトラックが表示状態か調べる
BOOL CPianoRollFrame::IsTrackVisible (long lTrackIndex) {
	// (1)現在のトラックのみ表示がONのときは、現在のトラックのみがVisible、他はUnVisible。
	// (2)すべてのトラックを表示がONのときは、全てのトラックがVisible。
	// (3)その他の場合(通常時)は、m_bTrackVisible[MAXMIDITRACKNUM]の値に従う。
	ASSERT (0 <= lTrackIndex && lTrackIndex < MAXMIDITRACKNUM);
	if (m_bOnlyCurTrack == TRUE &&
		GetCurTrackIndex () == lTrackIndex ||
		m_bShowAllTrack == TRUE ||
		m_bShowAllTrack == FALSE &&
		m_bOnlyCurTrack == FALSE &&
		m_bTrackVisible[lTrackIndex] == TRUE) {
		return TRUE;
	}
	return FALSE;
}

// 指定インデックスのトラックを表示状態にする
BOOL CPianoRollFrame::SetTrackVisible (long lTrackIndex) {
	// (1)現在のトラックのみ表示がONのときは、現在のトラックを指定トラックに変更する
	// (2)すべてのトラックを表示がONのときは、何もしない。
	// (3)その他の場合(通常時)は、m_bTrackVisible[lTrackIndex]をチェック・可視化する。
	ASSERT (0 <= lTrackIndex && lTrackIndex < MAXMIDITRACKNUM);
	if (m_bOnlyCurTrack == TRUE) {
		m_wndTrackCombo.SetCurSel (lTrackIndex);
		m_wndTrackList.SetCurSel (lTrackIndex);
	}
	else if (m_bShowAllTrack == TRUE) {
		;
	}
	else {
		m_wndTrackList.SetCheck (lTrackIndex, TRUE);
		m_bTrackVisible[lTrackIndex] = TRUE;
	}
	return TRUE;
}

// 指定種類のグラフが表示状態か調べる
BOOL CPianoRollFrame::IsGraphVisible (long lGraphKind) {
	// (1)現在のグラフのみ表示がONのときは、現在のグラフのみがVisible、他はUnVisible。
	// (2)すべてのグラフを表示がONのときは、全てのグラフがVisible。
	// (3)その他の場合(通常時)は、m_bGraphVisible[MAXMIDITRACKNUM]の値に従う。
	ASSERT (0 <= lGraphKind && lGraphKind < 256);
	if (m_bOnlyCurGraph == TRUE &&
		GetCurGraphKind () == lGraphKind ||
		m_bShowAllGraph == TRUE ||
		m_bShowAllGraph == FALSE &&
		m_bOnlyCurGraph == FALSE &&
		m_bGraphVisible[lGraphKind] == TRUE) {
		return TRUE;
	}
	return FALSE;
}

// 指定種類のグラフを表示状態にする
BOOL CPianoRollFrame::SetGraphVisible (long lGraphKind) {
	// (1)現在のグラフのみ表示がONのときは、現在のトラックを指定トラックに変更する
	// (2)すべてのグラフを表示がONのときは、何もしない。
	// (3)その他の場合(通常時)は、m_bGraphKindVisible[lGraphKind]をチェック・可視化する。
	ASSERT (0 <= lGraphKind && lGraphKind < m_wndGraphKindList.GetCount ());
	if (m_bOnlyCurGraph == TRUE) {
		m_wndGraphKindCombo.SetCurSel (lGraphKind);
		m_wndGraphKindList.SetCurSel (lGraphKind);
	}
	else if (m_bShowAllGraph == TRUE) {
		;
	}
	else {
		m_wndGraphKindList.SetCheck (lGraphKind, TRUE);
		m_bGraphVisible[lGraphKind] = TRUE;
	}
	return TRUE;
}

// トラックコンボの更新
BOOL CPianoRollFrame::UpdateTrackCombo () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	//pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	BOOL bTrackZeroOrigin = pSekaijuApp->m_theGeneralOption.m_bTrackZeroOrigin;
	// 旧状態の保持
	static MIDITrack* pOldMIDITrack[MAXMIDITRACKNUM];
	BOOL bOldTrackVisible[MAXMIDITRACKNUM];
	memcpy (bOldTrackVisible, m_bTrackVisible, sizeof (BOOL) * MAXMIDITRACKNUM);
	long lOldCurSel = m_wndTrackCombo.GetCurSel ();
	long lOldCount = m_wndTrackCombo.GetCount ();
	MIDITrack* pOldCurTrack = NULL;
	if (0 <= lOldCurSel && lOldCurSel < __min (lOldCount, MAXMIDITRACKNUM)) {
		pOldCurTrack = pOldMIDITrack[lOldCurSel];
	}

	// コンボの初期化
	//m_wndTrackCombo.ResetContent ();
	m_wndTrackCombo.RemoveAllItem ();

	// コンボに項目を追加
	long i = 0;
	long j = 0;
	TCHAR szTrackName1[1024];
	TCHAR szTrackName2[1024];
	CString strText;
	forEachTrack (pMIDIData, pMIDITrack) {
		if (i >= MAXMIDITRACKNUM) {
			break;
		}
		memset (szTrackName1, 0, sizeof (szTrackName1));
		memset (szTrackName2, 0, sizeof (szTrackName2));
		MIDITrack_GetName (pMIDITrack, szTrackName1, sizeof (szTrackName1) - 1);
		codestr2str (szTrackName1, TCSLEN (szTrackName1), szTrackName2, TSIZEOF (szTrackName2) - 1);
		strText.Format (_T("%d-%s"), i + (bTrackZeroOrigin ? 0 : 1), szTrackName2);
		long lForeColor = MIDITrack_GetForeColor (pMIDITrack);
		long lBackColor = ::GetSysColor (COLOR_WINDOW);
		m_wndTrackCombo.AddItem (strText, lForeColor, lBackColor);
		
		// 現在のトラックである場合選択
		if (pMIDITrack == pOldCurTrack) {
			m_wndTrackCombo.SetCurSel (i);
		}
		i++;
	}

	// 現在選択しているものがない場合は強制選択
	long lNewCount = m_wndTrackCombo.GetCount ();
	long lNewCurSel = m_wndTrackCombo.GetCurSel ();
	if (m_wndTrackCombo.GetCurSel () == CB_ERR) {
		if (0 <= lOldCurSel && lOldCurSel < lNewCount) {
			m_wndTrackCombo.SetCurSel (lOldCurSel);
		}
		else if (lNewCount >= 2) {
			m_wndTrackCombo.SetCurSel (1);
		}
		else {
			m_wndTrackCombo.SetCurSel (0);
		}
	}

	// 次回のアップデート呼び出しに備えて現状を保持する。
	i = 0;
	memset (pOldMIDITrack, 0, sizeof (MIDITrack*) * MAXMIDITRACKNUM);
	forEachTrack (pMIDIData, pMIDITrack) {
		if (i >= MAXMIDITRACKNUM) {
			break;
		}
		pOldMIDITrack[i] = pMIDITrack;
		i++;
	}
	
	//pSekaijuDoc->m_theCriticalSection.Unlock ();
	return TRUE;
}

// グラフの種類コンボの更新
BOOL CPianoRollFrame::UpdateGraphKindCombo () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	CString strText;
	m_wndGraphKindCombo.ResetContent ();
	VERIFY (strText.LoadString (IDS_TEMPO));
	m_wndGraphKindCombo.AddString (strText);
	VERIFY (strText.LoadString (IDS_VELOCITY));
	m_wndGraphKindCombo.AddString (strText);
	VERIFY (strText.LoadString (IDS_CHANNELAFTERTOUCH));
	m_wndGraphKindCombo.AddString (strText);
	VERIFY (strText.LoadString (IDS_PITCHBEND));
	m_wndGraphKindCombo.AddString (strText);
	TCHAR szName[1024];
	long lTrackIndex = GetCurTrackIndex ();
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lTrackIndex);
	long lPortNumber = MIDITrack_GetOutputPort (pMIDITrack);
	MIDIInstrumentDefinition* pMIDIInstDefNorm = NULL;
	MIDIControllerNameTable* pMIDIControllerNameTable = NULL;
	if (0 <= lPortNumber && lPortNumber < MAXMIDIOUTDEVICENUM) {
		pMIDIInstDefNorm = ((CSekaijuApp*)AfxGetApp())->m_pMIDIInstDefNorm[lPortNumber];
		if (pMIDIInstDefNorm) {
			pMIDIControllerNameTable = MIDIInstrumentDefinition_GetControllerNameTable (pMIDIInstDefNorm);
		}
	}
	for (long i = 0; i < 128; i++) {
		memset (szName, 0, sizeof (szName));
		if (pMIDIControllerNameTable) {
			MIDIControllerNameTable_GetName (pMIDIControllerNameTable, i, szName, TSIZEOF (szName) - 1);
		}
		CString strFormat;
		VERIFY (strFormat.LoadString (IDS_CC_D_S));
		strText.Format (strFormat, i, szName);
		m_wndGraphKindCombo.AddString (strText);
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	return TRUE;
}

// トラックリストボックスの更新
BOOL CPianoRollFrame::UpdateTrackList () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	//pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	BOOL bTrackZeroOrigin = pSekaijuApp->m_theGeneralOption.m_bTrackZeroOrigin;
	// 旧状態の保持
	static MIDITrack* pOldMIDITrack[MAXMIDITRACKNUM];
	BOOL bOldTrackVisible[MAXMIDITRACKNUM];
	memcpy (bOldTrackVisible, m_bTrackVisible, sizeof (BOOL) * MAXMIDITRACKNUM);
	long lOldCurSel = m_wndTrackList.GetCurSel ();
	long lOldCount = m_wndTrackList.GetCount ();
	MIDITrack* pOldCurTrack = NULL;
	if (0 <= lOldCurSel && lOldCurSel < __min (lOldCount, MAXMIDITRACKNUM)) {
		pOldCurTrack = pOldMIDITrack[lOldCurSel];
	}

	// リストの初期化
	//m_wndTrackList.ResetContent ();
	m_wndTrackList.RemoveAllItem ();

	// リストの更新
	long i = 0;
	long j = 0;
	TCHAR szTrackName1[1024];
	TCHAR szTrackName2[1024];
	CString strText;
	forEachTrack (pMIDIData, pMIDITrack) {
		if (i >= MAXMIDITRACKNUM) {
			break;
		}
		memset (szTrackName1, 0, sizeof (szTrackName1));
		memset (szTrackName2, 0, sizeof (szTrackName2));
		MIDITrack_GetName (pMIDITrack, szTrackName1, sizeof (szTrackName1) - 1);
		codestr2str (szTrackName1, TCSLEN (szTrackName1), szTrackName2, TSIZEOF (szTrackName2) - 1);
		strText.Format (_T("%d-%s"), i + (bTrackZeroOrigin ? 0 : 1), szTrackName2);
		long lForeColor = MIDITrack_GetForeColor (pMIDITrack);
		long lBackColor = ::GetSysColor (COLOR_WINDOW);
		m_wndTrackList.AddItem (strText, lForeColor, lBackColor);
		// チェック状態変数の更新
		m_bTrackVisible[i] = 1;
		for (j = 0; j < MAXMIDITRACKNUM; j++) {
			if (pOldMIDITrack[j] == NULL) {
				break;
			}
			if (pOldMIDITrack[j] == pMIDITrack) {
				m_bTrackVisible[i] = bOldTrackVisible[j];
				break;
			}
		}
		// 現在のトラックである場合選択
		if (pMIDITrack == pOldCurTrack) {
			m_wndTrackList.SetCurSel (i);
		}
		i++;
	}

	// 現在選択しているものがない場合は強制選択
	long lNewCount = m_wndTrackList.GetCount ();
	long lNewCurSel = m_wndTrackList.GetCurSel ();
	if (m_wndTrackList.GetCurSel () == LB_ERR) {
		if (0 <= lOldCurSel && lOldCurSel < lNewCount) {
			m_wndTrackList.SetCurSel (lOldCurSel);
		}
		else if (lNewCount >= 2) {
			m_wndTrackList.SetCurSel (1);
		}
		else {
			m_wndTrackList.SetCurSel (0);
		}
		lNewCurSel = m_wndTrackList.GetCurSel ();
	}

	// チェックボックスの完全更新
	if (m_bShowAllTrack) {
		for (i = 0; i < lNewCount; i++) {
			m_wndTrackList.SetCheck (i, 1);
		}
	}
	else if (m_bOnlyCurTrack) {
		for (i = 0; i < lNewCount; i++) {
			m_wndTrackList.SetCheck (i, 0);
		}
		if (0 <= lNewCurSel && lNewCurSel < lNewCount) {
			m_wndTrackList.SetCheck (lNewCurSel, 1);
		}
	}
	else {
		for (i = 0; i < lNewCount; i++) {
			m_wndTrackList.SetCheck (i, m_bTrackVisible[i]);
		}
	}

	
	// 次回のアップデート呼び出しに備えて現状を保持する。
	i = 0;
	memset (pOldMIDITrack, 0, sizeof (MIDITrack*) * MAXMIDITRACKNUM);
	forEachTrack (pMIDIData, pMIDITrack) {
		if (i >= MAXMIDITRACKNUM) {
			break;
		}
		pOldMIDITrack[i] = pMIDITrack;
		i++;
	}

	//pSekaijuDoc->m_theCriticalSection.Unlock ();
	return TRUE;
}

// グラフの種類リストの更新
BOOL CPianoRollFrame::UpdateGraphKindList () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	CString strText;
	m_wndGraphKindList.ResetContent ();
	VERIFY (strText.LoadString (IDS_TEMPO));
	m_wndGraphKindList.AddString (strText);
	VERIFY (strText.LoadString (IDS_VELOCITY));
	m_wndGraphKindList.AddString (strText);
	VERIFY (strText.LoadString (IDS_CHANNELAFTERTOUCH));
	m_wndGraphKindList.AddString (strText);
	VERIFY (strText.LoadString (IDS_PITCHBEND));
	m_wndGraphKindList.AddString (strText);
	TCHAR szName[1024];
	long lTrackIndex = GetCurTrackIndex ();
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lTrackIndex);
	long lPortNumber = MIDITrack_GetOutputPort (pMIDITrack);
	MIDIInstrumentDefinition* pMIDIInstDefNorm = NULL;
	MIDIControllerNameTable* pMIDIControllerNameTable = NULL;
	if (0 <= lPortNumber && lPortNumber < MAXMIDIOUTDEVICENUM) {
		pMIDIInstDefNorm = ((CSekaijuApp*)AfxGetApp())->m_pMIDIInstDefNorm[lPortNumber];
		if (pMIDIInstDefNorm) {
			pMIDIControllerNameTable = MIDIInstrumentDefinition_GetControllerNameTable (pMIDIInstDefNorm);
		}
	}
	for (long i = 0; i < 128; i++) {
		memset (szName, 0, sizeof (szName));
		if (pMIDIControllerNameTable) {
			MIDIControllerNameTable_GetName (pMIDIControllerNameTable, i, szName, TSIZEOF (szName) - 1);
		}
		CString strFormat;
		VERIFY (strFormat.LoadString (IDS_CC_D_S));
		strText.Format (strFormat, i, szName);
		m_wndGraphKindList.AddString (strText);
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	return TRUE;
}

// スナップコンボボックスの更新
BOOL CPianoRollFrame::UpdateSnapCombo () {
	CString strText;
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)GetDocument ();
	long lTimeResolution = MIDIData_GetTimeResolution (pSekaijuDoc->m_pMIDIData);
	long lCurSel = m_wndSnapCombo.GetCurSel ();
	m_wndSnapCombo.ResetContent ();
	// スナップコンボボックスの充満
	CString strFormat;
	VERIFY (strFormat.LoadString (IDS_D_4DIVNOTE)); // %d-4分音符
	strText.Format (strFormat, lTimeResolution * 1);
	m_wndSnapCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_8DIVNOTE)); // %d-8分音符
	strText.Format (strFormat, lTimeResolution / 2);
	m_wndSnapCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_12DIVNOTE)); // %d-3連8分音符
	strText.Format (strFormat, lTimeResolution / 3);
	m_wndSnapCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_16DIVNOTE)); // %d-16分音符
	strText.Format (strFormat, lTimeResolution / 4);
	m_wndSnapCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_24DIVNOTE)); // %d-3連16分音符
	strText.Format (strFormat, lTimeResolution / 6);
	m_wndSnapCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_32DIVNOTE)); // %d-32分音符
	strText.Format (strFormat, lTimeResolution / 8);
	m_wndSnapCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_48DIVNOTE)); // %d-3連32分音符
	strText.Format (strFormat, lTimeResolution / 12);
	m_wndSnapCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_FREE)); // %d-自由
	strText.Format (strFormat, 1);
	m_wndSnapCombo.AddString (strText);
	// カレントセル設定
	if (lCurSel >= 0) {
		m_wndSnapCombo.SetCurSel (lCurSel);
	}
	return TRUE;
}

// ベロシティコンボボックスの更新
BOOL CPianoRollFrame::UpdateVelocityCombo () {
	long i;
	CString strText;
	long lCurSel = m_wndVelocityCombo.GetCurSel ();
	m_wndVelocityCombo.ResetContent ();
	// ベロシティコンボボックスの充満
	for (i = 127; i >= 1; i--) {
		if (i == 127 || (i % 5) == 0) {
			strText.Format (_T("%d"), i);
			m_wndVelocityCombo.AddString (strText);
		}
	}
	// カレントセル設定
	if (lCurSel >= 0) {
		m_wndVelocityCombo.SetCurSel (lCurSel);
	}
	return TRUE;
}

// 長さコンボボックスの更新
BOOL CPianoRollFrame::UpdateDurationCombo () {
	CString strText;
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)GetDocument ();
	long lTimeResolution = MIDIData_GetTimeResolution (pSekaijuDoc->m_pMIDIData);
	//long lCurSel = m_wndDurationCombo.GetCurSel (); // 20100328廃止
	CString strCurText;
	m_wndDurationCombo.GetWindowText (strCurText); // 20100328追加
	long i = 0;
	long lCurSel = -1; // 20100328追加
	for (i = 0; i < m_wndDurationCombo.GetCount (); i++) { // 20100328追加
		CString strLBText; // 20100328追加
		m_wndDurationCombo.GetLBText (i, strLBText); // 20100328追加
		if (strLBText == strCurText) { // 20100328追加
			lCurSel = i; // 20100328追加
			break; // 20100328追加
		} // 20100328追加
	} // 20100328追加
	m_wndDurationCombo.ResetContent ();
	// 長さコンボボックスの充満
	strText.Format (_T("%d"), lTimeResolution * 4);
	m_wndDurationCombo.AddString (strText);
	strText.Format (_T("%d"), lTimeResolution * 3);
	m_wndDurationCombo.AddString (strText);
	strText.Format (_T("%d"), lTimeResolution * 2);
	m_wndDurationCombo.AddString (strText);
	strText.Format (_T("%d"), lTimeResolution * 1);
	m_wndDurationCombo.AddString (strText);
	strText.Format (_T("%d"), lTimeResolution / 2);
	m_wndDurationCombo.AddString (strText);
	strText.Format (_T("%d"), lTimeResolution / 3);
	m_wndDurationCombo.AddString (strText);
	strText.Format (_T("%d"), lTimeResolution / 4);
	m_wndDurationCombo.AddString (strText);
	strText.Format (_T("%d"), lTimeResolution / 6);
	m_wndDurationCombo.AddString (strText);
	strText.Format (_T("%d"), lTimeResolution / 8);
	m_wndDurationCombo.AddString (strText);
	strText.Format (_T("%d"), lTimeResolution / 12);
	m_wndDurationCombo.AddString (strText);
	// カレントセルの設定
	if (lCurSel >= 0) {
		m_wndDurationCombo.SetCurSel (lCurSel);
	}
	else { // 20100328追加
		m_wndDurationCombo.SetWindowText (strCurText); // 20100328追加
	}
	return TRUE;
}

// グラフスナップコンボボックスの更新
BOOL CPianoRollFrame::UpdateGraphSnapCombo () {
	CString strText;
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)GetDocument ();
	long lTimeResolution = MIDIData_GetTimeResolution (pSekaijuDoc->m_pMIDIData);
	long lCurSel = m_wndGraphSnapCombo.GetCurSel ();
	m_wndGraphSnapCombo.ResetContent ();
	// スナップコンボボックスの充満
	CString strFormat;
	VERIFY (strFormat.LoadString (IDS_D_4DIVNOTE)); // %d-4分音符
	strText.Format (strFormat, lTimeResolution * 1);
	m_wndGraphSnapCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_8DIVNOTE)); // %d-8分音符
	strText.Format (strFormat, lTimeResolution / 2);
	m_wndGraphSnapCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_12DIVNOTE)); // %d-3連8分音符
	strText.Format (strFormat, lTimeResolution / 3);
	m_wndGraphSnapCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_16DIVNOTE)); // %d-16分音符
	strText.Format (strFormat, lTimeResolution / 4);
	m_wndGraphSnapCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_24DIVNOTE)); // %d-3連16分音符
	strText.Format (strFormat, lTimeResolution / 6);
	m_wndGraphSnapCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_32DIVNOTE)); // %d-32分音符
	strText.Format (strFormat, lTimeResolution / 8);
	m_wndGraphSnapCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_48DIVNOTE)); // %d-3連32分音符
	strText.Format (strFormat, lTimeResolution / 12);
	m_wndGraphSnapCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_FREE)); // %d-自由
	strText.Format (strFormat, 1);
	m_wndGraphSnapCombo.AddString (strText);
	// カレントセル設定
	if (lCurSel >= 0) {
		m_wndGraphSnapCombo.SetCurSel (lCurSel);
	}
	return TRUE;
}


// キースクロールバー(縦)のデザイン設定
void CPianoRollFrame::RecalcKeyScrollInfo () {
	SCROLLINFO si;
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	si.nMax = 128 * m_lKeyZoom;
	si.nPage = m_lKeyHeight;
	m_wndKeyScroll.SetScrollInfo (&si, TRUE);
	m_lKeyScrollPos = m_wndKeyScroll.GetScrollPos ();
}

// ヴェロシティスクロールバー(縦)のデザイン設定
void CPianoRollFrame::RecalcVelScrollInfo () {
	SCROLLINFO si;
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	si.nMax = PIANOROLLFRAME_VELHEIGHT * m_lVelZoom + 16;
	si.nPage = m_lVelHeight;
	m_wndVelScroll.SetScrollInfo (&si, TRUE);
	m_lVelScrollPos = m_wndVelScroll.GetScrollPos ();
}

// タイムスクロールバー(横)のデザイン設定
void CPianoRollFrame::RecalcTimeScrollInfo () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTimeResolution = 120;
	long lEndTime = 0;
	if (pMIDIData) {
		lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
		lEndTime = MIDIData_GetEndTime (pMIDIData);
	}
	long lFeedTime = lTimeResolution * 4 * 120;
	SCROLLINFO si;
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	si.nMax = (lEndTime + lFeedTime) * m_lTimeZoom * 4 / lTimeResolution;
	si.nPage = m_lTimeWidth;
	m_wndTimeScroll.SetScrollInfo (&si, TRUE);
	m_lTimeScrollPos = m_wndTimeScroll.GetScrollPos ();
	// 注：ズーム倍率1倍のとき4分音符の長さを4ピクセルと定義している。
}


//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// ウィンドウ生成直前の構造体設定
BOOL CPianoRollFrame::PreCreateWindow (CREATESTRUCT& cs) {
	return  (CWnd::PreCreateWindow(cs));
}

// ウィンドウタイトルの自動設定(CMDIChildWnd::OnUpdateFrameTitleのオーバーライド)
void CPianoRollFrame::OnUpdateFrameTitle (BOOL bAddToTitle) {
	// update our parent window first
	GetMDIFrame()->OnUpdateFrameTitle (bAddToTitle);
	if ((GetStyle() & FWS_ADDTOTITLE) == 0) {
		return;     // leave child window alone!
	}
	CDocument* pDocument = GetActiveDocument();
	if (bAddToTitle && pDocument != NULL) {
		CString strPianoRoll;
		strPianoRoll.LoadString (IDS_PIANOROLL);
		CString strTitle;
		if (m_nWindow > 0) {
			strTitle.Format (_T("%s:%d(%s)"), pDocument->GetTitle (), m_nWindow, strPianoRoll);
		}
		else {
			strTitle.Format (_T("%s(%s)"), pDocument->GetTitle (), strPianoRoll);
		}
		this->SetWindowText (strTitle);
	}
}

// 再配置用関数(CFrameWnd::RecalcLayoutのオーバーライド)
void CPianoRollFrame::RecalcLayout (BOOL bNotify) {

	CRect rcClient;
	GetClientRect (&rcClient);

	// 基本クラスの関数を処理
	CFrameWnd::RecalcLayout (bNotify);

	// アイコン化時には各コントロールのサイズを再計算しない。
	if (rcClient.Width () == 0 || rcClient.Height () == 0) {
		return;
	}

	// ツールバー1の高さ取得
	CRect rcToolBar1;
	m_wndToolBar1.GetWindowRect (&rcToolBar1); 
	m_lToolBar1Height = rcToolBar1.Height ();

	// 高さ方向の位置計算
	if (rcClient.Height () - m_lToolBar1Height < 
		PIANOROLLFRAME_SCALEHEIGHT + m_lHScrollBarHeight +
		PIANOROLLFRAME_BORDERHEIGHT * 4 + PIANOROLLFRAME_SPLITTERHEIGHT) {
		m_lScaleHeight = rcClient.Height () - m_lHScrollBarHeight -
			PIANOROLLFRAME_BORDERHEIGHT * 4 - PIANOROLLFRAME_SPLITTERHEIGHT;
		m_lKeyHeight = 0;
		m_lVelHeight = 0;
	}
	else if (rcClient.Height () - m_lToolBar1Height < m_lVelHeight + 
		PIANOROLLFRAME_SCALEHEIGHT + m_lHScrollBarHeight +
		PIANOROLLFRAME_BORDERHEIGHT * 4 + PIANOROLLFRAME_SPLITTERHEIGHT) {
		m_lScaleHeight = PIANOROLLFRAME_SCALEHEIGHT;
		m_lKeyHeight = 0;
		m_lVelHeight = rcClient.Height () - m_lToolBar1Height -
			m_lScaleHeight - m_lHScrollBarHeight -
			PIANOROLLFRAME_BORDERHEIGHT * 4 - PIANOROLLFRAME_SPLITTERHEIGHT;
	}
	else {
		m_lScaleHeight = PIANOROLLFRAME_SCALEHEIGHT;
		m_lKeyHeight = rcClient.Height () -  m_lToolBar1Height -
			m_lScaleHeight - m_lVelHeight - m_lHScrollBarHeight -
			PIANOROLLFRAME_BORDERHEIGHT * 4 - PIANOROLLFRAME_SPLITTERHEIGHT;
		m_lVelHeight = m_lVelHeight;
	}
	//m_lVelHeight = __min (m_lVelHeight, 128);

	// 幅方向の位置計算
	if (rcClient.Width () <
		PIANOROLLFRAME_SCALEWIDTH + m_lVScrollBarWidth + 
		PIANOROLLFRAME_BORDERWIDTH * 4) {
		m_lScaleWidth = rcClient.Width () -
			m_lVScrollBarWidth - PIANOROLLFRAME_BORDERWIDTH * 4;
		m_lTimeWidth = 0;
		m_lTrackListWidth = 0;
			
	}
	
	else if (rcClient.Width () <
		PIANOROLLFRAME_SCALEWIDTH + m_lVScrollBarWidth + 
		PIANOROLLFRAME_BORDERWIDTH * 4 + PIANOROLLFRAME_SPLITTERWIDTH + m_lTrackListWidth) {
		//m_lScaleWidth = rcClient.Width () - 
		//	m_lVScrollBarWidth - PIANOROLLFRAME_BORDERWIDTH * 2;
		m_lScaleWidth = PIANOROLLFRAME_SCALEWIDTH;
		m_lTimeWidth = 0;
		m_lTrackListWidth = rcClient.Width () - m_lScaleWidth -
			m_lVScrollBarWidth - PIANOROLLFRAME_BORDERWIDTH * 4 -
			PIANOROLLFRAME_SPLITTERWIDTH;

	}
	else {
		m_lScaleWidth = PIANOROLLFRAME_SCALEWIDTH;
		m_lTimeWidth = rcClient.Width () - m_lScaleWidth -
			m_lVScrollBarWidth - PIANOROLLFRAME_BORDERWIDTH * 4 -
			PIANOROLLFRAME_SPLITTERWIDTH - m_lTrackListWidth;
		m_lTrackListWidth = m_lTrackListWidth;
	}

	// ビューの整列
	if (m_pScaleView) {
		m_pScaleView->MoveWindow (PIANOROLLFRAME_BORDERWIDTH, 
			m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT, 
			m_lScaleWidth, m_lScaleHeight);
	}

	if (m_pTimeScaleView) {
		m_pTimeScaleView->MoveWindow (PIANOROLLFRAME_BORDERWIDTH + m_lScaleWidth, 
			m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT, 
			m_lTimeWidth, m_lScaleHeight);
	}

	if (m_pKeyScaleView) {
		m_pKeyScaleView->MoveWindow (PIANOROLLFRAME_BORDERWIDTH, 
			m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT + m_lScaleHeight, 
			m_lScaleWidth, m_lKeyHeight);
	}

	if (m_pKeyTimeView) {
		m_pKeyTimeView->MoveWindow (PIANOROLLFRAME_BORDERWIDTH + m_lScaleWidth,
			m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT + m_lScaleHeight, 
			m_lTimeWidth, m_lKeyHeight);
	}

	if (m_pVelScaleView) {
		m_pVelScaleView->MoveWindow (PIANOROLLFRAME_BORDERWIDTH, 
			m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT * 3 + m_lScaleHeight + m_lKeyHeight +
			PIANOROLLFRAME_SPLITTERHEIGHT, m_lScaleWidth, m_lVelHeight);
	}

	if (m_pVelTimeView) {
		m_pVelTimeView->MoveWindow (PIANOROLLFRAME_BORDERWIDTH + m_lScaleWidth,
			m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT * 3 + m_lScaleHeight + m_lKeyHeight + 
			PIANOROLLFRAME_SPLITTERHEIGHT, m_lTimeWidth, m_lVelHeight);
	}
	// スクロールバーの整列
	m_wndTimeScroll.MoveWindow (PIANOROLLFRAME_BORDERWIDTH,
		m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT * 3 + m_lScaleHeight + m_lKeyHeight + 
		PIANOROLLFRAME_SPLITTERHEIGHT + m_lVelHeight,
		m_lScaleWidth + m_lTimeWidth - m_lVScrollBarWidth * 2, 
		m_lHScrollBarHeight);

	m_wndKeyScroll.MoveWindow (PIANOROLLFRAME_BORDERWIDTH + m_lScaleWidth + m_lTimeWidth, 
		m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT,
		m_lVScrollBarWidth, 
		m_lScaleHeight + m_lKeyHeight - m_lVScrollBarWidth * 2);

	m_wndVelScroll.MoveWindow (PIANOROLLFRAME_BORDERWIDTH + m_lScaleWidth + m_lTimeWidth, 
		m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT * 3 + m_lScaleHeight + m_lKeyHeight +
		PIANOROLLFRAME_SPLITTERHEIGHT, m_lVScrollBarWidth,
		m_lVelHeight - m_lVScrollBarWidth * 2);

	m_wndSizeScroll.MoveWindow (PIANOROLLFRAME_BORDERWIDTH + m_lScaleWidth + m_lTimeWidth, 
		m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT * 3 + m_lScaleHeight + m_lKeyHeight +
		PIANOROLLFRAME_SPLITTERHEIGHT + m_lVelHeight, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);

	// ズームボタンの整列
	m_wndTimeZoomDown.MoveWindow (PIANOROLLFRAME_BORDERWIDTH + m_lScaleWidth +
		m_lTimeWidth - m_lVScrollBarWidth * 2, 
		m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT * 3 + m_lScaleHeight + m_lKeyHeight +
		PIANOROLLFRAME_SPLITTERHEIGHT + m_lVelHeight, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);
	
	m_wndTimeZoomUp.MoveWindow (PIANOROLLFRAME_BORDERWIDTH + m_lScaleWidth +
		m_lTimeWidth - m_lVScrollBarWidth, 
		m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT * 3 + m_lScaleHeight + m_lKeyHeight +
		PIANOROLLFRAME_SPLITTERHEIGHT + m_lVelHeight, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);

	m_wndKeyZoomDown.MoveWindow
		(PIANOROLLFRAME_BORDERWIDTH + m_lScaleWidth + m_lTimeWidth, 
		m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT + m_lScaleHeight + m_lKeyHeight -
		m_lVScrollBarWidth * 2, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);

	m_wndKeyZoomUp.MoveWindow
		(PIANOROLLFRAME_BORDERWIDTH + m_lScaleWidth + m_lTimeWidth, 
		m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT + m_lScaleHeight + m_lKeyHeight -
		m_lVScrollBarWidth, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);
	
	m_wndVelZoomDown.MoveWindow
		(PIANOROLLFRAME_BORDERWIDTH + m_lScaleWidth + m_lTimeWidth, 
		m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT * 3 + m_lScaleHeight + m_lKeyHeight + 
		PIANOROLLFRAME_SPLITTERHEIGHT + m_lVelHeight - m_lVScrollBarWidth * 2, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);

	m_wndVelZoomUp.MoveWindow 
		(PIANOROLLFRAME_BORDERWIDTH + m_lScaleWidth + m_lTimeWidth, 
		m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT * 3 + m_lScaleHeight + m_lKeyHeight + 
		PIANOROLLFRAME_SPLITTERHEIGHT + m_lVelHeight - m_lVScrollBarWidth,
		m_lVScrollBarWidth, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);

	// リストの整列
	m_wndTrackList.MoveWindow
		(PIANOROLLFRAME_BORDERWIDTH * 3 + m_lScaleWidth + m_lTimeWidth + 
		m_lVScrollBarWidth + PIANOROLLFRAME_SPLITTERWIDTH, 
		m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT,
		m_lTrackListWidth,
		m_lScaleHeight + m_lKeyHeight);

	m_wndGraphKindList.MoveWindow
		(PIANOROLLFRAME_BORDERWIDTH * 3 + m_lScaleWidth + m_lTimeWidth + 
		m_lVScrollBarWidth + PIANOROLLFRAME_SPLITTERWIDTH, 
		m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT * 3 +
		m_lScaleHeight + m_lKeyHeight + PIANOROLLFRAME_SPLITTERHEIGHT,
		m_lTrackListWidth,
		m_lVelHeight + m_lHScrollBarHeight);


	// スクロールバーのサイズが変化したので、バーのデザインを再調整する。
	RecalcKeyScrollInfo ();
	RecalcVelScrollInfo ();
	RecalcTimeScrollInfo ();
}

// クライアント領域の生成(CFrameWnd::OnCreateClientのオーバーライド)
BOOL CPianoRollFrame::OnCreateClient (LPCREATESTRUCT lpcs, CCreateContext* pContext) {

	// サイズ調整用のダミービュー生成(Visible = FALSE)
	CWnd* pWnd = NULL;
	m_pDummyView = (CView*)CFrameWnd::CreateView (pContext, PIANOROLLFRAME_DUMMYVIEW);
	if (m_pDummyView == NULL) {
			return FALSE;
	}
	m_pDummyView->ShowWindow (SW_HIDE);

	// 印刷用のビュー生成(Visible = FALSE)
	pContext->m_pNewViewClass = RUNTIME_CLASS (CPianoRollPrintView);
	m_pPrintView = (CView*)CFrameWnd::CreateView (pContext, PIANOROLLFRAME_PRINTVIEW);
	if (m_pPrintView == NULL) {
			return FALSE;
	}
	m_pPrintView->ShowWindow (SW_HIDE);

	// ビュー1の生成
	pContext->m_pNewViewClass = RUNTIME_CLASS (CPianoRollScaleView);
	m_pScaleView = (CView*)CFrameWnd::CreateView (pContext, PIANOROLLFRAME_SCALEVIEW);
	if (m_pScaleView == NULL) {
			return FALSE;
	}
	m_pScaleView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);
	
	// ビュー2の生成
	pContext->m_pNewViewClass = RUNTIME_CLASS (CPianoRollTimeScaleView);
	m_pTimeScaleView = (CView*)CFrameWnd::CreateView (pContext, PIANOROLLFRAME_TIMESCALEVIEW);
	if (m_pTimeScaleView == NULL) {
			return FALSE;
	}
	m_pTimeScaleView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);
	
	// ビュー3の生成
	pContext->m_pNewViewClass = RUNTIME_CLASS (CPianoRollKeyScaleView);
	m_pKeyScaleView = (CView*)CFrameWnd::CreateView (pContext, PIANOROLLFRAME_KEYSCALEVIEW);
	if (m_pKeyScaleView == NULL) {
			return FALSE;
	}
	m_pKeyScaleView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);
	
	// ビュー4の生成
	pContext->m_pNewViewClass = RUNTIME_CLASS (CPianoRollKeyTimeView);
	m_pKeyTimeView = (CView*)CFrameWnd::CreateView (pContext, PIANOROLLFRAME_KEYTIMEVIEW);
	if (m_pKeyTimeView == NULL) {
			return FALSE;
	}
	m_pKeyTimeView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);
	
	// ビュー5の生成
	pContext->m_pNewViewClass = RUNTIME_CLASS (CPianoRollVelScaleView);
	m_pVelScaleView = (CView*)CFrameWnd::CreateView (pContext, PIANOROLLFRAME_VELSCALEVIEW);
	if (m_pVelScaleView == NULL) {
			return FALSE;
	}
	m_pVelScaleView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);
	
	// ビュー6の生成
	pContext->m_pNewViewClass = RUNTIME_CLASS (CPianoRollVelTimeView);
	m_pVelTimeView = (CView*)CFrameWnd::CreateView (pContext, PIANOROLLFRAME_VELTIMEVIEW);
	if (m_pVelTimeView == NULL) {
			return FALSE;
	}
	m_pVelTimeView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);

	// スクロールバーの生成
	m_wndTimeScroll.Create   
		(WS_CHILD|WS_VISIBLE|SBS_HORZ, CRect(0,0,0,0), this, PIANOROLLFRAME_TIMESCROLL);
	m_wndKeyScroll.Create    
		(WS_CHILD|WS_VISIBLE|SBS_VERT, CRect(0,0,0,0), this, PIANOROLLFRAME_KEYSCROLL);
	m_wndVelScroll.Create    
		(WS_CHILD|WS_VISIBLE|SBS_VERT, CRect(0,0,0,0), this, PIANOROLLFRAME_VELSCROLL);
	m_wndSizeScroll.Create   
		(WS_CHILD|WS_VISIBLE|SBS_SIZEBOX, CRect(0,0,0,0), this, PIANOROLLFRAME_SIZEBOX);
	
	// ズームボタン類の生成
	m_wndTimeZoomDown.Create 
		(_T("-"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, PIANOROLLFRAME_TIMEZOOMDOWN);
	m_wndTimeZoomUp.Create   
		(_T("+"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, PIANOROLLFRAME_TIMEZOOMUP);
	m_wndKeyZoomDown.Create  
		(_T("-"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, PIANOROLLFRAME_KEYZOOMDOWN);
	m_wndKeyZoomUp.Create    
		(_T("+"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, PIANOROLLFRAME_KEYZOOMUP);
	m_wndVelZoomDown.Create  
		(_T("-"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, PIANOROLLFRAME_VELZOOMDOWN);
	m_wndVelZoomUp.Create    
		(_T("+"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, PIANOROLLFRAME_VELZOOMUP);

	// トラックリストの作成
	m_wndTrackList.Create
		(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|
		LBS_NOTIFY|LBS_DISABLENOSCROLL|LBS_OWNERDRAWFIXED|LBS_HASSTRINGS|LBS_NOINTEGRALHEIGHT, 
		CRect(0,0,0,0), this, PIANOROLLFRAME_TRACKLIST);
	
	// コントローラの種類リストの作成
	m_wndGraphKindList.Create
		(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|
		LBS_NOTIFY|LBS_DISABLENOSCROLL|LBS_OWNERDRAWFIXED|LBS_HASSTRINGS|LBS_NOINTEGRALHEIGHT, 
		CRect(0,0,0,0), this, PIANOROLLFRAME_GRAPHKINDLIST);

	// コントロールの位置合わせはWM_SIZEなどによるRecalcLaoyoutに任せる。
	return TRUE;

}

// 印刷用のコマンドをトラップ(CFrameWnd::OnCmdMsgのオーバーライド)
BOOL CPianoRollFrame::OnCmdMsg (UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	// 印刷用のコマンドの場合、強制的にCPianoRollPrintViewに渡す。
	if ((nID == ID_FILE_PRINT || nID == ID_FILE_PRINT_DIRECT || nID == ID_FILE_PRINT_PREVIEW) &&
		pSekaijuApp->m_bRecording == FALSE) {
		if (m_pPrintView) {
			return ((CPianoRollPrintView*)m_pPrintView)->OnCmdMsg (nID, nCode, pExtra, pHandlerInfo);
		}
		return FALSE;
	}
	// その他のコマンドはデフォルトの処理とする。
	return CFrameWnd::OnCmdMsg (nID, nCode, pExtra, pHandlerInfo);
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// ウィンドウ生成時
int CPianoRollFrame::OnCreate (LPCREATESTRUCT lpCreateStruct) {

	CRect rcTemp;
	
	// ツールバー1の作成
	if (!m_wndToolBar1.Create (this) ||
		!m_wndToolBar1.LoadToolBar (IDR_PIANOROLL1)) {
		TRACE0 ("Failed to create toolbar\n");
		return -1;
	}
	m_wndToolBar1.SetBarStyle (m_wndToolBar1.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	//m_wndToolBar1.EnableDocking (CBRS_ALIGN_ANY);
	//EnableDocking (CBRS_ALIGN_ANY);
	//DockControlBar (&m_wndToolBar1);

	LoadAccelTable (MAKEINTRESOURCE (IDR_PIANOROLL));

	// トラックコンボの作成
	m_wndToolBar1.SetButtonInfo (6, IDC_TRACKCOMBO, TBBS_SEPARATOR, 80);
	m_wndToolBar1.GetItemRect (6, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 120;
	if (!m_wndTrackCombo.CreateEx (
		WS_EX_CLIENTEDGE, _T("COMBOBOX"), NULL,
		WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar1.GetSafeHwnd (), (HMENU)IDC_TRACKCOMBO)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndTrackCombo.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));
	
	// チャンネルコンボの作成
	m_wndToolBar1.SetButtonInfo (8, IDC_CHANNELCOMBO, TBBS_SEPARATOR, 40);
	m_wndToolBar1.GetItemRect (8, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 120;
	if (!m_wndChannelCombo.CreateEx (
		WS_EX_CLIENTEDGE, _T("COMBOBOX"), NULL,
		WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar1.GetSafeHwnd (), (HMENU)IDC_CHANNELCOMBO)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndChannelCombo.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));

	// スナップコンボの作成
	m_wndToolBar1.SetButtonInfo (10, IDC_SNAPCOMBO, TBBS_SEPARATOR, 100);
	m_wndToolBar1.GetItemRect (10, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 120;
	if (!m_wndSnapCombo.CreateEx (
		WS_EX_CLIENTEDGE, _T("COMBOBOX"), NULL,
		WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar1.GetSafeHwnd (), (HMENU)IDC_SNAPCOMBO)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndSnapCombo.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));

	// ベロシティコンボの作成
	m_wndToolBar1.SetButtonInfo (12, IDC_VELOCITYCOMBO, TBBS_SEPARATOR, 50);
	m_wndToolBar1.GetItemRect (12, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 120;
	if (!m_wndVelocityCombo.CreateEx (
		WS_EX_CLIENTEDGE, _T("COMBOBOX"), NULL,
		WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_VSCROLL | CBS_DROPDOWN,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar1.GetSafeHwnd (), (HMENU)IDC_VELOCITYCOMBO)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndVelocityCombo.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));


	// スピンの作成(テスト)
	//m_wndVelocitySpin.Create (
	//	WS_VISIBLE | WS_TABSTOP | WS_CHILD | UDS_ARROWKEYS | UDS_NOTHOUSANDS | UDS_AUTOBUDDY | UDS_ALIGNRIGHT,
	//	rcTemp, &m_wndToolBar1, IDC_VELOCITYSPIN);

	// 長さコンボの作成
	m_wndToolBar1.SetButtonInfo (14, IDC_DURATIONCOMBO, TBBS_SEPARATOR, 50);
	m_wndToolBar1.GetItemRect (14, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 120;
	if (!m_wndDurationCombo.CreateEx (
		WS_EX_CLIENTEDGE, _T("COMBOBOX"), NULL,
		WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_VSCROLL | CBS_DROPDOWN,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar1.GetSafeHwnd (), (HMENU)IDC_DURATIONCOMBO)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndDurationCombo.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));
	
	// グラフの種類コンボの作成
	m_wndToolBar1.SetButtonInfo (16, IDC_GRAPHKINDCOMBO, TBBS_SEPARATOR, 120);
	m_wndToolBar1.GetItemRect (16, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 120;
	if (!m_wndGraphKindCombo.CreateEx (
		WS_EX_CLIENTEDGE, _T("COMBOBOX"), NULL,
		WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar1.GetSafeHwnd (), (HMENU)IDC_GRAPHKINDCOMBO)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndGraphKindCombo.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));
	
	// グラフのスナップコンボの作成
	m_wndToolBar1.SetButtonInfo (18, IDC_GRAPHSNAPCOMBO, TBBS_SEPARATOR, 100);
	m_wndToolBar1.GetItemRect (18, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 120;
	if (!m_wndGraphSnapCombo.CreateEx (
		WS_EX_CLIENTEDGE, _T("COMBOBOX"), NULL,
		WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar1.GetSafeHwnd (), (HMENU)IDC_GRAPHSNAPCOMBO)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndGraphSnapCombo.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));
	
	// 親クラスの関数呼び出し
	int nRet = CChildFrame::OnCreate (lpCreateStruct);

	// スクロールポジションの位置あわせ
	SetKeyScrollPos (64 * m_lKeyZoom - m_lKeyHeight / 2); 

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;

	// トラックコンボボックスの充満
	UpdateTrackCombo ();

	// チャンネルコンボの充満
	int i;
	CString strText;
	for (i = 0; i < 16; i++) {
		strText.Format (_T("%d"), i + 1);
		m_wndChannelCombo.AddString (strText);
	}
	m_wndChannelCombo.SetCurSel (0);

	// カレントチャンネルの自動選択
	if (pSekaijuDoc->m_pTempTrack) {
		long lOutputChannel = MIDITrack_GetOutputChannel (pSekaijuDoc->m_pTempTrack);
		if (0 <= lOutputChannel && lOutputChannel <= 15) {
			SetCurChannel (lOutputChannel);
		}
	}
	
	// スナップコンボの充満
	UpdateSnapCombo ();
	m_wndSnapCombo.SetCurSel (3);

	// ベロシティコンボの充満
	UpdateVelocityCombo ();
	m_wndVelocityCombo.SetCurSel (6);

	// 長さコンボの充満
	UpdateDurationCombo ();
	m_wndDurationCombo.SetCurSel (3);

	// グラフスナップコンボの充満
	UpdateGraphSnapCombo ();
	m_wndGraphSnapCombo.SetCurSel (7);

	// トラックリストの充満
	UpdateTrackList ();
	m_wndTrackList.SetCurSel (0);
	m_wndTrackList.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));
	long lCount = m_wndTrackList.GetCount ();
	i = 0;
	if (pSekaijuDoc->m_pTempTrack) {
		forEachTrack (pMIDIData, pMIDITrack) {
			if (0 <= i && i < lCount) {
				m_wndTrackList.SetCheck (i, pMIDITrack == pSekaijuDoc->m_pTempTrack ? TRUE : FALSE);
				m_bTrackVisible[i] = (pMIDITrack == pSekaijuDoc->m_pTempTrack ? TRUE : FALSE);
			}
			i++;
		}
	}
	else {
		forEachTrack (pMIDIData, pMIDITrack) {
			if (0 <= i && i < lCount) {
				m_wndTrackList.SetCheck (i, TRUE);
				m_bTrackVisible[i] = (TRUE);
			}
			i++;
		}
	}
	m_bOnlyCurTrack = FALSE;
	m_bShowAllTrack = FALSE;

	// カレントトラックとカレントチャンネルを更新
	long lTrackIndex = 0;
	if (pSekaijuDoc->m_pTempTrack) {
		lTrackIndex = pSekaijuDoc->GetTrackIndex (pSekaijuDoc->m_pTempTrack);
	}
	else if (MIDIData_GetFormat (pMIDIData) == 1 && lCount >= 2) {
		lTrackIndex = 1;
	}
	ASSERT (0 <= lTrackIndex && lTrackIndex < lCount);
	if (m_bTrackVisible[lTrackIndex]) {
		SetCurTrackIndex (lTrackIndex);
		pMIDITrack = pSekaijuDoc->GetTrack (lTrackIndex);
		if (pMIDITrack) {
			long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
			if (0 <= lOutputChannel && lOutputChannel <= 15) {
				SetCurChannel (lOutputChannel);
			}
		}
	}


	// グラフの種類コンボの充満(先にトラックリストを完成させGetCurTrackIndexを効かせること)
	UpdateGraphKindCombo ();
	m_wndGraphKindCombo.SetCurSel (1);

	// グラフの種類リストの充満
	UpdateGraphKindList ();
	m_wndGraphKindList.SetCurSel (1);
	m_wndGraphKindList.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));
	lCount = m_wndGraphKindList.GetCount ();
	for (i = 0; i < lCount; i++) {
		m_wndGraphKindList.SetCheck (i, IsGraphVisible (i) ? 1 : 0);
	}

	// 自動ページ更新の設定
	if (pSekaijuApp->m_theGeneralOption.m_bEnableAutoPageUpdate) {
		m_bAutoPageUpdate = TRUE;
	}


	SetActiveView (m_pKeyTimeView, FALSE);
	m_pKeyTimeView->SetFocus ();

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	return nRet;
}

// ウィンドウ破棄時
void CPianoRollFrame::OnDestroy () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	//KillTimer (0x11);
	CChildFrame::OnDestroy ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// ウィンドウサイズ変更時
void CPianoRollFrame::OnSize (UINT nType, int cx, int cy) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	// 基本クラスの関数呼び出し
	CChildFrame::OnSize (nType, cx, cy);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// タイマー呼び出し時
void CPianoRollFrame::OnTimer (UINT nIDEvent) {
	//if (nIDEvent == 0x11) {
	//	m_pKeyTimeView->SendMessage (WM_TIMER, 11, NULL);
	//	m_pVelTimeView->SendMessage (WM_TIMER, 11, NULL);
	//}
}

// 背景消去(CFrameWnd::OnEraseBkgndのオーバーライド)
BOOL CPianoRollFrame::OnEraseBkgnd (CDC* pDC) {
	return 0;
}

// ウィンドウがアクティブになったとき
void CPianoRollFrame::OnMDIActivate (BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) {
	CChildFrame::OnMDIActivate (bActivate, pActivateWnd, pDeactivateWnd);
}

// 描画するとき
void CPianoRollFrame::OnPaint () {
	CPaintDC dc (this);
	CRect rcClient;
	GetClientRect (&rcClient);
	// 左上側領域のくぼみ描画
	CRect rcClient1 (rcClient);
	rcClient1.top = m_lToolBar1Height;
	rcClient1.bottom = m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT * 2 + 
		m_lScaleHeight + m_lKeyHeight;
	rcClient1.right = PIANOROLLFRAME_BORDERWIDTH * 2 + m_lScaleWidth + m_lTimeWidth + 
		m_lVScrollBarWidth;
	dc.Draw3dRect (&rcClient1, RGB (128, 128, 128), RGB (255, 255, 255));
	rcClient1.InflateRect (-1, -1);
	dc.Draw3dRect (&rcClient1, RGB (0, 0, 0), RGB (192, 192, 192));
	// 左下側領域のくぼみ描画
	CRect rcClient2 (rcClient);
	rcClient2.top = m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT * 2 + 
		m_lScaleHeight + m_lKeyHeight + PIANOROLLFRAME_SPLITTERHEIGHT;
	rcClient2.right = PIANOROLLFRAME_BORDERWIDTH * 2 + m_lScaleWidth + m_lTimeWidth + 
		m_lVScrollBarWidth;
	dc.Draw3dRect (&rcClient2, RGB (128, 128, 128), RGB (255, 255, 255));
	rcClient2.InflateRect (-1, -1);
	dc.Draw3dRect (&rcClient2, RGB (0, 0, 0), RGB (192, 192, 192));
	// 右上側領域のくぼみ描画
	CRect rcClient3 (rcClient);
	rcClient3.top = m_lToolBar1Height;
	rcClient3.bottom = m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT * 2 + 
		m_lScaleHeight + m_lKeyHeight;
	rcClient3.left = PIANOROLLFRAME_BORDERWIDTH * 2 + m_lScaleWidth + m_lTimeWidth + 
		m_lVScrollBarWidth + PIANOROLLFRAME_SPLITTERWIDTH;
	dc.Draw3dRect (&rcClient3, RGB (128, 128, 128), RGB (255, 255, 255));
	rcClient3.InflateRect (-1, -1);
	dc.Draw3dRect (&rcClient3, RGB (0, 0, 0), RGB (192, 192, 192));
	// 右下側領域のくぼみ描画
	CRect rcClient4 (rcClient);
	rcClient4.top =  m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT * 2 + 
		m_lScaleHeight + m_lKeyHeight + PIANOROLLFRAME_SPLITTERHEIGHT;
	rcClient4.left = PIANOROLLFRAME_BORDERWIDTH * 2 + m_lScaleWidth + m_lTimeWidth + 
		m_lVScrollBarWidth + PIANOROLLFRAME_SPLITTERWIDTH;
	dc.Draw3dRect (&rcClient4, RGB (128, 128, 128), RGB (255, 255, 255));
	rcClient4.InflateRect (-1, -1);
	dc.Draw3dRect (&rcClient4, RGB (0, 0, 0), RGB (192, 192, 192));
	// 横境界部分の描画
	CBrush brush;
	brush.CreateSolidBrush (::GetSysColor (COLOR_3DFACE));
	CRect rcClient5 (rcClient);
	rcClient5.top = rcClient1.bottom + 1;
	rcClient5.bottom = rcClient2.top - 1;
	dc.FillRect (&rcClient5, &brush);
	// 縦境界部分の描画
	CRect rcClient6 (rcClient);
	rcClient6.left = rcClient1.right + 1;
	rcClient6.right = rcClient3.left - 1;
	dc.FillRect (&rcClient6, &brush);
}

// キーが押された時
void CPianoRollFrame::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	switch (nChar) {
	// D(描画)P(ペン)
	case 'D':
	case 'P':
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_PIANOROLL_PEN, 0);
		}
		break;
	// L(線)
	case 'L':
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_PIANOROLL_LINE, 0);
		}
		break;
	// E(消しゴム)
	case 'E':
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_PIANOROLL_ERASER, 0);
		}
		break;
	// S(選択)
	case 'S':
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_PIANOROLL_SELECT, 0);
		}
		break;
	// B(スクラブ)
	case 'B':
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_PIANOROLL_SPEAKER, 0);
		}
		break;
	}
	return;
}



// マウス左ボタン押された時  
void CPianoRollFrame::OnLButtonDown (UINT nFlags, CPoint point) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	// 上半分(ピアノロール)と下半分(コントローラー)の境界線上をドラッグすると、境界線が上下に移動
	if (m_bSplitterMovingH || m_bSplitterMovingV) {
		SetCapture ();
		m_ptMouseDown = m_ptMouseMoveOld = point;
		CDC* pDC = GetDC ();
		DrawSplitterCaptor (pDC, point);
		ReleaseDC (pDC);
		if (m_bSplitterMovingH && m_bSplitterMovingV) {
			::SetCursor (pSekaijuApp->m_hCursorResizeAll);
		}
		else if (m_bSplitterMovingH) {
			::SetCursor (pSekaijuApp->m_hCursorResizeNS);
		}
		else if (m_bSplitterMovingV) {
			::SetCursor (pSekaijuApp->m_hCursorResizeWE);
		}
	}
}

// マウス右ボタン押された時  
void CPianoRollFrame::OnRButtonDown (UINT nFlags, CPoint point) {
}

// マウス左ボタン離されたとき
void CPianoRollFrame::OnLButtonUp (UINT nFlags, CPoint point) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (GetCapture () == this) {
		CSekaijuDoc* pSekaijuDoc = GetDocument ();
		pSekaijuDoc->m_theCriticalSection.Lock ();
		CDC* pDC = GetDC ();
		DrawSplitterCaptor (pDC, m_ptMouseMoveOld);
		ReleaseDC (pDC);
		ReleaseCapture ();
		::SetCursor (pSekaijuApp->m_hCursorArrow);
		CPoint ptDelta = point - m_ptMouseDown;
		if (m_bSplitterMovingH) {
			m_lVelHeight = CLIP (0, (m_lVelHeight - ptDelta.y), 
				PIANOROLLFRAME_VELHEIGHT * m_lVelZoom + 16);
		}
		if (m_bSplitterMovingV) {
			m_lTrackListWidth = CLIP (0,
				(m_lTrackListWidth - ptDelta.x), 1600);
		}
		m_bSplitterMovingH = FALSE;
		m_bSplitterMovingV = FALSE;
		RecalcLayout ();
		Invalidate ();
		pSekaijuDoc->m_theCriticalSection.Unlock ();
	}
}

// マウス右ボタン離されたとき
void CPianoRollFrame::OnRButtonUp (UINT nFlags, CPoint point) {


}

// マウスが動かされたとき
void CPianoRollFrame::OnMouseMove (UINT nFlags, CPoint point) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	// キャプター中
	if (GetCapture () == this) {
		CRect rcClient;
		GetClientRect (&rcClient);
		CDC* pDC = GetDC ();
		DrawSplitterCaptor (pDC, m_ptMouseMoveOld);
		DrawSplitterCaptor (pDC, point);
		ReleaseDC (pDC);
		m_ptMouseMoveOld = point;
	}
	// 非キャプター中
	else {
		// カーソルが水平スプリッター上にあるか
		m_bSplitterMovingH =
			m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT * 2 + m_lScaleHeight + m_lKeyHeight <= point.y &&
			point.y < m_lToolBar1Height + PIANOROLLFRAME_BORDERHEIGHT * 2 + m_lScaleHeight + m_lKeyHeight + 
			PIANOROLLFRAME_SPLITTERHEIGHT;
		// カーソルが垂直スプリッター上にあるか
		m_bSplitterMovingV =
			PIANOROLLFRAME_BORDERWIDTH * 2 + m_lScaleWidth + m_lTimeWidth + m_lVScrollBarWidth <= point.x &&
			point.x < PIANOROLLFRAME_BORDERWIDTH * 2 + m_lScaleWidth + m_lTimeWidth + m_lVScrollBarWidth +
			PIANOROLLFRAME_SPLITTERWIDTH;
		// カーソルが水平スプリッターと垂直スプリッターの交差部にある場合
		if (m_bSplitterMovingH && m_bSplitterMovingV) {
			::SetCursor (pSekaijuApp->m_hCursorResizeAll);
		}
		// カーソルが水平スプリッター上にある場合
		else if (m_bSplitterMovingH) {
			::SetCursor (pSekaijuApp->m_hCursorResizeNS);
		}
		// カーソルが垂直スプリッター上にある場合
		else if (m_bSplitterMovingV) {
			::SetCursor (pSekaijuApp->m_hCursorResizeWE);
		}
		// カーソルがスプリッターにない場合
		else {
			::SetCursor (pSekaijuApp->m_hCursorArrow);
		}
	}
}

// 時間方向ズームダウン(20091220:左端位置保持機能追加、自動ページ更新自動オフ追加)
void CPianoRollFrame::OnTimeZoomDown () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lOldTimeZoom = m_lTimeZoom;
	long lOldTimePos = m_wndTimeScroll.GetScrollPos ();
	long lNewTimeZoom = CLIP (1, m_lTimeZoom - 1, 16);
	long lNewTimePos = lOldTimePos * lNewTimeZoom / lOldTimeZoom;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_bAutoPageUpdate = FALSE;
	m_lTimeZoom = lNewTimeZoom;
	RecalcTimeScrollInfo ();
	m_wndTimeScroll.SetScrollPos (lNewTimePos);
	m_lTimeScrollPos = m_wndTimeScroll.GetScrollPos ();
	m_pTimeScaleView->Invalidate ();
	m_pKeyTimeView->Invalidate ();
	m_pVelTimeView->Invalidate ();
	//m_bAutoPageUpdate = TRUE;
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// 時間方向ズームアップ(20091220:左端位置保持機能追加、自動ページ更新自動オフ追加)
void CPianoRollFrame::OnTimeZoomUp () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lOldTimeZoom = m_lTimeZoom;
	long lOldTimePos = m_wndTimeScroll.GetScrollPos ();
	long lNewTimeZoom = CLIP (1, m_lTimeZoom + 1, 16);
	long lNewTimePos = lOldTimePos * lNewTimeZoom / lOldTimeZoom;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_bAutoPageUpdate = FALSE;
	m_lTimeZoom = lNewTimeZoom;
	RecalcTimeScrollInfo ();
	m_wndTimeScroll.SetScrollPos (lNewTimePos);
	m_lTimeScrollPos = m_wndTimeScroll.GetScrollPos ();
	m_pTimeScaleView->Invalidate ();
	m_pKeyTimeView->Invalidate ();
	m_pVelTimeView->Invalidate ();
	//m_bAutoPageUpdate = TRUE;
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// キー方向ズームダウン(20091220:上端位置保持機能追加)
void CPianoRollFrame::OnKeyZoomDown () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lOldKeyZoom = m_lKeyZoom;
	long lOldKeyPos = m_wndKeyScroll.GetScrollPos ();
	long lNewKeyZoom = CLIP (4, m_lKeyZoom - 1, 16);
	long lNewKeyPos = lOldKeyPos * lNewKeyZoom / lOldKeyZoom;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_lKeyZoom = lNewKeyZoom;
	RecalcKeyScrollInfo ();
	m_wndKeyScroll.SetScrollPos (lNewKeyPos);
	m_lKeyScrollPos = m_wndKeyScroll.GetScrollPos ();
	m_pKeyScaleView->Invalidate ();
	m_pKeyTimeView->Invalidate ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// キー方向ズームアップ(20091220:上端位置保持機能追加)
void CPianoRollFrame::OnKeyZoomUp () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lOldKeyZoom = m_lKeyZoom;
	long lOldKeyPos = m_wndKeyScroll.GetScrollPos ();
	long lNewKeyZoom = CLIP (4, m_lKeyZoom + 1, 16);
	long lNewKeyPos = lOldKeyPos * lNewKeyZoom / lOldKeyZoom;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_lKeyZoom = lNewKeyZoom;
	RecalcKeyScrollInfo ();
	m_wndKeyScroll.SetScrollPos (lNewKeyPos);
	m_lKeyScrollPos = m_wndKeyScroll.GetScrollPos ();
	m_pKeyScaleView->Invalidate ();
	m_pKeyTimeView->Invalidate ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// ベロシティ方向ズームダウン(20091220:上端位置保持機能追加)
void CPianoRollFrame::OnVelZoomDown () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lOldVelZoom = m_lVelZoom;
	long lOldVelPos = m_wndVelScroll.GetScrollPos ();
	long lNewVelZoom = CLIP (1, m_lVelZoom - 1, 4);
	long lNewVelPos = lOldVelPos * lNewVelZoom / lOldVelZoom;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_lVelZoom = lNewVelZoom;
	m_lVelHeight = MIN (m_lVelHeight, 128 * m_lVelZoom);
	RecalcLayout (FALSE);
	m_wndVelScroll.SetScrollPos (lNewVelPos);
	m_lVelScrollPos = m_wndVelScroll.GetScrollPos ();
	m_pVelScaleView->Invalidate ();
	m_pVelTimeView->Invalidate ();
	this->Invalidate ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// ベロシティ方向ズームアップ(20091220:上端位置保持機能追加)
void CPianoRollFrame::OnVelZoomUp () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lOldVelZoom = m_lVelZoom;
	long lOldVelPos = m_wndVelScroll.GetScrollPos ();
	long lNewVelZoom = CLIP (1, m_lVelZoom + 1, 4);
	long lNewVelPos = lOldVelPos * lNewVelZoom / lOldVelZoom;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_lVelZoom = lNewVelZoom;
	m_lVelHeight = MIN (m_lVelHeight, 128 * m_lVelZoom);
	RecalcLayout (FALSE);
	m_wndVelScroll.SetScrollPos (lNewVelPos);
	m_lVelScrollPos = m_wndVelScroll.GetScrollPos ();
	m_pVelScaleView->Invalidate ();
	m_pVelTimeView->Invalidate ();
	this->Invalidate ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// 水平スクロール
void CPianoRollFrame::OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	if (pScrollBar == &m_wndTimeScroll) {
		int nMin = 0;
		int nMax = 0;
		pScrollBar->GetScrollRange (&nMin, &nMax);
		long lNewPos = m_lTimeScrollPos;
		switch (nSBCode) {
		case SB_LINELEFT:
			lNewPos = m_lTimeScrollPos - m_lTimeZoom;
			break;
		case SB_LINERIGHT:
			lNewPos = m_lTimeScrollPos + m_lTimeZoom;
			break;
		case SB_PAGELEFT:
			lNewPos = m_lTimeScrollPos - m_lTimeZoom * 4;
			break;
		case SB_PAGERIGHT:
			lNewPos = m_lTimeScrollPos + m_lTimeZoom * 4;
			break;
		case SB_LEFT: // 20100206追加
			lNewPos = nMin;
			break;
		case SB_RIGHT: // 20100206追加
			lNewPos = nMax;
			break;
		case SB_THUMBTRACK:
			lNewPos = nPos;
			break;
		}
		SetTimeScrollPos (PIANOROLLFRAME_RANGE (0, lNewPos, 0x7FFFFFFF));
		m_bAutoPageUpdate = FALSE;
		m_pKeyTimeView->SetFocus ();
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// 垂直スクロール
void CPianoRollFrame::OnVScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	if (pScrollBar == &m_wndKeyScroll) {
		int nMin = 0;
		int nMax = 0;
		pScrollBar->GetScrollRange (&nMin, &nMax);
		long lNewPos = m_lKeyScrollPos;
		switch (nSBCode) {
		case SB_LINEUP:
			lNewPos = m_lKeyScrollPos - m_lKeyZoom;
			break;
		case SB_LINEDOWN:
			lNewPos = m_lKeyScrollPos + m_lKeyZoom;
			break;
		case SB_PAGEUP:
			lNewPos = m_lKeyScrollPos - m_lKeyZoom * 12;
			break;
		case SB_PAGEDOWN:
			lNewPos = m_lKeyScrollPos + m_lKeyZoom * 12;
			break;
		case SB_TOP: // 20100206追加
			lNewPos = nMin;
			break;
		case SB_BOTTOM: // 20100206追加
			lNewPos = nMax;
			break;
		case SB_THUMBTRACK:
			lNewPos = nPos;
			break;
		}
		SetKeyScrollPos (PIANOROLLFRAME_RANGE (0, lNewPos, 0x7FFFFFFF));
		m_pKeyTimeView->SetFocus ();
	}
	else if (pScrollBar == &m_wndVelScroll) {
		int nMin = 0;
		int nMax = 0;
		pScrollBar->GetScrollRange (&nMin, &nMax);
		long lNewPos = m_lVelScrollPos;
		switch (nSBCode) {
		case SB_LINEUP:
			lNewPos = m_lVelScrollPos - m_lVelZoom * 2;
			break;
		case SB_LINEDOWN:
			lNewPos = m_lVelScrollPos + m_lVelZoom * 2;
			break;
		case SB_PAGEUP:
			lNewPos = m_lVelScrollPos - m_lVelZoom * 20;
			break;
		case SB_PAGEDOWN:
			lNewPos = m_lVelScrollPos + m_lVelZoom * 20;
			break;
		case SB_TOP: // 20100206追加
			lNewPos = nMin;
			break;
		case SB_BOTTOM: // 20100206追加
			lNewPos = nMax;
			break;
		case SB_THUMBTRACK:
			lNewPos = nPos;
			break;
		}
		SetVelScrollPos (PIANOROLLFRAME_RANGE (0, lNewPos, 0x7FFFFFFF));
		m_pVelTimeView->SetFocus ();
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// 『ツール(&T)』-『ペン(&P)』
void CPianoRollFrame::OnPianoRollPen () {
	m_lCurTool = ID_PIANOROLL_PEN;
}

// 『ツール(&T)』-『ペン(&P)』
void CPianoRollFrame::OnUpdatePianoRollPenUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurTool == ID_PIANOROLL_PEN);
}

// 『ツール(&T)』-『線(&L)』
void CPianoRollFrame::OnPianoRollLine () {
	m_lCurTool = ID_PIANOROLL_LINE;
}

// 『ツール(&T)』-『線(&L)』
void CPianoRollFrame::OnUpdatePianoRollLineUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurTool == ID_PIANOROLL_LINE);
}

// 『ツール(&T)』-『消しゴム(&E)』
void CPianoRollFrame::OnPianoRollEraser () {
	m_lCurTool = ID_PIANOROLL_ERASER;
}

// 『ツール(&T)』-『消しゴム(&E)』
void CPianoRollFrame::OnUpdatePianoRollEraserUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurTool == ID_PIANOROLL_ERASER);
}

// 『ツール(&T)』-『選択(&S)』
void CPianoRollFrame::OnPianoRollSelect () {
	m_lCurTool = ID_PIANOROLL_SELECT;
}

// 『ツール(&T)』-『選択(&S)』
void CPianoRollFrame::OnUpdatePianoRollSelectUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurTool == ID_PIANOROLL_SELECT);
}

// 『ツール(&T)』-『スピーカ(&P)』
void CPianoRollFrame::OnPianoRollSpeaker () {
	m_lCurTool = ID_PIANOROLL_SPEAKER;
}

// 『ツール(&T)』-『スピーカ(&P)』
void CPianoRollFrame::OnUpdatePianoRollSpeakerUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurTool == ID_PIANOROLL_SPEAKER);
}

// 『ツール(&T)』-『現在のトラックのみ表示(&C)』
void CPianoRollFrame::OnPianoRollOnlyCurTrack () {
	long lTrackCount = m_wndTrackList.GetCount ();
	long lTrackCurSel = m_wndTrackList.GetCurSel ();
	if (m_bOnlyCurTrack) {
		m_bOnlyCurTrack = FALSE;
		m_bShowAllTrack = FALSE;
	}
	else {
		m_bOnlyCurTrack = TRUE;
		m_bShowAllTrack = FALSE;
	}
	for (long i = 0; i < lTrackCount; i++) {
		m_wndTrackList.SetCheck (i, IsTrackVisible (i) ? 1 : 0);
	}
	m_pKeyTimeView->Invalidate ();
	m_pVelTimeView->Invalidate ();
}

// 『ツール(&T)』-『現在のトラックのみ表示(&C)』
void CPianoRollFrame::OnUpdatePianoRollOnlyCurTrackUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_bOnlyCurTrack);
}

// 『ツール(&T)』-『全てのトラックを表示(&A)』
void CPianoRollFrame::OnPianoRollShowAllTrack () {
	long lTrackCount = m_wndTrackList.GetCount ();
	long lTrackCurSel = m_wndTrackList.GetCurSel ();
	if (m_bShowAllTrack) {
		m_bOnlyCurTrack = FALSE;
		m_bShowAllTrack = FALSE;
	}
	else {
		m_bOnlyCurTrack = FALSE;
		m_bShowAllTrack = TRUE;
	}
	for (long i = 0; i < lTrackCount; i++) {
		m_wndTrackList.SetCheck (i, IsTrackVisible (i) ? 1 : 0);
	}
	m_pKeyTimeView->Invalidate ();
	m_pVelTimeView->Invalidate ();
}	

// 『ツール(&T)』-『全てのトラックを表示(&A)』
void CPianoRollFrame::OnUpdatePianoRollShowAllTrackUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_bShowAllTrack);
}

// 『ツール(&T)』-『現在のグラフのみ表示(&C)』
void CPianoRollFrame::OnPianoRollOnlyCurGraph () {
	long lGraphKindCount = m_wndGraphKindList.GetCount ();
	long lGraphKindCurSel = m_wndGraphKindList.GetCurSel ();
	if (m_bOnlyCurGraph) {
		m_bOnlyCurGraph = FALSE;
		m_bShowAllGraph = FALSE;
	}
	else {
		m_bOnlyCurGraph = TRUE;
		m_bShowAllGraph = FALSE;
	}
	for (long i = 0; i < lGraphKindCount; i++) {
		m_wndGraphKindList.SetCheck (i, IsGraphVisible (i) ? 1 : 0);
	}
	m_pKeyTimeView->Invalidate ();
	m_pVelTimeView->Invalidate ();
	m_pVelScaleView->Invalidate ();
}

// 『ツール(&T)』-『現在のグラフのみ表示(&C)』
void CPianoRollFrame::OnUpdatePianoRollOnlyCurGraphUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_bOnlyCurGraph);
}


// 『ツール(&T)』-『全てのグラフを表示(&A)』
void CPianoRollFrame::OnPianoRollShowAllGraph () {
	long lGraphKindCount = m_wndGraphKindList.GetCount ();
	long lGraphKindCurSel = m_wndGraphKindList.GetCurSel ();
	if (m_bShowAllGraph) {
		m_bOnlyCurGraph = FALSE;
		m_bShowAllGraph = FALSE;
	}
	else {
		m_bOnlyCurGraph = FALSE;
		m_bShowAllGraph = TRUE;
	}
	for (long i = 0; i < lGraphKindCount; i++) {
		m_wndGraphKindList.SetCheck (i, IsGraphVisible (i) ? 1 : 0);
	}
	m_pKeyTimeView->Invalidate ();
	m_pVelTimeView->Invalidate ();
	m_pVelScaleView->Invalidate ();
}	

// 『ツール(&T)』-『全てのグラフを表示(&A)』
void CPianoRollFrame::OnUpdatePianoRollShowAllGraphUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_bShowAllGraph);
}

// 『ツール(&T)』-『自動ページ更新』
void CPianoRollFrame::OnPianoRollAutoPageUpdate () {
	m_bAutoPageUpdate = !m_bAutoPageUpdate;
}

// 『ツール(&T)』-『自動ページ更新』
void CPianoRollFrame::OnUpdatePianoRollAutoPageUpdateUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_bAutoPageUpdate);
}

// トラックコンボが選択され終わった時
void CPianoRollFrame::OnTrackComboSelEndOK () {
	// カレントトラックを更新する
	long lCurTrackIndex = m_wndTrackCombo.GetCurSel ();
	SetCurTrackIndex (lCurTrackIndex);
	SetTrackVisible (lCurTrackIndex);
	// カレントチャンネルも更新する
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lCurTrackIndex);
	if (pMIDITrack) {
		long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
		if (0 <= lOutputChannel && lOutputChannel <= 15) {
			SetCurChannel (lOutputChannel);
		}
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// 再描画
	m_pKeyScaleView->Invalidate ();
	m_pKeyTimeView->Invalidate ();
	m_pVelScaleView->Invalidate ();
	m_pVelTimeView->Invalidate ();
}

// トラックリストのチェックボックスが変化したとき
void CPianoRollFrame::OnTrackListChkChange () {
	// トラックの表示／非表示をチェックボックスの状態に合わせる
	long lCount = m_wndTrackList.GetCount ();
	long lCurSel = m_wndTrackList.GetCurSel ();
	if (m_bOnlyCurTrack) {
		m_bOnlyCurTrack = FALSE;
	}
	for (long i = 0; i < lCount; i++) {
		m_bTrackVisible[i] = m_wndTrackList.GetCheck (i);
		if (m_bTrackVisible[i] == FALSE) { // TODO
			m_bShowAllTrack = FALSE;
		}
	}
	m_pKeyScaleView->Invalidate ();
	m_pKeyTimeView->Invalidate ();
	m_pVelScaleView->Invalidate ();
	m_pVelTimeView->Invalidate ();
}

// トラックリストの選択項目が変化したとき
void CPianoRollFrame::OnTrackListSelChange () {
	// カレントトラックを変更する
	long lCurTrackIndex = m_wndTrackList.GetCurSel ();
	SetCurTrackIndex (lCurTrackIndex);
	// カレントチャンネルも更新する
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lCurTrackIndex);
	if (pMIDITrack) {
		long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
		if (0 <= lOutputChannel && lOutputChannel <= 15) {
			SetCurChannel (lOutputChannel);
		}
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// 再描画
	m_pKeyScaleView->Invalidate ();
	m_pKeyTimeView->Invalidate ();
	m_pVelScaleView->Invalidate ();
	m_pVelTimeView->Invalidate ();
}

// グラフの種類コンボが選択され終わった時
void CPianoRollFrame::OnGraphKindComboSelEndOK () {
	long lCurGraphKind = m_wndGraphKindCombo.GetCurSel ();
	SetCurGraphKind (lCurGraphKind);
	SetGraphVisible (lCurGraphKind);
	m_pVelScaleView->Invalidate ();
	m_pVelTimeView->Invalidate ();
}

// グラフの種類リストのチェックボックスが変化したとき
void CPianoRollFrame::OnGraphKindListChkChange () {
	// グラフの表示／非表示をチェックボックスの状態に合わせる
	long lCount = m_wndGraphKindList.GetCount ();
	long lCurSel = m_wndGraphKindList.GetCurSel ();
	if (m_bOnlyCurGraph) {
		m_bOnlyCurGraph = FALSE;
	}
	else {
		for (long i = 0; i < lCount; i++) {
			m_bGraphVisible[i] = m_wndGraphKindList.GetCheck (i);
			if (m_bGraphVisible[i] == FALSE) {
				m_bShowAllGraph = FALSE;
			}
		}
	}
	//m_pKeyTimeView->Invalidate ();
	m_pVelScaleView->Invalidate ();
	m_pVelTimeView->Invalidate ();
}

// グラフの種類リストの選択項目が変化したとき
void CPianoRollFrame::OnGraphKindListSelChange () {
	// カレントグラフを変更する。
	long lCurGraphKind = m_wndGraphKindList.GetCurSel ();
	SetCurGraphKind (lCurGraphKind);
	m_pVelScaleView->Invalidate ();
	m_pVelTimeView->Invalidate ();
}

// 『ポップアップ』-『このトラックのみ表示ON』
void CPianoRollFrame::OnPopupTrackVisibleOn () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	if (pSekaijuDoc->m_pTempTrack == NULL) { // 20100429追加
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	long lCount = m_wndTrackList.GetCount ();
	long i = 0;
	forEachTrack (pMIDIData, pMIDITrack) {
		if (0 <= i && i < lCount) {
			m_wndTrackList.SetCheck (i, pMIDITrack == pSekaijuDoc->m_pTempTrack ? TRUE : FALSE);
			m_bTrackVisible[i] = (pMIDITrack == pSekaijuDoc->m_pTempTrack ? TRUE : FALSE);
		}
		i++;
	}
	m_bOnlyCurTrack = FALSE;
	m_bShowAllTrack = FALSE;
	// カレントトラックとカレントチャンネルを更新
	long lTrackIndex = pSekaijuDoc->GetTrackIndex (pSekaijuDoc->m_pTempTrack);
	ASSERT (0 <= lTrackIndex && lTrackIndex < lCount);
	if (m_bTrackVisible[lTrackIndex]) {
		SetCurTrackIndex (lTrackIndex);
		pMIDITrack = pSekaijuDoc->GetTrack (lTrackIndex);
		if (pMIDITrack) {
			long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
			if (0 <= lOutputChannel && lOutputChannel <= 15) {
				SetCurChannel (lOutputChannel);
			}
		}
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// 再描画
	m_pKeyScaleView->Invalidate ();
	m_pKeyTimeView->Invalidate ();
	m_pVelScaleView->Invalidate ();
	m_pVelTimeView->Invalidate ();
}

// 『ポップアップ』-『このトラックのみ表示ON』 // 20100429追加
void CPianoRollFrame::OnUpdatePopupTrackVisibleOnUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	if (pSekaijuDoc->m_pTempTrack == NULL) { // 20100429追加
		pCmdUI->Enable (FALSE);
	}
}


// 『ポップアップ』-『このトラックのみ表示OFF』
void CPianoRollFrame::OnPopupTrackVisibleOff () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	if (pSekaijuDoc->m_pTempTrack == NULL) { // 20100429追加
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	long lCount = m_wndTrackList.GetCount ();
	long i = 0;
	forEachTrack (pMIDIData, pMIDITrack) {
		if (0 <= i && i < lCount) {
			m_wndTrackList.SetCheck (i, pMIDITrack == pSekaijuDoc->m_pTempTrack ? FALSE : TRUE);
			m_bTrackVisible[i] = (pMIDITrack == pSekaijuDoc->m_pTempTrack ? FALSE : TRUE);
		}
		i++;
	}
	m_bOnlyCurTrack = FALSE;
	m_bShowAllTrack = FALSE;
	// カレントトラックとカレントチャンネルを更新
	long lTrackIndex = pSekaijuDoc->GetTrackIndex (pSekaijuDoc->m_pTempTrack);
	ASSERT (0 <= lTrackIndex && lTrackIndex < lCount);
	if (m_bTrackVisible[lTrackIndex]) {
		SetCurTrackIndex (lTrackIndex);
		pMIDITrack = pSekaijuDoc->GetTrack (lTrackIndex);
		if (pMIDITrack) {
			long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
			if (0 <= lOutputChannel && lOutputChannel <= 15) {
				SetCurChannel (lOutputChannel);
			}
		}
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// 再描画
	m_pKeyScaleView->Invalidate ();
	m_pKeyTimeView->Invalidate ();
	m_pVelScaleView->Invalidate ();
	m_pVelTimeView->Invalidate ();
}

// 『ポップアップ』-『このトラックのみ表示OFF』 // 20100429追加
void CPianoRollFrame::OnUpdatePopupTrackVisibleOffUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	if (pSekaijuDoc->m_pTempTrack == NULL) { // 20100429追加
		pCmdUI->Enable (FALSE);
	}
}

// 『ポップアップ』-『全トラック表示ON』
void CPianoRollFrame::OnPopupTrackVisibleAll () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	long lCount = m_wndTrackList.GetCount ();
	long i = 0;
	forEachTrack (pMIDIData, pMIDITrack) {
		if (0 <= i && i < lCount) {
			m_wndTrackList.SetCheck (i, 1);
			m_bTrackVisible[i] = TRUE;
		}
		i++;
	}
	m_bOnlyCurTrack = FALSE;
	m_bShowAllTrack = FALSE;
	// カレントトラックとカレントチャンネルを更新 // 20100429廃止
	//long lTrackIndex = pSekaijuDoc->GetTrackIndex (pSekaijuDoc->m_pTempTrack);
	//ASSERT (0 <= lTrackIndex && lTrackIndex < lCount);
	//if (m_bTrackVisible[lTrackIndex]) {
	//	SetCurTrackIndex (lTrackIndex);
	//	pMIDITrack = pSekaijuDoc->GetTrack (lTrackIndex);
	//	if (pMIDITrack) {
	//		long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
	//		if (0 <= lOutputChannel && lOutputChannel <= 15) {
	//			SetCurChannel (lOutputChannel);
	//		}
	//	}
	//}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// 再描画
	m_pKeyScaleView->Invalidate ();
	m_pKeyTimeView->Invalidate ();
	m_pVelScaleView->Invalidate ();
	m_pVelTimeView->Invalidate ();

}

// 『ポップアップ』-『全トラック表示ON』 // 20100429追加
void CPianoRollFrame::OnUpdatePopupTrackVisibleAllUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

}

// 『ポップアップ』-『このイベントのプロパティ(&R)』// 20110109修正
void CPianoRollFrame::OnPopupEventProperty () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (pSekaijuDoc->m_bEditLocked) {
		return;
	}
	if (pSekaijuDoc->m_pTempEvent == NULL || pSekaijuDoc->m_pTempTrack == NULL) {
		return;
	}
	if (!MIDIEvent_IsNote (pSekaijuDoc->m_pTempEvent)) {
		return;
	}
	MIDIEvent* pNoteOffEvent = pSekaijuDoc->m_pTempEvent->m_pNextCombinedEvent;
	if (pNoteOffEvent == NULL) {
		return;
	}
	long lTrackIndex = pSekaijuDoc->GetTrackIndex (pSekaijuDoc->m_pTempTrack);
	long lTime = MIDIEvent_GetTime (pSekaijuDoc->m_pTempEvent);
	CString strTime;
	pSekaijuDoc->LongTimeToStringTime (pSekaijuDoc->m_pMIDIData, lTime, &strTime);
	CPropertyNoteDlg theDlg;
	theDlg.m_bTrackZeroOrigin = pSekaijuApp->m_theGeneralOption.m_bTrackZeroOrigin;
	theDlg.m_bNoteOnNoteOn0 = (MIDIEvent_GetKind (pNoteOffEvent) & 0xF0) == 0x80 ? FALSE : TRUE;
	MIDITrack* pMIDITrack;
	forEachTrack (pSekaijuDoc->m_pMIDIData, pMIDITrack) {
		TCHAR szTrackName[256];
		memset (szTrackName, 0, sizeof (szTrackName));
		MIDITrack_GetName (pMIDITrack, szTrackName, TSIZEOF (szTrackName));
		theDlg.m_theTrackNameArray.Add (szTrackName);
		long lTrackOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
		theDlg.m_theTrackOutputChannelArray.Add ((DWORD)lTrackOutputChannel);
	}
	long lKey = 0;
	for (lKey = 0; lKey < 128; lKey++) {
		CString strKeyName;
		strKeyName = pSekaijuDoc->GetKeyName (pSekaijuDoc->m_pTempTrack, lTime, lKey);
		theDlg.m_theKeyNameArray.Add (strKeyName);
	}
	theDlg.m_nTrackIndex = (int)lTrackIndex;
	theDlg.m_strTime = strTime;
	theDlg.m_nChannel = (int)MIDIEvent_GetChannel (pSekaijuDoc->m_pTempEvent) + 1;
	theDlg.m_nKey = (int)MIDIEvent_GetKey (pSekaijuDoc->m_pTempEvent);
	theDlg.m_nOnVelocity = (int)MIDIEvent_GetVelocity (pSekaijuDoc->m_pTempEvent);
	theDlg.m_nOffVelocity = (int)MIDIEvent_GetVelocity (pNoteOffEvent);
	theDlg.m_nDuration = (int)MIDIEvent_GetDuration (pSekaijuDoc->m_pTempEvent);
	if (theDlg.DoModal () == IDOK) {
		// トラック抽出
		long lTrackIndex = (long)theDlg.m_nTrackIndex;
		if (MIDIData_GetFormat (pSekaijuDoc->m_pMIDIData) == 1 && lTrackIndex == 0) {
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_NOTEEVENT_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			return;
		}
		MIDITrack* pNewTrack = pSekaijuDoc->GetTrack (lTrackIndex);
		if (pNewTrack == NULL) {
			CString strMsg;
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			return;
		}
		// タイム文字列をlong型に変換
		long lTime = MIDIEvent_GetTime (pSekaijuDoc->m_pTempEvent);
		long lErrorID = pSekaijuDoc->StringTimeToLongTime (pSekaijuDoc->m_pMIDIData, theDlg.m_strTime, &lTime);
		if (lErrorID != 0) {
			CString strMsg;
			VERIFY (strMsg.LoadString (lErrorID));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			return;
		}
		BeginWaitCursor ();
		pSekaijuDoc->m_theCriticalSection.Lock ();
		MIDIEvent* pNewEvent = NULL;
		MIDIEvent* pLastEvent = NULL;
		MIDIEvent* pCloneEvent = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_NOTE));
		pSekaijuDoc->AddHistoryUnit (strHistoryName);
		CHistoryUnit* pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
		// 所属トラックが変更されない場合
		if (pNewTrack == pSekaijuDoc->m_pTempTrack) {
			// EOTの履歴保持(20110109修正)
			pLastEvent = MIDITrack_GetLastEvent (pNewTrack);
			if (pLastEvent != NULL) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
					VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent));
				}
			}
			// イベントの置換と変更
			pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pSekaijuDoc->m_pTempEvent);
			VERIFY (pNewEvent = pSekaijuDoc->ReplaceMIDIEvent (pSekaijuDoc->m_pTempEvent));
			MIDIEvent_SetTime (pNewEvent, lTime);
			MIDIEvent_SetChannel (pNewEvent, theDlg.m_nChannel - 1);
			MIDIEvent_SetKey (pNewEvent, theDlg.m_nKey);
			MIDIEvent_SetVelocity (pNewEvent, theDlg.m_nOnVelocity);
			MIDIEvent_SetDuration (pNewEvent, theDlg.m_nDuration);
			if (!theDlg.m_bNoteOnNoteOn0) {
				MIDIEvent* pNewNoteOffEvent = pNewEvent->m_pNextCombinedEvent;
				MIDIEvent_SetVelocity (pNewNoteOffEvent, theDlg.m_nOffVelocity);
			}
			pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent);
			// EOTの履歴保持(20110109修正)
			pLastEvent = MIDITrack_GetLastEvent (pNewTrack);
			if (pLastEvent != NULL) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pLastEvent));
				}
			}
		}
		// 所属トラックが変更される場合
		else {
			// イベントの置換と除去
			pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pSekaijuDoc->m_pTempEvent);
			VERIFY (pNewEvent = pSekaijuDoc->ReplaceMIDIEvent (pSekaijuDoc->m_pTempEvent));
			VERIFY (MIDITrack_RemoveEvent (pSekaijuDoc->m_pTempTrack, pNewEvent));
			// EOTの履歴保持(20110109修正)
			pLastEvent = MIDITrack_GetLastEvent (pNewTrack);
			if (pLastEvent != NULL) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
					VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent));
				}
			}
			// イベントの挿入と変更
			VERIFY (MIDITrack_InsertEvent (pNewTrack, pNewEvent));
			MIDIEvent_SetTime (pNewEvent, lTime);
			MIDIEvent_SetChannel (pNewEvent, theDlg.m_nChannel - 1);
			MIDIEvent_SetKey (pNewEvent, theDlg.m_nKey);
			MIDIEvent_SetVelocity (pNewEvent, theDlg.m_nOnVelocity);
			MIDIEvent_SetDuration (pNewEvent, theDlg.m_nDuration);
			if (!theDlg.m_bNoteOnNoteOn0) {
				MIDIEvent* pNewNoteOffEvent = pNewEvent->m_pNextCombinedEvent;
				MIDIEvent_SetVelocity (pNewNoteOffEvent, theDlg.m_nOffVelocity);
			}
			pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent);
			// EOTの履歴保持(20110109修正)
			pLastEvent = MIDITrack_GetLastEvent (pNewTrack);
			if (pLastEvent != NULL) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pLastEvent));
				}
			}
		}
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		SetTrackVisible (theDlg.m_nTrackIndex);
		SetCurTrackIndex (theDlg.m_nTrackIndex);
		SetCurChannel (theDlg.m_nChannel - 1);
		SetCurVelocity (theDlg.m_nOnVelocity);
		SetCurDuration (theDlg.m_nDuration);
		pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		::Sleep (0);
	}
}

// 『ポップアップ』-『このイベントのプロパティ(&R)』
void CPianoRollFrame::OnUpdatePopupEventPropertyUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (pSekaijuDoc->m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (pSekaijuDoc->m_pTempEvent == NULL || pSekaijuDoc->m_pTempTrack == NULL) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (!MIDIEvent_IsNote (pSekaijuDoc->m_pTempEvent)) {
		pCmdUI->Enable (FALSE);
		return;
	}
	MIDIEvent* pNoteOffEvent = pSekaijuDoc->m_pTempEvent->m_pNextCombinedEvent;
	if (pNoteOffEvent == NULL) {
		pCmdUI->Enable (FALSE);
		return;
	}
}
