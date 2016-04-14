#include "DXUT.h"
#include "cEffect.h"
#include "cResourceFile.h"
#include "cResourceMgr.h"
#include "cSystemMgr.h"
#include "cObjectMgr.h"

cEffect::cEffect()
:m_IsRepeat( false )
, m_IsBillBoard( true )
, m_IsAlphaChange( false )
, m_IsAlphaMinus( true )
, m_IsAlphaDestroy( false )
, m_IsZEnable( true )
{
}


cEffect::~cEffect()
{
}

void	cEffect::Initialize()
{
	m_fAniKey = 0.0f;
	m_pResource = _GETSINGLE(cResourceMgr)->GetResource("effect_bullet");
	m_vScale *= 0.2f;
	m_vPosition.z += rand() % 1000 * 0.0001f;
	m_vPosition.x += (rand() % 1000 - 500) * 0.00001f;
}
void	cEffect::Update()
{
	if (m_IsAlphaDestroy)
	{
		m_fAlpha -= _GETS(cSystemMgr)->GetDeltaTime() * 4;
		if (m_fAlpha < 0.0f)
		{
			m_IsDelete = true;
		}
	}
	if (m_IsAlphaChange)
	{
		if (m_IsAlphaMinus)
		{
			m_fAlpha -= _GETS(cSystemMgr)->GetDeltaTime() * 1.2f;
			m_vScale.z -= 0.025f * _GETS(cSystemMgr)->GetDeltaTime();
			if (m_fAlpha < 0.3f)
			{
				m_fAlpha = 0.3f;
				m_IsAlphaMinus = false;
			}
		}
		else
		{
			m_fAlpha += _GETS(cSystemMgr)->GetDeltaTime() * 1.2f;
			m_vScale.z += 0.025f * _GETS(cSystemMgr)->GetDeltaTime();

			if (m_fAlpha > 1.0f)
			{
				m_fAlpha = 1.0f;
				m_IsAlphaMinus = true;
			}
		}
	}
	this->Translation();
}
void	cEffect::Render()
{
	_GETS(cObjectMgr)->SetIsMap(true);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_ZWRITEENABLE, false);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_ZENABLE, m_IsZEnable);
	this->Translation();
	m_pResource->Render(this, _GETSINGLE(cResourceMgr)->GetShader(SI_CUSTOM));
	DXUTGetD3D9Device()->SetRenderState(D3DRS_ZENABLE, true);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_ZWRITEENABLE, true);
	_GETS(cObjectMgr)->SetIsMap(false);

}

void	cEffect::Translation()
{
	D3DXMATRIXA16	matScale, matRot, matTrans;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	{
		D3DXQUATERNION	q;
		D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, m_vRotation.x, m_vRotation.z);
		D3DXMatrixRotationQuaternion(&matRot, &q);

	}
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	D3DXMATRIXA16	matView;
	if (m_IsBillBoard)
	{
		DXUTGetD3D9Device()->GetTransform(D3DTS_VIEW, &matView);

		memcpy(&matView._41, D3DXVECTOR3(0, 0, 0), sizeof(D3DXVECTOR3));

		D3DXMatrixTranspose(&matView, &matView);
	}
	else
	{
		D3DXMatrixIdentity(&matView);
	}

	m_matWorld = matScale	*	matRot	 * matView *	matTrans;
}