#include "DXUT.h"
#include "cSpreadUI.h"
#include "cResourceFile.h"
#include "cSystemMgr.h"
#include "cObjectMgr.h"

cSpreadUI::cSpreadUI()
:m_fTime( 0.0f )
, m_fPlusTime( 1.0f )
{
}


cSpreadUI::~cSpreadUI()
{
}

void	cSpreadUI::Initialize()
{
	m_vScale = D3DXVECTOR3(0, 1, 1);

	auto pList = _GETS(cObjectMgr)->GetObjectList();

	for each(auto it in (*pList))
	{
		if (typeid(*it) == typeid(cSpreadUI))
		{
			if (it->GetResource() != NULL)
			{
				if (it->GetResource()->GetKey() != "ui_notice_warning")
				{
					it->SetDelete(true);
				}
			}
		}
	}
}
void	cSpreadUI::Update()
{
	m_fTime += _GETS(cSystemMgr)->GetDeltaTime() * m_fPlusTime * 2;

	if (m_fTime > 2.0f)
	{
		m_fTime = 2.0f;
		m_fPlusTime = -1.0f;
	}


	float end = 1.0f;
	float start = 0.0f;
	float value = m_fTime;

	m_vScale.x = _GETS(cSystemMgr)->GetLerp(start, end, value);
//	m_vScale.y = _GETS(cSystemMgr)->GetLerp(start, end, value);
	if (m_fPlusTime <= -1.0f)
	{
		m_fAlpha += m_fPlusTime * _GETS(cSystemMgr)->GetDeltaTime();
		m_fTime += _GETS(cSystemMgr)->GetDeltaTime() * m_fPlusTime;
	}
	this->Translation();
}
void	cSpreadUI::Render()
{
	m_pResource->Render(this, NULL, false);
}