#include "stdafx.h"
#include "ComparativePlot.h"
#include "../compare/ComparativeReportResult.h"
#include "../compare/ComparativeQTimeReport.h"
#include "../compare/ComparativeQLengthReport.h"
#include "../compare/ComparativeThroughputReport.h"
#include "../compare/ComparativeSpaceDensityReport.h"
#include "../compare/ComparativePaxCountReport.h"
#include "../compare/ComparativeAcOperationReport.h"
#include "../compare/ComparativeDistanceTravelReport.h"
#include "../compare/ComparativeTimeTerminalReport.h"
#include "../compare/ModelToCompare.h"
#include "../compare/ModelsManager.h"

static const OLE_COLOR UniqueColors[] =
{
	(OLE_COLOR)RGB(255, 0, 0),
	(OLE_COLOR)RGB(0, 255, 0),
	(OLE_COLOR)RGB(0, 0, 255),
	(OLE_COLOR)RGB(255, 255, 0),
	(OLE_COLOR)RGB(0, 255, 255),
	(OLE_COLOR)RGB(255, 0, 255),
	(OLE_COLOR)RGB(0, 0, 0),
	(OLE_COLOR)RGB(255, 255, 255)
};

static void GetGradientColors(std::vector<COLORREF>& vGradientColors, COLORREF color1, COLORREF color2, size_t count)
{
	vGradientColors.clear();
	vGradientColors.resize(count, 0);

	BYTE r1 = GetRValue(color1);
	BYTE g1 = GetGValue(color1);
	BYTE b1 = GetBValue(color1);

	BYTE r2 = GetRValue(color2);
	BYTE g2 = GetGValue(color2);
	BYTE b2 = GetBValue(color2);

	char rStep = (r2 - r1) / count;
	char gStep = (g2 - g1) / count;
	char bStep = (b2 - b1) / count;

	COLORREF tempColor = 0;
	for (size_t i = 0; i < count; ++i)
	{
		tempColor = RGB((r1 + i * rStep), (g1 + i * gStep), (b1 + i * bStep));
		vGradientColors[i] = tempColor;
	}
}

static void GetUniqueColors(std::vector<COLORREF>& vUniqueColors, size_t count)
{
	vUniqueColors.clear();
	vUniqueColors.resize(count, 0);

	for (size_t i = 0; i < count; ++i)
		vUniqueColors[i] = UniqueColors[i % (sizeof(UniqueColors)/sizeof(OLE_COLOR))];
}

bool CComparativePlot::Draw3DChart(CComparativeReportResult& _reportData)
{
	bool bResult = true;

	try
	{
		Init3DChart();

		switch(_reportData.GetReportType())
		{
		case QueueTimeReport:
			bResult = Draw3DChart((CComparativeQTimeReport&)_reportData);
			break;
		case QueueLengthReport:
			bResult = Draw3DChart((CComparativeQLengthReport&)_reportData);
			break;
		case ThroughtputReport:
			bResult = Draw3DChart((CComparativeThroughputReport&)_reportData);
			break;
		case SpaceDensityReport:
			bResult = Draw3DChart((CComparativeSpaceDensityReport&)_reportData);
			break;
		case PaxCountReport:
			bResult = Draw3DChart((CComparativePaxCountReport&)_reportData);
			break;
		case AcOperationReport:
			bResult = Draw3DChart((CComparativeAcOperationReport&)_reportData);
			break;
		case TimeTerminalReport:
			bResult = Draw3DChart((CComparativeTimeTerminalReport&)_reportData);
			break;
		case DistanceTravelReport:
			bResult = Draw3DChart((CComparativeDistanceTravelReport&)_reportData);
			break;

		default:
			ASSERT(FALSE);
			break;
		}

		UniformAppearances();
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		bResult = false;
	}

	return bResult;
}

