#include "stdafx.h"
#include "AirsideAircraftMutiRunRunwayOperationResult.h"
#include "Common\TERMFILE.H"
#include "AirsideRunwayOperationReportParam.h"
#include "AirsideRunwayOperationsReport.h"
#include "CARC3DChart.h"

static const CString str[] = 
{
    _T("Min"),
    _T("Min Interval"),
    _T("Average"),
    _T("Max"),
    _T("Max Interval"),
    _T("Q1"),
    _T("Q2"),
    _T("Q3"),
    _T("P1"),
    _T("P5"),
    _T("P10"),
    _T("P90"),
    _T("P95"),
    _T("P99"),
    _T("Std Dev")
};

CAirsideAircraftMutiRunRunwayOperationResult::CAirsideAircraftMutiRunRunwayOperationResult(void)
{
}


CAirsideAircraftMutiRunRunwayOperationResult::~CAirsideAircraftMutiRunRunwayOperationResult(void)
{
	ClearData();
}
 
void CAirsideAircraftMutiRunRunwayOperationResult::LoadMultipleRunReport(CParameters* pParameter)
{
    ClearData();

	maRunwayLoadOperation mapLandingOperation;
	maRunwayLoadOperation mapTakeoffOperation;
	mapRunwayTimeOperation mapMovementInterval; 
    ArctermFile file;
    DelayResultPath::iterator iter = m_mapResultPath.begin();
    for (; iter != m_mapResultPath.end(); ++iter)
    {
        CString strResultPath = iter->second;
        CString strSimResult = iter->first;
        try
        {
            if (file.openFile(strResultPath.GetString(),READ))
            {
                file.getLine(); // 'Column'
                int iReportType = 0;
                file.getInteger(iReportType);
                file.getLine();
                if (iReportType == ASReportType_Detail)
                {
					file.getLine();
					int nOperationCount = 0;
					file.getInteger(nOperationCount);
					for (int i = 0; i < nOperationCount; i++)
					{
						file.skipField(1); 
						long lTime = 0;
						file.getInteger(lTime);
						CString strRunway;
						file.getField(strRunway.GetBuffer(1024),1024);
						strRunway.ReleaseBuffer();
						CString strOperation;
						file.getField(strOperation.GetBuffer(1024),1024);
						strOperation.ReleaseBuffer();
						if (strOperation.Compare("Landing") == 0)
						{
							mapLandingOperation[strSimResult][strRunway].push_back(lTime);
						}
						else if (strOperation.Compare("Takeoff") == 0)
						{
							mapTakeoffOperation[strSimResult][strRunway].push_back(lTime);
						}
						file.getLine();
					}

					int nRunwwayCount = 0;
					file.skipField(1);//skip runway count
					file.getInteger(nRunwwayCount);
					file.getLine();

					for (int nRunway = 0; nRunway < nRunwwayCount; ++ nRunway)
					{
						file.skipField(2);
						CString strMark;
						file.getField(strMark.GetBuffer(1024),1024);
						strMark.ReleaseBuffer();

						file.getLine();
						file.getLine();
						file.getLine();

						int nLeadTrailCount = 0;
						file.getInteger(nLeadTrailCount);
						for (int nLeadtrail = 0; nLeadtrail < nLeadTrailCount; ++ nLeadtrail)
						{
							file.skipField(1);
							CString strClassLeadName;
							file.getField(strClassLeadName.GetBuffer(1024),1024);
							file.skipField(1);
							CString strClassTrailName;
							file.getField(strClassTrailName.GetBuffer(1024),1024);
							int nFlightCount = 0;
							file.getInteger(nFlightCount);
							CString strClassification;
							strClassification.Format(_T("%s-%s"),strClassLeadName,strClassTrailName);

							ClassificationValue landTrail;
							landTrail.m_strLandTrail = strClassification;
							landTrail.m_lFlightCount = nFlightCount;
							m_mapLandTrailOperation[strSimResult][strMark].push_back(landTrail);
						}

						file.getLine();
						int nTimeValueCount = 0;
						file.getInteger(nTimeValueCount);
						for (int nTime = 0; nTime < nTimeValueCount; ++ nTime)
						{
							int nTimeValue = 0;
							file.getInteger(nTimeValue);
							int nLandingCount = 0;
							file.getInteger(nLandingCount);
							int nTakeOffCount = 0;
							file.getInteger(nTakeOffCount);

							//make runway mark and landing/takeoff format
							CString strLandingMark;
							strLandingMark.Format(_T("%sLanding"),strMark);
							RunwayTimeValue landingTime;
							landingTime.m_eTime = nTimeValue;
							landingTime.m_iOperation = nLandingCount;
							CString strTakeoffMark;
							strTakeoffMark.Format(_T("%sTakeoff"),strMark);
							RunwayTimeValue takeoffTime;
							takeoffTime.m_eTime = nTimeValue;
							takeoffTime.m_iOperation = nTakeOffCount;

							mapMovementInterval[strSimResult][strLandingMark].push_back(landingTime);
							mapMovementInterval[strSimResult][strTakeoffMark].push_back(takeoffTime);
						}
						file.getLine();
					}
                }
                else if(iReportType == ASReportType_Summary)
                {
                    file.getLine(); // skip the list titles
                    int nRunwwayCount = 0;
                    file.skipField(1);//skip runway count
                    file.getInteger(nRunwwayCount);
                    file.getLine();
                    for (int nRunway = 0; nRunway < nRunwwayCount; ++ nRunway)
                    {
                        CAirsideReportRunwayMark runWaymark;
                        //runway information
                        file.getInteger(runWaymark.m_nRunwayID);
                        file.getInteger((int&)runWaymark.m_enumRunwayMark);
                        file.getField(runWaymark.m_strMarkName.GetBuffer(1024),1024);
                        runWaymark.m_strMarkName.ReleaseBuffer();

                        //landing
                        {
                            SummaryRunwayOperationReportItem staSumItem;
                            staSumItem.m_runWaymark = runWaymark;
                            file.getInteger(staSumItem.m_minCount);
                            file.getField(staSumItem.m_strMinInterval.GetBuffer(1024),1024);
                            staSumItem.m_strMinInterval.ReleaseBuffer();
                            file.getInteger(staSumItem.m_nAverageCount);
                            file.getInteger(staSumItem.m_maxCount);
                            file.getField(staSumItem.m_strMaxInterval.GetBuffer(1024),1024);
                            staSumItem.m_strMaxInterval.ReleaseBuffer();
                            file.getInteger(staSumItem.m_nQ1);
                            file.getInteger(staSumItem.m_nQ2);
                            file.getInteger(staSumItem.m_nQ3);
                            file.getInteger(staSumItem.m_nP1);
                            file.getInteger(staSumItem.m_nP5);
                            file.getInteger(staSumItem.m_nP10);
                            file.getInteger(staSumItem.m_nP90);
                            file.getInteger(staSumItem.m_nP95);
                            file.getInteger(staSumItem.m_nP99);
                            file.getInteger(staSumItem.m_nStdDev);
                            m_summaryDataLanding[strSimResult].push_back(staSumItem);
                        }

                        //take off 
                        {
                            SummaryRunwayOperationReportItem staSumItem;
                            staSumItem.m_runWaymark = runWaymark;
                            file.getInteger(staSumItem.m_minCount);
                            file.getField(staSumItem.m_strMinInterval.GetBuffer(1024),1024);
                            staSumItem.m_strMinInterval.ReleaseBuffer();
                            file.getInteger(staSumItem.m_nAverageCount);
                            file.getInteger(staSumItem.m_maxCount);
                            file.getField(staSumItem.m_strMaxInterval.GetBuffer(1024),1024);
                            staSumItem.m_strMaxInterval.ReleaseBuffer();
                            file.getInteger(staSumItem.m_nQ1);
                            file.getInteger(staSumItem.m_nQ2);
                            file.getInteger(staSumItem.m_nQ3);
                            file.getInteger(staSumItem.m_nP1);
                            file.getInteger(staSumItem.m_nP5);
                            file.getInteger(staSumItem.m_nP10);
                            file.getInteger(staSumItem.m_nP90);
                            file.getInteger(staSumItem.m_nP95);
                            file.getInteger(staSumItem.m_nP99);
                            file.getInteger(staSumItem.m_nStdDev);
                            m_summaryDataTakeOff[strSimResult].push_back(staSumItem);
                        }

                        //movements
                        {
                            SummaryRunwayOperationReportItem staSumItem;
                            staSumItem.m_runWaymark = runWaymark;
                            file.getInteger(staSumItem.m_minCount);
                            file.getField(staSumItem.m_strMinInterval.GetBuffer(1024),1024);
                            staSumItem.m_strMinInterval.ReleaseBuffer();
                            file.getInteger(staSumItem.m_nAverageCount);
                            file.getInteger(staSumItem.m_maxCount);
                            file.getField(staSumItem.m_strMaxInterval.GetBuffer(1024),1024);
                            staSumItem.m_strMaxInterval.ReleaseBuffer();
                            file.getInteger(staSumItem.m_nQ1);
                            file.getInteger(staSumItem.m_nQ2);
                            file.getInteger(staSumItem.m_nQ3);
                            file.getInteger(staSumItem.m_nP1);
                            file.getInteger(staSumItem.m_nP5);
                            file.getInteger(staSumItem.m_nP10);
                            file.getInteger(staSumItem.m_nP90);
                            file.getInteger(staSumItem.m_nP95);
                            file.getInteger(staSumItem.m_nP99);
                            file.getInteger(staSumItem.m_nStdDev);
                            m_summaryDataMovement[strSimResult].push_back(staSumItem);
                        }
                        file.getLine();
                    }
                }
                file.closeIn();
            }
        }
        catch(...)
        {
            ClearData();
        }
    }

	//generate runway operation multiple run landing 
	BuildDetailMultipleRunwayOperation(m_mapLandingOperation,mapLandingOperation,pParameter);
	//generate runway operation multiple run takeoff
	BuildDetailMultipleRunwayOperation(m_mapTakeoffOperation,mapTakeoffOperation,pParameter);
	//generate runway operation multiple run movement interval
	BuildDetailMultipleRunwayTimeValue(m_mapMovementInterval,mapMovementInterval,pParameter);
}

