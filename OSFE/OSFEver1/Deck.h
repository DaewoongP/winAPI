#pragma once
#include "Spell.h"
#include "Slot.h"

class CDeck
{
public:
	CDeck();
	~CDeck();
public:
	void Add_Spell(CSpell* _pSpell);
	void Delete_Spell(SPELLID _eID);
	void Reload_Spell();
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();
public:
	deque<CSpell*>&	 Get_Deck() { return m_SpellDeck; }
	deque<CSpell*>&	 Get_Inventory() { return m_SpellInventory; }
	void Update_Rect();
	void Set_DeckY(float _fY) { m_tInfo.fY = _fY; }
	CSlot* Get_Slot() { return m_pSlot; }

	void Push_SlotQ();
	void Push_SlotW();

	void Wait_Reload();
	void Render_Reload(HDC hDC);
	void Move_Frame();

	void Reload_Deck();

	void Push_Yami();
private:
	deque<CSpell*>		m_SpellDeck;
	CSlot*				m_pSlot;
	INFO				m_tInfo;
	RECT				m_tRect;
	deque<CSpell*>		m_SpellInventory;
	DWORD				m_dwTime;
	bool				m_bReload;
	TCHAR*				m_pFrameKey;
	FRAME				m_tFrame;
	bool				m_bSound;
};