void CComparativePlot::Init3DChart()
{
// 	if(m_pChart == NULL)
// 		return ;

// 	TCHAR szFileName[MAX_PATH];
// 	GetModuleFileName(NULL, szFileName, MAX_PATH);
// 	CString strFilePath(szFileName);
// 	strFilePath = strFilePath.Left(strFilePath.ReverseFind('\\'));
// 	strFilePath += _T("\\ChartState.nsv");
// 
// 	_variant_t varFilePath(strFilePath);
// 	m_pChart->ImportExport()->Load(&varFilePath);
// 
// 	m_pChart->Categories()->RemoveAll();
// 
// 	m_pChart->Series()->RemoveAll();
// 	m_pChart->Series()->Add(_bstr_t("One"), VARIANT_TRUE);
// 
// 	m_pChart->Levels()->RemoveAll();
// 	m_pChart->LabelCollection()->RemoveAll();
// 	m_pChart->LabelCollection()->Add();
// 
// 	m_pChart->PresentGroups()->RemoveAll();
// 	m_pChart->PresentGroups()->Add(pgtBar);
// 	m_pChart->PresentGroups()->Add(pgtLine);
// 
// 	for(int i = 0; i < (int)m_vModelList.size(); i++)
// 	{
// 		CModelToCompare *pModel = m_vModelList[i];
// 		int j =0;
// 		for (int j= 0;j < pModel->GetSimResultCount(); ++j)
// 		{
// 			CString strText = _T("");
// 			strText.Format(_T("%s(%s)"),pModel->GetModelName(),pModel->GetSimResult(j));
// 			m_pChart->Levels()->Add(_bstr_t(strText), VARIANT_TRUE);
// 		}
// 	}
}

void CComparativePlot::UniformAppearances()
{
	int nIndex = 0;
	CString strText;

// 	size_t nSimCount = m_.size();
// 	std::vector<COLORREF> vUniqueColors, vGradientColors;
// 	GetUniqueColors(vUniqueColors, 7);
// 
// 	for(size_t i = 0; i < nSimCount; i++)
// 	{
// 		CModelToCompare* pModel = m_vModelList[i];
// 		size_t nResultCount = pModel->GetSimResultCount();
// 		GetGradientColors(vGradientColors, vUniqueColors[i], RGB(255, 255, 255), nResultCount);
// 
// 		for (size_t j= 0; j < nResultCount; ++j)
// 		{
// 			strText.Format(_T("%s(%s)"), pModel->GetModelName(), pModel->GetSimResult(j));
// 			COLORREF color = vGradientColors[j];

// 			pBar->Common()->UniformAppearances()->Add(_bstr_t(strText));
// 			pBar->Common()->UniformAppearances()->GetAt(nIndex)->FillEffect()->SetSolidColor(color);

			// Need 2 appearances for each line, one for the line part of the line and one for the data point. 
// 			for(int m = 0; m < 2; ++m)
// 			{
				//IDualAppearancePtr pApperance = pLine->Common()->UniformAppearances()->Add(_bstr_t(strText));
// 				pApperance->FillEffect()->Color = color;
// 				pApperance->Border()->Color = color;
// 				pApperance->Border()->Width = 3;
// 			}

// 			nIndex++;
// 		}
// 	}

	switch(m_iType)
	{
	case CMPBar_2D:
// 		pLine->Common()->Visible = TRUE;
// 		pLine->Common()->ShowInLegend = TRUE;
		break;

	case CMPLine_2D:
// 		pBar->Common()->Visible = TRUE;
// 		pBar->Common()->ShowInLegend = TRUE;
		break;

	case CMPBar_3D:
// 		pLine->Common()->Visible = TRUE;
// 		pLine->Common()->ShowInLegend = TRUE;
// 		m_pChart->Get3DView()->ProjectionType = ptPerspective;
		break;

	default:
		break;
	}
}

