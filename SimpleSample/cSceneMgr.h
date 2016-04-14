#pragma once

#include "value.h"
#include "cNode.h"
class cScene;
class cObjBase;
class cSceneMgr
	:public cSingleTon<cSceneMgr>, public cNode
{
private:
	_SYNTHESIZE_INHER(cScene*, m_pScene, Scene);
	_SYNTHESIZE_INHER(int, m_nSceneID, SceneID);
	_SYNTHESIZE_INHER(bool, m_IsChange , IsChange );
	_SYNTHESIZE_REF_INHER(float, m_fAlpha, Alpha);

	_SYNTHESIZE_INHER(cObjBase*, m_pFade, Fade);

	std::list<cObjBase*>	m_lstActionUI;
public:
	void	Init();

	void	Render();

	void	ChangeScene(int nID);

	void	OnClick(char* pKey);

	void	FadeOut();
	void	FadeIn();
public:
	cSceneMgr();
	~cSceneMgr();
};

