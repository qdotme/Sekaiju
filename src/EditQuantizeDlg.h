//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// クォンタイズダイアログクラス
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

#ifndef _EDITQUANTIZEDLG_H_
#define _EDITQUANTIZEDLG_H_

class CEditQuantizeDlg : public CDialog {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	int m_lTimeMode;                    // タイムモード(0=TPQNベース, ...)
	int m_lTimeResolution;              // 分解能[tick/4分音符]又は[サブフレーム/1フレーム]
	int m_nSnapTimeIndex;               // スナップタイムインデックス(0=4分音符,1=8分音符...)
	int m_nStrength;                    // 強度[%]
	int m_nTargetNoteOn;                // ノートオンを対象とする(0/1)
	int m_nTargetNoteOff;               // ノートオフを対象とする(0/1)

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CEditQuantizeDlg();                 // コンストラクタ
	enum {IDD = IDD_EDITQUANTIZE};

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
	BOOL SetAmountRange ();             // 強度の有効範囲を設定する
	BOOL FillSnapTimeCombo ();          // スナップタイムコンボボックスを充満させる


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
	DECLARE_MESSAGE_MAP ()
};

#endif
