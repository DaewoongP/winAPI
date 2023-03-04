#include "stdafx.h"
#include "HealOrb.h"
#include "BitmapMgr.h"
#include "SoundMgr.h"
#include "TileMgr.h"
#include "SceneMgr.h"
#include "EffectMgr.h"
#include "AbstractFactory.h"
#include "HealEffect.h"

CHealOrb::CHealOrb()
	:m_bJump(false)
{
}


CHealOrb::~CHealOrb()
{
	Release();
}

void CHealOrb::Initialize()
{
	// 24 * 24
	m_tInfo.fCX = 38.4f;
	m_tInfo.fCY = 38.4f;
	m_bJump = true;

	BITMAP->Insert_Bmp(L"../Image/Pet/HealOrbS.bmp", L"HealOrbS");

	m_pFrameKey = L"HealOrbS";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iImageEnd = 2;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 1;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 50;
	m_tFrame.dwTime = GetTickCount();
	// 플레이어 전체 타일 중 랜덤위치
	m_iTargetTile = (rand() % 4) + 8 * (rand() % 4);

	m_fSpeed = 2.5f;
	m_tSoundInfo = { L"foom.wav", SOUND_EFFECT04, 0.3f };
}

int CHealOrb::Update()
{
	if (m_bDead)
	{
		dynamic_cast<CTile*>(TILE->Get_Tile(m_iTargetTile))->Set_TileState(TILE_IDLE);
		return OBJ_DEAD;
	}
	dynamic_cast<CTile*>(TILE->Get_Tile(m_iTargetTile))->Set_TileState(TILE_HEAL);

	Heal_Position();
	Check_Tile();

	if (m_bJump == false && TILE->Get_TileInfo(m_iTargetTile).fY <= m_tInfo.fY)
	{
		if (SCENE->Get_Scene()->Get_Player()->Get_TileNum() == m_iTileNum)
		{
			static_cast<CCreature*>(SCENE->Get_Scene()->Get_Player())->Set_Heal(100);
			EFFECT->Add_Effect(EFFECT_SPELL, CAbstractFactory<CHealEffect>::Create_Effect(
				SCENE->Get_Scene()->Get_Player()->Get_Info().fX,
				SCENE->Get_Scene()->Get_Player()->Get_Info().fY
				)); 
			SOUND->PlaySound(L"heal_glow.wav", SOUND_EFFECT05, 0.3f);
		}
		m_bDead = true;
	}

	if (m_bJump == false && m_iTargetTile == m_iTileNum)
		Set_Pos(m_iTargetTile);

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CHealOrb::Late_Update()
{
	__super::Move_Frame();
}

void CHealOrb::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 24,
		m_tFrame.iMotionCnt * 24,
		24,		// 텍스처 원본 사이즈
		24,
		RGB(1, 1, 1));
}

void CHealOrb::Release()
{
}

void CHealOrb::Heal_Position()
{
	if (TILE->Get_TileInfo(m_iTargetTile).fX <= m_tInfo.fX)
	{
		m_tInfo.fX -= m_fSpeed;
	}
	else
		m_tInfo.fX += m_fSpeed;

	if (TILEINIT_Y - TILECY > m_tInfo.fY)
		m_bJump = false;

	if (m_bJump)
		m_tInfo.fY -= 3.f;
	else
		m_tInfo.fY += 4.f;
}

void CHealOrb::Collilsion_Event(CObj * _pObj)
{
}

void CHealOrb::Count_Trigger(int _iTriggerCnt)
{
}

void CHealOrb::Player_Effect()
{
}
