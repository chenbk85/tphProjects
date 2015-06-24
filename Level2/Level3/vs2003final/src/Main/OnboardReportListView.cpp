// OnboardReportListView.cpp : implementation file
//

#include "stdafx.h"
#include "TermPlan.h"
#include "OnboardReportListView.h"
#include ".\onboardreportlistview.h"
#include "TermPlanDoc.h"
#include "RepListViewSingleReportOperator.h"
#include "../OnboardReport/OnboardBaseParameter.h"
#include "../OnboardReport/OnboardReportManager.h"
#include "ARCReportManager.h"
#include "OnboardReportGraphView.h"
#include "OnboardReportControlView.h"
#include "OnboardRepListViewBaseOperator.h"
#include "ViewMsg.h"

#define  IDC_CMPREPORT_LISTCTRL 0x01
// OnboardReportListView

IMPLEMENT_DYNCREATE(OnboardReportListView, CFormView)

OnboardReportListView::OnboardReportListView()
	: CFormView(OnboardReportListView::IDD)
	, m_pRepListViewBaseOperator(NULL)
{
}

OnboardReportListView::~OnboardReportListView()
{
}

void OnboardReportListView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_ONBOARDREPORT_GRIDCTRL,m_wndListCtrl);
}

BEGIN_MESSAGE_MAP(OnboardReportListView, CFormView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	//ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_CMPREPORT_LISTCTRL, OnColumnclick)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)

	ON_COMMAND(ID_LISTVIEW_MAXIMIZE, OnListviewMaximize)
	ON_COMMAND(ID_LISTVIEW_NORMAL, OnListviewNormal)
	ON_COMMAND(ID_LISTVIEW_PRINT, OnListviewPrint)
	ON_COMMAND(ID_LISTVIEW_EXPORT, OnListviewExport)

END_MESSAGE_MAP()


// OnboardReportListView diagnostics

#ifdef _DEBUG
void OnboardReportListView::AssertValid() const
{
	CFormView::AssertValid();
}

void OnboardReportListView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// OnboardReportListView message handlers

void OnboardReportListView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
}

void OnboardReportListView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if (m_wndListCtrl.GetSafeHwnd())
	{
		CRect rc;
		GetClientRect(&rc);
		m_wndListCtrl.MoveWindow(&rc);
	}

	SetScaleToFitSize(CSize(cx,cy));
	// TODO: Add your message handler code here
}

void OnboardReportListView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
{
	// TODO: Add your specialized code here and/or call the base class

	CTermPlanDoc* pTermPlanDoc = (CTermPlanDoc* )GetDocument();

	if (lHint == AIRSIDEREPORT_SHOWREPORT)
	{
		pTermPlanDoc->GetARCReportManager().GetOnboardReportManager()->InitReportList(m_wndListCtrl, &m_ctlHeaderCtrl);
		pTermPlanDoc->GetARCReportManager().GetOnboardReportManager()->SetReportListContent(m_wndListCtrl);
	}
	
}

int OnboardReportListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CRect rtEmpty;
	rtEmpty.SetRectEmpty();
	if(m_wndListCtrl.Create(LBS_NOTIFY|LBS_HASSTRINGS|WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL,
		rtEmpty, this,IDC_CMPREPORT_LISTCTRL))
	{
		DWORD dwStyle =dwStyle = m_wndListCtrl.GetExtendedStyle();
		dwStyle |= LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
		m_wndListCtrl.SetExtendedStyle( dwStyle );

		m_ctlHeaderCtrl.SubclassWindow(m_wndListCtrl.GetHeaderCtrl()->m_hWnd );
	}

	return 0;
}

void OnboardReportListView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: Add your message handler code here
	CMenu menuPopup;
	menuPopup.LoadMenu( IDR_MENU_POPUP );
	CMenu* pMenu = menuPopup.GetSubMenu(7);

	pMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

void OnboardReportListView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add your specialized code here and/or call the base class

	//CFormView::OnPrint(pDC, pInfo);
	CString line; //This is the print line
	TEXTMETRIC metrics; //Font measurements
	int y = 0; //Current y position on report
	CFont TitleFont; //Font for Title
	CFont HeadingFont; //Font for headings
	CFont DetailFont; //Font for detail lines
	CFont FooterFont; //Font for footer
	//Tab stops at 1 inch, 2.5 inches, and 6.5 inches
	//	int TabStops[] = {100, 200,300,400,500};
	//Tab stops at 3.5 inches and 6.5 inches
	int FooterTabStops[] = {350, 650};
	if (!pInfo || pInfo->m_nCurPage == 1) {
		//Set the recordset at the beginning
	}
	short cxInch=pDC->GetDeviceCaps(LOGPIXELSX); 
	short cyInch=pDC->GetDeviceCaps(LOGPIXELSY); 

