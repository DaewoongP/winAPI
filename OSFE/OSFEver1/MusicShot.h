#pragma once
#include "Enemie.h"
#include "MusicEffect.h"

class CMusicShot : public CEnemie
{
public:
	CMusicShot();
	virtual ~CMusicShot();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Attack() override;
	virtual void Motion_Change() {};
	virtual void Collilsion_Event(CObj * _pObj) {};
	virtual void Count_Trigger(int _iTriggerCnt);
	void Move();
private:
	INFO		m_PlayerInfo;
	int			m_PlayerTileNum;
	bool		bAttack;
	INFO		m_tViInfo;
};

