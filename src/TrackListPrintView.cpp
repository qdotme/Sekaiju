//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// トラックリスト印刷ビュークラス
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
#include <afxpriv.h>
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "common.h"
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
#include "TrackListFrame.h"
#include "TrackListPrintView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TRACKLISTPRINTVIEW_SCALEHEIGHT 80
#define TRACKLISTPRINTVIEW_SCALEWIDTH 100
#define TRACKLISTPRINTVIEW_LEFTMARGIN 200
#define TRACKLISTPRINTVIEW_RIGHTMARGIN 200
#define TRACKLISTPRINTVIEW_TOPMARGIN 200
#define TRACKLISTPRINTVIEW_BOTTOMMARGIN 200

IMPLEMENT_DYNCREATE (CTrackListPrintView, CSekaijuView)

// メッセージマップ
BEGIN_MESSAGE_MAP (CTrackListPrintView, CSekaijuView)
	ON_COMMAND (ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND (ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND (ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CTrackListPrintView::CTrackListPrintView () {
	// 印刷用文字フォントの作成(2.5ミリ)
	CString strDefaultFontName;
	VERIFY (strDefaultFontName.LoadString (IDS_DEFAULTFONTNAME));
	m_theFont.CreateFont (25, 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,  
		strDefaultFontName);

}

// デストラクタ
CTrackListPrintView::~CTrackListPrintView () {
	m_theFont.DeleteObject ();
}


//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// セルの文字列を取得する
CString CTrackListPrintView::GetCellString (long lRow, long lColumn) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	ASSERT (0 <= lRow && lRow < MAXMIDITRACKNUM);
	ASSERT (0 <= lColumn && lColumn < TRACKLISTFRAME_NUMCOLUMN);
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lRow);
	MIDIEvent* pMIDIEvent = NULL;
	CString strText;
	long lFormat = MIDIData_GetFormat (pMIDIData);
	long lValue = 0;
	long lNumber = 0;
	MIDIIn* pMIDIIn = NULL;
	MIDIOut* pMIDIOut = NULL;
	CString strNone;
	TCHAR szBuf1[2048];
	TCHAR szBuf2[2048];
	VERIFY (strNone.LoadString (IDS_NONE));
	if (pMIDITrack) {
		long lColumnContent = pTrackListFrame->GetColumnContent (lColumn);
		switch (lColumnContent & 0xFFFF) {
		// トラック名(20120107修正)
		case TRACKLISTFRAME_TRACKNAME:
			MIDITrack_GetName (pMIDITrack, szBuf1, TSIZEOF (szBuf1) - 1);
			codestr2str (szBuf1, TCSLEN (szBuf1), szBuf2, TSIZEOF (szBuf2) - 1);
			strText = szBuf2;
			break;
		// 可視(未使用)
		case TRACKLISTFRAME_VISIBLE:
			if (pSekaijuDoc->GetTrackVisible (pMIDITrack)) {
				VERIFY (strText.LoadString (IDS_SHOW));
			}
			else {
				VERIFY (strText.LoadString (IDS_HIDE));	
			}
			break;
		// イネーブル(未使用)
		case TRACKLISTFRAME_ENABLE:
			if (pSekaijuDoc->GetTrackEnable (pMIDITrack)) {
				VERIFY (strText.LoadString (IDS_ENABLE));
			}
			else {
				VERIFY (strText.LoadString (IDS_LOCK));	
			}
			break;
		// 入力ON
		case TRACKLISTFRAME_INPUTON:
			if (MIDITrack_GetInputOn (pMIDITrack)) {
				VERIFY (strText.LoadString (IDS_ON));
			}
			else {
				VERIFY (strText.LoadString (IDS_OFF));	
			}
			break;
		// 出力ON
		case TRACKLISTFRAME_OUTPUTON:
			if (MIDITrack_GetOutputOn (pMIDITrack)) {			
				VERIFY (strText.LoadString (IDS_ON));
			}
			else {
				VERIFY (strText.LoadString (IDS_OFF));	
			}
			break;
		// 入力ポート番号
		case TRACKLISTFRAME_INPUTPORT:
			lValue = MIDITrack_GetInputPort (pMIDITrack);
			ASSERT (0 <= lValue && lValue < MAXMIDIINDEVICENUM);
			pSekaijuApp->m_theCriticalSection.Lock ();
			pMIDIIn = pSekaijuApp->m_pMIDIIn[lValue];
			if (pMIDIIn) {
				strText.Format (_T("%3d-%s"), lValue + 1, pMIDIIn->m_pDeviceName);
			}
			else {
				strText.Format (_T("%3d-%s"), lValue + 1, strNone);
			}
			pSekaijuApp->m_theCriticalSection.Unlock ();
			break;
		// 入力チャンネル
		case TRACKLISTFRAME_INPUTCHANNEL:
			lValue = MIDITrack_GetInputChannel (pMIDITrack);
			if (0 <= lValue && lValue <= 15) {
				strText.Format (_T("%3d"), lValue + 1);
			}
			else {
				VERIFY (strText.LoadString (IDS_N_A));
			}
			break;
		// 出力ポート番号
		case TRACKLISTFRAME_OUTPUTPORT:
			lValue = MIDITrack_GetOutputPort (pMIDITrack);
			ASSERT (0 <= lValue && lValue < MAXMIDIOUTDEVICENUM);
			pSekaijuApp->m_theCriticalSection.Lock ();
			pMIDIOut = pSekaijuApp->m_pMIDIOut[lValue];
			if (pMIDIOut) {
				strText.Format (_T("%3d-%s"), lValue + 1, pMIDIOut->m_pDeviceName);
			}
			else {
				strText.Format (_T("%3d-%s"), lValue + 1, strNone);
			}
			pSekaijuApp->m_theCriticalSection.Unlock ();
			break;
		// 出力チャンネル
		case TRACKLISTFRAME_OUTPUTCHANNEL:
			lValue = MIDITrack_GetOutputChannel (pMIDITrack);
			if (0 <= lValue && lValue <= 15) {
				strText.Format (_T("%3d"), lValue + 1);
			}
			else {
				VERIFY (strText.LoadString (IDS_N_A));
			}
			break;
		// 表示モード(通常／ドラム)
		case TRACKLISTFRAME_VIEWMODE:
			if (MIDITrack_GetViewMode (pMIDITrack)) {
				VERIFY (strText.LoadString (IDS_DRUM));
			}
			else {
				VERIFY (strText.LoadString (IDS_NORM));
			}
			break;
		// コントロールチェンジ
		case TRACKLISTFRAME_CONTROLCHANGE:
			lValue = -1;
			lNumber = pTrackListFrame->GetColumnContent (lColumn) >> 16;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsControlChange (pMIDIEvent)) {
					if (MIDIEvent_GetNumber (pMIDIEvent) == lNumber) {
						lValue = MIDIEvent_GetValue (pMIDIEvent);
						break;
					}
				}
			}
			if (0 <= lValue && lValue <= 127) {
				strText.Format (_T("%3d"), lValue);
			}
			else {
				strText = _T("---");
			}
			break;
		// プログラムチェンジ
		case TRACKLISTFRAME_PROGRAMCHANGE:
			lValue = -1;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsProgramChange (pMIDIEvent)) {
					lValue = MIDIEvent_GetNumber (pMIDIEvent);
					break;
				}
			}
			if (0 <= lValue && lValue <= 127 && pMIDIEvent) {
				long lTrackOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
				ASSERT (0 <= lTrackOutputPort && lTrackOutputPort < 16);
				long lTrackOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
				ASSERT (-1 <= lTrackOutputChannel && lTrackOutputChannel < 16);
				if (lTrackOutputChannel == -1) {
					lTrackOutputChannel = MIDIEvent_GetChannel (pMIDIEvent);
				}
				ASSERT (0 <= lTrackOutputChannel && lTrackOutputChannel < 16);
				long lTrackViewMode  = MIDITrack_GetViewMode (pMIDITrack);
				TCHAR szBuf[256];
				memset (szBuf, 0, sizeof (szBuf));
				long lBankMSB = MIDIEvent_GetBankMSB (pMIDIEvent);
				long lBankLSB = MIDIEvent_GetBankLSB (pMIDIEvent);
				long lBank = (lBankMSB << 7) | lBankLSB; //MIDIEvent_GetBank (pMIDIEvent);
				MIDIInstrumentDefinition* pInstDef = NULL;
				// このトラックの表示モードが「通常」の場合
				if (lTrackViewMode == 0) {
					pInstDef = pSekaijuApp->m_pMIDIInstDefNorm[lTrackOutputPort];
				}
				// このトラックの表示モードが「ドラム」の場合
				else {
					pInstDef = pSekaijuApp->m_pMIDIInstDefDrum[lTrackOutputPort];
				}
				// このトラックのインストゥルメント定義が見つかった
				if (pInstDef) {
					MIDIPatchNameTable* pPatchNameTable =
						MIDIInstrumentDefinition_GetPatchNameTable (pInstDef, lBank);
					// このインストゥルメント定義の指定バンクのPatchNameTableが見つかった
					if (pPatchNameTable) {
						TCHAR szBuf[256];
						memset (szBuf, 0, 256);
						MIDIPatchNameTable_GetName 
							(pPatchNameTable, lValue, szBuf, 255);
						strText.Format (_T("%d-%s"), lValue, szBuf);
					}
					else {
						strText.Format (_T("%d"), lValue);
					}
				}
				else {
					strText.Format (_T("%d"), lValue);
				}
			}
			else {
				strText = _T("---");
			}
			break;
		// タイム+
		case TRACKLISTFRAME_TIMEPLUS:
			if (lFormat == 1 && lRow == 0) {
				strText.Format (_T("---"));
			}
			else {
				lValue = MIDITrack_GetTimePlus (pMIDITrack);
				strText.Format (_T("%3d"), lValue);
			}
			break;
		// キー+
		case TRACKLISTFRAME_KEYPLUS:
			if (lFormat == 1 && lRow == 0) {
				strText.Format (_T("---"));
			}
			else {
				lValue = MIDITrack_GetKeyPlus (pMIDITrack);
				strText.Format (_T("%3d"), lValue);
			}
			break;
		// ベロシティ+
		case TRACKLISTFRAME_VELOCITYPLUS:
			if (lFormat == 1 && lRow == 0) {
				strText.Format (_T("---"));
			}
			else {
				lValue = MIDITrack_GetVelocityPlus (pMIDITrack);
				strText.Format (_T("%3d"), lValue);
			}
			break;
		// イベント数
		case TRACKLISTFRAME_NUMEVENT:
			lValue = MIDITrack_CountEvent (pMIDITrack);
			strText.Format (_T("%8d"), lValue);
			break;
		}
	}
	return strText;
}

