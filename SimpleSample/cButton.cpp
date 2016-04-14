#include "DXUT.h"
#include "cButton.h"
#include "cResourceFile.h"
#include "cSpriteFile.h"
#include "cSystemMgr.h"
#include "cSceneMgr.h"

cButton::cButton()
:m_fScaleTime( 0.0f )
{
}


cButton::~cButton()
{
}


void	cButton::Initialize()
{
	m_vScale = g_vZero;
}
void	cButton::Update()
{
	if (m_pResource != NULL)
	{
		if (typeid(*m_pResource) == typeid(cSpriteFile))
		{
			int nWidth = ((cSpriteFile*)m_pResource)->GetTexture(this)->dwWidth;
			int nHeight = ((cSpriteFile*)m_pResource)->GetTexture(this)->dwHeight;

			RECT	rt;
			SetRect(&rt,
				m_vPosition.x - nWidth * 0.5f,
				m_vPosition.y - nHeight * 0.5f,
				m_vPosition.x + nWidth * 0.5f,
				m_vPosition.y + nHeight * 0.5f);

			auto pt = _GETS(cSystemMgr)->GetMousePosition();
			m_fAniKey = 0.0f;
			if (PtInRect(&rt, pt))
			{
				if (m_fScaleTime == 0.0f)
				{
					m_fScaleTime = 1.0f;
				}
				m_fScaleTime -= _GETS(cSystemMgr)->GetDeltaTime();
				if (m_fScaleTime < 0.5f)
				{
					m_fScaleTime = 0.5f;
				}
				m_fAniKey = 0.1f;
				if (_GETS(cSystemMgr)->GetKeyState(VK_LBUTTON) == KS_DOWN) 
				{
					m_fAniKey = 0.2f;
				}
				if (_GETS(cSystemMgr)->GetKeyState(VK_LBUTTON) == KS_UP)
				{
					m_fAniKey = 0.2f;
					_GETS(cSceneMgr)->OnClick(m_pResource->GetKey());

				}

			}
			else
			{
				m_fScaleTime -= _GETS(cSystemMgr)->GetDeltaTime() * 3;
				if (m_fScaleTime < 0.0f)
				{
					m_fScaleTime = 0.0f;
				}
			}

			m_vScale +=  ( ( g_vOne *  _GETS(cSystemMgr)->GetLerp(0.95f, 1.15f, m_fScaleTime) ) - m_vScale ) * _GETS( cSystemMgr )->GetDeltaTime() * 4;
		
		}
	}
	this->Translation();
}
void	cButton::Render()
{
	m_pResource->Render(this, NULL, false);
}