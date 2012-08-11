//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 譜面フレームウィンドウクラス
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



// 音符情報(ひとつのおたまじゃくしにつきひとつ確保)
typedef struct tagMusicalScoreNoteInfo {
	MIDIEvent* m_pNoteOnEvent;          // ノートオンイベントへのポインタ
	MIDIEvent* m_pNoteOffEvent;         // ノートオフイベントへのポインタ
	long m_lNoteOnTime;                 // ノートオン時刻[Tick]
	long m_lNoteOnMeasure;              // ノートオン小節
	long m_lNoteOnBeat;                 // ノートオン拍
	long m_lNoteOnTick;                 // ノートオンティック
	long m_lNoteOffTime;                // ノートオフ時刻[Tick]
	long m_lNoteOffMeasure;             // ノートオフ小節
	long m_lNoteOffBeat;                // ノートオフ拍
	long m_lNoteOffTick;                // ノートオフティック
	long m_lFlags;                      // フラグ
	long m_lSelected;                   // 選択状態
	struct tagMusicalScoreNoteInfo* m_pPrevNoteInfo; // 次の音符情報へのポインタ(なければNULL)
	struct tagMusicalScoreNoteInfo* m_pNextNoteInfo; // 前の音符情報へのポインタ(なければNULL)
	void* m_pNoteGroupInfo;             // 旗グループ情報へのポインタ(なければNULL)
	void* m_pTripletGroupInfo;          // 3連符グループ情報へのポインタ(なければNULL)
} MusicalScoreNoteInfo;

// 旗グループ情報(複数の音符を旗をつなげて束ねる情報)
typedef struct tagMusicalScoreNoteGroupInfo {
	MusicalScoreNoteInfo* m_pFirstNoteInfo; // 最初の音符情報へのポインタ
	MusicalScoreNoteInfo* m_pLastNoteInfo;  // 最後の音符情報へのポインタ
	long m_lBeginTime;                  // 開始時刻
	long m_lEndTime;                    // 終了時刻
	long m_lMinKey;                     // 最低音階(0-127)
	long m_lMaxKey;                     // 最高音階(0-127)
	long m_lMinDur;                     // 最短音長さ
	long m_lMaxDur;                     // 最長音長さ
	long m_lNumNoteInfo;                // グループ内の音符数
} MusicalScoreNoteGroupInfo;

// 3連符グループ情報(複数の音符を-3-でつなげて束ねる情報)
typedef struct tagMusicalScoreTripletGroupInfo {
	MusicalScoreNoteInfo* m_pFirstNoteInfo; // 最初の音符情報へのポインタ
	MusicalScoreNoteInfo* m_pLastNoteInfo;  // 最後の音符情報へのポインタ
	long m_lBeginTime;                  // 開始時刻
	long m_lEndTime;                    // 終了時刻
	long m_lMinKey;                     // 最低音階(0-127)
	long m_lMaxKey;                     // 最高音階(0-127)
	long m_lMinDur;                     // 最短音長さ
	long m_lMaxDur;                     // 最長音長さ
	long m_lNumNoteInfo;                // グループ内の音符数
} MusicalScoreTripletGroupInfo;

// トラック情報
typedef struct {
	long m_lTop;                        // 上端座標[pixel]
	long m_lHeight;                     // 高さ[pixel]
	long m_lFlags;                      // フラグ
	long m_lMinKey;                     // トラック内最低音階(0-127)
	long m_lMaxKey;	                    // トラック内最高音階(0-127)
	CPtrArray m_theNoteInfoArray;       // 音符情報の配列
	CPtrArray m_theNoteGroupInfoArray;  // 音符グループ情報の配列
	CPtrArray m_theTripletGroupInfoArray; // 3連音符グループ情報の配列
	long m_lTopPrint;                   // 上端座標(印刷用)[*1/10mm]
	long m_lHeightPrint;                // 高さ(印刷用)[*1/10mm]
} MusicalScoreTrackInfo;

// 小節情報
typedef struct {
	long m_lTime;                       // この小節の開始タイム[Tick][Subframe]
	long m_lDuration;                   // この小節にの長さ[Tick][Subframe]
	long m_lLeft;                       // この小節の左座標[pixel]
	long m_lSignatureWidth;             // この小節の拍子記号・調性記号用幅[pixel]
	long m_lPreWidth;                   // この小節の左余白幅[pixel]
	long m_lWidth;                      // この小節の幅[pixel]
	long m_lPostWidth;                  // この小節の右余白幅[pixel]
	long m_lFlags;                      // フラグ
	long m_lTimeSignature;              // この小節の拍子記号
	long m_lKeySignature;               // この小節の調性記号
	long m_lLeftPrint;                  // この小節の左座標(印刷用)[*1/10mm]
	long m_lSignatureWidthPrint;        // この小節の拍子記号・調性記号用幅(印刷用)[*1/10mm]
	long m_lPreWidthPrint;              // この小節の左余白幅(印刷用)[*1/10mm]
	long m_lWidthPrint;                 // この小節の幅(印刷用)[*1/10mm] 
	long m_lPostWidthPrint;             // この小節の右余白幅[pixel](印刷用)[*1/10mm]
} MusicalScoreMeasureInfo;




