#include "stdafx.h"
#include "MyMenu.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "MouseMgr.h"
#include "SoundMgr.h"

CMyMenu::CMyMenu()
{
}


CMyMenu::~CMyMenu()
{
	Release();
}

void CMyMenu::Initialize()
{
	BITMAP->Insert_Bmp(L"../Image/Menu.bmp", L"Menu");
	SOUND->PlayBGM(L"Maintheme.wav", 0.2f);
	m_eMenu = GAME_START;
}

int CMyMenu::Update()
{
	if (m_eMenu == GAME_START)
	{
		if (KEY->Key_Down(VK_DOWN) )
			m_eMenu = GAME_END;
	}
	if (m_eMenu == GAME_END)
	{
		if (KEY->Key_Down(VK_UP))
			m_eMenu = GAME_START;
	}

	if (KEY->Key_Down('Q'))
	{
		switch (m_eMenu)
		{
		case GAME_START:
			SCENE->Scene_Change(SC_STAGE1);
			break;
		case GAME_END:
			DestroyWindow(g_hWnd);
			break;
		default:
			break;
		}
		return 0;
	}
	
	return 0;
}

void CMyMenu::Late_Update()
{
}

void CMyMenu::Render(HDC hDC)
{
	HDC	hMemDC = BITMAP->Find_Img(L"Menu");
	StretchBlt(hDC,
		0,
		0,
		WINCX, // 키울 사이즈
		WINCY,
		hMemDC,
		0,
		0,
		WINCX,		// 텍스처 원본 사이즈
		WINCY,
		SRCCOPY);
	Game_Start(hDC);
	Game_End(hDC);
}

void CMyMenu::Release()
{
	SOUND->StopSound(SOUND_BGM);
	KEY->Destroy_Instance();
}

void CMyMenu::Game_Start(HDC hDC)
{
	TCHAR buf[32] = {L"게임 시작"};	
	HFONT hFont = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("메이플스토리"));
	HFONT oldFont = (HFONT)SelectObject(hDC, hFont); // DC관련
	SetTextAlign(hDC, TA_CENTER); // 텍스트 중앙정렬
	if (m_eMenu == GAME_START)
		SetTextColor(hDC, RGB(254, 50, 102)); // 텍스트 컬러 설정
	else
		SetTextColor(hDC, RGB(255, 255, 255)); // 텍스트 컬러 설정
	SetBkMode(hDC, TRANSPARENT); // 매개변수 DC의 글자배경을 투명하게 한다. 
	TextOut(hDC, 400, 380, buf, lstrlen(buf));
	(HFONT)DeleteObject(oldFont); // 원래 있던폰트 제거
}

void CMyMenu::Game_End(HDC hDC)
{
	TCHAR buf[32] = { L"게임 종료" };
	HFONT hFont = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("메이플스토리"));
	HFONT oldFont = (HFONT)SelectObject(hDC, hFont); // DC관련
	SetTextAlign(hDC, TA_CENTER); // 텍스트 중앙정렬
	if (m_eMenu == GAME_END)
		SetTextColor(hDC, RGB(254, 50, 102)); // 텍스트 컬러 설정
	else
		SetTextColor(hDC, RGB(255, 255, 255)); // 텍스트 컬러 설정
	SetBkMode(hDC, TRANSPARENT); // 매개변수 DC의 글자배경을 투명하게 한다. 
	TextOut(hDC, 400, 450, buf, lstrlen(buf));
	(HFONT)DeleteObject(oldFont); // 원래 있던폰트 제거
}
