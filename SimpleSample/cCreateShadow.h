#pragma once
#include "cBaseShader.h"
class cCreateShadow :
	public cBaseShader
{
public:
	virtual	void	Setting(LPDIRECT3DTEXTURE9 pTex,
		D3DXMATRIXA16* pWorldMatrix, cObjBase* pObject = NULL);

public:
	cCreateShadow();
	~cCreateShadow();
};

