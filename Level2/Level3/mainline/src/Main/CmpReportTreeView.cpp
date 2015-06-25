#include "stdafx.h"
#include "TermPlan.h"
#include "CompareReportDoc.h"
#include "CmpReportTreeView.h"

#include "..\compare\ReportDef.h"
#include "..\compare\InputParameter.h"
#include "..\compare\ModelToCompare.h"
#include "..\Main\CompRepLogBar.h"
#include "..\main\resource.h"
#include "..\main\ModelSelectionDlg.h"
#include "..\Main\MainFrm.h"
#include "..\Main\ReportProperty.h"

#include "..\common\elaptime.h"
#include "..\common\SimAndReportManager.h"
#include "..\common\EchoSystem.h"

static const UINT ID_RUN = 101;
static const UINT ID_CANCEL = 102;
static const UINT ID_MAIN_TREE = 103;
static const UINT MENU_ADD_MODEL = 200;
static const UINT MENU_ADD_REPORT = 203;
static const UINT MENU_DELETE_MODEL = 206;
static const UINT MENU_EDIT_REPORT = 207;
static const UINT MENU_DELETE_REPORT = 208;
static const UINT MENU_UNAVAILABLE = 220;

static const int BUTTON_AREA_HEIGHT = 50;
static const int BUTTON_HEIGHT = 22;
static const int BUTTON_WIDTH = 80;

IMPLEMENT_DYNCREATE(CCmpReportTreeView, CFormView)
BEGIN_MESSAGE_MAP(CCmpReportTreeView, CFormView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_RUN, OnRun)
	ON_WM_TIMER()

	ON_COMMAND_RANGE(MENU_ADD_MODEL, MENU_UNAVAILABLE, OnChooseMenu)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

CCmpReportTreeView::CCmpReportTreeView()
	: CFormView(CCmpReportTreeView::IDD)
{
	LOGFONT lf;
	::ZeroMemory(&lf, sizeof(LOGFONT));
	lf.lfHeight = 8;
	strcpy(lf.lfFaceName, "MS Sans Serif");
	m_font.CreateFontIndirect(&lf);
	m_pSubMenu = NULL;
	m_pCmpReport = NULL;
}

CCmpReportTreeView::~CCmpReportTreeView()
{
	if(m_font.m_hObject != NULL)
	{
		m_font.DeleteObject();
		m_font.m_hObject = NULL;
	}
}

void CCmpReportTreeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}
void CCmpReportTreeView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

#ifdef _DEBUG
void CCmpReportTreeView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCmpReportTreeView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

int CCmpReportTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect emptyRect;
	emptyRect.SetRectEmpty();
	m_propTree.Create(WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_BORDER| TVS_HASBUTTONS | TVS_LINESATROOT | 
		TVS_HASLINES | TVS_DISABLEDRAGDROP | TVS_NOTOOLTIPS ,
		emptyRect, this, ID_MAIN_TREE);
	m_propTree.SetFont(&m_font);

	m_btnRun.Create(_T("Run"), WS_VISIBLE|WS_CHILD, emptyRect, this, ID_RUN);
	m_btnCancel.Create(_T("Cancel"), WS_VISIBLE|WS_CHILD, emptyRect, this, ID_CANCEL);

	m_btnRun.SetFont(&m_font);
	m_btnCancel.SetFont(&m_font);

	m_nMenu.LoadMenu(IDR_CTX_COMPREPORT);

	return 0;
}


void CCmpReportTreeView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	CCompareReportDoc* pDoc = (CCompareReportDoc*)GetDocument();
	m_pCmpReport = pDoc->GetCmpReport();
	InitParaWnd();
}

void CCmpReportTreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if(!IsWindowVisible())
		return;
	UpdateParaWnd();
}

void CCmpReportTreeView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	CDocument* pDoc = GetDocument();
	if (::IsWindow(m_propTree.m_hWnd))
		m_propTree.MoveWindow(0, 0, cx, (cy - BUTTON_AREA_HEIGHT));

	int x = 0, y = 0;
	x = cx - 15 - BUTTON_WIDTH;
	y = cy - BUTTON_AREA_HEIGHT + 10;
	if (::IsWindow(m_btnCancel.m_hWnd))
		m_btnCancel.MoveWindow(x, y, BUTTON_WIDTH, BUTTON_HEIGHT);
	x = cx - (15 + BUTTON_WIDTH)*2;
	if (::IsWindow(m_btnRun.m_hWnd))
		m_btnRun.MoveWindow(x, y, BUTTON_WIDTH, BUTTON_HEIGHT);
}