void CAirsideAircraftMutiRunRunwayOperationResult::InitListHead(CXListCtrl& cxListCtrl, CParameters* parameter, int iType, CSortableHeaderCtrl* piSHC)
{
    cxListCtrl.DeleteAllItems();

    while(cxListCtrl.GetHeaderCtrl()->GetItemCount() >0)
        cxListCtrl.DeleteColumn(0);

    if (parameter->getReportType() == ASReportType_Detail)
    {
		DWORD headStyle = LVCFMT_CENTER;
		headStyle &= ~HDF_OWNERDRAW;
		cxListCtrl.InsertColumn(0,"",headStyle,20);

		headStyle = LVCFMT_LEFT;
		headStyle &= ~HDF_OWNERDRAW;
		cxListCtrl.InsertColumn(1, _T("SimResult"), headStyle, 80);

		AirsideRunwayOperationReportParam* pOpPara = (AirsideRunwayOperationReportParam*)parameter;
		switch (pOpPara->getSubType())
		{
		case AirsideRunwayOperationsReport::ChartType_Detail_LandingsByRunway:
			InitDetailListHead(cxListCtrl,m_mapLandingOperation,piSHC);
			break;
		case AirsideRunwayOperationsReport::ChartType_Detail_TakeOffByRunway:
			InitDetailListHead(cxListCtrl,m_mapTakeoffOperation,piSHC);
			break;
		case AirsideRunwayOperationsReport::ChartType_Detail_LeadTrailByRunway:
			InitDetailClassificationHead(cxListCtrl,m_mapLandTrailOperation,piSHC);
			break;
		case AirsideRunwayOperationsReport::ChartType_Detail_MovementsPerRunway:
			InitDetailListHead(cxListCtrl,m_mapMovementInterval,piSHC);
			break;
		default:
			break;
		}
    }
    else if(parameter->getReportType() == ASReportType_Summary)
    {
        InitSummaryListHead(cxListCtrl, parameter, piSHC);
    }
}

