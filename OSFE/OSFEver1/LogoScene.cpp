#include "stdafx.h"
#include "LogoScene.h"
#include "BitmapMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

CLogoScene::CLogoScene()
{
}


CLogoScene::~CLogoScene()
{
	Release();
}

void CLogoScene::Initialize()
{
	BITMAP->Insert_Bmp(L"../Image/Logo.bmp", L"Logo");
}

int CLogoScene::Update()
{
	if (KEY->Key_Down(VK_RETURN))
	{
		SCENE->Scene_Change(SC_MENU);
		return 0;
	}

	return 0;
}

void CLogoScene::Late_Update()
{
}

void CLogoScene::Render(HDC hDC)
{
	HDC hLogoDC = BITMAP->Find_Img(L"Logo");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hLogoDC, 0, 0, SRCCOPY);
}

void CLogoScene::Release()
{
	KEY->Destroy_Instance();
}
