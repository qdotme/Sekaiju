//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MIDI出力同期モードページクラス
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

#ifndef _MIDIOUTSYNCMODEPAGE_H_
#define _MIDIOUTSYNCMODEPAGE_H_


class CMIDIOutSyncModePage : public CPropertyPage {
	DECLARE_DYNCREATE (CMIDIOutSyncModePage)

public:
	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
	CMIDIOutSyncModePage ();            // コンストラクタ
	virtual ~CMIDIOutSyncModePage ();   // デストラクタ

	//--------------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------------
public:
	int m_nMIDIOutSyncMode[MAXMIDIOUTDEVICENUM];

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
	afx_msg void OnSelChange (UINT nID);
	DECLARE_MESSAGE_MAP ()

};

#endif
