#pragma once
#include "ShotSpell.h"
#include "Bullet.h"
class CSuperMiniGun : public CShotSpell
{
public:
	CSuperMiniGun();
	virtual ~CSuperMiniGun();
public:
	virtual void Initialize();
	virtual int Update();
	virtual void Late_Update();
	virtual void Render(HDC hDC);
	virtual void Release();
	virtual void Collilsion_Event(CObj* _pObj) {}
	virtual void Count_Trigger(int _iTriggerCnt);
	virtual void Player_Effect();
private:
	CObj*		m_pBullet;
};

