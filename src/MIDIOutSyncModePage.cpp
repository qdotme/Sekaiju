//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MIDI出力同期モードページクラス
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
#include <afxext.h>
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "SekaijuApp.h"
#include "MIDIInSyncModePage.h"
#include "MIDIOutSyncModePage.h"
#include "MIDISyncModeSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




IMPLEMENT_DYNCREATE (CMIDIOutSyncModePage, CPropertyPage)

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CMIDIOutSyncModePage::CMIDIOutSyncModePage () : 
CPropertyPage (IDD_MIDIOUTSYNCMODE) {
}

// デストラクタ
CMIDIOutSyncModePage::~CMIDIOutSyncModePage () {
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CMIDIOutSyncModePage::DoDataExchange (CDataExchange* pDX) {
	long i;
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		DDX_CBIndex (pDX, IDC_MIDIOUTSYNCMODE_01 + i, m_nMIDIOutSyncMode[i]);
	}
}

// ダイアログの初期化
BOOL CMIDIOutSyncModePage::OnInitDialog () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CString strItem[6];
	strItem[0].LoadString (IDS_NONE);
	strItem[1].LoadString (IDS_SEND_MIDI_TIMING_CLOCK);
	strItem[2].LoadString (IDS_SEND_SMPTE24_MTC);
	strItem[3].LoadString (IDS_SEND_SMPTE25_MTC);
	strItem[4].LoadString (IDS_SEND_SMPTE29P97_MTC);
	strItem[5].LoadString (IDS_SEND_SMPTE30_MTC);
	long lPort = 0;
	for (lPort = 0; lPort < MAXMIDIINDEVICENUM; lPort++) {
		CComboBox* pComboBox = (CComboBox*)GetDlgItem (IDC_MIDIINSYNCMODE_01 + lPort);
		pComboBox->AddString (strItem[0]);
		pComboBox->AddString (strItem[1]);
		pComboBox->AddString (strItem[2]);
		pComboBox->AddString (strItem[3]);
		pComboBox->AddString (strItem[4]);
		pComboBox->AddString (strItem[5]);
	}
	CDialog::OnInitDialog(); // AddStringは基本関数の呼び出しより前(20090625)
	return TRUE;
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP (CMIDIOutSyncModePage, CPropertyPage)
	ON_CONTROL_RANGE (CBN_SELCHANGE, IDC_MIDIOUTSYNCMODE_01, IDC_MIDIOUTSYNCMODE_16, OnSelChange)
END_MESSAGE_MAP ()

void CMIDIOutSyncModePage::OnSelChange (UINT nID) {
	SetModified (TRUE);
}
