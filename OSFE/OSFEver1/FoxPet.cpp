#include "stdafx.h"
#include "FoxPet.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "AbstractFactory.h"
#include "SoundMgr.h"
#include "EffectMgr.h"
CFoxPet::CFoxPet()
{
}


CFoxPet::~CFoxPet()
{
	Release();
}

void CFoxPet::Initialize()
{
	// 120 x 115
	m_tInfo.fCX = 192.f;
	m_tInfo.fCY = 176.f;
	m_eObjID = PET;
	BITMAP->Insert_Bmp(L"../Image/Pet/Fox.bmp", L"Fox");
	m_pFrameKey = L"Fox";
	m_ePreState = STATE_END;
	m_eCurState = IDLE;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 0;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.iImageEnd = 7;
	m_tFrame.dwSpeed = 150;
	m_tFrame.dwTime = GetTickCount();

	m_iTileNum = Check_Tile();
	m_iPreTileNum = m_iTileNum;
	m_tCreature = { 800, 0, 0, 0 };
}

int CFoxPet::Update()
{
	if (m_tCreature.iHp <= 0)
	{
		m_eCurState = DEAD;
		m_tCreature.iHp = 0;
		Reset_DamageFont();
		Motion_Change();
		
		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
			return OBJ_DEAD;
		else
			return OBJ_NOEVENT;
	}
		
	Set_Pos(static_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Get_PreTileNum());
	//m_tInfo.fX = static_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Get_Info().fX;
	//m_tInfo.fY = static_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Get_Info().fY;
	Reset_DamageFont();
	Heal();
	
	Motion_Change();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CFoxPet::Late_Update()
{
	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		m_eCurState = IDLE;
	__super::Move_Frame();
}

void CFoxPet::Render(HDC hDC)
{
	CCreature::Render(hDC);
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 120,
		m_tFrame.iMotionCnt * 110,
		120,		// 텍스처 원본 사이즈
		110,
		RGB(1, 1, 1));

	for (auto& iter : m_DamageList)
	{
		iter->Render_Damage(hDC);
	}
}

void CFoxPet::Release()
{
}

void CFoxPet::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotion = 0;
			m_tFrame.iMotionEnd = 0;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.iImageEnd = 7;
			m_tFrame.dwSpeed = 150;
			m_tFrame.dwTime = GetTickCount();
			break;

		case ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotion = 1;
			m_tFrame.iMotionEnd = 1;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.iImageEnd = 7;
			m_tFrame.dwSpeed = 150;
			m_tFrame.dwTime = GetTickCount();
			break;
		case DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 12;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotion = 3;
			m_tFrame.iMotionEnd = 4;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.iImageEnd = 7;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CFoxPet::Collilsion_Event(CObj * _pObj)
{
	if (_pObj->Get_OBJID() == ENEMIE || _pObj->Get_OBJID() == ENEMIE_SPELL)
	{
		m_tCreature.iHp -= dynamic_cast<CEnemie*>(_pObj)->Get_Attack();
		m_DamageList.push_back(CAbstractFactory<CDamageUI>::Create_DamageUI(m_tInfo.fX - 50, m_tInfo.fY - 100 - m_iDamageY, static_cast<CEnemie*>(_pObj)->Get_Attack()));
		Set_DamageY(20);
		SOUND->PlaySound(L"enemy_hit_light.wav", SOUND_EFFECT12, 0.3f);
		EFFECT->Add_Effect(EFFECT_HIT, CAbstractFactory<CHitEffect>::Create_Effect(m_tInfo.fX, m_tInfo.fY));
	}
}

void CFoxPet::Count_Trigger(int _iTriggerCnt)
{
}

void CFoxPet::Heal()
{
	if (m_dwTime + 10000 < GetTickCount())
	{
		m_eCurState = ATTACK;
		SCENE->Get_Scene()->Add_Object(SPELL, CAbstractFactory<CHealOrb>::Create(m_tInfo.fX, m_tInfo.fY));

		m_dwTime = GetTickCount();
	}
}
