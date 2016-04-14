#pragma once
#include "cMoveableObj.h"

class cMovePattern;
class cGun;
class cMonster :
	public cMoveableObj
{
private:
	_SYNTHESIZE_INHER( cMovePattern*,	m_pMovePattern, MovePattern );
	_SYNTHESIZE_REF_INHER(D3DXVECTOR3, m_vLocalPos, LocaPos);
	_SYNTHESIZE_REF_INHER(D3DXVECTOR3, m_vOriScale, OriScale);
	_SYNTHESIZE_REF_INHER(D3DXVECTOR3, m_vShotDir, ShotDir);
	_SYNTHESIZE_INHER(cGun*, m_pGun, Gun);
	_SYNTHESIZE_INHER(bool, m_IsDivide, IsDivide);
	_SYNTHESIZE_INHER(bool, m_IsFly, IsFly);
	_SYNTHESIZE_INHER(bool, m_IsDie, IsDie);
public:
	virtual	void	Initialize();
	virtual	void	Update();
	virtual void	Render();

	virtual	void	DestoryObject();

	void	OnDieEffect();
	void	OnDie();
public:
	cMonster();
	~cMonster();
};

