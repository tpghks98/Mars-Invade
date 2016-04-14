#pragma once

#include "value.h"

class cObjBase;
class cBaseShader;
class cGeomNode
{
public:

	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	int						m_nNumTri;
	bool					m_IsRender;

	ST_MTL_TEX*				m_pMtlTex;

	std::vector<cGeomNode*>	m_vecChildren;
	std::vector<ST_POS_TRACK>	m_vecPosTrack;
	std::vector<ST_ROT_TRACK>	m_vecRotTrack;

	D3DXMATRIXA16		m_matWorldTM;
	D3DXMATRIXA16		m_matLocalTM;
	std::string			m_strName;

	std::vector<D3DXVECTOR3>	m_vecFaces;
	LPD3DXMESH			m_pMesh;

public:
	void	Update(int nKey, D3DXMATRIXA16* pmatParent);
	void	Render(cObjBase* pObj = NULL, cBaseShader* pShader = NULL);
	void	DrawGeom();
	void	Release();

	void	AddChild(cGeomNode* pGeomNode);
	void	CalcLocalTM(D3DXMATRIXA16* pmatParentTM);

	void	AnimLocalPos(int nKey, D3DXMATRIXA16& mT);
	void	AnimLocalRot(int nKey, D3DXMATRIXA16& mR);

	void	AnimLerp(int nKey, D3DXVECTOR3& v);
	void	AnimSLerp(int nKey, D3DXQUATERNION& q);

	D3DXMATRIXA16*	GetWorldMatrix(char* pNodeName);
	void	GetAllFaces(std::vector<D3DXVECTOR3>& vecFaces);
public:
	cGeomNode();
	~cGeomNode();
};

