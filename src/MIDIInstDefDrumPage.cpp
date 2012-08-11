//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MIDIインストゥルメント定義(ドラム)ページクラス
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
#include <afxext.h>
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "SekaijuApp.h"
#include "MIDIInDevicePage.h"
#include "MIDIOutDevicePage.h"
#include "MIDIInstDefNormPage.h"
#include "MIDIInstDefDrumPage.h"
#include "MIDIDeviceSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




IMPLEMENT_DYNCREATE (CMIDIInstDefDrumPage, CPropertyPage)

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CMIDIInstDefDrumPage::CMIDIInstDefDrumPage () : 
CPropertyPage (IDD_MIDIINSTDEFDRUM) {
}

// デストラクタ
CMIDIInstDefDrumPage::~CMIDIInstDefDrumPage () {
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CMIDIInstDefDrumPage::DoDataExchange (CDataExchange* pDX) {
	long i;
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		DDX_CBString (pDX, IDC_MIDIINSTDEFDRUM_01 + i, m_strMIDIInstDefDrumName[i]);
	}
}

// ダイアログの初期化
BOOL CMIDIInstDefDrumPage::OnInitDialog () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	long lNum = MIDIIn_GetDeviceNum ();
	for (long lPort = 0; lPort < MAXMIDIINDEVICENUM; lPort++) {
		MIDIInstrumentDefinition* pMIDIInstDef;
		CComboBox* pComboBox = (CComboBox*)GetDlgItem (IDC_MIDIINSTDEFDRUM_01 + lPort);
		int nIndex = 0;
		long i = 0;
		for (i = 0; i < MAXMIDIINSTRUMENTNUM; i++) {
			if (pSekaijuApp->m_pMIDIInstrument[i] != NULL) {
				forEachInstrumentDefinition (pSekaijuApp->m_pMIDIInstrument[i], pMIDIInstDef) {
					TCHAR szName[256];
					memset (szName, 0, sizeof (szName));
					MIDIInstrumentDefinition_GetTitle (pMIDIInstDef, szName, 255);
					pComboBox->AddString (szName);
					if (_tcscmp (szName, pSekaijuApp->m_strMIDIInstDefDrumName[lPort]) == 0) {
						pComboBox->SetCurSel (nIndex);
					}
					nIndex++;
				}
			}
		}
	}
	CDialog::OnInitDialog(); // AddStringは基本関数の呼び出しより前(20090625)
	return TRUE;
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CMIDIInstDefDrumPage, CPropertyPage)
	ON_CONTROL_RANGE (CBN_SELCHANGE, IDC_MIDIINSTDEFNORM_01, IDC_MIDIINSTDEFNORM_16, OnSelChange)
END_MESSAGE_MAP ()

void CMIDIInstDefDrumPage::OnSelChange (UINT nID) {
	SetModified (TRUE);
}
