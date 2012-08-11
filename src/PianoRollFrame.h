//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ピアノロールフレームウィンドウクラス
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

#ifndef _PIANOROLLFRAME_H_
#define _PIANOROLLFRAME_H_

class CPianoRollFrame : public CChildFrame {
	DECLARE_DYNCREATE (CPianoRollFrame)

	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
protected:
	long m_lToolBar1Height;             // ツールバー1の高さ[pixel]
	long m_lKeyHeight;                  // キービュー(上ペイン)の高さ[pixel]
	long m_lScaleHeight;                // 上部目盛りビューの高さ[pixel]
	long m_lVelHeight;                  // ベロシティビュー(下ペイン)の高さ[pixel]
	long m_lScaleWidth;                 // 左部目盛りビューの幅[pixel]
	long m_lTimeWidth;                  // 時間ビュー(左ペイン)の幅[pixel]
	long m_lHScrollBarHeight;           // 水平スクロールバーの高さ[pixel]
	long m_lVScrollBarWidth;            // 垂直スクロールバーの幅[pixel]
	long m_lTrackListWidth;             // トラックリスト(右ペイン)の幅[pixel]
	long m_lKeyZoom;                    // キー方向拡大倍率[倍]
	long m_lTimeZoom;                   // タイム方向拡大倍率[倍]
	long m_lVelZoom;                    // ベロシティ方向ズーム倍率[倍]
	long m_lKeyScrollPos;               // キー方向スクロール位置[pixel]
	long m_lVelScrollPos;               // ベロシティ方向スクロール位置[pixel]
	long m_lTimeScrollPos;              // 時間方向スクロール位置[pixel]
public:
	CFont m_theFont;                    // ピアノロールウィンドウで使うフォント
public:
	CSekaijuToolBar m_wndToolBar1;      // ツールバー1
	CColorfulComboBox m_wndTrackCombo;  // トラックコンボボックス
	CComboBox m_wndChannelCombo;        // チャンネルコンボボックス
	CComboBox m_wndSnapCombo;           // スナップコンボボックス
	CComboBox m_wndVelocityCombo;       // ベロシティコンボボックス
	CComboBox m_wndDurationCombo;       // 音長さコンボボックス
	CComboBox m_wndGraphKindCombo;      // グラフの種類コンボボックス
	CComboBox m_wndGraphSnapCombo;      // グラフスナップコンボボックス
	CView* m_pDummyView;                // ダミービュー(Visible=FALSE)へのポインタ
	CView* m_pPrintView;                // 印刷用ビュー(Visible=FALSE)へのポインタ
	CView* m_pScaleView;                // 目盛りビューへのポインタ
	CView* m_pTimeScaleView;            // 時間目盛りビューへのポインタ
	CView* m_pKeyScaleView;             // 音階目盛りビューへのポインタ
	CView* m_pKeyTimeView;              // 音階-時間ビューへのポインタ
	CView* m_pVelScaleView;             // ベロシティ目盛りビューへのポインタ
	CView* m_pVelTimeView;              // ベロシティ-時間ビューへのポインタ
	CColorfulCheckListBox m_wndTrackList; // トラックリストボックス
	CCheckListBox m_wndGraphKindList;   // グラフの種類リストボックス
	CScrollBar m_wndTimeScroll;         // 時間方向スクロールバー
	CScrollBar m_wndKeyScroll;          // 音階方向スクロールバー
	CScrollBar m_wndVelScroll;          // ベロシティ方向スクロールバー
	CScrollBar m_wndSizeScroll;         // サイズ変更スクロールバー
	CBitmapButton m_wndTimeZoomUp;      // 時間方向拡大ボタン
	CBitmapButton m_wndTimeZoomDown;    // 時間方向縮小ボタン
	CBitmapButton m_wndVelZoomUp;       // ベロシティ方向拡大ボタン
	CBitmapButton m_wndVelZoomDown;     // ベロシティ方向縮小ボタン
	CBitmapButton m_wndKeyZoomUp;       // キー方向拡大ボタン
	CBitmapButton m_wndKeyZoomDown;     // キー方向縮小ボタン
protected:
	CPoint m_ptMouseDown;               // マウスが押されたときの座標(スプリッターを動かすのに使う)
	CPoint m_ptMouseMoveOld;            // マウスが動かされたときの前回の座標(スプリッターを動かすのに使う)
	BOOL m_bSplitterMovingH;            // 現在水平スプリッターを動かしているところか？
	BOOL m_bSplitterMovingV;            // 現在垂直スプリッターを動かしているところか？
public:
	BOOL m_bAutoPageUpdate;             // 自動的にページを更新するか？
	long m_lCurTool;                    // 現在の選択されているツール(0=描画,1=線画,2=消しゴム,3=選択,4=試聴)
protected:
	BOOL m_bOnlyCurTrack;               // 現在のトラックのみを表示するか？
	BOOL m_bShowAllTrack;               // すべてのトラックを表示するか？
	BOOL m_bTrackVisible[MAXMIDITRACKNUM]; // トラック[0～65535]は可視か？
	BOOL m_bOnlyCurGraph;               // 現在のグラフのみを表示するか？
	BOOL m_bShowAllGraph;               // すべてのグラフを表示するか？
	BOOL m_bGraphVisible[256];          // グラフ[0～255]は可視か？

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CPianoRollFrame ();                 // コンストラクタ
	virtual ~CPianoRollFrame();         // デストラクタ

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	virtual CSekaijuDoc* GetDocument ();
	virtual long GetTimeZoom ();
	virtual long GetKeyZoom ();
	virtual long GetVelZoom ();
	virtual long XtoTime (long x);
	virtual long TimetoX (long lTime);
	virtual long YtoKey (long y);
	virtual long KeytoY (long lKey);
	virtual long YtoPitchBend (long y);
	virtual long YtoVel (long y);
	virtual long YtoTempoBPM (long y);
	virtual long VeltoY (long lVel);
	virtual long PitchBendtoY (long lPitchBend);
	virtual long TempoBPMtoY (long lTempo);
	virtual long GetTimeScrollPos ();
	virtual long GetKeyScrollPos ();
	virtual long GetVelScrollPos ();
	virtual long SetTimeScrollPos (long lTimeScrollPos);
	virtual long SetKeyScrollPos (long lKeyScrollPos);
	virtual long SetVelScrollPos (long lVelScrollPos);
	virtual long GetVisibleLeftTime ();
	virtual long GetVisibleRightTime ();
	virtual long GetVisibleTopKey ();
	virtual long GetVisibleBottomKey ();
	virtual long GetVisibleTopVel ();
	virtual long GetVisibleBottomVel ();
	virtual void DrawSplitterCaptor (CDC* pDC, CPoint pt);
	virtual long GetCurTrackIndex ();
	virtual long GetCurChannel ();
	virtual long GetCurSnap ();
	virtual long GetCurVelocity ();
	virtual long GetCurDuration ();
	virtual long GetCurGraphKind ();
	virtual long GetCurGraphSnap ();
	virtual BOOL SetCurTrackIndex (long lCurTrackIndex);
	virtual BOOL SetCurChannel (long lCurChannel);
	virtual BOOL SetCurSnap (long lCurSnap);
	virtual BOOL SetCurVelocity (long lCurVelocity);
	virtual BOOL SetCurDuration (long lCurDuration);
	virtual BOOL SetCurGraphKind (long lCurGraphKind);
	virtual BOOL SetCurGraphSnap (long lCurGraphKind);
	virtual BOOL IsTrackVisible (long lTrackIndex);
	virtual BOOL SetTrackVisible (long lTrackIndex);
	virtual BOOL IsGraphVisible (long lGraphKind);
	virtual BOOL SetGraphVisible (long lGraphKind);
	virtual BOOL UpdateTrackCombo ();
	virtual BOOL UpdateGraphKindCombo ();
	virtual BOOL UpdateTrackList ();
	virtual BOOL UpdateGraphKindList ();
	virtual BOOL UpdateSnapCombo ();
	virtual BOOL UpdateVelocityCombo ();
	virtual BOOL UpdateDurationCombo ();
	virtual BOOL UpdateGraphSnapCombo ();
	virtual void RecalcKeyScrollInfo ();
	virtual void RecalcVelScrollInfo ();
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
	afx_msg void OnMDIActivate (BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnPaint ();
	afx_msg void OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnMouseMove (UINT nFlags, CPoint point);
	afx_msg void OnTimeZoomDown ();
	afx_msg void OnTimeZoomUp ();
	afx_msg void OnKeyZoomDown ();
	afx_msg void OnKeyZoomUp ();
	afx_msg void OnVelZoomDown ();
	afx_msg void OnVelZoomUp ();
	afx_msg void OnVScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPianoRollPen ();
	afx_msg void OnUpdatePianoRollPenUI (CCmdUI* pCmdUI);
	afx_msg void OnPianoRollLine ();
	afx_msg void OnUpdatePianoRollLineUI (CCmdUI* pCmdUI);
	afx_msg void OnPianoRollEraser ();
	afx_msg void OnUpdatePianoRollEraserUI (CCmdUI* pCmdUI);
	afx_msg void OnPianoRollSelect ();
	afx_msg void OnUpdatePianoRollSelectUI (CCmdUI* pCmdUI);
	afx_msg void OnPianoRollSpeaker ();
	afx_msg void OnUpdatePianoRollSpeakerUI (CCmdUI* pCmdUI);
	afx_msg void OnPianoRollOnlyCurTrack ();
	afx_msg void OnUpdatePianoRollOnlyCurTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnPianoRollShowAllTrack ();
	afx_msg void OnUpdatePianoRollShowAllTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnPianoRollOnlyCurGraph ();
	afx_msg void OnUpdatePianoRollOnlyCurGraphUI (CCmdUI* pCmdUI);
	afx_msg void OnPianoRollShowAllGraph ();
	afx_msg void OnUpdatePianoRollShowAllGraphUI (CCmdUI* pCmdUI);
	afx_msg void OnPianoRollAutoPageUpdate ();
	afx_msg void OnUpdatePianoRollAutoPageUpdateUI (CCmdUI* pCmdUI);

	afx_msg void OnTrackComboSelEndOK ();	
	afx_msg void OnTrackListChkChange ();
	afx_msg void OnTrackListSelChange ();
	afx_msg void OnGraphKindComboSelEndOK ();	
	afx_msg void OnGraphKindListChkChange ();
	afx_msg void OnGraphKindListSelChange ();
	afx_msg void OnPopupEventProperty (); // 20100501追加
	afx_msg void OnUpdatePopupEventPropertyUI (CCmdUI* pCmdUI); // 20100501追加

	afx_msg void OnPopupTrackVisibleOn ();
	afx_msg void OnUpdatePopupTrackVisibleOnUI (CCmdUI* pCmdUI);
	afx_msg void OnPopupTrackVisibleOff ();
	afx_msg void OnUpdatePopupTrackVisibleOffUI (CCmdUI* pCmdUI);
	afx_msg void OnPopupTrackVisibleAll ();
	afx_msg void OnUpdatePopupTrackVisibleAllUI (CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};


#endif

