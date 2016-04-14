#pragma once

#include "cNode.h"

class cMoveableObj;
class cMovePattern
	:public cNode
{
public:
	virtual	void	MovePattern(const D3DXVECTOR3& vDir, cMoveableObj* pObj) PURE;
public:

	cMovePattern();
	virtual ~cMovePattern();
};

