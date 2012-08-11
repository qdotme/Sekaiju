//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 世界樹ドキュメントマネージャークラス
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
#include "resource.h"
#include "SekaijuDocManager.h"
#include "SekaijuFileDlg.h"

IMPLEMENT_DYNAMIC (CSekaijuDocManager, CDocManager)

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// CDocManager::DoPromptFileNameのオーバーライド
BOOL CSekaijuDocManager::DoPromptFileName 
(CString& strFileName, UINT nIDSTitle, DWORD lFlags, 
 BOOL bOpenFileDialog, CDocTemplate* pTemplate) {

	CSekaijuFileDlg theFileDlg (bOpenFileDialog);

	CString strTitle;
	VERIFY (strTitle.LoadString (nIDSTitle));

	// 注意事項：MSDN2002/4のOPENFILENAME構造体の説明
	// For compatibility reasons, the Places Bar is hidden if Flags is set to 
	// OFN_ENABLEHOOK and lStructSize is OPENFILENAME_SIZE_VERSION_400.
	// CFileDialogクラスでは強制的にOFN_ENABLEHOOKでAfxCommDlgProcにフックする。

	// 20081028 : m_ofnの値の設定(76or88)は次による
	//#ifndef OPENFILENAME_SIZE_VERSION_400
	//    theFileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME); //=76(Windows95/98/ME style)
	//#else
	//    theFileDlg.m_ofn.lStructSize = OPENFILENAME_SIZE_VERSION_400; //=76(Windows95/98/ME style)
	//#endif

	#if (_WIN32_WINNT >= 0x0500)
	    theFileDlg.m_ofn.lStructSize = 88; //=88(With placebar)
		theFileDlg.m_ofn.pvReserved = NULL;
		theFileDlg.m_ofn.dwReserved = 0;
		theFileDlg.m_ofn.FlagsEx = 0;
	#else
	    theFileDlg.m_ofn.lStructSize = 76; //=76(Without placebar if OFN_ENABLEHOOK used)
	#endif

	theFileDlg.m_ofn.nMaxFileTitle = _MAX_PATH;
	
	theFileDlg.m_ofn.Flags |= lFlags;

	// 拡張子フィルター
	CString strFilter;
	CString strDefault;
	CString strFilterAllMIDI[3];
	CString strFilterSKJ[3];
	CString strFilterCHY[3];
	CString strFilterMID[3];
	CString strFilterCSV[3];
	CString strFilterVSQ[3];
	CString strFilterAll[3];
	VERIFY (strFilterAllMIDI[0].LoadString (IDS_ALL_MIDI_SEQUENCE_AD_SKJ_AD_CHY_AD_MID_AD_VSQ));
	VERIFY (strFilterAllMIDI[1].LoadString (IDS_AD_SKJ_AD_CHY_AD_MID_AD_VSQ));
	VERIFY (strFilterSKJ[0].LoadString (IDS_SEKAIJU_SEQUENCE_FILES_AD_SKJ));
	VERIFY (strFilterSKJ[1].LoadString (IDS_AD_SKJ));
	VERIFY (strFilterSKJ[2].LoadString (IDS_D_SKJ));
	VERIFY (strFilterCHY[0].LoadString (IDS_CHERRY_SEQUENCE_FILES_AD_CHY));
	VERIFY (strFilterCHY[1].LoadString (IDS_AD_CHY));
	VERIFY (strFilterCHY[2].LoadString (IDS_D_CHY));
	VERIFY (strFilterMID[0].LoadString (IDS_STANDARD_MIDI_FILES_AD_MID));
	VERIFY (strFilterMID[1].LoadString (IDS_AD_MID));
	VERIFY (strFilterMID[2].LoadString (IDS_D_MID));
	VERIFY (strFilterCSV[0].LoadString (IDS_MIDI_CSV_FILES_AD_CSV));
	VERIFY (strFilterCSV[1].LoadString (IDS_AD_CSV));
	VERIFY (strFilterCSV[2].LoadString (IDS_D_CSV));
	VERIFY (strFilterVSQ[0].LoadString (IDS_VOCALOID2_FILES_AD_VSQ));
	VERIFY (strFilterVSQ[1].LoadString (IDS_AD_VSQ));
	VERIFY (strFilterVSQ[2].LoadString (IDS_D_VSQ));
	VERIFY (strFilterAll[0].LoadString (IDS_ALL_FILES_AD_A));
	VERIFY (strFilterAll[1].LoadString (IDS_AD_A));

	// 拡張子フィルター追加設定
	if (bOpenFileDialog == TRUE) {
		strFilter += strFilterAllMIDI[0]; // すべてのMIDIシーケンス(*.skj;*.chy;*.mid;*.csv;*.vsq)
		strFilter += (TCHAR)_T('\0');
		strFilter += strFilterAllMIDI[1]; // *.skj;*.chy;*.mid;*.csv;*.vsq
		strFilter += (TCHAR)_T('\0');
	}
	strFilter += strFilterSKJ[0]; // すべてのMIDIシーケンス(*.skj)
	strFilter += (TCHAR)_T('\0');
	strFilter += strFilterSKJ[1]; // *.skj
	strFilter += (TCHAR)_T('\0');
	strFilter += strFilterCHY[0]; // Cherryシーケンスファイル(*.chy)
	strFilter += (TCHAR)_T('\0');
	strFilter += strFilterCHY[1]; // *.chy
	strFilter += (TCHAR)_T('\0');
	strFilter += strFilterMID[0]; // スタンダードMIDIファイル(*.mid)
	strFilter += (TCHAR)_T('\0');
	strFilter += strFilterMID[1]; // *.mid
	strFilter += (TCHAR)_T('\0');
	strFilter += strFilterCSV[0]; // MIDICSVファイル(*.csv)
	strFilter += (TCHAR)_T('\0');
	strFilter += strFilterCSV[1]; // *.csv
	strFilter += (TCHAR)_T('\0');
	strFilter += strFilterVSQ[0]; // vocaloid2ファイル(*.vsq)
	strFilter += (TCHAR)_T('\0');
	strFilter += strFilterVSQ[1]; // *.vsq
	strFilter += (TCHAR)_T('\0');
	strFilter += strFilterAll[0]; // すべてのファイル(*.*)
	strFilter += (TCHAR)_T('\0');
	strFilter += strFilterAll[1]; // *.*
	strFilter += (TCHAR)_T('\0');
	strFilter += (TCHAR)_T('\0');
	theFileDlg.m_ofn.nMaxCustFilter = 1024;

	theFileDlg.m_ofn.lpstrFilter = strFilter;
#ifndef _MAC
	theFileDlg.m_ofn.lpstrTitle = strTitle;
#else
	theFileDlg.m_ofn.lpstrPrompt = strTitle;
#endif

	// デフォルトの拡張子を設定
	theFileDlg.m_strDefExt = strFilterSKJ[1]; // *.skj
	theFileDlg.m_ofn.lpstrDefExt = theFileDlg.m_strDefExt;

	// フィルターコンボボックスのデフォルト選択(20100613修正)
	CString strExt = strFileName.Right (4);
	if (strExt.CompareNoCase (strFilterSKJ[1].Right(4)) == 0) { // *.skj
		theFileDlg.m_ofn.nFilterIndex = 1;
	}
	else if (strExt.CompareNoCase (strFilterCHY[1].Right(4)) == 0) { // *.chy
		theFileDlg.m_ofn.nFilterIndex = 2;
	}
	else if (strExt.CompareNoCase (strFilterMID[1].Right(4)) == 0) { // *.mid
		theFileDlg.m_ofn.nFilterIndex = 3;
	}
	else if (strExt.CompareNoCase (strFilterCSV[1].Right(4)) == 0) { // *.csv
		theFileDlg.m_ofn.nFilterIndex = 4;
	}
	else if (strExt.CompareNoCase (strFilterVSQ[1].Right(4)) == 0) { // *.vsq
		theFileDlg.m_ofn.nFilterIndex = 5;
	}
	
	theFileDlg.m_ofn.lpstrFile = strFileName.GetBuffer(_MAX_PATH);

	// ファイルダイアログ.DoModal
	BOOL bResult = theFileDlg.DoModal() == IDOK ? TRUE : FALSE;

	strFileName.ReleaseBuffer();

	// 拡張子が付いていない場合は、選択したファイルタイプの拡張子を自動的に付ける
	if (bOpenFileDialog == FALSE) {
		if (theFileDlg.m_ofn.nFilterIndex == 1 &&
			(strFileName.Right (4)).CompareNoCase (strFilterSKJ[2]) != 0) { // .skj
			strFileName += strFilterSKJ[2];
		}
		else if (theFileDlg.m_ofn.nFilterIndex == 2 &&
			(strFileName.Right (4)).CompareNoCase (strFilterCHY[2]) != 0) { // .chy
			strFileName += strFilterCHY[2];
		}
		else if (theFileDlg.m_ofn.nFilterIndex == 3 &&
			(strFileName.Right (4)).CompareNoCase (strFilterMID[2]) != 0) { // .mid
			strFileName += strFilterMID[2];
		}
		else if (theFileDlg.m_ofn.nFilterIndex == 4 &&
			(strFileName.Right (4)).CompareNoCase (strFilterCSV[2]) != 0) { // .csv
			strFileName += strFilterCSV[2];
		}
		else if (theFileDlg.m_ofn.nFilterIndex == 5 &&
			(strFileName.Right (4)).CompareNoCase (strFilterVSQ[2]) != 0) { // .vsq
			strFileName += strFilterVSQ[2];
		}
	}

	return bResult;
}
