#include "DXUT.h"
#include "cMoveableObj.h"
#include "cSystemMgr.h"
#include "cObjectMgr.h"
#include "cCamera.h"
#include "shared.h"
#include "cEffect.h"

cMoveableObj::cMoveableObj()
:m_vDirection( g_vZero )
, m_fMoveSpeed( 0.1f )
, m_fMoveLength(0.0f)
, m_IsLookAtRot( false )
, m_IsSmoothRot( false )
, m_fHp( 10.0f)
, m_vUp(0, 1, 0)
, m_fMaxHp( 10.0f )
{
	D3DXQuaternionIdentity(&m_quatPrevLook);
}


cMoveableObj::~cMoveableObj()
{
}


void	cMoveableObj::Translation()
{
	D3DXMATRIXA16	matScale, matRot, matTrans,	matDir;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);

	{
		D3DXQUATERNION	q;
		D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, m_vRotation.x, m_vRotation.z);
		D3DXMatrixRotationQuaternion(&matRot, &q);
	}
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	D3DXMatrixIdentity(&matDir);

	if ( m_IsLookAtRot )
	{
		LookAtDirection(&matDir);
		if (m_IsSmoothRot)
		{
			D3DXQUATERNION	q;
			D3DXQuaternionRotationMatrix(&q, &matDir);

			float fTime = 5.0f  * _GETSINGLE(cSystemMgr)->GetDeltaTime();
			if (fTime > 1.0f)
			{
				fTime = 1.0f;
			}
			D3DXQuaternionSlerp(&m_quatPrevLook, &m_quatPrevLook, &q, fTime );
			D3DXMatrixRotationQuaternion(&matDir, &m_quatPrevLook);
		}
	}

	m_matWorld = matScale	*	matRot	* matDir *	matTrans;
}


void	cMoveableObj::LookAtDirection(D3DXMATRIXA16* pOut)
{
	D3DXMatrixIdentity(pOut);

	if (m_vDirection == g_vZero)
	{
		return;
	}

	D3DXVec3Normalize(&m_vDirection, &m_vDirection);

	D3DXVECTOR3	 vLRDir;

	D3DXVec3Cross(&vLRDir, &m_vDirection, &m_vUp);
	D3DXVec3Normalize(&vLRDir, &vLRDir);

	memcpy(&pOut->_11, &vLRDir, sizeof(D3DXVECTOR3));
	memcpy(&pOut->_21, &-m_vUp, sizeof(D3DXVECTOR3));
	memcpy(&pOut->_31, &( D3DXVECTOR3( 0, 0.5f, 0 ) +m_vDirection ), sizeof(D3DXVECTOR3));

}

void	cMoveableObj::MoveDirection(D3DXVECTOR3* vPosition, float fSpeed )
{
	m_vPosition += m_vDirection * m_fMoveSpeed * _GETS(cSystemMgr)->GetDeltaTime();

}


void	cMoveableObj::FixHeight()
{

	float fHeight = FLT_MAX;
	_GETSINGLE(cObjectMgr)->IsMapCollisionCheck(this, &fHeight);

	if (fHeight != FLT_MAX)
	{
		m_vPosition.y = fHeight;
	}

}
void	cMoveableObj::OnHit(float fDamage)
{
	if (m_fHitcolor >= 1.0f)
	{

		cObjBase::OnHit(fDamage);
		CreateObject<cEffect>(m_vPosition, "effect_bullet");

		m_fHp -= fDamage;
		if (m_fHp <= 0.0f)
		{
			m_fHp = 0.0f;
			DestoryObject();
		}
	}
}
