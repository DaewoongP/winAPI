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
		WINCX, // Ű�� ������
		WINCY,
		hMemDC,
		0,
		0,
		WINCX,		// �ؽ�ó ���� ������
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
	TCHAR buf[32] = {L"���� ����"};	
	HFONT hFont = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("�����ý��丮"));
	HFONT oldFont = (HFONT)SelectObject(hDC, hFont); // DC����
	SetTextAlign(hDC, TA_CENTER); // �ؽ�Ʈ �߾�����
	if (m_eMenu == GAME_START)
		SetTextColor(hDC, RGB(254, 50, 102)); // �ؽ�Ʈ �÷� ����
	else
		SetTextColor(hDC, RGB(255, 255, 255)); // �ؽ�Ʈ �÷� ����
	SetBkMode(hDC, TRANSPARENT); // �Ű����� DC�� ���ڹ���� �����ϰ� �Ѵ�. 
	TextOut(hDC, 400, 380, buf, lstrlen(buf));
	(HFONT)DeleteObject(oldFont); // ���� �ִ���Ʈ ����
}

void CMyMenu::Game_End(HDC hDC)
{
	TCHAR buf[32] = { L"���� ����" };
	HFONT hFont = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("�����ý��丮"));
	HFONT oldFont = (HFONT)SelectObject(hDC, hFont); // DC����
	SetTextAlign(hDC, TA_CENTER); // �ؽ�Ʈ �߾�����
	if (m_eMenu == GAME_END)
		SetTextColor(hDC, RGB(254, 50, 102)); // �ؽ�Ʈ �÷� ����
	else
		SetTextColor(hDC, RGB(255, 255, 255)); // �ؽ�Ʈ �÷� ����
	SetBkMode(hDC, TRANSPARENT); // �Ű����� DC�� ���ڹ���� �����ϰ� �Ѵ�. 
	TextOut(hDC, 400, 450, buf, lstrlen(buf));
	(HFONT)DeleteObject(oldFont); // ���� �ִ���Ʈ ����
}
