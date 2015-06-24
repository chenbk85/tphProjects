#pragma once
#include "AirsideObjectBaseDlg.h"
#include "../InputAirside/Roundabout.h"

class CUnitPiece;
class CAirsideRoundaboutDlg : public CAirsideObjectBaseDlg , public CUnitPiece
{
	DECLARE_DYNAMIC(CAirsideRoundaboutDlg)

public:
	CAirsideRoundaboutDlg(int nRoundaboutID ,int nAirportID,int nProjID,CWnd *pParent = NULL);
// 	CAirsideRoundaboutDlg(Roundabout * pRoundabout,int nProjID,CWnd * pParent = NULL);
	~CAirsideRoundaboutDlg(void);

protected:
	HTREEITEM m_hLaneNumber;
	HTREEITEM m_hLaneWidth;	
	HTREEITEM m_hLinkStretches;
	DistanceUnit m_dLaneWidth;
	int m_iLaneNum;
	InsecObjectPartVector m_vrLinkStretches;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void LoadTree(void);
	virtual void OnDoubleClickPropTree(HTREEITEM hTreeItem);
	virtual void OnContextMenu(CMenu& menuPopup, CMenu *& pMenu);

	virtual bool ConvertUnitFromDBtoGUI(void);
	virtual bool RefreshGUI(void);
	virtual bool ConvertUnitFromGUItoDB(void);

	virtual FallbackReason GetFallBackReason(void);
	virtual void SetObjectPath(CPath2008& path);
	virtual bool UpdateOtherData(void);
public:
	virtual BOOL OnInitDialog(void);
	DECLARE_MESSAGE_MAP()
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnOK(void);
	afx_msg void OnDefineNumber(void);
	afx_msg void OnDefineWidth(void);
};


