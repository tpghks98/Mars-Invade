#pragma once
#include "cObjBase.h"
class cEffect :
	public cObjBase
{
private:
	_SYNTHESIZE_INHER(bool, m_IsRepeat, IsRepeat);
	_SYNTHESIZE_INHER(bool, m_IsBillBoard, IsBillBoard);
	_SYNTHESIZE_INHER(bool, m_IsAlphaChange, AlphaChange);
	_SYNTHESIZE_REF_INHER(bool, m_IsAlphaMinus, IsAlphaMinus);
	_SYNTHESIZE_REF_INHER(bool, m_IsAlphaDestroy, IsAlphaDestroy);
	_SYNTHESIZE_REF_INHER(bool, m_IsZEnable, IsZEnable);
public:
	virtual	void	Initialize();
	virtual	void	Update();
	virtual void	Render();

	virtual	void	OnAnimationEnd()	{ if (m_IsRepeat) { m_fAniKey = 0.0f; } else m_IsDelete = true; }
	virtual	void	Translation();
public:
	cEffect();
	~cEffect();
};

