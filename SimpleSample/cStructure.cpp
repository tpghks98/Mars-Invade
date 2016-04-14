#include "DXUT.h"
#include "cStructure.h"
#include "cResourceFile.h"
#include "cResourceMgr.h"
#include "cObjectMgr.h"
#include "cMap.h"

cStructure::cStructure()
:m_pParentMap( NULL )
{
}


cStructure::~cStructure()
{
}

void	cStructure::Initialize()
{
	m_IsCollisionCheck = true;
}
void	cStructure::Update()
{
	if (!m_IsDelete)
	{
		if (m_pParentMap != NULL)
		{
			_D3DXMATRIXA16 matTrans, matRot;
			D3DXQUATERNION	q;
			D3DXQuaternionRotationYawPitchRoll(&q, m_pParentMap->GetRotation().y, m_pParentMap->GetRotation().x, m_pParentMap->GetRotation().z);
			D3DXMatrixRotationQuaternion(&matRot, &q);

			D3DXVECTOR3	vPos = m_vPosition;
		//	D3DXVec3TransformCoord(&vPos , &vPos, &matRot);
			D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);
			
			m_matWorld = matTrans * m_pParentMap->GetWorldMatrix();
		}
	}
}
void	cStructure::Render()
{
	this->DrawShadow();
	_GETS(cObjectMgr)->SetIsMap(true);
	_GETS(cObjectMgr)->SetIsDiffuse(true);

	m_pResource->Render(this, _GETSINGLE( cResourceMgr )->GetShader( SI_CUSTOM ) );
	_GETS(cObjectMgr)->SetIsMap(false);
	_GETS(cObjectMgr)->SetIsDiffuse(false);

}