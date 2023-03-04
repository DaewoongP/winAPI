#include "stdafx.h"
#include "Hailcaster.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "AbstractFactory.h"
#include "SoundMgr.h"
#include "EffectMgr.h"

CHailcaster::CHailcaster()
{
}


CHailcaster::~CHailcaster()
{
	Release();
}

void CHailcaster::Initialize()
{
	m_tInfo.fCX = 192.f;
	m_tInfo.fCY = 192.f;
	m_fSpeed = 10.f;
	m_iAttack = 10;
	m_bAttacked = false;
	m_eObjID = ENEMIE;
	m_eCurState = IDLE;
	m_iTileNum = Check_Tile();
	m_iPreTileNum = m_iTileNum;

	m_tCreature = { 500, 0, 100, 10};
	BITMAP->Insert_Bmp(L"../Image/Enemie/Hailcaster.bmp", L"Hailcaster");
	BITMAP->Insert_Bmp(L"../Image/Enemie/HailStorm.bmp", L"HailStorm");
	m_pFrameKey = L"Hailcaster";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iImageEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 0;

	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;

	m_tFrame.dwSpeed = 400;
	m_tFrame.dwTime = GetTickCount();

}

int CHailcaster::Update()
{
	if (m_tCreature.iHp <= 0)
	{
		static_cast<CCreature*>(SCENE->Get_Scene()->Get_Player())->Set_Loot(
			m_tCreature.fExp, m_tCreature.iMoney);
		return OBJ_DEAD;
	}

	Reset_DamageFont();

	Attack();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CHailcaster::Late_Update()
{
	Motion_Change();

	__super::Move_Frame();
}

void CHailcaster::Render(HDC hDC)
{
	CCreature::Render(hDC);

	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		(3 - m_tFrame.iFrameCnt) * 120,
		m_tFrame.iMotionCnt * 120,
		120,		// 텍스처 원본 사이즈
		120,
		RGB(1, 1, 1));

	for (auto& iter : m_DamageList)
	{
		iter->Render_Damage(hDC);
	}
}

void CHailcaster::Release()
{
	EFFECT->Add_Effect(EFFECT_PARTICLE, CAbstractFactory<CParticleEffect>::Create_Effect(m_tInfo.fX, m_tInfo.fY));
}

void CHailcaster::Attack()
{
	if (m_dwTime + 6000 < GetTickCount())
	{
		m_eCurState = ATTACK;
		for (int i = 0; i < 4; ++i)
		{
			SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CHailStorm>::Create(rand() % 4 + 8 * i));
		}
		m_dwTime = GetTickCount();
	}
	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		m_eCurState = IDLE;
}

void CHailcaster::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iImageEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.iMotionEnd = 0;

			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;

			m_tFrame.dwSpeed = 300;
			m_tFrame.dwTime = GetTickCount();
			break;
		case ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iImageEnd = 3;
			m_tFrame.iMotion = 1;
			m_tFrame.iMotionEnd = 2;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 250;
			m_tFrame.dwTime = GetTickCount();
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CHailcaster::Collilsion_Event(CObj * _pObj)
{
	switch (_pObj->Get_OBJID())
	{
	case SPELL:
		m_tCreature.iHp -= dynamic_cast<CSpell*>(_pObj)->Get_SpellInfo().iAtk;
		SOUND->PlaySound(L"enemy_hit_light.wav", SOUND_EFFECT12, 0.3f);
		m_DamageList.push_back(CAbstractFactory<CDamageUI>::Create_DamageUI(m_tInfo.fX - 50, m_tInfo.fY - 100 - m_iDamageY, static_cast<CSpell*>(_pObj)->Get_SpellInfo().iAtk));
		Set_DamageY(20);
		break;
	default:
		break;
	}
}

void CHailcaster::Count_Trigger(int _iTriggerCnt)
{
}
