#include "DXUT.h"
#include "cAseLoader.h"
#include "cAseFile.h"
#include "cGeomNode.h"

cAseLoader::cAseLoader()
:m_pRootNode( NULL )
, m_IsFacesCopy( false )
, m_pFaces( NULL )
, m_vMin( FLT_MAX, FLT_MAX, FLT_MAX )
, m_vMax( FLT_MIN, FLT_MIN, FLT_MIN )
{
}

cAseLoader::~cAseLoader()
{
}


void	cAseLoader::LoadAseFile(cAseFile* pAseFile, char* pFileName)
{
	fopen_s(&m_fp, pFileName, "r");

	char* szToken = NULL;

	while (szToken = GetToken())
	{
		if (IsEqual(szToken, "*SCENE")) Scene(pAseFile);
		_IS("*MATERIAL_LIST") MaterialList(pAseFile->m_vecMtlTex);
		_IS("*GEOMOBJECT") GeomObject(pAseFile->m_vecMtlTex);
	}

	fclose(m_fp);
	m_pRootNode->CalcLocalTM(NULL);

	pAseFile->m_pRootNode = m_pRootNode;

	D3DXVECTOR3	vMinToMax = m_vMin + m_vMax;

	pAseFile->m_vModelCenter = vMinToMax / 2;
	pAseFile->m_fModelRadius = D3DXVec3Length(
		&(m_vMin - pAseFile->m_vModelCenter));
}

void	cAseLoader::Scene(cAseFile* pAseFile)
{
	_START
		_IS("*SCENE_FIRSTFRAME") pAseFile->m_nFirstFrame = GetInteger();
		_IS("*SCENE_LASTFRAME") pAseFile->m_nLastFrame = GetInteger();	
		_IS("*SCENE_TICKSPERFRAME") pAseFile->m_nTicksPerFrame = GetInteger();
		_IS("*SCENE_FRAMESPEED") pAseFile->m_nFrameSpeed = GetInteger();

	_END
}

void	cAseLoader::MaterialList(std::vector<ST_MTL_TEX>& vecMtlTex)
{
	int nLevel = 0;
	char* szToken = NULL;
	do
	{
		szToken = GetToken();

		if (IsEqual(szToken, "{")) ++nLevel;
		else if (IsEqual(szToken, "}")) --nLevel;
		else if (IsEqual(szToken, "*MATERIAL_COUNT")) vecMtlTex.resize(GetInteger());
		else if (IsEqual(szToken, "*MATERIAL")) Material(vecMtlTex[GetInteger()]);
	} while (nLevel > 0);
}

void	cAseLoader::Material(ST_MTL_TEX& MtlTex)
{
	int nLevel = 0;
	char* szToken = NULL;
	do
	{
		szToken = GetToken();

		if (IsEqual(szToken, "{")) ++nLevel;
		else if (IsEqual(szToken, "}")) --nLevel;
		else if (IsEqual(szToken, "*MAP_DIFFUSE")) MapDiffuse(MtlTex);
	} while (nLevel > 0);
}

void	cAseLoader::MapDiffuse(ST_MTL_TEX& MtlTex)
{
	int nLevel = 0;
	char* szToken = NULL;
	do{
		szToken = GetToken();
		if (IsEqual(szToken, "{")) ++nLevel;
		else if (IsEqual(szToken, "}")) --nLevel;
		else if (IsEqual(szToken, "*BITMAP"))
		{
			std::string strName = GetToken();
			D3DXCreateTextureFromFileA(DXUTGetD3D9Device(), strName.c_str(), &MtlTex.pTex);
			strName   = strName.substr(0, strName.length() - 4);
			strName += "_Bump.PNG";
			D3DXCreateTextureFromFileA(DXUTGetD3D9Device(), strName.c_str(), &MtlTex.pBump);
		}
	} while (nLevel > 0);
}

