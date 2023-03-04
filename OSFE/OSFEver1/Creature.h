#pragma once
#include "Obj.h"
#include "ParticleEffect.h"
class CDamageUI;
class CCreature : public CObj
{
public:
	enum STATE { IDLE, RMOVE, LMOVE, ATTACK, ATTACK_END, HIT, EXHAUST, DEAD, STATE_END };
public:
	CCreature();
	virtual ~CCreature();
public:
	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC);
	virtual void Release() PURE;
	virtual void Collilsion_Event(CObj * _pObj) PURE;
	virtual void Count_Trigger(int _iTriggerCnt) PURE;
public:
	void Set_Damage(int _Damage);
	void Set_Shield(int _Shield) { m_tCreature.iShield += _Shield; }
	void Set_Heal(int _Heal) { m_tCreature.iHp += _Heal; }
	void Set_State(STATE _state) { m_eCurState = _state; }
	void Set_Loot(float _fExp, int _iMoney)
	{
		m_tCreature.fExp += _fExp;
		m_tCreature.iMoney += _iMoney;
	}

	CREATURE Get_Creature() { return m_tCreature; }
	void Reset_DamageFont();
	void Set_DamageY(int _Y) { m_iDamageY += _Y; }

	list<CDamageUI*> Get_DamageFont() { return m_DamageList; }
protected:
	CREATURE			m_tCreature;
	int					m_iPreTileNum;

	STATE				m_ePreState;
	STATE				m_eCurState;


	list<CDamageUI*>	m_DamageList;
	int					m_iDamageY;
	DWORD				m_dwDamageTime;
	bool				m_bDamaged;
};

