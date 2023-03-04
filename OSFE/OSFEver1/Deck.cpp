#include "stdafx.h"
#include "Deck.h"
#include "AbstractFactory.h"
#include "BitmapMgr.h"
#include "SoundMgr.h"
#include "SpawnTurretGun.h"
#include "SpawnTurretLaser.h"
#include "Yami.h"
#include "SpawnViolette.h"
#include "Sunshine.h"

CDeck::CDeck()
	:m_pSlot(nullptr), m_dwTime(GetTickCount()), m_pFrameKey(nullptr), m_bReload(false), m_bSound(true)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}


CDeck::~CDeck()
{
	Release();
}

void CDeck::Add_Spell(CSpell* _pSpell)
{
	if (_pSpell)
	{
		m_SpellInventory.push_back(_pSpell);
	}
}

void CDeck::Delete_Spell(SPELLID _eID)
{
	for (auto iter = m_SpellInventory.begin();
	iter != m_SpellInventory.end();)
	{
		if ((*iter)->Get_SpellID() == _eID)
		{
			Safe_Delete<CSpell*>(*iter);
			iter = m_SpellInventory.erase(iter);
		}
		else
			++iter;
	}
}

void CDeck::Reload_Spell()
{
	for (auto& iter : m_SpellInventory)
	{
		CObj* pObj = nullptr;
		switch (iter->Get_SpellID())
		{
		case SPELL_ICENEEDLE:
			pObj = new CIceNeedle;
			break;
		case SPELL_MINIGUN:
			pObj = new CSuperMiniGun;
			break;
		case SPELL_EXPLOSION:
			pObj = new CExplosion;
			break;
		case SPELL_FIRESTORM:
			pObj = new CFirestorm;
			break;
		case SPELL_SHIELDCATCH:
			pObj = new CShieldCatch;
			break;
		case SPELL_THUNDER:
			pObj = new CThunder;
			break;
		case SPELL_RAGNAROK:
			pObj = new CRagnarok;
			break;
		case SPELL_TURRETGUN:
			pObj = new CSpawnTurretGun;
			break;
		case SPELL_TURRETLASER:
			pObj = new CSpawnTurretLaser;
			break;
		case SPELL_VIOLETTE:
			pObj = new CSpawnViolette;
			break;
		case SPELL_YAMI:
			pObj = new CYami;
			break;
		case SPELL_SUNSHINE:
			pObj = new CSunshine;
			break;
		default:
			break;
		}
		pObj->Initialize();

		if (rand() % 2)
			m_SpellDeck.push_back(static_cast<CSpell*>(pObj));
		else
			m_SpellDeck.push_front(static_cast<CSpell*>(pObj));
	}
}

void CDeck::Initialize()
{
	// 기본 생성 스펠들
	m_SpellInventory.push_back(static_cast<CSpell*>(CAbstractFactory<CThunder>::Create()));
	m_SpellInventory.push_back(static_cast<CSpell*>(CAbstractFactory<CRagnarok>::Create()));
	m_SpellInventory.push_back(static_cast<CSpell*>(CAbstractFactory<CIceNeedle>::Create()));
	m_SpellInventory.push_back(static_cast<CSpell*>(CAbstractFactory<CShieldCatch>::Create()));
	m_tInfo.fX = 35;
	m_tInfo.fY = 475;
	
	if (!m_pSlot)
	{
		m_pSlot = new CSlot;
		m_pSlot->Initialize();
	}

	BITMAP->Insert_Bmp(L"../Image/UI/Reload.bmp", L"Reload");
	m_pFrameKey = L"Reload";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iImageEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 9;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 89;
	m_tFrame.dwTime = GetTickCount();
}

void CDeck::Update()
{
	if (m_tRect.bottom <= 475)
		m_tInfo.fY += 5.f;
		
	// 슬롯이 모두 비워지면
	if (m_SpellDeck.empty() && m_pSlot->Check_Empty())
	{
		if (m_bSound)
		{
			SOUND->PlaySound(L"shuffle_start.wav", SOUND_EFFECT20, 0.3f);
			m_bSound = false;
		}
			
		Wait_Reload();
	}
	else
	{
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 0;
		m_tFrame.iImageEnd = 0;
		m_tFrame.iMotion = 0;
		m_tFrame.iMotionEnd = 9;
		m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
		m_tFrame.iMotionCnt = m_tFrame.iMotion;
		m_tFrame.dwSpeed = 89;
		m_tFrame.dwTime = GetTickCount();
		m_dwTime = GetTickCount();
	}

	m_pSlot->Update();

	Update_Rect();
}

void CDeck::Late_Update()
{
	Move_Frame();
	m_pSlot->Late_Update();
}

void CDeck::Render(HDC hDC)	
{
	if (!m_SpellDeck.empty())
	{
		int i = 0;
		for (auto& iter : m_SpellDeck)
		{
			iter->Render_SpellIcon(hDC, m_tRect.left, m_tRect.top - 60 * i, 50, 50);
			++i;
		}
	}
	m_pSlot->Render(hDC);

	if (m_bReload)
	{
		Render_Reload(hDC);
		m_bReload = false;
	}
		
}

