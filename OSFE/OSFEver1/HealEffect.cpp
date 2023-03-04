#include "stdafx.h"
#include "HealEffect.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"

CHealEffect::CHealEffect()
{
}


CHealEffect::~CHealEffect()
{
	Release();
}

void CHealEffect::Initialize()
{
	// 26 x 26
	BITMAP->Insert_Bmp(L"../Image/FX/HealthPickup.bmp", L"HealthPickup");
	m_pFrameKey = L"HealthPickup";
	m_tInfo.fCX = 41.6f;
	m_tInfo.fCY = 41.6f;

	m_iEffectCnt = 1;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 10;
	m_tFrame.iImageEnd = 8;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 1;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();
}

void CHealEffect::Update()
{
	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		--m_iEffectCnt;
	Set_Pos(SCENE->Get_Scene()->Get_Player()->Get_Info().fX,
		SCENE->Get_Scene()->Get_Player()->Get_Info().fY);
	__super::Update_Rect();
}

void CHealEffect::Late_Update()
{
	__super::Move_Frame();
}

void CHealEffect::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top) - 40,
		int(m_tInfo.fCX),
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 26,
		m_tFrame.iMotionCnt * 26,
		26,
		26,
		RGB(1, 1, 1));
}

void CHealEffect::Release()
{
}
