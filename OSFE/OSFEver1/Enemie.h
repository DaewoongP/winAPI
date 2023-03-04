#pragma once
#include "Creature.h"
#include "Spell.h"
#include "Tile.h"
class CEnemie : public CCreature
{
protected:
	enum STATE { IDLE, ATTACK, HIT, DEAD, STATE_END };
public:
	CEnemie();
	virtual ~CEnemie();
public:
	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC) PURE;
	virtual void Release() PURE;
	virtual void Attack() PURE;
	virtual void Motion_Change() PURE;
	virtual void Collilsion_Event(CObj* _pObj) PURE;
	virtual void Count_Trigger(int _iTriggerCnt) PURE;
public:
	list<CTile*> Get_AttackTileList() { return m_AttackTileList; }
	int Get_Attack() { return m_iAttack; }

protected:
	STATE				m_ePreState;
	STATE				m_eCurState;
	int					m_iAttack;
	bool				m_bAttacked;
	list<CTile*>		m_AttackTileList;

	int					m_iPlayerTile;
	INFO				m_tPlayerInfo;
};

