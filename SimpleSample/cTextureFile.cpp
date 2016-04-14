#include "DXUT.h"
#include "cTextureFile.h"
#include "cObjBase.h"
#include "cSystemMgr.h"
#include "cResourceMgr.h"
#include "cAseFile.h"
#include "cGeomNode.h"

cTextureFile::cTextureFile()
:m_pMtlTex( NULL )
{
}


cTextureFile::~cTextureFile()
{
	/*
	for each(auto it in m_vecTexture)
	{
		SAFE_RELEASE(it);
	}
	*/
	SAFE_DELETE(m_pMtlTex);
}


void	cTextureFile::Render(cObjBase* pObj, cBaseShader* pShader , bool IsAni  )
{

	float f = pObj->GetAniKey() * 10.0f;

	if (IsAni)
	{
		f += _GETSINGLE(cSystemMgr)->GetDeltaTime() * 20.0f * m_fAniSpeed ;
	}
	pObj->SetAniKey(f * 0.1f);

	int nIndex = f;
	if (nIndex >= m_vecTexture.size())
	{
		nIndex = m_vecTexture.size() - 1;
		pObj->OnAnimationEnd();
	}
	else if (nIndex < 0)
	{
		nIndex = 0;
	}
	cAseFile*	pAseFile = (cAseFile*)
		_GETSINGLE(cResourceMgr)->GetPlan();
	if (m_pMtlTex == NULL)
	{
		m_pMtlTex = new ST_MTL_TEX();

	}

	m_pMtlTex->pTex
		 = m_vecTexture[nIndex];

	pAseFile->m_pRootNode->m_pMtlTex = m_pMtlTex;

	pAseFile->Render(pObj, pShader, false);
}


void	cTextureFile::AddTexture(LPDIRECT3DTEXTURE9 pTex) { m_vecTexture.push_back(pTex); }
