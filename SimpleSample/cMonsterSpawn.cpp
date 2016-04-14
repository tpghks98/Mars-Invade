#include "DXUT.h"
#include "cMonsterSpawn.h"
#include "cMonster.h"
#include "shared.h"
#include "cSideMove.h"
#include "cObjectMgr.h"
#include "cTimer.h"
#include "cRushMove.h"
#include "cMonsterGun.h"
#include "ThrowerMonster.h"
#include "cThrowMove.h"
#include "cBossMove.h"

cMonsterSpawn::cMonsterSpawn()
:m_vPlayerPosition( g_vZero )
, m_nCount( 0 )
, m_nMaxCount( 0 )
, m_IsThrow( false )
, m_fPer( 1.0f )
{
}


cMonsterSpawn::~cMonsterSpawn()
{
}


float cMonsterSpawn::GenerateMonster(const D3DXVECTOR3& vPlayerPosition)
{
	if (_GETSINGLE(cObjectMgr)->GetBoss() != NULL)
	{
		return 1.0f;
	}
	this->DeleteAllInherit();

	m_IsThrow = false;
	m_nCount = 0;
	m_nMaxCount = 1;
	m_vPlayerPosition = vPlayerPosition;
	float fPer = 0.7f;
	switch (_GETS(cObjectMgr)->GetBossCount())
	{
	case 0:
		fPer = 1.2f;
		break;
	case 1:
		fPer = 1.0f;
		break;
	case 2:
		fPer = 0.6f;
		break;
	case 3:
		fPer = 0.4f;
		break;
	}
	m_fPer = fPer * 0.6f;
	float fTime = 1.0f;
	switch ( rand() % 7 )
	{
	case 0:
		m_IsThrow = true;

		m_nMaxCount = 8;
		fTime = 7.5f;
		this->SideMonsterSpawn();
		_GETSINGLE(cObjectMgr)->CreateTimer(this)->Setup(this, 0.75f, true, std::bind(&cMonsterSpawn::SideMonsterSpawn , this));
		break;
	case 1:
		m_nMaxCount = 2;
		fTime = 2.4f;
		this->FrontGapMonsterSpawn( 2 , 15);
		break;
	case 2:
		m_nMaxCount = 3;
		fTime = 3.6f;
		this->FrontGapMonsterSpawn(3, 10);
		break;
	case 3:
		m_nMaxCount = 4;
		fTime = 4.8f;
		this->FrontGapMonsterSpawn(4, 7);
		break;
	case 4:
		m_nMaxCount = 2;
		fTime = 2.4f;
		m_vPlayerPosition.x = -10.0f;		
		this->SideMonsterSpawn();
		m_vPlayerPosition.x = 10.0f;
		this->SideMonsterSpawn();
		break;
	case 5:
		m_IsThrow = true;

		m_nMaxCount = 2;
		fTime = 2.4f;
		m_vPlayerPosition.x = -10.0f;
		this->SideMonsterSpawn();
		m_vPlayerPosition.x = 10.0f;
		this->SideMonsterSpawn();
		break;
	case 6:
		m_IsThrow = true;
		m_nMaxCount = 4;
		fTime = 4.8f;
		m_vPlayerPosition.x = -5.0f;
		this->SideMonsterSpawn();
		m_vPlayerPosition.x = 5.0f;
		this->SideMonsterSpawn();
		m_vPlayerPosition.x = -10.0f;
		this->SideMonsterSpawn();
		m_vPlayerPosition.x = 10.0f;
		this->SideMonsterSpawn();
		break;
	}

	return fTime * fPer; 
}



void	cMonsterSpawn::SideMonsterSpawn( )
{
	if (m_nMaxCount > m_nCount)
	{
		cMonster* pMonster;
		if (m_IsThrow)
		{
			char* pKey = "enemy2";
			bool IsDivide = false;
			float fScale = 0.45f;
			if (rand() % 2)
			{
				IsDivide = true;
				pKey = "enemy3";
				fScale = 2.0f;
			}
			pMonster = CreateMonster(m_vPlayerPosition, pKey);
			pMonster->SetIsDivide(IsDivide);
			pMonster->SetMovePattern(new cThrowMove());
			pMonster->SetGun(new cMonsterGun());
			pMonster->SetScale(pMonster->GetScale() * fScale);
		}
		else
		{
			 pMonster = CreateMonster(m_vPlayerPosition, "enemy");
			 pMonster->SetMovePattern(new cSideMove());

		}
	}
}
void	cMonsterSpawn::FrontMonsterSpawn()
{
	if (m_nMaxCount > m_nCount)
	{
		auto pMonster = CreateMonster(m_vPlayerPosition, "enemy");
		pMonster->SetMovePattern(new cRushMove());
		
	}
}

void	cMonsterSpawn::FrontGapMonsterSpawn(int nMax, int nGap)
{
	D3DXVECTOR3	vOriPos = m_vPlayerPosition;
	for (int n = 0; n < nMax; ++n)
	{
		m_vPlayerPosition = vOriPos + (n - ((nMax  / 2)) ) * D3DXVECTOR3(nGap, 0, 0);
		if (nMax % 2 == 0)
		{
			m_vPlayerPosition += D3DXVECTOR3(nGap * 0.5f, 0, 0);
		}
		this->FrontMonsterSpawn();
	}
}

cMonster*	cMonsterSpawn::CreateMonster(const D3DXVECTOR3& vPos, char* pKey)
{
	++m_nCount;
	auto pMonster = (cMonster*)CreateObject<cMonster>(vPos, pKey);
	pMonster->SetHp(7 / m_fPer);
//	pMonster->SetIsDivide(true);
	return pMonster;
}

cMonster*	cMonsterSpawn::CreateThrowMonster(const D3DXVECTOR3& vPos, char* pKey)
{
	++m_nCount;
	auto pMonster = (cMonster*)CreateObject<ThrowerMonster>(vPos, pKey);
	pMonster->SetHp(7 / m_fPer);

	return pMonster;
}
