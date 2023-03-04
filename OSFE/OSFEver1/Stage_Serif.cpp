#include "stdafx.h"
#include "Stage_Serif.h"
#include "BitmapMgr.h"
#include "AbstractFactory.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "SoundMgr.h"
#include "ScrollMgr.h"

CStage_Serif::CStage_Serif()
{
}


CStage_Serif::~CStage_Serif()
{
	Release();
}

void CStage_Serif::Initialize()
{
	BITMAP->Insert_Bmp(L"../Image/Stage/Stage_Serif/Serif-bg.bmp", L"Serif-bg");
	SOUND->PlayBGM(L"Battle_of_Eden.wav", 0.2f);
	Scroll_Accel = 0;
	__super::Initialize();

	Add_Object(ENEMIE, CAbstractFactory<CSerif>::Create(22));
}

int CStage_Serif::Update()
{
	if (!m_bReload)
	{
		static_cast<CPlayer*>(Get_Player())->Get_Deck()->Reload_Deck();
		m_bReload = true;
	}

	if (m_ObjList[ENEMIE].empty())
	{
		if (m_bSound)
		{
			SOUND->StopSound(SOUND_BGM);
			// 보스 처치시 브금 x
			//SOUND->PlayBGM(L"Victory.wav", 0.2f);
			m_bSound = false;
		}

		if (Get_UIList(UI_NEXT).empty())
			AddUI(UI_NEXT, CAbstractFactory<CNextUI>::Create_UI());

		if (KEY->Key_Down(VK_TAB))
		{
			SOUND->StopSound(SOUND_BGM);
			for (auto& iter : Get_ObjList(ENEMIE))
			{
				iter->Set_Dead();
			}
			Delete_Object(PLAYER);

			SCENE->Scene_Change(SC_WIN);
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

void CStage_Serif::Late_Update()
{
	__super::Late_Update();
}

void CStage_Serif::Render(HDC hDC)
{
	int iScroll_X = (int)SCROLL->Get_ScrollX();

	HDC hIceDC = BITMAP->Find_Img(L"Serif-bg");
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

void CStage_Serif::Release()
{
	__super::Release();
}
