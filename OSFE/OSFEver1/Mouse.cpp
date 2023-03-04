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
	GetCursorPos(&Pt);	// ���콺 Ŀ���� ��ǥ���� ������ �Լ�
	ScreenToClient(g_hWnd, &Pt);	// ��ũ�� ��ǥ ������ ���콺�� Ŭ���̾�Ʈ ������ ��ǥ�� ��ȯ �����ִ� �Լ�

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
