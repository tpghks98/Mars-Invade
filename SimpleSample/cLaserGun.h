#pragma once
#include "cGun.h"
#include "value.h"

class cLaser;
class cLaserGun :
	public cGun
{
private:
	_SYNTHESIZE_INHER(cLaser*, m_pLaser, Laser);
public:
	virtual	void	Shot(const D3DXVECTOR3& vDir, cObjBase* pObj);
public:
	cLaserGun();
	~cLaserGun();
};