class CMusicalScoreFrame : public CChildFrame {
	DECLARE_DYNCREATE (CMusicalScoreFrame)

	//-------------------------------------------------------------------------
	// アトリビュート
	//-------------------------------------------------------------------------
protected:
	long m_lToolBar1Height;             // ツールバー1の高さ[pixel]
	long m_lKeyHeight;                  // トラック番号-時間ビューの高さ[pixel]
	long m_lScaleHeight;                // 上部目盛りビューの高さ[pixel]
	long m_lScaleWidth;                 // 左部目盛りビューの高さ[pixel]
	long m_lTimeWidth;                  // トラック番号ー-時間ビューの幅[pixel]
	long m_lHScrollBarHeight;           // 水平スクロールバーの高さ[pixel]
	long m_lVScrollBarWidth;            // 垂直スクロールバーの高さ[pixel]
	long m_lTrackListWidth;             // トラックリストボックスの幅[pixel]
	long m_lTrackZoom;                  // トラック方向拡大倍率[倍]
	long m_lTimeZoom;                   // 時間方向拡大倍率[倍]
	long m_lTrackScrollPos;             // トラック方向スクロール位置[pixel]
	long m_lTimeScrollPos;              // 時間方向スクロール位置[pixel]
public:
	// 描画補助情報
	CPtrArray   m_theTrackInfoArray;    // トラックごとの情報
	CPtrArray   m_theMeasureInfoArray;  // 小節ごとの情報

public:
	CFont m_theFont;                    // 譜面ウィンドウで使うフォント
	CFont m_theTimeMeasureFont;         // 拍子記号・調性記号用のフォント
	CSekaijuToolBar m_wndToolBar1;      // ツールバー1(上)
	CColorfulComboBox m_wndTrackCombo;  // ツールバー上のトラックコンボボックス
	CComboBox m_wndChannelCombo;        // ツールバー上のチャンネルコンボボックス
	CComboBox m_wndSnapCombo;           // ツールバー上のスナップコンボボックス
	CComboBox m_wndVelocityCombo;       // ツールバー上のベロシティコンボボックス
	CComboBox m_wndDurationCombo;       // ツールバー上の音長さコンボボックス
	CComboBox m_wndResolutionCombo;     // ツールバー上の表示精度コンボボックス
	CView* m_pDummyView;                // ダミービュー(Visible=FALSE)へのポインタ
	CView* m_pPrintView;                // 印刷用ビュー(Visible=FALSE)へのポインタ
	CView* m_pScaleView;                // 目盛りビューへのポインタ
	CView* m_pTimeScaleView;            // 時間目盛りビューへのポインタ
	CView* m_pTrackScaleView;           // トラック番号目盛りビューへのポインタ
	CView* m_pTrackTimeView;            // トラック番号-タイムビューへのポインタ
	CColorfulCheckListBox m_wndTrackList;  // トラックリストコンボボックス
	CScrollBar m_wndTimeScroll;         // 時間方向スクロールバー
	CScrollBar m_wndTrackScroll;        // トラック方向スクロールバー
	CScrollBar m_wndSizeScroll;         // サイズ用スクロールバー
	CBitmapButton m_wndTimeZoomUp;      // 時間方向拡大ボタン
	CBitmapButton m_wndTimeZoomDown;    // 時間方向縮小ボタン
	CBitmapButton m_wndTrackZoomUp;     // トラック方向拡大ボタン
	CBitmapButton m_wndTrackZoomDown;   // トラック方向縮小ボタン

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

	//-------------------------------------------------------------------------
	// 構築と破壊
	//-------------------------------------------------------------------------
public:
	CMusicalScoreFrame ();              // コンストラクタ
	virtual ~CMusicalScoreFrame ();     // デストラクタ

