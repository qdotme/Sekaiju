//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// トラックリストオプション1ページクラス
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
//

#ifndef _TRACKLISTOPTION1PAGE_H_
#define _TRACKLISTOPTION1PAGE_H_

class CTrackListOption1Page : public CPropertyPage {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	long m_lDefRowZoom;                 // デフォルトの列方向拡大倍率
	long m_lDefColumnZoom;              // デフォルトの行方向拡大倍率
	long m_lDefTimeZoom;                // デフォルトのタイム方向拡大倍率
	long m_lDefNameWidth;               // デフォルトの名前の幅[pixel]
	long m_lDefColorWidth;              // デフォルトの色の幅[pixel]
	long m_lDefInputOnWidth;            // デフォルトの入力ONの幅[pixel]
	long m_lDefInputPortWidth;          // デフォルトの入力ポートの幅[pixel]
	long m_lDefInputChWidth;            // デフォルトの入力CHの幅[pixel]
	long m_lDefOutputOnWidth;           // デフォルトの出力ONの幅[pixel]
	long m_lDefOutputPortWidth;         // デフォルトの出力ポートの幅[pixel]
	long m_lDefOutputChWidth;           // デフォルトの出力CHの幅[pixel]
	long m_lDefViewModeWidth;           // デフォルトのビューモードの幅[pixel]
	// ※実際の列の幅[pixel]は、各列の幅に列方向拡大倍率をかけた値となる。
	
public:
	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
	CTrackListOption1Page ();           // コンストラクタ
	enum {IDD = IDD_TRACKLISTOPTION1};

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
