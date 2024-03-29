// IniFile.cpp: implementation of the CIniReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INI.h"
#include <afxcoll.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



// Used to retrieve a value give the section and key
CString CIniReader::getKeyValue(CString strKey,CString strSection)
{
	char ac_Result[255];

	// Get the info from the .ini file	
	m_lRetValue = GetPrivateProfileString((LPCTSTR)strSection,(LPCTSTR)strKey,
		"",ac_Result, 255, (LPCTSTR)m_strFileName);	

	CString strResult(ac_Result);
	return strResult;
}

// Used to add or set a key value pair to a section
long CIniReader::setKey(CString strValue, CString strKey, CString strSection)
{
	m_lRetValue = WritePrivateProfileString (strSection, strKey, 
                             strValue, m_strFileName);

	return m_lRetValue;
}

// Used to find out if a given section exists
BOOL CIniReader::sectionExists(CString strSection)
{
	char ac_Result[100];	
	CString csAux;
	// Get the info from the .ini file	
	m_lRetValue = GetPrivateProfileString((LPCTSTR)strSection,NULL,
		"",ac_Result, 90, (LPCTSTR)m_strFileName);
	// Return if we could retrieve any info from that section
	return (m_lRetValue > 0);
}

// Used to retrieve all of the  section names in the ini file
CStringList* CIniReader::getSectionNames()  //returns collection of section names
{
	char ac_Result[2000];
	m_sectionList->RemoveAll();
	
	m_lRetValue = GetPrivateProfileSectionNames(ac_Result,2000,(LPCTSTR)m_strFileName);
	
	CString strSectionName;
	for(int i=0; i<m_lRetValue; i++)
	{
		if(ac_Result[i] != '\0') {
			strSectionName = strSectionName + ac_Result[i];
		} else {
			if(strSectionName != "") {
				m_sectionList->InsertAfter(m_sectionList->GetTailPosition(),strSectionName);
			}
			strSectionName = "";
		}
	}

	return m_sectionList;
}

// Used to retrieve all key/value pairs of a given section.  
CStringList* CIniReader::getSectionData(CString strSection)  
{
	char ac_Result[2000];  //change size depending on needs
	m_sectionDataList->RemoveAll();
	m_lRetValue = GetPrivateProfileSection((LPCTSTR)strSection, ac_Result, 2000, (LPCTSTR)m_strFileName);

	CString strSectionData;
	for(int i=0; i<m_lRetValue; i++)
	{
		if(ac_Result[i] != '\0') {
			strSectionData = strSectionData + ac_Result[i];
		} else {
			if(strSectionData != "") {
				m_sectionDataList->InsertAfter(m_sectionDataList->GetTailPosition(),strSectionData);
			}
			strSectionData = "";
		}
	}

	return m_sectionDataList;
}

void CIniReader::setINIFileName(CString strINIFile)
{
	m_strFileName = strINIFile;
}

CString CIniReader::GetDataLeft(CString str)
{
	int pos=str.Find('=');
	if(pos!=-1)
	{
		return str.Left(pos) ;
	}
	else
		return CString("");

}

CString CIniReader::GetDataRight(CString str)
{
	int pos=str.Find('=');
	if(pos!=-1)
	{
		return str.Right(str.GetLength()- pos-1) ;
	}
	else
		return CString("");

}

int CIniReader::DispartStrToSomeInt(CString &str, int *pInt)
{
	int nIndex=str.Find(',',0);
	CString strTemp;
	int i=0;
	while(nIndex!=-1)
	{
		strTemp=str.Left(nIndex);
		pInt[i]=atoi(strTemp.GetBuffer(strTemp.GetLength()));
		str=str.Right(str.GetLength()-nIndex-1);
		nIndex=str.Find(',',0);
		i++;
		if(i==128) break;
	}
	return i;
}

void CIniReader::getSectionData(CString strSection, CStringList *pHistoryList)
{
	char ac_Result[2000];  //change size depending on needs
	m_lRetValue=GetPrivateProfileSection((LPCTSTR)strSection, ac_Result, 2000, (LPCTSTR)m_strFileName);
	
	CString strSectionData;
	for(int i=0; i<m_lRetValue; i++)
	{
		if(ac_Result[i] != '\0') {
			strSectionData = strSectionData + ac_Result[i];
		} else {
			if(strSectionData != "") {
				pHistoryList->InsertAfter(pHistoryList->GetTailPosition(),strSectionData);
			}
			strSectionData = "";
		}
	}
}
