#pragma once
#include "EntireSpell.h"

class CExplosion : public CEntireSpell
{
public:
	CExplosion();
	virtual ~CExplosion();
public:
	virtual void Initialize();
	virtual int Update();
	virtual void Late_Update();
	virtual void Render(HDC hDC);
	virtual void Release();
	virtual void Collilsion_Event(CObj* _pObj);
	virtual void Count_Trigger(int _iTriggerCnt) override;
	virtual void Player_Effect();
};