void CAirsideAircraftMutiRunRunwayOperationResult::FillListContent( CXListCtrl& cxListCtrl, CParameters * parameter, int iType /*= 0*/ )
{
    ASSERT(parameter);
    if (parameter->getReportType() == ASReportType_Detail)
    {
		AirsideRunwayOperationReportParam* pOpPara = (AirsideRunwayOperationReportParam*)parameter;
		switch (pOpPara->getSubType())
		{
		case AirsideRunwayOperationsReport::ChartType_Detail_LandingsByRunway:
			FillDetailListOperationContent(cxListCtrl,m_mapLandingOperation);
			break;
		case AirsideRunwayOperationsReport::ChartType_Detail_TakeOffByRunway:
			FillDetailListOperationContent(cxListCtrl,m_mapTakeoffOperation);
			break;
		case AirsideRunwayOperationsReport::ChartType_Detail_LeadTrailByRunway:
			InitDetailClassificationHead(cxListCtrl,m_mapLandTrailOperation);
			break;
		case AirsideRunwayOperationsReport::ChartType_Detail_MovementsPerRunway:
			FillDetailListOperationContent(cxListCtrl,m_mapMovementInterval);
			break;
		default:
			break;
		}
    }
    else if(parameter->getReportType() == ASReportType_Summary)
    {
        AirsideRunwayOperationReportParam* pRunWayOpParam = (AirsideRunwayOperationReportParam*)parameter;
        int nSubType = pRunWayOpParam->getSubType();
        switch(nSubType)
        {
        case AirsideRunwayOperationsReport::ChartType_Summary_RunwayOperationalStatistic_Operations:
            FillSummaryListContent(cxListCtrl, m_summaryDataMovement);
            break;
        case AirsideRunwayOperationsReport::ChartType_Summary_RunwayOperationalStatistic_Landing:
            FillSummaryListContent(cxListCtrl, m_summaryDataLanding);
            break;
        case AirsideRunwayOperationsReport::ChartType_Summary_RunwayOperationalStatistic_TakeOff:
            FillSummaryListContent(cxListCtrl, m_summaryDataLanding);
            break;
        default:
            break;
        }
    }
}

void CAirsideAircraftMutiRunRunwayOperationResult::Draw3DChart( CARC3DChart& chartWnd, CParameters *pParameter, int iType /*= 0*/ )
{
	if (pParameter->getReportType() == ASReportType_Detail)
	{
		AirsideRunwayOperationReportParam* pOpPara = (AirsideRunwayOperationReportParam*)pParameter;
		switch (pOpPara->getSubType())
		{
		case AirsideRunwayOperationsReport::ChartType_Detail_LandingsByRunway:
			Generate3DChartCountData(m_mapLandingOperation,chartWnd,pParameter,iType);
			break;
		case AirsideRunwayOperationsReport::ChartType_Detail_TakeOffByRunway:
			Generate3DChartCountData(m_mapTakeoffOperation,chartWnd,pParameter,iType);
			break;
		case AirsideRunwayOperationsReport::ChartType_Detail_LeadTrailByRunway:
			Generate3DChartLeadTrailData(m_mapLandTrailOperation,chartWnd,pParameter,iType);
			break;
		case AirsideRunwayOperationsReport::ChartType_Detail_MovementsPerRunway:
			Generate3DChartCountData(m_mapMovementInterval,chartWnd,pParameter,iType);
			break;
		default:
			break;
		}
	}    
	else if(pParameter->getReportType() == ASReportType_Summary)
    {
        DrawSummary3DChart(chartWnd, pParameter, iType);
    }
}

void CAirsideAircraftMutiRunRunwayOperationResult::ClearData()
{
	m_mapLandingOperation.clear();
	m_mapTakeoffOperation.clear();
	m_mapLandTrailOperation.clear();
	m_mapMovementInterval.clear();

    m_summaryDataLanding.clear();
    m_summaryDataTakeOff.clear();
    m_summaryDataMovement.clear();
}

void CAirsideAircraftMutiRunRunwayOperationResult::InitSummaryListHead(CXListCtrl& cxListCtrl, CParameters* pParam, CSortableHeaderCtrl* piSHC)
{
    int nCurCol = 0;
    DWORD headStyle = LVCFMT_CENTER;
    headStyle &= ~HDF_OWNERDRAW;
    cxListCtrl.InsertColumn(nCurCol,"",headStyle,20);
    nCurCol++;

    headStyle = LVCFMT_LEFT;
    headStyle &= ~HDF_OWNERDRAW;
    cxListCtrl.InsertColumn(nCurCol, _T("SimResult"), headStyle, 80);
    if(piSHC)
    {
        piSHC->SetDataType(nCurCol, dtSTRING);
    }
    nCurCol++;

    cxListCtrl.InsertColumn(nCurCol, _T("Runway"), headStyle, 80);
    if(piSHC)
    {
        piSHC->SetDataType(nCurCol, dtSTRING);
    }
    nCurCol++;

    AirsideRunwayOperationReportParam* pRunWayOpParam = (AirsideRunwayOperationReportParam*)pParam;
    int nSubType = pRunWayOpParam->getSubType();
    switch(nSubType)
    {
    case AirsideRunwayOperationsReport::ChartType_Summary_RunwayOperationalStatistic_TakeOff:
        for(int i=0; i<15; i++)
        {
            cxListCtrl.InsertColumn(nCurCol, str[i]+_T("(TO)"), LVCFMT_LEFT, 100);
            if(piSHC)
            {
                piSHC->SetDataType(nCurCol,dtINT);
            }
            nCurCol++;
        }
        break;
    case AirsideRunwayOperationsReport::ChartType_Summary_RunwayOperationalStatistic_Landing:
        for(int i=0; i<15; i++)
        {
            cxListCtrl.InsertColumn(nCurCol, str[i]+_T("(Landing)"), LVCFMT_LEFT, 100);
            if(piSHC)
            {
                piSHC->SetDataType(nCurCol,dtINT);
            }
            nCurCol++;
        }
        break;
    case AirsideRunwayOperationsReport::ChartType_Summary_RunwayOperationalStatistic_Operations:
        for(int i=0; i<15; i++)
        {
            cxListCtrl.InsertColumn(nCurCol, str[i]+_T("(Movements)"), LVCFMT_LEFT, 100);
            if(piSHC)
            {
                piSHC->SetDataType(nCurCol,dtINT);
            }
            nCurCol++;
        }
        break;
    default:
        break;
    }
}

