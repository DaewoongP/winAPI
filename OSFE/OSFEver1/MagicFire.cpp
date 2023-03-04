#include "stdafx.h"
#include "MagicFire.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"

CMagicFire::CMagicFire()
{
}


CMagicFire::~CMagicFire()
{
	Release();
}

void CMagicFire::Initialize()
{
	BITMAP->Insert_Bmp(L"../Image/Player/MagicChargeFire.bmp", L"MagicChargeFire");
	m_pFrameKey = L"MagicChargeFire";
	m_tInfo.fCX = 25.2f;
	m_tInfo.fCY = 121.6f;
	m_iEffectCnt = 66;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iImageEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 0;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();
}

void CMagicFire::Update()
{
	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		--m_iEffectCnt;

	Set_Pos(SCENE->Get_Scene()->Get_Player()->Get_Info().fX + 45.f,
		SCENE->Get_Scene()->Get_Player()->Get_Info().fY - 45.f);
	__super::Update_Rect();
}

void CMagicFire::Late_Update()
{
	__super::Move_Frame();
}

void CMagicFire::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 22,
		m_tFrame.iMotionCnt * 76,
		22,
		76,
		RGB(5, 3, 1));
}

void CMagicFire::Release()
{
}
