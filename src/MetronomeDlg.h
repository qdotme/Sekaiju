//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// メトロノームダイアログクラス
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

#ifndef _METRONOMEDLG_H_
#define _METRONOMEDLG_H_

class CMetronomeDlg : public CDialog {
	//--------------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------------
public:
	int m_nOn;                          // オン(0=オフ、1=ON)
	int m_nOutputPort;                  // 出力ポート(0～15)
	int m_nOutputChannel;               // 出力チャンネル(0～15)
	int m_nNoteKey1;                    // ノートキー強打(0～127)
	int m_nNoteVel1;                    // ノートベロシティ強打(1～127)
	int m_nNoteKey2;                    // ノートキー弱打(0～127)
	int m_nNoteVel2;                    // ノーとベロシティ弱打(0～127)

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CMetronomeDlg();                    // コンストラクタ
	enum {IDD = IDD_METRONOME};

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
	BOOL FillOutputPortCombo ();
	BOOL FillNoteKeyCombo ();

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual void DoDataExchange (CDataExchange* pDX);    // DDX/DDV のサポート
	virtual BOOL OnInitDialog ();

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnChangeOutput ();
	DECLARE_MESSAGE_MAP ()
};

#endif
