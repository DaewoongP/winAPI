#include "stdafx.h"
#include "Yami.h"
#include "SoundMgr.h"
#include "BitmapMgr.h"
#include "AbstractFactory.h"
#include "SceneMgr.h"

CYami::CYami()
	:m_pYamiShot(nullptr)
{
}


CYami::~CYami()
{
	Release();
}

void CYami::Initialize()
{
	m_eSpellID = SPELL_YAMI;
	m_pSpellName = L"พ๎ตา";
	BITMAP->Insert_Bmp(L"../Image/Spell/Yami.bmp", m_pSpellName);
	BITMAP->Insert_Bmp(L"../Image/Spell/YamiShot.bmp", L"YamiShot");
	m_tSpellInfo = { 99999, 100, 4, 50, 0,
		L"" };
	if (!m_pYamiShot)
	{
		m_pYamiShot = CAbstractFactory<CYamiShot>::Create();
	}
}

int CYami::Update()
{
	if (0 > m_tSpellInfo.iCnt)
		return OBJ_DEAD;

	m_iTileNum = SCENE->Get_Scene()->Get_Player()->Get_TileNum();
	if (m_dwTime + 50 < GetTickCount())
	{
		SCENE->Get_Scene()->Add_Object(SPELL, CAbstractFactory<CYamiShot>::Create_Spell(*static_cast<CYamiShot*>(m_pYamiShot), m_iTileNum));
		--m_tSpellInfo.iCnt;
		m_dwTime = GetTickCount();
	}

	return OBJ_NOEVENT;
}

void CYami::Late_Update()
{
}

void CYami::Render(HDC hDC)
{
}

void CYami::Release()
{
	Safe_Delete<CObj*>(m_pYamiShot);
}

void CYami::Collilsion_Event(CObj * _pObj)
{
}

void CYami::Count_Trigger(int _iTriggerCnt)
{
}

void CYami::Player_Effect()
{
}
