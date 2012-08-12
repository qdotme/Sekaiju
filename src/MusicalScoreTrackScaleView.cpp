//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 譜面トラックスケールビュークラス
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
#include "mousewheel.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDoc.h"
#include "SekaijuView.h"
#include "SekaijuToolBar.h"
#include "ChildFrame.h"
#include "ColorfulCheckListBox.h"
#include "ColorfulComboBox.h"
#include "MusicalScoreFrame.h"
#include "MusicalScoreTrackScaleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_TEXTBOX 3955

IMPLEMENT_DYNCREATE (CMusicalScoreTrackScaleView, CSekaijuView)

// メッセージマップ
BEGIN_MESSAGE_MAP (CMusicalScoreTrackScaleView, CSekaijuView)
	ON_WM_CREATE ()
	ON_WM_DESTROY ()
	ON_WM_KILLFOCUS ()
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

//-----------------------------------------------------------------------------
// 構築と破壊
//-----------------------------------------------------------------------------

// コンストラクタ
CMusicalScoreTrackScaleView::CMusicalScoreTrackScaleView () {
	m_lDownTrack = -1;
	m_lOldTrack = -1;
	m_lCurTrack = -1;
	m_lOldTime = 0;
	m_bSettingCellString = 0;
}

// デストラクタ
CMusicalScoreTrackScaleView::~CMusicalScoreTrackScaleView () {
}

//-----------------------------------------------------------------------------
// オペレーション
//-----------------------------------------------------------------------------

// ト音記号を描画する
void CMusicalScoreTrackScaleView::DrawGClef 
	(CDC* pDC, long x, long y, long rx, long ry) {
	POINT ptCenter;
	ptCenter.x = x;
	ptCenter.y = y;
	POINT pt[21];
	pt[0].x = ptCenter.x;
	pt[0].y = ptCenter.y + ry;
	pt[1].x = ptCenter.x - rx;
	pt[1].y = ptCenter.y;
	pt[2].x = ptCenter.x - rx;
	pt[2].y = ptCenter.y - ry;
	pt[3].x = ptCenter.x + rx;
	pt[3].y = ptCenter.y - ry;
	pt[4].x = ptCenter.x + rx * 2;
	pt[4].y = ptCenter.y;
	pt[5].x = ptCenter.x + rx * 2;
	pt[5].y = ptCenter.y + ry;
	pt[6].x = ptCenter.x + rx;
	pt[6].y = ptCenter.y + ry * 2;
	pt[7].x = ptCenter.x - rx * 1;
	pt[7].y = ptCenter.y + ry * 2;
	pt[8].x = ptCenter.x - rx * 2;
	pt[8].y = ptCenter.y + ry;
	pt[9].x = ptCenter.x - rx * 2;
	pt[9].y = ptCenter.y - ry;
	pt[10].x = ptCenter.x;
	pt[10].y = ptCenter.y - ry * 4;
	pt[11].x = ptCenter.x + rx * 1;
	pt[11].y = ptCenter.y - ry * 5;
	pt[12].x = ptCenter.x + rx * 1;
	pt[12].y = ptCenter.y - ry * 8;
	pt[13].x = ptCenter.x;
	pt[13].y = ptCenter.y - ry * 6;
	pt[14].x = ptCenter.x;
	pt[14].y = ptCenter.y - ry * 4;
	pt[15].x = ptCenter.x + rx;
	pt[15].y = ptCenter.y + ry * 4;
	pt[16].x = ptCenter.x;
	pt[16].y = ptCenter.y + ry * 5;
	pt[17].x = ptCenter.x - rx * 1 / 2;
	pt[17].y = ptCenter.y + ry * 5;
	pDC->Polyline (pt, 18);
	pt[0].x = ptCenter.x - rx;
	pt[0].y = ptCenter.y; 
	pt[1].x = ptCenter.x;
	pt[1].y = ptCenter.y - ry; 
	pt[2].x = ptCenter.x + rx;
	pt[2].y = ptCenter.y - ry; 
	pt[3].x = ptCenter.x + rx * 2;
	pt[3].y = ptCenter.y; 
	pt[4].x = ptCenter.x + rx * 2;
	pt[4].y = ptCenter.y - ry; 
	pt[5].x = ptCenter.x + rx;
	pt[5].y = ptCenter.y - ry * 2; 
	pt[6].x = ptCenter.x;
	pt[6].y = ptCenter.y - ry * 2; 
	pt[7].x = ptCenter.x - rx;
	pt[7].y = ptCenter.y - ry;
	pDC->Polygon (pt, 8);
	pt[0].x = ptCenter.x - rx * 2;
	pt[0].y = ptCenter.y - ry * 1;
	pt[1].x = ptCenter.x - rx * 2;
	pt[1].y = ptCenter.y - ry * 2;
	pt[2].x = ptCenter.x + rx * 1;
	pt[2].y = ptCenter.y - ry * 6;
	pt[3].x = ptCenter.x + rx * 1;
	pt[3].y = ptCenter.y - ry * 5;
	pDC->Polygon (pt, 4);
	pt[0].x = ptCenter.x;
	pt[0].y = ptCenter.y - ry * 6; 
	pt[1].x = ptCenter.x;
	pt[1].y = ptCenter.y - ry * 7; 
	pt[2].x = ptCenter.x + rx;
	pt[2].y = ptCenter.y - ry * 8;
	pt[3].x = ptCenter.x + rx;
	pt[3].y = ptCenter.y - ry * 7;
	pDC->Polygon (pt, 4);
	pt[0].x = ptCenter.x - rx * 1;
	pt[0].y = ptCenter.y + ry * 9 / 2;
	pt[1].x = ptCenter.x - rx * 1 / 2;
	pt[1].y = ptCenter.y + ry * 4;
	pt[2].x = ptCenter.x;
	pt[2].y = ptCenter.y + ry * 9 / 2;
	pt[3].x = ptCenter.x - rx * 1 / 2;
	pt[3].y = ptCenter.y + ry * 5;
	pDC->Polygon (pt, 4);
}

