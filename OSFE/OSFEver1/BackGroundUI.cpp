#include "stdafx.h"
#include "BackGroundUI.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"

#define MANALEFT 280
CBackGroundUI::CBackGroundUI()
{
}


CBackGroundUI::~CBackGroundUI()
{
	Release();
}

void CBackGroundUI::Initialize()
{
	m_eID = UI_BACKGROUND;
	BITMAP->Insert_Bmp(L"../Image/UI/SlotFrame.bmp", L"SlotFrame");
	BITMAP->Insert_Bmp(L"../Image/UI/Slot-Open.bmp", L"Slot-Open");
	BITMAP->Insert_Bmp(L"../Image/UI/ManaBar.bmp", L"ManaBar");
	BITMAP->Insert_Bmp(L"../Image/UI/ManaCrystal.bmp", L"ManaCrystal");
	
	m_tInfo = { 0, 0, 800, 50 };
	// 마나 전체길이 = 300
	m_tMana = { 0 , 554, 0, 50 };
	// 왼쪽 고정값
	__super::Update_Rect(); // 백그라운드는 한번만 업데이트
}

void CBackGroundUI::Update()
{
}

void CBackGroundUI::Late_Update()
{
}

void CBackGroundUI::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(L"TopNavBG");
	GdiTransparentBlt(hDC,
		0,
		0,
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		0,
		0,
		1920,		// 텍스처 원본 사이즈
		156,
		RGB(1, 1, 1));

	Deck_UI(hDC);
	Slot_UI(hDC);
	MP_UI(hDC);
}

void CBackGroundUI::Release()
{
}


void CBackGroundUI::Deck_UI(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(L"DeckCover");
	GdiTransparentBlt(hDC,
		-10,
		470,
		120, // 키울 사이즈
		120,
		hMemDC,
		0,
		0,
		50,		// 텍스처 원본 사이즈
		50,
		RGB(1, 1, 1));
}

void CBackGroundUI::Slot_UI(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(L"SlotFrame");

	//Q
	GdiTransparentBlt(hDC,
		90,
		510,
		90, // 키울 사이즈
		90,
		hMemDC,
		0,
		0,
		50,		// 텍스처 원본 사이즈
		50,
		RGB(1, 1, 1));

	//W
	GdiTransparentBlt(hDC,
		160,
		510,
		90, // 키울 사이즈
		90,
		hMemDC,
		0,
		0,
		50,		// 텍스처 원본 사이즈
		50,
		RGB(1, 1, 1));
}

void CBackGroundUI::MP_UI(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(L"ManaBar");
	GdiTransparentBlt(hDC,
		275,
		535,
		300, // 키울 사이즈
		30,
		hMemDC,
		0,
		28,
		810,		// 텍스처 원본 사이즈
		57,
		RGB(1, 1, 1));

	CREATURE Player_Creature = static_cast<CCreature*>(SCENE->Get_Scene()->Get_Player())->Get_Creature();
	GdiTransparentBlt(hDC,
		275, // 출력 left 좌표
		535, // 출력 top 좌표
		int(300 * Player_Creature.fMp / Player_Creature.iMaxMp), // 출력 사이즈 x -> 얘를 
		30, //  출력 사이즈 y
		hMemDC, // DC
		0,
		189,
		810,
		48,
		RGB(1, 1, 1));

	hMemDC = BITMAP->Find_Img(L"ManaCrystal");
	GdiTransparentBlt(hDC,
		250,
		515,
		75, // 키울 사이즈
		75,
		hMemDC,
		0,
		0,
		260,		// 텍스처 원본 사이즈
		260,
		RGB(1, 1, 1));

	TCHAR szMp[32] = {};
	TCHAR szMaxMp[32] = {};
	_itow_s(int(floor(Player_Creature.fMp)), szMp, 10);
	_itow_s(Player_Creature.iMaxMp, szMaxMp, 10);
	_tcscat_s(szMp, L"/");
	_tcscat_s(szMp, szMaxMp);
	HFONT hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("quadaptor"));
	HFONT oldFont = (HFONT)SelectObject(hDC, hFont);
	SetTextAlign(hDC, TA_CENTER);
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC,
		287,
		542,
		szMp, lstrlen(szMp));
	(HFONT)DeleteObject(oldFont);


}
