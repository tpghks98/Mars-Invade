#pragma once
#include "cObjBase.h"

class cResourceFile;
class cStructure;
class cMap :
	public cObjBase
{
private:
	std::list<cStructure*>		m_lstStructure;
	std::vector<D3DXVECTOR3>	m_vecFaces;

	_SYNTHESIZE_INHER(cResourceFile*, m_pSubMap, SubMap);
	_SYNTHESIZE_INHER(bool, m_IsCalcRot, IsCalcRot);
	_SYNTHESIZE_INHER(bool, m_IsMulRot, IsMulRot);
public:
	std::vector<D3DXVECTOR3>*	GetFacesPointer() { return &m_vecFaces;  }
public:
	virtual	void	Initialize();
	virtual	void	Update();
	virtual void	Render();

	virtual	void	Translation();
	void	DrawMap();

	void	AddStructure( cStructure* pStructure);
	void	AllDeleteStructure();
public:
	void	Scroll(float fValue);
	void	SetFaces();
public:
	cMap();
	~cMap();
};

