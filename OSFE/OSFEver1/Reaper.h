#pragma once
#include "Enemie.h"
class CReaper : public CEnemie
{
public:
	CReaper();
	virtual ~CReaper();
public:
	virtual void Initialize();
	virtual int Update();
	virtual void Late_Update();
	virtual void Render(HDC hDC);
	virtual void Release();
	virtual void Attack();
	virtual void Motion_Change();
	virtual void Collilsion_Event(CObj* _pObj);
	virtual void Count_Trigger(int _iTriggerCnt);
};

