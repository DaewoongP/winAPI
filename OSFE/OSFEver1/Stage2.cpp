#include "stdafx.h"
#include "Stage2.h"
#include "AbstractFactory.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "ScrollMgr.h"

CStage2::CStage2()
	:m_bRewardSelected(false)
{
}


CStage2::~CStage2()
{
	Release();
}

void CStage2::Initialize()
{
	BITMAP->Insert_Bmp(L"../Image/Stage/Stage1/Ice-bg.bmp", L"Ice-bg");
	SOUND->PlayBGM(L"Battle_of_Ice_II.wav", 0.2f);
	__super::Initialize();
	Scroll_Accel = 0;
	m_bSound = true;
	Add_Object(ENEMIE, CAbstractFactory<CReaper>::Create(31));
	Add_Object(ENEMIE, CAbstractFactory<CHailcaster>::Create(12));

	AddUI(UI_BACKGROUND, CAbstractFactory<CBackGroundUI>::Create_UI());
}

int CStage2::Update()
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
			SCENE->Scene_Change(SC_STAGE3);
			return 0;
		}
	}

	if (m_ObjList[ENEMIE].empty())
	{	
		if (m_bSound)
		{
			SOUND->StopSound(SOUND_BGM);
			SOUND->PlayBGM(L"Victory.wav", 0.2f);
			m_bSound = false;
		}
		if (!m_bRewardSelected)
		{
			if (Get_UIList(UI_REWARD).empty())
				AddUI(UI_REWARD, CAbstractFactory<CRewardUI>::Create_UI());
			else
				m_bRewardSelected = static_cast<CRewardUI*>(Get_UIList(UI_REWARD).back())->Get_Selected();
		}
		else
			DeleteUI(UI_REWARD);

		if (Get_UIList(UI_NEXT).empty())
			AddUI(UI_NEXT, CAbstractFactory<CNextUI>::Create_UI());

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

void CStage2::Late_Update()
{
	__super::Late_Update();
}

void CStage2::Render(HDC hDC)
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
				(int)SCROLL->Hit_ScrollX(m_fHit * 0.5f),
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

void CStage2::Release()
{
	SOUND->StopSound(SOUND_BGM);
	__super::Release();
}
