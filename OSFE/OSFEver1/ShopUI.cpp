#include "stdafx.h"
#include "ShopUI.h"
#include "BitmapMgr.h"

CShopUI::CShopUI()
	:m_pShop(nullptr)
{
}


CShopUI::~CShopUI()
{
	Release();
}

void CShopUI::Initialize()
{
	m_eID = UI_SHOP;
	BITMAP->Insert_Bmp(L"../Image/UI/ShopDeckMock.bmp", L"ShopDeckMock");
	BITMAP->Insert_Bmp(L"../Image/UI/Donate_blood.bmp", L"Donate_blood");
	BITMAP->Insert_Bmp(L"../Image/UI/Remove_spell.bmp", L"Remove_spell");
	BITMAP->Insert_Bmp(L"../Image/Pet/PetFox.bmp", L"PetFox");
	BITMAP->Insert_Bmp(L"../Image/Pet/PetDog.bmp", L"PetDog");
	BITMAP->Insert_Bmp(L"../Image/Enemie/Shopkeeper_Welcome.bmp", L"Shopkeeper_Welcome");
	__super::Update_Rect(); // 한번만 업데이트

	if (!m_pShop)
	{
		m_pShop = new CShop;
		m_pShop->Initialize();
	}
}

void CShopUI::Update()
{
	m_pShop->Update();
}

void CShopUI::Late_Update()
{
	m_pShop->Late_Update();
}

void CShopUI::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(L"ShopDeckMock");
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

	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("메이플스토리"));
	HFONT oldFont = (HFONT)SelectObject(hDC, hFont);
	SetTextAlign(hDC, TA_CENTER);
	SetTextColor(hDC, RGB(108, 189, 125));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC,
		400,
		30,
		L"상점", lstrlen(L"상점"));
	(HFONT)DeleteObject(oldFont);

	HDC		hBloodDC = BITMAP->Find_Img(L"Donate_blood");
	GdiTransparentBlt(hDC,
		450,
		420,
		50, // 키울 사이즈
		50,
		hBloodDC,
		0,
		0,
		256,		// 텍스처 원본 사이즈
		256,
		RGB(1, 1, 1));

	HDC		hFoxDC = BITMAP->Find_Img(L"PetFox");
	GdiTransparentBlt(hDC,
		40,
		380,
		150, // 키울 사이즈
		150,
		hFoxDC,
		0,
		0,
		256,		// 텍스처 원본 사이즈
		256,
		RGB(1, 1, 1));

	HDC		hDogDC = BITMAP->Find_Img(L"PetDog");
	GdiTransparentBlt(hDC,
		220,
		380,
		150, // 키울 사이즈
		150,
		hDogDC,
		0,
		0,
		256,		// 텍스처 원본 사이즈
		256,
		RGB(1, 1, 1));

	SpellCard_BackGround(hDC);

	m_pShop->Render(hDC);
}

void CShopUI::Release()
{
	Safe_Delete<CShop*>(m_pShop);
}

void CShopUI::SpellCard_BackGround(HDC hDC)
{
	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("메이플스토리"));
	HFONT oldFont = (HFONT)SelectObject(hDC, hFont);
	SetTextAlign(hDC, TA_CENTER);
	SetTextColor(hDC, RGB(42, 74, 60));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC,
		300,
		80,
		L"스펠", lstrlen(L"스펠"));
	(HFONT)DeleteObject(oldFont);

}
