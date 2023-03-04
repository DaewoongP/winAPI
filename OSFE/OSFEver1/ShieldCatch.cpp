#include "stdafx.h"
#include "ShieldCatch.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "EffectMgr.h"
#include "AbstractFactory.h"
#include "SoundMgr.h"

CShieldCatch::CShieldCatch()
	:m_iTargetTile(0)
{
}


CShieldCatch::~CShieldCatch()
{
}

void CShieldCatch::Initialize()
{
	// 48 x 48
	m_eSpellID = SPELL_SHIELDCATCH;
	m_pSpellName = L"방어막 잡기";
	m_tInfo.fCX = 76.8f;
	m_tInfo.fCY = 76.8f;
	m_tSpellInfo = { 0, 1, 1, 30, 60, L"잡으면 방어막 60 획득" };
	m_bJump = true;

	BITMAP->Insert_Bmp(L"../Image/Spell/ShieldCatch.bmp", m_pSpellName);
	
	m_pFrameKey = L"PotionShotBlue";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 1;
	m_tFrame.iImageEnd = 1;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 1;

	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;

	m_tFrame.dwSpeed = 50;
	m_tFrame.dwTime = GetTickCount();
	// 플레이어 전체 타일 중 랜덤위치
	m_iTargetTile = (rand() % 4) + 8 * (rand() % 4);

	m_fSpeed = 2.f;
	m_tSoundInfo = { L"foom.wav", SOUND_EFFECT04, 0.3f };
}

int CShieldCatch::Update()
{
	if (m_bDead)
	{
		dynamic_cast<CTile*>(TILE->Get_Tile(m_iTargetTile))->Set_TileState(TILE_IDLE);
		return OBJ_DEAD;
	}
	dynamic_cast<CTile*>(TILE->Get_Tile(m_iTargetTile))->Set_TileState(TILE_SHIELD);

	Potion_Position();
	Check_Tile();

	if (m_bJump == false && TILE->Get_TileInfo(m_iTargetTile).fY <= m_tInfo.fY)
	{
		if (SCENE->Get_Scene()->Get_Player()->Get_TileNum() == m_iTileNum)
		{
			static_cast<CCreature*>(SCENE->Get_Scene()->Get_Player())->Set_Shield(m_tSpellInfo.iShield);
			EFFECT->Add_Effect(EFFECT_SPELL, CAbstractFactory<CShieldEffect>::Create_Effect(
				SCENE->Get_Scene()->Get_Player()->Get_Info().fX,
				SCENE->Get_Scene()->Get_Player()->Get_Info().fY
				)); 
			SOUND->PlaySound(L"heal_glow.wav", SOUND_EFFECT05, 0.3f);
		}
		else
		{
			SOUND->PlaySound(L"glass_shatter_echo.wav", SOUND_EFFECT05, 0.3f);
		}
		m_bDead = true;
	}

	if (m_bJump == false && m_iTargetTile == m_iTileNum)
		Set_Pos(m_iTargetTile);

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CShieldCatch::Late_Update()
{
	__super::Move_Frame();
}

void CShieldCatch::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 48,
		m_tFrame.iMotionCnt * 48,
		48,		// 텍스처 원본 사이즈
		48,
		RGB(1, 1, 1));
}

void CShieldCatch::Release()
{
}

void CShieldCatch::Collilsion_Event(CObj * _pObj)
{
}

void CShieldCatch::Count_Trigger(int _iTriggerCnt)
{
	if (m_iRenderCnt == _iTriggerCnt)
	{
		
	}

	if (m_iRenderCnt)
		++m_iRenderCnt;
}

void CShieldCatch::Player_Effect()
{
}

void CShieldCatch::Potion_Position()
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
