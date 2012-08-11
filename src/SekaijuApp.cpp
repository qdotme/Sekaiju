//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 世界樹アプリケーションクラス
// (C)2002-2012 おーぷんMIDIぷろじぇくと／くず
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

#include "winver.h"
#include <afxwin.h>
#include <afxext.h>
#include <afxcmn.h>
#include <afxmt.h>
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "resource.h"
#include "ColorfulComboBox.h"
#include "ColorfulCheckListBox.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDocManager.h"
#include "SekaijuDocTemplate.h"
#include "SekaijuToolBar.h"
#include "MainFrame.h"
#include "ChildFrame.h"
#include "SekaijuDoc.h"
#include "SekaijuView.h"
#include "PianoRollFrame.h"
#include "TrackListFrame.h"

#include "MIDIDeviceSheet.h"
#include "MIDISyncModeSheet.h"
#include "AutoSaveDlg.h"
#include "LanguageDlg.h"
#include "MetronomeDlg.h"
#include "OptionSheet.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// メッセージマップ
BEGIN_MESSAGE_MAP (CSekaijuApp, CWinApp)
	ON_COMMAND (ID_FILE_NEW, CWinApp::OnFileNew)
	ON_UPDATE_COMMAND_UI (ID_FILE_NEW, CSekaijuApp::OnUpdateFileNewUI)
	ON_COMMAND (ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_UPDATE_COMMAND_UI (ID_FILE_OPEN, CSekaijuApp::OnUpdateFileOpenUI)
	ON_COMMAND (ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND (ID_CONTROL_TOBEGIN, OnControlToBegin)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_TOBEGIN, OnUpdateControlToBeginUI)
	ON_COMMAND (ID_CONTROL_TOEND, OnControlToEnd)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_TOEND, OnUpdateControlToEndUI)
	ON_COMMAND (ID_CONTROL_PLAY, OnControlPlay)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_PLAY, OnUpdateControlPlayUI)
	ON_COMMAND (ID_CONTROL_RECORD, OnControlRecord)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_RECORD, OnUpdateControlRecordUI)
	ON_COMMAND (ID_CONTROL_PREVMEASURE, OnControlPrevMeasure)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_PREVMEASURE, OnUpdateControlPrevMeasureUI)
	ON_COMMAND (ID_CONTROL_NEXTMEASURE, OnControlNextMeasure)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_NEXTMEASURE, OnUpdateControlNextMeasureUI)
	ON_COMMAND (ID_CONTROL_PREVBEAT, OnControlPrevBeat)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_PREVBEAT, OnUpdateControlPrevBeatUI)
	ON_COMMAND (ID_CONTROL_NEXTBEAT, OnControlNextBeat)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_NEXTBEAT, OnUpdateControlNextBeatUI)
	ON_COMMAND (ID_CONTROL_SPEEDNONE, OnControlSpeedNone)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_SPEEDNONE, OnUpdateControlSpeedNoneUI)
	ON_COMMAND (ID_CONTROL_SPEEDSLOW, OnControlSpeedSlow)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_SPEEDSLOW, OnUpdateControlSpeedSlowUI)
	ON_COMMAND (ID_CONTROL_SPEEDNORMAL, OnControlSpeedNormal)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_SPEEDNORMAL, OnUpdateControlSpeedNormalUI)
	ON_COMMAND (ID_CONTROL_SPEEDFAST, OnControlSpeedFast)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_SPEEDFAST, OnUpdateControlSpeedFastUI)
	ON_COMMAND (ID_CONTROL_SPEEDSLAVE, OnControlSpeedSlave)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_SPEEDSLAVE, OnUpdateControlSpeedSlaveUI)
	ON_COMMAND (ID_CONTROL_AUTOREPEAT, OnControlAutoRepeat)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_AUTOREPEAT, OnUpdateControlAutoRepeatUI)
	ON_COMMAND (ID_SETUP_MIDIDEVICE, OnSetupMIDIDevice)
	ON_COMMAND (ID_SETUP_MIDISYNCMODE, OnSetupMIDISyncMode)
	ON_COMMAND (ID_SETUP_INSTRUMENT, OnSetupInstrument)
	ON_COMMAND (ID_SETUP_METRONOME, OnSetupMetronome)
	ON_COMMAND (ID_SETUP_AUTOSAVE, OnSetupAutoSave)
	ON_COMMAND (ID_SETUP_LANGUAGE, OnSetupLanguage)
	ON_COMMAND (ID_SETUP_OPTIONS, OnSetupOptions)
	ON_COMMAND (ID_HELP_README, OnHelpReadMe)
	ON_COMMAND (ID_HELP_LICENSE, OnHelpLicense)
	ON_COMMAND (ID_HELP_MANUAL, OnHelpManual)
	ON_COMMAND (ID_HELP_PROJECTHOMEPAGE, OnHelpProjectHomePage)
	ON_COMMAND (ID_HELP_ABOUT, OnHelpAbout)
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CSekaijuApp::CSekaijuApp () {
	m_pPlayRecordThread = NULL;
	m_bPlayRecordThreadRunning = FALSE;	
	m_pCurChildWnd = NULL;
	m_pCurDocument = NULL;
	m_pOldChildWnd = NULL;
	m_pOldDocument = NULL;
	m_bPlaying = FALSE;
	m_bRecording = FALSE;
	m_lCurSpeedIndex = 2;
	m_lOldSpeedIndex = 2;
	long i;
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		m_pMIDIIn[i] = NULL;
	}
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		m_pMIDIOut[i] = NULL;
		m_pMIDIStatus[i] = NULL;
		m_pTempMIDIStatus[i] = NULL;
	}
	m_bIgnoreNoteEvent = FALSE;
	m_bFirstMetronome = FALSE;
	m_bInplaceEditing = FALSE;
	m_bInplaceListing = FALSE;
	m_bValueUpDowning = FALSE;
	// デフォルトの色を指定しておく
	m_lDefTrackColor[0] = 0x00FF0000;
	m_lDefTrackColor[1] = 0x00808000;
	m_lDefTrackColor[2] = 0x00008000;
	m_lDefTrackColor[3] = 0x00008080;
	m_lDefTrackColor[4] = 0x000000F0;
	m_lDefTrackColor[5] = 0x008000F0;
	m_lDefTrackColor[6] = 0x00800080;
	m_lDefTrackColor[7] = 0x00FF0080;
	m_lDefTrackColor[8] = 0x00FF0000;
	m_lDefTrackColor[9] = 0x00808000;
	m_lDefTrackColor[10] = 0x00008000;
	m_lDefTrackColor[11] = 0x00008080;
	m_lDefTrackColor[12] = 0x000000F0;
	m_lDefTrackColor[13] = 0x008000F0;
	m_lDefTrackColor[14] = 0x00800080;
	m_lDefTrackColor[15] = 0x00FF0080;
}

//------------------------------------------------------------------------------
// 唯一の CSekaijuApp オブジェクト
//------------------------------------------------------------------------------
CSekaijuApp theApp;


//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// CSekaijuApp クラスの初期化
BOOL CSekaijuApp::InitInstance () {

	// コモンコントロールの初期化
	// 次のInitCommonControlsは、
	// デバッグ版又はリリース版でsekaiju.exe.manufestと共用したときに
	// エラーを起こして起動できなくなる場合は、コメントアウトすること。
	//InitCommonControls ();
	
#ifdef _AFXDLL
	Enable3dControls();			// 共有DLLの中でMFCを使用する場合 
#else
	Enable3dControlsStatic();	// MFCと静的にリンクしている場合
#endif

	CString strMsg;
	CString strFileName;

	// EXEファイルのあるフルパス名を取得
	TCHAR szPath[_MAX_PATH];
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szFileName[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];
	::GetModuleFileName (this->m_hInstance, szPath, TSIZEOF (szPath));
	_tsplitpath (szPath, szDrive, szDir, szFileName, szExt);
	m_strExeFilePath.Format (_T("%s%s"), szDrive, szDir);
	if (m_strExeFilePath.Right (7) == _T("\\Debug\\")) {
		m_strExeFilePath = m_strExeFilePath.Left (m_strExeFilePath.GetLength () - 6);
	}
	else if (m_strExeFilePath.Right (9) == _T("\\Release\\")) {
		m_strExeFilePath = m_strExeFilePath.Left (m_strExeFilePath.GetLength () - 8);
	}
	else if (m_strExeFilePath.Right (8) == _T("\\DebugU\\")) {
		m_strExeFilePath = m_strExeFilePath.Left (m_strExeFilePath.GetLength () - 7);
	}
	else if (m_strExeFilePath.Right (10) == _T("\\ReleaseU\\")) {
		m_strExeFilePath = m_strExeFilePath.Left (m_strExeFilePath.GetLength () - 9);
	}

	// 初期化時のカレントディレクトリ取得
	memset (szPath, 0, sizeof (szPath));
	::GetCurrentDirectory (MAX_PATH, szPath);
	m_strInitialPath.Format (_T("%s"), szPath);

	// INIファイルのフルパス名の変更
	if (m_pszProfileName) {
		delete ((void*)m_pszProfileName);
		m_pszProfileName = NULL;
	}
	m_pszProfileName = new TCHAR[MAX_PATH + 1]; // 20120422修正
	if (!m_pszProfileName) {
		strMsg.LoadString (IDS_INSUFFICIENT_MEMORY_FOR_EXEFILENAME);
		AfxMessageBox (strMsg, MB_ICONSTOP);
		return FALSE;
	}
	memset ((void*)m_pszProfileName, 0, sizeof (TCHAR) * (MAX_PATH + 1)); // 20120422修正
	_sntprintf ((TCHAR*)m_pszProfileName, MAX_PATH, _T("%s%s"), m_strExeFilePath, _T("Sekaiju.ini"));

	// 標準のINIファイルのオプションをロードする(最近使ったファイルを含む)
	LoadStdProfileSettings (16);  

	// INIファイルから設定の読み込み
	LoadIniFile ();

	// リソースDLLの読み込み
	if (LoadResourceDLL () == FALSE) {
		return FALSE;
		// この関数の後でCSekaijuApp::ExitInstanceが呼び出される。
	}

	// アプリケーション名の変更(20100128:リソースのDLL化に伴い追加)
	TCHAR szAppName[256];
	memset (szAppName, 0, sizeof (szAppName));
	VERIFY (::LoadString (m_hResourceDLL, AFX_IDS_APP_TITLE, szAppName, 255));
	free ((void*)m_pszAppName );
	m_pszAppName = NULL;
	m_pszAppName = _tcsdup (szAppName);
	if (m_pszAppName == NULL) {
		strMsg.LoadString (IDS_INSUFFICIENT_MEMORY_FOR_EXEFILENAME);
		::MessageBox (NULL, strMsg, _T("Sekaiju"), MB_ICONSTOP);
		return FALSE;
	}
	AfxGetModuleState()->m_lpszCurrentAppName = m_pszAppName;

	// DDE、file open など標準のシェル コマンドのコマンドラインを解析。
	CCommandLineInfo cmdInfo;
	ParseCommandLine (cmdInfo); //20081019位置変更

	// 二重起動の防止
	if (CheckMultiExec (&cmdInfo) == FALSE) {
		return FALSE;
		// この関数の後でCSekaijuApp::ExitInstanceが呼び出される。
	}

	// 最後に開いたファイルを自動的に開く場合、cmdInfoを書き換える。
	if (m_theGeneralOption.m_bExecOpen == TRUE &&
		m_strLastOpenFileName[0] != _T("") &&
		cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew) {
		// 最後に開いたファイルが存在する場合のみ(20081019)
		if (::GetFileAttributes (m_strLastOpenFileName[0]) != -1) {
			cmdInfo.m_nShellCommand = CCommandLineInfo::FileOpen;
			cmdInfo.m_strFileName = m_strLastOpenFileName[0];
		}
	}

	// すべてのMIDI入力デバイスを開く
	OpenAllMIDIInDevice ();

	// すべてのMIDI出力デバイスを開く
	OpenAllMIDIOutDevice ();

	// すべてのMIDIインストゥルメント定義ファイル(*.ins)を読み込む
	LoadAllMIDIInstrument ();
	
	SelectAllMIDIInstDefNorm ();
	SelectAllMIDIInstDefDrum ();

	// カーソルの読み込み
	m_hCursorArrow = LoadStandardCursor (IDC_ARROW);
	m_hCursorCross = LoadStandardCursor (IDC_CROSS);
	m_hCursorSizeWE = LoadStandardCursor (IDC_SIZEWE);
	m_hCursorSizeNS = LoadStandardCursor (IDC_SIZENS);
	m_hCursorSizeAll = LoadStandardCursor (IDC_SIZEALL);
	m_hCursorSizeAllCopy = LoadCursor (IDC_SIZEALLCOPY);
	m_hCursorNo = LoadStandardCursor (IDC_NO);
	m_hCursorResizeWE = LoadCursor (IDC_RESIZEWE);
	m_hCursorResizeNS = LoadCursor (IDC_RESIZENS);
	m_hCursorResizeAll = LoadCursor (IDC_RESIZEALL);
	m_hCursorDraw = LoadCursor (IDC_DRAW);
	m_hCursorLine = LoadCursor (IDC_LINE);
	m_hCursorEraser = LoadCursor (IDC_ERASER);
	m_hCursorSelect = LoadCursor (IDC_SELECT);
	m_hCursorSelectAdd = LoadCursor (IDC_SELECTADD);
	m_hCursorSelect2 = LoadCursor (IDC_SELECT2);
	m_hCursorSelectAdd2 = LoadCursor (IDC_SELECTADD2);
	m_hCursorSpeaker = LoadCursor (IDC_SPEAKER);

	// イベントの種類名の読み込み
	long i = 0;
	for (i = 0; i < 256; i++) {
		m_strEventKindName[i].LoadString (IDS_EVENTKIND_00 + i);
		if (m_strEventKindName[i] == _T("")) {
			m_strEventKindName[i].Format (_T("0x%02X"), i);
		}
	}

	// キー名の読み込み
	for (i = 0; i < 256; i++) {
		m_strNoteKeyName[i].LoadString (IDS_NOTEKEY_0S00 + i);
		if (m_strNoteKeyName[i] == _T("")) {
			m_strNoteKeyName[i].Format (_T("ERR"));
		}
	}

	// デフォルトフォントの読み込み
	CString strDefaultFontName;
	VERIFY (strDefaultFontName.LoadString (IDS_DEFAULTFONTNAME));
	m_theDefaultFont.CreateFont (12, 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,  
		strDefaultFontName);

	// 一時変数の初期化
	m_bInplaceEditing = 0;
	m_bInplaceListing = 0;
	m_bValueUpDowning = 0;

	// 自動保存用ディレクトリの作成
	CString strDirName1;
	CString strDirName2;
	strDirName1.LoadString (IDS_AUTOSAVEDIRNAME);
	strDirName2 = m_strExeFilePath + strDirName1;
	if (strDirName2.Right (1) == _T("\\")) {
		strDirName2 = strDirName2.Left (strDirName2.GetLength () - 1);
	}
	::CreateDirectory ((LPCTSTR)strDirName2, NULL);

	// MIDIデータ用のドキュメントテンプレートを作成
	CSekaijuDocTemplate* pDocTemplate;
	pDocTemplate = new CSekaijuDocTemplate(
		IDR_SEKAIJUTYPE,
		RUNTIME_CLASS(CSekaijuDoc),
		RUNTIME_CLASS(CTrackListFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CSekaijuView));
	AddDocTemplate(pDocTemplate);
	m_pSekaijuDocTemplate = pDocTemplate;

	//DragAcceptFiles (TRUE); // 意味なし
	//EnableShellOpen(); // 意味なし

	// メインMDIフレームウィンドウを作成
	// ウィンドウクラス名をオリジナルなものにするためにLoadFrameではなくCreateを使う。
	// 注意：LoadFrameで構築するとCMainFrame::PreCreateWindowでの設定が反映されない。
	// 注意：Createで構築した場合、デフォルトメニューとアクセラレータは別途設定しなければならない。
	CMainFrame* pMainFrame = new CMainFrame;
	if (pMainFrame->Create (
		NULL, // ウィンドウクラス名(CMainFrame::PreCreateWindow内で設定する)
		this->m_pszAppName,  // タイトル
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,  // ウィンドウスタイル
		CRect (0, 0, 640, 400), // 仮配置(CMainFrame::PreCreateWindow内で再設定する)
		NULL, // 親
		MAKEINTRESOURCE (IDR_MAINFRAME), // メニュー
		0, // ExStyle 
		NULL) == FALSE) {
		return FALSE;
	}

	// デフォルトメニューの設定(メインMDIフレームをCreateで生成したため)
	ASSERT (pMainFrame->m_hWnd != NULL);
	pMainFrame->m_hMenuDefault = ::GetMenu (pMainFrame->m_hWnd);	
	
	// アクセラレータの設定(メインMDIフレームをCreateで生成したため)
	VERIFY (pMainFrame->LoadAccelTable (MAKEINTRESOURCE(IDR_MAINFRAME)));
	m_pMainWnd = pMainFrame;

	// 解析済みコマンドラインを反映
	if (!ProcessShellCommand (cmdInfo)) {
		// 自動的に最後に使ったファイルを開いたが失敗した場合
		if (m_theGeneralOption.m_bExecOpen == TRUE &&
			cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen &&
			cmdInfo.m_strFileName == m_strLastOpenFileName[0]) {
			m_strLastOpenFileName[0] = _T("");
			//AfxGetMainWnd ()->PostMessage (WM_COMMAND, ID_FILE_NEW, 0);
		}
		// 通常の失敗の場合
		else {
			return FALSE;
		}
	}

	// メイン ウィンドウが初期化されたので、表示と更新を行う。
	if (m_theGeneralOption.m_bRestoreWindowPlacement) {
		if (m_theWindowPlacement.m_bIconic) {
			m_nCmdShow = SW_SHOWMINIMIZED;
		}
		else if (m_theWindowPlacement.m_bZoomed) {
			m_nCmdShow = SW_SHOWMAXIMIZED;
		}
	}
	pMainFrame->ShowWindow (m_nCmdShow);
	pMainFrame->UpdateWindow();

	// メニューの更新
	UpdateMenu ();
	
	// 録音演奏用スレッドの開始
	m_bPlayRecordThreadRunning = TRUE;
	m_pPlayRecordThread = AfxBeginThread (PlayRecordThread, this);
	if (m_pPlayRecordThread == NULL) {
		_RPTF1 (_CRT_WARN, "PlayRecordThread Create Failed, Address=0x%p.\n", m_pPlayRecordThread);
		// "録音再生スレッドを作成することができません。"
		strMsg.LoadString (IDS_RECPLAYTHREAD_CREATE_ERROR);
		AfxMessageBox (strMsg, MB_ICONSTOP);
	}
	else {
		_RPTF1 (_CRT_WARN, "PlayRecordThread Create Successful, Address=0x%p.\n", m_pPlayRecordThread);
	}
	
	return TRUE;
}

// アプリケーションの後処理
int CSekaijuApp::ExitInstance () {
	m_theCriticalSection.Lock ();
	m_bPlayRecordThreadRunning = FALSE;
	m_theCriticalSection.Unlock ();

	::Sleep (500);
	int nRet = CWinApp::ExitInstance ();

	// INIファイルへ設定を保存
	SaveIniFile ();

	long i;
	
	// MIDI入力デバイスを閉じる
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		if (m_pMIDIIn[i] != NULL) {
			MIDIIn_Close (m_pMIDIIn[i]);
			m_pMIDIIn[i] = NULL;
		}
	}

	// MIDI出力デバイスを閉じる
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		if (m_pMIDIOut[i] != NULL) {
			MIDIOut_Close (m_pMIDIOut[i]);
			m_pMIDIOut[i] = NULL;
		}
		if (m_pMIDIStatus[i]) {
			MIDIStatus_Delete (m_pMIDIStatus[i]);
			m_pMIDIStatus[i] = NULL;
		}
		if (m_pTempMIDIStatus[i]) {
			MIDIStatus_Delete (m_pTempMIDIStatus[i]);
			m_pTempMIDIStatus[i] = NULL;
		}
	}

	// リソースDLLの開放
	FreeResourceDLL ();

	// INIファイルのフルパス用に割り当てられたメモリを解放する。
	if (m_pszProfileName) {
		delete ((void*)m_pszProfileName);
		m_pszProfileName = NULL;
	}

	// アプリケーション名用に割り当てられたメモリを解放する(20100202:リソースのDLL化に伴い追加)
	if (m_pszAppName) {
		free ((void*)m_pszAppName);
		m_pszAppName = NULL;
	}

	::Sleep (500);

	return nRet;

}

// CWinApp::AddDocTemplateのオーバーライド
void CSekaijuApp::AddDocTemplate (CDocTemplate* pTemplate) {
	if (m_pDocManager == NULL) {
		m_pDocManager = new CSekaijuDocManager;
	}
	m_pDocManager->AddDocTemplate (pTemplate);
}

// CWinApp::PreTranslateMessageのオーバーライド20080809,20091222
BOOL CSekaijuApp::PreTranslateMessage (MSG* pMsg) {
	if (pMsg->message == WM_KEYDOWN) {
		// インプレーステキスト入力時もしくは
		// インプレースリスト選択時は
		// アクセラレーターキーを効かなくする。
		if (m_bInplaceEditing || m_bInplaceListing) {
			return 0; // 通常の(アクセラレーターを介さない)処理
		}
		// 特定のウィンドウ上にフォーカスがある場合は、
		// そのウィンドウのキー操作を優先する(20091222追加)
		CWnd* pFocusedWnd = AfxGetMainWnd ()->GetFocus ();
		if (pFocusedWnd != NULL) {
			// コンボボックスの場合はGetFocus()によって、
			// コンボボックス内の"Edit"へのポインタが取得できるので、
			// その親のコントロールIDを調べなければならない。
			//char szClassName[256];
			//memset (szClassName, 0, sizeof (szClassName));
			//HWND hFocusedWnd = pFocusedWnd->GetSafeHwnd ();
			//GetClassName (hFocusedWnd, szClassName, sizeof (szClassName));
			int nCtrlID = pFocusedWnd->GetDlgCtrlID ();
			CWnd* pParentWnd = pFocusedWnd->GetParent ();
			int nParentCtrlID = 0;
			if (pParentWnd) {
				nParentCtrlID = pParentWnd->GetDlgCtrlID ();
			}
			// ピアノロールウィンドウのツールバーのベロシティコンボボックスの場合
			// ピアノロールウィンドウのツールバーの音長さコンボボックスの場合
			// イベントリストウィンドウのツールバーのタイムエディットの場合
			if (nParentCtrlID == IDC_VELOCITYCOMBO ||
				nParentCtrlID == IDC_DURATIONCOMBO ||
				nCtrlID == IDC_EVENTTIMEEDIT) {
				return 0; // 通常の(アクセラレーターを介さない)処理
			}
			return CWinApp::PreTranslateMessage (pMsg); // アクセラレーターを介した処理
		}
		return CWinApp::PreTranslateMessage (pMsg); // アクセラレーターを介した処理
	}
	return CWinApp::PreTranslateMessage (pMsg); // アクセラレーターを介した処理
}

