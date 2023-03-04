#include "stdafx.h"
#include "InventoryUI.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "MouseMgr.h"

CInventoryUI::CInventoryUI()
{
}


CInventoryUI::~CInventoryUI()
{
	Release();
}

void CInventoryUI::Initialize()
{
	m_eID = UI_INVENTORY;
	BITMAP->Insert_Bmp(L"../Image/UI/DeckBackgroundN.bmp", L"DeckBackgroundN");
	BITMAP->Insert_Bmp(L"../Image/UI/CursorSpell.bmp", L"SpellBackGround");
	BITMAP->Insert_Bmp(L"../Image/UI/Remove_spell.bmp", L"Remove_spell");
	// 인벤토리안의 스펠 위치 인포값으로 사용
	m_tInfo = { 600, 100, 30, 30 };

	m_pSpellInventory = dynamic_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Get_Deck()->Get_Inventory();

	__super::Update_Rect(); // 한번만 업데이트
}

void CInventoryUI::Update()
{
}

void CInventoryUI::Late_Update()
{
}

void CInventoryUI::Render(HDC hDC)
{
	
	HDC		hMemDC = BITMAP->Find_Img(L"DeckBackgroundN");
	GdiTransparentBlt(hDC,
		0,
		0,
		800, // 키울 사이즈
		600,
		hMemDC,
		0,
		0,
		1972,		// 텍스처 원본 사이즈
		1200,
		RGB(255, 255, 255));

	Spell_BackGround(hDC);
}

void CInventoryUI::Release()
{
}

void CInventoryUI::Spell_BackGround(HDC hDC)
{
	int i = 0;
	for (auto& iter : m_pSpellInventory)
	{
		// 스킬 백그라운드
		HDC		hSpellBackDC = BITMAP->Find_Img(L"SpellBackGround");
		GdiTransparentBlt(hDC,
			int(m_tInfo.fX) - 17,
			int(m_tInfo.fY + i * 50) - 20,
			200, // 키울 사이즈
			70,
			hSpellBackDC,
			0,
			0,
			200,		// 텍스처 원본 사이즈
			48,
			RGB(1, 1, 1));
		// 스킬 아이콘
		HDC		hIconDC = BITMAP->Find_Img(iter->Get_SpellName());
		GdiTransparentBlt(hDC,
			int(m_tInfo.fX),
			int(m_tInfo.fY + i * 50),
			int(m_tInfo.fCX), // 키울 사이즈
			int(m_tInfo.fCY),
			hIconDC,
			0,
			0,
			256,		// 텍스처 원본 사이즈
			256,
			RGB(1, 1, 1));
		// 스킬 이름
		HFONT hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
			VARIABLE_PITCH | FF_ROMAN, TEXT("메이플스토리"));
		HFONT oldFont = (HFONT)SelectObject(hDC, hFont);
		SetTextAlign(hDC, TA_LEFT);
		SetTextColor(hDC, RGB(255, 255, 255));
		SetBkMode(hDC, TRANSPARENT);
		TextOut(hDC,
			int(m_tInfo.fX) + 40,
			int(m_tInfo.fY + i * 50) + 13,
			iter->Get_SpellName(), lstrlen(iter->Get_SpellName()));
		(HFONT)DeleteObject(oldFont);

		++i;
	}
	i = 0;
	for (auto& iter : m_pSpellInventory)
	{
		// 스펠 백그라운드 위치
		Mouse_OverEvent(hDC, iter, m_tInfo.fX - 17, m_tInfo.fY + i * 50 - 20);
		++i;
	}

	//HDC		hRemoveDC = BITMAP->Find_Img(L"Remove_spell");
	//GdiTransparentBlt(hDC,
	//	650,
	//	530,
	//	36, // 키울 사이즈
	//	36,
	//	hRemoveDC,
	//	0,
	//	0,
	//	256,		// 텍스처 원본 사이즈
	//	256,
	//	RGB(1, 1, 1));
}

void CInventoryUI::Mouse_OverEvent(HDC hDC, CSpell* _Spell, float _left, float _top)
{
	if (MOUSE->Click_Event(_left, _top + 15, _left + 200, _top + 55))
	{
		_Spell->Render_SpellCard(hDC, int(_left), int(_top));
	}
}
