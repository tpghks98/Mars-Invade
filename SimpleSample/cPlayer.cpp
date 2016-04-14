#include "DXUT.h"
#include "cPlayer.h"
#include "cResourceFile.h"
#include "cResourceMgr.h"
#include "cSystemMgr.h"
#include "cCamera.h"
#include "cObjectMgr.h"
#include "shared.h"
#include "cNormalGun.h"
#include "cThreeWayGun.h"
#include "cFastGun.h"
#include "cMonster.h"
#include "cEffect.h"
#include "cSubPlayer.h"
#include "cGuidedGun.h"
#include "cLaserGun.h"
#include "cStructure.h"
#include "cLookCamera.h"
#include "cGauge.h"
#include "cSpreadUI.h"
#include "cBackEff.h"

cPlayer::cPlayer()
:m_vCameraDir( 0, 0 ,0 )
, m_fPlusHeight( 0.0f )
, m_vLocalRot( 0, 0, 0)
, m_vLocalPos( 0, 0, 0)
, m_fSubRot( 0.0f )
, m_nSubNum( 0)
, m_fBoostTime( 0.0f )
, m_nGunLevel( 1 )
, m_pShield( NULL )
, m_nNumGuid( 0 )
, m_fGuidTime( 0.0f )
{
}


cPlayer::~cPlayer()
{
	this->GunReset();
	_GETS(cObjectMgr)->SetPlayer(NULL);
	if (_GETS(cSystemMgr)->GetMainCamera() != NULL)
	{
		_GETS(cSystemMgr)->GetMainCamera()->SetTarget(NULL);

	}
	if (_GETSINGLE(cObjectMgr)->GetShadowCamera() != NULL)
	{
		_GETSINGLE(cObjectMgr)->GetShadowCamera()->SetTarget(NULL);

	}
}


void	cPlayer::Initialize()
{
	m_pBackEff = CreateObject<cBackEff>(g_vMiddle, "ui_back_normal");
	this->AddInherit(this, m_pBackEff);

	auto	pCamera	=		_GETSINGLE(cSystemMgr)->AddCamera();
	pCamera->SetTarget(this);
	pCamera->SetEye(D3DXVECTOR3(-10, 40, -50));
	((cLookCamera*)pCamera)->SetTime(0.5f);

	m_fHp = 10.0f;
	m_fMaxHp = m_fHp;

	_GETSINGLE(cSystemMgr)->GetMainCamera()->SetTarget(this);
	_GETSINGLE(cSystemMgr)->GetMainCamera()->SetDist(2.5f);
	m_pResource = _GETSINGLE(cResourceMgr)->GetResource("player");
	m_IsSmoothRot = true;

	m_vScale *= 0.04f;
	m_fMoveSpeed = 110.0f;

	m_vDirection = D3DXVECTOR3(0, 0, 1);

	_GETSINGLE(cObjectMgr)->GetShadowCamera()->SetTarget(this);
	_GETSINGLE(cObjectMgr)->GetShadowCamera()->SetDist(500);
	_GETSINGLE(cObjectMgr)->GetShadowCamera()->SetDir(
		D3DXVECTOR3(-0.00001f, -1, -0.0f));
	m_vCameraDir = D3DXVECTOR3(0, -1.5f, 2.5f);
	m_vPosition.z = -200.0f;

	m_IsCollisionCheck = true;
	m_fRecoveryTime = 1.0f;
//	m_vRotation.x = D3DX_PI * 0.15f;

	m_vPosition.y = 10.0f;
//	m_vPosition.z = -250.0f;
	/*
	CreateObject<cSubPlayer>(D3DXVECTOR3(80, 0, -25));
	CreateObject<cSubPlayer>(D3DXVECTOR3(-80, 0, -25));
	CreateObject<cSubPlayer>(D3DXVECTOR3(0, 80, -25));
	CreateObject<cSubPlayer>(D3DXVECTOR3(0, -80, -25));
	*/
	m_pEffectKey = "effect_normal_boost";

	m_pFireEffect = (cEffect*)CreateObject<cEffect>(m_vPosition, m_pEffectKey);
	m_pFireEffect->SetIsRepeat(true);
	m_pFireEffect->SetIsBillBoard(false);
	m_pFireEffect->SetScale(g_vOne * 0.05f);
	m_pFireEffect->SetAlphaChange(true);


	auto pGauge = (cGauge*)
		CreateObject<cGauge>(D3DXVECTOR3(g_nWinSizeX * 0.5f, 1043, 0), "ui_bar_player");

	pGauge->SetMax(&m_fMaxHp);
	pGauge->SetCurr(&m_fHp);

	this->AddInherit(this, pGauge );

	ResetGun();
}
void	cPlayer::Update()
{
	if (m_fHp > m_fMaxHp)
	{
		m_fHp = m_fMaxHp;
	}
	if (_GETSINGLE(cSystemMgr)->GetKeyState('1') == KS_ONCE)
	{
		UpgradeGun();
	}

	if (_GETSINGLE(cSystemMgr)->GetKeyState('2') == KS_ONCE)
	{
		OnShield();
	}
	if (_GETSINGLE(cSystemMgr)->GetKeyState('0') == KS_ONCE)
	{
		PlusAddMob();
	}
	if (_GETSINGLE(cSystemMgr)->GetKeyState(VK_SPACE) == KS_ONCE)
	{
		m_fBoostTime = 2.0f;
	}

	m_fSubRot += D3DX_PI * 2 * _GETS(cSystemMgr)->GetDeltaTime();

	if (_GETSINGLE(cSystemMgr)->GetKeyState('Z') == KS_ONCE)
	{
		if (m_nNumGuid > 0)
		{
			--m_nNumGuid;
			m_fGuidTime += 1.0f;
		}
	}

	if (m_fGuidTime > 0.0f)
	{
		m_fGuidTime -= _GETS(cSystemMgr)->GetDeltaTime();
		for (int n = 0; n < 2; ++n)
		{
			cGuidedGun pGun(this);
			pGun.SetBulletDamage(5.0f);
			pGun.Shot(m_vDirection, this);
		}
	}

	if (m_vPosition.z < 0.0f)
	{
		m_vPosition.z += _GETSINGLE(cSystemMgr)->GetDeltaTime() * 110.0f;
	}

	m_vPosition.y = 30.0f;
	this->BoostUpdate();

	this->BaseUpdate();
	this->KeyInput();
	this->Translation();

	m_pFireEffect->SetRotation(  m_vLocalRot);
	m_pFireEffect->SetPosition(m_vPosition + m_vLocalPos + D3DXVECTOR3( 0, 0.4f, 0 ) );

	if (m_pShield != NULL)
	{
		D3DXVECTOR3	v(m_matWorld._41, m_matWorld._42, m_matWorld._43 );
		m_pShield->SetPosition(v);
	}
}
void	cPlayer::Render()
{
	this->DrawShadow();

	m_pResource->Render(this, _GETSINGLE(cResourceMgr)->GetShader(SI_CUSTOM));
	DXUTGetD3D9Device()->SetRenderState(D3DRS_ZWRITEENABLE, false);

//	_GETSINGLE(cResourceMgr)->GetResource( m_pEffectKey)->Render(this, false);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_ZWRITEENABLE, true);

	_GETS(cObjectMgr)->DrawNumber(D3DXVECTOR3(1545, 128, 0), m_nGunLevel, 0.5f);
	_GETS(cObjectMgr)->DrawNumber(D3DXVECTOR3(1680, 128, 0), m_nNumGuid, 0.5f);
	_GETS(cObjectMgr)->DrawNumber(D3DXVECTOR3(1814, 128, 0), m_nSubNum, 0.5f);
}