// アイドリング時のオーバーライド
BOOL CSekaijuApp::OnIdle (LONG lCount) {
	// 通常のアイドリング処理(ツールバーの描画など)
	BOOL bRet = CWinApp::OnIdle (lCount);
	// ツールバーの更新(アイドリング256回に1回の割合で)
	if ((lCount & 0x000000FF) == 0) {
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
		if (pMainFrame) {
			CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
			if (pSekaijuDoc) {
				if (pMainFrame->m_wndPositionScroll.GetSafeHwnd ()) {
					if (pMainFrame->m_wndPositionScroll.IsWindowEnabled () == 0) {
						//pMainFrame->m_wndPositionScroll.EnableWindow (1);
					}
				}
			}
			else {
				if (pMainFrame->m_wndPositionScroll.GetSafeHwnd ()) {
					if (pMainFrame->m_wndPositionScroll.IsWindowEnabled () != 0) {
						//pMainFrame->m_wndPositionScroll.EnableWindow (0);
					}
				}
			}
		}
	}
	// 自動保存(アイドリング256回に1回の割合で)
	if (m_theAutoSaveDlgStatus.m_nOn && (lCount & 0x000000FF) == 0) {
		POSITION docpos = m_pSekaijuDocTemplate->GetFirstDocPosition ();
		while (docpos) {
			CSekaijuDoc* pSekaijuDoc =(CSekaijuDoc*)(m_pSekaijuDocTemplate->GetNextDoc (docpos));
			// ドキュメントが変更されている場合のみかつ保存ロックされていない場合のみ
			if (pSekaijuDoc->IsModified () && !(pSekaijuDoc->m_bSaveLocked)) {
				// 自動保存するべき時が来たならば
				time_t tmCurTime;
				time (&tmCurTime);
				if (tmCurTime - pSekaijuDoc->m_tmLastAutoSaveTime > m_theAutoSaveDlgStatus.m_lInterval &&
					(m_theAutoSaveDlgStatus.m_nDisableWhilePlaying & m_bPlaying) == 0 &&
					(m_theAutoSaveDlgStatus.m_nDisableWhileRecording & m_bRecording) == 0) {
					// 現状を自動保存
					struct tm* pLocalTime = localtime (&tmCurTime);
					CString strFileName1;
					CString strFileName2;
					CString strFileName3;
					VERIFY (strFileName1.LoadString (IDS_AUTOSAVEFILEFORMAT));
					strFileName2.Format (
						strFileName1,
						pLocalTime->tm_year + 1900,
						pLocalTime->tm_mon + 1,
						pLocalTime->tm_mday,
						pLocalTime->tm_hour,
						pLocalTime->tm_min,
						pLocalTime->tm_sec,
						(ULONG)pSekaijuDoc);
					strFileName3 = m_strExeFilePath + strFileName2;
					long lRet = pSekaijuDoc->OnSaveDocument (strFileName3);
					// 自動保存失敗
					if (lRet == FALSE) {
						_RPTF1 (_CRT_WARN, "AutoSaveFile Failed. - \"%s\"\n", strFileName3);
					}
					// 自動保存成功
					else {
						_RPTF1 (_CRT_WARN, "AutoSaveFile Successful. - \"%s\"\n", strFileName3);
						// 前回自動保存したファイルを削除(保存成功時で前回保存したファイルがある場合のみ)
						if (pSekaijuDoc->m_tmLastAutoSaveTime != 0) {
							pLocalTime = localtime (&(pSekaijuDoc->m_tmLastAutoSaveTime));
							VERIFY (strFileName1.LoadString (IDS_AUTOSAVEFILEFORMAT));
							strFileName2.Format (
								strFileName1,
								pLocalTime->tm_year + 1900,
								pLocalTime->tm_mon + 1,
								pLocalTime->tm_mday,
								pLocalTime->tm_hour,
								pLocalTime->tm_min,
								pLocalTime->tm_sec,
								(ULONG)pSekaijuDoc);
							strFileName3 = m_strExeFilePath + strFileName2;
							BOOL bRet = _tremove (strFileName3);
							if (bRet == FALSE) {
								_RPTF1 (_CRT_WARN, "AutoSaveFile Delete Failed. - \"%s\"\n", strFileName3);
							}
							else {
								_RPTF1 (_CRT_WARN, "AutoSaveFile Delete Successful. - \"%s\"\n", strFileName3);				
							}
						}
						// 前回保存した時刻を更新
						pSekaijuDoc->m_tmLastAutoSaveTime = tmCurTime;
						pSekaijuDoc->SetModifiedFlag (TRUE);
					}
				}
			}
		}
	}
	return bRet;
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// リソースDLLの読み込み
BOOL CSekaijuApp::LoadResourceDLL () {
	if (m_strLanguage == _T("English")) {
		m_hResourceDLL = ::LoadLibrary (_T("SekaijuEnu.dll"));
		if (m_hResourceDLL == NULL) {
			::MessageBox (NULL, _T("SekaijuEnu.dll Load failed!!"), _T("Sekaiju"), MB_ICONEXCLAMATION);
			return FALSE;
		}
	}
	else {
		m_hResourceDLL = ::LoadLibrary (_T("SekaijuJpn.dll"));
		if (m_hResourceDLL == NULL) {
			::MessageBox (NULL, _T("SekaijuJpn.dll Load failed!!"), _T("Sekaiju"), MB_ICONEXCLAMATION);
			return FALSE;
		}
	}
	AfxSetResourceHandle (m_hResourceDLL);
	return TRUE;
}

// リソースDLLの開放
BOOL CSekaijuApp::FreeResourceDLL () {
	if (m_hResourceDLL) {
		::FreeLibrary (m_hResourceDLL);
	}
	return TRUE;
}

// 二重起動の防止(起動不能の場合FALSEを返す)
BOOL CSekaijuApp::CheckMultiExec (CCommandLineInfo* pCmdInfo) {
	// 二重起動が許可されていないならば
	if (!m_theGeneralOption.m_bEnableMultiExec) {
		HWND hFindWnd = ::FindWindow (AfxGetAppName (), NULL);
		// 二重起動であるならば
		if (hFindWnd) {
			::SendMessage (hFindWnd, WM_COMMANDWAKEUP, NULL, NULL);
			// ファイル名が指定されている場合、
			// 既に起動している方のアプリにファイル名を渡す。
			if (pCmdInfo->m_strFileName != _T("")) {
				HANDLE hShare = CreateFileMapping
					 (INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, SHMSIZE, AfxGetAppName ());
				if (hShare == NULL) {
					// 共有メモリ(送信側)オープンエラー
					CString strMsg;
					VERIFY (strMsg.LoadString (IDS_SHAREMEMORY_FOR_SEND_OPEN_ERROR));
					AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
					return FALSE;
				}
				TCHAR* pShareMem = 
					(TCHAR*)::MapViewOfFile (hShare, FILE_MAP_WRITE, 0, 0, SHMSIZE);
				if (pShareMem == NULL) {
					// 共有メモリ(送信側)マッピングエラー
					::CloseHandle (hShare);
					CString strMsg;
					VERIFY (strMsg.LoadString (IDS_SHAREMEMORY_FOR_SEND_MAPPING_ERROR));
					AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
					return FALSE;
				}
				memset (pShareMem, 0, SHMSIZE);
				TCSNCPY (pShareMem, (LPCTSTR)(pCmdInfo->m_strFileName), SHMSIZE / sizeof(TCHAR) - 1);
				::UnmapViewOfFile (pShareMem);
				::SendMessage (hFindWnd, WM_COMMANDREADSHM, NULL, NULL);
				::CloseHandle (hShare);
				::PostMessage (hFindWnd, WM_COMMANDFILEOPEN, NULL, NULL);
			}
			return FALSE; 
			// TODO:MFCのヴァージョンによっては、InitInstance内で
			// return FALSEをするとメッセージが出てしまうという説がある。
		}
		// 二重起動でない
		else {
			return TRUE;
		}
	}
	// 二重起動が許可されている
	else {
		return TRUE;
	}
}

// INIファイルから設定を読み込み
BOOL CSekaijuApp::LoadIniFile () {
	long i;
	TCHAR szTemp[256];
	// [MIDIInDevice]
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		m_strMIDIInName[i] = GetProfileString (_T("MIDIInDevice"), szTemp, _T(""));
	}
	// [MIDIOutDevice]
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		m_strMIDIOutName[i] = GetProfileString (_T("MIDIOutDevice"), szTemp, _T(""));
	}
	if (m_strMIDIOutName[0] == _T("")) {
		m_strMIDIOutName[0] = _T("MIDI Mapper");
	}
	// [MIDIInstDefNorm]
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		m_strMIDIInstDefNormName[i] = GetProfileString (_T("MIDIInstDefNorm"), szTemp, 
			i == 0 ? _T("Microsoft GS Wavetable Synth") : _T(""));
	}
	// [MIDIInstDefDrum]
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		m_strMIDIInstDefDrumName[i] = GetProfileString (_T("MIDIInstDefDrum"), szTemp,
			i == 0 ? _T("Microsoft GS Wavetable Synth Drumsets") : _T(""));
	}
	// [MIDIInSyncMode]
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		m_lMIDIInSyncMode[i] = GetProfileInt (_T("MIDIInSyncMode"), szTemp, i == 0 ? 1 : 0);
	}
	// [MIDIOutSyncMode]
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		m_lMIDIOutSyncMode[i] = GetProfileInt (_T("MIDIOutSyncMode"), szTemp, i == 0 ? 1 : 0);
	}

	// [WindowPlacement]
	m_theWindowPlacement.m_bIconic = (BOOL)GetProfileInt (_T("WindowPlacement"), _T("Iconic"), FALSE);
	m_theWindowPlacement.m_bZoomed = (BOOL)GetProfileInt (_T("WindowPlacement"), _T("Zoomed"), FALSE);
	m_theWindowPlacement.m_nX = GetProfileInt (_T("WindowPlacement"), _T("X"), CW_USEDEFAULT);
	m_theWindowPlacement.m_nY = GetProfileInt (_T("WindowPlacement"), _T("Y"), CW_USEDEFAULT);
	m_theWindowPlacement.m_nWidth = GetProfileInt (_T("WindowPlacement"), _T("Width"), CW_USEDEFAULT);
	m_theWindowPlacement.m_nHeight =GetProfileInt (_T("WindowPlacement"), _T("Height"), CW_USEDEFAULT);

	// [GeneralOption]
	m_theGeneralOption.m_bEnableMultiExec = (BOOL)GetProfileInt (_T("GeneralOption"), _T("EnableMultiExec"), FALSE);
	m_theGeneralOption.m_bEnableMultiOpen = (BOOL)GetProfileInt (_T("GeneralOption"), _T("EnableMultiOpen"), TRUE);
	m_theGeneralOption.m_bRestoreWindowPlacement = (BOOL)GetProfileInt (_T("GeneralOption"), _T("RestoreWindowPlacement"), TRUE);
	m_theGeneralOption.m_bExecOpen = (BOOL)GetProfileInt (_T("GeneralOption"), _T("ExecOpen"), FALSE);
	m_theGeneralOption.m_bOpenPlay = (BOOL)GetProfileInt (_T("GeneralOption"), _T("OpenPlay"), FALSE);
	m_theGeneralOption.m_bPlayUpdate = (BOOL)GetProfileInt (_T("GeneralOption"), _T("PlayUpdate"), TRUE);
	m_theGeneralOption.m_bSearchUpdate = (BOOL)GetProfileInt (_T("GeneralOption"), _T("SearchUpdate"), TRUE);
	m_theGeneralOption.m_bSearchSysx = (BOOL)GetProfileInt (_T("GeneralOption"), _T("SearchSysx"), TRUE);
	m_theGeneralOption.m_bEnableCC111Loop = (BOOL)GetProfileInt (_T("GeneralOption"), _T("EnableCC111Loop"), TRUE);
	m_theGeneralOption.m_bPatchSearch = (BOOL)GetProfileInt (_T("GeneralOption"), _T("PatchSearch"), TRUE);
	m_theGeneralOption.m_bInvertCtrlMouseWheel = (BOOL)GetProfileInt (_T("GeneralOption"), _T("InvertCtrlMouseWheel"), FALSE);
	m_theGeneralOption.m_bTrackZeroOrigin = (BOOL)GetProfileInt (_T("GeneralOption"), _T("TrackZeroOrigin"), FALSE);
	m_theGeneralOption.m_bEventZeroOrigin = (BOOL)GetProfileInt (_T("GeneralOption"), _T("EventZeroOrigin"), FALSE);
	m_theGeneralOption.m_bEnableAutoPageUpdate = (BOOL)GetProfileInt (_T("GeneralOption"), _T("EnableAutoPageUpdate"), TRUE);
	m_theGeneralOption.m_bSendNoteOffHoldOffAtEnd = (BOOL)GetProfileInt (_T("GeneralOption"), _T("SendNoteOffHoldOffAtEnd"), TRUE);
	m_theGeneralOption.m_lUpDownDelta1 = GetProfileInt (_T("GeneralOption"), _T("UpDownDelta1"), 1);
	m_theGeneralOption.m_lUpDownDelta2 = GetProfileInt (_T("GeneralOption"), _T("UpDownDelta2"), 10);
	m_theGeneralOption.m_lKeyVelocity1 = GetProfileInt (_T("GeneralOption"), _T("KeyVelocity1"), 80);
	m_theGeneralOption.m_lKeyVelocity2 = GetProfileInt (_T("GeneralOption"), _T("KeyVelocity2"), 120);
	m_theGeneralOption.m_lSpeedSlow = GetProfileInt (_T("GeneralOption"), _T("SpeedSlow"), 5000);
	m_theGeneralOption.m_lSpeedNormal = GetProfileInt (_T("GeneralOption"), _T("SpeedNormal"), 10000);
	m_theGeneralOption.m_lSpeedFast = GetProfileInt (_T("GeneralOption"), _T("SpeedFast"), 20000);
	m_theGeneralOption.m_lPlayRecordInterval = GetProfileInt (_T("GeneralOption"), _T("PlayRecordInterval"), 5);
	m_theGeneralOption.m_lOctaveSignature = GetProfileInt (_T("GeneralOption"), _T("OctaveSignature"), 3);

	// [TrackListOption1]
	m_theTrackListOption1.m_lDefRowZoom = GetProfileInt (_T("TrackListOption1"), _T("DefRowZoom"), 24);
	m_theTrackListOption1.m_lDefColumnZoom = GetProfileInt (_T("TrackListOption1"), _T("DefColumnZoom"), 6);
	m_theTrackListOption1.m_lDefTimeZoom = GetProfileInt (_T("TrackListOption1"), _T("DefTimeZoom"), 6);
	m_theTrackListOption1.m_lDefNameWidth = GetProfileInt (_T("TrackListOption1"), _T("DefNameWidth"), 12);
	m_theTrackListOption1.m_lDefColorWidth = GetProfileInt (_T("TrackListOption1"), _T("DefColorWidth"), 2);
	m_theTrackListOption1.m_lDefInputOnWidth = GetProfileInt (_T("TrackListOption1"), _T("DefInputOnWidth"), 4);
	m_theTrackListOption1.m_lDefInputPortWidth = GetProfileInt (_T("TrackListOption1"), _T("DefInputPortWidth"), 16);
	m_theTrackListOption1.m_lDefInputChWidth = GetProfileInt (_T("TrackListOption1"), _T("DefInputChWidth"), 5);
	m_theTrackListOption1.m_lDefOutputOnWidth = GetProfileInt (_T("TrackListOption1"), _T("DefOutputOnWidth"), 4);
	m_theTrackListOption1.m_lDefOutputPortWidth = GetProfileInt (_T("TrackListOption1"), _T("DefOutputPortWidth"), 16);
	m_theTrackListOption1.m_lDefOutputChWidth = GetProfileInt (_T("TrackListOption1"), _T("DefOutputChWidth"), 5);
	m_theTrackListOption1.m_lDefViewModeWidth = GetProfileInt (_T("TrackListOption1"), _T("DefViewModeWidth"), 5);

	// [TrackListOption2]
	m_theTrackListOption2.m_lDefCC000Width = GetProfileInt (_T("TrackListOption2"), _T("DefCC000Width"), 5);
	m_theTrackListOption2.m_lDefCC032Width = GetProfileInt (_T("TrackListOption2"), _T("DefCC032Width"), 5);
	m_theTrackListOption2.m_lDefPCWidth = GetProfileInt (_T("TrackListOption2"), _T("DefPCWidth"), 16);
	m_theTrackListOption2.m_lDefCC007Width = GetProfileInt (_T("TrackListOption2"), _T("DefCC007Width"), 5);
	m_theTrackListOption2.m_lDefCC010Width = GetProfileInt (_T("TrackListOption2"), _T("DefCC010Width"), 5);
	m_theTrackListOption2.m_lDefCC091Width = GetProfileInt (_T("TrackListOption2"), _T("DefCC091Width"), 5);
	m_theTrackListOption2.m_lDefCC093Width = GetProfileInt (_T("TrackListOption2"), _T("DefCC093Width"), 5);
	m_theTrackListOption2.m_lDefCC094Width = GetProfileInt (_T("(TrackListOption2"), _T("DefCC094Width"), 5);
	m_theTrackListOption2.m_lDefKeyShiftWidth = GetProfileInt (_T("TrackListOption2"), _T("DefKeyShiftWidth"), 5);
	m_theTrackListOption2.m_lDefVelShiftWidth = GetProfileInt (_T("TrackListOption2"), _T("DefVelShiftWidth"), 5);
	m_theTrackListOption2.m_lDefTimeShiftWidth = GetProfileInt (_T("TrackListOption2"), _T("DefTimeShiftWidth"), 5);
	m_theTrackListOption2.m_lDefNumEventWidth = GetProfileInt (_T("TrackListOption2"), _T("DefNumEventWidth"), 8);

	// [PianoRollOption]
	m_thePianoRollOption.m_lDefKeyZoom = GetProfileInt (_T("PianoRollOption"), _T("DefKeyZoom"), 6);
	m_thePianoRollOption.m_lDefVelZoom = GetProfileInt (_T("PianoRollOption"), _T("DefVelZoom"), 1);
	m_thePianoRollOption.m_lDefTimeZoom = GetProfileInt (_T("PianoRollOption"), _T("DefTimeZoom"), 6);

	// [EventListOption]
	m_theEventListOption.m_lDefRowZoom = GetProfileInt (_T("EventListOption"), _T("DefRowZoom"), 20);
	m_theEventListOption.m_lDefColumnZoom = GetProfileInt (_T("EventListOption"), _T("DefColumnZoom"), 6);
	m_theEventListOption.m_lDefTrackWidth = GetProfileInt (_T("EventListOption"), _T("DefTrackWidth"), 16);
	m_theEventListOption.m_lDefMillisecWidth = GetProfileInt (_T("EventListOption"), _T("DefMillisecWidth"), 16);
	m_theEventListOption.m_lDefTimeWidth = GetProfileInt (_T("EventListOption"), _T("DefTimeWidth"), 16);
	m_theEventListOption.m_lDefKindWidth = GetProfileInt (_T("EventListOption"), _T("DefKindWidth"), 16);
	m_theEventListOption.m_lDefChWidth = GetProfileInt (_T("EventListOption"), _T("DefChWidth"), 8);
	m_theEventListOption.m_lDefVal1Width = GetProfileInt (_T("EventListOption"), _T("DefVal1Width"), 16);
	m_theEventListOption.m_lDefVal2Width = GetProfileInt (_T("EventListOption"), _T("DefVal2Width"), 16);
	m_theEventListOption.m_lDefVal3Width = GetProfileInt (_T("EventListOption"), _T("DefVal3Width"), 16);
	m_theEventListOption.m_bInsertEventAfter = GetProfileInt (_T("EventListOption"), _T("InsertEventAfter"), 0);
	m_theEventListOption.m_bDuplicateEventAfter = GetProfileInt (_T("EventListOption"), _T("DuplicateEventAfter"), 0);
	m_theEventListOption.m_bDeleteEventAfter = GetProfileInt (_T("EventListOption"), _T("DeleteEventAfter"), 1);

	// [MusicalScoreOption]
	m_theMusicalScoreOption.m_lDefTrackZoom = GetProfileInt (_T("MusicalScoreOption"), _T("DefTrackZoom"), 4);
	m_theMusicalScoreOption.m_lDefTimeZoom = GetProfileInt (_T("MusicalScoreOption"), _T("DefTimeZoom"), 8);

	// [Control]
	m_bPlaying = FALSE;
	m_bRecording = FALSE;
	m_lCurSpeedIndex = GetProfileInt (_T("Control"), _T("CurSpeedIndex"), 2);
	m_bAutoRepeat = GetProfileInt (_T("Control"), _T("AutoRepeat"), TRUE);

	// [EditTrackDlgStatus]
	m_theEditTrackDlgStatus.m_nAmount = GetProfileInt (_T("EditTrackDlgStatus"), _T("Amount"), 0);
	m_theEditTrackDlgStatus.m_nUnit = GetProfileInt (_T("EditTrackDlgStatus"), _T("Unit"), 0);

	// [EditTimeDlgStatus]
	m_theEditTimeDlgStatus.m_nAmount = GetProfileInt (_T("EditTimeDlgStatus"), _T("Amount"), 0);
	m_theEditTimeDlgStatus.m_nUnit = GetProfileInt (_T("EditTimeDlgStatus"), _T("Unit"), 0);

	// [EditTimeSmpDlgStatus]
	m_theEditTimeSmpDlgStatus.m_nAmount = GetProfileInt (_T("EditTimeSmpDlgStatus"), _T("Amount"), 0);
	m_theEditTimeSmpDlgStatus.m_nUnit = GetProfileInt (_T("EditTimeSmpDlgStatus"), _T("Unit"), 0);

	// [EditChannelDlgStatus]
	m_theEditChannelDlgStatus.m_nAmount = GetProfileInt (_T("EditChannelDlgStatus"), _T("Amount"), 0);
	m_theEditChannelDlgStatus.m_nUnit = GetProfileInt (_T("EditChannelDlgStatus"), _T("Unit"), 0);

	// [EditKeyDlgStatus]
	m_theEditKeyDlgStatus.m_nAmount = GetProfileInt (_T("EditKeyDlgStatus"), _T("Amount"), 0);
	m_theEditKeyDlgStatus.m_nUnit = GetProfileInt (_T("EditKeyDlgStatus"), _T("Unit"), 0);
	m_theEditKeyDlgStatus.m_nTargetNote = GetProfileInt (_T("EditKeyDlgStatus"), _T("TargetNote"), 1);
	m_theEditKeyDlgStatus.m_nTargetKeyAfter = GetProfileInt (_T("EditKeyDlgStatus"), _T("TargetKeyAfter"), 1);
	
	// [EditVelocityDlgStatus]
	m_theEditVelocityDlgStatus.m_nAmount = GetProfileInt (_T("EditVelocityDlgStatus"), _T("Amount"), 0);
	m_theEditVelocityDlgStatus.m_nUnit = GetProfileInt (_T("EditVelocityDlgStatus"), _T("Unit"), 0);
	m_theEditVelocityDlgStatus.m_nTargetNoteOn = GetProfileInt (_T("EditVelocityDlgStatus"), _T("TargetNoteOn"), 1);
	m_theEditVelocityDlgStatus.m_nTargetNoteOff = GetProfileInt (_T("EditVelocityDlgStatus"), _T("TargetNoteOff"), 1);

	// [EditDurationDlgStatus]
	m_theEditDurationDlgStatus.m_nAmount = GetProfileInt (_T("EditDurationDlgStatus"), _T("Amount"), 0);
	m_theEditDurationDlgStatus.m_nUnit = GetProfileInt (_T("EditDurationDlgStatus"), _T("Unit"), 0);

	// [EditValueDlgStatus]
	m_theEditValueDlgStatus.m_nAmount = GetProfileInt (_T("EditValueDlgStatus"), _T("Amount"), 0);
	m_theEditValueDlgStatus.m_nUnit = GetProfileInt (_T("EditValueDlgStatus"), _T("Unit"), 0);
	m_theEditValueDlgStatus.m_nTargetKeyAfter = GetProfileInt (_T("EditValueDlgStatus"), _T("TargetKeyAfter"), 1);
	m_theEditValueDlgStatus.m_nTargetControlChange = GetProfileInt (_T("EditValueDlgStatus"), _T("ControlChange"), 1);
	m_theEditValueDlgStatus.m_nTargetChannelAfter = GetProfileInt (_T("EditValueDlgStatus"), _T("ChannelAfter"), 1);
	m_theEditValueDlgStatus.m_nTargetPitchBend = GetProfileInt (_T("EditValueDlgStatus"), _T("PitchBend"), 1);

	// [EditBreakupAndTrillDlgStatus]
	m_theEditBreakupAndTrillDlgStatus.m_nDurationIndex = GetProfileInt (_T("EditBreakupAndTrillDlgStatus"), _T("DurationIndex"), 3);
	m_theEditBreakupAndTrillDlgStatus.m_nEnableTrill = GetProfileInt (_T("EditBreakupAndTrillDlgStatus"), _T("EnableTrill"), 1);
	m_theEditBreakupAndTrillDlgStatus.m_nKeyShift = GetProfileInt (_T("EditBreakupAndTrillDlgStatus"), _T("KeyShift"), 2);

	// [EditQuantizeDlgStatus]
	m_theEditQuantizeDlgStatus.m_nSnapTimeIndex = GetProfileInt (_T("EditQuantizeDlgStatus"), _T("SnapTimeIndex"), 3);
	m_theEditQuantizeDlgStatus.m_nStrength = GetProfileInt (_T("EditQuantizeDlgStatus"), _T("Strength"), 100);
	m_theEditQuantizeDlgStatus.m_nTargetNoteOn = GetProfileInt (_T("EditQuantizeDlgStatus"), _T("TargetNoteOn"), 1);
	m_theEditQuantizeDlgStatus.m_nTargetNoteOff = GetProfileInt (_T("EditQuantizeyDlgStatus"), _T("TargetNoteOff"), 0);

	// [EditBeatScanDlgStatus]
	m_theEditBeatScanDlgStatus.m_nBeatTrackIndex = GetProfileInt (_T("EditBeatScanDlgStatus"), _T("BeatTrackIndex"), 1);
	m_theEditBeatScanDlgStatus.m_nBeatIntervalIndex = GetProfileInt (_T("EditBeatScanDlgStatus"), _T("BeatIntervalIndex"), 0);
	m_theEditBeatScanDlgStatus.m_nInsertTempo = GetProfileInt (_T("EditBeatScanDlgStatus"), _T("InsertTempo"), 1);

	// [EditInsertMeasureDlgStatus]
	m_theEditInsertMeasureDlgStatus.m_nPosition = GetProfileInt (_T("EditInsertMeasureDlgStatus"), _T("Position"), 1);
	m_theEditInsertMeasureDlgStatus.m_nNumMeasure = GetProfileInt (_T("EditInsertMeasureDlgStatus"), _T("NumMeasure"), 1);

	// [EditRemoveMeasureDlgStatus]
	m_theEditRemoveMeasureDlgStatus.m_nPosition = GetProfileInt (_T("EditRemoveMeasureDlgStatus"), _T("Position"), 1);
	m_theEditRemoveMeasureDlgStatus.m_nNumMeasure = GetProfileInt (_T("EditRemoveMeasureDlgStatus"), _T("NumMeasure"), 1);

	// [MetronomeDlgStatus]
	m_theMetronomeDlgStatus.m_nOn = GetProfileInt (_T("MetronomeDlgStatus"), _T("On"), 1);
	m_theMetronomeDlgStatus.m_nOutputPort = GetProfileInt (_T("MetronomeDlgStatus"), _T("OutputPort"), 0);
	m_theMetronomeDlgStatus.m_nOutputChannel = GetProfileInt (_T("MetronomeDlgStatus"), _T("OutputChannel"), 9);
	m_theMetronomeDlgStatus.m_nNoteKey1 = GetProfileInt (_T("MetronomeDlgStatus"), _T("NoteKey1"), 60);
	m_theMetronomeDlgStatus.m_nNoteVel1 = GetProfileInt (_T("MetronomeDlgStatus"), _T("NoteVel1"), 120);
	m_theMetronomeDlgStatus.m_nNoteKey2 = GetProfileInt (_T("MetronomeDlgStatus"), _T("NoteKey2"), 61);
	m_theMetronomeDlgStatus.m_nNoteVel2 = GetProfileInt (_T("MetronomeDlgStatus"), _T("NoteVel2"), 120);

	// [AutoSaveDlgStatus]
	m_theAutoSaveDlgStatus.m_nOn = GetProfileInt (_T("AutoSaveDlgStatus"), _T("On"), 1);
	m_theAutoSaveDlgStatus.m_lInterval = GetProfileInt (_T("AutoSaveDlgStatus"), _T("Interval"), 600);
	m_theAutoSaveDlgStatus.m_nDisableWhilePlaying = GetProfileInt (_T("AutoSaveDlgStatus"), _T("DisableWhilePlaying"), 1);
	m_theAutoSaveDlgStatus.m_nDisableWhileRecording = GetProfileInt (_T("AutoSaveDlgStatus"),_T( "DisableWhileRecording"), 1);


	// [LastOpenFileName]
	m_strLastOpenFileName[0] = GetProfileString (_T("LastOpenFileName"), _T("File1"), _T(""));

	// [Language]
	m_strLanguage = GetProfileString (_T("Language"), _T("Language"), _T("Japanese"));
	return TRUE;
}

