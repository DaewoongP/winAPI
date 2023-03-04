#pragma once
#include "Spell.h"
#include "MusicEffect.h"
class CSpellMusicShot : public CSpell
{
public:
	CSpellMusicShot();
	virtual ~CSpellMusicShot();
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
	void Select_Music();
	void Target_Angle();
	void Attack();
private:
	INFO	m_tTargetInfo;
	float	m_fWidth;
	float	m_fHeight;
	static int	m_iSoundCnt;
};

