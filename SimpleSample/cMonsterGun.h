#pragma once
#include "cGun.h"
class cMonsterGun :
	public cGun
{
private:
	_SYNTHESIZE_INHER(int, m_nCount, Count);
public:
	virtual	void	Shot(const D3DXVECTOR3& vDir, cObjBase* pObj);
public:
	cMonsterGun();
	~cMonsterGun();
};

