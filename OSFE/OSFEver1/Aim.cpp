#include "stdafx.h"
#include "Aim.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"


CAim::CAim()
{
}


CAim::~CAim()
{
	Release();
}

void CAim::Initialize()
{
	BITMAP->Insert_Bmp(L"../Image/Player/AimMarker.bmp", L"AimMarker");
	m_pFrameKey = L"AimMarker";
	m_tInfo.fCX = 96;
	m_tInfo.fCY = 80;
	m_iEffectCnt = 1;
}

void CAim::Update()
{
	Aim_Pos();
	__super::Update_Rect();
}

void CAim::Late_Update()
{
	__super::Move_Frame();
}

void CAim::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY),
		hMemDC,
		0,
		0,
		60,
		50,
		RGB(1, 1, 1));
}

void CAim::Release()
{
}

void CAim::Aim_Pos()
{
	m_tInfo.fX = dynamic_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Get_Info().fX
		+ (TILECX + TILEITV) * 4.f;
	m_tInfo.fY = dynamic_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Get_Info().fY;
}
