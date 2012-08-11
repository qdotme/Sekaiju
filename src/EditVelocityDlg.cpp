//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ベロシティの変更ダイアログクラス
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
#include "EditVelocityDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CEditVelocityDlg::CEditVelocityDlg () : CDialog (CEditVelocityDlg::IDD) {
	m_nAmount = 0;
	m_nUnit = 0;
	m_nTargetNoteOn = 0;
	m_nTargetNoteOff = 0;
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

BOOL CEditVelocityDlg::SetAmountRange () {
	CButton* pAbsoluteUnitButton = (CButton*)GetDlgItem (IDC_EDITVELOCITY_ABSOLUTEUNIT);
	CButton* pRelativeUnitButton = (CButton*)GetDlgItem (IDC_EDITVELOCITY_RELATIVEUNIT);
	CString strValue;
	GetDlgItem (IDC_EDITVELOCITY_AMOUNT)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	if (pAbsoluteUnitButton->GetCheck ()) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVELOCITY_AMOUNTSP))->SetRange (1, 127);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVELOCITY_AMOUNTSP))->SetPos (CLIP (1, lValue, 127));
	}
	else if (pRelativeUnitButton->GetCheck ()) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVELOCITY_AMOUNTSP))->SetRange (-127, 127);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVELOCITY_AMOUNTSP))->SetPos (CLIP (-127, lValue, 127));
	}
	else {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVELOCITY_AMOUNTSP))->SetRange (0, 1000);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVELOCITY_AMOUNTSP))->SetPos (CLIP (0, lValue, 1000));
	}
	return TRUE;
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CEditVelocityDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_EDITVELOCITY_AMOUNT, m_nAmount);
	DDX_Radio (pDX, IDC_EDITVELOCITY_ABSOLUTEUNIT, m_nUnit);
	DDX_Check (pDX, IDC_EDITVELOCITY_TARGETNOTEON, m_nTargetNoteOn);
	DDX_Check (pDX, IDC_EDITVELOCITY_TARGETNOTEOFF, m_nTargetNoteOff);
	CButton* pAbsoluteUnitButton = (CButton*)GetDlgItem (IDC_EDITVELOCITY_ABSOLUTEUNIT);
	CButton* pRelativeUnitButton = (CButton*)GetDlgItem (IDC_EDITVELOCITY_RELATIVEUNIT);
	if (pAbsoluteUnitButton->GetCheck ()) {
		DDV_MinMaxInt (pDX, m_nAmount, 1, 127);
	}
	else if (pRelativeUnitButton->GetCheck ()) {
		DDV_MinMaxInt (pDX, m_nAmount, -127, 127);
	}
	else {
		DDV_MinMaxInt (pDX, m_nAmount, 0, 1000);
	}
}

// ダイアログ初期化時
BOOL CEditVelocityDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	SetAmountRange ();
	return bRet;
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CEditVelocityDlg, CDialog)
	ON_CONTROL_RANGE (BN_CLICKED,  IDC_EDITVELOCITY_ABSOLUTEUNIT, IDC_EDITVELOCITY_PERCENTUNIT, OnChangeUnit)
END_MESSAGE_MAP ()

// 単位が変更された
void CEditVelocityDlg::OnChangeUnit (UINT nID) {
	SetAmountRange ();
}


