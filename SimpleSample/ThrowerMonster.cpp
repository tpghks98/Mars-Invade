#include "DXUT.h"
#include "ThrowerMonster.h"
#include "cResourceFile.h"
#include "cResourceMgr.h"

ThrowerMonster::ThrowerMonster()
{
}


ThrowerMonster::~ThrowerMonster()
{
}

void	ThrowerMonster::Initialize()
{
	m_pResource = _GETSINGLE(cResourceMgr)->GetResource("woman");


}
void	ThrowerMonster::Update()
{
	this->Translation();
}
void	ThrowerMonster::Render()
{
	m_pResource->Render(this, _GETSINGLE(cResourceMgr)->GetShader(SI_CUSTOM), true);
}