#pragma once
#include "cMoveableObj.h"
class cLaser :
	public cMoveableObj
{
private:
	_SYNTHESIZE_REF_INHER(float, m_fPlusGauge, Gauge);
	_SYNTHESIZE_REF_INHER(bool, m_IsReverse, IsReverse);
	_SYNTHESIZE_REF_INHER(D3DXQUATERNION, m_quatRot, quatRot);
public:
	virtual	void	Initialize();
	virtual	void	Update()	;
	virtual void	Render()	;

	virtual	void	Translation();
	virtual	void	OnCollision(cObjBase* pObj);
	void	OnActive();
	void	OnReverseGauge();
public:
	cLaser();
	~cLaser();
};

