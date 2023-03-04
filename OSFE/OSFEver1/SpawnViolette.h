#pragma once
#include "SpawnSpell.h"
#include "SpellMusicShot.h"
class CSpawnViolette : public CSpawnSpell
{
private:
	enum STATE {MOVE, MUSIC, STATE_END};
public:
	CSpawnViolette();
	virtual ~CSpawnViolette();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Collilsion_Event(CObj * _pObj) override;
	virtual void Count_Trigger(int _iTriggerCnt) override;
	virtual void Player_Effect() override;
public:
	void Motion_Change();
private:
	CObj*			m_pMusicShot;
	bool			m_bSpawned;
	STATE			m_ePreState;
	STATE			m_eCurState;
};

