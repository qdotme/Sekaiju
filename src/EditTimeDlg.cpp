//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// タイムの変更(TPQNベース用)ダイアログクラス
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
#include "EditTimeDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

// コンストラクタ
CEditTimeDlg::CEditTimeDlg () : CDialog (CEditTimeDlg::IDD) {
	m_nAmount = 0;
	m_nUnit = 0;
}

// オペレーション
BOOL CEditTimeDlg::SetAmountRange () {
	CButton* pPercentUnitButton = (CButton*)GetDlgItem (IDC_EDITTIME_PERCENTUNIT);
	CString strValue;
	GetDlgItem (IDC_EDITTIME_AMOUNT)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	if (pPercentUnitButton->GetCheck ()) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTIME_AMOUNTSP))->SetRange (0, 1000);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTIME_AMOUNTSP))->SetPos (CLIP (0, lValue, 1000));
	}
	else {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTIME_AMOUNTSP))->SetRange (-8192, 8192);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTIME_AMOUNTSP))->SetPos (CLIP (-8192, lValue, 8192));
	}
	return TRUE;
}


// オーバーライド

// データエクスチェンジ
void CEditTimeDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_EDITTIME_AMOUNT, m_nAmount);
	DDX_Radio (pDX, IDC_EDITTIME_TICKUNIT, m_nUnit);
	CButton* pPercentUnitButton = (CButton*)GetDlgItem (IDC_EDITTIME_PERCENTUNIT);
	if (pPercentUnitButton->GetCheck ()) {
		DDV_MinMaxInt (pDX, m_nAmount, 0, 1000);
	}
	else {
		DDV_MinMaxInt (pDX, m_nAmount, -8192, 8192);
	}
}


BOOL CEditTimeDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	SetAmountRange ();
	return bRet;
}



// メッセージマップ
BEGIN_MESSAGE_MAP (CEditTimeDlg, CDialog)
	ON_CONTROL_RANGE (BN_CLICKED,  IDC_EDITTIME_TICKUNIT, IDC_EDITTIME_PERCENTUNIT, OnChangeUnit)
END_MESSAGE_MAP ()


/////////////////////////////////////////////////////////////////////////////
// CEditTimeDlg メッセージ ハンドラ

// 単位が変更された
void CEditTimeDlg::OnChangeUnit (UINT nID) {
	SetAmountRange ();
}