BOOL CCmpReportTreeView::OnEraseBkgnd(CDC* pDC)
{
	CRect rectClient;
	GetClientRect(&rectClient);
	rectClient.top = rectClient.bottom - BUTTON_AREA_HEIGHT;
	pDC->FillSolidRect(&rectClient, ::GetSysColor(COLOR_BTNFACE));

	return CFormView::OnEraseBkgnd(pDC);
}

void CCmpReportTreeView::InitParaWnd()
{
	COOLTREE_NODE_INFO cni;
	InitCooltreeNodeInfo(this, cni);

	m_hBasicInfo = m_propTree.InsertItem(_T("Basic Info"), cni, FALSE, FALSE, TVI_ROOT);
	m_hModelRoot = m_propTree.InsertItem(_T("Models"),cni, FALSE, FALSE, TVI_ROOT);
	m_hReportRoot = m_propTree.InsertItem(_T("Reports"),cni, FALSE, FALSE, TVI_ROOT);

	cni.net = NET_EDIT_WITH_VALUE;
	m_hProjName = m_propTree.InsertItem(_T("Name"), cni, FALSE, FALSE,m_hBasicInfo, TVI_LAST);
	m_hProjDesc = m_propTree.InsertItem(_T("Description"),cni, FALSE, FALSE, m_hBasicInfo, TVI_LAST);

	m_propTree.Expand(m_hBasicInfo, TVE_EXPAND);
}

BOOL CCmpReportTreeView::CheckData()
{
	CInputParameter* pInputParam = m_pCmpReport->GetComparativeProject()->GetInputParam();
	//	Any models
	if (!pInputParam->GetModelsManagerPtr()->GetModelsList().size())
	{
		AfxMessageBox(_T("Please assign one or more models."));
		return FALSE;
	}

	//	Any Reports
	if (!pInputParam->GetReportsManagerPtr()->GetReportsList().size())
	{
		AfxMessageBox(_T("Please assign one or more reports."));
		return FALSE;
	}

	return TRUE;
}
void CCmpReportTreeView::OnRun()
{
	if (CheckData())
	{
		if (m_pCmpReport->SaveProject())
			SetTimer(100, 500, NULL);
		else
			AfxMessageBox(_T("Save data failed!"));
	}
}

//CStatusBarXP* __statusBar = NULL;//todo

static void CALLBACK _ShowCopyInfo(LPCTSTR strPath)
{
	CString fileName;
	CString strFilePath = CString(strPath);
	int nPos = strFilePath.ReverseFind('\\');
	fileName = strFilePath.Mid(nPos + 1);
	CString strMsg;
	strMsg = _T("Copying   ") + fileName;
	//__statusBar->SetPaneText(0,strMsg);
}
void CCmpReportTreeView::RunCompareReport()
{
	CMainFrame* pFram = (CMainFrame*)AfxGetMainWnd();
	CWnd* pWnd = &(pFram->m_wndCompRepLogBar);
	((CCompRepLogBar*)pWnd)->m_pProj = m_pCmpReport->GetComparativeProject();
	((CCompRepLogBar*)pWnd)->SetParentIndex(1);

	pFram->ShowControlBar((CToolBar*) pWnd, TRUE, FALSE);//!bIsShown
	CRect rc1, rc2;
	pFram->m_wndCompRepLogBar.GetWindowRect(&rc1);
	pFram->m_wndCompRepLogBar.GetClientRect(&rc2);
	pFram->ChangeSize(rc1, rc2, 1);

	((CCompRepLogBar*)pWnd)->DeleteLogText();
	//__statusBar = &(pFram->m_wndStatusBar);
	m_pCmpReport->Run(this->GetSafeHwnd(), (CCompRepLogBar*)pWnd,_ShowCopyInfo);
	((CCompRepLogBar*)pWnd)->SetProgressPos( 0);
	GetDocument()->UpdateAllViews(this);
//	m_pReportManager->DisplayReport();
}