#define MYFONTSIZE 14 
#define HFONTNAME "MS Sans Serif"
	//
	int nFontHeight=MulDiv(MYFONTSIZE, -cyInch, 72); 
	if(nFontHeight % 2) nFontHeight++; 

	if(!TitleFont.CreateFont(static_cast<int>(nFontHeight*0.8), 0, 0, 0, FW_NORMAL, 0, 0,0, \
		GB2312_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, \
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, HFONTNAME)) 
		return ; 

	if(!HeadingFont.CreateFont(static_cast<int>(nFontHeight*0.7), 0, 0, 0, FW_NORMAL, 0, 0,0, \
		GB2312_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, \
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, HFONTNAME)) 
		return ; 

	if(!DetailFont.CreateFont(static_cast<int>(nFontHeight*0.7), 0, 0, 0, FW_NORMAL, 0, 0,0, \
		GB2312_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, \
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, HFONTNAME)) 
		return ; 

	if(!FooterFont.CreateFont(nFontHeight/2, 0, 0, 0, FW_NORMAL, 0, 0,0, \
		GB2312_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, \
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, HFONTNAME)) 
		return ;

	// Get the system's default printer's setting
	int	nPageVMargin,nPageHMargin,nPageHeight,nPageWidth;
	nPageVMargin = pDC->GetDeviceCaps(LOGPIXELSY) / 2;
	nPageHMargin = pDC->GetDeviceCaps(LOGPIXELSX) / 2;
	nPageHeight  = pDC->GetDeviceCaps(VERTRES);
	nPageWidth   = pDC->GetDeviceCaps(HORZRES);

	//Capture default settings when setting the footer font	
	CFont* OldFont = (CFont*)pDC->SelectObject(&FooterFont);	
	pDC->GetTextMetrics(&metrics);
	int nFooterHeight = metrics.tmHeight + metrics.tmExternalLeading;
	pDC->SelectObject(&TitleFont);
	//Retrieve the heading font measurements
	pDC->GetTextMetrics(&metrics);
	//Compute the heading line height
	int nLineHeight = metrics.tmHeight + metrics.tmExternalLeading;
	//Set Y to the line height.
	y += static_cast<int>(nLineHeight*1.5+nPageVMargin);
	pDC->TextOut(nPageHMargin, nPageVMargin, m_sHeader);
	//Draw a line under the heading
	pDC->MoveTo(nPageHMargin,y-nLineHeight/2);
	pDC->LineTo(nPageWidth-nPageHMargin,y-nLineHeight/2);
	//Set the Heading font
	pDC->SelectObject(&HeadingFont);
	y += nLineHeight;

	//Format the heading
	// get listctrl's header info
	int nCol = m_wndListCtrl.GetHeaderCtrl()->GetItemCount();
	HDITEM hi;
	hi.mask = HDI_TEXT ;
	hi.cchTextMax = 19;
	char chBuffer[20];
	hi.pszText = chBuffer;
	int nSpace=(nPageWidth-2*nPageHMargin) / nCol;
	for(int i = 0; i < nCol; i++ )
	{
		::ZeroMemory( chBuffer, 20);
		m_wndListCtrl.GetHeaderCtrl()->GetItem( i,&hi);
		pDC->TextOut(nPageHMargin+i*nSpace, y,hi.pszText);
	}

	//Compute the detail line height
	nLineHeight = metrics.tmHeight + metrics.tmExternalLeading;
	y += 2*nLineHeight; //Adjust y position
	//Set the detail font
	pDC->SelectObject(&DetailFont);
	//Retrieve detail font measurements
	pDC->GetTextMetrics(&metrics);
	//Compute the detail line height
	nLineHeight = metrics.tmHeight + metrics.tmExternalLeading;

	//Scroll through the list
	int nIndex=1;
	int nRowCount = m_wndListCtrl.GetItemCount();
	while ( nIndex <= nRowCount ) 
	{
		if (pInfo && y > nPageHeight-nPageVMargin-nFooterHeight-nLineHeight-6) 
		{   // start a new page
			pInfo->SetMaxPage(pInfo->m_nCurPage + 1);
			break;
		}
		//Format the detail line
		for( int nSubitem = 0; nSubitem < nCol; nSubitem++ )
		{

			char chLine[20];
			m_wndListCtrl.GetItemText(nIndex-1,nSubitem,chLine,19);
			pDC->TextOut(nPageHMargin+nSubitem*nSpace, y,chLine);
		}

		y += nLineHeight; //Adjust y position
		nIndex++; 
	}
	if (pInfo) {
		//Draw a line above the footer
		pDC->MoveTo(nPageHMargin,static_cast<int>(nPageHeight-nPageVMargin-nFooterHeight*1.5));
		pDC->LineTo(nPageWidth-nPageHMargin,static_cast<int>(nPageHeight-nPageVMargin-nFooterHeight*1.5));
		//Set the footer font
		pDC->SelectObject(&FooterFont);
		//Format the footer
		line.Format(" \tPage %d",pInfo->m_nCurPage);
		line = m_sFoot + line;
		//Output the footer at the bottom using tabs
		pDC->TabbedTextOut(nPageHMargin, nPageHeight-nPageVMargin-nFooterHeight, line, 2, FooterTabStops, 0);
	}
	//Restore default settings
	pDC->SelectObject(OldFont);
}
void OnboardReportListView::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int nTestIndex = pNMListView->iSubItem;
	if( nTestIndex >= 0 )
	{
		CWaitCursor	wCursor;
		if(::GetKeyState( VK_CONTROL ) < 0 ) // Is <CTRL> Key Down
			m_ctlHeaderCtrl.SortColumn( nTestIndex, MULTI_COLUMN_SORT );
		else
			m_ctlHeaderCtrl.SortColumn( nTestIndex, SINGLE_COLUMN_SORT );
		m_ctlHeaderCtrl.SaveSortList();
	}		
	*pResult = 0;
}
void OnboardReportListView::ResetAllContent()
{
	if(m_pRepListViewBaseOperator)
	{
		m_pRepListViewBaseOperator->ResetAllContent();
	}
}
void OnboardReportListView::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_pRepListViewBaseOperator)
		m_pRepListViewBaseOperator->OnCustomDraw(pNMHDR,pResult);

}
void OnboardReportListView::OnListviewMaximize() 
{
	// TODO: Add your command handler code here
	CRect rectMax;
	GetParentFrame()->GetClientRect(&rectMax);
	rectMax.top+=2;
	
	CTermPlanDoc* pDoc = (CTermPlanDoc*)(GetDocument());
	CView * pTempView;
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		pTempView = pDoc->GetNextView(pos);
		if(pTempView->IsKindOf(RUNTIME_CLASS(OnboardReportGraphView)))
		{
			pTempView->ShowWindow(SW_HIDE);
		}
		if(pTempView->IsKindOf(RUNTIME_CLASS(OnboardReportControlView)))
		{
			pTempView->ShowWindow(SW_HIDE);
		}
	}
	GetParent()->MoveWindow(&rectMax);
	MoveWindow(&rectMax);
	GetParentFrame()->Invalidate();
}

