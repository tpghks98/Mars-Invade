#include "DXUT.h"
#include "cNumber.h"
#include "cResourceFile.h"
#include "cResourceMgr.h"
#include "cSystemMgr.h"

cNumber::cNumber()
:m_nNumber( 0 )
{
}


cNumber::~cNumber()
{
}

void	cNumber::Initialize()
{
	m_pResource = _GETSINGLE(cResourceMgr)->GetResource("ui_number");
	m_fAlpha = 0.0f;
}
void	cNumber::Update()
{
	if (m_fAlpha < 1.0f)
	{
		m_fAlpha += _GETS(cSystemMgr)->GetDeltaTime() * 0.25f;
	}
	this->Translation();
}
void	cNumber::Render()
{
	int nSize = 40;
	char szToken[99];
	itoa(m_nNumber, szToken, 10);

	std::string str = szToken;

	D3DXVECTOR3	vOri = m_vPosition;
	m_vPosition.x -= nSize * str.size() * m_vScale.x;

	for each(auto it in str)
	{
		m_vPosition.x += nSize * m_vScale.x;
		this->Translation();
		char c[2];
		c[0] = it;
		c[1] = NULL;
		int a =
			atoi(c);

		m_fAniKey = (a)* 0.1f;
		m_pResource->Render(this, NULL, false);
	}
	m_vPosition = vOri;
}