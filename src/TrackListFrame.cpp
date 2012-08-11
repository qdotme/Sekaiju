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
#include <afxmt.h>
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "resource.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDoc.h"
#include "SekaijuView.h"
#include "SekaijuToolBar.h"
#include "SekaijuFileDlg.h"
#include "MainFrame.h"
#include "ChildFrame.h"
#include "TrackListFrame.h"
#include "TrackListPrintView.h"
#include "TrackListScaleView.h"
#include "TrackListModeScaleView.h"
#include "TrackListTimeScaleView.h"
#include "TrackListTrackScaleView.h"
#include "TrackListTrackModeView.h"
#include "TrackListTrackTimeView.h"

#include "..\\..\\MIDIData\\MIDIData.h"



// アロケーションの監視
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 子ウィンドウのサイズ
#define TRACKLISTFRAME_SCALEHEIGHT 32
#define TRACKLISTFRAME_SCALEWIDTH 32
#define TRACKLISTFRAME_MODEWIDTH 256
#define TRACKLISTFRAME_TIMEWIDTH 600
#define TRACKLISTFRAME_TRACKHEIGHT 480
#define TRACKLISTFRAME_SCROLLBARHEIGHT 16
#define TRACKLISTFRAME_SCROLLBARWIDTH 16
#define TRACKLISTFRAME_BORDERWIDTH 2
#define TRACKLISTFRAME_BORDERHEIGHT 2
#define TRACKLISTFRAME_SPLITTERWIDTH 4
#define TRACKLISTFRAME_SPLITTERHEIGHT 4

// 子ウィンドウIDを定義
#define TRACKLISTFRAME_DUMMYVIEW      (AFX_IDW_PANE_FIRST + 0)
#define TRACKLISTFRAME_PRINTVIEW      (AFX_IDW_PANE_FIRST + 1)
#define TRACKLISTFRAME_SCALEVIEW      (AFX_IDW_PANE_FIRST + 32)
#define TRACKLISTFRAME_MODESCALEVIEW  (AFX_IDW_PANE_FIRST + 33)
#define TRACKLISTFRAME_TIMESCALEVIEW  (AFX_IDW_PANE_FIRST + 34)
#define TRACKLISTFRAME_TRACKSCALEVIEW (AFX_IDW_PANE_FIRST + 35)
#define TRACKLISTFRAME_TRACKMODEVIEW  (AFX_IDW_PANE_FIRST + 36)
#define TRACKLISTFRAME_TRACKTIMEVIEW  (AFX_IDW_PANE_FIRST + 37)
#define TRACKLISTFRAME_COLUMNSCROLL   (AFX_IDW_PANE_FIRST + 48)
#define TRACKLISTFRAME_TIMESCROLL     (AFX_IDW_PANE_FIRST + 49)
#define TRACKLISTFRAME_ROWSCROLL      (AFX_IDW_PANE_FIRST + 50)
#define TRACKLISTFRAME_SIZEBOX        (AFX_IDW_PANE_FIRST + 51)
#define TRACKLISTFRAME_COLUMNZOOMDOWN (AFX_IDW_PANE_FIRST + 52)
#define TRACKLISTFRAME_COLUMNZOOMUP   (AFX_IDW_PANE_FIRST + 53)
#define TRACKLISTFRAME_TIMEZOOMDOWN   (AFX_IDW_PANE_FIRST + 54)
#define TRACKLISTFRAME_TIMEZOOMUP     (AFX_IDW_PANE_FIRST + 55)
#define TRACKLISTFRAME_ROWZOOMDOWN    (AFX_IDW_PANE_FIRST + 56)
#define TRACKLISTFRAME_ROWZOOMUP      (AFX_IDW_PANE_FIRST + 57)

//
// トラックリストフレームのクライアント領域に配置されたオブジェクト
//	　　　　　　　　　　　　　　　　　　　　　　　　　
//　　　　　　 　　                                  ┃                      m_lVSc
//                   ┃←m_lScale→│←  m_lMode   →★←    m_lTime   →│←rollBa→┃																		TIMEWIDTH  　　　  →│←LBAR→┃
//                       Width            Width      ┃       Width          rWidth
//        ━
//        ↑         ┏━━━━━━┯━━━━━━━━┳━━━━━━━━━━━━━┯━┓
//  m_lScaleHeight   ┃CView*      │CView*  　 　   ┃CView*                    │↑┃
//        ↓         ┃m_pScaleView│m_pModeScaleView┃m_pTimeScaleView        　├─┨
//        ─         ┠──────┼────────╂─────────────┤　┃
//        ↑         ┃CView*      │CView*      　  ┃CView*    　　　          │□┃CScrollBar
//                   ┃m_pTrack    │m_pTrackModeView┃m_pTrackTimeView      　  │　┃m_wndRowScroll
//                   ┃ScaleView   │　　　　　　    ┃        　　　　　　　　  ├─┨
//  m_lTrackHeight   ┃      　　　│　　　　　　　　★　　　　　　　　        　│↓┃
//                   ┃      　　　│　　　　　　　　┃　　　　　　　　          ├─┨CButton
//                   ┃      　　　│　　　　　　　　┃　　　　　　　　          │－┃m_wndRowZoomDown
//        ↓         ┃      　　　│　　　　　　　　┃　　　　　　　          　├─┨CButton
//        ─         ┃      　　　│　　　　　　　　┃　　　　　　　          　│＋┃m_wndRowZoomUp
//        ↑         ┠─┬────┴──┬─┬─┬─╂─┬─────┬─┬─┬─┼─┨CScrollBar
//m_lHScrollBarHeight┃←│　　□　      │→│－│＋┃←│　　□　　│→│－│＋│⊿┃m_wndSizeScroll
//        ↓         ┗━┷━━━━━━━┷━┷━┷━┻━┷━━━━━┷━┷━┷━┷━┛
//        ━          CScrollBar    CButton     CButton     CScrollBar CButton   CButton
//                    m_wndColumn   m_wndColmun m_wndColumn m_wndTime  m_wndTime m_wndTime
//                    Scroll        ZoomDown    ZoomUpUP    Scroll     ZoomDown  ZoomUp
//
// (あ)―――：単純な境界(0px)。
// (い)━━━：太く立体的な境界線。BORDERWIDTH(2px)又はBORDERHEIGHT(2px)で示す幅を占領
// (う)━★━：スプリッター境界線。(い)*2+SPRITTERWIDTH(4px)又はSPRITTERHEIGHT(4px)で示す幅を占領。
//


// メッセージマップ
IMPLEMENT_DYNCREATE(CTrackListFrame, CChildFrame)

BEGIN_MESSAGE_MAP(CTrackListFrame, CChildFrame)
	ON_WM_CREATE ()
	ON_WM_DESTROY ()
	ON_WM_SIZE ()
	ON_WM_TIMER ()
	ON_WM_ERASEBKGND ()
	ON_WM_MDIACTIVATE ()
	ON_WM_CLOSE ()
	ON_WM_PAINT ()	
	ON_WM_LBUTTONDOWN ()	
	ON_WM_RBUTTONDOWN ()	
	ON_WM_LBUTTONUP ()	
	ON_WM_RBUTTONUP ()	
	ON_WM_MOUSEMOVE ()
	ON_WM_HSCROLL ()	
	ON_WM_VSCROLL ()

	ON_BN_CLICKED (TRACKLISTFRAME_COLUMNZOOMDOWN, OnColumnZoomDown)
	ON_BN_CLICKED (TRACKLISTFRAME_COLUMNZOOMUP, OnColumnZoomUp)
	ON_BN_CLICKED (TRACKLISTFRAME_TIMEZOOMDOWN, OnTimeZoomDown)
	ON_BN_CLICKED (TRACKLISTFRAME_TIMEZOOMUP, OnTimeZoomUp)
	ON_BN_CLICKED (TRACKLISTFRAME_ROWZOOMDOWN, OnRowZoomDown)
	ON_BN_CLICKED (TRACKLISTFRAME_ROWZOOMUP, OnRowZoomUp)

	ON_COMMAND (ID_TRACKLIST_INSERTTRACK, OnTrackListInsertTrack)
	ON_UPDATE_COMMAND_UI (ID_TRACKLIST_INSERTTRACK, OnUpdateTrackListInsertTrackUI)
	ON_COMMAND (ID_TRACKLIST_DUPLICATETRACK, OnTrackListDuplicateTrack)
	ON_UPDATE_COMMAND_UI (ID_TRACKLIST_DUPLICATETRACK, OnUpdateTrackListDuplicateTrackUI)
	ON_COMMAND (ID_TRACKLIST_DELETETRACK, OnTrackListDeleteTrack)
	ON_UPDATE_COMMAND_UI (ID_TRACKLIST_DELETETRACK, OnUpdateTrackListDeleteTrackUI)
	ON_COMMAND (ID_TRACKLIST_MOVEUPTRACK, OnTrackListMoveUpTrack)
	ON_UPDATE_COMMAND_UI (ID_TRACKLIST_MOVEUPTRACK, OnUpdateTrackListMoveUpTrackUI)
	ON_COMMAND (ID_TRACKLIST_MOVEDOWNTRACK, OnTrackListMoveDownTrack)
	ON_UPDATE_COMMAND_UI (ID_TRACKLIST_MOVEDOWNTRACK, OnUpdateTrackListMoveDownTrackUI)
	ON_COMMAND (ID_TRACKLIST_SELECT, OnTrackListSelect)
	ON_UPDATE_COMMAND_UI (ID_TRACKLIST_SELECT, OnUpdateTrackListSelectUI)
	ON_COMMAND (ID_TRACKLIST_SPEAKER, OnTrackListSpeaker)
	ON_UPDATE_COMMAND_UI (ID_TRACKLIST_SPEAKER, OnUpdateTrackListSpeakerUI)
	ON_COMMAND (ID_TRACKLIST_AUTOPAGEUPDATE, OnTrackListAutoPageUpdate)
	ON_UPDATE_COMMAND_UI (ID_TRACKLIST_AUTOPAGEUPDATE, OnUpdateTrackListAutoPageUpdateUI)
	ON_COMMAND (ID_TRACKLIST_SAVEAS, OnTrackListSaveAs)
	ON_UPDATE_COMMAND_UI (ID_TRACKLIST_SAVEAS, OnUpdateTrackListSaveAsUI)

