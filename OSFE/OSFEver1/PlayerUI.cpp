#include "stdafx.h"
#include "PlayerUI.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"

CPlayerUI::CPlayerUI()
{
}


CPlayerUI::~CPlayerUI()
{
	Release();
}

void CPlayerUI::Initialize()
{
	m_eID = UI_PLAYER;
	BITMAP->Insert_Bmp(L"../Image/UI/TopNavBG.bmp", L"TopNavBG");
	BITMAP->Insert_Bmp(L"../Image/UI/DeckCover.bmp", L"DeckCover");
	BITMAP->Insert_Bmp(L"../Image/UI/health-icon.bmp", L"health-icon");
	BITMAP->Insert_Bmp(L"../Image/UI/money-icon.bmp", L"money-icon");
	BITMAP->Insert_Bmp(L"../Image/UI/Button_I.bmp", L"Button_I");

	m_tInfo = { 0, 0, 800, 50 };

	__super::Update_Rect(); // �ѹ��� ������Ʈ
}

void CPlayerUI::Update()
{
}

void CPlayerUI::Late_Update()
{
}

void CPlayerUI::Render(HDC hDC)
{

	HP_UI(hDC);
	Money_UI(hDC);
	Level_UI(hDC);
	Inven_UI(hDC);
}

void CPlayerUI::Release()
{
}

void CPlayerUI::HP_UI(HDC hDC)
{
	TCHAR szHp[32] = {};
	TCHAR szMaxHp[32] = {};
	CREATURE Player_Creature = static_cast<CCreature*>(SCENE->Get_Scene()->Get_Player())->Get_Creature();
	_itow_s(Player_Creature.iHp, szHp, 10);
	_itow_s(Player_Creature.iMaxHp, szMaxHp, 10);
	_tcscat_s(szHp, L"/");
	_tcscat_s(szHp, szMaxHp);
	HFONT hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("quadaptor"));
	HFONT oldFont = (HFONT)SelectObject(hDC, hFont);
	SetTextAlign(hDC, TA_CENTER);
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC,
		165,
		30,
		szHp, lstrlen(szHp));
	(HFONT)DeleteObject(oldFont);

	HDC		hMemDC = BITMAP->Find_Img(L"health-icon");
	GdiTransparentBlt(hDC,
		95,
		30,
		20,
		20,
		hMemDC,
		0,
		0,
		512,		// �ؽ�ó ���� ������
		512,
		RGB(1, 1, 1));
}

void CPlayerUI::Money_UI(HDC hDC)
{
	TCHAR szMoney[32] = {};
	CREATURE Player_Creature = static_cast<CCreature*>(SCENE->Get_Scene()->Get_Player())->Get_Creature();
	_itow_s(Player_Creature.iMoney, szMoney, 10);
	HFONT hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("quadaptor"));
	HFONT oldFont = (HFONT)SelectObject(hDC, hFont);
	SetTextAlign(hDC, TA_LEFT);
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC,
		125,
		7,
		szMoney, lstrlen(szMoney));
	(HFONT)DeleteObject(oldFont);

	HDC		hMemDC = BITMAP->Find_Img(L"money-icon");
	GdiTransparentBlt(hDC,
		95,
		7,
		20,
		20,
		hMemDC,
		0,
		0,
		512,		// �ؽ�ó ���� ������
		512,
		RGB(1, 1, 1));
}

void CPlayerUI::Level_UI(HDC hDC)
{
	TCHAR szLevel[32] = {};
	CREATURE Player_Creature = static_cast<CCreature*>(SCENE->Get_Scene()->Get_Player())->Get_Creature();
	_itow_s(Player_Creature.iLevel, szLevel, 10);
	HFONT hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("quadaptor"));
	HFONT oldFont = (HFONT)SelectObject(hDC, hFont);
	SetTextAlign(hDC, TA_LEFT);
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC,
		50,
		30,
		szLevel, lstrlen(szLevel));
	(HFONT)DeleteObject(oldFont);

	hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("�����ý��丮"));
	oldFont = (HFONT)SelectObject(hDC, hFont);
	SetTextAlign(hDC, TA_LEFT);
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC,
		10,
		30,
		L"����", lstrlen(L"����"));
	(HFONT)DeleteObject(oldFont);

	hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("�����ý��丮"));
	oldFont = (HFONT)SelectObject(hDC, hFont);
	SetTextAlign(hDC, TA_LEFT);
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC,
		10,
		7,
		L"���", lstrlen(L"���"));
	(HFONT)DeleteObject(oldFont);
}

void CPlayerUI::Inven_UI(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(L"Button_I");
	GdiTransparentBlt(hDC,
		590,
		2,
		48, // Ű�� ������
		48,
		hMemDC,
		0,
		0,
		48,		// �ؽ�ó ���� ������
		48,
		RGB(1, 1, 1));

	HFONT hFont = CreateFont(25, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("�����ý��丮"));
	HFONT oldFont = (HFONT)SelectObject(hDC, hFont);
	SetTextAlign(hDC, TA_CENTER);
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC,
		680,
		12,
		L"�κ��丮", lstrlen(L"�κ��丮"));
	(HFONT)DeleteObject(oldFont);
}
