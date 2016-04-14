#include "DXUT.h"
#include "cLaserGun.h"
#include "cLaser.h"
#include "shared.h"
#include "cPlayer.h"

cLaserGun::cLaserGun()
:m_pLaser( NULL )
{
}


cLaserGun::~cLaserGun()
{
	if (m_pLaser != NULL)
	{
		m_pLaser->SetDelete(true);
	}
}
void	cLaserGun::Shot(const D3DXVECTOR3& vDir, cObjBase* pObj)
{
	if (m_pLaser == NULL)
	{
		m_pLaser = (cLaser*)CreateObject<cLaser>(pObj->GetPosition());
	}

	auto Matrix = pObj->GetWorldMatrix();
	m_pLaser->SetPosition(D3DXVECTOR3(Matrix._41, Matrix._42, Matrix._43) - vDir * 1.5f);
	D3DXQUATERNION	q;
	D3DXQuaternionRotationMatrix(&q, &Matrix);
	m_pLaser->SetquatRot(q);
	m_pLaser->OnActive();

	if (typeid(*pObj) == typeid(cPlayer))
	{
		D3DXVECTOR3	v = ((cPlayer*)pObj)->GetLocalRot();
		v.x = 0.0f;

		m_pLaser->SetRotation( v);
	}
}

