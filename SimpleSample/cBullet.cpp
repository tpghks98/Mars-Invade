#include "DXUT.h"
#include "cBullet.h"
#include "cResourceMgr.h"
#include "cResourceFile.h"
#include "cTypeInfo.h"
#include "shared.h"
#include "cEffect.h"
#include "cPlayer.h"
#include "cTimer.h"
#include "cObjectMgr.h"
#include "cMonster.h"
#include "cSystemMgr.h"

cBullet::cBullet()
:m_pTargetInfo( NULL )
, m_fDamage( 0.0f )
, m_IsGuidShot( false )
, m_pTargetObject(NULL)
{
}


cBullet::~cBullet()
{
	SAFE_DELETE(m_pTargetInfo);
}


void	cBullet::Initialize()
{

	m_IsLookAtRot = true;
	m_IsCollisionCheck = true;
	m_IsLookAtRot = true;
	m_vUp = _GETSINGLE(cObjectMgr)->GetWorldUp();


}
void	cBullet::Update()
{
	float fLength = FLT_MAX;
	if (m_IsGuidShot)
	{
		m_vRotation.x = 0.0f;
		auto iter = _GETS(cObjectMgr)->m_lstObject.begin();
		auto en_it = _GETS(cObjectMgr)->m_lstObject.end();
		D3DXVECTOR3	vDir = D3DXVECTOR3(0, 0, 1);
		D3DXVECTOR3	vDist = g_vZero;
		while (iter != en_it)
		{
			if (typeid(**iter) == typeid(cMonster)  )
			{
				vDist = (*iter)->GetPosition() - m_vPosition;
				float fDist = D3DXVec3Length(&vDist);
				if (fDist < fLength)
				{
					fLength = fDist;
					D3DXVec3Normalize(&vDir, &vDist);
				}
			}
			++iter;
		
		}
		m_vDirection = (vDir - m_vDirection) * 10.0f * _GETS(cSystemMgr)->GetDeltaTime() + m_vDirection;

	}
	if (m_vPosition.z > 300.0f || m_vPosition.z < -100.0f)
	{
		m_IsDelete = true;
	}
	this->MoveDirection();
	this->Translation();
	this->BaseUpdate();



}
void	cBullet::Render()
{
	_GETS(cObjectMgr)->SetIsMap(true);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_ZWRITEENABLE, false);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	this->DrawShadow();


	m_pResource->Render(this, _GETSINGLE( cResourceMgr )->GetShader( SI_CUSTOM  ) );
	DXUTGetD3D9Device()->SetRenderState(D3DRS_ZWRITEENABLE, true);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	_GETS(cObjectMgr)->SetIsMap(false);

}

void	cBullet::OnCollision(cObjBase* pObj)
{
	
	if (typeid(*pObj) == m_pTargetInfo->GetType())
	{
		pObj->OnHit(m_fDamage);

		DestoryObject();
		((cEffect*)CreateObject<cEffect>(m_vPosition, "effect_bullet")
			)->SetIsZEnable( false ) ;
		_GETS(cSystemMgr)->Shaking(0.1f, 0.0f, false);


	}
	else if (typeid(*pObj) == typeid(cBullet))
	{
		
		if (m_IsGuidShot &&
			m_pTargetInfo->GetType() != ((cBullet*)pObj)->GetTargetInfo()->GetType())
		{
			pObj->DestoryObject();
			this->DestoryObject();
			CreateObject<cEffect>(m_vPosition, "effect_bullet");
		}

	}
}

void	cBullet::TargetSetup(cTypeInfo* pInfo)
{
	m_pTargetInfo = pInfo;

	float fTime = 1.5f;
	if (pInfo->GetType() == typeid( cPlayer ))
	{
		fTime = 6.5f;
		if (m_pResource != NULL)
		{
			m_vRotation.x = D3DX_PI * -0.2f;

			if (m_pResource->GetKey() == "bullet_enemy1")
			{
				m_vRotation.x = D3DX_PI * -0.5f;
				m_vScale *= 0.5f;
			}
		}

	}

	auto pTimer = _GETSINGLE(cObjectMgr)->CreateTimer(this);
	pTimer->Setup(this, fTime, false, std::bind(&cBullet::DestoryObject, this));
}


void	cBullet::Translation()
{
	D3DXMATRIXA16	matScale, matRot, matTrans, matDir;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	{
		D3DXQUATERNION	q;
		D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, m_vRotation.x, m_vRotation.z);
		D3DXMatrixRotationQuaternion(&matRot, &q);

	}
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	D3DXMATRIXA16	matView;
	DXUTGetD3D9Device()->GetTransform(D3DTS_VIEW, &matView);

	memcpy(&matView._41, D3DXVECTOR3(0, 0, 0), sizeof(D3DXVECTOR3));

	D3DXMatrixTranspose(&matView, &matView);


	D3DXMatrixIdentity(&matDir);

	if (m_IsLookAtRot)
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
			D3DXQuaternionSlerp(&m_quatPrevLook, &m_quatPrevLook, &q, fTime);
			D3DXQUATERNION q2;
			D3DXQuaternionRotationMatrix(&q2, &matView);
			q2 = m_quatPrevLook;
			D3DXMatrixRotationQuaternion(&matDir, &q2);
		}
	}

	m_matWorld = matScale	*  matDir  * matRot *	matTrans;

}