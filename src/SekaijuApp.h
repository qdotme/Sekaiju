//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 世界樹アプリケーションクラス
// (C)2002-2011 おーぷんMIDIぷろじぇくと／くず
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

#ifndef _AFXWIN_H_
#define _AFXWIN_H_

#include <afxmt.h>

#define MAXMIDIINDEVICENUM 16           // 最大MIDI入力デバイス数
#define MAXMIDIOUTDEVICENUM 16          // 最大MIDI出力デバイス数
#define MAXMIDITRACKNUM 65536           // 最大MIDIトラック数(1つのMIDIデータに付き)
#define MAXMIDIINSTRUMENTNUM 256        // 最大MIDIインストゥルメント(*.ins)数

#define WM_COMMANDWAKEUP (WM_USER + 0)
#define WM_COMMANDREADSHM (WM_USER + 1)
#define WM_COMMANDFILEOPEN (WM_USER + 2)
#define SHMSIZE 1024

// 汎用マクロ(最小、最大、挟み込み)
#ifndef MIN
#define MIN(A,B) ((A)>(B)?(B):(A))      // AとBの小さいほうを返す
#endif
#ifndef MAX
#define MAX(A,B) ((A)>(B)?(A):(B))      // AとBの大きいほうを返す
#endif
#ifndef CLIP
#define CLIP(A,B,C) ((A)>(B)?(A):((B)>(C)?(C):(B))) //BをAとCの間で丸めた値を返す
#endif

// TSIZEOFマクロ //20120211追加
#ifndef TSIZEOF
#define TSIZEOF(STR) (sizeof(STR)/sizeof(TCHAR))
#endif
#ifndef TCSLEN
#ifdef UNICODE
#define TCSLEN(STRING) wcslen(STRING)
#else
#define TCSLEN(STRING) strlen(STRING)
#endif
#endif
#ifndef TCSNCPY
#ifdef UNICODE
#define TCSNCPY(STRING1,STRING2,N) wcsncpy(STRING1,STRING2,N)
#else
#define TCSNCPY(STRING1,STRING2,N) strncpy(STRING1,STRING2,N)
#endif
#endif

// SendDiffrentStatusのフラグ用
#define SDS_NOTEOFF 0x0001              // ノートオフの状態を最新状態に復元する
#define SDS_NOTEON 0x0002               // ノートオンの状態を最新状態に復元する
#define SDS_NOTE (SDS_NOTEOFF | SDS_NOTEON) // ノートの状態を最新状態に復元する
#define SDS_KEYAFTER 0x0004             // キーアフタータッチの値をを最新状態に復元する
#define SDS_CONTROLCHANGE 0x0008        // コントロールチェンジの値を最新状態に復元する
#define SDS_PROGRAMCHANGE 0x0010        // プログラムチェンジの値を最新状態に復元する
#define SDS_CHANNELAFTER 0x0020         // チャンネルアフタータッチの値を最新状態に復元する
#define SDS_PITCHBEND 0x0040            // ピッチベンドの値を最新状態に復元する
#define SDS_RPN 0x0100                  // RPNの値を最新状態に復元する
#define SDS_NRPN 0x0200                 // NRPNの値を最新状態に復元する
#define SDS_MASTERVOLUME 0x0400         // マスターボリュームの値を最新状態に復元する
#define SDS_ALL 0xFFFF                  // すべての値を最新状態に復元する

#include "resource.h"                   // メイン シンボル

// ウィンドウの配置
typedef struct {
	int m_bIconic;                      // アイコン化されているか？
	int m_bZoomed;                      // 最大化されているか？
	int m_nX;                           // メインウィンドウ左上のX座標[pixel]
	int m_nY;                           // メインウィンドウ左上のY座標[pixel]
	int m_nWidth;                       // メインウィンドウの幅[pixel]
	int m_nHeight;                      // メインウィンドウの高さ[pixel]
} WindowPlacement;

