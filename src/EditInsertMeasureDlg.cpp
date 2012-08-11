//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 小節の挿入ダイアログクラス
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
#include <afxcmn.h>
#include "resource.h"
#include "EditInsertMeasureDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CEditInsertMeasureDlg::CEditInsertMeasureDlg () : CDialog (CEditInsertMeasureDlg::IDD) {
	m_nPosition = 0;
	m_nNumMeasure = 0;
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

BOOL CEditInsertMeasureDlg::SetAmountRange () {
	CString strValue;
	GetDlgItem (IDC_EDITINSERTMEASURE_POSITION)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	if (m_bZeroOrigin) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITINSERTMEASURE_POSITIONSP))->SetRange (0, 32767);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITINSERTMEASURE_POSITIONSP))->SetPos (CLIP (0, lValue, 32767));
	}
	else {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITINSERTMEASURE_POSITIONSP))->SetRange (1, 32767);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITINSERTMEASURE_POSITIONSP))->SetPos (CLIP (1, lValue, 32767));
	}
	GetDlgItem (IDC_EDITINSERTMEASURE_NUMMEASURE)->GetWindowText (strValue);
	lValue = _ttol (strValue);
	((CSpinButtonCtrl*)GetDlgItem (IDC_EDITINSERTMEASURE_NUMMEASURESP))->SetRange (1, 256);
	((CSpinButtonCtrl*)GetDlgItem (IDC_EDITINSERTMEASURE_NUMMEASURESP))->SetPos (CLIP (1, lValue, 256));
	return TRUE;
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CEditInsertMeasureDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_EDITINSERTMEASURE_POSITION, m_nPosition);
	if (m_bZeroOrigin) {
		DDV_MinMaxInt (pDX, m_nPosition, 0, 32767);
	}
	else {
		DDV_MinMaxInt (pDX, m_nPosition, 1, 32767);
	}
	DDX_Text (pDX, IDC_EDITINSERTMEASURE_NUMMEASURE, m_nNumMeasure);
	DDV_MinMaxInt (pDX, m_nNumMeasure, 1, 256);
}

// ダイアログ初期化時
BOOL CEditInsertMeasureDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	SetAmountRange ();
	return bRet;
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CEditInsertMeasureDlg, CDialog)
END_MESSAGE_MAP ()



