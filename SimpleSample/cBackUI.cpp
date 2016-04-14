#include "DXUT.h"
#include "cBackUI.h"
#include "cResourceFile.h"
#include "cSystemMgr.h"


cBackUI::cBackUI()
{
}


cBackUI::~cBackUI()
{
}

void	cBackUI::Initialize()
{
	m_vScale = D3DXVECTOR3(2, 2, 2);
}
void	cBackUI::Update()
{
	m_vScale += (g_vOne - m_vScale) * _GETS(cSystemMgr)->GetDeltaTime() * 3.0f;

	this->Translation();
}
void	cBackUI::Render()
{
	m_pResource->Render(this, NULL, false);
}