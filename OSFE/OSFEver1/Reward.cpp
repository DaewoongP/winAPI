#include "stdafx.h"
#include "Reward.h"
#include "AbstractFactory.h"
#include "KeyMgr.h"
#include "SoundMgr.h"
#include "MouseMgr.h"
#include "SceneMgr.h"
#include "BitmapMgr.h"
#include "SpawnTurretLaser.h"
#include "SpawnTurretGun.h"
#include "Sunshine.h"

CReward::CReward()
	:m_bSelectSpell(false)
{
}


CReward::~CReward()
{
	Release();
}

void CReward::Initialize()
{
	BITMAP->Insert_Bmp(L"../Image/UI/Key-Spacebar.bmp", L"Key-Spacebar");
	m_SpellReward.push_back(static_cast<CSpell*>(CAbstractFactory<CSpawnTurretLaser>::Create()));
	m_SpellReward.push_back(static_cast<CSpell*>(CAbstractFactory<CSpawnTurretGun>::Create()));
	m_SpellReward.push_back(static_cast<CSpell*>(CAbstractFactory<CSunshine>::Create()));
}

int CReward::Update()
{
	if (m_bSelectSpell)
		return 1;

	Select_Spell(m_SpellReward);

	return 0;
}

void CReward::Late_Update()
{
}

void CReward::Render(HDC hDC)
{
	int i = 0;
	for (auto& iter : m_SpellReward)
	{
		// 랜덤출력 알고리즘은 나중에
		// 아래 구매함수때문에 iter 자체를 섞어야함.
		if (i >= 3)
			break;
		else
		{
			iter->Render_SpellCard(hDC, 120 + 180 * i, 200);
			++i;
		}
	}
	HDC hMemDC = BITMAP->Find_Img(L"Key-Spacebar");
	GdiTransparentBlt(hDC,
		325,
		500,
		70,
		70,
		hMemDC,
		0,
		0,
		256,
		256,
		RGB(1,1,1));

	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("메이플스토리"));
	HFONT oldFont = (HFONT)SelectObject(hDC, hFont);
	SetTextAlign(hDC, TA_CENTER);
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC,
		360,
		480,
		L"보상 스킵", lstrlen(L"보상 스킵"));
	(HFONT)DeleteObject(oldFont);
}

void CReward::Release()
{
	for (auto iter = m_SpellReward.begin();
	iter != m_SpellReward.end();)
	{
		Safe_Delete<CSpell*>(*iter);
		iter = m_SpellReward.erase(iter);
	}
	m_SpellReward.clear();
	KEY->Destroy_Instance();
}

void CReward::Select_Spell(deque<CSpell*>& _Spell)
{
	int i = 0;

	if (KEY->Key_Down(VK_LBUTTON))
	{
		if (MOUSE->Click_Event(120.f, 200.f, 240.f, 380.f))
		{
			m_bSelectSpell = true;
		}
		else if (MOUSE->Click_Event(300.f, 200.f, 420.f, 380.f))
		{
			m_bSelectSpell = true;
			i = 1;
		}
		else if (MOUSE->Click_Event(480.f, 200.f, 600.f, 380.f))
		{
			m_bSelectSpell = true;
			i = 2;
		}
	}

	if (KEY->Key_Down(VK_SPACE))
	{
		m_bSelectSpell = true;
		return;
	}

	for (auto& iter : m_SpellReward)
	{
		// left, top 은 렌더 스펠카드의 정보값
		// right, bottom 은 렌더 스펠카드의 크기만큼 더해준 값.
		if (i == 0 && m_bSelectSpell == true)
		{
			CSpell* pSpell = nullptr;
			switch (iter->Get_SpellID())
			{
			case SPELL_ICENEEDLE:
				pSpell = new CIceNeedle;
				break;
			case SPELL_MINIGUN:
				pSpell = new CSuperMiniGun;
				break;
			case SPELL_EXPLOSION:
				pSpell = new CExplosion;
				break;
			case SPELL_FIRESTORM:
				pSpell = new CFirestorm;
				break;
			case SPELL_SHIELDCATCH:
				pSpell = new CShieldCatch;
				break;
			case SPELL_TURRETGUN:
				pSpell = new CSpawnTurretGun;
				break;
			case SPELL_TURRETLASER:
				pSpell = new CSpawnTurretLaser;
				break;
			case SPELL_SUNSHINE:
				pSpell = new CSunshine;
				break;
			default:
				break;
			}
			pSpell->Initialize();

			SOUND->PlaySound(L"chooseOption.wav", SOUND_EFFECT21, 0.3f);
			static_cast<CPlayer*>(
				SCENE->Get_Scene()->Get_Player())->Get_Deck()->Get_Inventory().push_back(pSpell);
			static_cast<CPlayer*>(
				SCENE->Get_Scene()->Get_Player())->Get_Deck()->Reload_Deck();
			break;
		}
		--i;
	}
}
