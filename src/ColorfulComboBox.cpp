//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// カラフルなコンボボックスボックスクラス
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
#include "ColorfulComboBox.h"

// TSIZEOFマクロ //20120211追加
#ifndef TSIZEOF
#define TSIZEOF(STR) (sizeof(STR)/sizeof(TCHAR))
#endif
#ifndef TCSLEN
#ifdef UNICODE
#define TCSLEN(STRING) wcslen(STRING)
#else
#define TCSLEN(STRING) strlen(STRING)
#endif
#endif
#ifndef TCSNCPY
#ifdef UNICODE
#define TCSNCPY(STRING1,STRING2,N) wcsncpy(STRING1,STRING2,N)
#else
#define TCSNCPY(STRING1,STRING2,N) strncpy(STRING1,STRING2,N)
#endif
#endif

// メッセージマップ
IMPLEMENT_DYNCREATE (CColorfulComboBox, CComboBox)

BEGIN_MESSAGE_MAP (CColorfulComboBox, CComboBox)
	//ON_WM_CREATE ()
	ON_WM_DESTROY ()
END_MESSAGE_MAP ()

typedef struct {
	TCHAR m_szText[1024];
	COLORREF m_lForeColor;
	COLORREF m_lBackColor;
} ItemData;

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CColorfulComboBox::CColorfulComboBox () {
	CComboBox::CComboBox ();
}

// デストラクタ
CColorfulComboBox::~CColorfulComboBox () {
	CComboBox::~CComboBox ();
}


//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// ウィンドウスタイル変更
BOOL CColorfulComboBox::PreCreateWindow (CREATESTRUCT& cs) {
	if (!CComboBox::PreCreateWindow (cs)) {
		return FALSE;
	}
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~(CBS_OWNERDRAWVARIABLE | CBS_SORT);
	cs.style |= CBS_OWNERDRAWFIXED;
	return TRUE;
}

// アイテムの高さ指定
void CColorfulComboBox::MeasureItem (LPMEASUREITEMSTRUCT lpMeasureItemStruct) {
	lpMeasureItemStruct->itemHeight = 14;
}

// アイテムの描画
void CColorfulComboBox::DrawItem (LPDRAWITEMSTRUCT lpDrawItemStruct) {
	int nIndex = lpDrawItemStruct->itemID;
	CRect rcItem = lpDrawItemStruct->rcItem;

	CDC dc;
	dc.Attach (lpDrawItemStruct->hDC);	

	long lBackColor = ::GetSysColor (COLOR_WINDOW);
	if (lpDrawItemStruct->itemState & ODS_SELECTED) {
		lBackColor = ::GetSysColor (COLOR_HIGHLIGHT);
	}
	if (nIndex != (UINT)-1) {
		ItemData* pItemData = (ItemData*)GetItemDataPtr (nIndex);
		if (lpDrawItemStruct->itemState & ODS_SELECTED) {
			lBackColor = pItemData->m_lForeColor;
		}
		else {
			lBackColor = pItemData->m_lBackColor;
		}
	}

	CBrush* pBrush = new CBrush;
	pBrush->CreateSolidBrush (lBackColor);
	dc.FillRect (rcItem, pBrush);
	delete pBrush;

	if (lpDrawItemStruct->itemState & ODS_FOCUS) {
		dc.DrawFocusRect (rcItem);
	}

	if (nIndex != (UINT)-1) {
		ItemData* pItemData = (ItemData*)GetItemDataPtr (nIndex);
		CFont* pFont = CFont::FromHandle 
			((HFONT)(::GetStockObject (DEFAULT_GUI_FONT)));
		CFont* pOldFont = dc.SelectObject (pFont);
		long lTextColor = (lpDrawItemStruct->itemState & ODS_SELECTED) ?
			::GetSysColor (COLOR_HIGHLIGHTTEXT) : pItemData->m_lForeColor;
		dc.SetBkMode (TRANSPARENT);
		dc.SetTextColor (lTextColor);
		dc.DrawText (pItemData->m_szText, rcItem, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		dc.SelectObject (pOldFont);
	}
	dc.Detach ();


}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// アイテムの追加
int CColorfulComboBox::AddItem (LPCTSTR lpszText, COLORREF lForeColor, COLORREF lBackColor) {
	int nIndex = AddString (_T(""));
	if (nIndex == CB_ERR || nIndex == CB_ERRSPACE) {
		return -1;
	}
	ItemData* pItemData = (ItemData*)calloc (sizeof (ItemData), 1);
	if (pItemData == NULL) {
		return -1;
	}
	TCSNCPY (pItemData->m_szText, lpszText, 1023);
	pItemData->m_lForeColor = lForeColor;
	pItemData->m_lBackColor = lBackColor;
	int ret = SetItemDataPtr (nIndex, pItemData);
	if (ret < 0) {
		free (pItemData);
		return -1;
	}
	return ret;

}

// アイテムの全削除
void CColorfulComboBox::RemoveAllItem () {
	long i;
	long lItemCount = this->GetCount ();
	for (i = 0; i < lItemCount; i++) {
		ItemData* pItemData = (ItemData*)GetItemDataPtr (i);
		free (pItemData);
		pItemData = NULL;
	}
	ResetContent ();
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// ウィンドウ破壊時
void CColorfulComboBox::OnDestroy () {
	RemoveAllItem ();
	CComboBox::OnDestroy ();
}


