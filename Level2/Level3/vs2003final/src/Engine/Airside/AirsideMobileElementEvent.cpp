#include "StdAfx.h"
#include ".\airsidemobileelementevent.h"
#include "AirsideMobileElement.h"

CAirsideMobileElementEvent::CAirsideMobileElementEvent(CAirsideMobileElement* mobileElement)
:m_pMobileElement(mobileElement)
{
}

CAirsideMobileElementEvent::~CAirsideMobileElementEvent(void)
{
}
ElapsedTime CAirsideMobileElementEvent::removeMoveEvent(CAirsideMobileElement *anElement)
{
	ElapsedTime aTime = -1l;
	//ElapsedTime prevEventTime = anElement->GetPrevEventTime();
	//if( prevEventTime == -1l )
	//	return aTime;

	int nCount = m_pEventList->getCount();
	//int nIdx = m_pEventList->FindFirstMatch( prevEventTime );
	//if( nIdx < 0  )
	//	return aTime;

	for( int i=0; i<nCount; i++ )
	{
		Event* pEventInList = m_pEventList->getItem( i );
		//if( (long)(pEventInList->getTime()) != (long)prevEventTime  )
		//	break;

		if( pEventInList->IsAirsideMobileEllementEvent())
		{

			CAirsideMobileElementEvent *pMoveEvent = (CAirsideMobileElementEvent*)pEventInList;
			if( pMoveEvent->m_pMobileElement == anElement)
			{
				aTime = pMoveEvent->time;
				//				m_pEventList->deleteItem( nIdx );
				m_pEventList->deleteItem( i );
				break;
			}
		}
	}
	return aTime;
}


// remove all event which is generated by _pElement
void CAirsideMobileElementEvent::RemoveAllMobileElementEvent( CAirsideMobileElement * _pElement )
{
	int nCount = m_pEventList->getCount();
	for( int i=nCount-1; i>=0; --i )
	{
		Event* pEventInList = m_pEventList->getItem( i );

		if( pEventInList->IsAirsideMobileEllementEvent())
		{

			CAirsideMobileElementEvent *pMoveEvent = (CAirsideMobileElementEvent*)pEventInList;
			if( pMoveEvent->m_pMobileElement == _pElement)
			{
				m_pEventList->deleteItem( i );
				break;
			}
		}
	}
}
void CAirsideMobileElementEvent::addEvent()
{
	removeMoveEvent(m_pMobileElement);

	Event::addEvent();
}