// トラックの変更ダイアログ状態
typedef struct {
	int m_nAmount;                      // トラック番号又は変更量
	int m_nUnit;                        // 単位(0=絶対指定,1=相対変化)
} EditTrackDlgStatus;

// タイムの変更(TPQNベース用)ダイアログ状態
typedef struct {
	int m_nAmount;                      // タイムの変更量
	int m_nUnit;                        // 単位(0=小節,1=拍,2=ティック,3=パーセント)
} EditTimeDlgStatus;

// タイムの変更(SMPTEベース用)ダイアログ状態
typedef struct {
	int m_nAmount;                      // タイムの変更量
	int m_nUnit;                        // 単位(0=サブフレーム,1=フレーム,2=パーセント)
} EditTimeSmpDlgStatus;

// チャンネルの変更ダイアログの状態
typedef struct {
	int m_nAmount;                      // トラック番号又は変更量
	int m_nUnit;                        // 単位(0=絶対指定,1=相対変化)
} EditChannelStatus;

// キーの変更ダイアログの状態
typedef struct {
	int m_nAmount;                      // 音程の変更量
	int m_nUnit;                        // 単位(0=半音, 1=オクターブ)
	int m_nTargetNote;                  // ノートオン・ノートオフを対象とする(0/1)
	int m_nTargetKeyAfter;              // キーアフタータッチを対象とする(0/1)
} EditKeyDlgStatus;

// ベロシティの変更ダイアログの状態
typedef struct {
	int m_nAmount;                      // ベロシティの変更量
	int m_nUnit;                        // 単位(0=増減,1=パーセント)
	int m_nTargetNoteOn;                // ノートオンを対象にする(0/1)
	int m_nTargetNoteOff;               // ノートオフを対象にする(0/1)
} EditVelocityDlgStatus;

// 音長さの変更ダイアログの状態
typedef struct {
	int m_nAmount;                      // 音長さの変更量
	int m_nUnit;                        // 単位(0=ティック, 1=パーセント)
} EditDurationDlgStatus;

// 値の変更ダイアログの状態
typedef struct {
	int m_nAmount;                      // 値の変更量
	int m_nUnit;                        // 単位(0=増減,1=パーセント)
	int m_nTargetKeyAfter;              // キーアフタータッチを対象とする(0/1)
	int m_nTargetControlChange;         // コントロールチェンジを対象とする(0/1)
	int m_nTargetChannelAfter;          // チャンネルアフタータッチを対象とする(0/1)
	int m_nTargetPitchBend;             // ピッチベンドを対象とする(0/1)
} EditValueDlgStatus;

// 音符の細分化とトリル化ダイアログの状態
typedef struct {
	int m_nDurationIndex;               // 細分化後の1音の音長さインデックス(0=4分音符～)
	int m_nEnableTrill;                 // トリルを有効にするか？
	int m_nKeyShift;                    // トリル音のキーシフト
} EditBreakupAndTrillDlgStatus;

// クォンタイズダイアログの状態
typedef struct {
	int m_nSnapTimeIndex;               // スナップタイムインデックス(0=4分音符～)
	int m_nStrength;                    // 強度[%]
	int m_nTargetNoteOn;                // ターゲットノートオン
	int m_nTargetNoteOff;               // ターゲットノートオフ
} EditQuantizeDlgStatus;

// ビート検出とテンポ自動挿入ダイアログの状態
typedef struct {
	int m_nBeatTrackIndex;              // ビートが記述されたトラック番号(0～65535)
	int m_nBeatIntervalIndex;           // ビート間隔(1～)[tick]
	int m_nInsertTempo;                 // テンポを自動挿入するか？
} EditBeatScanDlgStatus;

// 小節の挿入ダイアログの状態
typedef struct {
	int m_nPosition;                    // 位置
	int m_nNumMeasure;                  // 小節数
} EditInsertMeasureDlgStatus;

