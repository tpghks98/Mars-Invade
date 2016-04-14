#include "DXUT.h"
#include "cStone.h"
#include "cResourceFile.h"
#include "cResourceMgr.h"

cStone::cStone()
{
}


cStone::~cStone()
{
}


void	cStone::Initialize()
{

}
void	cStone::Update()
{
	this->Translation();
}
void	cStone::Render()
{
	m_pResource->Render(this, _GETSINGLE(cResourceMgr)->GetShader(SI_CUSTOM), true);
}
void	cStone::OnCollision(cObjBase* pObj)
{

}