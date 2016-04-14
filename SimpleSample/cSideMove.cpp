#include "DXUT.h"
#include "cSideMove.h"
#include "cMoveableObj.h"
#include "cSystemMgr.h"

cSideMove::cSideMove()
:m_fRot( D3DX_PI * 0.5f)
, m_fPerSpeed( 1 )
, m_fPlusSpeed( -1 )
{
	if (rand() % 2)
	{
		m_fPerSpeed = 1;
	}
	else
	{
		m_fPerSpeed = -1;
	}
}


cSideMove::~cSideMove()
{
}


void	cSideMove::MovePattern(const D3DXVECTOR3& vDir, cMoveableObj* pObj)
{
	D3DXVECTOR3	vUp = pObj->GetUp();

	D3DXVECTOR3	vLRDir;
	D3DXMATRIXA16	matRot;
	D3DXQUATERNION	q;
	D3DXQuaternionRotationAxis(&q, &vUp, m_fRot);
	D3DXMatrixRotationQuaternion(&matRot, &q);

	D3DXVec3TransformCoord(&vLRDir, &vDir, &matRot);

	D3DXVECTOR3 vPos =		pObj->GetPosition();
	vPos += vLRDir * pObj->GetMoveSpeed()* 1.5f  * m_fPerSpeed * _GETSINGLE(cSystemMgr)->GetDeltaTime() ;

	
	vPos += vDir * pObj->GetMoveSpeed() * _GETSINGLE(cSystemMgr)->GetDeltaTime();

	pObj->SetDirection(vDir + vLRDir  * m_fPerSpeed);

	pObj->SetPosition(vPos);
	m_fPerSpeed += m_fPlusSpeed * _GETS(cSystemMgr)->GetDeltaTime();
	if (m_fPerSpeed < -1.0f)
	{
		m_fPlusSpeed = 1.0f;
	}
	else if ( m_fPerSpeed > 1.0f )
	{
		m_fPlusSpeed = -1.0f;
	}

}
