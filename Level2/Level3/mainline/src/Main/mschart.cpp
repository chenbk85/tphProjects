// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "mschart.h"

/////////////////////////////////////////////////////////////////////////////
// CMSChart1

IMPLEMENT_DYNCREATE(CMSChart1, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CMSChart1 properties

/////////////////////////////////////////////////////////////////////////////
// CMSChart1 operations

BOOL CMSChart1::GetAutoIncrement()
{
	BOOL result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetAutoIncrement(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CMSChart1::GetRandomFill()
{
	BOOL result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetRandomFill(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CMSChart1::GetChartType()
{
	long result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetChartType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CMSChart1::GetColumn()
{
	short result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetColumn(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CMSChart1::GetColumnCount()
{
	short result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetColumnCount(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString CMSChart1::GetColumnLabel()
{
	CString result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetColumnLabel(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

short CMSChart1::GetColumnLabelCount()
{
	short result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetColumnLabelCount(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CMSChart1::GetColumnLabelIndex()
{
	short result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetColumnLabelIndex(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString CMSChart1::GetData()
{
	CString result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetData(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString CMSChart1::GetFootnoteText()
{
	CString result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetFootnoteText(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL CMSChart1::GetRepaint()
{
	BOOL result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetRepaint(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

short CMSChart1::GetRow()
{
	short result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetRow(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CMSChart1::GetRowCount()
{
	short result;
	InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetRowCount(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString CMSChart1::GetRowLabel()
{
	CString result;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetRowLabel(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

short CMSChart1::GetRowLabelCount()
{
	short result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetRowLabelCount(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CMSChart1::GetRowLabelIndex()
{
	short result;
	InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetRowLabelIndex(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CMSChart1::GetSeriesColumn()
{
	short result;
	InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetSeriesColumn(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CMSChart1::GetSeriesType()
{
	long result;
	InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetSeriesType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x12, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CMSChart1::GetShowLegend()
{
	BOOL result;
	InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetShowLegend(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x13, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CMSChart1::GetDrawMode()
{
	long result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetDrawMode(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CMSChart1::GetBorderStyle()
{
	long result;
	InvokeHelper(DISPID_BORDERSTYLE, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetBorderStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_BORDERSTYLE, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CMSChart1::GetEnabled()
{
	BOOL result;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CMSChart1::GetHWnd()
{
	long result;
	InvokeHelper(DISPID_HWND, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH CMSChart1::GetTitle()
{
	LPDISPATCH result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH CMSChart1::GetFootnote()
{
	LPDISPATCH result;
	InvokeHelper(0x1b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString CMSChart1::GetTitleText()
{
	CString result;
	InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetTitleText(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL CMSChart1::GetStacking()
{
	BOOL result;
	InvokeHelper(0x1d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetStacking(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x1d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CMSChart1::GetTextLengthType()
{
	long result;
	InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetTextLengthType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CMSChart1::GetAllowSelections()
{
	BOOL result;
	InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetAllowSelections(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x20, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CMSChart1::GetAllowSeriesSelection()
{
	BOOL result;
	InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetAllowSeriesSelection(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x21, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CMSChart1::GetAllowDynamicRotation()
{
	BOOL result;
	InvokeHelper(0x22, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetAllowDynamicRotation(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x22, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

short CMSChart1::GetActiveSeriesCount()
{
	short result;
	InvokeHelper(0x23, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

LPDISPATCH CMSChart1::GetBackdrop()
{
	LPDISPATCH result;
	InvokeHelper(0x24, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH CMSChart1::GetLegend()
{
	LPDISPATCH result;
	InvokeHelper(0x26, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH CMSChart1::GetDataGrid()
{
	LPDISPATCH result;
	InvokeHelper(0x27, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH CMSChart1::GetPlot()
{
	LPDISPATCH result;
	InvokeHelper(0x28, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL CMSChart1::GetAllowDithering()
{
	BOOL result;
	InvokeHelper(0x2b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetAllowDithering(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CMSChart1::GetDoSetCursor()
{
	BOOL result;
	InvokeHelper(0x2d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetDoSetCursor(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CMSChart1::GetChart3d()
{
	BOOL result;
	InvokeHelper(0x2f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

VARIANT CMSChart1::GetChartData()
{
	VARIANT result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetChartData(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

long CMSChart1::GetMousePointer()
{
	long result;
	InvokeHelper(0x4a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetMousePointer(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void CMSChart1::Refresh()
{
	InvokeHelper(DISPID_REFRESH, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CMSChart1::EditPaste()
{
	InvokeHelper(0x31, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CMSChart1::EditCopy()
{
	InvokeHelper(0x32, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CMSChart1::Layout()
{
	InvokeHelper(0x34, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CMSChart1::ToDefaults()
{
	InvokeHelper(0x35, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CMSChart1::SelectPart(short part, short index1, short index2, short index3, short index4)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0x38, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 part, index1, index2, index3, index4);
}

void CMSChart1::GetSelectedPart(short* part, short* index1, short* index2, short* index3, short* index4)
{
	static BYTE parms[] =
		VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2;
	InvokeHelper(0x39, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 part, index1, index2, index3, index4);
}

void CMSChart1::TwipsToChartPart(long xVal, long yVal, short* part, short* index1, short* index2, short* index3, short* index4)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2;
	InvokeHelper(0x3b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 xVal, yVal, part, index1, index2, index3, index4);
}

long CMSChart1::GetOLEDragMode()
{
	long result;
	InvokeHelper(0x60e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetOLEDragMode(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CMSChart1::GetOLEDropMode()
{
	long result;
	InvokeHelper(0x60f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetOLEDropMode(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void CMSChart1::OLEDrag()
{
	InvokeHelper(0x610, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPUNKNOWN CMSChart1::GetDataSource()
{
	LPUNKNOWN result;
	InvokeHelper(0x47, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetRefDataSource(LPUNKNOWN newValue)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x47, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 newValue);
}

CString CMSChart1::GetDataMember()
{
	CString result;
	InvokeHelper(0x4b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CMSChart1::SetDataMember(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x4b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "mschart.h"

/////////////////////////////////////////////////////////////////////////////
// CMschart

IMPLEMENT_DYNCREATE(CMschart, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CMschart properties

/////////////////////////////////////////////////////////////////////////////
// CMschart operations
