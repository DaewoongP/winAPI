#include "stdafx.h"
#include "Beam.h"
#include "TileMgr.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
CBeam::CBeam()
{
}


CBeam::~CBeam()
{
	Release();
}

void CBeam::Initialize()
{
	m_tInfo.fCX = 44.8f;
	m_tInfo.fCY = 372.8f;
	// 비트맵은미리 대입 (여러번 적용을 피하기 위해서.)
	m_pFrameKey = L"BreakerBeam";
	m_eObjID = ENEMIE_SPELL;
	m_iAttack = 100;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 6;
	m_tFrame.iImageEnd = 6;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 0;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 150;
	m_tFrame.dwTime = GetTickCount();

	m_iTileNum = Check_Tile();
	SOUND->PlaySound(L"beam_shot_low.wav", SOUND_EFFECT07, 0.3f);
}

int CBeam::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Attack();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CBeam::Late_Update()
{
	__super::Move_Frame();

	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
	{
		dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum))->Set_TileState(TILE_IDLE);
		m_AttackTileList.push_back(dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum)));
		m_bDead = true;
	}
}

void CBeam::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top) - 180,
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 28,
		m_tFrame.iMotionCnt * 233,
		28,		// 텍스처 원본 사이즈
		233,
		RGB(1, 1, 1));
}

void CBeam::Release()
{
	dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum))->Set_TileState(TILE_IDLE);
	m_AttackTileList.push_back(dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum)));
	static_cast<CViolette*>(SCENE->Get_Scene()->Get_ObjList(ENEMIE).front())->Set_Pattern(false);
}

void CBeam::Attack()
{
	m_AttackTileList.clear();
	dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum))->Set_TileState(TILE_ATTACKED);
}

void CBeam::Count_Trigger(int _iTriggerCnt)
{
}
