// ShapesBar.h: interface for the CShapesBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHAPESBAR_H__B592C191_B7B4_4123_BD3B_9E5D858B5DB9__INCLUDED_)
#define AFX_SHAPESBAR_H__B592C191_B7B4_4123_BD3B_9E5D858B5DB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ShapesListBox.h"
#include "scbarg.h"
#include "XPStyle\GfxOutBarCtrl.h"

#define WM_SHAPEBAR_NOTIFY		WM_USER + 100

class CShapesBar : public CSizingControlBarG  
{
// Construction
public:
	//CShapesBar();
	//virtual ~CShapesBar();

// Attributes
public:
protected:
	CBrush m_brush;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectSelectBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	void CreateOutlookBar(CShape::CShapeList* pSL);
	BOOL ImportShapeBarData(CString ImportFile);
	BOOL ExportShapeBarData(CString ExportFile);
	void ImportUserShapeBars();
	void ExportUserShapeBars();
	CString UserProjectPath;
	// Generated message map functions
protected:
	CFont m_font;
	CGfxOutBarCtrl m_wndOutBarCtrl;
	CImageList m_smallIL;
	CImageList m_largeIL;
	int folder_index;
	int item_index;

	//{{AFX_MSG(CShapesBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSLBSelChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPopMenu(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnAddFolder();
	afx_msg void OnFolderRename();
	afx_msg void OnChangeFolderPath();
	afx_msg void OnImport();
	afx_msg void OnExport();
	afx_msg void OnDeleteFolder();

	afx_msg void OnAddItem();
	afx_msg void OnItemRename();
	afx_msg void OnChangeItemPicture();
	afx_msg void OnChangeItemModel();
	afx_msg void OnDeleteItem();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SHAPESBAR_H__B592C191_B7B4_4123_BD3B_9E5D858B5DB9__INCLUDED_)
