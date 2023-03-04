#pragma once
#include "Spell.h"
class CBuffSpell : public CSpell
{ // น๖วม
public:
	CBuffSpell();
	virtual ~CBuffSpell();
public:
	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC) PURE;
	virtual void Release() PURE;
	virtual void Collilsion_Event(CObj* _pObj) PURE;
	virtual void Count_Trigger(int _iTriggerCnt) PURE;
	virtual void Player_Effect() PURE;
};