// INIファイルに設定を保存
BOOL CSekaijuApp::SaveIniFile () {
	long i;
	TCHAR szTemp[256];
	// [MIDIInDevice]
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		WriteProfileString (_T("MIDIInDevice"), szTemp, m_strMIDIInName[i]);
	}
	// [MIDIOutDevice]
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		WriteProfileString (_T("MIDIOutDevice"), szTemp, m_strMIDIOutName[i]);
	}
	// [MIDIInstDefNorm]
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		WriteProfileString (_T("MIDIInstDefNorm"), szTemp, m_strMIDIInstDefNormName[i]);
	}
	// [MIDIInstDefDrum]
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		WriteProfileString (_T("MIDIInstDefDrum"), szTemp, m_strMIDIInstDefDrumName[i]);
	}
	// [MIDIInSyncMode]
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		WriteProfileInt (_T("MIDIInSyncMode"), szTemp, m_lMIDIInSyncMode[i]);
	}
	// [MIDIOutSyncMode]
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		WriteProfileInt (_T("MIDIOutSyncMode"), szTemp, m_lMIDIOutSyncMode[i]);
	}

	// [WindowPlacement]
	WriteProfileInt (_T("WindowPlacement"), _T("Iconic"),  m_theWindowPlacement.m_bIconic);
	WriteProfileInt (_T("WindowPlacement"), _T("Zoomed"),  m_theWindowPlacement.m_bZoomed);
	WriteProfileInt (_T("WindowPlacement"), _T("X"),  m_theWindowPlacement.m_nX);
	WriteProfileInt (_T("WindowPlacement"), _T("Y"),  m_theWindowPlacement.m_nY);
	WriteProfileInt (_T("WindowPlacement"), _T("Width"),  m_theWindowPlacement.m_nWidth);
	WriteProfileInt (_T("WindowPlacement"), _T("Height"),  m_theWindowPlacement.m_nHeight);

	// [GeneralOption]
	WriteProfileInt (_T("GeneralOption"), _T("EnableMultiExec"),  m_theGeneralOption.m_bEnableMultiExec);
	WriteProfileInt (_T("GeneralOption"), _T("EnableMultiOpen"), m_theGeneralOption.m_bEnableMultiOpen);
	WriteProfileInt (_T("GeneralOption"), _T("RestoreWindowPlacement"),  m_theGeneralOption.m_bRestoreWindowPlacement);
	WriteProfileInt (_T("GeneralOption"), _T("ExecOpen"),  m_theGeneralOption.m_bExecOpen);
	WriteProfileInt (_T("GeneralOption"), _T("OpenPlay"),  m_theGeneralOption.m_bOpenPlay);
	WriteProfileInt (_T("GeneralOption"), _T("PlayUpdate"), m_theGeneralOption.m_bPlayUpdate);
	WriteProfileInt (_T("GeneralOption"), _T("SearchUpdate"), m_theGeneralOption.m_bSearchUpdate);
	WriteProfileInt (_T("GeneralOption"), _T("SearchSysx"), m_theGeneralOption.m_bSearchSysx);
	WriteProfileInt (_T("GeneralOption"), _T("EnableCC111Loop"), m_theGeneralOption.m_bEnableCC111Loop);
	WriteProfileInt (_T("GeneralOption"), _T("PatchSearch"), m_theGeneralOption.m_bPatchSearch);
	WriteProfileInt (_T("GeneralOption"), _T("InvertCtrlMouseWheel"), m_theGeneralOption.m_bInvertCtrlMouseWheel);
	WriteProfileInt (_T("GeneralOption"), _T("TrackZeroOrigin"), m_theGeneralOption.m_bTrackZeroOrigin);
	WriteProfileInt (_T("GeneralOption"), _T("EventZeroOrigin"), m_theGeneralOption.m_bEventZeroOrigin);
	WriteProfileInt (_T("GeneralOption"), _T("EnableAutoPageUpdate"), m_theGeneralOption.m_bEnableAutoPageUpdate);
	WriteProfileInt (_T("GeneralOption"), _T("SendNoteOffHoldOffAtEnd"), m_theGeneralOption.m_bSendNoteOffHoldOffAtEnd);
	WriteProfileInt (_T("GeneralOption"), _T("UpDownDelta1"), m_theGeneralOption.m_lUpDownDelta1);
	WriteProfileInt (_T("GeneralOption"), _T("UpDownDelta2"), m_theGeneralOption.m_lUpDownDelta2);
	WriteProfileInt (_T("GeneralOption"), _T("KeyVelocity1"), m_theGeneralOption.m_lKeyVelocity1);
	WriteProfileInt (_T("GeneralOption"), _T("KeyVelocity2"), m_theGeneralOption.m_lKeyVelocity2);
	WriteProfileInt (_T("GeneralOption"), _T("SpeedSlow"), m_theGeneralOption.m_lSpeedSlow);
	WriteProfileInt (_T("GeneralOption"), _T("SpeedNormal"), m_theGeneralOption.m_lSpeedNormal);
	WriteProfileInt (_T("GeneralOption"), _T("SpeedFast"), m_theGeneralOption.m_lSpeedFast);
	WriteProfileInt (_T("GeneralOption"), _T("PlayRecordInterval"), m_theGeneralOption.m_lPlayRecordInterval);
	WriteProfileInt (_T("GeneralOption"), _T("OctaveSignature"), m_theGeneralOption.m_lOctaveSignature);

	// [TrackListOption1]
	WriteProfileInt (_T("TrackListOption1"), _T("DefRowZoom"), m_theTrackListOption1.m_lDefRowZoom);
	WriteProfileInt (_T("TrackListOption1"), _T("DefColumnZoom"), m_theTrackListOption1.m_lDefColumnZoom);
	WriteProfileInt (_T("TrackListOption1"), _T("DefTimeZoom"), m_theTrackListOption1.m_lDefTimeZoom);
	WriteProfileInt (_T("TrackListOption1"), _T("DefNameWidth"), m_theTrackListOption1.m_lDefNameWidth);
	WriteProfileInt (_T("TrackListOption1"), _T("DefColorWidth"), m_theTrackListOption1.m_lDefColorWidth);
	WriteProfileInt (_T("TrackListOption1"), _T("DefInputOnWidth"), m_theTrackListOption1.m_lDefInputOnWidth);
	WriteProfileInt (_T("TrackListOption1"), _T("DefInputPortWidth"), m_theTrackListOption1.m_lDefInputPortWidth);
	WriteProfileInt (_T("TrackListOption1"), _T("DefInputChWidth"), m_theTrackListOption1.m_lDefInputChWidth);
	WriteProfileInt (_T("TrackListOption1"), _T("DefOutputOnWidth"), m_theTrackListOption1.m_lDefOutputOnWidth);
	WriteProfileInt (_T("TrackListOption1"), _T("DefOutputPortWidth"), m_theTrackListOption1.m_lDefOutputPortWidth);
	WriteProfileInt (_T("TrackListOption1"), _T("DefOutputChWidth"), m_theTrackListOption1.m_lDefOutputChWidth);
	WriteProfileInt (_T("TrackListOption1"), _T("DefViewModeWidth"), m_theTrackListOption1.m_lDefViewModeWidth);

	// [TrackListOption2]
	WriteProfileInt (_T("TrackListOption2"), _T("DefCC000Width"), m_theTrackListOption2.m_lDefCC000Width);
	WriteProfileInt (_T("TrackListOption2"), _T("DefCC032Width"), m_theTrackListOption2.m_lDefCC032Width);
	WriteProfileInt (_T("TrackListOption2"), _T("DefPCWidth"), m_theTrackListOption2.m_lDefPCWidth);
	WriteProfileInt (_T("TrackListOption2"), _T("DefCC007Width"), m_theTrackListOption2.m_lDefCC007Width);
	WriteProfileInt (_T("TrackListOption2"), _T("DefCC010Width"), m_theTrackListOption2.m_lDefCC010Width);
	WriteProfileInt (_T("TrackListOption2"), _T("DefCC091Width"), m_theTrackListOption2.m_lDefCC091Width);
	WriteProfileInt (_T("TrackListOption2"), _T("DefCC093Width"), m_theTrackListOption2.m_lDefCC093Width);
	WriteProfileInt (_T("TrackListOption2"), _T("DefCC094Width"), m_theTrackListOption2.m_lDefCC094Width);
	WriteProfileInt (_T("TrackListOption2"), _T("DefKeyShiftWidth"), m_theTrackListOption2.m_lDefKeyShiftWidth);
	WriteProfileInt (_T("TrackListOption2"), _T("DefVelShiftWidth"), m_theTrackListOption2.m_lDefVelShiftWidth);
	WriteProfileInt (_T("TrackListOption2"), _T("DefTimeShiftWidth"), m_theTrackListOption2.m_lDefTimeShiftWidth);
	WriteProfileInt (_T("TrackListOption2"), _T("DefNumEventWidth"), m_theTrackListOption2.m_lDefNumEventWidth);

	// [PianoRollOption]
	WriteProfileInt (_T("PianoRollOption"), _T("DefKeyZoom"), m_thePianoRollOption.m_lDefKeyZoom);
	WriteProfileInt (_T("PianoRollOption"), _T("DefVelZoom"), m_thePianoRollOption.m_lDefVelZoom);
	WriteProfileInt (_T("PianoRollOption"), _T("DefTimeZoom"), m_thePianoRollOption.m_lDefTimeZoom);

	// [EventListOption]
	WriteProfileInt (_T("EventListOption"), _T("DefRowZoom"), m_theEventListOption.m_lDefRowZoom);
	WriteProfileInt (_T("EventListOption"), _T("DefColumnZoom"), m_theEventListOption.m_lDefColumnZoom);
	WriteProfileInt (_T("EventListOption"), _T("DefTrackWidth"), m_theEventListOption.m_lDefTrackWidth);
	WriteProfileInt (_T("EventListOption"), _T("DefMillisecWidth"), m_theEventListOption.m_lDefMillisecWidth);
	WriteProfileInt (_T("EventListOption"), _T("DefTimeWidth"), m_theEventListOption.m_lDefTimeWidth);
	WriteProfileInt (_T("EventListOption"), _T("DefKindWidth"), m_theEventListOption.m_lDefKindWidth);
	WriteProfileInt (_T("EventListOption"), _T("DefChWidth"), m_theEventListOption.m_lDefChWidth);
	WriteProfileInt (_T("EventListOption"), _T("DefVal1Width"), m_theEventListOption.m_lDefVal1Width);
	WriteProfileInt (_T("EventListOption"), _T("DefVal2Width"), m_theEventListOption.m_lDefVal2Width);
	WriteProfileInt (_T("EventListOption"), _T("DefVal3Width"), m_theEventListOption.m_lDefVal3Width);
	WriteProfileInt (_T("EventListOption"), _T("InsertEventAfter"), m_theEventListOption.m_bInsertEventAfter);
	WriteProfileInt (_T("EventListOption"), _T("DuplicateEventAfter"), m_theEventListOption.m_bDuplicateEventAfter);
	WriteProfileInt (_T("EventListOption"), _T("DeleteEventAfter"), m_theEventListOption.m_bDeleteEventAfter);
	
	// [MusicalScoreOption]
	WriteProfileInt (_T("MusicalScoreOption"), _T("DefTrackZoom"), m_theMusicalScoreOption.m_lDefTrackZoom);
	WriteProfileInt (_T("MusicalScoreOption"), _T("DefTimeZoom"), m_theMusicalScoreOption.m_lDefTimeZoom);

	// [Control]
	WriteProfileInt (_T("Control"), _T("Playing"),m_bPlaying);
	WriteProfileInt (_T("Control"), _T("Recording"), m_bRecording);
	WriteProfileInt (_T("Control"), _T("CurSpeedIndex"), m_lCurSpeedIndex);
	WriteProfileInt (_T("Control"), _T("AutoRepeat"), m_bAutoRepeat);
	
	// [EditTrackDlgStatus]
	WriteProfileInt (_T("EditTrackDlgStatus"), _T("Amount"), m_theEditTrackDlgStatus.m_nAmount);
	WriteProfileInt (_T("EditTrackDlgStatus"), _T("Unit"), m_theEditTrackDlgStatus.m_nUnit);
	
	// [EditTimeDlgStatus]
	WriteProfileInt (_T("EditTimeDlgStatus"), _T("Amount"), m_theEditTimeDlgStatus.m_nAmount);
	WriteProfileInt (_T("EditTimeDlgStatus"), _T("Unit"), m_theEditTimeDlgStatus.m_nUnit);

	// [EditTimeSmpDlgStatus]
	WriteProfileInt (_T("EditTimeSmpDlgStatus"), _T("Amount"), m_theEditTimeSmpDlgStatus.m_nAmount);
	WriteProfileInt (_T("EditTimeSmpDlgStatus"), _T("Unit"), m_theEditTimeSmpDlgStatus.m_nUnit);

	// [EditChannelDlgStatus]
	WriteProfileInt (_T("EditChannelDlgStatus"), _T("Amount"), m_theEditChannelDlgStatus.m_nAmount);
	WriteProfileInt (_T("EditChannelDlgStatus"), _T("Unit"), m_theEditChannelDlgStatus.m_nUnit);

	// [EditKeyDlgStatus]
	WriteProfileInt (_T("EditKeyDlgStatus"), _T("Amount"), m_theEditKeyDlgStatus.m_nAmount);
	WriteProfileInt (_T("EditKeyDlgStatus"), _T("Unit"), m_theEditKeyDlgStatus.m_nUnit);
	WriteProfileInt (_T("EditKeyDlgStatus"), _T("TargetNote"), m_theEditKeyDlgStatus.m_nTargetNote);
	WriteProfileInt (_T("EditKeyDlgStatus"), _T("TargetKeyAfter"), m_theEditKeyDlgStatus.m_nTargetKeyAfter);
	
	// [EditVelocityDlgStatus]
	WriteProfileInt (_T("EditVelocityDlgStatus"), _T("Amount"), m_theEditVelocityDlgStatus.m_nAmount);
	WriteProfileInt (_T("EditVelocityDlgStatus"), _T("Unit"), m_theEditVelocityDlgStatus.m_nUnit);
	WriteProfileInt (_T("EditVelocityDlgStatus"), _T("TargetNoteOn"), m_theEditVelocityDlgStatus.m_nTargetNoteOn);
	WriteProfileInt (_T("EditVelocityDlgStatus"), _T("TargetNoteOff"), m_theEditVelocityDlgStatus.m_nTargetNoteOff);

	// [EditDurationDlgStatus]
	WriteProfileInt (_T("EditDurationDlgStatus"), _T("Amount"), m_theEditDurationDlgStatus.m_nAmount);
	WriteProfileInt (_T("EditDurationDlgStatus"), _T("Unit"), m_theEditDurationDlgStatus.m_nUnit);

	// [EditValueDlgStatus]
	WriteProfileInt (_T("EditValueDlgStatus"), _T("Amount"), m_theEditValueDlgStatus.m_nAmount);
	WriteProfileInt (_T("EditValueDlgStatus"), _T("Unit"), m_theEditValueDlgStatus.m_nUnit);
	WriteProfileInt (_T("EditValueDlgStatus"), _T("TargetKeyAfter"), m_theEditValueDlgStatus.m_nTargetKeyAfter);
	WriteProfileInt (_T("EditValueDlgStatus"), _T("ControlChange"), m_theEditValueDlgStatus.m_nTargetControlChange);
	WriteProfileInt (_T("EditValueDlgStatus"), _T("ChannelAfter"), m_theEditValueDlgStatus.m_nTargetChannelAfter);
	WriteProfileInt (_T("EditValueDlgStatus"), _T("PitchBend"), m_theEditValueDlgStatus.m_nTargetPitchBend);
	
	// [EditBreakupAndTrillDlgStatus]
	WriteProfileInt (_T("EditBreakupAndTrillDlgStatus"), _T("DurationIndex"), m_theEditBreakupAndTrillDlgStatus.m_nDurationIndex);
	WriteProfileInt (_T("EditBreakupAndTrillDlgStatus"), _T("EnableTrill"), m_theEditBreakupAndTrillDlgStatus.m_nEnableTrill);
	WriteProfileInt (_T("EditBreakupAndTrillDlgStatus"), _T("KeyShift"), m_theEditBreakupAndTrillDlgStatus.m_nKeyShift);

	// [EditQuantizeDlgStatus]
	WriteProfileInt (_T("EditQuantizeDlgStatus"), _T("SnapTimeIndex"), m_theEditQuantizeDlgStatus.m_nSnapTimeIndex);
	WriteProfileInt (_T("EditQuantizeDlgStatus"), _T("Strength"), m_theEditQuantizeDlgStatus.m_nStrength);
	WriteProfileInt (_T("EditQuantizeDlgStatus"), _T("TargetNoteOn"), m_theEditQuantizeDlgStatus.m_nTargetNoteOn);
	WriteProfileInt (_T("EditQuantizeDlgStatus"), _T("TargetNoteOff"), m_theEditQuantizeDlgStatus.m_nTargetNoteOff);

	// [EditBeatScanDlgStatus]
	WriteProfileInt (_T("EditBeatScanDlgStatus"), _T("BeatTrackIndex"), m_theEditBeatScanDlgStatus.m_nBeatTrackIndex);
	WriteProfileInt (_T("EditBeatScanDlgStatus"), _T("BeatIntervalIndex"), m_theEditBeatScanDlgStatus.m_nBeatIntervalIndex);
	WriteProfileInt (_T("EditBeatScanDlgStatus"), _T("InsertTempo"), m_theEditBeatScanDlgStatus.m_nInsertTempo);

	// [EditInsertMeasureDlgStatus]
	WriteProfileInt (_T("EditInsertMeasureDlgStatus"), _T("Position"), m_theEditInsertMeasureDlgStatus.m_nPosition);
	WriteProfileInt (_T("EditInsertMeasureDlgStatus"), _T("NumMeasure"), m_theEditInsertMeasureDlgStatus.m_nNumMeasure);

	// [EditRemoveMeasureDlgStatus]
	WriteProfileInt (_T("EditRemoveMeasureDlgStatus"), _T("Position"), m_theEditRemoveMeasureDlgStatus.m_nPosition);
	WriteProfileInt (_T("EditRemoveMeasureDlgStatus"), _T("NumMeasure"), m_theEditRemoveMeasureDlgStatus.m_nNumMeasure);

	// [MetronomeDlgStatus]
	WriteProfileInt (_T("MetronomeDlgStatus"), _T("On"), m_theMetronomeDlgStatus.m_nOn);
	WriteProfileInt (_T("MetronomeDlgStatus"), _T("OutputPort"), m_theMetronomeDlgStatus.m_nOutputPort);
	WriteProfileInt (_T("MetronomeDlgStatus"), _T("OutputChannel"), m_theMetronomeDlgStatus.m_nOutputChannel);
	WriteProfileInt (_T("MetronomeDlgStatus"), _T("NoteKey1"), m_theMetronomeDlgStatus.m_nNoteKey1);
	WriteProfileInt (_T("MetronomeDlgStatus"), _T("NoteVel1"), m_theMetronomeDlgStatus.m_nNoteVel1);
	WriteProfileInt (_T("MetronomeDlgStatus"), _T("NoteKey2"), m_theMetronomeDlgStatus.m_nNoteKey2);
	WriteProfileInt (_T("MetronomeDlgStatus"), _T("NoteVel2"), m_theMetronomeDlgStatus.m_nNoteVel2);
	
	// [AutoSaveDlgStatus]
	WriteProfileInt (_T("AutoSaveDlgStatus"), _T("On"), m_theAutoSaveDlgStatus.m_nOn);
	WriteProfileInt (_T("AutoSaveDlgStatus"), _T("Interval"), m_theAutoSaveDlgStatus.m_lInterval);
	WriteProfileInt (_T("AutoSaveDlgStatus"), _T("DisableWhilePlaying"), m_theAutoSaveDlgStatus.m_nDisableWhilePlaying);
	WriteProfileInt (_T("AutoSaveDlgStatus"), _T("DisableWhileRecording"), m_theAutoSaveDlgStatus.m_nDisableWhileRecording);

	// [LastOpenFileName]
	WriteProfileString (_T("LastOpenFileName"), _T("File1"), m_strLastOpenFileName[0]);

	// [Language]
	WriteProfileString (_T("Language"), _T("Language"), m_strLanguage);

	return TRUE;
}


// すべてのMIDIインストゥルメント定義ファイル(*.ins)を読み込む
long CSekaijuApp::LoadAllMIDIInstrument () {
	long i = 0;
	// 既にMIDIインストゥルメントが読み込まれいている場合は削除
	for (i = 0; i < MAXMIDIINSTRUMENTNUM; i++) {
		if (m_pMIDIInstrument[i]) {
			MIDIInstrument_Delete (m_pMIDIInstrument[i]);
			m_pMIDIInstrument[i] = NULL;
		}
	}
	// 空のMIDIインストゥルメントを生成 // 20100220廃止
	//m_pMIDIInstrument = MIDIInstrument_Create ();
	//if (m_pMIDIInstrument == NULL) {
	//	//_RPTF0 (_CRT_WARN, "エラー：OnCreate内でMIDIインストゥルメントを読み込めません。\n");
	//	//MessageBox (g_hMainWnd, MIDIINSTRUMENT_CANT_LOAD, APPNAME, MB_ICONSTOP);
	//	return 0;
	//}
	// 拡張子(*.ins)のファイルを全て読み込む
	i = 0;
	CString strFileName1;
	CString strFileName2;
	VERIFY (strFileName1.LoadString (IDS_INSTRUMENTFILENAME));
	strFileName2 = m_strExeFilePath + strFileName1;
	HANDLE hFind = NULL;
	WIN32_FIND_DATA fd;
	hFind = ::FindFirstFile ((LPCTSTR)strFileName2, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (_tcscmp (fd.cFileName, _T(".")) != 0 &&
				_tcscmp (fd.cFileName, _T("..")) != 0) {
				CString strFileName3;
				CString strFileName4;
				VERIFY (strFileName3.LoadString (IDS_INSTRUMENTDIRNAME));
				strFileName4 = m_strExeFilePath + strFileName3 + fd.cFileName;
				m_pMIDIInstrument[i] = MIDIInstrument_Load ((LPCTSTR)strFileName4); // 20100220変更
				if (m_pMIDIInstrument[i] == NULL) {
					_RPTF1 (_CRT_WARN, "MIDIInstDefFile Load Failed. - \"%s\"\n", strFileName4);
				}
				else {
					_RPTF1 (_CRT_WARN, "MIDIInstDefFile Load Successful. - \"%s\"\n", strFileName4);
				}
				i++;
			}
		} while (::FindNextFile (hFind, &fd) && i < MAXMIDIINSTRUMENTNUM); // 20100220条件追加
		::FindClose (hFind);
	}
	return i;
}


