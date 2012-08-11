//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// チャンネルの変更ダイアログクラス
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
#include "EditChannelDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

// コンストラクタ
CEditChannelDlg::CEditChannelDlg () : CDialog (CEditChannelDlg::IDD) {
	m_nAmount = 0;                      // チャンネル値又は変更量
	m_nUnit = 0;                        // 単位
}

// オペレーション
BOOL CEditChannelDlg::SetAmountRange () {
	CButton* pByTrackUnitButton = (CButton*)GetDlgItem (IDC_EDITCHANNEL_BYTRACKUNIT);
	CButton* pAbsoluteUnitButton = (CButton*)GetDlgItem (IDC_EDITCHANNEL_ABSOLUTEUNIT);
	CButton* pRelativeUnitButton = (CButton*)GetDlgItem (IDC_EDITCHANNEL_RELATIVEUNIT);
	CString strValue;
	GetDlgItem (IDC_EDITCHANNEL_AMOUNT)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	if (pByTrackUnitButton->GetCheck ()) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITCHANNEL_AMOUNT))->EnableWindow (FALSE);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITCHANNEL_AMOUNTSP))->EnableWindow (FALSE);
	}
	else if (pAbsoluteUnitButton->GetCheck ()) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITCHANNEL_AMOUNT))->EnableWindow (TRUE);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITCHANNEL_AMOUNTSP))->EnableWindow (TRUE);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITCHANNEL_AMOUNTSP))->SetRange (1, 16);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITCHANNEL_AMOUNTSP))->SetPos (CLIP (1, lValue, 16));
	}
	else if (pRelativeUnitButton->GetCheck ()) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITCHANNEL_AMOUNT))->EnableWindow (TRUE);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITCHANNEL_AMOUNTSP))->EnableWindow (TRUE);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITCHANNEL_AMOUNTSP))->SetRange (-15, 15);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITCHANNEL_AMOUNTSP))->SetPos (CLIP (-15, lValue, 15));
	}
	return TRUE;
}


// オーバーライド

// データエクスチェンジ
void CEditChannelDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_EDITCHANNEL_AMOUNT, m_nAmount);
	DDX_Radio (pDX, IDC_EDITCHANNEL_BYTRACKUNIT, m_nUnit);
	CButton* pByTrackUnitButton = (CButton*)GetDlgItem (IDC_EDITCHANNEL_BYTRACKUNIT);
	CButton* pAbsoluteUnitButton = (CButton*)GetDlgItem (IDC_EDITCHANNEL_ABSOLUTEUNIT);
	CButton* pRelativeUnitButton = (CButton*)GetDlgItem (IDC_EDITCHANNEL_RELATIVEUNIT);
	if (pByTrackUnitButton->GetCheck ()) {
		;
	}
	else if (pAbsoluteUnitButton->GetCheck ()) {
		DDV_MinMaxInt (pDX, m_nAmount, 1, 16);
	}
	else if (pRelativeUnitButton->GetCheck ()) {
		DDV_MinMaxInt (pDX, m_nAmount, -15, 15);
	}
}


BOOL CEditChannelDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	SetAmountRange ();
	return bRet;
}



// メッセージマップ
BEGIN_MESSAGE_MAP (CEditChannelDlg, CDialog)
	ON_CONTROL_RANGE (BN_CLICKED,  IDC_EDITCHANNEL_BYTRACKUNIT, IDC_EDITCHANNEL_RELATIVEUNIT, OnChangeUnit)
END_MESSAGE_MAP ()


/////////////////////////////////////////////////////////////////////////////
// CEditChannelDlg メッセージ ハンドラ

// 単位が変更された
void CEditChannelDlg::OnChangeUnit (UINT nID) {
	SetAmountRange ();
}


