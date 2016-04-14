#include "DXUT.h"
#include "cBackEff.h"
#include "cResourceFile.h"
#include "cSystemMgr.h"
#include "cResourceMgr.h"

cBackEff::cBackEff()
:m_fAlphaPlus( 1.0f )
{
}


cBackEff::~cBackEff()
{
}


void	cBackEff::Initialize()
{
	m_vPosition.z = 0.01f;
}
void	cBackEff::Update()
{
	this->Translation();
	
	m_fAlpha += m_fAlphaPlus * _GETS(cSystemMgr)->GetDeltaTime();

	if (m_fAlpha < 0.3f)
	{
		m_pResource = (_GETS(cResourceMgr)->GetResource("ui_back_normal"));
		m_fAlphaPlus = 1;
	}
	else if (m_fAlpha > 0.7f)
	{
		m_fAlphaPlus =  -1;
	}
}
void	cBackEff::Render()
{
	m_pResource->Render(this, NULL, false);
}