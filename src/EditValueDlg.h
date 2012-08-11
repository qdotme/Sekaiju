//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 値の変更ダイアログクラス
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

#ifndef _EDITVALUEDLG_H_
#define _EDITVALUEDLG_H_

class CEditValueDlg : public CDialog {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	int m_nAmount;                      // 値の変更量
	int m_nUnit;                        // 単位(0=増減,1=パーセント)
	int m_nTargetKeyAfter;              // キーアフタータッチを対象とする(0/1)
	int m_nTargetControlChange;         // コントロールチェンジを対象とする(0/1)
	int m_nTargetChannelAfter;          // チャンネルアフタータッチを対象とする(0/1)
	int m_nTargetPitchBend;             // ピッチベンドを対象とする(0/1)

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CEditValueDlg ();                   // コンストラクタ
	enum {IDD = IDD_EDITVALUE};

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	BOOL SetAmountRange ();             // 値の変更量の有効範囲を設定する

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual void DoDataExchange (CDataExchange* pDX);    // DDX/DDV のサポート
	virtual BOOL OnInitDialog ();       // ダイアログの初期化

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnChangeUnit (UINT nID);
	afx_msg void OnChangeTarget (UINT nID);
	DECLARE_MESSAGE_MAP ()
};

#endif
