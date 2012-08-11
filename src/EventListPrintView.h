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

#ifndef _EVENTLISTPRINTVIEW_H_
#define _EVENTLISTPRINTVIEW_H_

class CEventListPrintView : public CSekaijuView {

public:
	DECLARE_DYNCREATE (CEventListPrintView)

	// CEventListFrameからCEventListPrintView::OnCmdMsgの呼び出しを許可する。
	friend class CEventListFrame;

	// 印刷関係
	CSize m_sizDevPaper;                // 物理紙サイズ[ドット]
	CSize m_sizLogPaper;                // 論理紙サイズ[*1/10mm]
	CSize m_sizLogPrinterDPI;           // プリンタのDPI
	CFont m_theFont;                    // 印刷用フォント
	long m_lNumEventPerPage;            // 1ページあたりのイベント数
	long m_lMaxPage;                    // 最大ページ数

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CEventListPrintView();             // コンストラクタ
	virtual ~CEventListPrintView ();   // デストラクタ

	//------------------------------------------------------------------------------
	// オペレーション
	//------------------------------------------------------------------------------

protected:
	CRect GetRectFromCell (long lRow, long lColumn);
	CString GetCellString (long lRow, long lColumn);
	void DrawIndexScaleView (CDC* pDC, CPrintInfo* pInfo);
	void DrawScaleView (CDC* pDC, CPrintInfo* pInfo);
	void DrawIndexPropertyView (CDC* pDC, CPrintInfo* pInfo);
	void DrawPropertyScaleView (CDC* pDC, CPrintInfo* pInfo);

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
