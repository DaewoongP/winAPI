#pragma once
#include "UI.h"
#include "Spell.h"
class CInventoryUI : public CUI
{
public:
	CInventoryUI();
	virtual ~CInventoryUI();
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
public:
	void Spell_BackGround(HDC hDC);
	void Mouse_OverEvent(HDC hDC, CSpell* _Spell, float _left, float _top);
private:
	deque<CSpell*>			m_pSpellInventory;
};

