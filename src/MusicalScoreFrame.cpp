//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 譜面フレームウィンドウクラス
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
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDoc.h"
#include "SekaijuView.h"
#include "SekaijuToolBar.h"
#include "MainFrame.h"
#include "ChildFrame.h"
#include "ColorfulCheckListBox.h"
#include "ColorfulComboBox.h"
#include "MusicalScoreFrame.h"
#include "MusicalScorePrintView.h"
#include "MusicalScoreScaleView.h"
#include "MusicalScoreTimeScaleView.h"
#include "MusicalScoreTrackScaleView.h"
#include "MusicalScoreTrackTimeView.h"
#include "PropertyNoteDlg.h"

// アロケーションの監視
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 子ウィンドウのサイズ
#define MUSICALSCOREFRAME_SCALEHEIGHT 32
#define MUSICALSCOREFRAME_SCALEWIDTH 160
#define MUSICALSCOREFRAME_TIMEWIDTH 600
#define MUSICALSCOREFRAME_TRACKHEIGHT 256


#define MUSICALSCOREFRAME_VELHEIGHT 128
#define MUSICALSCOREFRAME_SCROLLBARHEIGHT 16
#define MUSICALSCOREFRAME_SCROLLBARWIDTH 16
#define MUSICALSCOREFRAME_BORDERWIDTH 2
#define MUSICALSCOREFRAME_BORDERHEIGHT 2
#define MUSICALSCOREFRAME_SPLITTERWIDTH 4
#define MUSICALSCOREFRAME_SPLITTERHEIGHT 4
#define MUSICALSCOREFRAME_TRACKLISTWIDTH 120

// 子ウィンドウIDを定義
#define MUSICALSCOREFRAME_DUMMYVIEW      (AFX_IDW_PANE_FIRST + 0)
#define MUSICALSCOREFRAME_PRINTVIEW      (AFX_IDW_PANE_FIRST + 1)
#define MUSICALSCOREFRAME_SCALEVIEW      (AFX_IDW_PANE_FIRST + 32)
#define MUSICALSCOREFRAME_TIMESCALEVIEW  (AFX_IDW_PANE_FIRST + 33)
#define MUSICALSCOREFRAME_TRACKSCALEVIEW (AFX_IDW_PANE_FIRST + 34)
#define MUSICALSCOREFRAME_TRACKTIMEVIEW  (AFX_IDW_PANE_FIRST + 35)
#define MUSICALSCOREFRAME_VELSCALEVIEW   (AFX_IDW_PANE_FIRST + 36)
#define MUSICALSCOREFRAME_VELTIMEVIEW    (AFX_IDW_PANE_FIRST + 37)
#define MUSICALSCOREFRAME_TIMESCROLL     (AFX_IDW_PANE_FIRST + 48)
#define MUSICALSCOREFRAME_TRACKSCROLL    (AFX_IDW_PANE_FIRST + 49)
#define MUSICALSCOREFRAME_VELSCROLL      (AFX_IDW_PANE_FIRST + 50)
#define MUSICALSCOREFRAME_SIZEBOX        (AFX_IDW_PANE_FIRST + 51)
#define MUSICALSCOREFRAME_TIMEZOOMDOWN   (AFX_IDW_PANE_FIRST + 52)
#define MUSICALSCOREFRAME_TIMEZOOMUP     (AFX_IDW_PANE_FIRST + 53)
#define MUSICALSCOREFRAME_TRACKZOOMDOWN  (AFX_IDW_PANE_FIRST + 54)
#define MUSICALSCOREFRAME_TRACKZOOMUP    (AFX_IDW_PANE_FIRST + 55)
#define MUSICALSCOREFRAME_VELZOOMDOWN    (AFX_IDW_PANE_FIRST + 56)
#define MUSICALSCOREFRAME_VELZOOMUP      (AFX_IDW_PANE_FIRST + 57)
#define MUSICALSCOREFRAME_TRACKLIST      (AFX_IDW_PANE_FIRST + 58)
#define MUSICALSCOREFRAME_GRAPHKINDLIST  (AFX_IDW_PANE_FIRST + 59)

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
//        ↑        ┃m_pKeyScale │m_pTrackTimeView     　　     │　┃              │　┃
//                  ┃View        │  　　　　　　　　　　　　　├─┨              │　┃
//   m_lKeyHeight   ┃　　　      │   CScrollBar m_wndTrackScroll│↓┃              │　┃
//                  ┃　　　      │　　　　　　　　　　　　　  ├─┨              ├─┨
//                  ┃　　　      │　  CButton m_wndTrackZoomDown│－┃              │↓┃
//                  ┃　　　      │　　　　　　　　　　　　　　├─┨　　　　　　　┼─┨
//        ↓        ┃　　　      │　 　 CButton m_wndTrackZoomUp│＋┃　　　　　　　│　┃
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
#define MUSICALSCOREFRAME_RANGE(A,B,C) ((A)>(B)?(A):((B)>(C)?(C):(B)))

// メッセージマップ
IMPLEMENT_DYNCREATE (CMusicalScoreFrame, CChildFrame)

BEGIN_MESSAGE_MAP (CMusicalScoreFrame, CChildFrame)
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
	ON_BN_CLICKED (MUSICALSCOREFRAME_TIMEZOOMDOWN, OnTimeZoomDown)
	ON_BN_CLICKED (MUSICALSCOREFRAME_TIMEZOOMUP, OnTimeZoomUp)
	ON_BN_CLICKED (MUSICALSCOREFRAME_TRACKZOOMDOWN, OnTrackZoomDown)
	ON_BN_CLICKED (MUSICALSCOREFRAME_TRACKZOOMUP, OnTrackZoomUp)
//	ON_WM_CHILDACTIVATE ()	
	ON_COMMAND (ID_MUSICALSCORE_PEN, OnMusicalScorePen)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_PEN, OnUpdateMusicalScorePenUI)
	ON_COMMAND (ID_MUSICALSCORE_ERASER, OnMusicalScoreEraser)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_ERASER, OnUpdateMusicalScoreEraserUI)
	ON_COMMAND (ID_MUSICALSCORE_SELECT, OnMusicalScoreSelect)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_SELECT, OnUpdateMusicalScoreSelectUI)
	ON_COMMAND (ID_MUSICALSCORE_SPEAKER, OnMusicalScoreSpeaker)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_SPEAKER, OnUpdateMusicalScoreSpeakerUI)
	ON_COMMAND (ID_MUSICALSCORE_WHOLENOTE, OnMusicalScoreWholeNote)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_WHOLENOTE, OnUpdateMusicalScoreWholeNoteUI)
	ON_COMMAND (ID_MUSICALSCORE_HALFNOTE, OnMusicalScoreHalfNote)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_HALFNOTE, OnUpdateMusicalScoreHalfNoteUI)
	ON_COMMAND (ID_MUSICALSCORE_QUARTERNOTE, OnMusicalScoreQuarterNote)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_QUARTERNOTE, OnUpdateMusicalScoreQuarterNoteUI)
	ON_COMMAND (ID_MUSICALSCORE_QUAVERNOTE, OnMusicalScoreQuaverNote)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_QUAVERNOTE, OnUpdateMusicalScoreQuaverNoteUI)
	ON_COMMAND (ID_MUSICALSCORE_SEMIQUAVERNOTE, OnMusicalScoreSemiQuaverNote)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_SEMIQUAVERNOTE, OnUpdateMusicalScoreSemiQuaverNoteUI)
	ON_COMMAND (ID_MUSICALSCORE_DEMISEMIQUAVERNOTE, OnMusicalScoreDemiSemiQuaverNote)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_DEMISEMIQUAVERNOTE, OnUpdateMusicalScoreDemiSemiQuaverNoteUI)
	ON_COMMAND (ID_MUSICALSCORE_DOTTED, OnMusicalScoreDotted)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_DOTTED, OnUpdateMusicalScoreDottedUI)
	ON_COMMAND (ID_MUSICALSCORE_TRIPLET, OnMusicalScoreTriplet)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_TRIPLET, OnUpdateMusicalScoreTripletUI)
	ON_COMMAND (ID_MUSICALSCORE_ONLYCURTRACK, OnMusicalScoreOnlyCurTrack)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_ONLYCURTRACK, OnUpdateMusicalScoreOnlyCurTrackUI)
	ON_COMMAND (ID_MUSICALSCORE_SHOWALLTRACK, OnMusicalScoreShowAllTrack)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_SHOWALLTRACK, OnUpdateMusicalScoreShowAllTrackUI)
	ON_COMMAND (ID_MUSICALSCORE_AUTOPAGEUPDATE, OnMusicalScoreAutoPageUpdate)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_AUTOPAGEUPDATE, OnUpdateMusicalScoreAutoPageUpdateUI)

	ON_COMMAND (ID_POPUP_TRACKVISIBLEON, OnPopupTrackVisibleOn)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKVISIBLEON, OnUpdatePopupTrackVisibleOnUI)
	ON_COMMAND (ID_POPUP_TRACKVISIBLEOFF, OnPopupTrackVisibleOff)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKVISIBLEOFF, OnUpdatePopupTrackVisibleOffUI)
	ON_COMMAND (ID_POPUP_TRACKVISIBLEALL, OnPopupTrackVisibleAll)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKVISIBLEALL, OnUpdatePopupTrackVisibleAllUI)
	ON_COMMAND (ID_POPUP_EVENTPROPERTY, OnPopupEventProperty)
	ON_UPDATE_COMMAND_UI (ID_POPUP_EVENTPROPERTY, OnUpdatePopupEventPropertyUI)

	ON_CBN_SELENDOK (IDC_SNAPCOMBO, OnSnapComboSelEndOK)	
	ON_CBN_SELENDOK (IDC_TRACKCOMBO, OnTrackComboSelEndOK)	
	ON_CLBN_CHKCHANGE (MUSICALSCOREFRAME_TRACKLIST, OnTrackListChkChange)
	ON_LBN_SELCHANGE (MUSICALSCOREFRAME_TRACKLIST, OnTrackListSelChange)
	ON_CBN_SELENDOK (IDC_RESOLUTIONCOMBO, OnResolutionComboSelEndOK)	


END_MESSAGE_MAP ()

//-----------------------------------------------------------------------------
// 構築と破壊
//-----------------------------------------------------------------------------

// コンストラクタ
CMusicalScoreFrame::CMusicalScoreFrame () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	m_lKeyHeight = MUSICALSCOREFRAME_TRACKHEIGHT;
	m_lScaleHeight = MUSICALSCOREFRAME_SCALEHEIGHT;
	m_lScaleWidth = MUSICALSCOREFRAME_SCALEWIDTH;
	m_lTimeWidth = MUSICALSCOREFRAME_TIMEWIDTH;
	//m_lSplitterHeight = MUSICALSCOREFRAME_SPLITTERHEIGHT;
	//m_lSplitterWidth = MUSICALSCOREFRAME_SPLITTERWIDTH;
	m_lHScrollBarHeight = ::GetSystemMetrics (SM_CYHSCROLL);
	m_lVScrollBarWidth = ::GetSystemMetrics (SM_CXVSCROLL);
	m_lTrackListWidth = MUSICALSCOREFRAME_TRACKLISTWIDTH;
	m_lTrackZoom = pSekaijuApp->m_theMusicalScoreOption.m_lDefTrackZoom;
	m_lTimeZoom = pSekaijuApp->m_theMusicalScoreOption.m_lDefTimeZoom;
	m_lTimeScrollPos = 0;
	CString strFontName;
	VERIFY (strFontName.LoadString (IDS_DEFAULTFONTNAME));
	m_theFont.CreateFont (12, 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,  
		strFontName);
	CString strTimeMeasureFontName;
	VERIFY (strTimeMeasureFontName.LoadString (IDS_TIMEMEASUREFONTNAME));
	m_theTimeMeasureFont.CreateFont (16, 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,  
		strTimeMeasureFontName);
	m_lCurTool = ID_MUSICALSCORE_PEN;
	m_bAutoPageUpdate = FALSE;
	m_bOnlyCurTrack = FALSE;
	m_bShowAllTrack = FALSE;
	for (long i = 0; i < MAXMIDITRACKNUM; i++) {
		m_bTrackVisible[i] = TRUE;
	}
}

// デストラクタ
CMusicalScoreFrame::~CMusicalScoreFrame () {
	m_theFont.DeleteObject ();
	m_theTimeMeasureFont.DeleteObject ();
	// トラック情報配列削除
	VERIFY (DeleteTrackInfoArray ());
	// 小節情報配列削除
	VERIFY (DeleteMeasureInfoArray ());

}

// ドキュメントの取得
CSekaijuDoc* CMusicalScoreFrame::GetDocument () {
	ASSERT (m_pDummyView);
	return (CSekaijuDoc*)m_pDummyView->GetDocument ();
}

//-----------------------------------------------------------------------------
// オペレーション
//-----------------------------------------------------------------------------

// 時間方向のズーム倍率取得
long CMusicalScoreFrame::GetTimeZoom () {
	return m_lTimeZoom;
}

// トラック方向のズーム倍率取得
long CMusicalScoreFrame::GetTrackZoom () {
	return m_lTrackZoom;
}

// y座標をトラックインデックスに変換
long CMusicalScoreFrame::YtoTrackIndex (long y) {
	long lNumTrackInfo = GetTrackInfoCount ();
	long i;
	for (i = 0; i < lNumTrackInfo; i++) {
		MusicalScoreTrackInfo* pTrackInfo = GetTrackInfo (i);
		if (pTrackInfo->m_lTop * m_lTrackZoom <= y && y < (pTrackInfo->m_lTop + pTrackInfo->m_lHeight) * m_lTrackZoom) {
			return i;
		}
	}
	return MAXMIDITRACKNUM;
}

// y座標をノートキーに変換
long CMusicalScoreFrame::TrackIndexYtoKey (long lTrackIndex, long y, long lKeySignature) {
	long lNumTrackInfo = GetTrackInfoCount ();
	if (lTrackIndex < 0 || lTrackIndex >= lNumTrackInfo) {
		return -1;
	}
	MusicalScoreTrackInfo* pTrackInfo = GetTrackInfo (lTrackIndex);
	long lKey;
	for (lKey = 0; lKey < 128; lKey++) {
		long lTempY = TrackIndexKeytoY (lTrackIndex, lKey, lKeySignature);
		if (lTempY < y) {
			if (KeytoSF (lKey, lKeySignature) == 0) {
				return lKey;
			}
		}
	}
	return 128;
}

// 指定の調性記号において、指定キーは譜面上の何番目の線に乗るかを返す。
// 同じキーでも調性記号により36になったり37になったりする。
long CMusicalScoreFrame::KeytoLineNo (long lKey, long lKeySignature) {
	long lsf = lKeySignature & 0x000000FF;
	long lmi = (lKeySignature >> 8) & 0x000000FF;
	lsf = CLIP (-7, (char)lsf, 7);
	long lDeltaTable[15][12] = {
	//   C, -, D, -, E, F, -, G, -, A, -, B
		{0, 1, 2, 2, 3, 3, 4, 4, 5, 6, 6, 7}, // 7b (Cb-major / Ab-minor)
		{0, 1, 1, 2, 3, 3, 4, 4, 5, 6, 6, 7}, // 6b (Gb-major / Eb-minor)
		{0, 1, 1, 2, 3, 3, 4, 4, 5, 5, 6, 7}, // 5b (Db-major / Bb-minor)
		{0, 1, 1, 2, 2, 3, 4, 4, 5, 5, 6, 7}, // 4b (Ab-major / F-minor)
		{0, 1, 1, 2, 2, 3, 4, 4, 5, 5, 6, 6}, // 3b (Eb-major / C-minor)
		{0, 1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6}, // 2b (Bb-major / G-minor)
		{0, 0, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6}, // 1b (F-major / D-minor)
		{0, 0, 1, 2, 2, 3, 3, 4, 4, 5, 6, 6}, // 0 (C-major / A-minor)
		{0, 0, 1, 1, 2, 3, 3, 4, 4, 5, 6, 6}, // 1# (G-major / E-minor)
		{0, 0, 1, 1, 2, 3, 3, 4, 4, 5, 5, 6}, // 2# (D-major / B-minor)
		{0, 0, 1, 1, 2, 2, 3, 4, 4, 5, 5, 6}, // 3# (A-major / F#-minor)
		{-1, 0, 1, 1, 2, 2, 3, 4, 4, 5, 5, 6}, // 4# (E-major / C#-minor)
		{-1, 0, 1, 1, 2, 2, 3, 3, 4, 5, 5, 6}, // 5# (B-major / G#-minor)
		{-1, 0, 0, 1, 2, 2, 3, 3, 4, 5, 5, 6}, // 6# (F#-major / D#-minor)
		{-1, 0, 0, 1, 2, 2, 3, 3, 4, 4, 5, 6}  // 7# (C#-major / A#-minor)
	};
	return ((lKey / 12) * 7 + lDeltaTable[lsf + 7][lKey % 12]);
}

