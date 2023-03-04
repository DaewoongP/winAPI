#include "stdafx.h"
#include "ExplodeEffect.h"
#include "BitmapMgr.h"

CExplodeEffect::CExplodeEffect()
{
}


CExplodeEffect::~CExplodeEffect()
{
	Release();
}

void CExplodeEffect::Initialize()
{
	// 50 x 40
	BITMAP->Insert_Bmp(L"../Image/FX/Explosion1x1.bmp", L"Explosion1x1");
	m_pFrameKey = L"Explosion1x1";
	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 84.f;
	m_iEffectCnt = 1;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 11;
	m_tFrame.iImageEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 2;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();
}

void CExplodeEffect::Update()
{
	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		--m_iEffectCnt;

	__super::Update_Rect();
}

void CExplodeEffect::Late_Update()
{
	__super::Move_Frame();
}

void CExplodeEffect::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top - 30,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		m_tFrame.iFrameCnt * 50,
		m_tFrame.iMotionCnt * 40,
		50,
		40,
		RGB(1, 1, 1));
}

void CExplodeEffect::Release()
{
}
