#include "stdafx.h"
#include "GateBullet.h"
#include "BitmapMgr.h"
#include "TileMgr.h"

CGateBullet::CGateBullet()
{
}


CGateBullet::~CGateBullet()
{
	Release();
}

void CGateBullet::Initialize()
{
	m_tInfo.fCX = 12.f;
	m_tInfo.fCY = 12.f;
	m_fSpeed = 9.f;

	m_eObjID = ENEMIE_SPELL;
	m_iAttack = 100;
	m_pFrameKey = L"Sweeper_Bullet";
}

int CGateBullet::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Count_Trigger(20);

	Check_Tile();

	if (m_iTargetTile == m_iTileNum)
	{
		return OBJ_DEAD;
	}

	Set_Angle();
	
	m_tInfo.fX -= m_fWidth / m_fSpeed;
	m_tInfo.fY += m_fHeight / m_fSpeed;

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CGateBullet::Late_Update()
{
	__super::Move_Frame();
}

void CGateBullet::Render(HDC hDC)
{
	if (m_bRenderSkip)
	{
		m_bRenderSkip = false;
		return;
	}

	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		0,
		0,
		8,		// 텍스처 원본 사이즈
		8,
		RGB(1, 1, 1));
}

void CGateBullet::Release()
{
}

void CGateBullet::Attack()
{
}

void CGateBullet::Motion_Change()
{
}

void CGateBullet::Collilsion_Event(CObj * _pObj)
{
}

void CGateBullet::Count_Trigger(int _iTriggerCnt)
{
	if (m_iRenderCnt == 1)
		static_cast<CTile*>(TILE->Get_Tile(m_iTargetTile))->Set_TileState(TILE_ATTACKED);
	if (m_iRenderCnt == _iTriggerCnt)
	{
		static_cast<CTile*>(TILE->Get_Tile(m_iTargetTile))->Set_TileState(TILE_IDLE);
		m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTargetTile)));
	}
	if (m_iRenderCnt == _iTriggerCnt + 1)
	{
		m_AttackTileList.clear();
	}
	if (m_iRenderCnt)
		++m_iRenderCnt;
}

void CGateBullet::Set_Angle()
{
	m_fWidth = m_tInfo.fX - TILE->Get_TileInfo(m_iTargetTile).fX;
	m_fHeight =  TILE->Get_TileInfo(m_iTargetTile).fY - m_tInfo.fY;
}
