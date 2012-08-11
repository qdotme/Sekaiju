//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MIDI同期モードプロパティシートクラス
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

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CMIDISyncModeSheet::CMIDISyncModeSheet (CWnd* pParentWnd) :
CPropertySheet (IDS_MIDISYNCMODE, pParentWnd) {
	AddPage (&m_theMIDIInSyncModePage);
	AddPage (&m_theMIDIOutSyncModePage);
	//m_nCurPage = iSelectPage;
}

// デストラクタ
CMIDISyncModeSheet::~CMIDISyncModeSheet () {
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP (CMIDISyncModeSheet, CPropertySheet)
	ON_BN_CLICKED (ID_APPLY_NOW, OnApplyNow)
END_MESSAGE_MAP ()


// 『適用』ボタンが押された
void CMIDISyncModeSheet::OnApplyNow () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	GetActivePage ()->UpdateData (TRUE);
	pSekaijuApp->ApplyMIDISyncModeSheet (this);
	m_theMIDIInSyncModePage.SetModified (FALSE);
	m_theMIDIOutSyncModePage.SetModified (FALSE);
}