END_MESSAGE_MAP()

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CTrackListFrame::CTrackListFrame () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	m_lScaleWidth = TRACKLISTFRAME_SCALEWIDTH;
	m_lScaleHeight = TRACKLISTFRAME_SCALEHEIGHT;
	m_lTrackHeight = TRACKLISTFRAME_TRACKHEIGHT;
	m_lModeWidth = TRACKLISTFRAME_MODEWIDTH;
	m_lTimeWidth = TRACKLISTFRAME_TIMEWIDTH;
	m_lHScrollBarHeight = ::GetSystemMetrics (SM_CYHSCROLL);
	m_lVScrollBarWidth = ::GetSystemMetrics (SM_CXVSCROLL);
	m_lRowZoom = pSekaijuApp->m_theTrackListOption1.m_lDefRowZoom;
	m_lColumnZoom = pSekaijuApp->m_theTrackListOption1.m_lDefColumnZoom;
	m_lTimeZoom = pSekaijuApp->m_theTrackListOption1.m_lDefTimeZoom;
	// カラムの定義
	m_lColumnBaseWidth[0] = pSekaijuApp->m_theTrackListOption1.m_lDefNameWidth;
	m_lColumnBaseWidth[1] = pSekaijuApp->m_theTrackListOption1.m_lDefColorWidth;
	m_lColumnBaseWidth[2] = pSekaijuApp->m_theTrackListOption1.m_lDefInputOnWidth;
	m_lColumnBaseWidth[3] = pSekaijuApp->m_theTrackListOption1.m_lDefInputPortWidth;
	m_lColumnBaseWidth[4] = pSekaijuApp->m_theTrackListOption1.m_lDefInputChWidth;
	m_lColumnBaseWidth[5] = pSekaijuApp->m_theTrackListOption1.m_lDefOutputOnWidth;
	m_lColumnBaseWidth[6] = pSekaijuApp->m_theTrackListOption1.m_lDefOutputPortWidth;
	m_lColumnBaseWidth[7] = pSekaijuApp->m_theTrackListOption1.m_lDefOutputChWidth;
	m_lColumnBaseWidth[8] = pSekaijuApp->m_theTrackListOption1.m_lDefViewModeWidth;
	m_lColumnBaseWidth[9] = pSekaijuApp->m_theTrackListOption2.m_lDefCC000Width;
	m_lColumnBaseWidth[10] = pSekaijuApp->m_theTrackListOption2.m_lDefCC032Width;
	m_lColumnBaseWidth[11] = pSekaijuApp->m_theTrackListOption2.m_lDefPCWidth;
	m_lColumnBaseWidth[12] = pSekaijuApp->m_theTrackListOption2.m_lDefCC007Width;
	m_lColumnBaseWidth[13] = pSekaijuApp->m_theTrackListOption2.m_lDefCC010Width;
	m_lColumnBaseWidth[14] = pSekaijuApp->m_theTrackListOption2.m_lDefCC091Width;
	m_lColumnBaseWidth[15] = pSekaijuApp->m_theTrackListOption2.m_lDefCC093Width;
	m_lColumnBaseWidth[16] = pSekaijuApp->m_theTrackListOption2.m_lDefCC094Width;
	m_lColumnBaseWidth[17] = pSekaijuApp->m_theTrackListOption2.m_lDefKeyShiftWidth;
	m_lColumnBaseWidth[18] = pSekaijuApp->m_theTrackListOption2.m_lDefVelShiftWidth;
	m_lColumnBaseWidth[19] = pSekaijuApp->m_theTrackListOption2.m_lDefTimeShiftWidth;
	m_lColumnBaseWidth[20] = pSekaijuApp->m_theTrackListOption2.m_lDefNumEventWidth;

	VERIFY (m_strColumnTitle[0].LoadString (IDS_NAME));
	VERIFY (m_strColumnTitle[1].LoadString (IDS_COLOR));
	VERIFY (m_strColumnTitle[2].LoadString (IDS_INPUTON));
	VERIFY (m_strColumnTitle[3].LoadString (IDS_INPUTPORT));
	VERIFY (m_strColumnTitle[4].LoadString (IDS_INPUTCHANNEL));
	VERIFY (m_strColumnTitle[5].LoadString (IDS_OUTPUTON));
	VERIFY (m_strColumnTitle[6].LoadString (IDS_OUTPUTPORT));
	VERIFY (m_strColumnTitle[7].LoadString (IDS_OUTPUTCHANNEL));
	VERIFY (m_strColumnTitle[8].LoadString (IDS_VIEWMODE));
	VERIFY (m_strColumnTitle[9].LoadString (IDS_CC_0));
	VERIFY (m_strColumnTitle[10].LoadString (IDS_CC_32));
	VERIFY (m_strColumnTitle[11].LoadString (IDS_PROGRAM_NUMBER));
	VERIFY (m_strColumnTitle[12].LoadString (IDS_VOLUME));
	VERIFY (m_strColumnTitle[13].LoadString (IDS_PAN));
	VERIFY (m_strColumnTitle[14].LoadString (IDS_REVERB));
	VERIFY (m_strColumnTitle[15].LoadString (IDS_CHORUS));
	VERIFY (m_strColumnTitle[16].LoadString (IDS_DELAY));
	VERIFY (m_strColumnTitle[17].LoadString (IDS_TIMEPLUS));
	VERIFY (m_strColumnTitle[18].LoadString (IDS_KEYPLUS));
	VERIFY (m_strColumnTitle[19].LoadString (IDS_VELPLUS));
	VERIFY (m_strColumnTitle[20].LoadString (IDS_NUMEVENT));

	m_lColumnContent[0] = TRACKLISTFRAME_TRACKNAME;
	m_lColumnContent[1] = TRACKLISTFRAME_FORECOLOR;
	m_lColumnContent[2] = TRACKLISTFRAME_INPUTON;
	m_lColumnContent[3] = TRACKLISTFRAME_INPUTPORT;
	m_lColumnContent[4] = TRACKLISTFRAME_INPUTCHANNEL;
	m_lColumnContent[5] = TRACKLISTFRAME_OUTPUTON;
	m_lColumnContent[6] = TRACKLISTFRAME_OUTPUTPORT;
	m_lColumnContent[7] = TRACKLISTFRAME_OUTPUTCHANNEL;
	m_lColumnContent[8] = TRACKLISTFRAME_VIEWMODE;
	m_lColumnContent[9] = TRACKLISTFRAME_CONTROLCHANGE | (0 << 16);
	m_lColumnContent[10] = TRACKLISTFRAME_CONTROLCHANGE | (32 << 16);
	m_lColumnContent[11] = TRACKLISTFRAME_PROGRAMCHANGE;
	m_lColumnContent[12] = TRACKLISTFRAME_CONTROLCHANGE | (7 << 16);
	m_lColumnContent[13] = TRACKLISTFRAME_CONTROLCHANGE | (10 << 16);
	m_lColumnContent[14] = TRACKLISTFRAME_CONTROLCHANGE | (91 << 16);
	m_lColumnContent[15] = TRACKLISTFRAME_CONTROLCHANGE | (93 << 16);
	m_lColumnContent[16] = TRACKLISTFRAME_CONTROLCHANGE | (94 << 16);
	m_lColumnContent[17] = TRACKLISTFRAME_TIMEPLUS;
	m_lColumnContent[18] = TRACKLISTFRAME_KEYPLUS;
	m_lColumnContent[19] = TRACKLISTFRAME_VELOCITYPLUS;
	m_lColumnContent[20] = TRACKLISTFRAME_NUMEVENT;
	
	// デフォルトフォントの作成
	CString strFontName;
	VERIFY (strFontName.LoadString (IDS_DEFAULTFONTNAME));
	m_theFont.CreateFont (12, 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,  
		strFontName);
	m_lCurTool = ID_TRACKLIST_SELECT;

	m_bAutoPageUpdate = FALSE;

}

// デストラクタ
CTrackListFrame::~CTrackListFrame () {
	m_theFont.DeleteObject ();
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// ドキュメントへのポインタ取得
CSekaijuDoc* CTrackListFrame::GetDocument () {
	ASSERT (m_pDummyView);
	return (CSekaijuDoc*)m_pDummyView->GetDocument ();
}

// 指定した列の基本幅を取得
long CTrackListFrame::GetColumnBaseWidth (long lColumn) {
	ASSERT (0 <= lColumn && lColumn < TRACKLISTFRAME_NUMCOLUMN);
	return m_lColumnBaseWidth[lColumn];
}

// 指定した列の基本幅を設定
long CTrackListFrame::SetColumnBaseWidth (long lColumn, long lColumnBaseWidth) {
	ASSERT (0 <= lColumn && lColumn < TRACKLISTFRAME_NUMCOLUMN);
	m_lColumnBaseWidth[lColumn] = lColumnBaseWidth;
	return 1;
}

// 指定した列のタイトルを取得
CString CTrackListFrame::GetColumnTitle (long lColumn) {
	ASSERT (0 <= lColumn && lColumn < TRACKLISTFRAME_NUMCOLUMN);
	return m_strColumnTitle[lColumn];
}

// 指定した列のコンテンツ種類を取得
long CTrackListFrame::GetColumnContent (long lColumn) {
	ASSERT (0 <= lColumn && lColumn < TRACKLISTFRAME_NUMCOLUMN);
	return m_lColumnContent[lColumn];
}


// 行方向のズーム倍率を取得
long CTrackListFrame::GetRowZoom () {
	return m_lRowZoom;
}

// 列方向のズーム倍率を取得
long CTrackListFrame::GetColumnZoom () {
	return m_lColumnZoom;
}

// 時間方向のズーム倍率を取得
long CTrackListFrame::GetTimeZoom () {
	return m_lTimeZoom;
}

// 指定の列の左座標を取得
long CTrackListFrame::GetColumnLeft (long lColumn) {
	long lSum = 0;
	for (long j = 0; j < lColumn; j++) {
		lSum += m_lColumnBaseWidth[j];
	}
	return lSum * m_lColumnZoom;
}

// 指定の列の幅を取得
long CTrackListFrame::GetColumnWidth (long lColumn) {
	return m_lColumnBaseWidth[lColumn] * m_lColumnZoom;
}

// y座標をトラックに変換
long CTrackListFrame::YtoRow (long y) {
	return y / m_lRowZoom;
}

// トラックをy座標に変換
long CTrackListFrame::RowtoY (long lTrack) {
	return m_lRowZoom * lTrack;
}

// x座標からモードを取得
long CTrackListFrame::XtoColumn (long x) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	long lSum = 0;
	long lNextSum = 0;
	for (long j = 0; j < TRACKLISTFRAME_NUMCOLUMN; j++) {		
		lNextSum += m_lColumnBaseWidth[j] * m_lColumnZoom;
		if (lSum <= x && x < lNextSum) {
			return j;
		}
		lSum = lNextSum;
	}
	return -1;
}

// モードをx座標に変換
long CTrackListFrame::ColumntoX (long lMode) {
	return GetColumnLeft (lMode);
}

// x座標からタイムを取得
long CTrackListFrame::XtoTime (long x) {
	long lTimeResolution = MIDIData_GetTimeResolution (GetDocument ()->m_pMIDIData);
	return x * lTimeResolution / m_lTimeZoom;
}