void CCmpReportTreeView::RemoveSubItem( HTREEITEM hItem )
{
	if(hItem == NULL)
		return;
	HTREEITEM hChildItem;
	while((hChildItem = m_propTree.GetChildItem(hItem)) != NULL)
	{
		RemoveSubItem(hChildItem);
		m_propTree.DeleteItem(hChildItem);
	}
}

void CCmpReportTreeView::UpdateParaItem( HTREEITEM hItem )
{
	CString strItemText;
	if(hItem == NULL)
		return;
	if(hItem == m_hProjName)
	{
		CString strName = m_pCmpReport->GetComparativeProject()->GetName();
		if(strName.IsEmpty())
			strItemText = "Name";
		else
			strItemText.Format("Name: %s", m_pCmpReport->GetComparativeProject()->GetName());
		m_propTree.SetItemText(hItem, strItemText);
	}
	else if(hItem == m_hProjDesc)
	{
		CString strDesc = m_pCmpReport->GetComparativeProject()->GetDescription();
		if(strDesc.IsEmpty())
			strItemText = "Description";
		else
			strItemText.Format("Description: %s", strDesc);
		m_propTree.SetItemText(hItem, strItemText);
	}
	else if(hItem == m_hModelRoot)
	{
		RemoveSubItem(m_hModelRoot);
		COOLTREE_NODE_INFO cni;
		InitCooltreeNodeInfo(this, cni);
		CModelsManager* pManager = m_pCmpReport->GetComparativeProject()->GetInputParam()->GetModelsManagerPtr();
		for(int i = 0; i < (int)pManager->GetModelsList().size(); i++)
		{

			CString strModel = "";
			strModel.Format("Model-%d", i);
			HTREEITEM hModel = m_propTree.InsertItem(strModel, cni, FALSE, FALSE, m_hModelRoot);

			CModelToCompare *pModelToCompare = pManager->GetModelsList().at(i);
			CString strName = pModelToCompare->GetUniqueName();
			strItemText.Format("Name: %s", strName);
			m_propTree.InsertItem(strItemText, cni, FALSE, FALSE, hModel);
			int nSimCount = pModelToCompare->GetSimResultCount();
			CString strSimResult;
			for (int j = 0; j < nSimCount; ++j)
			{
				strSimResult += pModelToCompare->GetSimResult(j);
				strSimResult += ",";
			}
			strSimResult.TrimRight(_T(","));
			strItemText.Format("SimResult: %s", strSimResult);
			HTREEITEM hSimResultItem = m_propTree.InsertItem(strItemText, cni, FALSE, FALSE, hModel);
			CString strPath = pModelToCompare->GetModelLocation();
			int nLenName = strName.GetLength();
			int nLenPath = strPath.GetLength();
			strPath = strPath.Left(nLenPath-nLenName-1);
			strItemText.Format("Path: %s", strPath);
			HTREEITEM hPathItem = m_propTree.InsertItem(strItemText, cni, FALSE, FALSE, hModel);
			m_propTree.Expand(hModel, TVE_EXPAND);
		}
		m_propTree.Expand(m_hModelRoot, TVE_EXPAND);
	}
	else if(hItem == m_hReportRoot)
	{
		RemoveSubItem(m_hReportRoot);
		COOLTREE_NODE_INFO cni;
		InitCooltreeNodeInfo(this, cni);
		CReportsManager* pRManager = m_pCmpReport->GetComparativeProject()->GetInputParam()->GetReportsManagerPtr();
		std::vector<CReportToCompare>& vReports = pRManager->GetReportsList();
		for (int i = 0; i < static_cast<int>(vReports.size()); i++)
		{
			const CReportToCompare& report = vReports.at(i);
			HTREEITEM hItem2 = m_propTree.InsertItem(report.GetName(), cni, FALSE, FALSE, m_hReportRoot);
			CReportParamToCompare param = report.GetParameter();
			int iIndex = -1;
			switch (report.GetCategory())
			{
			case ENUM_QUEUELENGTH_REP:
				iIndex = 0;
				break;
			case ENUM_QUEUETIME_REP:
				iIndex = 1;
				break;
			case ENUM_THROUGHPUT_REP:
				iIndex = 2;
				break;
			case ENUM_PAXDENS_REP:
				iIndex = 3;
				break;
			case ENUM_PAXCOUNT_REP:
				iIndex = 4;
				break;	
			case ENUM_ACOPERATION_REP:
				iIndex = 5;
				break;
			case ENUM_DURATION_REP:
				iIndex = 6;
				break;
			case ENUM_DISTANCE_REP:
				iIndex = 7;
				break;
			}
			if (iIndex == -1)
			{
				continue;
			}
			CString strTemp = s_szReportCategoryName[iIndex];
			strItemText.Format("Report Type: %s", strTemp);
			HTREEITEM hRepName = m_propTree.InsertItem(strItemText, cni, FALSE, FALSE, hItem2);

			strTemp = GetRegularDateTime(param.GetStartTime().printTime());
			strItemText.Format("Start Time: %s", strTemp);
			HTREEITEM hRepStartTime = m_propTree.InsertItem(strItemText, cni, FALSE, FALSE, hItem2, hRepName);

			strTemp = GetRegularDateTime(param.GetEndTime().printTime());
			strItemText.Format("End Time: %s", strTemp);
			HTREEITEM hRepEndTime = m_propTree.InsertItem(strItemText, cni, FALSE, FALSE, hItem2, hRepStartTime);

			if(report.GetCategory() == ENUM_DISTANCE_REP)
			{
				LONG lInterval;
				param.GetInterval(lInterval);
				strItemText.Format("Interval: %d", lInterval);
			}
			else
			{
				strTemp = param.GetInterval().printTime();
				strItemText.Format("Interval: %s", strTemp);
			}
			HTREEITEM hInterval = m_propTree.InsertItem(strItemText, cni, FALSE, FALSE, hItem2, hRepEndTime);

			//write Model Parameter
			std::vector<CModelParameter> vModelParam;
			int nModelParamCount = param.GetModelParameter(vModelParam);

			CModelsManager* pModelManager = m_pCmpReport->GetComparativeProject()->GetInputParam()->GetModelsManagerPtr();

			for (int nModelParam = 0; nModelParam< nModelParamCount; ++nModelParam)
			{
				CModelParameter& modelParam = vModelParam[nModelParam];
				CString strModelName = pModelManager->GetModelsList().at(nModelParam)->GetModelName();
				HTREEITEM hModelItem = m_propTree.InsertItem(strModelName, cni, FALSE, FALSE, hItem2, hInterval);

				if(iIndex == 3)
				{
					CString strTemp = modelParam.GetArea();
					if(strTemp.IsEmpty())
						strItemText = "Areas";
					else
						strItemText.Format("Areas: %s", strTemp);
					HTREEITEM hAreas = m_propTree.InsertItem(strItemText, cni, FALSE, FALSE, hModelItem);
				}

				if(report.GetCategory() == ENUM_QUEUETIME_REP ||
					report.GetCategory() == ENUM_DURATION_REP ||
					report.GetCategory() == ENUM_DISTANCE_REP ||
					report.GetCategory() == ENUM_ACOPERATION_REP||
					report.GetCategory() == ENUM_PAXDENS_REP )
				{
					std::vector<CMobileElemConstraint> vPaxType;
					if (modelParam.GetPaxType(vPaxType))
					{
						strTemp = _T("Passanger Type");
						HTREEITEM hPaxItem = m_propTree.InsertItem("Passenger Type", cni, FALSE, FALSE, hModelItem);
						CString strPax;
						for (int i = 0; i < static_cast<int>(vPaxType.size()); i++)
						{
							vPaxType[i].screenPrint(strPax);
							m_propTree.InsertItem(strPax, cni, FALSE, FALSE, hPaxItem);
						}
						m_propTree.Expand(hPaxItem, TVE_EXPAND);
					}	
				}
				if (report.GetCategory() == ENUM_DURATION_REP ||
					report.GetCategory() == ENUM_DISTANCE_REP)
				{
					// from or to processor
					CReportParameter::FROM_TO_PROCS _fromtoProcs;
					modelParam.GetFromToProcs(_fromtoProcs);

					HTREEITEM hFromToItem = m_propTree.InsertItem("From To Processors", cni, FALSE, FALSE, hModelItem);
					HTREEITEM hFromItem = m_propTree.InsertItem("From", cni, FALSE, FALSE, hFromToItem);
					HTREEITEM hToItem = m_propTree.InsertItem("To", cni, FALSE, FALSE, hFromToItem);

					for (int nFrom = 0; nFrom < (int)_fromtoProcs.m_vFromProcs.size(); ++ nFrom)
					{
						CString strProc = _fromtoProcs.m_vFromProcs.at(nFrom).GetIDString();
						m_propTree.InsertItem(strProc, cni, FALSE, FALSE, hFromItem);
					}

					for (int nTo = 0; nTo < (int)_fromtoProcs.m_vToProcs.size(); ++ nTo)
					{
						CString strProc = _fromtoProcs.m_vToProcs.at(nTo).GetIDString();
						m_propTree.InsertItem(strProc, cni, FALSE, FALSE, hToItem);
					}
				}
				else if(report.GetCategory() != ENUM_ACOPERATION_REP)
				{
					std::vector<ProcessorID> vProcGroup;
					if (modelParam.GetProcessorID(vProcGroup))
					{
						HTREEITEM hProcTypeItem = m_propTree.InsertItem("Processor Type", cni, FALSE, FALSE, hModelItem);
						char szProc[128];
						for (int i = 0; i < static_cast<int>(vProcGroup.size()); i++)
						{
							memset(szProc, 0, sizeof(szProc) / sizeof(char));
							vProcGroup[i].printID(szProc);
							m_propTree.InsertItem(szProc, cni, FALSE, FALSE, hProcTypeItem);
						}
						m_propTree.Expand(hProcTypeItem, TVE_EXPAND);
					}
				}
				m_propTree.Expand(hModelItem, TVE_EXPAND);
			}
		}
		m_propTree.Expand(m_hReportRoot, TVE_EXPAND);
	}
//	m_pCmpReport->SetModifyFlag(TRUE);
//	m_pCmpReport->SaveProject();
}