void CAirsideAircraftMutiRunRunwayOperationResult::FillSummaryListContent(CXListCtrl& cxListCtrl, const mapSummaryData& mapSum)
{
    mapSummaryData::const_iterator itor = mapSum.begin();
    int idx = 0;
    for(; itor!=mapSum.end(); itor++)
    {
        const vectorSummaryRunwayOpRepItem& vStaSumItems = itor->second;
        size_t nCount = vStaSumItems.size();
        for(size_t i=0; i<nCount; i++)
        {
            CString strIndex;
            strIndex.Format(_T("%d"),idx+1);
            cxListCtrl.InsertItem(idx,strIndex);

            CString strSimName = itor->first;
            int nCurSimResult = atoi(strSimName.Mid(9,strSimName.GetLength()));
            CString strRun = _T("");
            strRun.Format(_T("Run%d"),nCurSimResult+1);

            int nCurCol = 1;
            cxListCtrl.SetItemText(idx, nCurCol, strRun);
            cxListCtrl.SetItemData(idx, idx);
            nCurCol++;

            const SummaryRunwayOperationReportItem& sumItem = vStaSumItems.at(i);

            CString strTemp;
            strTemp.Format("%s", sumItem.m_runWaymark.m_strMarkName);
            cxListCtrl.SetItemText(idx, nCurCol, strTemp);
            nCurCol++;

            strTemp.Format("%d", sumItem.m_minCount);
            cxListCtrl.SetItemText(idx, nCurCol, strTemp);
            nCurCol++;

            strTemp.Format("%s", sumItem.m_strMinInterval);
            cxListCtrl.SetItemText(idx, nCurCol, strTemp);
            nCurCol++;

            strTemp.Format("%d", sumItem.m_nAverageCount);
            cxListCtrl.SetItemText(idx, nCurCol, strTemp);
            nCurCol++;

            strTemp.Format("%d", sumItem.m_maxCount);
            cxListCtrl.SetItemText(idx, nCurCol, strTemp);
            nCurCol++;

            strTemp.Format("%s", sumItem.m_strMaxInterval);
            cxListCtrl.SetItemText(idx, nCurCol, strTemp);
            nCurCol++;

            strTemp.Format("%d", sumItem.m_nQ1);
            cxListCtrl.SetItemText(idx, nCurCol, strTemp);
            nCurCol++;

            strTemp.Format("%d", sumItem.m_nQ2);
            cxListCtrl.SetItemText(idx, nCurCol, strTemp);
            nCurCol++;

            strTemp.Format("%d", sumItem.m_nQ3);
            cxListCtrl.SetItemText(idx, nCurCol, strTemp);
            nCurCol++;

            strTemp.Format("%d", sumItem.m_nP1);
            cxListCtrl.SetItemText(idx, nCurCol, strTemp);
            nCurCol++;

            strTemp.Format("%d", sumItem.m_nP5);
            cxListCtrl.SetItemText(idx, nCurCol, strTemp);
            nCurCol++;

            strTemp.Format("%d", sumItem.m_nP10);
            cxListCtrl.SetItemText(idx, nCurCol, strTemp);
            nCurCol++;

            strTemp.Format("%d", sumItem.m_nP90);
            cxListCtrl.SetItemText(idx, nCurCol, strTemp);
            nCurCol++;

            strTemp.Format("%d", sumItem.m_nP95);
            cxListCtrl.SetItemText(idx, nCurCol, strTemp);
            nCurCol++;

            strTemp.Format("%d", sumItem.m_nP99);
            cxListCtrl.SetItemText(idx, nCurCol, strTemp);
            nCurCol++;

            strTemp.Format("%d", sumItem.m_nStdDev);
            cxListCtrl.SetItemText(idx, nCurCol, strTemp);
            nCurCol++;

            idx++;
        }
    }
}

void CAirsideAircraftMutiRunRunwayOperationResult::DrawSummary3DChart(CARC3DChart& chartWnd, CParameters *pParameter, int iType)
{
    C2DChartData c2dGraphData;
    AirsideRunwayOperationReportParam* pRunWayOpParam = (AirsideRunwayOperationReportParam*)pParameter;
    int nSubType = pRunWayOpParam->getSubType();
    switch(nSubType)
    {
    case AirsideRunwayOperationsReport::ChartType_Summary_RunwayOperationalStatistic_Operations:
        SetSummaryMovement3DChartString(c2dGraphData, pParameter);
        GenerateSummary2DChartData(c2dGraphData, m_summaryDataMovement, iType);
        break;
    case AirsideRunwayOperationsReport::ChartType_Summary_RunwayOperationalStatistic_Landing:
        SetSummaryLanding3DChartString(c2dGraphData, pParameter);
        GenerateSummary2DChartData(c2dGraphData, m_summaryDataLanding, iType);
        break;
    case AirsideRunwayOperationsReport::ChartType_Summary_RunwayOperationalStatistic_TakeOff:
        SetSummaryTakeOff3DChartString(c2dGraphData, pParameter);
        GenerateSummary2DChartData(c2dGraphData, m_summaryDataTakeOff, iType);
        break;
    default:
        break;
    }

    chartWnd.DrawChart(c2dGraphData);
}

