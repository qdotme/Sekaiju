//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MIDI出力デバイスページクラス
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

#ifndef _MIDIOUTDEVICEPAGE_H_
#define _MIDIOUTDEVICEPAGE_H_

class CMIDIOutDevicePage : public CPropertyPage {
	DECLARE_DYNCREATE (CMIDIOutDevicePage)

	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	CString m_strMIDIOutName[MAXMIDIOUTDEVICENUM];  // MIDI出力同期モード

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CMIDIOutDevicePage ();              // コンストラクタ
	virtual ~CMIDIOutDevicePage ();     // デストラクタ

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
