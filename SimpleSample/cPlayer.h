#pragma once
#include "cMoveableObj.h"

class cGun;
class cEffect;
class cObjBase;
class cNumber;

class cPlayer :
	public cMoveableObj
{
private:
	_SYNTHESIZE_INHER(std::list<cGun*>, m_lstGun, lstGun);
	_SYNTHESIZE_REF_INHER(D3DXVECTOR3, m_vCameraDir, CameraDir);
	_SYNTHESIZE_REF_INHER(D3DXVECTOR3, m_vLocalRot, LocalRot);
	_SYNTHESIZE_REF_INHER(D3DXVECTOR3, m_vLocalPos, LocalPos);
	_SYNTHESIZE_REF_INHER(float, m_fPlusHeight, PlusHeight);
	_SYNTHESIZE_REF_INHER(float, m_fSubRot, SubRot);
	_SYNTHESIZE_INHER(int, m_nSubNum, SubNum);
	_SYNTHESIZE_INHER(int, m_nGunLevel, GunLevel);
	_SYNTHESIZE_INHER(int, m_nNumGuid, NumGuid);
	_SYNTHESIZE_INHER(float, m_fGuidTime , GuidTime);
	_SYNTHESIZE_INHER(cObjBase*, m_pBackEff, BackEff);
	_SYNTHESIZE_INHER(float, m_fBoostTime, BoostTime);


	_SYNTHESIZE_INHER(cEffect*, m_pShield, Shield);
	_SYNTHESIZE_INHER(char*, m_pEffectKey, EffectKey);
	_SYNTHESIZE_INHER(cEffect*, m_pFireEffect, FireEffect);

	
public:
	virtual	void	Initialize();
	virtual	void	Update();
	virtual void	Render();

	virtual	void	Translation();

	void	GunReset();

private:
	void	KeyInput();
	void	KeyRotation( float fAngle );
	void	BoostUpdate();
public:
	void	PlusAddMob();
	void	UpgradeGun();
	void	ResetGun();
	void	OnShield();
	virtual	void	OnCollision(cObjBase* pObj);
	virtual	void	OnHit(float fDamage);
public:
	cPlayer();
	~cPlayer();
};