void CAirsideAircraftMutiRunRunwayOperationResult::GenerateSummary2DChartData(C2DChartData& c2dGraphData, const mapSummaryData& mapSumData, int iType)
{
    int nCount = sizeof(str) / sizeof(str[0]);
    for(int i=0; i<nCount; i++)
    {
        if(i==1 || i==4)
            continue;
        CString strLegend = str[i];
        c2dGraphData.m_vrLegend.push_back(strLegend);
    }

    RumwayMarkWithLandingTakeOff* pMarkToShow = (RumwayMarkWithLandingTakeOff*)iType;
    if(pMarkToShow == NULL)
        return;
    c2dGraphData.m_vr2DChartData.resize(13);
    mapSummaryData::const_iterator iter = mapSumData.begin();
    for(; iter != mapSumData.end(); ++iter)
    {
        CString strSimName = iter->first;
        int nCurSimResult = atoi(strSimName.Mid(9,strSimName.GetLength()));
        CString strXTickTitle;
        strXTickTitle.Format(_T("Run%d"), nCurSimResult+1);
        c2dGraphData.m_vrXTickTitle.push_back(strXTickTitle);

        const vectorSummaryRunwayOpRepItem& vItems = iter->second;
        size_t nCount = vItems.size();
        for(size_t i=0; i<nCount; i++)
        {
            const SummaryRunwayOperationReportItem& item = vItems.at(i);
            if(item.m_runWaymark == pMarkToShow->m_mark)
            {
                c2dGraphData.m_vr2DChartData[0].push_back((double)item.m_minCount);
                c2dGraphData.m_vr2DChartData[1].push_back((double)item.m_nAverageCount);
                c2dGraphData.m_vr2DChartData[2].push_back((double)item.m_maxCount);
                c2dGraphData.m_vr2DChartData[3].push_back((double)item.m_nQ1);
                c2dGraphData.m_vr2DChartData[4].push_back((double)item.m_nQ2);
                c2dGraphData.m_vr2DChartData[5].push_back((double)item.m_nQ3);
                c2dGraphData.m_vr2DChartData[6].push_back((double)item.m_nP1);
                c2dGraphData.m_vr2DChartData[7].push_back((double)item.m_nP5);
                c2dGraphData.m_vr2DChartData[8].push_back((double)item.m_nP10);
                c2dGraphData.m_vr2DChartData[9].push_back((double)item.m_nP90);
                c2dGraphData.m_vr2DChartData[10].push_back((double)item.m_nP95);
                c2dGraphData.m_vr2DChartData[11].push_back((double)item.m_nP99);
                c2dGraphData.m_vr2DChartData[12].push_back((double)item.m_nStdDev);
            }
        }
    }
}

void CAirsideAircraftMutiRunRunwayOperationResult::SetSummaryMovement3DChartString(C2DChartData& c2dGraphData, CParameters *pParameter)
{
    c2dGraphData.m_strChartTitle = _T(" Runway Operational Statistic - Movements ");
    c2dGraphData.m_strYtitle = _T("Number of Operations");
    c2dGraphData.m_strXtitle = _T("Runs");	

    //set footer
    CString strFooter(_T(""));
    strFooter.Format(_T("Runway Operational Statistic - Movements %s "), pParameter->GetParameterString());
    c2dGraphData.m_strFooter = strFooter;
}

void CAirsideAircraftMutiRunRunwayOperationResult::SetSummaryLanding3DChartString( C2DChartData& c2dGraphData, CParameters *pParameter )
{
    c2dGraphData.m_strChartTitle = _T(" Runway Operational Statistic - Landing ");
    c2dGraphData.m_strYtitle = _T("Number of Operations");
    c2dGraphData.m_strXtitle = _T("Runs");	

    //set footer
    CString strFooter(_T(""));
    strFooter.Format(_T("Runway Operational Statistic - Landing %s "), pParameter->GetParameterString());
    c2dGraphData.m_strFooter = strFooter;
}

