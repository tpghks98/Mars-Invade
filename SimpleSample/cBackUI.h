#pragma once
#include "cObjBase.h"
class cBackUI :
	public cObjBase
{
public:
	virtual	void	Initialize();
	virtual	void	Update()	;
	virtual void	Render()	;
public:
	cBackUI();
	~cBackUI();
};

