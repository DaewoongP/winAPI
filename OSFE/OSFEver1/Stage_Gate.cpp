#include "stdafx.h"
#include "Stage_Gate.h"
#include "BitmapMgr.h"
#include "AbstractFactory.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "SoundMgr.h"
#include "ScrollMgr.h"

CStage_Gate::CStage_Gate()
{
}


CStage_Gate::~CStage_Gate()
{
	Release();
}

void CStage_Gate::Initialize()
{
	BITMAP->Insert_Bmp(L"../Image/Stage/Stage_Gate/Wasteland-bg.bmp", L"Wasteland-bg");
	SOUND->PlayBGM(L"Eden's_Door.wav", 0.2f);
	Scroll_Accel = 0;
	__super::Initialize();

	Add_Object(ENEMIE, CAbstractFactory<CGate>::Create(23));
}

int CStage_Gate::Update()
{
	if (0 >= static_cast<CPlayer*>(Get_Player())->Get_Creature().iHp)
	{
		SOUND->StopSound(SOUND_BGM);
		for (auto& iter : Get_ObjList(ENEMIE))
		{
			iter->Set_Dead();
		}
		Delete_Object(PLAYER);
		
		SCENE->Scene_Change(SC_DEAD);
		return 0;
	}
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
			if (Get_UIList(UI_MAP).empty())
				AddUI(UI_MAP, CAbstractFactory<CMapUI>::Create_UI());
			else
				DeleteUI(UI_MAP);
			return 0;
		}
	}

	// 고정형 보스라 이동하기 좀 그럴듯?
	//if (SCROLL->Get_ScrollX() < 1712)
	//{
	//	Move_Stage(); // ScrollX += 16.f
	//}

	__super::Update();
	return 0;
}

void CStage_Gate::Late_Update()
{
	__super::Late_Update();
}

void CStage_Gate::Render(HDC hDC)
{
	int iScroll_X = (int)SCROLL->Get_ScrollX();

	HDC hIceDC = BITMAP->Find_Img(L"Wasteland-bg");
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

void CStage_Gate::Release()
{
	__super::Release();
}
