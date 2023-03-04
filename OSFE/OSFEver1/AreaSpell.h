#pragma once
#include "Spell.h"
class CAreaSpell : public CSpell
{ // 타일관련 장판 스펠
public:
	CAreaSpell();
	virtual ~CAreaSpell();
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

