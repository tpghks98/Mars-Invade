#pragma once

#include "cObjBase.h"
class cSubMap
	:public cObjBase

{
public:
	virtual	void	Initialize();
	virtual	void	Update();
	virtual void	Render();
public:
	cSubMap();
	~cSubMap();
};

