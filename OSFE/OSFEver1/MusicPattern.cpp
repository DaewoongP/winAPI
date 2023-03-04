#include "stdafx.h"
#include "MusicPattern.h"
#include "TileMgr.h"
#include "SceneMgr.h"
#include "Creature.h"
#include "AbstractFactory.h"
#include "EffectMgr.h"
#include "SoundMgr.h"

CMusicPattern::CMusicPattern()
	:m_iPatternCnt(0), m_iPatternEnd(0)
{
	ZeroMemory(&m_Pattern, sizeof(m_Pattern));
}


CMusicPattern::~CMusicPattern()
{
	Release();
}

void CMusicPattern::Initialize()
{
	//m_eSpellID = SPELL_SHIELDCATCH;
	m_tSpellInfo.iShield = 30;

	int Pattern1[16] = { 11, 10, 9, 1, 2, 1, 0, 1, 2, 10, 9, 8, 9, 17, 25, 24 };
	int Pattern2[17] = { 19, 11, 10, 2, 3, 11, 10, 2, 3, 11, 19, 27, 26, 18, 10, 2, 10 };

	switch (rand() % 2)
	{
	case 0:
		memcpy(m_Pattern, Pattern1, sizeof(m_Pattern));
		m_iPatternEnd = 16;
		break;
	case 1:
		memcpy(m_Pattern, Pattern2, sizeof(m_Pattern));
		m_iPatternEnd = 17;
		break;
	}
	
}

int  CMusicPattern::Update()
{
	if (m_bDead)
		return OBJ_DEAD;
	
	Check_Tile();

	if (m_dwTime + 500 < GetTickCount())
	{
		if (m_iPatternCnt > 0)
		{
			Check_Position();
			static_cast<CTile*>(TILE->Get_Tile(m_Pattern[m_iPatternCnt - 1]))->Set_TileState(TILE_IDLE);
		}
			
		static_cast<CTile*>(TILE->Get_Tile(m_Pattern[m_iPatternCnt]))->Set_TileState(TILE_MUSIC);
		++m_iPatternCnt;
		if (m_iPatternCnt + 1 > m_iPatternEnd)
		{
			static_cast<CTile*>(TILE->Get_Tile(m_Pattern[m_iPatternCnt - 1]))->Set_TileState(TILE_IDLE);
			m_bDead = true;
		}
			
		m_dwTime = GetTickCount();
	}
	return OBJ_NOEVENT;
}

void CMusicPattern::Late_Update()
{
}

void CMusicPattern::Render(HDC hDC)
{
}

void CMusicPattern::Release()
{
	SCENE->Get_Scene()->Add_Object(ENEMIE, CAbstractFactory<CFateLine>::Create(7));
}

void CMusicPattern::Collilsion_Event(CObj * _pObj)
{
}

void CMusicPattern::Count_Trigger(int _iTriggerCnt)
{
}

void CMusicPattern::Check_Position()
{
	if (SCENE->Get_Scene()->Get_Player()->Get_TileNum() == m_Pattern[m_iPatternCnt - 1])
	{
		static_cast<CCreature*>(SCENE->Get_Scene()->Get_Player())->Set_Shield(m_tSpellInfo.iShield);
		EFFECT->Add_Effect(EFFECT_TILE, CAbstractFactory<CRhythmEffect>::Create_Effect());
		SOUND->PlaySound(L"ice_crash_short.wav", SOUND_EFFECT08, 0.3f);
	}
}
