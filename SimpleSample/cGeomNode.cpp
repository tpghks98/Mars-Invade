#include "DXUT.h"
#include "cGeomNode.h"
#include "cBaseShader.h"
#include "cObjectMgr.h"
#include "cResourceMgr.h"

cGeomNode::cGeomNode()
:m_nNumTri( 0 )
,m_IsRender( true )
, m_pMtlTex( NULL )
{
	D3DXMatrixIdentity(&m_matWorldTM);
	D3DXMatrixIdentity(&m_matLocalTM);
}


cGeomNode::~cGeomNode()
{
	Release();
}

void	cGeomNode::Update(int nKey, D3DXMATRIXA16* pmatParent)
{
	D3DXMATRIXA16	mT, mR;

	AnimLocalPos(nKey, mT);
	AnimLocalRot(nKey, mR);

	m_matLocalTM = mR	*	mT;

	if (pmatParent)
	{
		m_matWorldTM = m_matLocalTM * (*pmatParent);
	}
	else
	{
		m_matWorldTM = m_matLocalTM;
	}
	for each(auto Child in m_vecChildren)
	{
		Child->Update(nKey, &m_matWorldTM);
	}
}

void	cGeomNode::Render(cObjBase* pObj , cBaseShader* pShader )
{
	if (m_IsRender)
	{

		if (pShader == NULL)
		{
			if (m_pMtlTex)
			{
				DXUTGetD3D9Device()->SetTexture(0, m_pMtlTex->pTex);
			}
			DrawGeom();
		}
		else
		{
			LPDIRECT3DTEXTURE9	pTex = NULL;
			if (m_pMtlTex != NULL)
			{
				pTex = m_pMtlTex->pTex;
				if (m_pMtlTex->pBump != NULL)
				{
					_GETSINGLE(cResourceMgr)->SetBumpTex(m_pMtlTex->pBump);
				}
				else
				{
					_GETSINGLE(cResourceMgr)->SetBumpTex(NULL);
				}
			}

			pShader->Setting(pTex, &m_matWorldTM, pObj);

			UINT nPass = 0;
			pShader->GetEffect()->Begin(&nPass, 0);
			{
				for (int n = 0; n < nPass; ++n)
				{
					pShader->GetEffect()->BeginPass(n);
					DrawGeom();
					pShader->GetEffect()->EndPass();
				}
			}
			pShader->GetEffect()->End();
		}
	}

	for each(auto iter in m_vecChildren)
	{
		iter->Render(pObj, pShader);
	}
}


void	cGeomNode::DrawGeom()
{
	DXUTGetD3D9Device()->SetTransform(D3DTS_WORLD, &m_matWorldTM);

	if (_GETSINGLE(cObjectMgr)->GetIsBump())
	{
		m_pMesh->DrawSubset(0);

	}
	else
	{
		DXUTGetD3D9Device()->SetStreamSource(0, m_pVB, 0, sizeof(ST_PNT_VERTEX));
		DXUTGetD3D9Device()->SetFVF(ST_PNT_VERTEX::FVF);

		DXUTGetD3D9Device()->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_nNumTri);

	}
	
	
}

void	cGeomNode::Release()
{
	SAFE_RELEASE(m_pVB);

	for each(auto Child in m_vecChildren)
	{
		SAFE_DELETE(Child);
	}
	SAFE_RELEASE(m_pMesh);
	m_vecChildren.clear();
}

void	cGeomNode::AddChild(cGeomNode* pGeomNode)
{
	m_vecChildren.push_back(pGeomNode);
}

void	cGeomNode::CalcLocalTM(D3DXMATRIXA16* pmatParentTM)
{
	if (pmatParentTM == NULL)
	{
		m_matLocalTM = m_matWorldTM;
	}
	else
	{
		D3DXMATRIXA16	matInvParent;
		D3DXMatrixInverse(&matInvParent, 0, pmatParentTM);
		m_matLocalTM = m_matWorldTM * matInvParent;
	}

	for each(auto Child in m_vecChildren)
	{
		Child->CalcLocalTM(&this->m_matWorldTM);
	}
}