// ヘ音記号を描画する
void CMusicalScoreTrackScaleView::DrawFClef 
	(CDC* pDC, long x, long y, long rx, long ry) {
	POINT ptCenter;
	ptCenter.x = x;
	ptCenter.y = y;
	POINT pt[9];
	pt[0].x = ptCenter.x - rx;
	pt[0].y = ptCenter.y;
	pt[1].x = ptCenter.x - rx * 2;
	pt[1].y = ptCenter.y;
	pt[2].x = ptCenter.x - rx * 2;
	pt[2].y = ptCenter.y - ry;
	pt[3].x = ptCenter.x - rx;
	pt[3].y = ptCenter.y - ry * 2;
	pt[4].x = ptCenter.x + rx;
	pt[4].y = ptCenter.y - ry * 2;
	pt[5].x = ptCenter.x + rx * 2;
	pt[5].y = ptCenter.y - ry;
	pt[6].x = ptCenter.x + rx * 2;
	pt[6].y = ptCenter.y + ry;
	pt[7].x = ptCenter.x;
	pt[7].y = ptCenter.y + ry * 4;
	pt[8].x = ptCenter.x - rx * 2;
	pt[8].y = ptCenter.y + ry * 6;
	pDC->Polyline (pt, 9);
	pt[0].x = ptCenter.x - rx * 2;
	pt[0].y = ptCenter.y;
	pt[1].x = ptCenter.x - rx * 3 / 2;
	pt[1].y = ptCenter.y - ry * 1 / 2;
	pt[2].x = ptCenter.x - rx;
	pt[2].y = ptCenter.y;
	pt[3].x = ptCenter.x - rx * 3 / 2;
	pt[3].y = ptCenter.y + ry * 1 / 2;
	pDC->Polygon (pt, 4);
	pt[0].x = ptCenter.x;
	pt[0].y = ptCenter.y - ry * 2;
	pt[1].x = ptCenter.x + rx;
	pt[1].y = ptCenter.y - ry * 1;
	pt[2].x = ptCenter.x + rx;
	pt[2].y = ptCenter.y + ry * 1;
	pt[3].x = ptCenter.x;
	pt[3].y = ptCenter.y + ry * 4;
	pt[4].x = ptCenter.x + rx * 2;
	pt[4].y = ptCenter.y + ry * 1;
	pt[5].x = ptCenter.x + rx * 2;
	pt[5].y = ptCenter.y - ry * 1;
	pt[6].x = ptCenter.x + rx * 1;
	pt[6].y = ptCenter.y - ry * 2;
	pDC->Polygon (pt, 7);
	pt[0].x = ptCenter.x + rx * 3 + rx / 2;
	pt[0].y = ptCenter.y - ry * 1;
	pt[1].x = ptCenter.x + rx * 3;
	pt[1].y = ptCenter.y - ry * 1 - ry / 2;
	pt[2].x = ptCenter.x + rx * 3 - rx / 2;
	pt[2].y = ptCenter.y - ry * 1;
	pt[3].x = ptCenter.x + rx * 3;
	pt[3].y = ptCenter.y - ry * 1 + ry / 2;
	pDC->Polygon (pt, 4);
	pt[0].x = ptCenter.x + rx * 3 + rx / 2;
	pt[0].y = ptCenter.y + ry * 1;
	pt[1].x = ptCenter.x + rx * 3;
	pt[1].y = ptCenter.y + ry * 1 - ry / 2;
	pt[2].x = ptCenter.x + rx * 3 - ry / 2;
	pt[2].y = ptCenter.y + ry * 1;
	pt[3].x = ptCenter.x + rx * 3;
	pt[3].y = ptCenter.y + ry * 1 + ry / 2;
	pDC->Polygon (pt, 4);
}

// ♭の描画
void CMusicalScoreTrackScaleView::DrawFlat 
	(CDC* pDC, long x, long y, long rx, long ry) {
	POINT ptCenter;
	ptCenter.x = x;
	ptCenter.y = y;
	POINT pt[9];
	pt[0].x = ptCenter.x - rx / 2;
	pt[0].y = ptCenter.y;
	pt[1].x = ptCenter.x;
	pt[1].y = ptCenter.y - ry;
	pt[2].x = ptCenter.x + rx / 2;
	pt[2].y = ptCenter.y - ry;
	pt[3].x = ptCenter.x + rx / 2;
	pt[3].y = ptCenter.y;
	pt[4].x = ptCenter.x - rx / 2;
	pt[4].y = ptCenter.y + ry;
	pt[5].x = ptCenter.x - rx / 2;
	pt[5].y = ptCenter.y - ry * 4;
	pDC->Polyline (pt, 6);
}