void	cPlayer::KeyInput()
{
	D3DXVECTOR3	vPlusRot = g_vZero;
	D3DXVECTOR3	vPlusPos = g_vZero;
	if (_GETS(cSystemMgr)->GetKeyState(VK_UP) == KS_DOWN)
	{

		vPlusRot.x -= D3DX_PI * DXUTGetElapsedTime();
		vPlusPos.y += _GETSINGLE(cSystemMgr)->GetDeltaTime() * 110.0f;
	//	m_vRotation.x -= D3DX_PI * _GETSINGLE(cSystemMgr)->GetDeltaTime() * 0.5f;

	}
	else if (_GETS(cSystemMgr)->GetKeyState(VK_DOWN) == KS_DOWN)
	{

		vPlusRot.x += D3DX_PI * DXUTGetElapsedTime();
		vPlusPos.y -= _GETSINGLE(cSystemMgr)->GetDeltaTime() * 110.0f;
//		m_vRotation.x += D3DX_PI * _GETSINGLE(cSystemMgr)->GetDeltaTime() * 0.5f;

	}
	else
	{
		m_vLocalRot.x -= m_vLocalRot.x * _GETS(cSystemMgr)->GetDeltaTime() * 6.0f;
	}

	if (_GETS(cSystemMgr)->GetKeyState(VK_LEFT) == KS_DOWN)
	{
		if (m_vPosition.x > -30.0f)
		{
			vPlusPos.x -= _GETSINGLE(cSystemMgr)->GetDeltaTime() * 110.0f;
		}
		vPlusRot.z += D3DX_PI * DXUTGetElapsedTime();
	//	m_vRotation.y -= D3DX_PI * _GETSINGLE(cSystemMgr)->GetDeltaTime() * 0.5f;


	}
	else if (_GETS(cSystemMgr)->GetKeyState(VK_RIGHT) == KS_DOWN)
	{
		if (m_vPosition.x < 30.0f)
		{
			vPlusPos.x += _GETSINGLE(cSystemMgr)->GetDeltaTime() * 110.0f;
		}
		vPlusRot.z -= D3DX_PI * DXUTGetElapsedTime();
//		m_vRotation.y += D3DX_PI * _GETSINGLE(cSystemMgr)->GetDeltaTime() * 0.5f;

	}
	else
	{
		m_vLocalRot.z -= m_vLocalRot.z * _GETS(cSystemMgr)->GetDeltaTime() * 6.0f ;
	}
	

	if (_GETS(cSystemMgr)->IsEmptyCameraList())
	{
		for each(auto it in m_lstGun)
		{
			it->Shot(m_vDirection, this);

		}
	}
	
	if (_GETS(cSystemMgr)->GetKeyState('A') == KS_DOWN)
	{
		m_vRotation.y += -D3DXToRadian(90) * _GETS( cSystemMgr )->GetDeltaTime() ;
		vPlusRot.z += D3DX_PI * DXUTGetElapsedTime();

	}
	if (_GETS(cSystemMgr)->GetKeyState('B') == KS_DOWN)
	{
		m_vRotation.y += D3DXToRadian(90) * _GETS(cSystemMgr)->GetDeltaTime() ;
		vPlusRot.z -= D3DX_PI * DXUTGetElapsedTime();
	}

	D3DXMATRIXA16	matRot;
	
	D3DXQUATERNION	q;
		
	D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, 0.0f, 0.0f);
	D3DXMatrixRotationQuaternion(&matRot, &q);
		
	m_vLocalRot += vPlusRot;
	m_vPosition += vPlusPos * 0.25f;

	if (m_vLocalRot.x > D3DX_PI * 0.1f)
	{
		m_vLocalRot.x = D3DX_PI * 0.1f;
	}
	else if (m_vLocalRot.x < -D3DX_PI * 0.1f)
	{
		m_vLocalRot.x = -D3DX_PI * 0.1f;
	}

	if (m_vLocalRot.z > D3DX_PI * 0.25f)
	{
		m_vLocalRot.z = D3DX_PI * 0.25f;
	}
	else if (m_vLocalRot.z < -D3DX_PI * 0.25f)
	{
		m_vLocalRot.z = -D3DX_PI * 0.25f;
	}

	auto pMainCamera = _GETSINGLE(cSystemMgr)->GetMainCamera();

	m_vPosition.y += m_fPlusHeight;

	D3DXVECTOR3 vD;
	D3DXVec3TransformCoord(&vD, &D3DXVECTOR3(0, 0.0f, 1.2f), &matRot);
