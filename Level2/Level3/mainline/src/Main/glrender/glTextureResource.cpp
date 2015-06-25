#include "StdAfx.h"

#include ".\gltextureresource.h"
#include <Il/il.h>
#include <Il/ilut.h>

#include <Common/ARCMathCommon.h>
const static CString strTextureDataFile = "textures.data" ;

CTexture::CTexture()
{
	m_iTextureId = -1;
	m_eTextureType  = GL_TEXTURE_2D;
}

CTexture::CTexture( const CString& strFile ) :m_strFileName(strFile)
{
	m_iTextureId = -1;
	m_eTextureType  = GL_TEXTURE_2D;
}
void CTexture::Apply()
{
	if(!glIsTexture(m_iTextureId)) {
		ilInit();

		ilutInit();
		ilutRenderer(ILUT_OPENGL);
		ilutEnable(ILUT_OPENGL_CONV);
		
		m_iTextureId = ilutGLLoadImage(m_strFileName.GetBuffer());
      
	}
	glEnable(m_eTextureType);
	glBindTexture(m_eTextureType,m_iTextureId);	
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}

void CTexture::UnApply()
{
	glDisable(m_eTextureType);
}

CTexture::~CTexture(){
	glDeleteTextures(1,&m_iTextureId);
}

GLenum CTexture::GetTextureType() const
{
	return m_eTextureType;
}
CTextureResource::CTextureResource(const CString& foldpath)
{
	m_strpath = foldpath;
}

CTextureResource::~CTextureResource(void)
{
}

CTexture * CTextureResource::getTexture(const CString& strID){
	
	std::map<CString, ref_ptr<CTexture> >::iterator itr;
	for(itr = m_vResideTextures.begin();itr!= m_vResideTextures.end();itr++)
	{
		if((*itr).first.CompareNoCase(strID) == 0)
		{
			return (*itr).second.get();
		}
	}
	return NULL;
}

CTexture * CTextureResource::NewTextureAndReside(const CString& filename,const CString& strID)
{
	CTexture * ret = NULL;
	if(ret = getTexture(strID)) return ret;
	if(ret = NewTexture(filename) ){
		m_vResideTextures[strID] = ret;
		return ret;
	}
	return NULL;
}

CTexture * CTextureResource::NewTexture(const CString& filename)
{
	CTexture * ret  = new CTexture;	
	ret->m_strFileName = filename;	
	return ret;
}

BOOL CTextureResource::Load()
{
	CFile * pFile = NULL;
	CString sFileName = m_strpath + _T("\\") + strTextureDataFile;
	
	try{
		pFile = new CFile(sFileName, CFile::modeRead | CFile::shareDenyNone);
		CArchive ar(pFile, CArchive::load);
		const static int strbufsize = 512;
		char line[strbufsize];
		//skip a line;
		ar.ReadString(line,strbufsize);
		//read head
		ar.ReadString(line,strbufsize);
		if(_stricmp(line,"Textures Database")==0){
			//skip the column name;
			ar.ReadString(line,strbufsize);
			//read the values
			//
			CString sPath = m_strpath + "\\";
			while(*(ar.ReadString(line, 512)) != '\0')
			{				
				CString name, filename;
				char* b = line;
				char* p = NULL;
				int c = 1;
				while((p = strchr(b, ',')) != NULL)
				{
					*p = '\0';
					switch(c)
					{
					case 1: //name
						name = _T(b);
						break;
					case 2: //texture file
						filename = _T(b);
						break;					
					default:
						break;
					}
					b = ++p;
					c++;
				}
				if(b!=NULL) // the last column did not have a comma after it
				{
					switch(c)
					{
					case 1: //name
						name = _T(b);
						break;
					case 2: //texture file
						filename = _T(b);
					default:
						break;
					}
				}
				NewTextureAndReside(sPath + filename,name);
			}

		}
	}

	catch(CException * e){
		AfxMessageBox("Error reading Texture DB file: " + sFileName, MB_ICONEXCLAMATION | MB_OK);
		e->Delete();
		if(pFile != NULL)
			delete pFile;
		return FALSE;
	}
	return TRUE;
}

void CTextureResource::removeTexture(const CString& strID)
{
	m_vResideTextures.erase(strID);
}
void CTextureResource::SetResourcePath(const CString& foldpath){
	m_strpath  = foldpath;
}