#include "stdafx.h"
#include "MusicEffect.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"

CMusicEffect::CMusicEffect()
{
}


CMusicEffect::~CMusicEffect()
{
	Release();
}

void CMusicEffect::Initialize()
{
	BITMAP->Insert_Bmp(L"../Image/FX/MusicBlast.bmp", L"MusicBlast");
	m_pFrameKey = L"MusicBlast";
	m_tInfo.fCX = 57.6f;
	m_tInfo.fCY = 86.4f;

	m_iEffectCnt = 1;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 9;
	m_tFrame.iImageEnd = 6;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 1;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();
}

void CMusicEffect::Update()
{
	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		--m_iEffectCnt;
	Set_Pos(m_tInfo.fX,m_tInfo.fY);
	__super::Update_Rect();
}

void CMusicEffect::Late_Update()
{
	__super::Move_Frame();
}

void CMusicEffect::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 36,
		m_tFrame.iMotionCnt * 54,
		36,
		54,
		RGB(1, 1, 1));
}

void CMusicEffect::Release()
{
}
