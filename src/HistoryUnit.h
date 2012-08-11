//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 履歴ユニットクラス
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

#ifndef _HISTORYUNIT_H_
#define _HISTORYUNIT_H_

class CHistoryUnit {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	CString m_strName;                  // この履歴ユニットの名前
	CTime m_theTime;                    // この履歴ユニットが記録された時刻
	CPtrArray m_theHistoryRecordArray;  // この履歴ユニットが持つ履歴記録(複数)へのポインタ配列

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CHistoryUnit ();                    // コンストラクタ
	virtual ~CHistoryUnit ();           // デストラクタ

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	virtual long AddHistoryRecord (long lType, void* pObject);

};

#endif
