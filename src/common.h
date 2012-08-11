//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 共用ルーチン
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

#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

// 数字として認識できる文字列か調べる(戻り値:認識できた数字の文字数)
long IsNumeric (const TCHAR* pszText);

/* 16進法で使用可能な文字なら1を、そうでなければ0を返す */
long IsHexDigit (TCHAR c);

/* バイナリ配列を16進テキスト文字列(半角スペース区切り)に変換する */
long bin2txt (BYTE* pBin, long lBinLen, TCHAR* pszText, long lTextLen);

/* 16進テキスト文字列(半角スペース区切り)をバイナリ配列に変換する */
long txt2bin (TCHAR* pszText, long lTextLen, BYTE* pBin, long lBinLen);

/* '\t''\r''\n''\\'コード入りの文字列をコード無しの文字列(C言語スタイル)に変換する　*/
/* 戻り値：出来上がった文字列のバイト数 */
long codestr2str (TCHAR* pszCodeStr, long lCodeStrLen, TCHAR* pszStr, long lStrLen);

/* コード無しの文字列(C言語スタイル)を'\t''\r''\n''\\'コード入りの文字列に変換する */
/* 戻り値：出来上がった文字列のバイト数 */
long str2codestr (TCHAR* pszStr, long lStrLen, TCHAR* pszCodeStr, long lCodeStrLen);


#ifdef __cplusplus
}
#endif

#endif
