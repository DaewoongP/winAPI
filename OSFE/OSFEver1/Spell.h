#pragma once
#include "Obj.h"
class CSpell : public CObj
{
public:
	CSpell();
	virtual ~CSpell();
public:
	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC) PURE;
	virtual void Release() PURE;
	virtual void Collilsion_Event(CObj* _pObj) PURE;
	virtual void Count_Trigger(int _iTriggerCnt) PURE;
	virtual void Player_Effect() PURE;
public:
	virtual void Render_SpellIcon(HDC hDC, int _left, int _top, int _CX, int _CY);
	virtual void Render_SpellCard(HDC hDC, int _left, int _top);
	
	SPELLINFO Get_SpellInfo() { return m_tSpellInfo; }
	SPELLID Get_SpellID() { return m_eSpellID; }
	TCHAR* Get_SpellName() { return m_pSpellName; }
	SOUNDINFO Get_SoundInfo() { return m_tSoundInfo; }

	void Set_PlayerPos(int _TileNum)
	{ 
		m_iPlayerTile = _TileNum; 
		Set_Pos(_TileNum);
	}
protected:
	SPELLINFO		m_tSpellInfo;
	int				m_iPlayerTile;
	SPELLID			m_eSpellID;
	TCHAR*			m_pSpellName;
	SOUNDINFO		m_tSoundInfo;
};
