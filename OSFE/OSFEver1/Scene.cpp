#include "stdafx.h"
#include "Scene.h"
#include "TileMgr.h"
#include "CollilsionMgr.h"
#include "EffectMgr.h"
#include "AbstractFactory.h"
#include "MouseMgr.h"
#include "ScrollMgr.h"
#include "SceneMgr.h"

CScene::CScene()
	:m_bSound(false), m_fHit(0.f), m_bReload(false)
{
}


CScene::~CScene()
{
	Release();
}

void CScene::Add_Object(OBJID eID, CObj * pObj)
{
	if (OBJ_END <= eID || nullptr == pObj)
		return;

	m_ObjList[eID].push_back(pObj);
}

void CScene::Delete_Object(OBJID eID)
{
	for_each(m_ObjList[eID].begin(), m_ObjList[eID].end(), Safe_Delete<CObj*>);
	m_ObjList[eID].clear();
}

void CScene::AddUI(UIID eID, CUI * pUI)
{
	if (UI_END <= eID || nullptr == pUI)
		return;

	m_UIList[eID].push_back(pUI);
}

void CScene::DeleteUI(UIID eID)
{
	for_each(m_UIList[eID].begin(), m_UIList[eID].end(), Safe_Delete<CUI*>);
	m_UIList[eID].clear();
}

void CScene::Initialize()
{
	AddUI(UI_BACKGROUND, CAbstractFactory<CBackGroundUI>::Create_UI());
	TILE->Initialize();
	// 플레이어는 init 생략
	for (size_t i = PET; i < OBJ_END; ++i)
	{
		for (auto& iter = m_ObjList[i].begin();
		iter != m_ObjList[i].end(); ++iter)
		{
			(*iter)->Initialize();
		}
	}
	AddUI(UI_PLAYER, CAbstractFactory<CPlayerUI>::Create_UI());
	for (size_t i = 0; i < UI_END; ++i)
	{
		for (auto& iter = m_UIList[i].begin();
		iter != m_UIList[i].end(); ++iter)
		{
			(*iter)->Initialize();
		}
	}

	MOUSE->Initialize();
}

int CScene::Update()
{
	srand(unsigned(time(NULL)));

	TILE->Update();
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter = m_ObjList[i].begin();
		iter != m_ObjList[i].end();)
		{
			int iResult = (*iter)->Update();

			if (iResult == OBJ_DEAD)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}
	for (size_t i = 0; i < UI_END; ++i)
	{
		for (auto& iter = m_UIList[i].begin();
		iter != m_UIList[i].end(); ++iter)
		{
			(*iter)->Update();
		}
	}
	EFFECT->Update();

	MOUSE->Update();
	return 0;
}

void CScene::Late_Update()
{
	TILE->Late_Update();
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter = m_ObjList[i].begin();
		iter != m_ObjList[i].end(); ++iter)
		{
			(*iter)->Late_Update();

			if (m_ObjList[i].empty())
				break;
		}
	}
	if (!m_ObjList[ENEMIE].empty())
		CCollilsionMgr::Collision_Tile(m_ObjList[ENEMIE], m_ObjList[SPELL]);
	if (!m_ObjList[TURRET].empty())
		CCollilsionMgr::Collision_Tile(m_ObjList[TURRET], m_ObjList[SPELL]);
	if (!m_ObjList[ENEMIE].empty() && m_ObjList[ENEMIE].front()->Get_OBJID() == GATE)
		CCollilsionMgr::Collision_Gate(m_ObjList[ENEMIE].front(), m_ObjList[SPELL]);

	CCollilsionMgr::Collision_TileEvent(m_ObjList[PLAYER], m_ObjList[ENEMIE]);
	CCollilsionMgr::Collision_TileEvent(m_ObjList[PLAYER], m_ObjList[ENEMIE_SPELL]);

	if (!m_ObjList[PET].empty())
	{
		CCollilsionMgr::Collision_TileEvent(m_ObjList[PET], m_ObjList[ENEMIE]);
		CCollilsionMgr::Collision_TileEvent(m_ObjList[PET], m_ObjList[ENEMIE_SPELL]);
	}
	
	if (!m_ObjList[TURRET].empty())
	{
		CCollilsionMgr::Collision_TileEvent(m_ObjList[TURRET], m_ObjList[ENEMIE]);
		CCollilsionMgr::Collision_TileEvent(m_ObjList[TURRET], m_ObjList[ENEMIE_SPELL]);
	}
	

	for (size_t i = 0; i < UI_END; ++i)
	{
		for (auto& iter = m_UIList[i].begin();
		iter != m_UIList[i].end(); ++iter)
		{
			(*iter)->Late_Update();
		}
	}
	EFFECT->Late_Update();
	MOUSE->Late_Update();
}

void CScene::Render(HDC hDC)
{
	// 백그라운드
	for (auto& iter = m_UIList[UI_BACKGROUND].begin();
	iter != m_UIList[UI_BACKGROUND].end(); ++iter)
	{
		(*iter)->Render(hDC);
	}
	// 타일
	TILE->Render(hDC);
	// 타일 이펙트
	for (auto& iter : EFFECT->Get_Effect(EFFECT_TILE))
	{
		iter->Render(hDC);
	}
	// 오브젝트
	for (size_t i = PET; i < OBJ_END; ++i)
	{
		for (auto& iter = m_ObjList[i].begin();
		iter != m_ObjList[i].end(); ++iter)
		{
			(*iter)->Render(hDC);
		}

		if (i == SPELL)
			m_ObjList[PLAYER].front()->Render(hDC);
	}

	EFFECT->Render(hDC);
	
	for (size_t i = UI_DAMAGE; i < UI_END; ++i)
	{
		for (auto& iter = m_UIList[i].begin();
		iter != m_UIList[i].end(); ++iter)
		{
			(*iter)->Render(hDC);
		}
	}

	MOUSE->Render(hDC);
}

void CScene::Release()
{
	for (size_t i = GATE; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_Delete<CObj*>);
		m_ObjList[i].clear();
	}
	TILE->Destroy_Instance();
	EFFECT->Destroy_Instance();
	for (size_t i = 0; i < UI_END; ++i)
	{
		for_each(m_UIList[i].begin(), m_UIList[i].end(), Safe_Delete<CUI*>);
		m_UIList[i].clear();
	}
	MOUSE->Destroy_Instance();
	SCROLL->Destroy_Instance();
}

void CScene::Move_Stage()
{
	if (1460 < SCROLL->Get_ScrollX())
	{
		Scroll_Accel -= 0.5f;
		if (Scroll_Accel < -15.f)
		{
			Scroll_Accel = -15.5f;
		}
			
	}
	SCROLL->Set_ScrollX(16.f + Scroll_Accel);
}

void CScene::Player_Dead()
{
}
