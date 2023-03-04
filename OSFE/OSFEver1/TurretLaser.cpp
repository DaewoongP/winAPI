#include "stdafx.h"
#include "TurretLaser.h"
#include "SceneMgr.h"
#include "AbstractFactory.h"
#include "BitmapMgr.h"
#include "SoundMgr.h"
#include "EffectMgr.h"
#include "ExplodeEffect.h"

CTurretLaser::CTurretLaser()
	:m_iFrameCnt(0), m_bLaserRender(false)
{
}


CTurretLaser::~CTurretLaser()
{
	Release();
}

void CTurretLaser::Initialize()
{
	m_tInfo.fCX = 192.f;
	m_tInfo.fCY = 192.f;
	m_eObjID = TURRET;
	m_pFrameKey = L"Turretlaser";
	BITMAP->Insert_Bmp(L"../Image/Spell/LuxCharge.bmp", L"LuxCharge");
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 18;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 2;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.iImageEnd = 7;
	m_tFrame.dwSpeed = 150;
	m_tFrame.dwTime = GetTickCount();
	
	m_iTileNum = Check_Tile();
	m_iPreTileNum = m_iTileNum;
	m_tCreature = { 120, 0, 0, 0 };

	m_iRenderCnt = 110;
	m_tInfo.fX = TILE->Get_TileInfo(m_iTileNum).fX;
	m_tInfo.fY = TILE->Get_TileInfo(m_iTileNum).fY;
	SOUND->PlaySound(L"machine_spawn.wav", SOUND_EFFECT28, 0.3f);
}

int CTurretLaser::Update()
{
	if (m_tCreature.iHp <= 0)
	{
		// dead effect
		return OBJ_DEAD;
	}
	if (m_bDead)
		return OBJ_DEAD;

	Reset_DamageFont();

	Count_Trigger(190);

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CTurretLaser::Late_Update()
{
	__super::Move_Frame();
}

void CTurretLaser::Render(HDC hDC)
{
	if (m_bRenderSkip)
	{
		m_bRenderSkip = false;
		return;
	}

	CCreature::Render(hDC);

	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 120,
		m_tFrame.iMotionCnt * 120,
		120,		// 텍스처 원본 사이즈
		120,
		RGB(1, 1, 1));

	if (m_bLaserRender)
	{
		HDC		hLaserDC = BITMAP->Find_Img(L"LuxCharge");
		GdiTransparentBlt(hDC,
			int(m_tRect.left) - 675,
			int(m_tRect.top) - 15,
			1600, // 키울 사이즈
			200,
			hLaserDC,
			m_iFrameCnt * 600,
			0,
			600,		// 텍스처 원본 사이즈
			200,
			RGB(1, 1, 1));
		if (m_iRenderCnt % 10 == 0)
		{
			++m_iFrameCnt;
		}
		if (2 < m_iFrameCnt)
		{
			m_iFrameCnt = 0;
			m_bLaserRender = false;
		}
	}

	for (auto& iter : m_DamageList)
	{
		iter->Render_Damage(hDC);
	}
}

void CTurretLaser::Release()
{
	EFFECT->Add_Effect(EFFECT_TILE, CAbstractFactory<CExplodeEffect>::Create_Effect(m_tInfo.fX, m_tInfo.fY));
	SOUND->PlaySound(L"explode_fire.wav", SOUND_EFFECT27, 0.3f);
}

void CTurretLaser::Collilsion_Event(CObj * _pObj)
{
	if (_pObj->Get_OBJID() == ENEMIE || _pObj->Get_OBJID() == ENEMIE_SPELL)
	{
		m_tCreature.iHp -= dynamic_cast<CEnemie*>(_pObj)->Get_Attack();
		m_DamageList.push_back(CAbstractFactory<CDamageUI>::Create_DamageUI(m_tInfo.fX - 50, m_tInfo.fY - 100 - m_iDamageY, static_cast<CEnemie*>(_pObj)->Get_Attack()));
		Set_DamageY(20);
		SOUND->PlaySound(L"enemy_hit_light.wav", SOUND_EFFECT12, 0.3f);
		EFFECT->Add_Effect(EFFECT_HIT, CAbstractFactory<CHitEffect>::Create_Effect(m_tInfo.fX, m_tInfo.fY));
	}

	if (_pObj->Get_OBJID() == SPELL)
	{
		m_tCreature.iHp -= dynamic_cast<CSpell*>(_pObj)->Get_SpellInfo().iAtk;
		m_DamageList.push_back(CAbstractFactory<CDamageUI>::Create_DamageUI(m_tInfo.fX - 50, m_tInfo.fY - 100 - m_iDamageY, dynamic_cast<CSpell*>(_pObj)->Get_SpellInfo().iAtk));
		Set_DamageY(20);
		SOUND->PlaySound(L"enemy_hit_light.wav", SOUND_EFFECT12, 0.3f);
		EFFECT->Add_Effect(EFFECT_HIT, CAbstractFactory<CHitEffect>::Create_Effect(m_tInfo.fX, m_tInfo.fY));
	}
}

void CTurretLaser::Count_Trigger(int _iTriggerCnt)
{
	if (m_iRenderCnt == _iTriggerCnt)
	{
		m_bLaserRender = true;
		for (auto& iter : SCENE->Get_Scene()->Get_ObjList(ENEMIE))
		{
			if (iter->Get_TileNum() / 8 == m_iTileNum / 8)
				static_cast<CCreature*>(iter)->Set_Damage(150);
		}
		SOUND->PlaySound(L"laser_echo_zap.wav", SOUND_EFFECT26, 0.3f);
		m_iRenderCnt = 1;
	}

	if (m_iRenderCnt)
		++m_iRenderCnt;
}