// 指定の調性記号において、指定キーに#bの臨時記号が付くかどうかを返す。
// 戻り値：0なし,2:bb,3:b,4:ナチュラル,5:#,6:##をつけるべきである。
long CMusicalScoreFrame::KeytoSF (long lKey, long lKeySignature) {
	long lsf = lKeySignature & 0x000000FF;
	long lmi = (lKeySignature >> 8) & 0x000000FF;
	lsf = CLIP (-7, (char)lsf, 7);
	long lSFTable[15][12] = {
	//   C, -, D, -, E, F, -, G, -, A, -, B
		{4, 0, 2, 0, 0, 4, 0, 4, 0, 2, 0, 0}, // 7b (Cb-major / Ab-minor)
		{4, 0, 4, 0, 3, 0, 0, 4, 0, 2, 0, 0}, // 6b (Gb-major / Eb-minor)
		{0, 0, 4, 0, 3, 0, 0, 4, 0, 4, 0, 3}, // 5b (Db-major / Bb-minor)
		{0, 0, 4, 0, 4, 0, 3, 0, 0, 4, 0, 3}, // 4b (Ab-major / F-minor)
		{0, 3, 0, 0, 4, 0, 3, 0, 0, 4, 0, 4}, // 3b (Eb-major / C-minor)
		{0, 3, 0, 0, 4, 0, 5, 0, 3, 0, 0, 4}, // 2b (Bb-major / G-minor)
		{0, 5, 0, 3, 0, 0, 5, 0, 3, 0, 0, 4}, // 1b (F-major / D-minor)
		{0, 5, 0, 3, 0, 0, 5, 0, 5, 0, 3, 0}, // 0 (C-major / A-minor)
		{0, 5, 0, 5, 0, 4, 0, 0, 5, 0, 3, 0}, // 1# (G-major / E-minor)
		{4, 0, 0, 5, 0, 4, 0, 0, 5, 0, 5, 0}, // 2# (D-major / B-minor) // 20110111 G,G#修正
		{4, 0, 0, 5, 0, 5, 0, 4, 0, 0, 5, 0}, // 3# (A-major / F#-minor)
		{5, 0, 4, 0, 0, 5, 0, 4, 0, 0, 5, 0}, // 4# (E-major / C#-minor)
		{5, 0, 4, 0, 0, 5, 0, 6, 0, 4, 0, 0}, // 5# (B-major / G#-minor)
		{5, 0, 6, 0, 4, 0, 0, 6, 0, 4, 0, 0}, // 6# (F#-major / D#-minor)
		{0, 0, 6, 0, 4, 0, 0, 6, 0, 6, 0, 4}  // 7# (C#-major / A#-minor)
	};
	return (lSFTable[lsf + 7][lKey % 12]);
}

// トラックインデックス(0～65535)・五線番号をY座標[pixel]に変換
long CMusicalScoreFrame::TrackIndexLineNotoY (long lTrackIndex, long lLineNo) {
	ASSERT (0<= lTrackIndex &&lTrackIndex < GetTrackInfoCount ());
	// デフォルトのm_lTrackZoom=4。
	MusicalScoreTrackInfo* pTrackInfo = NULL;
	VERIFY (pTrackInfo = GetTrackInfo (lTrackIndex));
	long lTrackTop = pTrackInfo->m_lTop;
	long lTrackHeight = pTrackInfo->m_lHeight;
	long lTrackFlags = pTrackInfo->m_lFlags;
	long lKey000Y = 0;
	long ry = 4;
	switch (lTrackFlags & 0x0000000F) {
	case 1: // ト音記号
		lKey000Y = (lTrackTop + lTrackHeight / 2) * m_lTrackZoom + 41 * ry;
		break;
	case 2: // へ音記号
		lKey000Y = (lTrackTop + lTrackHeight / 2) * m_lTrackZoom + 29 * ry;
		break;
	case 3: // 大譜表

		lKey000Y = (lTrackTop + lTrackHeight / 2) * m_lTrackZoom + 35 * ry;
		break;
	default:
		lKey000Y = (lTrackTop + lTrackHeight / 2) * m_lTrackZoom + 35 * ry;
		break;
	}
	return lKey000Y - lLineNo * ry;
}

// トラックインデックス(0～65535)・ノートキー(0～127)をy座標[pixel]に変換
long CMusicalScoreFrame::TrackIndexKeytoY (long lTrackIndex, long lKey, long lKeySignature) {
	long lLineNo = KeytoLineNo (lKey, lKeySignature);
	return TrackIndexLineNotoY (lTrackIndex, lLineNo);
}



// x座標[pixel]からタイム[tick]を取得
long CMusicalScoreFrame::XtoTime (long x) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lMeasureCount = m_theMeasureInfoArray.GetSize ();
	long j;
	for (j = 0; j < lMeasureCount - 1; j++) {
		if (GetMeasureLeft (j + 1) * m_lTimeZoom > x) {
			break;
		}
	}
	MusicalScoreMeasureInfo* pMeasureInfo = NULL;
	VERIFY (pMeasureInfo = GetMeasureInfo (j));
	long lDeltaX_TimeZoom = x - 
		(pMeasureInfo->m_lLeft + pMeasureInfo->m_lSignatureWidth + 
		pMeasureInfo->m_lPreWidth) * m_lTimeZoom;
	lDeltaX_TimeZoom = MAX (0, lDeltaX_TimeZoom);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	return pMeasureInfo->m_lTime + lDeltaX_TimeZoom * lTimeResolution / 8 / m_lTimeZoom;
	// 注：ズーム倍率1倍のとき4分音符の長さを8ピクセルと定義している。

}

// タイム[tick]をx座標[pixel]に変換
long CMusicalScoreFrame::TimetoX (long lTime) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lMeasureCount = m_theMeasureInfoArray.GetSize ();
	long lMeasure, lBeat, lTick;
	MIDIData_BreakTime (pMIDIData, lTime, &lMeasure, &lBeat, &lTick);
	ASSERT (0 <= lMeasure && lMeasure < lMeasureCount);
	long j;
	for (j = 0; j < lMeasureCount - 1; j++) {
		if (GetMeasureTime (j + 1) > lTime) {
			break;
		}
	}
	long lMeasureTime = GetMeasureTime (j);
	long lDeltaTime = lTime - lMeasureTime;
	ASSERT (lDeltaTime >= 0);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	MusicalScoreMeasureInfo* pMeasureInfo = NULL;
	VERIFY (pMeasureInfo = GetMeasureInfo (j));

	return 
		pMeasureInfo->m_lLeft * m_lTimeZoom + 
		pMeasureInfo->m_lSignatureWidth * m_lTimeZoom +
		pMeasureInfo->m_lPreWidth * m_lTimeZoom +
 		(lDeltaTime * 8 * m_lTimeZoom / lTimeResolution);
	// 注：ズーム倍率1倍のとき4分音符の長さを8ピクセルと定義している。
}

// 小節をx座標[pixel]に変換(小節境界線位置)
long CMusicalScoreFrame::MeasuretoX (long lMeasure) {
	long lMeasureCount = GetMeasureInfoCount ();
	ASSERT (0 <= lMeasure && lMeasure < lMeasureCount);
	MusicalScoreMeasureInfo* pMeasureInfo = NULL;
	VERIFY (pMeasureInfo = GetMeasureInfo (lMeasure));
	return pMeasureInfo->m_lLeft * m_lTimeZoom;
}

// 小節をx座標[pixel]に変換(小節境界線+拍子調整記号位置)
long CMusicalScoreFrame::MeasuretoX2 (long lMeasure) {
	long lMeasureCount = GetMeasureInfoCount ();
	ASSERT (0 <= lMeasure && lMeasure < lMeasureCount);
	MusicalScoreMeasureInfo* pMeasureInfo = NULL;
	VERIFY (pMeasureInfo = GetMeasureInfo (lMeasure));
	return (pMeasureInfo->m_lLeft + pMeasureInfo->m_lSignatureWidth) * m_lTimeZoom;
}

// 時間方向のスクロールポジション取得
long CMusicalScoreFrame::GetTimeScrollPos () {
	return m_lTimeScrollPos;
}

// トラック方向のスクロールポジション取得
long CMusicalScoreFrame::GetTrackScrollPos () {
	return m_lTrackScrollPos;
}

// 時間方向のスクロールポジション設定
long CMusicalScoreFrame::SetTimeScrollPos (long lTimeScrollPos) {
	long lOldTimeScrollPos = m_lTimeScrollPos;
	m_wndTimeScroll.SetScrollPos (lTimeScrollPos);
	m_lTimeScrollPos = m_wndTimeScroll.GetScrollPos ();
	long lDeltaTimeScrollPos = m_lTimeScrollPos - lOldTimeScrollPos;
	m_pTimeScaleView->ScrollWindow (-lDeltaTimeScrollPos, 0);
	m_pTrackTimeView->ScrollWindow (-lDeltaTimeScrollPos, 0);
	m_pTimeScaleView->UpdateWindow ();
	m_pTrackTimeView->UpdateWindow ();
	return m_lTimeScrollPos;
}

// トラック向のスクロールポジション設定
long CMusicalScoreFrame::SetTrackScrollPos (long lTrackScrollPos) {
	long lOldTrackScrollPos = m_lTrackScrollPos;
	m_wndTrackScroll.SetScrollPos (lTrackScrollPos);
	m_lTrackScrollPos = m_wndTrackScroll.GetScrollPos ();
	long lDeltaTrackScrollPos = m_lTrackScrollPos - lOldTrackScrollPos;
	m_pTrackScaleView->ScrollWindow (0, -lDeltaTrackScrollPos);
	m_pTrackTimeView->ScrollWindow (0, -lDeltaTrackScrollPos);
	m_pTrackScaleView->UpdateWindow ();
	m_pTrackTimeView->UpdateWindow ();
	return m_lTrackScrollPos;
}


// 表示されているタイムの左端を求める
long CMusicalScoreFrame::GetVisibleLeftTime () {
	return XtoTime (m_lTimeScrollPos);
}

// 表示されているタイムの右端を求める
long CMusicalScoreFrame::GetVisibleRightTime () {
	CRect rcClient;
	m_pTimeScaleView->GetClientRect (&rcClient);
	return XtoTime (m_lTimeScrollPos + rcClient.Width ());
}

// 表示されているトラックの上限を計算
long CMusicalScoreFrame::GetVisibleTopTrack () {
	long lTrackCount = GetTrackInfoCount ();
	long lTrackIndex;
	for (lTrackIndex = 0; lTrackIndex < lTrackCount - 1; lTrackIndex++) {
		if (GetTrackInfo (lTrackIndex + 1)->m_lTop * m_lTrackZoom >= m_lTrackScrollPos) {
			break;
		}
	}
	return lTrackIndex;
}

// 表示されているトラックの下限を計算
long CMusicalScoreFrame::GetVisibleBottomTrack () {
	CRect rcClient;
	m_pTrackScaleView->GetClientRect (&rcClient);
	long lTrackCount = m_theTrackInfoArray.GetSize ();
	long lTrackIndex;
	for (lTrackIndex = 0; lTrackIndex < lTrackCount; lTrackIndex++) {
		if (GetTrackInfo (lTrackIndex)->m_lTop * m_lTrackZoom >= m_lTrackScrollPos + rcClient.Height ()) {
			break;
		}
	}
	return lTrackIndex;
}

// スプリッターキャプターの描画
void CMusicalScoreFrame::DrawSplitterCaptor (CDC* pDC, CPoint pt) {
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
long CMusicalScoreFrame::GetCurTrackIndex () {
	return m_wndTrackList.GetCurSel ();
}

// 現在のチャンネルを取得
long CMusicalScoreFrame::GetCurChannel () {
	return m_wndChannelCombo.GetCurSel ();
}

// 現在のスナップ[ティック]を取得
long CMusicalScoreFrame::GetCurSnap () {
	CString strText;
	m_wndSnapCombo.GetWindowText (strText);
	return _ttol (strText);
}

// 現在のベロシティを取得
long CMusicalScoreFrame::GetCurVelocity () {
	CString strText;
	m_wndVelocityCombo.GetWindowText (strText);
	return _ttol (strText);
}

// 現在の(音符の)長さ[ティック]を取得
long CMusicalScoreFrame::GetCurDuration () {
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	return _ttol (strText);
}

// 現在の表示精度[ティック]を取得
long CMusicalScoreFrame::GetCurResolution () {
	CString strText;
	m_wndResolutionCombo.GetWindowText (strText);
	return _ttol (strText);
}

// 現在のトラックのインデックスを設定
BOOL CMusicalScoreFrame::SetCurTrackIndex (long lCurTrackIndex) {
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
		m_pTrackTimeView->Invalidate ();
	}
	return TRUE;
}

// 現在のチャンネルを設定
BOOL CMusicalScoreFrame::SetCurChannel (long lCurChannel) {
	ASSERT (0 <= lCurChannel && lCurChannel < 16);
	m_wndChannelCombo.SetCurSel (lCurChannel);
	return TRUE;
}

// 現在のスナップ[ティック]を設定
BOOL CMusicalScoreFrame::SetCurSnap (long lCurSnap) {
	ASSERT (1 <= lCurSnap);
	CString strText;
	strText.Format (_T("%d"), lCurSnap);
	m_wndSnapCombo.SetWindowText (strText);
	return TRUE;
}

// 現在のベロシティを設定
BOOL CMusicalScoreFrame::SetCurVelocity (long lCurVelocity) {
	ASSERT (0 <= lCurVelocity && lCurVelocity <= 127);
	CString strText;
	strText.Format (_T("%d"), lCurVelocity);
	m_wndVelocityCombo.SetWindowText (strText);
	return TRUE;
}

// 現在の(音符の)長さ[ティック]を設定
BOOL CMusicalScoreFrame::SetCurDuration (long lCurDuration) {
	ASSERT (0 <= lCurDuration);
	CString strText;
	strText.Format (_T("%d"), lCurDuration);
	m_wndDurationCombo.SetWindowText (strText);
	return TRUE;
}

// 現在の表示精度[ティック]を設定
BOOL CMusicalScoreFrame::SetCurResolution (long lCurResolution) {
	ASSERT (0 <= lCurResolution);
	CString strText;
	strText.Format (_T("%d"), lCurResolution);
	m_wndResolutionCombo.SetWindowText (strText);
	return TRUE;
}

// 指定インデックスのトラックが表示状態か調べる
BOOL CMusicalScoreFrame::IsTrackVisible (long lTrackIndex) {
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
BOOL CMusicalScoreFrame::SetTrackVisible (long lTrackIndex) {
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



// トラックコンボの更新
BOOL CMusicalScoreFrame::UpdateTrackCombo () {
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
		MIDITrack_GetName (pMIDITrack, szTrackName1, TSIZEOF (szTrackName1) - 1);
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


// トラックリストボックスの更新
BOOL CMusicalScoreFrame::UpdateTrackList () {
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
		MIDITrack_GetName (pMIDITrack, szTrackName1, TSIZEOF (szTrackName1) - 1);
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


// スナップコンボボックスの更新
BOOL CMusicalScoreFrame::UpdateSnapCombo () {
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
BOOL CMusicalScoreFrame::UpdateVelocityCombo () {
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
BOOL CMusicalScoreFrame::UpdateDurationCombo () {
	CString strText;
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)GetDocument ();
	long lTimeResolution = MIDIData_GetTimeResolution (pSekaijuDoc->m_pMIDIData);
	//long lCurSel = m_wndDurationCombo.GetCurSel (); // 20100328廃止
	CString strCurText;
	m_wndDurationCombo.GetWindowText (strCurText); // 20100328追加
	long i = 0;
	long lCurSel = -1;
	for (i = 0; i < m_wndDurationCombo.GetCount (); i++) {
		CString strLBText;
		m_wndDurationCombo.GetLBText (i, strLBText);
		if (strLBText == strCurText) {
			lCurSel = i;
			break;
		}
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

// 表示精度コンボボックスの更新
BOOL CMusicalScoreFrame::UpdateResolutionCombo () {
	CString strText;
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)GetDocument ();
	long lTimeResolution = MIDIData_GetTimeResolution (pSekaijuDoc->m_pMIDIData);
	long lCurSel = m_wndResolutionCombo.GetCurSel ();
	m_wndResolutionCombo.ResetContent ();
	// 表示精度コンボボックスの充満
	CString strFormat;
	VERIFY (strFormat.LoadString (IDS_D_4DIVNOTE)); // %d-4分音符
	strText.Format (strFormat, lTimeResolution * 1);
	m_wndResolutionCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_8DIVNOTE)); // %d-8分音符
	strText.Format (strFormat, lTimeResolution / 2);
	m_wndResolutionCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_12DIVNOTE)); // %d-3連8分音符
	strText.Format (strFormat, lTimeResolution / 3);
	m_wndResolutionCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_16DIVNOTE)); // %d-16分音符
	strText.Format (strFormat, lTimeResolution / 4);
	m_wndResolutionCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_24DIVNOTE)); // %d-3連16分音符
	strText.Format (strFormat, lTimeResolution / 6);
	m_wndResolutionCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_32DIVNOTE)); // %d-32分音符
	strText.Format (strFormat, lTimeResolution / 8);
	m_wndResolutionCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_48DIVNOTE)); // %d-3連32分音符
	strText.Format (strFormat, lTimeResolution / 12);
	m_wndResolutionCombo.AddString (strText);
	// カレントセル設定
	if (lCurSel >= 0) {
		m_wndResolutionCombo.SetCurSel (lCurSel);
	}
	return TRUE;
}