// 小節の除去ダイアログの状態
typedef struct {
	int m_nPosition;                    // 位置
	int m_nNumMeasure;                  // 小節数
} EditRemoveMeasureDlgStatus;

// メトロノームダイアログの状態
typedef struct {
	int m_nOn;                          // メトロノームオン
	int m_nOutputPort;                  // 出力ポート(0～15)
	int m_nOutputChannel;               // 出力チャンネル(0～15)
	int m_nNoteKey1;                    // 強打ノートキー(0～127)
	int m_nNoteVel1;                    // 強打ベロシティ(1～127)
	int m_nNoteKey2;                    // 弱打ノートキー(0～127)
	int m_nNoteVel2;                    // 弱打ベロシティ(1～127)
} MetronomeDlgStatus;

// 自動保存ダイアログの状態
typedef struct {
	int m_nOn;                          // 自動保存オン
	long m_lInterval;                   // 自動保存間隔(1～7200)[秒]
	int m_nDisableWhilePlaying;         // 演奏中は自動保存しない
	int m_nDisableWhileRecording;       // リアルタイム入力中は自動保存しない
} AutoSaveDlgStatus;

// オプション(一般)ページの状態
typedef struct {
	BOOL m_bEnableMultiExec;            // 複数の世界樹を起動することを許可する
	BOOL m_bEnableMultiOpen;            // 複数のMIDIデータを開くことを許可する
	BOOL m_bRestoreWindowPlacement;     // 起動時に前回のウィンドウ位置を復元する
	BOOL m_bExecOpen;                   // 起動時に最後に開いたMIDIデータを自動的に開く
	BOOL m_bOpenPlay;                   // MIDIデータを開くと自動的に演奏を開始する
	BOOL m_bPlayUpdate;                 // 演奏位置移動時にパッチ･コントローラ･ピッチベンド･RPN･NRPNを最新値に更新する
	BOOL m_bSearchUpdate;               // 演奏開始時にパッチ･コントローラ･ピッチベンド･RPN･NRPNを最新値に更新する 
	BOOL m_bSearchSysx;                 // 演奏開始時にシステムエクスクルーシヴ(マスターヴォリュームなど)を最新値に更新する
	BOOL m_bEnableCC111Loop;            // オートリピート時にCC#111の位置からループ開始する
	BOOL m_bInvertCtrlMouseWheel;       // Ctrl+マウスホイール回転時の演奏位置移動方向を反転にする
	BOOL m_bTrackZeroOrigin;            // トラック番号を0から数える
	BOOL m_bEventZeroOrigin;            // イベント番号を0から数える
	BOOL m_bPatchSearch;                // CC#0･CC#32･プログラムチェンジ操作時に有効な音色だけを検索する
	BOOL m_bEnableAutoPageUpdate;       // 演奏開始時・位置移動時に自動的ページ更新をオンにする
	BOOL m_bSendNoteOffHoldOffAtEnd;    // 曲の終端に達したときノートオフやホールドオフを送信する
	long m_lUpDownDelta1;               // ▲▼の左クリック又は[+][-]キーで増減する量(1～127)
	long m_lUpDownDelta2;               // ▲▼の右クリック又は[Shift]+[+][-]キーで増減する量(1～127)
	long m_lKeyVelocity1;               // 鍵盤の左クリック又は[Z]-[\]キーで発音するベロシティ(1～127)
	long m_lKeyVelocity2;               // 鍵盤の右クリック又は[Shift]+[Z]-[\]キーで発音するベロシティ(1～127)
	long m_lSpeedSlow;                  // スピード=低速で演奏時のテンポ倍率(1～<5000>～100000)[*1/100%]
	long m_lSpeedNormal;                // スピード=標準で演奏時のテンポ倍率(1～<10000>～100000)[*1/100%]
	long m_lSpeedFast;                  // スピード=高速で演奏時のテンポ倍率(1～<20000>～100000)[*1/100%]
	long m_lPlayRecordInterval;         // MIDIデータ録音演奏時のwhileループ呼び出し間隔(1～1000)[ミリ秒]
	long m_lOctaveSignature;            // 中央のド(ｷｰ60)のｵｸﾀｰﾌﾞ番号表記(3～5)
} GeneralOption;

