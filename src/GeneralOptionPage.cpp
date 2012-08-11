//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// オプション(全般)プロパティページクラス
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
#include "GeneralOptionPage.h"

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
CGeneralOptionPage::CGeneralOptionPage () : CPropertyPage (CGeneralOptionPage::IDD) {
	m_bEnableMultiExec = FALSE;
	m_bEnableMultiOpen = TRUE;
	m_bRestoreWindowPlacement = TRUE;
	m_bExecOpen = FALSE; 
	m_bOpenPlay = FALSE;
	m_bPlayUpdate = TRUE;
	m_bSearchUpdate = TRUE;
	m_bEnableCC111Loop = TRUE;
	m_bPatchSearch = TRUE;
	m_bInvertCtrlMouseWheel = FALSE;
	m_bTrackZeroOrigin = FALSE;
	m_bEventZeroOrigin = FALSE;
	m_lSpeedSlow = 50;
	m_lSpeedNormal = 100;
	m_lSpeedFast = 200;
	m_lPlayRecordInterval = 5;
	m_lOctaveSignature = 5;
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CGeneralOptionPage::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Check (pDX, IDC_GENERALOPTION_ENABLEMULTIEXEC, m_bEnableMultiExec);
	DDX_Check (pDX, IDC_GENERALOPTION_ENABLEMULTIOPEN, m_bEnableMultiOpen);
	DDX_Check (pDX, IDC_GENERALOPTION_RESTOREWINDOWPLACEMENT, m_bRestoreWindowPlacement);
	DDX_Check (pDX, IDC_GENERALOPTION_EXECOPEN, m_bExecOpen);
	DDX_Check (pDX, IDC_GENERALOPTION_OPENPLAY, m_bOpenPlay);
	DDX_Check (pDX, IDC_GENERALOPTION_PLAYUPDATE, m_bPlayUpdate);
	DDX_Check (pDX, IDC_GENERALOPTION_SEARCHUPDATE, m_bSearchUpdate);
	DDX_Check (pDX, IDC_GENERALOPTION_ENABLECC111LOOP, m_bEnableCC111Loop);
	DDX_Check (pDX, IDC_GENERALOPTION_PATCHSEARCH, m_bPatchSearch);
	DDX_Check (pDX, IDC_GENERALOPTION_INVERTCTRLMOUSEWHEEL, m_bInvertCtrlMouseWheel);
	DDX_Check (pDX, IDC_GENERALOPTION_TRACKZEROORIGIN, m_bTrackZeroOrigin);
	DDX_Check (pDX, IDC_GENERALOPTION_EVENTZEROORIGIN, m_bEventZeroOrigin);
	DDX_Check (pDX, IDC_GENERALOPTION_ENABLEAUTOPAGEUPDATE, m_bEnableAutoPageUpdate);
	DDX_Check (pDX, IDC_GENERALOPTION_SENDNOTEOFFHOLDOFFATEND, m_bSendNoteOffHoldOffAtEnd);
	// DDV_MinMaxXXXは各々のDDX_TEXTの直後に配置すること(20090501訂正)
	DDX_Text (pDX, IDC_GENERALOPTION_SPEEDSLOW, m_lSpeedSlow);
	DDV_MinMaxInt (pDX, m_lSpeedSlow, 1, 1000);
	DDX_Text (pDX, IDC_GENERALOPTION_SPEEDNORMAL, m_lSpeedNormal);
	DDV_MinMaxInt (pDX, m_lSpeedNormal, 1, 1000);
	DDX_Text (pDX, IDC_GENERALOPTION_SPEEDFAST, m_lSpeedFast);
	DDV_MinMaxInt (pDX, m_lSpeedFast, 1, 1000);
	DDX_Text (pDX, IDC_GENERALOPTION_PLAYRECORDINTERVAL, m_lPlayRecordInterval);
	DDV_MinMaxInt (pDX, m_lPlayRecordInterval, 1, 1000);
	DDX_Text (pDX, IDC_GENERALOPTION_OCTAVESIGNATURE, m_lOctaveSignature);
	DDV_MinMaxInt (pDX, m_lOctaveSignature, 3, 5);
}

// ダイアログの初期化
BOOL CGeneralOptionPage::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	((CSpinButtonCtrl*)GetDlgItem (IDC_GENERALOPTION_SPEEDSLOWSP))->SetRange (1, 1000);
	((CSpinButtonCtrl*)GetDlgItem (IDC_GENERALOPTION_SPEEDNORMALSP))->SetRange (1, 1000);
	((CSpinButtonCtrl*)GetDlgItem (IDC_GENERALOPTION_SPEEDFASTSP))->SetRange (1, 1000);
	((CSpinButtonCtrl*)GetDlgItem (IDC_GENERALOPTION_PLAYRECORDINTERVALSP))->SetRange (1, 1000); //20200205修正
	((CSpinButtonCtrl*)GetDlgItem (IDC_GENERALOPTION_OCTAVESIGNATURESP))->SetRange (3, 5);
	return bRet;
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP (CGeneralOptionPage, CPropertyPage)
END_MESSAGE_MAP ()




