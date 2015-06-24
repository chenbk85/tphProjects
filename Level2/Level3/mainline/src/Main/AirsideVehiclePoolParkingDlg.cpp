#include "StdAfx.h"
#include "resource.h"

#include "../AirsideGUI/UnitPiece.h"
#include "../InputAirside/ARCUnitManagerOwnerEnum.h"
#include ".\AirsideVehiclePoolParkingDlg.h"
#include "../common\WinMsg.h"

IMPLEMENT_DYNAMIC(CAirsideVehiclePoolParkingDlg, CAirsideObjectBaseDlg)
BEGIN_MESSAGE_MAP(CAirsideVehiclePoolParkingDlg,CAirsideObjectBaseDlg)
	ON_COMMAND(ID_ALTOBJECTVERTICAL_EDITVERTICALPROFILE,OnEditVerticalProfile)
END_MESSAGE_MAP()
CAirsideVehiclePoolParkingDlg::CAirsideVehiclePoolParkingDlg(int nVehiclePoolParkingID ,int nAirportID,int nProjID,CWnd *pParent)
:CAirsideObjectBaseDlg(nVehiclePoolParkingID,nAirportID, nProjID,pParent),m_vLevelList(nAirportID)
{
	m_pObject = new VehiclePoolParking();
	m_pObject->setID(nVehiclePoolParkingID);
	m_pObject->setAptID(nAirportID);
	m_bPathModified = false;
}

//CAirsideVehiclePoolParkingDlg::CAirsideVehiclePoolParkingDlg(VehiclePoolParking* pVehiclePoolParking,int nProjID,CWnd * pParent /*= NULL*/ )
//: CAirsideObjectBaseDlg(pVehiclePoolParking, nProjID,pParent),m_vLevelList(pVehiclePoolParking?pVehiclePoolParking->getAptID():-1)
//{
//	if(pVehiclePoolParking)
//		m_path = pVehiclePoolParking->GetPath();
//	m_bPathModified = false;
//}