void	cGeomNode::AnimLocalPos(int nKey, D3DXMATRIXA16& mT)
{
	D3DXMatrixIdentity(&mT);
	if (m_vecPosTrack.empty())
	{
		mT._41 = m_matLocalTM._41;
		mT._42 = m_matLocalTM._42;
		mT._43 = m_matLocalTM._43;
	}
	else
	{
		D3DXVECTOR3	v;
		AnimLerp(nKey, v);
		mT._41 = v.x;
		mT._42 = v.y;
		mT._43 = v.z;
	}
}
void	cGeomNode::AnimLocalRot(int nKey, D3DXMATRIXA16& mR)
{
	D3DXMatrixIdentity(&mR);

	if (m_vecRotTrack.empty())
	{
		mR = m_matLocalTM;
		mR._41 = 0.0f;
		mR._42 = 0.0f;
		mR._43 = 0.0f;
	}
	else
	{
		D3DXQUATERNION	q;
		AnimSLerp(nKey, q);
		D3DXMatrixRotationQuaternion(&mR, &q);
	}
}


void	cGeomNode::AnimLerp(int nKey, D3DXVECTOR3& v)
{
	if (nKey <= m_vecPosTrack[0].nKey)
	{
		v = m_vecPosTrack[0].v;
	}
	else if (nKey >= m_vecPosTrack.back().nKey)
	{
		v = m_vecPosTrack.back().v;
	}
	else
	{
		int	nNextIdx = 0;
		for (int n = 0; n < m_vecPosTrack.size(); ++n)
		{
			if (nKey < m_vecPosTrack[n].nKey)
			{
				nNextIdx = n;
				break;
			}
		}

		int	nPrevIdx = nNextIdx - 1;

		float	t = (nKey - m_vecPosTrack[nPrevIdx].nKey)
			/ (float)(m_vecPosTrack[nNextIdx].nKey - m_vecPosTrack[nPrevIdx].nKey);
		D3DXVec3Lerp(&v, &m_vecPosTrack[nPrevIdx].v, &m_vecPosTrack[nNextIdx].v , t );
	}
}

void	cGeomNode::AnimSLerp(int nKey, D3DXQUATERNION& q)
{
	if (nKey <= m_vecRotTrack[0].nKey)
	{
		q = m_vecRotTrack[0].q;
	}
	else if (nKey >= m_vecRotTrack.back().nKey)
	{
		q = m_vecRotTrack.back().q;
	}
	else
	{
		int	nNextIdx = 0;
		for (int n = 0; n < m_vecRotTrack.size(); ++n)
		{
			if (nKey < m_vecRotTrack[n].nKey)
			{
				nNextIdx = n;
				break;
			}
		}

		int nPrevIdx = nNextIdx - 1;
		float	t = (nKey - m_vecRotTrack[nPrevIdx].nKey)
			/ (float)(m_vecRotTrack[nNextIdx].nKey - m_vecRotTrack[nPrevIdx].nKey);

		D3DXQuaternionSlerp(&q, &m_vecRotTrack[nPrevIdx].q, &m_vecRotTrack[nNextIdx].q, t);


	}
}

D3DXMATRIXA16*	cGeomNode::GetWorldMatrix(char* pNodeName)
{
	if (m_strName == pNodeName)
	{
		return &m_matWorldTM;
	}
	else
	{
		for (int n = 0; n < m_vecChildren.size(); ++n)
		{
			auto pWorld = m_vecChildren[n]->GetWorldMatrix(pNodeName);

			if (pWorld != NULL)
			{
				return pWorld;
			}
		}

		return NULL;
	}
}

void	cGeomNode::GetAllFaces(std::vector<D3DXVECTOR3>& vecFaces)
{
	for each(auto it in m_vecFaces)
	{
		vecFaces.push_back(it);
	}

	for each(auto iter in m_vecChildren)
	{
		iter->GetAllFaces(vecFaces);
	}
}