long CSekaijuApp::SelectAllMIDIInstDefNorm () {
	long lPort = 0;
	long j = 0;
	CString strMsg;
	CString strMsg2;
	for (lPort = 0; lPort < MAXMIDIINDEVICENUM; lPort++) {
		// インストゥルメント定義名称よりインストゥルメント定義へのポインタを取得
		for (j = 0; j < MAXMIDIINSTRUMENTNUM; j++) {
			if (m_pMIDIInstrument[j] != NULL) {
				m_pMIDIInstDefNorm[lPort] = MIDIInstrument_GetInstrumentDefinitionFromTitle 
					(m_pMIDIInstrument[j], m_strMIDIInstDefNormName[lPort]);
				if (m_pMIDIInstDefNorm[lPort] != NULL) {
					break;
				}
			}
		}
		// インストゥルメント定義が見つかった
		if (m_pMIDIInstDefNorm[lPort] != NULL) {
			_RPTF2 (_CRT_WARN, "MIDIInstDef[%d] Find Successful. - \"%s\"\n", 
				lPort, m_strMIDIInstDefNormName[lPort]);
		}
		// インストゥルメント定義が見つからなかった
		else {
			_RPTF2 (_CRT_WARN, "MIDIInstDef[%d] Find Failed. - \"%s\"\n", 
				lPort, m_strMIDIInstDefNormName[lPort]);
			if (m_strMIDIInstDefNormName[lPort] != _T("")) { // 20080829修正
				// %s\n-MIDIインストゥルメント定義[%d]が見つかりません。
				strMsg.LoadString (IDS_S_N_MIDIINSTDEF_D_FIND_FAILED);
				strMsg2.Format (strMsg, m_strMIDIInstDefNormName[lPort], lPort + 1);
				AfxMessageBox (strMsg2, MB_OK | MB_ICONEXCLAMATION);
			}
			m_strMIDIInstDefNormName[lPort] = _T("");
		}
	}
	return 1;
}

long CSekaijuApp::SelectAllMIDIInstDefDrum () {
	long lPort = 0;
	long j = 0;
	CString strMsg;
	CString strMsg2;
	for (lPort = 0; lPort < MAXMIDIINDEVICENUM; lPort++) {
		// インストゥルメント定義名称よりインストゥルメント定義へのポインタを取得
		for (j = 0; j < MAXMIDIINSTRUMENTNUM; j++) {
			if (m_pMIDIInstrument[j] != NULL) {
				m_pMIDIInstDefDrum[lPort] = MIDIInstrument_GetInstrumentDefinitionFromTitle 
					(m_pMIDIInstrument[j], m_strMIDIInstDefDrumName[lPort]);
				if (m_pMIDIInstDefDrum[lPort] != NULL) {
					break;
				}
			}
		}
		// インストゥルメント定義が見つかった
		if (m_pMIDIInstDefDrum[lPort] != NULL) {
			_RPTF2 (_CRT_WARN, "MIDIInstDef[%d] Find Successful. - \"%s\"\n", 
				lPort, m_strMIDIInstDefDrumName[lPort]);
		}
		// インストゥルメント定義が見つからなかった
		else {
			_RPTF2 (_CRT_WARN, "MIDIInstDef[%d] Find Failed. - \"%s\"\n", 
				lPort, m_strMIDIInstDefDrumName[lPort]);
			if (m_strMIDIInstDefDrumName[lPort] != _T("")) { // 20080829修正
				// %s\n-MIDIインストゥルメント定義[%d]が見つかりません。
				strMsg.LoadString (IDS_S_N_MIDIINSTDEF_D_FIND_FAILED);
				strMsg2.Format (strMsg, m_strMIDIInstDefDrumName[lPort], lPort + 1);
				AfxMessageBox (strMsg2, MB_OK | MB_ICONEXCLAMATION);
			}
			m_strMIDIInstDefDrumName[lPort] = _T("");
		}
	}
	return 1;
}

//-----------------------------------------------------------------------------
// オペレーション
//-----------------------------------------------------------------------------

// メニューの更新
void CSekaijuApp::UpdateMenu () {
	CMenu* pMenu = AfxGetMainWnd ()->GetMenu ();
	CString str1;
	CString str2;
	str1.LoadString (IDS_CONTROL_SPEEDSLOW);
	str2.Format (str1, m_theGeneralOption.m_lSpeedSlow / 100);
	pMenu->ModifyMenu (ID_CONTROL_SPEEDSLOW, MF_BYCOMMAND, ID_CONTROL_SPEEDSLOW, (LPCTSTR)str2);
	str1.LoadString (IDS_CONTROL_SPEEDNORMAL);
	str2.Format (str1, m_theGeneralOption.m_lSpeedNormal / 100);
	pMenu->ModifyMenu (ID_CONTROL_SPEEDNORMAL, MF_BYCOMMAND, ID_CONTROL_SPEEDNORMAL, (LPCTSTR)str2);
	str1.LoadString (IDS_CONTROL_SPEEDFAST);
	str2.Format (str1, m_theGeneralOption.m_lSpeedFast / 100);
	pMenu->ModifyMenu (ID_CONTROL_SPEEDFAST, MF_BYCOMMAND, ID_CONTROL_SPEEDFAST, (LPCTSTR)str2);
}

// 現在のウィンドウと現在のドキュメントへのポインタを更新
void CSekaijuApp::UpdateCurWndAndDocPtr () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	// カレント子ウィンドウの更新
	m_pCurChildWnd = (CMDIChildWnd*)(pMainFrame->GetActiveFrame ());
	// 以前のドキュメントとカレントドキュメントの更新
	if (m_pCurChildWnd) {
		m_pOldDocument = m_pCurDocument;
		m_pCurDocument = m_pCurChildWnd->GetActiveDocument ();
	}
	else {
		m_pOldDocument = m_pCurDocument;
		m_pCurDocument = NULL;
	}
	// カレントドキュメントが変更された場合
	if (m_pCurDocument != m_pOldDocument) {
		CSekaijuDoc* pOldSekaijuDoc = (CSekaijuDoc*)m_pOldDocument;
		CSekaijuDoc* pCurSekaijuDoc = (CSekaijuDoc*)m_pCurDocument;
		// ドキュメントがある場合クリティカルセクションロック
		if (pOldSekaijuDoc) {
			pOldSekaijuDoc->m_theCriticalSection.Lock ();
		}
		if (pCurSekaijuDoc) {
			pCurSekaijuDoc->m_theCriticalSection.Lock ();
		}
		this->m_theCriticalSection.Lock ();
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
		//CScrollBar* pPositionScroll = &(pMainFrame->m_wndPositionScroll);
		// 演奏用ドキュメント・記録用ドキュメントの変更処理
		// 古いドキュメントがある場合
		if (pOldSekaijuDoc) {
			// 録音中の場合は強制停止(20080709追加)(20081003修正)
			if (m_bRecording) {
				// 注意：録音中に突然閉じられた場合pCurHistoryUnit取得不可。
				// 注意：これはPostNcDestroyから呼び出される。CSekaijuDoc::OnCloseDocumentより後。
				// 注意：よってCSekaijuDoc::OnCloseDocumentでStopRecordingしておくこと。
				StopRecording (pOldSekaijuDoc);
			}
			// 旧ドキュメントのMIDIクロックの強制停止
			if (pOldSekaijuDoc->m_pMIDIClock) {
				MIDIClock_Stop (pOldSekaijuDoc->m_pMIDIClock);
			}
			// 発音の強制停止
			SendAllNoteOff ();
			SendAllHold1Off ();
			SendAllSostenutoOff ();
			SendAllHold2Off ();
			SendAllSoundOff ();
			// 新しいドキュメントがある場合
			if (pCurSekaijuDoc) {
				// ポジションスクロールバーの範囲設定
				long lEndTime = MIDIData_GetEndTime (pCurSekaijuDoc->m_pMIDIData);
				pMainFrame->SetPositionScrollRange (0, lEndTime, TRUE);
				// MIDIクロックのsスピードと同期モードをアプリケーションに合わせる(20090630廃止)
				// pCurSekaijuDoc->ApplyAppCurSpeedIndex ();
				// 演奏中又は録音中の場合は、MIDIStatusを更新する。
				if (m_bPlaying || m_bRecording) {
					ResetTempMIDIStatusArray ();
					pCurSekaijuDoc->TimeMIDIStatus 
						(pCurSekaijuDoc->m_lOldTime, m_pTempMIDIStatus);
					long lFlags = m_bPlaying ? SDS_ALL : SDS_ALL & ~SDS_NOTE;
					m_bIgnoreNoteEvent = TRUE; // 20090625追加
					SendDifferentStatus (lFlags);
				}
				// 演奏中の場合は、新ドキュメントの演奏を開始する。
				// 録音中の場合は、新ドキュメントの録音を開始する。
				if (m_bPlaying || m_bRecording) {
					MIDIClock_Start (pCurSekaijuDoc->m_pMIDIClock);
				}
			}
			// 新しいドキュメントがない場合
			else {
				m_bPlaying = FALSE;
				m_bRecording = FALSE;
			}
		}
		this->m_theCriticalSection.Unlock ();
		// ドキュメントがある場合クリティカルセクション解放
		if (m_pOldDocument) {
			((CSekaijuDoc*)m_pOldDocument)->m_theCriticalSection.Unlock ();
		}
		if (m_pCurDocument) {
			((CSekaijuDoc*)m_pCurDocument)->m_theCriticalSection.Unlock ();
		}
	}
	_RPTF2 (_CRT_WARN, 
		"CMainFrame::UpdateCurWndAndDocPtr : m_pCurDocument=%p, m_pOldDocument=%p.\n",
		m_pCurDocument, m_pOldDocument);
}

// MIDI同期入力ポート番号と同期入力モードを取得する(なければFALSEを返す)
BOOL CSekaijuApp::GetCurSyncInputPortAndMode (long* pPort, long* pMode) {
	long lSyncPort = 0;
	long lSyncMode = 0;
	for (lSyncPort = 0; lSyncPort < MAXMIDIINDEVICENUM; lSyncPort++) {
		if (m_lMIDIInSyncMode[lSyncPort] == 1 || m_lMIDIInSyncMode[lSyncPort] == 2) {
			*pPort = lSyncPort;
			*pMode = m_lMIDIInSyncMode[lSyncPort];
			return TRUE;
		}
	}
	*pPort = 0;
	*pMode = 0;
	return FALSE;
}



// PlayRecordThread-演奏・記録用スレッド 
UINT CSekaijuApp::PlayRecordThread (LPVOID pInfo) {
	// 注意:静的なメンバ関数はthisポインタにアクセスすることができない。
	CString strMsg;
	strMsg = ("CSekaijuApp::PlayRecordThread Started.\n");
	_RPTF0 (_CRT_WARN, strMsg);
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)pInfo;
	while (pSekaijuApp->m_bPlayRecordThreadRunning != FALSE) {
		pSekaijuApp->PlayRecordProc (pInfo);
		::Sleep (pSekaijuApp->m_theGeneralOption.m_lPlayRecordInterval);
	}
	strMsg = ("CSekaijuApp::PlayRecordThread Finished.\n");
	_RPTF0 (_CRT_WARN, strMsg);
	return 1;
}


// PlayRecordProc(約5ミリ秒おきにCSekaijuApp::PlayRecordThreadから呼び出される。)
BOOL CSekaijuApp::PlayRecordProc (LPVOID pInfo) {
	BYTE byMsg[1024];
	long lLen;
	long i, j;
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)pInfo;
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)m_pCurDocument;
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if (pSekaijuDoc == NULL || pSekaijuApp == NULL) {
		return FALSE;
	}
	if (pSekaijuDoc->m_pMIDIData == NULL || pSekaijuDoc->m_pMIDIClock == NULL) {
		return FALSE;
	}
	// クリティカルセクションロック
	pSekaijuDoc->m_theCriticalSection.Lock ();
	pSekaijuApp->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDIClock* pMIDIClock = pSekaijuDoc->m_pMIDIClock;
	if (pMIDIData == NULL || pMIDIClock == NULL) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		pSekaijuApp->m_theCriticalSection.Unlock ();
		return FALSE;
	}
	long lCurTime = 0;
	long lCurMillisec = 0;
	lCurTime = MIDIClock_GetTickCount (pMIDIClock);
	lCurMillisec = MIDIClock_GetMillisec (pMIDIClock);
	pSekaijuDoc->m_lNewTime = lCurTime;
	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);

	// データの入力処理(記録用)
	// 各MIDI入力デバイスについて
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		// MIDI入力デバイスが有効ならば
		if (m_pMIDIIn[i] != NULL) {
			long lMIDIInSyncMode = pSekaijuApp->m_lMIDIInSyncMode[i];
			// MIDIメッセージの取得できる限り
			while (1) {
				// MIDIメッセージの取得
				memset (byMsg, 0, sizeof (byMsg));
				lLen = MIDIIn_GetMIDIMessage (m_pMIDIIn[i], byMsg, sizeof (byMsg));
				if (lLen <= 0) {
					break;
				}
				// アクティブセンシングの場合
				if (byMsg[0] == 0xFE) {
					;
				}
				// MIDIタイムコードクォーターフレームの場合
				else if (byMsg[0] == 0xF1 || byMsg[0] == 0xF0) {
					if (lMIDIInSyncMode >= 2) {
						if (pMIDIClock) {
							MIDIClock_PutMIDIMessage (pMIDIClock, byMsg, lLen);
						}
					}
				}
				// MIDIタイミングクロックの場合
				else if (byMsg[0] == 0xF8) {
					if (lMIDIInSyncMode == 1) {
						if (pMIDIClock) {
							MIDIClock_PutMIDIMessage (pMIDIClock, byMsg, lLen);
						}
					}
				}
				// ソングポジションセレクタの場合
				else if (byMsg[0] == 0xF2) {
					if (lMIDIInSyncMode == 1) {
						long lVal = ((byMsg[2] & 0x7F) << 7) | (byMsg[1] & 0x7F);
						long lTickCount = lVal * lTimeResolution / 4;
						SetPlayPosition (pSekaijuDoc, lTickCount);
					}
				}
				// スタートの場合
				else if (byMsg[0] == 0xFA && pSekaijuDoc) {
					if (pMIDIClock && !m_bPlaying && !m_bRecording) {
						pMainFrame->PostMessage (WM_COMMAND, ID_CONTROL_TOBEGIN, 0);
						pMainFrame->PostMessage (WM_COMMAND, ID_CONTROL_PLAY, 0);
					}
					else if (pMIDIClock && m_bPlaying && !m_bRecording) { // 20090704追加
						pMainFrame->PostMessage (WM_COMMAND, ID_CONTROL_TOBEGIN, 0);
						pMainFrame->PostMessage (WM_COMMAND, ID_CONTROL_PLAY, 0);
					}
				}
				// コンティニューの場合
				else if (byMsg[0] == 0xFB && pSekaijuDoc) {
					if (pSekaijuDoc->m_pMIDIClock && !m_bPlaying && !m_bRecording) {
						pMainFrame->PostMessage (WM_COMMAND, ID_CONTROL_PLAY, 0);
					}
				}
				// ストップの場合
				else if (byMsg[0] == 0xFC && pSekaijuDoc) {
					if (pSekaijuDoc->m_pMIDIClock && (m_bPlaying || m_bRecording)) {
						pMainFrame->PostMessage (WM_COMMAND, ID_CONTROL_PLAY, 0);
					}
				}
				// MIDIチャンネルメッセージの場合
				else if (0x80 <= byMsg[0] && byMsg[0] <= 0xEF && pSekaijuDoc) {
					// シンクロスタート処理
					if (m_bRecording && m_lCurSpeedIndex == 0) {
						m_lCurSpeedIndex = m_lOldSpeedIndex;
						MIDIClock_Stop (pMIDIClock);
						pSekaijuDoc->ApplyAppCurSpeedIndex ();
						MIDIClock_Start (pMIDIClock);
					}
					// リアルタイム入力処理
					MIDITrack* pMIDITrack = NULL;
					forEachTrack (pSekaijuDoc->m_pMIDIData, pMIDITrack) {
						long lInputOn = MIDITrack_GetInputOn (pMIDITrack);
						long lInputPort = MIDITrack_GetInputPort (pMIDITrack);
						long lInputChannel = MIDITrack_GetInputChannel (pMIDITrack);
						long lOutputOn = MIDITrack_GetOutputOn (pMIDITrack);
						long lOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
						long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
						if (lInputOn && lInputPort == i && lInputChannel == (byMsg[0] & 0x0F)) {
							// 出力チャンネルの変換
							BYTE byMsg2[sizeof(byMsg)];
							memset (byMsg2, 0, sizeof (byMsg));
							memcpy (byMsg2, byMsg, sizeof (byMsg));
							if (0 <= lOutputChannel && lOutputChannel < 16) {
								byMsg2[0] &= 0xF0;
								byMsg2[0] |= (BYTE)lOutputChannel;
							}
							// 音出し処理(演奏中でなくかつ録音中でない場合のみ)
							if (!(m_bPlaying && m_bRecording)) {
								MIDIOut* pMIDIOut = NULL;
								if (0 <= lOutputPort && lOutputPort < MAXMIDIOUTDEVICENUM) {
									pMIDIOut = m_pMIDIOut[lOutputPort];
								}
								if (pMIDIOut) {
									//TODO:MIDIメッセージがループバックする場合下記を無効にしてください。
									MIDIOut_PutMIDIMessage (pMIDIOut, byMsg2, lLen);
								}
							}
							// このMIDIメッセージをMIDIデータに記録
							if (m_bRecording) {
								MIDIEvent* pMIDIEvent = MIDIEvent_Create
									(pSekaijuDoc->m_lNewTime, (byMsg[0] & 0xF0), byMsg2, lLen);
								if (pMIDIEvent) {
									//pMIDIEvent->m_lUserFlag |= MIDIEVENT_REALTIMEGENERATE;
									MIDITrack_InsertEvent (pMIDITrack, pMIDIEvent);
									// ノートオフの場合のノートオンとの結合処理
									if (MIDIEvent_IsNoteOff (pMIDIEvent)) {
										long lTempRecordedEventCount = m_theTempRecordedEventArray.GetSize ();
										long j;
										for (j = lTempRecordedEventCount - 1; j >= 0; j--) {
											MIDIEvent* pTempEvent = 
												(MIDIEvent*)(m_theTempRecordedEventArray.GetAt (j));
											if (MIDIEvent_IsNoteOn (pTempEvent) &&
												pTempEvent->m_pNextCombinedEvent == NULL) {
												if (MIDIEvent_GetChannel (pTempEvent) == 
													MIDIEvent_GetChannel (pMIDIEvent)) {
													if (MIDIEvent_GetKey (pTempEvent) ==
														MIDIEvent_GetKey (pMIDIEvent)) {
														pTempEvent->m_pNextCombinedEvent = pMIDIEvent;
														pMIDIEvent->m_pPrevCombinedEvent = pTempEvent;
													}
												}
											}
										}
										//MIDIEvent_Combine (pMIDIEvent);
									}
									m_theTempRecordedEventArray.Add (pMIDIEvent);
								}
							}
						}
					}
				}
				// システムエクスクルーシブメッセージの場合
				else if (byMsg[0] == 0xF0 || byMsg[0] == 0xF7) {
					// シンクロスタート処理
					if (m_bRecording && m_lCurSpeedIndex == 0) {
						m_lCurSpeedIndex = m_lOldSpeedIndex;
						MIDIClock_Stop (pMIDIClock);
						pSekaijuDoc->ApplyAppCurSpeedIndex ();
						MIDIClock_Start (pMIDIClock);
					}
					// リアルタイム入力処理
					MIDITrack* pMIDITrack = NULL;
					forEachTrack (pSekaijuDoc->m_pMIDIData, pMIDITrack) {
						long lInputOn = MIDITrack_GetInputOn (pMIDITrack);
						long lInputPort = MIDITrack_GetInputPort (pMIDITrack);
						long lInputChannel = MIDITrack_GetInputChannel (pMIDITrack);
						long lOutputOn = MIDITrack_GetOutputOn (pMIDITrack);
						long lOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
						long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
						if (lInputOn && lInputPort == i && lInputChannel == -1) {
							// 音出し処理(演奏中でなくかつ録音中でない場合のみ)
							if (!(m_bPlaying && m_bRecording)) {
								MIDIOut* pMIDIOut = NULL;
								if (0 <= lOutputPort && lOutputPort < MAXMIDIOUTDEVICENUM) {
									pMIDIOut = m_pMIDIOut[lOutputPort];
								}
								if (pMIDIOut) {
									//TODO:MIDIメッセージがループバックする場合下記を無効にしてください。
									MIDIOut_PutMIDIMessage (pMIDIOut, byMsg, lLen);
								}
							}
							// このMIDIメッセージをMIDIデータに記録
							if (m_bRecording) {
								MIDITrack_InsertSysExEvent
									(pMIDITrack, pSekaijuDoc->m_lNewTime, byMsg, lLen);
							}
						}
					}
				}
			}
		}
	}

	// MIDIタイミングクロックとSMPTE/MTCの送出処理(20090624追加)
	if (m_bPlaying) {
		long lOutputPort;
		// MIDIタイミングクロック数計算(20090620追加)
		long lNumMIDITimingClock = 0;
		if (lTimeMode == MIDIDATA_TPQNBASE) {
			long lOldClock = pSekaijuDoc->m_lOldTime * 24 / lTimeResolution;
			long lNewClock = pSekaijuDoc->m_lNewTime * 24 / lTimeResolution;
			lNumMIDITimingClock = lNewClock - lOldClock;
			lNumMIDITimingClock = MAX (0, lNumMIDITimingClock);
		}
		// MIDIタイミングクロックの作成(20090620追加)
		memset (byMsg, 0, sizeof (byMsg));
		byMsg[0] = 0xF8;
		// MIDIタイミングクロック出力処理(20090620追加)
		for (lOutputPort = 0; lOutputPort < MAXMIDIOUTDEVICENUM; lOutputPort++) {
			if (pSekaijuApp->m_lMIDIOutSyncMode[lOutputPort] == 1) {
				if (m_pMIDIOut[lOutputPort]) {
					for (j = 0; j < lNumMIDITimingClock; j++) {
						MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, 1);
					}
				}
				if (m_pMIDIStatus[lOutputPort]) {
					for (j = 0; j < lNumMIDITimingClock; j++) {
						MIDIStatus_PutMIDIMessage (m_pMIDIStatus[lOutputPort], byMsg, 1);
					}
				}
			}
		}
		// 前回の時：分：秒：フレーム格納用変数
		static long lOldSec;
		static long lOldMinute;
		static long lOldHour;
		static long lOldFrame[4];
		// 現在の時：分：秒：フレーム計算(20090624追加)
		long lMillisec = lCurMillisec % 1000;
		long lSec = (lCurMillisec / 1000) % 60;
		long lMinute = (lCurMillisec / 60000) % 60;
		long lHour = (lCurMillisec / 3600000);
		long lFrame[4] = {
			lMillisec * 24 / 1000, lMillisec * 25 / 1000, 
			lMillisec * 2997 / 100000, lMillisec * 30 / 1000}; 
		// SMPTE24,SMPTE25,SMPTE29.97,SMPTE30用のMIDIタイムコードクォーターフレームを作成
		memset (byMsg, 0, sizeof (byMsg));
		long lFrameRate;
		for (lFrameRate = 0; lFrameRate < 4; lFrameRate++) {
			byMsg[lFrameRate * 16 + 0] = 0xF1;
			byMsg[lFrameRate * 16 + 1] = 0x00 | (lFrame[lFrameRate] & 0x0F);
			byMsg[lFrameRate * 16 + 2] = 0xF1;
			byMsg[lFrameRate * 16 + 3] = 0x10 | ((lFrame[lFrameRate] >> 4) & 0x0F);
			byMsg[lFrameRate * 16 + 4] = 0xF1;
			byMsg[lFrameRate * 16 + 5] = 0x20 | (lSec & 0x0F);
			byMsg[lFrameRate * 16 + 6] = 0xF1;
			byMsg[lFrameRate * 16 + 7] = 0x30 | ((lSec >> 4) & 0x0F);
			byMsg[lFrameRate * 16 + 8] = 0xF1;
			byMsg[lFrameRate * 16 + 9] = 0x40 | (lMinute & 0x0F);
			byMsg[lFrameRate * 16 + 10] = 0xF1;
			byMsg[lFrameRate * 16 + 11] = 0x50 | ((lMinute >> 4) & 0x0F);
			byMsg[lFrameRate * 16 + 12] = 0xF1;
			byMsg[lFrameRate * 16 + 13] = 0x60 | (lHour & 0x0F);
			byMsg[lFrameRate * 16 + 14] = 0xF1;
			byMsg[lFrameRate * 16 + 15] = 0x70 | ((lFrameRate & 0x03) << 1) | ((lHour >> 4) & 0x01); 
		}
		// MIDIタイムコードクォーターフレーム出力処理(20090624追加)
		for (lOutputPort = 0; lOutputPort < MAXMIDIOUTDEVICENUM; lOutputPort++) {
			lFrameRate = pSekaijuApp->m_lMIDIOutSyncMode[lOutputPort] - 2;
			if (0 <= lFrameRate && lFrameRate <= 3) { 
				// 現在の時：分：秒：フレームが前回の時：分：秒：フレームと異なる場合のみ
				if (lFrameRate == 0 && (lFrame[0] != lOldFrame[0] || lSec != lOldSec || lMinute != lOldMinute) || 
					lFrameRate == 1 && (lFrame[1] != lOldFrame[1] || lSec != lOldSec || lMinute != lOldMinute) ||
					lFrameRate == 2 && (lFrame[2] != lOldFrame[2] || lSec != lOldSec || lMinute != lOldMinute) ||
					lFrameRate == 3 && (lFrame[3] != lOldFrame[3] || lSec != lOldSec || lMinute != lOldMinute)) {
					if (m_pMIDIOut[lOutputPort]) {
						for (j = 0; j < 8; j++) {
							long lPos = lFrameRate * 16 + j * 2;
							MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], &byMsg[lPos], 2);
						}
					}
					if (m_pMIDIStatus[lOutputPort]) {
						for (j = 0; j < 8; j++) {
							long lPos = lFrameRate * 16 + j * 2;
							MIDIStatus_PutMIDIMessage (m_pMIDIStatus[lOutputPort], &byMsg[lPos], 2);
						}
					}
				}
			}
		}
		// 前回の時：分：秒：フレームに現在の時：分：秒：フレームを格納
		lOldSec = lSec;
		lOldMinute = lMinute;
		lOldHour = lHour;
		lOldFrame[0] = lFrame[0];
		lOldFrame[1] = lFrame[1];
		lOldFrame[2] = lFrame[2];
		lOldFrame[3] = lFrame[3];
	}

	// メトロノーム出力処理(20080825追加)
	if (m_bRecording && m_theMetronomeDlgStatus.m_nOn) {
		if (lTimeMode == MIDIDATA_TPQNBASE) {
			long lOldMeasure, lOldBeat, lOldTick;
			MIDIData_BreakTime (pMIDIData, 
				pSekaijuDoc->m_lOldTime, &lOldMeasure, &lOldBeat, &lOldTick);
			long lNewMeasure, lNewBeat, lNewTick;
			MIDIData_BreakTime (pMIDIData, 
				pSekaijuDoc->m_lNewTime, &lNewMeasure, &lNewBeat, &lNewTick);
			long lOutputPort = CLIP (0, m_theMetronomeDlgStatus.m_nOutputPort, 15);
			long lOutputChannel = CLIP (0, m_theMetronomeDlgStatus.m_nOutputChannel, 15);
			// 最初の1小節目の強打を鳴らすためのトリック
			if (pSekaijuDoc->m_lOldTime == pSekaijuDoc->m_lNewTime && 
				lOldBeat == 0 && lOldTick == 0 && this->m_bFirstMetronome == TRUE) {
				lOldMeasure --;
			}
			m_bFirstMetronome = FALSE;
			long lMeasure = 0;
			for (lMeasure = lOldMeasure; lMeasure < lNewMeasure; lMeasure++) {
				memset (byMsg, 0, sizeof (byMsg));
				byMsg[0] = 0x90 | (BYTE)lOutputChannel;
				byMsg[1] = CLIP (0, m_theMetronomeDlgStatus.m_nNoteKey1, 127);
				byMsg[2] = CLIP (1, m_theMetronomeDlgStatus.m_nNoteVel1, 127);
				if (m_pMIDIOut[lOutputPort]) {
					MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, 3);
				}
				byMsg[2] = 0;
				if (m_pMIDIOut[lOutputPort]) {
					MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, 3);
				}
			}
			long lBeat = 0;
			for (lBeat = lOldBeat; lBeat < lNewBeat; lBeat++) {
				if (1) {
					memset (byMsg, 0, sizeof (byMsg));
					byMsg[0] = 0x90 | (BYTE)lOutputChannel;
					byMsg[1] = CLIP (0, m_theMetronomeDlgStatus.m_nNoteKey2, 127);
					byMsg[2] = CLIP (1, m_theMetronomeDlgStatus.m_nNoteVel2, 127);
					if (m_pMIDIOut[lOutputPort]) {
						MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, 3);
					}
					byMsg[2] = 0;
					if (m_pMIDIOut[lOutputPort]) {
						MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, 3);
					}
				}
			}
		}
	}

	// MIDIデータの出力処理(演奏用)
	if (m_bPlaying) {
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		// 各トラックについて
		forEachTrack (pSekaijuDoc->m_pMIDIData, pMIDITrack) {
			// このトラックの出力がONならば
			if (MIDITrack_GetOutputOn (pMIDITrack)) {
				long lOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
				long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
				forEachEvent (pMIDITrack, pMIDIEvent) {
					long lTime = MIDIEvent_GetTime (pMIDIEvent);
					lTime = CLIP (0, lTime + MIDITrack_GetTimePlus (pMIDITrack), 0x7FFFFFFF);
					if (pSekaijuDoc->m_lOldTime <= lTime && lTime < pSekaijuDoc->m_lNewTime) {
						// MIDIチャンネルイベントの場合
						if (MIDIEvent_IsMIDIEvent (pMIDIEvent)) {
							memset (byMsg, 0, sizeof (byMsg));
							lLen = MIDIEvent_GetLen (pMIDIEvent);
							MIDIEvent_GetData (pMIDIEvent, byMsg, lLen);
							// 出力チャンネルが設定されている場合はそのチャンネルに従う
							if (0 <= lOutputChannel && lOutputChannel < 16) {
								byMsg[0] &= 0xF0;
								byMsg[0] |= (BYTE)lOutputChannel;
							}
							// キー+とベロシティ+の反映
							if (MIDIEvent_IsNoteOn (pMIDIEvent)) {
								byMsg[1] = (BYTE)CLIP (0, byMsg[1] + MIDITrack_GetKeyPlus (pMIDITrack), 127);
								byMsg[2] = (BYTE)CLIP (1, byMsg[2] + MIDITrack_GetVelocityPlus (pMIDITrack), 127);
							}
							else if (MIDIEvent_IsNoteOff (pMIDIEvent) || MIDIEvent_IsKeyAftertouch (pMIDIEvent)) {
								byMsg[1] = (BYTE)CLIP (0, byMsg[1] + MIDITrack_GetKeyPlus (pMIDITrack), 127);
							}
							// 演奏位置を動かして発音状態を復元した直後は二重発音・二重消音を防止する
							if (m_bIgnoreNoteEvent && 
								!(MIDIEvent_IsNoteOn (pMIDIEvent) && lTime == pSekaijuDoc->m_lOldTime)  &&
								!(MIDIEvent_IsNoteOff (pMIDIEvent) && lTime == pSekaijuDoc->m_lOldTime) ||
								m_bIgnoreNoteEvent == FALSE) {
								if (m_pMIDIOut[lOutputPort]) {
									MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, lLen);
								}
								if (m_pMIDIStatus[lOutputPort]) {
									MIDIStatus_PutMIDIMessage (m_pMIDIStatus[lOutputPort], byMsg, lLen);
								}
							}
							else {
								//_RPTF3 (_CRT_WARN, "PutMIDIMessage Ignored (%02x%02x%02x...)\n", 
								//	byMsg[0], byMsg[1], byMsg[2]);
							}
						}
						// システムエクスクルーシブイベントの場合
						else if (MIDIEvent_IsSysExEvent (pMIDIEvent)) {
							memset (byMsg, 0, sizeof (byMsg));
							lLen = MIDIEvent_GetLen (pMIDIEvent);
							MIDIEvent_GetData (pMIDIEvent, byMsg, lLen);
							if (m_pMIDIOut[lOutputPort]) {
								MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, lLen);
							}
							if (m_pMIDIStatus[lOutputPort]) {
								MIDIStatus_PutMIDIMessage (m_pMIDIStatus[lOutputPort], byMsg, lLen);
							}
						}
						// テンポイベントの場合
						else if (MIDIEvent_IsTempo (pMIDIEvent)) {
							long lTempo = MIDIEvent_GetTempo (pMIDIEvent);
							if (pMIDIClock) {
								MIDIClock_SetTempo (pMIDIClock, lTempo);
							}
						}
					}
				}
			}
		}
		// 二十発音防止用のm_bIngoreNoteEventフラグのクリア
		if (pSekaijuDoc->m_lOldTime != pSekaijuDoc->m_lNewTime) {
			m_bIgnoreNoteEvent = FALSE;
		}
		// 演奏範囲タイムの更新
		pSekaijuDoc->m_lOldTime = pSekaijuDoc->m_lNewTime;
		// 録音中でない場合かつ終端に達した場合
		long lEndTime = MIDIData_GetEndTime (pSekaijuDoc->m_pMIDIData);
		if (!m_bRecording &&
			pSekaijuDoc->m_lOldTime > lEndTime) {
			// オートリピート処理
			if (m_bAutoRepeat) {
				if (m_theGeneralOption.m_bSendNoteOffHoldOffAtEnd) {
					SendAllNoteOff (); // 20081027追加
					SendAllHold1Off (); // 20081219追加
					SendAllSostenutoOff (); // 20081219追加
					SendAllHold2Off (); // 20081219追加
				}
				// CC#111の位置からループ開始する場合(RPGツクール方式) // 20081211追加
				if (m_theGeneralOption.m_bEnableCC111Loop) {
					// 最後のCC#111イベントのタイムを探索
					long lCC111Time = 0;
					long lCC111Millisec = 0;
					long lTempo = 0;
					forEachTrack (pMIDIData, pMIDITrack) {
						forEachEvent (pMIDITrack, pMIDIEvent) {
							if (MIDIEvent_IsControlChange (pMIDIEvent)) {
								if (MIDIEvent_GetNumber (pMIDIEvent) == 111) {
									long lTime = MIDIEvent_GetTime (pMIDIEvent);
									if (lTime > lCC111Time) {
										lCC111Time = lTime;
									}
								}
							}
						}
					}
					lCC111Millisec = MIDIData_TimeToMillisec (pMIDIData, lCC111Time);
					MIDIData_FindTempo (pMIDIData, lCC111Time, &lTempo);
					MIDIClock_Stop (pMIDIClock);
					MIDIClock_SetTickCount (pMIDIClock, lCC111Time);
					MIDIClock_SetMillisec (pMIDIClock, lCC111Millisec);
					if (lTempo != 0) {
						MIDIClock_SetTempo (pMIDIClock, lTempo);
					}
					pSekaijuDoc->m_lOldTime = lCC111Time;
					pSekaijuDoc->m_lNewTime = lCC111Time;
					MIDIClock_Start (pMIDIClock);
				}
				// 最初からループ開始する場合
				else {
					MIDIClock_Reset (pSekaijuDoc->m_pMIDIClock);
					pSekaijuDoc->m_lOldTime = 0;
					pSekaijuDoc->m_lNewTime = 0;
				}
			}
			// 自動停止処理
			else {
				if (m_theGeneralOption.m_bSendNoteOffHoldOffAtEnd) {
					SendAllNoteOff (); // 20081027追加
					SendAllHold1Off (); // 20081218追加
					SendAllSostenutoOff (); // 20081218追加
					SendAllHold2Off (); // 20081218追加
				}
				MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
				pSekaijuDoc->m_lOldTime = lEndTime;
				pSekaijuDoc->m_lNewTime = lEndTime;
			}
		}
	}
	
	// クリティカルセクション解放
	pSekaijuApp->m_theCriticalSection.Unlock ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	return TRUE;
}

