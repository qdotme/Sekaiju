//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 履歴ユニットクラス
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
#include <afxmt.h>
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuDoc.h"

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CHistoryUnit::CHistoryUnit () {
	m_strName = _T("");
}

// デストラクタ
CHistoryUnit::~CHistoryUnit () {
	long i;
	long lHistoryRecordCount = m_theHistoryRecordArray.GetSize ();
	for (i = lHistoryRecordCount - 1; i >= 0; i--) {
		MIDIEvent* pMIDIEvent = NULL;
		MIDITrack * pMIDITrack = NULL;
		MIDIData* pMIDIData = NULL;
		CHistoryRecord* pHistoryRecord = (CHistoryRecord*)(m_theHistoryRecordArray.GetAt(i));
		switch (pHistoryRecord->m_lType) {
		case HISTORYRECORD_INSERTEVENT:
			pMIDIEvent = (MIDIEvent*)(pHistoryRecord->m_pObject);
			// ドキュメントで使われていないイベントは実体を削除
			if ((pMIDIEvent->m_lUserFlag & MIDIEVENT_ALIVE) == 0) {
				MIDIEvent_DeleteSingle (pMIDIEvent);
				pMIDIEvent = NULL;
			}
			break; // 20080714break忘れ修正
		case HISTORYRECORD_REMOVEEVENT:
			pMIDIEvent = (MIDIEvent*)(pHistoryRecord->m_pObject);
			// ドキュメントで使われていないイベントは実体を削除
			if ((pMIDIEvent->m_lUserFlag & MIDIEVENT_ALIVE) == 0 &&
				(pMIDIEvent->m_lUserFlag & MIDIEVENT_RESISTEREDASINSERTED) == 0) {
				MIDIEvent_DeleteSingle (pMIDIEvent);
				pMIDIEvent = NULL;
			}
			break;
		case HISTORYRECORD_INSERTTRACK:
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pObject);
			// ドキュメントで使われていないトラックは実体を削除
			if ((pMIDITrack->m_lUserFlag & MIDITRACK_ALIVE) == 0) {
				MIDITrack_Delete (pMIDITrack);
				pMIDITrack = NULL;
			}
			break; // 20080714break忘れ修正
		case HISTORYRECORD_REMOVETRACK:
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pObject);
			// ドキュメントで使われていないトラックは実体を削除
			if ((pMIDITrack->m_lUserFlag & MIDITRACK_ALIVE) == 0 &&
				(pMIDITrack->m_lUserFlag & MIDITRACK_RESISTEREDASINSERTED) == 0) {
				MIDITrack_Delete (pMIDITrack);
				pMIDITrack = NULL;
			}
			break;
		case HISTORYRECORD_INSERTDATA:
			pMIDIData = (MIDIData*)(pHistoryRecord->m_pObject);
			// ドキュメントで使われていないMIDIDataオブジェクトは実体を削除
			if ((pMIDIData->m_lUserFlag & MIDIDATA_ALIVE) == 0) {
				MIDIData_Delete (pMIDIData);
				pMIDIData = NULL;
			}
			break;
		case HISTORYRECORD_REMOVEDATA:
			pMIDIData = (MIDIData*)(pHistoryRecord->m_pObject);
			// ドキュメントで使われていないMIDIDataオブジェクトは実体を削除
			if ((pMIDIData->m_lUserFlag & MIDIDATA_ALIVE) == 0 &&
				(pMIDIData->m_lUserFlag & MIDIDATA_RESISTEREDASINSERTED) == 0) {
				MIDIData_Delete (pMIDIData);
				pMIDIData = NULL;
			}
			break;
		}
		delete pHistoryRecord;
	}
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// この履歴ユニットに履歴記録をひとつ追加
long CHistoryUnit::AddHistoryRecord (long lType, void* pObject) {
	long lCount = 0;
	MIDIEvent* pMIDIEvent = NULL;
	MIDIEvent* pNextEvent = NULL;
	MIDIEvent* pPrevEvent = NULL;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pTempTrack = NULL;
	MIDIData* pMIDIData = NULL;
	CHistoryRecord* pHistoryRecord = NULL;
	switch (lType) {
	// MIDIEventオブジェクト挿入の記録
	case HISTORYRECORD_INSERTEVENT:
		pMIDIEvent = (MIDIEvent*)pObject;
		pMIDIEvent = MIDIEvent_GetFirstCombinedEvent (pMIDIEvent);
		ASSERT (pMIDIEvent);
		while (pMIDIEvent) {
			ASSERT (pMIDIEvent->m_pParent);
			pHistoryRecord = new CHistoryRecord ();
			pHistoryRecord->m_lType = lType;
			pHistoryRecord->m_pObject = pMIDIEvent;
			pHistoryRecord->m_pFirstChild = NULL;
			pHistoryRecord->m_pLastChild = NULL;
			pNextEvent = pMIDIEvent->m_pNextEvent;
			while (pNextEvent) {
				if (pNextEvent->m_lUserFlag & MIDIEVENT_ALIVE) {
					break;
				}
				pNextEvent = pNextEvent->m_pNextEvent;
			}
			pHistoryRecord->m_pNextObject = pNextEvent;
			pPrevEvent = pMIDIEvent->m_pPrevEvent;
			while (pPrevEvent) {
				if (pPrevEvent->m_lUserFlag & MIDIEVENT_ALIVE) {
					break;
				}
				pPrevEvent = pPrevEvent->m_pPrevEvent;
			}
			pHistoryRecord->m_pPrevObject = pPrevEvent;
			pHistoryRecord->m_pParent = pMIDIEvent->m_pParent;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_RESISTEREDASINSERTED;
			//_RPTF4 (_CRT_WARN, "HISTORYRECORD_INSERTEVENT (%08x(%08x), prev=%08x, next=%08x)\n",
			//		(long)pMIDIEvent, pMIDIEvent->m_lData, (long)pPrevEvent, (long)pNextEvent);
			//FILE* pFile = fopen ("history.txt", "at");
			//fprintf (pFile, "HISTORYRECORD_INSERTEVENT (%08x(%08x), prev=%08x, next=%08x)\n",
			//	(long)pMIDIEvent, pMIDIEvent->m_lData, (long)pPrevEvent, (long)pNextEvent);
			//fclose (pFile);
			
			m_theHistoryRecordArray.Add (pHistoryRecord);
			pMIDIEvent = pMIDIEvent->m_pNextCombinedEvent;
			lCount++;
		}
		break;
	// MIDIEventオブジェクト除去の記録
	case HISTORYRECORD_REMOVEEVENT:
		pMIDIEvent = (MIDIEvent*)pObject;
		pMIDIEvent = MIDIEvent_GetFirstCombinedEvent (pMIDIEvent);
		ASSERT (pMIDIEvent);
		while (pMIDIEvent) {
			ASSERT (pMIDIEvent->m_pParent);
			pHistoryRecord = new CHistoryRecord ();
			pHistoryRecord->m_lType = lType;
			pHistoryRecord->m_pObject = pMIDIEvent;
			pHistoryRecord->m_pFirstChild = NULL;
			pHistoryRecord->m_pLastChild = NULL;
			pNextEvent = pMIDIEvent->m_pNextEvent;
			while (pNextEvent) {
				//TODO 次の判定式両方必要か不明。
				if (pNextEvent->m_lUserFlag & MIDIEVENT_ALIVE) {
				//if (!(pNextEvent->m_lUserFlag & MIDIEVENT_DEAD)) {
					break;
				}
				pNextEvent = pNextEvent->m_pNextEvent;
			}
			pHistoryRecord->m_pNextObject = pNextEvent;
			pPrevEvent = pMIDIEvent->m_pPrevEvent;
			while (pPrevEvent) {
				//TODO 次の判定式両方必要か不明。
				if (pPrevEvent->m_lUserFlag & MIDIEVENT_ALIVE) {
				//if (!(pPrevEvent->m_lUserFlag & MIDIEVENT_DEAD)) {
					break;
				}
				pPrevEvent = pPrevEvent->m_pPrevEvent;
			}
			pHistoryRecord->m_pPrevObject = pPrevEvent;
			pHistoryRecord->m_pParent = pMIDIEvent->m_pParent;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_DEAD;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_RESISTEREDASREMOVED;
			m_theHistoryRecordArray.Add (pHistoryRecord);
			//_RPTF4 (_CRT_WARN, "HISTORYRECORD_REMOVEEVENT (%08x(%08x), prev=%08x, next=%08x)\n",
			//		(long)pMIDIEvent, pMIDIEvent->m_lData, (long)pPrevEvent, (long)pNextEvent);
			//FILE* pFile = fopen ("history.txt", "at");
			//fprintf (pFile, "HISTORYRECORD_REMOVEEVENT (%08x(%08x), prev=%08x, next=%08x)\n",
			//	(long)pMIDIEvent, pMIDIEvent->m_lData, (long)pPrevEvent, (long)pNextEvent);
			//fclose (pFile);
			pMIDIEvent = pMIDIEvent->m_pNextCombinedEvent;
			lCount++;
		}
		break;
	// MIDIEventオブジェクト全挿入の記録(20090116追加)
	case HISTORYRECORD_INSERTEVENTALL:
		pMIDIEvent = (MIDIEvent*)pObject;
		ASSERT (pMIDIEvent);
		while (pMIDIEvent) {
			ASSERT (pMIDIEvent->m_pParent);
			pHistoryRecord = new CHistoryRecord ();
			pHistoryRecord->m_lType = lType;
			pHistoryRecord->m_pObject = pMIDIEvent;
			pHistoryRecord->m_pFirstChild = NULL;
			pHistoryRecord->m_pLastChild = NULL;
			pHistoryRecord->m_pNextObject = pMIDIEvent->m_pNextEvent;
			pHistoryRecord->m_pPrevObject = pMIDIEvent->m_pPrevEvent;
			pHistoryRecord->m_pParent = pMIDIEvent->m_pParent;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_RESISTEREDASINSERTED;
			m_theHistoryRecordArray.Add (pHistoryRecord);
			pMIDIEvent = pMIDIEvent->m_pNextEvent;
			lCount++;
		}
		break;
	// MIDIEventオブジェクト全除去の記録(20090116追加)
	case HISTORYRECORD_REMOVEEVENTALL:
		pMIDIEvent = (MIDIEvent*)pObject;
		ASSERT (pMIDIEvent);
		while (pMIDIEvent) {
			ASSERT (pMIDIEvent->m_pParent);
			pHistoryRecord = new CHistoryRecord ();
			pHistoryRecord->m_lType = lType;
			pHistoryRecord->m_pObject = pMIDIEvent;
			pHistoryRecord->m_pFirstChild = NULL;
			pHistoryRecord->m_pLastChild = NULL;
			pHistoryRecord->m_pNextObject = pMIDIEvent->m_pNextEvent;
			pHistoryRecord->m_pPrevObject =pMIDIEvent->m_pPrevEvent;
			pHistoryRecord->m_pParent = pMIDIEvent->m_pParent;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_DEAD;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_RESISTEREDASREMOVED;
			m_theHistoryRecordArray.Add (pHistoryRecord);
			pMIDIEvent = pMIDIEvent->m_pNextEvent;
			lCount++;
		}
		break;
	// MIDITrackオブジェクト挿入の記録
	case HISTORYRECORD_INSERTTRACK:
		pMIDITrack = (MIDITrack*)pObject;
		pHistoryRecord = new CHistoryRecord ();
		pHistoryRecord->m_lType = lType;
		pHistoryRecord->m_pObject = pMIDITrack;
		pHistoryRecord->m_pFirstChild = pMIDITrack->m_pFirstEvent;
		pHistoryRecord->m_pLastChild = pMIDITrack->m_pLastEvent;
		pHistoryRecord->m_pPrevObject = pMIDITrack->m_pPrevTrack;
		pHistoryRecord->m_pNextObject = pMIDITrack->m_pNextTrack;
		pHistoryRecord->m_pParent = pMIDITrack->m_pParent;
		pMIDITrack->m_lUserFlag &= ~MIDITRACK_DEAD;
		pMIDITrack->m_lUserFlag |= MIDITRACK_ALIVE;
		pMIDITrack->m_lUserFlag |= MIDITRACK_RESISTEREDASINSERTED;
		forEachEvent (pMIDITrack, pMIDIEvent) {
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
		}
		m_theHistoryRecordArray.Add (pHistoryRecord);
		lCount = 1;
		break;
	// MIDITrackオブジェクト除去の記録
	case HISTORYRECORD_REMOVETRACK:
		pMIDITrack = (MIDITrack*)pObject;
		pHistoryRecord = new CHistoryRecord ();
		pHistoryRecord->m_lType = lType;
		pHistoryRecord->m_pObject = pMIDITrack;
		pHistoryRecord->m_pFirstChild = pMIDITrack->m_pFirstEvent;
		pHistoryRecord->m_pLastChild = pMIDITrack->m_pLastEvent;
		pHistoryRecord->m_pPrevObject = pMIDITrack->m_pPrevTrack;
		pHistoryRecord->m_pNextObject = pMIDITrack->m_pNextTrack;
		pHistoryRecord->m_pParent = pMIDITrack->m_pParent;
		pMIDITrack->m_lUserFlag |= MIDITRACK_DEAD;
		pMIDITrack->m_lUserFlag &= ~MIDITRACK_ALIVE;
		pMIDITrack->m_lUserFlag |= MIDITRACK_RESISTEREDASREMOVED;
		forEachEvent (pMIDITrack, pMIDIEvent) {
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_DEAD;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_ALIVE;
		}
		m_theHistoryRecordArray.Add (pHistoryRecord);
		lCount = 1;
		break;

	// MIDIDataオブジェクト挿入の記録
	case HISTORYRECORD_INSERTDATA:
		pMIDIData = (MIDIData*)pObject;
		pHistoryRecord = new CHistoryRecord ();
		pHistoryRecord->m_lType = lType;
		pHistoryRecord->m_pObject = pMIDIData;
		pHistoryRecord->m_pFirstChild = pMIDIData->m_pFirstTrack;
		pHistoryRecord->m_pLastChild = pMIDIData->m_pLastTrack;
		pHistoryRecord->m_pPrevObject = NULL;
		pHistoryRecord->m_pNextObject = NULL;
		pHistoryRecord->m_pParent = NULL;
		pMIDIData->m_lUserFlag &= ~MIDIDATA_DEAD;
		pMIDIData->m_lUserFlag |= MIDIDATA_ALIVE;
		pMIDIData->m_lUserFlag |= MIDIDATA_RESISTEREDASINSERTED;
		forEachTrack (pMIDIData, pMIDITrack) {
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_DEAD;
			pMIDITrack->m_lUserFlag |= MIDITRACK_ALIVE;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
				pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
			}
		}
		m_theHistoryRecordArray.Add (pHistoryRecord);
		lCount = 1;
		break;
	// MIDIDataオブジェクト除去の記録
	case HISTORYRECORD_REMOVEDATA:
		pMIDIData = (MIDIData*)pObject;
		pHistoryRecord = new CHistoryRecord ();
		pHistoryRecord->m_lType = lType;
		pHistoryRecord->m_pObject = pMIDIData;
		pHistoryRecord->m_pFirstChild = pMIDIData->m_pFirstTrack;
		pHistoryRecord->m_pLastChild = pMIDIData->m_pLastTrack;
		pHistoryRecord->m_pPrevObject = NULL;
		pHistoryRecord->m_pNextObject = NULL;
		pHistoryRecord->m_pParent = NULL;
		pMIDIData->m_lUserFlag |= MIDIDATA_DEAD;
		pMIDIData->m_lUserFlag &= ~MIDIDATA_ALIVE;
		pMIDIData->m_lUserFlag |= MIDIDATA_RESISTEREDASREMOVED;
		forEachTrack (pMIDIData, pMIDITrack) {
			pMIDITrack->m_lUserFlag |= MIDITRACK_DEAD;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_ALIVE;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				pMIDIEvent->m_lUserFlag |= MIDIEVENT_DEAD;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_ALIVE;
			}
		}
		m_theHistoryRecordArray.Add (pHistoryRecord);
		lCount = 1;
		break;
	}
	return lCount;
}
