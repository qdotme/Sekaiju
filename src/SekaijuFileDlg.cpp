//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 世界樹ファイルダイアログクラス
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
#include <afxcmn.h>
#include <afxext.h>
#include <afxdlgs.h>
#include <dlgs.h>       // for standard control IDs for commdlg
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "SekaijuApp.h"
#include "SekaijuFileDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define new DEBUG_NEW


//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ1
CSekaijuFileDlg::CSekaijuFileDlg (BOOL bOpenFileDialog) : 
	//20091018修正:MFC8.0以前においては、次のデフォルトコンストラクタを使用する。
	CFileDialog (bOpenFileDialog) {
	//20091018修正:MFC9.0において、CFileDialogのm_bVistaStyleに強制的にFALSEを指定する。
	//CFileDialog (bOpenFileDialog, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	//	NULL, NULL, 0, FALSE) {;
}

// コンストラクタ2
CSekaijuFileDlg::CSekaijuFileDlg (BOOL bOpenFileDialog,
	LPCTSTR lpszDefExt, LPCTSTR lpszFileName, DWORD dwFlags,
	LPCTSTR lpszFilter, CWnd* pParentWnd) : 
	//20091018修正:MFC8.0以前においては、次のデフォルトコンストラクタを使用する。
	CFileDialog (bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd) {
	//20091018修正:MFC9.0において、CFileDialogのm_bVistaStyleに強制的にFALSEを指定する。
	//CFileDialog (bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd,
	//	0, FALSE) {
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// 注意事項：MSDN2002/4のOPENFILENAME構造体の説明
// For compatibility reasons, the Places Bar is hidden if Flags is set to 
// OFN_ENABLEHOOK and lStructSize is OPENFILENAME_SIZE_VERSION_400.
	
//int CSekaijuFileDlg::DoModal () {
	//m_ofn.Flags |= OFN_EXPLORER | OFN_ENABLEHOOK;
	//m_ofn.lpfnHook = (LPOFNHOOKPROC) SaveAsHookProc;
	//if (m_bOpenFileDialog) {
	//	return ::GetOpenFileName (&m_ofn);
	//}
	//else {
	//	return ::GetSaveFileName (&m_ofn);
	//}
//}


// ファイルタイプコンボボックスが変更されたとき
void CSekaijuFileDlg::OnTypeChange () {
	// 注意：m_ofn.lFlagsにOFN_EXPLOLER(0x00080000)を追加しておくこと。
	// 保存用ファイルダイアログのみ
	if (!m_bOpenFileDialog) {
		// ファイル名の拡張子をファイルタイプコンボボックス(0x047c)で選択した拡張子に書き換える。
		CString strFileTitle = GetFileTitle ();
		if (!strFileTitle.IsEmpty ()) {
			CString strExt;
			if (AfxExtractSubString (strExt, m_ofn.lpstrFilter, 2 * m_ofn.nFilterIndex - 1, (TCHAR)'\0')) {
				LPTSTR pszExtension = _tcschr ((LPTSTR)(LPCTSTR)strExt, _T('.'));//PathFindExtension (strExt);
				if (_tcscmp (pszExtension, _T(".*")) != 0 && TCSLEN (pszExtension) >= 2) {
					// 拡張子を入力しなかった場合に自動的に付加される拡張子を変更
					m_strDefExt = CString (pszExtension + 1);
					m_ofn.lpstrDefExt =  m_strDefExt;
					// ファイル名コンボボックスの拡張子を自動変更
					CString strText = strFileTitle + pszExtension;
					//if (!m_bVistaSytle) {
					SetControlText (0x047c, (LPSTR)(LPCTSTR)strText); 
					// 注意：0x047cはWindows2000/XPスタイルのファイルダイアログにおけるファイル名ID
					SetControlText (0x0480, (LPSTR)(LPCTSTR)strText); 
					// 注意：0x0480はWindows95/98/MEスタイルのファイルダイアログにおけるファイル名ID
					// TODO：ここにWindowsVistaスタイルのファイルダイアログにおけるファイル名IDを記述。
					// (名前をつけて保存ダイアログの中身のコントロールIDをspy++で調べる)
					//}
				}
			}
		}
	}
	CFileDialog::OnTypeChange ();
}

