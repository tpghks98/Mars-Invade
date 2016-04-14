#include "DXUT.h"
#include "cObjectMgr.h"
#include "cObjBase.h"
#include "cPlayer.h"
#include "cMap.h"
#include "cResourceMgr.h"
#include "cAseFile.h"
#include "cTimer.h"
#include "cCamera.h"
#include "cSystemMgr.h"
#include "shared.h"
#include "cMonster.h"
#include "cEffect.h"
#include "cGun.h"
#include "cMapGenerate.h"
#include "cMonsterSpawn.h"
#include "cBossMove.h"
#include "cBossGun.h"
#include "cLookCamera.h"
#include "cBackUI.h"
#include "cSpreadUI.h"
#include "cGauge.h"
#include "cNumber.h"


cObjectMgr::cObjectMgr()
:m_pWorldMap( NULL )
, m_IsMap( false) 
, m_IsBump( true) 
, m_vWorldUp( 0, 1, 0 )
, m_pPlayer( NULL )
, m_fScrollSpeed( 350.0f )
, m_pBoss(NULL)
, m_IsDiffuse( false )
, m_pMapGenerate( NULL )
, m_pShadowCamera( NULL )
, m_pMonsterSpawn( NULL )
, m_pShadowDepth( NULL )
, m_pShadowTex( NULL )
, m_pOriDepth( NULL )
, m_pOriTex( NULL )
, m_nScore( 0 )
, m_nBossCount( 0 )
, m_pScoreNumber( NULL )
{
}


cObjectMgr::~cObjectMgr()
{
	SAFE_DELETE(m_pShadowCamera);
	SAFE_DELETE(m_pMonsterSpawn);
	Release();
}

class cSort
{
public:
	bool	operator()(cObjBase* pObj1, cObjBase* pObj2)
	{
		return pObj1->GetPosition().z > pObj2->GetPosition().z;
	}
};
class cRankSort
{
public:
	bool	operator()(ST_RANK_DATA p1, ST_RANK_DATA p2)
	{
		return p1.nScore > p2.nScore;
	}
};


void	cObjectMgr::Initialize()
{
	DXUTGetD3D9Device()->GetRenderTarget(0, &m_pOriTex);
	DXUTGetD3D9Device()->GetDepthStencilSurface(&m_pOriDepth); 
	int nShadowMapSize = 8196;


		DXUTGetD3D9Device()->CreateTexture(nShadowMapSize, nShadowMapSize,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT,
		&m_pShadowTex, NULL);



	DXUTGetD3D9Device()->CreateDepthStencilSurface(nShadowMapSize,
		nShadowMapSize, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, true,
		&m_pShadowDepth, NULL);

	m_pShadowCamera = new cCamera();
	m_pShadowCamera->Setup(D3DXVECTOR3(0, 100.0f, 0),
		D3DXVECTOR3(0.1f, 0, 0.1f), D3DXVECTOR3(0, 1, 0));

	ST_RANK_DATA data;
	data.nScore = 50;
	data.strName = "ABC";
	this->AddRankData(data);
	data.nScore = 25;
	data.strName = "DKK";
	this->AddRankData(data);
	data.nScore = 30;
	data.strName = "TGI";
	this->AddRankData(data);
	data.nScore = 0;
	data.strName = "QWE";
	this->AddRankData(data);

}
void	cObjectMgr::Update()
{
	if (_GETS(cSystemMgr)->GetKeyState('H') == KS_ONCE)
	{
		Waring();
	}

	int num = 50;
	int b = 5;	
	if (num*b / b != num)
	{
		b += 250;
	}

	if (_GETSINGLE(cSystemMgr)->GetKeyState('T') == KS_ONCE)
	{
		m_IsBump = !m_IsBump;
	}
	TimerUpdate();

	auto iter = m_lstObject.begin();
	auto end_iter = m_lstObject.end();

	while (iter != m_lstObject.end())
	{
		if (typeid((**iter)) == typeid(cGun))
		{
			break;
		}
		if ((*iter)->GetDelete())
		{
			SAFE_DELETE(*iter);
			iter = m_lstObject.erase(iter);
			continue;
		}
		(*iter)->Translation();
		(*iter)->Update();
		if ((*iter)->GetDelete())
		{
			SAFE_DELETE(*iter);
			iter = m_lstObject.erase(iter);
			continue;
		}
		++iter;
	}

	auto it_ui = m_lstUI.begin();
	auto en_it_ui = m_lstUI.end();

	while (it_ui != en_it_ui)
	{
		if ((*it_ui)->GetDelete())
		{
			SAFE_DELETE(*it_ui);
			it_ui = m_lstUI.erase(it_ui);
			continue;
		}
		(*it_ui)->Translation();
		(*it_ui)->Update();
		++it_ui;
	}


	if (m_pShadowCamera != NULL)
	{
		m_pShadowCamera->Update(false);
	}

	this->EffectUpdate();
	this->CollisionCheck();

}
void	cObjectMgr::Render()
{
	auto iter = m_lstObject.begin();
	auto end_iter = m_lstObject.end();

	while (iter != end_iter)
	{
		(*iter)->Render();
		++iter;
	}
	this->EffectRender();
	
	for each(auto iter in m_lstUI)
	{
		iter->Render();
	}

	this->DrawNumber(D3DXVECTOR3(425, 46, 0), m_nScore, 1.0f);
}

