#pragma once
#include "cResourceFile.h"

#include "value.h"
class cTextureFile :
	public cResourceFile
{
public:
	std::vector<LPDIRECT3DTEXTURE9>	m_vecTexture;
	ST_MTL_TEX*			m_pMtlTex;

	void	AddTexture(LPDIRECT3DTEXTURE9 pTex);
public:
	virtual	void	Render(cObjBase* pObj, cBaseShader* pShader = NULL, bool IsAni = true);
public:
	cTextureFile();
	~cTextureFile();
};

