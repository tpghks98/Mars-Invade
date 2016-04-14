#pragma once
#include "cGun.h"
class cThreeWayGun :
	public cGun
{
public:
	virtual	void	Shot(const D3DXVECTOR3& vDir, cObjBase* pObj);
public:
	cThreeWayGun(cObjBase* pObjOwner);
	~cThreeWayGun();
};

