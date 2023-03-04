#include "stdafx.h"
#include "Shop.h"
#include "AbstractFactory.h"
#include "BitmapMgr.h"
#include "MouseMgr.h"
#include "SceneMgr.h"
#include "ShopUI.h"
#include "InventoryUI.h"
#include "KeyMgr.h"
#include "SoundMgr.h"

CShop::CShop()
{
}


CShop::~CShop()
{
	Release();
}

void CShop::Initialize()
{
	m_SpellDeque.push_back(static_cast<CSpell*>(CAbstractFactory<CSuperMiniGun>::Create()));
	m_SpellDeque.push_back(static_cast<CSpell*>(CAbstractFactory<CFirestorm>::Create()));
	m_SpellDeque.push_back(static_cast<CSpell*>(CAbstractFactory<CExplosion>::Create()));
}

void CShop::Update()
{
	Buy_Spell(m_SpellDeque);
}

void CShop::Late_Update()
{
}

void CShop::Render(HDC hDC)
{
	int i = 0;
	for (auto& iter : m_SpellDeque)
	{
		// 랜덤출력 알고리즘은 나중에
		// 아래 구매함수때문에 iter 자체를 섞어야함.
		if (i >= 3)
			break;
		else
		{
			iter->Render_SpellCard(hDC, 60 + 180 * i, 120);
			++i;
		}
	}
}

void CShop::Release()
{
	for (auto iter = m_SpellDeque.begin();
	iter != m_SpellDeque.end();)
	{
		Safe_Delete<CSpell*>(*iter);
		iter = m_SpellDeque.erase(iter);
	}
	m_SpellDeque.clear();
}

void CShop::Buy_Spell(deque<CSpell*>& _Spell)
{
	int i = 0;
	bool bChk = false;
	bool donate_blood = false;
	bool buy_fox = false;
	bool buy_dog = false;

	if (KEY->Key_Down(VK_LBUTTON))
	{
		if (MOUSE->Click_Event(60.f, 120.f, 180.f, 300.f))
		{
			bChk = true;
		}
		else if (MOUSE->Click_Event(240.f, 120.f, 360.f, 300.f))
		{
			bChk = true;
			i = 1;
		}
		else if (MOUSE->Click_Event(420.f, 120.f, 540.f, 300.f))
		{
			bChk = true;
			i = 2;
		}
		else if (MOUSE->Click_Event(450.f, 420.f, 500.f, 470.f))
		{
			donate_blood = true;
		}
		else if (MOUSE->Click_Event(40.f, 380.f, 190.f, 530.f))
		{
			buy_fox = true;
		}

		else if (MOUSE->Click_Event(220.f, 380.f, 370.f, 530.f))
		{
			buy_dog = true;
		}
	}

	if (donate_blood)
	{
		static_cast<CCreature*>(SCENE->Get_Scene()->Get_Player())->Set_Damage(50 + rand() % 50);
		static_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Set_Money(100 + rand() % 100);
		SOUND->PlaySound(L"enemy_hit_light.wav", SOUND_EFFECT12, 0.3f);
		donate_blood = false;
	}

	if (buy_fox)
	{
		if (static_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Get_Creature().iMoney
			< 200)
		{
			SOUND->PlaySound(L"locked_denied.wav", SOUND_EFFECT18, 0.3f);
			return;
		}
		static_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Set_Money(-200);
		SOUND->PlaySound(L"chaching.wav", SOUND_EFFECT11, 0.3f);
		SCENE->Get_Scene()->Add_Object(PET, CAbstractFactory<CFoxPet>::Create());
		buy_fox = false;
	}

	if (buy_dog)
	{
		if (static_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Get_Creature().iMoney
			< 200)
		{
			SOUND->PlaySound(L"locked_denied.wav", SOUND_EFFECT18, 0.3f);
			return;
		}
		static_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Set_Money(-200);
		SOUND->PlaySound(L"chaching.wav", SOUND_EFFECT11, 0.3f);
		SCENE->Get_Scene()->Add_Object(PET, CAbstractFactory<CDogPet>::Create());
		buy_fox = false;
	}
	
	for (auto& iter : m_SpellDeque)
	{
		// left, top 은 렌더 스펠카드의 정보값
		// right, bottom 은 렌더 스펠카드의 크기만큼 더해준 값.
		if (i == 0 && bChk == true)
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
			default:
				break;
			}
			pSpell->Initialize();
			if (static_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Get_Creature().iMoney
				< pSpell->Get_SpellInfo().iMoney)
			{
				SOUND->PlaySound(L"locked_denied.wav", SOUND_EFFECT18, 0.3f);
				pSpell = nullptr;
				return;
			}
				
			static_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Set_Money(-pSpell->Get_SpellInfo().iMoney);
			SOUND->PlaySound(L"chaching.wav", SOUND_EFFECT11, 0.3f);
			// 덱에 넣어주고
			static_cast<CPlayer*>(
				SCENE->Get_Scene()->Get_Player())->Get_Deck()->Get_Inventory().push_back(pSpell);
			// 인벤토리UI 갱신
			SCENE->Get_Scene()->DeleteUI(UI_INVENTORY);
			SCENE->Get_Scene()->AddUI(UI_INVENTORY, CAbstractFactory<CInventoryUI>::Create_UI());
			break;
		}
		--i;
	}
	
}
