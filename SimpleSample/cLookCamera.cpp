#include "DXUT.h"
#include "cLookCamera.h"
#include "cObjBase.h"
#include "cSystemMgr.h"

cLookCamera::cLookCamera()
:m_fTime( 0.0f )
, m_fTimeSpeed( 1.0f )
{
}


cLookCamera::~cLookCamera()
{
}


void	cLookCamera::Update(bool IsMatrix)
{

	m_fTime += _GETS(cSystemMgr)->GetDeltaTime() * 0.25f * m_fTimeSpeed;
	if (m_fTime >= 0.5f && m_fTime <= 0.65f)
	{
		m_fTime -= _GETS(cSystemMgr)->GetDeltaTime() * 0.1f * m_fTimeSpeed;
	}
	if (m_fTime > 0.75f)
	{
		m_fTime += _GETS(cSystemMgr)->GetDeltaTime() * 0.5f;
	}
	if (m_fTime > 1.0f)
	{
		m_IsDelete = true;
	}
	D3DXVECTOR3 vEye = m_vEye;
	D3DXVECTOR3 vAt = m_vAt;

	if (m_pTarget != NULL)
	{
		vAt = m_pTarget->GetPosition();

		float fTime = m_fTime;

		float f = cos(fTime * D3DX_PI);
		if (f <= 0.0f)
		{
			f *= -1.0f;
		}
		f = 1.0f - f;

		vAt.x = _GETS(cSystemMgr)->GetLerp(_GETSINGLE(cSystemMgr)->GetMainCamera()->GetAt().x, vAt.x, f);
		vAt.y = _GETS(cSystemMgr)->GetLerp(_GETSINGLE(cSystemMgr)->GetMainCamera()->GetAt().y, vAt.y, f);
		vAt.z = _GETS(cSystemMgr)->GetLerp(_GETSINGLE(cSystemMgr)->GetMainCamera()->GetAt().z, vAt.z, f);
		vEye.x = _GETS(cSystemMgr)->GetLerp(_GETSINGLE(cSystemMgr)->GetMainCamera()->GetEye().x, vEye.x, f);
		vEye.y = _GETS(cSystemMgr)->GetLerp(_GETSINGLE(cSystemMgr)->GetMainCamera()->GetEye().y, vEye.y, f);
		vEye.z = _GETS(cSystemMgr)->GetLerp(_GETSINGLE(cSystemMgr)->GetMainCamera()->GetEye().z, vEye.z, f);
	}

	if (IsMatrix)
	{
		D3DXMATRIXA16  matView;
		D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &m_vUp);

		DXUTGetD3D9Device()->SetTransform(D3DTS_VIEW, &matView);
	}
}
