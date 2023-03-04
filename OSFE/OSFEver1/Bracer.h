#pragma once
#include "ShotSpell.h"
class CBracer : public CShotSpell
{
public:
	CBracer();
	virtual ~CBracer();
public:
	virtual void Initialize();
	virtual int Update();
	virtual void Late_Update();
	virtual void Render(HDC hDC);
	virtual void Release();
	virtual void Collilsion_Event(CObj* _pObj);
	virtual void Count_Trigger(int _iTriggerCnt);
	virtual void Player_Effect();
};