// キースクロールバー(縦)のデザイン設定
void CMusicalScoreFrame::RecalcTrackScrollInfo () {
	long lTrackInfoCount = GetTrackInfoCount ();
	MusicalScoreTrackInfo* pLastTrackInfo = NULL;
	VERIFY (pLastTrackInfo = GetTrackInfo (lTrackInfoCount - 1));
	long lLastTrackTop = pLastTrackInfo->m_lTop;//GetTrackTop (lTrackInfoCount - 1);
	long lLastTrackHeight = pLastTrackInfo->m_lHeight;//GetTrackHeight (lTrackInfoCount - 1);
	SCROLLINFO si;
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	si.nMax = (lLastTrackTop + lLastTrackHeight) * m_lTrackZoom;
	si.nPage = m_lKeyHeight;
	m_wndTrackScroll.SetScrollInfo (&si, TRUE);
	m_lTrackScrollPos = m_wndTrackScroll.GetScrollPos ();
}


// タイムスクロールバー(横)のデザイン設定
void CMusicalScoreFrame::RecalcTimeScrollInfo () {
	long lMeasureInfoCount = m_theMeasureInfoArray.GetSize ();
	MusicalScoreMeasureInfo* pFirstMeasureInfo = NULL;
	VERIFY (pFirstMeasureInfo = GetMeasureInfo (0));
	long lStartX = pFirstMeasureInfo->m_lLeft + pFirstMeasureInfo->m_lSignatureWidth;
	MusicalScoreMeasureInfo* pLastMeasureInfo = NULL;
	VERIFY (pLastMeasureInfo = GetMeasureInfo (MAX (0, lMeasureInfoCount - 2)));
	long lEndX = pLastMeasureInfo->m_lLeft;
	SCROLLINFO si;
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = lStartX * m_lTimeZoom;
	si.nMax = lEndX * m_lTimeZoom;
	si.nPage = m_lTimeWidth;
	m_wndTimeScroll.SetScrollInfo (&si, TRUE);
	m_lTimeScrollPos = m_wndTimeScroll.GetScrollPos ();
	// 注：ズーム倍率1倍のとき4分音符の長さを8ピクセルと定義している。
}


// トラック情報配列削除(各トラック情報配列内の音符情報配列と音符グループ情報配列の削除を含む)
BOOL CMusicalScoreFrame::DeleteTrackInfoArray () {
	long i;
	long lNumTrackInfo = GetTrackInfoCount ();
	for (i = 0; i < lNumTrackInfo; i++) {
		MusicalScoreTrackInfo* pTrackInfo = GetTrackInfo (i);
		// トラック情報配列[i]内の3連符グループ情報配列削除
		VERIFY (DeleteTripletGroupInfoArray (i));
		// トラック情報配列[i]内の音符グループ情報配列削除
		VERIFY (DeleteNoteGroupInfoArray (i));
		// トラック情報配列[i]内の音符情報配列削除
		VERIFY (DeleteNoteInfoArray (i));
		// トラック情報配列[i]を削除
		delete (pTrackInfo);
	}
	m_theTrackInfoArray.RemoveAll ();
	return TRUE;
}

// トラック情報配列[lTrackIndex]内の音符情報配列削除
BOOL CMusicalScoreFrame::DeleteNoteInfoArray (long lTrackIndex) {
	long lNumTrackInfo = GetTrackInfoCount ();
	ASSERT (0 <= lTrackIndex && lTrackIndex < lNumTrackInfo);
	MusicalScoreTrackInfo* pTrackInfo = GetTrackInfo (lTrackIndex);
	ASSERT (pTrackInfo);
	long j;
	long lNumNoteInfo = pTrackInfo->m_theNoteInfoArray.GetSize ();
	for (j = 0; j < lNumNoteInfo; j++) {
		delete (pTrackInfo->m_theNoteInfoArray.GetAt (j));
	}
	pTrackInfo->m_theNoteInfoArray.RemoveAll ();
	return TRUE;
}

// トラック情報配列[lTrackIndex]内の音符グループ情報配列削除
BOOL CMusicalScoreFrame::DeleteNoteGroupInfoArray (long lTrackIndex) {
	long lNumTrackInfo = GetTrackInfoCount ();
	ASSERT (0 <= lTrackIndex && lTrackIndex < lNumTrackInfo);
	MusicalScoreTrackInfo* pTrackInfo = GetTrackInfo (lTrackIndex);
	ASSERT (pTrackInfo);
	long j;
	long lNumNoteGroupInfo = pTrackInfo->m_theNoteGroupInfoArray.GetSize ();
	for (j = 0; j < lNumNoteGroupInfo; j++) {
		delete (pTrackInfo->m_theNoteGroupInfoArray.GetAt (j));
	}
	pTrackInfo->m_theNoteGroupInfoArray.RemoveAll ();
	return TRUE;
}

// トラック情報配列[lTrackIndex]内の3連音符グループ情報配列削除
BOOL CMusicalScoreFrame::DeleteTripletGroupInfoArray (long lTrackIndex) {
	long lNumTrackInfo = GetTrackInfoCount ();
	ASSERT (0 <= lTrackIndex && lTrackIndex < lNumTrackInfo);
	MusicalScoreTrackInfo* pTrackInfo = GetTrackInfo (lTrackIndex);
	ASSERT (pTrackInfo);
	long j;
	long lNumTripletGroupInfo = pTrackInfo->m_theTripletGroupInfoArray.GetSize ();
	for (j = 0; j < lNumTripletGroupInfo; j++) {
		delete (pTrackInfo->m_theTripletGroupInfoArray.GetAt (j));
	}
	pTrackInfo->m_theTripletGroupInfoArray.RemoveAll ();
	return TRUE;
}

// 小節情報配列削除
BOOL CMusicalScoreFrame::DeleteMeasureInfoArray () {
	long j;
	long lNumMeasureInfo = GetMeasureInfoCount ();
	for (j = 0; j < lNumMeasureInfo; j++) {
		free (m_theMeasureInfoArray.GetAt (j));
	}
	m_theMeasureInfoArray.RemoveAll ();
	return TRUE;
}

// 小節情報配列更新
BOOL CMusicalScoreFrame::UpdateMeasureInfoArray () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);

	// 小節情報配列をいったん強制削除
	VERIFY (DeleteMeasureInfoArray ());

	long lEndMeasure, lEndBeat, lEndTick;
	long lEndTime = MIDIData_GetEndTime (pMIDIData);
	long lEndTimeM = lEndTime;
	MIDIData_BreakTime (pMIDIData, lEndTime, &lEndMeasure, &lEndBeat, &lEndTick);
	if (lEndBeat != 0 || lEndTick != 0) {
		MIDIData_MakeTime (pMIDIData, lEndMeasure + 1, 0, 0, &lEndTimeM);
	}
	long lFeedTime = lTimeResolution * 4  * 120; // 4/4で120小節の余長
	lEndTime = CLIP (0, lEndTimeM + lFeedTime, 0x7FFFFFFF);
	MIDIData_BreakTime (pMIDIData, lEndTime, &lEndMeasure, &lEndBeat, &lEndTick);

	long lMeasure;
	long lOldTimeSignature = 4 | (2 << 8);
	long lOldKeySignature = 0 | (0 << 8);
	long lNextMeasureLeft = 0;
	long lCurMeasureTime = 0;
	long lNextMeasureTime = 0;
	for (lMeasure = 0; lMeasure < lEndMeasure; lMeasure++) {
		MusicalScoreMeasureInfo* pMeasureInfo = new MusicalScoreMeasureInfo ();
		if (pMeasureInfo == NULL) {
			break;
		}
		// 左座標[pixel]
		long lMeasureLeft = lNextMeasureLeft;
		pMeasureInfo->m_lLeft = lNextMeasureLeft;
		// 開始時刻[Tick][Subframe]
		pMeasureInfo->m_lTime = lCurMeasureTime;
		// 長さ[Tick][Subframe]
		MIDIData_MakeTime (pMIDIData, lMeasure + 1, 0, 0, &lNextMeasureTime);
		pMeasureInfo->m_lDuration = lNextMeasureTime - lCurMeasureTime;
		// 拍子記号
		long lnn, ldd, lcc, lbb;
		MIDIData_FindTimeSignature (pMIDIData, lCurMeasureTime, &lnn, &ldd, &lcc, &lbb);
		long lTimeSignature = ((lnn & 0xFF) | ((ldd & 0xFF) << 8));
		pMeasureInfo->m_lTimeSignature = lTimeSignature;
		// 調性記号
		long lsf, lmi;
		MIDIData_FindKeySignature (pMIDIData, lCurMeasureTime, &lsf, &lmi);
		long lKeySignature = ((lsf & 0xFF) | ((lmi & 0xFF) << 8));
		pMeasureInfo->m_lKeySignature = lKeySignature;
		// 拍子記号/調性記号用幅[pixel]
		if (lKeySignature != lOldKeySignature ||
			lTimeSignature != lOldTimeSignature ||
			lMeasure == 0) { // 同じ記号が入っていても描画するので要調整
			pMeasureInfo->m_lSignatureWidth = 16;
		}
		else {
			pMeasureInfo->m_lSignatureWidth = 0;			
		}
		// 左余白幅[pixel]
		pMeasureInfo->m_lPreWidth = 4;
		// 小節音符部幅[pixel]
		// 注：ズーム倍率1倍のとき4分音符の長さを8ピクセルと定義している。
		pMeasureInfo->m_lWidth = 8 * 4 * lnn / (1 << ldd);
		// 右余白幅[pixel]
		pMeasureInfo->m_lPostWidth = 0;
		// フラグ
		pMeasureInfo->m_lFlags = 0;
		// 印刷用変数の仮設定
		pMeasureInfo->m_lLeftPrint = pMeasureInfo->m_lLeft;
		pMeasureInfo->m_lSignatureWidthPrint = pMeasureInfo->m_lSignatureWidth;
		pMeasureInfo->m_lPreWidthPrint = pMeasureInfo->m_lPreWidth;
		pMeasureInfo->m_lWidthPrint = pMeasureInfo->m_lWidth;
		pMeasureInfo->m_lPostWidthPrint = pMeasureInfo->m_lPostWidth;
		// 小節情報構造体を配列に追加
		m_theMeasureInfoArray.Add (pMeasureInfo);
		// 次回のループに備える
		lCurMeasureTime = lNextMeasureTime;
		lNextMeasureLeft = 
			lMeasureLeft + 
			pMeasureInfo->m_lSignatureWidth + 
			pMeasureInfo->m_lPreWidth + 
			pMeasureInfo->m_lWidth + 
			pMeasureInfo->m_lPostWidth;
		lOldTimeSignature = lTimeSignature;
		lOldKeySignature = lKeySignature;
	}
	return TRUE;
}

// トラック情報配列更新(各トラック情報配列内の音符情報配列・音符グループ情報配列の更新含む)
BOOL CMusicalScoreFrame::UpdateTrackInfoArray () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;

	// トラック情報配列をいったん強制削除(各トラック情報配列内の音符情報配列・音符グループ情報配列の削除含む)
	VERIFY (DeleteTrackInfoArray ());

	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	long lNextTop = 0;
	// 各トラックの最大キー・最小キー取得およびト音記号・ヘ音記号・大譜表の識別
	long i = 0;
	forEachTrack (pMIDIData, pMIDITrack) {
		MusicalScoreTrackInfo* pTrackInfo = new MusicalScoreTrackInfo ();
		if (pTrackInfo == NULL) {
			break;
		}
		pTrackInfo->m_lTop = lNextTop;
		if (IsTrackVisible (i)) {
			// このトラックのノートイベントの最大キーと最小キーを求める。
			long lMaxKey = 0;
			long lMinKey = 127;
			long lCount = 0;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsNoteOn (pMIDIEvent) || MIDIEvent_IsNoteOff (pMIDIEvent)) {
					long lKey = MIDIEvent_GetKey (pMIDIEvent);
					if (lKey < lMinKey) {
						lMinKey = lKey;
					}
					if (lKey > lMaxKey) {
						lMaxKey = lKey;
					}
					lCount++;
				}
			}
			// 音符が存在しない(ト音記号とする)
			if (lMinKey == 127 && lMaxKey == 0) {
				pTrackInfo->m_lFlags = 1;
				pTrackInfo->m_lHeight = 24;
			}
			// ト音記号
			else if (lMinKey >= 60 && lCount > 0) {
				pTrackInfo->m_lFlags = 1;
				pTrackInfo->m_lHeight = 24;
			}
			// ヘ音記号
			else if (lMaxKey <= 60 && lCount > 0) {
				pTrackInfo->m_lFlags = 2;
				pTrackInfo->m_lHeight = 24;
			}
			// 大譜表

			else {
				pTrackInfo->m_lFlags = 3;
				pTrackInfo->m_lHeight = 36;
			}
		}
		else {
			// 表示しない
			pTrackInfo->m_lFlags = 0;
			pTrackInfo->m_lHeight = 0;
		}
		// 印刷用変数の仮設定
		pTrackInfo->m_lTopPrint = pTrackInfo->m_lTop;
		pTrackInfo->m_lHeightPrint = pTrackInfo->m_lHeight;

		m_theTrackInfoArray.Add (pTrackInfo);
		lNextTop = pTrackInfo->m_lTop + pTrackInfo->m_lHeight;
		i++;
	}

	i = 0;
	forEachTrack (pMIDIData, pMIDITrack) {  
		if (IsTrackVisible (i)) {
			// トラック情報配列[i]内の音符情報配列を更新
			UpdateNoteInfoArray (i, pMIDITrack);
			// トラック情報配列[i]内の音符グループ情報配列を更新
			UpdateNoteGroupInfoArray (i, pMIDITrack);
			// トラック情報配列[i]内の3連符グループ情報配列を更新
			UpdateTripletGroupInfoArray (i, pMIDITrack);
		}
		i++;
	}

	return TRUE;
}