//	vD = D3DXVECTOR3(0, -1.5f, 1);


	static float fPer = 1.35f;
	if (_GETS(cSystemMgr)->GetKeyState('U') == KS_DOWN)
	{
		fPer -= DXUTGetElapsedTime();
	}
	else if (_GETS(cSystemMgr)->GetKeyState('I') == KS_DOWN)
	{
		fPer += DXUTGetElapsedTime();
	}
	pMainCamera->SetDir(vD * fPer);
	pMainCamera->SetPlusDir( D3DXVECTOR3( 0, -2.3f, -0.25f ) *( 1.0f / fPer));
	pMainCamera->SetUp(D3DXVECTOR3( 0,1 , 0));
	// -1.7f , -0.2f
	// 0 , 0, 1.6f
}

void	cPlayer:: KeyRotation(float fAngle)
{
	D3DXMATRIXA16	matRot;
	D3DXMatrixRotationY(&matRot, fAngle);

	D3DXVec3TransformCoord(&m_vDirection, &m_vDirection, &matRot);
}
void	cPlayer::OnCollision(cObjBase* pObj)
{

	if (typeid(*pObj) == typeid(cMonster))
	{
		this->OnHit(1.0f);
	}


}

void	cPlayer::Translation()
{
	
	D3DXMATRIXA16	matScale, matRot, matTrans, matDir;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);

	{
		D3DXQUATERNION	q;
		D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, m_vRotation.x, m_vRotation.z);
		D3DXMatrixRotationQuaternion(&matRot, &q);

		D3DXVec3TransformCoord(&m_vDirection, &D3DXVECTOR3(0, 0, 1), &matRot);
		m_vDirection = D3DXVECTOR3(0, 0, 1);

		D3DXQuaternionRotationYawPitchRoll(&q, m_vLocalRot.y + m_vRotation.y, m_vLocalRot.x + m_vRotation.x, m_vLocalRot.z + m_vRotation.z);
		D3DXMatrixRotationQuaternion(&matRot, &q);

	}
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z + m_vLocalPos.z);
	D3DXMatrixIdentity(&matDir);

	m_matWorld = matScale	*	matRot	* matDir *	matTrans;
}

