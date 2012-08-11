//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// トラックリストオプション2ページクラス
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

#ifndef _TRACKLISTOPTION2PAGE_H_
#define _TRACKLISTOPTION2PAGE_H_

class CTrackListOption2Page : public CPropertyPage {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	long m_lDefCC000Width;              // デフォルトのCC#0の幅[pixel]
	long m_lDefCC032Width;              // デフォルトのCC#32の幅[pixel]
	long m_lDefPCWidth;                 // デフォルトのプログラムナンバーの幅[pixel]
	long m_lDefCC007Width;              // デフォルトのボリュームの幅[pixel]
	long m_lDefCC010Width;              // デフォルトのパンの幅[pixel]
	long m_lDefCC091Width;              // デフォルトのリバーブの幅[pixel]
	long m_lDefCC093Width;              // デフォルトのコーラスの幅[pixel]
	long m_lDefCC094Width;              // デフォルトのディレイの幅[pixel]
	long m_lDefKeyShiftWidth;           // デフォルトのキーシフトの幅[pixel]
	long m_lDefVelShiftWidth;           // デフォルトのベロシティシフトの幅[pixel]
	long m_lDefTimeShiftWidth;          // デフォルトのタイムシフトの幅[pixel]
	long m_lDefNumEventWidth;           // デフォルトのイベント数の幅[pixel]
	// ※実際の列の幅[pixel]は、各列の幅に列方向拡大倍率をかけた値となる。
	
public:
	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
	CTrackListOption2Page ();           // コンストラクタ
	enum {IDD = IDD_TRACKLISTOPTION2};

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------

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
	//afx_msg void OnChangeEnableAutoSave ();
	DECLARE_MESSAGE_MAP ()
};

#endif
