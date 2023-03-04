#pragma once
#include "EntireSpell.h"
class CThunder : public CEntireSpell
{
public:
	CThunder();
	virtual ~CThunder();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Collilsion_Event(CObj * _pObj) override;
	virtual void Count_Trigger(int _iTriggerCnt) override;
	virtual void Player_Effect() override;
private:
	bool			bOnce;
};