void CDeck::Release()
{
	for (size_t i = 0; i < m_SpellDeck.size();)
	{
		Safe_Delete<CSpell*>(m_SpellDeck.front());
		m_SpellDeck.pop_front();
	}
	m_SpellDeck.clear();

	for (size_t i = 0; i < m_SpellInventory.size();)
	{
		Safe_Delete<CSpell*>(m_SpellInventory.front());
		m_SpellInventory.pop_front();
	}
	m_SpellInventory.clear();
	
	Safe_Delete<CSlot*>(m_pSlot);
}

void CDeck::Update_Rect()
{
	m_tRect = { long(m_tInfo.fX - m_tInfo.fCX * 0.5f),
		long(m_tInfo.fY - m_tInfo.fCY * 0.5f),
		long(m_tInfo.fX + m_tInfo.fCX * 0.5f),
		long(m_tInfo.fY + m_tInfo.fCY * 0.5f) };
}

void CDeck::Push_SlotQ()
{
	if (!m_SpellDeck.empty())
	{
		m_pSlot->Set_InfoQ({ 35, 475, 50, 50 });
		m_pSlot->Set_SlotQ(m_SpellDeck.front());
		m_SpellDeck.pop_front();
	}
}

void CDeck::Push_SlotW()
{
	if (!m_SpellDeck.empty())
	{
		m_pSlot->Set_InfoW({ 35, 475, 50, 50 });
		m_pSlot->Set_SlotW(m_SpellDeck.front());
		m_SpellDeck.pop_front();
	}
}

void CDeck::Wait_Reload()
{
	if (m_dwTime + 1000 < GetTickCount())
	{
		SOUND->PlaySound(L"shuffle_end.wav", SOUND_EFFECT19, 0.3f);
		m_bSound = true;
		m_SpellDeck.clear();
		Reload_Spell();
		m_pSlot->Set_SlotQ(m_SpellDeck.front());
		m_SpellDeck.pop_front();
		m_pSlot->Set_SlotW(m_SpellDeck.front());
		m_SpellDeck.pop_front();

		m_dwTime = GetTickCount();
	}
	else
		m_bReload = true;		
}

void CDeck::Render_Reload(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		35,
		480,
		50, // 키울 사이즈
		50,
		hMemDC,
		m_tFrame.iFrameCnt * 180,
		(9 - m_tFrame.iMotionCnt) * 180,
		180,		// 텍스처 원본 사이즈
		180,
		RGB(1, 1, 1));
}

void CDeck::Move_Frame(void)
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iFrameCnt;
		// 이미지 가로 장수보다 프레임이 작을경우 그냥 회전.
		if (m_tFrame.iFrameEnd <= m_tFrame.iImageEnd)
		{
			if (m_tFrame.iFrameCnt > m_tFrame.iFrameEnd)
			{
				m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
				m_tFrame.iMotionCnt = m_tFrame.iMotion;
				m_tFrame.iFrameEnd += (m_tFrame.iImageEnd + 1) * (m_tFrame.iMotionEnd - m_tFrame.iMotion);
			}
		}
		else
		{	// 세로가 엔드보다 커지면 초기화
			if (m_tFrame.iMotionCnt > m_tFrame.iMotionEnd)
			{
				m_tFrame.iMotionCnt = m_tFrame.iMotion;
			}
			else
			{
				// 이미지 가로 끝에 도달하면 초기화
				if (m_tFrame.iFrameCnt > m_tFrame.iImageEnd)
				{
					m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
					++m_tFrame.iMotionCnt;
					m_tFrame.iFrameEnd -= m_tFrame.iImageEnd + 1;
				}
			}
		}
		m_tFrame.dwTime = GetTickCount();
	}
}

void CDeck::Reload_Deck()
{
	for (size_t i = 0; i < m_SpellDeck.size();)
	{
		Safe_Delete<CSpell*>(m_SpellDeck.front());
		m_SpellDeck.pop_front();
	}
	m_SpellDeck.clear();
	Safe_Delete<CSpell*>(m_pSlot->Get_Slot().front());
	Safe_Delete<CSpell*>(m_pSlot->Get_Slot().back());
}

void CDeck::Push_Yami()
{
	for (size_t i = 0; i < m_SpellInventory.size();)
	{
		Safe_Delete<CSpell*>(m_SpellInventory.front());
		m_SpellInventory.pop_front();
	}
	m_SpellInventory.clear();

	m_SpellInventory.push_back(static_cast<CSpell*>(CAbstractFactory<CYami>::Create()));
	m_SpellInventory.push_back(static_cast<CSpell*>(CAbstractFactory<CYami>::Create()));
	m_SpellInventory.push_back(static_cast<CSpell*>(CAbstractFactory<CYami>::Create()));
	m_SpellInventory.push_back(static_cast<CSpell*>(CAbstractFactory<CYami>::Create()));
	m_SpellInventory.push_back(static_cast<CSpell*>(CAbstractFactory<CYami>::Create()));
	m_SpellInventory.push_back(static_cast<CSpell*>(CAbstractFactory<CYami>::Create()));
	m_SpellInventory.push_back(static_cast<CSpell*>(CAbstractFactory<CYami>::Create()));
	m_SpellInventory.push_back(static_cast<CSpell*>(CAbstractFactory<CYami>::Create()));
	m_SpellInventory.push_back(static_cast<CSpell*>(CAbstractFactory<CYami>::Create()));
	m_SpellInventory.push_back(static_cast<CSpell*>(CAbstractFactory<CYami>::Create()));

	Reload_Deck();
}
