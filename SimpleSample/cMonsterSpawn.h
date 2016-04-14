#pragma once
#include "cNode.h"

class cMonster;
class cMonsterSpawn :
	public cNode
{
private:
	D3DXVECTOR3		m_vPlayerPosition;
	int				m_nCount;
	int				m_nMaxCount;
	bool			m_IsThrow;
	float			m_fPer;
public:
	float	GenerateMonster(const D3DXVECTOR3& vPlayerPosition);


public:
	void	SideMonsterSpawn( );
	void	FrontMonsterSpawn();
	void	FrontGapMonsterSpawn( int nMax, int nGap);

	cMonster*	CreateMonster(const D3DXVECTOR3& vPos, char* pKey);
	cMonster*	CreateThrowMonster(const D3DXVECTOR3& vPos, char* pKey);
public:
	cMonsterSpawn();
	~cMonsterSpawn();
};

