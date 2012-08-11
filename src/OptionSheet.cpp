//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// オプションプロパティシートクラス
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
#include "GeneralOptionPage.h"
#include "TrackListOption1Page.h"
#include "TrackListOption2Page.h"
#include "PianoRollOptionPage.h"
#include "EventListOptionPage.h"
#include "OptionSheet.h"

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
COptionSheet::COptionSheet (CWnd* pParentWnd) :
CPropertySheet (IDS_OPTIONS, pParentWnd) {
	AddPage (&m_theGeneralOptionPage);
	AddPage (&m_theTrackListOption1Page);
	AddPage (&m_theTrackListOption2Page);
	AddPage (&m_thePianoRollOptionPage);
	AddPage (&m_theEventListOptionPage);
	AddPage (&m_theMusicalScoreOptionPage);
	//m_nCurPage = iSelectPage;
}

// デストラクタ
COptionSheet::~COptionSheet () {
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP (COptionSheet, CPropertySheet)
	ON_BN_CLICKED (ID_APPLY_NOW, OnApplyNow)
END_MESSAGE_MAP ()


// 『適用』ボタンが押された
void COptionSheet::OnApplyNow () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	GetActivePage ()->UpdateData (TRUE);
	pSekaijuApp->ApplyOptionSheet (this);
	m_theGeneralOptionPage.SetModified (FALSE);
	m_theTrackListOption1Page.SetModified (FALSE);
	m_theTrackListOption2Page.SetModified (FALSE);
	m_thePianoRollOptionPage.SetModified (FALSE);
	m_theEventListOptionPage.SetModified (FALSE);
	m_theMusicalScoreOptionPage.SetModified (FALSE);
}
