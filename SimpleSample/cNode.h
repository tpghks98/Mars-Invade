#pragma once

#include "value.h"
class cNode
{
protected:
	std::list<cNode*>	m_lstParent;
	std::list<cNode*>	m_lstChild;

	_SYNTHESIZE_INHER(bool, m_IsDelete, Delete);
public:
	void	AddParent(cNode* pNode) { m_lstParent.push_back(pNode); }
	void	AddChild(cNode* pNode) { m_lstChild.push_back(pNode); }


	void	DeleteParent(cNode* pNode) {
		auto it = m_lstParent.begin();
		auto en_it = m_lstParent.end();
		while (it != en_it)
		{
			if ((*it) == pNode)
			{
				it = m_lstParent.erase(it);
				continue;
			}
			++it;
		}
	}
	void	DeleteChild(cNode* pNode) { m_lstChild.remove(pNode); }

	static void AddInherit(cNode* pParent, cNode* pChild)
	{
		pParent->AddChild(pChild);
		pChild->AddParent(pParent);
	}
	void DeleteAllInherit();
public:
	cNode();
	virtual ~cNode();
};

