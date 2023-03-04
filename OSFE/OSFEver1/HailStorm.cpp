#include "stdafx.h"
#include "HailStorm.h"
#include "BitmapMgr.h"
#include "TileMgr.h"
#include "SoundMgr.h"


CHailStorm::CHailStorm()
{
}


CHailStorm::~CHailStorm()
{
	Release();
}

void CHailStorm::Initialize()
{
	m_tInfo.fCX = 384.f;
	m_tInfo.fCY = 576.f;
	// 비트맵은 캐스터에서 미리 대입 (여러번 적용을 피하기 위해서.)
	m_pFrameKey = L"HailStorm";
	m_eObjID = ENEMIE_SPELL;
	m_iAttack = 110;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 9;
	m_tFrame.iImageEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 2;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();

	m_iTileNum = Check_Tile();
	SOUND->PlaySound(L"flare-deploy.wav", SOUND_EFFECT14, 0.1f);
}

int CHailStorm::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Count_Trigger(35);

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CHailStorm::Late_Update()
{
	__super::Move_Frame();
	
	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
	{
		m_bDead = true;
	}
}

void CHailStorm::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		(3 - m_tFrame.iFrameCnt) * 240,
		m_tFrame.iMotionCnt * 360,
		240,		// 텍스처 원본 사이즈
		360,
		RGB(1, 1, 1));
}

void CHailStorm::Release()
{
	dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum))->Set_TileState(TILE_IDLE);
}

void CHailStorm::Attack()
{
	m_AttackTileList.clear();
	dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum))->Set_TileState(TILE_ATTACKED);
}

void CHailStorm::Count_Trigger(int _iTriggerCnt)
{
	if (m_iRenderCnt == _iTriggerCnt)
	{
		dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum))->Set_TileState(TILE_IDLE);
		m_AttackTileList.push_back(dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum)));
		m_iRenderCnt = 0;
	}
	else if (m_iRenderCnt < _iTriggerCnt)
		Attack();
	if (m_iRenderCnt)
		++m_iRenderCnt;
}

