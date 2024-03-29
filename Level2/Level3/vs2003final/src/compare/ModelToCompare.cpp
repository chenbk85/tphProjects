// ModelToCompare.cpp: implementation of the CModelToCompare class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

#include "ModelToCompare.h"

#include "../Engine/terminal.h"
#include "../Reports/ReportParaOfProcs.h"
#include "../common/SimAndReportManager.h"
#include "../common/AirportDatabase.h"
#include "../common/AirportDatabaseList.h"

#include "../Engine/terminal.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModelToCompare::CModelToCompare()
{
	m_terminal = NULL;
	m_bNeedCopy = false;

}

CModelToCompare::~CModelToCompare()
{
	if(m_terminal->m_pAirportDB != NULL)
	{
		m_terminal->m_pAirportDB->clearData();
	}
	delete m_terminal;
	m_terminal = NULL;
}

const CString& CModelToCompare::GetModelName() const
{
	return m_strModelName;
}
void CModelToCompare::SetUniqueName(const CString& strName)
{
	m_strUniqueName = strName;

}
const CString& CModelToCompare::GetUniqueName() const
{
	return m_strUniqueName;
}
void CModelToCompare::SetModelName(const CString& strName)
{
	m_strModelName = strName;
}

const CString& CModelToCompare::GetModelLocation() const
{
	return m_strModelLocation;
}

void CModelToCompare::SetModelLocation(const CString& strLocation)
{
	m_strModelLocation = strLocation;
}
const CString& CModelToCompare::GetLocalPath() const
{
	return m_strLocalPath;
}

void CModelToCompare::SetLocalPath(const CString& strLocalPath)
{
	m_strLocalPath = strLocalPath;
}

const CString& CModelToCompare::GetDataBasePath() const
{
	return m_strDatabasePath;
}

void CModelToCompare::SetDataBasePath(const CString& strDataBasePath)
{
	m_strDatabasePath = strDataBasePath;
}

const CString& CModelToCompare::GetLastModifyTime() const
{
	return m_lastModifiedTime;
}

void CModelToCompare::SetLastModifyTime(const CString& strLastTime)
{
	m_lastModifiedTime = strLastTime;
}

Terminal *CModelToCompare::GetTerminal()
{
	//if (m_terminal == NULL)
	//{
	//	m_terminal = InitTerminal();
	//}
	return m_terminal;
}
InputTerminal *CModelToCompare::GetInputTerminal()
{
	//if (m_terminal == NULL)
	//{
	//	m_terminal = InitTerminal();
	//}
	return m_terminal;
}
BOOL CModelToCompare::TransferFiles(const CString& strSource, const CString& strDest,void (CALLBACK* _ShowCopyInfo)(LPCTSTR))
{
	CFileOperation fo;
	fo.SetAskIfReadOnly(false);
	fo.SetOverwriteMode(true);
	return fo.NotCopyFolderOfSimResult(strSource, strDest,_ShowCopyInfo);
}


void CModelToCompare::RemoveFiles(const CString &strPath)
{
	CFileOperation fo;
	fo.SetAskIfReadOnly(false);
	fo.Delete(strPath);
}

