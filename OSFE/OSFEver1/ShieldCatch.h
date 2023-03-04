#pragma once
#include "BuffSpell.h"
#include "TileMgr.h"
#include "ShieldEffect.h"
class CShieldCatch : public CBuffSpell
{
public:
	CShieldCatch();
	virtual ~CShieldCatch();
public:
	virtual void Initialize();
	virtual int Update();
	virtual void Late_Update();
	virtual void Render(HDC hDC);
	virtual void Release();
	virtual void Collilsion_Event(CObj* _pObj);
	virtual void Count_Trigger(int _iTriggerCnt);
	virtual void Player_Effect();
public:
	void Potion_Position();
private:
	bool	m_bJump;
	int		m_iTargetTile;
};

