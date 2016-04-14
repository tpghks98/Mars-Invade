#include "DXUT.h"
#include "cFastGun.h"
#include "cResourceMgr.h"
#include "cObjBase.h"
#include "cObjectMgr.h"
#include "cSystemMgr.h"

cFastGun::cFastGun(cObjBase* pObjOwner)
{
	m_pObjOwner = pObjOwner;
	m_fBulletSpeed = 220.0f;
	m_fBulletDamage = 1.5f;
	m_fShotCooltime = 0.05f;
	m_pBulletResource = _GETSINGLE(cResourceMgr)->GetResource("bullet");
}


cFastGun::~cFastGun()
{
	m_pObjOwner = NULL;
}

void	cFastGun::Shot(const D3DXVECTOR3& vDir, cObjBase* pObj)
{
	if (m_IsCanShot)
	{
		_GETS(cSystemMgr)->CreateSound(L"Data/Sound/shot.wav", false);

		D3DXMATRIXA16	matRot;
		D3DXQUATERNION q;
		auto matWorld = pObj->GetWorldMatrix();
		D3DXVECTOR3	vObjPos = D3DXVECTOR3(matWorld._41, matWorld._42, matWorld._43);
		D3DXVECTOR3	vPos;

		for (int n = 0; n < 2; ++n)
		{
			D3DXQuaternionRotationAxis(&q, &_GETSINGLE(cObjectMgr)->GetWorldUp(), (n * 2 - 1) * D3DX_PI * 0.25f);
			D3DXMatrixRotationQuaternion(&matRot, &q);
			D3DXVec3TransformCoord(&vPos, &vDir, &matRot);
			vPos += vObjPos;
			auto pBullet = InstantiateBullet(vDir, vPos);
			if (pBullet != NULL)
			{
				this->BulletTypeSetup(pBullet, pObj);
			}
		}
		this->SetCoolTime();
	}

}
