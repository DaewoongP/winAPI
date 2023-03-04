#include "stdafx.h"
#include "Mouse.h"


CMouse::CMouse()
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
}


CMouse::~CMouse()
{
	Release();
}

void CMouse::Initialize()
{
}

void CMouse::Update()
{
	POINT	Pt{};

	//SetCursor(NULL);
	GetCursorPos(&Pt);	// 마우스 커서의 좌표값을 얻어오는 함수
	ScreenToClient(g_hWnd, &Pt);	// 스크린 좌표 상태의 마우스를 클라이언트 상태의 좌표로 변환 시켜주는 함수

	m_tInfo.fX = (float)Pt.x;
	m_tInfo.fY = (float)Pt.y;
}

void CMouse::Late_Update()
{
}

void CMouse::Render(HDC hDC)
{
}

void CMouse::Release()
{
}
