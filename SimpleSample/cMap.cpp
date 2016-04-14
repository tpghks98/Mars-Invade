#include "DXUT.h"
#include "cMap.h"
#include "cResourceMgr.h"
#include "cResourceFile.h"
#include "cAseFile.h"
#include "cGeomNode.h"
#include "cObjectMgr.h"
#include "cSystemMgr.h"
#include "shared.h"
#include "cSubMap.h"
#include "cObjectMgr.h"
#include "cStructure.h"

cMap::cMap()
:m_pSubMap( NULL )
, m_IsCalcRot( false )
, m_IsMulRot( false )
{
}


cMap::~cMap()
{
	AllDeleteStructure();
}


void	cMap::Initialize()
{
//	m_pResource = _GETSINGLE(cResourceMgr)->GetResource("map_s_front_floor");

	_GETSINGLE(cObjectMgr)->SetWorldMap(this);
}
void	cMap::Update()
{
	if (m_IsCalcRot)
	{
	}
	this->Translation();
}
void	cMap::Render()
{
	
}
void	cMap::DrawMap()
{
	m_pResource->Render(this, _GETSINGLE(cResourceMgr)->GetShader(SI_APPLY), false);

}


void	cMap::Scroll(float fValue)
{
	m_vPosition.z -= fValue * _GETSINGLE( cSystemMgr )->GetDeltaTime();


//	m_vRotation.x += fValue * _GETS( cSystemMgr )->GetDeltaTime();

//	auto v = m_pSubMap->GetRotation();
//	v.x += fValue * 0.5f * _GETS(cSystemMgr)->GetDeltaTime();

//	m_pSubMap->SetRotation(v);
}

void	cMap::SetFaces()
{
	if (m_pResource != NULL)
	{
		if (typeid(cAseFile) == typeid(*m_pResource))
		{
			((cAseFile*)m_pResource)->m_pRootNode->GetAllFaces(m_vecFaces);
		}
	}
}

void	cMap::AddStructure(cStructure* pStructure)
{
	m_lstStructure.push_back(pStructure);
}

void	cMap::AllDeleteStructure()
{
	for each(auto it in m_lstStructure)
	{
		it->SetDelete(true);
	}

	m_lstStructure.clear();
}

void	cMap::Translation()
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
	if (m_IsMulRot)
	{
		D3DXVECTOR3	v = D3DXVECTOR3(-0.004f, 0, 294.734f);
		D3DXVECTOR3	vRot;
		D3DXVec3TransformCoord(&vRot, &v, &matRot);

		v = v - vRot;

		D3DXMatrixTranslation(&matTrans, v.x, v.y, v.z);
		m_matWorld *= matTrans;
	}

}