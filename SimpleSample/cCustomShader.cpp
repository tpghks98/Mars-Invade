#include "DXUT.h"
#include "cCustomShader.h"
#include "cSystemMgr.h"
#include "cCamera.h"
#include "cObjBase.h"
#include "cAseFile.h"
#include "cObjectMgr.h"
#include "cResourceMgr.h"
cCustomShader::cCustomShader()
{
}


cCustomShader::~cCustomShader()
{
}


void	cCustomShader::Setting(LPDIRECT3DTEXTURE9 pTex,
	D3DXMATRIXA16* pWorldMatrix, cObjBase* pObject)
{
	cBaseShader::Setting(pTex, pWorldMatrix, pObject);


	D3DXVECTOR4	v;

	D3DXMATRIXA16	matView, matProj, matInvWorldView;

	DXUTGetD3D9Device()->GetTransform(D3DTS_VIEW, &matView);
	DXUTGetD3D9Device()->GetTransform(D3DTS_PROJECTION, &matProj);


	matInvWorldView = (*pWorldMatrix) * matView;
	D3DXMatrixInverse(&matInvWorldView, NULL, &matInvWorldView);
	D3DXMatrixTranspose(&matInvWorldView, &matInvWorldView);

	m_pEffect->SetMatrix((D3DXHANDLE)"g_mInvWorldView", &matInvWorldView);
	m_pEffect->SetMatrix((D3DXHANDLE)"g_mProj", &matProj);
	v = D3DXVECTOR4(matView._41, matView._42, matView._43, 0);
	v = D3DXVECTOR4( _GETSINGLE(cSystemMgr)->GetMainCamera()->GetEye(), 0  );

	m_pEffect->SetVector((D3DXHANDLE)"g_vCameraPos", &v);
	m_pEffect->SetBool((D3DXHANDLE)"g_IsMap", _GETSINGLE(cObjectMgr)->GetIsMap());
	m_pEffect->SetBool((D3DXHANDLE)"g_IsDiffuse", _GETSINGLE(cObjectMgr)->GetIsDiffuse());
	
	if (_GETSINGLE(cResourceMgr)->GetBumpTex() != NULL)
	{
		m_pEffect->SetTexture((D3DXHANDLE)"g_BumpTexture",
			_GETSINGLE(cResourceMgr)->GetBumpTex());
		m_pEffect->SetBool((D3DXHANDLE)"g_IsBump", true);
	}
	else
	{
		m_pEffect->SetBool((D3DXHANDLE)"g_IsBump", false);
	}

	v = v - (D3DXVECTOR4(_GETSINGLE(cSystemMgr)->GetMainCamera()->GetAt(), 0));

	D3DXVec4Normalize(&v, &v);

	m_pEffect->SetVector((D3DXHANDLE)"g_vViewDir", &v);
	v = D3DXVECTOR4(0.0f, 1.0f, 1.0f, 0.0f);
	m_pEffect->SetVector((D3DXHANDLE)"g_LightDir[0]", &v);
	v = D3DXVECTOR4(0.5f, -1.0f, 0.5f, 0.0f);
	m_pEffect->SetVector((D3DXHANDLE)"g_LightDir[1]", &v);
	v = D3DXVECTOR4(0.0f, 1.0f, -1.0f, 0.0f);
	m_pEffect->SetVector((D3DXHANDLE)"g_LightDir[2]", &v);
	v = D3DXVECTOR4(-1.0f, 1.0f, 0.0f, 0.0f);
	m_pEffect->SetVector((D3DXHANDLE)"g_LightDir[3]", &v);
	v = D3DXVECTOR4(0.0f, 1, 0, 0.0f);
	m_pEffect->SetVector((D3DXHANDLE)"g_LightDir[4]", &v);

	if (pTex != NULL)
	{
		m_pEffect->SetTexture((D3DXHANDLE)"g_MeshTexture", pTex);
	}
	m_pEffect->SetFloat((D3DXHANDLE)"g_fAlpha", pObject->GetAlpha());
	m_pEffect->SetFloat((D3DXHANDLE)"g_fHitColor", pObject->GetHitColor());

}