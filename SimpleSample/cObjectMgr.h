#pragma once

#include "value.h"
#include "cNode.h"
class cObjBase;
class cMap;
class cMoveableObj;
class cTimer;
class cNode;
class cCamera;
class cEffect;
class cPlayer;
class cMonsterSpawn;
class cMonster;
class cMapGenerate;
class cNumber;

class cObjectMgr
	:public	cSingleTon<cObjectMgr>, public cNode
{

public:
	std::list<cObjBase*> m_lstObject;
	std::list<cEffect*> m_lstEffect;
	std::list<cObjBase*> m_lstUI;

	std::list<cTimer*>	m_lstTimer;
	std::list<ST_RANK_DATA> m_lstScore;
private:
	_SYNTHESIZE_INHER( cMapGenerate*,	m_pMapGenerate, MapGenerate );
	cMonsterSpawn*	m_pMonsterSpawn;
	cMap*		m_pWorldMap;
	cNumber*	m_pScoreNumber;
	

	_SYNTHESIZE_INHER(bool, m_IsMap, IsMap);
	_SYNTHESIZE_INHER(int, m_nScore, Score);
	_SYNTHESIZE_INHER(int, m_nBossCount, BossCount);
	_SYNTHESIZE_INHER(cPlayer*, m_pPlayer, Player);
	_SYNTHESIZE_INHER(cMonster*, m_pBoss, Boss);

	_SYNTHESIZE_INHER(cCamera*, m_pShadowCamera, ShadowCamera);
	_SYNTHESIZE_INHER(LPDIRECT3DTEXTURE9, m_pShadowTex, ShadowTex);
	_SYNTHESIZE_INHER(LPDIRECT3DSURFACE9, m_pShadowDepth, ShadowDepth);

	_SYNTHESIZE_INHER(LPDIRECT3DSURFACE9, m_pOriTex, OriText);
	_SYNTHESIZE_INHER(LPDIRECT3DSURFACE9, m_pOriDepth, OriDepth);


	_SYNTHESIZE_INHER(bool, m_IsBump, IsBump);
	_SYNTHESIZE_INHER(bool, m_IsDiffuse, IsDiffuse);
	_SYNTHESIZE_REF_INHER(D3DXVECTOR3, m_vWorldUp, WorldUp);
	float m_fScrollSpeed;

public:
	float	GetScrollSpeed() { return m_fScrollSpeed; }
	void	SetScrollSpeed(const float& f);

public:
	void	Initialize();
	void	Update();
	void	Render();

	void	Release();

public:
	void	SetWorldMap(cMap* pMap) { m_pWorldMap = pMap; }
	bool	IsMapCollisionCheck(cMoveableObj* pObj, float* fHeight);

	void	ChangeShaodwBuf();
	void	ResetBuf();


	void	GenerateMonster();

	void	Waring();
	void	BossGenerate();
	void	DrawNumber(const D3DXVECTOR3& v, int nNumber, float fScale);

	std::list<cObjBase*>*	GetObjectList() { return &m_lstObject; }

	void	GameStart();
	void	AllDeleteObject();

	void	AddRankData(ST_RANK_DATA pData);
public:
	cTimer* CreateTimer(cNode* pParentNode);
private:
	void	TimerUpdate();
	void	CollisionCheck();
	void	EffectUpdate();
	void	EffectRender();


public:
	cObjectMgr();
	~cObjectMgr();
};

