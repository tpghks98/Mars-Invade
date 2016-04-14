#include "DXUT.h"
#include "cRushMove.h"
#include "cSystemMgr.h"
#include "cMoveableObj.h"
#include "cObjectMgr.h"
#include "cPlayer.h"
#include "cTimer.h"

cRushMove::cRushMove()
:m_vDirection( g_vZero)
, m_fTime( 0.0f )
, m_fPlusY( 1.0f )
{
	_GETS(cObjectMgr)->CreateTimer(this)->Setup(this, 0.6f, true, std::bind(&cRushMove::OnReverseY, this));
}


cRushMove::~cRushMove()
{
}



void	cRushMove::MovePattern(const D3DXVECTOR3& vDir, cMoveableObj* pObj)
{
	D3DXVECTOR3 vPos = pObj->GetPosition();
	vPos += vDir * pObj->GetMoveSpeed() * _GETSINGLE(cSystemMgr)->GetDeltaTime();



	if (m_vDirection == g_vZero)
	{
		if (_GETSINGLE(cObjectMgr)->GetPlayer() != NULL)
		{
			auto pPlayer = _GETSINGLE(cObjectMgr)->GetPlayer();
			D3DXVECTOR3 vDist = pPlayer->GetPosition() - vPos;
			if (D3DXVec3Length(&vDist) < 40.0f)
			{
				m_fTime += _GETS(cSystemMgr)->GetDeltaTime();
				if (m_fTime > 0.6f)
				{
					m_vDirection = pPlayer->GetPosition() - vPos;
					D3DXVec3Normalize(&m_vDirection, &m_vDirection);
				}
			}
		}
	}
	else
	{
		pObj->SetDirection(m_vDirection);
		vPos += m_vDirection * _GETSINGLE(cSystemMgr)->GetDeltaTime() * 150.0f;
	}
	vPos.y += m_fPlusY * _GETS(cSystemMgr)->GetDeltaTime() * 2.0f;
	pObj->SetPosition(vPos);
}

void		cRushMove::OnReverseY()
{
	m_fPlusY *= -1.0f;
}