// トラック情報配列[lTrackIndex]内の音符情報配列を更新
BOOL CMusicalScoreFrame::UpdateNoteInfoArray (long lTrackIndex, MIDITrack* pMIDITrack) {
	long lNumTrackInfoArray = GetTrackInfoCount ();
	ASSERT (0 <= lTrackIndex && lTrackIndex < lNumTrackInfoArray);
	ASSERT (pMIDITrack);
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDIEvent* pMIDIEvent = NULL;
	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);

	// トラック情報配列[lTrackIndex]内の音符情報配列をいったん削除
	VERIFY (DeleteNoteInfoArray (lTrackIndex));

	// SMPTEベースでは対応しない。
	if (lTimeMode != MIDIDATA_TPQNBASE) {
		return TRUE;
	}

	// 現在の表示精度[ティック]を取得
	// 0=4分音符,1=8分音符,2=3連8分音符,3=16分音符,4=3連16分音符,5=32分音符,6=3連32分音符
	long lViewResolutionIndex = m_wndResolutionCombo.GetCurSel ();
	long lViewResolution = lTimeResolution; // 普通音符用表示解像度
	long lViewResolution3 = lTimeResolution; // 3連音符用表示解像度
	switch (lViewResolutionIndex) {
	case 0: // 4分音符
		lViewResolution = lTimeResolution;
		lViewResolution3 = lTimeResolution;
		break;
	case 1: // 8分音符
		lViewResolution = lTimeResolution / 2;
		lViewResolution3 = lTimeResolution / 2;
		break;
	case 2: // 3連8分音符
		lViewResolution = lTimeResolution / 2;
		lViewResolution3 = lTimeResolution / 3;
		break;
	case 3: // 16分音符
		lViewResolution = lTimeResolution / 4;
		lViewResolution3 = lTimeResolution / 3;
		break;
	case 4: // 3連16分音符
		lViewResolution = lTimeResolution / 4;
		lViewResolution3 = lTimeResolution / 6;
		break;
	case 5: // 32分音符
		lViewResolution = lTimeResolution / 8;
		lViewResolution3 = lTimeResolution / 6;
		break;
	case 6: // 3連32分音符
		lViewResolution = lTimeResolution / 8;
		lViewResolution3 = lTimeResolution / 12;
		break;
	}
	if (lViewResolution <= 0) {
		lViewResolution = 1;
	}
	if (lViewResolution3 <= 0) {
		lViewResolution = 1;
	}

	// 表現可能な音符の長さ
	long lDur960 = lTimeResolution * 8;
	long lDur840 = lTimeResolution * 7;
	long lDur720 = lTimeResolution * 6;
	long lDur600 = lTimeResolution * 5;
	long lDur480 = lTimeResolution * 4; // 全音符
	long lDur360 = lTimeResolution * 3; // 付点2分音符
	long lDur240 = lTimeResolution * 2; // 2分音符
	long lDur180 = lTimeResolution * 3 / 2; // 付点4分音符
	long lDur120 = lTimeResolution; // 4分音符
	long lDur90 = lTimeResolution * 3 / 4; // 付点8分音符
	long lDur80 = lTimeResolution * 2 / 3; // 3連4分音符
	long lDur60 = lTimeResolution / 2; // 8分音符
	long lDur45 = lTimeResolution * 3 / 8; // 付点16分音符
	long lDur40 = lTimeResolution / 3; // 3連8分音符
	long lDur30 = lTimeResolution / 4; // 16分音符
	long lDur20 = lTimeResolution / 6; // 3連16分音符
	long lDur15 = lTimeResolution / 8; // 32分音符
	long lDur10 = lTimeResolution / 12; // 3連32分音符

	// 補正ノートオンタイムを計算し、m_lUser1に格納
	forEachEvent (pMIDITrack, pMIDIEvent) {
		if (MIDIEvent_IsNoteOn (pMIDIEvent) && MIDIEvent_IsNote (pMIDIEvent)) {
			long lInTime = MIDIEvent_GetTime (pMIDIEvent);
			long lOutTime = lInTime + MIDIEvent_GetDuration (pMIDIEvent);
			// レゾ=例:16分音符でクォンタイズ
			if (lInTime % (lViewResolution3) == 0) { // 3連n分音符(例:3連16分音符の倍数)	
				pMIDIEvent->m_lUser1 = lInTime;
			}
			else { // 通常n分音符又は付点n分音符(例:16分音符の倍数)
				pMIDIEvent->m_lUser1 = 
					((lInTime + lViewResolution / 2 - 1) / lViewResolution) * (lViewResolution);
			}
		}
	}

	// 補正ノートオフタイムを計算し、m_lUser2に格納
	forEachEvent (pMIDITrack, pMIDIEvent) {
		if (MIDIEvent_IsNoteOn (pMIDIEvent) && MIDIEvent_IsNote (pMIDIEvent)) {
			// このノートイベントの補正後開始時刻を取得
			long lNoteOnTime, lNoteOnMeasure, lNoteOnBeat, lNoteOnTick;
			lNoteOnTime = pMIDIEvent->m_lUser1;
			MIDIData_BreakTime (pMIDIData, lNoteOnTime, &lNoteOnMeasure, &lNoteOnBeat, &lNoteOnTick);
			// このノートイベントの正式な終了時刻を取得
			long lNoteOffTime, lNoteOffMeasure, lNoteOffBeat, lNoteOffTick;
			lNoteOffTime = MIDIEvent_GetTime (pMIDIEvent->m_pNextCombinedEvent);
			MIDIData_BreakTime (pMIDIData, lNoteOffTime, &lNoteOffMeasure, &lNoteOffBeat, &lNoteOffTick);
			// 次の時刻のノートイベントの補正後開始時刻を取得
			long lNextOnTime, lNextOnMeasure, lNextOnBeat, lNextOnTick;
			MIDIEvent* pNextEvent = MIDIEvent_GetNextEvent (pMIDIEvent);
			while (pNextEvent) {
				if (MIDIEvent_IsNoteOn (pNextEvent) && MIDIEvent_IsNote (pNextEvent)) {
					if (pNextEvent->m_lUser1 > pMIDIEvent->m_lUser1) {
						break;
					}
				}
				pNextEvent = MIDIEvent_GetNextEvent (pNextEvent);
			}
			if (pNextEvent) {
				lNextOnTime = pNextEvent->m_lUser1;
				MIDIData_BreakTime (pMIDIData, lNextOnTime, &lNextOnMeasure, &lNextOnBeat, &lNextOnTick);
				if (lNextOnMeasure > lNoteOffMeasure) {
					lNextOnTime = GetMeasureTime (lNoteOffMeasure + 1);
				}
			}
			else {
				lNextOnTime = GetMeasureTime (lNoteOnMeasure + 1);
			}
			// 3連n分音符の場合
			if (lNoteOnTime % lViewResolution3 == 0 &&
				(lNextOnTime - lNoteOnTime) % lViewResolution != 0) {
				pMIDIEvent->m_lUser2 = 
					((lNoteOffTime + lViewResolution3 / 2 - 1) / lViewResolution3) * lViewResolution3;
				if (pMIDIEvent->m_lUser2 - pMIDIEvent->m_lUser1 < lViewResolution3) {
					pMIDIEvent->m_lUser2 = pMIDIEvent->m_lUser1 + lViewResolution3; // (例)フィル3連16分音符
				}
				
			}
			// 通常n分音符又は付点n分音符の場合
			else { 
				pMIDIEvent->m_lUser2 = 
					((lNoteOffTime + lViewResolution / 2 - 1) / lViewResolution) * lViewResolution;
				if (pMIDIEvent->m_lUser2 - pMIDIEvent->m_lUser1 < lViewResolution) {
					pMIDIEvent->m_lUser2 = pMIDIEvent->m_lUser1 + lViewResolution; // (例)フィル16分音符
				}
			}
		}
	}

	// 音符情報の生成と配列への登録
	MusicalScoreTrackInfo* pTrackInfo = GetTrackInfo (lTrackIndex);
	forEachEvent (pMIDITrack, pMIDIEvent) {
		if (MIDIEvent_IsNoteOn (pMIDIEvent) && MIDIEvent_IsNote (pMIDIEvent)) {
			long lNoteOnMeasure, lNoteOnBeat, lNoteOnTick;
			long lNoteOffMeasure, lNoteOffBeat, lNoteOffTick;

			MIDIData_BreakTime (pMIDIData, pMIDIEvent->m_lUser1, &lNoteOnMeasure, &lNoteOnBeat, &lNoteOnTick);
			MIDIData_BreakTime (pMIDIData, pMIDIEvent->m_lUser2, &lNoteOffMeasure, &lNoteOffBeat, &lNoteOffTick);

			long lMeasure;
			// このノートイベントの各所属小節について
			for (lMeasure = lNoteOnMeasure; lMeasure <= lNoteOffMeasure; lMeasure++) {
				MusicalScoreMeasureInfo* pMeasureInfo = GetMeasureInfo (lMeasure);
				long lMeasureTime =  pMeasureInfo->m_lTime; // この小節の開始タイム[ティック]
				long lnn = (pMeasureInfo->m_lTimeSignature & 0xFF); // 分子(この小節の拍数)[拍]
				long ldd = (pMeasureInfo->m_lTimeSignature & 0xFF00) >> 8; // 分母
				long lBeatDur = 4 * lTimeResolution / (0x01 << ldd); // 1拍の長さ[ティック]
				long lNextMeasureTime = lMeasureTime + lBeatDur * lnn; // 次の小節の開始タイム[ティック]
				if (pMIDIEvent->m_lUser2 - lMeasureTime <= 0) {
					break;
				}
				// パターンA:この小節で始まりこの小節で終わる
				if (lMeasure == lNoteOnMeasure && 
					(lMeasure == lNoteOffMeasure || (lMeasure == lNoteOffMeasure - 1 && lNoteOffBeat == 0 && lNoteOffTick == 0))) {
					long lDur = pMIDIEvent->m_lUser2 - pMIDIEvent->m_lUser1;
					// 通常音符で表現できない場合
					if (lDur != lDur960 &&
						lDur != lDur840 &&
						lDur != lDur720 &&
						lDur != lDur600 &&
						lDur != lDur480 &&
						lDur != lDur360 &&
						lDur != lDur240 && 
						lDur != lDur180 &&
						lDur != lDur120 &&
						lDur != lDur90 &&
						lDur != lDur80 &&
						lDur != lDur60 &&
						lDur != lDur45 &&
						lDur != lDur40 &&
						lDur != lDur30 &&
						lDur != lDur20 &&
						lDur != lDur15 &&
						lDur != lDur10) {
						long lDivideTime1 = 0;
						long lDivideMeasure1 = 0;
						long lDivideBeat1 = 0;
						long lDivideTick1 = 0;
						long lDivideTime2 = 0;
						long lDivideMeasure2 = 0;
						long lDivideBeat2 = 0;
						long lDivideTick2 = 0;
						// a)最初の拍に半端があり、次の拍にまたがる場合
						if (lNoteOnTick > 0 && (lDur > lBeatDur || (lNoteOnBeat != lNoteOffBeat && lNoteOffTick > 0 ))) {
							lDivideTime1 = lMeasureTime + (lNoteOnBeat + 1) * lBeatDur;
							lDivideMeasure1 = lMeasure;
							lDivideBeat1 = lNoteOnBeat + 1;
							lDivideTick1 = 0;
						}
						// b)最後の拍に半端があり、前の拍からつながる場合
						if (lNoteOnBeat != lNoteOffBeat && lNoteOffTick > 0) {
							lDivideTime2 = lMeasureTime + (lNoteOffBeat) * lBeatDur;
							lDivideMeasure2 = lMeasure;
							lDivideBeat2 = lNoteOffBeat;
							lDivideTick2 = 0;
						}
						// c)最初の拍又は最後の拍につながるべき半端はない場合
						if (lDivideTime1 == lDivideTime2) {
							// 4分音符より短く、既存音符では表現できない長さの場合、8分音符+残りで表現
							if (lDur < lDur120) {
								lDivideTime2 = pMIDIEvent->m_lUser1 + lDur60;
								MIDIData_BreakTime (pMIDIData, lDivideTime2, &lDivideMeasure2, &lDivideBeat2, &lDivideTick2);
							}
							// 2分音符より短く、既存音符では表現できない長さの場合、4分音符+残りで表現
							else if (lDur < lDur240) {
								lDivideTime2 = pMIDIEvent->m_lUser1 + lDur120;
								MIDIData_BreakTime (pMIDIData, lDivideTime2, &lDivideMeasure2, &lDivideBeat2, &lDivideTick2);
							}
							// 付点2分音符より短く、既存音符では表現できない長さの場合、2分音符+残りで表現
							else if (lDur < lDur360) {
								lDivideTime2 = pMIDIEvent->m_lUser1 + lDur240;
								MIDIData_BreakTime (pMIDIData, lDivideTime2, &lDivideMeasure2, &lDivideBeat2, &lDivideTick2);
							}
							// 1分音符より短く、既存音符では表現できない長さの場合、付点2分音符+残りで表現
							else if (lDur < lDur480) {
								lDivideTime2 = pMIDIEvent->m_lUser1 + lDur360;
								MIDIData_BreakTime (pMIDIData, lDivideTime2, &lDivideMeasure2, &lDivideBeat2, &lDivideTick2);
							}
							// その他の場合、分割をあきらめ、最も近い音符で表現。
							else {
								lDivideTime2 = 0;
							}
						}
						// 拍境界分割なし(音符1つ)
						if (lDivideTime1 == 0 && lDivideTime2 == 0) {
							MusicalScoreNoteInfo* pNoteInfo1 = CreateNote
								(pMIDIEvent, pMIDIEvent->m_lUser1, lNoteOnMeasure, lNoteOnBeat, lNoteOnTick,
								pMIDIEvent->m_lUser2, lNoteOffMeasure, lNoteOffBeat, lNoteOffTick, 0x00000000);
							AddNoteInfo (pTrackInfo, pNoteInfo1);
						}
						// 最初の拍境界で分割(音符2つ)
						else if (lDivideTime1 != 0 && lDivideTime2 == 0) {
							MusicalScoreNoteInfo* pNoteInfo1 = CreateNote
								(pMIDIEvent, pMIDIEvent->m_lUser1, lNoteOnMeasure, lNoteOnBeat, lNoteOnTick,
								lDivideTime1, lDivideMeasure1, lDivideBeat1, lDivideTick1, 0x00000001);
							AddNoteInfo (pTrackInfo, pNoteInfo1);
							MusicalScoreNoteInfo* pNoteInfo2 = CreateNote
								(pMIDIEvent, lDivideTime1, lDivideMeasure1, lDivideBeat1, lDivideTick1,
								pMIDIEvent->m_lUser2, lNoteOffMeasure, lNoteOffBeat, lNoteOffTick, 0x00000002);
							AddNoteInfo (pTrackInfo, pNoteInfo2);
						}
						// 最後の拍境界で分割(音符2つ)
						else if (lDivideTime1 == 0 && lDivideTime2 != 0) {
							MusicalScoreNoteInfo* pNoteInfo1 = CreateNote
								(pMIDIEvent, pMIDIEvent->m_lUser1, lNoteOnMeasure, lNoteOnBeat, lNoteOnTick,
								lDivideTime2, lDivideMeasure2, lDivideBeat2, lDivideTick2, 0x00000001);
							AddNoteInfo (pTrackInfo, pNoteInfo1);
							MusicalScoreNoteInfo* pNoteInfo2 = CreateNote
								(pMIDIEvent, lDivideTime2, lDivideMeasure2, lDivideBeat2, lDivideTick2,
								pMIDIEvent->m_lUser2, lNoteOffMeasure, lNoteOffBeat, lNoteOffTick, 0x00000002);
							AddNoteInfo (pTrackInfo, pNoteInfo2);
						}
						// 最初の拍境界と最後の拍境界で分割(音符3つ)
						else if (lDivideTime1 != 0 && lDivideTime2 != 0) {
							MusicalScoreNoteInfo* pNoteInfo1 = CreateNote
								(pMIDIEvent, pMIDIEvent->m_lUser1, lNoteOnMeasure, lNoteOnBeat, lNoteOnTick,
								lDivideTime1,lDivideMeasure1, lDivideBeat1, lDivideTick1, 0x00000001);
							AddNoteInfo (pTrackInfo, pNoteInfo1);
							MusicalScoreNoteInfo* pNoteInfo2 = CreateNote
								(pMIDIEvent, lDivideTime1, lDivideMeasure1, lDivideBeat1, lDivideTick1,
								lDivideTime2,lDivideMeasure2, lDivideBeat2, lDivideTick2, 0x00000003);
							AddNoteInfo (pTrackInfo, pNoteInfo2);
							MusicalScoreNoteInfo* pNoteInfo3 = CreateNote
								(pMIDIEvent, lDivideTime2, lDivideMeasure2, lDivideBeat2, lDivideTick2,
								pMIDIEvent->m_lUser2, lNoteOffMeasure, lNoteOffBeat, lNoteOffTick, 0x00000002);
							AddNoteInfo (pTrackInfo, pNoteInfo3);
						}
					}
					// 通常の音符で表現できる場合
					else {
						MusicalScoreNoteInfo* pNoteInfo = CreateNote
							(pMIDIEvent, pMIDIEvent->m_lUser1, lNoteOnMeasure, lNoteOnBeat, lNoteOnTick,
							pMIDIEvent->m_lUser2, lNoteOffMeasure, lNoteOffBeat, lNoteOffTick, 0x00000000);
						AddNoteInfo (pTrackInfo, pNoteInfo);
					}
				}
				// パターンB:この小節から始まり次の小節へタイで続く
				else if (lMeasure == lNoteOnMeasure && lMeasure < lNoteOffMeasure) {
					long lDur = lNextMeasureTime - pMIDIEvent->m_lUser1;
					// 拍境界をまたいでおり、通常音符で表現できない場合
					if (lNoteOnBeat < lnn - 1 &&
						lDur != lDur960 &&
						lDur != lDur840 &&
						lDur != lDur720 &&
						lDur != lDur600 &&
						lDur != lDur480 &&
						lDur != lDur360 &&
						lDur != lDur240 && 
						lDur != lDur180 &&
						lDur != lDur120 &&
						lDur != lDur90 &&
						lDur != lDur80 &&
						lDur != lDur60 &&
						lDur != lDur45 &&
						lDur != lDur40 &&
						lDur != lDur30 &&
						lDur != lDur20 &&
						lDur != lDur15 &&
						lDur != lDur10) {
						long lDivideTime = lMeasureTime + (lNoteOnBeat + 1) * lBeatDur;
						long lDivideMeasure = lMeasure;
						long lDivideBeat = lNoteOnBeat + 1;
						long lDivideTick = 0;
						MusicalScoreNoteInfo* pNoteInfo1 = CreateNote
							(pMIDIEvent, pMIDIEvent->m_lUser1, lNoteOnMeasure, lNoteOnBeat, lNoteOnTick, 
							lDivideTime, lDivideMeasure, lDivideBeat, lDivideTick, 0x00000001);
						AddNoteInfo (pTrackInfo, pNoteInfo1);
						MusicalScoreNoteInfo* pNoteInfo2 = CreateNote
							(pMIDIEvent, lDivideTime, lDivideMeasure, lDivideBeat, lDivideTick, 
							lNextMeasureTime, lMeasure + 1, 0, 0, 0x00000006);
						AddNoteInfo (pTrackInfo, pNoteInfo2);
					}
					// 音符ひとつで表現できる場合
					else {
						MusicalScoreNoteInfo* pNoteInfo = CreateNote
							(pMIDIEvent, pMIDIEvent->m_lUser1, lNoteOnMeasure, lNoteOnBeat, lNoteOnTick, 
							lNextMeasureTime, lNoteOnMeasure + 1, 0, 0, 0x00000004);
						AddNoteInfo (pTrackInfo, pNoteInfo);
					}
				}
				// パターンC:前の小節からタイで続きこの小節で終了する
				else if (lMeasure > lNoteOnMeasure && 
					(lMeasure == lNoteOffMeasure || (lMeasure == lNoteOffMeasure - 1 && lNoteOffBeat == 0 && lNoteOffTick == 0))) {
					long lDur = pMIDIEvent->m_lUser2 - lMeasureTime;
					// 拍境界をまたいでおり、通常音符で表現できない場合
					if (lNoteOffBeat > 0 &&
						lDur != lDur960 &&
						lDur != lDur840 &&
						lDur != lDur720 &&
						lDur != lDur600 &&
						lDur != lDur480 &&
						lDur != lDur360 &&
						lDur != lDur240 && 
						lDur != lDur180 &&
						lDur != lDur120 &&
						lDur != lDur90 &&
						lDur != lDur80 &&
						lDur != lDur60 &&
						lDur != lDur45 &&
						lDur != lDur40 &&
						lDur != lDur30 &&
						lDur != lDur20 &&
						lDur != lDur15 &&
						lDur != lDur10) {
						long lDivideTime = lMeasureTime + (lNoteOffBeat) * lBeatDur;
						long lDivideMeasure = lMeasure;
						long lDivideBeat = lNoteOffBeat;
						long lDivideTick = 0;
						MusicalScoreNoteInfo* pNoteInfo1 = CreateNote
							(pMIDIEvent, lMeasureTime, lMeasure, 0, 0,
							lDivideTime, lDivideMeasure, lDivideBeat, lDivideTick, 0x00000007);
						AddNoteInfo (pTrackInfo, pNoteInfo1);
						MusicalScoreNoteInfo* pNoteInfo2 = CreateNote
							(pMIDIEvent, lDivideTime, lDivideMeasure, lDivideBeat, lDivideTick, 
							pMIDIEvent->m_lUser2, lNoteOffMeasure, lNoteOffBeat, lNoteOffTick, 0x00000002);
						AddNoteInfo (pTrackInfo, pNoteInfo2);
					}
					// 音符ひとつで表現できる場合
					else {
						MusicalScoreNoteInfo* pNoteInfo = CreateNote
							(pMIDIEvent, lMeasureTime, lMeasure, 0, 0,
							pMIDIEvent->m_lUser2, lNoteOffMeasure, lNoteOffBeat, lNoteOffTick, 0x00000005);
						AddNoteInfo (pTrackInfo, pNoteInfo);
					}

				}
				// パターンD:前の小節からタイで続き次の小節へタイで続く(全音符のみ)
				else if (lMeasure > lNoteOnMeasure && lMeasure < lNoteOffMeasure) {
					MusicalScoreNoteInfo* pNoteInfo = CreateNote 
						(pMIDIEvent, lMeasureTime, lMeasure, 0, 0,
						lNextMeasureTime, lMeasure + 1, 0, 0, 0x00000008);
					AddNoteInfo (pTrackInfo, pNoteInfo);
				}
			}
		}
	}

	// 音符情報のポインタ連結
	long lNumNoteInfo = pTrackInfo->m_theNoteInfoArray.GetSize ();
	long j;
	for (j = 0; j < lNumNoteInfo; j++) {
		MusicalScoreNoteInfo* pNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j));
		if (j > 0) {
			pNoteInfo->m_pPrevNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j - 1));
		}
		else {
			pNoteInfo->m_pPrevNoteInfo = NULL;
		}
		if (j < lNumNoteInfo - 1) {
			pNoteInfo->m_pNextNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j + 1));
		}
		else {
			pNoteInfo->m_pNextNoteInfo = NULL;
		}
	}

	// 補正ノートオンタイムと補正ノートオフタイムのゼロリセット
	forEachEvent (pMIDITrack, pMIDIEvent) {
		if (MIDIEvent_IsNoteOn (pMIDIEvent) && MIDIEvent_IsNote (pMIDIEvent)) {
			pMIDIEvent->m_lUser1 = 0;
			pMIDIEvent->m_lUser2 = 0;
		}
	}
	return TRUE;
}

