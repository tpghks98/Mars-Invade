#include "DXUT.h"
#include "cObjBase.h"
#include "cObjectMgr.h"
#include "cResourceFile.h"
#include "cResourceMgr.h"
#include "cSystemMgr.h"
#include "cCamera.h"

cObjBase::cObjBase()
:m_vPosition( g_vZero )
, m_vRotation( g_vZero)
, m_vScale( g_vOne )
, m_fAniKey( 0.0f )
, m_pResource( NULL )
, m_fAlpha( 1.0f )
, m_IsCollisionCheck( false )
, m_fHitcolor( 1.0f )
, m_fRecoveryTime( 32.0f)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cObjBase::~cObjBase()
{
}


void	cObjBase::Translation()
{
	D3DXMATRIXA16	matScale, matRot, matTrans;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	{
		D3DXQUATERNION	q;
		D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, m_vRotation.x, m_vRotation.z);
		D3DXMatrixRotationQuaternion(&matRot, &q);

	}
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = matScale	*	matRot	*	matTrans;
}


void	cObjBase::DrawShadow()
{
	_GETSINGLE(cObjectMgr)->ChangeShaodwBuf();
	m_pResource->Render(this
		, _GETSINGLE(cResourceMgr)->GetShader(SI_CREATE));
	_GETSINGLE(cObjectMgr)->ResetBuf();
}


void	cObjBase::OnHit(float fDamage)
{
	m_fHitcolor = 0.0f;
}

void	cObjBase::BaseUpdate()
{
	if (m_fHitcolor < 1.0f)
	{
		m_fHitcolor += _GETSINGLE(cSystemMgr)->GetDeltaTime() * m_fRecoveryTime;
		if (m_fHitcolor > 1.0f)
		{
			m_fHitcolor = 1.0f;
		}
	}
}
