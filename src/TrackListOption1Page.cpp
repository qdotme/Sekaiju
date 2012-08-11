//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// トラックリストオプション1ページクラス
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
#include "TrackListOption1Page.h"

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
CTrackListOption1Page::CTrackListOption1Page () : CPropertyPage (CTrackListOption1Page::IDD) {
	m_lDefRowZoom = 0;
	m_lDefColumnZoom = 0;
	m_lDefTimeZoom = 0;
	m_lDefNameWidth = 0;
	m_lDefColorWidth = 0;
	m_lDefInputOnWidth = 0;
	m_lDefInputPortWidth = 0;
	m_lDefInputChWidth = 0;
	m_lDefOutputOnWidth = 0;
	m_lDefOutputPortWidth = 0;
	m_lDefOutputChWidth = 0;
	m_lDefViewModeWidth = 0;

}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CTrackListOption1Page::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_TRACKLISTOPTION1_DEFROWZOOM, m_lDefRowZoom);
	DDV_MinMaxInt (pDX, m_lDefRowZoom, 16, 64);
	DDX_Text (pDX, IDC_TRACKLISTOPTION1_DEFCOLUMNZOOM, m_lDefColumnZoom);
	DDV_MinMaxInt (pDX, m_lDefColumnZoom, 2, 16);
	DDX_Text (pDX, IDC_TRACKLISTOPTION1_DEFTIMEZOOM, m_lDefTimeZoom);
	DDV_MinMaxInt (pDX, m_lDefTimeZoom, 2, 16);
	DDX_Text (pDX, IDC_TRACKLISTOPTION1_DEFNAMEWIDTH, m_lDefNameWidth);
	DDV_MinMaxInt (pDX, m_lDefNameWidth, 1, 32);
	DDX_Text (pDX, IDC_TRACKLISTOPTION1_DEFCOLORWIDTH, m_lDefColorWidth);
	DDV_MinMaxInt (pDX, m_lDefColorWidth, 1, 32);
	DDX_Text (pDX, IDC_TRACKLISTOPTION1_DEFINPUTONWIDTH, m_lDefInputOnWidth);
	DDV_MinMaxInt (pDX, m_lDefInputOnWidth, 1, 32);
	DDX_Text (pDX, IDC_TRACKLISTOPTION1_DEFINPUTPORTWIDTH, m_lDefInputPortWidth);
	DDV_MinMaxInt (pDX, m_lDefInputPortWidth, 1, 32);
	DDX_Text (pDX, IDC_TRACKLISTOPTION1_DEFINPUTCHWIDTH, m_lDefInputChWidth);
	DDV_MinMaxInt (pDX, m_lDefInputChWidth, 1, 32);
	DDX_Text (pDX, IDC_TRACKLISTOPTION1_DEFOUTPUTONWIDTH, m_lDefOutputOnWidth);
	DDV_MinMaxInt (pDX, m_lDefOutputOnWidth, 1, 32);
	DDX_Text (pDX, IDC_TRACKLISTOPTION1_DEFOUTPUTPORTWIDTH, m_lDefOutputPortWidth);
	DDV_MinMaxInt (pDX, m_lDefOutputPortWidth, 1, 32);
	DDX_Text (pDX, IDC_TRACKLISTOPTION1_DEFOUTPUTCHWIDTH, m_lDefOutputChWidth);
	DDV_MinMaxInt (pDX, m_lDefOutputChWidth, 1, 32);
	DDX_Text (pDX, IDC_TRACKLISTOPTION1_DEFVIEWMODEWIDTH, m_lDefViewModeWidth);
	DDV_MinMaxInt (pDX, m_lDefViewModeWidth, 1, 32);

}

// ダイアログの初期化
BOOL CTrackListOption1Page::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION1_DEFROWZOOMSP))->SetRange (16, 64);
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION1_DEFCOLUMNZOOMSP))->SetRange (2, 16);
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION1_DEFTIMEZOOMSP))->SetRange (2, 16);
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION1_DEFNAMEWIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION1_DEFCOLORWIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION1_DEFINPUTONWIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION1_DEFINPUTPORTWIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION1_DEFINPUTCHWIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION1_DEFOUTPUTONWIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION1_DEFOUTPUTPORTWIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION1_DEFOUTPUTCHWIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION1_DEFVIEWMODEWIDTHSP))->SetRange (1, 32);
	return bRet;
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP (CTrackListOption1Page, CPropertyPage)
END_MESSAGE_MAP ()


