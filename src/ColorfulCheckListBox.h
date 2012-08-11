//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// カラフルなチェックリストボックスクラス
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

#ifndef _COLORFULCHECKLISTBOX_H_
#define _COLORFULCHECKLISTBOX_H_

class CColorfulCheckListBox : public CCheckListBox {
	DECLARE_DYNCREATE (CColorfulCheckListBox)

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CColorfulCheckListBox ();           // コンストラクタ
	virtual ~CColorfulCheckListBox ();  // デストラクタ

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	int  AddItem (LPCTSTR lpszText, COLORREF lForeColor, COLORREF lBackColor);
	void RemoveAllItem ();

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual BOOL PreCreateWindow (CREATESTRUCT& cs);
	virtual void MeasureItem (LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem (LPDRAWITEMSTRUCT lpDrawItemStruct);

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnDestroy ();
	DECLARE_MESSAGE_MAP ()

};

#endif
