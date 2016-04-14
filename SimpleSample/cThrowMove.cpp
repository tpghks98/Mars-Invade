#include "DXUT.h"
#include "cThrowMove.h"
#include "cMoveableObj.h"
#include "cObjectMgr.h"
#include "cSystemMgr.h"
#include "cPlayer.h"
#include "cTimer.h"

cThrowMove::cThrowMove()
: m_vMoveDir( g_vZero)
, m_pObjOwner( NULL )
{
}


cThrowMove::~cThrowMove()
{
}


void	cThrowMove::MovePattern(const D3DXVECTOR3& vDir, cMoveableObj* pObj)
{
	D3DXVECTOR3 vPos = pObj->GetPosition();

	m_pObjOwner = pObj;

	if (_GETSINGLE(cObjectMgr)->GetPlayer() != NULL)
	{
		if (pObj->GetPosition().z > 70.0f)
		{
			vPos += vDir * pObj->GetMoveSpeed() * _GETSINGLE(cSystemMgr)->GetDeltaTime();
		}
		else
		{
			if (m_vMoveDir == g_vZero)
			{
				MoveDirReset();
			}
			vPos += m_vMoveDir * pObj->GetMoveSpeed() * _GETSINGLE(cSystemMgr)->GetDeltaTime();
		}
	}

	pObj->SetPosition(vPos);
}

void	cThrowMove::MoveDirReset()
{
	m_vMoveDir = D3DXVECTOR3(rand() % 1000 * 0.001f - 0.5f, 0, rand() % 1000 * 0.001f - 0.5f);

	if (m_pObjOwner != NULL)
	{
		if (m_pObjOwner->GetPosition().x < -10.0f)
		{
			m_vMoveDir.x = 1.0f;
		}
		else if (m_pObjOwner->GetPosition().x > 10.0f)
		{
			m_vMoveDir.x = -1.0f;
		}
	}
	D3DXVec3Normalize(&m_vMoveDir, &m_vMoveDir);

	_GETS(cObjectMgr)->CreateTimer(this)->Setup(this, 0.4f, false, std::bind(&cThrowMove::MoveDirReset, this) );
}
