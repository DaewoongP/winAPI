#include "stdafx.h"
#include "MapUI.h"
#include "BitmapMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"

CMapUI::CMapUI()
{
}


CMapUI::~CMapUI()
{
	Release();
}

void CMapUI::Initialize()
{
	m_eID = UI_MAP;
	m_tInfo = { 320, 320, 80, 80 };
	BITMAP->Insert_Bmp(L"../Image/UI/Map1.bmp", L"Map1");
	BITMAP->Insert_Bmp(L"../Image/UI/Map2.bmp", L"Map2");
	BITMAP->Insert_Bmp(L"../Image/UI/Map3.bmp", L"Map3");
	BITMAP->Insert_Bmp(L"../Image/UI/Map4.bmp", L"Map4");
	BITMAP->Insert_Bmp(L"../Image/UI/LocationMarker.bmp", L"LocationMarker");
	//BITMAP->Insert_Bmp(L"../Image/UI/Map4.bmp", L"Map4");
	m_fSpeed = 2.f;
}

void CMapUI::Update()
{
	switch (SCENE->Get_SceneID())
	{
	case SC_STAGE1:
		m_tInfo.fX = 320.f;
		break;
	case SC_STAGE2:
		m_tInfo.fX = 480.f;
		break;
	case SC_STAGE3:
		m_tInfo.fX = 640.f;
		break;
	case SC_STAGE4:
		m_tInfo.fX = 400.f;
		break;
	default:
		break;
	}
	if (m_tInfo.fCX < 80.f)
		m_fSpeed *= -1;
	else if (m_tInfo.fCX > 120.f)
		m_fSpeed *= -1;
	m_tInfo.fCX += m_fSpeed;
	m_tInfo.fCY += m_fSpeed;

	__super::Update_Rect();
}

void CMapUI::Late_Update()
{
}

void CMapUI::Render(HDC hDC)
{
	HDC	hMemDC = BITMAP->Find_Img(L"Map1");
	switch (SCENE->Get_SceneID())
	{
	case SC_STAGE1:
		hMemDC = BITMAP->Find_Img(L"Map1");
		break;
	case SC_STAGE2:
		hMemDC = BITMAP->Find_Img(L"Map2");
		break;
	case SC_STAGE3:
		hMemDC = BITMAP->Find_Img(L"Map3");
		break;
	case SC_STAGE4:
		hMemDC = BITMAP->Find_Img(L"Map4");
		break;
	default:
		break;
	}

	// location marker
	// 80 ~ 120
	StretchBlt(hDC,
		0,
		0,
		800, // 키울 사이즈
		600,
		hMemDC,
		0,
		0,
		800,		// 텍스처 원본 사이즈
		600,
		SRCCOPY);

	HDC	hLocationDC = BITMAP->Find_Img(L"LocationMarker");
	GdiTransparentBlt(hDC, 
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hLocationDC, 
		0, 0,
		512, 512,
		RGB(1, 1, 1));
}

void CMapUI::Release()
{
}

