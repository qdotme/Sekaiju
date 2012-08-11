//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ピアノロール印刷ビュークラス
// (C)2002-2011 おーぷんMIDIぷろじぇくと／くず
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

#ifndef _MUSICALSCOREPRINTVIEW_H_
#define _MUSICALSCOREPRINTVIEW_H_

class CMusicalScorePrintView : public CSekaijuView {
	DECLARE_DYNCREATE (CMusicalScorePrintView)

	// CMusicalScoreFrameからCMusicalScorePrintView::OnCmdMsgの呼び出しを許可する。
	friend class CMusicalScoreFrame;

	// 印刷関係
	CSize m_sizDevPaper;                // 物理紙サイズ[ドット]
	CSize m_sizLogPaper;                // 論理紙サイズ[*1/10mm]
	CSize m_sizLogPrinterDPI;           // プリンタのDPI
	CFont m_theFont;                    // 印刷用フォント
	long m_lMaxRowPage;                 // 最大ページ数(縦方向)
	long m_lMaxRollPage;                // 最大ページ数(譜面ロール方向)
	long m_lMaxPage;                    // 最大ページ数(縦方向*譜面ロール方向)
	CUIntArray m_theRowPageInfoArray;    // 縦方向のページ情報配列(先頭小節番号)
	CUIntArray m_theRollPageInfoArray;   // 譜面ロール方向のページ情報配列(先頭トラック番号)

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CMusicalScorePrintView ();             // コンストラクタ
	virtual ~CMusicalScorePrintView ();    // デストラクタ

	//------------------------------------------------------------------------------
	// オペレーション
	//------------------------------------------------------------------------------

protected:
	
	long GetRowPageInfoCount () {return m_theRowPageInfoArray.GetSize ();};
	long GetRollPageInfoCount () {return m_theRollPageInfoArray.GetSize ();};

	void CalcMeasureInfoArray ();
	void CalcTrackInfoArray ();

	long TrackIndexLineNotoY (long lTrackIndex, long lLineNo);
	long TrackIndexKeytoY (long lTrackIndex, long lKey, long lKeySignature);
	long TimetoX (long lTime);
	long MeasuretoX (long lMeasure);
	long MeasuretoX2 (long lMeasure);
	
	void DrawTadpole (CDC* pDC, long x, long y, long rx, long ry, long lFlags);
	void DrawGClef (CDC* pDC, long x, long y, long rx, long ry);
	void DrawFClef (CDC* pDC, long x, long y, long rx, long ry);
	void DrawFlat (CDC* pDC, long x, long y, long rx, long ry);
	void DrawSharp (CDC* pDC, long x, long y, long rx, long ry);
	void DrawNatural (CDC* pDC, long x, long y, long rx, long ry);
	void DrawTimeAndKeySignature (CDC* pDC, long lTrackIndex, long lTime);
	void DrawHorzAuxiliaryLine (CDC* pDC, long x, long y, long r, long lFlags);
	void DrawDot (CDC* pDC, long x, long y, long rx, long ry, long lFlags);
	void DrawTieHalf (CDC* pDC, long x1, long x2, long y, long rx, long ry, long lFlags);
	void DrawPole (CDC* pDC, long x1, long x2, long y1, long y2, long rx, long ry, long lFlags);
	void DrawSingleFlag (CDC* pDC, long x1, long x2, long y1, long y2, long rx, long ry, long lFlags);
	void DrawChainedFlag (CDC* pDC, long x1, long x2, long y1, long y2, long rx, long ry, long lFlags);
	void DrawTripletSign (CDC* pDC, long x1, long x2, long y1, long y2, long rx, long ry, long lFlags);
	void DrawNote (CDC* pDC, long lTrackIndex, MusicalScoreNoteInfo* pNoteInfo, long lFlags);
	void DrawFlagAndText (CDC* pDC, long lTime, LPCTSTR lpszText, long lColor);

	void DrawTrackScaleView (CDC* pDC, CPrintInfo* pInfo);
	void DrawTrackTimeView (CDC* pDC, CPrintInfo* pInfo);
	void DrawScaleView (CDC* pDC, CPrintInfo* pInfo);
	void DrawTimeScaleView (CDC* pDC, CPrintInfo* pInfo);

	CString GetCellString (long lTrack);

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual void OnPrint (CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting (CPrintInfo* pInfo);
	virtual void OnBeginPrinting (CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting (CDC* pDC, CPrintInfo* pInfo);

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	DECLARE_MESSAGE_MAP ()

};

#endif

