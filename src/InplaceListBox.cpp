//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// インプレースリストボックスクラス
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

#include "winver.h"
#include <afxwin.h>
#include <afxext.h>

#include "InplaceListBox.h"


// アロケーションの監視
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// メッセージマップ
IMPLEMENT_DYNCREATE (CInplaceListBox, CListBox)

BEGIN_MESSAGE_MAP (CInplaceListBox, CListBox)
	ON_WM_KILLFOCUS ()
	ON_WM_KEYDOWN ()
	ON_WM_LBUTTONDBLCLK ()
END_MESSAGE_MAP()

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CInplaceListBox::CInplaceListBox () {

}

// デストラクタ
CInplaceListBox::~CInplaceListBox () {
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// フォーカスが失われたとき
void CInplaceListBox::OnKillFocus (CWnd* pNewWnd) {
	_RPTF1 (_CRT_WARN, "CInplaceListBox::OnKillFocus (pNewWnd=0x%08x)\n", pNewWnd);
	CWnd* pParentWnd = GetParent ();
	if (pParentWnd != pNewWnd) {
		pParentWnd->SendMessage (WM_KILLFOCUS, (WPARAM)pNewWnd, (LPARAM)0L);
	}
	else {
		//pParentWnd->SetFocus ();
	}
}

// キー押し下げ時
void CInplaceListBox::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CWnd* pParentWnd = GetParent ();
	switch (nChar) {
	case VK_UP:
	case VK_LEFT:
	case VK_RIGHT:
	case VK_DOWN:
		//TODO:上下左右キーは編集終了か選択できるようにしておく
		//GetParent ()->SendMessage (WM_KEYDOWN, VK_RETURN, (nRepCnt << 16) | nFlags);
		//GetParent ()->PostMessage (WM_KEYDOWN, nChar, (nRepCnt << 16) | nFlags);
		break;
		return;
	case VK_RETURN:
		//GetParent ()->SendMessage (WM_KEYDOWN, VK_RETURN, (nRepCnt << 16) | nFlags);
		pParentWnd->SetFocus ();
		pParentWnd->PostMessage (WM_KEYDOWN, VK_RETURN, (nRepCnt << 16) | nFlags);
		break;
	case VK_ESCAPE:
		//GetParent ()->SendMessage (WM_KEYDOWN, VK_ESCAPE, (nRepCnt << 16) | nFlags);
		pParentWnd->SetFocus ();
		pParentWnd->PostMessage (WM_KEYDOWN, VK_ESCAPE, (nRepCnt << 16) | nFlags);
		break;
	}
	CListBox::OnKeyDown (nChar, nRepCnt, nFlags);
}

// マウス左ボタンダブルクリック時
void CInplaceListBox::OnLButtonDblClk (UINT nFlags, CPoint point) {
	// 選択項目の確定信号(VK_RETURN)を親ウィンドウに送る
	GetParent ()->SendMessage (WM_KEYDOWN, VK_RETURN, (1 << 16) | 0);
	CListBox::OnLButtonDblClk (nFlags, point);
}
