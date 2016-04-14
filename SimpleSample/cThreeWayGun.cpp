#include "DXUT.h"
#include "cThreeWayGun.h"
#include "cResourceMgr.h"
#include "cObjectMgr.h"
#include "cObjBase.h"

cThreeWayGun::cThreeWayGun(cObjBase* pObjOwner)
{
	m_pObjOwner = pObjOwner;
	m_fBulletSpeed = 220.0f;
	m_fBulletDamage = 1.5f;
	m_fShotCooltime = 0.075f;
	m_pBulletResource = _GETSINGLE(cResourceMgr)->GetResource("bullet");
}


cThreeWayGun::~cThreeWayGun()
{
}


void	cThreeWayGun::Shot(const D3DXVECTOR3& vDir, cObjBase* pObj)
{
	if (m_IsCanShot)
	{
		D3DXVECTOR3	vBulletDir;
		D3DXMATRIXA16	matRot;
		auto matWorld = pObj->GetWorldMatrix();
		D3DXVECTOR3	vObjPos = D3DXVECTOR3(matWorld._41, matWorld._42, matWorld._43);
		for (int n = 0; n < 3; ++n)
		{
			m_IsCanShot = true;
			D3DXQUATERNION q;
			D3DXQuaternionRotationAxis(&q, &_GETSINGLE(cObjectMgr)->GetWorldUp(), (n - 1) * D3DX_PI * 0.015f);
			D3DXMatrixRotationQuaternion(&matRot, &q);


			D3DXVec3TransformCoord(&vBulletDir, &vDir, &matRot);
			auto pBullet = InstantiateBullet(vBulletDir, vObjPos);
			if (pBullet != NULL)
			{
				this->BulletTypeSetup(pBullet, pObj);
			}
		}
		this->SetCoolTime();
	}

}