bool CComparativePlot::Draw3DChart(CComparativeQTimeReport& _reportData)
{
	const QTimeMap& mapQTime = _reportData.GetResult();

	C2DChartData c2dGraphData;
	// Update Title
	c2dGraphData.m_strChartTitle = _T(" Queue Time Report ");
	c2dGraphData.m_strYtitle = _T("Passenger Count");
	c2dGraphData.m_strXtitle = _T("Time");

	//set footer
	CString strFooter;
	c2dGraphData.m_strFooter = strFooter;

	// Alloc data space
	if( mapQTime.size()>0)
	{
		const std::vector<int>& vLength = mapQTime.begin()->second;
		std::vector<double> vSegmentData(mapQTime.size());
		vSegmentData.clear();
		for(int nSeg = 0; nSeg < (int)vLength.size(); nSeg++)
		{
			//c2dGraphData.m_vrLegend.push_back(mapQTime.first);
			c2dGraphData.m_vr2DChartData.push_back(vSegmentData);
		}	
	}

	// Insert legend.
	std::vector<CString> vSimName  =  _reportData.GetSimNameList();
	int simNameCount = vSimName.size();
	for(int i=0; i<simNameCount; i++)
	{
		c2dGraphData.m_vrLegend.push_back(vSimName[i]);
	}

	// Insert data
	CString XTickTitle;
	std::vector<CString> vXTickTitle;
	ElapsedTime tDuration, tPrev;
	tDuration.set(0, 0, 0);
	int nXTick = 0;
	for( QTimeMap::const_iterator iterLine = mapQTime.begin(); iterLine != mapQTime.end(); iterLine++, nXTick++)
	{
		const std::vector<int>& vLength = iterLine->second;

		if (tDuration.asSeconds() == 0)
			tDuration = iterLine->first;
		tPrev = iterLine->first - tDuration;
		XTickTitle = tPrev.printTime();
		XTickTitle += _T(" - ");
		XTickTitle += iterLine->first.printTime();
		vXTickTitle.push_back(XTickTitle);
		for(int nSeg = 0; nSeg < (int)vLength.size(); nSeg++)
		{
			(c2dGraphData.m_vr2DChartData[nSeg]).push_back((double)vLength[nSeg]);
		}
	}

	c2dGraphData.m_vrXTickTitle = vXTickTitle;
	m_3DChart.DrawChart(c2dGraphData);
	return true;
}

bool CComparativePlot::Draw3DChart(CComparativeQLengthReport& _reportData)
{
	const QLengthMap& mapQLength = _reportData.GetResult();
	C2DChartData c2dGraphData;
	// Update Title
	c2dGraphData.m_strChartTitle = _T(" Queue Length Report ");
	c2dGraphData.m_strYtitle = _T("Queue Length");
	c2dGraphData.m_strXtitle = _T("Time");

	//set footer
	CString strFooter;
	c2dGraphData.m_strFooter = strFooter;

	// Alloc data space
	if( mapQLength.size()>0)
	{
		const std::vector<int>& vLength = mapQLength.begin()->second;
		std::vector<double> vSegmentData(mapQLength.size());
		vSegmentData.clear();
		for(int nSeg = 0; nSeg < (int)vLength.size(); nSeg++)
		{
			c2dGraphData.m_vr2DChartData.push_back(vSegmentData);
		}
	}

	// Insert legend.
	std::vector<CString> vSimName  =  _reportData.GetSimNameList();
	int simNameCount = vSimName.size();
	for(int i=0; i<simNameCount; i++)
	{
		c2dGraphData.m_vrLegend.push_back(vSimName[i]);
	}

	// Insert data
	CString XTickTitle;
	std::vector<CString> vXTickTitle;
	CString sTime;
	int nXTick = 0;

	for( QLengthMap::const_iterator iterLine = mapQLength.begin(); iterLine != mapQLength.end(); iterLine++, nXTick++)
	{
		const std::vector<int>& vLength = iterLine->second;

		//set row label
		ElapsedTime t = iterLine->first;
		t.set(t.asSeconds() % WholeDay);
		t.printTime(sTime.GetBuffer(32), FALSE);
		sTime.ReleaseBuffer();
		XTickTitle = sTime;
		vXTickTitle.push_back(XTickTitle);
		//set data
		for(int nSeg = 0; nSeg < (int)vLength.size(); nSeg++)
		{
			(c2dGraphData.m_vr2DChartData[nSeg]).push_back((double)vLength[nSeg]);
		}
	}
	c2dGraphData.m_vrXTickTitle = vXTickTitle;
	m_3DChart.DrawChart(c2dGraphData);
	return true;
}