void	cAseLoader::GeomObject(std::vector<ST_MTL_TEX>& vecMtlTex)
{
	int nLevel = 0;
	char* szToken = NULL;
	std::string szName;

	do{
		szToken = GetToken();
		if (IsEqual(szToken, "{")) ++nLevel;
		else if (IsEqual(szToken, "}")) --nLevel;
		else if (IsEqual(szToken, "*NODE_NAME"))
		{
			szName = GetToken();
			m_mapGeomNode[szName] = new cGeomNode();

			m_mapGeomNode[szName]->m_strName = szName;
			if (m_pRootNode == NULL)
			{
				m_pRootNode = m_mapGeomNode[szName];
			}
			if (szName.substr(0, 3) == "Bip")
			{
				m_mapGeomNode[szName]->m_IsRender = false;
			}
		}
		else if (IsEqual(szToken, "*NODE_PARENT"))
		{
			char* szParentName = GetToken();
			m_mapGeomNode[szParentName]->AddChild(m_mapGeomNode[szName]);
		}
		else if (IsEqual(szToken, "*NODE_TM"))
		{
			NodeTM(m_mapGeomNode[szName]);
		}
		else if (IsEqual(szToken, "*MESH"))
		{
			Mesh(m_mapGeomNode[szName]);
		}
		else if (IsEqual(szToken, "*MATERIAL_REF"))
		{
			m_mapGeomNode[szName]->m_pMtlTex = &vecMtlTex[GetInteger()];
		}
		else if (IsEqual(szToken, "*TM_ANIMATION"))
		{
			TMAnimation(m_mapGeomNode[szName]);
		}
	} while (nLevel > 0);
}
void	cAseLoader::NodeTM(cGeomNode* pGeomNode)
{
	int nLevel = 0;
	char* szToken = NULL;
	do
	{
		szToken = GetToken();
		if (IsEqual(szToken, "{")) ++nLevel;
		else if (IsEqual(szToken, "}")) --nLevel;
		_IS("*TM_ROW0")
		{
			pGeomNode->m_matWorldTM._11 = GetFloat();
			pGeomNode->m_matWorldTM._13 = GetFloat();
			pGeomNode->m_matWorldTM._12 = GetFloat();
			pGeomNode->m_matWorldTM._14 = 0.0f;
		}
		_IS("*TM_ROW1")
		{
			pGeomNode->m_matWorldTM._31 = GetFloat();
			pGeomNode->m_matWorldTM._33 = GetFloat();
			pGeomNode->m_matWorldTM._32 = GetFloat();
			pGeomNode->m_matWorldTM._34 = 0.0f;
		}
		_IS("*TM_ROW2")
		{
			pGeomNode->m_matWorldTM._21 = GetFloat();
			pGeomNode->m_matWorldTM._23 = GetFloat();
			pGeomNode->m_matWorldTM._22 = GetFloat();
			pGeomNode->m_matWorldTM._24 = 0.0f;
		}
		_IS("*TM_ROW3")
		{
			pGeomNode->m_matWorldTM._41 = GetFloat();
			pGeomNode->m_matWorldTM._43 = GetFloat();
			pGeomNode->m_matWorldTM._42 = GetFloat();
			pGeomNode->m_matWorldTM._44 = 1.0f;
		}
	} while (nLevel > 0);
}

