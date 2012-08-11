//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// メトロノームダイアログクラス
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
#include <afxcmn.h>
#include <afxext.h>

#include "resource.h"
#include "MetronomeDlg.h"

#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "SekaijuApp.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CMetronomeDlg::CMetronomeDlg () : CDialog (CMetronomeDlg::IDD) {
	m_nOn = 0;
	m_nOutputPort = 0;
	m_nOutputChannel = 10;
	m_nNoteKey1 = 0;
	m_nNoteVel1 = 1;
	m_nNoteKey2 = 0;
	m_nNoteVel2 = 1;
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// 出力先ポートコンボボックスの充満
BOOL CMetronomeDlg::FillOutputPortCombo () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CComboBox* pComboBox = (CComboBox*)GetDlgItem (IDC_METRONOME_OUTPUTPORT);
	if (pComboBox == NULL) {
		return FALSE;
	}
	int nCurSel = pComboBox->GetCurSel (); // 初回はコンボボックス空のためエラー(-1)を返す
	pComboBox->ResetContent ();
	for (long lPort = 0; lPort < MAXMIDIOUTDEVICENUM; lPort++) {
		CString strTextLine;
		if (pSekaijuApp->m_strMIDIOutName[lPort] == _T("")) {
			CString strNone;
			VERIFY (strNone.LoadString (IDS_NONE));
			strTextLine.Format (_T("%d-%s"), lPort + 1, strNone);
		}
		else {
			strTextLine.Format (_T("%d-%s"), lPort + 1, pSekaijuApp->m_strMIDIOutName[lPort]);
		}
		pComboBox->AddString (strTextLine); 
	}
	if (0 <= nCurSel && nCurSel <= 15) {
		pComboBox->SetCurSel (nCurSel);
	}
	else {
		pComboBox->SetCurSel (m_nOutputPort);
	}
	return TRUE;
}

// ノート・キーコンボボックスの充満
BOOL CMetronomeDlg::FillNoteKeyCombo () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	long lPort = ((CComboBox*)GetDlgItem (IDC_METRONOME_OUTPUTPORT))->GetCurSel ();
	lPort = CLIP (0, lPort, 15);
	CString strChannel;
	GetDlgItem (IDC_METRONOME_OUTPUTCHANNEL)->GetWindowText (strChannel);
	long lChannel = _ttol ((LPCTSTR)strChannel);
	lChannel = CLIP (0, lChannel, 15);
	long lOctaveSignature = pSekaijuApp->m_theGeneralOption.m_lOctaveSignature;

	MIDIInstrumentDefinition* pMIDIInstDefDrum = pSekaijuApp->m_pMIDIInstDefDrum[lPort];
	MIDINoteNameTable* pNoteNameTable = NULL;
	if (pMIDIInstDefDrum && lChannel == 10) {
		pNoteNameTable = MIDIInstrumentDefinition_GetNoteNameTable (pMIDIInstDefDrum, 0, 0);
	}
	
	for (long i = 0; i < 2; i++) {
		CComboBox* pComboBox = (CComboBox*)GetDlgItem (IDC_METRONOME_NOTEKEY1 + i * 4);
		if (pComboBox == NULL) {
			continue;
		}
		int nCurSel = pComboBox->GetCurSel (); // 初回はコンボボックス空のためエラー(-1)を返す
		pComboBox->ResetContent ();
		for (long lKey = 0; lKey <= 127; lKey++) {
			CString strTextLine;
			if (pNoteNameTable) {
				TCHAR szName[256];
				memset (szName, 0, sizeof (szName));
				MIDINoteNameTable_GetName (pNoteNameTable, lKey, szName, TSIZEOF (szName));
				strTextLine.Format (_T("%d-%s"), lKey, szName);
			}
			else {
				CString strName;
				strName.LoadString (IDS_NOTEKEY_0S00 + lKey % 12);
				strTextLine.Format (_T("%d-%s%d"), lKey, strName, lKey / 12 + lOctaveSignature - 5);
			}
			pComboBox->AddString (strTextLine); 
		}
		if (0 <= nCurSel && nCurSel <= 127) {
			pComboBox->SetCurSel (nCurSel);
		}
		else {
			pComboBox->SetCurSel (i == 0 ? m_nNoteKey1 : m_nNoteKey2);
		}
	}
	return TRUE;
}


// オーバーライド

// データエクスチェンジ
void CMetronomeDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	// DDV_MinMaxXXXは各々のDDX_TEXTの直後に配置すること(20090501訂正)
	DDX_Check (pDX, IDC_METRONOME_ON, m_nOn);
	DDX_CBIndex (pDX, IDC_METRONOME_OUTPUTPORT, m_nOutputPort);
	DDX_Text (pDX, IDC_METRONOME_OUTPUTCHANNEL, m_nOutputChannel);
	DDV_MinMaxInt (pDX, m_nOutputChannel, 1, 16);
	DDX_CBIndex (pDX, IDC_METRONOME_NOTEKEY1, m_nNoteKey1);
	DDX_Text (pDX, IDC_METRONOME_NOTEVEL1, m_nNoteVel1);
	DDV_MinMaxInt (pDX, m_nNoteVel1, 1, 127);
	DDX_CBIndex (pDX, IDC_METRONOME_NOTEKEY2, m_nNoteKey2);
	DDX_Text (pDX, IDC_METRONOME_NOTEVEL2, m_nNoteVel2);
	DDV_MinMaxInt (pDX, m_nNoteVel2, 1, 127);
}


// ダイアログの初期化
BOOL CMetronomeDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	FillOutputPortCombo ();
	FillNoteKeyCombo ();
	//CString strValue;
	//long lValue = 0;
	//GetDlgItem (IDC_METRONOME_OUTPUT)->GetWindowText (strValue);
	//lValue = atol (strValue);
	((CSpinButtonCtrl*)GetDlgItem (IDC_METRONOME_OUTPUTCHANNELSP))->SetRange (1, 16);
	//((CSpinButtonCtrl*)GetDlgItem (IDC_METRONOME_OUTPUTCHANNELSP))->SetPos (CLIP (1, lValue, 16));
	//GetDlgItem (IDC_METRONOME_NOTEVEL1)->GetWindowText (strValue);
	//lValue = atol (strValue);
	((CSpinButtonCtrl*)GetDlgItem (IDC_METRONOME_NOTEVEL1SP))->SetRange (1, 127);
	//((CSpinButtonCtrl*)GetDlgItem (IDC_METRONOME_NOTEVEL1SP))->SetPos (CLIP (1, lValue, 127));
	//GetDlgItem (IDC_METRONOME_NOTEVEL1)->GetWindowText (strValue);
	//lValue = atol (strValue);
	((CSpinButtonCtrl*)GetDlgItem (IDC_METRONOME_NOTEVEL2SP))->SetRange (1, 127);
	//((CSpinButtonCtrl*)GetDlgItem (IDC_METRONOME_NOTEVEL2SP))->SetPos (CLIP (1, lValue, 127));
	return bRet;
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CMetronomeDlg, CDialog)
	ON_CONTROL (CBN_SELCHANGE, IDC_METRONOME_OUTPUTPORT, OnChangeOutput)
	ON_CONTROL (EN_CHANGE, IDC_METRONOME_OUTPUTCHANNEL, OnChangeOutput)
END_MESSAGE_MAP ()

// 出力先が変更された
void CMetronomeDlg::OnChangeOutput () {
	FillNoteKeyCombo ();
}


