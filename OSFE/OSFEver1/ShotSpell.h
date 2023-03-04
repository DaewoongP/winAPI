#pragma once
#include "Spell.h"
class CShotSpell : public CSpell
{ // 투사체 (직선, 대각선)
public:
	CShotSpell();
	virtual ~CShotSpell();
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