// トラックリストオプションページ1の設定
typedef struct TTrackListOption1 {
	long m_lDefRowZoom;                 // デフォルトの行方向拡大倍率[倍]
	long m_lDefColumnZoom;              // デフォルトの列方向拡大倍率[倍]
	long m_lDefTimeZoom;                // デフォルトの時間方向拡大倍率[倍]
	long m_lDefNameWidth;               // デフォルトの名前の幅[pixel]
	long m_lDefColorWidth;              // デフォルトの色の幅[pixel]
	long m_lDefInputOnWidth;            // デフォルトの入力ONの幅[pixel]
	long m_lDefInputPortWidth;          // デフォルトの入力ポートの幅[pixel]
	long m_lDefInputChWidth;            // デフォルトの入力CHの幅[pixel]
	long m_lDefOutputOnWidth;           // デフォルトの出力ONの幅[pixel]
	long m_lDefOutputPortWidth;         // デフォルトの出力ポートの幅[pixel]
	long m_lDefOutputChWidth;           // デフォルトの出力CHの幅[pixel]
	long m_lDefViewModeWidth;           // デフォルトの表示モードの幅[pixel]
} TrackListOption1;

// トラックリストオプションページ2の設定
typedef struct TTrackListOption2 {
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
} TrackListOption2;

// ピアノロールオプションページの設定
typedef struct {
	long m_lDefKeyZoom;                 // デフォルトのキー方向拡大倍率[倍]
	long m_lDefVelZoom;                 // デフォルトのベロシティ方向拡大倍率[倍]
	long m_lDefTimeZoom;                // デフォルトの時間方向拡大倍率[倍]
} PianoRollOption;

// イベントリストオプションページの設定
typedef struct {
	long m_lDefRowZoom;                 // デフォルトの行方向拡大倍率[倍]
	long m_lDefColumnZoom;              // デフォルトの列方向拡大倍率[倍]
	long m_lDefTrackWidth;              // デフォルトのトラックの幅[pixel]
	long m_lDefMillisecWidth;           // デフォルトのミリ秒の幅[pixel]
	long m_lDefTimeWidth;               // デフォルトのタイムの幅[pixel]
	long m_lDefKindWidth;               // デフォルトのイベントの種類の幅[pixel]
	long m_lDefChWidth;                 // デフォルトのチャンネルの幅[pixel]
	long m_lDefVal1Width;               // デフォルトの値1の幅[pixel]
	long m_lDefVal2Width;               // デフォルトの値2の幅[pixel]
	long m_lDefVal3Width;               // デフォルトの値3の幅[pixel]
	BOOL m_bInsertEventAfter;           // 同時刻のイベントの直後に挿入する
	BOOL m_bDuplicateEventAfter;        // 現在のイベントの直後に挿入する
	BOOL m_bDeleteEventAfter;           // 削除したイベントの次のイベントにフォーカスを合わせる
} EventListOption;

// 譜面オプションページの設定
typedef struct {
	long m_lDefTrackZoom;               // デフォルトのトラック方向拡大倍率[倍]
	long m_lDefTimeZoom;                // デフォルトの時間方向拡大倍率[倍]
} MusicalScoreOption;




