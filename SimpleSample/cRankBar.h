#pragma once
#include "cObjBase.h"
class cRankBar :
	public cObjBase
{
public:
	virtual	void	Initialize();
	virtual void	Render()	;
	virtual	void	Update()	;
public:
	cRankBar();
	~cRankBar();
};

