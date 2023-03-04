#include "stdafx.h"
#include "SpawnTurretGun.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "AbstractFactory.h"

CSpawnTurretGun::CSpawnTurretGun()
{
}


CSpawnTurretGun::~CSpawnTurretGun()
{
	Release();
}

void CSpawnTurretGun::Initialize()
{
	m_eSpellID = SPELL_TURRETGUN;
	m_pSpellName = L"家醚 磐房";
	BITMAP->Insert_Bmp(L"../Image/Spell/SumTurretgun.bmp", m_pSpellName);
	BITMAP->Insert_Bmp(L"../Image/Spell/DroneShot.bmp", L"TurretBullet");
	m_tSpellInfo = { 10, 1, 3, 30, 0,
		L"家醚 磐房阑 家券" };
}

int CSpawnTurretGun::Update()
{
	if (0 >= m_tSpellInfo.iCnt)
		return OBJ_DEAD;

	m_iTileNum = SCENE->Get_Scene()->Get_Player()->Get_TileNum();

	SCENE->Get_Scene()->Add_Object(TURRET, CAbstractFactory<CTurretGun>::Create(m_iTileNum + 1));
	--m_tSpellInfo.iCnt;


	return OBJ_NOEVENT;
}

void CSpawnTurretGun::Late_Update()
{
}

void CSpawnTurretGun::Render(HDC hDC)
{
}

void CSpawnTurretGun::Release()
{
}

void CSpawnTurretGun::Collilsion_Event(CObj * _pObj)
{
}

void CSpawnTurretGun::Count_Trigger(int _iTriggerCnt)
{
}

void CSpawnTurretGun::Player_Effect()
{
}
