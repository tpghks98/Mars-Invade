#pragma once

#include "cNode.h"
#include "value.h"
class cResourceFile;
class cObjBase;
class cBullet;
class cGun
	:public cNode
{
protected:
	_SYNTHESIZE_REF_INHER(float, m_fBulletSpeed, BulletSpeed);
	_SYNTHESIZE_REF_INHER(float, m_fShotCooltime, ShotCoolTime);
	_SYNTHESIZE_REF_INHER(float, m_fBulletDamage, BulletDamage);

	_SYNTHESIZE_INHER(bool, m_IsCanShot, IsCanShot);

	_SYNTHESIZE_INHER(cResourceFile*,	m_pBulletResource, BulletResource);
	_SYNTHESIZE_INHER(cObjBase*,	m_pObjOwner, OwnerObj);

public:
	virtual	void	Shot( const D3DXVECTOR3& vDir, cObjBase* pObj)	PURE;

	virtual	cBullet*	InstantiateBullet(const D3DXVECTOR3& vDir, const D3DXVECTOR3& vPos = g_vOne );

	void	Reloading();
protected:
	void	BulletTypeSetup(cBullet* pBullet, cObjBase* pObj);
	void	SetCoolTime();
public:
	cGun();
	virtual ~cGun();
};

