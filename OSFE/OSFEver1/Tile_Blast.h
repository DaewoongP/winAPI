#pragma once
#include "Enemie.h"
#include "Serif.h"
class CTile_Blast : public CEnemie
{
public:
	CTile_Blast();
	virtual ~CTile_Blast();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Attack() override;
	virtual void Motion_Change() override;
	virtual void Collilsion_Event(CObj * _pObj) override;
	virtual void Count_Trigger(int _iTriggerCnt) override;
};