bool CComparativePlot::Draw3DChart(CComparativeThroughputReport& _reportData)
{
	const std::vector<CompThroughputData>& vData = _reportData.GetResult();
	C2DChartData c2dGraphData;
	// Update Title
	c2dGraphData.m_strChartTitle = _T(" Throughput Report ");
	c2dGraphData.m_strYtitle = _T("Units Served");
	c2dGraphData.m_strXtitle = _T("Time of Day");

	//set footer
	CString strFooter;
	c2dGraphData.m_strFooter = strFooter;
	
	// Alloc data space
	if( vData.size()>0)
	{
		int simCount = vData.begin()->vPaxServed.size();
		std::vector<double> vSimData(vData.size());
		vSimData.clear();
		for(int nSim = 0; nSim < simCount; nSim++)
		{
			c2dGraphData.m_vr2DChartData.push_back(vSimData);
		}	
	}	

	// Insert legend.
	std::vector<CString> vSimName  =  _reportData.GetSimNameList();
	int simNameCount = vSimName.size();
	for(int i=0; i<simNameCount; i++)
	{
		c2dGraphData.m_vrLegend.push_back(vSimName[i]);
	}
	
	// Insert data
	CString XTickTitle;
	std::vector<CString> vXTickTitle;
	CString sTime;
	int nXTick = 0;

	for(std::vector<CompThroughputData>::const_iterator iterLine = vData.begin(); 
		iterLine != vData.end(); iterLine++, nXTick++)
	{
		//set row label
		ElapsedTime t = iterLine->etStart;
		t.set(t.asSeconds() % WholeDay);
		XTickTitle = t.printTime(0);
		XTickTitle += "~";
		t = iterLine->etEnd;
		t.set(t.asSeconds() % WholeDay);
		XTickTitle += t.printTime(0);
		vXTickTitle.push_back(XTickTitle);
		//set data
		const std::vector<int>& vLength = iterLine->vPaxServed;
		for(int nSeg = 0; nSeg < (int)vLength.size(); nSeg++)
		{
			(c2dGraphData.m_vr2DChartData[nSeg]).push_back((double)vLength[nSeg]);
		}
	}
	c2dGraphData.m_vrXTickTitle = vXTickTitle;
	m_3DChart.DrawChart(c2dGraphData);
	return true;
}

bool CComparativePlot::Draw3DChart(CComparativeSpaceDensityReport& _reportData)
{
	const PaxDensMap& mapPaxDens = _reportData.GetResult();
	C2DChartData c2dGraphData;
	// Update Title
	c2dGraphData.m_strChartTitle = _T(" Space Density Report ");
	c2dGraphData.m_strYtitle = _T("Passengers In Area");
	c2dGraphData.m_strXtitle = _T("Time of Day");

	//set footer
	CString strFooter;
	c2dGraphData.m_strFooter = strFooter;

	// Alloc data space
	if( mapPaxDens.size()>0)
	{
		int simCount = mapPaxDens.begin()->second.size();
		std::vector<double> vSegmentData(mapPaxDens.size());
		vSegmentData.clear();
		for(int nSeg = 0; nSeg < simCount; nSeg++)
		{
			c2dGraphData.m_vr2DChartData.push_back(vSegmentData);
		}	
	}

	// Insert legend.
	std::vector<CString> vSimName  =  _reportData.GetSimNameList();
	int simNameCount = vSimName.size();
	for(int i=0; i<simNameCount; i++)
	{
		c2dGraphData.m_vrLegend.push_back(vSimName[i]);
	}

	// Insert data
	CString XTickTitle;
	std::vector<CString> vXTickTitle;
	CString sTime;
	int nXTick = 0;

	for( PaxDensMap::const_iterator iterLine = mapPaxDens.begin(); iterLine != mapPaxDens.end(); iterLine++, nXTick++)
	{
		const std::vector<int>& vLength = iterLine->second;

		//set row label
		ElapsedTime t = iterLine->first;
		t.set(t.asSeconds() % WholeDay);
		t.printTime(sTime.GetBuffer(32), FALSE);
		sTime.ReleaseBuffer();
		XTickTitle = sTime;
		vXTickTitle.push_back(XTickTitle);
		//set data
		for(int nSeg = 0; nSeg < (int)vLength.size(); nSeg++)
		{
			(c2dGraphData.m_vr2DChartData[nSeg]).push_back((double)vLength[nSeg]);
		}
	}
	c2dGraphData.m_vrXTickTitle = vXTickTitle;
	m_3DChart.DrawChart(c2dGraphData);
	return true;
}

