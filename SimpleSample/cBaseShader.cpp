#include "DXUT.h"
#include "cBaseShader.h"


cBaseShader::cBaseShader()
{
}


cBaseShader::~cBaseShader()
{
	if (m_pEffect != NULL)
	{
		m_pEffect->Release();
		m_pEffect = NULL;
	}
}


void	cBaseShader::Setting(LPDIRECT3DTEXTURE9 pTex,
	D3DXMATRIXA16* pWorldMatrix, cObjBase* pObject)
{
	D3DXMATRIXA16	matView, matProj;

	DXUTGetD3D9Device()->GetTransform(D3DTS_PROJECTION, &matProj);
	DXUTGetD3D9Device()->GetTransform(D3DTS_VIEW, &matView );

	D3DXMATRIXA16	matWorldViewProj = (*pWorldMatrix) * matView * matProj;

	m_pEffect->SetMatrix((D3DXHANDLE)"g_mWorldViewProjection", &matWorldViewProj);
	m_pEffect->SetMatrix((D3DXHANDLE)"g_mWorld", pWorldMatrix);
}