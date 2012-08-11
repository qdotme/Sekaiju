//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// イベントリストオプションページクラス
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

#ifndef _EVENTLISTOPTION_H_
#define _EVENTLISTOPTION_H_

class CEventListOptionPage : public CPropertyPage {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	long m_lDefRowZoom;                 // デフォルトの行方向拡大率[倍]
	long m_lDefColumnZoom;              // デフォルトの列方向拡大率[倍]
	long m_lDefTrackWidth;              // デフォルトのトラック幅[pixel]
	long m_lDefMillisecWidth;           // デフォルトの時:分:秒:ミリ秒幅[pixel]
	long m_lDefTimeWidth;               // デフォルトのタイム幅[pixel]
	long m_lDefKindWidth;               // デフォルトのイベントの種類幅[pixel]
	long m_lDefChWidth;                 // デフォルトのチャンネル幅[pixel]
	long m_lDefVal1Width;               // デフォルトの値1幅[pixel]
	long m_lDefVal2Width;               // デフォルトの値2幅[pixel]
	long m_lDefVal3Width;               // デフォルトの値3幅[pixel]
	BOOL m_bInsertEventAfter;           // 同時刻のイベントの直後に挿入する
	BOOL m_bDuplicateEventAfter;        // 現在のイベントの直後に挿入する
	BOOL m_bDeleteEventAfter;           // 削除したイベントの次のイベントにフォーカスを合わせる

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	// コンストラクタ
	CEventListOptionPage ();
	enum {IDD = IDD_EVENTLISTOPTION};

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