void	cAseLoader::Mesh(cGeomNode* pGeomNode)
{
	std::vector<ST_PNT_VERTEX> vecVertices;
	std::vector<D3DXVECTOR3>	vecV;
	std::vector<D3DXVECTOR2>	vecT;
	std::vector<DWORD>	vecIndices;


	_START
		_IS("*MESH_NUMVERTEX") vecV.resize(GetInteger());
	_IS("*MESH_NUMTVERTEX") vecT.resize(GetInteger());
	_IS("*MESH_NUMFACES") vecVertices.resize(GetInteger() * 3);
	_IS("*MESH_VERTEX_LIST") MeshVertexList(vecV);
	_IS("*MESH_FACE_LIST") MeshFaceList(vecV, vecIndices, vecVertices);
	_IS("*MESH_TVERTLIST") MeshTVertList(vecT);
	_IS("*MESH_TFACELIST") MeshTFaceList(vecT, vecVertices);
	_IS("*MESH_NORMALS") MeshNormal(vecVertices);
	_END
		;
	D3DXVECTOR3	vMin, vMax;

	D3DXComputeBoundingBox(&vecVertices[0].v, vecVertices.size(), 
		sizeof(ST_PNT_VERTEX)
		, &vMin, &vMax);

	vMin *= 0.35f;
	vMax *= 0.35f;

	D3DXVec3Minimize(&m_vMin, &m_vMin, &vMin);
	D3DXVec3Maximize(&m_vMax, &m_vMax, &vMax);

	D3DXMATRIXA16	matInvWorld;
	D3DXMatrixInverse(&matInvWorld, 0, &pGeomNode->m_matWorldTM);

	for (auto i = 0; i < vecVertices.size(); ++i)
	{
		D3DXVec3TransformCoord(&vecVertices[i].v, &vecVertices[i].v, &matInvWorld);
	}

	DXUTGetD3D9Device()->CreateVertexBuffer(sizeof(ST_PNT_VERTEX)* vecVertices.size(),
		0, ST_PNT_VERTEX::FVF, D3DPOOL_MANAGED, &pGeomNode->m_pVB, NULL);

	LPVOID	pVertices;
	pGeomNode->m_pVB->Lock(0, 0, &pVertices, 0);

	memcpy(pVertices, &vecVertices[0], sizeof(ST_PNT_VERTEX)* vecVertices.size());
	pGeomNode->m_pVB->Unlock();

	pGeomNode->m_nNumTri = vecVertices.size() / 3;



	D3DXCreateMeshFVF(vecVertices.size() / 3, vecVertices.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT
		, ST_PNT_VERTEX::FVF, DXUTGetD3D9Device(), &pGeomNode->m_pMesh);

	pGeomNode->m_pMesh->LockVertexBuffer(D3DLOCK_DISCARD, &pVertices);
	memcpy(pVertices, &vecVertices[0], vecVertices.size() * sizeof(ST_PNT_VERTEX));
	pGeomNode->m_pMesh->UnlockVertexBuffer();

	LPVOID pIndices;
	pGeomNode->m_pMesh->LockIndexBuffer(D3DLOCK_DISCARD, &pIndices);
	memcpy(pIndices, &vecIndices[0], vecIndices.size() * sizeof(DWORD));
	pGeomNode->m_pMesh->UnlockIndexBuffer();

	DWORD*	adjacencyInfo = new	DWORD[vecIndices.size()];
	pGeomNode->m_pMesh->GenerateAdjacency(0.0f, adjacencyInfo);
	DWORD*	OptimizeInfo = new	DWORD[vecIndices.size()];

	pGeomNode->m_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | 
		D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, adjacencyInfo, 
		OptimizeInfo, 0, 0);

	delete[]	OptimizeInfo;

	pGeomNode->m_pMesh->GenerateAdjacency(0.1f, adjacencyInfo);

	// normal vector 계산... 
	D3DXComputeNormals(pGeomNode->m_pMesh, adjacencyInfo);

	// tangent vector 계산... 
	D3DXComputeTangent(pGeomNode->m_pMesh, 0, 0, 0, 0, adjacencyInfo);

	

	delete[]	adjacencyInfo;


}

void	cAseLoader::MeshVertexList(std::vector<D3DXVECTOR3>& vecV)
{
	_START
		_IS("*MESH_VERTEX")
	{
			int nIdx = GetInteger();
			vecV[nIdx].x = GetFloat();
			vecV[nIdx].z = GetFloat();
			vecV[nIdx].y = GetFloat();
		}
	_END
}

