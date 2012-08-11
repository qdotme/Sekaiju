//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 自動保存ダイアログクラス
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
#include <afxcmn.h>
#include "resource.h"
#include "AutoSaveDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif


//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CAutoSaveDlg::CAutoSaveDlg () : CDialog (CAutoSaveDlg::IDD) {
	m_nOn = 0;
	m_lInterval = 0;
	m_nDisableWhilePlaying = 0;
	m_nDisableWhileRecording = 0;
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// 『自動保存間隔』の値範囲設定(1～120[分])
BOOL CAutoSaveDlg::SetIntervalRange () {
	((CSpinButtonCtrl*)GetDlgItem (IDC_AUTOSAVE_INTERVALSP))->SetRange (1, 120);
	((CSpinButtonCtrl*)GetDlgItem (IDC_AUTOSAVE_INTERVALSP))->SetPos (CLIP (1, m_lInterval, 120));
	return TRUE;
}

// 『自動保存間隔』を更新
BOOL CAutoSaveDlg::UpdateInterval () {
	int nOn = ((CButton*)GetDlgItem (IDC_AUTOSAVE_ON))->GetCheck ();
	GetDlgItem (IDC_AUTOSAVE_INTERVAL)->EnableWindow (nOn);
	GetDlgItem (IDC_AUTOSAVE_INTERVALSP)->EnableWindow (nOn);
	return TRUE;
}

// 『演奏中は自動保存しない』を更新
BOOL CAutoSaveDlg::UpdateDisableWhilePlaying () {
	int nOn = ((CButton*)GetDlgItem (IDC_AUTOSAVE_ON))->GetCheck ();
	GetDlgItem (IDC_AUTOSAVE_DISABLEWHILEPLAYING)->EnableWindow (nOn);
	return TRUE;
}

// 『リアルタイム入力中は自動保存しない』を更新
BOOL CAutoSaveDlg::UpdateDisableWhileRecording () {
	int nOn = ((CButton*)GetDlgItem (IDC_AUTOSAVE_ON))->GetCheck ();
	GetDlgItem (IDC_AUTOSAVE_DISABLEWHILERECORDING)->EnableWindow (nOn);
	return TRUE;
}


//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CAutoSaveDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Check (pDX, IDC_AUTOSAVE_ON, m_nOn);
	DDX_Text (pDX, IDC_AUTOSAVE_INTERVAL, m_lInterval);
	DDV_MinMaxInt (pDX, m_lInterval, 1, 120);
	DDX_Check (pDX, IDC_AUTOSAVE_DISABLEWHILEPLAYING, m_nDisableWhilePlaying);
	DDX_Check (pDX, IDC_AUTOSAVE_DISABLEWHILERECORDING, m_nDisableWhileRecording);
}

// ダイアログの初期化
BOOL CAutoSaveDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	((CSpinButtonCtrl*)GetDlgItem (IDC_AUTOSAVE_INTERVALSP))->SetRange (1, 120);
	UpdateInterval ();
	UpdateDisableWhilePlaying ();
	UpdateDisableWhileRecording ();
	SetIntervalRange ();
	return bRet;
}



//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP (CAutoSaveDlg, CDialog)
	ON_BN_CLICKED (IDC_AUTOSAVE_ON, OnChangeOn)
	ON_BN_CLICKED (IDC_AUTOSAVE_DELETEALLFILENOW, OnDeleteAllFileNow)	
END_MESSAGE_MAP ()

// 『自動保存をする(ドキュメントが変更されている場合のみ)』
void CAutoSaveDlg::OnChangeOn () {
	UpdateInterval ();
	UpdateDisableWhilePlaying ();
	UpdateDisableWhileRecording ();
}

// 『今すぐすべての自動保存ファイルを削除する』
void CAutoSaveDlg::OnDeleteAllFileNow () {
		
	// 本当にすべて削除してよいか確認
	long lRet = 0;
	CString strMsg;
	VERIFY (strMsg.LoadString (IDS_ARE_YOU_SURE_TO_DELETE_ALL_AUTOSAVEFILE_NOW_REALLY));
	lRet = AfxMessageBox (strMsg, MB_ICONQUESTION | MB_YESNOCANCEL);
	if (lRet != IDYES) {
		return;
	}
	
	// autosaveディレクトリ内の拡張子(*.skj)のファイルを全て削除する。
	lRet = 0;
	CString strFileName1;
	CString strFileName2;
	VERIFY (strFileName1.LoadString (IDS_AUTOSAVEFILENAME));
	strFileName2 = m_strExeFilePath + strFileName1;
	HANDLE hFind = NULL;
	WIN32_FIND_DATA fd;
	hFind = ::FindFirstFile ((LPCTSTR)strFileName2, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (_tcscmp (fd.cFileName, _T(".")) != 0 &&
				_tcscmp (fd.cFileName, _T("..")) != 0) {
				CString strFileName3;
				CString strFileName4;
				VERIFY (strFileName3.LoadString (IDS_AUTOSAVEDIRNAME));
				strFileName4 = m_strExeFilePath + strFileName3 + fd.cFileName;
				BOOL bRet = _tremove (strFileName4);
				if (bRet == FALSE) {
					_RPTF1 (_CRT_WARN, "AutoSaveFile Delete Failed. - \"%s\"\n", strFileName4);				
				}
				else {
					_RPTF1 (_CRT_WARN, "AutoSaveFile Delete Successful. - \"%s\"\n", strFileName4);
				}
				lRet++;
			}
		} while (::FindNextFile (hFind, &fd));
		::FindClose (hFind);
	}
}


