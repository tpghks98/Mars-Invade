#pragma once
#include "cMonster.h"
class ThrowerMonster :
	public cMonster
{
public:
	virtual	void	Initialize();
	virtual	void	Update();
	virtual void	Render();
public:
	ThrowerMonster();
	~ThrowerMonster();
};

