#include "DXUT.h"
#include "cSystemMgr.h"
#include "cCamera.h"
#include "cObjectMgr.h"
#include "cTimer.h"
#include "cLookCamera.h"


#include <dsound.h>
#include "DXUT\Optional\SDKsound.h"
#include "DXUT\Optional\SDKsound.cpp"
#include "DXUT\Optional\SDKwavefile.h"
#include "DXUT\Optional\SDKwavefile.cpp"


cSystemMgr::cSystemMgr()
:m_fTimeScale( 1.0f )
, m_fShakePower( 0.0f )
, m_vShakePlus( 1, 1 )
, m_fDecreasePower( 0.3f )
, m_pCamera( NULL )
{
}


cSystemMgr::~cSystemMgr()
{
	SAFE_RELEASE(m_pFont);
	SAFE_DELETE(m_pCamera);

	for each(auto it in m_lstCamera)
	{
		SAFE_DELETE(it);
	}

	m_lstCamera.clear();
	SAFE_DELETE(m_pSoundManager);

	for each(auto pSound in m_lstSound)
	{
		SAFE_DELETE(pSound);
	}

	m_lstSound.clear();
}


void	cSystemMgr::Initialize()
{
	
	m_pSoundManager = new CSoundManager();
	m_pSoundManager->Initialize(DXUTGetHWND(), DSSCL_NORMAL);
	m_pSoundManager->SetPrimaryBufferFormat(2, 22050, 16);
	
	D3DXCreateFontA(DXUTGetD3D9Device(), 100, 0, FW_BOLD, 1, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, "ÈÞ¸ÕÆíÁöÃ¼", &m_pFont);


	memset(m_IsKeyState, false, sizeof(m_IsKeyState));
	memset(m_IsPrevKeyState, false, sizeof(m_IsPrevKeyState));

	m_ptMouse = ST_POINT(g_nWinSizeX * 0.5f, g_nWinSizeY * 0.5f);
	m_ptGap = ST_POINT(0, 0);

	m_pCamera = new cCamera();
	m_pCamera->Setup(D3DXVECTOR3(0, 60, -100), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0));
	m_pCamera->SetIsSmooth(true);
}

void	cSystemMgr::Update()
{

	m_pCamera->Update();
	_WHILEIT(m_lstCamera)
	_ENDIT(m_lstCamera)

	this->InputUpdate();
	this->ShakeUpdate();

	
	auto iter = m_lstSound.begin();
	auto end_iter = m_lstSound.end();

	while (iter != end_iter)
	{
		if (!(*iter)->IsSoundPlaying())
		{
			SAFE_DELETE(*iter);
			iter = m_lstSound.erase(iter);
			continue;
		}
		++iter;
	}
	
	if (this->GetKeyState('O') == KS_ONCE)
	{
		m_fTimeScale = !m_fTimeScale;
	}
}

void		cSystemMgr::SetMousePosition(const ST_POINT&  pt)
{
	m_ptMouse = pt;

	ST_POINT ptMouse(pt);
	ClientToScreen(DXUTGetHWND(), &ptMouse);

	SetCursorPos(ptMouse.x, ptMouse.y);
}


void	cSystemMgr::InputUpdate()
{
	memcpy(m_IsPrevKeyState, m_IsKeyState, sizeof(m_IsPrevKeyState));

	for (int n = 0; n < 256; ++n)
	{
		m_IsKeyState[n] = GetAsyncKeyState(n) & 0x8000;
	}
	
	ST_POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(DXUTGetHWND(), &pt);


	m_ptGap.x = pt.x - m_ptMouse.x;
	m_ptGap.y = pt.y - m_ptMouse.y;

	m_ptMouse = pt;
}


