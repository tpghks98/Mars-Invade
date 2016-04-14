#include "DXUT.h"
#include "cItem.h"
#include "cResourceFile.h"
#include "cResourceMgr.h"
#include "cPlayer.h"
#include "cAbility.h"
#include "cSpeedAbility.h"
#include "shared.h"
#include "cEffect.h"
#include "cSystemMgr.h"
#include "cSpreadUI.h"

cItem::cItem()
:m_nID( IT_MAX )
, m_pItemAbility( NULL )
{
}


cItem::~cItem()
{
	SAFE_DELETE(m_pItemAbility);
}


void	cItem::Initialize()
{
	m_vScale *= 0.02f;
	m_IsCollisionCheck = false;
	m_vDirection = D3DXVECTOR3(-1, 0, 0);
	m_fMoveSpeed = 120.0f;
	m_vDirection = D3DXVECTOR3( rand() % 2 * 2 - 1, 0, 0.0f);
}
void	cItem::Update()
{
	if (_GETSINGLE(cObjectMgr)->GetPlayer() != NULL)
	{
		auto pPlayer = _GETSINGLE(cObjectMgr)->GetPlayer();
		auto mat = pPlayer->GetWorldMatrix();
		D3DXVECTOR3 v(mat._41, mat._42, mat._43);
		D3DXVECTOR3	vPrevDir = m_vDirection;
		m_vDirection = v - m_vPosition;
		if (D3DXVec3Length(&( m_vDirection)) < 2.0f)
		{
			this->OnCollision(pPlayer);
		}
		m_vDirection.y = 0.0f;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		D3DXVec3Lerp(&m_vDirection, &vPrevDir, &m_vDirection, _GETS( cSystemMgr )->GetDeltaTime() * 10.0f );
	}
	m_vPosition.y = 31.0f;

	this->MoveDirection();
	this->Translation();

	
}
void	cItem::Render()
{
	if (m_pResource != NULL)
	{
		this->DrawShadow();
		m_pResource->Render(this, _GETSINGLE(cResourceMgr)->GetShader(SI_CUSTOM));
	}
}

void	cItem::Setup()
{
	m_pResource = _GETSINGLE(cResourceMgr)->GetResource("woman");

	switch (m_nID)
	{
	case IT_SPEED:
		m_pResource = _GETSINGLE(cResourceMgr)->GetResource("item_speed");
		break;
	case IT_ADD:
		m_pResource = _GETSINGLE(cResourceMgr)->GetResource("item_add");

	case IT_HP:
		m_pResource = _GETSINGLE(cResourceMgr)->GetResource("item_heal");
		break;
	case IT_MISSLE:
		m_pResource = _GETSINGLE(cResourceMgr)->GetResource("item_missle");
		break;
	case IT_SHIELD:
		m_pResource = _GETSINGLE(cResourceMgr)->GetResource("item_shield");
		break;
	case IT_UPGRADE:
		m_pResource = _GETSINGLE(cResourceMgr)->GetResource("item_upgrade");
		break;
	default:
		break;
	}


}

void	cItem::OnCollision(cObjBase* pObj)
{
	if (typeid(*pObj) == typeid(cPlayer))
	{
		if (m_pItemAbility != NULL)
		{
			m_pItemAbility->OnAbility(pObj);
		}
		m_IsDelete = true;
		
		auto mat = pObj->GetWorldMatrix();
		D3DXVECTOR3	v(mat._41, mat._42, mat._43 - 1.0f);

		auto pEff = ((cEffect*)CreateObject<cEffect>(v , "effect_item"));
		pEff->SetIsBillBoard(false);
		pEff->SetRotation( D3DXVECTOR3( D3DX_PI * 0.6f, 0, 0 ));
		pEff->SetScale(pEff->GetScale() * 0.75f);
		switch (m_nID)
		{
		case IT_SPEED:
			((cPlayer*)pObj)->SetBoostTime( 3.0f );
			CreateObject<cSpreadUI>(D3DXVECTOR3(g_nWinSizeX * 0.5f, 150.0f, 0), "ui_notice_speed");
			break;
		case IT_ADD:
			((cPlayer*)pObj)->PlusAddMob();
			break;
		case IT_HP:
			((cPlayer*)pObj)->SetHp(((cPlayer*)pObj)->GetHp() + 3.0f);
			CreateObject<cSpreadUI>(D3DXVECTOR3(g_nWinSizeX * 0.5f, 150.0f, 0), "ui_notice_heal");
			break;
		case IT_MISSLE:
			((cPlayer*)pObj)->SetNumGuid(  ((cPlayer*)pObj)->GetNumGuid() + 1 );
			CreateObject<cSpreadUI>(D3DXVECTOR3(g_nWinSizeX * 0.5f, 150.0f, 0), "ui_notice_missle");
			break;
		case IT_SHIELD:
			((cPlayer*)pObj)->OnShield();
			CreateObject<cSpreadUI>(D3DXVECTOR3(g_nWinSizeX * 0.5f, 150.0f, 0), "ui_notice_shield");
			break;
		case IT_UPGRADE:
			((cPlayer*)pObj)->UpgradeGun();
			break;
		default:
			break;
		}

	}
}