// #の描画
void CMusicalScoreTrackScaleView::DrawSharp
	(CDC* pDC, long x, long y, long rx, long ry) {
	POINT ptCenter;
	ptCenter.x = x;
	ptCenter.y = y;
	POINT pt[9];
	pt[0].x = ptCenter.x - rx;
	pt[0].y = ptCenter.y + ry + ry / 2;
	pt[1].x = ptCenter.x + rx;
	pt[1].y = ptCenter.y + ry - ry / 2;
	pDC->Polyline (pt, 2);
	pt[0].x = ptCenter.x - rx;
	pt[0].y = ptCenter.y - ry + ry / 2;
	pt[1].x = ptCenter.x + rx;
	pt[1].y = ptCenter.y - ry - ry / 2;
	pDC->Polyline (pt, 2);
	pt[0].x = ptCenter.x - rx / 2;
	pt[0].y = ptCenter.y - ry * 2;
	pt[1].x = ptCenter.x - rx / 2;
	pt[1].y = ptCenter.y + ry * 2;
	pDC->Polyline (pt, 2);
	pt[0].x = ptCenter.x + rx / 2;
	pt[0].y = ptCenter.y - ry * 2;
	pt[1].x = ptCenter.x + rx / 2;
	pt[1].y = ptCenter.y + ry * 2;
	pDC->Polyline (pt, 2);
};

