#pragma once

#include "value.h"

class cResourceFile;
class cBaseShader;
class cResourceMgr
	:public cSingleTon<cResourceMgr>
{
private:
	std::map<std::string, cResourceFile*> m_mapResource;
	std::vector<cBaseShader*>	m_vecShader;

	_SYNTHESIZE_INHER(LPDIRECT3DTEXTURE9, m_pBumpTex, BumpTex);
	_SYNTHESIZE_INHER(cResourceFile*, m_pPlan, Plan);
	_SYNTHESIZE_REF_INHER(float, m_fPerRect, PerRect);
public:
	void	AddAseFile(char* pKey, char* pFileName);
	void	AddShader(int nID, char* pFileName);
	void	AddTexture(char* pKey, LPCWSTR pFileName, int nFrame = 1);
	void	AddSprite(char* pKey, LPCWSTR pFileName, int nFrame = 1);

	cResourceFile*	GetResource(char* pKey);
	cBaseShader*	GetShader(int nID) { return m_vecShader[nID]; }
public:
	cResourceMgr();
	~cResourceMgr();
};

