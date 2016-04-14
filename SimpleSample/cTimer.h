#pragma once
#include "cNode.h"

class cTimer :
	public cNode
{
private:
	std::function<void()> m_pFunc;
	cNode*	m_pTargetNode;

	_SYNTHESIZE_REF_INHER(float, m_fCurrTime, CurrTime);
	_SYNTHESIZE_REF_INHER(float, m_fMaxTime,	MaxTime);

	_SYNTHESIZE_INHER(bool, m_IsRepeat, IsRepeat);
	
public:
	void	Setup(cNode* pTargetNode, float fMaxTime, bool IsRepeat, std::function<void()> f);
	void	Update();


public:
	cTimer(void);
	~cTimer(void);
};

