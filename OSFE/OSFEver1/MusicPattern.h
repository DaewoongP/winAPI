#pragma once
#include "Spell.h"
#include "FateLine.h"
#include "RhythmEffect.h"

class CMusicPattern : public CSpell
{
public:
	CMusicPattern();
	~CMusicPattern();
public:
	virtual void Initialize();
	virtual int  Update();
	virtual void Late_Update();
	virtual void Render(HDC hDC);
	virtual void Release();
	virtual void Collilsion_Event(CObj* _pObj);
	virtual void Count_Trigger(int _iTriggerCnt);
	virtual void Render_SpellIcon(HDC hDC, int _left, int _top) {}
	virtual void Player_Effect() {}
public:
	void Check_Position();
private:
	int			m_Pattern[30];
	int			m_iPatternCnt;
	int			m_iPatternEnd;
};

