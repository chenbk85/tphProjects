#if !defined(AFX_PAXDISTDLG_H__EC38000E_7200_4BB6_9855_7949DB7D48AC__INCLUDED_)
#define AFX_PAXDISTDLG_H__EC38000E_7200_4BB6_9855_7949DB7D48AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PaxDistDlg.h : header file
//

#include "PrintableTreeCtrl.h"
#include "inputs\in_term.h"
#include "MoblieElemTips.h"
#include <MFCExControl/SplitterControl.h>
/////////////////////////////////////////////////////////////////////////////
// CPaxDistDlg dialog

class CPaxDistDlg : public CDialog
{
// Construction
public:
	void ReloadFlightList( const char* p_str = NULL );
	CPaxDistDlg(CWnd* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPaxDistDlg)
	enum { IDD = IDD_DIALOG_PAXDIST };
	CButton	m_stcFrame2;
	CStatic	m_toolbarcontenter2;
	CButton	m_stcFrame;
	CButton	m_btnCancel;
	CButton	m_btnOK;
	CStatic	m_toolbarcontenter;
	CButton	m_btnSave;
	CPrintableTreeCtrl	m_treeDist;
	CListBox	m_listboxFlight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaxDistDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ShowTips( int iCurSel );
	//show tips fo the list
	CMoblieElemTips m_toolTips;
	int m_nListPreIndex;
	
	void InitToolbar();
	CToolBar m_ToolBar;
	CToolBar m_TreeToolBar;
	CString GetProjPath();
	int GetLevelIdx( HTREEITEM _hItem );
	void SetChildRed( HTREEITEM _hItem );
	PaxTypeDistribution* GetPaxDist();
	void GetIdxPath( HTREEITEM _hItem, int* _pIdxPath );
	void CheckPercentFull( HTREEITEM _hItem );
	void SetPercent(int _nPercent);
	void InsertPaxDistItem( const PaxTypeDistribution* _pPaxDist, HTREEITEM _hParent, int _nIdxLevel, int* _pIdxPath );
	void ReloadTree();
	CString GetItemLabel( const CString& _csName,int _nPercent );
	InputTerminal* GetInputTerminal();
	void DeleteAllChildren(HTREEITEM hItem);
	// start [5/6/2004]
	void EvenPercentCurrentLevel();
	void EvenRemainPercentCurrentLevel();
	void GetAllItemAtDepth(const HTREEITEM hParent,std::vector<HTREEITEM>& vItems, int nDepth);
	void ModifyPercentAllBranch(int _nNewPercent);
	// end [5/6/2004]
	void SetPercentAllBranch(int nLevel,int nPercent, int nSel);
	void SetLevelBranchPercent(int nLevel,int nCurLevel, int nPercent, int nSel,int* _pIdxPath );

	void SetEvenRemainPercent(int nLevel,int nPercent, int nSel);
	void SetEvenRemainLevelPercent(int nLevel,int nCurLevel, int nUsedPercent, int nSel,int* _pIdxPath);

	void SetEventAllBranchPercent(int nLevel,int nPercent, int nSel);
	void SetEventLevelPercent(int nLevel,int nCurLevel, int nUsedPercent, int nSel,int* _pIdxPath);
	void DoResize(int delta);//add by colin,2011/4/26
	HTREEITEM m_hRClickItem;
 	bool m_bSized;

	//-1, singly modify percent; 
	//0 , modify all branches' percent; 
	//1 , even all branches' percent; 
	//2 , even remaining all branches' percent.   [5/6/2004]
	int m_nAllBranchesFlag;

	CSplitterControl m_wndSplitter;//add by colin,2011/4/26
	PaxTypeDistribution* m_pCopyDistData;


	// Generated message map functions
	//{{AFX_MSG(CPaxDistDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListFlight();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaxdistModifypercent();
	afx_msg void OnPaxdistEvenpercent();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnButtonSave();
	afx_msg void OnPeoplemoverNew();
	afx_msg void OnPeoplemoverDelete();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCancelMode();
	afx_msg void OnPrintDist();
	afx_msg void OnPaxdistEvenRemainPercent();
	afx_msg LRESULT OnEndEditPercent( WPARAM p_wParam, LPARAM p_lParam );//  [5/6/2004]
	afx_msg void OnAllBranchesPercentEdit(UINT nID);//  [5/6/2004]
	afx_msg void OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBtnCopy();
	afx_msg void OnBtnPaste();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAXDISTDLG_H__EC38000E_7200_4BB6_9855_7949DB7D48AC__INCLUDED_)
