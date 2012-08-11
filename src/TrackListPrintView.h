//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// イベントリスト印刷ビュークラス
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

#ifndef _TRACKLISTPRINTVIEW_H_
#define _TRACKLISTPRINTVIEW_H_

class CTrackListPrintView : public CSekaijuView {

public:
	DECLARE_DYNCREATE (CTrackListPrintView)

	// CTrackListFrameからCTrackListPrintView::OnCmdMsgの呼び出しを許可する。
	friend class CTrackListFrame;

	// 印刷関係
	CSize m_sizDevPaper;                // 物理紙サイズ[ドット]
	CSize m_sizLogPaper;                // 論理紙サイズ[*1/10mm]
	CSize m_sizLogPrinterDPI;           // プリンタのDPI
	CFont m_theFont;                    // 印刷用フォント
	long m_lNumTrackPerPage;            // 1ページあたりのトラック数
	long m_lMaxRowPage;                 // 最大ページ数(縦方向)
	long m_lMaxColPage;                 // 最大ページ数(横方向)
	long m_lMaxRollPage;                // 最大ページ数(ピアノロール方向)
	long m_lMaxPage;                    // 最大ページ数(縦方向*(横方向+ピアノロール方向))

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CTrackListPrintView();              // コンストラクタ
	virtual ~CTrackListPrintView ();    // デストラクタ

	//------------------------------------------------------------------------------
	// オペレーション
	//------------------------------------------------------------------------------
protected:
	CString GetCellString (long lRow, long lColumn);
	long TimetoX (long lTime);
	void DrawIndexScaleView (CDC* pDC, CPrintInfo* pInfo);
	void DrawScaleView (CDC* pDC, CPrintInfo* pInfo);
	void DrawIndexPropertyView (CDC* pDC, CPrintInfo* pInfo);
	void DrawPropertyScaleView (CDC* pDC, CPrintInfo* pInfo);
	void DrawTrackTimeView (CDC* pDC, CPrintInfo* pInfo);
	void DrawFlagAndText (CDC* pDC, long lTime, LPCTSTR lpszText, long lColor);
	void DrawTimeScaleView (CDC* pDC, CPrintInfo* pInfo);

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
