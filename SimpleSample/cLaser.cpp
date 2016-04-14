#include "DXUT.h"
#include "cLaser.h"
#include "cResourceFile.h"
#include "cResourceMgr.h"
#include "cSystemMgr.h"
#include "cObjectMgr.h"
#include "cMonster.h"
#include "cTimer.h"

cLaser::cLaser()
:m_fPlusGauge( 1.0f )
, m_IsReverse( false )
{
	D3DXQuaternionIdentity(&m_quatRot);
}


cLaser::~cLaser()
{
}

void	cLaser::Initialize()
{
	m_vDirection = D3DXVECTOR3(0, 0, 1);
	m_pResource = _GETSINGLE(cResourceMgr)->GetResource("effect_laser");
	m_vScale.x = 0.0f;
	m_vScale.z = 0.0f;

	m_vScale *= 0.05f;
	m_vScale.x *= 0.5f;
	
	m_IsCollisionCheck = true;
}
void	cLaser::Update()
{
	m_fAlpha = 0.75f;
	this->Translation();
}
void	cLaser::Render()
{
	_GETSINGLE(cObjectMgr)->SetIsMap(true);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_ZWRITEENABLE, false);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	D3DXVECTOR3	vOriPos = m_vPosition;
	m_vPosition.x -= 0.75f;

	this->Translation();
	this->DrawShadow();
	m_pResource->Render(this, _GETSINGLE( cResourceMgr )->GetShader( SI_CUSTOM ));

	m_vPosition.x += 1.5f;

	this->Translation();
	this->DrawShadow();
	m_pResource->Render(this, _GETSINGLE(cResourceMgr)->GetShader(SI_CUSTOM));

	m_vPosition = vOriPos;

	DXUTGetD3D9Device()->SetRenderState(D3DRS_ZWRITEENABLE, true);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	_GETSINGLE(cObjectMgr)->SetIsMap(false);
}
void	cLaser::OnCollision(cObjBase* pObj)
{
	if (typeid(*pObj) == typeid(cMonster))
	{
		if (abs(pObj->GetPosition().x - m_vPosition.x ) < 3.0f )
		{
			pObj->OnHit(1.0f);
		}
	}
}


void	cLaser::OnActive()
{
	m_vScale.z +=  _GETS(cSystemMgr)->GetDeltaTime() * 1.2f * m_fPlusGauge;
	m_vScale.x +=  _GETS(cSystemMgr)->GetDeltaTime() * 0.08f * m_fPlusGauge;
	m_fAlpha += m_fPlusGauge * _GETS(cSystemMgr)->GetDeltaTime();

	if (m_vScale.z > 0.6f)
	{
		m_vScale.z = 0.6;
		if (!m_IsReverse)
		{
			m_IsReverse = true;
			_GETS(cObjectMgr)->CreateTimer(this)->Setup(this, 0.75f, false, std::bind(&cLaser::OnReverseGauge, this));
		}
	}
	else if (m_vScale.z < 0.0f)
	{
		m_fPlusGauge = 1.0f;
		m_IsReverse = false;
	}
	if (m_vScale.x > 0.04f)
	{
		m_vScale.x = 0.04f;
	}
	else if (m_vScale.x < 0.0f)
	{
		m_vScale.x = 0.0f;
	}

	if (m_fAlpha < 0.3f)
	{
		m_fAlpha = 0.3f;
	}
	else if (m_fAlpha >1.0f)
	{
		m_fAlpha = 1.0f;
	}
}

void	cLaser::OnReverseGauge()
{
	m_fPlusGauge = -1.0f;
}
void	cLaser::Translation()
{
	D3DXMATRIXA16	matScale, matRot, matTrans, matQuat;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	{
		D3DXQUATERNION	q;
		D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, m_vRotation.x, m_vRotation.z);
		D3DXMatrixRotationQuaternion(&matRot, &q);

	}
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	D3DXMatrixRotationQuaternion(&matQuat, &m_quatRot);

	m_matWorld = matScale	*	matRot	* matQuat*	matTrans;
}
