//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ピアノロールオプションページクラス
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

#ifndef _PIANOROLLOPTIONPAGE_H_
#define _PIANOROLLOPTIONPAGE_H_

class CPianoRollOptionPage : public CPropertyPage {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	long m_lDefKeyZoom;                 // デフォルトのキー方向拡大倍率[倍]
	long m_lDefVelZoom;                 // デフォルトのベロシティ方向拡大倍率[倍]
	long m_lDefTimeZoom;                // デフォルトのタイム方向拡大倍率[倍]
	
	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CPianoRollOptionPage ();            // コンストラクタ
	enum {IDD = IDD_PIANOROLLOPTION};

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
