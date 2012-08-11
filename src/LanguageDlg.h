//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 言語ダイアログクラス
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

#ifndef _LANGUAGEDLG_H_
#define _LANGUAGEDLG_H_

class CLanguageDlg : public CDialog {
	//--------------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------------
public:
	CString m_strLanguage;              // 言語名

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CLanguageDlg();                    // コンストラクタ
	enum {IDD = IDD_LANGUAGE};

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
	BOOL FillLanguageCombo ();

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual void DoDataExchange (CDataExchange* pDX);    // DDX/DDV のサポート
	virtual BOOL OnInitDialog ();

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	DECLARE_MESSAGE_MAP ()
};

#endif
