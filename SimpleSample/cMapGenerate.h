#pragma once
#include "cObjBase.h"

class cMap;
class cMapGenerate :
	public cObjBase
{
private:
	_SYNTHESIZE_REF_INHER(std::vector<ST_MAP_DATA>, m_vecMapData, MapData);
	_SYNTHESIZE_REF_INHER(std::vector<cResourceFile*>, m_vecStructure, vecStructure);
	_SYNTHESIZE_REF_INHER(std::list<cMap*>, m_lstMap, MapList);


	_SYNTHESIZE_REF_INHER(D3DXVECTOR3, m_vPlusPosition, PlusPosition);
	_SYNTHESIZE_REF_INHER(D3DXVECTOR3, m_vPlusRotation, PlusRotation);
	_SYNTHESIZE_INHER(int, m_nCount, nCount);

	_SYNTHESIZE_INHER(bool, m_IsScroll, IsScroll);
public:
	virtual	void	Initialize();
	virtual	void	Update()	;
	virtual void	Render()	;

public:
	void	GenerateMap( bool IsFront = false);
private:
	void	AddMapData(const D3DXVECTOR3& vT, const D3DXVECTOR3& vR, char* pKey);
	void	AddStucture(char* pKey);
	void	CreateStrcture(cMap* pMap);
public:
	cMapGenerate();
	virtual ~cMapGenerate();
};