void CCmpReportTreeView::UpdateParaWnd()
{
	UpdateParaItem(m_hProjName);
	UpdateParaItem(m_hProjDesc);
	UpdateParaItem(m_hModelRoot);
	UpdateParaItem(m_hReportRoot);
}

CString CCmpReportTreeView::GetRegularDateTime(LPCTSTR elaptimestr, bool needsec)
{
	CModelsManager* pModelManager = m_pCmpReport->GetComparativeProject()->GetInputParam()->GetModelsManagerPtr();

	if((int)pModelManager->GetModelsList().size()  == 0)
		return "";

	Terminal *pTerminal = pModelManager->GetModelsList().at(0)->GetTerminal();

	CSimAndReportManager *ptSim = pTerminal->GetSimReportManager();
	CStartDate tstartdate = ptSim->GetStartDate();
	ElapsedTime etime;
	etime.SetTime(elaptimestr);

	CString retstr, tstr;
	bool bAbsDate;
	COleDateTime _tdate, _ttime;
	int _nDtIdx;
	tstartdate.GetDateTime(etime, bAbsDate, _tdate, _nDtIdx, _ttime);
	if(!needsec)
		tstr = _ttime.Format(" %H:%M");
	else
		tstr = _ttime.Format(" %H:%M:%S");
	if(bAbsDate)
	{
		retstr = _tdate.Format("%Y-%m-%d");
	}
	else
	{
		retstr.Format("Day%d", _nDtIdx + 1 );
	}
	retstr += tstr;

	return retstr;
}

