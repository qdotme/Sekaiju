//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// イベントリストフレームウィンドウクラス
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

#ifndef _EVENTLISTFRAME_H_
#define _EVENTLISTFRAME_H_

class CEventListFrame : public CChildFrame {
	DECLARE_DYNCREATE (CEventListFrame)

	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
protected:
	long m_lToolBar1Height;             // ツールバーの高さ[pixel]
	long m_lScaleHeight;                // 項目目盛りビューの高さ[pixel]
	long m_lIndexHeight;                // イベント番号-項目ビューの高さ[pixel]
	long m_lScaleWidth;                 // イベント番号目盛りビューの幅[pixel]
	long m_lPropertyWidth;              // イベント番号-項目ビューの幅[pixel]
	long m_lTrackListWidth;             // トラックリストボックスの幅[pixel]
	long m_lTrackListHeight;            // トラックリストボックスの高さ[pixel]
	long m_lEventKindListHeight;        // イベントの種類リストボックスの高さ[pixel]
	long m_lHScrollBarHeight;           // 水平スクロールバーの高さ[pixel]
	long m_lVScrollBarWidth;            // 垂直スクロールバーの幅[pixel]
	long m_lRowZoom;                    // 行方向ズーム倍率[倍]
	long m_lColumnZoom;                 // 列方向ズーム倍率[倍]
	long m_lRowScrollPos;               // 行方向スクロール位置[pixel]
	long m_lColumnScrollPos;            // 列方向スクロール位置[pixel]

public:
	CSekaijuToolBar m_wndToolBar1;      // ツールバー
	CColorfulComboBox m_wndEventTrackCombo; // ツールバー上のイベントのトラック番号コンボボックス
	CEdit m_wndEventTimeEdit;           // ツールバー上のイベントのタイムエディットボックス
	CComboBox m_wndEventKindCombo;      // ツールバー上のイベントの種類コンボボックス
	CComboBox m_wndEventChannelCombo;   // ツールバー上のイベントのチャンネルコンボボックス
	CView* m_pDummyView;                // ダミービュー(Visible=FALSE)へのポインタ
	CView* m_pPrintView;                // 印刷用ビュー(Visible=FALSE)へのポインタ
	CView* m_pScaleView;                // 目盛りビューへのポインタ
	CView* m_pPropertyScaleView;        // 項目目盛りビューへのポインタ
	CView* m_pIndexScaleView;           // イベント番号目盛りビューへのポインタ
	CView* m_pIndexPropertyView;        // イベント番号-項目ビューへのポインタ
	CColorfulCheckListBox m_wndTrackList; // トラックリストボックス
	CCheckListBox m_wndEventKindList;   // イベントの種類リストボックス
	CScrollBar m_wndColumnScroll;       // 列方向スクロールバー
	CScrollBar m_wndRowScroll;          // 行方向スクロールバー
	CScrollBar m_wndSizeScroll;         // サイズスクロールバー
	CBitmapButton m_wndColumnZoomUp;    // 列方向拡大ボタン
	CBitmapButton m_wndColumnZoomDown;  // 列方向縮小ボタン
	CBitmapButton m_wndRowZoomUp;       // 行方向拡大ボタン
	CBitmapButton m_wndRowZoomDown;     // 行方向縮小ボタン

public:
	BOOL m_bAutoPageUpdate;             // 自動的にページを更新するか？

protected:
	CPoint m_ptMouseDown;               // マウスが押されたときの座標(スプリッターを動かすのに使う)
	CPoint m_ptMouseMoveOld;            // マウスが動かされたときの前回の座標(スプリッターを動かすのに使う)
	BOOL m_bSplitterMovingH;            // 現在水平スプリッターを動かしているところか？
	BOOL m_bSplitterMovingV;            // 現在垂直スプリッターを動かしているところか？
	CPtrArray m_theVisibleEventArray;   // 表示させるイベントへのポインタの配列
	CFont m_theFont;                    // イベントリストウィンドウで使うフォント
	long m_lColumnBaseWidth[8];         // 各列の幅[pixel]
	CString m_strColumnTitle[8];        // 各列のタイトル文字列(例："トラック", "時:分:秒:ミリ秒"...)

protected:
	BOOL m_bOnlyCurTrack;               // 現在のトラックのみを表示するか？
	BOOL m_bShowAllTrack;               // すべてのトラックを表示するか？
	BOOL m_bTrackVisible[MAXMIDITRACKNUM]; // 各々のトラック[0～65525]は可視か？
	BOOL m_bOnlyCurEventKind;           // 現在のイベントの種類のみを表示するか？
	BOOL m_bShowAllEventKind;           // すべてのイベントの種類を表示するか？
	BOOL m_bEventKindVisible[256];      // 各々のイベントの種類[0～255]は可視か？

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CEventListFrame ();                 // コンストラクタ
	virtual ~CEventListFrame ();        // デストラクタ

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	virtual CSekaijuDoc* GetDocument ();
	virtual CFont* GetParentFont ();
	virtual long GetVisibleTopRow ();
	virtual long GetVisibleBottomRow ();
	virtual long GetColumnBaseWidth (long lColumn);
	virtual long SetColumnBaseWidth (long lColumn, long lBaseWidth);
	virtual CString GetColumnTitle (long lColumn);
	virtual long SetColumnTitle (long lColumn, const TCHAR* pszColumnTitle);
	virtual long GetColumnLeft (long lColumn);
	virtual long GetColumnWidth (long lColumn);
	virtual long XtoColumn (long x);
	virtual long ColumntoX (long lColumn);
	virtual long YtoRow (long y);
	virtual long RowtoY (long lRow);
	virtual long GetColumnZoom ();
	virtual long GetRowZoom ();
	virtual long GetColumnScrollPos ();
	virtual long GetRowScrollPos ();
	virtual long SetColumnScrollPos (long lColumnScrollPos);
	virtual long SetRowScrollPos (long lRowScrollPos);
	virtual long GetVisibleEventCount ();
	virtual MIDIEvent* GetVisibleEvent (long lIndex);
	virtual void DrawSplitterCaptor (CDC* pDC, CPoint pt);
	virtual BOOL MakeVisibleEventArray ();
	virtual BOOL ReplaceVisibleEvent (MIDIEvent* pOldEvent, MIDIEvent* pNewEvent);
	virtual long GetCurTrackIndex ();
	virtual long GetCurEventKind ();
	virtual BOOL SetCurTrackIndex (long lCurTrackIndex);
	virtual BOOL SetCurEventKind (long lCurEventKind);
	virtual BOOL SetCurChannel (long lCurChannel);
	virtual BOOL IsTrackVisible (long lTrackIndex);
	virtual BOOL SetTrackVisible (long lTrackIndex);
	virtual BOOL IsEventKindVisible (long lEventKind);
	virtual BOOL SetEventKindVisible (long lEventKind);
	virtual BOOL UpdateTrackCombo ();
	virtual BOOL UpdateTrackList ();
	virtual long EventKindtoListIndex (long lEventKind);
	virtual long ListIndextoEventKind (long lListIndex);
	virtual void RecalcRowScrollInfo ();
	virtual void RecalcColumnScrollInfo ();

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
	afx_msg void OnMDIActivate (BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnClose ();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnMouseMove (UINT nFlags, CPoint point);
	afx_msg void OnColumnZoomDown ();
	afx_msg void OnColumnZoomUp ();
	afx_msg void OnRowZoomDown ();
	afx_msg void OnRowZoomUp ();
	afx_msg void OnVScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);