void CAirsideAircraftMutiRunRunwayOperationResult::SetSummaryTakeOff3DChartString( C2DChartData& c2dGraphData, CParameters *pParameter )
{
    c2dGraphData.m_strChartTitle = _T(" Runway Operational Statistic - Take Off ");
    c2dGraphData.m_strYtitle = _T("Number of Operations");
    c2dGraphData.m_strXtitle = _T("Runs");	

    //set footer
    CString strFooter(_T(""));
    strFooter.Format(_T("Runway Operational Statistic - Take Off %s "), pParameter->GetParameterString());
    c2dGraphData.m_strFooter = strFooter;
}
void CAirsideAircraftMutiRunRunwayOperationResult::BuildDetailMultipleRunwayOperation( mapRunwayDetailOperation& mapDetailData,maRunwayLoadOperation mapData,CParameters* pParameter )
{
	if (mapData.empty())
		return;

	ElapsedTime estMinDelayTime = pParameter->getStartTime();
	ElapsedTime estMaxDelayTime = pParameter->getEndTime();

	long lMinDelayTime = estMinDelayTime.asSeconds();
	long lMaxDelayTime = estMaxDelayTime.asSeconds();

	if (lMinDelayTime > lMaxDelayTime)
		return;

	long iInterval = pParameter->getInterval();
	ElapsedTime estUserIntervalTime = ElapsedTime(iInterval);

	long lDelayTimeSegmentCount = 0;             //the count of the delayTime segment
	if (0 < pParameter->getInterval())
	{
		lDelayTimeSegmentCount = (lMaxDelayTime - lMinDelayTime) / (iInterval);
		estMinDelayTime = ElapsedTime((lMinDelayTime - lMinDelayTime%(iInterval)) /100);

		//		lDelayTimeSegmentCount++;
	}
	else
	{
		lDelayTimeSegmentCount= ClacTimeRange(estMaxDelayTime, estMinDelayTime, estUserIntervalTime);
	}

	maRunwayLoadOperation::iterator opIter = mapData.begin();
	for (; opIter != mapData.end(); ++opIter)
	{
		mapDetailData[opIter->first].clear();

		mapLoadResult::iterator iter = opIter->second.begin();
		for (; iter != opIter->second.end(); ++iter)
		{
			for (long i = 0; i < lDelayTimeSegmentCount; i++)
			{
				ElapsedTime estTempMinDelayTime = estMinDelayTime + ElapsedTime(estUserIntervalTime.asSeconds()*i);
				ElapsedTime estTempMaxDelayTime = estMinDelayTime + ElapsedTime(estUserIntervalTime.asSeconds()*(i + 1));

				MultipleRunReportData delayData;
				delayData.m_iStart = estTempMinDelayTime.getPrecisely();
				delayData.m_iEnd = estTempMaxDelayTime.getPrecisely();

				long iCount = GetIntervalCount(delayData.m_iStart,delayData.m_iEnd,iter->second);
				delayData.m_iData = iCount;
				mapDetailData[opIter->first][iter->first].push_back(delayData);
			}
		}
	}


}

void CAirsideAircraftMutiRunRunwayOperationResult::BuildDetailMultipleRunwayTimeValue( mapRunwayDetailOperation& mapDetailData,mapRunwayTimeOperation mapData,CParameters* pParameter )
{
	if (mapData.empty())
		return;

	ElapsedTime estMinDelayTime = pParameter->getStartTime();
	ElapsedTime estMaxDelayTime = pParameter->getEndTime();

	long lMinDelayTime = estMinDelayTime.asSeconds();
	long lMaxDelayTime = estMaxDelayTime.asSeconds();

	if (lMinDelayTime > lMaxDelayTime)
		return;

	long iInterval = pParameter->getInterval();
	ElapsedTime estUserIntervalTime = ElapsedTime(iInterval);

	long lDelayTimeSegmentCount = 0;             //the count of the delayTime segment
	if (0 < pParameter->getInterval())
	{
		lDelayTimeSegmentCount = (lMaxDelayTime - lMinDelayTime) / (iInterval);
		estMinDelayTime = ElapsedTime((lMinDelayTime - lMinDelayTime%(iInterval)) /100);

		//		lDelayTimeSegmentCount++;
	}
	else
	{
		lDelayTimeSegmentCount= ClacTimeRange(estMaxDelayTime, estMinDelayTime, estUserIntervalTime);
	}
	mapRunwayTimeOperation::iterator timeIter = mapData.begin();
	for (; timeIter != mapData.end(); ++timeIter)
	{
		mapDetailData[timeIter->first].clear();
		mapRunwayTimeValue::iterator iter = timeIter->second.begin();
		for (; iter != timeIter->second.end(); ++iter)
		{
			mapDetailData[timeIter->first][iter->first].clear();
			for (long i = 0; i < lDelayTimeSegmentCount; i++)
			{
				ElapsedTime estTempMinDelayTime = estMinDelayTime + ElapsedTime(estUserIntervalTime.asSeconds()*i);
				ElapsedTime estTempMaxDelayTime = estMinDelayTime + ElapsedTime(estUserIntervalTime.asSeconds()*(i + 1));

				MultipleRunReportData delayData;
				delayData.m_iStart = estTempMinDelayTime.getPrecisely();
				delayData.m_iEnd = estTempMaxDelayTime.getPrecisely();

				long iCount = GetIntervalTimeValue(delayData.m_iStart,delayData.m_iEnd,iter->second);
				delayData.m_iData = iCount;
				mapDetailData[timeIter->first][iter->first].push_back(delayData);
			}
		}
	}

}

long CAirsideAircraftMutiRunRunwayOperationResult::GetIntervalTimeValue( long iStart,long iEnd,std::vector<RunwayTimeValue> vData ) const
{
	long lOperation = 0;
	for (unsigned i = 0; i < vData.size(); i++)
	{
		RunwayTimeValue runwayTime = vData.at(i);
		if (runwayTime.m_eTime >= iStart && runwayTime.m_eTime < iEnd)
		{
			lOperation += runwayTime.m_iOperation;
		}
	}
	return lOperation;
}

void CAirsideAircraftMutiRunRunwayOperationResult::InitDetailListHead( CXListCtrl& cxListCtrl,mapRunwayDetailOperation mapDetailData,CSortableHeaderCtrl* piSHC/*=NULL*/ )
{
	if (mapDetailData.empty())
		return;

	mapRunwayDetailOperation::iterator iter = mapDetailData.begin();

	if (iter->second.empty())
		return;
	
	MultiRunDetailMap::iterator mapIter = iter->second.begin();

	size_t rangeCount = mapIter->second.size();
	for (size_t i = 0; i < rangeCount; ++i)
	{
		MultipleRunReportData delayData = mapIter->second.at(i);
		CString strRange;
		ElapsedTime eStartTime;
		ElapsedTime eEndTime;
		eStartTime.setPrecisely(delayData.m_iStart);
		eEndTime.setPrecisely(delayData.m_iEnd);
		strRange.Format(_T("%s - %s"),eStartTime.printTime(), eEndTime.printTime());

		DWORD dwStyle = LVCFMT_LEFT;
		dwStyle &= ~HDF_OWNERDRAW;

		cxListCtrl.InsertColumn(2+i, strRange, /*LVCFMT_LEFT*/dwStyle, 100);
	}
}

