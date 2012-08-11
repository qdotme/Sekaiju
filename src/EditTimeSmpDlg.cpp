//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// タイムの変更(SMPTEベース用)ダイアログクラス
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
#include "EditTimeSmpDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

// コンストラクタ
CEditTimeSmpDlg::CEditTimeSmpDlg () : CDialog (CEditTimeSmpDlg::IDD) {
	m_nAmount = 0;
	m_nUnit = 0;
}

// オペレーション
BOOL CEditTimeSmpDlg::SetAmountRange () {
	CButton* pPercentUnitButton = (CButton*)GetDlgItem (IDC_EDITTIMESMP_PERCENTUNIT);
	CString strValue;
	GetDlgItem (IDC_EDITTIMESMP_AMOUNT)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	if (pPercentUnitButton->GetCheck ()) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTIMESMP_AMOUNTSP))->SetRange (0, 1000);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTIMESMP_AMOUNTSP))->SetPos (CLIP (0, lValue, 1000));
	}
	else {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTIMESMP_AMOUNTSP))->SetRange (-8192, 8192);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTIMESMP_AMOUNTSP))->SetPos (CLIP (-8192, lValue, 8192));
	}
	return TRUE;
}


// オーバーライド

// データエクスチェンジ
void CEditTimeSmpDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_EDITTIMESMP_AMOUNT, m_nAmount);
	DDX_Radio (pDX, IDC_EDITTIMESMP_TICKUNIT, m_nUnit);
	CButton* pPercentUnitButton = (CButton*)GetDlgItem (IDC_EDITTIMESMP_PERCENTUNIT);
	if (pPercentUnitButton->GetCheck ()) {
		DDV_MinMaxInt (pDX, m_nAmount, 0, 1000);
	}
	else {
		DDV_MinMaxInt (pDX, m_nAmount, -8192, 8192);
	}
}


BOOL CEditTimeSmpDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	SetAmountRange ();
	return bRet;
}



// メッセージマップ
BEGIN_MESSAGE_MAP (CEditTimeSmpDlg, CDialog)
	ON_CONTROL_RANGE (BN_CLICKED,  IDC_EDITTIMESMP_TICKUNIT, IDC_EDITTIMESMP_PERCENTUNIT, OnChangeUnit)
END_MESSAGE_MAP ()


/////////////////////////////////////////////////////////////////////////////
// CEditTimeSmpDlg メッセージ ハンドラ

// 単位が変更された
void CEditTimeSmpDlg::OnChangeUnit (UINT nID) {
	SetAmountRange ();
}