BOOL CModelToCompare::IsLocalModel(const CString &strPath)
{
	CString str = _T("(LOCAL)");
	if (strPath.Left(2) == _T("\\\\"))
	{
		int nPos = strPath.Find(_T("\\"), 2);
		if (nPos != -1)
			str = strPath.Mid(2, nPos - 2);
		else
			str = strPath.Mid(2, strPath.GetLength() - 2);
	}

	str.MakeUpper();

	return ((str == _T("(LOCAL)")) ? TRUE : FALSE);

}
Terminal* CModelToCompare::InitTerminal(CCompRepLogBar* pStatus, CString strName, void (CALLBACK* _ShowCopyInfo)(LPCTSTR))
{
	if (m_terminal != NULL)
		return m_terminal;

	CString strPath = PROJMANAGER->GetAppPath();
	strPath += _T("\\Comparative Report\\") + strName;
		

	CString strStatus;
	CString strTempPath = strPath;
	CString strDbName = "";
	int	 nProgressStatus = 0;
	CString strImportDBPath = PROJMANAGER->GetAppPath() + _T("\\ImportDB\\");
	
	//	Copy data
	BOOL bLocal = IsLocalModel(m_strModelLocation);

	if ( !bLocal)// network model data
	{

		CString strDbPath;   //get the GlobalDBList.ini path
		CString strProjPath = m_strModelLocation; //the project path
		m_strLocalPath = strPath + _T('\\') + m_strModelName;//save the local path

		CString strNetworkProjPath;//the network project file's path 
		int nPos  = strProjPath.ReverseFind('\\');
		strNetworkProjPath = strProjPath.Mid(0,nPos);

		CString strNetGetAppPath;//the network App path
		nPos = strNetworkProjPath.ReverseFind('\\');
        strNetGetAppPath = strNetworkProjPath.Mid(0,nPos);

		CString strNetworkDBPath;//the network ImportDB Path
		strNetworkDBPath = strNetGetAppPath + _T("\\ImportDB\\");

		//get the projInfo file  , read the dbIndex or dbName.
		//if dbname is not exist ,get the Globaldblist and 
		//find out the dbname according to the index
		PROJECTINFO pi;	
		PROJMANAGER->GetProjectInfo(m_strModelName, &pi, strNetworkProjPath);//get the project information

		//get the project  name
		nPos = m_strModelLocation.ReverseFind('\\');
		CString strModelname = m_strModelLocation.Mid(nPos+1,m_strModelLocation.GetLength()-1);

		//find that if the project exist or not in local
		CString strLocalProjectPath = strPath + _T('\\') + strModelname;
		CFileFind file; 
		BOOL bFind = file.FindFile(strLocalProjectPath,0);

		if ( !bFind)
		{

			TransferFiles(m_strModelLocation, strPath,_ShowCopyInfo);//copy the project
			m_lastModifiedTime = pi.modifytime.Format("%y %m %d %H %M");//save the last modify time

			if (strlen(pi.dbname) != 0)
			{
				strDbName = pi.dbname;//get AirportDataBase name
				CString strImportDbPath = strNetworkDBPath + strDbName;

				TransferFiles(strImportDbPath, strPath,_ShowCopyInfo);//copy the DataBase
				m_bNeedCopy = true;
				m_strDatabasePath = strPath + _T('\\') + strDbName;

			}
			else//dbname is not exist ,get the Globaldblist and find out the dbname according to the index
			{
				strDbPath = strNetGetAppPath + _T("\\GlobalDBList.ini");

				CAirportDatabaseList dbList;
				dbList._LoadDataSet(strDbPath);

				if (dbList.getDBCount() < pi.lUniqueIndex)
				{
					return NULL;
				}
				strDbName = dbList.getAirportDBByIndex(pi.lUniqueIndex)->getName();//get AirportDataBase name
				CString strDBFolderPath = dbList.getAirportDBByIndex(pi.lUniqueIndex)->getFolder();
				nPos = strDBFolderPath.ReverseFind('\\');
				CString strName = strDBFolderPath.Mid(nPos+1,strDBFolderPath.GetLength()-1);//get database real name

				CString strImportDbPath = strNetworkDBPath + strName;//the network database path
				TransferFiles(strImportDbPath, strPath,_ShowCopyInfo);

				m_strDatabasePath = strPath + _T('\\') + strName;	

			}

		}
		else//project exist in local
		{
			PROJECTINFO piLocal;
			PROJMANAGER->GetProjectInfo(strModelname, &piLocal, strPath);//get the project information

			if ( pi.modifytime != piLocal.modifytime)//if modify,copy project again 
			{

				TransferFiles(m_strModelLocation, strPath,_ShowCopyInfo);
				m_lastModifiedTime = pi.modifytime.Format("%y %m %d %H %M");//save the last modify time

				if (strlen(pi.dbname) != 0)
				{

					strDbName = pi.dbname;//get AirportDataBase name
					CString strNetImportDbPath = strNetworkDBPath + strDbName;
					TransferFiles(strNetImportDbPath, strPath,_ShowCopyInfo);

					m_strDatabasePath = strPath +  _T('\\') + strDbName;//save the database's path

				}
				else//dbname is not exist ,get the Globaldblist and find out the dbname according to the index
				{
					strDbPath = strNetGetAppPath + _T("\\GlobalDBList.ini");

					CAirportDatabaseList dbList;
					dbList._LoadDataSet(strDbPath);

					if (dbList.getDBCount() < pi.lUniqueIndex)
					{

						return NULL;
					}

					strDbName = dbList.getAirportDBByIndex(pi.lUniqueIndex)->getName();//get AirportDataBase name

					CString strDBFolderPath = dbList.getAirportDBByIndex(pi.lUniqueIndex)->getFolder();
					nPos = strDBFolderPath.ReverseFind('\\');
					CString strName = strDBFolderPath.Mid(nPos+1,strDBFolderPath.GetLength()-1);//get the database real name

					CString strImportDbPath = strNetworkDBPath + strName;//database path


					TransferFiles(strImportDbPath, strPath,_ShowCopyInfo);

					m_strDatabasePath = strPath +  _T('\\') + strDbName;

				}
			}
			else//project exist in local,and the time is the same as network,and do not copy the project
			{


				//get the terminal data
				Terminal *term = new Terminal;
		
				nPos = m_strDatabasePath.ReverseFind('\\');
				CString strLocalDB = m_strDatabasePath.Mid(nPos+1,m_strDatabasePath.GetLength()-1);//get the database real name


				CAirportDatabase *pAirPort = new CAirportDatabase;
				pAirPort->setFolder(m_strDatabasePath);

				term->m_pAirportDB = pAirPort;
				if( !term->m_pAirportDB->hadLoadDatabase() )
					term->m_pAirportDB->loadDatabase();
				term->loadInputs(strPath + _T("\\") + m_strModelName);
				term->GetSimReportManager()->loadDataSet(strPath + _T("\\") + m_strModelName);
				term->GetSimReportManager()->SetCurrentSimResult(0);
				
				m_terminal = term;
				return term;
			}
		}
	}
	else
	{
		//	local data
		strPath = PROJMANAGER->GetAppPath() + _T("\\Project");
		m_strLocalPath = m_strModelLocation;
	}

	//get the terminal data
	Terminal *term = new Terminal;
	if(IsLocalModel(m_strModelLocation))
	{
		PROJECTINFO pi;	

		PROJMANAGER->GetProjectInfo(m_strModelName, &pi, strPath);

		CAirportDatabase *pAirPort = NULL;
		pAirPort = AIRPORTDBLIST->getAirportDBByName( pi.dbname );
		term->m_pAirportDB = pAirPort;
		if(pAirPort == NULL)
		{
			return NULL;
		}
		if( !term->m_pAirportDB->hadLoadDatabase() )
			term->m_pAirportDB->loadDatabase();

		m_strDatabasePath = PROJMANAGER->GetAppPath() + _T("\\ImportDB\\") + pAirPort->getName();
		m_lastModifiedTime = pi.modifytime.Format("%y %m %d %H %M");
	}		
	else
	{
		CAirportDatabase *pAirPort = new CAirportDatabase;
		pAirPort->setFolder(m_strDatabasePath);

		term->m_pAirportDB = pAirPort;
		if( !term->m_pAirportDB->hadLoadDatabase() )
			term->m_pAirportDB->loadDatabase();
	}			


	term->loadInputs(strPath + _T("\\") + m_strModelName);
	term->GetSimReportManager()->loadDataSet(strPath + _T("\\") + m_strModelName);
	term->GetSimReportManager()->SetCurrentSimResult(0);

	m_terminal = term;

	return term;
}

