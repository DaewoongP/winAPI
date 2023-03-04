#include "stdafx.h"
#include "NextUI.h"
#include "BitmapMgr.h"

CNextUI::CNextUI()
	:m_fSpeed(0)
{
}


CNextUI::~CNextUI()
{
	Release();
}

void CNextUI::Initialize()
{
	m_eID = UI_NEXT;
	m_tInfo.fX = 740.f;
	m_tInfo.fY = 300.f;
	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 75.f;
	m_fSpeed = 0.3f;
	BITMAP->Insert_Bmp(L"../Image/UI/MapTab.bmp", L"MapTab");

}

void CNextUI::Update()
{
	if (m_tInfo.fX > 750.f)
		m_fSpeed *= -1;
	else if (m_tInfo.fX < 730.f)
		m_fSpeed *= -1;

	m_tInfo.fX -= m_fSpeed;
	__super::Update_Rect();
}

void CNextUI::Late_Update()
{
}

void CNextUI::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(L"MapTab");
	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		0,
		0,
		234,		// 텍스처 원본 사이즈
		165,
		RGB(1, 1, 1));

}

void CNextUI::Release()
{
}
