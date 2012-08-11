//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MIDI同期モードプロパティシートクラス
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

#ifndef _MIDISYNCMODESHEET_H_
#define _MIDISYNCMODESHEET_H_

#include "MIDIInSyncModePage.h"
#include "MIDIOutSyncModePage.h"

class CMIDISyncModeSheet : public CPropertySheet {

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CMIDISyncModeSheet (CWnd* pParentWnd);  // コンストラクタ
	virtual ~CMIDISyncModeSheet ();         // デストラクタ
	
	//--------------------------------------------------------------------------
	// 各ページ
	//--------------------------------------------------------------------------
public:
	CMIDIInSyncModePage m_theMIDIInSyncModePage;    // MIDI入力同期モードページ
	CMIDIOutSyncModePage m_theMIDIOutSyncModePage;  // MIDI出力同期モードページ
	
	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnApplyNow ();
	DECLARE_MESSAGE_MAP ()
};

#endif
