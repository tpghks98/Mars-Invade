#pragma once
#include "cNode.h"

class cObjBase;
class cAbility :
	public cNode
{
public:
	virtual	void	OnAbility(cObjBase* pTarget) {}
public:
	cAbility();
	virtual ~cAbility();
};

