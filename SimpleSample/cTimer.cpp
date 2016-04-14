#include "DXUT.h"
#include "cTimer.h"
#include "cSystemMgr.h"
#include "cGun.h"
#include "cObjectMgr.h"

cTimer::cTimer()
: m_pTargetNode( NULL )
, m_fMaxTime( 0.0f )
, m_fCurrTime( 0.0f )
, m_IsRepeat( false )
{
}


cTimer::~cTimer()
{
}
void	cTimer::Setup(cNode* pTargetNode, float fMaxTime,
	bool IsRepeat, std::function<void()> f)
{
	m_fCurrTime = 0.0f;
	m_pFunc = f;
	m_pTargetNode = pTargetNode;
	m_IsRepeat = IsRepeat;
	m_fMaxTime = fMaxTime;
}

void	cTimer::Update()
{
	if (!m_IsDelete)
	{
		m_fCurrTime += _GETSINGLE(cSystemMgr)->GetDeltaTime();

		if (m_fCurrTime >= m_fMaxTime)
		{
			m_pFunc();
			if (m_IsRepeat)
			{
				m_fCurrTime = 0.0f;
			}
			else
			{
				m_IsDelete = true;
			}

		}
	}
}