void CCmpReportTreeView::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 100)
	{
		KillTimer(nIDEvent);
		RunCompareReport();
	}

	CWnd::OnTimer(nIDEvent);
}

void CCmpReportTreeView::AddModel()
{
	CWaitCursor wc;

	CModelsManager* pManager = m_pCmpReport->GetComparativeProject()->GetInputParam()->GetModelsManagerPtr();
	CModelSelectionDlg dlg(pManager,this);
	wc.Restore();
	if(dlg.DoModal() == IDOK)
	{
		UpdateParaItem(m_hModelRoot);
		m_pCmpReport->SetModifyFlag(TRUE);
		m_pCmpReport->SaveProject();
	}
}

void CCmpReportTreeView::DeleteModel()
{			
	HTREEITEM hSelItem = m_propTree.GetSelectedItem();
	HTREEITEM hSubItem = m_propTree.GetChildItem(hSelItem);
	CString strModelName = m_propTree.GetItemText(hSubItem);
	CString strPadding = "Name: ";
	strModelName = strModelName.Right(strModelName.GetLength() - strPadding.GetLength());
	if(m_pCmpReport->GetComparativeProject()->GetInputParam()->DeleteModel(strModelName))
	{
		UpdateParaItem(m_hModelRoot);
		UpdateParaItem(m_hReportRoot);
		m_pCmpReport->SetModifyFlag(TRUE);
		m_pCmpReport->SaveProject();
	}
}