// 全ポートにオールホールド1オフ(CC#64-0)を送信
void CSekaijuApp::SendAllHold1Off () {
	int i, j;
	BYTE byMsg[3] = {0xB0, 64, 0};
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		for (j = 0; j < 16; j++) {
			byMsg[0] = 0xB0 | j;
			if (m_pMIDIOut[i]) {
				MIDIOut_PutMIDIMessage (m_pMIDIOut[i], byMsg, 3);
			}
			if (m_pMIDIStatus[i]) {
				MIDIStatus_PutMIDIMessage (m_pMIDIStatus[i], byMsg, 3);
			}
		}
	}
}

// 全ポートにオールソステヌートオフ(CC#66-0)を送信
void CSekaijuApp::SendAllSostenutoOff () {
	int i, j;
	BYTE byMsg[3] = {0xB0, 66, 0};
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		for (j = 0; j < 16; j++) {
			byMsg[0] = 0xB0 | j;
			if (m_pMIDIOut[i]) {
				MIDIOut_PutMIDIMessage (m_pMIDIOut[i], byMsg, 3);
			}
			if (m_pMIDIStatus[i]) {
				MIDIStatus_PutMIDIMessage (m_pMIDIStatus[i], byMsg, 3);
			}
		}
	}
}


// 全ポートにオールホールド2オフ(CC#69-0)を送信
void CSekaijuApp::SendAllHold2Off () {
	int i, j;
	BYTE byMsg[3] = {0xB0, 69, 0};
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		for (j = 0; j < 16; j++) {
			byMsg[0] = 0xB0 | j;
			if (m_pMIDIOut[i]) {
				MIDIOut_PutMIDIMessage (m_pMIDIOut[i], byMsg, 3);
			}
			if (m_pMIDIStatus[i]) {
				MIDIStatus_PutMIDIMessage (m_pMIDIStatus[i], byMsg, 3);
			}
		}
	}
}

// 全ポートにオールサウンドオフ(CC#120)を送信
void CSekaijuApp::SendAllSoundOff () {
	int i, j;
	BYTE byMsg[3] = {0xB0, 120, 0};
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		for (j = 0; j < 16; j++) {
			byMsg[0] = 0xB0 | j;
			if (m_pMIDIOut[i]) {
				MIDIOut_PutMIDIMessage (m_pMIDIOut[i], byMsg, 3);
			}
			if (m_pMIDIStatus[i]) {
				MIDIStatus_PutMIDIMessage (m_pMIDIStatus[i], byMsg, 3);
			}
		}
	}
}

// 全ポートにリセットオールコントローラー(CC#121)を送信
void CSekaijuApp::SendResetAllController () {
	int i, j;
	BYTE byMsg[3] = {0xB0, 121, 0};
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		for (j = 0; j < 16; j++) {
			byMsg[0] = 0xB0 | j;
			if (m_pMIDIOut[i]) {
				MIDIOut_PutMIDIMessage (m_pMIDIOut[i], byMsg, 3);
			}
			if (m_pMIDIStatus[i]) {
				MIDIStatus_PutMIDIMessage (m_pMIDIStatus[i], byMsg, 3);
			}
		}
	}
}

// 全ポートにオールノートオフ(CC#123)を送信
void CSekaijuApp::SendAllNoteOff () {
	int i, j;
	BYTE byMsg[3] = {0xB0, 123, 0};
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		for (j = 0; j < 16; j++) {
			byMsg[0] = 0xB0 | j;
			if (m_pMIDIOut[i]) {
				MIDIOut_PutMIDIMessage (m_pMIDIOut[i], byMsg, 3);
			}
			if (m_pMIDIStatus[i]) {
				MIDIStatus_PutMIDIMessage (m_pMIDIStatus[i], byMsg, 3);
			}
		}
	}
}

// m_theMIDIStatusArrayの内容をリセット
void CSekaijuApp::ResetMIDIStatusArray () {
	for (int nPort = 0; nPort < MAXMIDIOUTDEVICENUM; nPort++) {
		if (m_pMIDIStatus[nPort]) {
			MIDIStatus_PutReset (m_pMIDIStatus[nPort]);
		}
	}
}

// m_theTempMIDIStatusArrayの内容をリセットします。
void CSekaijuApp::ResetTempMIDIStatusArray () {
	for (int nPort = 0; nPort < MAXMIDIOUTDEVICENUM; nPort++) {
		if (m_pTempMIDIStatus[nPort]) {
			MIDIStatus_PutReset (m_pTempMIDIStatus[nPort]);
		}
	}
}


// 全ポートにMIDIStatusArrayとTempMIDIStatusArrayの差分のみを送信
// lFlags : 復元するイベントの種類を示すフラグのビット和。
//          Sekaiju.hに示したSDS_系フラグを足し合わせる。
void CSekaijuApp::SendDifferentStatus (long lFlags) {
	int nPort, i, j;
	long lCurVal;
	long lTempVal;
	BYTE byCurBuf[128];
	BYTE byTempBuf[128];
	BYTE byMsg[256];
	BYTE byCC101[] = {   0,    0,    0,    0};
	BYTE byCC100[] = {   0,    1,    2,    5};
	BYTE byCC99[] = {   1,    1,    1,    1,    1,    1,    1,    1,   
		 24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24, 
		 24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24, 
		 24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24, 
		 24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24, 
		 24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24, 
		 24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24, 
		 24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24, 
		 24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24, 
		 26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 
		 26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 
		 26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 
		 26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 
		 26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 
		 26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 
		 26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 
		 26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 
		 28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28, 
		 28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28, 
		 28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28, 
		 28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28, 
		 28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28, 
		 28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28, 
		 28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28, 
		 28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28, 
		 29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29, 
		 29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29, 
		 29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29, 
		 29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29, 
		 29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29, 
		 29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29, 
		 29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29, 
		 29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29, 
		 30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30, 
		 30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30, 
		 30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30, 
		 30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30, 
		 30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30, 
		 30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30, 
		 30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30, 
		 30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30, 
		 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31, 
		 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31, 
		 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31, 
		 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31, 
		 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31, 
		 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31, 
		 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31, 
		 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31, 
		 
	};
	BYTE byCC98[] = {   8,    9,   10,   32,   33,   99,  100,  102,  
		  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
		 16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
		 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
		 48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
		 64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
		 80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
		 96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
		112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
		  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
		 16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
		 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
		 48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
		 64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
		 80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
		 96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
		112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
		  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
		 16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
		 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
		 48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
		 64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
		 80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
		 96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
		112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
		  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
		 16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
		 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
		 48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
		 64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
		 80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
		 96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
		112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
		  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
		 16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
		 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
		 48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
		 64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
		 80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
		 96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
		112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
		  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
		 16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
		 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
		 48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
		 64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
		 80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
		 96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
		112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
	};
	// 各ポート毎に
 	for (nPort = 0; nPort < MAXMIDIOUTDEVICENUM; nPort++) {
		if (m_pMIDIStatus[nPort] == NULL || m_pMIDIOut[nPort] == NULL || 
			m_pTempMIDIStatus[nPort] == NULL) {
			continue;
		}
		// マスターボリューム
		if (lFlags & SDS_MASTERVOLUME) {
			long lCurMasterVolume = MIDIStatus_GetMasterVolume (m_pMIDIStatus[nPort]);
			long lTempMasterVolume = MIDIStatus_GetMasterVolume (m_pTempMIDIStatus[nPort]);
			if (lCurMasterVolume != lTempMasterVolume) {
				// マスターボリューム {0xF0, 0x7F, 0xid, 0x04, 0x01, 0xll, 0xmm, 0xF7}
				byMsg[0] = 0xF0;
				byMsg[1] = 0x7F;
				byMsg[2] = 0x7F;
				byMsg[3] = 0x04;
				byMsg[4] = 0x01; 
				byMsg[5] = 0x00;
				byMsg[6] = (BYTE)CLIP (0, lTempMasterVolume, 127);
				byMsg[7] = 0xF7;
				MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 8);
				MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 8);
			}
		}
		// 各パート毎に
		for (i = 0; i < 16; i++) {
			MIDIPart* pMIDIPart = MIDIStatus_GetMIDIPart (m_pMIDIStatus[nPort], i);
			MIDIPart* pTempPart = MIDIStatus_GetMIDIPart (m_pTempMIDIStatus[nPort], i);
			// キーアフタータッチ
			if (lFlags & SDS_KEYAFTER) {
				MIDIPart_GetKeyAfterTouchEx (pMIDIPart, byCurBuf, 128);
				MIDIPart_GetKeyAfterTouchEx (pTempPart, byTempBuf, 128);
				for (j = 0; j < 128; j++) {
					if (byCurBuf[j] != byTempBuf[j]) {
						byMsg[0] = 0xA0 + (BYTE)i;
						byMsg[1] = (BYTE)j;
						byMsg[2] = byTempBuf[j];
						MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
						MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
					}
				}
			}
			// コントロールチェンジ
			if (lFlags & SDS_CONTROLCHANGE) {
				MIDIPart_GetControlChangeEx (pMIDIPart, byCurBuf, 128);
				MIDIPart_GetControlChangeEx (pTempPart, byTempBuf, 128);
				for (j = 0; j < 120; j++) { // 20081218 128→120に変更
					if (byCurBuf[j] != byTempBuf[j] && j != 0 && j != 32) { // 20111016パッチチェンジは除く
						byMsg[0] = 0xB0 + (BYTE)i;
						byMsg[1] = (BYTE)j;
						byMsg[2] = byTempBuf[j];
						MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
						MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
					}
				}
			}
			// RPN
			if (lFlags & SDS_RPN) {
				for (j = 0; j < sizeof (byCC101); j++) {
					lCurVal = 
						(MIDIPart_GetRPNMSB (pMIDIPart, byCC101[j], byCC100[j]) << 7) |
						(MIDIPart_GetRPNLSB (pMIDIPart, byCC101[j], byCC100[j]) & 0x7F);
					lTempVal = 
						(MIDIPart_GetRPNMSB (pTempPart, byCC101[j], byCC100[j]) << 7) |
						(MIDIPart_GetRPNLSB (pTempPart, byCC101[j], byCC100[j]) & 0x7F);
					if (lCurVal != lTempVal) {
						// CC#99=127(NRPN=NULL)(初回のみ)
						if (byCurBuf[99] != 127) {
							byCurBuf[99] = 127;
							byMsg[0] = 0xB0 + (BYTE)i;
							byMsg[1] = 99;
							byMsg[2] = 127;
							MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
							MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						}
						// CC#98=127(NRPN=NULL)(初回のみ)
						if (byCurBuf[98] != 127) {
							byCurBuf[98] = 127;
							byMsg[0] = 0xB0 + (BYTE)i;
							byMsg[1] = 98;
							byMsg[2] = 127;
							MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
							MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						}
						// CC#101
						byMsg[0] = 0xB0 + (BYTE)i;
						byMsg[1] = 101;
						byMsg[2] = byCC101[j];
						MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
						MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						// CC#100
						byMsg[0] = 0xB0 + (BYTE)i;
						byMsg[1] = 100;
						byMsg[2] = byCC100[j];
						MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
						MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						// CC#6
						if ((lCurVal >> 7) != (lTempVal >> 7)) {
							byMsg[0] = 0xB0 + (BYTE)i;
							byMsg[1] = 6;
							byMsg[2] = (BYTE)(lTempVal >> 7);
							MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
							MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						}
						// CC#38
						if ((lCurVal & 0x7F) != (lTempVal & 0x7F)) {
							byMsg[0] = 0xB0 + (BYTE)i;
							byMsg[1] = 38;
							byMsg[2] = (BYTE)(lTempVal & 0x7F);
							MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
							MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						}
					}
				}
			}
			// NRPN
			if (lFlags & SDS_NRPN) {
				for (j = 0; j < sizeof (byCC99); j++) {
					lCurVal = 
						(MIDIPart_GetRPNMSB (pMIDIPart, byCC99[j], byCC98[j]) << 7) |
						(MIDIPart_GetRPNLSB (pMIDIPart, byCC99[j], byCC98[j]) & 0x7F);
					lTempVal = 
						(MIDIPart_GetRPNMSB (pTempPart, byCC99[j], byCC98[j]) << 7) |
						(MIDIPart_GetRPNLSB (pTempPart, byCC99[j], byCC98[j]) & 0x7F);
					if (lCurVal != lTempVal) {
						// CC#101=127(RPN=NULL)(初回のみ)
						if (byCurBuf[101] != 127) {
							byCurBuf[101] = 127;
							byMsg[0] = 0xB0 + (BYTE)i;
							byMsg[1] = 101;
							byMsg[2] = 127;
							MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
							MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						}
						// CC#100=127(NRPN=NULL)(初回のみ)
						if (byCurBuf[100] != 127) {
							byCurBuf[100] = 127;
							byMsg[0] = 0xB0 + (BYTE)i;
							byMsg[1] = 100;
							byMsg[2] = 127;
							MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
							MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						}
						// CC#99
						byMsg[0] = 0xB0 + (BYTE)i;
						byMsg[1] = 99;
						byMsg[2] = byCC99[j];
						MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
						MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						// CC#98
						byMsg[0] = 0xB0 + (BYTE)i;
						byMsg[1] = 98;
						byMsg[2] = byCC98[j];
						MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
						MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						// CC#6
						if ((lCurVal >> 7) != (lTempVal >> 7)) {
							byMsg[0] = 0xB0 + (BYTE)i;
							byMsg[1] = 6;
							byMsg[2] = (BYTE)(lTempVal >> 7);
							MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
							MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						}
						// CC#38
						if ((lCurVal & 0x7F) != (lTempVal & 0x7F)) {
							byMsg[0] = 0xB0 + (BYTE)i;
							byMsg[1] = 38;
							byMsg[2] = (BYTE)(lTempVal & 0x7F);
							MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
							MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						}
					}
				}
			}
			// パッチチェンジ(20111016追加修正)
			if (lFlags & (SDS_CONTROLCHANGE | SDS_PROGRAMCHANGE)) {
				MIDIPart_GetControlChangeEx (pMIDIPart, byCurBuf, 128);
				MIDIPart_GetControlChangeEx (pTempPart, byTempBuf, 128);
				lCurVal = MIDIPart_GetProgramChange (pMIDIPart);
				lTempVal = MIDIPart_GetProgramChange (pTempPart);
				if (byCurBuf[0] != byTempBuf[0] ||
					byCurBuf[32] != byTempBuf[32] ||
					lCurVal != lTempVal) {
					byMsg[0] = 0xB0 + (BYTE)i;
					byMsg[1] = (BYTE)0;
					byMsg[2] = byTempBuf[0];
					MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
					MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
					byMsg[0] = 0xB0 + (BYTE)i;
					byMsg[1] = (BYTE)32;
					byMsg[2] = byTempBuf[32];
					MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
					MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
					byMsg[0] = 0xC0 + (BYTE)i;
					byMsg[1] = (BYTE)lTempVal;
					MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 2);
					MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 2);
				}
			}
			// チャンネルアフタータッチ
			if (lFlags & SDS_CHANNELAFTER) {
				lCurVal = MIDIPart_GetChannelAfterTouch (pMIDIPart);
				lTempVal = MIDIPart_GetChannelAfterTouch (pTempPart);
				if (lCurVal != lTempVal) {
					memset (byMsg, 0, sizeof (byMsg));
					byMsg[0] = 0xD0 + (BYTE)i;
					byMsg[1] = (BYTE)lTempVal;
					MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 2);
					MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 2);
				}
			}
			// ピッチベンド
			if (lFlags & SDS_PITCHBEND) {
				lCurVal = MIDIPart_GetPitchBend (pMIDIPart);
				lTempVal = MIDIPart_GetPitchBend (pTempPart);
				if (lCurVal != lTempVal) {
					memset (byMsg, 0, sizeof (byMsg));
					byMsg[0] = 0xE0 + (BYTE)i;
					byMsg[1] = (BYTE)(lTempVal & 0x007F);
					byMsg[2] = (BYTE)(lTempVal >> 7);
					MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
					MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
				}
			}
			// ノート
			if (lFlags & SDS_NOTE) {
				MIDIPart_GetNoteKeepEx (pMIDIPart, byCurBuf, 128);
				MIDIPart_GetNoteKeepEx (pTempPart, byTempBuf, 128);
				for (j = 0; j < 128; j++) {
					if (byCurBuf[j] != byTempBuf[j]) {
						byMsg[0] = 0x90 + (BYTE)i;
						byMsg[1] = (BYTE)j;
						byMsg[2] = byTempBuf[j];
						MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
						MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
					}
				}
			}
			//TRACE0 ("restored.\n");
		}
	}
}

