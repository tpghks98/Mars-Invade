#include "DXUT.h"
#include "cGun.h"
#include "cBullet.h"
#include "shared.h"
#include "cResourceFile.h"
#include "cObjectMgr.h"
#include "cTimer.h"
#include "cPlayer.h"
#include "cMonster.h"
#include "cTypeInfo.h"
#include "cSubPlayer.h"

cGun::cGun()
:m_pObjOwner( NULL )
, m_IsCanShot( true) 
, m_fBulletDamage(0.0f )
, m_fBulletSpeed( 0.0f)
, m_fShotCooltime( 0.0f )
{
}


cGun::~cGun()
{
}



cBullet*	cGun::InstantiateBullet(const D3DXVECTOR3& vDir, const D3DXVECTOR3& vPos)
{
	if (m_pObjOwner == NULL)
	{
		return NULL;
	}

	cBullet*	pBullet = NULL;
	if (m_IsCanShot)
	{
		D3DXVECTOR3	vPosition = vPos;
		if (vPosition == g_vOne)
		{
			vPosition = m_pObjOwner->GetPosition() + vDir;
		}
		
		pBullet = (cBullet*)CreateObject<cBullet>(vPosition, m_pBulletResource->GetKey());
		 
		if (m_pBulletResource->GetKey() == "bullet")
		{
			pBullet->SetScale(pBullet->GetScale() * 0.04f);
		}
		else if (m_pBulletResource->GetKey() == "bullet_enemy1")
		{
			pBullet->SetScale(pBullet->GetScale() * 0.17f);
		}
		else
		{
			pBullet->SetScale(pBullet->GetScale() * 0.37f);
		}


		pBullet->SetDirection(vDir);
		pBullet->SetMoveSpeed(m_fBulletSpeed);
		pBullet->SetDamage(m_fBulletDamage);

	}
	return pBullet;
}

void	cGun::Reloading()
{
	m_IsCanShot = true;
}


void	cGun::BulletTypeSetup(cBullet* pBullet, cObjBase* pObj)
{
	if (typeid((*pObj)) == typeid(cPlayer) ||
		typeid((*pObj)) == typeid( cSubPlayer )) 
	{
		pBullet->TargetSetup(new cTypeInfo(typeid(cMonster)));
	}
	else if (typeid(*pObj) == typeid(cMonster))
	{
		pBullet->TargetSetup(new cTypeInfo(typeid(cPlayer)));
	}
}

void	cGun::SetCoolTime()
{
	if (m_fShotCooltime != 0.0f)
	{
		m_IsCanShot = false;
		auto pTimer = _GETSINGLE(cObjectMgr)->CreateTimer(this);
		pTimer->Setup(this, m_fShotCooltime, false, std::bind(&cGun::Reloading, this));
	}
}