void CCmpReportTreeView::AddReport()
{
	CWaitCursor wc;
	CReportProperty dlg(this);
	dlg.m_strProjName = m_pCmpReport->GetComparativeProject()->GetName();
	CModelsManager* pMManager = m_pCmpReport->GetComparativeProject()->GetInputParam()->GetModelsManagerPtr();
	CString strError = pMManager->InitTerminal(NULL,dlg.m_strProjName,NULL);
	if (!strError.IsEmpty())
	{
		AfxMessageBox(_T("Cann't load project:") + strError);
		return;
	}

	CReportsManager* pRManager = m_pCmpReport->GetComparativeProject()->GetInputParam()->GetReportsManagerPtr();

	dlg.SetManager(pMManager,pRManager);

	dlg.SetProjName(dlg.m_strProjName);
	wc.Restore();
	if(dlg.HasModelInLocation() == CReportProperty::MT_NOMODEL)
	{
		MessageBox("No valid model, please add one at least!");
		return ;
	}

	if(dlg.DoModal() == IDOK)
	{
		const CReportToCompare& report = dlg.GetReport();
		pRManager->AddReport(report);
		UpdateParaItem(m_hReportRoot);
		m_pCmpReport->SetModifyFlag(TRUE);
		m_pCmpReport->SaveProject();
	}
}

void CCmpReportTreeView::EditReport()
{
	CWaitCursor wc;

	BOOL bFound = FALSE;
	CString strReportName = m_propTree.GetItemText(m_propTree.GetSelectedItem());
	CModelsManager* pMManager = m_pCmpReport->GetComparativeProject()->GetInputParam()->GetModelsManagerPtr();
	CReportsManager* pRManager = m_pCmpReport->GetComparativeProject()->GetInputParam()->GetReportsManagerPtr();
	std::vector<CReportToCompare>& vReports = pRManager->GetReportsList();
	std::vector<CReportToCompare>::iterator iter;
	CReportToCompare report;
	for (iter = vReports.begin(); iter != vReports.end(); iter++)
	{
		if (strReportName.CompareNoCase(iter->GetName()) == 0)
		{
			report = *iter;
			bFound = TRUE;
			break;
		}
	}

	if (bFound)
	{
		CReportProperty dlg(this);
		dlg.m_strProjName = m_pCmpReport->GetComparativeProject()->GetName();
		dlg.SetManager(pMManager,pRManager);

		dlg.SetProjName(dlg.m_strProjName);
		dlg.GetReport() = report;
		wc.Restore();

		if(dlg.DoModal() == IDOK)
		{
			const CReportToCompare& report = dlg.GetReport();
			vReports.erase(iter);
			pRManager->AddReport(report);
			UpdateParaItem(m_hReportRoot);
			m_pCmpReport->SetModifyFlag(TRUE);
			m_pCmpReport->SaveProject();
		}
	}
}

void CCmpReportTreeView::DeleteReport()
{
	HTREEITEM hSelItem = m_propTree.GetSelectedItem();
	CString strReportName = m_propTree.GetItemText(hSelItem);
	CReportsManager* pRManager = m_pCmpReport->GetComparativeProject()->GetInputParam()->GetReportsManagerPtr();
	std::vector<CReportToCompare>& vReports = pRManager->GetReportsList();
	std::vector<CReportToCompare>::iterator iter;
	for (iter = vReports.begin(); iter != vReports.end(); iter++)
	{
		if (iter->GetName() == strReportName)
		{
			vReports.erase(iter);
			break;
		}
	}
	UpdateParaItem(m_hReportRoot);
	m_pCmpReport->SetModifyFlag(TRUE);
	m_pCmpReport->SaveProject();
}

