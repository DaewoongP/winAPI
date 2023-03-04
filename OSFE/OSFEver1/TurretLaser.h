#pragma once
#include "Creature.h"
class CTurretLaser : public CCreature
{
public:
	CTurretLaser();
	virtual ~CTurretLaser();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Collilsion_Event(CObj * _pObj) override;
	virtual void Count_Trigger(int _iTriggerCnt) override;
private:
	int			m_iFrameCnt;
	bool		m_bLaserRender;
};

