//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MIDIデバイスプロパティシートクラス
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
#include "MIDIInDevicePage.h"
#include "MIDIOutDevicePage.h"
#include "MIDIInstDefNormPage.h"
#include "MIDIInstDefDrumPage.h"
#include "MIDIDeviceSheet.h"

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CMIDIDeviceSheet::CMIDIDeviceSheet (CWnd* pParentWnd) :
CPropertySheet (IDS_MIDIDEVICE_AND_INSTRUMENT, pParentWnd) {
	AddPage (&m_theMIDIInDevicePage);
	AddPage (&m_theMIDIOutDevicePage);
	AddPage (&m_theMIDIInstDefNormPage);
	AddPage (&m_theMIDIInstDefDrumPage);
	//m_nCurPage = iSelectPage;
}

// デストラクタ
CMIDIDeviceSheet::~CMIDIDeviceSheet () {
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CMIDIDeviceSheet, CPropertySheet)
	ON_BN_CLICKED (ID_APPLY_NOW, OnApplyNow)
END_MESSAGE_MAP ()

// 『適用』ボタンが押された
void CMIDIDeviceSheet::OnApplyNow () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	GetActivePage ()->UpdateData (TRUE);
	pSekaijuApp->ApplyMIDIDeviceSheet (this);
	m_theMIDIInDevicePage.SetModified (FALSE);
	m_theMIDIOutDevicePage.SetModified (FALSE);
	m_theMIDIInstDefNormPage.SetModified (FALSE);
	m_theMIDIInstDefDrumPage.SetModified (FALSE);
}

