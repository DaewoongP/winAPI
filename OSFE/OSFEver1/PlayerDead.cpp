#include "stdafx.h"
#include "PlayerDead.h"
#include "BitmapMgr.h"
#include "AbstractFactory.h"
#include "EffectMgr.h"
#include "SoundMgr.h"
#include "ExplodeEffect.h"
#include "SceneMgr.h"
CPlayerDead::CPlayerDead()
{
}


CPlayerDead::~CPlayerDead()
{
	Release();
}

void CPlayerDead::Initialize()
{
	BITMAP->Insert_Bmp(L"../Image/Stage/Player_Dead/DeadScene.bmp", L"DeadScene");
	BITMAP->Insert_Bmp(L"../Image/Player/SaffronWitch.bmp", L"PLAYER");
	SOUND->PlayBGM(L"Defeat.wav", 0.2f);
	__super::Initialize();
	m_dwTime = GetTickCount();
	DeadCnt = 0;
}

int CPlayerDead::Update()
{
	if (m_dwTime + 500 < GetTickCount())
	{
		if (DeadCnt > 10)
		{
			SCENE->Scene_Change(SC_MENU);
			return 0;
		}
			
		EFFECT->Add_Effect(EFFECT_SPELL, CAbstractFactory<CExplodeEffect>::Create_Effect(395, 330));
		SOUND->PlaySound(L"explode_fire.wav", SOUND_EFFECT06, 0.3f);
		++DeadCnt;
		m_dwTime = GetTickCount();
	}
	__super::Update();
	return 0;
}

void CPlayerDead::Late_Update()
{
	EFFECT->Late_Update();
}

void CPlayerDead::Render(HDC hDC)
{
	HDC hDeadDC = BITMAP->Find_Img(L"DeadScene");
	StretchBlt(hDC,
		0,
		0,
		WINCX,
		WINCY,
		hDeadDC,
		0,
		0,
		800,
		600,
		SRCCOPY);

	Player_Dead(hDC);
	EFFECT->Render(hDC);
}

void CPlayerDead::Release()
{
	SOUND->StopAll();
	__super::Release();
}

void CPlayerDead::Player_Dead(HDC hDC)
{
	HDC hDeadDC = BITMAP->Find_Img(L"PLAYER");
	StretchBlt(hDC,
		300,
		150,
		224,
		208,
		hDeadDC,
		0,
		600,
		140,
		120,
		SRCCOPY);
}
