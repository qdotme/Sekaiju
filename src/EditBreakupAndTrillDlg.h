//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 細分化とトリルダイアログクラス
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

#ifndef _EDITBREAKUPANDTRILLDLG_H_
#define _EDITBREAKUPANDTRILLDLG_H_

class CEditBreakupAndTrillDlg : public CDialog {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	long m_lTimeMode;                   // タイムモード
	long m_lTimeResolution;             // 分解能[tick/4分音符]又は[サブフレーム/1フレーム]
	int m_nDurationIndex;               // 細分化後の1音の長さインデックス(0=4分音符～)
	int m_nEnableTrill;                 // トリルを有効にするか？
	int m_nKeyShift;                    // トリル音のキーシフト

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CEditBreakupAndTrillDlg();          // コンストラクタ
	enum {IDD = IDD_EDITBREAKUPANDTRILL};

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	BOOL FillDurationCombo ();          // 音長さコンボボックスの充満
	BOOL SetKeyShiftRange ();           // トリル音のキーシフトの有効範囲設定
	BOOL UpdateKeyShift ();             // トリル音のキーシフトエディットボックス更新

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual void DoDataExchange (CDataExchange* pDX);
	virtual BOOL OnInitDialog ();       // ダイアログの初期化

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	DECLARE_MESSAGE_MAP ()
	afx_msg void OnChangeEnableTrill ();
};

#endif
