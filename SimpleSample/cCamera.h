#pragma once
#include "cNode.h"

class cObjBase;
class cCamera :
	public cNode
{
private:
	_SYNTHESIZE_REF_INHER(D3DXVECTOR3, m_vEye, Eye);
	_SYNTHESIZE_REF_INHER(D3DXVECTOR3, m_vAt, At);
	_SYNTHESIZE_REF_INHER(D3DXVECTOR3, m_vUp, Up);
	_SYNTHESIZE_REF_INHER(D3DXVECTOR3, m_vShake, Shake);
	_SYNTHESIZE_REF_INHER(D3DXVECTOR3, m_vPrevShake, PrevShake);

	_SYNTHESIZE_INHER(cObjBase*, m_pTarget, Target);
	_SYNTHESIZE_REF_INHER(float, m_fDist, Dist);
	_SYNTHESIZE_REF_INHER(float, m_fMoveSpeed, MoveSpeed );
	_SYNTHESIZE_REF_INHER(D3DXVECTOR3, m_vPlusDir, PlusDir);
	_SYNTHESIZE_REF_INHER(D3DXVECTOR3, m_vDir, Dir);
	_SYNTHESIZE_REF_INHER( float, m_fRotation, Rotation );


	_SYNTHESIZE_INHER(bool, m_IsSmooth, IsSmooth);
	_SYNTHESIZE_INHER(bool, m_IsShakeEye, ShakeEye);

public:
	void	Setup( const D3DXVECTOR3&, const D3DXVECTOR3&, const D3DXVECTOR3&);
	virtual void	Update( bool IsMatrix = true);

	void	GetMatrixView(D3DXMATRIXA16* pOut);
	void	GetMatrixProj(D3DXMATRIXA16* pOut);
	void	RotationKey(const float&f);

public:
	cCamera();
	~cCamera();
};