// トラック情報配列[lTrackIndex]内の音符グループ情報配列を更新
BOOL CMusicalScoreFrame::UpdateNoteGroupInfoArray (long lTrackIndex, MIDITrack* pMIDITrack) {
	long lNumTrackInfoArray = GetTrackInfoCount ();
	ASSERT (0 <= lTrackIndex && lTrackIndex < lNumTrackInfoArray);
	ASSERT (pMIDITrack);
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);

	// 表現可能な音符の長さ
	long lDur960 = lTimeResolution * 8;
	long lDur840 = lTimeResolution * 7;
	long lDur720 = lTimeResolution * 6;
	long lDur600 = lTimeResolution * 5;
	long lDur480 = lTimeResolution * 4; // 全音符
	long lDur360 = lTimeResolution * 3; // 付点2分音符
	long lDur240 = lTimeResolution * 2; // 2分音符
	long lDur180 = lTimeResolution * 3 / 2; // 付点4分音符
	long lDur120 = lTimeResolution; // 4分音符
	long lDur90 = lTimeResolution * 3 / 4; // 付点8分音符
	long lDur80 = lTimeResolution * 2 / 3; // 3連4分音符
	long lDur60 = lTimeResolution / 2; // 8分音符
	long lDur45 = lTimeResolution * 3 / 8; // 付点16分音符
	long lDur40 = lTimeResolution / 3; // 3連8分音符
	long lDur30 = lTimeResolution / 4; // 16分音符
	long lDur20 = lTimeResolution / 6; // 3連16分音符
	long lDur15 = lTimeResolution / 8; // 32分音符
	long lDur10 = lTimeResolution / 12; // 3連32分音符

	// トラック情報配列[lTrackIndex]内の音符グループ情報配列をいったん削除
	VERIFY (DeleteNoteGroupInfoArray (lTrackIndex));

	// 音符グループ情報の生成と配列への登録
	MusicalScoreTrackInfo* pTrackInfo = GetTrackInfo (lTrackIndex);
	long lNumNoteInfo = pTrackInfo->m_theNoteInfoArray.GetSize ();
	long j;
	MusicalScoreNoteGroupInfo* pNoteGroupInfo = NULL;
	BOOL bGroupContinueFlag = FALSE;
	for (j = 0; j < lNumNoteInfo; j++) {
		MusicalScoreNoteInfo* pNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j));
		long lDur = pNoteInfo->m_lNoteOffTime - pNoteInfo->m_lNoteOnTime;
		// この音符は旗付けが必要である場合
		if (lDur < lDur120 && lDur != lDur80) {
			// この音符がまだどこのグループにも所属していない場合
			if (pNoteInfo->m_pNoteGroupInfo == NULL) {
				// 新しい音符グループ情報を生成する必要がある場合
				if (bGroupContinueFlag == FALSE) {
					pNoteGroupInfo = 
						CreateNoteGroupInfo (pNoteInfo);
					AddNoteGroupInfo (pTrackInfo, pNoteGroupInfo);
				}
				pNoteInfo->m_pNoteGroupInfo = pNoteGroupInfo;
				pNoteGroupInfo->m_pLastNoteInfo = pNoteInfo;
				// 次の音符がある場合
				if (j + 1 < lNumNoteInfo) {
					MusicalScoreNoteInfo* pNextNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j + 1));
					// 次の音符のオンタイムが現在の音符のオフタイムと離れている場合	
					// 次の音符のオンタイムが現在の音符のオンタイムと同一でない場合
					if (pNoteInfo->m_lNoteOffTime != pNextNoteInfo->m_lNoteOnTime &&
						pNoteInfo->m_lNoteOnTime != pNextNoteInfo->m_lNoteOnTime) {
						bGroupContinueFlag = FALSE; // 現在の音符グループは終了
					}
					// 次の音符の所属拍が現在の音符の所属拍と異なる場合
					else if (pNoteInfo->m_lNoteOnBeat != pNextNoteInfo->m_lNoteOnBeat ||
						pNoteInfo->m_lNoteOnMeasure != pNextNoteInfo->m_lNoteOnMeasure) {
						bGroupContinueFlag = FALSE; // 現在の音符グループは終了
					}
					// 次の音符が8分音符を超えるの長さの場合
					else if (pNextNoteInfo->m_lNoteOffTime - pNextNoteInfo->m_lNoteOnTime > lTimeResolution / 2) {
						bGroupContinueFlag = FALSE; // 現在の音符グループは終了
					}
					else {
						bGroupContinueFlag = TRUE; // 現在の音符グループを続ける
					}
				}
				// 次の音符がない場合
				else {
					bGroupContinueFlag = FALSE;
				}
				// グループ内最大キー・最小キーの更新
				long lKey = MIDIEvent_GetKey (pNoteInfo->m_pNoteOnEvent);
				if (pNoteGroupInfo->m_lMaxKey < lKey) {
					pNoteGroupInfo->m_lMaxKey = lKey;
				}
				if (pNoteGroupInfo->m_lMinKey > lKey) {
					pNoteGroupInfo->m_lMinKey = lKey;
				}
				// グループ内の最大長さ・最小長さの更新
				long lDur = pNoteInfo->m_lNoteOffTime - pNoteInfo->m_lNoteOnTime;
				if (pNoteGroupInfo->m_lMaxDur < lDur) {
					pNoteGroupInfo->m_lMaxDur = lDur;
				}
				if (pNoteGroupInfo->m_lMinDur > lDur) {
					pNoteGroupInfo->m_lMinDur = lDur;
				}
				// 音符グループ内の音符数をインクリメント
				pNoteGroupInfo->m_lNumNoteInfo++;
			}
		}
	}

	return TRUE;
}


// トラック情報配列[lTrackIndex]内の3連音符グループ情報配列を更新
BOOL CMusicalScoreFrame::UpdateTripletGroupInfoArray (long lTrackIndex, MIDITrack* pMIDITrack) {
	long lNumTrackInfoArray = GetTrackInfoCount ();
	ASSERT (0 <= lTrackIndex && lTrackIndex < lNumTrackInfoArray);
	ASSERT (pMIDITrack);
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);

	// 表現可能な音符の長さ
	long lDur960 = lTimeResolution * 8;
	long lDur840 = lTimeResolution * 7;
	long lDur720 = lTimeResolution * 6;
	long lDur600 = lTimeResolution * 5;
	long lDur480 = lTimeResolution * 4; // 全音符
	long lDur360 = lTimeResolution * 3; // 付点2分音符
	long lDur240 = lTimeResolution * 2; // 2分音符
	long lDur180 = lTimeResolution * 3 / 2; // 付点4分音符
	long lDur120 = lTimeResolution; // 4分音符
	long lDur90 = lTimeResolution * 3 / 4; // 付点8分音符
	long lDur80 = lTimeResolution * 2 / 3; // 3連4分音符
	long lDur60 = lTimeResolution / 2; // 8分音符
	long lDur45 = lTimeResolution * 3 / 8; // 付点16分音符
	long lDur40 = lTimeResolution / 3; // 3連8分音符
	long lDur30 = lTimeResolution / 4; // 16分音符
	long lDur20 = lTimeResolution / 6; // 3連16分音符
	long lDur15 = lTimeResolution / 8; // 32分音符
	long lDur10 = lTimeResolution / 12; // 3連32分音符


	// トラック情報配列[lTrackIndex]内の音符グループ情報配列をいったん削除
	VERIFY (DeleteTripletGroupInfoArray (lTrackIndex));

	// 3連音符グループ情報の生成と配列への登録
	MusicalScoreTrackInfo* pTrackInfo = GetTrackInfo (lTrackIndex);
	long lNumNoteInfo = pTrackInfo->m_theNoteInfoArray.GetSize ();
	long j;
	MusicalScoreTripletGroupInfo* pTripletGroupInfo = NULL;
	BOOL bGroupContinueFlag = FALSE;
	for (j = 0; j < lNumNoteInfo; j++) {
		MusicalScoreNoteInfo* pNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j));
		long lDur = pNoteInfo->m_lNoteOffTime - pNoteInfo->m_lNoteOnTime;
		// 3連音符の場合
		if (lDur == lDur80 ||
			lDur == lDur40 ||
			lDur == lDur20 || 
			lDur == lDur10) {
			// この音符がまだどこの3連グループにも所属していない場合
			if (pNoteInfo->m_pTripletGroupInfo == NULL) {
				// 新しい3連音符グループ情報を生成する必要がある場合
				if (bGroupContinueFlag == FALSE) {
					VERIFY (pTripletGroupInfo = CreateTripletGroupInfo (pNoteInfo));
					VERIFY (AddTripletGroupInfo (pTrackInfo, pTripletGroupInfo));
				}
				// ポインタの設定
				pNoteInfo->m_pTripletGroupInfo = pTripletGroupInfo;
				pTripletGroupInfo->m_pLastNoteInfo = pNoteInfo;
				// 次の音符がある場合
				if (j + 1 < lNumNoteInfo) {
					MusicalScoreNoteInfo* pNextNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j + 1));
					// 次の音符がこの音符より後でかつ3連音符でない場合
					long lNextDur = pNextNoteInfo->m_lNoteOffTime - pNextNoteInfo->m_lNoteOnTime;
					if (pNextNoteInfo->m_lNoteOnTime >= pNoteInfo->m_lNoteOffTime &&
						lNextDur != lDur80 &&
						lNextDur != lDur40 &&
						lNextDur != lDur20 &&
						lNextDur != lDur10) {
						bGroupContinueFlag = FALSE; // 現在の3連音符グループは終了
					}
					// 次の音符の所属小節が現在の音符の所属小節と異なる場合
					else if (pNoteInfo->m_lNoteOnMeasure != pNextNoteInfo->m_lNoteOnMeasure) {
						bGroupContinueFlag = FALSE; // 現在の3連音符グループは終了
					}
					// 次の音符の所属拍が現在の音符の所属拍と異なる場合(音符が拍境界で切れている場合のみ)
					else if (pNoteInfo->m_lNoteOnBeat != pNextNoteInfo->m_lNoteOnBeat &&
						pNextNoteInfo->m_lNoteOffTick > 0) {
						bGroupContinueFlag = FALSE; // 現在の3連音符グループは終了
					}
					else {
						bGroupContinueFlag = TRUE; // 現在の3連音符グループを続ける
					}
				}
				// 次の音符がない場合
				else {
					bGroupContinueFlag = FALSE;
				}
				// グループ内最大キー・最小キーの更新
				long lKey = MIDIEvent_GetKey (pNoteInfo->m_pNoteOnEvent);
				if (pTripletGroupInfo->m_lMaxKey < lKey) {
					pTripletGroupInfo->m_lMaxKey = lKey;
				}
				if (pTripletGroupInfo->m_lMinKey > lKey) {
					pTripletGroupInfo->m_lMinKey = lKey;
				}
				// グループ内の最大長さ・最小長さの更新
				long lDur = pNoteInfo->m_lNoteOffTime - pNoteInfo->m_lNoteOnTime;
				if (pTripletGroupInfo->m_lMaxDur < lDur) {
					pTripletGroupInfo->m_lMaxDur = lDur;
				}
				if (pTripletGroupInfo->m_lMinDur > lDur) {
					pTripletGroupInfo->m_lMinDur = lDur;
				}
				// 3連音符グループ内の音符数をインクリメント
				pTripletGroupInfo->m_lNumNoteInfo++;
			}
			// 3連符開始時刻[Tick]と終了時刻[Tick]の補正
			if (pTripletGroupInfo->m_lMinDur == lDur80) {
				pTripletGroupInfo->m_lBeginTime = 
					(pTripletGroupInfo->m_pFirstNoteInfo->m_lNoteOnTime / lDur80) * lDur80;
				pTripletGroupInfo->m_lEndTime = 
					((pTripletGroupInfo->m_pLastNoteInfo->m_lNoteOffTime + lDur80 - 1) / lDur80) * lDur80;
			}
			else if (pTripletGroupInfo->m_lMinDur == lDur40) {
				pTripletGroupInfo->m_lBeginTime = 
					(pTripletGroupInfo->m_pFirstNoteInfo->m_lNoteOnTime / lDur40) * lDur40;
				pTripletGroupInfo->m_lEndTime = 
					((pTripletGroupInfo->m_pLastNoteInfo->m_lNoteOffTime + lDur40 - 1) / lDur40) * lDur40;
			}
			else if (pTripletGroupInfo->m_lMinDur == lDur20) {
				pTripletGroupInfo->m_lBeginTime = 
					(pTripletGroupInfo->m_pFirstNoteInfo->m_lNoteOnTime / lDur20) * lDur20;
				pTripletGroupInfo->m_lEndTime = 
					((pTripletGroupInfo->m_pLastNoteInfo->m_lNoteOffTime + lDur20 - 1) / lDur20) * lDur20;
			}
			else if (pTripletGroupInfo->m_lMinDur == lDur10) {
				pTripletGroupInfo->m_lBeginTime = 
					(pTripletGroupInfo->m_pFirstNoteInfo->m_lNoteOnTime / lDur10) * lDur10;
				pTripletGroupInfo->m_lEndTime = 
					((pTripletGroupInfo->m_pLastNoteInfo->m_lNoteOffTime + lDur10 - 1) / lDur10) * lDur10;
			}
		}
	}

	return TRUE;
}



