#pragma once
#include "cGun.h"
class cNormalGun :
	public cGun
{
public:
	virtual	void	Shot(const D3DXVECTOR3& vDir, cObjBase* pObj);


public:
	cNormalGun( cObjBase* pObjOwner);
	~cNormalGun();
};

