#pragma once
#include "cGun.h"
class cFastGun :
	public cGun
{
public:
	virtual	void	Shot(const D3DXVECTOR3& vDir, cObjBase* pObj);
public:
	cFastGun(cObjBase* pObjOwner);
	~cFastGun();
};

