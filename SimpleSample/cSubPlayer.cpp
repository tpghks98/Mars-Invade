#include "DXUT.h"
#include "cSubPlayer.h"
#include "cResourceFile.h"
#include "cResourceMgr.h"
#include "cObjectMgr.h"
#include "cPlayer.h"
#include "cSystemMgr.h"
#include "cNormalGun.h"

cSubPlayer::cSubPlayer()
:m_pPlayer( NULL )
{
}


cSubPlayer::~cSubPlayer()
{
	SAFE_DELETE(m_pGun);
}

void	cSubPlayer::Initialize()
{
	m_pGun = new cNormalGun(this);
	m_vScale *= 1.8f;
	m_pResource = _GETSINGLE(cResourceMgr)->GetResource("player");

}
void	cSubPlayer::Update()
{
	this->Translation();
	if (_GETS(cSystemMgr)->IsEmptyCameraList())
	{
		m_pGun->Shot(D3DXVECTOR3(0, 0, 1), this);
	}
}
void	cSubPlayer::Render()
{
	this->DrawShadow();

	m_pResource->Render(this, _GETSINGLE(cResourceMgr)->GetShader(SI_CUSTOM), true);
}

void	cSubPlayer::Translation()
{
	D3DXMATRIXA16	matScale, matRot, matTrans, matRotZ;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	{
		D3DXQUATERNION	q;
		D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, m_vRotation.x, m_vRotation.z);
		D3DXMatrixRotationQuaternion(&matRot, &q);

	}
	if (_GETSINGLE(cObjectMgr)->GetPlayer() != NULL)
	{
		D3DXMatrixRotationZ(&matRotZ, _GETSINGLE(cObjectMgr)->GetPlayer()->GetSubRot());
	}
	else
	{
		D3DXMatrixIdentity(&matRotZ);
	}

	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = matScale	* matRot *	matTrans * matRotZ;
	if (_GETSINGLE(cObjectMgr)->GetPlayer() != NULL)
	{
		matTrans = _GETSINGLE(cObjectMgr)->GetPlayer()->GetWorldMatrix();
		D3DXVECTOR3	v = D3DXVECTOR3(matTrans._41, matTrans._42, matTrans._43);


		m_matWorld *= matTrans;
		m_matWorld._43 -= 1.0f;
	}
}