// タイムをx座標に変換
long CTrackListFrame::TimetoX (long lTime) {
	long lTimeResolution = MIDIData_GetTimeResolution (GetDocument ()->m_pMIDIData);
	return lTime * m_lTimeZoom / lTimeResolution;
}





// 最も上に表示されている行を取得
long CTrackListFrame::GetVisibleTopRow () {
	return m_lRowScrollPos / m_lRowZoom;
}

// 最も下に表示されている行を取得
long CTrackListFrame::GetVisibleBottomRow () {
	CRect rcClient;
	m_pTrackScaleView->GetClientRect (&rcClient);
	return (m_lRowScrollPos + rcClient.Height ()) / m_lRowZoom;
}

// 表示されているタイムの左端を計算
long CTrackListFrame::GetVisibleLeftTime () {
	long lTimeResolution = MIDIData_GetTimeResolution (GetDocument ()->m_pMIDIData);
	return m_lTimeScrollPos * lTimeResolution / m_lTimeZoom;
}

// 表示されているタイムの右端を計算
long CTrackListFrame::GetVisibleRightTime () {
	CRect rcClient;
	m_pTimeScaleView->GetClientRect (&rcClient);
	long lTimeResolution = MIDIData_GetTimeResolution (GetDocument ()->m_pMIDIData);
	return (m_lTimeScrollPos + rcClient.Width ()) * lTimeResolution / m_lTimeZoom;
}


// 時間方向のスクロールポジションを取得
long CTrackListFrame::GetTimeScrollPos () {
	return m_lTimeScrollPos;
}

// 行方向のスクロールポジションを取得
long CTrackListFrame::GetRowScrollPos () {
	return m_lRowScrollPos;
}

// 列方向のスクロールポジションを取得
long CTrackListFrame::GetColumnScrollPos () {
	return m_lColumnScrollPos;
}

// 時間方向のスクロールポジション設定
long CTrackListFrame::SetTimeScrollPos (long lTimeScrollPos) {
	long lOldTimeScrollPos = m_lTimeScrollPos;
	m_wndTimeScroll.SetScrollPos (lTimeScrollPos);
	m_lTimeScrollPos = m_wndTimeScroll.GetScrollPos ();
	long lDeltaTimeScrollPos = m_lTimeScrollPos - lOldTimeScrollPos;
	//((CTrackListTrackTimeView*)m_pTrackTimeView)->m_bTimeScrolled = TRUE;
	m_pTimeScaleView->ScrollWindow (-lDeltaTimeScrollPos, 0);
	m_pTrackTimeView->ScrollWindow (-lDeltaTimeScrollPos, 0);
	m_pTimeScaleView->UpdateWindow ();
	m_pTrackTimeView->UpdateWindow ();
	return m_lTimeScrollPos;
}

// 列方向のスクロールポジション設定
long CTrackListFrame::SetColumnScrollPos (long lColumnScrollPos) {
	long lOldColumnScrollPos = m_lColumnScrollPos;
	m_wndColumnScroll.SetScrollPos (lColumnScrollPos);
	m_lColumnScrollPos = m_wndColumnScroll.GetScrollPos ();
	long lDeltaColumnScrollPos = m_lColumnScrollPos - lOldColumnScrollPos; //20080823修正
	m_pModeScaleView->ScrollWindow (-lDeltaColumnScrollPos, 0);
	m_pTrackModeView->ScrollWindow (-lDeltaColumnScrollPos, 0);
	return m_lColumnScrollPos;
}

// 行方向のスクロールポジション設定
long CTrackListFrame::SetRowScrollPos (long lRowScrollPos) {
	long lOldRowScrollPos = m_lRowScrollPos;
	m_wndRowScroll.SetScrollPos (lRowScrollPos);
	m_lRowScrollPos = m_wndRowScroll.GetScrollPos ();
	long lDeltaRowScrollPos = m_lRowScrollPos - lOldRowScrollPos;
	m_pTrackScaleView->ScrollWindow (0, -lDeltaRowScrollPos);
	m_pTrackModeView->ScrollWindow (0, -lDeltaRowScrollPos);
	m_pTrackTimeView->ScrollWindow (0, -lDeltaRowScrollPos);
	return m_lRowScrollPos;
}

// スプリッターキャプターの描画
void CTrackListFrame::DrawSplitterCaptor (CDC* pDC, CPoint pt) {
	CRect rcClient;
	GetClientRect (&rcClient);
	CPen pen;
	CPen* pOldPen;
	pen.CreatePen (PS_SOLID, 4, ::GetSysColor(COLOR_BTNSHADOW));
	pDC->SetROP2 (R2_XORPEN);
	pOldPen = pDC->SelectObject (&pen);
	pDC->MoveTo (pt.x, 0);
	pDC->LineTo (pt.x, rcClient.Height());
	pDC->SelectObject (pOldPen);
}

// 行スクロールバー(縦)のデザイン設定
void CTrackListFrame::RecalcRowScrollInfo () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lTrackCount = 17;
	if (pSekaijuDoc) {
		pSekaijuDoc->m_theCriticalSection.Lock ();
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		if (pMIDIData) {
			lTrackCount = MIDIData_CountTrack (pMIDIData);
		}
		pSekaijuDoc->m_theCriticalSection.Unlock ();
	}
	SCROLLINFO si;
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	si.nMax = CLIP (17, lTrackCount + 16, MAXMIDITRACKNUM - 1) * m_lRowZoom;
	si.nPage = m_lTrackHeight;
	m_wndRowScroll.SetScrollInfo (&si, TRUE);
	m_lRowScrollPos = m_wndRowScroll.GetScrollPos ();
}

// 列スクロールバー(縦)のデザイン設定
void CTrackListFrame::RecalcColumnScrollInfo () {
	SCROLLINFO si;
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	si.nMax = 0;
	for (int i = 0; i < TRACKLISTFRAME_NUMCOLUMN; i++) {
		si.nMax += m_lColumnBaseWidth[i];
	}
	si.nMax *= m_lColumnZoom;
	si.nPage = m_lModeWidth;
	m_wndColumnScroll.SetScrollInfo (&si, TRUE);
	m_lColumnScrollPos = m_wndColumnScroll.GetScrollPos ();
}

// タイムスクロールバー(横)のデザイン設定
void CTrackListFrame::RecalcTimeScrollInfo () {
	long lTimeResolution = 120;
	long lEndTime = 0;
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	if (pSekaijuDoc) {
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		pSekaijuDoc->m_theCriticalSection.Lock ();
		if (pMIDIData) {
			lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
			lEndTime = MIDIData_GetEndTime (pMIDIData);
		}
		pSekaijuDoc->m_theCriticalSection.Unlock ();
	}
	long lFeedTime = lTimeResolution * 4 * 120;
	SCROLLINFO si;
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	si.nMax = (lEndTime + lFeedTime) * m_lTimeZoom / lTimeResolution;
	si.nMax = MIN (si.nMax, 0x7FFFFFFF);
	si.nPage = m_lTimeWidth;
	m_wndTimeScroll.SetScrollInfo (&si, TRUE);
	m_lTimeScrollPos = m_wndTimeScroll.GetScrollPos ();
	// 注：ズーム倍率1倍のとき4分音符の長さを1ピクセルと定義している。
}



//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// ウィンドウ生成直前の構造体設定
BOOL CTrackListFrame::PreCreateWindow (CREATESTRUCT& cs) {
	return  (CWnd::PreCreateWindow(cs));
}

// 背景消去(CFrameWnd::OnEraseBkgndのオーバーライド)
BOOL CTrackListFrame::OnEraseBkgnd (CDC* pDC) {
	return 0;
}

// ウィンドウタイトルの自動設定(CMDIChildWnd::OnUpdateFrameTitleのオーバーライド)
void CTrackListFrame::OnUpdateFrameTitle (BOOL bAddToTitle) {
	// update our parent window first
  	GetMDIFrame()->OnUpdateFrameTitle (bAddToTitle);
	if ((GetStyle() & FWS_ADDTOTITLE) == 0) {
		return;     // leave child window alone!
	}
	CDocument* pDocument = GetActiveDocument();
	if (bAddToTitle && pDocument != NULL) {
		CString strTrackList;
		strTrackList.LoadString (IDS_TRACKLIST);
		CString strTitle;
		if (m_nWindow > 0) {
			strTitle.Format (_T("%s:%d(%s)"), pDocument->GetTitle (), m_nWindow, strTrackList);
		}
		else {
			strTitle.Format (_T("%s(%s)"), pDocument->GetTitle (), strTrackList);
		}
		this->SetWindowText (strTitle);
	}
}

