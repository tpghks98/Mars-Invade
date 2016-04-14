#pragma once

#include "value.h"
class cObjBase;
class cBaseShader
{
protected:
	_SYNTHESIZE_INHER(LPD3DXEFFECT, m_pEffect, Effect);

	virtual	void	Setting(LPDIRECT3DTEXTURE9 pTex,
		D3DXMATRIXA16* pWorldMatrix, cObjBase* pObject = NULL);
public:
	cBaseShader();
	virtual ~cBaseShader();
};

