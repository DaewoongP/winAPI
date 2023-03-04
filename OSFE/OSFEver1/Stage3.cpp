#include "stdafx.h"
#include "Stage3.h"
#include "BitmapMgr.h"
#include "AbstractFactory.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "SoundMgr.h"
#include "ScrollMgr.h"

CStage3::CStage3()
{
}


CStage3::~CStage3()
{
	Release();
}

void CStage3::Initialize()
{
	BITMAP->Insert_Bmp(L"../Image/Stage/Stage1/Ice-bg.bmp", L"Ice-bg");
	__super::Initialize();
	Scroll_Accel = 0;
	SOUND->PlayBGM(L"The_Arctic.wav", 0.2f);
	// 상점 스테이지
	Add_Object(SHOP, CAbstractFactory<CShopKeeper>::Create(20));
}

int CStage3::Update()
{
	if (!m_bReload)
	{
		static_cast<CPlayer*>(Get_Player())->Get_Deck()->Reload_Deck();
		m_bReload = true;
	}

	if (!Get_UIList(UI_MAP).empty())
	{
		if (KEY->Key_Up('Q'))
		{
			SCENE->Scene_Change(SC_STAGE4);
			return 0;
		}
	}

	if (m_ObjList[ENEMIE].empty())
	{
		if (Get_UIList(UI_NEXT).empty())
			AddUI(UI_NEXT, CAbstractFactory<CNextUI>::Create_UI());
		if (m_bSound)
		{
			SOUND->StopSound(SOUND_BGM);
			SOUND->PlayBGM(L"Victory.wav", 0.2f);
			m_bSound = false;
		}
		if (KEY->Key_Down(VK_TAB))
		{
			if (Get_UIList(UI_MAP).empty())
				AddUI(UI_MAP, CAbstractFactory<CMapUI>::Create_UI());
			else
				DeleteUI(UI_MAP);
			return 0;
		}
	}

	if (SCROLL->Get_ScrollX() < 1712)
	{
		Move_Stage(); // ScrollX += 16.f
	}
	__super::Update();
	return 0;
}

void CStage3::Late_Update()
{
	__super::Late_Update();
}

void CStage3::Render(HDC hDC)
{
	int iScroll_X = (int)SCROLL->Get_ScrollX();

	HDC hIceDC = BITMAP->Find_Img(L"Ice-bg");
	if (iScroll_X == 1712)
	{
		if (m_fHit)
		{
			StretchBlt(hDC,
				0,
				0,
				WINCX,
				WINCY,
				hIceDC,
				(int)SCROLL->Hit_ScrollX(m_fHit),
				0,
				428,
				240,
				SRCCOPY);
			if (m_dwTime + 150 < GetTickCount())
			{
				m_fHit = 0;
				m_dwTime = GetTickCount();
			}
		}
		else
			StretchBlt(hDC,
				0,
				0,
				WINCX,
				WINCY,
				hIceDC,
				0,
				0,
				428,
				240,
				SRCCOPY);
	}
	else
		StretchBlt(hDC,
			0,
			0,
			WINCX,
			WINCY,
			hIceDC,
			iScroll_X,
			0,
			428,
			240,
			SRCCOPY);

	__super::Render(hDC);
}

void CStage3::Release()
{
	SOUND->StopSound(SOUND_BGM);
	__super::Release();
}
