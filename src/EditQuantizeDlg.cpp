//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// クォンタイズダイアログクラス
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
#include "EditQuantizeDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//-----------------------------------------------------------------------------
// 構築と破壊
//-----------------------------------------------------------------------------

// コンストラクタ
CEditQuantizeDlg::CEditQuantizeDlg () : CDialog (CEditQuantizeDlg::IDD) {
	m_lTimeMode = 0;
	m_lTimeResolution = 0;
	m_nSnapTimeIndex = 3;
	//m_strSnapTime = _T("");
	m_nStrength = 0;
	m_nTargetNoteOn = 0;
	m_nTargetNoteOff = 0;
}

//-----------------------------------------------------------------------------
// オペレーション
//-----------------------------------------------------------------------------

// 値の範囲設定
BOOL CEditQuantizeDlg::SetAmountRange () {
	((CSpinButtonCtrl*)GetDlgItem (IDC_EDITQUANTIZE_STRENGTHSP))->SetRange (0, 100);
	return TRUE;
}

// スナップタイムコンボボックスの充満
BOOL CEditQuantizeDlg::FillSnapTimeCombo () {
	CComboBox* pSnapTimeCombo = (CComboBox*)GetDlgItem (IDC_EDITQUANTIZE_SNAPTIME);
	CString strTextLine;
	CString strFormat;
	if (m_lTimeMode == 0) {
		VERIFY (strFormat.LoadString (IDS_D_4DIVNOTE));
		strTextLine.Format (strFormat, m_lTimeResolution / 1);
		pSnapTimeCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_8DIVNOTE));
		strTextLine.Format (strFormat, m_lTimeResolution / 2);
		pSnapTimeCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_12DIVNOTE));
		strTextLine.Format (strFormat, m_lTimeResolution / 3);
		pSnapTimeCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_16DIVNOTE));
		strTextLine.Format (strFormat, m_lTimeResolution / 4);
		pSnapTimeCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_24DIVNOTE));
		strTextLine.Format (strFormat,  m_lTimeResolution / 6);
		pSnapTimeCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_32DIVNOTE));
		strTextLine.Format (strFormat, m_lTimeResolution / 8);
		pSnapTimeCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_48DIVNOTE));
		strTextLine.Format (strFormat, m_lTimeResolution / 12);
		pSnapTimeCombo->AddString (strTextLine);
	}
	else {
		VERIFY (strFormat.LoadString (IDS_D_1FRAME));
		strTextLine.Format (strFormat, m_lTimeResolution / 1);
		pSnapTimeCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_2DIVFRAME));
		strTextLine.Format (strFormat, m_lTimeResolution / 2);
		pSnapTimeCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_3DIVFRAME));
		strTextLine.Format (strFormat, m_lTimeResolution / 3);
		pSnapTimeCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_4DIVFRAME));
		strTextLine.Format (strFormat, m_lTimeResolution / 4);
		pSnapTimeCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_6DIVFRAME));
		strTextLine.Format (strFormat, m_lTimeResolution / 6);
		pSnapTimeCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_8DIVFRAME));
		strTextLine.Format (strFormat, m_lTimeResolution / 8);
		pSnapTimeCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_12DIVFRAME));
		strTextLine.Format (strFormat, m_lTimeResolution / 12);
		pSnapTimeCombo->AddString (strTextLine);
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// オーバーライド
//-----------------------------------------------------------------------------

// データエクスチェンジ
void CEditQuantizeDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	//DDX_CBString (pDX, IDC_EDITQUANTIZE_SNAPTIME, m_strSnapTime);
	DDX_CBIndex (pDX, IDC_EDITQUANTIZE_SNAPTIME, m_nSnapTimeIndex);
	DDX_Text (pDX, IDC_EDITQUANTIZE_STRENGTH, m_nStrength);
	DDX_Check (pDX, IDC_EDITQUANTIZE_TARGETNOTEON, m_nTargetNoteOn);
	DDX_Check (pDX, IDC_EDITQUANTIZE_TARGETNOTEOFF, m_nTargetNoteOff);
	DDV_MinMaxInt (pDX, m_nStrength, 0, 100);
}

// ダイアログ初期化時
BOOL CEditQuantizeDlg::OnInitDialog () {
	FillSnapTimeCombo ();
	BOOL bRet = CDialog::OnInitDialog ();
	SetAmountRange ();
	return bRet;
}

//-----------------------------------------------------------------------------
// メッセージマップ
//-----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP (CEditQuantizeDlg, CDialog)
END_MESSAGE_MAP ()