void	cObjectMgr::Release()
{
	for each(auto iter in m_lstUI)
	{
		SAFE_DELETE(iter);
	}
	for each(auto iter in m_lstObject)
	{
		SAFE_DELETE( iter);
	}
	m_lstObject.clear();
	for each(auto iter in m_lstEffect)
	{
		SAFE_DELETE(iter);
	}
	m_lstEffect.clear();
	for each(auto iter in m_lstTimer)
	{
		SAFE_DELETE(iter);
	}
	m_lstTimer.clear();
	SAFE_RELEASE(m_pShadowDepth);
	SAFE_RELEASE(m_pShadowTex);
	SAFE_RELEASE(m_pOriTex);
	SAFE_RELEASE(m_pOriDepth);

}

bool	cObjectMgr::IsMapCollisionCheck(cMoveableObj* pObj, float* fHeight)
{
	if (m_pWorldMap == NULL)
	{
		return false;
	}
	if (typeid(*pObj->GetResource()) == typeid(cAseFile))
	{
		cAseFile* pAsefile = (cAseFile*)pObj->GetResource();

		D3DXVECTOR3	vDir, vOri, vPlus;
		vDir = D3DXVECTOR3(0, -1, 0);
		vPlus = ( pObj->GetDirection() * pObj->GetScale().x * pAsefile->m_fModelRadius ) + (pAsefile->m_vModelCenter * pObj->GetScale().x);
		D3DXMATRIXA16 matRot;
		D3DXMatrixRotationY(&matRot, D3DX_PI * 0.5f);

		float fHeighTest = FLT_MIN;
		for (int n = 0; n < 1; ++n)
		{
			bool IsColl = true;
			vOri = pObj->GetPosition();
			vOri.y = 10000.0f;

			int	nFaces = m_pWorldMap->GetFacesPointer()->size() / 3;

			float fU, fV, fDist;
			for (int i = 0; i < nFaces; ++i)
			{
				if (D3DXIntersectTri(
					&((*m_pWorldMap->GetFacesPointer())[i * 3 + 1] * 1),
					&((*m_pWorldMap->GetFacesPointer())[i * 3 + 2] * 1),
					&((*m_pWorldMap->GetFacesPointer())[i * 3 + 0] * 1),
					&vOri, &vDir, &fU, &fV, &fDist))
				{
					IsColl = false;
					if (fHeighTest < vOri.y - fDist)
					{
						(*fHeight) = vOri.y - fDist;
					}
				}

			}
			if (IsColl)
			{
				(*fHeight) = 0.0f;
				return true;
			}

			D3DXVec3TransformCoord(&vPlus, &vPlus, &matRot);
		}

		return false;

	}
}

cTimer* cObjectMgr::CreateTimer(cNode* pParentNode)
{
	cTimer* pTimer = new cTimer();

	m_lstTimer.push_back(pTimer);

	cNode::AddInherit(pParentNode, pTimer);
	return pTimer;
}

void	cObjectMgr::TimerUpdate()
{
	auto iter = m_lstTimer.begin();
	auto end_iter = m_lstTimer.end();

	while (iter != end_iter)
	{
		(*iter)->Update();
		if ((*iter)->GetDelete())
		{
			SAFE_DELETE(*iter);
			iter =m_lstTimer.erase(iter);
			continue;
		}
		++iter;
	}
}



