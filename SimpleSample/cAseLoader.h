#pragma once

#include "value.h"

class cAseFile;
class cGeomNode;
class cAseLoader
{
public:
	FILE*	m_fp;

	char	m_szToken[1024];
	std::map<std::string, cGeomNode*> m_mapGeomNode;
	cGeomNode*				m_pRootNode;
	

	bool	m_IsFacesCopy;
	std::vector<D3DXVECTOR3>*	m_pFaces;
public:
	void	LoadAseFile(cAseFile* pAseFile, char* pFileName);

	void	Scene(cAseFile* pAseFile);
	void	MaterialList(std::vector<ST_MTL_TEX>& vecMtlTex);
	void	Material(ST_MTL_TEX& MtlTex);
	void	MapDiffuse(ST_MTL_TEX& MtlTex);

	void	GeomObject(std::vector<ST_MTL_TEX>& vecMtlTex);
	void	NodeTM(cGeomNode* pGeomNode);

	void	Mesh(cGeomNode* pGeomNode);
	void	MeshVertexList(std::vector<D3DXVECTOR3>& vecV);
	void	MeshFaceList(std::vector<D3DXVECTOR3>& vecV,
		std::vector<DWORD>& vecIn,
		std::vector<ST_PNT_VERTEX>& vecVertices);
	void	MeshTVertList(std::vector<D3DXVECTOR2>& vecT);
	void	MeshTFaceList(std::vector<D3DXVECTOR2>& vecT, std::vector<ST_PNT_VERTEX>& vecVertices);
	void	MeshNormal(std::vector<ST_PNT_VERTEX>& vecVertices);

	void	TMAnimation(cGeomNode* pGeomNode);
	void	ControlPosTrack(cGeomNode* pGeomNode);
	void	ControlRotTrack(cGeomNode* pGeomNode);

	bool	IsEqual(char* str1, char* str2) { return strcmp(str1, str2) == 0; }
	bool	IsWhiteSpace(char c) { return c < 33;  }

	char*	GetToken()
	{
		ZeroMemory(m_szToken, sizeof(m_szToken));

		int		nCnt	=	0;
		bool	IsQuit	=	false;
		char	c;

		while (!feof(m_fp))
		{
			c = (char)fgetc(m_fp);

			if (c == '\"' )
			{
				if (IsQuit)
				{
					break;
				}
				else
				{
					IsQuit = true;
					continue;
				}
			}

			if (IsWhiteSpace(c) && !IsQuit)
			{
				if (nCnt == 0)
				{
					continue;
				}
				else
				{
					break;
				}
			}

			m_szToken[nCnt++] = c;
		}

		if (nCnt == 0)
			return NULL;

		return m_szToken;
	}

	int	GetInteger()	{
		return atoi(GetToken());
	}
	float GetFloat() {
		return atof(GetToken());
	}
public:
	D3DXVECTOR3	m_vMin;
	D3DXVECTOR3	m_vMax;

public:
	cAseLoader();
	~cAseLoader();
};

