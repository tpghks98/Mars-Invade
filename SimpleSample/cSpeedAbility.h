#pragma once
#include "cAbility.h"
class cSpeedAbility :
	public cAbility
{
public:
	virtual	void	OnAbility(cObjBase* pTarget);
public:
	cSpeedAbility();
	~cSpeedAbility();
};

