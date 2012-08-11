//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// イベントリストオプションページクラス
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

#include "resource.h"
#include "EventListOptionPage.h"

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
CEventListOptionPage::CEventListOptionPage () : CPropertyPage (CEventListOptionPage::IDD) {
	m_lDefRowZoom = 0;
	m_lDefColumnZoom = 0;
	m_lDefTrackWidth = 0;
	m_lDefMillisecWidth = 0;
	m_lDefTimeWidth = 0;
	m_lDefKindWidth = 0;
	m_lDefChWidth = 0;
	m_lDefVal1Width = 0;
	m_lDefVal2Width = 0;
	m_lDefVal3Width = 0;
	m_bInsertEventAfter = FALSE;
	m_bDuplicateEventAfter = FALSE;
	m_bDeleteEventAfter = FALSE;
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CEventListOptionPage::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_EVENTLISTOPTION_DEFROWZOOM, m_lDefRowZoom);
	DDV_MinMaxInt (pDX, m_lDefRowZoom, 16, 32);
	DDX_Text (pDX, IDC_EVENTLISTOPTION_DEFCOLUMNZOOM, m_lDefColumnZoom);
	DDV_MinMaxInt (pDX, m_lDefColumnZoom, 2, 16);
	DDX_Text (pDX, IDC_EVENTLISTOPTION_DEFTRACKWIDTH, m_lDefTrackWidth);
	DDV_MinMaxInt (pDX, m_lDefTrackWidth, 1, 32);
	DDX_Text (pDX, IDC_EVENTLISTOPTION_DEFMILLISECWIDTH, m_lDefMillisecWidth);
	DDV_MinMaxInt (pDX, m_lDefMillisecWidth, 1, 32);
	DDX_Text (pDX, IDC_EVENTLISTOPTION_DEFTIMEWIDTH, m_lDefTimeWidth);
	DDV_MinMaxInt (pDX, m_lDefTimeWidth, 1, 32);
	DDX_Text (pDX, IDC_EVENTLISTOPTION_DEFKINDWIDTH, m_lDefKindWidth);
	DDV_MinMaxInt (pDX, m_lDefKindWidth, 1, 32);
	DDX_Text (pDX, IDC_EVENTLISTOPTION_DEFCHWIDTH, m_lDefChWidth);
	DDV_MinMaxInt (pDX, m_lDefChWidth, 1, 32);
	DDX_Text (pDX, IDC_EVENTLISTOPTION_DEFVAL1WIDTH, m_lDefVal1Width);
	DDV_MinMaxInt (pDX, m_lDefVal1Width, 1, 32);
	DDX_Text (pDX, IDC_EVENTLISTOPTION_DEFVAL2WIDTH, m_lDefVal2Width);
	DDV_MinMaxInt (pDX, m_lDefVal2Width, 1, 32);
	DDX_Text (pDX, IDC_EVENTLISTOPTION_DEFVAL3WIDTH, m_lDefVal3Width);
	DDV_MinMaxInt (pDX, m_lDefVal3Width, 1, 32);
	DDX_Radio (pDX, IDC_EVENTLISTOPTION_INSERTEVENTBEFORE, m_bInsertEventAfter);
	DDX_Radio (pDX, IDC_EVENTLISTOPTION_DUPLICATEEVENTBEFORE, m_bDuplicateEventAfter);
	DDX_Radio (pDX, IDC_EVENTLISTOPTION_DELETEEVENTBEFORE, m_bDeleteEventAfter);

}



// ダイアログの初期化
BOOL CEventListOptionPage::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	((CSpinButtonCtrl*)GetDlgItem (IDC_EVENTLISTOPTION_DEFROWZOOMSP))->SetRange (16, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_EVENTLISTOPTION_DEFCOLUMNZOOMSP))->SetRange (2, 16);
	((CSpinButtonCtrl*)GetDlgItem (IDC_EVENTLISTOPTION_DEFTRACKWIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_EVENTLISTOPTION_DEFMILLISECWIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_EVENTLISTOPTION_DEFTIMEWIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_EVENTLISTOPTION_DEFKINDWIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_EVENTLISTOPTION_DEFCHWIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_EVENTLISTOPTION_DEFVAL1WIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_EVENTLISTOPTION_DEFVAL2WIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_EVENTLISTOPTION_DEFVAL3WIDTHSP))->SetRange (1, 32);
	
	return bRet;
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP (CEventListOptionPage, CPropertyPage)
END_MESSAGE_MAP ()

