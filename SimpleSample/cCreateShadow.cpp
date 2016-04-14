#include "DXUT.h"
#include "cCreateShadow.h"
#include "cObjectMgr.h"
#include "cCamera.h"

cCreateShadow::cCreateShadow()
{
}


cCreateShadow::~cCreateShadow()
{
}
void	cCreateShadow::Setting(LPDIRECT3DTEXTURE9 pTex,
	D3DXMATRIXA16* pWorldMatrix, cObjBase* pObject)
{
	cBaseShader::Setting(pTex, pWorldMatrix, pObject);

	D3DXMATRIXA16	matView, matProj;

	_GETSINGLE(cObjectMgr)->GetShadowCamera()->GetMatrixView(&matView);
	_GETSINGLE(cObjectMgr)->GetShadowCamera()->GetMatrixProj( &matProj );


	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI * 0.4f, 1, 
		10, 10000);

	m_pEffect->SetMatrix((D3DXHANDLE)"g_LightView", &matView);
	m_pEffect->SetMatrix((D3DXHANDLE)"g_LightProj", &matProj);

	if (pTex != NULL)
	{
		m_pEffect->SetTexture((D3DXHANDLE)"g_MeshTexture", pTex);
	}
}