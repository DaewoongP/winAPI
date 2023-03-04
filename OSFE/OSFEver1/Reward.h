#pragma once	
#include "Spell.h"
class CReward
{
public:
	CReward();
	~CReward();
public:
	void Initialize();
	int Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();

	deque<CSpell*> Get_Spell() { return m_SpellReward; }
	void Select_Spell(deque<CSpell*>& _Spell);
private:
	deque<CSpell*>		m_SpellReward;
	bool				m_bSelectSpell;
};

