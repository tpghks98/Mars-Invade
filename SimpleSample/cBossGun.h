#pragma once
#include "cGun.h"
class cBossGun :
	public cGun
{
private:
	_SYNTHESIZE_REF_INHER(float, m_fRotPlus, RotPlus);
	_SYNTHESIZE_INHER(int, m_nCount, Count);
	_SYNTHESIZE_INHER(int, m_nMaxCount, MaxCount);
	_SYNTHESIZE_INHER(int, m_nBossPattern, BossPattern);
	_SYNTHESIZE_INHER(bool, m_IsHalfPlus, HalfPlus);
public:
	virtual	void	Shot(const D3DXVECTOR3& vDir, cObjBase* pObj);
public:
	cBossGun();
	~cBossGun();
};

