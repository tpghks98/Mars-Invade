#pragma once

#include "value.h"
#include "cNode.h"
class cCamera;
class CSoundManager;
class CSound;
class cSystemMgr
	:public cSingleTon<cSystemMgr>, public cNode
{

private:
	bool	m_IsKeyState[256];
	bool	m_IsPrevKeyState[256];

	ST_POINT	m_ptMouse;
	ST_POINT	m_ptGap;
	cCamera*	m_pCamera;
	std::list<cCamera*>	m_lstCamera;

	_SYNTHESIZE_REF_INHER(float, m_fTimeScale, TimeScale);
	_SYNTHESIZE_REF_INHER(float, m_fShakePower, ShakePower);
	_SYNTHESIZE_REF_INHER(float, m_fDecreasePower, DecreasePower);
	_SYNTHESIZE_REF_INHER(D3DXVECTOR2, m_vShakePlus, ShakePlus);

	CSoundManager*	m_pSoundManager;
	std::list<CSound*>	m_lstSound;
public:

	LPD3DXFONT	m_pFont;
public:
	void	Initialize();
	void	Update();

	void	CreateSound( LPWSTR pFileName, bool IsRepeat);

	ST_POINT	GetMousePosition() { return m_ptMouse; }
	void		SetMousePosition( const ST_POINT&  pt);
	float		GetDeltaTime() { float f = DXUTGetElapsedTime() * m_fTimeScale; if (f > 0.017f) f = 0.017f; return f; }

	int			GetKeyState(int nKey);

	cCamera*	GetMainCamera()	{
		return	m_pCamera;
	}
	cCamera*	AddCamera();
	bool		IsEmptyCameraList() { return m_lstCamera.empty(); }

	void		Shaking( float fPower, float fDecrease, bool IsShakeEye);
	void		ShakeUpdate();
	void		ReverseShake();
	void		RandomShakeValue();

	float		GetLerp(float fStart, float fEnd, float fValue);
private:
	void	InputUpdate();
public:
	cSystemMgr();
	~cSystemMgr();
};

