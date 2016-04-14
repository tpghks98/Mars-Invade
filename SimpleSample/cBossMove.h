#pragma once
#include "cMovePattern.h"

class cObjBase;
class cBossMove :
	public cMovePattern
{
private:
	_SYNTHESIZE_REF_INHER(D3DXVECTOR3, m_vMoveDir, MoveDir);
	_SYNTHESIZE_INHER(cObjBase*, m_pObjOwner, ObjOwner);

	void	MoveDirReset();
public:
	virtual	void	MovePattern(const D3DXVECTOR3& vDir, cMoveableObj* pObj);
public:
	cBossMove();
	~cBossMove();
};

