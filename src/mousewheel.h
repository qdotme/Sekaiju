//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// マウスホイール対応ヘッダーファイル(VisualC++4.0用)
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

#ifndef _MOUSEWHEEL_H_
#define _MOUSEWHEEL_H_

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#endif

#ifndef WHEELDELTA
#define WHEELDELTA 120
#endif

#ifndef ON_WM_MOUSEWHEEL40
#define ON_WM_MOUSEWHEEL40() \
	{ WM_MOUSEWHEEL, 0, 0, 0, AfxSig_vwp, \
		(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))OnMouseWheel40 },
#endif

#endif
