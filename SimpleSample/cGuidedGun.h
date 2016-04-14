#pragma once
#include "cGun.h"
class cObjBase;
class cGuidedGun :
	public cGun
{
public:
	virtual	void	Shot(const D3DXVECTOR3& vDir, cObjBase* pObj);
public:
	cGuidedGun( cObjBase* pObj);
	~cGuidedGun();
};

