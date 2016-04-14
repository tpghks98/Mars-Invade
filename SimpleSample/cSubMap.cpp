#include "DXUT.h"
#include "cSubMap.h"
#include "cObjectMgr.h"
#include "cResourceMgr.h"
#include "cResourceFile.h"
#include "cSystemMgr.h"


cSubMap::cSubMap()
{
}


cSubMap::~cSubMap()
{
}


void	cSubMap::Initialize()
{
	m_vScale.x *= 0.7f;
	m_pResource = _GETSINGLE(cResourceMgr)->GetResource("map_top");
}
void	cSubMap::Update()
{
	this->Translation();
}
void	cSubMap::Render()
{
	_GETS(cObjectMgr)->SetIsMap(true);
	m_pResource->Render(this, _GETSINGLE(cResourceMgr)->GetShader(SI_CUSTOM), false);
	_GETS(cObjectMgr)->SetIsMap(false);

}