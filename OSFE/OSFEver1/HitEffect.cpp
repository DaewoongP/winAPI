#include "stdafx.h"
#include "HitEffect.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"

CHitEffect::CHitEffect()
{
}


CHitEffect::~CHitEffect()
{
	Release();
}

void CHitEffect::Initialize()
{
	BITMAP->Insert_Bmp(L"../Image/FX/hitFX-01S.bmp", L"hitFX-01S");
	m_pFrameKey = L"hitFX-01S";
	m_tInfo.fCX = 192.f;
	m_tInfo.fCY = 192.f;

	m_iEffectCnt = 1;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.iImageEnd = 2;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 1;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();
}

void CHitEffect::Update()
{
	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		--m_iEffectCnt;
	
	__super::Update_Rect();
}

void CHitEffect::Late_Update()
{
	__super::Move_Frame();
}

void CHitEffect::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 120,
		m_tFrame.iMotionCnt * 120,
		120,
		120,
		RGB(1, 1, 1));
}

void CHitEffect::Release()
{
}
