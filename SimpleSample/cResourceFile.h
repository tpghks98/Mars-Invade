#pragma once

#include "value.h"
class cObjBase;
class cBaseShader;
class cResourceFile
{
private:
	_SYNTHESIZE_INHER(char*, m_pKey, Key);
	_SYNTHESIZE_REF_INHER( float, m_fAniSpeed, AniSpeed)

public:
	virtual	void	Render(cObjBase* pObj, cBaseShader* pShader = NULL, bool IsAni = true) PURE;
public:
	cResourceFile();
	virtual ~cResourceFile();
};

