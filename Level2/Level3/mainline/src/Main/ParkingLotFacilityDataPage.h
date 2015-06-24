#pragma once
#include "..\MFCExControl\CoolTree.h"
#include "ResizePS\ResizablePage.h"
#include "MFCExControl/SplitterControl.h"


// CParkingLotFacilityDataPage dialog
class InputLandside;
class CFacilityBehaviorList;
class CFacilityParkingLotBehavior;
class InputTerminal;

class CParkingLotFacilityDataPage : public CResizablePage
{
	DECLARE_DYNAMIC(CParkingLotFacilityDataPage)

public:
	CParkingLotFacilityDataPage(InputLandside *pInputLandside,
		InputTerminal* _pInTerm,
		CFacilityBehaviorList *pBehaviorDB,
		enum ALTOBJECT_TYPE enumFacType,
		UINT nIDCaption,
		int nProjectID);

	virtual ~CParkingLotFacilityDataPage();

	// Dialog Data
	enum { IDD = IDD_PROPPAGE_FACILITYDATA };
public:

	class ParkingLotTreeNodeData
	{
	public:
		enum ParkingLotTreeNodeType
		{
			TeminalProcessor_Node,
			TeminalLinkage_Node
		};

		ParkingLotTreeNodeData()
		:m_emType(TeminalProcessor_Node)
		{

		}
		~ParkingLotTreeNodeData()
		{

		}

		DWORD m_dwData;
		ParkingLotTreeNodeType m_emType;
	};

protected:
	ALTOBJECT_TYPE	m_enumType;
	int				m_nProjectID;

protected:
	CStatic		m_staticLinkage;
	CCoolTree	m_TreeData;
	CListBox	m_listboxFacility;

	CToolBar m_ToolBar;
	CToolBar m_ToolBarLeft;
	CSplitterControl m_wndSplitter;

	InputLandside *m_pInputLandside;
	InputTerminal *m_pInTerm;
	CFacilityBehaviorList *m_pBehaviorDB;
protected:
	void InitToolbar();	
	void InitSpiltter();
	void SetSplitterRange();


	void InitFacilityList();
	int AddFacilityToList(CFacilityParkingLotBehavior *pBehavior);

	void UpdateFacilityToolbarState();

	void LoadTreeProperties();
	void UpdateTreeToolbarState();
	CFacilityParkingLotBehavior *GetCurrentBehavior();

	//check the item in the existed in the list
	bool IsItemDuplicate(HTREEITEM hParentItem,const CString& strName);

protected:
	HTREEITEM m_hCapacity;
	std::vector<ParkingLotTreeNodeData*> m_vParkingLotTreeNode;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	virtual afx_msg void OnToolbarbuttonadd();
	virtual afx_msg void OnToolbarbuttondel();
	virtual afx_msg void OnLinkageOnetoone();

	virtual afx_msg void OnSelchangeListFacility();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnApply();
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTvnSelchangedTreeData(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
