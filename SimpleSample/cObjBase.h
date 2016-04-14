#pragma once
#include "cNode.h"

class cResourceFile;
class cObjBase
	:public cNode
{
private:
	_SYNTHESIZE_REF_INHER(D3DXVECTOR3, m_vPosition, Position);
	_SYNTHESIZE_REF_INHER(D3DXVECTOR3, m_vScale, Scale);
	_SYNTHESIZE_REF_INHER(D3DXVECTOR3, m_vRotation, Rotation );

	_SYNTHESIZE_REF_INHER(D3DXMATRIXA16, m_matWorld, WorldMatrix );

	_SYNTHESIZE_REF_INHER(float, m_fAniKey, AniKey);
	_SYNTHESIZE_REF_INHER(float, m_fAlpha, Alpha);
	_SYNTHESIZE_REF_INHER(float, m_fHitcolor, HitColor);
	_SYNTHESIZE_REF_INHER(float, m_fRecoveryTime, RecoveryTime);

	_SYNTHESIZE_INHER(cResourceFile*, m_pResource, Resource);
	_SYNTHESIZE_INHER(bool, m_IsCollisionCheck, IsCollisionCheck);
public:
	virtual	void	Initialize()	PURE;
	virtual	void	Update()		PURE;
	virtual void	Render()		PURE;
	virtual	void	BaseUpdate();


	virtual	void	Translation();

	virtual	void	OnCollision(cObjBase* pObj) {}
	virtual void	OnHit(float fDamage);
	virtual	void	OnAnimationEnd()	{ m_fAniKey = 0.0f; }
	virtual	void	DrawShadow();
	virtual	void	DestoryObject() { m_IsDelete = true; }
public:
	cObjBase();
	virtual ~cObjBase();
};

