//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ビュークラス
// (C)2002-2010 おーぷんMIDIぷろじぇくと／くず
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

// このクラスの役割はCSekaijuDoc* GetDocument()を提供することのみであり、
// 実用的なビューはCSekaijuViewから派生させて作ること。
// なお、このクラスはVisible=FALSEのダミービューとしても使える。

#ifndef _SEKAIJUVIEW_H_
#define _SEKAIJUVIEW_H_

class CSekaijuView : public CView {
	DECLARE_DYNCREATE(CSekaijuView)
	
	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CSekaijuView ();                    // コンストラクタ
	virtual ~CSekaijuView ();           // デストラクタ

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	CSekaijuDoc* GetDocument ();        // ドキュメントの取得
	
	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual void OnDraw (CDC* pDC);
	virtual BOOL PreCreateWindow (CREATESTRUCT& cs);
	
	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG
inline CSekaijuDoc* CSekaijuView::GetDocument () {
	return (CSekaijuDoc*)m_pDocument;
}
#endif



#endif
