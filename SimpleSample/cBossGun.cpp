#include "DXUT.h"
#include "cBossGun.h"
#include "cObjBase.h"
#include "cPlayer.h"
#include "cObjectMgr.h"
#include "cResourceMgr.h"

cBossGun::cBossGun()
:m_nCount( 0 )
, m_fRotPlus( D3DX_PI / 16.0f )
, m_nMaxCount( 64 )
, m_nBossPattern( BS_SLOW_ROT )
, m_IsHalfPlus( false )
{
	m_fBulletSpeed = 55.0f;
	m_fBulletDamage = 1.0f;
	m_fShotCooltime = 1.5f;
	m_pBulletResource = _GETSINGLE(cResourceMgr)->GetResource("bullet_enemy1");
}


cBossGun::~cBossGun()
{
	_GETSINGLE(cObjectMgr)->SetBoss(NULL);
}


void	cBossGun::Shot(const D3DXVECTOR3& vDir, cObjBase* pObj)
{
	if (pObj->GetPosition().z < 80.0f)
	{
		if (m_IsCanShot)
		{
			if (m_nCount == 0)
			{
				m_nBossPattern = rand() % BS_MAX;
				m_IsHalfPlus = rand() % 2;
				if (m_nMaxCount <= 8)
				{
					if (m_nBossPattern == BS_SLOW_ROT ||
						BS_FAST_ROT)
					{
						m_nBossPattern = BS_FRONT_ROT;
					}
				}
			}


			m_pObjOwner = pObj;

			auto matWorld = pObj->GetWorldMatrix();
			D3DXVECTOR3	vObjPos = D3DXVECTOR3(matWorld._41, matWorld._42, matWorld._43);
			vObjPos.y = 30.0f;

			int nShotCount = 1;
			bool IsFront = false;

			{
				switch (m_nBossPattern)
				{
				case BS_SLOW_ROT:
					m_fShotCooltime = 0.01f;
					break;
				case BS_FAST_ROT:
					m_fShotCooltime = rand() % 1000 * 0.001f + 1.0f;
					nShotCount = m_nMaxCount;
					break;
				case BS_FRONT_ROT:
					IsFront = true;
					m_fShotCooltime = 0.02f;
					break;

				}
			}
			D3DXVECTOR3	vDirection = vDir;
			D3DXMATRIXA16	matRot;
			for (int n = 0; n < nShotCount; ++n)
			{
				float f = m_fRotPlus * m_nCount;
				while (f > D3DX_PI * 0.5f)
				{
					f -= D3DX_PI;
				}
				if (m_IsHalfPlus)
				{
					f += m_fRotPlus * 0.5f;
				}

				D3DXMatrixRotationY(&matRot, f);
				D3DXVec3TransformCoord(&vDirection, &vDirection, &matRot);

				m_nCount++;
				auto pBullet = InstantiateBullet(vDirection, vObjPos);
				if (pBullet != NULL)
				{
					this->BulletTypeSetup(pBullet, pObj);
				}
			}
			if (m_nCount > m_nMaxCount)
			{
				if (m_fShotCooltime < 0.1f)
				{
					m_fShotCooltime = rand() % 1000 * 0.001f * 0.5f + 0.5f;
				}
				m_nCount = 0;
			}

			this->SetCoolTime();
		}
	}
}