void CModelToCompare::AddSimResult(const CString& strSimResult)
{
	m_vSimResult.push_back(strSimResult);
}
int CModelToCompare::GetSimResultCount()
{
	return static_cast<int>(m_vSimResult.size());
}
CString CModelToCompare::GetSimResult(int nIndex)
{
	ASSERT(nIndex >=0 && nIndex < GetSimResultCount());
	return m_vSimResult[nIndex];

}
void CModelToCompare::ClearSimResult()
{
	m_vSimResult.clear();
}

//////////////////////////////////////////////////////////////////////////
//	CModelToCompareDataSet
CModelToCompareDataSet::CModelToCompareDataSet() : DataSet(ModelInfoFile)
{
}

CModelToCompareDataSet::~CModelToCompareDataSet()
{
}

void CModelToCompareDataSet::clear()
{
}

void CModelToCompareDataSet::readData(ArctermFile& p_file)
{
	char buf[256];
	
	m_vModels.clear();
	
	
	while (p_file.getLine() == 1)
	{
		CModelToCompare *model = new CModelToCompare;
		
		// read the Model name;
		memset(buf, 0, sizeof(buf) / sizeof(char));
		if (p_file.getField(buf, 255) != 0)
			model->SetModelName(buf);

		int nSimResultCount;
		p_file.getInteger(nSimResultCount);
		for (int i =0; i < nSimResultCount; i++)
		{
			p_file.getField(buf,255);
			model->AddSimResult(buf);
		}

		// read the unique name;
		memset(buf, 0, sizeof(buf) / sizeof(char));
		if (p_file.getField(buf, 255) != 0)
			model->SetUniqueName(buf);

		// read the model location
		memset(buf,0,sizeof(buf) / sizeof(char));
		if (p_file.getField(buf, 255) != 0)
			model->SetModelLocation(buf);

		//read the local path
		memset(buf, 0, sizeof(buf) / sizeof(char));
		if (p_file.getField(buf, 255) != 0)
			model->SetLocalPath(buf);

		//read the database path
		memset(buf, 0, sizeof(buf) / sizeof(char));
		if (p_file.getField(buf, 255) != 0)
			model->SetDataBasePath(buf);

		//read the last modify time
		memset(buf, 0, sizeof(buf) / sizeof(char));
		if (p_file.getField(buf, 255) != 0)
			model->SetLastModifyTime(buf);

		model->InitTerminal(NULL,m_strProjName,NULL);
		
		m_vModels.push_back(model);
	}
}