	//-------------------------------------------------------------------------
	// オペレーション
	//-------------------------------------------------------------------------
public:
	virtual CSekaijuDoc* GetDocument ();
	virtual long GetTimeZoom ();
	virtual long GetTrackZoom ();
	virtual long XtoTime (long x);
	virtual long TimetoX (long lTime);
	virtual long MeasuretoX (long lMeasure);
	virtual long MeasuretoX2 (long lMeasure);
	virtual long YtoTrackIndex (long y);
	virtual long TrackIndexYtoKey (long lTrackIndex, long y, long lKeySignature);
	virtual long KeytoLineNo (long lKey, long lKeySignature);
	virtual long KeytoSF (long lKey, long lKeySignature);
	virtual long TrackIndexLineNotoY (long lTrackIndex, long lLineNo);
	virtual long TrackIndexKeytoY (long lTrackIndex, long lKey, long lKeySignature);
	virtual long GetTimeScrollPos ();
	virtual long GetTrackScrollPos ();
	virtual long SetTimeScrollPos (long lTimeScrollPos);
	virtual long SetTrackScrollPos (long lTrackScrollPos);
	virtual long GetVisibleLeftTime ();
	virtual long GetVisibleRightTime ();
	virtual long GetVisibleTopTrack ();
	virtual long GetVisibleBottomTrack ();
	virtual void DrawSplitterCaptor (CDC* pDC, CPoint pt);
	virtual long GetCurTrackIndex ();
	virtual long GetCurChannel ();
	virtual long GetCurSnap ();
	virtual long GetCurVelocity ();
	virtual long GetCurDuration ();
	virtual long GetCurResolution ();
	virtual BOOL SetCurTrackIndex (long lCurTrackIndex);
	virtual BOOL SetCurChannel (long lCurChannel);
	virtual BOOL SetCurSnap (long lCurSnap);
	virtual BOOL SetCurVelocity (long lCurVelocity);
	virtual BOOL SetCurDuration (long lCurDuration);
	virtual BOOL SetCurResolution (long lCurResolution);
	virtual BOOL IsTrackVisible (long lTrackIndex);
	virtual BOOL SetTrackVisible (long lTrackIndex);
	virtual BOOL UpdateTrackCombo ();
	virtual BOOL UpdateTrackList ();
	virtual BOOL UpdateSnapCombo ();
	virtual BOOL UpdateVelocityCombo ();
	virtual BOOL UpdateDurationCombo ();
	virtual BOOL UpdateResolutionCombo ();
	virtual void RecalcTrackScrollInfo ();
	virtual void RecalcTimeScrollInfo ();


	virtual BOOL DeleteTrackInfoArray ();
	virtual BOOL DeleteMeasureInfoArray ();
	virtual BOOL DeleteNoteInfoArray (long lTrackIndex);
	virtual BOOL DeleteNoteGroupInfoArray (long lTrackIndex);
	virtual BOOL DeleteTripletGroupInfoArray (long lTrackIndex);
	virtual BOOL UpdateMeasureInfoArray ();
	virtual BOOL UpdateTrackInfoArray ();
	virtual BOOL UpdateNoteInfoArray (long lTrackIndex, MIDITrack* pMIDITrack);
	virtual BOOL UpdateNoteGroupInfoArray (long lTrackIndex, MIDITrack* pMIDITrack);
	virtual BOOL UpdateTripletGroupInfoArray (long lTrackIndex, MIDITrack* pMIDITrack);
	virtual MusicalScoreNoteInfo* CreateNote
		(MIDIEvent* pNoteEvent, long lNoteOnTime, long lNoteOnMeasure, long lNoteOnBeat, long lNoteOnTick, 
		long lNoteOffTime, long lNoteOffMeasure, long lNoteOffBeat, long lNoteOffTick, long lFlags);
	virtual void DeleteNoteInfo (MusicalScoreNoteInfo* pNoteInfo);
	virtual BOOL AddNoteInfo (MusicalScoreTrackInfo* pTrackInfo, MusicalScoreNoteInfo* pNoteInfo);
	MusicalScoreNoteGroupInfo* CreateNoteGroupInfo (MusicalScoreNoteInfo* pNoteInfo);
	void DeleteNoteGroupInfo (MusicalScoreNoteGroupInfo* pNoteGroupInfo);
	BOOL AddNoteGroupInfo (MusicalScoreTrackInfo* pTrackInfo, MusicalScoreNoteGroupInfo* pNoteGroupInfo);
	MusicalScoreTripletGroupInfo* CreateTripletGroupInfo (MusicalScoreNoteInfo* pNoteInfo);
	void DeleteTripletGroupInfo (MusicalScoreTripletGroupInfo* pTripletGroupInfo);
	BOOL AddTripletGroupInfo (MusicalScoreTrackInfo* pTrackInfo, MusicalScoreTripletGroupInfo* pNoteGroupInfo);

