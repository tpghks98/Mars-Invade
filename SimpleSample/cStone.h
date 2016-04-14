#pragma once
#include "cBullet.h"
class cStone :
	public cBullet
{
private:
	_SYNTHESIZE_REF_INHER( float, m_f)
public:
	virtual	void	Initialize();
	virtual	void	Update();
	virtual void	Render();

	virtual	void	OnCollision(cObjBase* pObj);
public:
	cStone();
	~cStone();
};

