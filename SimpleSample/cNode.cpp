#include "DXUT.h"
#include "cNode.h"


cNode::cNode()
{
	m_IsDelete = false;
}


cNode::~cNode()
{
	DeleteAllInherit();
}

void cNode::DeleteAllInherit()
{
	for each(auto it in m_lstChild)
	{
		it->m_IsDelete = true;
		it->DeleteParent(this);
	}

	for each(auto it in m_lstParent)
	{
		it->DeleteChild(this);
	}

	m_lstChild.clear();
	m_lstParent.clear();
}