void CCmpReportTreeView::OnContextMenu( CWnd* pWnd, CPoint point )
{
	CPoint pt = point;
	m_propTree.ScreenToClient( &pt );

	UINT iRet;
	HTREEITEM hCurItem = m_propTree.HitTest( pt, &iRet );
	if(hCurItem == NULL)
		return ;

	if(hCurItem == m_hModelRoot)
	{
		CMenu menuProj;
		menuProj.CreatePopupMenu();
		menuProj.AppendMenu(MF_STRING | MF_ENABLED , MENU_ADD_MODEL, _T("Add new model"));
		menuProj.AppendMenu(MF_STRING | MF_ENABLED , MENU_UNAVAILABLE, _T("Comments"));
		menuProj.AppendMenu(MF_STRING | MF_ENABLED , MENU_UNAVAILABLE, _T("Help"));
		menuProj.TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
	}
	if(hCurItem == m_hReportRoot)
	{
		CMenu menuReport;
		menuReport.CreatePopupMenu();
		menuReport.AppendMenu(MF_STRING | MF_ENABLED , MENU_ADD_REPORT, _T("Add new report"));
		menuReport.AppendMenu(MF_STRING | MF_ENABLED , MENU_UNAVAILABLE, _T("Comments"));
		menuReport.AppendMenu(MF_STRING | MF_ENABLED , MENU_UNAVAILABLE, _T("Help"));
		menuReport.TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
	}
	HTREEITEM hPareItem = m_propTree.GetParentItem(hCurItem);
	if(hPareItem == NULL) return;
	if(hPareItem == m_hModelRoot)
	{
		CMenu menuProj;
		menuProj.CreatePopupMenu();
		menuProj.AppendMenu(MF_STRING | MF_ENABLED , MENU_DELETE_MODEL, _T("Delete"));
		menuProj.AppendMenu(MF_STRING | MF_ENABLED , MENU_UNAVAILABLE, _T("Comments"));
		menuProj.AppendMenu(MF_STRING | MF_ENABLED , MENU_UNAVAILABLE, _T("Help"));
		menuProj.TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);		
	}
	if(hPareItem == m_hReportRoot)
	{
		CMenu menuProj;
		menuProj.CreatePopupMenu();
		menuProj.AppendMenu(MF_STRING | MF_ENABLED , MENU_EDIT_REPORT, _T("Edit"));
		menuProj.AppendMenu(MF_STRING | MF_ENABLED , MENU_DELETE_REPORT, _T("Delete"));
		menuProj.AppendMenu(MF_STRING | MF_ENABLED , MENU_UNAVAILABLE, _T("Comments"));
		menuProj.AppendMenu(MF_STRING | MF_ENABLED , MENU_UNAVAILABLE, _T("Help"));
		menuProj.TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);		
	}
}

