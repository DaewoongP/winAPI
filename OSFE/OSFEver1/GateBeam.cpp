#include "stdafx.h"
#include "GateBeam.h"
#include "BitmapMgr.h"
#include "TileMgr.h"
#include "EffectMgr.h"
#include "BeamLineEffect.h"

CGateBeam::CGateBeam()
{
}


CGateBeam::~CGateBeam()
{
	Release();
}

void CGateBeam::Initialize()
{
	m_tInfo.fCX = 60.8f;
	m_tInfo.fCY = 32.f;

	m_eObjID = ENEMIE_SPELL;

	BITMAP->Insert_Bmp(L"../Image/Enemie/Gate/TracerBlast.bmp", L"TracerBlast");
	m_pFrameKey = L"TracerBlast";

	m_iAttack = 50;
	m_fSpeed = 30.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 2;
	m_tFrame.iImageEnd = 1;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 1;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 200;
	m_tFrame.dwTime = GetTickCount();
	m_tPointPtr = { &m_tInfo.fX, &m_tInfo.fY };
}

int CGateBeam::Update()
{
	if (m_bDead)
		return OBJ_DEAD;
	if (m_tInfo.fX > 600)
		return OBJ_DEAD;
	
	Move_Tile();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CGateBeam::Late_Update()
{
	__super::Move_Frame();
}

void CGateBeam::Render(HDC hDC)
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
		m_tFrame.iFrameCnt * 38,
		m_tFrame.iMotionCnt * 20,
		38,		// 텍스처 원본 사이즈
		20,
		RGB(1, 1, 1));
}

void CGateBeam::Release()
{
}

void CGateBeam::Attack()
{
	if (m_dwTime + 200 < GetTickCount())
	{
		
		m_dwTime = GetTickCount();
	}
}

void CGateBeam::Motion_Change()
{
}

void CGateBeam::Collilsion_Event(CObj * _pObj)
{
}

void CGateBeam::Count_Trigger(int _iTriggerCnt)
{
}

void CGateBeam::Move_Tile()
{
	if (m_bAttacked)
	{
		m_AttackTileList.clear();
		m_bAttacked = false;
	}

	if (m_iTileNum / 8 == 0 || m_iTileNum / 8 == 2)
	{
		m_tInfo.fX += (TILECX + TILEITV) / m_fSpeed;
		m_tInfo.fY += (TILECY + TILEITV) / m_fSpeed;

		if (TILE->Get_TileInfo(m_iTileNum).fX + (TILECX + TILEITV) <= m_tInfo.fX)
		{
			Check_Tile();
			Set_Pos(m_iTileNum);
			m_bAttacked = true;
			m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum)));
		}
	}
	else if (m_iTileNum / 8 == 1 || m_iTileNum / 8 == 3)
	{
		m_tInfo.fX += (TILECX + TILEITV) / m_fSpeed;
		m_tInfo.fY -= (TILECY + TILEITV) / m_fSpeed;
		
		if (TILE->Get_TileInfo(m_iTileNum).fX + (TILECX + TILEITV) <= m_tInfo.fX)
		{
			Check_Tile();
			Set_Pos(m_iTileNum);
			m_bAttacked = true;
			m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum)));
		}
	}
	
}

