#include "DXUT.h"
#include "cCamera.h"
#include "cObjBase.h"
#include "cSystemMgr.h"
#include "cPlayer.h"

cCamera::cCamera()
:m_vEye( g_vZero )
, m_vAt( g_vZero )
, m_vUp( D3DXVECTOR3(0, 1, 0 ) )
, m_pTarget( NULL )
, m_fDist( 1.0f )
, m_IsSmooth( false )
, m_vDir( 0, -1, 1 )
, m_fMoveSpeed( 8.0f )
, m_fRotation( 0.0f )
, m_vPlusDir( 0, 0, 0)
, m_vShake( 0, 0 , 0)
, m_vPrevShake( 0, 0, 0 )
, m_IsShakeEye( false )
{
}


cCamera::~cCamera()
{
}


void	cCamera::Setup(const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt
	, const D3DXVECTOR3& vUp)
{
	m_vEye = vEye;
	m_vAt = vAt;
	m_vUp = vUp;

	D3DVIEWPORT9 vp;
	DXUTGetD3D9Device()->GetViewport(&vp);

	D3DXMATRIXA16	matProj, matView;

	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI * 0.6f, vp.Width / vp.Height, 0.01f, 1000.0f);
	D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vAt, &m_vUp);

	DXUTGetD3D9Device()->SetTransform(D3DTS_PROJECTION, &matProj);
	DXUTGetD3D9Device()->SetTransform(D3DTS_VIEW, &matView );
}
void	cCamera::Update(bool IsMatrix)
{
	D3DXVECTOR3 vEye = m_vEye;
	D3DXVECTOR3 vAt = m_vAt;

	if (m_pTarget != NULL)
	{
		vAt = m_pTarget->GetPosition() + m_vDir * m_fDist;

		D3DXMATRIXA16	matRot;
		D3DXMatrixRotationAxis(&matRot, &m_vUp, m_fRotation);
		D3DXMatrixRotationY(&matRot, m_fRotation);

		D3DXVECTOR3	vDir = m_vDir;

	//	D3DXVec3TransformCoord(&vDir, &vDir, &matRot);
		vEye = m_pTarget->GetPosition() - (vDir + m_vPlusDir) * m_fDist * 2;
		m_fRotation += (0.0f - m_fRotation) * _GETSINGLE(cSystemMgr)->GetDeltaTime() * 4.0f;
	}

	if (m_IsSmooth)
	{
		float fLength = 
			D3DXVec3Length(&(m_vAt - vAt));
		m_fMoveSpeed = fLength * 0.5f;
		if (m_fMoveSpeed < 10.0f)
		{
			m_fMoveSpeed = 10.0f;
		}

		float fTime = _GETSINGLE(cSystemMgr)->GetDeltaTime();
		D3DXVec3Lerp(&vAt, &m_vAt, &vAt, fTime * m_fMoveSpeed);
		D3DXVec3Lerp(&vEye, &m_vEye, &vEye, fTime * m_fMoveSpeed );
	}

	D3DXVECTOR3 vShake;
	D3DXVec3Lerp(&vShake, &m_vPrevShake, &m_vShake,
		_GETS(cSystemMgr)->GetDeltaTime() * 10.0f);

	m_vPrevShake = m_vShake;
	m_vAt = vAt + m_vShake * _GETSINGLE( cSystemMgr )->GetTimeScale();
	m_vEye = vEye ;
	if (m_IsShakeEye)
	{
		m_vEye += vShake;
	}

	m_vShake += -(m_vShake)* _GETSINGLE(cSystemMgr)->GetDeltaTime();

	if (IsMatrix)
	{
		D3DXMATRIXA16  matView;
		D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vAt, &m_vUp);

		DXUTGetD3D9Device()->SetTransform(D3DTS_VIEW, &matView);
	}


}


void	cCamera::GetMatrixView(D3DXMATRIXA16* pOut){
	D3DXMatrixLookAtLH(pOut, &m_vEye, &m_vAt, &m_vUp);
}
void	cCamera::GetMatrixProj(D3DXMATRIXA16* pOut){
	D3DVIEWPORT9 vp;
	DXUTGetD3D9Device()->GetViewport(&vp);

	D3DXMatrixPerspectiveFovLH(pOut, D3DX_PI * 0.28f, 1366 / 768, 0.1f, 1000.0f);

}

void	cCamera::RotationKey(const float&f)
{
	m_fRotation += f * _GETSINGLE(cSystemMgr)->GetDeltaTime();
}
