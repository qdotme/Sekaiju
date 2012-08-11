//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// トラックリストフレームウィンドウクラス
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

#define TRACKLISTFRAME_NUMCOLUMN 21

#define TRACKLISTFRAME_TRACKNAME     0x01
#define TRACKLISTFRAME_VISIBLE       0x30
#define TRACKLISTFRAME_ENABLE        0x31
#define TRACKLISTFRAME_VIEWMODE      0x33
#define TRACKLISTFRAME_FORECOLOR     0x34
#define TRACKLISTFRAME_BACKCOLOR     0x35
#define TRACKLISTFRAME_INPUTON       0x36
#define TRACKLISTFRAME_INPUTPORT     0x37
#define TRACKLISTFRAME_INPUTCHANNEL  0x38
#define TRACKLISTFRAME_OUTPUTON      0x39
#define TRACKLISTFRAME_OUTPUTPORT    0x3A
#define TRACKLISTFRAME_OUTPUTCHANNEL 0x3B
#define TRACKLISTFRAME_TIMEPLUS      0x3C
#define TRACKLISTFRAME_KEYPLUS       0x3D
#define TRACKLISTFRAME_VELOCITYPLUS  0x3E
#define TRACKLISTFRAME_NUMEVENT      0xFF
#define TRACKLISTFRAME_CONTROLCHANGE 0xB0
#define TRACKLISTFRAME_PROGRAMCHANGE 0xC0

#ifndef _TRACKLISTFRAME_H_
#define _TRACKLISTFRAME_H_

