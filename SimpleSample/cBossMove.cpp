#include "DXUT.h"
#include "cBossMove.h"
#include "cMoveableObj.h"
#include "cResourceMgr.h"
#include "cSystemMgr.h"
#include  "cObjectMgr.h"
#include "cMapGenerate.h"
#include "cTimer.h"
#include "cObjBase.h"
#include "cMonster.h"

cBossMove::cBossMove()
:m_pObjOwner( NULL )
, m_vMoveDir( g_vZero )
{
}


cBossMove::~cBossMove()
{
	_GETSINGLE(cObjectMgr)->GetMapGenerate()->SetIsScroll(true);
}


void	cBossMove::MovePattern(const D3DXVECTOR3& vDir, cMoveableObj* pObj)
{
	m_pObjOwner = pObj;
	D3DXVECTOR3 vPos = pObj->GetPosition();


	if (pObj->GetPosition().z > 60.0f)
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

		_GETSINGLE(cObjectMgr)->GetMapGenerate()->SetIsScroll(false);

		auto vRot = m_pObjOwner->GetRotation();
		vRot += D3DXVECTOR3(0, 0, -m_vMoveDir.x * D3DX_PI * 0.2f) * _GETSINGLE(cSystemMgr)->GetDeltaTime();
		if (vRot.z > D3DX_PI * 1.1f)
		{
			vRot.z = D3DX_PI * 1.1f;
		}
		else if (vRot.z < D3DX_PI * 0.9f)
		{
			vRot.z = D3DX_PI * 0.9f;
		}
		pObj->SetRotation(vRot);
	}
	pObj->SetPosition(vPos);
}
void	cBossMove::MoveDirReset()
{

	if (m_pObjOwner != NULL && 
		typeid(*m_pObjOwner) == typeid(cMonster)
		&& ((cMonster*)m_pObjOwner)->GetIsFly())
	{
		m_vMoveDir = D3DXVECTOR3(rand() % 1000 * 0.001f - 0.5f, 0, rand() % 1000 * 0.001f - 0.5f);

		if (m_pObjOwner->GetPosition().x < -4.0f)
		{
			m_vMoveDir.x = 1.0f;
		}
		else if (m_pObjOwner->GetPosition().x > 4.0f)
		{
			m_vMoveDir.x = -1.0f;
		}
		if (m_pObjOwner->GetPosition().z < 40.0f)
		{
			m_vMoveDir.z = 1.0f;
		}
		else if (m_pObjOwner->GetPosition().z > 60.0f)
		{
			m_vMoveDir.z = -1.0f;
		}

		D3DXVec3Normalize(&m_vMoveDir, &m_vMoveDir);
		_GETS(cObjectMgr)->CreateTimer(this)->Setup(this, 0.3f, false, std::bind(&cBossMove::MoveDirReset, this));

	}

}