class CSekaijuApp : public CWinApp {

	
	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CSekaijuApp();                      // コンストラクタ


	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	HINSTANCE m_hResourceDLL;
public:
	// ドキュメントテンプレートへのポインタを保持
	CDocTemplate* m_pSekaijuDocTemplate;
public:
	// MIDIデバイスオブジェクトとMIDIステータスオブジェクト
	MIDIIn*     m_pMIDIIn[MAXMIDIINDEVICENUM];
	MIDIOut*    m_pMIDIOut[MAXMIDIOUTDEVICENUM];
	MIDIStatus* m_pMIDIStatus[MAXMIDIOUTDEVICENUM];
	MIDIStatus* m_pTempMIDIStatus[MAXMIDIOUTDEVICENUM];
	MIDIInstrument* m_pMIDIInstrument[MAXMIDIINSTRUMENTNUM]; // 20100220修正
	MIDIInstrumentDefinition* m_pMIDIInstDefNorm[MAXMIDIOUTDEVICENUM];
	MIDIInstrumentDefinition* m_pMIDIInstDefDrum[MAXMIDIOUTDEVICENUM];
public:
	// MIDI入出力などに対するクリティカルセクション
	CCriticalSection m_theCriticalSection;
public:
	// パス名
	CString m_strExeFilePath;           // 実行ファイルのあるパス名
	CString m_strInitialPath;           // iniファイルのあるパス名
	CString m_strFileOpenPath;          // ファイルオープンダイアログのパス名
	CString m_strSysxOpenPath;          // SYSXオープンダイアログのパス名

	// MIDI入出力デバイスの名前
	CString m_strMIDIInName[MAXMIDIINDEVICENUM];
	CString m_strMIDIOutName[MAXMIDIOUTDEVICENUM];

	// MIDIインストゥルメント定義の名前
	CString m_strMIDIInstDefNormName[MAXMIDIOUTDEVICENUM];
	CString m_strMIDIInstDefDrumName[MAXMIDIOUTDEVICENUM];

	// MIDI同期モード
	long m_lMIDIInSyncMode[MAXMIDIINDEVICENUM];
	long m_lMIDIOutSyncMode[MAXMIDIOUTDEVICENUM];

	// コントロール状態
	BOOL m_bPlaying;       // 演奏フラグ (0=停止中,1=演奏中)
	BOOL m_bRecording;     // 録音フラグ (0=停止中,1=録音中)
	BOOL m_bAutoRepeat;    // オートリピート(0=OFF,1=ON)
	long m_lCurSpeedIndex; // スピード (0=静止,1=低速,2=標準,3=高速,4=他機器にスレーブ)
	long m_lOldSpeedIndex; // スピード (0=静止,1=低速,2=標準,3=高速,4=他機器にスレーブ)

	// 全般オプション状態
	GeneralOption m_theGeneralOption;

	// トラックリストオプション1状態
	TrackListOption1 m_theTrackListOption1;

	// トラックリストオプション2状態
	TrackListOption2 m_theTrackListOption2;

	// ピアノロールオプション状態
	PianoRollOption m_thePianoRollOption;

	// イベントリストオプション状態
	EventListOption m_theEventListOption;

	// 譜面オプション状態
	MusicalScoreOption m_theMusicalScoreOption;
	
	// ウィンドウ配置状態
	WindowPlacement m_theWindowPlacement;

	// EditTrackダイアログ状態
	EditTrackDlgStatus m_theEditTrackDlgStatus;
	
	// EditTimeダイアログ状態
	EditTimeDlgStatus m_theEditTimeDlgStatus;

	// EditTimeSmpダイアログ状態
	EditTimeSmpDlgStatus m_theEditTimeSmpDlgStatus;

	// EditChannelダイアログ状態
	EditChannelStatus m_theEditChannelDlgStatus;

	// EditKeyダイアログ状態
	EditKeyDlgStatus m_theEditKeyDlgStatus;

	// EditVelocityダイアログ状態
	EditVelocityDlgStatus m_theEditVelocityDlgStatus;

	// EditDurationダイアログ状態
	EditDurationDlgStatus m_theEditDurationDlgStatus;

	// EditValueダイアログ状態
	EditValueDlgStatus m_theEditValueDlgStatus;

	// EditBreakupAndTrillダイアログ状態
	EditBreakupAndTrillDlgStatus m_theEditBreakupAndTrillDlgStatus;

