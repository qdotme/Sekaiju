//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MDI子フレームウィンドウクラス
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

#include "winver.h"
#include <afxwin.h>
#include <afxext.h>
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "SekaijuApp.h"
#include "SekaijuToolBar.h"
#include "MainFrame.h"
#include "ChildFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE (CChildFrame, CMDIChildWnd)

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CChildFrame::CChildFrame () {
}

// デストラクタ
CChildFrame::~CChildFrame () {

}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// ドキュメントへのポインタ取得
CDocument* CChildFrame::GetDocument () {
	CView* pActiveView = GetActiveView ();
	if (pActiveView) {
		return pActiveView->GetDocument ();
	}
	else {
		return NULL;
	}
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// ウィンドウが破棄された後
void CChildFrame::PostNcDestroy () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	pSekaijuApp->UpdateCurWndAndDocPtr ();
	CMDIChildWnd::PostNcDestroy ();
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CChildFrame, CMDIChildWnd)
	ON_WM_MDIACTIVATE ()
END_MESSAGE_MAP ()

// ウィンドウがアクティブになったとき
void CChildFrame::OnMDIActivate (BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (bActivate == TRUE) {
		pSekaijuApp->UpdateCurWndAndDocPtr ();
	}
	CMDIChildWnd::OnMDIActivate (bActivate, pActivateWnd, pDeactivateWnd);
}