MusicalScoreNoteInfo* CMusicalScoreFrame::CreateNote 
	(MIDIEvent* pNoteEvent, long lNoteOnTime, long lNoteOnMeasure, long lNoteOnBeat, long lNoteOnTick, 
	long lNoteOffTime, long lNoteOffMeasure, long lNoteOffBeat, long lNoteOffTick, long lFlags) {
	MusicalScoreNoteInfo* pNoteInfo = new MusicalScoreNoteInfo;
	if (pNoteInfo == NULL) {
		return NULL;
	}
	pNoteInfo->m_pNoteOnEvent = pNoteEvent;
	pNoteInfo->m_pNoteOffEvent = pNoteEvent->m_pNextCombinedEvent;
	pNoteInfo->m_lNoteOnTime = lNoteOnTime;
	pNoteInfo->m_lNoteOnMeasure = lNoteOnMeasure;
	pNoteInfo->m_lNoteOnBeat = lNoteOnBeat;
	pNoteInfo->m_lNoteOnTick = lNoteOnTick;
	pNoteInfo->m_lNoteOffTime = lNoteOffTime;
	pNoteInfo->m_lNoteOffMeasure = lNoteOffMeasure;
	pNoteInfo->m_lNoteOffBeat = lNoteOffBeat;
	pNoteInfo->m_lNoteOffTick = lNoteOffTick;
	pNoteInfo->m_lFlags = lFlags;
	pNoteInfo->m_lSelected = (pNoteEvent->m_lUserFlag & MIDIEVENT_SELECTED) ? 1 : 0;
	pNoteInfo->m_pNoteGroupInfo = NULL;
	pNoteInfo->m_pTripletGroupInfo = NULL;
	pNoteInfo->m_pNextNoteInfo = NULL;
	pNoteInfo->m_pPrevNoteInfo = NULL;
	ASSERT (MIDIEvent_GetParent (pNoteEvent));
	return pNoteInfo;
}

void CMusicalScoreFrame::DeleteNoteInfo (MusicalScoreNoteInfo* pNoteInfo) {
	delete (pNoteInfo);
}

BOOL CMusicalScoreFrame::AddNoteInfo (MusicalScoreTrackInfo* pTrackInfo, MusicalScoreNoteInfo* pNoteInfo) {
	long lNumNoteInfo = pTrackInfo->m_theNoteInfoArray.GetSize ();
	long j;
	// lNoteOnTime順に指定トラックの音符情報配列に音符情報を登録する。
	for (j = lNumNoteInfo - 1; j >= 0; j--) {
		MusicalScoreNoteInfo* pNoteInfo2 = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j));
		if (pNoteInfo2->m_lNoteOnTime <= pNoteInfo->m_lNoteOnTime) {
			break;
		}
	}
	pTrackInfo->m_theNoteInfoArray.InsertAt (j + 1, pNoteInfo);
	return TRUE;
}

MusicalScoreNoteGroupInfo* CMusicalScoreFrame::CreateNoteGroupInfo 
	(MusicalScoreNoteInfo* pNoteInfo) {
	MusicalScoreNoteGroupInfo* pNoteGroupInfo = new MusicalScoreNoteGroupInfo;
	if (pNoteGroupInfo == NULL) {
		return NULL;
	}
	pNoteGroupInfo->m_pFirstNoteInfo = pNoteInfo;
	pNoteGroupInfo->m_pLastNoteInfo = pNoteInfo;
	pNoteGroupInfo->m_lMinKey = MIDIEvent_GetKey (pNoteInfo->m_pNoteOnEvent);
	pNoteGroupInfo->m_lMaxKey = MIDIEvent_GetKey (pNoteInfo->m_pNoteOnEvent);
	pNoteGroupInfo->m_lMinDur = pNoteInfo->m_lNoteOffTime - pNoteInfo->m_lNoteOnTime;
	pNoteGroupInfo->m_lMaxDur = pNoteInfo->m_lNoteOffTime - pNoteInfo->m_lNoteOnTime;
	pNoteGroupInfo->m_lNumNoteInfo = 1;
	return pNoteGroupInfo;
}

void CMusicalScoreFrame::DeleteNoteGroupInfo (MusicalScoreNoteGroupInfo* pNoteGroupInfo) {
	delete (pNoteGroupInfo);
}

BOOL CMusicalScoreFrame::AddNoteGroupInfo (MusicalScoreTrackInfo* pTrackInfo, MusicalScoreNoteGroupInfo* pNoteGroupInfo) {
	pTrackInfo->m_theNoteGroupInfoArray.Add (pNoteGroupInfo);
	return TRUE;
}

MusicalScoreTripletGroupInfo* CMusicalScoreFrame::CreateTripletGroupInfo 
	(MusicalScoreNoteInfo* pNoteInfo) {
	MusicalScoreTripletGroupInfo* pTripletGroupInfo = new MusicalScoreTripletGroupInfo;
	if (pTripletGroupInfo == NULL) {
		return NULL;
	}
	pTripletGroupInfo->m_pFirstNoteInfo = pNoteInfo;
	pTripletGroupInfo->m_pLastNoteInfo = pNoteInfo;
	pTripletGroupInfo->m_lMinKey = MIDIEvent_GetKey (pNoteInfo->m_pNoteOnEvent);
	pTripletGroupInfo->m_lMaxKey = MIDIEvent_GetKey (pNoteInfo->m_pNoteOnEvent);
	pTripletGroupInfo->m_lMinDur = pNoteInfo->m_lNoteOffTime - pNoteInfo->m_lNoteOnTime;
	pTripletGroupInfo->m_lMaxDur = pNoteInfo->m_lNoteOffTime - pNoteInfo->m_lNoteOnTime;
	pTripletGroupInfo->m_lNumNoteInfo = 1;
	return pTripletGroupInfo;
}

void CMusicalScoreFrame::DeleteTripletGroupInfo (MusicalScoreTripletGroupInfo* pTripletGroupInfo) {
	delete (pTripletGroupInfo);
}

BOOL CMusicalScoreFrame::AddTripletGroupInfo (MusicalScoreTrackInfo* pTrackInfo, MusicalScoreTripletGroupInfo* pTripletGroupInfo) {
	pTrackInfo->m_theNoteGroupInfoArray.Add (pTripletGroupInfo);
	return TRUE;
}



//-----------------------------------------------------------------------------
// オーバーライド
//-----------------------------------------------------------------------------

// ウィンドウ生成直前の構造体設定
BOOL CMusicalScoreFrame::PreCreateWindow (CREATESTRUCT& cs) {
	return  (CWnd::PreCreateWindow(cs));
}

// ウィンドウタイトルの自動設定(CMDIChildWnd::OnUpdateFrameTitleのオーバーライド)
void CMusicalScoreFrame::OnUpdateFrameTitle (BOOL bAddToTitle) {
	// update our parent window first
	GetMDIFrame()->OnUpdateFrameTitle (bAddToTitle);
	if ((GetStyle() & FWS_ADDTOTITLE) == 0) {
		return;     // leave child window alone!
	}
	CDocument* pDocument = GetActiveDocument();
	if (bAddToTitle && pDocument != NULL) {
		CString strMusicalScore;
		strMusicalScore.LoadString (IDS_MUSICALSCORE);
		CString strTitle;
		if (m_nWindow > 0) {
			strTitle.Format (_T("%s:%d(%s)"), pDocument->GetTitle (), m_nWindow, strMusicalScore);
		}
		else {
			strTitle.Format (_T("%s(%s)"), pDocument->GetTitle (), strMusicalScore);
		}
		this->SetWindowText (strTitle);
	}
}

// 再配置用関数(CFrameWnd::RecalcLayoutのオーバーライド)
void CMusicalScoreFrame::RecalcLayout (BOOL bNotify) {

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
		MUSICALSCOREFRAME_SCALEHEIGHT + m_lHScrollBarHeight +
		MUSICALSCOREFRAME_BORDERHEIGHT * 2) {
		m_lScaleHeight = rcClient.Height () - m_lHScrollBarHeight -
			MUSICALSCOREFRAME_BORDERHEIGHT * 2;
		m_lKeyHeight = 0;
	}
	else if (rcClient.Height () - m_lToolBar1Height < 
		MUSICALSCOREFRAME_SCALEHEIGHT + m_lHScrollBarHeight +
		MUSICALSCOREFRAME_BORDERHEIGHT * 2) {
		m_lScaleHeight = MUSICALSCOREFRAME_SCALEHEIGHT;
		m_lKeyHeight = 0;
	}
	else {
		m_lScaleHeight = MUSICALSCOREFRAME_SCALEHEIGHT;
		m_lKeyHeight = rcClient.Height () -  m_lToolBar1Height -
			m_lScaleHeight - m_lHScrollBarHeight -
			MUSICALSCOREFRAME_BORDERHEIGHT * 2;
	}
	//m_lVelHeight = __min (m_lVelHeight, 128);

	// 幅方向の位置計算
	if (rcClient.Width () <
		MUSICALSCOREFRAME_SCALEWIDTH + m_lVScrollBarWidth + 
		MUSICALSCOREFRAME_BORDERWIDTH * 4) {
		m_lScaleWidth = rcClient.Width () -
			m_lVScrollBarWidth - MUSICALSCOREFRAME_BORDERWIDTH * 4;
		m_lTimeWidth = 0;
		m_lTrackListWidth = 0;
			
	}
	
	else if (rcClient.Width () <
		MUSICALSCOREFRAME_SCALEWIDTH + m_lVScrollBarWidth + 
		MUSICALSCOREFRAME_BORDERWIDTH * 4 + MUSICALSCOREFRAME_SPLITTERWIDTH + m_lTrackListWidth) {
		//m_lScaleWidth = rcClient.Width () - 
		//	m_lVScrollBarWidth - MUSICALSCOREFRAME_BORDERWIDTH * 2;
		m_lScaleWidth = MUSICALSCOREFRAME_SCALEWIDTH;
		m_lTimeWidth = 0;
		m_lTrackListWidth = rcClient.Width () - m_lScaleWidth -
			m_lVScrollBarWidth - MUSICALSCOREFRAME_BORDERWIDTH * 4 -
			MUSICALSCOREFRAME_SPLITTERWIDTH;

	}
	else {
		m_lScaleWidth = MUSICALSCOREFRAME_SCALEWIDTH;
		m_lTimeWidth = rcClient.Width () - m_lScaleWidth -
			m_lVScrollBarWidth - MUSICALSCOREFRAME_BORDERWIDTH * 4 -
			MUSICALSCOREFRAME_SPLITTERWIDTH - m_lTrackListWidth;
		m_lTrackListWidth = m_lTrackListWidth;
	}

	// ビューの整列
	if (m_pScaleView) {
		m_pScaleView->MoveWindow (MUSICALSCOREFRAME_BORDERWIDTH, 
			m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT, 
			m_lScaleWidth, m_lScaleHeight);
	}

	if (m_pTimeScaleView) {
		m_pTimeScaleView->MoveWindow (MUSICALSCOREFRAME_BORDERWIDTH + m_lScaleWidth, 
			m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT, 
			m_lTimeWidth, m_lScaleHeight);
	}

	if (m_pTrackScaleView) {
		m_pTrackScaleView->MoveWindow (MUSICALSCOREFRAME_BORDERWIDTH, 
			m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT + m_lScaleHeight, 
			m_lScaleWidth, m_lKeyHeight);
	}

	if (m_pTrackTimeView) {
		m_pTrackTimeView->MoveWindow (MUSICALSCOREFRAME_BORDERWIDTH + m_lScaleWidth,
			m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT + m_lScaleHeight, 
			m_lTimeWidth, m_lKeyHeight);
	}

	// スクロールバーの整列
	m_wndTimeScroll.MoveWindow (MUSICALSCOREFRAME_BORDERWIDTH,
		m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT * 1 + m_lScaleHeight + m_lKeyHeight,
		m_lScaleWidth + m_lTimeWidth - m_lVScrollBarWidth * 2, 
		m_lHScrollBarHeight);

	m_wndTrackScroll.MoveWindow (MUSICALSCOREFRAME_BORDERWIDTH + m_lScaleWidth + m_lTimeWidth, 
		m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT,
		m_lVScrollBarWidth, 
		m_lScaleHeight + m_lKeyHeight - m_lVScrollBarWidth * 2);

	m_wndSizeScroll.MoveWindow (MUSICALSCOREFRAME_BORDERWIDTH + m_lScaleWidth + m_lTimeWidth, 
		m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT * 1 + m_lScaleHeight + m_lKeyHeight, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);

	// ズームボタンの整列
	m_wndTimeZoomDown.MoveWindow (MUSICALSCOREFRAME_BORDERWIDTH + m_lScaleWidth +
		m_lTimeWidth - m_lVScrollBarWidth * 2, 
		m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT * 1 + m_lScaleHeight + m_lKeyHeight, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);
	
	m_wndTimeZoomUp.MoveWindow (MUSICALSCOREFRAME_BORDERWIDTH + m_lScaleWidth +
		m_lTimeWidth - m_lVScrollBarWidth, 
		m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT * 1 + m_lScaleHeight + m_lKeyHeight, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);

	m_wndTrackZoomDown.MoveWindow
		(MUSICALSCOREFRAME_BORDERWIDTH + m_lScaleWidth + m_lTimeWidth, 
		m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT + m_lScaleHeight + m_lKeyHeight -
		m_lVScrollBarWidth * 2, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);

	m_wndTrackZoomUp.MoveWindow
		(MUSICALSCOREFRAME_BORDERWIDTH + m_lScaleWidth + m_lTimeWidth, 
		m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT + m_lScaleHeight + m_lKeyHeight -
		m_lVScrollBarWidth, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);
	

	// リストの整列
	m_wndTrackList.MoveWindow
		(MUSICALSCOREFRAME_BORDERWIDTH * 3 + m_lScaleWidth + m_lTimeWidth + 
		m_lVScrollBarWidth + MUSICALSCOREFRAME_SPLITTERWIDTH, 
		m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT,
		m_lTrackListWidth,
		m_lScaleHeight + m_lKeyHeight + m_lHScrollBarHeight);


	// スクロールバーのサイズが変化したので、バーのデザインを再調整する。
	RecalcTrackScrollInfo ();
	RecalcTimeScrollInfo ();
}

