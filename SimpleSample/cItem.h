#pragma once
#include "cMoveableObj.h"

class cAbility;
class cItem :
	public cMoveableObj
{
public:
	_SYNTHESIZE_INHER(int, m_nID, ID);
	_SYNTHESIZE_INHER(cAbility*, m_pItemAbility, ItemAbilty);
public:
	virtual	void	Initialize();
	virtual	void	Update()	;
	virtual void	Render()	;

	virtual	void	OnCollision(cObjBase* pObj);
public:
	void	Setup();
public:
	cItem();
	~cItem();
};