CAirsideVehiclePoolParkingDlg::~CAirsideVehiclePoolParkingDlg(void)
{
	m_path.clear();
}
void CAirsideVehiclePoolParkingDlg::DoDataExchange(CDataExchange* pDX)
{
	CAirsideObjectBaseDlg::DoDataExchange(pDX);
}
BOOL CAirsideVehiclePoolParkingDlg::OnInitDialog(void)
{
	CAirsideObjectBaseDlg::OnInitDialog();
	// If no bitmap is defined for the static control, define the bitmap 
	// to the system close bitmap.
	HBITMAP bitmap = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_RUNWAY));

	ASSERT(bitmap);
	m_ImageStatic.SetBitmap( /*bitmap*/ 0);

	if (m_nObjID != -1)
	{
		SetWindowText(_T("Modify VehiclePoolParking"));
	}
	else
	{
		SetWindowText(_T("Define VehiclePoolParking"));
	}

	if(GetObject()->getID() != -1)
	{
		((VehiclePoolParking *)GetObject())->ReadObject(GetObject()->getID());
		m_path = ((VehiclePoolParking *)GetObject())->GetPath();
	}

	/////
	//initialize unit manager
	CUnitPiece::InitializeUnitPiece(m_nProjID,UM_ID_7,this);
	CRect rectItem;
	GetDlgItem(IDOK)->GetWindowRect(rectItem);
	ScreenToClient(rectItem);
	int yPos = rectItem.top;
	m_treeProp.GetWindowRect(rectItem);
	ScreenToClient(rectItem);
	int xPos = rectItem.left;
	CUnitPiece::MoveUnitButton(xPos,yPos);
	SetResize(XIAOHUABUTTON_ID, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	CUnitPiece::SetUnitInUse(ARCUnit_Length_InUse);
	/////

	LoadTree();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CAirsideVehiclePoolParkingDlg::OnContextMenu(CMenu& menuPopup, CMenu *& pMenu)
{
	if(m_hRClickItem == m_hCtrlPoints)
	{
		pMenu=menuPopup.GetSubMenu(73);
	}
	else if (m_hRClickItem == m_hVerticalProfile)
	{
		pMenu = menuPopup.GetSubMenu( 74 );
	}
}


LRESULT CAirsideVehiclePoolParkingDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{	
	return CAirsideObjectBaseDlg::DefWindowProc(message, wParam, lParam);
}

FallbackReason CAirsideVehiclePoolParkingDlg::GetFallBackReason(void)
{	
	return PICK_MANYPOINTS;
}
void CAirsideVehiclePoolParkingDlg::SetObjectPath(CPath2008& path)
{
	((VehiclePoolParking *)GetObject())->SetPath(path);
	m_path = path;
	m_bPathModified = true;
	LoadTree();
}

void CAirsideVehiclePoolParkingDlg::LoadTreeSubItemCtrlPoints(HTREEITEM preItem,const CPath2008& path )
{
	m_vLevelList.ReadFullLevelList();

	AirsideObjectTreeCtrlItemDataEx aoidDataEx;
	ItemStringSectionColor isscStringColor;
	CString strTemp = _T("");
	HTREEITEM hItemTemp = 0;

	int iPathCount = path.getCount();
	for(int i = 0;i < iPathCount; i++)
	{
		CString csPoint;
		double FloorAlt = path.getPoint(i).getZ()/SCALE_FACTOR;//floors.getVisibleFloorAltitude(  ) ;
		csPoint.Format("x = %.2f; y = %.2f ; floor z = %.2f",CARCLengthUnit::ConvertLength(DEFAULT_DATABASE_LENGTH_UNIT,CUnitPiece::GetCurSelLengthUnit(), path.getPoint(i).getX() ), \
			CARCLengthUnit::ConvertLength(DEFAULT_DATABASE_LENGTH_UNIT,CUnitPiece::GetCurSelLengthUnit(), path.getPoint(i).getY() ), FloorAlt);

		aoidDataEx.lSize = sizeof(aoidDataEx);
		aoidDataEx.dwptrItemData = 0;
		aoidDataEx.vrItemStringSectionColorShow.clear();
		isscStringColor.colorSection = RGB(0,0,255);
		strTemp.Format(_T("%.2f"),CARCLengthUnit::ConvertLength(DEFAULT_DATABASE_LENGTH_UNIT,CUnitPiece::GetCurSelLengthUnit(),path.getPoint(i).getX() ));		
		isscStringColor.strSection = strTemp;
		aoidDataEx.vrItemStringSectionColorShow.push_back(isscStringColor);

		strTemp.Format(_T("%.2f"),CARCLengthUnit::ConvertLength(DEFAULT_DATABASE_LENGTH_UNIT,CUnitPiece::GetCurSelLengthUnit(),path.getPoint(i).getY() ));		
		isscStringColor.strSection = strTemp;
		aoidDataEx.vrItemStringSectionColorShow.push_back(isscStringColor);

		strTemp.Format(_T("%.2f"),FloorAlt);		
		isscStringColor.strSection = strTemp;
		aoidDataEx.vrItemStringSectionColorShow.push_back(isscStringColor);

		hItemTemp = m_treeProp.InsertItem( csPoint, preItem  );
		m_treeProp.SetItemDataEx(hItemTemp,aoidDataEx);
	}	
}

void CAirsideVehiclePoolParkingDlg::LoadTree(void)
{
	m_treeProp.DeleteAllItems();
	m_treeProp.SetRedraw(0);

	CString strLabel = _T("");

	// control points.
	strLabel = CString( "Area Vertices(") + CARCLengthUnit::GetLengthUnitString(CUnitPiece::GetCurSelLengthUnit()) + CString( ")" );

	m_hCtrlPoints = m_treeProp.InsertItem(strLabel);
	LoadTreeSubItemCtrlPoints(m_hCtrlPoints,m_path);	
	m_treeProp.Expand( m_hCtrlPoints, TVE_EXPAND );

	////vertical profile.
	//strLabel = _T("Vertical Profile");
	//m_hVerticalProfile = m_treeProp.InsertItem(strLabel);

	m_treeProp.SetRedraw(1);
}


void CAirsideVehiclePoolParkingDlg::OnOK(void)
{
	if(GetObject())
	{
		CPath2008 path = ((VehiclePoolParking *)GetObject())->GetPath();
		if (path.getCount() ==0)
		{
			MessageBox(_T("Please pick the VehiclePoolParking path from the map."));
			return ;
		}		
		((VehiclePoolParking *)GetObject())->SetPath(m_path);
	}

	CAirsideObjectBaseDlg::OnOK();
}

bool CAirsideVehiclePoolParkingDlg::UpdateOtherData(void)
{
	CPath2008 path = ((VehiclePoolParking *)GetObject())->GetPath();
	if (path.getCount() ==0)
	{
		MessageBox(_T("Please pick the VehiclePoolParking path from the map."));
		return (false);
	}
	try
	{
		if (m_bPathModified && m_nObjID != -1)
			((VehiclePoolParking *)GetObject())->UpdateObject(m_nObjID);
	}
	catch (CADOException &e)
	{
		e.ErrorMessage();
		MessageBox(_T("Cann't update the Object path."));
		return (false);
	}

	return (true);
}

void CAirsideVehiclePoolParkingDlg::OnEditVerticalProfile(void)
{
	// get every floor's real height
	//CTermPlanDoc* pDoc	= GetDocument();
	//CFloors & floors = pDoc->GetLandsideDoc()->GetLevels();
	//int nFloorCount = pDoc->GetLandsideDoc()->GetLevels().m_vFloors.size();
	//std::vector<double>vFloorAltitudes;
	//
	//for(int i=0; i<nFloorCount; i++) 
	//{
	//	vFloorAltitudes.push_back( i );				
	//}

	////get x (distance from point 0) and z
	//std::vector<double>vXYPos;
	//std::vector<double>vZPos;
	//	
	//CPath2008 tmpPath =m_vCtrlPoints.getPath();	
	//if( tmpPath.getCount() < 2)
	//{
	//	return;
	//}
	////adjust path
	////AdjustZVal(tmpPath);

	//vXYPos.reserve(tmpPath.getCount());
	//vZPos.reserve(tmpPath.getCount());		
	//vXYPos.push_back(0.0); 
	//vZPos.push_back( tmpPath.getPoint(0).getZ() /SCALE_FACTOR);
	//
	//double dLensoFar = 0.0;
	//
	//for(int i=1;i<tmpPath.getCount();i++)
	//{
	//	ARCVector3 v3D(tmpPath[i].getX()-tmpPath[i-1].getX(), tmpPath[i].getY()-tmpPath[i-1].getY(), 0.0);
	//	dLensoFar += UNITSMANAGER->ConvertLength(v3D.Magnitude());
	//	vXYPos.push_back(dLensoFar);				
	//	vZPos.push_back( tmpPath[i].getZ() /SCALE_FACTOR );
	//}

	//CStringArray strArrFloorName;
	//strArrFloorName.SetSize(nFloorCount);
	//for(int k=0;k<nFloorCount;k++)
	//{
	//	strArrFloorName[k] = pDoc->GetLandsideDoc()->GetLevels().m_vFloors[k]->FloorName();
	//}
	//CChangeVerticalProfileDlg dlg(vXYPos,vZPos,vFloorAltitudes,tmpPath,&strArrFloorName);
	//

	//if( dlg.DoModal() == IDOK )
	//{	
	//	ASSERT(tmpPath.getCount() == (int) vZPos.size());
	//	vZPos = dlg.m_wndPtLineChart.m_vPointYPos;
	//	for(size_t i=0;i<m_vCtrlPoints.size();i++)
	//	{
	//		m_vCtrlPoints[i][VZ] = vZPos[i] * SCALE_FACTOR ;//pDoc->GetLandsideDoc()->GetLevels().getVisibleAltitude( vZPos[i] * SCALE_FACTOR ) ;
	//	}
	//	
	//	LoadTree();
	//}
}

bool CAirsideVehiclePoolParkingDlg::ConvertUnitFromDBtoGUI(void)
{
	//it do nothing , so return (false);
	return (false);
}

bool CAirsideVehiclePoolParkingDlg::RefreshGUI(void)
{
	LoadTree();
	return (true);
}

bool CAirsideVehiclePoolParkingDlg::ConvertUnitFromGUItoDB(void)
{
	//it do nothing , so return (false);
	return (false);
}