// 再配置用関数(CFrameWnd::RecalcLayoutのオーバーライド)
void CTrackListFrame::RecalcLayout (BOOL bNotify) {

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
	if (rcClient.Height () < 
		m_lToolBar1Height +
		TRACKLISTFRAME_SCALEHEIGHT + m_lHScrollBarHeight +
		TRACKLISTFRAME_BORDERHEIGHT * 2) {
		m_lScaleHeight = rcClient.Height () - m_lToolBar1Height - m_lHScrollBarHeight -
			TRACKLISTFRAME_BORDERHEIGHT * 2;
		m_lTrackHeight = 0;
	}
	else {
		m_lScaleHeight = TRACKLISTFRAME_SCALEHEIGHT;
		m_lTrackHeight = rcClient.Height () - m_lToolBar1Height - m_lScaleHeight - 
			m_lHScrollBarHeight - TRACKLISTFRAME_BORDERHEIGHT * 2;
	}

	// 幅方向の位置計算
	if (rcClient.Width () <
		TRACKLISTFRAME_SCALEWIDTH + m_lVScrollBarWidth + 
		TRACKLISTFRAME_BORDERWIDTH * 4 + TRACKLISTFRAME_SPLITTERWIDTH) {
		m_lScaleWidth = rcClient.Width () - TRACKLISTFRAME_SPLITTERWIDTH -
			m_lVScrollBarWidth - TRACKLISTFRAME_BORDERWIDTH * 4;
		m_lModeWidth = 0;
		m_lTimeWidth = 0;
	}
	else if (rcClient.Width () < 
		TRACKLISTFRAME_SCALEWIDTH + m_lVScrollBarWidth + m_lModeWidth +
		TRACKLISTFRAME_BORDERWIDTH * 4 + TRACKLISTFRAME_SPLITTERWIDTH +
		m_lVScrollBarWidth) {
		m_lScaleWidth = TRACKLISTFRAME_SCALEWIDTH;
		m_lModeWidth = rcClient.Width () - TRACKLISTFRAME_SPLITTERWIDTH - 
			m_lScaleWidth - m_lVScrollBarWidth - TRACKLISTFRAME_BORDERWIDTH * 4;
		m_lTimeWidth = 0;

	}
	else {
		//m_lScaleWidth  = TRACKLISTFRAME_SCALEWIDTH;
		//m_lModeWidth = TRACKLISTFRAME_MODEWIDTH;
		m_lTimeWidth = rcClient.Width () - m_lScaleWidth - m_lModeWidth - 
			TRACKLISTFRAME_SPLITTERWIDTH -
			m_lVScrollBarWidth - TRACKLISTFRAME_BORDERWIDTH * 4;
	}

	// ビューの整列
	if (m_pScaleView) {
		m_pScaleView->MoveWindow (TRACKLISTFRAME_BORDERWIDTH, 
			m_lToolBar1Height + TRACKLISTFRAME_BORDERHEIGHT, 
			m_lScaleWidth, m_lScaleHeight);
	}

	if (m_pModeScaleView) {
		m_pModeScaleView->MoveWindow (TRACKLISTFRAME_BORDERWIDTH + m_lScaleWidth, 
			m_lToolBar1Height + TRACKLISTFRAME_BORDERHEIGHT, 
			m_lModeWidth, m_lScaleHeight);
	}

	if (m_pTimeScaleView) {
		m_pTimeScaleView->MoveWindow (TRACKLISTFRAME_BORDERWIDTH * 3 + m_lScaleWidth + 
			m_lModeWidth + TRACKLISTFRAME_SPLITTERWIDTH, 
			m_lToolBar1Height + TRACKLISTFRAME_BORDERHEIGHT, 
			m_lTimeWidth, m_lScaleHeight);
	}

	if (m_pTrackScaleView) {
		m_pTrackScaleView->MoveWindow (TRACKLISTFRAME_BORDERWIDTH,
			m_lToolBar1Height + TRACKLISTFRAME_BORDERHEIGHT + m_lScaleHeight, 
			m_lScaleWidth, m_lTrackHeight);
	}

	if (m_pTrackModeView) {
		m_pTrackModeView->MoveWindow (TRACKLISTFRAME_BORDERWIDTH + m_lScaleWidth, 
			m_lToolBar1Height + TRACKLISTFRAME_BORDERHEIGHT + m_lScaleHeight, 
			m_lModeWidth, m_lTrackHeight);
	}

	if (m_pTrackTimeView) {
		m_pTrackTimeView->MoveWindow (TRACKLISTFRAME_BORDERWIDTH * 3 + m_lScaleWidth +
			 m_lModeWidth + TRACKLISTFRAME_SPLITTERWIDTH, 
			 m_lToolBar1Height + TRACKLISTFRAME_BORDERHEIGHT + m_lScaleHeight, 
			 m_lTimeWidth, m_lTrackHeight);
	}
	// スクロールバーの整列
	m_wndColumnScroll.MoveWindow (TRACKLISTFRAME_BORDERWIDTH,
		m_lToolBar1Height + TRACKLISTFRAME_BORDERHEIGHT + m_lScaleHeight + m_lTrackHeight, 
		m_lScaleWidth + m_lModeWidth - m_lVScrollBarWidth * 2, 
		m_lHScrollBarHeight);

	m_wndTimeScroll.MoveWindow (TRACKLISTFRAME_BORDERWIDTH * 3 + m_lScaleWidth +
		m_lModeWidth + TRACKLISTFRAME_SPLITTERWIDTH, 
		m_lToolBar1Height + TRACKLISTFRAME_BORDERHEIGHT + m_lScaleHeight + m_lTrackHeight,
		m_lTimeWidth - m_lVScrollBarWidth * 2, 
		m_lHScrollBarHeight);

	m_wndRowScroll.MoveWindow (TRACKLISTFRAME_BORDERWIDTH * 3 + m_lScaleWidth +
		m_lModeWidth + m_lTimeWidth + TRACKLISTFRAME_SPLITTERWIDTH,
		m_lToolBar1Height + TRACKLISTFRAME_BORDERHEIGHT,
		m_lVScrollBarWidth,
		m_lScaleHeight + m_lTrackHeight - m_lHScrollBarHeight * 2);

	m_wndSizeScroll.MoveWindow (TRACKLISTFRAME_BORDERWIDTH * 3 + m_lScaleWidth +
		m_lModeWidth + m_lTimeWidth + TRACKLISTFRAME_SPLITTERWIDTH,
		m_lToolBar1Height + TRACKLISTFRAME_BORDERHEIGHT + m_lScaleHeight + m_lTrackHeight,
		m_lVScrollBarWidth, m_lHScrollBarHeight);

	// ズームボタンの整列
	m_wndColumnZoomDown.MoveWindow (TRACKLISTFRAME_BORDERWIDTH + m_lScaleWidth +
		m_lModeWidth - m_lVScrollBarWidth * 2, 
		m_lToolBar1Height + TRACKLISTFRAME_BORDERHEIGHT + m_lScaleHeight + m_lTrackHeight,
		m_lVScrollBarWidth, m_lHScrollBarHeight);
	
	m_wndColumnZoomUp.MoveWindow (TRACKLISTFRAME_BORDERWIDTH + m_lScaleWidth +
		m_lModeWidth - m_lVScrollBarWidth, 
		m_lToolBar1Height + TRACKLISTFRAME_BORDERHEIGHT + m_lScaleHeight + m_lTrackHeight,
		m_lVScrollBarWidth, m_lHScrollBarHeight);
	
	m_wndTimeZoomDown.MoveWindow (TRACKLISTFRAME_BORDERWIDTH * 3 + m_lScaleWidth +
		m_lModeWidth + TRACKLISTFRAME_SPLITTERWIDTH + m_lTimeWidth - 
		m_lVScrollBarWidth * 2, 
		m_lToolBar1Height + TRACKLISTFRAME_BORDERHEIGHT + m_lScaleHeight + m_lTrackHeight,
		m_lVScrollBarWidth, m_lHScrollBarHeight);
	
	m_wndTimeZoomUp.MoveWindow (TRACKLISTFRAME_BORDERWIDTH * 3 + m_lScaleWidth +
		m_lModeWidth + TRACKLISTFRAME_SPLITTERWIDTH + m_lTimeWidth - 
		m_lVScrollBarWidth, 
		m_lToolBar1Height + TRACKLISTFRAME_BORDERHEIGHT + m_lScaleHeight + m_lTrackHeight,
		m_lVScrollBarWidth, m_lHScrollBarHeight);

	m_wndRowZoomDown.MoveWindow
		(TRACKLISTFRAME_BORDERWIDTH * 3 + m_lScaleWidth + m_lModeWidth +
		m_lTimeWidth + TRACKLISTFRAME_SPLITTERWIDTH, 
		m_lToolBar1Height + TRACKLISTFRAME_BORDERHEIGHT + m_lScaleHeight + m_lTrackHeight -
		m_lHScrollBarHeight * 2, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);

	m_wndRowZoomUp.MoveWindow
		(TRACKLISTFRAME_BORDERWIDTH * 3 + m_lScaleWidth + m_lModeWidth + 
		m_lTimeWidth + TRACKLISTFRAME_SPLITTERWIDTH, 
		m_lToolBar1Height + TRACKLISTFRAME_BORDERHEIGHT + m_lScaleHeight + m_lTrackHeight -
		m_lHScrollBarHeight, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);

	// スクロールバーのサイズが変化したので、バーのデザインを再調整する。
	RecalcRowScrollInfo ();
	RecalcColumnScrollInfo ();
	RecalcTimeScrollInfo ();

}

