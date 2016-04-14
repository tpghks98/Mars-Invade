#include "DXUT.h"
#include "cMapGenerate.h"
#include "cResourceMgr.h"
#include "cResourceFile.h"
#include "cMap.h"
#include "shared.h"
#include "cSystemMgr.h"
#include "cStructure.h"


cMapGenerate::cMapGenerate()
:m_vPlusPosition(g_vZero)
, m_vPlusRotation(g_vZero )
, m_IsScroll( true )
, m_nCount(0 )
{
}


cMapGenerate::~cMapGenerate()
{
}


void	cMapGenerate::Initialize()
{
	m_vPlusPosition.z = -350.0f;
	m_vPlusPosition = -D3DXVECTOR3(-0.004f, 0, 296.734f + 350.0f) ;
	this->AddMapData(D3DXVECTOR3(-0.004f, 0, 294.734f ), D3DXVECTOR3(0, 0, 0), "map");
	this->AddMapData(D3DXVECTOR3(-0.004f, 0, 294.734f), D3DXVECTOR3(0, -D3DXToRadian(30), 0), "map_left");
	this->AddMapData(D3DXVECTOR3(-0.004f, 0, 294.734f), D3DXVECTOR3(0, D3DXToRadian(30), 0), "map_right");
	this->AddMapData(D3DXVECTOR3(-0.004f, 0, 294.734f), D3DXVECTOR3(-D3DXToRadian(30), 0, 0), "map_up");
	this->AddMapData(D3DXVECTOR3(-0.004f, 0, 294.734f), D3DXVECTOR3(D3DXToRadian(30), 0, 0), "map_under");
	


//	this->AddMapData(D3DXVECTOR3(-125.78f, 0.0f, 170.717f), D3DXVECTOR3(0, -D3DX_PI * 0.5f, 0), "map_s_left_floor", "map_s_left");
//	this->AddMapData(D3DXVECTOR3(126.016f, 0.0f, 171.883f), D3DXVECTOR3(0, D3DX_PI * 0.5f, 0), "map_s_right_floor", "map_s_right");


	
	
	this->AddStucture("structure_stone1");
	this->AddStucture("structure_stone2");
	this->AddStucture("structure_stone3");
	this->AddStucture("structure_stone4");
	this->AddStucture("structure_stone5");
	this->AddStucture("structure_stone6");
	this->AddStucture("structure_stone7");
	this->AddStucture("structure_cylinder1");
	this->AddStucture("structure_cylinder2");
	this->AddStucture("structure_up_stone1");
	this->AddStucture("structure_up_stone2");
	this->AddStucture("structure_side_stone");
	this->AddStucture("structure_box1");
	this->AddStucture("structure_box2");
//	this->AddStucture("structure_tree");
	
		
		

		
		
		
	for (int n = 0; n < 5; ++n)
	{
		GenerateMap( true);
	}
}
void	cMapGenerate::Update()
{

	m_vPlusPosition.z -= _GETSINGLE(cObjectMgr)->GetScrollSpeed() *
		_GETS(cSystemMgr)->GetDeltaTime();

	auto it = m_lstMap.begin();
	auto en_it = m_lstMap.end();
	int nCount = 0;
	while (it != en_it)
	{
		(*it)->Scroll(_GETSINGLE(cObjectMgr)->GetScrollSpeed());

		if ((*it)->GetPosition().z < -600.0f)
		{
			GenerateMap();
			(*it)->SetDelete(true);
			it = m_lstMap.erase(it);
		}
		else if ((*it)->GetPosition().z < 50.0f )
		{
			++nCount;
			if (!(*it)->GetIsCalcRot())
			{
				m_vPlusRotation +=
					(*it)->GetRotation();
				(*it)->SetIsCalcRot(true);
			}
		}
		++it;
	}
	it = m_lstMap.begin();

	D3DXVECTOR3 vPlusRot = -m_vPlusRotation;
	(*it)->SetIsMulRot(false);

	vPlusRot = -m_vPlusRotation;
	D3DXVec3Normalize(&vPlusRot, &vPlusRot);
	vPlusRot *= _GETS(cSystemMgr)->GetDeltaTime()  * 4;
	int nDegree = 2;
	if (m_vPlusRotation.x < D3DXToRadian(nDegree) &&
		m_vPlusRotation.x > -D3DXToRadian(nDegree))
	{
		vPlusRot.x = -m_vPlusRotation.x;
	}
	if (m_vPlusRotation.y < D3DXToRadian(nDegree) &&
		m_vPlusRotation.y > -D3DXToRadian(nDegree))
	{
		vPlusRot.y = -m_vPlusRotation.y;
	}


	m_vPlusRotation += vPlusRot;


	int nTest = 0;
	while (it != en_it)
	{
		if (nCount <= 0)
		{
			break;
	}
		(*it)->SetIsMulRot(true);
		if (nCount == 1)
		{
			(*it)->SetIsMulRot(false);
		}

		(*it)->SetRotation((*it)->GetRotation() + vPlusRot);

		--nCount;
		++it;
	}

}
void	cMapGenerate::Render()
{
	for each(auto iter in m_lstMap)
	{
		iter->DrawMap();
	}
	_GETSINGLE(cObjectMgr)->ChangeShaodwBuf();

	DXUTGetD3D9Device()->Clear(0,
		NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_ARGB(0, 255, 255, 255), 1, 0);

	_GETSINGLE(cObjectMgr)->ResetBuf();
}

