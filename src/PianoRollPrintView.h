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

#ifndef _PIANOROLLPRINTVIEW_H_
#define _PIANOROLLPRINTVIEW_H_

class CPianoRollPrintView : public CSekaijuView {
	DECLARE_DYNCREATE (CPianoRollPrintView)

	// CPianoRollFrameからCPianoRollPrintView::OnCmdMsgの呼び出しを許可する。
	friend class CPianoRollFrame;

	// 印刷関係
	CSize m_sizDevPaper;                // 物理紙サイズ[ドット]
	CSize m_sizLogPaper;                // 論理紙サイズ[*1/10mm]
	CSize m_sizLogPrinterDPI;           // プリンタのDPI
	CFont m_theFont;                    // 印刷用フォント

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CPianoRollPrintView ();             // コンストラクタ
	virtual ~CPianoRollPrintView ();    // デストラクタ

	//------------------------------------------------------------------------------
	// オペレーション
	//------------------------------------------------------------------------------

protected:
	long TimetoX (long lTime);
	long VeltoY (long lVel);
	long PitchBendtoY (long lPitchBend);
	long TempoBPMtoY (long lTempoBPM);
	long KeyToY (long lKey);
	void DrawFlagAndText (CDC* pDC, long lTime, LPCTSTR lpszText, long lColor);
	void DrawVelScaleView (CDC* pDC);
	void DrawVelTimeView (CDC* pDC);
	void DrawKeyScaleView (CDC* pDC);
	void DrawKeyTimeView (CDC* pDC);
	void DrawScaleView (CDC* pDC);
	void DrawTimeScaleView (CDC* pDC);

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

