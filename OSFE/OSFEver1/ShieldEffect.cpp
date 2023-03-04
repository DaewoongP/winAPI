#include "stdafx.h"
#include "ShieldEffect.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"

CShieldEffect::CShieldEffect()
{
}


CShieldEffect::~CShieldEffect()
{
	Release();
}

void CShieldEffect::Initialize()
{
	// 18 x 38
	BITMAP->Insert_Bmp(L"../Image/FX/ShieldBlast.bmp", L"ShieldBlast");
	m_pFrameKey = L"ShieldBlast";
	m_tInfo.fCX = 28.8f;
	m_tInfo.fCY = 60.8f;

	m_iEffectCnt = 1;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 8;
	m_tFrame.iImageEnd = 6;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 1;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();
}

void CShieldEffect::Update()
{
	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		--m_iEffectCnt;
	Set_Pos(SCENE->Get_Scene()->Get_Player()->Get_Info().fX,
		SCENE->Get_Scene()->Get_Player()->Get_Info().fY);
	__super::Update_Rect();
}

void CShieldEffect::Late_Update()
{
	__super::Move_Frame();
}

void CShieldEffect::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 18,
		m_tFrame.iMotionCnt * 38,
		18,
		38,
		RGB(1, 1, 1));
}

void CShieldEffect::Release()
{
}
