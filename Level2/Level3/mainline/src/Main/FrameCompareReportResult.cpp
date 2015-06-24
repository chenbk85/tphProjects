// FrameCompareReportResult.cpp : implementation file
//

#include "stdafx.h"
#include "termplan.h"
#include "FrameCompareReportResult.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrameCompareReportResult

IMPLEMENT_DYNCREATE(CFrameCompareReportResult, CMDIChildWnd)

CFrameCompareReportResult::CFrameCompareReportResult()
{
}

CFrameCompareReportResult::~CFrameCompareReportResult()
{
}


BEGIN_MESSAGE_MAP(CFrameCompareReportResult, CMDIChildWnd)
	//{{AFX_MSG_MAP(CFrameCompareReportResult)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrameCompareReportResult message handlers
void CFrameCompareReportResult::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// update our parent window first
	GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);
	
	if ((GetStyle() & FWS_ADDTOTITLE) == 0)
		return;     // leave child window alone!
	
	CDocument* pDocument = GetActiveDocument();
	if(bAddToTitle)
	{
		TCHAR szText[256+_MAX_PATH];
		if (pDocument == NULL)
			lstrcpy(szText, m_strTitle);
		else
			lstrcpy(szText, pDocument->GetTitle() + CString(_T("         Compare Report Result")));

		this->SetWindowText(szText);
	}		
}

int CFrameCompareReportResult::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CRect rc, rcclient;
	this->GetParentFrame()->GetClientRect(&rcclient);
	this->GetParentFrame()->GetWindowRect(&rc);
	// TRACE("\n--%d--%d--%d--%d\n--%d--%d--%d--%d", 
//		rcclient.left, rcclient.right, rcclient.top, rcclient.bottom,
//		rc.left, rc.right, rc.top, rc.bottom);

	MoveWindow(	rcclient.left+rcclient.Width()/4+1, rcclient.top, 
		rcclient.right-rcclient.Width()/4-1, rcclient.Height()-(rc.Height()-rcclient.Height())-1);

	return 0;
}

void CFrameCompareReportResult::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
}
