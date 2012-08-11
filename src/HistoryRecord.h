//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 履歴記録クラス
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

#ifndef _HISTORYRECORD_H_
#define _HISTORYRECORD_H_

#define HISTORYRECORD_INSERTEVENT 0x0001        // MIDIイベント挿入
#define HISTORYRECORD_REMOVEEVENT 0x0002        // MIDIイベント削除
#define HISTORYRECORD_INSERTEVENTALL 0x0003     // MIDIイベント全挿入
#define HISTORYRECORD_REMOVEEVENTALL 0x0004     // MIDIイベント全削除
#define HISTORYRECORD_INSERTTRACK 0x0011        // MIDIトラック挿入
#define HISTORYRECORD_REMOVETRACK 0x0012        // MIDIトラック削除
#define HISTORYRECORD_INSERTDATA  0x0021        // MIDIデータ挿入(未使用)
#define HISTORYRECORD_REMOVEDATA  0x0022        // MIDIデータ削除(未使用)

class CHistoryRecord {

	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	long m_lType;           // 記録タイプ
	void* m_pObject;        // オブジェクトへのポインタ
	void* m_pPrevObject;    // オブジェクトが双方向リンクリスト要素の場合の前のオブジェクトへのポインタ
	void* m_pNextObject;    // オブジェクトが双方向リンクリスト要素の場合の次のオブジェクトへのポインタ
	void* m_pFirstChild;    // オブジェクトが子を持つ場合の最初の子オブジェクトへのポインタ
	void* m_pLastChild;     // オブジェクトが子を持つ場合の最後の子オブジェクトへのポインタ
	void* m_pParent;        // オブジェクトが親を持つ場合の親オブジェクトへのポインタ

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CHistoryRecord ();                  // コンストラクタ
	virtual ~CHistoryRecord ();         // デストラクタ
};

#endif

