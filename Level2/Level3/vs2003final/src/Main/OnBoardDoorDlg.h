#pragma once
#include "onboardobjectbasedlg.h"
#include "AirsideGUI/UnitPiece.h"

class CAircaftLayOut;
class CDoor;


class COnBoardDoorDlg :
	public COnBoardObjectBaseDlg,public CUnitPiece
{
	DECLARE_DYNAMIC(COnBoardDoorDlg)
public:
	COnBoardDoorDlg(CDoor *pDoor,CAircaftLayOut *pLayout, int nProjID, CWnd* pParent = NULL);
	~COnBoardDoorDlg(void);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void LoadTree();
	virtual void OnDoubleClickPropTree(HTREEITEM hTreeItem);
	virtual void OnContextMenu(CMenu& menuPopup, CMenu *& pMenu);


	virtual FallbackReason GetFallBackReason();
	virtual void SetObjectPath(CPath2008& path);
	virtual bool UpdateOtherData();

	DECLARE_MESSAGE_MAP()


public:
	virtual BOOL OnInitDialog();

protected:
	HTREEITEM m_hItemPosition;
	HTREEITEM m_hItemWidth;
	HTREEITEM m_hItemLength;
	HTREEITEM m_hItemHeight;
	HTREEITEM m_hItemRotation;
protected:
	int m_nProjID;
	CAircaftLayOut *m_pLayout;
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnOK();

	virtual bool ConvertUnitFromDBtoGUI(void);
	virtual bool RefreshGUI(void);
	virtual bool ConvertUnitFromGUItoDB(void);

};
