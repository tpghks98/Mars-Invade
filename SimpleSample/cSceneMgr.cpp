#include "DXUT.h"
#include "cSceneMgr.h"
#include "cScene.h"
#include "cIntroScene.h"
#include "cHomeScene.h"
#include "cPlayScene.h"
#include "cSystemMgr.h"
#include "cObjectMgr.h"
#include "cTimer.h"
#include "cObjBase.h"
#include "cBackUI.h"
#include "cResourceMgr.h"
#include "cRankBar.h"
#include "shared.h"

cSceneMgr::cSceneMgr()
:m_nSceneID( SI_INTRO )
, m_pScene( NULL )
, m_IsChange( false) 
, m_fAlpha( 1.0f )
, m_pFade( NULL )
{
}


cSceneMgr::~cSceneMgr()
{
}


void	cSceneMgr::Init()
{
	m_pFade = new cBackUI();
	m_pFade->Initialize();
	m_pFade->SetResource(_GETS(cResourceMgr)->GetResource("ui_fade_background"));
	m_pFade->SetPosition(g_vMiddle);
}

void	cSceneMgr::ChangeScene(int nID)
{
	if (!m_IsChange)
	{
		m_IsChange = true;
		_GETS(cObjectMgr)->CreateTimer(this)->Setup(this, 0.0f, true, std::bind(&cSceneMgr::FadeOut, this));

		m_nSceneID = nID;
	}
}

void	cSceneMgr::OnClick(char* pKey)
{
	for each(auto it in m_lstActionUI)
	{
		it->SetDelete(true);
	}
	m_lstActionUI.clear();
	if (pKey == "ui_main_start_button")
	{
		ChangeScene(SI_PLAY);
	}
	else if (pKey == "ui_main_rank_button")
	{
		m_lstActionUI.push_back(  CreateObject<cRankBar>(g_vMiddle) );
	}
}

void	cSceneMgr::FadeOut()
{
	m_fAlpha += _GETS(cSystemMgr)->GetDeltaTime() * 3;
	if (m_fAlpha > 1.0f)
	{
		m_fAlpha = 1.0f;
		this->DeleteAllInherit();
		_GETS(cObjectMgr)->AllDeleteObject();

		_GETS(cObjectMgr)->CreateTimer(this)->Setup(this, 0.0f, true, std::bind(&cSceneMgr::FadeIn, this));
		
		SAFE_DELETE(m_pScene)
		switch (m_nSceneID)
		{
			case SI_INTRO:
				m_pScene = new cIntroScene();
				break;
			case SI_HOME:
				m_pScene = new cHomeScene();
				break;
			case SI_PLAY:
				m_pScene = new cPlayScene();
				break;
			default:
				break;
		}
		if (m_pScene != NULL)
		{
			m_pScene->Init();
		}
	}
}
void	cSceneMgr::FadeIn()
{
	m_fAlpha -= _GETS(cSystemMgr)->GetDeltaTime() * 3;
	if (m_fAlpha < 0.0f)
	{
		m_fAlpha = 0.0f;
		this->DeleteAllInherit();
		m_IsChange = false;
	}
}

void	cSceneMgr::Render()
{
	m_pFade->SetAlpha(m_fAlpha);
	m_pFade->Update();
	m_pFade->Render();
}
