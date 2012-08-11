//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// プロパティノートダイアログクラス
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
#include "PropertyNoteDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//******************************************************************************
// 構築と破壊
//******************************************************************************

// コンストラクタ
CPropertyNoteDlg::CPropertyNoteDlg () : CDialog (CPropertyNoteDlg::IDD) {
	m_bTrackZeroOrigin = FALSE;
	m_bNoteOnNoteOn0 = FALSE;
	m_nTrackIndex = 0;
	m_strTime = _T("");
	m_nChannel = 0;
	m_nKey = 0;
	m_nOnVelocity = 0;
	m_nOffVelocity = 0;
	m_nDuration = 0;
}

//******************************************************************************
// オペレーション
//******************************************************************************

// チャンネル可変範囲設定
BOOL CPropertyNoteDlg::SetChannelRange () {
	CString strValue;
	GetDlgItem (IDC_PROPERTYNOTE_CHANNEL)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	((CSpinButtonCtrl*)GetDlgItem (IDC_PROPERTYNOTE_CHANNELSP))->SetRange (1, 16);
	((CSpinButtonCtrl*)GetDlgItem (IDC_PROPERTYNOTE_CHANNELSP))->SetPos (CLIP (1, lValue, 16));
	return TRUE;
}

// 打鍵ベロシティ可変範囲設定
BOOL CPropertyNoteDlg::SetOnVelocityRange () {
	CString strValue;
	GetDlgItem (IDC_PROPERTYNOTE_ONVELOCITY)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	((CSpinButtonCtrl*)GetDlgItem (IDC_PROPERTYNOTE_ONVELOCITYSP))->SetRange (1, 127);
	((CSpinButtonCtrl*)GetDlgItem (IDC_PROPERTYNOTE_ONVELOCITYSP))->SetPos (CLIP (1, lValue, 127));
	return TRUE;
}

// 離鍵ベロシティ可変範囲設定
BOOL CPropertyNoteDlg::SetOffVelocityRange () {
	CString strValue;
	GetDlgItem (IDC_PROPERTYNOTE_OFFVELOCITY)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	((CSpinButtonCtrl*)GetDlgItem (IDC_PROPERTYNOTE_OFFVELOCITYSP))->SetRange (0, 127);
	((CSpinButtonCtrl*)GetDlgItem (IDC_PROPERTYNOTE_OFFVELOCITYSP))->SetPos (CLIP (0, lValue, 127));
	return TRUE;
}

// 音長さ可変範囲設定
BOOL CPropertyNoteDlg::SetDurationRange () {
	CString strValue;
	GetDlgItem (IDC_PROPERTYNOTE_DURATION)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	((CSpinButtonCtrl*)GetDlgItem (IDC_PROPERTYNOTE_DURATIONSP))->SetRange (1, 32767);
	((CSpinButtonCtrl*)GetDlgItem (IDC_PROPERTYNOTE_DURATIONSP))->SetPos (CLIP (1, lValue, 32767));
	return TRUE;
}

// トラックインデックスコンボボックス充満
BOOL CPropertyNoteDlg::FillTrackIndexCombo () {
	long i = 0;
	for (i = 0; i < m_theTrackNameArray.GetSize (); i++) {
		CString strItem;
		strItem.Format (_T("%d-%s"), i + (m_bTrackZeroOrigin ? 0 : 1), m_theTrackNameArray.GetAt (i));
		((CComboBox*)GetDlgItem (IDC_PROPERTYNOTE_TRACKINDEX))->AddString (strItem);
	}
	return TRUE;
}

// キーコンボボックス充満
BOOL CPropertyNoteDlg::FillKeyCombo () {
	long i = 0;
	for (i = 0; i < m_theKeyNameArray.GetSize (); i++) {
		CString strItem;
		strItem.Format (_T("%d-%s"), i, m_theKeyNameArray.GetAt (i));
		((CComboBox*)GetDlgItem (IDC_PROPERTYNOTE_KEY))->AddString (strItem);
	}
	return TRUE;
}

//******************************************************************************
// オーバーライド
//******************************************************************************

// データエクスチェンジ
void CPropertyNoteDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_CBIndex (pDX, IDC_PROPERTYNOTE_TRACKINDEX, m_nTrackIndex);
	DDX_Text (pDX, IDC_PROPERTYNOTE_TIME, m_strTime);
	DDX_Text (pDX, IDC_PROPERTYNOTE_CHANNEL, m_nChannel);
	DDV_MinMaxInt (pDX, m_nChannel, 1, 16);
	DDX_CBIndex (pDX, IDC_PROPERTYNOTE_KEY, m_nKey);
	DDX_Text (pDX, IDC_PROPERTYNOTE_ONVELOCITY, m_nOnVelocity);
	DDV_MinMaxInt (pDX, m_nOnVelocity, 1, 127);
	DDX_Text (pDX, IDC_PROPERTYNOTE_OFFVELOCITY, m_nOffVelocity);
	DDV_MinMaxInt (pDX, m_nOffVelocity, 0, 127);
	DDX_Text (pDX, IDC_PROPERTYNOTE_DURATION, m_nDuration);
	DDV_MinMaxInt (pDX, m_nDuration, 1, 65535);
}

// ダイアログ初期化
BOOL CPropertyNoteDlg::OnInitDialog () {
	FillTrackIndexCombo ();
	FillKeyCombo ();
	BOOL bRet = CDialog::OnInitDialog ();
	SetChannelRange ();
	SetOnVelocityRange ();
	SetOffVelocityRange ();
	SetDurationRange ();
	if (m_bNoteOnNoteOn0) {
		GetDlgItem (IDC_PROPERTYNOTE_OFFVELOCITY)->EnableWindow (FALSE);
		GetDlgItem (IDC_PROPERTYNOTE_OFFVELOCITYSP)->EnableWindow (FALSE);
	}
	return bRet;
}


//******************************************************************************
// メッセージマップ
//******************************************************************************

BEGIN_MESSAGE_MAP (CPropertyNoteDlg, CDialog)
	ON_CBN_SELENDOK (IDC_PROPERTYNOTE_TRACKINDEX, OnTrackIndexSelEndOK)
END_MESSAGE_MAP ()

void CPropertyNoteDlg::OnTrackIndexSelEndOK () {
	int nTrackIndex = ((CComboBox*)GetDlgItem (IDC_PROPERTYNOTE_TRACKINDEX))->GetCurSel ();
	long lTrackOutputChannel = (long)m_theTrackOutputChannelArray.GetAt (nTrackIndex);
	if (0 <= lTrackOutputChannel && lTrackOutputChannel < 16) {
		CString strChannel;
		strChannel.Format (_T("%d"), lTrackOutputChannel + 1);
		GetDlgItem (IDC_PROPERTYNOTE_CHANNEL)->SetWindowText (strChannel);
	}
}