void	cObjectMgr::ChangeShaodwBuf()
{
	LPDIRECT3DSURFACE9	pShadowSurface = NULL;
	m_pShadowTex->GetSurfaceLevel(0, &pShadowSurface);
	DXUTGetD3D9Device()->SetRenderTarget(0, pShadowSurface);

	pShadowSurface->Release();
	pShadowSurface = NULL;

	DXUTGetD3D9Device()->SetDepthStencilSurface(m_pShadowDepth);
}
void	cObjectMgr::ResetBuf()
{
	DXUTGetD3D9Device()->SetRenderTarget(0, m_pOriTex);
	DXUTGetD3D9Device()->SetDepthStencilSurface(m_pOriDepth);

}

void	cObjectMgr::CollisionCheck()
{
	auto	iter = m_lstObject.begin();
	auto	end_iter = m_lstObject.end();


	auto	iter2 = m_lstObject.begin();

	while (iter != end_iter)
	{
		if ((*iter)->GetResource() == NULL)
		{
			++iter;
			continue;
		}
		if (typeid(*(*iter)->GetResource()) != typeid (cAseFile)
			|| !(*iter)->GetIsCollisionCheck())
		{
 			++iter;
			continue;
		}
		while (iter2 != end_iter)
		{
			if ((*iter2)->GetResource() == NULL)
			{
				++iter2;
				continue;
			}
			if (typeid(*(*iter2)->GetResource()) != typeid (cAseFile) ||
				!(*iter2)->GetIsCollisionCheck())
			{
				++iter2;
				continue;
			}
			if ((*iter) == (*iter2))
			{
				++iter2;
				continue;
			}
			auto pAse1 = (cAseFile*)(*iter)->GetResource();
			auto pAse2 = (cAseFile*)(*iter2)->GetResource();

			float fScale1 = ((*iter)->GetScale().x + (*iter)->GetScale().z)* 0.5f;
			float fScale2 = ((*iter2)->GetScale().x + (*iter2)->GetScale().z)* 0.5f;
			D3DXVECTOR3 vPos_1 = (*iter)->GetPosition() + pAse1->m_vModelCenter * fScale1;
			D3DXVECTOR3 vPos_2 = (*iter2)->GetPosition() + pAse2->m_vModelCenter * fScale2;
			D3DXVECTOR3	vDist = vPos_1 - vPos_2;

			float fLength = D3DXVec3Length(&vDist);
			float fDist = pAse1->m_fModelRadius * fScale1 + pAse2->m_fModelRadius * fScale2;

			if (fLength < fDist)
			{
				(*iter)->OnCollision((*iter2));
				(*iter2)->OnCollision((*iter));
			}
			++iter2;
		}
		++iter;
		iter2 = iter;

	}
}

void	cObjectMgr::EffectUpdate()
{
	auto iter = m_lstEffect.begin();
	auto end_iter = m_lstEffect.end();

	while (iter != end_iter)
	{
		if ((*iter)->GetDelete())
		{
			SAFE_DELETE(*iter);
			iter = m_lstEffect.erase(iter);
			continue;
		}
		(*iter)->Update();

		++iter;
	}
	m_lstEffect.sort(cSort());

}
void	cObjectMgr::EffectRender()
{
	auto iter = m_lstEffect.begin();
	auto end_iter = m_lstEffect.end();

	while (iter != end_iter)
	{
		(*iter)->Render();
		++iter;
	}
}


void	cObjectMgr::GenerateMonster()
{
	D3DXVECTOR3 vPos = D3DXVECTOR3( 0,20, 150 ) ;
	float fTime = m_pMonsterSpawn->GenerateMonster(vPos);

	CreateTimer(this)->Setup(this, fTime, false, std::bind(&cObjectMgr::GenerateMonster, this));
}

void	cObjectMgr::SetScrollSpeed(const float& f)
{
	if (m_pMapGenerate != NULL)
	{
		if (m_pMapGenerate->GetIsScroll() )
		{
			m_fScrollSpeed = (f - m_fScrollSpeed) * _GETSINGLE(cSystemMgr)->GetDeltaTime() + m_fScrollSpeed;
		}
		else
		{
			m_fScrollSpeed = (- m_fScrollSpeed * 7.5f) * _GETSINGLE(cSystemMgr)->GetDeltaTime() + m_fScrollSpeed;
		}
	}
}

