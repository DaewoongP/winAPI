#include "stdafx.h"
#include "SuperMiniGun.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "AbstractFactory.h"

CSuperMiniGun::CSuperMiniGun()
{
}


CSuperMiniGun::~CSuperMiniGun()
{
	Release();
}

void CSuperMiniGun::Initialize()
{
	//22, 4
	m_eSpellID = SPELL_MINIGUN;
	m_pSpellName = L"슈퍼 미니 건";
	BITMAP->Insert_Bmp(L"../Image/Spell/SuperMinigun.bmp", m_pSpellName);
	m_tSpellInfo = { 2, 100, 4, 50, 0,
	L"100개의 총알을 발사"};
	m_pBullet = nullptr;
	if (!m_pBullet)
	{
		m_pBullet = CAbstractFactory<CBullet>::Create();
	}
}

int CSuperMiniGun::Update()
{
	if (0 > m_tSpellInfo.iCnt)
		return OBJ_DEAD;

	m_iTileNum = SCENE->Get_Scene()->Get_Player()->Get_TileNum();
	if (m_dwTime + 50 < GetTickCount())
	{
		SCENE->Get_Scene()->Add_Object(SPELL, CAbstractFactory<CBullet>::Create_Spell(*static_cast<CBullet*>(m_pBullet), m_iTileNum));
		--m_tSpellInfo.iCnt;
		m_dwTime = GetTickCount();
	}

	return OBJ_NOEVENT;
}

void CSuperMiniGun::Late_Update()
{
}

void CSuperMiniGun::Render(HDC hDC)
{
}

void CSuperMiniGun::Release()
{
	Safe_Delete<CObj*>(m_pBullet);
}

void CSuperMiniGun::Count_Trigger(int _iTriggerCnt)
{
}

void CSuperMiniGun::Player_Effect()
{
}