// MIDI入力デバイスを開く
void CSekaijuApp::OpenAllMIDIInDevice () {
	long i;
	CString strMsg;
	CString strMsg2;
	m_theCriticalSection.Lock ();
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		m_pMIDIIn[i] = MIDIIn_Reopen (m_pMIDIIn[i], m_strMIDIInName[i]);
		if (m_pMIDIIn[i]) {
			_RPTF2 (_CRT_WARN, "MIDIInDevice[%d] Open Successful. - \"%s\"\n", 
				i, m_strMIDIInName[i]);
		}
		else {
			if (m_strMIDIInName[i] != _T("") && m_strMIDIInName[i] != MIDIIO_NONE && 
				m_strMIDIInName[i] != MIDIIO_NONEJ) {
				_RPTF2 (_CRT_WARN, "MIDIInDevice[%d] Open Failed. - \"%s\"\n", 
					i, m_strMIDIInName[i]);
				// "%s\nMIDIInデバイス[%d]が開けません。"
				strMsg.LoadString (IDS_S_N_MIDIINDEVICE_D_OPEN_FAILED);
				strMsg2.Format (strMsg, m_strMIDIInName[i], i + 1);
				m_theCriticalSection.Unlock ();
				AfxMessageBox (strMsg2, MB_OK | MB_ICONEXCLAMATION);
				m_theCriticalSection.Lock ();
			}
			m_strMIDIInName[i] = _T("");
		}
	}
	m_theCriticalSection.Unlock ();
}

// MIDI出力デバイスを開く
void CSekaijuApp::OpenAllMIDIOutDevice () {
	long i;
	CString strMsg;
	CString strMsg2;
	m_theCriticalSection.Lock ();
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		MIDIStatus_Delete (m_pMIDIStatus[i]);
		m_pMIDIStatus[i] = NULL;
		MIDIStatus_Delete (m_pTempMIDIStatus[i]);
		m_pTempMIDIStatus[i] = NULL;
		m_pMIDIOut[i] = MIDIOut_Reopen (m_pMIDIOut[i], m_strMIDIOutName[i]);
		if (m_pMIDIOut[i]) {
			_RPTF2 (_CRT_WARN, "MIDIOutDevice[%d] Open Successful. - \"%s\"\n",
				i, m_strMIDIOutName[i]);
			m_pMIDIStatus[i] = MIDIStatus_Create (MIDISTATUS_MODENATIVE, 16, 2);
			m_pTempMIDIStatus[i] = MIDIStatus_Create (MIDISTATUS_MODENATIVE, 16, 2);
		}
		else {
			if (m_strMIDIOutName[i] != _T("") && m_strMIDIOutName[i] != MIDIIO_NONE &&
				m_strMIDIOutName[i] != MIDIIO_NONEJ) {
				_RPTF2 (_CRT_WARN, "MIDIOutDevice[%d] Open Failed. - \"%s\"\n",
					i, m_strMIDIOutName[i]);
				// "%s\nMIDIInデバイス[%d]が開けません。"
				strMsg.LoadString (IDS_S_N_MIDIOUTDEVICE_D_OPEN_FAILED);
				strMsg2.Format (strMsg, m_strMIDIOutName[i], i + 1);
				m_theCriticalSection.Unlock ();
				AfxMessageBox (strMsg2, MB_OK | MB_ICONEXCLAMATION);
				m_theCriticalSection.Lock ();
			}
			m_strMIDIOutName[i] = _T("");
		}
	}
	m_theCriticalSection.Unlock ();
}

// 演奏位置指定(絶対ティック指定)
long CSekaijuApp::SetPlayPosition (CDocument* pDocument, long lTargetTime) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	//CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame ()->GetActiveDocument ());
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)pDocument;
	if (pSekaijuDoc == NULL) {
		return 0;
	}
	if (pSekaijuDoc->m_pMIDIData == NULL || pSekaijuDoc->m_pMIDIClock == NULL) {
		return 0;
	}
	long lTempo = MIDIEVENT_DEFTEMPO;
	long lMillisec = 0;
	long lEndTime = MIDIData_GetEndTime (pSekaijuDoc->m_pMIDIData);
	lTargetTime = CLIP (0, lTargetTime, lEndTime);
	pSekaijuDoc->m_lOldTime = lTargetTime;
	lMillisec = MIDIData_TimeToMillisec (pSekaijuDoc->m_pMIDIData, lTargetTime);
	// ソングポジションセレクタ(0xF2)の送出
	long lTimeMode, lTimeResolution;
	MIDIData_GetTimeBase (pSekaijuDoc->m_pMIDIData, &lTimeMode, &lTimeResolution);
	long lOutputPort;
	BYTE byMsg[256];
	memset (byMsg, 0, sizeof (byMsg));
	byMsg[0] = 0xF2;
	byMsg[1] = (lTargetTime * 4 / lTimeResolution) & 0x7F;
	byMsg[2] = ((lTargetTime * 4 / lTimeResolution) >> 7) & 0x7F;
	for (lOutputPort = 0; lOutputPort < MAXMIDIOUTDEVICENUM; lOutputPort++) {
		if (m_lMIDIOutSyncMode[lOutputPort] == 1) {
			if (m_pMIDIOut[lOutputPort]) {
				MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, 3);
			}
			if (m_pMIDIStatus[lOutputPort]) {
				MIDIStatus_PutMIDIMessage (m_pMIDIStatus[lOutputPort], byMsg, 3);
			}
		}
	}
	/* lTargetTime時のテンポをlTempoに取得 */
	VERIFY (MIDIData_FindTempo (pSekaijuDoc->m_pMIDIData, lTargetTime, &lTempo));
	/* MIDIステータスをlTargetTime時の状態に合わせる */
	ResetTempMIDIStatusArray ();
	pSekaijuDoc->TimeMIDIStatus (lTargetTime, m_pTempMIDIStatus);
	long lFlags;
	/* 演奏中ならば(a)すべて又は(b)ノートのみを復元する */
	if (m_bPlaying) {
		lFlags = m_theGeneralOption.m_bSearchUpdate ? SDS_ALL : SDS_NOTE;
		m_bIgnoreNoteEvent = 1;
	}
	/* 停止中ならば(a)ノートを除くすべてを復元するか(b)何も復元しない */
	else {
		lFlags = m_theGeneralOption.m_bSearchUpdate ? (SDS_ALL & ~SDS_NOTE) : 0;
		m_bIgnoreNoteEvent = 0;
	}
	SendDifferentStatus (lFlags);
	/* MIDIクロックの更新 */
	if (m_bPlaying) {
		MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
	}
	MIDIClock_Reset (pSekaijuDoc->m_pMIDIClock);
	MIDIClock_SetTickCount (pSekaijuDoc->m_pMIDIClock, lTargetTime);
	MIDIClock_SetMillisec (pSekaijuDoc->m_pMIDIClock, lMillisec);
	MIDIClock_SetTempo (pSekaijuDoc->m_pMIDIClock, lTempo);
	if (m_bPlaying) {
		MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
	}
	return 1;
}

// 演奏の開始
long CSekaijuApp::StartPlaying (CDocument* pDocument) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)pDocument;
	long lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
	long lOutputPort;
	BYTE byMsg[256];
	memset (byMsg, 0, sizeof (byMsg));
	byMsg[0] = (lCurTime == 0 ? 0xFA : 0xFB);
	// スタート(0xFA)又はコンティニュー(0xFB)メッセージの送出
	for (lOutputPort = 0; lOutputPort < MAXMIDIOUTDEVICENUM; lOutputPort++) {
		if (m_lMIDIOutSyncMode[lOutputPort] >= 1) {
			if (m_pMIDIOut[lOutputPort]) {
				MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, 1);
			}
			if (m_pMIDIStatus[lOutputPort]) {
				MIDIStatus_PutMIDIMessage (m_pMIDIStatus[lOutputPort], byMsg, 1);
			}
		}
	}
	// 現在の状態を復元する
	ResetTempMIDIStatusArray ();
	pSekaijuDoc->TimeMIDIStatus (lCurTime, m_pTempMIDIStatus); // 20090625修正
	SendDifferentStatus (m_theGeneralOption.m_bPlayUpdate ? SDS_ALL : SDS_NOTE);
	m_bIgnoreNoteEvent = TRUE; // 20090625修正
	// 演奏状態にする
	pSekaijuDoc->m_lOldTime = lCurTime;
	if (!m_bRecording) {
		MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
	}
	m_bPlaying = TRUE;
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_PLAYSTARTED);
	return 1;
}

// 演奏の停止(録音中の場合はStopRecordingを使うこと)
long CSekaijuApp::StopPlaying (CDocument* pDocument) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)pDocument;
	long lOutputPort;
	BYTE byMsg[256];
	memset (byMsg, 0, sizeof (byMsg));
	byMsg[0] = 0xFC;
	// ストップメッセージ(0xFC)の送出
	for (lOutputPort = 0; lOutputPort < MAXMIDIOUTDEVICENUM; lOutputPort++) {
		if (m_lMIDIOutSyncMode[lOutputPort] >= 1) {
			if (m_pMIDIOut[lOutputPort]) {
				MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, 1);
			}
			if (m_pMIDIStatus[lOutputPort]) {
				MIDIStatus_PutMIDIMessage (m_pMIDIStatus[lOutputPort], byMsg, 1);
			}
		}
	}

	// 演奏・録音の停止
	m_bRecording = FALSE;
	m_bPlaying = FALSE;
	MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
	SendAllNoteOff ();
	SendAllHold1Off ();
	SendAllSostenutoOff ();
	SendAllHold2Off ();
	SendAllSoundOff ();
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_PLAYSTOPED);
	return 1;
}


// 録音の開始
long CSekaijuApp::StartRecording (CDocument* pDocument) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)pDocument;
	long lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
	// 演奏中でない場合
	if (!m_bPlaying) {
		long lOutputPort;
		BYTE byMsg[256];
		memset (byMsg, 0, sizeof (byMsg));
		byMsg[0] = (lCurTime == 0 ? 0xFA : 0xFB);
		// スタート(0xFA)又はコンティニュー(0xFB)メッセージの送出
		for (lOutputPort = 0; lOutputPort < MAXMIDIOUTDEVICENUM; lOutputPort++) {
			if (m_lMIDIOutSyncMode[lOutputPort] >= 1) {
				if (m_pMIDIOut[lOutputPort]) {
					MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, 1);
				}
				if (m_pMIDIStatus[lOutputPort]) {
					MIDIStatus_PutMIDIMessage (m_pMIDIStatus[lOutputPort], byMsg, 1);
				}
			}
		}
		// 現在の状態を復元する
		ResetTempMIDIStatusArray ();
		pSekaijuDoc->TimeMIDIStatus (lCurTime, m_pTempMIDIStatus); // 20090626修正
		SendDifferentStatus (m_theGeneralOption.m_bPlayUpdate ? SDS_ALL : SDS_NOTE);
		m_bIgnoreNoteEvent = TRUE; // 20090626修正
		pSekaijuDoc->m_lOldTime = lCurTime;
		MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
		m_bPlaying = TRUE;
	}
	// 履歴記録
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pLastEvent = NULL;
	MIDIEvent* pCloneEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	pSekaijuDoc->AddHistoryUnit (_T("リアルタイム入力"));
	VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
	// EOTの履歴記録
	forEachTrack (pSekaijuDoc->m_pMIDIData, pMIDITrack) {
		pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
		if (pLastEvent) {
			if (MIDIEvent_IsEndofTrack (pLastEvent)) {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
				VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent));
			}
		}
	}
	// 記録開始
	pSekaijuDoc->SetModifiedFlag (TRUE);
	m_theTempRecordedEventArray.RemoveAll ();
	m_bFirstMetronome = TRUE;
	m_bRecording = TRUE;
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_RECORDSTARTED | SEKAIJUDOC_MIDIEVENTCHANGED);
	return 1;
}

// 録音の停止
long CSekaijuApp::StopRecording (CDocument* pDocument) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)pDocument;
	long lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
	long lOutputPort;
	BYTE byMsg[256];
	memset (byMsg, 0, sizeof (byMsg));
	byMsg[0] = 0xFC;
	// ストップメッセージの送出
	for (lOutputPort = 0; lOutputPort < MAXMIDIOUTDEVICENUM; lOutputPort++) {
		if (m_lMIDIOutSyncMode[lOutputPort] >= 1) {
			if (m_pMIDIOut[lOutputPort]) {
				MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, 1);
			}
			if (m_pMIDIStatus[lOutputPort]) {
				MIDIStatus_PutMIDIMessage (m_pMIDIStatus[lOutputPort], byMsg, 1);
			}
		}
	}
	// 演奏・記録停止
	m_bRecording = FALSE;
	m_bPlaying = FALSE;
	MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
	SendAllNoteOff ();
	SendAllHold1Off ();
	SendAllHold1Off ();
	SendAllHold2Off ();
	SendAllSoundOff ();
	// 履歴記録
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	MIDIEvent* pLastEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
	long lTempRecordedEventCount = m_theTempRecordedEventArray.GetSize ();
	long j;
	// 録音されたすべてのノートイベントの結合確認
	for (j = 0; j < lTempRecordedEventCount; j++) {
		pTempEvent = (MIDIEvent*)m_theTempRecordedEventArray.GetAt (j);
		// ノートオンがないノートオフイベントの場合、ノートオフイベント削除
		if (MIDIEvent_IsNoteOff (pTempEvent) && pTempEvent->m_pPrevCombinedEvent == NULL) {
			MIDIEvent_Delete (pTempEvent);
			pTempEvent = NULL;
			m_theTempRecordedEventArray.RemoveAt (j);
			j--;
			lTempRecordedEventCount = m_theTempRecordedEventArray.GetSize ();
		}
		// ノートオフがないノートオンイベントの場合、ノートオンイベント追加
		else if (MIDIEvent_IsNoteOn (pTempEvent) && pTempEvent->m_pNextCombinedEvent == NULL) {
			long lTime = MIDIEvent_GetTime (pTempEvent);
			long lChannel = MIDIEvent_GetChannel (pTempEvent);
			long lKey = MIDIEvent_GetKey (pTempEvent);
			MIDITrack* pTempTrack = MIDIEvent_GetParent (pTempEvent);
			ASSERT (pTempTrack);
			MIDIEvent* pNoteOffEvent = NULL;
			VERIFY (pNoteOffEvent = MIDIEvent_CreateNoteOff (lCurTime, lChannel, lKey, 0));
			VERIFY (MIDITrack_InsertEvent (pTempTrack, pNoteOffEvent));
			//VERIFY (MIDIEvent_Combine (pTempEvent));
			pTempEvent->m_pNextCombinedEvent = pNoteOffEvent;
			pNoteOffEvent->m_pPrevCombinedEvent = pTempEvent;
			m_theTempRecordedEventArray.Add (pNoteOffEvent);
			lTempRecordedEventCount = m_theTempRecordedEventArray.GetSize ();
		}
	}

	// 録音されたすべてのイベントを履歴に記録(結合ノートはノートオンのみ)
	for (j = 0; j < lTempRecordedEventCount; j++) {
		pTempEvent = (MIDIEvent*)m_theTempRecordedEventArray.GetAt (j);
		if (pTempEvent->m_pPrevCombinedEvent == NULL) {
			pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent);
		}
	}
	// EOTの履歴記録
	forEachTrack (pSekaijuDoc->m_pMIDIData, pMIDITrack) {
		pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
		if (pLastEvent) {
			if (MIDIEvent_IsEndofTrack (pLastEvent)) {
				pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pLastEvent);
			}
		}
	}
	m_theTempRecordedEventArray.RemoveAll ();
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_RECORDSTOPED | SEKAIJUDOC_MIDIEVENTCHANGED);
	return 1;
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// 『ファイル(&F)』-『新規作成(&N)』
void CSekaijuApp::OnUpdateFileNewUI (CCmdUI* pCmdUI) {
	//if (m_bRecording) {
	//	pCmdUI->Enable (FALSE);
	//}
}

// 『ファイル(&F)』-『開く(&O)』
void CSekaijuApp::OnUpdateFileOpenUI (CCmdUI* pCmdUI) {
	//if (m_bRecording) {
	//	pCmdUI->Enable (FALSE);
	//}
}

// 『コントロール(&C)』-『最初に戻る(&W)』
void CSekaijuApp::OnControlToBegin () {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());

	// ドキュメントがない場合
	if (pSekaijuDoc == NULL) {
		m_bRecording = FALSE;
		m_bPlaying = FALSE;
		return;
	}
	
	// ドキュメントがある場合
	pSekaijuDoc->m_theCriticalSection.Lock ();
	// 記録中の場合
	if (m_bRecording) {
		StopRecording (pSekaijuDoc);
	}
	// 記録中でない場合
	else {
		StopPlaying (pSekaijuDoc);
	}
	//MIDIClock_Reset (pSekaijuDoc->m_pMIDIClock); // 20080825移動 // 20090627廃止
	SetPlayPosition (pSekaijuDoc, 0); // 20090627追加
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED); // 20080922追加
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// 『コントロール(&C)』-『最初に戻る(&W)』のUI更新
void CSekaijuApp::OnUpdateControlToBeginUI (CCmdUI* pCmdUI) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if (pMainFrame == NULL) {
		return;
	}
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc) {
		pCmdUI->Enable (1);
	}
	else {
		pCmdUI->Enable (0);
	}
}

// 『コントロール(&C)』-『最後に進む』
void CSekaijuApp::OnControlToEnd () {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	
	// ドキュメントがない場合
	if (pSekaijuDoc == NULL) {
		m_bRecording = FALSE;
		m_bPlaying = FALSE;
		return;
	}

	// ドキュメントがある場合
	pSekaijuDoc->m_theCriticalSection.Lock ();
	// 記録中の場合
	if (m_bRecording) {
		StopRecording (pSekaijuDoc);
	}
	// 記録中でない場合
	else {
		StopPlaying (pSekaijuDoc);
	}
	long lEndTime = MIDIData_GetEndTime (pSekaijuDoc->m_pMIDIData);
	SetPlayPosition (pSekaijuDoc, lEndTime);
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED); // 20080922追加
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// 『コントロール(&C)』-『最後に進む』のUI更新
void CSekaijuApp::OnUpdateControlToEndUI (CCmdUI* pCmdUI) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if (pMainFrame == NULL) {
		return;
	}
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc) {
		pCmdUI->Enable (1);
	}
	else {
		pCmdUI->Enable (0);
	}
}

	
// 『コントロール(&C)』-『演奏(&P)』
void CSekaijuApp::OnControlPlay () {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc == NULL) {
		return;
	}
	if (pSekaijuDoc->m_pMIDIData == NULL || pSekaijuDoc->m_pMIDIClock == NULL) {
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	this->m_theCriticalSection.Lock ();
	long lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
	// 停止中の場合
	if (!m_bPlaying) {
		StartPlaying (pSekaijuDoc);
	}
	// 演奏中の場合
	else {
		// 演奏中かつ録音中の場合
		if (m_bRecording) {
			StopRecording (pSekaijuDoc);
		}
		// 演奏中であるが録音中でない場合
		else {
			StopPlaying (pSekaijuDoc);
		}
	}
	this->m_theCriticalSection.Unlock ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// 『コントロール(&C)』-『演奏(&P)』のUI更新
void CSekaijuApp::OnUpdateControlPlayUI (CCmdUI* pCmdUI) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if (pMainFrame == NULL) {
		return;
	}
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc) {
		pCmdUI->SetCheck (m_bPlaying);
		pCmdUI->Enable (1);
	}
	else {
		pCmdUI->SetCheck (0);
		pCmdUI->Enable (0);
	}
}

// 『コントロール(&C)』-『録音(&R)』
void CSekaijuApp::OnControlRecord () {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc == NULL) {
		return;
	}
	if (pSekaijuDoc->m_pMIDIData == NULL || pSekaijuDoc->m_pMIDIClock == NULL) {
		return;
	}
	if (this->m_bInplaceEditing || this->m_bInplaceListing || this->m_bValueUpDowning) {
		return;
	}
	if (pSekaijuDoc->m_bEditLocked) {
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	this->m_theCriticalSection.Lock ();
	// 停止中の場合
	if (!m_bRecording) {
		StartRecording (pSekaijuDoc);
	}
	// 記録中の場合
	else {
		StopRecording (pSekaijuDoc);
	}
	this->m_theCriticalSection.Unlock ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// 『コントロール(&C)』-『録音(&R)』UI更新
void CSekaijuApp::OnUpdateControlRecordUI (CCmdUI* pCmdUI) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if (pMainFrame == NULL) {
		return;
	}
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc) {
		pCmdUI->SetCheck (m_bRecording);
		if (this->m_bInplaceEditing || this->m_bInplaceListing || this->m_bValueUpDowning) {
			pCmdUI->Enable (0);
		}
		else if (pSekaijuDoc->m_bEditLocked) {
			pCmdUI->Enable (0);
		}
		else {
			pCmdUI->Enable (1);
		}
	}
	else {
		pCmdUI->SetCheck (0);
		pCmdUI->Enable (0);
	}
}



