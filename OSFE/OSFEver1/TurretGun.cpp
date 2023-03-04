#include "stdafx.h"
#include "TurretGun.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "AbstractFactory.h"
#include "SoundMgr.h"
#include "EffectMgr.h"
#include "ExplodeEffect.h"

CTurretGun::CTurretGun()
	:m_pTurretBullet(nullptr)
{
}


CTurretGun::~CTurretGun()
{
	Release();
}

void CTurretGun::Initialize()
{
	m_tInfo.fCX = 192.f;
	m_tInfo.fCY = 192.f;
	m_eObjID = TURRET;
	m_pFrameKey = L"Turretgun";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 8;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 2;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.iImageEnd = 3;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();

	m_iTileNum = Check_Tile();
	m_iPreTileNum = m_iTileNum;
	m_tCreature = { 120, 0, 0, 0 };
	if (!m_pTurretBullet)
	{
		m_pTurretBullet = CAbstractFactory<CTurretBullet>::Create();
	}
	m_dwTime = GetTickCount() - 800;

	m_tInfo.fX = TILE->Get_TileInfo(m_iTileNum).fX;
	m_tInfo.fY = TILE->Get_TileInfo(m_iTileNum).fY;
	SOUND->PlaySound(L"machine_spawn.wav", SOUND_EFFECT28, 0.3f);
}

int CTurretGun::Update()
{
	if (m_tCreature.iHp <= 0)
	{
		// dead effect
		return OBJ_DEAD;
	}
	if (m_bDead)
		return OBJ_DEAD;

	if (m_tFrame.iFrameCnt == 3)
	{
		if (m_dwTime + 100 < GetTickCount())
		{
			SCENE->Get_Scene()->Add_Object(SPELL, CAbstractFactory<CTurretBullet>::Create_Spell(*static_cast<CTurretBullet*>(m_pTurretBullet), m_iTileNum));
			m_dwTime = GetTickCount();
		}
	}

	Reset_DamageFont();
	
	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CTurretGun::Late_Update()
{
	__super::Move_Frame();
}

void CTurretGun::Render(HDC hDC)
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

	for (auto& iter : m_DamageList)
	{
		iter->Render_Damage(hDC);
	}
}

void CTurretGun::Release()
{
	EFFECT->Add_Effect(EFFECT_TILE, CAbstractFactory<CExplodeEffect>::Create_Effect(m_tInfo.fX, m_tInfo.fY));
	SOUND->PlaySound(L"explode_fire.wav", SOUND_EFFECT27, 0.3f);
	Safe_Delete<CObj*>(m_pTurretBullet);
}

void CTurretGun::Collilsion_Event(CObj * _pObj)
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

void CTurretGun::Count_Trigger(int _iTriggerCnt)
{
}
