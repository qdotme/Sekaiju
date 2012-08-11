//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// プロパティノートダイアログクラス
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

#ifndef _PROPERTYNOTEDLG_H_
#define _PROPERTYNOTEDLG_H_

class CPropertyNoteDlg : public CDialog {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	BOOL m_bTrackZeroOrigin;                 // トラック番号を0から数えるか
	BOOL m_bNoteOnNoteOn0;                   // ノートオン+ノートオン(ベロシティ0)か
	CStringArray m_theTrackNameArray;        // トラック名配列
	CDWordArray m_theTrackOutputChannelArray;// 出力チャンネル配列
	CStringArray m_theKeyNameArray;          // キー名配列
	int m_nTrackIndex;                       // トラック
	CString m_strTime;                       // タイム
	int m_nChannel;                          // チャンネル
	int m_nKey;                              // キー
	int m_nOnVelocity;                       // 打鍵ベロシティ
	int m_nOffVelocity;                      // 離鍵ベロシティ
	int m_nDuration;                         // 音長さ

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CPropertyNoteDlg();                      // コンストラクタ
	enum {IDD = IDD_PROPERTYNOTE};

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	BOOL SetChannelRange ();                 // チャンネルの可変範囲設定
	BOOL SetOnVelocityRange ();              // 打鍵ベロシティの可変範囲設定
	BOOL SetOffVelocityRange ();             // 離鍵ベロシティの可変範囲設定
	BOOL SetDurationRange ();                // 音長さの可変範囲設定
	BOOL FillTrackIndexCombo ();             // トラックインデックスコンボボックス充満
	BOOL FillKeyCombo ();                    // キーインデックスコンボボックス充満

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual void DoDataExchange (CDataExchange* pDX);    // DDX/DDV のサポート
	virtual BOOL OnInitDialog ();            // ダイアログの初期化

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg void CPropertyNoteDlg::OnTrackIndexSelEndOK ();
	DECLARE_MESSAGE_MAP ()
};

#endif