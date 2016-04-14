#pragma once
#include "cObjBase.h"
class cSpreadUI :
	public cObjBase
{
private:
	_SYNTHESIZE_REF_INHER(float, m_fTime, Time);
	_SYNTHESIZE_REF_INHER(float, m_fPlusTime, PlusTime);
public:
	virtual	void	Initialize();
	virtual	void	Update();
	virtual void	Render();
public:
	cSpreadUI();
	~cSpreadUI();
};

