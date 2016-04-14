#pragma once
#include "cResourceFile.h"
#include "value.h"

class cSpriteFile :
	public cResourceFile
{
public:
	std::vector<DXUTTextureNode*>	m_vecTexture;

	void	AddTexture(DXUTTextureNode* pTex) { m_vecTexture.push_back(pTex); }

public:
	DXUTTextureNode*	GetTexture(cObjBase* pObj);
	virtual	void	Render(cObjBase* pObj, cBaseShader* pShader = NULL, bool IsAni = true);
public:
	cSpriteFile();
	~cSpriteFile();
};

