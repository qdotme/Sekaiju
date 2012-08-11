//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// オプション(全般)プロパティページクラス
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

#ifndef _GENERALOPTIONPAGE_H_
#define _GENERALOPTIONPAGE_H_

class CGeneralOptionPage : public CPropertyPage {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	BOOL m_bEnableMultiExec;            // 複数の世界樹を起動すること許可する
	BOOL m_bEnableMultiOpen;            // 複数のMIDIデータを開くことを許可する
	BOOL m_bRestoreWindowPlacement;     // 起動時に前回のウィンドウ位置を復元する
	BOOL m_bExecOpen;                   // 起動時に最後に開いたMIDIデータを自動的に開く
	BOOL m_bOpenPlay;                   // MIDIデータを開くと自動的に演奏を開始する
	BOOL m_bPlayUpdate;                 // 演奏位置移動時にパッチ･コントローラ･ピッチベンド･RPN･NRPNを最新値に更新する 
	BOOL m_bSearchUpdate;               // 演奏開始時にパッチ･コントローラ･ピッチベンド･RPN･NRPNを最新値に更新する
	BOOL m_bEnableCC111Loop;            // オートリピート時にCC#111の位置からループ開始する
	BOOL m_bPatchSearch;                // CC#0･CC#32･プログラムチェンジ操作時に有効な音色だけを検索する
	BOOL m_bInvertCtrlMouseWheel;       // Ctrl+マウスホイールの演奏位置移動方向を反転
	BOOL m_bTrackZeroOrigin;            // トラック番号を0から数える
	BOOL m_bEventZeroOrigin;            // イベント番号を0から数える
	BOOL m_bEnableAutoPageUpdate;       // 演奏開始時・位置移動時に自動的ページ更新をオンにする。
	BOOL m_bSendNoteOffHoldOffAtEnd;    // 曲の終端に達したときノートオフやホールドオフを送信する
	long m_lUpDownDelta1;               // ▲▼の左クリック又は[+][-]キーで増減する量(1～127)
	long m_lUpDownDelta2;               // ▲▼の右クリック又は[Shift]+[+][-]キーで増減する量(1～127)
	long m_lKeyVelocity1;               // 鍵盤の左クリック又は[Z]-[\]キーで発音するベロシティ(1～127)
	long m_lKeyVelocity2;               // 鍵盤の右クリック又は[Shift]+[Z]-[\]キーで発音するベロシティ(1～127)
	long m_lSpeedSlow;                  // スピード=低速で演奏時のテンポ倍率(1～<50>～1000)[%]
	long m_lSpeedNormal;                // スピード=標準で演奏時のテンポ倍率(1～<100>～1000)[%]
	long m_lSpeedFast;                  // スピード=高速で演奏時のテンポ倍率(1～<200>～1000)[%]
	long m_lPlayRecordInterval;         // MIDIデータ録音演奏時のwhileループ呼び出し間隔(1～1000)[ミリ秒]
	long m_lOctaveSignature;            // 中央のド(ｷｰ60)のｵｸﾀｰﾌﾞ番号表記(3～5)

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CGeneralOptionPage ();              // コンストラクタ
	enum {IDD = IDD_GENERALOPTION};

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
	DECLARE_MESSAGE_MAP ()
};

#endif