// クライアント領域の生成(CFrameWnd::OnCreateClientのオーバーライド)
BOOL CTrackListFrame::OnCreateClient (LPCREATESTRUCT lpcs, CCreateContext* pContext) {

	// サイズ調整用のダミービュー生成(Visible = FALSE)
	CWnd* pWnd = NULL;
	pContext->m_pNewViewClass = RUNTIME_CLASS (CSekaijuView);
	m_pDummyView = (CView*)CFrameWnd::CreateView (pContext, TRACKLISTFRAME_DUMMYVIEW);
	if (m_pDummyView == NULL) {
			return FALSE;
	}
	m_pDummyView->ShowWindow (SW_HIDE);
	
	// 印刷ビューの生成(Visible = FALSE)
	pContext->m_pNewViewClass = RUNTIME_CLASS (CTrackListPrintView);
	m_pPrintView = (CView*)CFrameWnd::CreateView (pContext, TRACKLISTFRAME_PRINTVIEW);
	if (m_pPrintView == NULL) {
			return FALSE;
	}
	m_pDummyView->ShowWindow (SW_HIDE);

	// スケールビューの生成
	pContext->m_pNewViewClass = RUNTIME_CLASS (CTrackListScaleView);
	m_pScaleView = (CView*)CFrameWnd::CreateView (pContext, TRACKLISTFRAME_SCALEVIEW);
	if (m_pScaleView == NULL) {
			return FALSE;
	}
	m_pScaleView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);

	// モードスケールビューの生成
	pContext->m_pNewViewClass = RUNTIME_CLASS (CTrackListModeScaleView);
	m_pModeScaleView = (CView*)CFrameWnd::CreateView (pContext, TRACKLISTFRAME_MODESCALEVIEW);
	if (m_pModeScaleView == NULL) {
			return FALSE;
	}
	m_pModeScaleView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);
	
	// タイムスケールビューの生成
	pContext->m_pNewViewClass = RUNTIME_CLASS (CTrackListTimeScaleView);
	m_pTimeScaleView = (CView*)CFrameWnd::CreateView (pContext, TRACKLISTFRAME_TIMESCALEVIEW);
	if (m_pTimeScaleView == NULL) {
			return FALSE;
	}
	m_pTimeScaleView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);
	
	// トラックスケールビューの生成
	pContext->m_pNewViewClass = RUNTIME_CLASS (CTrackListTrackScaleView);
	m_pTrackScaleView = (CView*)CFrameWnd::CreateView (pContext, TRACKLISTFRAME_TRACKSCALEVIEW);
	if (m_pTrackScaleView == NULL) {
			return FALSE;
	}
	m_pTrackScaleView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);
	
	// トラックモードビューの生成
	pContext->m_pNewViewClass = RUNTIME_CLASS (CTrackListTrackModeView);
	m_pTrackModeView = (CView*)CFrameWnd::CreateView (pContext, TRACKLISTFRAME_TRACKMODEVIEW);
	if (m_pTrackModeView == NULL) {
			return FALSE;
	}
	m_pTrackModeView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);
	
	// トラックタイムビューの生成
	pContext->m_pNewViewClass = RUNTIME_CLASS (CTrackListTrackTimeView);
	m_pTrackTimeView = (CView*)CFrameWnd::CreateView (pContext, TRACKLISTFRAME_TRACKTIMEVIEW);
	if (m_pTrackTimeView == NULL) {
			return FALSE;
	}
	m_pTrackTimeView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);

	// スクロールバーの生成
	m_wndColumnScroll.Create   
		(WS_CHILD|WS_VISIBLE|SBS_HORZ, CRect(0,0,0,0), this, TRACKLISTFRAME_COLUMNSCROLL);
	m_wndTimeScroll.Create    
		(WS_CHILD|WS_VISIBLE|SBS_HORZ, CRect(0,0,0,0), this, TRACKLISTFRAME_TIMESCROLL);
	m_wndRowScroll.Create    
		(WS_CHILD|WS_VISIBLE|SBS_VERT, CRect(0,0,0,0), this, TRACKLISTFRAME_ROWSCROLL);
	m_wndSizeScroll.Create   
		(WS_CHILD|WS_VISIBLE|SBS_SIZEBOX, CRect(0,0,0,0), this, TRACKLISTFRAME_SIZEBOX);
	
	// ズームボタン類の生成
	m_wndColumnZoomDown.Create (_T("-"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, TRACKLISTFRAME_COLUMNZOOMDOWN);
	m_wndColumnZoomUp.Create   (_T("+"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, TRACKLISTFRAME_COLUMNZOOMUP);
	m_wndTimeZoomDown.Create (_T("-"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, TRACKLISTFRAME_TIMEZOOMDOWN);
	m_wndTimeZoomUp.Create   (_T("+"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, TRACKLISTFRAME_TIMEZOOMUP);
	m_wndRowZoomDown.Create (_T("-"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, TRACKLISTFRAME_ROWZOOMDOWN);
	m_wndRowZoomUp.Create  (_T("+"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, TRACKLISTFRAME_ROWZOOMUP);
	
	// コントロールの位置合わせはWM_SIZEなどによるRecalcLaoyoutに任せる。

	// フォーカスの設定
	SetActiveView (m_pTrackModeView);

	return TRUE;
}

// 印刷用のコマンドをトラップ(CFrameWnd::OnCmdMsgのオーバーライド)
BOOL CTrackListFrame::OnCmdMsg (UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	// 印刷用のコマンドの場合、強制的にCTrackListPrintViewに渡す。
	if ((nID == ID_FILE_PRINT || nID == ID_FILE_PRINT_DIRECT || nID == ID_FILE_PRINT_PREVIEW) &&
		pSekaijuApp->m_bRecording == FALSE) {
		if (m_pPrintView) {
			return ((CTrackListPrintView*)m_pPrintView)->OnCmdMsg (nID, nCode, pExtra, pHandlerInfo);
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
int CTrackListFrame::OnCreate (LPCREATESTRUCT lpCreateStruct) {
	CRect rcTemp;

	// ツールバー1の作成
	if (!m_wndToolBar1.Create (this) ||
		!m_wndToolBar1.LoadToolBar (IDR_TRACKLIST1)) {
		TRACE0 ("Failed to create toolbar\n");
		return -1;
	}
	m_wndToolBar1.SetBarStyle (m_wndToolBar1.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	//m_wndToolBar1.EnableDocking (CBRS_ALIGN_ANY);
	//EnableDocking (CBRS_ALIGN_ANY);
	//DockControlBar (&m_wndToolBar1);

	LoadAccelTable (MAKEINTRESOURCE (IDR_TRACKLIST));

	// 親クラスの関数呼び出し
	int nRet = CChildFrame::OnCreate (lpCreateStruct);	

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();

	// 自動ページ更新の設定
	if (pSekaijuApp->m_theGeneralOption.m_bEnableAutoPageUpdate) {
		m_bAutoPageUpdate = TRUE;
	}

	SetActiveView (m_pTrackModeView, FALSE);
	m_pTrackModeView->SetFocus ();
	return nRet;

}

// ウィンドウ破棄時
void CTrackListFrame::OnDestroy () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	CChildFrame::OnDestroy ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// ウィンドウサイズ変更時
void CTrackListFrame::OnSize (UINT nType, int cx, int cy) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	CChildFrame::OnSize (nType, cx, cy);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// タイマー呼び出し時
void CTrackListFrame::OnTimer (UINT nIDEvent) {
	;
}

// 描画するとき
void CTrackListFrame::OnPaint () {
	CPaintDC dc (this);
	CRect rcClient;
	GetClientRect (&rcClient);
	// 上側領域のくぼみ描画
	CRect rcClient1 (rcClient);
	rcClient1.top = m_lToolBar1Height;
	rcClient1.right = TRACKLISTFRAME_BORDERWIDTH * 2 + m_lScaleWidth + m_lModeWidth;
	dc.Draw3dRect (&rcClient1, RGB (128, 128, 128), RGB (255, 255, 255));
	rcClient1.InflateRect (-1, -1);
	dc.Draw3dRect (&rcClient1, RGB (0, 0, 0), RGB (192, 192, 192));
	// 下側領域のくぼみ描画
	CRect rcClient2 (rcClient);
	rcClient2.top = m_lToolBar1Height;
	rcClient2.left = TRACKLISTFRAME_BORDERWIDTH * 2 + m_lScaleWidth + m_lModeWidth + TRACKLISTFRAME_SPLITTERWIDTH;
	dc.Draw3dRect (&rcClient2, RGB (128, 128, 128), RGB (255, 255, 255));
	rcClient2.InflateRect (-1, -1);
	dc.Draw3dRect (&rcClient2, RGB (0, 0, 0), RGB (192, 192, 192));
	// 境界部分の描画
	CRect rcClient3 (rcClient);
	rcClient3.top = m_lToolBar1Height;
	rcClient3.left = rcClient1.right + 1;
	rcClient3.right = rcClient2.left - 1;
	CBrush brush;
	brush.CreateSolidBrush (::GetSysColor (COLOR_3DFACE));
	dc.FillRect (&rcClient3, &brush);
}

// ウィンドウがアクティブになったとき
void CTrackListFrame::OnMDIActivate (BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) {
	CChildFrame::OnMDIActivate (bActivate, pActivateWnd, pDeactivateWnd);
}

// 閉じるボタンが押されたとき
void CTrackListFrame::OnClose () {
	_RPTF0 (_CRT_WARN, ("CTrackListFrame::OnClose ()\n"));
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	// インプレースエディット及びインプレースリストボックスの終了
	if (m_pTrackModeView) {
		CTrackListTrackModeView* pTrackModeView = (CTrackListTrackModeView*)m_pTrackModeView;
		if (pTrackModeView->IsTextEditing ()) {
			pTrackModeView->EndTextEditingOK ();
			pSekaijuDoc->SetModifiedFlag (TRUE);
			pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED | SEKAIJUDOC_MIDITRACKCHANGED);
		}
		//if (pTrackModeView->IsListSelecting ()) {
		//	pTrackModeView->EndListSelectingOK ();
		//	pSekaijuDoc->SetModifiedFlag (TRUE);
		//	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED | SEKAIJUDOC_MIDITRACKCHANGED);
		//}
	}
	CChildFrame::OnClose ();
}

// マウス左ボタン押された時  
void CTrackListFrame::OnLButtonDown (UINT nFlags, CPoint point) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (TRACKLISTFRAME_BORDERWIDTH * 2 + m_lScaleWidth + m_lModeWidth <= point.x &&
		point.x < TRACKLISTFRAME_BORDERHEIGHT * 2 + m_lScaleWidth + m_lModeWidth + 
		TRACKLISTFRAME_SPLITTERWIDTH) {
		SetCapture ();
		m_ptMouseDown = m_ptMouseMoveOld = point;
		CDC* pDC = GetDC ();
		DrawSplitterCaptor (pDC, point);
		ReleaseDC (pDC);
		::SetCursor (pSekaijuApp->m_hCursorResizeWE);
	}
}

// マウス右ボタン押された時  
void CTrackListFrame::OnRButtonDown (UINT nFlags, CPoint point) {


}

// マウス左ボタン離されたとき
void CTrackListFrame::OnLButtonUp (UINT nFlags, CPoint point) {
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
		m_lScaleWidth = CLIP (0, point.x, TRACKLISTFRAME_SCALEWIDTH);
		m_lModeWidth = point.x - m_lScaleWidth;
		RecalcLayout ();
		Invalidate ();
		pSekaijuDoc->m_theCriticalSection.Unlock ();
	}
}

// マウス右ボタン離されたとき
void CTrackListFrame::OnRButtonUp (UINT nFlags, CPoint point) {


}

// マウスが動かされたとき
void CTrackListFrame::OnMouseMove (UINT nFlags, CPoint point) {
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
		// カーソルがスプリッターの上にある
		if (TRACKLISTFRAME_BORDERWIDTH * 2 + m_lScaleWidth + m_lModeWidth <= point.x &&
			point.x < TRACKLISTFRAME_BORDERWIDTH * 2 + m_lScaleWidth + m_lModeWidth + 
			TRACKLISTFRAME_SPLITTERWIDTH) {
			::SetCursor (pSekaijuApp->m_hCursorResizeWE);
		}
		// カーソルがスプリッターの上にない
		else {
			::SetCursor (pSekaijuApp->m_hCursorArrow);
		}
	}
}











// 列方向ズームダウン(20091220:左端位置保持機能追加)
void CTrackListFrame::OnColumnZoomDown () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lOldColumnZoom = m_lColumnZoom;
	long lOldColumnPos = m_wndColumnScroll.GetScrollPos ();
	long lNewColumnZoom = CLIP (2, m_lColumnZoom - 1, 16);
	long lNewColumnPos = lOldColumnPos * lNewColumnZoom / lOldColumnZoom;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_lColumnZoom = lNewColumnZoom;
	RecalcColumnScrollInfo ();
	m_wndColumnScroll.SetScrollPos (lNewColumnPos);
	m_lColumnScrollPos = m_wndColumnScroll.GetScrollPos ();
	m_pModeScaleView->Invalidate ();
	m_pTrackModeView->Invalidate ();
	m_pTrackModeView->SetFocus ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// 列方向ズームアップ(20091220:左端位置保持機能追加)
void CTrackListFrame::OnColumnZoomUp () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lOldColumnZoom = m_lColumnZoom;
	long lOldColumnPos = m_wndColumnScroll.GetScrollPos ();
	long lNewColumnZoom = CLIP (2, m_lColumnZoom + 1, 16);
	long lNewColumnPos = lOldColumnPos * lNewColumnZoom / lOldColumnZoom;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_lColumnZoom = lNewColumnZoom;
	RecalcColumnScrollInfo ();
	m_wndColumnScroll.SetScrollPos (lNewColumnPos);
	m_lColumnScrollPos = m_wndColumnScroll.GetScrollPos ();
	m_pModeScaleView->Invalidate ();
	m_pTrackModeView->Invalidate ();
	m_pTrackModeView->SetFocus ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// 時間方向ズームダウン(20091220:左端位置保持機能追加、自動ページ更新自動オフ追加)
void CTrackListFrame::OnTimeZoomDown () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lOldTimeZoom = m_lTimeZoom;
	long lOldTimePos = m_wndTimeScroll.GetScrollPos ();
	long lNewTimeZoom = CLIP (2, m_lTimeZoom - 2, 16);
	long lNewTimePos = lOldTimePos * lNewTimeZoom / lOldTimeZoom;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_bAutoPageUpdate = FALSE;
	m_lTimeZoom = lNewTimeZoom;
	RecalcTimeScrollInfo ();
	m_wndTimeScroll.SetScrollPos (lNewTimePos);
	m_lTimeScrollPos = m_wndTimeScroll.GetScrollPos ();
	m_pTimeScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
	//m_bAutoPageUpdate = TRUE;
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}


// 時間方向ズームアップ(20091220:左端位置保持機能追加、自動ページ更新自動オフ追加)
void CTrackListFrame::OnTimeZoomUp () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lOldTimeZoom = m_lTimeZoom;
	long lOldTimePos = m_wndTimeScroll.GetScrollPos ();
	long lNewTimeZoom = CLIP (2, m_lTimeZoom + 2, 16);
	long lNewTimePos = lOldTimePos * lNewTimeZoom / lOldTimeZoom;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_bAutoPageUpdate = FALSE;
	m_lTimeZoom = lNewTimeZoom;
	RecalcTimeScrollInfo ();
	m_wndTimeScroll.SetScrollPos (lNewTimePos);
	m_lTimeScrollPos = m_wndTimeScroll.GetScrollPos ();
	m_pTimeScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
	//m_bAutoPageUpdate = TRUE;
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// 行方向ズームダウン(20091220:上端位置保持機能追加)
void CTrackListFrame::OnRowZoomDown () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lOldRowZoom = m_lRowZoom;
	long lOldRowPos = m_wndRowScroll.GetScrollPos ();
	long lNewRowZoom = CLIP (16, m_lRowZoom - 8, 64);
	long lNewRowPos = lOldRowPos * lNewRowZoom / lOldRowZoom;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_lRowZoom = lNewRowZoom;
	RecalcRowScrollInfo ();
	m_wndRowScroll.SetScrollPos (lNewRowPos);
	m_lRowScrollPos = m_wndRowScroll.GetScrollPos ();
	m_pTrackScaleView->Invalidate ();
	m_pTrackModeView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// 行方向ズームアップ(20091220:上端位置保持機能追加)
void CTrackListFrame::OnRowZoomUp () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lOldRowZoom = m_lRowZoom;
	long lOldRowPos = m_wndRowScroll.GetScrollPos ();
	long lNewRowZoom = CLIP (16, m_lRowZoom + 8, 64);
	long lNewRowPos = lOldRowPos * lNewRowZoom / lOldRowZoom;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_lRowZoom = lNewRowZoom;
	RecalcRowScrollInfo ();
	m_wndRowScroll.SetScrollPos (lNewRowPos);
	m_lRowScrollPos = m_wndRowScroll.GetScrollPos ();
	m_pTrackScaleView->Invalidate ();
	m_pTrackModeView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// 水平スクロール
void CTrackListFrame::OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	if (pScrollBar == &m_wndColumnScroll) {
		int nMin = 0;
		int nMax = 0;
		pScrollBar->GetScrollRange (&nMin, &nMax);
		long lNewPos = m_lColumnScrollPos;
		switch (nSBCode) {
		case SB_LINELEFT:
			lNewPos = m_lColumnScrollPos - m_lColumnZoom;
			break;
		case SB_LINERIGHT:
			lNewPos = m_lColumnScrollPos + m_lColumnZoom;
			break;
		case SB_PAGELEFT:
			lNewPos = m_lColumnScrollPos - m_lColumnZoom * 4;
			break;
		case SB_PAGERIGHT:
			lNewPos = m_lColumnScrollPos + m_lColumnZoom * 4;
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
		SetColumnScrollPos (CLIP (0, lNewPos, 0x7FFFFFFF));
		m_pTrackModeView->SetFocus ();
	}
	else if (pScrollBar == &m_wndTimeScroll) {
		int nMin = 0;
		int nMax = 0;
		pScrollBar->GetScrollRange (&nMin, &nMax);
		long lNewPos = m_lTimeScrollPos;
		switch (nSBCode) {
		case SB_LINELEFT:
			lNewPos = m_lTimeScrollPos - m_lTimeZoom * 2;
			break;
		case SB_LINERIGHT:
			lNewPos = m_lTimeScrollPos + m_lTimeZoom * 2;
			break;
		case SB_PAGELEFT:
			lNewPos = m_lTimeScrollPos - m_lTimeZoom * 16;
			break;
		case SB_PAGERIGHT:
			lNewPos = m_lTimeScrollPos + m_lTimeZoom * 16;
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
		SetTimeScrollPos (CLIP (0, lNewPos, 0x7FFFFFFF));
		m_bAutoPageUpdate = FALSE;
		m_pTrackTimeView->SetFocus ();
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// 垂直スクロール
void CTrackListFrame::OnVScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	if (pScrollBar == &m_wndRowScroll) {
		int nMin = 0;
		int nMax = 0;
		pScrollBar->GetScrollRange (&nMin, &nMax);
		long lNewPos = m_lRowScrollPos;
		switch (nSBCode) {
		case SB_LINEDOWN:
			lNewPos = m_lRowScrollPos + m_lRowZoom;
			break;
		case SB_LINEUP:
			lNewPos = m_lRowScrollPos - m_lRowZoom;
			break;
		case SB_PAGEDOWN:
			lNewPos = m_lRowScrollPos + m_lRowZoom * 12;
			break;
		case SB_PAGEUP:
			lNewPos = m_lRowScrollPos - m_lRowZoom * 12;
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
		SetRowScrollPos (CLIP (0, lNewPos, 0x7FFFFFFF));
		m_pTrackModeView->SetFocus ();
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}



// 『トラックを1行挿入』
void CTrackListFrame::OnTrackListInsertTrack () {
	_RPTF0 (_CRT_WARN, "OnTrackListInsertTrack\n");
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	// リアルタイム入力中は何もしない。
	if (pSekaijuApp->m_bRecording) {
		return;
	}
	// MIDIデータが編集ロックされている場合は何もしない。
	if (pSekaijuDoc->m_bEditLocked) {
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTrackIndex = ((CTrackListTrackModeView*)m_pTrackModeView)->m_lCurRow;
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	long lFormat = MIDIData_GetFormat (pMIDIData);
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lTrackIndex);
	MIDITrack* pLastTrack = MIDIData_GetLastTrack (pMIDIData);
	// SMFフォーマット0の場合
	if (lFormat == 0) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_ADD_TRACK_IN_FORMAT0_MIDIDATA));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	// SMFフォーマット1でトラック0の場合
	if (lFormat == 1 && lTrackIndex == 0) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_ADD_TRACK_BEFORE_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	// MIDIデータのトラック数が65536以上の場合
	if (lTrackCount >= MAXMIDITRACKNUM) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_ADD_TRACK_ANY_MORE));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION); 
		return;
	}
	// 新規MIDIトラックの作成
	long i = 0;
	MIDIEvent* pTempEvent = NULL;
	MIDITrack* pNewTrack = MIDITrack_Create ();
	if (pNewTrack == NULL) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_INSUFFICIENT_MEMORY_OR_INSUFFICIENT_RESOURCE));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	// カレントMIDIトラックが存在する場合、そのトラックのプロパティをコピー
	if (pMIDITrack) {
		i = lTrackIndex;
		pNewTrack->m_lInputOn = pMIDITrack->m_lInputOn;
		pNewTrack->m_lInputPort = pMIDITrack->m_lInputPort;
		pNewTrack->m_lInputChannel = pMIDITrack->m_lInputChannel;
		pNewTrack->m_lOutputOn = pMIDITrack->m_lOutputOn;
		pNewTrack->m_lOutputPort = pMIDITrack->m_lOutputPort;
		pNewTrack->m_lOutputChannel = pMIDITrack->m_lOutputChannel;
		pNewTrack->m_lViewMode = pMIDITrack->m_lViewMode;
		pNewTrack->m_lForeColor = pMIDITrack->m_lForeColor;
		pNewTrack->m_lBackColor = pMIDITrack->m_lBackColor;
		pNewTrack->m_lKeyPlus = pMIDITrack->m_lKeyPlus;
		pNewTrack->m_lVelocityPlus = pMIDITrack->m_lVelocityPlus;
		pNewTrack->m_lTimePlus = pMIDITrack->m_lTimePlus;
		pNewTrack->m_lReserved1 = pMIDITrack->m_lReserved1;
		pNewTrack->m_lReserved2 = pMIDITrack->m_lReserved2;
		pNewTrack->m_lReserved3 = pMIDITrack->m_lReserved3;
		pNewTrack->m_lReserved4 = pMIDITrack->m_lReserved4;
		pNewTrack->m_lUser1 = pMIDITrack->m_lUser1;
		pNewTrack->m_lUser2 = pMIDITrack->m_lUser2;
		pNewTrack->m_lUser3 = pMIDITrack->m_lUser3;
		pNewTrack->m_lUserFlag = pMIDITrack->m_lUserFlag;
	}
	// カレントMIDIトラックが存在しない場合、新規MIDIトラックのプロパティを適宜設定
	else {
		i = lTrackCount;
		MIDITrack_SetInputOn (pNewTrack, 1);
		MIDITrack_SetInputPort (pNewTrack, 0);
		MIDITrack_SetInputChannel (pNewTrack, (i + 15) % 16);
		MIDITrack_SetOutputOn (pNewTrack, 1);
		MIDITrack_SetOutputPort (pNewTrack, 0);
		MIDITrack_SetOutputChannel (pNewTrack, (i + 15) % 16);
		MIDITrack_SetViewMode (pNewTrack, 0);
		MIDITrack_SetForeColor (pNewTrack, pSekaijuApp->m_lDefTrackColor[i % 16]);
		MIDITrack_SetBackColor (pNewTrack, 0x00FFFFFF);
		MIDITrack_SetKeyPlus (pNewTrack, 0);
		MIDITrack_SetVelocityPlus (pNewTrack, 0);
		MIDITrack_SetTimePlus (pNewTrack, 0);
		pNewTrack->m_lReserved1 = 0;
		pNewTrack->m_lReserved2 = 0;
		pNewTrack->m_lReserved3 = 0;
		pNewTrack->m_lReserved4 = 0;
		pNewTrack->m_lUser1 = 0;
		pNewTrack->m_lUser2 = 0;
		pNewTrack->m_lUser3 = 0;
		pNewTrack->m_lUserFlag = 0;
	}
	// 新規MIDIトラックの挿入
	if (pMIDITrack) {
		MIDIData_InsertTrackBefore (pMIDIData, pNewTrack, pMIDITrack);
	}
	else {
		MIDIData_InsertTrackAfter (pMIDIData, pNewTrack, pLastTrack);
	}
	// 履歴に記録
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_INSERT_TRACK));
	VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
	VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pNewTrack));
	// 新規MIDIトラックにデフォルトのMIDIイベントを追加
	VERIFY (pSekaijuDoc->AddDefaultEventToTrack (pNewTrack, i == 0 ? 0x000B : 0x0007, pCurHistoryUnit));
	
	//((CTrackListTrackModeView*)m_pTrackModeView)->m_lCurRow++;
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDITRACKCHANGED);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// 『トラックを1行挿入』
void CTrackListFrame::OnUpdateTrackListInsertTrackUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	// リアルタイム入力中は何もしない。
	if (pSekaijuApp->m_bRecording) {
		pCmdUI->Enable (FALSE);
		return;
	}
	// MIDIデータが編集ロックされている場合は何もしない。
	if (pSekaijuDoc->m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTrackIndex = ((CTrackListTrackModeView*)m_pTrackModeView)->m_lCurRow;
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	long lFormat = MIDIData_GetFormat (pMIDIData);
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lTrackIndex);
	if (lFormat == 0) {
		pCmdUI->Enable (FALSE);
	}
	else if (lFormat == 1 && lTrackIndex == 0) {
		pCmdUI->Enable (FALSE);
	}
	else if (lTrackCount >= MAXMIDITRACKNUM) {
		pCmdUI->Enable (FALSE);
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// 『トラックを1行複写』
void CTrackListFrame::OnTrackListDuplicateTrack () {
	_RPTF0 (_CRT_WARN, "OnTrackListInsertCopyTrack\n");
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	// リアルタイム入力中は何もしない。
	if (pSekaijuApp->m_bRecording) {
		return;
	}
	// MIDIデータが編集ロックされている場合は何もしない。
	if (pSekaijuDoc->m_bEditLocked) {
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTrackIndex = ((CTrackListTrackModeView*)m_pTrackModeView)->m_lCurRow;
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	long lFormat = MIDIData_GetFormat (pMIDIData);
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lTrackIndex);
	CHistoryUnit* pCurHistoryUnit = NULL;
	// SMFフォーマット0の場合
	if (lFormat == 0) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_DUPLICATE_TRACK_IN_FORMAT0_MIDIDATA));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	// SMFフォーマット1でトラック0の場合
	if (lFormat == 1 && lTrackIndex == 0) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_DUPLICATE_THE_FIRST_TRACK_IN_FORMAT1_MIDIATA));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	// カレントトラックが存在しない場合
	if (pMIDITrack == NULL) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_THE_SOURCE_TRACK_IS_EMPTY));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	// このMIDIデータのトラック数が65536以上の場合
	if (lTrackCount >= MAXMIDITRACKNUM) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_ADD_TRACK_ANY_MORE));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION); 
		return;
	}
	// 新規MIDIトラックの作成
	long i = lTrackIndex;
	MIDITrack* pNewTrack = MIDITrack_CreateClone (pMIDITrack);
	if (pNewTrack == NULL) {
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_INSUFFICIENT_MEMORY_OR_INSUFFICIENT_RESOURCE));
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	// 履歴記録
	CHistoryUnit* pHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_DUPLICATE_TRACK));
	VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
	VERIFY (pHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
	// 新規MIDIトラックの挿入
	MIDIData_InsertTrackBefore (pMIDIData, pNewTrack, pMIDITrack);
	VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pNewTrack));
	// 新しいMIDIトラックにデフォルトで含まれているイベントの履歴記録
	MIDIEvent* pNewEvent = NULL;
	forEachEvent (pNewTrack, pNewEvent) {
		if (pNewEvent->m_pPrevCombinedEvent == NULL) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
	}
	//((CTrackListTrackModeView*)m_pTrackModeView)->m_lCurRow++;
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDITRACKCHANGED);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// 『トラックを1行複写』
void CTrackListFrame::OnUpdateTrackListDuplicateTrackUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	// リアルタイム入力中は何もしない。
	if (pSekaijuApp->m_bRecording) {
		pCmdUI->Enable (FALSE);
		return;
	}
	// MIDIデータが編集ロックされている場合は何もしない。
	if (pSekaijuDoc->m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTrackIndex = ((CTrackListTrackModeView*)m_pTrackModeView)->m_lCurRow;
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	long lFormat = MIDIData_GetFormat (pMIDIData);
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lTrackIndex);
	// SMFフォーマット0の場合
	if (lFormat == 0) {
		pCmdUI->Enable (FALSE);
	}
	// SMFフォーマット1でトラック0の場合
	else if (lFormat == 1 && lTrackIndex == 0) {
		pCmdUI->Enable (FALSE);
	}
	// カレントトラックが存在しない場合
	else if (pMIDITrack == NULL) {
		pCmdUI->Enable (FALSE);
	}
	// このMIDIデータのトラック数が65536以上の場合
	else if (lTrackCount >= MAXMIDITRACKNUM) {
		pCmdUI->Enable (FALSE);
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// 『トラックを1行削除』
void CTrackListFrame::OnTrackListDeleteTrack () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	// リアルタイム入力中は何もしない。
	if (pSekaijuApp->m_bRecording) {
		return;
	}
	// MIDIデータが編集ロックされている場合は何もしない。
	if (pSekaijuDoc->m_bEditLocked) {
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTrackIndex = ((CTrackListTrackModeView*)m_pTrackModeView)->m_lCurRow;
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	long lFormat = MIDIData_GetFormat (pMIDIData);
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lTrackIndex);
	// SMFフォーマット2以外でトラック0の場合
	if (lTrackIndex == 0 && lFormat != 2) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_DELETE_THE_FIRST_TRACK));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	// カレントトラックが存在しない場合
	if (pMIDITrack == NULL) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_THE_SPECIFIED_TRACK_IS_EMPTY));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	// ヒストリ記録
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_DELETE_TRACK));
	VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
	VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack));
	// トラックをMIDIデータから除去
	MIDIData_RemoveTrack (pMIDIData, pMIDITrack);
	//((CTrackListTrackModeView*)m_pTrackModeView)->m_lCurRow--;
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDITRACKCHANGED);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// 『トラックを1行削除』
void CTrackListFrame::OnUpdateTrackListDeleteTrackUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	// リアルタイム入力中は何もしない。
	if (pSekaijuApp->m_bRecording) {
		pCmdUI->Enable (FALSE);
		return;
	}
	// MIDIデータが編集ロックされている場合は何もしない。
	if (pSekaijuDoc->m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTrackIndex = ((CTrackListTrackModeView*)m_pTrackModeView)->m_lCurRow;
	long lFormat = MIDIData_GetFormat (pMIDIData);
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lTrackIndex);
	// SMFフォーマット2以外でトラック0の場合
	if (lTrackIndex == 0 && lFormat != 2) {
		pCmdUI->Enable (FALSE);
	}
	// カレントトラックが存在しない場合
	else if (pMIDITrack == NULL) {
		pCmdUI->Enable (FALSE);
	}
	else {
		pCmdUI->Enable (TRUE);
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// 『トラックを1行上へ』
void CTrackListFrame::OnTrackListMoveUpTrack () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	// リアルタイム入力中は何もしない。
	if (pSekaijuApp->m_bRecording) {
		return;
	}
	// MIDIデータが編集ロックされている場合は何もしない。
	if (pSekaijuDoc->m_bEditLocked) {
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTrackIndex = ((CTrackListTrackModeView*)m_pTrackModeView)->m_lCurRow;
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	long lFormat = MIDIData_GetFormat (pMIDIData);
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lTrackIndex);
	// カレントトラックが存在しない場合
	if (pMIDITrack == NULL) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_NO_TRACK_IS_SELECTED_TO_BE_MOVED));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	// SMFフォーマット0の場合
	if (lFormat == 0) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_MOVE_TRACK_IN_FORMAT0_MIDIDATA));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	// SMFフォーマット1でカレントトラックが0又は1の場合
	if (lFormat == 1 && lTrackIndex <= 1) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_MOVE_UP_THE_FIRST_AND_THE_SECOND_TRACK_IN_FORMAT1_MIDIDATA));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	// SMFフォーマット2でカレントトラックが0の場合
	if (lFormat == 2 && lTrackIndex == 0) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_MOVE_UP_THE_FIRST_TRACK_IN_FORMAT2_MIDIDATA));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	// 履歴記録
	CHistoryUnit* pCurHistoryUnit = NULL;
	MIDITrack* pTempTrack = pMIDITrack->m_pPrevTrack;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_MOVE_TRACK));
	VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
	VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack));
	// 該当トラックをいったんMIDIデータから除去する
	long lRet = MIDIData_RemoveTrack (pMIDIData, pMIDITrack);
	if (lRet == 0) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_TRACK_MOVE_FAILED));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	// 除去したトラックをMIDIデータに挿入する
	lRet = MIDIData_InsertTrackBefore (pMIDIData, pMIDITrack, pTempTrack);
	if (lRet == 0) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_TRACK_MOVE_FAILED));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pMIDITrack));
	((CTrackListTrackModeView*)m_pTrackModeView)->m_lCurRow--;
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDITRACKCHANGED);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}


