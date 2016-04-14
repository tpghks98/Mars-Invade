#pragma once
#include "cMovePattern.h"
class cSideMove :
	public cMovePattern
{
public:
	_SYNTHESIZE_REF_INHER(float, m_fPerSpeed, PerSpeed);
	_SYNTHESIZE_REF_INHER(float, m_fRot, Rot);
	_SYNTHESIZE_REF_INHER(float, m_fPlusSpeed, PlusSpeed );

public:
	virtual	void	MovePattern(const D3DXVECTOR3& vDir, cMoveableObj* pObj);
public:
	cSideMove();
	~cSideMove();
};

