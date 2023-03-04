#pragma once
#include "ShotSpell.h"
class CBullet : public CShotSpell
{
public:
	CBullet();
	virtual ~CBullet();
public:
	virtual void Initialize();
	virtual int Update();
	virtual void Late_Update();
	virtual void Render(HDC hDC);
	virtual void Release();
	virtual void Collilsion_Event(CObj* _pObj);
	virtual void Count_Trigger(int _iTriggerCnt);
	virtual void Player_Effect() {}
};