// 拍子記号・調性記号の描画
void CMusicalScoreTrackScaleView::DrawTimeAndKeySignature 
	(CDC* pDC, long lTrackIndex, long lTime) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MusicalScoreTrackInfo* pTrackInfo = NULL;
	VERIFY (pTrackInfo = pMusicalScoreFrame->GetTrackInfo (lTrackIndex));
	long lTrackFlags = pTrackInfo->m_lFlags;
	long x = 40;
	long y = 0;
	long rx = 4;
	long ry = 4;
	long lGCrefSharpLineNo[7] = {45, 42, 46, 43, 40, 44, 41};
	long lFCrefSharpLineNo[7] = {31, 28, 32, 29, 33, 30, 34};
	long lGCrefFlatLineNo[7] = {41, 44, 40, 43, 39, 42, 38};
	long lFCrefFlatLineNo[7] = {27, 30, 26, 29, 25, 28, 24};
	long lsf, lmi;
	MIDIData_FindKeySignature (pMIDIData, lTime, &lsf, &lmi);
	long lnn, ldd, lcc, lbb;
	MIDIData_FindTimeSignature (pMIDIData, lTime, &lnn, &ldd, &lcc, &lbb);
	CString strText1;
	CString strText2;
	strText1.Format (_T("%d"), lnn);
	strText2.Format (_T("%d"), (1 << ldd));
	CRect rcText1 (x + rx * 10 + rx * 2 * abs(lsf), 0, x + rx * 10 + rx * 2 * abs(lsf) + 24, 0);
	CRect rcText2 (x + rx * 10 + rx * 2 * abs(lsf), 0, x + rx * 10 + rx * 2 * abs(lsf) + 24, 0);
	long j;
	switch (lTrackFlags & 0x0000000F) {
	case 1: // ト音記号
		y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, 39);
		DrawGClef (pDC, x + rx * 4, y, rx, ry);
		if (lsf > 0) {
			for (j = 1; j <= lsf; j++) {
				y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, lGCrefSharpLineNo[j - 1]);
				DrawSharp (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		else if (lsf < 0) {
			for (j = 1; j <= -lsf; j++) {
				y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, lGCrefFlatLineNo[j - 1]);
				DrawFlat (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, 41);
		rcText1.top = y - ry * 4;
		rcText1.bottom = y;
		pDC->DrawText (strText1, &rcText1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		rcText2.top = y;
		rcText2.bottom = y + ry * 4;
		pDC->DrawText (strText2, &rcText2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		break;
	case 2: // ヘ音記号
		y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, 31);
		DrawFClef (pDC, x + rx * 4, y, rx, ry);
		if (lsf > 0) {
			for (j = 1; j <= lsf; j++) {
				y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, lFCrefSharpLineNo[j - 1]);
				DrawSharp (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		else if (lsf < 0) {
			for (j = 1; j <= -lsf; j++) {
				y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, lFCrefFlatLineNo[j - 1]);
				DrawFlat (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, 29);
		rcText1.top = y - ry * 4;
		rcText1.bottom = y;
		pDC->DrawText (strText1, &rcText1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		rcText2.top = y;
		rcText2.bottom = y + ry * 4;
		pDC->DrawText (strText2, &rcText2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		break;
	case 3: // 大譜表
		
		y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, 39);
		DrawGClef (pDC, x + rx * 4, y, rx, ry);
		if (lsf > 0) {
			for (j = 1; j <= lsf; j++) {
				y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, lGCrefSharpLineNo[j - 1]);
				DrawSharp (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		else if (lsf < 0) {
			for (j = 1; j <= -lsf; j++) {
				y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, lGCrefFlatLineNo[j - 1]);
				DrawFlat (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, 41);
		rcText1.top = y - ry * 4;
		rcText1.bottom = y;
		pDC->DrawText (strText1, &rcText1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		rcText2.top = y;
		rcText2.bottom = y + ry * 4;
		pDC->DrawText (strText2, &rcText2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		
		y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, 31);
		DrawFClef (pDC, x + rx * 4, y, rx, ry);
		if (lsf > 0) {
			for (j = 1; j <= lsf; j++) {
				y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, lFCrefSharpLineNo[j - 1]);
				DrawSharp (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		else if (lsf < 0) {
			for (j = 1; j <= -lsf; j++) {
				y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, lFCrefFlatLineNo[j - 1]);
				DrawFlat (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, 29);
		rcText1.top = y - ry * 4;
		rcText1.bottom = y;
		pDC->DrawText (strText1, &rcText1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		rcText2.top = y;
		rcText2.bottom = y + ry * 4;
		pDC->DrawText (strText2, &rcText2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		break;
	}

}


// セルの文字列を取得(トラック名限定)
CString CMusicalScoreTrackScaleView::GetCellString (long lTrack) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	ASSERT (0 <= lTrack && lTrack < MAXMIDITRACKNUM);
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lTrack);
	MIDIEvent* pMIDIEvent = NULL;
	CString strText;
	strText = pSekaijuDoc->GetTrackName (pMIDITrack);
	TCHAR szBuf2[2048];
	memset (szBuf2, 0, sizeof (szBuf2));
	codestr2str ((LPTSTR)(LPCTSTR)strText, strText.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
	strText = szBuf2;
	return strText;
}

// セルの文字列を設定(トラック名限定)
BOOL CMusicalScoreTrackScaleView::SetCellString (long lTrack, CString strText) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	ASSERT (0 <= lTrack && lTrack < MAXMIDITRACKNUM);
	m_bSettingCellString = 1;
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pTempTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	MIDITrack* pCloneTrack = NULL;
	MIDIEvent* pCloneEvent = NULL;
	long lFormat = MIDIData_GetFormat (pMIDIData);
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	long lValue = 0;
	long lNumber = 0;
	ASSERT (0 <= lTrack && lTrack < MAXMIDITRACKNUM);
	// トラック名
	// 新しい履歴の用意
	VERIFY (strHistoryName.LoadString (IDS_MODIFY_TRACKNAME));
	VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
	// 現在のトラックへのポインタを取得
	pMIDITrack = pSekaijuDoc->GetTrack (lTrack);
	if (pMIDITrack == NULL) {
		return FALSE;
	}
	// 最初のトラック名イベントを探索
	forEachEvent (pMIDITrack, pTempEvent) {
		if (MIDIEvent_GetKind (pTempEvent) == MIDIEVENT_TRACKNAME) {
			break;
		}
	}
	// 最初のトラック名イベントが見つからなかった場合
	if (pTempEvent == NULL) {
		TCHAR szBuf2[2048];
		memset (szBuf2, 0, sizeof (szBuf2));
		str2codestr ((LPTSTR)(LPCTSTR)(strText), strText.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
		VERIFY (pTempEvent = MIDIEvent_CreateTrackName (0, szBuf2));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pTempEvent));
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
	}
	// 最初のトラック名が見つかった場合
	else {
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
		VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent));
		TCHAR szBuf2[2048];
		memset (szBuf2, 0, sizeof (szBuf2));
		str2codestr ((LPTSTR)(LPCTSTR)(strText), strText.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
		MIDIEvent_SetText (pCloneEvent, szBuf2);
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
	}
	m_bSettingCellString = 0;
	return TRUE;
}


// 現在テキストボックスで編集中かどうか返す。
BOOL CMusicalScoreTrackScaleView::IsTextEditing () {
	return (m_theTextBox.GetStyle () & WS_VISIBLE) ? TRUE : FALSE;
}

// テキストボックスでの編集を開始する。
BOOL CMusicalScoreTrackScaleView::BeginTextEditing () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lFormat = MIDIData_GetFormat (pMIDIData);
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	pSekaijuApp->m_bInplaceEditing = 1;
	ASSERT (0 <= m_lCurTrack && m_lCurTrack < MAXMIDITRACKNUM);
	CString strCellString = GetCellString (m_lCurTrack);
	m_theTextBox.SetWindowText (strCellString);
	m_theTextBox.SetSel (0, -1, TRUE);
	m_theTextBox.EmptyUndoBuffer ();
	m_theTextBox.ShowWindow (SW_SHOW);
	m_theTextBox.SetFocus ();
	m_theTextBox.UpdateWindow ();
	return TRUE;
}

// テキストボックスでの編集を終了し、新しい値を格納する。
BOOL CMusicalScoreTrackScaleView::EndTextEditingOK () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();

	// 新しい履歴の作成・不足トラックの追加・値の設定はSetCellStringがやる。

	// 現在のセルの編集テキストを反映
	CString strText;
	m_theTextBox.GetWindowText (strText);
	if (SetCellString (m_lCurTrack, strText)) {
		pSekaijuDoc->SetModifiedFlag (TRUE);
		pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIEVENTCHANGED);
	}

	// 編集終了
	m_theTextBox.ShowWindow (SW_HIDE);
	this->SetFocus ();
	pSekaijuApp->m_bInplaceEditing = 0;
	return TRUE;
}

// テキストボックスでの編集を終了し、新しい値を格納しない。
BOOL CMusicalScoreTrackScaleView::EndTextEditingCancel () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	m_theTextBox.ShowWindow (SW_HIDE);
	this->SetFocus ();
	pSekaijuApp->m_bInplaceEditing = 0;
	return TRUE;
}

// テキストボックスの移動(VISIBLE==FALSE時含む)
BOOL CMusicalScoreTrackScaleView::MoveTextBox (long lTrackIndex) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*) GetParent ();
	MusicalScoreTrackInfo* pTrackInfo = pMusicalScoreFrame->GetTrackInfo (lTrackIndex);
	long lTrackZoom = pMusicalScoreFrame->GetTrackZoom ();
	CRect rcClient;
	GetClientRect (&rcClient);
	CRect rcNewCell (40, pTrackInfo->m_lTop * lTrackZoom, 
		rcClient.right, pTrackInfo->m_lTop * lTrackZoom + 13);
	rcNewCell -= CSize (0, pMusicalScoreFrame->GetTrackScrollPos ());
	m_theTextBox.MoveWindow 
		(rcNewCell.left, rcNewCell.top,	rcNewCell.Width (), 13);
	return TRUE;
}



//-----------------------------------------------------------------------------
// オーバーライド
//-----------------------------------------------------------------------------

// 原点の移動をオーバーライド
void CMusicalScoreTrackScaleView::OnPrepareDC (CDC* pDC, CPrintInfo* pInfo) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	pDC->SetWindowOrg (0, pMusicalScoreFrame->GetTrackScrollPos ());
}

// 描画
void CMusicalScoreTrackScaleView::OnDraw (CDC* pDC) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument();
	ASSERT_VALID (pSekaijuDoc);
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	CRect rcClient;
	GetClientRect (&rcClient);
	CFont* pOldFont = NULL;
	pDC->SetBkMode (TRANSPARENT);
	long lTrackZoom = pMusicalScoreFrame->GetTrackZoom ();
	long lVisibleTopTrack = pMusicalScoreFrame->GetVisibleTopTrack ();
	long lVisibleBottomTrack = pMusicalScoreFrame->GetVisibleBottomTrack ();
	long rx = 4;
	long ry = 4;
	BOOL bTrackZeroOrigin = pSekaijuApp->m_theGeneralOption.m_bTrackZeroOrigin;
	long i = 0;
	MIDITrack* pTempTrack = NULL;

	// ボタン部描画
	i = 0;
	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);
	long lColorBlack = RGB (0, 0, 0);
	long lColorWhite = RGB (255, 255, 255);
	pOldFont = pDC->SelectObject (&(pMusicalScoreFrame->m_theFont));
	forEachTrack (pMIDIData, pTempTrack) {
		if (pMusicalScoreFrame->IsTrackVisible (i)) {
			if (lVisibleTopTrack <= i && i <= lVisibleBottomTrack) { 
				MusicalScoreTrackInfo* pTrackInfo = pMusicalScoreFrame->GetTrackInfo (i);
				TCHAR szBuf[256];
				memset (szBuf, 0, sizeof (szBuf));
				_sntprintf (szBuf, 255, _T("%d"), i + (bTrackZeroOrigin ? 0 : 1));
				BOOL bSelectTrack = TRUE;
				long lCount = 0;
				MIDIEvent* pTempEvent = NULL;
				forEachEvent (pTempTrack, pTempEvent) {
					if (MIDIEvent_IsNoteOn (pTempEvent)) {
						if (!pSekaijuDoc->IsEventSelected (pTempEvent)) {
							bSelectTrack = FALSE;
						}
						lCount++;
					}
				}
				if (GetCapture () == this) {
					long lMinTrack = __min (m_lDownTrack, m_lCurTrack);
					long lMaxTrack = __max (m_lDownTrack, m_lCurTrack);
					if (lMinTrack <= i && i <= lMaxTrack) {
						bSelectTrack = TRUE;
					}
				}
				CRect theRect (0, pTrackInfo->m_lTop * lTrackZoom, 
					32, (pTrackInfo->m_lTop + pTrackInfo->m_lHeight) * lTrackZoom);
				if (bSelectTrack && lCount > 0) {
					pDC->FillSolidRect (&theRect, lColorBlack);
					pDC->Draw3dRect (&theRect, lColorBlack, lColorBtnShadow);
					pDC->SetTextColor (lColorWhite);
					pDC->DrawText (szBuf, -1, &theRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				}
				else {
					pDC->FillSolidRect (&theRect, lColorBtnFace);
					pDC->Draw3dRect (&theRect, lColorBtnHighlight, lColorBtnShadow);
					pDC->SetTextColor (lColorBtnText);
					pDC->DrawText (szBuf, -1, &theRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				}
			}
		}
		i++;
	}
	pDC->SelectObject (pOldFont);
	
	//トラック名描画
	i = 0;
	pOldFont = pDC->SelectObject (&pMusicalScoreFrame->m_theFont);
	forEachTrack (pMIDIData, pTempTrack) {
		if (pMusicalScoreFrame->IsTrackVisible (i)) {
			if (lVisibleTopTrack <= i && i <= lVisibleBottomTrack) { 
				MusicalScoreTrackInfo* pTrackInfo = pMusicalScoreFrame->GetTrackInfo (i);
				long lTrackTop = pTrackInfo->m_lTop;
				long lTrackHeight = pTrackInfo->m_lHeight;
				long lColor = MIDITrack_GetForeColor (pTempTrack);
				pDC->SetTextColor (lColor);
				CString strTrackName;
				strTrackName = GetCellString (i);
				CRect rcTrackName (40, lTrackTop * lTrackZoom, rcClient.right, 
					(lTrackTop + lTrackHeight) * lTrackZoom);
				pDC->DrawText (strTrackName, rcTrackName, DT_TOP | DT_LEFT);
			}
		}
		i++;
	}
	pDC->SelectObject (pOldFont);
	
	// 五線描画	
	i = 0;
	CPen penKey (PS_SOLID, 1, RGB (192, 192, 192));
	CPen* pOldPen = pDC->SelectObject (&penKey);
	forEachTrack (pMIDIData, pTempTrack) {
		if (pMusicalScoreFrame->IsTrackVisible (i)) {
			if (lVisibleTopTrack <= i && i <= lVisibleBottomTrack) { 
				MusicalScoreTrackInfo* pTrackInfo = pMusicalScoreFrame->GetTrackInfo (i);
				long lTrackTop = pTrackInfo->m_lTop;
				long lTrackHeight = pTrackInfo->m_lHeight;
				long lTrackFlags  = pTrackInfo->m_lFlags;
				long yc = (lTrackTop + lTrackHeight / 2) * lTrackZoom;
				long ii;
				long y;
				switch (lTrackFlags & 0x0000000F) {
				case 0:
					break;
				case 1: // ト音記号
				case 2: // ヘ音記号
					for (ii = -2; ii <= 2; ii++) {
						y = yc + ii * 2 * ry;
						pDC->MoveTo (40, y);
						pDC->LineTo (rcClient.right, y);
					}
					break;
				case 3: // 大譜表

					for (ii = -5; ii <= -1; ii++) {
						y = yc + ii * 2 * ry;
						pDC->MoveTo (40, y);
						pDC->LineTo (rcClient.right, y);
					}
					for (ii = 1; ii <= 5; ii++) {
						y = yc + ii * 2 * ry;
						pDC->MoveTo (40, y);
						pDC->LineTo (rcClient.right, y);
					}
					break;
				}
				// 大譜表縦線描画
				long y1, y2;
				switch (lTrackFlags & 0x0000000F) {
				case 3:
					y1 = yc - 10 * ry;
					y2 = yc + 10 * ry;
					pDC->FillSolidRect
						(40 - 1, y1, 1, y2 - y1 + 1, RGB (192, 192, 192));
					pDC->FillSolidRect
						(40 - 5, y1, 2, y2 - y1 + 1, RGB (192, 192, 192));
				}
			}
		}
		i++;
	}
	pDC->SelectObject (pOldPen);

	// 拍子記号・調性記号の描画
	i = 0;
	pOldFont = pDC->SelectObject (&(pMusicalScoreFrame->m_theTimeMeasureFont));
	long lTimeScrollPos = pMusicalScoreFrame->GetTimeScrollPos ();
	long lTime = pMusicalScoreFrame->XtoTime (lTimeScrollPos);
	forEachTrack (pMIDIData, pTempTrack) {
		if (pMusicalScoreFrame->IsTrackVisible (i)) {
			if (lVisibleTopTrack <= i && i <= lVisibleBottomTrack) { 
				long lColor = MIDITrack_GetForeColor (pTempTrack);
				pDC->SetTextColor (lColor);
				CPen theTrackPen;
				CBrush theTrackBrush;
				theTrackPen.CreatePen (PS_SOLID, 1, lColor);
				theTrackBrush.CreateSolidBrush (lColor);
				CPen* pOldPen = pDC->SelectObject (&theTrackPen);
				CBrush* pOldBrush = pDC->SelectObject (&theTrackBrush);
				DrawTimeAndKeySignature (pDC, i, lTime);
				pDC->SelectObject (pOldPen);
				pDC->SelectObject (pOldBrush);
				theTrackPen.DeleteObject ();
				theTrackBrush.DeleteObject ();
			}
		}
		i++;
	}
	pDC->SelectObject (pOldFont);
	

}




//-----------------------------------------------------------------------------
// メッセージマップ
//-----------------------------------------------------------------------------

// ウィンドウ生成時
BOOL CMusicalScoreTrackScaleView::OnCreate (LPCREATESTRUCT lpcs) {
	// インプレースエディットの作成
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	m_theTextBox.Create (WS_CHILD /*| WS_VISIBLE | WS_BORDER*/ | ES_AUTOHSCROLL, 
		CRect (40, 0, 80, 12), this, IDC_TEXTBOX);
	m_theTextBox.SetFont (&(pMusicalScoreFrame->m_theFont));

	SetTimer (0x13, 55, NULL);
	return CSekaijuView::OnCreate (lpcs);
}

// ウィンドウ破壊時
void CMusicalScoreTrackScaleView::OnDestroy () {
	KillTimer (0x13);	
}

// フォーカスを失ったとき
void CMusicalScoreTrackScaleView::OnKillFocus (CWnd* pNewWnd) {
	_RPTF1 (_CRT_WARN, "CMusicalScoreTrackScaleView::OnKillFocus (pNewWnd=0x%08x)\n", (long)pNewWnd);
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	// インプレーステキストボックスにフォーカスが移った場合を除き
	if (pNewWnd != &m_theTextBox && !m_bSettingCellString) {
		// テキスト編集中の場合はその内容を確定
		if (IsTextEditing ()) {
			EndTextEditingOK ();
		}
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CView::OnKillFocus (pNewWnd);
}

// キーが押された時
void CMusicalScoreTrackScaleView::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	switch (nChar) {
	// ↑
	case VK_UP:
		pMusicalScoreFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEUP, 
			(LPARAM)(pMusicalScoreFrame->m_wndTrackScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// ↓
	case VK_DOWN:
		pMusicalScoreFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEDOWN, 
			(LPARAM)(pMusicalScoreFrame->m_wndTrackScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// PageUp
	case VK_PRIOR:
		pMusicalScoreFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_PAGEUP, 
			(LPARAM)(pMusicalScoreFrame->m_wndTrackScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// PageDown
	case VK_NEXT:
		pMusicalScoreFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_PAGEDOWN, 
			(LPARAM)(pMusicalScoreFrame->m_wndTrackScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// 改行(Enter)キー
	case VK_RETURN:
		// テキスト編集中でない場合
		if (IsTextEditing () == FALSE) {
		}
		// テキスト編集中の場合(CInplaceEditクラスからPostMessageされる。)
		else {
			EndTextEditingOK ();
			pSekaijuDoc->SetModifiedFlag (TRUE);
			pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		}
		break;
	// エスケープ(Esc)キー(CInplaceEditクラスからPostMessageされる。)
	case VK_ESCAPE:
		EndTextEditingCancel ();
		break;
	}
	return;
}


// マウス左ボタン押された時 
void CMusicalScoreTrackScaleView::OnLButtonDown (UINT nFlags, CPoint point) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	// 録音中は何もしない
	if (pSekaijuApp->m_bRecording) {
		::SetCursor (pSekaijuApp->m_hCursorNo);
		return;
	}
	// MIDIデータが編集ロックされている場合は何もしない
	if (pSekaijuDoc->m_bEditLocked) {
		::SetCursor (pSekaijuApp->m_hCursorNo);
		return;
	}
	
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (0, pMusicalScoreFrame->GetTrackScrollPos ());
	point += CSize (0, pMusicalScoreFrame->GetTrackScrollPos ());

	// テキスト編集の終了
	if (IsTextEditing ()) {
		EndTextEditingOK ();
		pSekaijuDoc->SetModifiedFlag (TRUE);
	}

	// ボタン部でマウスを押し下げた場合
	if (0 <= point.x && point.x < 32) { 
		long i = 0;

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

		m_lOldTrack = pMusicalScoreFrame->YtoTrackIndex (m_ptMouseDown.y);
		m_lDownTrack = pMusicalScoreFrame->YtoTrackIndex (point.y);
		m_lCurTrack = pMusicalScoreFrame->YtoTrackIndex (point.y);

		// 該当トラックの全ノートイベントを選択
		if (nFlags & MK_SHIFT) {
			long lNumTrack = MIDIData_CountTrack (pMIDIData);
			if (0 <= m_lCurTrack && m_lCurTrack < lNumTrack) {
				long lMinTrack = __min (m_lOldTrack, m_lCurTrack);
				long lMaxTrack = __max (m_lOldTrack, m_lCurTrack);
				forEachTrack (pMIDIData, pMIDITrack) {
					if (pMusicalScoreFrame->IsTrackVisible (i)) {
						if (lMinTrack <= i && i <= lMaxTrack) {
							forEachEvent (pMIDITrack, pMIDIEvent) {
								if ((MIDIEvent_IsNoteOn (pMIDIEvent) ||
									MIDIEvent_IsNoteOff (pMIDIEvent)) &&
									pMIDIEvent->m_pPrevCombinedEvent == NULL) {
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
			}
		}
		else {
			long lNumTrack = MIDIData_CountTrack (pMIDIData);
			if (0 <= m_lCurTrack && m_lCurTrack < lNumTrack) {
				pMIDITrack = pSekaijuDoc->GetTrack (m_lCurTrack);
			}
			if (pMIDITrack) {
				forEachEvent (pMIDITrack, pMIDIEvent) {
					if ((MIDIEvent_IsNoteOn (pMIDIEvent) ||
						MIDIEvent_IsNoteOff (pMIDIEvent)) &&
						pMIDIEvent->m_pPrevCombinedEvent == NULL) {
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
	
		SetTimer (0x21, 55, NULL);
		SetCapture ();
		Invalidate ();
	}

	m_ptMouseDown = m_ptMouseMove = point;
	m_nMouseDownFlags = m_nMouseMoveFlags = nFlags;

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// マウス右ボタン押された時  
void CMusicalScoreTrackScaleView::OnRButtonDown (UINT nFlags, CPoint point) {

}

// マウス左ボタン離されたとき
void CMusicalScoreTrackScaleView::OnLButtonUp (UINT nFlags, CPoint point) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	// 録音中は何もしない
	if (pSekaijuApp->m_bRecording) {
		::SetCursor (pSekaijuApp->m_hCursorNo);
		return;
	}
	// MIDIデータが編集ロックされている場合は何もしない
	if (pSekaijuDoc->m_bEditLocked) {
		::SetCursor (pSekaijuApp->m_hCursorNo);
		return;
	}

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (0, pMusicalScoreFrame->GetTrackScrollPos ());
	point += CSize (0, pMusicalScoreFrame->GetTrackScrollPos ());

	if (GetCapture () == this) {
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		
		pSekaijuDoc->m_theCriticalSection.Lock ();
		KillTimer (0x21);
		ReleaseCapture ();

		long lMinTrack = __min (m_lDownTrack, m_lCurTrack);
		long lMaxTrack = __max (m_lDownTrack, m_lCurTrack);

		// 該当範囲にあるノートイベントの選択
		long i = 0;
		forEachTrack (pMIDIData, pMIDITrack) {
			if (pMusicalScoreFrame->IsTrackVisible (i)) {
				if (lMinTrack <= i && i <= lMaxTrack) {
					forEachEvent (pMIDITrack, pMIDIEvent) {
						if ((MIDIEvent_IsNoteOn (pMIDIEvent) ||
							MIDIEvent_IsNoteOff (pMIDIEvent)) &&
							pMIDIEvent->m_pPrevCombinedEvent == NULL) {
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
void CMusicalScoreTrackScaleView::OnRButtonUp (UINT nFlags, CPoint point) {
}

// マウスが動かされたとき
void CMusicalScoreTrackScaleView::OnMouseMove (UINT nFlags, CPoint point) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	// 録音中は何もしない
	if (pSekaijuApp->m_bRecording) {
		::SetCursor (pSekaijuApp->m_hCursorNo);
		return;
	}
	// MIDIデータが編集ロックされている場合は何もしない
	if (pSekaijuDoc->m_bEditLocked) {
		::SetCursor (pSekaijuApp->m_hCursorNo);
		return;
	}

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (0, pMusicalScoreFrame->GetTrackScrollPos ());
	point += CSize (0, pMusicalScoreFrame->GetTrackScrollPos ());

	if (GetCapture () == this) {
		pSekaijuDoc->m_theCriticalSection.Lock ();
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;

		m_lOldTrack = pMusicalScoreFrame->YtoTrackIndex (m_ptMouseMove.y);
		m_lCurTrack = pMusicalScoreFrame->YtoTrackIndex (point.y);
		long lNumTrack = MIDIData_GetNumTrack (pMIDIData);
		if (0 <= m_lCurTrack && m_lCurTrack < lNumTrack) {
			// 前回のトラックと今回のトラックが異なる場合のみ
			if (m_lOldTrack != m_lCurTrack) {
				// セル再描画
				Invalidate (FALSE);
			}
		}
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}
	m_ptMouseMove = point;
	m_nMouseMoveFlags = nFlags;


}

// マウス左ボタンがダブルクリックされたとき
void CMusicalScoreTrackScaleView::OnLButtonDblClk (UINT nFlags, CPoint point) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*) GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();

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
	
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lFormat = MIDIData_GetFormat (pMIDIData);

	// OnLButtonDownを実行し、現在編集中のセルを取りやめ、新しいフォーカスにセルを移動
	SendMessage (WM_LBUTTONDOWN, nFlags, ((point.y  & 0x0000FFFF) << 16) | (point.x & 0x0000FFFF));
	if (IsTextEditing () == TRUE) {
		return;
	}

	pSekaijuDoc->m_theCriticalSection.Lock ();

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (0, pMusicalScoreFrame->GetTrackScrollPos ());
	point += CSize (0, pMusicalScoreFrame->GetTrackScrollPos ());

	if (40 <= point.x && point.x < rcClient.right) {
		long lCurTrack = pMusicalScoreFrame->YtoTrackIndex (point.y);
		long lNumTrack = MIDIData_CountTrack (pMIDIData);
		if (0 <= lCurTrack && lCurTrack < lNumTrack) {
			MusicalScoreTrackInfo* pTrackInfo = pMusicalScoreFrame->GetTrackInfo (lCurTrack);
			MoveTextBox (lCurTrack);
			// テキスト編集モードに突入
			m_lCurTrack = lCurTrack;
			BeginTextEditing ();
		}
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// マウス右ボタンがダブルクリックされたとき
void CMusicalScoreTrackScaleView::OnRButtonDblClk (UINT nFlags, CPoint point) {
}


// タイマー時
void CMusicalScoreTrackScaleView::OnTimer (UINT nIDEvent) {	

	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	if (nIDEvent == 0x13) {
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		long lTimeScrollPos = pMusicalScoreFrame->GetTimeScrollPos ();
		long lTime = pMusicalScoreFrame->XtoTime (lTimeScrollPos);
		if (lTime != m_lOldTime) {
			Invalidate ();
			m_lOldTime = lTime;
		}
	}
	else if (nIDEvent == 0x21) {
		if (GetCapture () == this) {
			CRect rcClient;
			GetClientRect (&rcClient);
			rcClient += CSize (0, pMusicalScoreFrame->GetTrackScrollPos ());
			if (!rcClient.PtInRect (m_ptMouseMove)) {
				pSekaijuDoc->m_theCriticalSection.Lock ();
				long lOldTrackScrollPos = pMusicalScoreFrame->GetTrackScrollPos ();
				if (m_ptMouseMove.y < rcClient.top) {
					pMusicalScoreFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEUP,
						(LPARAM)(pMusicalScoreFrame->m_wndTrackScroll.GetSafeHwnd ()));
				}
				else if (m_ptMouseMove.y >= rcClient.bottom) {
					pMusicalScoreFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEDOWN,
						(LPARAM)(pMusicalScoreFrame->m_wndTrackScroll.GetSafeHwnd ()));
				}
				WORD wX = (WORD)(m_ptMouseMove.x);
				WORD wY = (WORD)(m_ptMouseMove.y - lOldTrackScrollPos);
				PostMessage (WM_MOUSEMOVE, (WPARAM)m_nMouseMoveFlags, (LPARAM)((wY << 16) | wX));
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				::Sleep (0);
			}
		}
	}
}

// マウスホイールが回された時
void CMusicalScoreTrackScaleView::OnMouseWheel40 (UINT nFlags, CPoint point) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lDelta = (short)HIWORD (nFlags);
	long lFlags = LOWORD (nFlags);
	if (lFlags & MK_CONTROL) {
		if (lDelta > 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel == 0 ||
			lDelta < 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel != 0) {
			pMusicalScoreFrame->PostMessage (WM_COMMAND, ID_CONTROL_PREVMEASURE);
		}
		else {
			pMusicalScoreFrame->PostMessage (WM_COMMAND, ID_CONTROL_NEXTMEASURE);
		}
	}
	else {
		long lTrackScrollPos = pMusicalScoreFrame->GetTrackScrollPos ();
		long ry = 4;
		lTrackScrollPos -= ry * 2 * lDelta / WHEELDELTA;
		pMusicalScoreFrame->SetTrackScrollPos (lTrackScrollPos);
	}
}
