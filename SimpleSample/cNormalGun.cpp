#include "DXUT.h"
#include "cNormalGun.h"
#include "cResourceMgr.h"
#include "cBullet.h"
#include "cObjectMgr.h"
#include "cSystemMgr.h"

cNormalGun::cNormalGun(cObjBase* pObjOwner)
{
	m_pObjOwner = pObjOwner;
	m_fBulletSpeed = 220.0f;
	m_fBulletDamage = 1.0f;
	m_fShotCooltime = 0.1f;
	m_pBulletResource = _GETSINGLE(cResourceMgr)->GetResource("bullet");
}


cNormalGun::~cNormalGun()
{
}


void	cNormalGun::Shot(const D3DXVECTOR3& vDir, cObjBase* pObj)
{
	if (m_IsCanShot)
	{
		_GETS(cSystemMgr)->CreateSound(L"Data/Sound/shot.wav", false);

		D3DXMATRIXA16	matRot;
		D3DXQUATERNION q;
		auto matWorld = pObj->GetWorldMatrix();
		D3DXVECTOR3	vObjPos = D3DXVECTOR3(matWorld._41, matWorld._42, matWorld._43);
		D3DXVECTOR3	vPos;

		for (int n = 0; n < 1; ++n)
		{
			D3DXQuaternionRotationAxis(&q, &_GETSINGLE( cObjectMgr)->GetWorldUp(), (n * 2 - 1) * D3DX_PI * 0.25f);
			D3DXMatrixRotationQuaternion(&matRot, &q);
//			D3DXVec3TransformCoord(&vPos, &vDir, &matRot);
			vPos = vObjPos;
			auto pBullet = InstantiateBullet(vDir ,vPos);
			if (pBullet != NULL)
			{
				this->BulletTypeSetup(pBullet, pObj);
			}
		}
		this->SetCoolTime();
	}

}
