#include "DXUT.h"
#include "cSpeedAbility.h"
#include "cObjBase.h"
#include "cPlayer.h"

cSpeedAbility::cSpeedAbility()
{
}


cSpeedAbility::~cSpeedAbility()
{
}

void	cSpeedAbility::OnAbility(cObjBase* pTarget)
{
	if (typeid(*pTarget) == typeid(cPlayer))
	{
		float fSpeed = 
			((cPlayer*)pTarget)->GetMoveSpeed();
		fSpeed += D3DX_PI * 0.1f;
		if (fSpeed > D3DX_PI)
		{
			fSpeed = D3DX_PI;
		}
		((cPlayer*)pTarget)->SetMoveSpeed(fSpeed);
	}
}
