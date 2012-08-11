//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 譜面オプションページクラス
// (C)2002-2011 おーぷんMIDIぷろじぇくと／くず
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
#include "MusicalScoreOptionPage.h"

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
CMusicalScoreOptionPage::CMusicalScoreOptionPage () : CPropertyPage (CMusicalScoreOptionPage::IDD) {
	m_lDefTrackZoom = 0;
	m_lDefTimeZoom = 0;
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CMusicalScoreOptionPage::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_MUSICALSCOREOPTION_DEFTRACKZOOM, m_lDefTrackZoom);
	DDV_MinMaxInt (pDX, m_lDefTrackZoom, 1, 8);
	DDX_Text (pDX, IDC_MUSICALSCOREOPTION_DEFTIMEZOOM, m_lDefTimeZoom);
	DDV_MinMaxInt (pDX, m_lDefTimeZoom, 1, 16);

}

// ダイアログの初期化
BOOL CMusicalScoreOptionPage::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	((CSpinButtonCtrl*)GetDlgItem (IDC_MUSICALSCOREOPTION_DEFTRACKZOOMSP))->SetRange (1, 8);
	((CSpinButtonCtrl*)GetDlgItem (IDC_MUSICALSCOREOPTION_DEFTIMEZOOMSP))->SetRange (1, 16);
	return bRet;
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CMusicalScoreOptionPage, CPropertyPage)
END_MESSAGE_MAP ()