	// EditQuantizeダイアログ状態
	EditQuantizeDlgStatus m_theEditQuantizeDlgStatus;

	// EditBeatScanダイアログ状態
	EditBeatScanDlgStatus m_theEditBeatScanDlgStatus;

	// EditInsertMeasureダイアログ状態
	EditInsertMeasureDlgStatus m_theEditInsertMeasureDlgStatus;

	// EditRemoveMeasureダイアログ状態
	EditRemoveMeasureDlgStatus m_theEditRemoveMeasureDlgStatus;

	// Metronomeダイアログ状態
	MetronomeDlgStatus m_theMetronomeDlgStatus;

	// 自動保存ダイアログ状態
	AutoSaveDlgStatus m_theAutoSaveDlgStatus;

	// 最後に開いたファイル名
	CString m_strLastOpenFileName[16];

	// 言語
	CString m_strLanguage;

	// デフォルトフォント
	CFont m_theDefaultFont;

	// デフォルトトラックカラー
	long m_lDefTrackColor[16];

	// カーソル
	HCURSOR m_hCursorArrow;
	HCURSOR m_hCursorCross;
	HCURSOR m_hCursorSizeWE;
	HCURSOR m_hCursorSizeNS;
	HCURSOR m_hCursorSizeAll;
	HCURSOR m_hCursorSizeAllCopy;
	HCURSOR m_hCursorNo;
	HCURSOR m_hCursorResizeWE;
	HCURSOR m_hCursorResizeNS;
	HCURSOR m_hCursorResizeAll;
	HCURSOR m_hCursorDraw;
	HCURSOR m_hCursorLine;
	HCURSOR m_hCursorEraser;
	HCURSOR m_hCursorSelect;
	HCURSOR m_hCursorSelectAdd;
	HCURSOR m_hCursorSelect2;
	HCURSOR m_hCursorSelectAdd2;
	HCURSOR m_hCursorSpeaker;

	// イベントの種類名
	CString m_strEventKindName[256];

	// ノートキー名
	CString m_strNoteKeyName[256];

	// 一時的な変数
	BOOL m_bIgnoreNoteEvent;
	BOOL m_bFirstMetronome;
	CPtrArray m_theTempRecordedEventArray;
	BOOL m_bInplaceEditing;
	BOOL m_bInplaceListing;
	BOOL m_bValueUpDowning;

	// 現在の子ウィンドウと現在のドキュメント
	CMDIChildWnd* m_pCurChildWnd;
	CDocument* m_pCurDocument;
	CMDIChildWnd* m_pOldChildWnd;
	CDocument* m_pOldDocument;

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual BOOL InitInstance ();
	virtual int  ExitInstance ();
	virtual void AddDocTemplate (CDocTemplate* pTemplate);
	virtual BOOL PreTranslateMessage (MSG* pMsg);
  	virtual BOOL OnIdle (LONG lCount);

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	virtual BOOL LoadResourceDLL ();
	virtual BOOL FreeResourceDLL ();
	virtual BOOL CheckMultiExec (CCommandLineInfo* pCmdInfo);
	virtual BOOL LoadIniFile ();
	virtual BOOL SaveIniFile ();
	virtual void UpdateMenu ();
	virtual void UpdateCurWndAndDocPtr ();
	virtual void ResetMIDIStatusArray ();
	virtual void ResetTempMIDIStatusArray ();
	virtual void SendAllHold1Off ();
	virtual void SendAllSostenutoOff ();
	virtual void SendAllHold2Off ();
	virtual void SendAllSoundOff ();
	virtual void SendResetAllController ();
	virtual void SendAllNoteOff ();
	virtual void SendDifferentStatus (long lFlags);
	virtual void OpenAllMIDIInDevice ();
	virtual void OpenAllMIDIOutDevice ();
	virtual long LoadAllMIDIInstrument ();
	virtual long SelectAllMIDIInstDefNorm ();
	virtual long SelectAllMIDIInstDefDrum ();
	virtual long SetPlayPosition (CDocument* pDocument, long lTargetTime);
	virtual long StartPlaying (CDocument* pDocument);
	virtual long StopPlaying (CDocument* pDocument);
	virtual long StartRecording (CDocument* pDocument);
	virtual long StopRecording (CDocument* pDocument);
	virtual void ApplyMIDIDeviceSheet (CPropertySheet* pSheet);
	virtual void ApplyMIDISyncModeSheet (CPropertySheet* pSheet);
	virtual void ApplyOptionSheet (CPropertySheet* pSheet);
	virtual BOOL GetCurSyncInputPortAndMode (long* pPort, long* pMode);

public:
	// 演奏・録音用スレッド関連
	static UINT PlayRecordThread (LPVOID pInfo);
	virtual BOOL PlayRecordProc (LPVOID pInfo);
	CWinThread* m_pPlayRecordThread;
	BOOL m_bPlayRecordThreadRunning;

