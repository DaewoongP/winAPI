#pragma once
#include "Pet.h"
#include "HealOrb.h"
class CFoxPet : public CPet
{
public:
	CFoxPet();
	virtual ~CFoxPet();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Motion_Change() override;
	virtual void Collilsion_Event(CObj * _pObj) override;
	virtual void Count_Trigger(int _iTriggerCnt) override;
public:
	void Heal();
};

