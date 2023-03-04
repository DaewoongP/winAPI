#pragma once
#include "UI.h"
#include "Creature.h"
#include "ShopKeeper.h"
#include "Deck.h"
#include "Slot.h"
#include "Tile.h"
#include "Bracer.h"
#include "Explosion.h"
#include "IceNeedle.h"
#include "Firestorm.h"
#include "SuperMiniGun.h"
#include "ShieldCatch.h"
#include "MagicFire.h"
#include "Thunder.h"
#include "Ragnarok.h"
#include "Aim.h"
#include "HitEffect.h"

class CPlayer : public CCreature
{
private:
	enum KEYINPUT {KEY_LEFT, KEY_UP, KEY_RIGHT, KEY_DOWN, KEY_NULL};
public:
	CPlayer();
	virtual ~CPlayer();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Collilsion_Event(CObj* _pObj) override;
	virtual void Count_Trigger(int _iTriggerCnt) override;
public:
	void Update_AttackInfo();

	void Motion_Change();
	void Set_Money(int _money) { m_tCreature.iMoney += _money; }
	void Set_Exhaust(bool _Exhaust) { m_bExhaust = _Exhaust; }
	void Set_Exhaust_byExplosion() 
	{
	 	m_eCurState = EXHAUST;
		m_dwExhaustTime = GetTickCount();
	}
	void Set_MP(int _mp) { m_tCreature.fMp -= _mp; }
	int	Get_PreTileNum() { return m_iPreTileNum; }
	
	CDeck*	Get_Deck() { return m_pDeck; }

	void Key_Input();
	void Key_Left();
	void Key_Up();
	void Key_Right();
	void Key_Down();

	void Other_Frame(FRAME& _frame);

	void Use_SlotQ();
	void Use_SlotW();
	void Open_Inventory();

	void Render_Spell_Slot(HDC hDC);
	void Render_Spell_Use(HDC hDC, CSpell* _Spell);

	void Level_Up();
	void Set_SavedVi(bool _saved) { m_bSavedVi = _saved; }
	bool Get_SavedVi() { return m_bSavedVi; }
private:
	int					m_preTileNum;
	vector<CSpell*>		m_pSpell;
	INFO				m_tAttackInfo;
	RECT				m_tAttackRect;
	KEYINPUT			m_iKeyInput;
	INFO				m_tPreLocation;
	FRAME				m_tFrame_Effect;
	FRAME				m_tFrame_Shield;
	DWORD				m_dwPreDamage;
	CAim*				m_pAim;
	bool				m_bFinMotion;
	CDeck*				m_pDeck;
	CSlot*				m_pSlot;

	bool				m_bExhaust;
	DWORD				m_dwExhaustTime;
	bool				m_bSavedVi;
};

