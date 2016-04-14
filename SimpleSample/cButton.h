#pragma once
#include "cObjBase.h"
class cButton :
	public cObjBase
{
	_SYNTHESIZE_REF_INHER(float, m_fScaleTime, ScaleTime);
public:
	virtual	void	Initialize();
	virtual	void	Update()	;
	virtual void	Render()	;
public:
	cButton();
	~cButton();
};

