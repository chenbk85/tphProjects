#include "StdAfx.h"
#include ".\airsidebaggagecartinsim.h"
#include "Common\ARCTracker.h"
#include "Person.h"
#include "BagCartsParkingSpotInSim.h"
#include "AirsideBaggageBehavior.h"
#include "TerminalMobElementBehavior.h"
#include "Loader.h"
#include "Common\Point2008.h"

AirsideBaggageCartInSim::AirsideBaggageCartInSim(int id,int nPrjID,CVehicleSpecificationItem *pVehicleSpecItem)
:AirsideVehicleInSim(id, nPrjID, pVehicleSpecItem)
{
}

AirsideBaggageCartInSim::~AirsideBaggageCartInSim(void)
{
}

int AirsideBaggageCartInSim::getCapacity() const
{
	return static_cast<int>(m_pVehicleSpecificationItem->getCapacity());
}

void AirsideBaggageCartInSim::ReleaseBaggage(Processor *pProc, CBagCartsParkingSpotInSim *pBagCartsSpotInSim, ElapsedTime &eTime )
{
	PLACE_METHOD_TRACK_STRING();
	int nBagCount = static_cast<int>(m_vBaggage.size());
	for (int nBag = 0; nBag < nBagCount; ++ nBag)
	{
		AirsideBaggageBehavior *pBagBehavior = m_vBaggage.at(nBag);
		if(pBagBehavior == NULL)
			continue;
		CPoint2008 ptParkingSpot = pBagCartsSpotInSim->GetServicePoint();
		pBagBehavior->setLocation(ptParkingSpot);

		pBagBehavior->setDestination( ptParkingSpot);
		//m_pax->getLogEntry().setEntryTime(time) ;
		pBagBehavior->WriteLog(eTime) ;
		Person *pBag = pBagBehavior->getMobileElement();

		pBag->setState(Birth);
		pBag->setBehavior( new TerminalMobElementBehavior(pBag ) );

		TerminalMobElementBehavior* spTerminalBehavior = (TerminalMobElementBehavior*)pBag->getBehavior(MobElementBehavior::TerminalBehavior);
		if (spTerminalBehavior)
		{
			Point ptLoader = pProc->GetServiceLocation();
		
			spTerminalBehavior->setLocation(ptLoader);
			spTerminalBehavior->setDestination(ptLoader);
			//spTerminalBehavior->SetTransferTypeState(TerminalMobElementBehavior::TRANSFER_DEPARTURE) ;
			pBag->generateEvent(eTime + ElapsedTime(nBag *1L) ,FALSE) ;

		}
	}
	eTime +=  ElapsedTime(nBag *1L);

	m_vBaggage.clear();

}

int AirsideBaggageCartInSim::GetBagCount() const
{
	return static_cast<int>(m_vBaggage.size());
}

void AirsideBaggageCartInSim::AddBaggage( AirsideBaggageBehavior *pBag )
{
	m_vBaggage.push_back(pBag);
}

bool AirsideBaggageCartInSim::IsFull() const
{
	if(m_vBaggage.size() == getCapacity())
		return true;
	return false;

}

void AirsideBaggageCartInSim::ReleaseBaggageToFlight( const CPoint2008& ptCargoDoor, ElapsedTime &eTime )
{
	ElapsedTime eFinishedTime = eTime;
	PLACE_METHOD_TRACK_STRING();
	int nBagCount = static_cast<int>(m_vBaggage.size());
	for (int nBag = 0; nBag < nBagCount; ++ nBag)
	{
		ElapsedTime eBagTime = eTime;
		AirsideBaggageBehavior *pBagBehavior = m_vBaggage.at(nBag);
		if(pBagBehavior == NULL)
			continue;
	
		pBagBehavior->setState(MOVETOFLIGHTCARGO);
		pBagBehavior->setLocation(GetPosition());
		pBagBehavior->WriteLog(eBagTime);

		pBagBehavior->setDestination( ptCargoDoor);
		eBagTime += pBagBehavior->moveTime();
		pBagBehavior->setLocation(ptCargoDoor);
		pBagBehavior->setState(ARRIVEATFLIGHTCARGO);
		pBagBehavior->WriteLog(eBagTime) ;
		Person *pBag = pBagBehavior->getMobileElement();

		pBagBehavior->setState(Death);
		eBagTime += ElapsedTime(100L);
		pBagBehavior->WriteLog(eBagTime); 

		if(pBagBehavior->getMobileElement())
			pBagBehavior->getMobileElement()->flushLog(eFinishedTime);

		eFinishedTime = eBagTime;
	}
	
	eTime = eFinishedTime;
	m_vBaggage.clear();
}

void AirsideBaggageCartInSim::WirteLog( const CPoint2008& point, const double speed, const ElapsedTime& t, bool bPushBack /* = false */ )
{
	AirsideVehicleInSim::WirteLog(point,speed,t,bPushBack) ;
	

	ARCVector2 vDir(point- prePos);
	double dangle = vDir.AngleFromCoorndinateX();
	CPoint2008 realPosition ;
	for (int i = 0 ; i < (int)m_vBaggage.size() ; ++i)
	{
		AirsideBaggageBehavior* p_behavior = m_vBaggage.at(i);

		CPoint2008 offsetPt = p_behavior->GetOffSetInBus();
		if(point != prePos)
		{
			offsetPt.rotate(-(vDir.AngleFromCoorndinateX()));
			realPosition = prePos + offsetPt ;
			realPosition.setZ( prePos.getZ()) ;
			p_behavior->setLocation(realPosition) ;
			p_behavior->WriteLog(m_preTime,m_preSpeed) ;
		}
		else
			offsetPt.rotate(m_dir) ;

		realPosition = point + offsetPt ;
		realPosition.setZ(point.getZ()) ;
		p_behavior->setLocation(realPosition) ;
		p_behavior->WriteLog(t,speed) ;
	}
	if(prePos != point)
	{
		ARCVector2 vDir(point- prePos);
		m_dir = -(vDir.AngleFromCoorndinateX());
		prePos = point;
	}
	m_preTime = t ;
	m_preSpeed = speed ;
	prePos = point;
}