// 『コントロール(&C)』-『前の小節』
void CSekaijuApp::OnControlPrevMeasure () {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc == NULL) {
		return;
	}
	if (pSekaijuDoc->m_pMIDIData == NULL || pSekaijuDoc->m_pMIDIClock == NULL) {
		return;
	}
	if (m_bRecording) {
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	this->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	SendAllNoteOff ();
	SendAllHold1Off ();
	SendAllSostenutoOff ();
	SendAllHold2Off ();
	SendAllSoundOff ();
	long lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
	long lTimeMode = 0;
	long lTimeResolution = 120;
	MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);
	if (lTimeMode == MIDIDATA_TPQNBASE) {	
		long lMeasure, lBeat, lTick;
		MIDIData_BreakTime (pSekaijuDoc->m_pMIDIData, lCurTime, &lMeasure, &lBeat, &lTick);
		if (lTick * 2 / lTimeResolution < 1) { 
			MIDIData_MakeTime (pSekaijuDoc->m_pMIDIData, __max (0, lMeasure - 1), 0, 0, &lCurTime);
		}
		else {
			MIDIData_MakeTime (pSekaijuDoc->m_pMIDIData, __max (0, lMeasure), 0, 0, &lCurTime);
		}
		SetPlayPosition (pSekaijuDoc, lCurTime);
	}
	else {
		if (lCurTime % lTimeResolution < lTimeResolution / 2) {
			lCurTime = (lCurTime - lTimeResolution * 10) / lTimeResolution * lTimeResolution;
			lCurTime = __max (0, lCurTime);
		}
		else {
			lCurTime = (lCurTime - lTimeResolution * 10) / lTimeResolution * lTimeResolution;
			lCurTime = __max (0, lCurTime);
		}
		SetPlayPosition (pSekaijuDoc, lCurTime);
	}
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED);
	this->m_theCriticalSection.Unlock ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// 『コントロール(&C)』-『前の小節』のUI更新
void CSekaijuApp::OnUpdateControlPrevMeasureUI (CCmdUI* pCmdUI) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if (pMainFrame == NULL) {
		return;
	}
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc) {
		pCmdUI->Enable (1);
	}
	else {
		pCmdUI->Enable (0);
	}
}

// 『コントロール(&C)』-『次の小節』
void CSekaijuApp::OnControlNextMeasure () {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc == NULL) {
		return;
	}
	if (pSekaijuDoc->m_pMIDIData == NULL || pSekaijuDoc->m_pMIDIClock == NULL) {
		return;
	}
	if (m_bRecording) {
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	this->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	SendAllNoteOff ();
	SendAllHold1Off ();
	SendAllSostenutoOff ();
	SendAllHold2Off ();
	SendAllSoundOff ();
	long lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
	long lTimeMode = 0;
	long lTimeResolution = 120;
	MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);
	if (lTimeMode == MIDIDATA_TPQNBASE) {	
		long lMeasure, lBeat, lTick;
		MIDIData_BreakTime (pSekaijuDoc->m_pMIDIData, lCurTime, &lMeasure, &lBeat, &lTick);
		MIDIData_MakeTime (pSekaijuDoc->m_pMIDIData, __min (65535, lMeasure + 1), 0, 0, &lCurTime);
		SetPlayPosition (pSekaijuDoc, lCurTime);
	}
	else {
		lCurTime = (lCurTime + (lTimeResolution * 10)) / lTimeResolution * lTimeResolution;
		lCurTime = __min (0x7FFFFFFF, lCurTime);
		SetPlayPosition (pSekaijuDoc, lCurTime);
	}
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED);
	this->m_theCriticalSection.Unlock ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// 『コントロール(&C)』-『次の小節』のUI更新
void CSekaijuApp::OnUpdateControlNextMeasureUI (CCmdUI* pCmdUI) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if (pMainFrame == NULL) {
		return;
	}
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc) {
		pCmdUI->Enable (1);
	}
	else {
		pCmdUI->Enable (0);
	}
}

// 『コントロール(&C)』-『前の拍』
void CSekaijuApp::OnControlPrevBeat () {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc == NULL) {
		return;
	}
	if (pSekaijuDoc->m_pMIDIData == NULL || pSekaijuDoc->m_pMIDIClock == NULL) {
		return;
	}
	if (m_bRecording) {
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	this->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	SendAllNoteOff ();
	SendAllHold1Off ();
	SendAllSostenutoOff ();
	SendAllHold2Off ();
	SendAllSoundOff ();
	long lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
	long lTimeMode = 0;
	long lTimeResolution = 120;
	MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);
	if (lTimeMode == MIDIDATA_TPQNBASE) {	
		long lMeasure, lBeat, lTick;
		MIDIData_BreakTime (pSekaijuDoc->m_pMIDIData, lCurTime, &lMeasure, &lBeat, &lTick);
		if (lTick * 2 / lTimeResolution < 1) { 
			MIDIData_MakeTime (pSekaijuDoc->m_pMIDIData, __max (lMeasure, 0), lBeat - 1, 0, &lCurTime);
		}
		else {
			MIDIData_MakeTime (pSekaijuDoc->m_pMIDIData, __max (lMeasure, 0), lBeat - 1, 0, &lCurTime);
		}
		SetPlayPosition (pSekaijuDoc, lCurTime);
	}
	else {
		if (lCurTime % lTimeResolution < lTimeResolution / 2) {
			lCurTime = ((lCurTime - lTimeResolution) / lTimeResolution) * lTimeResolution;
			lCurTime = __max (0, lCurTime);
		}
		else {
			lCurTime = lCurTime / lTimeResolution * lTimeResolution;
			lCurTime = __max (0, lCurTime);
		}
		SetPlayPosition (pSekaijuDoc, lCurTime);
	}
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED);
	this->m_theCriticalSection.Unlock ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// 『コントロール(&C)』-『前の拍』のUI更新
void CSekaijuApp::OnUpdateControlPrevBeatUI (CCmdUI* pCmdUI) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if (pMainFrame == NULL) {
		return;
	}
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc) {
		pCmdUI->Enable (1);
	}
	else {
		pCmdUI->Enable (0);
	}
}

// 『コントロール(&C)』-『次の拍』
void CSekaijuApp::OnControlNextBeat () {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc == NULL) {
		return;
	}
	if (pSekaijuDoc->m_pMIDIData == NULL || pSekaijuDoc->m_pMIDIClock == NULL) {
		return;
	}
	if (m_bRecording) {
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	this->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	SendAllNoteOff ();
	SendAllHold1Off ();
	SendAllSostenutoOff ();
	SendAllHold2Off ();
	SendAllSoundOff ();
	long lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
	long lTimeMode = 0;
	long lTimeResolution = 120;
	MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);
	if (lTimeMode == MIDIDATA_TPQNBASE) {	
		long lMeasure, lBeat, lTick;
		MIDIData_BreakTime (pSekaijuDoc->m_pMIDIData, lCurTime, &lMeasure, &lBeat, &lTick);
		MIDIData_MakeTime (pSekaijuDoc->m_pMIDIData, __min (65535, lMeasure), lBeat + 1, 0, &lCurTime);
		SetPlayPosition (pSekaijuDoc, lCurTime);
	}
	else {
		lCurTime = ((lCurTime + lTimeResolution) / lTimeResolution) * lTimeResolution;
		lCurTime = __min (0x7FFFFFFF, lCurTime);
		SetPlayPosition (pSekaijuDoc, lCurTime);
	}
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED);
	this->m_theCriticalSection.Unlock ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// 『コントロール(&C)』-『次の拍』のUI更新
void CSekaijuApp::OnUpdateControlNextBeatUI (CCmdUI* pCmdUI) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if (pMainFrame == NULL) {
		return;
	}
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc) {
		pCmdUI->Enable (1);
	}
	else {
		pCmdUI->Enable (0);
	}
}


// 『コントロール(&C)』-『スピード=静止』
void CSekaijuApp::OnControlSpeedNone () {
	if (m_bRecording) {
		return;
	}
	if (m_lCurSpeedIndex != 0) {
		m_lOldSpeedIndex = m_lCurSpeedIndex;
	}
	m_lCurSpeedIndex = 0;

	// 開いている全ドキュメントのMIDIクロックのスピードと同期モード再設定
	POSITION docpos = m_pSekaijuDocTemplate->GetFirstDocPosition ();
	while (docpos) {
		CSekaijuDoc* pSekaijuDoc =(CSekaijuDoc*)(m_pSekaijuDocTemplate->GetNextDoc (docpos));
		long lRunning = MIDIClock_IsRunning (pSekaijuDoc->m_pMIDIClock);
		pSekaijuDoc->m_theCriticalSection.Lock ();
		if (lRunning) {
			MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
		}
		pSekaijuDoc->ApplyAppCurSpeedIndex ();
		if (lRunning) {
			MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
		}
		//pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED);
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);		
	}
}

// 『コントロール(&C)』-『スピード=静止』
void CSekaijuApp::OnUpdateControlSpeedNoneUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurSpeedIndex == 0 ? 1 : 0);
	pCmdUI->Enable (m_bRecording ? 0 : 1);
	//CToolBar* pToolBar = (CToolBar*)pCmdUI;
	//pToolBar->Invalidate (TRUE);
}

// 『コントロール(&C)』-『スピード=低速』
void CSekaijuApp::OnControlSpeedSlow () {
	if (m_lCurSpeedIndex != 1) {
		m_lOldSpeedIndex = m_lCurSpeedIndex;
	}
	m_lCurSpeedIndex = 1;

	// 開いている全ドキュメントのMIDIクロックのスピードと同期モード再設定
	POSITION docpos = m_pSekaijuDocTemplate->GetFirstDocPosition ();
	while (docpos) {
		CSekaijuDoc* pSekaijuDoc =(CSekaijuDoc*)(m_pSekaijuDocTemplate->GetNextDoc (docpos));
		long lRunning = MIDIClock_IsRunning (pSekaijuDoc->m_pMIDIClock);
		pSekaijuDoc->m_theCriticalSection.Lock ();
		if (lRunning) {
			MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
		}
		pSekaijuDoc->ApplyAppCurSpeedIndex ();
		if (lRunning) {
			MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
		}
		//pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED);
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);		
	}
}

// 『コントロール(&C)』-『スピード=低速』
void CSekaijuApp::OnUpdateControlSpeedSlowUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurSpeedIndex == 1 ? 1 : 0);
}

// 『コントロール(&C)』-『スピード=標準』
void CSekaijuApp::OnControlSpeedNormal () {
	if (m_lCurSpeedIndex != 2) {
		m_lOldSpeedIndex = m_lCurSpeedIndex;
	}
	m_lCurSpeedIndex = 2;

	// 開いている全ドキュメントのMIDIクロックのスピードと同期モード再設定
	POSITION docpos = m_pSekaijuDocTemplate->GetFirstDocPosition ();
	while (docpos) {
		CSekaijuDoc* pSekaijuDoc =(CSekaijuDoc*)(m_pSekaijuDocTemplate->GetNextDoc (docpos));
		long lRunning = MIDIClock_IsRunning (pSekaijuDoc->m_pMIDIClock);
		pSekaijuDoc->m_theCriticalSection.Lock ();
		if (lRunning) {
			MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
		}
		pSekaijuDoc->ApplyAppCurSpeedIndex ();
		if (lRunning) {
			MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
		}
		//pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED);
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);		
	}
}

// 『コントロール(&C)』-『スピード=標準』
void CSekaijuApp::OnUpdateControlSpeedNormalUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurSpeedIndex == 2 ? 1 : 0);
}

// 『コントロール(&C)』-『スピード=高速』
void CSekaijuApp::OnControlSpeedFast () {
	if (m_lCurSpeedIndex != 3) {
		m_lOldSpeedIndex = m_lCurSpeedIndex;
	}
	m_lCurSpeedIndex = 3;

	// 開いている全ドキュメントのMIDIクロックのスピードと同期モード再設定
	POSITION docpos = m_pSekaijuDocTemplate->GetFirstDocPosition ();
	while (docpos) {
		CSekaijuDoc* pSekaijuDoc =(CSekaijuDoc*)(m_pSekaijuDocTemplate->GetNextDoc (docpos));
		long lRunning = MIDIClock_IsRunning (pSekaijuDoc->m_pMIDIClock);
		pSekaijuDoc->m_theCriticalSection.Lock ();
		if (lRunning) {
			MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
		}
		pSekaijuDoc->ApplyAppCurSpeedIndex ();
		if (lRunning) {
			MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
		}
		//pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED);
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);		
	}

}

// 『コントロール(&C)』-『スピード=高速』
void CSekaijuApp::OnUpdateControlSpeedFastUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurSpeedIndex == 3 ? 1 : 0);
}

// 『コントロール(&C)』-『スピード=他機器にスレーブ』
void CSekaijuApp::OnControlSpeedSlave () {
	long lSyncInputPort = 0;
	long lSyncInputMode = 0;
	GetCurSyncInputPortAndMode (&lSyncInputPort, &lSyncInputMode);
	// スレーブするものが見つからない場合
	if (lSyncInputMode == 0) {
		return;
	}
	
	if (m_lCurSpeedIndex != 4) {
		m_lOldSpeedIndex = m_lCurSpeedIndex;
	}
	m_lCurSpeedIndex = 4;

	// 開いている全ドキュメントのMIDIクロックのスピードと同期モード再設定
	POSITION docpos = m_pSekaijuDocTemplate->GetFirstDocPosition ();
	while (docpos) {
		CSekaijuDoc* pSekaijuDoc =(CSekaijuDoc*)(m_pSekaijuDocTemplate->GetNextDoc (docpos));
		long lRunning = MIDIClock_IsRunning (pSekaijuDoc->m_pMIDIClock);
		pSekaijuDoc->m_theCriticalSection.Lock ();
		if (lRunning) {
			MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
		}
		pSekaijuDoc->ApplyAppCurSpeedIndex ();
		if (lRunning) {
			MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
		}
		//pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED);
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);		
	}

}

// 『コントロール(&C)』-『スピード=他機器にスレーブ』
void CSekaijuApp::OnUpdateControlSpeedSlaveUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurSpeedIndex == 4 ? 1 : 0);
}


// 『コントロール(&C)』-『オートリピート(&A)』
void CSekaijuApp::OnControlAutoRepeat () {
	m_bAutoRepeat = !m_bAutoRepeat;
}

// 『コントロール(&C)』-『オートリピート(&A)』
void CSekaijuApp::OnUpdateControlAutoRepeatUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_bAutoRepeat);
}


// 『設定(&S)』-『MIDIデバイスとインストゥルメント(&D)...』
void CSekaijuApp::OnSetupMIDIDevice () {
	long i;
	CString strNone;
	VERIFY (strNone.LoadString (IDS_NONE));
	CMIDIDeviceSheet theSheet (AfxGetMainWnd ());
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		if (m_strMIDIInName[i] == _T("")) {
			theSheet.m_theMIDIInDevicePage.m_strMIDIInName[i] = _T("");
		}
		else {
			theSheet.m_theMIDIInDevicePage.m_strMIDIInName[i] = m_strMIDIInName[i];
		}
	}
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		if (m_strMIDIOutName[i] == _T("")) {
			theSheet.m_theMIDIOutDevicePage.m_strMIDIOutName[i] = _T("");
		}
		else {
			theSheet.m_theMIDIOutDevicePage.m_strMIDIOutName[i] = m_strMIDIOutName[i];
		}
	}
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		if (m_strMIDIInstDefNormName[i] == _T("")) {
			theSheet.m_theMIDIInstDefNormPage.m_strMIDIInstDefNormName[i] = _T("");
		}
		else {
			theSheet.m_theMIDIInstDefNormPage.m_strMIDIInstDefNormName[i] = m_strMIDIInstDefNormName[i];
		}
	}
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		if (m_strMIDIInstDefDrumName[i] == _T("")) {
			theSheet.m_theMIDIInstDefDrumPage.m_strMIDIInstDefDrumName[i] = _T("");
		}
		else {
			theSheet.m_theMIDIInstDefDrumPage.m_strMIDIInstDefDrumName[i] = m_strMIDIInstDefDrumName[i];
		}
	}
	if (theSheet.DoModal () == IDOK) {
		for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
			m_strMIDIInName[i] = theSheet.m_theMIDIInDevicePage.m_strMIDIInName[i];
		}
		for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
			m_strMIDIOutName[i] = theSheet.m_theMIDIOutDevicePage.m_strMIDIOutName[i];
		}
		for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
			m_strMIDIInstDefNormName[i] = theSheet.m_theMIDIInstDefNormPage.m_strMIDIInstDefNormName[i];
		}
		for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
			m_strMIDIInstDefDrumName[i] = theSheet.m_theMIDIInstDefDrumPage.m_strMIDIInstDefDrumName[i];
		}
		OpenAllMIDIInDevice ();
		OpenAllMIDIOutDevice ();
		SelectAllMIDIInstDefNorm ();
		SelectAllMIDIInstDefDrum ();
		((CSekaijuDocTemplate*)m_pSekaijuDocTemplate)->UpdateAllViews 
			(NULL, SEKAIJUDOC_MIDIDATACHANGED | SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED);
	}
}

// 『設定(&S)』-『MIDI同期モード(&S)...』
void CSekaijuApp::OnSetupMIDISyncMode () {
	long i;
	CMIDISyncModeSheet theSheet (AfxGetMainWnd ());
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		if (0 <= m_lMIDIInSyncMode[i] && m_lMIDIInSyncMode[i] <= 2) {
			theSheet.m_theMIDIInSyncModePage.m_nMIDIInSyncMode[i] = m_lMIDIInSyncMode[i];
		}
		else {
			theSheet.m_theMIDIInSyncModePage.m_nMIDIInSyncMode[i] = 0;
		}
	}
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		if (0 <= m_lMIDIOutSyncMode[i] && m_lMIDIOutSyncMode[i] <= 5) {
			theSheet.m_theMIDIOutSyncModePage.m_nMIDIOutSyncMode[i] = m_lMIDIOutSyncMode[i];
		}
		else {
			theSheet.m_theMIDIOutSyncModePage.m_nMIDIOutSyncMode[i] = 0;
		}
	}
	if (theSheet.DoModal () == IDOK) {
		for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
			m_lMIDIInSyncMode[i] = theSheet.m_theMIDIInSyncModePage.m_nMIDIInSyncMode[i];
		}
		for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
			m_lMIDIOutSyncMode[i] = theSheet.m_theMIDIOutSyncModePage.m_nMIDIOutSyncMode[i];
		}

		// スレーブするものが見つからない場合マスター標準速度とする。
		long lSyncInputPort = 0;
		long lSyncInputMode = 0;
		GetCurSyncInputPortAndMode (&lSyncInputPort, &lSyncInputMode);
		if (lSyncInputMode == 0 && m_lCurSpeedIndex == 4) {
			m_lCurSpeedIndex = 2;
		}

		// 開いている全ドキュメントのMIDIクロックのスピードと同期モード再設定
		POSITION docpos = m_pSekaijuDocTemplate->GetFirstDocPosition ();
		while (docpos) {
			CSekaijuDoc* pSekaijuDoc =(CSekaijuDoc*)(m_pSekaijuDocTemplate->GetNextDoc (docpos));
			long lRunning = MIDIClock_IsRunning (pSekaijuDoc->m_pMIDIClock);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			if (lRunning) {
				MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
			}
			pSekaijuDoc->ApplyAppCurSpeedIndex ();
			if (lRunning) {
				MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
			}
			pSekaijuDoc->UpdateAllViews (NULL, 0xFFFFFFFF);
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			::Sleep (0);		
		}
	}
}


// MIDIデバイスとインストゥルメントで『適用』を押したとき
void CSekaijuApp::ApplyMIDIDeviceSheet (CPropertySheet* pSheet) {
	long i;
	CMIDIDeviceSheet* pMIDIDeviceSheet = (CMIDIDeviceSheet*)pSheet;
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		m_strMIDIInName[i] = pMIDIDeviceSheet->m_theMIDIInDevicePage.m_strMIDIInName[i];
	}
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		m_strMIDIOutName[i] = pMIDIDeviceSheet->m_theMIDIOutDevicePage.m_strMIDIOutName[i];
	}
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		m_strMIDIInstDefNormName[i] = pMIDIDeviceSheet->m_theMIDIInstDefNormPage.m_strMIDIInstDefNormName[i];
	}
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		m_strMIDIInstDefDrumName[i] = pMIDIDeviceSheet->m_theMIDIInstDefDrumPage.m_strMIDIInstDefDrumName[i];
	}
	OpenAllMIDIInDevice ();
	OpenAllMIDIOutDevice ();
	SelectAllMIDIInstDefNorm ();
	SelectAllMIDIInstDefDrum ();
	((CSekaijuDocTemplate*)m_pSekaijuDocTemplate)->UpdateAllViews 
		(NULL, SEKAIJUDOC_MIDIDATACHANGED | SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED);
}

// MIDI同期モードプロパティシートで『適用』を押したとき
void CSekaijuApp::ApplyMIDISyncModeSheet (CPropertySheet* pSheet) {
	long i;
	CMIDISyncModeSheet* pMIDISyncModeSheet = (CMIDISyncModeSheet*)pSheet;
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		m_lMIDIInSyncMode[i] = pMIDISyncModeSheet->m_theMIDIInSyncModePage.m_nMIDIInSyncMode[i];
	}
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		m_lMIDIOutSyncMode[i] = pMIDISyncModeSheet->m_theMIDIOutSyncModePage.m_nMIDIOutSyncMode[i];
	}

	// スレーブするものが見つからない場合マスター標準速度とする。
	long lSyncInputPort = 0;
	long lSyncInputMode = 0;
	GetCurSyncInputPortAndMode (&lSyncInputPort, &lSyncInputMode);
	if (lSyncInputMode == 0 && m_lCurSpeedIndex == 4) {
		m_lCurSpeedIndex = 2;
	}

	// 開いている全ドキュメントのMIDIクロックのスピードと同期モード再設定
	POSITION docpos = m_pSekaijuDocTemplate->GetFirstDocPosition ();
	while (docpos) {
		CSekaijuDoc* pSekaijuDoc =(CSekaijuDoc*)(m_pSekaijuDocTemplate->GetNextDoc (docpos));
		long lRunning = MIDIClock_IsRunning (pSekaijuDoc->m_pMIDIClock);
		pSekaijuDoc->m_theCriticalSection.Lock ();
		if (lRunning) {
			MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
		}
		pSekaijuDoc->ApplyAppCurSpeedIndex ();
		if (lRunning) {
			MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
		}
		pSekaijuDoc->UpdateAllViews (NULL, 0xFFFFFFFF);
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);		
	}
}


// 『設定(&S)』-『インストゥルメント(&I)...』
void CSekaijuApp::OnSetupInstrument () {

}

// 『設定(&S)』-『メトロノーム(&M)...』
void CSekaijuApp::OnSetupMetronome () {
	CMetronomeDlg theDlg;
	theDlg.m_nOn = m_theMetronomeDlgStatus.m_nOn;
	theDlg.m_nOutputPort = CLIP (0, m_theMetronomeDlgStatus.m_nOutputPort, 15);
	theDlg.m_nOutputChannel = CLIP (0, m_theMetronomeDlgStatus.m_nOutputChannel, 15) + 1;
	theDlg.m_nNoteKey1 = CLIP (0, m_theMetronomeDlgStatus.m_nNoteKey1, 127);
	theDlg.m_nNoteVel1 = CLIP (0, m_theMetronomeDlgStatus.m_nNoteVel1, 127);
	theDlg.m_nNoteKey2 = CLIP (0, m_theMetronomeDlgStatus.m_nNoteKey2, 127);
	theDlg.m_nNoteVel2 = CLIP (0, m_theMetronomeDlgStatus.m_nNoteVel2, 127);
	if (theDlg.DoModal () == IDOK) {
		m_theMetronomeDlgStatus.m_nOn =theDlg.m_nOn; 
		m_theMetronomeDlgStatus.m_nOutputPort = theDlg.m_nOutputPort;
		m_theMetronomeDlgStatus.m_nOutputChannel = theDlg.m_nOutputChannel - 1;
		m_theMetronomeDlgStatus.m_nNoteKey1 = theDlg.m_nNoteKey1;
		m_theMetronomeDlgStatus.m_nNoteVel1 = theDlg.m_nNoteVel1;
		m_theMetronomeDlgStatus.m_nNoteKey2 = theDlg.m_nNoteKey2;
		m_theMetronomeDlgStatus.m_nNoteVel2 = theDlg.m_nNoteVel2;
	}
}

// 『設定(&S)』-『自動保存(&A)...』
void CSekaijuApp::OnSetupAutoSave () {
	CAutoSaveDlg theDlg;
	theDlg.m_strExeFilePath = m_strExeFilePath;
	theDlg.m_nOn = m_theAutoSaveDlgStatus.m_nOn;
	theDlg.m_lInterval = CLIP (1, m_theAutoSaveDlgStatus.m_lInterval / 60, 120);
	theDlg.m_nDisableWhilePlaying = m_theAutoSaveDlgStatus.m_nDisableWhilePlaying;
	theDlg.m_nDisableWhileRecording = m_theAutoSaveDlgStatus.m_nDisableWhileRecording;
	if (theDlg.DoModal () == IDOK) {
		m_theAutoSaveDlgStatus.m_nOn = theDlg.m_nOn;
		m_theAutoSaveDlgStatus.m_lInterval =CLIP (1,  theDlg.m_lInterval, 120) * 60;
		m_theAutoSaveDlgStatus.m_nDisableWhilePlaying = theDlg.m_nDisableWhilePlaying;
		m_theAutoSaveDlgStatus.m_nDisableWhileRecording = theDlg.m_nDisableWhileRecording;
	}
}

