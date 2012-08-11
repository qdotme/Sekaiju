//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ビート検出とテンポ自動挿入ダイアログクラス
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

#ifndef _EDITBEATSCANDLG_H_
#define _EDITBEATSCANDLG_H_

class CEditBeatScanDlg : public CDialog {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	long m_lTimeMode;                   // タイムモード
	long m_lTimeResolution;             // 分解能[tick/4分音符]又は[サブフレーム/1フレーム]
	CStringArray m_theTrackNameArray;   // トラック名の配列
	int m_nBeatTrackIndex;              // ビートの記録されたトラック番号(0～65535)
	int m_nBeatIntervalIndex;           // ビートの間隔インデックス(0=4分音符～)
	int m_nInsertTempo;                 // テンポを自動挿入するか？

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CEditBeatScanDlg();                 // コンストラクタ
	enum {IDD = IDD_EDITBEATSCAN};

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	BOOL FillBeatTrackCombo ();
	BOOL FillBeatIntervalCombo ();

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
	DECLARE_MESSAGE_MAP ()

};

#endif
