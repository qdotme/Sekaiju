//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ピアノロール印刷ビュークラス
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
#include <afxpriv.h>
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
#include "MainFrame.h"
#include "ChildFrame.h"
#include "PianoRollFrame.h"
#include "PianoRollPrintView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define PIANOROLLPRINTVIEW_LEFTMARGIN 200
#define PIANOROLLPRINTVIEW_RIGHTMARGIN 200
#define PIANOROLLPRINTVIEW_TOPMARGIN 200
#define PIANOROLLPRINTVIEW_BOTTOMMARGIN 200
#define PIANOROLLPRINTVIEW_VELHEIGHT 256
#define PIANOROLLPRINTVIEW_KEYHEIGHT 2560
#define PIANOROLLPRINTVIEW_SCALEHEIGHT 100
#define PIANOROLLPRINTVIEW_SCALEWIDTH 100


IMPLEMENT_DYNCREATE (CPianoRollPrintView, CSekaijuView)

// メッセージマップ
BEGIN_MESSAGE_MAP (CPianoRollPrintView, CSekaijuView)
	ON_COMMAND (ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND (ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND (ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CPianoRollPrintView::CPianoRollPrintView () {
	// 印刷用文字フォントの作成(2.5ミリ)
	CString strDefaultFontName;
	VERIFY (strDefaultFontName.LoadString (IDS_DEFAULTFONTNAME));
	m_theFont.CreateFont (25, 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,  
		strDefaultFontName);

}

// デストラクタ
CPianoRollPrintView::~CPianoRollPrintView () {
	m_theFont.DeleteObject ();
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// 時刻[tick]をx座標[*1/10mm]に変換する
long CPianoRollPrintView::TimetoX (long lTime) {
	long lTimeResolution = MIDIData_GetTimeResolution (GetDocument ()->m_pMIDIData);
	return lTime * 80 / lTimeResolution; // 4分音符を8mmと定義する。
}

// ベロシティ又は値(0～127)をy座標[*1/10mm]に変換する。
long CPianoRollPrintView::VeltoY (long lVel) {
	return lVel * PIANOROLLPRINTVIEW_VELHEIGHT / 128 + 16;
}

// ピッチベンド(0～16383)をy座標[*1/10mm]に変換する。
long CPianoRollPrintView::PitchBendtoY (long lPitchBend) {
	return lPitchBend * PIANOROLLPRINTVIEW_VELHEIGHT / 16384 + 16;
}

// テンポBPM(1～256*)をy座標[*1/10mm]に変換する。
long CPianoRollPrintView::TempoBPMtoY (long lTempoBPM) {
	return MIN (lTempoBPM, 256) * PIANOROLLPRINTVIEW_VELHEIGHT / 256 + 16;
}

// キー(0～127)をy座標[*1/10mm]に変換する。
long CPianoRollPrintView::KeyToY (long lKey) {
	return lKey * PIANOROLLPRINTVIEW_KEYHEIGHT / 128;
}


// 指定時刻にフラグとテキストを描画
void CPianoRollPrintView::DrawFlagAndText 
(CDC* pDC, long lTime, LPCTSTR lpszText, long lColor) {
	long x = TimetoX (lTime);
	CRect rcBack (x, PIANOROLLPRINTVIEW_SCALEHEIGHT / 2, x + 1024, PIANOROLLPRINTVIEW_SCALEHEIGHT);
	CRect rcFlag (x, PIANOROLLPRINTVIEW_SCALEHEIGHT * 6 / 10, x + 20, PIANOROLLPRINTVIEW_SCALEHEIGHT * 9 / 10);
	CRect rcText (x + 30, PIANOROLLPRINTVIEW_SCALEHEIGHT / 2, x + 1024, PIANOROLLPRINTVIEW_SCALEHEIGHT);
	CPen pen;
	pen.CreatePen (PS_SOLID, 1, lColor);
	CPen* pOldPen = pDC->SelectObject (&pen);
	pDC->FillSolidRect (&rcBack, ::GetSysColor (COLOR_BTNFACE));
	pDC->FillSolidRect (&rcFlag, lColor);
	pDC->MoveTo (x, PIANOROLLPRINTVIEW_SCALEHEIGHT * 5 / 10);
	pDC->LineTo (x, PIANOROLLPRINTVIEW_SCALEHEIGHT * 9 / 10);
	pDC->SetBkMode (TRANSPARENT);
	long lOldColor = pDC->GetTextColor ();
	CFont* pOldFont = pDC->SelectObject (&m_theFont);
	pDC->SetTextColor (lColor);
	pDC->DrawText (lpszText, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	pDC->SetTextColor (lOldColor);
	pDC->SelectObject (pOldFont);
	pDC->SelectObject (pOldPen);
}

// ベロシティ目盛り描画
void CPianoRollPrintView::DrawVelScaleView (CDC* pDC) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);

	// 背景グレー塗りつぶし
	pDC->FillSolidRect (0, 0, PIANOROLLPRINTVIEW_SCALEWIDTH, PIANOROLLPRINTVIEW_VELHEIGHT + 32, lColorBtnFace);

	// 目盛り横線描画
	long i;
	for (i = 0; i <= 8; i++) {
		long y = PIANOROLLPRINTVIEW_VELHEIGHT * i / 8 + 16;
		pDC->MoveTo (0, y);
		pDC->LineTo (PIANOROLLPRINTVIEW_SCALEWIDTH, y);
	}

	// 値文字描画
	pDC->SetBkMode (TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject (&m_theFont);
	// ベロシティ値又はチャンネルアフタータッチ値又はコントロールチェンジ値の描画(0～64～127)
	if (pPianoRollFrame->GetCurGraphKind () == 1 ||
		pPianoRollFrame->GetCurGraphKind () == 2 ||
		pPianoRollFrame->GetCurGraphKind () >= 4) {
		for (i = 0; i <= 8; i++) {
			long y = PIANOROLLPRINTVIEW_VELHEIGHT * i / 8 + 16;
			CRect rcText (PIANOROLLPRINTVIEW_SCALEWIDTH / 2 - 30, y - 10, PIANOROLLPRINTVIEW_SCALEWIDTH / 2 + 30, y + 10);
			pDC->FillSolidRect (rcText, lColorBtnFace);
			CString strText;
			strText.Format (_T("%d"), CLIP (0, i * 16, 127));
			pDC->DrawText (strText, rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
	}
	// ピッチベンド値の描画(-8192～0～8191)
	else if (pPianoRollFrame->GetCurGraphKind () == 3) {
		for (i = 0; i <= 8; i++) {
			long y = PIANOROLLPRINTVIEW_VELHEIGHT * i / 8 + 16;
			CRect rcText (PIANOROLLPRINTVIEW_SCALEWIDTH / 2 - 30, y - 10, PIANOROLLPRINTVIEW_SCALEWIDTH / 2 + 30, y + 10);
			pDC->FillSolidRect (rcText, lColorBtnFace);
			CString strText;
			strText.Format (_T("%d"), CLIP (-8192, (i - 4) * 2048, 8191));
			pDC->DrawText (strText, rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
	}
	// テンポ値の描画(0～256)
	else if (pPianoRollFrame->GetCurGraphKind () == 0) {
		for (i = 0; i <= 8; i++) {
			long y = PIANOROLLPRINTVIEW_VELHEIGHT * i / 8 + 16;
			CRect rcText (PIANOROLLPRINTVIEW_SCALEWIDTH / 2 - 30, y - 10, PIANOROLLPRINTVIEW_SCALEWIDTH / 2 + 30, y + 10);
			pDC->FillSolidRect (rcText, lColorBtnFace);
			CString strText;
			strText.Format (_T("%d"), CLIP (0, i * 32, 256));
			pDC->DrawText (strText, rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
	}
	pDC->SelectObject (pOldFont);

}

// ベロシティ-時刻グラフ描画
void CPianoRollPrintView::DrawVelTimeView (CDC* pDC) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;

	CPen penMeasure (PS_SOLID, 1, RGB (128, 128, 255));
	CPen penBeat (PS_SOLID, 1, RGB (192, 192, 192));
	CPen penKey (PS_SOLID, 1, RGB (192, 192, 192));
	CPen penOctave (PS_SOLID, 1, RGB (128, 128, 255));

	// MIDIデータの性質を取得
	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	long lEndTime = MIDIData_GetEndTime (pMIDIData);
	long lEndMeasure, lEndBeat, lEndTick;
	MIDIData_BreakTime (pMIDIData, lEndTime, &lEndMeasure, &lEndBeat, &lEndTick);

	// ロール部の幅を計算
	long lLastTime;
	MIDIData_MakeTime (pMIDIData, lEndMeasure + 1, 0, 0, &lLastTime);
	long lRollWidth;
	if (lTimeMode == MIDIDATA_TPQNBASE) {
		long lFeedTime = lTimeResolution * 120;
		lRollWidth = TimetoX (lLastTime + lFeedTime);
	}
	else {
		long lFeedTime = lTimeResolution * 120;
		lRollWidth = TimetoX (lLastTime + lFeedTime);
	}
	
	// ベロシティロール部横線
	long i = 0;
	for (i = 0; i <= 8; i++) {
		long y = PIANOROLLPRINTVIEW_VELHEIGHT * i / 8 + 16;
		CPen* pOldPen = pDC->SelectObject (i % 4 == 0 ? &penOctave : &penKey);
		pDC->MoveTo (0, y);
		pDC->LineTo (lRollWidth, y);
		pDC->SelectObject (pOldPen);
	}

	// ベロシティロール部縦線
	if (lTimeMode == MIDIDATA_TPQNBASE) {
		long j;
		for (j = 0; j < lEndMeasure + 120; j++) {
			CPen* pOldPen;
			// 小節線の描画
			long lTime, lnn, ldd, lcc, lbb;
			MIDIData_MakeTimeEx (pMIDIData, j, 0, 0, &lTime, &lnn, &ldd, &lcc, &lbb);
			long x = TimetoX (lTime);
			pOldPen = pDC->SelectObject (&penMeasure);
			pDC->MoveTo (x, 0);
			pDC->LineTo (x, PIANOROLLPRINTVIEW_VELHEIGHT + 32);
			pDC->SelectObject (pOldPen);
			// 拍線の描画
			pOldPen = pDC->SelectObject (&penBeat);
			long lUnitTick = lTimeResolution * 4 / (1 << ldd);
			for (long jj = 1; jj < lnn; jj++) {
				long x = TimetoX (lTime + jj * lUnitTick);
				pDC->MoveTo (x, 0);
				pDC->LineTo (x, PIANOROLLPRINTVIEW_VELHEIGHT + 32);
			}
			pDC->SelectObject (pOldPen);
		}
	}
	else {
		long lEndFrameNumber = lEndTime / lTimeResolution;
		CPen* pOldPen;
		pOldPen = pDC->SelectObject (&penMeasure);
		long j;
		for (j = 0; j <= lEndFrameNumber; j++) {
			// フレーム境界線の描画
			long lTime = j * lTimeResolution;
			long x = TimetoX (lTime);
			pDC->MoveTo (x, 0);
			pDC->LineTo (x, PIANOROLLPRINTVIEW_VELHEIGHT + 32);
		}
		pDC->SelectObject (pOldPen);
	}

	// ベロシティ値
	i = 0;
	forEachTrack (pMIDIData, pMIDITrack) {
		// そのトラックが可視状態であれば
		if (pPianoRollFrame->IsTrackVisible (i)) {
			long lOldTempo = 60000000 / 100;
			long lOldTempoTime = 0;
			CPen penControl (PS_SOLID, 1, MIDITrack_GetForeColor (pMIDITrack));
			CPen* pOldPen = NULL;
			CPen* pLastTempoPen = NULL;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				long x, y;
				long lTime = MIDIEvent_GetTime (pMIDIEvent);
				// ベロシティの描画
				if (MIDIEvent_IsNoteOn (pMIDIEvent)) {
					if (pPianoRollFrame->IsGraphVisible (1)) {
						pOldPen = pDC->SelectObject (&penControl);
						x = TimetoX (lTime);
						long lValue = MIDIEvent_GetVelocity (pMIDIEvent);
						y = VeltoY (lValue);
						long y0 = VeltoY (0);
						pDC->MoveTo (x, y);
						pDC->LineTo (x, y0);
						pDC->SelectObject (pOldPen);
					}
				}
				// チャンネルアフタータッチの描画
				else if (MIDIEvent_IsChannelAftertouch (pMIDIEvent)) {
					if (pPianoRollFrame->IsGraphVisible (2)) {
						pOldPen = pDC->SelectObject (&penControl);
						x = TimetoX (lTime);
						long lValue = MIDIEvent_GetValue (pMIDIEvent);
						y = VeltoY (lValue);
						long y0 = VeltoY (0);
						pDC->MoveTo (x, y);
						pDC->LineTo (x, y0);
						if (lValue == 0) {
							pDC->Ellipse (x - 5, y0 - 5, x + 5, y0 + 5);
						}
						pDC->SelectObject (pOldPen);
					}
				}
				// ピッチベンドの描画
				else if (MIDIEvent_IsPitchBend (pMIDIEvent)) {
					if (pPianoRollFrame->IsGraphVisible (3)) {
						pOldPen = pDC->SelectObject (&penControl);
						x = TimetoX (lTime);
						long lValue = MIDIEvent_GetValue (pMIDIEvent);
						y = PitchBendtoY (lValue);
						long yc = PitchBendtoY (8192);
						pDC->MoveTo (x, y);
						pDC->LineTo (x, yc);
						if (lValue == 8192) {
							pDC->Ellipse (x - 5, yc - 5, x + 5, yc + 5);
						}
						pDC->SelectObject (pOldPen);
					}
				}
				// コントロールチェンジの描画
				else if (MIDIEvent_IsControlChange (pMIDIEvent)) {
					long lNumber = MIDIEvent_GetNumber (pMIDIEvent);
					if (pPianoRollFrame->IsGraphVisible (4 + lNumber)) {
						pOldPen = pDC->SelectObject (&penControl);
						x = TimetoX (lTime);
						long lValue = MIDIEvent_GetValue (pMIDIEvent);
						y = VeltoY (lValue);
						long y0 = VeltoY (0);
						pDC->MoveTo (x, y);
						pDC->LineTo (x, y0);
						if (lValue == 0) {
							pDC->Ellipse (x - 5, y0 - 5, x + 5 , y0 + 5);
						}
						pDC->SelectObject (pOldPen);
					}
				}
				// テンポの描画(トラック0のみ)
				if (MIDIEvent_IsTempo (pMIDIEvent) && i == 0) {
					if (pPianoRollFrame->IsGraphVisible (0)) {
						pOldPen = pDC->SelectObject (&penControl);
						x = TimetoX (lTime);
						long xold = TimetoX (lOldTempoTime);
						long lValue = MIDIEvent_GetTempo (pMIDIEvent);
						lValue = CLIP (1, lValue, 60000000);
						y = TempoBPMtoY (60000000 / lValue);
						long yold = TempoBPMtoY (60000000 / lOldTempo);
						pDC->MoveTo (xold, yold);
						pDC->LineTo (x, yold);
						pDC->LineTo (x, y);
						lOldTempo = lValue;
						lOldTempoTime = lTime;
						pLastTempoPen = pDC->SelectObject (pOldPen);
					}
				}
			}
	
			// テンポの最終横線を描画する(トラック0のみ)
			if (pPianoRollFrame->IsGraphVisible (0) && i == 0) {
				if (pLastTempoPen) {
					pOldPen = pDC->SelectObject (pLastTempoPen);
				}
				else {
					pOldPen = pDC->SelectObject (&penControl);
				}
				long lTime = MIDIData_GetEndTime (pMIDIData);
				long x = TimetoX (lTime);
				long xold = TimetoX (lOldTempoTime);
				long yold = TempoBPMtoY (60000000 / lOldTempo);
				pDC->MoveTo (xold, yold);
				pDC->LineTo (x, yold);
				pDC->SelectObject (pOldPen);
			}
	
		}
		i++;
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();

}

// 鍵盤目盛り描画
void CPianoRollPrintView::DrawKeyScaleView (CDC* pDC) {

	long lColorBlack = RGB (0, 0, 0);
	long lColorWhite = RGB (255, 255, 255);

	// 文字背景色を透明に
	pDC->SetBkMode (TRANSPARENT);
	
	// 鍵盤部描画
	long w = PIANOROLLPRINTVIEW_SCALEWIDTH;
	long lKeyZoom = PIANOROLLPRINTVIEW_KEYHEIGHT / 128;
	long i = 0;
	// 10オクターブループ
	for (i = 0; i < 11; i++) {
		long lOctY = 12 * i * lKeyZoom;
		// 白鍵と白鍵の間の線
		pDC->MoveTo (0, lOctY + lKeyZoom * 0);
		pDC->LineTo (w, lOctY + lKeyZoom * 0);
		pDC->MoveTo (0, lOctY + lKeyZoom * 3 / 2);
		pDC->LineTo (w, lOctY + lKeyZoom * 3 / 2);
		pDC->MoveTo (0, lOctY + lKeyZoom * 7 / 2);
		pDC->LineTo (w, lOctY + lKeyZoom * 7 / 2);
		pDC->MoveTo (0, lOctY + lKeyZoom * 5);
		pDC->LineTo (w, lOctY + lKeyZoom * 5);
		pDC->MoveTo (0, lOctY + lKeyZoom * 13 / 2);
		pDC->LineTo (w, lOctY + lKeyZoom * 13 / 2);
		pDC->MoveTo (0, lOctY + lKeyZoom * 17 / 2);
		pDC->LineTo (w, lOctY + lKeyZoom * 17 / 2);
		pDC->MoveTo (0, lOctY + lKeyZoom * 21 / 2);
		pDC->LineTo (w, lOctY + lKeyZoom * 21 / 2);
		// 黒鍵部分塗りつぶし
		pDC->FillSolidRect 
			(0, lOctY +  lKeyZoom * 1, w * 2 / 4, lKeyZoom, RGB (0, 0, 0));
		pDC->FillSolidRect 
			(0, lOctY +  lKeyZoom * 3, w * 2 / 4, lKeyZoom, RGB (0, 0, 0));
		pDC->FillSolidRect 
			(0, lOctY +  lKeyZoom * 6, w * 2 / 4, lKeyZoom, RGB (0, 0, 0));
		pDC->FillSolidRect 
			(0, lOctY +  lKeyZoom * 8, w * 2 / 4, lKeyZoom, RGB (0, 0, 0));
		pDC->FillSolidRect 
			(0, lOctY +  lKeyZoom * 10, w * 2 / 4, lKeyZoom, RGB (0, 0, 0));
	}
	//pDC->MoveTo (0, 0);
	//pDC->LineTo (0, m_nKeyZoom * 128);
	//pDC->MoveTo (w - 1, 0);
	//pDC->LineTo (w - 1, m_nKeyZoom * 128);
	
	// 鍵盤文字描画
	CFont* pOldFont = pDC->SelectObject (&m_theFont);
	for (i = 0; i < 11; i++) {
		long lOctY = 12 * i * lKeyZoom;
		CRect rcText;
		rcText.left = w * 1 / 2;
		rcText.right = w;
		rcText.top = lOctY + lKeyZoom / 2 - 10;
		rcText.bottom = lOctY + lKeyZoom / 2 + 10;
		pDC->FillSolidRect (rcText, lColorWhite);
		CString strText;
		strText.Format (_T("%d"), i * 12);
		pDC->DrawText (strText, rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
	pDC->SelectObject (pOldFont);


}

// 鍵盤-時刻グラフ描画
void CPianoRollPrintView::DrawKeyTimeView (CDC* pDC) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;

	long lColorBlack = RGB (0, 0, 0);
	long lColorWhite = RGB (255, 255, 255);

	CPen penMeasure (PS_SOLID, 1, RGB (128, 128, 255));
	CPen penBeat (PS_SOLID, 1, RGB (192, 192, 192));
	CPen penKey (PS_SOLID, 1, RGB (192, 192, 192));
	CPen penOctave (PS_SOLID, 1, RGB (128, 128, 255));
	
	// MIDIデータの性質を取得
	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	long lEndTime = MIDIData_GetEndTime (pMIDIData);
	long lEndMeasure, lEndBeat, lEndTick;
	MIDIData_BreakTime (pMIDIData, lEndTime, &lEndMeasure, &lEndBeat, &lEndTick);

	// ロール部の幅を計算
	long lLastTime;
	MIDIData_MakeTime (pMIDIData, lEndMeasure + 1, 0, 0, &lLastTime);
	long lRollWidth;
	if (lTimeMode == MIDIDATA_TPQNBASE) {
		long lFeedTime = lTimeResolution * 120;
		lRollWidth = TimetoX (lLastTime + lFeedTime);
	}
	else {
		long lFeedTime = lTimeResolution * 120;
		lRollWidth = TimetoX (lLastTime + lFeedTime);
	}

	// 鍵盤ロール部横線
	long lKeyZoom = PIANOROLLPRINTVIEW_KEYHEIGHT / 128;
	long i;
	for (i = 0; i <= 127; i++) {
		if (i % 12 == 1 || i % 12 == 3 || i % 12 == 6 || i % 12 == 8 || i % 12 == 10) {
			pDC->FillSolidRect (0, i * lKeyZoom, lRollWidth, lKeyZoom, RGB (255, 255, 200));
		}
		CPen* pOldPen = pDC->SelectObject (i % 12 == 0 ? &penOctave : &penKey);
		pDC->MoveTo (0, i * lKeyZoom);
		pDC->LineTo (lRollWidth , i * lKeyZoom);
		pDC->SelectObject (pOldPen);
	}

	// 鍵盤ロール部縦線
	if (lTimeMode == MIDIDATA_TPQNBASE) {
		long j;
		for (j = 0; j < lEndMeasure + 120; j++) {
			CPen* pOldPen;
			// 小節線の描画
			long lTime, lnn, ldd, lcc, lbb;
			MIDIData_MakeTimeEx (pMIDIData, j, 0, 0, &lTime, &lnn, &ldd, &lcc, &lbb);
			long x = TimetoX (lTime);
			pOldPen = pDC->SelectObject (&penMeasure);
			pDC->MoveTo (x, 0);
			pDC->LineTo (x, PIANOROLLPRINTVIEW_KEYHEIGHT);
			pDC->SelectObject (pOldPen);
			// 拍線の描画
			pOldPen = pDC->SelectObject (&penBeat);
			long lUnitTick = lTimeResolution * 4 / (1 << ldd);
			for (long jj = 1; jj < lnn; jj++) {
				long x = TimetoX (lTime + jj * lUnitTick);
				pDC->MoveTo (x, 0);
				pDC->LineTo (x, PIANOROLLPRINTVIEW_KEYHEIGHT);
			}
			pDC->SelectObject (pOldPen);
		}
	}
	else {
		long lEndFrameNumber = lEndTime / lTimeResolution;
		CPen* pOldPen;
		pOldPen = pDC->SelectObject (&penMeasure);
		long j;
		for (j = 0; j <= lEndFrameNumber; j++) {
			// フレーム境界線の描画
			long lTime = j * lTimeResolution;
			long x = TimetoX (lTime);
			pDC->MoveTo (x, 0);
			pDC->LineTo (x, PIANOROLLPRINTVIEW_KEYHEIGHT);
		}
		pDC->SelectObject (pOldPen);
	}

	// ロール部音符バー
	i = 0;
	forEachTrack (pMIDIData, pMIDITrack) {
		if (pPianoRollFrame->IsTrackVisible (i)) {
			long lForeColor = MIDITrack_GetForeColor (pMIDITrack);
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsNoteOn (pMIDIEvent) && MIDIEvent_IsNote (pMIDIEvent)) {
					long lNoteOnTime = MIDIEvent_GetTime (pMIDIEvent);
					long lDuration = MIDIEvent_GetDuration (pMIDIEvent);
					long lNoteOffTime = lNoteOnTime + lDuration;
					long lKey = MIDIEvent_GetKey (pMIDIEvent);
					long x1 = TimetoX (lNoteOnTime);
					long x2 = TimetoX (lNoteOffTime);
					long y1 = lKey * lKeyZoom;
					long y2 = (lKey + 1) * lKeyZoom;
					pDC->FillSolidRect (CRect (x1, y1, x2, y2), lForeColor);
					pDC->FillSolidRect (CRect (x1, y2 - 1, x2, y2), lColorWhite);
					pDC->FillSolidRect (CRect (x1, y1, x1 + 1, y2), lColorWhite);
					pDC->FillSolidRect (CRect (x1, y1, x2, y1 + 1), lColorBlack);
					pDC->FillSolidRect (CRect (x2 - 1, y1, x2, y2), lColorBlack);
				}
			}
		}
		i++;
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}


// 目盛り描画
void CPianoRollPrintView::DrawScaleView (CDC* pDC) {
	// 左上余白部塗りつぶし
	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	pDC->FillSolidRect (0, 0, PIANOROLLPRINTVIEW_SCALEWIDTH, PIANOROLLPRINTVIEW_SCALEHEIGHT, lColorBtnFace);
}

// 時刻目盛り描画
void CPianoRollPrintView::DrawTimeScaleView (CDC* pDC) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;

	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);

	// MIDIデータの性質を取得
	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	long lEndTime = MIDIData_GetEndTime (pMIDIData);
	long lEndMeasure, lEndBeat, lEndTick;
	MIDIData_BreakTime (pMIDIData, lEndTime, &lEndMeasure, &lEndBeat, &lEndTick);

	// ロール部の幅を計算
	long lLastTime;
	MIDIData_MakeTime (pMIDIData, lEndMeasure + 1, 0, 0, &lLastTime);
	long lRollWidth;
	if (lTimeMode == MIDIDATA_TPQNBASE) {
		long lFeedTime = lTimeResolution * 120;
		lRollWidth = TimetoX (lLastTime + lFeedTime);
	}
	else {
		long lFeedTime = lTimeResolution * 120;
		lRollWidth = TimetoX (lLastTime + lFeedTime);
	}

	// 文字背景色を透明に
	pDC->SetBkMode (TRANSPARENT);

	// 時刻ロール部文字
	pDC->FillSolidRect (0, 0, lRollWidth, PIANOROLLPRINTVIEW_SCALEHEIGHT, lColorBtnFace);
	long lOldTime = 0;
	CString strText1;
	CString strText2;
	pMIDITrack = MIDIData_GetFirstTrack (pMIDIData);
	if (pMIDITrack) {
		long lColorTrack1 = MIDITrack_GetForeColor (pMIDITrack);
		forEachEvent (pMIDITrack, pMIDIEvent) {
			long lTime = MIDIEvent_GetTime (pMIDIEvent);
			// テンポ
			if (MIDIEvent_IsTempo (pMIDIEvent)) {
				long lTempo = MIDIEvent_GetTempo (pMIDIEvent);
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
				strText1.Format (_T("%d%s"), labs (lsf), lsf >= 0 ? "#" : "b");
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
				TCHAR szBuf[256];
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
		if (strText2 != _T("")) {
			DrawFlagAndText (pDC, lOldTime, strText2, lColorTrack1);
		}
	}

	// 時刻目盛部
	if (lTimeMode == MIDIDATA_TPQNBASE) {
		long j;
		CFont* pOldFont = pDC->SelectObject (&m_theFont);
		for (j = 0; j < lEndMeasure + 120; j++) {
			long lLeftTime;
			long lRightTime;
			MIDIData_MakeTime (pMIDIData, j, 0, 0, &lLeftTime);
			MIDIData_MakeTime (pMIDIData, j + 1, 0, 0, &lRightTime);
			long x1 = TimetoX (lLeftTime);
			long x2 = TimetoX (lRightTime);
			long y1 = 0;
			long y2 = PIANOROLLPRINTVIEW_SCALEHEIGHT / 2;
			pDC->FillSolidRect (CRect (x1, y2 - 1, x2, y2), lColorBtnHighlight);
			pDC->FillSolidRect (CRect (x1, y1, x1 + 1, y2), lColorBtnHighlight);
			pDC->FillSolidRect (CRect (x1, y1, x2, y1 + 1), lColorBtnShadow);
			pDC->FillSolidRect (CRect (x2 - 1, y1, x2, y2), lColorBtnShadow);
			CRect rcText (x1, y1, x2, y2);
			CString strText;
			strText.Format (_T("%d"), j + 1);
			pDC->DrawText (strText, rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
		pDC->SelectObject (pOldFont);
	}
	else {
		long lEndFrameNumber = lEndTime / lTimeResolution;
	}

	pSekaijuDoc->m_theCriticalSection.Unlock ();

}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// 印刷時
void CPianoRollPrintView::OnPrint (CDC* pDC, CPrintInfo* pInfo) {

	pDC->SetMapMode (MM_LOMETRIC);
	CPoint ptWindowOrg (0, 0);

	long lLeftMargin = PIANOROLLPRINTVIEW_LEFTMARGIN;
	long lRightMargin = PIANOROLLPRINTVIEW_RIGHTMARGIN;
	long lTopMargin = PIANOROLLPRINTVIEW_TOPMARGIN;
	long lBottomMargin = PIANOROLLPRINTVIEW_BOTTOMMARGIN;
	long lVelHeight = PIANOROLLPRINTVIEW_VELHEIGHT + 32;
	long lScaleHeight = PIANOROLLPRINTVIEW_SCALEHEIGHT;
	long lKeyHeight = MIN (m_sizLogPaper.cy - lVelHeight - lScaleHeight - lTopMargin - lBottomMargin, 
		PIANOROLLPRINTVIEW_KEYHEIGHT); 
	long lScaleWidth = PIANOROLLPRINTVIEW_SCALEWIDTH;
	long lTimeWidth = m_sizLogPaper.cx - lScaleWidth - lLeftMargin - lRightMargin;
	
	CRgn theRgn;
	CRect rcClip;

	// ベロシティ目盛り部
	ptWindowOrg.x = 0;
	ptWindowOrg.y = m_sizLogPaper.cy;
	pDC->SetWindowOrg (ptWindowOrg);
	rcClip.left = lLeftMargin;
	rcClip.right = lLeftMargin + lScaleWidth;
	rcClip.top = lBottomMargin;
	rcClip.bottom = lBottomMargin + lVelHeight;
	if (pDC->IsKindOf (RUNTIME_CLASS (CPreviewDC))) {
		CPreviewDC* pPreviewDC = (CPreviewDC*)pDC;
		pPreviewDC->LPtoDP (&rcClip.TopLeft ());
		pPreviewDC->LPtoDP (&rcClip.BottomRight ());
		pPreviewDC->PrinterDPtoScreenDP (&rcClip.TopLeft ());
		pPreviewDC->PrinterDPtoScreenDP (&rcClip.BottomRight ());
		CPoint ptOrg;
		::GetViewportOrgEx (pDC->m_hDC, &ptOrg);
		rcClip += ptOrg;
	}
	else {
		pDC->LPtoDP (&rcClip.TopLeft ());
		pDC->LPtoDP (&rcClip.BottomRight ());
	}
	theRgn.CreateRectRgnIndirect (rcClip);
	pDC->SelectClipRgn (&theRgn);
	ptWindowOrg.x = -lLeftMargin;
	ptWindowOrg.y = m_sizLogPaper.cy - lBottomMargin;
	pDC->SetWindowOrg (ptWindowOrg);
	DrawVelScaleView (pDC);
	pDC->SelectClipRgn (NULL);
	theRgn.DeleteObject ();
	
	// 鍵盤目盛り部
	ptWindowOrg.x = 0;
	ptWindowOrg.y = m_sizLogPaper.cy;
	pDC->SetWindowOrg (ptWindowOrg);
	rcClip.left = lLeftMargin;
	rcClip.right = lLeftMargin + lScaleWidth;
	rcClip.top = lBottomMargin + lVelHeight;
	rcClip.bottom = lBottomMargin + lVelHeight + lKeyHeight;
	if (pDC->IsKindOf (RUNTIME_CLASS (CPreviewDC))) {
		CPreviewDC* pPreviewDC = (CPreviewDC*)pDC;
		pPreviewDC->LPtoDP (&rcClip.TopLeft ());
		pPreviewDC->LPtoDP (&rcClip.BottomRight ());
		pPreviewDC->PrinterDPtoScreenDP (&rcClip.TopLeft ());
		pPreviewDC->PrinterDPtoScreenDP (&rcClip.BottomRight ());
		CPoint ptOrg;
		::GetViewportOrgEx (pDC->m_hDC, &ptOrg);
		rcClip += ptOrg;
	}
	else {
		pDC->LPtoDP (&rcClip.TopLeft ());
		pDC->LPtoDP (&rcClip.BottomRight ());
	}
	theRgn.CreateRectRgnIndirect (rcClip);
	pDC->SelectClipRgn (&theRgn);
	ptWindowOrg.x = -lLeftMargin;
	ptWindowOrg.y = m_sizLogPaper.cy - lBottomMargin - lVelHeight + (PIANOROLLPRINTVIEW_KEYHEIGHT - lKeyHeight) / 2;
	pDC->SetWindowOrg (ptWindowOrg);
	DrawKeyScaleView (pDC);
	pDC->SelectClipRgn (NULL);
	theRgn.DeleteObject ();

	// 左上余白部
	ptWindowOrg.x = 0;
	ptWindowOrg.y = m_sizLogPaper.cy;
	pDC->SetWindowOrg (ptWindowOrg);
	rcClip.left = -lLeftMargin;
	rcClip.right = lLeftMargin + lScaleWidth;
	rcClip.top = lBottomMargin + lVelHeight + lKeyHeight;
	rcClip.bottom = lBottomMargin + lVelHeight + lKeyHeight + lScaleHeight;
	if (pDC->IsKindOf (RUNTIME_CLASS (CPreviewDC))) {
		CPreviewDC* pPreviewDC = (CPreviewDC*)pDC;
		pPreviewDC->LPtoDP (&rcClip.TopLeft ());
		pPreviewDC->LPtoDP (&rcClip.BottomRight ());
		pPreviewDC->PrinterDPtoScreenDP (&rcClip.TopLeft ());
		pPreviewDC->PrinterDPtoScreenDP (&rcClip.BottomRight ());
		CPoint ptOrg;
		::GetViewportOrgEx (pDC->m_hDC, &ptOrg);
		rcClip += ptOrg;
	}
	else {
		pDC->LPtoDP (&rcClip.TopLeft ());
		pDC->LPtoDP (&rcClip.BottomRight ());
	}
	theRgn.CreateRectRgnIndirect (rcClip);
	pDC->SelectClipRgn (&theRgn);
	ptWindowOrg.x = -lLeftMargin;
	ptWindowOrg.y = m_sizLogPaper.cy - lBottomMargin - lVelHeight - lKeyHeight;
	pDC->SetWindowOrg (ptWindowOrg);
	DrawScaleView (pDC);
	pDC->SelectClipRgn (NULL);
	theRgn.DeleteObject ();

	// ベロシティロール部
	ptWindowOrg.x = 0;
	ptWindowOrg.y = m_sizLogPaper.cy;
	pDC->SetWindowOrg (ptWindowOrg);
	rcClip.left = lLeftMargin + lScaleWidth;
	rcClip.right = lLeftMargin + lScaleWidth + lTimeWidth;
	rcClip.top = lBottomMargin;
	rcClip.bottom = lBottomMargin + lVelHeight;
	if (pDC->IsKindOf (RUNTIME_CLASS (CPreviewDC))) {
		CPreviewDC* pPreviewDC = (CPreviewDC*)pDC;
		pPreviewDC->LPtoDP (&rcClip.TopLeft ());
		pPreviewDC->LPtoDP (&rcClip.BottomRight ());
		pPreviewDC->PrinterDPtoScreenDP (&rcClip.TopLeft ());
		pPreviewDC->PrinterDPtoScreenDP (&rcClip.BottomRight ());
		CPoint ptOrg;
		::GetViewportOrgEx (pDC->m_hDC, &ptOrg);
		rcClip += ptOrg;
	}
	else {
		pDC->LPtoDP (&rcClip.TopLeft ());
		pDC->LPtoDP (&rcClip.BottomRight ());
	}
	theRgn.CreateRectRgnIndirect (rcClip);
	pDC->SelectClipRgn (&theRgn);
	ptWindowOrg.x = -lLeftMargin - lScaleWidth + lTimeWidth * (pInfo->m_nCurPage - 1);
	ptWindowOrg.y = m_sizLogPaper.cy - lBottomMargin;
	pDC->SetWindowOrg (ptWindowOrg);
	DrawVelTimeView (pDC);
	pDC->SelectClipRgn (NULL);
	theRgn.DeleteObject ();
	
	// 鍵盤ロール部
	ptWindowOrg.x = 0;
	ptWindowOrg.y = m_sizLogPaper.cy;
	pDC->SetWindowOrg (ptWindowOrg);
	rcClip.left = lLeftMargin + lScaleWidth;
	rcClip.right = lLeftMargin + lScaleWidth + lTimeWidth;
	rcClip.top = lBottomMargin + lVelHeight;
	rcClip.bottom = lBottomMargin + lVelHeight + lKeyHeight;
	if (pDC->IsKindOf (RUNTIME_CLASS (CPreviewDC))) {
		CPreviewDC* pPreviewDC = (CPreviewDC*)pDC;
		pPreviewDC->LPtoDP (&rcClip.TopLeft ());
		pPreviewDC->LPtoDP (&rcClip.BottomRight ());
		pPreviewDC->PrinterDPtoScreenDP (&rcClip.TopLeft ());
		pPreviewDC->PrinterDPtoScreenDP (&rcClip.BottomRight ());
		CPoint ptOrg;
		::GetViewportOrgEx (pDC->m_hDC, &ptOrg);
		rcClip += ptOrg;
	}
	else {
		pDC->LPtoDP (&rcClip.TopLeft ());
		pDC->LPtoDP (&rcClip.BottomRight ());
	}
	theRgn.CreateRectRgnIndirect (rcClip);
	pDC->SelectClipRgn (&theRgn);
	ptWindowOrg.x = -lLeftMargin - lScaleWidth + lTimeWidth * (pInfo->m_nCurPage - 1);
	ptWindowOrg.y = m_sizLogPaper.cy - lBottomMargin - lVelHeight + (PIANOROLLPRINTVIEW_KEYHEIGHT - lKeyHeight) / 2;
	pDC->SetWindowOrg (ptWindowOrg);
	DrawKeyTimeView (pDC);
	pDC->SelectClipRgn (NULL);
	theRgn.DeleteObject ();

	// 時刻目盛り部
	ptWindowOrg.x = 0;
	ptWindowOrg.y = m_sizLogPaper.cy;
	pDC->SetWindowOrg (ptWindowOrg);
	rcClip.left = lLeftMargin + lScaleWidth;
	rcClip.right = lLeftMargin + lScaleWidth + lTimeWidth;
	rcClip.top = lBottomMargin + lVelHeight + lKeyHeight;
	rcClip.bottom = lBottomMargin + lVelHeight + lKeyHeight + lScaleHeight;
	if (pDC->IsKindOf (RUNTIME_CLASS (CPreviewDC))) {
		CPreviewDC* pPreviewDC = (CPreviewDC*)pDC;
		pPreviewDC->LPtoDP (&rcClip.TopLeft ());
		pPreviewDC->LPtoDP (&rcClip.BottomRight ());
		pPreviewDC->PrinterDPtoScreenDP (&rcClip.TopLeft ());
		pPreviewDC->PrinterDPtoScreenDP (&rcClip.BottomRight ());
		CPoint ptOrg;
		::GetViewportOrgEx (pDC->m_hDC, &ptOrg);
		rcClip += ptOrg;
	}
	else {
		pDC->LPtoDP (&rcClip.TopLeft ());
		pDC->LPtoDP (&rcClip.BottomRight ());
	}
	theRgn.CreateRectRgnIndirect (rcClip);
	pDC->SelectClipRgn (&theRgn);
	ptWindowOrg.x = -lLeftMargin - lScaleWidth + lTimeWidth * (pInfo->m_nCurPage - 1);
	ptWindowOrg.y = m_sizLogPaper.cy - lBottomMargin - lVelHeight - lKeyHeight;
	pDC->SetWindowOrg (ptWindowOrg);
	DrawTimeScaleView (pDC);
	pDC->SelectClipRgn (NULL);
	theRgn.DeleteObject ();

	// ヘッダー(タイトル)
	ptWindowOrg.x = 0;
	ptWindowOrg.y = m_sizLogPaper.cy;
	pDC->SetWindowOrg (ptWindowOrg);
	pDC->SetTextColor (RGB (0, 0 ,0));
	CRect rcText;
	rcText.left = lLeftMargin;
	rcText.right = m_sizLogPaper.cx - lRightMargin;
	rcText.top = m_sizLogPaper.cy - lTopMargin;
	rcText.bottom = m_sizLogPaper.cy - lTopMargin / 2;
	TCHAR szText[256];
	memset (szText, 0, sizeof (szText));
	MIDIData_GetTitle (GetDocument()->m_pMIDIData, szText, TSIZEOF (szText));
	CString strText;
	if (TCSLEN (szText) == 0) {
		strText = GetDocument()->GetTitle ();
	}
	else {
		strText.Format (_T("%s"), szText);
	}
	CFont* pOldFont = pDC->SelectObject (&m_theFont);
	pDC->DrawText (strText, rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	pDC->SelectObject (pOldFont);

	// フッター(ページ数/全ページ数)
	rcText.left = lLeftMargin;
	rcText.right = m_sizLogPaper.cx - lRightMargin;
	rcText.top = lTopMargin / 2;
	rcText.bottom = lTopMargin;
	strText.Format (_T("%d/%d"), pInfo->m_nCurPage, pInfo->GetMaxPage ());
	pOldFont = pDC->SelectObject (&m_theFont);
	pDC->DrawText (strText, rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	pDC->SelectObject (pOldFont);

}

// 印刷準備時
BOOL CPianoRollPrintView::OnPreparePrinting (CPrintInfo* pInfo) {
	// デフォルトの印刷準備
	return DoPreparePrinting (pInfo);
}

// 印刷開始時
void CPianoRollPrintView::OnBeginPrinting (CDC* pDC, CPrintInfo* pInfo) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	
	// 紙サイズの取得
	m_sizDevPaper.cx = pDC->GetDeviceCaps (HORZRES); // [pixel]
	m_sizDevPaper.cy = pDC->GetDeviceCaps (VERTRES); // [pixel]
	m_sizLogPaper.cx = pDC->GetDeviceCaps (HORZSIZE) * 10; // [*0.1mm]
	m_sizLogPaper.cy = pDC->GetDeviceCaps (VERTSIZE) * 10; // [*0.1mm]
	m_sizLogPrinterDPI.cx = pDC->GetDeviceCaps (LOGPIXELSX); // [dpi]
	m_sizLogPrinterDPI.cy = pDC->GetDeviceCaps (LOGPIXELSY); // [dpi]
	
	// MIDIデータの性質を取得
	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	long lEndTime = MIDIData_GetEndTime (pMIDIData);
	long lEndMeasure, lEndBeat, lEndTick;
	MIDIData_BreakTime (pMIDIData, lEndTime, &lEndMeasure, &lEndBeat, &lEndTick);

	// ロール部の幅を計算
	long lLastTime = 0;
	MIDIData_MakeTime (pMIDIData, lEndMeasure + 1, 0, 0, &lLastTime);
	long lRollWidth = this->TimetoX (lLastTime);
	long lLeftMargin = PIANOROLLPRINTVIEW_LEFTMARGIN;
	long lRightMargin = PIANOROLLPRINTVIEW_RIGHTMARGIN;
	long lScaleWidth = PIANOROLLPRINTVIEW_SCALEWIDTH;
	long lTimeWidth = m_sizLogPaper.cx - lScaleWidth - lLeftMargin - lRightMargin;
	long lNumPage = lRollWidth / MAX (lTimeWidth, 1) + 1;

	// 印刷ページ数の設定
	pInfo->SetMaxPage (lNumPage);

	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// 印刷終了時
void CPianoRollPrintView::OnEndPrinting (CDC* pDC, CPrintInfo* pInfo) {
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

