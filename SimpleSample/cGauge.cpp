#include "DXUT.h"
#include "cGauge.h"
#include "cResourceFile.h"
#include "cResourceMgr.h"
#include "cSystemMgr.h"

cGauge::cGauge()
:m_pCurr( NULL )
, m_pMax( NULL )
, m_fPer( 0.0f )
{
}


cGauge::~cGauge()
{
}

void	cGauge::Initialize()
{
}
void	cGauge::Update()
{
	if (_GETS(cSystemMgr)->IsEmptyCameraList())
	{
		float fPer = (*m_pCurr) / (*m_pMax);
		m_fPer += (fPer - m_fPer) * _GETS(cSystemMgr)->GetDeltaTime() * 5;
	}
}
void	cGauge::Render()
{
	if (!m_IsDelete )
	{
		_GETS(cResourceMgr)->SetPerRect(m_fPer);
		m_pResource->Render(this, NULL, false);
	}
}