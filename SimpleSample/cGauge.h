#pragma once
#include "cObjBase.h"
class cGauge :
	public cObjBase
{
public:
	_SYNTHESIZE_INHER(float, m_fPer, Per);
	_SYNTHESIZE_INHER(float*, m_pCurr, Curr);
	_SYNTHESIZE_INHER(float*, m_pMax, Max);
public:
	virtual	void	Initialize();
	virtual	void	Update();
	virtual void	Render();
public:
	cGauge();
	~cGauge();
};

