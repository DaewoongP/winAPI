#include "stdafx.h"
#include "Firestorm.h"
#include "BitmapMgr.h"
#include "AbstractFactory.h"
#include "SceneMgr.h"

CFirestorm::CFirestorm()
	:m_pFireRain(nullptr)
{
}


CFirestorm::~CFirestorm()
{
	Release();
}

void CFirestorm::Initialize()
{
	m_eSpellID = SPELL_FIRESTORM;
	m_pSpellName = L"È­¿° ÆøÇ³";
	BITMAP->Insert_Bmp(L"../Image/Spell/StormFire.bmp", m_pSpellName);
	BITMAP->Insert_Bmp(L"../Image/Spell/StormFireS.bmp", L"FireStorm");
	m_tSpellInfo = { 30, 12, 4, 40, 0,
	L"È­¿°À» ³²±â´Â Åõ»çÃ¼¸¦ ¶³¾î¶ß¸²"};
	if (!m_pFireRain)
	{
		m_pFireRain = CAbstractFactory<CFireRain>::Create();
	}
}

int CFirestorm::Update()
{
	if (0 > m_tSpellInfo.iCnt)
		return OBJ_DEAD;

	if (m_dwTime + 500 < GetTickCount())
	{
		for (int i = 0; i < 4; ++i)
		{
			SCENE->Get_Scene()->Add_Object(SPELL, CAbstractFactory<CFireRain>::Create_Spell(*static_cast<CFireRain*>(m_pFireRain),rand() % 4 + 8 * i + 4));
			--m_tSpellInfo.iCnt;
		}
		m_dwTime = GetTickCount();
	}

	return OBJ_NOEVENT;
}

void CFirestorm::Late_Update()
{
}

void CFirestorm::Render(HDC hDC)
{
}

void CFirestorm::Release()
{
	Safe_Delete<CObj*>(m_pFireRain);
}

void CFirestorm::Collilsion_Event(CObj * _pObj)
{
}

void CFirestorm::Count_Trigger(int _iTriggerCnt)
{
	if (m_iRenderCnt == _iTriggerCnt)
	{

	}

	if (m_iRenderCnt)
		++m_iRenderCnt;
}

void CFirestorm::Player_Effect()
{
}
