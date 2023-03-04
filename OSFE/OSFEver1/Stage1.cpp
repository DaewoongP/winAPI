#include "stdafx.h"
#include "Stage1.h"
#include "BitmapMgr.h"
#include "TileMgr.h"
#include "AbstractFactory.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "ScrollMgr.h"

CStage1::CStage1()
	:m_bRewardSelected(false)
{
}


CStage1::~CStage1()
{
	Release();
}

void CStage1::Initialize()
{
	BITMAP->Insert_Bmp(L"../Image/Stage/Stage1/Ice-bg.bmp", L"Ice-bg");
	SOUND->PlayBGM(L"Battle_of_Ice_I.wav", 0.2f);
	m_bSound = true;
	Scroll_Accel = 0;
	__super::Initialize();
	Add_Object(PLAYER, CAbstractFactory<CPlayer>::Create(0));
	Add_Object(ENEMIE, CAbstractFactory<CHailcaster>::Create(30));
	Add_Object(ENEMIE, CAbstractFactory<CLiger>::Create(22));
}

int CStage1::Update()
{ 
	if (!Get_UIList(UI_MAP).empty())
	{
		if (KEY->Key_Up('Q'))
		{
			SCENE->Scene_Change(SC_STAGE2);
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

void CStage1::Late_Update()
{
	__super::Late_Update();
}

void CStage1::Render(HDC hDC)
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
void CStage1::Release()
{
	SOUND->StopSound(SOUND_BGM);
	__super::Release();
}
