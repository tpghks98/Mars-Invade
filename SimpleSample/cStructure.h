#pragma once
#include "cObjBase.h"

class cMap;
class cStructure :
	public cObjBase
{
	_SYNTHESIZE_INHER(cMap*, m_pParentMap, ParentMap);
public:
	virtual	void	Initialize();
	virtual	void	Update()	;
	virtual void	Render()	;
public:
	cStructure();
	~cStructure();
};