void OnboardReportListView::OnListviewNormal() 
{
	// TODO: Add your command handler code here

	CTermPlanDoc* pDoc = (CTermPlanDoc*)(GetDocument());
	CView * pTempView;
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		pTempView = pDoc->GetNextView(pos);
		if(pTempView->IsKindOf(RUNTIME_CLASS(OnboardReportGraphView)))
		{
			pTempView->ShowWindow(SW_SHOW);
		}
		if(pTempView->IsKindOf(RUNTIME_CLASS(OnboardReportControlView)))
		{
			pTempView->ShowWindow(SW_SHOW);
		}

	}
	GetParentFrame()->ShowWindow(SW_NORMAL);
	GetParentFrame()->ShowWindow(SW_MAXIMIZE);
}
void OnboardReportListView::OnListviewPrint() 
{
	// TODO: Add your command handler code here

	m_sHeader="Onboard Report  ";
	CTermPlanDoc* pTermPlanDoc = (CTermPlanDoc* )GetDocument();
	if(pTermPlanDoc)
	{
		m_sHeader = pTermPlanDoc->GetARCReportManager().GetOnboardReportManager()->GetCurrentReportName();
	}



	m_sFoot="Aviation Research Corporation";
	CDC dc;
	CPrintDialog printDlg(FALSE, PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE);
	if (m_wndListCtrl.GetSelectedCount() == 0)						// if there are no selected items in the list control
		printDlg.m_pd.Flags = printDlg.m_pd.Flags | PD_NOSELECTION; //then disable the selection radio button
	else															//else ...
		printDlg.m_pd.Flags = printDlg.m_pd.Flags | PD_SELECTION;

	if(printDlg.DoModal() == IDCANCEL )
		return;
	dc.Attach( printDlg.GetPrinterDC() );
	dc.m_bPrinting = TRUE;
	CString sTitle;
	sTitle = "Report List Data";
	DOCINFO di;
	::ZeroMemory( &di, sizeof(DOCINFO) );
	di.cbSize = sizeof( DOCINFO );
	di.lpszDocName = sTitle;

	BOOL bPrintingOK = dc.StartDoc( &di );

	CPrintInfo info;
	info.m_rectDraw.SetRect( 0,0,dc.GetDeviceCaps( HORZRES ),dc.GetDeviceCaps( VERTRES ));

	OnBeginPrinting( &dc, &info );
	//	for( UINT page = info.GetMinPage(); page < info.GetMaxPage() && bPrintingOK; page++ )
	//	{
	//		info.m_nCurPage = page;
	if(printDlg.PrintSelection() == FALSE)	//print all.
		OnPrint( &dc, &info );
	else 
		PrintSelectedItems(&dc, &info);				//print only the selected items.
	bPrintingOK = TRUE;
	//	}
	OnEndPrinting( &dc, &info );

	if( bPrintingOK )
		dc.EndDoc();
	else
		dc.AbortDoc();
	dc.Detach();


}
void OnboardReportListView::PrintSelectedItems(CDC *pDC, CPrintInfo *pInfo)
{
	CString line; //This is the print line
	TEXTMETRIC metrics; //Font measurements
	int y = 0; //Current y position on report
	CFont TitleFont; //Font for Title
	CFont HeadingFont; //Font for headings
	CFont DetailFont; //Font for detail lines
	CFont FooterFont; //Font for footer
	//Tab stops at 1 inch, 2.5 inches, and 6.5 inches
    //	int TabStops[] = {100, 200,300,400,500};
	//Tab stops at 3.5 inches and 6.5 inches
	int FooterTabStops[] = {350, 650};
	if (!pInfo || pInfo->m_nCurPage == 1) {
		//Set the recordset at the beginning
	}
	short cxInch=pDC->GetDeviceCaps(LOGPIXELSX); 
	short cyInch=pDC->GetDeviceCaps(LOGPIXELSY); 
	
#define MYFONTSIZE 14 
#define HFONTNAME "MS Sans Serif"
	//
	int nFontHeight=MulDiv(MYFONTSIZE, -cyInch, 72); 
	if(nFontHeight % 2) nFontHeight++; 
	
	if(!TitleFont.CreateFont(static_cast<int>(nFontHeight*0.8), 0, 0, 0, FW_NORMAL, 0, 0,0, \
		GB2312_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, \
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, HFONTNAME)) 
		return ; 
	
	if(!HeadingFont.CreateFont(static_cast<int>(nFontHeight*0.7), 0, 0, 0, FW_NORMAL, 0, 0,0, \
		GB2312_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, \
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, HFONTNAME)) 
		return ; 
	
	if(!DetailFont.CreateFont(static_cast<int>(nFontHeight*0.7), 0, 0, 0, FW_NORMAL, 0, 0,0, \
		GB2312_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, \
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, HFONTNAME)) 
		return ; 
	
	if(!FooterFont.CreateFont(nFontHeight/2, 0, 0, 0, FW_NORMAL, 0, 0,0, \
		GB2312_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, \
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, HFONTNAME)) 
		return ;
	
	// Get the system's default printer's setting
	int	nPageVMargin,nPageHMargin,nPageHeight,nPageWidth;
	nPageVMargin = pDC->GetDeviceCaps(LOGPIXELSY) / 2;
	nPageHMargin = pDC->GetDeviceCaps(LOGPIXELSX) / 2;
	nPageHeight  = pDC->GetDeviceCaps(VERTRES);
	nPageWidth   = pDC->GetDeviceCaps(HORZRES);
	
	//Capture default settings when setting the footer font	
	CFont* OldFont = (CFont*)pDC->SelectObject(&FooterFont);	
	pDC->GetTextMetrics(&metrics);
	int nFooterHeight = metrics.tmHeight + metrics.tmExternalLeading;
	pDC->SelectObject(&TitleFont);
	//Retrieve the heading font measurements
	pDC->GetTextMetrics(&metrics);
	//Compute the heading line height
	int nLineHeight = metrics.tmHeight + metrics.tmExternalLeading;
	//Set Y to the line height.
	y += static_cast<int>(nLineHeight*1.5+nPageVMargin);
	pDC->TextOut(nPageHMargin, nPageVMargin, m_sHeader);
	//Draw a line under the heading
	pDC->MoveTo(nPageHMargin,y-nLineHeight/2);
	pDC->LineTo(nPageWidth-nPageHMargin,y-nLineHeight/2);
	//Set the Heading font
	pDC->SelectObject(&HeadingFont);
    y += nLineHeight;

	//Format the heading
	// get listctrl's header info
    int nCol = m_wndListCtrl.GetHeaderCtrl()->GetItemCount();
	HDITEM hi;
	hi.mask = HDI_TEXT ;
	hi.cchTextMax = 19;
	char chBuffer[20];
	hi.pszText = chBuffer;
	int nSpace=(nPageWidth-2*nPageHMargin) / nCol;
	for(int i = 0; i < nCol; i++ )
	{
		::ZeroMemory( chBuffer, 20);
		m_wndListCtrl.GetHeaderCtrl()->GetItem( i,&hi);
		pDC->TextOut(nPageHMargin+i*nSpace, y,hi.pszText);
	}
	
	//Compute the detail line height
	nLineHeight = metrics.tmHeight + metrics.tmExternalLeading;
	y += 2*nLineHeight; //Adjust y position
	//Set the detail font
	pDC->SelectObject(&DetailFont);
	//Retrieve detail font measurements
	pDC->GetTextMetrics(&metrics);
	//Compute the detail line height
	nLineHeight = metrics.tmHeight + metrics.tmExternalLeading;
	
	//Scroll through the list
	int nIndex=1;
	POSITION ps  = m_wndListCtrl.GetFirstSelectedItemPosition();
	if(ps == NULL) 
		return;
	int curSelectedItem = m_wndListCtrl.GetNextSelectedItem(ps);
	int nRowCount = m_wndListCtrl.GetSelectedCount();
	while ( nIndex <= nRowCount ) 
	{
		if (pInfo && y > nPageHeight-nPageVMargin-nFooterHeight-nLineHeight-6) 
		{   // start a new page
			pInfo->SetMaxPage(pInfo->m_nCurPage + 1);
			break;
		}
		//Format the detail line
		for( int nSubitem = 0; nSubitem < nCol; nSubitem++ )
		{
			
			char chLine[20];
			m_wndListCtrl.GetItemText(curSelectedItem, nSubitem, chLine, 19);
			pDC->TextOut(nPageHMargin+nSubitem*nSpace, y,chLine);
		}
		curSelectedItem = m_wndListCtrl.GetNextSelectedItem(ps);
		y += nLineHeight; //Adjust y position
		nIndex++; 
	}
	if (pInfo) {
		//Draw a line above the footer
		pDC->MoveTo(nPageHMargin,static_cast<int>(nPageHeight-nPageVMargin-nFooterHeight*1.5));
		pDC->LineTo(nPageWidth-nPageHMargin,static_cast<int>(nPageHeight-nPageVMargin-nFooterHeight*1.5));
		//Set the footer font
		pDC->SelectObject(&FooterFont);
		//Format the footer
		line.Format(" \tPage %d",pInfo->m_nCurPage);
		line = m_sFoot + line;
		//Output the footer at the bottom using tabs
		pDC->TabbedTextOut(nPageHMargin, nPageHeight-nPageVMargin-nFooterHeight, line, 2, FooterTabStops, 0);
	}
	//Restore default settings
	pDC->SelectObject(OldFont);
}


