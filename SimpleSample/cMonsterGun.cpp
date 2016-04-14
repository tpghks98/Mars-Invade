#include "DXUT.h"
#include "cMonsterGun.h"
#include "cResourceMgr.h"
#include "cObjectMgr.h"
#include "cPlayer.h"

cMonsterGun::cMonsterGun()
:m_nCount( 0 )
{
	m_fBulletSpeed = 55.0f;
	m_fBulletDamage = 1.0f;
	m_fShotCooltime = 1.5f;
	m_pBulletResource = _GETSINGLE(cResourceMgr)->GetResource("bullet_enemy1");
}


cMonsterGun::~cMonsterGun()
{
}

void	cMonsterGun::Shot(const D3DXVECTOR3& vDir, cObjBase* pObj)
{
	if (pObj->GetPosition().z < 70.0f)
	{
		if (m_IsCanShot)
		{
			m_pObjOwner = pObj;

			auto matWorld = pObj->GetWorldMatrix();
			D3DXVECTOR3	vObjPos = D3DXVECTOR3(matWorld._41, matWorld._42, matWorld._43);

			D3DXVECTOR3	vDirection = vDir;
			if (_GETS(cObjectMgr)->GetPlayer() != NULL)
			{
				vDirection = _GETS(cObjectMgr)->GetPlayer()->GetPosition()- pObj->GetPosition();
				D3DXVec3Normalize(&vDirection, &vDirection);
				D3DXMATRIXA16	matRot;
				float fRand = (rand() % 1000 * 0.001f - 0.5f) * D3DX_PI * 0.25f;
				D3DXMatrixRotationY(&matRot, fRand);
				D3DXVec3TransformCoord(&vDirection, &vDirection, &matRot);
				vDirection.y = 0.0f;
			}

			m_nCount++;
			if (m_nCount > 1)
			{
				m_fShotCooltime = 2.0f;
				m_nCount = 0;
			}
			else
			{
				m_fShotCooltime = 0.1f;
			}

			auto pBullet = InstantiateBullet(vDirection, vObjPos);
			
			if (pBullet != NULL)
			{
				this->BulletTypeSetup(pBullet, pObj);
			}

			this->SetCoolTime();
		}
	}
}