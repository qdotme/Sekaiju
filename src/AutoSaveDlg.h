//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 自動保存ダイアログクラス
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

#ifndef _AUTOSAVEDLG_H_
#define _AUTOSAVEDLG_H_

class CAutoSaveDlg : public CDialog {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	CString m_strExeFilePath;           // 実行ファイルのパス名
	int m_nOn;                          // 自動保存オン
	int m_lInterval;                    // 自動保存間隔(1-120)[分]
	int m_nDisableWhilePlaying;         // 演奏中は自動保存しない
	int m_nDisableWhileRecording;       // リアルタイム入力中は自動保存しない

public:
	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
	// コンストラクタ
	CAutoSaveDlg ();
	enum {IDD = IDD_AUTOSAVE};

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
	BOOL SetIntervalRange ();
	BOOL UpdateInterval ();
	BOOL UpdateDisableWhilePlaying ();
	BOOL UpdateDisableWhileRecording ();

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual void DoDataExchange (CDataExchange* pDX);
	virtual BOOL OnInitDialog ();

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	DECLARE_MESSAGE_MAP ()
	afx_msg void OnChangeOn ();
	afx_msg void OnDeleteAllFileNow ();
};

#endif
