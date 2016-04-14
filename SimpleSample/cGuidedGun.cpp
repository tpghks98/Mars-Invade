#include "DXUT.h"
#include "cGuidedGun.h"
#include "cResourceMgr.h"
#include "cObjBase.h"
#include "cBullet.h"

cGuidedGun::cGuidedGun(cObjBase* pObj)
{
	m_pObjOwner = pObj;
	m_fBulletSpeed = 200.0f;
	m_fBulletDamage = 1.5f;
	m_fShotCooltime = 0.05f;
	m_pBulletResource = _GETSINGLE(cResourceMgr)->GetResource("bullet");
}


cGuidedGun::~cGuidedGun()
{
}

void	cGuidedGun::Shot(const D3DXVECTOR3& vDir, cObjBase* pObj)
{
	if (m_IsCanShot)
	{
		D3DXMATRIXA16	matRot;
		D3DXQUATERNION q;
		auto matWorld = pObj->GetWorldMatrix();
		D3DXVECTOR3	vObjPos = D3DXVECTOR3(matWorld._41, matWorld._42, matWorld._43);
		D3DXVECTOR3	vDirection;

		float fPower = D3DX_PI * 0.9f * (rand() % 1000 * 0.001f - 0.5f );
		if (fPower < 0.0f)
		{
			fPower += -D3DX_PI * 0.1f;
		}
		else
		{
			fPower += D3DX_PI * 0.1f;
		}
		 
		D3DXQuaternionRotationYawPitchRoll( &q, fPower, D3DX_PI * 0.1f * ( rand() % 1000 * 0.001f - 0.5f ) , 0 );
		D3DXMatrixRotationQuaternion(&matRot, &q);

		D3DXVec3TransformCoord(&vDirection, &vDir, &matRot);

		D3DXVECTOR3	vPos = vObjPos + vDirection + D3DXVECTOR3(rand() % 1000 * 0.001f - 0.5f, rand() % 1000 * 0.001f - 0.5f, 0 );

		auto pBullet = InstantiateBullet(vDirection, vPos);
		if (pBullet != NULL)
		{
			this->BulletTypeSetup(pBullet, pObj);
			this->SetCoolTime();
			pBullet->SetGuidShot(true);
		}
	}
}