void CAirsideAircraftMutiRunRunwayOperationResult::FillDetailListLandTrailContent( CXListCtrl& cxListCtrl,mapLandTrailOperation mapDetailData )
{
	mapLandTrailOperation::iterator iter = mapDetailData.begin();
	int idx = 0;
	for (; iter != mapDetailData.end(); ++iter)
	{
		mapRunwayMark::iterator markIter = iter->second.begin();
		for (; markIter != iter->second.end(); ++markIter)
		{
			CString strIndex;
			strIndex.Format(_T("%d"),idx+1);
			cxListCtrl.InsertItem(idx,strIndex);

			CString strSimName = iter->first;
			int nCurSimResult = atoi(strSimName.Mid(9,strSimName.GetLength()));
			CString strRun = _T("");
			strRun.Format(_T("Run%d"),nCurSimResult+1);

			cxListCtrl.SetItemText(idx,1,strRun);
			cxListCtrl.SetItemText(idx,2,markIter->first);
			cxListCtrl.SetItemData(idx,idx);

			for (size_t n = 0; n < markIter->second.size(); ++n)
			{
				ClassificationValue typeData = markIter->second.at(n);
				CString strCount;
				if(n <  iter->second.size())
					strCount.Format(_T("%d"), typeData.m_lFlightCount);
				else
					strCount.Format(_T("%d"),0) ;

				cxListCtrl.SetItemText(idx, n + 2, strCount);
			}
			idx++;
		}
	}
}

void CAirsideAircraftMutiRunRunwayOperationResult::InitDetailClassificationHead( CXListCtrl& cxListCtrl,mapLandTrailOperation mapDetailData,CSortableHeaderCtrl* piSHC/*=NULL*/ )
{
	if (mapDetailData.empty())
		return;

	mapLandTrailOperation::iterator iter = mapDetailData.begin();

	if (iter->second.empty())
		return;

	mapRunwayMark::iterator markIter = iter->second.begin();

	size_t rangeCount = markIter->second.size();
	for (size_t i = 0; i < rangeCount; ++i)
	{
		ClassificationValue typeData = markIter->second.at(i);

		DWORD dwStyle = LVCFMT_LEFT;
		dwStyle &= ~HDF_OWNERDRAW;

		cxListCtrl.InsertColumn(2+i, typeData.m_strLandTrail, /*LVCFMT_LEFT*/dwStyle, 100);
	}
}

void CAirsideAircraftMutiRunRunwayOperationResult::FillDetailListOperationContent( CXListCtrl& cxListCtrl,mapRunwayDetailOperation mapDetailData )
{
	mapRunwayDetailOperation::iterator iter = mapDetailData.begin();
	int idx = 0;
	for (; iter != mapDetailData.end(); ++iter)
	{
		MultiRunDetailMap::iterator detaiIter = iter->second.begin();
		for (; detaiIter != iter->second.end(); ++detaiIter)
		{
			CString strIndex;
			strIndex.Format(_T("%d"),idx+1);
			cxListCtrl.InsertItem(idx,strIndex);

			CString strSimName = iter->first;
			int nCurSimResult = atoi(strSimName.Mid(9,strSimName.GetLength()));
			CString strRun = _T("");
			strRun.Format(_T("Run%d"),nCurSimResult+1);

			cxListCtrl.SetItemText(idx,1,strRun);
			cxListCtrl.SetItemText(idx,2,detaiIter->first);
			cxListCtrl.SetItemData(idx,idx);

			for (size_t n = 0; n < detaiIter->second.size(); ++n)
			{
				MultipleRunReportData delayData = detaiIter->second.at(n);
				CString strCount;
				if(n <  iter->second.size())
					strCount.Format(_T("%d"), delayData.m_iData);
				else
					strCount.Format(_T("%d"),0) ;

				cxListCtrl.SetItemText(idx, n + 2, strCount);
			}
			idx++;
		}
	}
}

void CAirsideAircraftMutiRunRunwayOperationResult::SetDetail3DChartString( C2DChartData& c2dGraphData,CParameters *pParameter )
{
	ASSERT(pParameter);
	if (pParameter->getReportType() == ASReportType_Detail)
	{
		AirsideRunwayOperationReportParam* pOpPara = (AirsideRunwayOperationReportParam*)pParameter;
		switch (pOpPara->getSubType())
		{
		case AirsideRunwayOperationsReport::ChartType_Detail_LandingsByRunway:
			SetDetailLanding3DChartString(c2dGraphData,pParameter);
			break;
		case AirsideRunwayOperationsReport::ChartType_Detail_TakeOffByRunway:
			SetDetailTakeoff3DChartString(c2dGraphData,pParameter);
			break;
		case AirsideRunwayOperationsReport::ChartType_Detail_LeadTrailByRunway:
			SetDetailLandTrail3DChartString(c2dGraphData,pParameter);
			break;
		case AirsideRunwayOperationsReport::ChartType_Detail_MovementsPerRunway:
			SetDetailMovement3DChartString(c2dGraphData,pParameter);
			break;
		default:
			break;
		}
	}
}

void CAirsideAircraftMutiRunRunwayOperationResult::SetDetailLanding3DChartString( C2DChartData& c2dGraphData,CParameters *pParameter )
{
	c2dGraphData.m_strChartTitle = _T(" Landings by Runway ");
	c2dGraphData.m_strYtitle = _T("Number of Operations");
	c2dGraphData.m_strXtitle = _T("Time");

	//set footer
	CString strFooter(_T(""));
	strFooter.Format(_T("%s %s,%s "), c2dGraphData.m_strChartTitle,
		pParameter->getStartTime().printTime(), 
		pParameter->getEndTime().printTime());

}

