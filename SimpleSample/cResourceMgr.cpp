#include "DXUT.h"
#include "cResourceMgr.h"
#include "cAseFile.h"
#include "cAseLoader.h"
#include "cGeomNode.h"
#include "cCustomShader.h"
#include "cCreateShadow.h"
#include "cApplyShadow.h"
#include "cTextureFile.h"
#include "cSpriteFile.h"

cResourceMgr::cResourceMgr()
:m_pBumpTex( NULL )
, m_pPlan( NULL )
, m_fPerRect( 1.0f )
{
	m_vecShader.resize(SI_MAX);
}


cResourceMgr::~cResourceMgr()
{
	for each(auto it in m_mapResource)
	{
		SAFE_DELETE( it.second);
	}
	m_mapResource.clear();

	for each(auto it in m_vecShader)
	{
		SAFE_DELETE(it);
	}
	m_vecShader.clear();
}


void	cResourceMgr::AddAseFile(char* pKey, char* pFileName)
{
	cAseFile* pAseFile = new cAseFile();

	cAseLoader Loader;
	if (pKey == "map")
	{
		Loader.m_IsFacesCopy = true;
	}
	Loader.LoadAseFile(pAseFile, pFileName);

	m_mapResource.insert(std::pair<std::string, cResourceFile*>(pKey, pAseFile));

	pAseFile->SetKey(pKey);
}


void	cResourceMgr::AddShader(int nID, char* pFileName)
{
	
	DWORD dwFlag = 0;

	LPD3DXEFFECT	pEffect;
	LPD3DXBUFFER	pError;

	D3DXCreateEffectFromFileA(DXUTGetD3D9Device(), pFileName, 
		NULL, NULL, dwFlag, NULL, &pEffect, &pError);

	if (!pEffect &&  pError)
	{
		int nSize = pError->GetBufferSize();

		void*	ask = pError->GetBufferPointer();

		if (ask)
		{
			char* str = new char[nSize];
			sprintf(str, (const char*)ask, nSize );

			delete[] str;
		}
		return;
	}

	cBaseShader*	pShader = NULL;

	switch (nID)
	{
	case SI_CUSTOM:
		pShader = new cCustomShader();
		break;
	case SI_CREATE:
		pShader = new cCreateShadow();
		break;
	case SI_APPLY:
		pShader = new cApplyShadow();
		break;
	}
	pShader->SetEffect(pEffect);
	m_vecShader[nID] = pShader;
}

void	cResourceMgr::AddTexture(char* pKey, LPCWSTR pFileName, int nFrame)
{
	cTextureFile*	pTextureFile = new	cTextureFile();

	WCHAR szFile[2048];
	std::wstring str;

	for (int n = 0; n < nFrame; ++n)
	{
		LPDIRECT3DTEXTURE9 pTex;

		wsprintf(szFile, pFileName, n);

		DXUTTextureNode*	pTextureNode = g_DialogResourceManager.GetTextureNode(g_DialogResourceManager.AddTexture(szFile));

		pTextureFile->AddTexture(pTextureNode->pTexture9);
	}

	m_mapResource.insert(std::pair<std::string, cResourceFile*>(pKey, pTextureFile));

}

void	cResourceMgr::AddSprite(char* pKey, LPCWSTR pFileName, int nFrame)
{
	cSpriteFile*	pSpriteFile = new	cSpriteFile();

	WCHAR	szFile[2048] = {};
	for (int n = 0; n < nFrame; ++n)
	{
		if (n < 10)
		{
			wsprintf(szFile, pFileName, n);
		}

		DXUTTextureNode*	pTextureNode = g_DialogResourceManager.GetTextureNode( g_DialogResourceManager.AddTexture(szFile) );

		pSpriteFile->AddTexture(pTextureNode);
	}
	pSpriteFile->SetKey( pKey );

	m_mapResource.insert(std::pair < std::string, cResourceFile*>(pKey, pSpriteFile));
}



cResourceFile*	cResourceMgr::GetResource(char* pKey)
{
	auto p = m_mapResource.find(pKey);
	 return p->second; 
}