LRESULT CCmpReportTreeView::DefWindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
	CString strItemText;
	if(message == UM_CEW_EDIT_BEGIN)
	{
		HTREEITEM hCurItem=(HTREEITEM)wParam;
		CComparativeProject* pComProj = m_pCmpReport->GetComparativeProject();
		if(m_propTree.GetSelectedItem() == m_hProjName)
		{
			CString strValue = pComProj->GetName();
			m_propTree.GetEditWnd(hCurItem)->SetWindowText(strValue);
			((CEdit*)m_propTree.GetEditWnd(hCurItem))->SetSel(0, -1, true); 
		}
		else if(m_propTree.GetSelectedItem() == m_hProjDesc)
		{
			CString strValue = pComProj->GetDescription();
			m_propTree.GetEditWnd(hCurItem)->SetWindowText(strValue);
			((CEdit*)m_propTree.GetEditWnd(hCurItem))->SetSel(0, -1, true);
		}
	}
	if(message == UM_CEW_EDITSPIN_END)
	{
		HTREEITEM hCurItem = (HTREEITEM)wParam;
		CString strValue=*((CString*)lParam);
		CComparativeProject* pComProj = m_pCmpReport->GetComparativeProject();
		if(hCurItem == m_hProjName)
		{
			CString strOriName = m_pCmpReport->GetOriginProjName();
			if(strValue.IsEmpty())
			{
				ReleaseCapture();
				MessageBox("The name is empty, please set the project name!");
				if(strOriName.IsEmpty())
					strItemText = "Name";
				else
					strItemText.Format("Name: %s", strOriName);
				m_propTree.SetItemText(hCurItem, strItemText);
				return 0;
			}
			if (strValue.CompareNoCase(strOriName) == 0)
			{
				ReleaseCapture();
				strItemText.Format("Name: %s", strOriName);
				m_propTree.SetItemText(hCurItem, strItemText);
				return 0;
			}
			if(m_pCmpReport->ProjExists(strValue))
			{
				ReleaseCapture();
				MessageBox("The name is already exists, please rename!");
				if(strOriName.IsEmpty())
					strItemText = "Name";
				else
					strItemText.Format("Name: %s", strOriName);
				m_propTree.SetItemText(hCurItem, strItemText);
				return 0;
			}
			strItemText.Format("Name: %s", strValue.MakeUpper());
			m_propTree.SetItemText(hCurItem, strItemText);
			pComProj->SetName(strValue);
		}
		else if(hCurItem == m_hProjDesc)
		{
			if(m_pCmpReport->GetComparativeProject()->GetName().IsEmpty())
			{
				MessageBox("The name is empty, please set the project name!");
				m_propTree.SetItemText(hCurItem, "Description");
				return 0;
			}
			if(strValue.IsEmpty())
				strItemText = "Description";
			else
				strItemText.Format("Description: %s", strValue);
			m_propTree.SetItemText(hCurItem, strItemText);
			pComProj->SetDescription(strValue);
		}
		m_pCmpReport->SetModifyFlag(TRUE);
		m_pCmpReport->SaveProject();
	}
	return CFormView::DefWindowProc(message, wParam, lParam);
}

void CCmpReportTreeView::InitCooltreeNodeInfo( CWnd* pParent,COOLTREE_NODE_INFO& CNI,BOOL bVerify/*=TRUE*/ )
{
	CNI.bEnable=TRUE;
	CNI.dwItemData=NULL;
	CNI.fMaxValue=100;
	CNI.fMinValue=0;
	CNI.net=NET_NORMAL;
	CNI.nImage=TIIT_NORMAL;
	CNI.nImageDisable=TIIT_NORMAL_DISABLE;
	CNI.nImageSeled=CNI.nImage;
	CNI.nMaxCharNum=256;
	CNI.nt=NT_NORMAL;
	CNI.pEditWnd=NULL;
	CNI.bVerify=FALSE;
	CNI.clrItem=RGB(0,0,0);
	CNI.bAutoSetItemText=TRUE;
	CFont* pFont = pParent->GetFont();
	if(pFont)
		pFont->GetLogFont(&(CNI.lfontItem));
	else
		memset(&CNI.lfontItem, 0, sizeof(LOGFONT));
	CNI.unMenuID=0;
	CNI.bInvalidData = FALSE;
}

void CCmpReportTreeView::OnChooseMenu( UINT nID )
{
	if(nID == MENU_UNAVAILABLE)
		return;
	int x = 0;
	HTREEITEM hCurItem = m_propTree.GetSelectedItem();
	if(hCurItem == NULL) return;
	if(hCurItem == m_hModelRoot)
	{
		switch(nID)
		{
		case MENU_ADD_MODEL:
			AddModel();
			break;
		default:
			break;
		}
	}
	if(hCurItem == m_hReportRoot)
	{
		switch(nID)
		{
		case MENU_ADD_REPORT:
			AddReport();
			break;
		default:
			break;
		}
	}
	HTREEITEM hPareItem = m_propTree.GetParentItem(hCurItem);
	if(hPareItem == m_hModelRoot)
	{
		switch(nID)
		{
		case MENU_DELETE_MODEL:
			DeleteModel();
			break;
		default:
			break;
		}
	}
	if(hPareItem == m_hReportRoot)
	{
		switch(nID)
		{
		case MENU_EDIT_REPORT:
			EditReport();
			break;
		case MENU_DELETE_REPORT:
			DeleteReport();
			break;
		default:
			break;
		}
	}
	return;
}
