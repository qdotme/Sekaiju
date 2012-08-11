//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MDI親フレームウィンドウクラス
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

#ifndef _MAINFRAME_H_
#define _MAINFRAME_H_

class CMainFrame : public CMDIFrameWnd {
	DECLARE_DYNAMIC (CMainFrame)

	//--------------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------------
public:
	CString     m_strWndClass;          // ウィンドウクラス名
	CString     m_strTempFileName;      // 一時的なファイル名格納場所
		
	//--------------------------------------------------------------------------
	// ツールバーとステータスバーとその内部のウィンドウ
	//--------------------------------------------------------------------------
public:  
	CStatusBar  m_wndStatusBar;         // ステータスバー
	CSekaijuToolBar m_wndToolBar1;      // ツールバー1
	CSekaijuToolBar m_wndToolBar2;      // ツールバー2
	CSekaijuToolBar m_wndToolBar3;      // ツールバー3(未使用)
	CEdit       m_wndMillisecEdit;      // 時:分:秒:ミリ秒用エディット(読み取り専用)
	CEdit       m_wndTimeEdit;          // 小節:拍:ティックorフレーム:サブフレーム用エディット(読み取り専用)
	CScrollBar  m_wndPositionScroll;    // 位置スクロールバー
	CEdit       m_wndMeasureEdit;       // 拍子記号・調性記号用エディット(読み取り専用)
	CEdit       m_wndTempoEdit;         // テンポ用エディット(読み取り専用)

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CMainFrame ();                      // コンストラクタ
	virtual ~CMainFrame();              // デストラクタ

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	void SetPositionScrollRange (long lStartTime, long lEndTime, BOOL bRedraw);
	int SetWindowTextWhenDifferent (CWnd* pWnd, LPCTSTR lpszText);

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
public:
	virtual CFrameWnd* GetActiveFrame ();
protected:
	virtual BOOL PreCreateWindow (CREATESTRUCT& cs);


	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy ();
	afx_msg void OnTimer (UINT nIDEvent);
	afx_msg void OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseWheel40 (UINT nFlags, CPoint point);
	afx_msg void OnViewToolbar1 ();
	afx_msg void OnUpdateViewToolbar1UI (CCmdUI* pCmdUI);
	afx_msg void OnViewToolbar2 ();
	afx_msg void OnUpdateViewToolbar2UI (CCmdUI* pCmdUI);
	afx_msg long OnCommandWakeUp (WPARAM wParam, LPARAM lParam);
	afx_msg long OnCommandReadShm (WPARAM wParam, LPARAM lParam);
	afx_msg long OnCommandFileOpen (WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP ()

};

#endif