// クライアント領域の生成(CFrameWnd::OnCreateClientのオーバーライド)
BOOL CMusicalScoreFrame::OnCreateClient (LPCREATESTRUCT lpcs, CCreateContext* pContext) {

	// サイズ調整用のダミービュー生成(Visible = FALSE)
	CWnd* pWnd = NULL;
	m_pDummyView = (CView*)CFrameWnd::CreateView (pContext, MUSICALSCOREFRAME_DUMMYVIEW);
	if (m_pDummyView == NULL) {
			return FALSE;
	}
	m_pDummyView->ShowWindow (SW_HIDE);

	// 印刷用ビューの生成(Visible = FALSE)
	pContext->m_pNewViewClass = RUNTIME_CLASS (CMusicalScorePrintView);
	m_pPrintView = (CView*)CFrameWnd::CreateView (pContext, MUSICALSCOREFRAME_PRINTVIEW);
	if (m_pPrintView == NULL) {
			return FALSE;
	}
	m_pPrintView->ShowWindow (SW_HIDE);
	
	// ビュー1の生成
	pContext->m_pNewViewClass = RUNTIME_CLASS (CMusicalScoreScaleView);
	m_pScaleView = (CView*)CFrameWnd::CreateView (pContext, MUSICALSCOREFRAME_SCALEVIEW);
	if (m_pScaleView == NULL) {
			return FALSE;
	}
	m_pScaleView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);

	// ビュー2の生成
	pContext->m_pNewViewClass = RUNTIME_CLASS (CMusicalScoreTimeScaleView);
	m_pTimeScaleView = (CView*)CFrameWnd::CreateView (pContext, MUSICALSCOREFRAME_TIMESCALEVIEW);
	if (m_pTimeScaleView == NULL) {
			return FALSE;
	}
	m_pTimeScaleView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);
	// ビュー3の生成
	pContext->m_pNewViewClass = RUNTIME_CLASS (CMusicalScoreTrackScaleView);
	m_pTrackScaleView = (CView*)CFrameWnd::CreateView (pContext, MUSICALSCOREFRAME_TRACKSCALEVIEW);
	if (m_pTrackScaleView == NULL) {
			return FALSE;
	}
	m_pTrackScaleView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);
	// ビュー4の生成
	pContext->m_pNewViewClass = RUNTIME_CLASS (CMusicalScoreTrackTimeView);
	m_pTrackTimeView = (CView*)CFrameWnd::CreateView (pContext, MUSICALSCOREFRAME_TRACKTIMEVIEW);
	if (m_pTrackTimeView == NULL) {
			return FALSE;
	}
	m_pTrackTimeView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);

	// スクロールバーの生成
	m_wndTimeScroll.Create   
		(WS_CHILD|WS_VISIBLE|SBS_HORZ, CRect(0,0,0,0), this, MUSICALSCOREFRAME_TIMESCROLL);
	m_wndTrackScroll.Create    
		(WS_CHILD|WS_VISIBLE|SBS_VERT, CRect(0,0,0,0), this, MUSICALSCOREFRAME_TRACKSCROLL);
	m_wndSizeScroll.Create   
		(WS_CHILD|WS_VISIBLE|SBS_SIZEBOX, CRect(0,0,0,0), this, MUSICALSCOREFRAME_SIZEBOX);
	
	// ズームボタン類の生成
	m_wndTimeZoomDown.Create 
		(_T("-"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, MUSICALSCOREFRAME_TIMEZOOMDOWN);
	m_wndTimeZoomUp.Create   
		(_T("+"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, MUSICALSCOREFRAME_TIMEZOOMUP);
	m_wndTrackZoomDown.Create  
		(_T("-"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, MUSICALSCOREFRAME_TRACKZOOMDOWN);
	m_wndTrackZoomUp.Create    
		(_T("+"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, MUSICALSCOREFRAME_TRACKZOOMUP);

	// トラックリストの作成
	m_wndTrackList.Create
		(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|
		LBS_NOTIFY|LBS_DISABLENOSCROLL|LBS_OWNERDRAWFIXED|LBS_HASSTRINGS|LBS_NOINTEGRALHEIGHT, 
		CRect(0,0,0,0), this, MUSICALSCOREFRAME_TRACKLIST);
	

	// コントロールの位置合わせはWM_SIZEなどによるRecalcLaoyoutに任せる。
	
	
	
	
	// 描画情報配列の更新
	VERIFY (UpdateMeasureInfoArray ());
	VERIFY (UpdateTrackInfoArray ());
	RecalcTrackScrollInfo ();
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
	return TRUE;

}

// 印刷用のコマンドをトラップ(CFrameWnd::OnCmdMsgのオーバーライド)
BOOL CMusicalScoreFrame::OnCmdMsg (UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	// 印刷用のコマンドの場合、強制的にCMusicalScorePrintViewに渡す。
	if ((nID == ID_FILE_PRINT || nID == ID_FILE_PRINT_DIRECT || nID == ID_FILE_PRINT_PREVIEW) &&
		pSekaijuApp->m_bRecording == FALSE) {
		if (m_pPrintView) {
			return ((CMusicalScorePrintView*)m_pPrintView)->OnCmdMsg (nID, nCode, pExtra, pHandlerInfo);
		}
		return FALSE;
	}
	// その他のコマンドはデフォルトの処理とする。
	return CFrameWnd::OnCmdMsg (nID, nCode, pExtra, pHandlerInfo);
}

//-----------------------------------------------------------------------------
// メッセージマップ
//-----------------------------------------------------------------------------

// ウィンドウ生成時
int CMusicalScoreFrame::OnCreate (LPCREATESTRUCT lpCreateStruct) {

	CRect rcTemp;

	// ツールバー1の作成
	if (!m_wndToolBar1.Create (this) ||
		!m_wndToolBar1.LoadToolBar (IDR_MUSICALSCORE1)) {
		TRACE0 ("Failed to create toolbar\n");
		return -1;
	}
	m_wndToolBar1.SetBarStyle (m_wndToolBar1.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	//m_wndToolBar1.EnableDocking (CBRS_ALIGN_ANY);
	//EnableDocking (CBRS_ALIGN_ANY);
	//DockControlBar (&m_wndToolBar1);

	LoadAccelTable (MAKEINTRESOURCE (IDR_MUSICALSCORE));

	// トラックコンボの作成
	m_wndToolBar1.SetButtonInfo (5, IDC_TRACKCOMBO, TBBS_SEPARATOR, 80);
	m_wndToolBar1.GetItemRect (5, &rcTemp);
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
	m_wndToolBar1.SetButtonInfo (7, IDC_CHANNELCOMBO, TBBS_SEPARATOR, 40);
	m_wndToolBar1.GetItemRect (7, &rcTemp);
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
	m_wndToolBar1.SetButtonInfo (9, IDC_SNAPCOMBO, TBBS_SEPARATOR, 100);
	m_wndToolBar1.GetItemRect (9, &rcTemp);
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
	m_wndToolBar1.SetButtonInfo (11, IDC_VELOCITYCOMBO, TBBS_SEPARATOR, 50);
	m_wndToolBar1.GetItemRect (11, &rcTemp);
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

	// 長さコンボの作成
	m_wndToolBar1.SetButtonInfo (13, IDC_DURATIONCOMBO, TBBS_SEPARATOR, 50);
	m_wndToolBar1.GetItemRect (13, &rcTemp);
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
	
	// 表示精度コンボの作成
	m_wndToolBar1.SetButtonInfo (24, IDC_RESOLUTIONCOMBO, TBBS_SEPARATOR, 100);
	m_wndToolBar1.GetItemRect (24, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 120;
	if (!m_wndResolutionCombo.CreateEx (
		WS_EX_CLIENTEDGE, _T("COMBOBOX"), NULL,
		WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar1.GetSafeHwnd (), (HMENU)IDC_RESOLUTIONCOMBO)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndResolutionCombo.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));


	// 親クラスの関数呼び出し
	int nRet = CChildFrame::OnCreate (lpCreateStruct);

	// スクロールポジションの位置あわせ
	// SetTrackScrollPos (64 * m_lTrackZoom - m_lKeyHeight / 2); 

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

	// 表示精度コンボの充満
	UpdateResolutionCombo ();
	m_wndResolutionCombo.SetCurSel (4);

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

	// 自動ページ更新の設定
	if (pSekaijuApp->m_theGeneralOption.m_bEnableAutoPageUpdate) {
		m_bAutoPageUpdate = TRUE;
	}

	SetActiveView (m_pTrackTimeView, FALSE);
	m_pTrackTimeView->SetFocus ();

	UpdateMeasureInfoArray ();
	UpdateTrackInfoArray ();

	int x = this->MeasuretoX2 (0) + 1;
	this->SetTimeScrollPos (x);


	pSekaijuDoc->m_theCriticalSection.Unlock ();
	return nRet;
}

// ウィンドウ破棄時
void CMusicalScoreFrame::OnDestroy () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	// トラック情報配列削除
	VERIFY (DeleteTrackInfoArray ());
	// 小節情報配列削除
	VERIFY (DeleteMeasureInfoArray ());

	//KillTimer (0x11);
	CChildFrame::OnDestroy ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// ウィンドウサイズ変更時
void CMusicalScoreFrame::OnSize (UINT nType, int cx, int cy) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	// 基本クラスの関数呼び出し
	CChildFrame::OnSize (nType, cx, cy);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// タイマー呼び出し時
void CMusicalScoreFrame::OnTimer (UINT nIDEvent) {
	//if (nIDEvent == 0x11) {
	//	m_pTrackTimeView->SendMessage (WM_TIMER, 11, NULL);
	//	m_pVelTimeView->SendMessage (WM_TIMER, 11, NULL);
	//}
}

// 背景消去(CFrameWnd::OnEraseBkgndのオーバーライド)
BOOL CMusicalScoreFrame::OnEraseBkgnd (CDC* pDC) {
	return 0;
}

// ウィンドウがアクティブになったとき
void CMusicalScoreFrame::OnMDIActivate (BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) {
	CChildFrame::OnMDIActivate (bActivate, pActivateWnd, pDeactivateWnd);
}

// 描画するとき
void CMusicalScoreFrame::OnPaint () {
	CPaintDC dc (this);
	CRect rcClient;
	GetClientRect (&rcClient);
	// 左側領域のくぼみ描画
	CRect rcClient1 (rcClient);
	rcClient1.top = m_lToolBar1Height;
	rcClient1.bottom = m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT * 2 + 
		m_lScaleHeight + m_lKeyHeight + m_lHScrollBarHeight;
	rcClient1.right = MUSICALSCOREFRAME_BORDERWIDTH * 2 + m_lScaleWidth + m_lTimeWidth + 
		m_lVScrollBarWidth;
	dc.Draw3dRect (&rcClient1, RGB (128, 128, 128), RGB (255, 255, 255));
	rcClient1.InflateRect (-1, -1);
	dc.Draw3dRect (&rcClient1, RGB (0, 0, 0), RGB (192, 192, 192));
	// 右上側領域のくぼみ描画
	CRect rcClient3 (rcClient);
	rcClient3.top = m_lToolBar1Height;
	rcClient3.bottom = m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT * 2 + 
		m_lScaleHeight + m_lKeyHeight + m_lHScrollBarHeight;
	rcClient3.left = MUSICALSCOREFRAME_BORDERWIDTH * 2 + m_lScaleWidth + m_lTimeWidth + 
		m_lVScrollBarWidth + MUSICALSCOREFRAME_SPLITTERWIDTH;
	dc.Draw3dRect (&rcClient3, RGB (128, 128, 128), RGB (255, 255, 255));
	rcClient3.InflateRect (-1, -1);
	dc.Draw3dRect (&rcClient3, RGB (0, 0, 0), RGB (192, 192, 192));
	// 横境界部分の描画
	CBrush brush;
	brush.CreateSolidBrush (::GetSysColor (COLOR_3DFACE));
	CRect rcClient6 (rcClient);
	rcClient6.left = rcClient1.right + 1;
	rcClient6.right = rcClient3.left - 1;
	dc.FillRect (&rcClient6, &brush);
}

// キーが押された時
void CMusicalScoreFrame::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	switch (nChar) {
	// D(描画)P(ペン)
	case 'D':
	case 'P':
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_PEN, 0);
		}
		break;
	// L(線)
	case 'L':
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_LINE, 0);
		}
		break;
	// E(消しゴム)
	case 'E':
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_ERASER, 0);
		}
		break;
	// S(選択)
	case 'S':
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_SELECT, 0);
		}
		break;
	// B(スクラブ)
	case 'B':
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_SPEAKER, 0);
		}
		break;
	// 1(全音符)
	case '1' :
	case VK_NUMPAD1:
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_WHOLENOTE, 0);
		}
		break;
	// 2(2分音符)
	case '2' :
	case VK_NUMPAD2:
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_HALFNOTE, 0);
		}
		break;
	// 4(4分音符)
	case '4' :
	case VK_NUMPAD4:
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_QUARTERNOTE, 0);
		}
		break;
	// 8(8分音符)
	case '8' :
	case VK_NUMPAD8:
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_QUAVERNOTE, 0);
		}
		break;
	// 16(16分音符)
	case '6' :
	case VK_NUMPAD6:
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_SEMIQUAVERNOTE, 0);
		}
		break;
	// 32(32分音符)
	case '9' :
	case VK_NUMPAD9:
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_DEMISEMIQUAVERNOTE, 0);
		}
		break;
	// .(付点)
	case '.' :
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_DOTTED, 0);
		}
		break;
	// t(3連)
	case 't' :
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_TRIPLET, 0);
		}
		break;
	}
	return;
}



// マウス左ボタン押された時  
void CMusicalScoreFrame::OnLButtonDown (UINT nFlags, CPoint point) {
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
void CMusicalScoreFrame::OnRButtonDown (UINT nFlags, CPoint point) {
}

// マウス左ボタン離されたとき
void CMusicalScoreFrame::OnLButtonUp (UINT nFlags, CPoint point) {
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
		if (m_bSplitterMovingV) {
			m_lTrackListWidth = MUSICALSCOREFRAME_RANGE (0,
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
void CMusicalScoreFrame::OnRButtonUp (UINT nFlags, CPoint point) {


}

// マウスが動かされたとき
void CMusicalScoreFrame::OnMouseMove (UINT nFlags, CPoint point) {
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
			m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT * 2 + m_lScaleHeight + m_lKeyHeight <= point.y &&
			point.y < m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT * 2 + m_lScaleHeight + m_lKeyHeight + 
			MUSICALSCOREFRAME_SPLITTERHEIGHT;
		// カーソルが垂直スプリッター上にあるか
		m_bSplitterMovingV =
			MUSICALSCOREFRAME_BORDERWIDTH * 2 + m_lScaleWidth + m_lTimeWidth + m_lVScrollBarWidth <= point.x &&
			point.x < MUSICALSCOREFRAME_BORDERWIDTH * 2 + m_lScaleWidth + m_lTimeWidth + m_lVScrollBarWidth +
			MUSICALSCOREFRAME_SPLITTERWIDTH;
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
void CMusicalScoreFrame::OnTimeZoomDown () {
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
	m_pTrackTimeView->Invalidate ();
	//m_bAutoPageUpdate = TRUE;
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// 時間方向ズームアップ(20091220:左端位置保持機能追加、自動ページ更新自動オフ追加)
void CMusicalScoreFrame::OnTimeZoomUp () {
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
	m_pTrackTimeView->Invalidate ();
	//m_bAutoPageUpdate = TRUE;
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// トラック方向ズームダウン(20091220:上端位置保持機能追加)
void CMusicalScoreFrame::OnTrackZoomDown () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lOldTrackZoom = m_lTrackZoom;
	long lOldTrackPos = m_wndTrackScroll.GetScrollPos ();
	long lNewTrackZoom = CLIP (1, m_lTrackZoom - 1, 8);
	long lNewTrackPos = lOldTrackPos * lNewTrackZoom / lOldTrackZoom;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_lTrackZoom = lNewTrackZoom;
	RecalcTrackScrollInfo ();
	m_wndTrackScroll.SetScrollPos (lNewTrackPos);
	m_lTrackScrollPos = m_wndTrackScroll.GetScrollPos ();
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// トラック方向ズームアップ(20091220:上端位置保持機能追加)
void CMusicalScoreFrame::OnTrackZoomUp () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lOldTrackZoom = m_lTrackZoom;
	long lOldTrackPos = m_wndTrackScroll.GetScrollPos ();
	long lNewTrackZoom = CLIP (1, m_lTrackZoom + 1, 8);
	long lNewTrackPos = lOldTrackPos * lNewTrackZoom / lOldTrackZoom;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_lTrackZoom = lNewTrackZoom;
	RecalcTrackScrollInfo ();
	m_wndTrackScroll.SetScrollPos (lNewTrackPos);
	m_lTrackScrollPos = m_wndTrackScroll.GetScrollPos ();
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}


// 水平スクロール
void CMusicalScoreFrame::OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
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
		SetTimeScrollPos (MUSICALSCOREFRAME_RANGE (0, lNewPos, 0x7FFFFFFF));
		m_bAutoPageUpdate = FALSE;
		m_pTrackTimeView->SetFocus ();
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// 垂直スクロール
void CMusicalScoreFrame::OnVScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	if (pScrollBar == &m_wndTrackScroll) {
		int nMin = 0;
		int nMax = 0;
		long ry = 4;
		pScrollBar->GetScrollRange (&nMin, &nMax);
		long lNewPos = m_lTrackScrollPos;
		switch (nSBCode) {
		case SB_LINEUP:
			lNewPos = m_lTrackScrollPos - ry * 2;
			break;
		case SB_LINEDOWN:
			lNewPos = m_lTrackScrollPos + ry * 2;
			break;
		case SB_PAGEUP:
			lNewPos = m_lTrackScrollPos - ry * 10;
			break;
		case SB_PAGEDOWN:
			lNewPos = m_lTrackScrollPos + ry * 10;
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
		SetTrackScrollPos (CLIP (0, lNewPos, 0x7FFFFFFF));
		m_pTrackTimeView->SetFocus ();
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// 『ツール(&T)』-『ペン(&P)』
void CMusicalScoreFrame::OnMusicalScorePen () {
	m_lCurTool = ID_MUSICALSCORE_PEN;
}

// 『ツール(&T)』-『ペン(&P)』
void CMusicalScoreFrame::OnUpdateMusicalScorePenUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurTool == ID_MUSICALSCORE_PEN);
}

// 『ツール(&T)』-『線(&L)』
void CMusicalScoreFrame::OnMusicalScoreLine () {
	m_lCurTool = ID_MUSICALSCORE_LINE;
}

// 『ツール(&T)』-『線(&L)』
void CMusicalScoreFrame::OnUpdateMusicalScoreLineUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurTool == ID_MUSICALSCORE_LINE);
}

// 『ツール(&T)』-『消しゴム(&E)』
void CMusicalScoreFrame::OnMusicalScoreEraser () {
	m_lCurTool = ID_MUSICALSCORE_ERASER;
}

// 『ツール(&T)』-『消しゴム(&E)』
void CMusicalScoreFrame::OnUpdateMusicalScoreEraserUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurTool == ID_MUSICALSCORE_ERASER);
}

// 『ツール(&T)』-『選択(&S)』
void CMusicalScoreFrame::OnMusicalScoreSelect () {
	m_lCurTool = ID_MUSICALSCORE_SELECT;
}

// 『ツール(&T)』-『選択(&S)』
void CMusicalScoreFrame::OnUpdateMusicalScoreSelectUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurTool == ID_MUSICALSCORE_SELECT);
}