void	cPlayer::GunReset()
{
	auto it = m_lstGun.begin();
	auto en_it = m_lstGun.end();


	while (it != en_it)
	{
		SAFE_DELETE(*it);
		it = m_lstGun.erase(it);
	}
}

void	cPlayer::OnHit(float fDamage)
{
	if (m_fBoostTime <= 0.0f)
	{
		if (m_pShield != NULL)
		{
			m_pShield->SetIsAlphaDestroy(true);
			m_pShield = NULL;
		}
		else
		{
			cMoveableObj::OnHit(fDamage);
			_GETS(cSystemMgr)->Shaking(1.0f, 0.5f, false);
			m_pBackEff->SetResource(_GETS(cResourceMgr)->GetResource("ui_back_hit"));
			m_pBackEff->SetAlpha(1.5f);
			this->ResetGun();
		}
	}
}

void	cPlayer::BoostUpdate()
{
	if (m_fBoostTime > 0.0f)
	{
		m_fBoostTime -= _GETS(cSystemMgr)->GetDeltaTime();
		m_pEffectKey = "effect_boost";

		_GETS(cSystemMgr)->Shaking(0.5f, 0.001f, false);
		_GETSINGLE(cObjectMgr)->SetScrollSpeed(450.0f);
		m_vLocalPos.z += _GETSINGLE(cSystemMgr)->GetDeltaTime() * 20.0f;
		if (m_vLocalPos.z > 12.0f)
		{
			m_vLocalPos.z = 12.0f;
		}
		m_vRotation.x -= D3DX_PI * _GETS(cSystemMgr)->GetDeltaTime();
		if (m_vRotation.x < D3DX_PI * -0.05f)
		{
			m_vRotation.x = D3DX_PI * -0.05f;
		}
	}
	else
	{
		m_fBoostTime = 0.0f;
		m_pEffectKey = "effect_normal_boost";

		_GETSINGLE(cObjectMgr)->SetScrollSpeed(200.0f);
		m_vLocalPos.z -= _GETSINGLE(cSystemMgr)->GetDeltaTime() * 20.0f;
		if (m_vLocalPos.z < -4.0f)
		{
			m_vLocalPos.z = -4.0f;
		}
		m_vRotation.x += D3DX_PI * _GETS(cSystemMgr)->GetDeltaTime();
		if (m_vRotation.x > D3DX_PI * 0.15f)
		{
			m_vRotation.x = D3DX_PI * 0.15f;
		}
	}
	m_pFireEffect->SetResource(_GETSINGLE(cResourceMgr)->GetResource(m_pEffectKey));
}

void	cPlayer::PlusAddMob()
{
	if (m_nSubNum < 4)
	{
		D3DXVECTOR3	vPosition = D3DXVECTOR3(80, 0, 0);
		D3DXMATRIXA16	matRot;
		D3DXMatrixRotationZ(&matRot, m_nSubNum * D3DX_PI * 0.5f);
		D3DXVec3TransformCoord(&vPosition, &vPosition, &matRot);

		char* pKey = "player_add2";


		CreateObject<cSubPlayer>(vPosition, pKey)->SetRotation(D3DXVECTOR3(0, 0, m_nSubNum * D3DX_PI * 0.5f));

		++m_nSubNum;
		CreateObject<cSpreadUI>(D3DXVECTOR3(g_nWinSizeX * 0.5f, 150.0f, 0), "ui_notice_add");

	}
}

void	cPlayer::UpgradeGun()
{
	GunReset();

	++m_nGunLevel;
	if (m_nGunLevel > 5)
	{
		m_nGunLevel = 5;
	}
	switch (m_nGunLevel)
	{
	case 1:
		m_lstGun.push_back(new cNormalGun(this));
		break;
	case 2:
		m_lstGun.push_back(new cFastGun(this));
		break;
	case 3:
		m_lstGun.push_back(new cThreeWayGun(this));
		break;
	case 4:
		m_lstGun.push_back(new cLaserGun());
		break;
	case 5:
		m_lstGun.push_back(new cLaserGun());
		m_lstGun.push_back(new cThreeWayGun(this));
		break;
	}
	if (m_nGunLevel >= 2)
	{
		CreateObject<cSpreadUI>(D3DXVECTOR3(g_nWinSizeX * 0.5f, 150.0f, 0), "ui_notice_power");
	}
}

void	cPlayer::ResetGun()
{
	m_nGunLevel = 0;
	UpgradeGun();
}

void	cPlayer::OnShield()
{
	
	if (m_pShield == NULL)
	{
		m_pShield = (cEffect*)CreateObject<cEffect>(m_vPosition, "effect_shield");
		m_pShield->SetIsRepeat(true);
		m_pShield->SetRotation(D3DXVECTOR3(0, 0, D3DX_PI * -0.5f));

		AddInherit(this, m_pShield);
	}
	
}
