#pragma once
#include "cMovePattern.h"
class cRushMove :
	public cMovePattern
{
private:
	D3DXVECTOR3		m_vDirection;
	float			m_fTime;
	float			m_fPlusY;
public:
	virtual	void	MovePattern(const D3DXVECTOR3& vDir, cMoveableObj* pObj);

	void			OnReverseY();
public:
	cRushMove();
	~cRushMove();
};