	afx_msg void OnEventListInsertEvent ();
	afx_msg void OnUpdateEventListInsertEventUI (CCmdUI* pCmdUI);
	afx_msg void OnEventListDuplicateEvent ();
	afx_msg void OnUpdateEventListDuplicateEventUI (CCmdUI* pCmdUI);
	afx_msg void OnEventListDeleteEvent ();
	afx_msg void OnUpdateEventListDeleteEventUI (CCmdUI* pCmdUI);
	afx_msg void OnEventListOnlyCurTrack ();
	afx_msg void OnUpdateEventListOnlyCurTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnEventListShowAllTrack ();
	afx_msg void OnUpdateEventListShowAllTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnEventListOnlyCurEventKind ();
	afx_msg void OnUpdateEventListOnlyCurEventKindUI (CCmdUI* pCmdUI);
	afx_msg void OnEventListShowAllEventKind ();
	afx_msg void OnUpdateEventListShowAllEventKindUI (CCmdUI* pCmdUI);
	afx_msg void OnEventListAutoPageUpdate ();
	afx_msg void OnUpdateEventListAutoPageUpdateUI (CCmdUI* pCmdUI);
	afx_msg void OnEventListSaveAs ();
	afx_msg void OnUpdateEventListSaveAsUI (CCmdUI* pCmdUI);

	afx_msg void OnTrackComboSelEndOK ();
	afx_msg void OnTrackListChkChange ();
	afx_msg void OnTrackListSelChange ();

	afx_msg void OnEventKindComboSelEndOK ();	
	afx_msg void OnEventKindListChkChange ();
	afx_msg void OnEventKindListSelChange ();

	afx_msg void OnPopupTrackVisibleOn ();
	afx_msg void OnUpdatePopupTrackVisibleOnUI (CCmdUI* pCmdUI); // 20100429追加
	afx_msg void OnPopupTrackVisibleOff ();
	afx_msg void OnUpdatePopupTrackVisibleOffUI (CCmdUI* pCmdUI); // 20100429追加
	afx_msg void OnPopupTrackVisibleAll ();
	afx_msg void OnUpdatePopupTrackVisibleAllUI (CCmdUI* pCmdUI); // 20100429追加
	afx_msg void OnPopupEventKindVisibleOn ();
	afx_msg void OnUpdatePopupEventKindVisibleOnUI (CCmdUI* pCmdUI); // 20100429追加
	afx_msg void OnPopupEventKindVisibleOff ();
	afx_msg void OnUpdatePopupEventKindVisibleOffUI (CCmdUI* pCmdUI); // 20100429追加
	afx_msg void OnPopupEventKindVisibleAll ();
	afx_msg void OnUpdatePopupEventKindVisibleAllUI (CCmdUI* pCmdUI); // 20100429追加

	DECLARE_MESSAGE_MAP()
};

#endif

