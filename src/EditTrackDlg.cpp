//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// トラックの変更ダイアログクラス
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
#include <afxext.h>
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "resource.h"
#include "SekaijuApp.h"
#include "EditTrackDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

// コンストラクタ
CEditTrackDlg::CEditTrackDlg () : CDialog (CEditTrackDlg::IDD) {
	m_nAmount = 1;
	m_nUnit = 0;
}

// オペレーション
BOOL CEditTrackDlg::SetAmountRange () {
	CButton* pAbsoluteUnitButton = (CButton*)GetDlgItem (IDC_EDITTRACK_ABSOLUTEUNIT);
	CButton* pRelativeUnitButton = (CButton*)GetDlgItem (IDC_EDITTRACK_RELATIVEUNIT);
	CString strValue;
	GetDlgItem (IDC_EDITTRACK_AMOUNT)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	if (pAbsoluteUnitButton->GetCheck ()) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTRACK_AMOUNTSP))->SetRange (1, 256);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTRACK_AMOUNTSP))->SetPos (CLIP (1, lValue, 256));
	}
	else if (pRelativeUnitButton->GetCheck ()) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTRACK_AMOUNTSP))->SetRange (-255, 2555);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTRACK_AMOUNTSP))->SetPos (CLIP (-255, lValue, 255));
	}
	return TRUE;
}


// オーバーライド

// データエクスチェンジ
void CEditTrackDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_EDITTRACK_AMOUNT, m_nAmount);
	DDX_Radio (pDX, IDC_EDITTRACK_ABSOLUTEUNIT, m_nUnit);
	CButton* pAbsoluteUnitButton = (CButton*)GetDlgItem (IDC_EDITTRACK_ABSOLUTEUNIT);
	CButton* pRelativeUnitButton = (CButton*)GetDlgItem (IDC_EDITTRACK_RELATIVEUNIT);
	if (pAbsoluteUnitButton->GetCheck ()) {
		CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
		BOOL bTrackZeroOrigin = pSekaijuApp->m_theGeneralOption.m_bTrackZeroOrigin;
		DDV_MinMaxInt (pDX, m_nAmount, 
			(bTrackZeroOrigin ? 0 : 1),
			(bTrackZeroOrigin ? 255 : 256)); // TODO:
	}
	else if (pRelativeUnitButton->GetCheck ()) {
		DDV_MinMaxInt (pDX, m_nAmount, -255, 255);
	}
}


BOOL CEditTrackDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	SetAmountRange ();
	return bRet;
}



// メッセージマップ
BEGIN_MESSAGE_MAP (CEditTrackDlg, CDialog)
	ON_CONTROL_RANGE (BN_CLICKED,  IDC_EDITTRACK_ABSOLUTEUNIT, IDC_EDITTRACK_RELATIVEUNIT, OnChangeUnit)
END_MESSAGE_MAP ()


/////////////////////////////////////////////////////////////////////////////
// CEditTrackDlg メッセージ ハンドラ

// 単位が変更された
void CEditTrackDlg::OnChangeUnit (UINT nID) {
	SetAmountRange ();
}


