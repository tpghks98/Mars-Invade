#include "DXUT.h"
#include "cMonster.h"
#include "cResourceMgr.h"
#include "cResourceFile.h"
#include "cObjectMgr.h"
#include "cItem.h"
#include "shared.h"
#include "cPlayer.h"
#include "cSideMove.h"
#include "cAseFile.h"
#include "cSystemMgr.h"
#include "cEffect.h"
#include "cNormalGun.h"
#include "cMonsterGun.h"
#include "cThrowMove.h"
#include "cBossGun.h"
#include "cTimer.h"

cMonster::cMonster()
:m_pMovePattern(NULL)
, m_vLocalPos( 0, 0,0 )
, m_IsDivide( false )
, m_vOriScale( g_vZero)
, m_vShotDir( g_vZero )
, m_pGun( NULL )
, m_IsFly( true )
, m_IsDie( false )
{
}


cMonster::~cMonster()
{
	SAFE_DELETE(m_pGun);
	SAFE_DELETE(m_pMovePattern);
	for (int n = 0; n < 2; ++n)
	{
		CreateObject<cEffect>(m_vPosition + D3DXVECTOR3(rand() % 1000 * 0.001f - 0.5f, rand() % 1000 * 0.001f - 0.5f, 0 ) * 5, "effect_bomb");
	}

	_GETS(cSystemMgr)->Shaking(0.1f, 0.0f, false);
	if (m_IsDivide)
	{
		for (int n = 0; n < 2; ++n)
		{
			auto pObj = (cMonster*)
				CreateObject<cMonster>(m_vPosition + D3DXVECTOR3((n * 2 - 1) * -3, 0, 0), m_pResource->GetKey());
			pObj->SetScale(m_vScale * 0.25f);
			pObj->SetMovePattern(new cThrowMove());
			pObj->SetGun(new cMonsterGun());
	
		}
	}

	if ( !( rand() % 5 ))
	{
		cItem* pItem = (cItem*)CreateObject<cItem>(m_vPosition);
		pItem->SetID(rand() % IT_MAX);
		pItem->Setup();
	}
	_GETS(cObjectMgr)->SetScore(_GETS(cObjectMgr)->GetScore() + 10);
	
}


void	cMonster::Initialize()
{

	m_vRotation.y = D3DX_PI;
	m_vRotation.z = D3DX_PI;
	m_vRotation.x += D3DX_PI * 0.15f;

	m_IsCollisionCheck = true;
	m_vScale *= 0.1f;
	m_vDirection = D3DXVECTOR3(0, 0, -1);
	m_vShotDir = D3DXVECTOR3(0, 0, -1);
	m_fMoveSpeed = 35.0f;
	m_fHp = 5.0f;
	m_IsLookAtRot = true;
	m_IsSmoothRot = true;
	m_vPosition.y = 30.0f;
	
//	this->MoveDirection();
}
void	cMonster::Update()
{
	if (m_vOriScale == g_vZero)
	{
		m_vOriScale = m_vScale;
	}
	auto pPlayer = _GETSINGLE(cObjectMgr)->GetPlayer();


	if (pPlayer != NULL)
	{
		m_vDirection = pPlayer->GetPosition() - m_vPosition;
		m_vDirection = D3DXVECTOR3(0, 0, -1);
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
	}


	if (m_pMovePattern != NULL)
	{
		m_pMovePattern->MovePattern(m_vDirection, this);
	}
	else
	{
		m_vDirection = D3DXVECTOR3(0, 0, -1);
		this->MoveDirection();
	}
	
	m_fMoveSpeed -= m_fMoveSpeed * 0.25f * _GETS(cSystemMgr)->GetDeltaTime();
	m_vScale += m_vOriScale * _GETSINGLE(cSystemMgr)->GetDeltaTime();
	if (m_vScale.x > m_vOriScale.x * 2)
	{
		m_vScale = m_vOriScale * 2;
	}
	if (m_fMoveSpeed < 20.0f)
	{
		m_fMoveSpeed = 20.0f;
	}
	if (m_vPosition.z > 45.0f)
	{
		m_vPosition.z -= (m_vPosition.z - 45.0f) * 3 * _GETSINGLE(cSystemMgr)->GetDeltaTime();
	}
	else if (m_vPosition.z < -30.0f)
	{
		m_IsDelete = true;
	}


	this->BaseUpdate();

	if (!m_IsFly)
	{
		m_vPosition.y = 5.0f;
	}

	this->Translation();
	if (!m_IsFly)
	{
		m_vPosition.y = 30.0f;
	}
	if (m_pGun != NULL)
	{
		//if (m_pGun->GetIsCanShot())
		//{
		//	D3DXMATRIXA16	matRot;
		//	D3DXMatrixRotationY(&matRot, D3DX_PI * 0.05f);
		//	D3DXVec3TransformCoord(&m_vShotDir, &m_vShotDir, &matRot);
		//}
		if (_GETS(cSystemMgr)->IsEmptyCameraList() )
			m_pGun->Shot(m_vShotDir, this);
	}
}
void	cMonster::Render()
{
	this->DrawShadow();
	m_pResource->Render(this, _GETSINGLE(cResourceMgr)->GetShader(SI_CUSTOM));
}

void	cMonster::DestoryObject()
{
	if (m_pGun != NULL)
	{
		if (typeid(*m_pGun) == typeid(cBossGun))
		{	
			if (!m_IsDie)
			{
				m_IsDie = true;
				_GETS(cObjectMgr)->CreateTimer(this)->Setup(this, 0.05f, true, std::bind(&cMonster::OnDieEffect, this));
				_GETS(cObjectMgr)->CreateTimer(this)->Setup(this, 1.0f, false, std::bind(&cMonster::OnDie, this));
			}
		}
		else
		{
			m_IsDelete = true;
		}
	}
	else
	{
		m_IsDelete = true;
	}
}

void	cMonster::OnDieEffect()
{
	auto pEf = (cEffect*)
	CreateObject<cEffect>(m_vPosition + D3DXVECTOR3(rand() % 1000 * 0.001f - 0.5f, rand() % 1000 * 0.001f - 0.5f, -0.3f) * 10, "effect_bomb");
	pEf->SetIsZEnable( false );
}
void	cMonster::OnDie()
{
	m_IsDelete = true;
}