// トラックを1行上へ
void CTrackListFrame::OnUpdateTrackListMoveUpTrackUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	// リアルタイム入力中は何もしない。
	if (pSekaijuApp->m_bRecording) {
		pCmdUI->Enable (FALSE);
		return;
	}
	// MIDIデータが編集ロックされている場合は何もしない。
	if (pSekaijuDoc->m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTrackIndex = ((CTrackListTrackModeView*)m_pTrackModeView)->m_lCurRow;
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	long lFormat = MIDIData_GetFormat (pMIDIData);
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lTrackIndex);
	CHistoryUnit* pCurHistoryUnit = NULL;
	// カレントトラックが存在しない場合
	if (pMIDITrack == NULL) {
		pCmdUI->Enable (FALSE);
	}
	// SMFフォーマット0の場合
	if (lFormat == 0) {
		pCmdUI->Enable (FALSE);
	}
	// SMFフォーマット1でカレントトラックが0又は1の場合
	if (lFormat == 1 && lTrackIndex <= 1) {
		pCmdUI->Enable (FALSE);
	}
	// SMFフォーマット2でカレントトラックが0の場合
	if (lFormat == 2 && lTrackIndex == 0) {
		pCmdUI->Enable (FALSE);
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// 『トラックを1行下へ』
void CTrackListFrame::OnTrackListMoveDownTrack () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	// リアルタイム入力中は何もしない。
	if (pSekaijuApp->m_bRecording) {
		return;
	}
	// MIDIデータが編集ロックされている場合は何もしない。
	if (pSekaijuDoc->m_bEditLocked) {
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTrackIndex = ((CTrackListTrackModeView*)m_pTrackModeView)->m_lCurRow;
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	long lFormat = MIDIData_GetFormat (pMIDIData);
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lTrackIndex);
	CHistoryUnit* pCurHistoryUnit = NULL;
	// カレントトラックが存在しない場合
	if (pMIDITrack == NULL) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_NO_TRACK_IS_SELECTED_TO_BE_MOVED));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	// SMFフォーマット0の場合
	if (lFormat == 0) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_MOVE_TRACK_IN_FORMAT0_MIDIDATA));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	// SMFフォーマット1でカレントトラックが0の場合
	if (lFormat == 1 && lTrackIndex == 0) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_MOVE_DOWN_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	// カレントトラックが最後のトラックの場合
	if (lTrackIndex >= lTrackCount - 1) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_MOVE_DOWN_THIS_TRACK_ANY_MORE));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	MIDITrack* pTempTrack = pMIDITrack->m_pNextTrack;
	// 履歴記録
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_MOVE_TRACK));
	VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
	VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack));
	long lRet = MIDIData_RemoveTrack (pMIDIData, pMIDITrack);
	// 該当トラックをいったんMIDIデータから除去する
	if (lRet == 0) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_TRACK_MOVE_FAILED));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	// 除去したトラックをMIDIデータに挿入する
	lRet = MIDIData_InsertTrackAfter (pMIDIData, pMIDITrack, pTempTrack);
	if (lRet == 0) {
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_TRACK_MOVE_FAILED));
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pMIDITrack));
	((CTrackListTrackModeView*)m_pTrackModeView)->m_lCurRow++;
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDITRACKCHANGED);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// 『トラックを1行下へ』
void CTrackListFrame::OnUpdateTrackListMoveDownTrackUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	// リアルタイム入力中は何もしない。
	if (pSekaijuApp->m_bRecording) {
		pCmdUI->Enable (FALSE);
		return;
	}
	// MIDIデータが編集ロックされている場合は何もしない。
	if (pSekaijuDoc->m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTrackIndex = ((CTrackListTrackModeView*)m_pTrackModeView)->m_lCurRow;
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	long lFormat = MIDIData_GetFormat (pMIDIData);
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lTrackIndex);
	CHistoryUnit* pCurHistoryUnit = NULL;
	// カレントトラックが存在しない場合
	if (pMIDITrack == NULL) {
		pCmdUI->Enable (FALSE);
	}
	// SMFフォーマット0の場合
	if (lFormat == 0) {
		pCmdUI->Enable (FALSE);
	}
	// SMFフォーマット1でカレントトラックが0の場合
	if (lFormat == 1 && lTrackIndex == 0) {
		pCmdUI->Enable (FALSE);
	}
	// カレントトラックが最後のトラックの場合
	if (lTrackIndex >= lTrackCount - 1) {
		pCmdUI->Enable (FALSE);
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// 『選択(S)』
void CTrackListFrame::OnTrackListSelect () {
	m_lCurTool = ID_TRACKLIST_SELECT;
}

// 『選択(S)』
void CTrackListFrame::OnUpdateTrackListSelectUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurTool == ID_TRACKLIST_SELECT ? 1 : 0);
}

