#include "stdafx.h"
#include "FateLine.h"
#include "TileMgr.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

CFateLine::CFateLine()
{
}


CFateLine::~CFateLine()
{
	Release();
}

void CFateLine::Initialize()
{
	//14 x 240

	m_tInfo.fCX = 22.4f;
	m_tInfo.fCY = 384.f;
	// 비트맵은미리 대입 (여러번 적용을 피하기 위해서.)
	m_pFrameKey = L"FateLine";
	m_eObjID = ENEMIE_SPELL;
	m_iAttack = 400;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 2;
	m_tFrame.iImageEnd = 2;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 0;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 400;
	m_tFrame.dwTime = GetTickCount();
	m_fSpeed = 20.f;
	m_iTileNum = Check_Tile();
	SOUND->PlaySound(L"flare-light.wav", SOUND_EFFECT10, 0.3f);
}

int CFateLine::Update()
{
	if (m_bDead)
		return OBJ_DEAD;
	m_tInfo.fX -= m_fSpeed;
	Attack();
	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CFateLine::Late_Update()
{
	if (m_tRect.right < TILEINIT_X)
	{
		for (int i = 3; i >= 0; --i)
		{
			static_cast<CTile*>(TILE->Get_Tile(i))->Set_TileState(TILE_IDLE);
			static_cast<CTile*>(TILE->Get_Tile(i + 8))->Set_TileState(TILE_IDLE);
			static_cast<CTile*>(TILE->Get_Tile(i + 16))->Set_TileState(TILE_IDLE);
			static_cast<CTile*>(TILE->Get_Tile(i + 24))->Set_TileState(TILE_IDLE);
		}
	}
	if (m_tRect.right < 0)
	{
		SCENE->Get_Scene()->Get_Player()->Collilsion_Event(this);
		m_bDead = true;
	}

	__super::Move_Frame();
}

void CFateLine::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 14,
		m_tFrame.iMotionCnt * 240,
		14,		// 텍스처 원본 사이즈
		240,
		RGB(1, 1, 1));
}

void CFateLine::Release()
{
	static_cast<CViolette*>(SCENE->Get_Scene()->Get_ObjList(ENEMIE).front())->Set_Pattern(false);
}

void CFateLine::Attack()
{
	m_AttackTileList.clear();
	if (m_dwTime + 50 < GetTickCount())
	{
		for (int i = 3; i >= 0; --i)
		{
			static_cast<CTile*>(TILE->Get_Tile(i))->Set_TileState(TILE_ATTACKED);
			static_cast<CTile*>(TILE->Get_Tile(i + 8))->Set_TileState(TILE_ATTACKED);
			static_cast<CTile*>(TILE->Get_Tile(i + 16))->Set_TileState(TILE_ATTACKED);
			static_cast<CTile*>(TILE->Get_Tile(i + 24))->Set_TileState(TILE_ATTACKED);
		}
		m_dwTime = GetTickCount();
	}
}

void CFateLine::Collilsion_Event(CObj * _pObj)
{
}

void CFateLine::Count_Trigger(int _iTriggerCnt)
{
}
