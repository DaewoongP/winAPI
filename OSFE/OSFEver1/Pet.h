#pragma once
#include "Creature.h"
class CPet : public CCreature
{
public:
	CPet();
	virtual ~CPet();
public:
	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC) PURE;
	virtual void Release() PURE;
	virtual void Motion_Change() PURE;
	virtual void Collilsion_Event(CObj * _pObj) PURE;
	virtual void Count_Trigger(int _iTriggerCnt) PURE;
};