// 時刻[tick]をx座標[*1/10mm]に変換する
long CTrackListPrintView::TimetoX (long lTime) {
	// 4分音符を2mmとする。
	long lTimeResolution = MIDIData_GetTimeResolution (GetDocument ()->m_pMIDIData);
	return lTime * 20 / lTimeResolution;
}



// トラック番号部描画
void CTrackListPrintView::DrawIndexScaleView (CDC* pDC, CPrintInfo* pInfo) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;

	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);

	pDC->SetBkMode (TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject (&m_theFont);

	long lRowZoom = 80;
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	long lRowCount = m_lNumTrackPerPage * m_lMaxRowPage;
	long lCurPage = pInfo->m_nCurPage - 1;
	long lCurRowPage = lCurPage % m_lMaxRowPage;
	long lCurColPage = lCurPage / m_lMaxRowPage;
	long i;
	long iMin = m_lNumTrackPerPage * lCurRowPage;
	long iMax = m_lNumTrackPerPage * (lCurRowPage + 1);
	for (i = iMin; i < iMax; i++) {
		long x1 = 0;
		long x2 = TRACKLISTPRINTVIEW_SCALEWIDTH;
		long y1 = lRowZoom * (lRowCount - i);
		long y2 = y1 - lRowZoom;
		pDC->FillSolidRect (CRect (x1, y1, x2, y2), lColorBtnFace);
		pDC->FillSolidRect (CRect (x1, y2 - 1, x2, y2), lColorBtnHighlight);
		pDC->FillSolidRect (CRect (x1, y1, x1 + 1, y2), lColorBtnHighlight);
		pDC->FillSolidRect (CRect (x1, y1, x2, y1 + 1), lColorBtnShadow);
		pDC->FillSolidRect (CRect (x2 - 1, y1, x2, y2), lColorBtnShadow);
		CRect rcText (x1, y1, x2, y2);
		CString strText;
		strText.Format (_T("%d"), pSekaijuApp->m_theGeneralOption.m_bEventZeroOrigin ? i : i + 1);
		pDC->DrawText (strText, rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}

	pDC->SelectObject (pOldFont);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// 左上描画
void CTrackListPrintView::DrawScaleView (CDC* pDC, CPrintInfo* pInfo) {
	// 左上余白部描画
	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	pDC->FillSolidRect (0, 0, TRACKLISTPRINTVIEW_SCALEWIDTH, TRACKLISTPRINTVIEW_SCALEHEIGHT, lColorBtnFace);
}

// リスト部描画
void CTrackListPrintView::DrawIndexPropertyView (CDC* pDC, CPrintInfo* pInfo) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;

	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);
	long lColorBlack = RGB (0, 0, 0);
	long lColorWhite = RGB (255, 255, 255);

	pDC->SetBkMode (TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject (&m_theFont);

	CPen thePen (PS_SOLID, 1, RGB (128, 128, 128));
	CPen* pOldPen = pDC->SelectObject (&thePen);

	long lRowZoom = 80;
	long lLeftMargin = TRACKLISTPRINTVIEW_LEFTMARGIN;
	long lRightMargin = TRACKLISTPRINTVIEW_RIGHTMARGIN;
	long lScaleWidth = TRACKLISTPRINTVIEW_SCALEWIDTH;

	long i, j;
	// 行の背景色描画
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	long lRowCount = m_lNumTrackPerPage * m_lMaxRowPage;
	long lCurPage = pInfo->m_nCurPage - 1;
	long lCurRowPage = lCurPage % m_lMaxRowPage;
	long lCurColPage = lCurPage / m_lMaxRowPage;
	long iMin = m_lNumTrackPerPage * lCurRowPage;
	long iMax = m_lNumTrackPerPage * (lCurRowPage + 1);
	for (i = iMin; i < iMax; i++) {
		long x1 = 0;
		long x2 = (m_sizLogPaper.cx - lLeftMargin - lRightMargin - lScaleWidth) * 2;
		long y1 = lRowZoom * (lRowCount - i);
		long y2 = y1 - lRowZoom;
		long lBackColor = (i % 2 ? RGB(255,255,192) : RGB (255,255,255));
		pDC->FillSolidRect (CRect (x1, y1, x2, y2), lBackColor);
	}

	// 横線の描画
	for (i = iMin; i <= iMax; i++) {
		long x1 = 0;
		long x2 = (m_sizLogPaper.cx - lLeftMargin - lRightMargin - lScaleWidth) * 2;
		long y1 = lRowZoom * (lRowCount - i);
		long y2 = y1 - lRowZoom;
		pDC->MoveTo (x1, y1 + 1);
		pDC->LineTo (x2, y1 + 1);
	}

	// 縦線の描画
	long lWidthDiv[] = {0, 7, 8, 10, 16, 18, 20, 26, 28, 32, 34, 36, 44, 46, 48, 50, 52, 54, 56, 58, 60, 64};
	for (j = 0; j < TRACKLISTFRAME_NUMCOLUMN; j++) {
		long x1 = (m_sizLogPaper.cx - lLeftMargin - lRightMargin - lScaleWidth) * lWidthDiv[j] / 32;
		long x2 = (m_sizLogPaper.cx - lLeftMargin - lRightMargin - lScaleWidth) * lWidthDiv[j + 1] / 32;
		long y1 = 0;
		long y2 = lRowZoom * lRowCount;
		pDC->MoveTo (x1 - 1, y1);
		pDC->LineTo (x1 - 1, y2);
	}

	// 各セルの文字などの描画
	pDC->SetBkMode (TRANSPARENT);
	for (i = iMin; i < iMax; i++) {
		if (i < lTrackCount) {
			pMIDITrack = pSekaijuDoc->GetTrack (i);
		}
		else {
			pMIDITrack = NULL;
		}
		if (pMIDITrack) {
			long lTrackForeColor = MIDITrack_GetForeColor (pMIDITrack);
			pDC->SetTextColor (lTrackForeColor);
			// 各列について
			for (j = 0; j < TRACKLISTFRAME_NUMCOLUMN; j++) {
				long x1 = (m_sizLogPaper.cx - lLeftMargin - lRightMargin - lScaleWidth) * lWidthDiv[j] / 32;
				long x2 = (m_sizLogPaper.cx - lLeftMargin - lRightMargin - lScaleWidth) * lWidthDiv[j + 1] / 32;
				long y1 = lRowZoom * (lRowCount - i);
				long y2 = y1 - lRowZoom;
				CRect rcCell (x1, y1, x2, y2);
				CRect rcText = rcCell;
				long lColumnContent = pTrackListFrame->GetColumnContent (j);
				if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
					rcText.right -= 10;
				}
				// 各セルの文字描画
				CString strCellString;
				CRect rcColor;
				switch (lColumnContent & 0xFFFF) {
				case TRACKLISTFRAME_FORECOLOR:
					rcColor.left = rcCell.left + 10;
					rcColor.right = rcCell.right - 10;
					rcColor.top = rcCell.bottom + lRowZoom / 2 - 10;
					rcColor.bottom = rcCell.bottom + lRowZoom / 2 + 10;
					pDC->FillSolidRect (&rcColor, lTrackForeColor); 
					break;
				case TRACKLISTFRAME_TRACKNAME:
				case TRACKLISTFRAME_VISIBLE:
				case TRACKLISTFRAME_ENABLE:
				case TRACKLISTFRAME_INPUTON:
				case TRACKLISTFRAME_INPUTPORT:
				case TRACKLISTFRAME_INPUTCHANNEL:
				case TRACKLISTFRAME_OUTPUTON:
				case TRACKLISTFRAME_OUTPUTPORT:
				case TRACKLISTFRAME_OUTPUTCHANNEL:
				case TRACKLISTFRAME_VIEWMODE:
				case TRACKLISTFRAME_CONTROLCHANGE:
				case TRACKLISTFRAME_PROGRAMCHANGE:
				case TRACKLISTFRAME_TIMEPLUS:
				case TRACKLISTFRAME_KEYPLUS:
				case TRACKLISTFRAME_VELOCITYPLUS:
				case TRACKLISTFRAME_NUMEVENT:
					strCellString = GetCellString (i, j);
					pDC->DrawText (strCellString, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
					break;
				}
			}
		}
	}	
	
	pDC->SelectObject (pOldPen);
	pDC->SelectObject (pOldFont);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// 上部項目部描画
void CTrackListPrintView::DrawPropertyScaleView (CDC* pDC, CPrintInfo* pInfo) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);
	long lColorBlack = RGB (0, 0, 0);
	long lColorWhite = RGB (255, 255, 255);
	long lLeftMargin = TRACKLISTPRINTVIEW_LEFTMARGIN;
	long lRightMargin = TRACKLISTPRINTVIEW_RIGHTMARGIN;
	long lScaleWidth = TRACKLISTPRINTVIEW_SCALEWIDTH;
	pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextColor (RGB (0, 0, 0));
	CFont* pOldFont = pDC->SelectObject (&m_theFont);
	long lWidthDiv[] = {0, 7, 8, 10, 16, 18, 20, 26, 28, 32, 34, 36, 44, 46, 48, 50, 52, 54, 56, 58, 60, 64};
	for (long j = 0; j < 21; j++) {
		long x1 = (m_sizLogPaper.cx - lLeftMargin - lRightMargin - lScaleWidth) * lWidthDiv[j] / 32;
		long x2 = (m_sizLogPaper.cx - lLeftMargin - lRightMargin - lScaleWidth) * lWidthDiv[j + 1] / 32;
		long y1 = 0;
		long y2 = TRACKLISTPRINTVIEW_SCALEHEIGHT;
		pDC->FillSolidRect (CRect (x1, y1, x2, y2), lColorBtnFace);
		pDC->FillSolidRect (CRect (x1, y2 - 1, x2, y2), lColorBtnHighlight);
		pDC->FillSolidRect (CRect (x1, y1, x1 + 1, y2), lColorBtnHighlight);
		pDC->FillSolidRect (CRect (x1, y1, x2, y1 + 1), lColorBtnShadow);
		pDC->FillSolidRect (CRect (x2 - 1, y1, x2, y2), lColorBtnShadow);
		CRect rcText (x1, y1, x2, y2);
		CString strText (pTrackListFrame->GetColumnTitle (j));
		pDC->DrawText (strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
	pDC->SelectObject (pOldFont);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}


// トラックプレビュー部描画
void CTrackListPrintView::DrawTrackTimeView (CDC* pDC, CPrintInfo* pInfo) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;

	long lEndTime, lEndMeasure, lEndBeat, lEndTick;
	lEndTime = MIDIData_GetEndTime (pMIDIData);
	MIDIData_BreakTime (pMIDIData, lEndTime, &lEndMeasure, &lEndBeat, &lEndTick);

	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);
	long lColorBlack = RGB (0, 0, 0);
	long lColorWhite = RGB (255, 255, 255);

	pDC->SetBkMode (TRANSPARENT);

	long lRowZoom = 80;
	long lLeftMargin = TRACKLISTPRINTVIEW_LEFTMARGIN;
	long lRightMargin = TRACKLISTPRINTVIEW_RIGHTMARGIN;
	long lScaleWidth = TRACKLISTPRINTVIEW_SCALEWIDTH;

	long i, j;
	// 行の背景色描画
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	long lRowCount = m_lNumTrackPerPage * m_lMaxRowPage;
	long lCurPage = pInfo->m_nCurPage - 1;
	long lCurRowPage = (lCurPage - m_lMaxRowPage * m_lMaxColPage) % m_lMaxRowPage;
	long lCurRollPage = (lCurPage - m_lMaxRowPage * m_lMaxColPage) / m_lMaxRowPage;
	long iMin = m_lNumTrackPerPage * lCurRowPage;
	long iMax = m_lNumTrackPerPage * (lCurRowPage + 1);
	for (i = iMin; i < iMax; i++) {
		long x1 = 0;
		long x2 = (m_sizLogPaper.cx - lLeftMargin - lRightMargin - lScaleWidth) * m_lMaxRollPage;
		long y1 = lRowZoom * (lRowCount - i);
		long y2 = y1 - lRowZoom;
		long lBackColor = (i % 2 ? RGB(255,255,192) : RGB (255,255,255));
		pDC->FillSolidRect (CRect (x1, y1, x2, y2), lBackColor);
	}

	// 横線の描画
	CPen thePen (PS_SOLID, 1, RGB (128, 128, 128));
	CPen* pOldPen = pDC->SelectObject (&thePen);
	for (i = iMin; i <= iMax; i++) {
		long x1 = 0;
		long x2 = (m_sizLogPaper.cx - lLeftMargin - lRightMargin - lScaleWidth) * m_lMaxRollPage;
		long y1 = lRowZoom * (lRowCount - i);
		long y2 = y1 - lRowZoom;
		pDC->MoveTo (x1, y1 + 1);
		pDC->LineTo (x2, y1 + 1);
	}
	pDC->SelectObject (pOldPen);

	// 縦線(小節境界線又はフレーム境界線)の描画
	CPen penMeasure (PS_SOLID, 1, RGB (128, 128, 255));
	pOldPen = pDC->SelectObject (&penMeasure);
	for (j = 0; j < lEndMeasure + 120; j++) {
		long lTime;
		MIDIData_MakeTime (pMIDIData, j, 0, 0, &lTime);
		long x = this->TimetoX (lTime);
		long y1 = 0;
		long y2 = lRowZoom * lRowCount;
		pDC->SelectObject (&penMeasure);
		pDC->MoveTo (x, y1);
		pDC->LineTo (x, y2);
	}
	pDC->SelectObject (pOldPen);

	// データ部の描画
	i = 0;
	forEachTrack (pMIDIData, pMIDITrack) {
		if (iMin <= i && i < __min (iMax, lTrackCount)) {
			long lTrackColor = MIDITrack_GetForeColor (pMIDITrack);
			long y = lRowZoom * (lRowCount - i);
			CPen theTrackPen (PS_SOLID, 1, lTrackColor);
			pOldPen = pDC->SelectObject (&theTrackPen);
			forEachEvent (pMIDITrack, pMIDIEvent) {
				// ノートイベントの描画
				if (MIDIEvent_IsNote (pMIDIEvent)) {
					long lTime = MIDIEvent_GetTime (pMIDIEvent);
					long lKey = MIDIEvent_GetKey (pMIDIEvent);
					long lDur = MIDIEvent_GetDuration (pMIDIEvent);
					long x1 = this->TimetoX (lTime);
					long x2 = this->TimetoX (lTime + lDur) + 1;
					long y1 = y - lRowZoom + lRowZoom * lKey / 128;
					long y2 = y1 + 1;
					//CRect rcNote (x1, y1, x2, y2);
					//pDC->FillSolidRect (&rcNote, lTrackColor);
					pDC->MoveTo (x1, y1);
					pDC->LineTo (x2, y1);
				}
				// キーアフタータッチの描画
				else if (MIDIEvent_IsKeyAftertouch (pMIDIEvent)) {
					long lTime = MIDIEvent_GetTime (pMIDIEvent);
					long lValue = MIDIEvent_GetValue (pMIDIEvent);
					long x1 = this->TimetoX (lTime);
					long x2 = x1 + 1;
					long y1 = y - lRowZoom;
					long y2 = y1 + lRowZoom * (lValue) / 128;
					//CRect rcNote (x1, y1, x2, y2);
					//pDC->FillSolidRect (&rcNote, lTrackColor);
					pDC->MoveTo (x1, y1);
					pDC->LineTo (x1, y2);
					if (lValue == 0) {
						pDC->Ellipse (x1 - 5, y1 - 5, x1 + 5, y1 + 5); 
					}
				}
				// コントロールチェンジイベントの描画
				else if (MIDIEvent_IsControlChange (pMIDIEvent)) {
					long lTime = MIDIEvent_GetTime (pMIDIEvent);
					long lValue = MIDIEvent_GetValue (pMIDIEvent);
					long x1 = this->TimetoX (lTime);
					long x2 = x1 + 1;
					long y1 = y - lRowZoom;
					long y2 = y1 + lRowZoom * (lValue) / 128;
					//CRect rcNote (x1, y1, x2, y2);
					//pDC->FillSolidRect (&rcNote, lTrackColor);
					pDC->MoveTo (x1, y1);
					pDC->LineTo (x1, y2);
					if (lValue == 0) {
						pDC->Ellipse (x1 - 5, y1 - 5, x1 + 5, y1 + 5); 
					}
				}
				// プログラムチェンジイベントの描画
				else if (MIDIEvent_IsProgramChange (pMIDIEvent)) {
					long lTime = MIDIEvent_GetTime (pMIDIEvent);
					long lValue = MIDIEvent_GetValue (pMIDIEvent);
					long x1 = this->TimetoX (lTime);
					long x2 = x1 + 1;
					long y1 = y - lRowZoom;
					long y2 = y1 + lRowZoom * (lValue) / 128;
					//CRect rcNote (x1, y1, x2, y2);
					//pDC->FillSolidRect (&rcNote, lTrackColor);
					pDC->MoveTo (x1, y1);
					pDC->LineTo (x1, y2);
					if (lValue == 0) {
						pDC->Ellipse (x1 - 5, y1 - 5, x1 + 5, y1 + 5); 
					}
				}
				// チャンネルアフタータッチの描画
				else if (MIDIEvent_IsChannelAftertouch (pMIDIEvent)) {
					long lTime = MIDIEvent_GetTime (pMIDIEvent);
					long lValue = MIDIEvent_GetValue (pMIDIEvent);
					long x1 = this->TimetoX (lTime);
					long x2 = x1 + 1;
					long y1 = y - lRowZoom;
					long y2 = y1 + lRowZoom * (lValue) / 128;
					//CRect rcNote (x1, y1, x2, y2);
					//pDC->FillSolidRect (&rcNote, lTrackColor);
					pDC->MoveTo (x1, y1);
					pDC->LineTo (x1, y2);
					if (lValue == 0) {
						pDC->Ellipse (x1 - 5, y1 - 5, x1 + 5, y1 + 5); 
					}
				}
				// ピッチベンドの描画
				else if (MIDIEvent_IsPitchBend (pMIDIEvent)) {
					long lTime = MIDIEvent_GetTime (pMIDIEvent);
					long lValue = MIDIEvent_GetValue (pMIDIEvent);
					long x1 = this->TimetoX (lTime);
					long x2 = x1 + 1;
					long y1 = y - lRowZoom / 2;
					long y2 = y1 + lRowZoom * (lValue - 8192) / 16384;
					//CRect rcNote (x1, y1, x2, y2);
					//pDC->FillSolidRect (&rcNote, lTrackColor);
					pDC->MoveTo (x1, y1);
					pDC->LineTo (x1, y2);
					if (lValue == 8192) {
						pDC->Ellipse (x1 - 5, y1 - 5, x1 + 5, y1 + 5); 
					}
				}
			}
			pDC->SelectObject (pOldPen);
		}
		i++;
	}
	
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

//
void CTrackListPrintView::DrawFlagAndText (CDC* pDC, long lTime, LPCTSTR lpszText, long lColor) {
	long lScaleHeight = TRACKLISTPRINTVIEW_SCALEHEIGHT;
	long x = this->TimetoX (lTime);
	CRect rcBack (x, lScaleHeight, x + 1024, lScaleHeight / 2);
	CRect rcFlag (x, lScaleHeight - 10, x + 16, lScaleHeight / 2 + 10);
	CRect rcText (x + 20, lScaleHeight, x + 1024, lScaleHeight / 2);
	CPen pen;
	pen.CreatePen (PS_SOLID, 1, lColor);
	CPen* pOldPen = pDC->SelectObject (&pen);
	pDC->FillSolidRect (&rcBack, ::GetSysColor (COLOR_BTNFACE));
	pDC->FillSolidRect (&rcFlag, lColor);
	pDC->MoveTo (x, lScaleHeight - 10);
	pDC->LineTo (x, lScaleHeight / 2);
	pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextColor (lColor);
	pDC->DrawText (lpszText, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	pDC->SelectObject (pOldPen);
}

// 上部時刻目盛り部描画
void CTrackListPrintView::DrawTimeScaleView (CDC* pDC, CPrintInfo* pInfo) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;

	long lEndTime, lEndMeasure, lEndBeat, lEndTick;
	lEndTime = MIDIData_GetEndTime (pMIDIData);
	MIDIData_BreakTime (pMIDIData, lEndTime, &lEndMeasure, &lEndBeat, &lEndTick);

	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);
	long lColorBlack = RGB (0, 0, 0);
	long lColorWhite = RGB (255, 255, 255);
	long lLeftMargin = TRACKLISTPRINTVIEW_LEFTMARGIN;
	long lRightMargin = TRACKLISTPRINTVIEW_RIGHTMARGIN;
	long lScaleWidth = TRACKLISTPRINTVIEW_SCALEWIDTH;

	long j;

	pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextColor (RGB (0, 0, 0));

	CFont* pOldFont = pDC->SelectObject (&m_theFont);

	// エリア全体塗りつぶし
	long x1 = 0;
	long x2 = (m_sizLogPaper.cx - lLeftMargin - lRightMargin - lScaleWidth) * m_lMaxRollPage;
	long y1 = 0;
	long y2 = TRACKLISTPRINTVIEW_SCALEHEIGHT;
	pDC->FillSolidRect (CRect (x1, y1, x2, y2), lColorBtnFace);
	
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
			if (0 <= lTime && lTime <= lEndTime) {
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
					TCHAR szBuf[256];
					memset (szBuf, 0, sizeof (szBuf));
					MIDIEvent_GetText (pMIDIEvent, szBuf, sizeof (szBuf) - 1);
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
	pDC->SetTextColor (RGB (0, 0, 0));
	for (j = 0; j <= lEndMeasure + 120; j++) {
		long lTime1 = 0;
		long lTime2 = 0;
		MIDIData_MakeTime (pMIDIData, j, 0, 0, &lTime1);
		MIDIData_MakeTime (pMIDIData, j + 1, 0, 0, &lTime2);
		long x1 = this->TimetoX (lTime1);
		long x2 = this->TimetoX (lTime2);
		long y1 = 0;
		long y2 = TRACKLISTPRINTVIEW_SCALEHEIGHT / 2;
		pDC->FillSolidRect (CRect (x1, y1, x2, y2), lColorBtnFace);
		pDC->FillSolidRect (CRect (x1, y2 - 1, x2, y2), lColorBtnHighlight);
		pDC->FillSolidRect (CRect (x1, y1, x1 + 1, y2), lColorBtnHighlight);
		pDC->FillSolidRect (CRect (x1, y1, x2, y1 + 1), lColorBtnShadow);
		pDC->FillSolidRect (CRect (x2 - 1, y1, x2, y2), lColorBtnShadow);
		CRect rcText (x1, y1, x2, y2);
		CString strText;
		strText.Format (_T("%d"), j + 1);
		pDC->DrawText (strText, &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	pDC->SelectObject (pOldFont);
	
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}






//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// 印刷時
void CTrackListPrintView::OnPrint (CDC* pDC, CPrintInfo* pInfo) {

	pDC->SetMapMode (MM_LOMETRIC);
	CPoint ptWindowOrg (0, 0);

	long lLeftMargin = TRACKLISTPRINTVIEW_LEFTMARGIN;
	long lRightMargin = TRACKLISTPRINTVIEW_RIGHTMARGIN;
	long lTopMargin = TRACKLISTPRINTVIEW_TOPMARGIN;
	long lBottomMargin = TRACKLISTPRINTVIEW_BOTTOMMARGIN;
	long lScaleHeight = TRACKLISTPRINTVIEW_SCALEHEIGHT;
	long lScaleWidth = TRACKLISTPRINTVIEW_SCALEWIDTH;
	long lListHeight = (m_sizLogPaper.cy - lScaleHeight - lTopMargin - lBottomMargin) / 80 * 80;
	long lListWidth = (m_sizLogPaper.cx - lScaleWidth- lLeftMargin - lRightMargin);

	long lCurPage = pInfo->m_nCurPage - 1;
	long lCurRowPage = 0;
	long lCurColPage = 0;
	long lCurRollPage = 0;
	if (lCurPage < m_lMaxColPage * m_lMaxRowPage) {
		lCurRowPage = lCurPage % m_lMaxRowPage;
		lCurColPage = lCurPage / m_lMaxRowPage;
	}
	else {
		lCurRowPage = (lCurPage - m_lMaxColPage * m_lMaxRowPage) % m_lMaxRowPage;
		lCurRollPage = (lCurPage - m_lMaxColPage * m_lMaxRowPage) / m_lMaxRowPage;
	}

	CRgn theRgn;
	CRect rcClip;

	// トラック番号部
	ptWindowOrg.x = 0;
	ptWindowOrg.y = m_sizLogPaper.cy;
	pDC->SetWindowOrg (ptWindowOrg);
	rcClip.left = lLeftMargin;
	rcClip.right = lLeftMargin + lScaleWidth;
	rcClip.top = lBottomMargin;
	rcClip.bottom = lBottomMargin + lListHeight;
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
	ptWindowOrg.y = m_sizLogPaper.cy - lBottomMargin + lListHeight * (m_lMaxRowPage - lCurRowPage - 1);
	pDC->SetWindowOrg (ptWindowOrg);
	DrawIndexScaleView (pDC, pInfo);
	pDC->SelectClipRgn (NULL);
	theRgn.DeleteObject ();
	
	// 左上余白部
	ptWindowOrg.x = 0;
	ptWindowOrg.y = m_sizLogPaper.cy;
	pDC->SetWindowOrg (ptWindowOrg);
	rcClip.left = lLeftMargin;
	rcClip.right = lLeftMargin + lScaleWidth;
	rcClip.top = lBottomMargin + lListHeight;
	rcClip.bottom = lBottomMargin + lListHeight + lScaleHeight;
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
	ptWindowOrg.y = m_sizLogPaper.cy - lBottomMargin - lListHeight;
	pDC->SetWindowOrg (ptWindowOrg);
	DrawScaleView (pDC, pInfo);
	pDC->SelectClipRgn (NULL);
	theRgn.DeleteObject ();

	if (lCurPage < m_lMaxColPage * m_lMaxRowPage) {

		// トラックリスト部
		ptWindowOrg.x = 0;
		ptWindowOrg.y = m_sizLogPaper.cy;
		pDC->SetWindowOrg (ptWindowOrg);
		rcClip.left = lLeftMargin + lScaleWidth;
		rcClip.right = lLeftMargin + lScaleWidth + lListWidth;
		rcClip.top = lBottomMargin;
		rcClip.bottom = lBottomMargin + lListHeight;
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
		ptWindowOrg.x = -lLeftMargin - lScaleWidth + lListWidth * lCurColPage;
		ptWindowOrg.y = m_sizLogPaper.cy - lBottomMargin + lListHeight * (m_lMaxRowPage - lCurRowPage - 1);
		pDC->SetWindowOrg (ptWindowOrg);
		DrawIndexPropertyView (pDC, pInfo);
		pDC->SelectClipRgn (NULL);
		theRgn.DeleteObject ();
		

		// 上部リスト項目名部
		ptWindowOrg.x = 0;
		ptWindowOrg.y = m_sizLogPaper.cy;
		pDC->SetWindowOrg (ptWindowOrg);
		rcClip.left = lLeftMargin + lScaleWidth;
		rcClip.right = lLeftMargin + lScaleWidth + lListWidth;
		rcClip.top = lBottomMargin + lListHeight;
		rcClip.bottom = lBottomMargin + lListHeight + lScaleHeight;
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
		ptWindowOrg.x = -lLeftMargin - lScaleWidth + lListWidth * lCurColPage;
		ptWindowOrg.y = m_sizLogPaper.cy - lBottomMargin - lListHeight;
		pDC->SetWindowOrg (ptWindowOrg);
		DrawPropertyScaleView (pDC, pInfo);
		pDC->SelectClipRgn (NULL);
		theRgn.DeleteObject ();
	}

	else {

		// トラックプレビュー部
		ptWindowOrg.x = 0;
		ptWindowOrg.y = m_sizLogPaper.cy;
		pDC->SetWindowOrg (ptWindowOrg);
		rcClip.left = lLeftMargin + lScaleWidth;
		rcClip.right = lLeftMargin + lScaleWidth + lListWidth;
		rcClip.top = lBottomMargin;
		rcClip.bottom = lBottomMargin + lListHeight;
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
		ptWindowOrg.x = -lLeftMargin - lScaleWidth + lListWidth * lCurRollPage;
		ptWindowOrg.y = m_sizLogPaper.cy - lBottomMargin + lListHeight * (m_lMaxRowPage - lCurRowPage - 1);
		pDC->SetWindowOrg (ptWindowOrg);
		DrawTrackTimeView (pDC, pInfo);
		pDC->SelectClipRgn (NULL);
		theRgn.DeleteObject ();

		// 上部時刻目盛り部
		ptWindowOrg.x = 0;
		ptWindowOrg.y = m_sizLogPaper.cy;
		pDC->SetWindowOrg (ptWindowOrg);
		rcClip.left = lLeftMargin + lScaleWidth;
		rcClip.right = lLeftMargin + lScaleWidth + lListWidth;
		rcClip.top = lBottomMargin + lListHeight;
		rcClip.bottom = lBottomMargin + lListHeight + lScaleHeight;
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
		ptWindowOrg.x = -lLeftMargin - lScaleWidth + lListWidth * lCurRollPage;
		ptWindowOrg.y = m_sizLogPaper.cy - lBottomMargin - lListHeight;
		pDC->SetWindowOrg (ptWindowOrg);
		DrawTimeScaleView (pDC, pInfo);
		pDC->SelectClipRgn (NULL);
		theRgn.DeleteObject ();
	}

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
	MIDIData_GetTitle (GetDocument()->m_pMIDIData, szText, sizeof (szText));
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
BOOL CTrackListPrintView::OnPreparePrinting (CPrintInfo* pInfo) {
	// デフォルトの印刷準備
	return DoPreparePrinting (pInfo);
}

// 印刷開始時
void CTrackListPrintView::OnBeginPrinting (CDC* pDC, CPrintInfo* pInfo) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	
	// 紙サイズの取得
	m_sizDevPaper.cx = pDC->GetDeviceCaps (HORZRES); // [pixel]
	m_sizDevPaper.cy = pDC->GetDeviceCaps (VERTRES); // [pixel]
	m_sizLogPaper.cx = pDC->GetDeviceCaps (HORZSIZE) * 10; // [*0.1mm]
	m_sizLogPaper.cy = pDC->GetDeviceCaps (VERTSIZE) * 10; // [*0.1mm]
	m_sizLogPrinterDPI.cx = pDC->GetDeviceCaps (LOGPIXELSX); // [dpi]
	m_sizLogPrinterDPI.cy = pDC->GetDeviceCaps (LOGPIXELSY); // [dpi]

	// リスト全体の高さとページ数を計算
	long lRowZoom = 80;
	long lScaleHeight = TRACKLISTPRINTVIEW_SCALEHEIGHT;
	long lTopMargin = TRACKLISTPRINTVIEW_TOPMARGIN;
	long lBottomMargin = TRACKLISTPRINTVIEW_BOTTOMMARGIN;
	long lNumTrack = MIDIData_CountTrack (pMIDIData);
	m_lNumTrackPerPage = MAX ((m_sizLogPaper.cy - lScaleHeight - lTopMargin - lBottomMargin) / lRowZoom, 1);
	m_lMaxRowPage = lNumTrack / MAX (m_lNumTrackPerPage, 1) + 1;
	m_lMaxColPage = 2;

	// ロール部の幅とページ数を計算
	long lEndTime = MIDIData_GetEndTime (pMIDIData);
	long lEndMeasure, lEndBeat, lEndTick;
	MIDIData_BreakTime (pMIDIData, lEndTime, &lEndMeasure, &lEndBeat, &lEndTick); 
	long lLastTime = 0;
	MIDIData_MakeTime (pMIDIData, lEndMeasure + 1, 0, 0, &lLastTime);
	long lRollWidth = this->TimetoX (lLastTime);
	long lLeftMargin = TRACKLISTPRINTVIEW_LEFTMARGIN;
	long lRightMargin = TRACKLISTPRINTVIEW_RIGHTMARGIN;
	long lScaleWidth = TRACKLISTPRINTVIEW_SCALEWIDTH;
	long lTimeWidth = m_sizLogPaper.cx - lScaleWidth - lLeftMargin - lRightMargin;
	m_lMaxRollPage = lRollWidth / MAX (lTimeWidth, 1) + 1;

	// 印刷ページ数の設定
	m_lMaxPage = m_lMaxRowPage * (m_lMaxColPage + m_lMaxRollPage);
	pInfo->SetMaxPage (m_lMaxPage);

	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// 印刷終了時
void CTrackListPrintView::OnEndPrinting (CDC* pDC, CPrintInfo* pInfo) {
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

