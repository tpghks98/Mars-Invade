#pragma once
#include "cObjBase.h"
class cMoveableObj :
	public cObjBase
{
protected:
	_SYNTHESIZE_REF_INHER(D3DXVECTOR3, m_vDirection, Direction);
	_SYNTHESIZE_REF_INHER(D3DXVECTOR3, m_vUp, Up);
	_SYNTHESIZE_REF_INHER(float, m_fMoveSpeed, MoveSpeed);
	_SYNTHESIZE_REF_INHER(float, m_fMoveLength, MoveLength);
	_SYNTHESIZE_REF_INHER(float, m_fHp, Hp);
	_SYNTHESIZE_REF_INHER(float, m_fMaxHp, MaxHp);
	_SYNTHESIZE_REF_INHER(D3DXQUATERNION, m_quatPrevLook, PrevLook);

	_SYNTHESIZE_INHER(bool, m_IsLookAtRot, IsLookAtRot);
	_SYNTHESIZE_INHER(bool, m_IsSmoothRot, IsSmoothRot);

public:
	virtual	void	Initialize()	PURE;
	virtual	void	Update()		PURE;
	virtual void	Render()		PURE;

	virtual	void	Translation();

	virtual	void	MoveDirection( D3DXVECTOR3* vPosition = NULL, float fSpeed = 0.0f );

	virtual	void	OnHit(float fDamage);
	virtual	void	FixHeight();
protected:
	void	LookAtDirection(D3DXMATRIXA16* pOut);
public:
	cMoveableObj();
	virtual ~cMoveableObj();
};

