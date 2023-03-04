#include "stdafx.h"
#include "Gate.h"
#include "BitmapMgr.h"
#include "AbstractFactory.h"
#include "DamageUI.h"
#include "SoundMgr.h"
#include "SceneMgr.h"

CGate::CGate()
	:m_iSpawn1Cnt(0),m_iSpawn2Cnt(0), m_iSpawnTileNum(-1), m_iCannon1Cnt(0), m_iCannon2Cnt(0), 
	m_dwCannon(GetTickCount()), m_bGateDead(false)
{
	ZeroMemory(&m_tSpawnInfo, sizeof(INFO));
}


CGate::~CGate()
{
	Release();
}

void CGate::Initialize()
{
	// 200 x 360
	m_tInfo.fCX = 320.f;
	m_tInfo.fCY = 576.f;

	m_bAttacked = false;
	m_eObjID = GATE;
	m_ePreState = STATE_END;
	m_eCurState = IDLE;
	m_iTileNum = Check_Tile();
	m_tCreature = { 2000, 0, 100, 10 };

	BITMAP->Insert_Bmp(L"../Image/Enemie/Gate/Gate.bmp", L"Gate");
	BITMAP->Insert_Bmp(L"../Image/Enemie/Gate/Gate 15.bmp", L"Gate 15");
	m_pFrameKey = L"Gate";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iImageEnd = 3;
	m_tFrame.iMotion = 1;
	m_tFrame.iMotionEnd = 2;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();
}

int CGate::Update()
{
	if (m_bGateDead)
	{
		SCENE->Get_Scene()->Add_Object(ENEMIE, CAbstractFactory<CSerif>::Create(22));
		SOUND->PlaySound(L"explosion_bump.wav", SOUND_EFFECT02, 0.3f);	
		m_bGateDead = false;
	}
	if (m_eCurState == DEAD)
		return 0;

	if (m_tCreature.iHp <= 0)
	{
		for (auto& iter : SCENE->Get_Scene()->Get_ObjList(ENEMIE))
		{
			iter->Set_Dead();
		}
		m_eCurState = DEAD;
		m_bGateDead = true;
		return 0;
	}

	Reset_DamageFont();
	Attack();
	Spawn_Cannon();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CGate::Late_Update()
{
	if (m_eCurState == DEAD)
		return;
	Motion_Change();
	__super::Move_Frame();
}

void CGate::Render(HDC hDC)
{
	if (m_eCurState == DEAD)
	{
		HDC		hMemDC = BITMAP->Find_Img(L"Gate 15");
		GdiTransparentBlt(hDC,
			int(m_tRect.left),
			int(m_tRect.top),
			int(m_tInfo.fCX), // 키울 사이즈
			int(m_tInfo.fCY),
			hMemDC,
			0,
			0,
			170,		// 텍스처 원본 사이즈
			324,
			RGB(1, 1, 1));
		return;
	}
		
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left) - 20,
		int(m_tRect.top) - 20,
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		(3 - m_tFrame.iFrameCnt) * 200,
		m_tFrame.iMotionCnt * 360,
		200,		// 텍스처 원본 사이즈
		360,
		RGB(1, 1, 1));

	CCreature::Render(hDC);

	for (auto& iter : m_DamageList)
	{
		iter->Render_Damage(hDC);
	}
}

void CGate::Release()
{
}

void CGate::Attack()
{
	if (m_eCurState == DEAD)
		return;

	if (m_dwTime + 2000 < GetTickCount())
	{
		Select_Pattern();
		m_dwTime = GetTickCount();
	}
}

void CGate::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 11;
			m_tFrame.iImageEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.iMotionEnd = 2;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 150;
			m_tFrame.dwTime = GetTickCount();
			break;
		case DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iImageEnd = 9;
			m_tFrame.iMotion = 0;
			m_tFrame.iMotionEnd = 0;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 150;
			m_tFrame.dwTime = GetTickCount();
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CGate::Collilsion_Event(CObj * _pObj)
{
	if (m_eCurState == DEAD)
		return;

	switch (_pObj->Get_OBJID())
	{
	case SPELL:
		m_tCreature.iHp -= static_cast<CSpell*>(_pObj)->Get_SpellInfo().iAtk;
		SOUND->PlaySound(L"enemy_hit_light.wav", SOUND_EFFECT12, 0.3f);
		m_DamageList.push_back(CAbstractFactory<CDamageUI>::Create_DamageUI(m_tInfo.fX + 50, m_tInfo.fY - 250 - m_iDamageY, static_cast<CSpell*>(_pObj)->Get_SpellInfo().iAtk));
		Set_DamageY(20);
		break;
	default:
		break;
	}
}

void CGate::Count_Trigger(int _iTriggerCnt)
{
}

void CGate::Select_Pattern()
{
	if (m_iSpawn1Cnt != 0 && m_iSpawn2Cnt != 0)
		return;

	if (0 == m_iSpawn1Cnt)
	{
		++m_iSpawn1Cnt;
		m_iSpawnTileNum = 14;
	}
	else if (0 == m_iSpawn2Cnt)
	{
		++m_iSpawn2Cnt;
		m_iSpawnTileNum = 22;
	}

	int iNum = rand() % PATTERN_END;

	switch (iNum)
	{
	case BEAMER:
		SCENE->Get_Scene()->Add_Object(ENEMIE, CAbstractFactory<CGateBeamer>::Create(m_iSpawnTileNum));
		break;
	case MISSILER:
		SCENE->Get_Scene()->Add_Object(ENEMIE, CAbstractFactory<CGateMissiler>::Create(m_iSpawnTileNum));
		break;
	case SWEEPER:
		SCENE->Get_Scene()->Add_Object(ENEMIE, CAbstractFactory<CGateSweeper>::Create(m_iSpawnTileNum));
		break;
	default:
		break;
	}	
}

void CGate::Spawn_Cannon()
{
	if (m_tCreature.iMaxHp * 0.5f >= m_tCreature.iHp &&
		(m_iCannon1Cnt == 0 || m_iCannon2Cnt == 0))
	{
		if (m_dwCannon + 5000 < GetTickCount())
		{
			if (0 == m_iCannon1Cnt)
			{
				++m_iCannon1Cnt;
				m_iSpawnTileNum = 14;
			}
			else if (0 == m_iCannon2Cnt)
			{
				++m_iCannon2Cnt;
				m_iSpawnTileNum = 22;
			}
			
			SCENE->Get_Scene()->Add_Object(ENEMIE, CAbstractFactory<CGateCannon>::Create(m_iSpawnTileNum));
			m_dwCannon = GetTickCount();
		}
	}
}