class CTrackListFrame : public CChildFrame {
	DECLARE_DYNCREATE (CTrackListFrame)

	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
protected:
	long m_lToolBar1Height;             // ツールバーの高さ[pixel]
	long m_lScaleHeight;                // 上部目盛りビューの高さ[pixel]
	long m_lScaleWidth;                 // 左部目盛りビューの幅[pixel]
	long m_lTrackHeight;                // トラックビューの高さ[pixel]
	long m_lModeWidth;                  // モードビュー(左ペイン)の幅[pixel]
	long m_lTimeWidth;                  // タイムビュー(右ペイン)の幅[pixel]
	long m_lHScrollBarHeight;           // 水平スクロールバーの高さ[pixel]
	long m_lVScrollBarWidth;            // 垂直スクロールバーの幅[pixel]
	long m_lColumnZoom;                 // 列方向拡大倍率[倍]
	long m_lTimeZoom;                   // 時間方向拡大倍率[倍]
	long m_lRowZoom;                    // 行方向拡大倍率[倍]
	long m_lColumnScrollPos;            // 列方向スクロール位置[pixel]
	long m_lTimeScrollPos;              // 時間方向スクロール位置[pixel]
	long m_lRowScrollPos;               // 行方向スクロール位置[pixel]
	long m_lColumnBaseWidth[TRACKLISTFRAME_NUMCOLUMN];  // 各列の幅(拡大倍率をかけていない)[pixel]
	CString m_strColumnTitle[TRACKLISTFRAME_NUMCOLUMN]; // 各列のタイトル文字列
	long m_lColumnContent[TRACKLISTFRAME_NUMCOLUMN];    // 各列の表示内容識別コード
	// 各列の表示内容識別コードは、
	// TRACKLISTFRAME_TRACKNAME～TRACKLISTFRAME_PROGRAMCHANGEの中から選択する。
	// TRACKLISTFRAME_CONTROLCHANGEでは9～16ビット目でCC#番号を指定する。

public:
	CSekaijuToolBar m_wndToolBar1;      // ツールバー
	CView* m_pDummyView;                // ダミービュー(Visible=FALSE)へのポインタ
	CView* m_pPrintView;                // 印刷ビュー(Visible=FALSE)へのポインタ
	CView* m_pScaleView;                // 目盛りビューへのポインタ
	CView* m_pModeScaleView;            // 項目目盛りビューへのポインタ
	CView* m_pTimeScaleView;            // タイム目盛りビューへのポインタ
	CView* m_pTrackScaleView;           // トラック番号目盛りビューへのポインタ
	CView* m_pTrackModeView;            // トラック番号-項目ビュー(左ペイン)へのポインタ
	CView* m_pTrackTimeView;            // トラック番号-タイムビュー(右ペイン)へのポインタ
	CScrollBar m_wndColumnScroll;       // 列方向スクロールバー
	CScrollBar m_wndTimeScroll;         // 時間方向スクロールバー
	CScrollBar m_wndRowScroll;          // 行方向スクロールバー
	CScrollBar m_wndSizeScroll;         // サイズスクロールバー
	CBitmapButton m_wndColumnZoomUp;    // 列方向拡大ボタン
	CBitmapButton m_wndColumnZoomDown;  // 列方向縮小ボタン
	CBitmapButton m_wndTimeZoomUp;      // 時間方向拡大ボタン
	CBitmapButton m_wndTimeZoomDown;    // 時間方向縮小ボタン
	CBitmapButton m_wndRowZoomUp;       // 行方向拡大ボタン
	CBitmapButton m_wndRowZoomDown;     // 行方向縮小ボタン

public:
	CFont m_theFont;                    // トラックビューウィンドウで使うフォント
	long m_lCurTool;                    // 現在選択されているツール番号(0=選択,1=試聴)
	BOOL m_bAutoPageUpdate;             // 自動的にページを更新するか？

protected:
	CPoint m_ptMouseDown;               // マウスが押された座標(スプリッターを動かすのに使う)
	CPoint m_ptMouseMoveOld;            // マウスが動かされた前回の座標(スプリッターを動かすのに使う)


	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CTrackListFrame ();                 // コンストラクタ
	virtual ~CTrackListFrame();         // デストラクタ


	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	CSekaijuDoc* GetDocument ();
	virtual long GetColumnBaseWidth (long lColumn);
	virtual long SetColumnBaseWidth (long lColumn, long lColumnBaseWidth);
	virtual CString GetColumnTitle (long lColumn);
	virtual long GetColumnContent (long lColumn);
	virtual long GetRowZoom ();
	virtual long GetColumnZoom ();
	virtual long GetTimeZoom ();
	virtual long GetColumnLeft (long lColumn);
	virtual long GetColumnWidth (long lColumn);
	virtual long XtoColumn (long x);
	virtual long ColumntoX (long lMode);
	virtual long XtoTime (long x);
	virtual long TimetoX (long lTime);
	virtual long YtoRow (long y);
	virtual long RowtoY (long lKey);
	virtual long GetVisibleTopRow ();
	virtual long GetVisibleBottomRow ();
	virtual long GetVisibleLeftTime ();
	virtual long GetVisibleRightTime ();
	virtual long GetTimeScrollPos ();
	virtual long GetRowScrollPos ();
	virtual long GetColumnScrollPos ();
	virtual long SetTimeScrollPos (long lTimeScrollPos);
	virtual long SetColumnScrollPos (long lModeScrollPos);
	virtual long SetRowScrollPos (long lTrackScrollPos);
	virtual void DrawSplitterCaptor (CDC* pDC, CPoint pt);
	virtual void RecalcRowScrollInfo ();
	virtual void RecalcColumnScrollInfo ();
	virtual void RecalcTimeScrollInfo ();


	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual BOOL PreCreateWindow (CREATESTRUCT& cs);
	virtual void OnUpdateFrameTitle (BOOL bAddToTitle);
	virtual void RecalcLayout (BOOL bNotify = TRUE);
	virtual BOOL OnCreateClient (LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL OnCmdMsg (UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy ();
	afx_msg void OnSize (UINT nType, int cx, int cy);
	afx_msg void OnTimer (UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd (CDC* pDC);
	afx_msg void OnPaint( );
	afx_msg void OnMDIActivate (BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnClose ();
	afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnMouseMove (UINT nFlags, CPoint point);
	afx_msg void OnColumnZoomDown ();
	afx_msg void OnColumnZoomUp ();
	afx_msg void OnTimeZoomDown ();
	afx_msg void OnTimeZoomUp ();
	afx_msg void OnRowZoomDown ();
	afx_msg void OnRowZoomUp ();
	afx_msg void OnVScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTrackListInsertTrack ();
	afx_msg void OnUpdateTrackListInsertTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnTrackListDuplicateTrack ();
	afx_msg void OnUpdateTrackListDuplicateTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnTrackListDeleteTrack ();
	afx_msg void OnUpdateTrackListDeleteTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnTrackListMoveUpTrack ();
	afx_msg void OnUpdateTrackListMoveUpTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnTrackListMoveDownTrack ();
	afx_msg void OnUpdateTrackListMoveDownTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnTrackListSelect ();
	afx_msg void OnUpdateTrackListSelectUI (CCmdUI* pCmdUI);
	afx_msg void OnTrackListSpeaker ();
	afx_msg void OnUpdateTrackListSpeakerUI (CCmdUI* pCmdUI);
	afx_msg void OnTrackListAutoPageUpdate ();
	afx_msg void OnUpdateTrackListAutoPageUpdateUI (CCmdUI* pCmdUI);
	afx_msg void OnTrackListSaveAs ();
	afx_msg void OnUpdateTrackListSaveAsUI (CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};


#endif

