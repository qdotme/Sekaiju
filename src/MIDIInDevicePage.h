//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MIDI入力デバイスページクラス
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

#ifndef _MIDIINDEVICEPAGE_H_
#define _MIDIINDEVICEPAGE_H_

class CMIDIInDevicePage : public CPropertyPage {
	DECLARE_DYNCREATE (CMIDIInDevicePage)

	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
	CString m_strMIDIInName[MAXMIDIINDEVICENUM];    // MIDIInデバイス名[0～15]

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CMIDIInDevicePage ();               // コンストラクタ
	virtual ~CMIDIInDevicePage ();      // デストラクタ

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