void OnboardReportListView::OnListviewExport() 
{
// 	if(m_pRepListViewBaseOperator)
// 		m_pRepListViewBaseOperator->OnListviewExport();

	CFileDialog filedlg( FALSE, "csv", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"Report files (*." + CString(_T("csv")) + ") | *." +  CString(_T("csv")) + ";*." +  CString(_T("CSV")) + "|All type (*.*)|*.*|", NULL );
	if(filedlg.DoModal()!=IDOK)
		return;
	CString csFileName=filedlg.GetPathName();

	ArctermFile  _file ;
	if(!_file.openFile(csFileName,WRITE) )
	{
		MessageBox(_T("Create File error"),_T("Error"),MB_OK) ;
		return  ;
	}
	CString error ;
	CTermPlanDoc* pTermPlanDoc = (CTermPlanDoc* )GetDocument();
	if(pTermPlanDoc == NULL)
		return;

	if(!pTermPlanDoc->GetARCReportManager().GetOnboardReportManager()->ExportListCtrlToCvsFile(_file,m_wndListCtrl))
	{
		MessageBox(_T("Export Report Failed"),"Error",MB_OK) ;
	}
	else
	{
		MessageBox(_T("Export Report Successful"),"Success",MB_OK) ;
	}
}

OnboardBaseParameter* OnboardReportListView::GetReportParameter()
{
	CTermPlanDoc * pDoc=(CTermPlanDoc *)GetDocument();
	return pDoc->GetARCReportManager().GetOnboardReportManager()->GetParameters();
}