bool CComparativePlot::Draw3DChart(CComparativePaxCountReport& _reportData)
{
	const PaxCountMap& mapPaxCount = _reportData.GetResult();
	C2DChartData c2dGraphData;
	// Update Title
	c2dGraphData.m_strChartTitle = _T(" Pax Count Report ");
	c2dGraphData.m_strYtitle = _T("Passengers In Area");
	c2dGraphData.m_strXtitle = _T("Time of Day");

	//set footer
	CString strFooter;
	c2dGraphData.m_strFooter = strFooter;

	// Alloc data space
	if( mapPaxCount.size()>0)
	{
		int simCount = mapPaxCount.begin()->second.size();
		std::vector<double> vSegmentData(mapPaxCount.size());
		vSegmentData.clear();
		for(int nSeg = 0; nSeg < simCount; nSeg++)
		{
			c2dGraphData.m_vr2DChartData.push_back(vSegmentData);
		}	
	}

	// Insert legend.
	std::vector<CString> vSimName  =  _reportData.GetSimNameList();
	int simNameCount = vSimName.size();
	for(int i=0; i<simNameCount; i++)
	{
		c2dGraphData.m_vrLegend.push_back(vSimName[i]);
	}

	// Insert data
	CString XTickTitle;
	std::vector<CString> vXTickTitle;
	CString sTime;
	int nXTick = 0;

	for( PaxDensMap::const_iterator iterLine = mapPaxCount.begin(); iterLine != mapPaxCount.end(); iterLine++, nXTick++)
	{
		const std::vector<int>& vLength = iterLine->second;

		//set row label
		ElapsedTime t = iterLine->first;
		t.set(t.asSeconds() % WholeDay);
		t.printTime(sTime.GetBuffer(32), FALSE);
		sTime.ReleaseBuffer();
		XTickTitle = sTime;
		vXTickTitle.push_back(XTickTitle);
		//set data
		for(int nSeg = 0; nSeg < (int)vLength.size(); nSeg++)
		{
			(c2dGraphData.m_vr2DChartData[nSeg]).push_back((double)vLength[nSeg]);
		}
	}
	c2dGraphData.m_vrXTickTitle = vXTickTitle;
	m_3DChart.DrawChart(c2dGraphData);
	return true;
}

bool CComparativePlot::Draw3DChart(CComparativeAcOperationReport& _reportData)
{
	const AcOperationMap& mapPaxDens = _reportData.GetResult();

	C2DChartData c2dGraphData;
	// Update Title
	c2dGraphData.m_strChartTitle = _T(" AcOperation Report ");
	c2dGraphData.m_strYtitle = _T("Number of Operation");
	c2dGraphData.m_strXtitle = _T("Time of Day");

	//set footer
	CString strFooter;
	c2dGraphData.m_strFooter = strFooter;

	// Alloc data space
	if( mapPaxDens.size()>0)
	{
		int simCount = mapPaxDens.begin()->second.size();
		std::vector<double> vSegmentData(mapPaxDens.size());
		vSegmentData.clear();
		for(int nSeg = 0; nSeg < simCount; nSeg++)
		{
			c2dGraphData.m_vr2DChartData.push_back(vSegmentData);
		}	
	}

	// Insert legend.
	std::vector<CString> vSimName  =  _reportData.GetSimNameList();
	int simNameCount = vSimName.size();
	for(int i=0; i<simNameCount; i++)
	{
		c2dGraphData.m_vrLegend.push_back(vSimName[i]);
	}

	// Insert data
	CString XTickTitle;
	std::vector<CString> vXTickTitle;
	CString sTime;
	int nXTick = 0;

	for( PaxDensMap::const_iterator iterLine = mapPaxDens.begin(); iterLine != mapPaxDens.end(); iterLine++, nXTick++)
	{
		const std::vector<int>& vLength = iterLine->second;

		//set row label
		ElapsedTime t = iterLine->first;
		t.set(t.asSeconds() % WholeDay);
		t.printTime(sTime.GetBuffer(32), FALSE);
		sTime.ReleaseBuffer();
		XTickTitle = sTime;

		vXTickTitle.push_back(XTickTitle);
		//set data
		for(int nSeg = 0; nSeg < (int)vLength.size(); nSeg++)
		{
			(c2dGraphData.m_vr2DChartData[nSeg]).push_back((double)vLength[nSeg]);
		}
	}
	c2dGraphData.m_vrXTickTitle = vXTickTitle;
	m_3DChart.DrawChart(c2dGraphData);
	return true;
}

