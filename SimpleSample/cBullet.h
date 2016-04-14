#pragma once
#include "cMoveableObj.h"

class cTypeInfo;
class cBullet :
	public cMoveableObj
{
private:
	_SYNTHESIZE_INHER(cTypeInfo*, m_pTargetInfo, TargetInfo);
	_SYNTHESIZE_REF_INHER(float, m_fDamage, Damage);
	_SYNTHESIZE_INHER(bool, m_IsGuidShot, GuidShot);
	_SYNTHESIZE_INHER(cObjBase*, m_pTargetObject, TargetObject);
public:
	virtual	void	Initialize();
	virtual	void	Update()	;
	virtual void	Render()	;

	virtual	void	OnCollision(cObjBase* pObj);
	void	TargetSetup(cTypeInfo* pInfo);
	virtual void	Translation();


public:
	cBullet();
	~cBullet();
};

