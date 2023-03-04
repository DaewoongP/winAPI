#pragma once
#include "Define.h"
#include "Obj.h"
#include "UI.h"
#include "BackGroundUI.h"
#include "RewardUI.h"
#include "MapUI.h"
#include "NextUI.h"
#include "PlayerUI.h"
#include "Enemie.h"
#include "KeyMgr.h"

#include "Player.h"
#include "Liger.h"
#include "Reaper.h"
#include "Hailcaster.h"
#include "Violette.h"
#include "ShopKeeper.h"
#include "Shop.h"
#include "Serif.h"
#include "Gate.h"

class CScene
{
public:
	CScene();
	virtual ~CScene();
public:
	virtual void	Initialize();
	virtual int		Update();
	virtual void	Late_Update();
	virtual void	Render(HDC hDC);
	virtual void	Release();
public:
	void Add_Object(OBJID eID, CObj * pObj);
	void Delete_Object(OBJID eID);

	void AddUI(UIID eID, CUI* pUI);
	void DeleteUI(UIID eID);
public:
	virtual void	Move_Stage();

	CObj* Get_Player() { return m_ObjList[PLAYER].back(); }
	CObj* Get_Pet() 
	{ 
		if (!m_ObjList[PET].empty())
			return m_ObjList[PET].back();
		else
			return nullptr;
	}
	CObj* Get_ShopKeeper() { return m_ObjList[SHOP].back(); }
	list<CObj*> Get_ObjList(OBJID _ObjID) { return m_ObjList[_ObjID]; }
	list<CUI*> Get_UIList(UIID _UIID) { return m_UIList[_UIID]; }

	void Set_Hit(float _fscroll) 
	{ 
		m_dwTime = GetTickCount();
		m_fHit = _fscroll;
	}

	void Player_Dead();
protected:
	list<CObj*>			m_ObjList[OBJ_END];
	list<CUI*>			m_UIList[UI_END];
	list<CObj*>			m_RenderList[OBJ_END];

	bool				m_bSound;
	float				m_fHit;
	DWORD				m_dwTime;
	float				Scroll_Accel;
	bool				m_bReload;
};	

