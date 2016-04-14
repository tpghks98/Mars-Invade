#pragma once
#include "cObjBase.h"

class cPlayer;
class cGun;
class cSubPlayer :
	public cObjBase
{
private:
	_SYNTHESIZE_INHER(cPlayer*, m_pPlayer, Player);
	_SYNTHESIZE_INHER(cGun*, m_pGun, Gun);

public:
	virtual	void	Initialize();
	virtual	void	Update();
	virtual void	Render();


	virtual	void	Translation();
public:
	cSubPlayer();
	~cSubPlayer();
};

