#pragma once
#include "Spell.h"
#include "FoxPet.h"
#include "DogPet.h"
class CShop
{
public:
	CShop();
	~CShop();
public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();

	void Buy_Spell(deque<CSpell*>& _Spell);
	deque<CSpell*> Get_Spell() { return m_SpellDeque; }
private:
	deque<CSpell*>		m_SpellDeque;
};

