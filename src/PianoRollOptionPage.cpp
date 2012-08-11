//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ピアノロールオプションページクラス
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
#include "PianoRollOptionPage.h"

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
CPianoRollOptionPage::CPianoRollOptionPage () : CPropertyPage (CPianoRollOptionPage::IDD) {
	m_lDefKeyZoom = 0;
	m_lDefVelZoom = 0;
	m_lDefTimeZoom = 0;
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CPianoRollOptionPage::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_PIANOROLLOPTION_DEFKEYZOOM, m_lDefKeyZoom);
	DDV_MinMaxInt (pDX, m_lDefKeyZoom, 4, 16);
	DDX_Text (pDX, IDC_PIANOROLLOPTION_DEFVELZOOM, m_lDefVelZoom);
	DDV_MinMaxInt (pDX, m_lDefVelZoom, 1, 4);
	DDX_Text (pDX, IDC_PIANOROLLOPTION_DEFTIMEZOOM, m_lDefTimeZoom);
	DDV_MinMaxInt (pDX, m_lDefTimeZoom, 1, 16);

}

// ダイアログの初期化
BOOL CPianoRollOptionPage::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	((CSpinButtonCtrl*)GetDlgItem (IDC_PIANOROLLOPTION_DEFKEYZOOMSP))->SetRange (4, 16);
	((CSpinButtonCtrl*)GetDlgItem (IDC_PIANOROLLOPTION_DEFVELZOOMSP))->SetRange (1, 4);
	((CSpinButtonCtrl*)GetDlgItem (IDC_PIANOROLLOPTION_DEFTIMEZOOMSP))->SetRange (1, 16);
	return bRet;
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CPianoRollOptionPage, CPropertyPage)
END_MESSAGE_MAP ()