void	cAseLoader::MeshFaceList(std::vector<D3DXVECTOR3>& vecV,
	std::vector<DWORD>& vecIn, 
	std::vector<ST_PNT_VERTEX>& vecVertices)
{
	_START
		_IS("*MESH_FACE")
	{
			int	nFaceIdx = GetInteger();
			GetToken();
			vecVertices[nFaceIdx * 3 + 0].v = vecV[GetInteger()];
			GetToken();
			vecVertices[nFaceIdx * 3 + 2].v = vecV[GetInteger()];
			GetToken();
			vecVertices[nFaceIdx * 3 + 1].v = vecV[GetInteger()];
			vecIn.push_back(nFaceIdx * 3 + 0);
			vecIn.push_back(nFaceIdx * 3 + 1);
			vecIn.push_back(nFaceIdx * 3 + 2);
			if (m_IsFacesCopy)
			{
				m_pFaces = &m_pRootNode->m_vecFaces;
				if (m_pFaces != NULL)
				{
					m_pFaces->push_back(vecVertices[nFaceIdx * 3 + 0].v);
					m_pFaces->push_back(vecVertices[nFaceIdx * 3 + 1].v);
					m_pFaces->push_back(vecVertices[nFaceIdx * 3 + 2].v);
				}

			}

		}
	_END
}

void	cAseLoader::MeshTVertList(std::vector<D3DXVECTOR2>& vecT)
{
	_START
		_IS("*MESH_TVERT")
	{
			int nIdx = GetInteger();
			vecT[nIdx].x = GetFloat();
			vecT[nIdx].y = 1.0f - GetFloat();
		}
	_END
}

void	cAseLoader::MeshTFaceList(std::vector<D3DXVECTOR2>& vecT, std::vector<ST_PNT_VERTEX>& vecVertices)
{
	_START
		_IS("*MESH_TFACE")
	{
			int nFaceIdx = GetInteger();
			vecVertices[nFaceIdx * 3 + 0].t = vecT[GetInteger()];
			vecVertices[nFaceIdx * 3 + 2].t = vecT[GetInteger()];
			vecVertices[nFaceIdx * 3 + 1].t = vecT[GetInteger()];
		}
	_END
}

void	cAseLoader::MeshNormal(std::vector<ST_PNT_VERTEX>& vecVertices)
{
	int	nFaceIdx = 0;
	int	nCnt = 0;
	int	nIdx[3] = { 0, 2, 1 };
	_START
		_IS("*MESH_VERTEXNORMAL")
	{
			GetInteger();
			D3DXVECTOR3	n;
			n.x = GetFloat();
			n.z = GetFloat();
			n.y = GetFloat();

			vecVertices[3 * nFaceIdx + nIdx[nCnt++]].n = n;
		}
	_IS("*MESH_FACENORMAL")
	{
		nFaceIdx = GetInteger();
		nCnt = 0;
	}
	_END
}

void	cAseLoader::TMAnimation(cGeomNode* pGeomNode)
{
	_START
		_IS("*CONTROL_POS_TRACK")
	{
			ControlPosTrack(pGeomNode);
		}
	_IS("*CONTROL_ROT_TRACK")
	{
		ControlRotTrack(pGeomNode);
	}
	_END
}

void	cAseLoader::ControlPosTrack(cGeomNode* pGeomNode)
{
	_START
		_IS("*CONTROL_POS_SAMPLE")
	{
			ST_POS_TRACK	pt;
			pt.nKey = GetInteger();
			pt.v.x = GetFloat();
			pt.v.z = GetFloat();
			pt.v.y = GetFloat();

			pGeomNode->m_vecPosTrack.push_back(pt);
		}
	_END
}

void	cAseLoader::ControlRotTrack(cGeomNode* pGeomNode)
{
	_START
		_IS("*CONTROL_ROT_SAMPLE")
	{
			ST_ROT_TRACK	rt;
			rt.nKey = GetInteger();
			rt.q.x = GetFloat();
			rt.q.z = GetFloat();
			rt.q.y = GetFloat();
			rt.q.w = GetFloat();

			rt.q.x = sin(rt.q.w / 2.0f) * rt.q.x;
			rt.q.y = sin(rt.q.w / 2.0f) * rt.q.y;
			rt.q.z = sin(rt.q.w / 2.0f) * rt.q.z;
			rt.q.w = cos(rt.q.w / 2.0f) ;

			if (!pGeomNode->m_vecRotTrack.empty())
			{
				rt.q = pGeomNode->m_vecRotTrack.back().q * rt.q;
			}
			pGeomNode->m_vecRotTrack.push_back(rt);
		}
	_END
}