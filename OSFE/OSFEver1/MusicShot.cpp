#include "stdafx.h"
#include "MusicShot.h"
#include "TileMgr.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "EffectMgr.h"
#include "AbstractFactory.h"
#include "SoundMgr.h"

CMusicShot::CMusicShot()
	:bAttack(false)
{
	ZeroMemory(&m_PlayerInfo, sizeof(m_PlayerInfo));
	ZeroMemory(&m_tViInfo, sizeof(m_tViInfo));
}


CMusicShot::~CMusicShot()
{
	Release();
}

void CMusicShot::Initialize()
{
	m_tInfo.fCX = 56.f;
	m_tInfo.fCY = 28.8f;
	// 비트맵은미리 대입 (여러번 적용을 피하기 위해서.)
	m_pFrameKey = L"MusicMeasureShot";
	m_eObjID = ENEMIE_SPELL;
	m_iAttack = 100;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 9;
	m_tFrame.iImageEnd = 2;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 3;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();
	m_fSpeed = 10.f;
	m_iTileNum = Check_Tile();
	
	m_iPreTileNum = m_iTileNum;

	m_tViInfo = SCENE->Get_Scene()->Get_ObjList(ENEMIE).front()->Get_Info();
	m_PlayerInfo = SCENE->Get_Scene()->Get_Player()->Get_Info();
	m_PlayerTileNum = SCENE->Get_Scene()->Get_Player()->Get_TileNum();
	// 시간나면 샷 넘버 랜덤
	SOUND->PlaySound(L"violin_shot_01.wav", SOUND_EFFECT08, 0.3f);
}

int CMusicShot::Update()
{
	if (m_bDead)
	{
		EFFECT->Add_Effect(EFFECT_SPELL, CAbstractFactory<CMusicEffect>::Create_Effect(
			m_PlayerInfo.fX, m_PlayerInfo.fY));
		return OBJ_DEAD;
	}
		
	Check_Tile();
	if (!bAttack)
		Move();
	else
		Attack();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CMusicShot::Late_Update()
{
	if (m_PlayerTileNum == m_iTileNum)
	{
		static_cast<CTile*>(TILE->Get_Tile(m_PlayerTileNum))->Set_TileState(TILE_IDLE);
		m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_PlayerTileNum)));
		m_bDead = true;
	}

	__super::Move_Frame();
}

void CMusicShot::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		(2 - m_tFrame.iFrameCnt) * 35,
		m_tFrame.iMotionCnt * 18,
		35,		// 텍스처 원본 사이즈
		18,
		RGB(1, 1, 1));
}

void CMusicShot::Release()
{
	static_cast<CTile*>(TILE->Get_Tile(m_PlayerTileNum))->Set_TileState(TILE_IDLE);
	m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_PlayerTileNum)));
	static_cast<CViolette*>(SCENE->Get_Scene()->Get_ObjList(ENEMIE).front())->Set_Pattern(false);
}

void CMusicShot::Attack()
{
	m_AttackTileList.clear();
	static_cast<CTile*>(TILE->Get_Tile(m_PlayerTileNum))->Set_TileState(TILE_ATTACKED);

	m_tInfo.fX -= 10.f;
	if (m_PlayerInfo.fY < m_tInfo.fY)
		m_tInfo.fY -= 4.f;
	else
		m_tInfo.fY += 4.f;
}

void CMusicShot::Count_Trigger(int _iTriggerCnt)
{
}

void CMusicShot::Move()
{
	m_tInfo.fX += 1.f;
	if (m_tInfo.fX > m_tViInfo.fX + 60)
		bAttack = true;
}
