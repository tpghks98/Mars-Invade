#include "DXUT.h"
#include "cApplyShadow.h"
#include "cObjectMgr.h"
#include "cSystemMgr.h"
#include "cCamera.h"
#include "cResourceMgr.h"

cApplyShadow::cApplyShadow()
{
}


cApplyShadow::~cApplyShadow()
{
}
void	cApplyShadow::Setting(LPDIRECT3DTEXTURE9 pTex,
	D3DXMATRIXA16* pWorldMatrix, cObjBase* pObject)
{

	cBaseShader::Setting(pTex, pWorldMatrix, pObject);


	D3DXMATRIXA16	matView, matProj;

	_GETSINGLE(cObjectMgr)->GetShadowCamera()->GetMatrixView(&matView);
	_GETSINGLE(cObjectMgr)->GetShadowCamera()->GetMatrixProj(&matProj);


	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI * 0.4f, 1,
		10, 10000);

	m_pEffect->SetMatrix((D3DXHANDLE)"g_LightView", &matView);
	m_pEffect->SetMatrix((D3DXHANDLE)"g_LightProj", &matProj);

	auto p = _GETSINGLE(cObjectMgr)->GetShadowTex();
	m_pEffect->SetTexture( (D3DXHANDLE)"g_sh",
		 p);
	if (pTex != NULL)
	{
		m_pEffect->SetTexture((D3DXHANDLE)"g_MeshTexture", pTex);
	}

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

}