	//--------------------------------------------------------------------------
	// インプリメンテーション
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnUpdateFileNewUI (CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOpenUI (CCmdUI* pCmdUI);
	afx_msg void OnControlToBegin ();
	afx_msg void OnUpdateControlToBeginUI (CCmdUI* pCmdUI);
	afx_msg void OnControlToEnd ();
	afx_msg void OnUpdateControlToEndUI (CCmdUI* pCmdUI);
	afx_msg void OnControlPlay ();
	afx_msg void OnUpdateControlPlayUI (CCmdUI* pCmdUI);
	afx_msg void OnControlRecord ();
	afx_msg void OnUpdateControlRecordUI (CCmdUI* pCmdUI);
	afx_msg void OnControlPrevMeasure ();
	afx_msg void OnUpdateControlPrevMeasureUI (CCmdUI* pCmdUI);
	afx_msg void OnControlNextMeasure ();
	afx_msg void OnUpdateControlNextMeasureUI (CCmdUI* pCmdUI);
	afx_msg void OnControlPrevBeat ();
	afx_msg void OnUpdateControlPrevBeatUI (CCmdUI* pCmdUI);
	afx_msg void OnControlNextBeat ();
	afx_msg void OnUpdateControlNextBeatUI (CCmdUI* pCmdUI);
	afx_msg void OnControlSpeedNone ();
	afx_msg void OnUpdateControlSpeedNoneUI (CCmdUI* pCmdUI);
	afx_msg void OnControlSpeedSlow ();
	afx_msg void OnUpdateControlSpeedSlowUI (CCmdUI* pCmdUI);
	afx_msg void OnControlSpeedNormal ();
	afx_msg void OnUpdateControlSpeedNormalUI (CCmdUI* pCmdUI);
	afx_msg void OnControlSpeedFast ();
	afx_msg void OnUpdateControlSpeedFastUI (CCmdUI* pCmdUI);
	afx_msg void OnControlSpeedSlave ();
	afx_msg void OnUpdateControlSpeedSlaveUI (CCmdUI* pCmdUI);
	afx_msg void OnControlAutoRepeat ();
	afx_msg void OnUpdateControlAutoRepeatUI (CCmdUI* pCmdUI);
	afx_msg void OnSetupMIDIDevice ();
	afx_msg void OnSetupMIDISyncMode ();
	afx_msg void OnSetupInstrument ();
	afx_msg void OnSetupMetronome ();
	afx_msg void OnSetupAutoSave ();
	afx_msg void OnSetupLanguage ();
	afx_msg void OnSetupOptions ();
	afx_msg void OnHelpReadMe ();
	afx_msg void OnHelpLicense ();
	afx_msg void OnHelpManual ();
	afx_msg void OnHelpProjectHomePage ();
	afx_msg void OnHelpAbout ();
	DECLARE_MESSAGE_MAP()
};

#endif
