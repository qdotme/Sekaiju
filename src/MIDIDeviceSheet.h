//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MIDIデバイスプロパティシートクラス
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

#ifndef _MIDIDEVICESHEET_H_
#define _MIDIDEVICESHEET_H_

#include "MIDIInDevicePage.h"
#include "MIDIOutDevicePage.h"
#include "MIDIInstDefNormPage.h"
#include "MIDIInstDefDrumPage.h"

class CMIDIDeviceSheet : public CPropertySheet {
	//--------------------------------------------------------------------------
	// 各プロパティページ
	//--------------------------------------------------------------------------
public:
	CMIDIInDevicePage m_theMIDIInDevicePage;        // MIDI入力デバイスページ
	CMIDIOutDevicePage m_theMIDIOutDevicePage;      // MIDI出力デバイスページ
	CMIDIInstDefNormPage m_theMIDIInstDefNormPage;  // MIDIインストゥルメント(通常)ページ
	CMIDIInstDefDrumPage m_theMIDIInstDefDrumPage;  // MIDIインストゥルメント(ドラム)ページ
	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CMIDIDeviceSheet (CWnd* pParentWnd);    // コンストラクタ
	virtual ~CMIDIDeviceSheet ();           // デストラクタ
	
	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnApplyNow ();
	DECLARE_MESSAGE_MAP ()
};

#endif