void CModelToCompareDataSet::readObsoleteData(ArctermFile& p_file)
{
}

void CModelToCompareDataSet::writeData(ArctermFile& p_file) const
{
	for (unsigned i = 0; i < m_vModels.size(); i++)
	{
		p_file.writeField(m_vModels[i]->GetModelName());//write the model name
		
		int nSimResultCount = m_vModels[i]->GetSimResultCount();
		p_file.writeInt(nSimResultCount);
		for (int j = 0; j < nSimResultCount; ++j)
		{
			p_file.writeField(m_vModels[i]->GetSimResult(j));
		}

		p_file.writeField(m_vModels[i]->GetUniqueName());//write the unique name
		p_file.writeField(m_vModels[i]->GetModelLocation());//write the network path
		p_file.writeField(m_vModels[i]->GetLocalPath());//write the local path
		p_file.writeField(m_vModels[i]->GetDataBasePath());//write the database path
		p_file.writeField(m_vModels[i]->GetLastModifyTime());//write the last modify time
		p_file.writeLine();
	}

	if (m_vModels.empty())
		p_file.writeLine();
}

int CModelToCompareDataSet::GetModels(OUT std::vector<CModelToCompare *>& vModels)
{
	vModels = m_vModels;
	return m_vModels.size();
}

void CModelToCompareDataSet::SetModels(const std::vector<CModelToCompare *>& vModels)
{
	m_vModels = vModels;
}
