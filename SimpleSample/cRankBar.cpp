#include "DXUT.h"
#include "cRankBar.h"
#include "cResourceFile.h"
#include "cResourceMgr.h"
#include "cObjectMgr.h"
#include "cSystemMgr.h"
cRankBar::cRankBar()
{
}


cRankBar::~cRankBar()
{
}


void	cRankBar::Initialize()
{
	m_pResource = _GETS(cResourceMgr)->GetResource("ui_main_rank_bar");
	m_vScale = D3DXVECTOR3(0, 0, 0);
}
void	cRankBar::Render()
{
	m_pResource->Render(this, NULL, false);

	int nCount = 0;

	if (m_vScale.x >= 0.7f)
	{
		for each(auto it in
			_GETS(cObjectMgr)->m_lstScore)
		{
			RECT	rt;
			D3DXVECTOR3	v = m_vPosition;
			v.x -= 330 * m_vScale.x;
			v.y -= 260.0f * m_vScale.y;
			v.y += nCount * 135.0f * m_vScale.x;
			SetRect(&rt,
				v.x - 50,
				v.y,
				0, 0);

			_GETS(cSystemMgr)->m_pFont->DrawTextA(NULL,
				it.strName.c_str(), -1, &rt, DT_LEFT | DT_NOCLIP,
				D3DXCOLOR(1, 1, 1, 1));
			++nCount;

			v.x += 650.0f;

			SetRect(&rt,
				v.x - 50,
				v.y,
				0, 0);
			char szBuf[128];
			itoa(it.nScore, szBuf, 10);

			_GETS(cSystemMgr)->m_pFont->DrawTextA(NULL,
				szBuf, -1, &rt, DT_LEFT | DT_NOCLIP,
				D3DXCOLOR(1, 1, 1, 1));
		}
	}
}
void	cRankBar::Update()
{
	m_vScale += (g_vOne - m_vScale) * _GETS(cSystemMgr)->GetDeltaTime() * 3.0f;

	this->Translation();
}