void	cMapGenerate::AddMapData(const D3DXVECTOR3& vT, const D3DXVECTOR3& vR, char* pKey)
{
	ST_MAP_DATA mapdata;
	mapdata.pResourceFile = _GETS(cResourceMgr)->GetResource(pKey);
//	mapdata.pTopResourceFile = _GETS(cResourceMgr)->GetResource(pTopKey);
	mapdata.vPlusPosition = vT;
	mapdata.vPlusRotation = vR;

	m_vecMapData.push_back(mapdata);
}


void	cMapGenerate::GenerateMap(bool IsFront)
{
	ST_MAP_DATA data = m_vecMapData[ rand() % m_vecMapData.size()];

	if (IsFront)
	{
		data = m_vecMapData[0];
	}
//	pMap->SetSubMap(data.pTopResourceFile);
	D3DXVECTOR3	vPlus = data.vPlusPosition;
	m_vPlusPosition += vPlus;
	cMap*	pMap = (cMap*)CreateObject<cMap>(m_vPlusPosition, data.pResourceFile->GetKey());


	D3DXMATRIXA16	matRot;

	D3DXQUATERNION	q;

	D3DXQuaternionRotationYawPitchRoll(&q, m_vPlusRotation.y, m_vPlusRotation.x, m_vPlusRotation.z);
	D3DXMatrixRotationQuaternion(&matRot, &q);
//	D3DXMatrixInverse(&matRot, NULL, &matRot);

	D3DXVec3TransformCoord(&vPlus, &vPlus, &matRot);


	
	pMap->SetRotation(data.vPlusRotation);
	m_lstMap.push_back(pMap);

	CreateStrcture(pMap);
}

void	cMapGenerate::AddStucture(char* pKey)
{
	m_vecStructure.push_back(_GETS(cResourceMgr)->GetResource(pKey));
}


void	cMapGenerate::CreateStrcture(cMap* pMap)
{
	for (int n = 0; n < 30; ++n)
	{
		auto pStruct = (cStructure*)CreateObject<cStructure>( 
			D3DXVECTOR3( rand() % 1000 * 0.001f -0.5f, 0, rand() % 1000  * 0.001f * 18)
			* 15.0f);

		if (!m_vecStructure.empty())
			pStruct->SetResource(m_vecStructure[rand() % m_vecStructure.size()]);

		
		auto v = pStruct->GetPosition();

		if (pStruct->GetResource()->GetKey() == "structure_cylinder1" ||
			pStruct->GetResource()->GetKey() == "structure_cylinder2")
		{
			v.x = 0.0f;
		}
		else
		{
			if (v.x > 0.0f)
			{
				v.x += 20.0f;
			}
			else
			{
				v.x -= 20.0f;
			}
			if (pStruct->GetResource()->GetKey() == "structure_side_stone")
			{
				if (rand() % 2)
				{
					v.x = -35.0f;
				}
				else
				{
					v.x = 35.0f;
				}
			}
		}
		pStruct->SetPosition(v);
		

		pStruct->SetParentMap(pMap);
		pMap->AddStructure(pStruct);
	}
}
