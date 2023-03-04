#include "stdafx.h"
#include "SpawnTurretLaser.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "AbstractFactory.h"

CSpawnTurretLaser::CSpawnTurretLaser()
{
}


CSpawnTurretLaser::~CSpawnTurretLaser()
{
	Release();
}

void CSpawnTurretLaser::Initialize()
{
	m_eSpellID = SPELL_TURRETLASER;
	m_pSpellName = L"레이저 터렛";
	BITMAP->Insert_Bmp(L"../Image/Spell/SumTurretLaser.bmp", m_pSpellName);
	m_tSpellInfo = { 150, 1, 3, 30, 0,
		L"레이저 터렛을 소환" };
}

int CSpawnTurretLaser::Update()
{
	if (0 >= m_tSpellInfo.iCnt)
		return OBJ_DEAD;

	m_iTileNum = SCENE->Get_Scene()->Get_Player()->Get_TileNum();

	SCENE->Get_Scene()->Add_Object(TURRET, CAbstractFactory<CTurretLaser>::Create(m_iTileNum + 1));
	--m_tSpellInfo.iCnt;

	return OBJ_NOEVENT;
}

void CSpawnTurretLaser::Late_Update()
{
}

void CSpawnTurretLaser::Render(HDC hDC)
{
}

void CSpawnTurretLaser::Release()
{
}

void CSpawnTurretLaser::Collilsion_Event(CObj * _pObj)
{
}

void CSpawnTurretLaser::Count_Trigger(int _iTriggerCnt)
{
}

void CSpawnTurretLaser::Player_Effect()
{
}