// 『設定(&S)』-『言語(&L)...』
void CSekaijuApp::OnSetupLanguage () {
	CLanguageDlg theDlg;
	theDlg.m_strLanguage = m_strLanguage;
	if (theDlg.DoModal () == IDOK) {
		m_strLanguage = theDlg.m_strLanguage;
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_LANGUAGE_WILL_BE_CHANGED_AT_THE_NEXT_STARTUP));
		AfxMessageBox (strMsg, MB_ICONINFORMATION);
	}
}


// 『設定(&S)』-『オプション(&O)...』
void CSekaijuApp::OnSetupOptions () {
	COptionSheet theSheet (AfxGetMainWnd ());
	theSheet.m_theGeneralOptionPage.m_bEnableMultiExec = m_theGeneralOption.m_bEnableMultiExec;
	theSheet.m_theGeneralOptionPage.m_bEnableMultiOpen = m_theGeneralOption.m_bEnableMultiOpen;
	theSheet.m_theGeneralOptionPage.m_bRestoreWindowPlacement = m_theGeneralOption.m_bRestoreWindowPlacement;
	theSheet.m_theGeneralOptionPage.m_bExecOpen = m_theGeneralOption.m_bExecOpen;
	theSheet.m_theGeneralOptionPage.m_bOpenPlay = m_theGeneralOption.m_bOpenPlay;
	theSheet.m_theGeneralOptionPage.m_bPlayUpdate = m_theGeneralOption.m_bPlayUpdate;
	theSheet.m_theGeneralOptionPage.m_bSearchUpdate = m_theGeneralOption.m_bSearchUpdate;
	theSheet.m_theGeneralOptionPage.m_bEnableCC111Loop = m_theGeneralOption.m_bEnableCC111Loop;
	theSheet.m_theGeneralOptionPage.m_bPatchSearch = m_theGeneralOption.m_bPatchSearch;
	theSheet.m_theGeneralOptionPage.m_bInvertCtrlMouseWheel = m_theGeneralOption.m_bInvertCtrlMouseWheel;
	theSheet.m_theGeneralOptionPage.m_bTrackZeroOrigin = m_theGeneralOption.m_bTrackZeroOrigin;
	theSheet.m_theGeneralOptionPage.m_bEventZeroOrigin = m_theGeneralOption.m_bEventZeroOrigin;
	theSheet.m_theGeneralOptionPage.m_bEnableAutoPageUpdate = m_theGeneralOption.m_bEnableAutoPageUpdate;
	theSheet.m_theGeneralOptionPage.m_bSendNoteOffHoldOffAtEnd = m_theGeneralOption.m_bSendNoteOffHoldOffAtEnd;
	theSheet.m_theGeneralOptionPage.m_lUpDownDelta1 = CLIP (1, m_theGeneralOption.m_lUpDownDelta1, 16);
	theSheet.m_theGeneralOptionPage.m_lUpDownDelta2 = CLIP (1, m_theGeneralOption.m_lUpDownDelta2, 16);
	theSheet.m_theGeneralOptionPage.m_lKeyVelocity1 = CLIP (1, m_theGeneralOption.m_lKeyVelocity1, 127);
	theSheet.m_theGeneralOptionPage.m_lKeyVelocity2 = CLIP (1, m_theGeneralOption.m_lKeyVelocity2, 127);
	theSheet.m_theGeneralOptionPage.m_lSpeedSlow = CLIP (1, m_theGeneralOption.m_lSpeedSlow / 100, 1000);
	theSheet.m_theGeneralOptionPage.m_lSpeedNormal = CLIP (1, m_theGeneralOption.m_lSpeedNormal / 100, 1000);
	theSheet.m_theGeneralOptionPage.m_lSpeedFast = CLIP (1, m_theGeneralOption.m_lSpeedFast / 100, 1000);
	theSheet.m_theGeneralOptionPage.m_lPlayRecordInterval = CLIP (1, m_theGeneralOption.m_lPlayRecordInterval, 1000);
	theSheet.m_theGeneralOptionPage.m_lOctaveSignature = CLIP (3, m_theGeneralOption.m_lOctaveSignature, 5);
	
	theSheet.m_theTrackListOption1Page.m_lDefRowZoom = CLIP (16, m_theTrackListOption1.m_lDefRowZoom, 64);
	theSheet.m_theTrackListOption1Page.m_lDefColumnZoom = CLIP (2, m_theTrackListOption1.m_lDefColumnZoom, 16);
	theSheet.m_theTrackListOption1Page.m_lDefTimeZoom = CLIP (2, m_theTrackListOption1.m_lDefTimeZoom, 16);
	theSheet.m_theTrackListOption1Page.m_lDefNameWidth = CLIP (1, m_theTrackListOption1.m_lDefNameWidth, 32);
	theSheet.m_theTrackListOption1Page.m_lDefColorWidth = CLIP (1, m_theTrackListOption1.m_lDefColorWidth, 32);
	theSheet.m_theTrackListOption1Page.m_lDefInputOnWidth = CLIP (1, m_theTrackListOption1.m_lDefInputOnWidth, 32);
	theSheet.m_theTrackListOption1Page.m_lDefInputPortWidth = CLIP (1, m_theTrackListOption1.m_lDefInputPortWidth, 32);
	theSheet.m_theTrackListOption1Page.m_lDefInputChWidth = CLIP (1, m_theTrackListOption1.m_lDefInputChWidth, 32);
	theSheet.m_theTrackListOption1Page.m_lDefOutputOnWidth = CLIP (1, m_theTrackListOption1.m_lDefOutputOnWidth, 32);
	theSheet.m_theTrackListOption1Page.m_lDefOutputPortWidth = CLIP (1, m_theTrackListOption1.m_lDefOutputPortWidth, 32);
	theSheet.m_theTrackListOption1Page.m_lDefOutputChWidth = CLIP (1, m_theTrackListOption1.m_lDefOutputChWidth, 32);
	theSheet.m_theTrackListOption1Page.m_lDefViewModeWidth = CLIP (1, m_theTrackListOption1.m_lDefViewModeWidth, 32);

	theSheet.m_theTrackListOption2Page.m_lDefCC000Width = CLIP (1, m_theTrackListOption2.m_lDefCC000Width, 32);
	theSheet.m_theTrackListOption2Page.m_lDefCC032Width = CLIP (1, m_theTrackListOption2.m_lDefCC032Width, 32);
	theSheet.m_theTrackListOption2Page.m_lDefPCWidth = CLIP (1, m_theTrackListOption2.m_lDefPCWidth, 32);
	theSheet.m_theTrackListOption2Page.m_lDefCC007Width = CLIP (1, m_theTrackListOption2.m_lDefCC007Width, 32);
	theSheet.m_theTrackListOption2Page.m_lDefCC010Width = CLIP (1, m_theTrackListOption2.m_lDefCC010Width, 32);
	theSheet.m_theTrackListOption2Page.m_lDefCC091Width = CLIP (1, m_theTrackListOption2.m_lDefCC091Width, 32);
	theSheet.m_theTrackListOption2Page.m_lDefCC093Width = CLIP (1, m_theTrackListOption2.m_lDefCC093Width, 32);
	theSheet.m_theTrackListOption2Page.m_lDefCC094Width = CLIP (1, m_theTrackListOption2.m_lDefCC094Width, 32);
	theSheet.m_theTrackListOption2Page.m_lDefKeyShiftWidth = CLIP (1, m_theTrackListOption2.m_lDefKeyShiftWidth, 32);
	theSheet.m_theTrackListOption2Page.m_lDefVelShiftWidth = CLIP (1, m_theTrackListOption2.m_lDefVelShiftWidth, 32);
	theSheet.m_theTrackListOption2Page.m_lDefTimeShiftWidth = CLIP (1, m_theTrackListOption2.m_lDefTimeShiftWidth, 32);
	theSheet.m_theTrackListOption2Page.m_lDefNumEventWidth = CLIP (1, m_theTrackListOption2.m_lDefNumEventWidth, 32);

	theSheet.m_thePianoRollOptionPage.m_lDefKeyZoom = CLIP (4, m_thePianoRollOption.m_lDefKeyZoom, 16);
	theSheet.m_thePianoRollOptionPage.m_lDefVelZoom = CLIP (1, m_thePianoRollOption.m_lDefVelZoom, 4);
	theSheet.m_thePianoRollOptionPage.m_lDefTimeZoom = CLIP (1, m_thePianoRollOption.m_lDefTimeZoom, 16);

	theSheet.m_theEventListOptionPage.m_lDefRowZoom = CLIP (16, m_theEventListOption.m_lDefRowZoom, 32);
	theSheet.m_theEventListOptionPage.m_lDefColumnZoom = CLIP (2, m_theEventListOption.m_lDefColumnZoom, 16);
	theSheet.m_theEventListOptionPage.m_lDefTrackWidth = CLIP (1, m_theEventListOption.m_lDefTrackWidth, 32);
	theSheet.m_theEventListOptionPage.m_lDefMillisecWidth = CLIP (1, m_theEventListOption.m_lDefMillisecWidth, 32);
	theSheet.m_theEventListOptionPage.m_lDefTimeWidth = CLIP (1, m_theEventListOption.m_lDefTimeWidth, 32);
	theSheet.m_theEventListOptionPage.m_lDefKindWidth = CLIP (1, m_theEventListOption.m_lDefKindWidth, 32);
	theSheet.m_theEventListOptionPage.m_lDefChWidth = CLIP (1, m_theEventListOption.m_lDefChWidth, 32);
	theSheet.m_theEventListOptionPage.m_lDefVal1Width = CLIP (1, m_theEventListOption.m_lDefVal1Width, 32);
	theSheet.m_theEventListOptionPage.m_lDefVal2Width = CLIP (1, m_theEventListOption.m_lDefVal2Width, 32);
	theSheet.m_theEventListOptionPage.m_lDefVal3Width = CLIP (1, m_theEventListOption.m_lDefVal3Width, 32);

	theSheet.m_theEventListOptionPage.m_bInsertEventAfter = m_theEventListOption.m_bInsertEventAfter;
	theSheet.m_theEventListOptionPage.m_bDuplicateEventAfter = m_theEventListOption.m_bDuplicateEventAfter;
	theSheet.m_theEventListOptionPage.m_bDeleteEventAfter = m_theEventListOption.m_bDeleteEventAfter;

	theSheet.m_theMusicalScoreOptionPage.m_lDefTrackZoom = CLIP (1, m_theMusicalScoreOption.m_lDefTrackZoom, 8);
	theSheet.m_theMusicalScoreOptionPage.m_lDefTimeZoom = CLIP (1, m_theMusicalScoreOption.m_lDefTimeZoom, 16);

	if (theSheet.DoModal () == IDOK) {
		m_theGeneralOption.m_bEnableMultiExec = theSheet.m_theGeneralOptionPage.m_bEnableMultiExec;
		m_theGeneralOption.m_bEnableMultiOpen = theSheet.m_theGeneralOptionPage.m_bEnableMultiOpen;
		m_theGeneralOption.m_bRestoreWindowPlacement = theSheet.m_theGeneralOptionPage.m_bRestoreWindowPlacement;
		m_theGeneralOption.m_bExecOpen = theSheet.m_theGeneralOptionPage.m_bExecOpen;
		m_theGeneralOption.m_bOpenPlay = theSheet.m_theGeneralOptionPage.m_bOpenPlay;
		m_theGeneralOption.m_bPlayUpdate = theSheet.m_theGeneralOptionPage.m_bPlayUpdate;
		m_theGeneralOption.m_bSearchUpdate = theSheet.m_theGeneralOptionPage.m_bSearchUpdate;
		m_theGeneralOption.m_bEnableCC111Loop = theSheet.m_theGeneralOptionPage.m_bEnableCC111Loop;
		m_theGeneralOption.m_bPatchSearch = theSheet.m_theGeneralOptionPage.m_bPatchSearch;
		m_theGeneralOption.m_bInvertCtrlMouseWheel = theSheet.m_theGeneralOptionPage.m_bInvertCtrlMouseWheel;
		m_theGeneralOption.m_bTrackZeroOrigin = theSheet.m_theGeneralOptionPage.m_bTrackZeroOrigin;
		m_theGeneralOption.m_bEventZeroOrigin = theSheet.m_theGeneralOptionPage.m_bEventZeroOrigin;
		m_theGeneralOption.m_bEnableAutoPageUpdate = theSheet.m_theGeneralOptionPage.m_bEnableAutoPageUpdate;
		m_theGeneralOption.m_bSendNoteOffHoldOffAtEnd = theSheet.m_theGeneralOptionPage.m_bSendNoteOffHoldOffAtEnd;

		m_theGeneralOption.m_lUpDownDelta1 = theSheet.m_theGeneralOptionPage.m_lUpDownDelta1;
		m_theGeneralOption.m_lUpDownDelta2 = theSheet.m_theGeneralOptionPage.m_lUpDownDelta2;
		m_theGeneralOption.m_lKeyVelocity1 = theSheet.m_theGeneralOptionPage.m_lKeyVelocity1;
		m_theGeneralOption.m_lKeyVelocity2 = theSheet.m_theGeneralOptionPage.m_lKeyVelocity2;
		m_theGeneralOption.m_lSpeedSlow = theSheet.m_theGeneralOptionPage.m_lSpeedSlow * 100;
		m_theGeneralOption.m_lSpeedNormal = theSheet.m_theGeneralOptionPage.m_lSpeedNormal * 100;
		m_theGeneralOption.m_lSpeedFast = theSheet.m_theGeneralOptionPage.m_lSpeedFast * 100;
		m_theGeneralOption.m_lPlayRecordInterval = theSheet.m_theGeneralOptionPage.m_lPlayRecordInterval;
		m_theGeneralOption.m_lOctaveSignature = theSheet.m_theGeneralOptionPage.m_lOctaveSignature;

		m_theTrackListOption1.m_lDefRowZoom = theSheet.m_theTrackListOption1Page.m_lDefRowZoom;
		m_theTrackListOption1.m_lDefColumnZoom = theSheet.m_theTrackListOption1Page.m_lDefColumnZoom;
		m_theTrackListOption1.m_lDefTimeZoom = theSheet.m_theTrackListOption1Page.m_lDefTimeZoom;
		m_theTrackListOption1.m_lDefNameWidth = theSheet.m_theTrackListOption1Page.m_lDefNameWidth;
		m_theTrackListOption1.m_lDefColorWidth = theSheet.m_theTrackListOption1Page.m_lDefColorWidth;
		m_theTrackListOption1.m_lDefInputOnWidth = theSheet.m_theTrackListOption1Page.m_lDefInputOnWidth;
		m_theTrackListOption1.m_lDefInputPortWidth = theSheet.m_theTrackListOption1Page.m_lDefInputPortWidth;
		m_theTrackListOption1.m_lDefInputChWidth = theSheet.m_theTrackListOption1Page.m_lDefInputChWidth;
		m_theTrackListOption1.m_lDefOutputOnWidth = theSheet.m_theTrackListOption1Page.m_lDefOutputOnWidth;
		m_theTrackListOption1.m_lDefOutputPortWidth = theSheet.m_theTrackListOption1Page.m_lDefOutputPortWidth;
		m_theTrackListOption1.m_lDefOutputChWidth = theSheet.m_theTrackListOption1Page.m_lDefOutputChWidth;
		m_theTrackListOption1.m_lDefViewModeWidth = theSheet.m_theTrackListOption1Page.m_lDefViewModeWidth;

		m_theTrackListOption2.m_lDefCC000Width = theSheet.m_theTrackListOption2Page.m_lDefCC000Width;
		m_theTrackListOption2.m_lDefCC032Width = theSheet.m_theTrackListOption2Page.m_lDefCC032Width;
		m_theTrackListOption2.m_lDefPCWidth = theSheet.m_theTrackListOption2Page.m_lDefPCWidth;
		m_theTrackListOption2.m_lDefCC007Width = theSheet.m_theTrackListOption2Page.m_lDefCC007Width;
		m_theTrackListOption2.m_lDefCC010Width = theSheet.m_theTrackListOption2Page.m_lDefCC010Width;
		m_theTrackListOption2.m_lDefCC091Width = theSheet.m_theTrackListOption2Page.m_lDefCC091Width;
		m_theTrackListOption2.m_lDefCC093Width = theSheet.m_theTrackListOption2Page.m_lDefCC093Width;
		m_theTrackListOption2.m_lDefCC094Width = theSheet.m_theTrackListOption2Page.m_lDefCC094Width;
		m_theTrackListOption2.m_lDefKeyShiftWidth = theSheet.m_theTrackListOption2Page.m_lDefKeyShiftWidth;
		m_theTrackListOption2.m_lDefVelShiftWidth = theSheet.m_theTrackListOption2Page.m_lDefVelShiftWidth;
		m_theTrackListOption2.m_lDefTimeShiftWidth = theSheet.m_theTrackListOption2Page.m_lDefTimeShiftWidth;
		m_theTrackListOption2.m_lDefNumEventWidth= theSheet.m_theTrackListOption2Page.m_lDefNumEventWidth;

		m_thePianoRollOption.m_lDefKeyZoom = theSheet.m_thePianoRollOptionPage.m_lDefKeyZoom;
		m_thePianoRollOption.m_lDefVelZoom = theSheet.m_thePianoRollOptionPage.m_lDefVelZoom;
		m_thePianoRollOption.m_lDefTimeZoom = theSheet.m_thePianoRollOptionPage.m_lDefTimeZoom;

		m_theEventListOption.m_lDefRowZoom = theSheet.m_theEventListOptionPage.m_lDefRowZoom;
		m_theEventListOption.m_lDefColumnZoom = theSheet.m_theEventListOptionPage.m_lDefColumnZoom;
		m_theEventListOption.m_lDefTrackWidth = theSheet.m_theEventListOptionPage.m_lDefTrackWidth;
		m_theEventListOption.m_lDefMillisecWidth = theSheet.m_theEventListOptionPage.m_lDefMillisecWidth;
		m_theEventListOption.m_lDefTimeWidth = theSheet.m_theEventListOptionPage.m_lDefTimeWidth;
		m_theEventListOption.m_lDefKindWidth = theSheet.m_theEventListOptionPage.m_lDefKindWidth;
		m_theEventListOption.m_lDefChWidth = theSheet.m_theEventListOptionPage.m_lDefChWidth;
		m_theEventListOption.m_lDefVal1Width = theSheet.m_theEventListOptionPage.m_lDefVal1Width;
		m_theEventListOption.m_lDefVal2Width = theSheet.m_theEventListOptionPage.m_lDefVal2Width;
		m_theEventListOption.m_lDefVal3Width = theSheet.m_theEventListOptionPage.m_lDefVal3Width;

		m_theEventListOption.m_bInsertEventAfter = 	theSheet.m_theEventListOptionPage.m_bInsertEventAfter;
		m_theEventListOption.m_bDuplicateEventAfter = theSheet.m_theEventListOptionPage.m_bDuplicateEventAfter;
		m_theEventListOption.m_bDeleteEventAfter = theSheet.m_theEventListOptionPage.m_bDeleteEventAfter;
		
		m_theMusicalScoreOption.m_lDefTrackZoom = theSheet.m_theMusicalScoreOptionPage.m_lDefTrackZoom;
		m_theMusicalScoreOption.m_lDefTimeZoom = theSheet.m_theMusicalScoreOptionPage.m_lDefTimeZoom;

		
		// 開いている全ドキュメントのMIDIクロックのスピードと同期モード再設定
		POSITION docpos = m_pSekaijuDocTemplate->GetFirstDocPosition ();
		while (docpos) {
			CSekaijuDoc* pSekaijuDoc =(CSekaijuDoc*)(m_pSekaijuDocTemplate->GetNextDoc (docpos));
			long lRunning = MIDIClock_IsRunning (pSekaijuDoc->m_pMIDIClock);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			if (lRunning) {
				MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
			}
			pSekaijuDoc->ApplyAppCurSpeedIndex ();
			if (lRunning) {
				MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
			}
			pSekaijuDoc->UpdateAllViews (NULL, 0xFFFFFFFF);
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			::Sleep (0);		
		}
		
		// メニューの更新
		UpdateMenu ();
	}
}

// オプションプロパティシートで『適用』を押したとき
void CSekaijuApp::ApplyOptionSheet (CPropertySheet* pSheet) {

}

// 『ヘルプ(&H)』-『readme(&R)...』
void CSekaijuApp::OnHelpReadMe () {
	// ShellExecuteやGetOpenFileNameやGetSaveFileNameは
	// デバッグ版ではアクセス違反が発生するが、
	// リリース版では問題ないとの情報あり。
	CString strMsg;
	CString strMsg2;
	CString strFileName1;
	CString strFileName2;
	strFileName1.LoadString (IDS_READMEFILENAME);
	strFileName2 = m_strExeFilePath + strFileName1;
	long lRet = (long)::ShellExecute (this->m_pMainWnd->GetSafeHwnd (), 
		_T("open"), (LPCTSTR)strFileName2, NULL, NULL, SW_SHOW);
	if (lRet == 0) {
		// "メモリ不足又はリソース不足です。"
		strMsg.LoadString (IDS_INSUFFICIENT_MEMORY_OR_INSUFFICIENT_RESOURCE);
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
	}
	else if (1 <= lRet && lRet < 32) {
		// "%s\n-ファイルが開けません。"
		strMsg.LoadString (IDS_S_N_FILE_OPEN_FAILED);
		strMsg2.Format (strMsg, strFileName2);
		AfxMessageBox (strMsg2, MB_ICONEXCLAMATION);
	}
}

// 『ヘルプ(&H)』-『ライセンス(&L)...』
void CSekaijuApp::OnHelpLicense () {
	CString strMsg;
	CString strMsg2;
	CString strFileName1;
	CString strFileName2;
	strFileName1.LoadString (IDS_LICENSEFILENAME);
	strFileName2 = m_strExeFilePath + strFileName1;
	long lRet = (long)::ShellExecute (this->m_pMainWnd->GetSafeHwnd (), 
		_T("open"), (LPCTSTR)strFileName2, NULL, NULL, SW_SHOW);
	if (lRet == 0) {
		// "メモリ不足又はリソース不足です。"
		strMsg.LoadString (IDS_INSUFFICIENT_MEMORY_OR_INSUFFICIENT_RESOURCE);
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
	}
	else if (1 <= lRet && lRet < 32) {
		// "%s\n-ファイルが開けません。"
		strMsg.LoadString (IDS_S_N_FILE_OPEN_FAILED);
		strMsg2.Format (strMsg, strFileName2);
		AfxMessageBox (strMsg2, MB_ICONEXCLAMATION);
	}
}

// 『ヘルプ(&H)』-『取扱説明書(PDF)(&M)...』
void CSekaijuApp::OnHelpManual () {
	CString strMsg;
	CString strMsg2;
	CString strFileName1;
	CString strFileName2;
	strFileName1.LoadString (IDS_MANUALFILENAME);
	strFileName2 = m_strExeFilePath + strFileName1;
	long lRet = (long)::ShellExecute (this->m_pMainWnd->GetSafeHwnd (), 
		_T("open"), (LPCTSTR)strFileName2, NULL, NULL, SW_SHOW);
	if (lRet == 0) {
		// "メモリ不足又はリソース不足です。"
		strMsg.LoadString (IDS_INSUFFICIENT_MEMORY_OR_INSUFFICIENT_RESOURCE);
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
	}
	else if (1 <= lRet && lRet < 32) {
		// "%s\n-ファイルが開けません。"
		strMsg.LoadString (IDS_S_N_FILE_OPEN_FAILED);
		strMsg2.Format (strMsg, strFileName2);
		AfxMessageBox (strMsg2, MB_ICONEXCLAMATION);
	}
}


// 『ヘルプ(&H)』-『プロジェクトホームページ(&P)...』
void CSekaijuApp::OnHelpProjectHomePage () {
	CString strMsg;
	CString strMsg2;
	CString strFileName1;
	strFileName1.LoadString (IDS_HOMEPAGEADDRESS);
	long lRet = (long)::ShellExecute (this->m_pMainWnd->GetSafeHwnd (), 
		_T("open"), (LPCTSTR)strFileName1, NULL, NULL, SW_SHOW);
	if (lRet == 0) {
		// "メモリ不足又はリソース不足です。"
		strMsg.LoadString (IDS_INSUFFICIENT_MEMORY_OR_INSUFFICIENT_RESOURCE);
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
	}
	else if (1 <= lRet && lRet < 32) {
		// "%s\n-ファイルが開けません。"
		strMsg.LoadString (IDS_S_N_FILE_OPEN_FAILED);
		strMsg2.Format (strMsg, strFileName1);
		AfxMessageBox (strMsg2, MB_ICONEXCLAMATION);
	}
}


// 『ヘルプ(&H)』-『ヴァージョン情報(&A)...』
void CSekaijuApp::OnHelpAbout () {
	CAboutDlg theDlg;
	theDlg.DoModal();
}
