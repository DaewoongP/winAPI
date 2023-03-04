#pragma once
#include "Spell.h"

class CSlot
{
public:
	CSlot();
	~CSlot();
public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();
public:
	deque<CSpell*>& Get_Slot() { return m_SpellSlot; }
	void Set_SlotQ(CSpell* _Spell) { m_SpellSlot.front() = _Spell; }
	void Set_SlotW(CSpell* _Spell) { m_SpellSlot.back() = _Spell; }

	void Set_InfoQ(INFO _InfoQ) { m_tInfoQ = _InfoQ; }
	void Set_InfoW(INFO _InfoW) { m_tInfoW = _InfoW; }

	bool Check_Empty();
	void Update_Rect();

private:
	deque<CSpell*>		m_SpellSlot;

	INFO				m_tInfoQ;
	INFO				m_tInfoW;
	RECT				m_tRectQ;
	RECT				m_tRectW;
};

