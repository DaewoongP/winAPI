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
	// ���� ��ü���� = 300
	m_tMana = { 0 , 554, 0, 50 };
	// ���� ������
	__super::Update_Rect(); // ��׶���� �ѹ��� ������Ʈ
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
		int(m_tInfo.fCX), // Ű�� ������
		int(m_tInfo.fCY),
		hMemDC,
		0,
		0,
		1920,		// �ؽ�ó ���� ������
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
		120, // Ű�� ������
		120,
		hMemDC,
		0,
		0,
		50,		// �ؽ�ó ���� ������
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
		90, // Ű�� ������
		90,
		hMemDC,
		0,
		0,
		50,		// �ؽ�ó ���� ������
		50,
		RGB(1, 1, 1));

	//W
	GdiTransparentBlt(hDC,
		160,
		510,
		90, // Ű�� ������
		90,
		hMemDC,
		0,
		0,
		50,		// �ؽ�ó ���� ������
		50,
		RGB(1, 1, 1));
}

void CBackGroundUI::MP_UI(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(L"ManaBar");
	GdiTransparentBlt(hDC,
		275,
		535,
		300, // Ű�� ������
		30,
		hMemDC,
		0,
		28,
		810,		// �ؽ�ó ���� ������
		57,
		RGB(1, 1, 1));

	CREATURE Player_Creature = static_cast<CCreature*>(SCENE->Get_Scene()->Get_Player())->Get_Creature();
	GdiTransparentBlt(hDC,
		275, // ��� left ��ǥ
		535, // ��� top ��ǥ
		int(300 * Player_Creature.fMp / Player_Creature.iMaxMp), // ��� ������ x -> �긦 
		30, //  ��� ������ y
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
		75, // Ű�� ������
		75,
		hMemDC,
		0,
		0,
		260,		// �ؽ�ó ���� ������
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