// 『試聴(B)』
void CTrackListFrame::OnTrackListSpeaker () {
	m_lCurTool = ID_TRACKLIST_SPEAKER;
}

// 『試聴(B)』
void CTrackListFrame::OnUpdateTrackListSpeakerUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurTool == ID_TRACKLIST_SPEAKER ? 1 : 0);
}

// 『自動ページ更新』
void CTrackListFrame::OnTrackListAutoPageUpdate () {
	m_bAutoPageUpdate = !m_bAutoPageUpdate;
}

// 『自動ページ更新』
void CTrackListFrame::OnUpdateTrackListAutoPageUpdateUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_bAutoPageUpdate);
}

// 『トラックビューをCSV又はテキストで保存』
void CTrackListFrame::OnTrackListSaveAs () {
	CString strFileName;
	CSekaijuFileDlg theFileDlg (FALSE);

	CString strTitle;
	VERIFY (strTitle.LoadString (AFX_IDS_APP_TITLE));

	// 注意事項：MSDN2002/4のOPENFILENAME構造体の説明
	// For compatibility reasons, the Places Bar is hidden if Flags is set to 
	// OFN_ENABLEHOOK and lStructSize is OPENFILENAME_SIZE_VERSION_400.
	// CFileDialogクラスでは強制的にOFN_ENABLEHOOKでAfxCommDlgProcにフックする。

	// 20081028 : m_ofnの値の設定(76or88)は次による
	//#ifndef OPENFILENAME_SIZE_VERSION_400
	//    theFileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME); //=76(Windows95/98/ME style)
	//#else
	//    theFileDlg.m_ofn.lStructSize = OPENFILENAME_SIZE_VERSION_400; //=76(Windows95/98/ME style)
	//#endif
	
	#if (_WIN32_WINNT >= 0x0500)
	    theFileDlg.m_ofn.lStructSize = 88; //=88(With placebar)
		theFileDlg.m_ofn.pvReserved = NULL;
		theFileDlg.m_ofn.dwReserved = 0;
		theFileDlg.m_ofn.FlagsEx = 0;
	#else
	    theFileDlg.m_ofn.lStructSize = 76; //=76(Without placebar if OFN_ENABLEHOOK used)
	#endif

	theFileDlg.m_ofn.nMaxFileTitle = _MAX_PATH;
	
	//theFileDlg.m_ofn.Flags |= lFlags;

	// 拡張子フィルター
	CString strFilter;
	CString strDefault;
	CString strFilterCSV[3];
	CString strFilterTXT[3];
	VERIFY (strFilterCSV[0].LoadString (IDS_COMMA_SEPARATED_TEXT_FILES_AD_CSV));
	VERIFY (strFilterCSV[1].LoadString (IDS_AD_CSV));
	VERIFY (strFilterCSV[2].LoadString (IDS_D_CSV));
	VERIFY (strFilterTXT[0].LoadString (IDS_TAB_SEPARATED_TEXT_FILES_AD_TXT));
	VERIFY (strFilterTXT[1].LoadString (IDS_AD_TXT));
	VERIFY (strFilterTXT[2].LoadString (IDS_D_TXT));

	// 拡張子フィルター追加設定
	strFilter += strFilterCSV[0]; // カンマ区切りテキストファイル(*.csv)
	strFilter += (TCHAR)_T('\0');
	strFilter += strFilterCSV[1]; // *.csv
	strFilter += (TCHAR)_T('\0');
	strFilter += strFilterTXT[0]; // タブ区切りテキストファイル(*.txt)
	strFilter += (TCHAR)_T('\0');
	strFilter += strFilterTXT[1]; // *.txt
	strFilter += (TCHAR)_T('\0');
	strFilter += (TCHAR)_T('\0');
	theFileDlg.m_ofn.nMaxCustFilter = 1024;

	theFileDlg.m_ofn.lpstrFilter = strFilter;
#ifndef _MAC
	theFileDlg.m_ofn.lpstrTitle = strTitle;
#else
	theFileDlg.m_ofn.lpstrPrompt = strTitle;
#endif

	// デフォルトの拡張子設定
	theFileDlg.m_strDefExt = strFilterCSV[1];
	theFileDlg.m_ofn.lpstrDefExt = theFileDlg.m_strDefExt;

	// フィルターコンボボックスのデフォルト選択
	if ((strFileName.Right (4)).CompareNoCase (strFilterCSV[2]) == 0) {
		theFileDlg.m_ofn.nFilterIndex = 1;
	}
	else if ((strFileName.Right (4)).CompareNoCase (strFilterTXT[2]) == 0) {
		theFileDlg.m_ofn.nFilterIndex = 2;
	}
	
	theFileDlg.m_ofn.lpstrFile = strFileName.GetBuffer (_MAX_PATH);

	// ファイルダイアログ.DoModal
	BOOL bResult = theFileDlg.DoModal() == IDOK ? TRUE : FALSE;

	strFileName.ReleaseBuffer();

	// 拡張子が付いていない場合は、選択したファイルタイプの拡張子を自動的に付ける
	if (theFileDlg.m_ofn.nFilterIndex == 1 &&
		(strFileName.Right (4)).CompareNoCase (strFilterCSV[2]) != 0) { // .csv
		strFileName += strFilterCSV[2];
	}
	else if (theFileDlg.m_ofn.nFilterIndex == 2 &&
		(strFileName.Right (4)).CompareNoCase (strFilterTXT[2]) != 0) { // .txt
		strFileName += strFilterTXT[2];
	}

	// キャンセルが押された場合は却下
	if (bResult == FALSE) {
		return;
	}

	// ビューがない場合は却下
	if (m_pTrackModeView == NULL) {
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_CURRENT_VIEW_IS_NOT_EXIST));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	CTrackListTrackModeView* pTrackModeView = (CTrackListTrackModeView*)m_pTrackModeView;

	// 文字列バッファ確保
	CString strTextLine;
	CString strCell;
	CString strSeparator;
	if (theFileDlg.m_ofn.nFilterIndex == 1) {
		strSeparator = _T(", ");
	}
	else {
		strSeparator = _T("\t");
	}

	// ファイルオープン
	CStdioFile theFile;
	BOOL bRet = theFile.Open (strFileName, CFile::modeCreate | CFile::modeWrite);
	if (bRet == FALSE) {
		CString strMsg;
		CString strFormat;
		strFormat.LoadString (IDS_S_N_FILE_OPEN_FAILED);
		strMsg.Format (strFormat, strFileName);
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}

	// 先頭行の文字列書き込み
	long i = 0;
	long j = 0;
	strTextLine.Format (_T(""));
	strTextLine += strSeparator;
	for (j = 0; j < TRACKLISTFRAME_NUMCOLUMN; j++) {
		strCell = GetColumnTitle (j);
		strTextLine += strCell;
		if (j < TRACKLISTFRAME_NUMCOLUMN - 1) {
			strTextLine += strSeparator;
		}
		else {
			strTextLine += _T("\n");
		}
	}
	theFile.WriteString (strTextLine);

	// 各行の文字列書き込み
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	ASSERT (pMIDIData);
	long lRowCount = MIDIData_CountTrack (pMIDIData);
	for (i = 0; i < lRowCount; i++) {
		strTextLine.Format (_T("%d"), i + 1);
		strTextLine += strSeparator;
		for (j = 0; j < TRACKLISTFRAME_NUMCOLUMN; j++) {
			strCell = pTrackModeView->GetCellString (i, j);
			strTextLine += strCell;
			if (j < TRACKLISTFRAME_NUMCOLUMN - 1) {
				strTextLine += strSeparator;
			}
			else {
				strTextLine += _T("\n");
			}
		}
		theFile.WriteString (strTextLine);
	}

	// ファイルクローズ
	theFile.Close ();
}	

// 『トラックビューをCSV又はテキストで保存』
void CTrackListFrame::OnUpdateTrackListSaveAsUI (CCmdUI* pCmdUI) {
}