int		cSystemMgr::GetKeyState(int nKey)
{
	if (!m_IsKeyState[nKey] && !m_IsPrevKeyState[nKey])
	{
		return KS_NONE;
	}
	else if (m_IsKeyState[nKey] && m_IsPrevKeyState[nKey])
	{
		return KS_DOWN;
	}
	else if (m_IsKeyState[nKey] && !m_IsPrevKeyState[nKey])
	{
		return KS_ONCE;
	}

	return KS_UP;
}

void	cSystemMgr::CreateSound( LPWSTR pFileName, bool IsRepeat)
{
	
	CSound*	pSound;
	m_pSoundManager->Create(&pSound, pFileName);


//	pSound->Play();
	
	m_lstSound.emplace_back(pSound);
	
}


void	cSystemMgr::Shaking(float fPower, float fDecrease, bool IsShakeEye)
{
	if (m_fShakePower == 0.0f)
	{
		m_fDecreasePower = fDecrease;
		this->DeleteAllInherit();
		_GETS(cObjectMgr)->CreateTimer(this)->Setup(this, 0.05f, false, std::bind(&cSystemMgr::ReverseShake, this));
		_GETS(cObjectMgr)->CreateTimer(this)->Setup(this, 0.15f, false, std::bind(&cSystemMgr::RandomShakeValue, this));
		m_pCamera->SetShakeEye(IsShakeEye);
	}
	m_fShakePower += fPower * 0.3f;


}
void	cSystemMgr::ShakeUpdate()
{
	if (m_fShakePower > 0.0f)
	{
		D3DXVECTOR3 vShake = g_vZero;
		vShake.x += m_fShakePower * m_vShakePlus.x * 
			this->GetDeltaTime() * 10.0f;
		vShake.y += m_fShakePower * m_vShakePlus.y * 
			this->GetDeltaTime() * 10.0f;


		m_pCamera->SetShake(vShake);
	}
}

void	cSystemMgr::ReverseShake()
{
	if (m_fShakePower > 0.01f)
	{
		m_vShakePlus *= -1;
		_GETS(cObjectMgr)->CreateTimer(this)->Setup(this, 0.1f, false, std::bind(&cSystemMgr::ReverseShake, this));
	}
	else
	{
		m_fShakePower = 0.0f;
	}
}

void		cSystemMgr::RandomShakeValue()
{
	m_vShakePlus.x = (rand() % 2 * 2) - 1;
	m_vShakePlus.y = (rand() % 2 * 2) - 1;
	_GETS(cObjectMgr)->CreateTimer(this)->Setup(this, 0.2f, false, std::bind(&cSystemMgr::RandomShakeValue, this));
	m_fShakePower *= m_fDecreasePower;
}

cCamera*	cSystemMgr::AddCamera()
{
	cCamera*	pCamera = new	cLookCamera();

	m_lstCamera.push_back(pCamera);

	return pCamera;
}


float		cSystemMgr::GetLerp(float fStart, float fEnd, float fValue)
{
	float s = 1.70158f;
	if (fValue > 1.0f)
	{
		fValue = 1.0f;
	}
	else if (fValue < 0.0f)
	{
		fValue = 0.0f;
	}

//	return fStart + (fEnd - fStart) * fValue;

	fValue /= 0.5f;
	fEnd -= fStart;
	if (fValue < 1) return fEnd / 2 * fValue * fValue + fStart;
	fValue--;
	return -fEnd / 2 * (fValue * (fValue - 2) - 1) + fStart;


	/*
	fValue--;
	fEnd-= fStart;
	return fEnd * (fValue* fValue* fValue+ 1) + fStart;
	*/

	/*
	fValue--;
	fEnd-= fStart;
	return -fEnd* (fValue* fValue* fValue* fValue - 1) +fStart;
	*/
	// easeInQuad
	/*
	fEnd -= fStart;

	return fEnd * fValue* fValue + fStart;
	*/
	/* // EaseOut
	fEnd -= fStart;
	fValue = (fValue / 1) - 1;
	return fEnd * ((fValue)* fValue * ((s + 1) * fValue + s) + 1) + fStart;
	*/
}
