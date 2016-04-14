#pragma once
#include "cCamera.h"
class cLookCamera :
	public cCamera
{
private:
	_SYNTHESIZE_REF_INHER(float, m_fTime, Time);
	_SYNTHESIZE_REF_INHER(float, m_fTimeSpeed, TimeSpeed);
public:
	virtual void	Update(bool IsMatrix = true);
public:
	cLookCamera();
	~cLookCamera();
};

