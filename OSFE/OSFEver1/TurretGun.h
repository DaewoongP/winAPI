#pragma once
#include "Creature.h"
#include "TurretBullet.h"
class CTurretGun : public CCreature
{
public:
	CTurretGun();
	virtual ~CTurretGun();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Collilsion_Event(CObj * _pObj) override;
	virtual void Count_Trigger(int _iTriggerCnt) override;
private:
	CObj*		m_pTurretBullet;
};

