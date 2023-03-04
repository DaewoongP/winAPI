#include "stdafx.h"
#include "LightBall.h"
#include "SoundMgr.h"
#include "BitmapMgr.h"
#include "TileMgr.h"

int CLightBall::m_siLoc = 0;

CLightBall::CLightBall()
{
	++m_siLoc;
	m_iLoc = m_siLoc;
}


CLightBall::~CLightBall()
{
	Release();
}

void CLightBall::Initialize()
{
	m_tInfo.fCX = 19.2f;
	m_tInfo.fCY = 19.2f;
	m_eObjID = ENEMIE_SPELL;
	m_fSpeed = 10.f;
	m_iAttack = 50;
	m_pFrameKey = L"LightBall";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 1;
	m_tFrame.iImageEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 1;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 120;
	m_tFrame.dwTime = GetTickCount();
}

int CLightBall::Update()
{
	if (m_bDead)
		return OBJ_DEAD;
	if (m_tRect.right < 0 || m_tRect.bottom < 0 ||
		m_tRect.left > WINCX || m_tRect.top > WINCY)
		return OBJ_DEAD;

	Check_Tile();

	if (m_iTileNum != m_iPreTileNum)
	{
		m_AttackTileList.push_back(dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum)));
		m_iPreTileNum = m_iTileNum;
	}
	else
		m_AttackTileList.clear();

	Attack();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CLightBall::Late_Update()
{
	__super::Move_Frame();
}

void CLightBall::Render(HDC hDC)
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
		m_tFrame.iFrameCnt * 12,
		m_tFrame.iMotionCnt * 12,
		12,		// 텍스처 원본 사이즈
		12,
		RGB(1, 1, 1));
}

void CLightBall::Release()
{
}

void CLightBall::Attack()
{
	switch (m_iLoc % 4)
	{
	case 0:
		m_tInfo.fX -= m_fSpeed;
		break;
	case 1:
		m_tInfo.fX += m_fSpeed;
		break;
	case 2:
		m_tInfo.fY -= m_fSpeed;
		break;
	case 3:
		m_tInfo.fY += m_fSpeed;
		break;
	default:
		break;
	}
}

void CLightBall::Motion_Change()
{
}

void CLightBall::Collilsion_Event(CObj * _pObj)
{
	this->Set_Dead();
}

void CLightBall::Count_Trigger(int _iTriggerCnt)
{
}
