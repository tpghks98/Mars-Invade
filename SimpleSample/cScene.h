#pragma once
#include "cNode.h"
class cScene :
	public cNode
{
public:
	virtual	void	Init()	PURE;
public:
	cScene();
	virtual ~cScene();
};

