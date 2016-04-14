#pragma once
#include "cObjBase.h"
class cBackEff :
	public cObjBase
{
private:
	_SYNTHESIZE_REF_INHER(float, m_fAlphaPlus, AlphaPlus);
public:
	virtual	void	Initialize();
	virtual	void	Update()	;
	virtual void	Render()	;
public:
	cBackEff();
	~cBackEff();
};