bool CComparativePlot::Draw3DChart(CComparativeTimeTerminalReport& _reportData)
{
	const QTimeMap& mapQTime = _reportData.GetResult();

	C2DChartData c2dGraphData;
	// Update Title
	c2dGraphData.m_strChartTitle = _T(" Terminal Time Report ");
	c2dGraphData.m_strYtitle = _T("Passenger Count");
	c2dGraphData.m_strXtitle = _T("Time");

	//set footer
	CString strFooter;
	c2dGraphData.m_strFooter = strFooter;

	// Alloc data space
	if( mapQTime.size()>0)
	{
		const std::vector<int>& vLength = mapQTime.begin()->second;
		std::vector<double> vSegmentData(mapQTime.size());
		vSegmentData.clear();
		for(int nSeg = 0; nSeg < (int)vLength.size(); nSeg++)
		{
			//c2dGraphData.m_vrLegend.push_back(mapQTime.first);
			c2dGraphData.m_vr2DChartData.push_back(vSegmentData);
		}	
	}

	// Insert legend.
	std::vector<CString> vSimName  =  _reportData.GetSimNameList();
	int simNameCount = vSimName.size();
	for(int i=0; i<simNameCount; i++)
	{
		c2dGraphData.m_vrLegend.push_back(vSimName[i]);
	}

	// Insert data
	CString XTickTitle;
	std::vector<CString> vXTickTitle;
	ElapsedTime tDuration, tPrev;
	tDuration.set(0, 0, 0);
	int nXTick = 0;
	for( QTimeMap::const_iterator iterLine = mapQTime.begin(); iterLine != mapQTime.end(); iterLine++, nXTick++)
	{
		const std::vector<int>& vLength = iterLine->second;

		if (tDuration.asSeconds() == 0)
			tDuration = iterLine->first;
		tPrev = iterLine->first - tDuration;
		XTickTitle = tPrev.printTime();
		XTickTitle += _T(" - ");
		XTickTitle += iterLine->first.printTime();
		vXTickTitle.push_back(XTickTitle);
		for(int nSeg = 0; nSeg < (int)vLength.size(); nSeg++)
		{
			(c2dGraphData.m_vr2DChartData[nSeg]).push_back((double)vLength[nSeg]);
		}
	}

	c2dGraphData.m_vrXTickTitle = vXTickTitle;
	m_3DChart.DrawChart(c2dGraphData);
	return true;
}

bool CComparativePlot::Draw3DChart(CComparativeDistanceTravelReport & _reportData)
{
	const DistanceMap& mapDistance = _reportData.GetResult();

	C2DChartData c2dGraphData;
	// Update Title
	c2dGraphData.m_strChartTitle = _T(" Distance Travel Report ");
	c2dGraphData.m_strYtitle = _T("Passenger Count");
	c2dGraphData.m_strXtitle = _T("Distance");

	//set footer
	CString strFooter;
	c2dGraphData.m_strFooter = strFooter;

	// Alloc data space
	if( mapDistance.size()>0)
	{
		const std::vector<int>& vLength = mapDistance.begin()->second;
		std::vector<double> vSegmentData(mapDistance.size());
		vSegmentData.clear();
		for(int nSeg = 0; nSeg < (int)vLength.size(); nSeg++)
		{
			//c2dGraphData.m_vrLegend.push_back(mapQTime.first);
			c2dGraphData.m_vr2DChartData.push_back(vSegmentData);
		}	
	}

	// Insert legend.
	std::vector<CString> vSimName  =  _reportData.GetSimNameList();
	int simNameCount = vSimName.size();
	for(int i=0; i<simNameCount; i++)
	{
		c2dGraphData.m_vrLegend.push_back(vSimName[i]);
	}

	// Insert data
	CString XTickTitle;
	std::vector<CString> vXTickTitle;
	LONG tDuration = 0L, tPrev = 0L;
	int nXTick = 0;
	for( DistanceMap::const_iterator iterLine = mapDistance.begin(); iterLine != mapDistance.end(); iterLine++, nXTick++)
	{
		const std::vector<int>& vLength = iterLine->second;

		if (tDuration == 0)
			tDuration = iterLine->first;

		tPrev = iterLine->first - tDuration;
		XTickTitle.Format("%d - %d",tPrev,iterLine->first);
		vXTickTitle.push_back(XTickTitle);
		for(int nSeg = 0; nSeg < (int)vLength.size(); nSeg++)
		{
			(c2dGraphData.m_vr2DChartData[nSeg]).push_back((double)vLength[nSeg]);
		}
	}

	c2dGraphData.m_vrXTickTitle = vXTickTitle;
	m_3DChart.DrawChart(c2dGraphData);
	return true;
}

bool CComparativePlot::Update3DChart(ThreeDChartType iType)
{
	m_iType = iType;
	return true;
}