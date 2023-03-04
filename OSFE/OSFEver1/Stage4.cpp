#include "stdafx.h"
#include "Stage4.h"
#include "BitmapMgr.h"
#include "AbstractFactory.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "SoundMgr.h"
#include "ScrollMgr.h"

CStage4::CStage4()
	:m_bRewardSelected(false), m_bSaveEvent(false)
{
}


CStage4::~CStage4()
{
	Release();
}

void CStage4::Initialize()
{
	BITMAP->Insert_Bmp(L"../Image/Stage/Stage1/Ice-bg.bmp", L"Ice-bg");
	SOUND->PlayBGM(L"Violette.wav", 0.3f);
	Scroll_Accel = 0;
	__super::Initialize();

	Add_Object(ENEMIE, CAbstractFactory<CViolette>::Create(22));
}

int CStage4::Update()
{
	if (m_bSaveEvent)
	{
		DeleteUI(UI_MAP);
		// 세이브 이벤트 쓰던 bool값에 대입해서 멤버변수 최소화 (이벤트 함수를 bool값 반환)
		m_bSaveEvent = static_cast<CViolette*>(Get_ObjList(ENEMIE).front())->Save_Event();

		if (!m_bSaveEvent)
		{
			Get_ObjList(ENEMIE).front()->Set_Dead();
			SCENE->Scene_Change(SC_SERIF);
		}
		return 0;
	}

	if (!m_bReload)
	{
		static_cast<CPlayer*>(Get_Player())->Get_Deck()->Reload_Deck();
		m_bReload = true;
	}

	if (!Get_UIList(UI_MAP).empty())
	{
		if (KEY->Key_Up('Q'))
		{
			if (static_cast<CPlayer*>(Get_Player())->Get_SavedVi())
			{
				m_bSaveEvent = true;
			}
			else
			{
				// 그냥 죽이고 이동
				SCENE->Scene_Change(SC_GATE);
			}
			return 0;
		}
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

		// 보스 죽였을시 보상선택 / 살리면 보상선택 x 체력회복
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

void CStage4::Late_Update()
{
	__super::Late_Update();
}

void CStage4::Render(HDC hDC)
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

void CStage4::Release()
{
	__super::Release();
}
