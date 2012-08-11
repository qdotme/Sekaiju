//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// オプションプロパティシートクラス
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

#ifndef _OPTIONSHEET_H_
#define _OPTIONSHEET_H_

#include "GeneralOptionPage.h"
#include "TrackListOption1Page.h"
#include "TrackListOption2Page.h"
#include "PianoRollOptionPage.h"
#include "EventListOptionPage.h"
#include "MusicalScoreOptionPage.h"

class COptionSheet : public CPropertySheet {
	//--------------------------------------------------------------------------
	// 各ページ
	//--------------------------------------------------------------------------
public:
	CGeneralOptionPage m_theGeneralOptionPage;
	CTrackListOption1Page m_theTrackListOption1Page;
	CTrackListOption2Page m_theTrackListOption2Page;
	CPianoRollOptionPage m_thePianoRollOptionPage;
	CEventListOptionPage m_theEventListOptionPage;
	CMusicalScoreOptionPage m_theMusicalScoreOptionPage;

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	COptionSheet (CWnd* pParentWnd);    // コンストラクタ
	virtual ~COptionSheet ();           // デストラクタ

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnApplyNow ();
	DECLARE_MESSAGE_MAP ()
};

#endif
