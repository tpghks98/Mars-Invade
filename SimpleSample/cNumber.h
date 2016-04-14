#pragma once
#include "cObjBase.h"
class cNumber :
	public cObjBase
{
private:

	_SYNTHESIZE_INHER(int, m_nNumber, Number);
public:
	virtual	void	Initialize();
	virtual	void	Update()	;
	virtual void	Render()	;
public:
	cNumber();
	~cNumber();
};