	virtual long GetTrackInfoCount ()
		{return m_theTrackInfoArray.GetSize ();} 
	virtual MusicalScoreTrackInfo* GetTrackInfo (long lTrackIndex)
		{return lTrackIndex < m_theTrackInfoArray.GetSize () ? 
		((MusicalScoreTrackInfo*)m_theTrackInfoArray.GetAt (lTrackIndex)) : NULL;}

	virtual long GetMeasureInfoCount ()
		{return m_theMeasureInfoArray.GetSize ();}
	virtual MusicalScoreMeasureInfo* GetMeasureInfo (long lMeasureIndex)
		{return lMeasureIndex < m_theMeasureInfoArray.GetSize () ?
		((MusicalScoreMeasureInfo*)m_theMeasureInfoArray.GetAt (lMeasureIndex)) : NULL;}
	virtual long GetMeasureTime (long lMeasureIndex)
		{return lMeasureIndex < m_theMeasureInfoArray.GetSize () ?
		((MusicalScoreMeasureInfo*)m_theMeasureInfoArray.GetAt (lMeasureIndex))->m_lTime : 0;}
	virtual long GetMeasureLeft (long lMeasureIndex)
		{return lMeasureIndex < m_theMeasureInfoArray.GetSize () ?
		((MusicalScoreMeasureInfo*)m_theMeasureInfoArray.GetAt (lMeasureIndex))->m_lLeft : 0;}


	//-------------------------------------------------------------------------
	// オーバーライド
	//-------------------------------------------------------------------------
protected:
	virtual BOOL PreCreateWindow (CREATESTRUCT& cs);
	virtual void OnUpdateFrameTitle (BOOL bAddToTitle);
	virtual void RecalcLayout (BOOL bNotify = TRUE);
	virtual BOOL OnCreateClient (LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL OnCmdMsg (UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	//-------------------------------------------------------------------------
	// メッセージマップ
	//-------------------------------------------------------------------------
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
	afx_msg void OnTrackZoomDown ();
	afx_msg void OnTrackZoomUp ();
	afx_msg void OnVScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMusicalScorePen ();
	afx_msg void OnUpdateMusicalScorePenUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreLine ();
	afx_msg void OnUpdateMusicalScoreLineUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreEraser ();
	afx_msg void OnUpdateMusicalScoreEraserUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreSelect ();
	afx_msg void OnUpdateMusicalScoreSelectUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreSpeaker ();
	afx_msg void OnUpdateMusicalScoreSpeakerUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreWholeNote ();
	afx_msg void OnUpdateMusicalScoreWholeNoteUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreHalfNote ();
	afx_msg void OnUpdateMusicalScoreHalfNoteUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreQuarterNote ();
	afx_msg void OnUpdateMusicalScoreQuarterNoteUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreQuaverNote ();
	afx_msg void OnUpdateMusicalScoreQuaverNoteUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreSemiQuaverNote ();
	afx_msg void OnUpdateMusicalScoreSemiQuaverNoteUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreDemiSemiQuaverNote ();
	afx_msg void OnUpdateMusicalScoreDemiSemiQuaverNoteUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreDotted ();
	afx_msg void OnUpdateMusicalScoreDottedUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreTriplet ();
	afx_msg void OnUpdateMusicalScoreTripletUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreOnlyCurTrack ();
	afx_msg void OnUpdateMusicalScoreOnlyCurTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreShowAllTrack ();
	afx_msg void OnUpdateMusicalScoreShowAllTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreAutoPageUpdate ();
	afx_msg void OnUpdateMusicalScoreAutoPageUpdateUI (CCmdUI* pCmdUI);

	afx_msg void OnTrackComboSelEndOK ();	
	afx_msg void OnTrackListChkChange ();
	afx_msg void OnTrackListSelChange ();
	afx_msg void OnSnapComboSelEndOK ();
	afx_msg void OnResolutionComboSelEndOK ();

	afx_msg void OnPopupTrackVisibleOn ();
	afx_msg void OnUpdatePopupTrackVisibleOnUI (CCmdUI* pCmdUI);
	afx_msg void OnPopupTrackVisibleOff ();
	afx_msg void OnUpdatePopupTrackVisibleOffUI (CCmdUI* pCmdUI);
	afx_msg void OnPopupTrackVisibleAll ();
	afx_msg void OnUpdatePopupTrackVisibleAllUI (CCmdUI* pCmdUI);
	afx_msg void OnPopupEventProperty ();
	afx_msg void OnUpdatePopupEventPropertyUI (CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};