void	cObjectMgr::Waring()
{
	if (m_pBoss == NULL)
	{
		CreateObject<cSpreadUI>(D3DXVECTOR3(g_nWinSizeX * 0.5f, 300.0f, 0), "ui_notice_warning");
		CreateTimer(this)->Setup(this, 1.0f, false, std::bind(&cObjectMgr::BossGenerate, this));
	}
}

void	cObjectMgr::BossGenerate()
{
	float fHp = 100.0f;
	char* pKey = "boss";
	float fScale = 4.0f;
	bool IsFly = false;
	switch (m_nBossCount)
	{
	case 0:
		pKey = "miniboss";
		fScale = 0.85f;
		fHp = 300.0f;
		IsFly = true;
		break;
	case 1:
		pKey = "boss";
		fHp = 500.0f;
		fScale = 4.0f;
		break;
	}
	m_pBoss = (cMonster*)CreateObject<cMonster>(D3DXVECTOR3(0, 10, 150), pKey);
	m_pBoss->SetMovePattern(new cBossMove());
	m_pBoss->SetGun(new cBossGun());
	m_pBoss->SetHp(fHp);
	m_pBoss->SetMaxHp(fHp);
	m_pBoss->SetScale(m_pBoss->GetScale() * fScale);
	m_pBoss->SetIsFly(IsFly);


	auto pGauge = (cGauge*)
		CreateObject<cGauge>(D3DXVECTOR3(g_nWinSizeX * 0.5f, 20, 0), "ui_bar_boss" );
	
	pGauge->SetMax(m_pBoss->GetMaxHpPointer());
	pGauge->SetCurr(m_pBoss->GetHpPointer() );

	this->AddInherit(m_pBoss, pGauge);

	auto	pCamera = _GETSINGLE(cSystemMgr)->AddCamera();
	pCamera->SetTarget(m_pBoss);
	pCamera->SetEye(D3DXVECTOR3(0, 20, 5));
	((cLookCamera*)pCamera)->SetTimeSpeed(2.0f);

	++m_nBossCount;
}

void	cObjectMgr::DrawNumber(const D3DXVECTOR3& v, int nNumber,  float fScale)
{
	if (m_pScoreNumber != NULL)
	{
		m_pScoreNumber->SetPosition(v);
		m_pScoreNumber->SetNumber(nNumber);
		m_pScoreNumber->SetScale(g_vOne * fScale);
		m_pScoreNumber->Render();
	}
}

void	cObjectMgr::GameStart()
{
	m_pMapGenerate = (cMapGenerate*)
		CreateObject<cMapGenerate>();



	CreateObject<cBackUI>(g_vMiddle, "ui_background");

	m_pPlayer = (cPlayer*)
		CreateObject<cPlayer>(D3DXVECTOR3(0, 0, 0));

	m_pMonsterSpawn = new cMonsterSpawn();
	this->GenerateMonster();
//		BossGenerate();
	CreateTimer(this)->Setup(this, 70.0f, false, std::bind(&cObjectMgr::Waring, this));
	CreateTimer(this)->Setup(this, 20.0f, false, std::bind(&cObjectMgr::Waring, this));

	m_pScoreNumber = (cNumber*)CreateObject<cNumber>(D3DXVECTOR3(425, 46, 0), "ui_number");
}

void	cObjectMgr::AllDeleteObject()
{
	for each(auto iter in m_lstUI)
	{
		iter->SetDelete(true);
	}
	for each(auto iter in m_lstObject)
	{
		iter->SetDelete(true);


	}
	for each(auto iter in m_lstEffect)
	{
		iter->SetDelete(true);


	}
	for each(auto iter in m_lstTimer)
	{
		iter->SetDelete(true);


	}
}

void	cObjectMgr::AddRankData(ST_RANK_DATA pData)
{
	m_lstScore.push_back(pData);

	m_lstScore.sort(cRankSort());

	if (m_lstScore.size() > 5)
	{
		m_lstScore.erase(--m_lstScore.end());
	}
}
