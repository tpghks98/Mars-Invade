#include "DXUT.h"
#include "cAseFile.h"
#include "cGeomNode.h"
#include "cObjBase.h"
#include "cSystemMgr.h"

cAseFile::cAseFile()
{
}


cAseFile::~cAseFile()
{
	SAFE_DELETE(m_pRootNode);

	for each(auto it in m_vecMtlTex)
	{
		SAFE_RELEASE(it.pTex);
		SAFE_RELEASE(it.pBump)

	}
}

void	cAseFile::Render(cObjBase* pObj, cBaseShader* pShader, bool IsAni)
{
	float f = pObj->GetAniKey();

	if (f < m_nFirstFrame)
	{
		f = m_nFirstFrame;
	}
	

	if (IsAni)
	{
		f += m_nFrameSpeed * m_nTicksPerFrame * 0.005f * m_fAniSpeed * _GETSINGLE(cSystemMgr)->GetDeltaTime();
	}
	bool IsAniEnd = false;
	if (f < m_nFirstFrame)
	{
		f = m_nFirstFrame;
	}
	else if (f >= m_nLastFrame)
	{
		f = m_nLastFrame;
		IsAniEnd = true;
	}
	pObj->SetAniKey(f);

	if (IsAniEnd)
	{
		pObj->OnAnimationEnd();
	}

	DWORD dwTime = f * m_nTicksPerFrame;

	m_pRootNode->Update(dwTime, pObj->GetWorldMatrixPointer());
	m_pRootNode->Render(pObj, pShader);
}