// 『ツール(&T)』-『スピーカ(&P)』
void CMusicalScoreFrame::OnMusicalScoreSpeaker () {
	m_lCurTool = ID_MUSICALSCORE_SPEAKER;
}

// 『ツール(&T)』-『スピーカ(&P)』
void CMusicalScoreFrame::OnUpdateMusicalScoreSpeakerUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurTool == ID_MUSICALSCORE_SPEAKER);
}

// 『ツール(&T)』-『全音符』
void CMusicalScoreFrame::OnMusicalScoreWholeNote () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	strText.Format (_T("%d"), lTimeResolution * 4);
	m_wndDurationCombo.SetWindowText (strText);
}

// 『ツール(&T)』-『全音符』
void CMusicalScoreFrame::OnUpdateMusicalScoreWholeNoteUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	long lCurDuration = _ttol (strText);
	// 全音符
	pCmdUI->SetCheck (lCurDuration == lTimeResolution * 4);
}

// 『ツール(&T)』-『2分音符』
void CMusicalScoreFrame::OnMusicalScoreHalfNote () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	// 2分音符
	strText.Format (_T("%d"), lTimeResolution * 2);
	m_wndDurationCombo.SetWindowText (strText);
}

// 『ツール(&T)』-『2分音符』
void CMusicalScoreFrame::OnUpdateMusicalScoreHalfNoteUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	long lCurDuration = _ttol (strText);
	// 2分音符・付点2分音符・3連2分音符
	pCmdUI->SetCheck (lCurDuration == lTimeResolution * 2 ||
		lCurDuration == lTimeResolution * 3 ||
		lCurDuration == lTimeResolution * 4 / 3);
}

// 『ツール(&T)』-『4分音符』
void CMusicalScoreFrame::OnMusicalScoreQuarterNote () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	strText.Format (_T("%d"), lTimeResolution * 1);
	// 4分音符
	m_wndDurationCombo.SetWindowText (strText);
}

// 『ツール(&T)』-『4分音符』
void CMusicalScoreFrame::OnUpdateMusicalScoreQuarterNoteUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	long lCurDuration = _ttol (strText);
	// 4分音符・付点4分音符・3連4分音符
	pCmdUI->SetCheck (lCurDuration == lTimeResolution * 1 ||
		lCurDuration == lTimeResolution * 3 / 2 ||
		lCurDuration == lTimeResolution * 2 / 3);
}

// 『ツール(&T)』-『8分音符』
void CMusicalScoreFrame::OnMusicalScoreQuaverNote () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	// 8分音符
	strText.Format (_T("%d"), lTimeResolution / 2);
	m_wndDurationCombo.SetWindowText (strText);
}

// 『ツール(&T)』-『8分音符』
void CMusicalScoreFrame::OnUpdateMusicalScoreQuaverNoteUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	long lCurDuration = _ttol (strText);
	// 8分音符・付点8分音符・3連8分音符
	pCmdUI->SetCheck (lCurDuration == lTimeResolution / 2 ||
		lCurDuration == lTimeResolution * 3 / 4 ||
		lCurDuration == lTimeResolution * 1 / 3);
}

// 『ツール(&T)』-『16分音符』
void CMusicalScoreFrame::OnMusicalScoreSemiQuaverNote () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	// 16分音符
	strText.Format (_T("%d"), lTimeResolution / 4);
	m_wndDurationCombo.SetWindowText (strText);
}

// 『ツール(&T)』-『16分音符』
void CMusicalScoreFrame::OnUpdateMusicalScoreSemiQuaverNoteUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	long lCurDuration =_ttol (strText);
	// 16分音符・付点16分音符・3連16分音符
	pCmdUI->SetCheck (lCurDuration == lTimeResolution / 4 ||
		lCurDuration == lTimeResolution * 3 / 8 ||
		lCurDuration == lTimeResolution * 1 / 6);
}

// 『ツール(&T)』-『32分音符』
void CMusicalScoreFrame::OnMusicalScoreDemiSemiQuaverNote () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	// 32分音符
	strText.Format (_T("%d"), lTimeResolution / 8);
	m_wndDurationCombo.SetWindowText (strText);
}

// 『ツール(&T)』-『32分音符』
void CMusicalScoreFrame::OnUpdateMusicalScoreDemiSemiQuaverNoteUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	long lCurDuration = _ttol (strText);
	// 32分音符・付点32分音符
	pCmdUI->SetCheck (lCurDuration == lTimeResolution / 8 ||
		lCurDuration == lTimeResolution * 1 / 12);
}

// 『ツール(&T)』-『付点』
void CMusicalScoreFrame::OnMusicalScoreDotted () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	long lCurDuration = _ttol (strText);
	// 2分音符→付点2分音符
	if (lCurDuration == lTimeResolution * 2) {
		lCurDuration = lTimeResolution * 3;
	}
	// 4分音符→付点4分音符
	else if (lCurDuration == lTimeResolution * 1) {
		lCurDuration = lTimeResolution * 3 / 2;
	}
	// 8分音符→付点8分音符
	else if (lCurDuration == lTimeResolution / 2) {
		lCurDuration = lTimeResolution * 3 / 4;
	}
	// 16分音符→付点16分音符
	else if (lCurDuration == lTimeResolution / 4) {
		lCurDuration = lTimeResolution * 3 / 8;
	}
	// 付点2分音符→2分音符
	else if (lCurDuration == lTimeResolution * 3) {
		lCurDuration = lTimeResolution * 2;
	}
	// 付点4分音符→4分音符
	else if (lCurDuration == lTimeResolution * 3 / 2) {
		lCurDuration = lTimeResolution * 1;
	}
	// 付点8分音符→8分音符
	else if (lCurDuration == lTimeResolution * 3 / 4) {
		lCurDuration = lTimeResolution / 2;
	}
	// 付点16分音符→16分音符
	else if (lCurDuration == lTimeResolution * 3 / 8) {
		lCurDuration = lTimeResolution / 4;
	}
	strText.Format (_T("%d"), lCurDuration);
	m_wndDurationCombo.SetWindowText (strText);
}

// 『ツール(&T)』-『付点』
void CMusicalScoreFrame::OnUpdateMusicalScoreDottedUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	long lCurDuration = _ttol (strText);
	// 2分音符・4分音符・8分音符・16分音符
	// 付点2分音符・付点4分音符・付点8分音符・付点16分音符
	pCmdUI->Enable (lCurDuration == lTimeResolution * 2 ||
		lCurDuration == lTimeResolution * 3 ||
		lCurDuration == lTimeResolution * 1 ||
		lCurDuration == lTimeResolution * 3 / 2 ||
		lCurDuration == lTimeResolution / 2 || 
		lCurDuration == lTimeResolution * 3 / 4 ||
		lCurDuration == lTimeResolution / 4 || 
		lCurDuration == lTimeResolution * 3 / 8);	
	// 付点2分音符・付点4分音符・付点8分音符・付点16分音符
	pCmdUI->SetCheck (lCurDuration == lTimeResolution * 3 ||
		lCurDuration == lTimeResolution * 3 / 2 ||
		lCurDuration == lTimeResolution * 3 / 4 ||
		lCurDuration == lTimeResolution * 3 / 8);
}

// 『ツール(&T)』-『3連』
void CMusicalScoreFrame::OnMusicalScoreTriplet () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	long lCurDuration = _ttol (strText);
	// 2分音符→3連2分音符
	if (lCurDuration == lTimeResolution * 2) { 
		lCurDuration = lTimeResolution * 4 / 3;
	}
	// 4分音符→3連4分音符
	else if (lCurDuration == lTimeResolution * 1) {
		lCurDuration = lTimeResolution * 2 / 3;
	}
	// 8分音符→3連8分音符
	else if (lCurDuration == lTimeResolution / 2) {
		lCurDuration = lTimeResolution / 3;
	}
	// 16分音符→3連16分音符
	else if (lCurDuration == lTimeResolution / 4) {
		lCurDuration = lTimeResolution / 6;
	}
	// 32分音符→3連32分音符
	else if (lCurDuration == lTimeResolution / 8) {
		lCurDuration = lTimeResolution / 12;
	}
	// 3連2分音符→2分音符
	else if (lCurDuration == lTimeResolution * 4 / 3) {
		lCurDuration = lTimeResolution * 2;
	}
	// 3連4分音符→4分音符
	else if (lCurDuration == lTimeResolution * 2 / 3) {
		lCurDuration = lTimeResolution * 1;
	}
	// 3連8分音符→8分音符
	else if (lCurDuration == lTimeResolution / 3) {
		lCurDuration = lTimeResolution / 2;
	}
	// 3連16分音符→16分音符
	else if (lCurDuration == lTimeResolution / 6) {
		lCurDuration = lTimeResolution / 4;
	}
	// 3連32分音符→32分音符
	else if (lCurDuration == lTimeResolution / 12) {
		lCurDuration = lTimeResolution / 8;
	}
	strText.Format (_T("%d"), lCurDuration);
	m_wndDurationCombo.SetWindowText (strText);
}

// 『ツール(&T)』-『3連』
void CMusicalScoreFrame::OnUpdateMusicalScoreTripletUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	long lCurDuration = _ttol (strText);
	// 2分音符・4分音符・8分音符・16分音符・32分音符
	// 3連2分音符・3連4分音符・3連8分音符・3連16分音符・3連32分音符
	pCmdUI->Enable (lCurDuration == lTimeResolution * 2 ||
		lCurDuration == lTimeResolution * 4 / 3 ||
		lCurDuration == lTimeResolution * 1 ||
		lCurDuration == lTimeResolution * 2 / 3 ||
		lCurDuration == lTimeResolution / 2 ||
		lCurDuration == lTimeResolution / 3 || 
		lCurDuration == lTimeResolution / 4 ||
		lCurDuration == lTimeResolution / 6 || 
		lCurDuration == lTimeResolution / 8 ||
		lCurDuration == lTimeResolution / 12);	
	// 3連2分音符・3連4分音符・3連8分音符・3連16分音符・3連32分音符
	pCmdUI->SetCheck 
		(lCurDuration == lTimeResolution * 4 / 3 || 
		lCurDuration == lTimeResolution * 2 / 3 ||
		lCurDuration == lTimeResolution / 3 ||
		lCurDuration == lTimeResolution / 6 ||
		lCurDuration == lTimeResolution / 12);
}

// 『ツール(&T)』-『現在のトラックのみ表示(&C)』
void CMusicalScoreFrame::OnMusicalScoreOnlyCurTrack () {
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
	VERIFY (UpdateMeasureInfoArray ());
	VERIFY (UpdateTrackInfoArray ());
	RecalcTrackScrollInfo ();
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
}

// 『ツール(&T)』-『現在のトラックのみ表示(&C)』
void CMusicalScoreFrame::OnUpdateMusicalScoreOnlyCurTrackUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_bOnlyCurTrack);
}

// 『ツール(&T)』-『全てのトラックを表示(&A)』
void CMusicalScoreFrame::OnMusicalScoreShowAllTrack () {
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
	VERIFY (UpdateMeasureInfoArray ());
	VERIFY (UpdateTrackInfoArray ());
	RecalcTrackScrollInfo ();
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();

}	

// 『ツール(&T)』-『全てのトラックを表示(&A)』
void CMusicalScoreFrame::OnUpdateMusicalScoreShowAllTrackUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_bShowAllTrack);
}


// 『ツール(&T)』-『自動ページ更新』
void CMusicalScoreFrame::OnMusicalScoreAutoPageUpdate () {
	m_bAutoPageUpdate = !m_bAutoPageUpdate;
}

// 『ツール(&T)』-『自動ページ更新』
void CMusicalScoreFrame::OnUpdateMusicalScoreAutoPageUpdateUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_bAutoPageUpdate);
}


// トラックコンボが選択され終わった時
void CMusicalScoreFrame::OnTrackComboSelEndOK () {
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
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
}

// トラックリストのチェックボックスが変化したとき
void CMusicalScoreFrame::OnTrackListChkChange () {
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
	VERIFY (UpdateMeasureInfoArray ());
	VERIFY (UpdateTrackInfoArray ());
	RecalcTrackScrollInfo ();
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
}

// トラックリストの選択項目が変化したとき
void CMusicalScoreFrame::OnTrackListSelChange () {
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
	VERIFY (UpdateMeasureInfoArray ()); // 20110104追加
	VERIFY (UpdateTrackInfoArray ()); // 20110104追加
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// 再描画
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
}

// スナップコンボが選択され終わった時(20111017追加)
void CMusicalScoreFrame::OnSnapComboSelEndOK () {
	long lSnapComboIndex = m_wndSnapCombo.GetCurSel ();
	long lResolutionComboIndex = m_wndResolutionCombo.GetCurSel ();
	if (0 <= lSnapComboIndex && lSnapComboIndex <= 6 && lResolutionComboIndex < lSnapComboIndex) {
		m_wndResolutionCombo.SetCurSel (lSnapComboIndex);
		this->PostMessage (WM_COMMAND, (WPARAM)((CBN_SELENDOK << 16) | IDC_RESOLUTIONCOMBO),
			(LPARAM)(m_wndResolutionCombo.GetSafeHwnd ()));
	}
}

// 表示精度コンボが選択され終わった時
void CMusicalScoreFrame::OnResolutionComboSelEndOK () {
	// 全トラックの音符情報と音符グループ情報を更新する
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	UpdateTrackInfoArray ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// 再描画
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
}


// 『ポップアップ』-『このトラックのみ表示ON』
void CMusicalScoreFrame::OnPopupTrackVisibleOn () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
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
	if (pSekaijuDoc->m_pTempTrack) { // 20110104追加
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
	}
	VERIFY (UpdateMeasureInfoArray ()); // 20110104追加
	VERIFY (UpdateTrackInfoArray ()); // 20110104追加
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// 再描画
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
}

// 『ポップアップ』-『このトラックのみ表示ON』
void CMusicalScoreFrame::OnUpdatePopupTrackVisibleOnUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	if (pSekaijuDoc->m_pTempTrack == NULL) {
		pCmdUI->Enable (FALSE);	
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// 『ポップアップ』-『このトラックのみ表示OFF』
void CMusicalScoreFrame::OnPopupTrackVisibleOff () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
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
	if (pSekaijuDoc->m_pTempTrack) { // 20110104追加
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
	}
	VERIFY (UpdateMeasureInfoArray ()); // 20110104追加
	VERIFY (UpdateTrackInfoArray ()); // 20110104追加
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// 再描画
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
}

// 『ポップアップ』-『このトラックのみ表示OFF』
void CMusicalScoreFrame::OnUpdatePopupTrackVisibleOffUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	if (pSekaijuDoc->m_pTempTrack == NULL) {
		pCmdUI->Enable (FALSE);	
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// 『ポップアップ』-『全トラック表示ON』
void CMusicalScoreFrame::OnPopupTrackVisibleAll () {
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
	// カレントトラックとカレントチャンネルを更新
	if (pSekaijuDoc->m_pTempTrack) { // 20110104追加
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
	}
	VERIFY (UpdateMeasureInfoArray ()); // 20110104追加
	VERIFY (UpdateTrackInfoArray ()); // 20110104追加
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// 再描画
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();

}

// 『ポップアップ』-『全トラック表示ON』
void CMusicalScoreFrame::OnUpdatePopupTrackVisibleAllUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}


// 『ポップアップ』-『このイベントのプロパティ(&R)』 // 20110109修正
void CMusicalScoreFrame::OnPopupEventProperty () {
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
		MIDIEvent* pCloneEvent = NULL;
		MIDIEvent* pLastEvent = NULL;
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

// 『ポップアップ』-『このイベントのプロパティ(&R)』 // 20110104追加
void CMusicalScoreFrame::OnUpdatePopupEventPropertyUI (CCmdUI* pCmdUI) {
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

