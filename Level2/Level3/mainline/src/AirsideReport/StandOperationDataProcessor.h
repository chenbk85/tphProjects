#pragma once

#include <map>
#include <vector>

#include "Common/ALTObjectID.h"
#include "StandOperationData.h"


//class StandOperationDataItem
//{
//public:
//
//	friend class StandOperationDataProcessor;
//
//	StandOperationDataItem();
//	~StandOperationDataItem();
//
//	ALTObjectID GetAltStandID() const { return m_altStandID; }
//	void SetAltStandID(ALTObjectID val) { m_altStandID = val; }
//
//	int GetArrCofliction() const { return m_nArrCofliction; }
//	void IncArrCofliction() { m_nArrCofliction++; }
//
//	int GetDepCofliction() const { return m_nDepCofliction; }
//	void IncDepCofliction() { m_nDepCofliction++; }
//
//	int GetCofliction()const {return (m_nArrCofliction+m_nDepCofliction);}
//
//	int GetDataCount() const { return (int)m_vOpData.size(); }
//	CDetailStandOperationData* GetData(int nIndex) { return m_vOpData[nIndex]; }
//	void AddData(CDetailStandOperationData* pData) { m_vOpData.push_back(pData); }
//
//protected:
//	StandOperationDataItem(bool bDeleteData);
//
//private:
//	ALTObjectID m_altStandID;
//	int m_nArrCofliction;
//	int m_nDepCofliction;
//	CDetailStandOperationDataList m_vOpData;
//
//	bool m_bDeleteData;
//};
//
//
//
//class StandOperationDataCollection
//{
//public:
//
//	friend class StandOperationDataProcessor;
//
//	StandOperationDataCollection() {}
//	~StandOperationDataCollection()
//	{
//// 		ClearData();
//	}
//
//	int GetDataCount() const { return (int)m_vOpDataCollection.size(); }
//	StandOperationDataItem* GetData(int nIndex) { return m_vOpDataCollection[nIndex]; }
//	void AddData(StandOperationDataItem* pData) { m_vOpDataCollection.push_back(pData); }
//
//protected:
//	void ClearData();
//
//
//private:
//	std::vector<StandOperationDataItem*> m_vOpDataCollection;
//
//	// copy forbidden
//	StandOperationDataCollection(const StandOperationDataCollection& rhs);
//	StandOperationDataCollection& operator=(const StandOperationDataCollection& rhs);
//};


#include "Results/ARCBaseLog.h"
#include "Results/AirsideFlightLogItem.h"
#include "AirsideBaseReport.h"


class AirsideFlightStandOperationLog;
class StandOperationAnalyzeTemp;

class StandOperationDataProcessor
{
public:
	StandOperationDataProcessor();
	~StandOperationDataProcessor();

	void LoadDataAndProcess(CBGetLogFilePath pCBGetLogFilePath, CParameters* pParameter);
	const std::vector<CStandOperationReportData*>& GetData();

	//long GetDelayTimeOnTaxiway(const AirsideFlightLogItem& item, long lFromTime, long lToTime);
private:
	//typedef std::map<int/*stand id in database*/, StandOperationDataItem*> StandOperationDataMap;
	void ProcessLogs( std::vector<AirsideFlightStandOperationLog*>& vStandOpLogs, AirsideFlightLogItem& item, bool bOnlyStand);

	//void BuildDataFromMap( CParameters* pParameter, StandOperationDataMap& opDataPlanedMap, StandOperationDataMap& opDataActualMap );
	//void AddStandOpItemToMap( StandOperationAnalyzeTemp* pTemp, StandOperationDataMap &opDataPlanedMap, StandOperationDataMap &opDataActualMap );

	std::vector<CStandOperationReportData*> m_vStandOperationReportData;

	//StandOperationDataCollection m_allStandOperationData;

	//class ActualOffTimeSorter
	//{
	//public:
	//	bool operator() (CDetailStandOperationData* lhs, CDetailStandOperationData* rhs) const
	//	{
	//		return lhs->m_lActualOff < rhs->m_lActualOff;
	//	}
	//};
	//class PlanOffTimeSorter
	//{
	//public:
	//	bool operator() (CDetailStandOperationData* lhs, CDetailStandOperationData* rhs) const
	//	{
	//		return lhs->m_lSchedOff < rhs->m_lSchedOff;
	//	}
	//};
};



//// a help class to analyze data
//class StandOperationAnalyzeTemp
//{
//public:
//	StandOperationAnalyzeTemp()
//		: pData(NULL)
//		, nPlanedStand(-1)
//		, nActualStand(-1)
//	{
//
//	}
//
//	~StandOperationAnalyzeTemp();
//
//	void InitOperationAnalyzeTemp( AirsideFlightStandOperationLog* pLog );
//
//
//	CDetailStandOperationData* pData;
//	int nPlanedStand;
//	int nActualStand;
//	ALTObjectID altPlanedStandID;
//	ALTObjectID altActualStandID;
//
//};