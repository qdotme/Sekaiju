//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// トラックリストモードスケールビュークラス
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

#ifndef _TRACKLISTMODESCALEVIEW_H_
#define _TRACKLISTMODESCALEVIEW_H_

class CTrackListModeScaleView : public CSekaijuView {

	DECLARE_DYNCREATE (CTrackListModeScaleView)

	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
protected:
	CPoint m_ptMouseDown;               // マウスが押されたときの座標
	CPoint m_ptMouseMoveOld;            // マウスが動かされたときの前回の前回の座標
	long   m_lTempColumnIndex;          // 一時的な列番号
	long   m_lTempColumnBaseWidth;      // 一時的な列幅(拡大倍率をかけていない)[pixel]

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CTrackListModeScaleView ();         // コンストラクタ
	virtual ~CTrackListModeScaleView(); // デストラクタ

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual void OnPrepareDC (CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw (CDC* pDC);

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg BOOL OnCreate (LPCREATESTRUCT lpcs);
	afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnMouseMove (UINT nFlags, CPoint point);
	afx_msg void OnMouseWheel40 (UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP ()

};

#endif