void CAirsideAircraftMutiRunRunwayOperationResult::SetDetailTakeoff3DChartString( C2DChartData& c2dGraphData,CParameters *pParameter )
{
	c2dGraphData.m_strChartTitle = _T(" Take Off by Runway ");
	c2dGraphData.m_strYtitle = _T("Number of Operations");
	c2dGraphData.m_strXtitle = _T("Time");

	//set footer
	CString strFooter(_T(""));
	strFooter.Format(_T("%s %s,%s "), c2dGraphData.m_strChartTitle,
		pParameter->getStartTime().printTime(), 
		pParameter->getEndTime().printTime());

}

void CAirsideAircraftMutiRunRunwayOperationResult::SetDetailLandTrail3DChartString( C2DChartData& c2dGraphData,CParameters *pParameter )
{
	c2dGraphData.m_strChartTitle = _T(" Land Trail Operations by Runway ");


	c2dGraphData.m_strYtitle = _T("Number of Operations");
	c2dGraphData.m_strXtitle = _T("Runway");

	//set footer
	CString strFooter(_T(""));
	strFooter.Format(_T("%s %s,%s "), c2dGraphData.m_strChartTitle,
		pParameter->getStartTime().printTime(), 
		pParameter->getEndTime().printTime());
}

void CAirsideAircraftMutiRunRunwayOperationResult::SetDetailMovement3DChartString( C2DChartData& c2dGraphData,CParameters *pParameter )
{
	c2dGraphData.m_strChartTitle = _T(" Movements per Interval ");


	c2dGraphData.m_strYtitle = _T("Number of Operations");
	c2dGraphData.m_strXtitle = _T("Time of Day");

	//set footer
	CString strFooter(_T(""));
	strFooter.Format(_T("%s %s,%s "), c2dGraphData.m_strChartTitle,
		pParameter->getStartTime().printTime(), 
		pParameter->getEndTime().printTime());
}

void CAirsideAircraftMutiRunRunwayOperationResult::Generate3DChartCountData( mapRunwayDetailOperation mapDetailData,CARC3DChart& chartWnd, CParameters *pParameter,int iType )
{
	C2DChartData c2dGraphData;

	SetDetail3DChartString(c2dGraphData,pParameter);

	if (mapDetailData.empty() == true)
		return;

	RumwayMarkWithLandingTakeOff* pMark = (RumwayMarkWithLandingTakeOff*)iType;
	mapRunwayDetailOperation::iterator iter = mapDetailData.begin();
	CString strMarkOperation = pMark->GetMarkOperation();
	for (unsigned iTitle = 0; iTitle <(iter->second)[strMarkOperation].size(); iTitle++)
	{
		MultipleRunReportData delayData = iter->second[strMarkOperation].at(iTitle);
		ElapsedTime eStartTime;
		ElapsedTime eEndTime;
		eStartTime.setPrecisely(delayData.m_iStart);
		eEndTime.setPrecisely(delayData.m_iEnd);

		CString strTimeRange;
		strTimeRange.Format(_T("%02d:%02d:%02d-%02d:%02d:%02d"), eStartTime.GetHour(), eStartTime.GetMinute(),eStartTime.GetSecond(), eEndTime.GetHour(), eEndTime.GetSecond(),eEndTime.GetSecond());
		c2dGraphData.m_vrXTickTitle.push_back(strTimeRange);
	}

	for (iter = mapDetailData.begin(); iter != mapDetailData.end(); ++iter)
	{
		CString strSimName = iter->first;
		int nCurSimResult = atoi(strSimName.Mid(9,strSimName.GetLength()));
		CString strLegend;
		strLegend.Format(_T("Run%d"),nCurSimResult+1);
		c2dGraphData.m_vrLegend.push_back(strLegend);

		std::vector<double>  vData;
		for (unsigned i = 0; i <(iter->second)[strMarkOperation].size(); i++)
		{
			MultipleRunReportData delayData = (iter->second)[strMarkOperation].at(i);
			vData.push_back(delayData.m_iData);
		}
		c2dGraphData.m_vr2DChartData.push_back(vData);
	}
	chartWnd.DrawChart(c2dGraphData);
}

void CAirsideAircraftMutiRunRunwayOperationResult::Generate3DChartLeadTrailData( mapLandTrailOperation mapDetailData,CARC3DChart& chartWnd, CParameters *pParameter,int iType )
{
	C2DChartData c2dGraphData;

	SetDetail3DChartString(c2dGraphData,pParameter);

	if (mapDetailData.empty() == true)
		return;

	RumwayMarkWithLandingTakeOff* pMark = (RumwayMarkWithLandingTakeOff*)iType;
	mapLandTrailOperation::iterator iter = mapDetailData.begin();
	
	CString strMarkOperation = pMark->GetMarkOperation();
	for (unsigned iTitle = 0; iTitle <(iter->second)[strMarkOperation].size(); iTitle++)
	{
		ClassificationValue typeName = iter->second[strMarkOperation].at(iTitle);
		c2dGraphData.m_vrXTickTitle.push_back(typeName.m_strLandTrail);
	}

	for (iter = mapDetailData.begin(); iter != mapDetailData.end(); ++iter)
	{
		CString strSimName = iter->first;
		int nCurSimResult = atoi(strSimName.Mid(9,strSimName.GetLength()));
		CString strLegend;
		strLegend.Format(_T("Run%d"),nCurSimResult+1);
		c2dGraphData.m_vrLegend.push_back(strLegend);

		std::vector<double>  vData;
		for (unsigned i = 0; i < (iter->second)[strMarkOperation].size(); i++)
		{
			ClassificationValue typeName =(iter->second)[strMarkOperation].at(i);
			vData.push_back(typeName.m_lFlightCount);
		}
		c2dGraphData.m_vr2DChartData.push_back(vData);
	}
	chartWnd.DrawChart(c2dGraphData);
}

