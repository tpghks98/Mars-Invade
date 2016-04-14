#include "DXUT.h"
#include "cSpriteFile.h"
#include "cObjBase.h"
#include "cSystemMgr.h"
#include "cResourceMgr.h"

cSpriteFile::cSpriteFile()
{
}


cSpriteFile::~cSpriteFile()
{
	m_vecTexture.clear();
}

void	cSpriteFile::Render(cObjBase* pObj, cBaseShader* pShader, bool IsAni)
{
	float f = pObj->GetAniKey() * 10.0f;

	if (IsAni)
	{
		f += _GETSINGLE(cSystemMgr)->GetDeltaTime() * 20.0f * m_fAniSpeed;
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

	int nWidth = m_vecTexture[nIndex]->dwWidth;
	int nHeight = m_vecTexture[nIndex]->dwHeight;

	D3DXVECTOR3		vCenter;
	vCenter.x = nWidth * 0.5f;
	vCenter.y = nHeight * 0.5f;
	vCenter.z = 0.0f;

	RECT rt;
	SetRect(&rt, 0, 0, nWidth * _GETS( cResourceMgr )->GetPerRect(), nHeight);

	g_pSprite9->SetTransform(pObj->GetWorldMatrixPointer());

	g_pSprite9->Begin(D3DXSPRITE_ALPHABLEND);
	g_pSprite9->Draw(m_vecTexture[nIndex]->pTexture9, &rt, &vCenter, NULL, D3DXCOLOR(1, 1, 1, pObj->GetAlpha()));
	g_pSprite9->End();

	_GETS(cResourceMgr)->SetPerRect(1.0f);
}

DXUTTextureNode*	cSpriteFile::GetTexture(cObjBase* pObj)
{
	float f = pObj->GetAniKey() * 10.0f;
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

	return m_vecTexture[nIndex];
}
