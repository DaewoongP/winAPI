#include "stdafx.h"
#include "LightSwordEffect.h"
#include "BitmapMgr.h"

CLightSwordEffect::CLightSwordEffect()
{
}


CLightSwordEffect::~CLightSwordEffect()
{
	Release();
}

void CLightSwordEffect::Initialize()
{
	BITMAP->Insert_Bmp(L"../Image/FX/ShineBlast.bmp", L"ShineBlast");
	m_pFrameKey = L"ShineBlast";
	m_tInfo.fCX = 73.f;
	m_tInfo.fCY = 48.f;
	m_iEffectCnt = 1;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iImageEnd = 4;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 0;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();
}

void CLightSwordEffect::Update()
{
	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		--m_iEffectCnt;

	__super::Update_Rect();
}

void CLightSwordEffect::Late_Update()
{
	__super::Move_Frame();
}

void CLightSwordEffect::Render(HDC hDC)
{
	//46 x 30
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top - 10,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		m_tFrame.iFrameCnt * 46,
		m_tFrame.iMotionCnt * 30,
		46,
		30,
		RGB(1, 1, 1));
}

void CLightSwordEffect::Release()
{
}
