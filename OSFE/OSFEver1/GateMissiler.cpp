#include "stdafx.h"
#include "GateMissiler.h"
#include "BitmapMgr.h"
#include "DamageUI.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "AbstractFactory.h"

CGateMissiler::CGateMissiler()
	:m_iRenderOffsetY(0), m_pMissile(nullptr)
{
}


CGateMissiler::~CGateMissiler()
{
	Release();
}

void CGateMissiler::Initialize()
{
	// 200 x 250
	m_tInfo.fCX = 320.f;
	m_tInfo.fCY = 400.f;

	m_bAttacked = false;
	m_eObjID = ENEMIE;

	m_tCreature = { 200, 0, 100, 10 };

	BITMAP->Insert_Bmp(L"../Image/Enemie/Gate/Gatemissiler.bmp", L"Gatemissiler");
	BITMAP->Insert_Bmp(L"../Image/Enemie/Gate/Plg.bmp", L"GatePlg");
	BITMAP->Insert_Bmp(L"../Image/Enemie/Gate/Reset.bmp", L"GateReset");
	BITMAP->Insert_Bmp(L"../Image/Enemie/Gate/Missile.bmp", L"Missile");
	m_pFrameKey = L"Gatemissiler";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 14;
	m_tFrame.iImageEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 3;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 200;
	m_tFrame.dwTime = GetTickCount();
	
	if (!m_pMissile)
	{
		m_pMissile = new CGateMissile;
		m_pMissile->Initialize();
	}
}

int CGateMissiler::Update()
{
	if (m_bDead || 0 >= m_tCreature.iHp)
	{
		if (m_iTileNum == 22)
			static_cast<CGate*>(SCENE->Get_Scene()->Get_ObjList(ENEMIE).front())->Set_Spawn2Cnt(-1);
		else
			static_cast<CGate*>(SCENE->Get_Scene()->Get_ObjList(ENEMIE).front())->Set_Spawn1Cnt(-1);
		return OBJ_DEAD;
	}

	Count_Trigger(1);

	Reset_DamageFont();

	Attack();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CGateMissiler::Late_Update()
{
	__super::Move_Frame();
}

void CGateMissiler::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top) + m_iRenderOffsetY,
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		(3 - m_tFrame.iFrameCnt) * 200,
		m_tFrame.iMotionCnt * 250,
		200,		// 텍스처 원본 사이즈
		250,
		RGB(1, 1, 1));

	CCreature::Render(hDC);

	for (auto& iter : m_DamageList)
	{
		iter->Render_Damage(hDC);
	}
}

void CGateMissiler::Release()
{
	Safe_Delete<CGateMissile*>(m_pMissile);
}

void CGateMissiler::Attack()
{
	if (m_dwTime + 5000 < GetTickCount())
	{
		m_pMissile->Set_Pos(float(m_tInfo.fX + 30), float(m_tRect.top + 58 + m_iRenderOffsetY));

		int	m_MissileTarget[5] = { -8, -1, 0, 1, 8 };

		for (int i = 0; i < 5; ++i)
		{
			if (SCENE->Get_Scene()->Get_Player()->Get_TileNum() + m_MissileTarget[i] < 0 || 
				SCENE->Get_Scene()->Get_Player()->Get_TileNum() + m_MissileTarget[i] > 32)
				continue;
			SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CGateMissile>::Create(
				*m_pMissile,
				SCENE->Get_Scene()->Get_Player()->Get_TileNum() + m_MissileTarget[i]));
		}

		m_dwTime = GetTickCount();
	}
}

void CGateMissiler::Motion_Change()
{
}

void CGateMissiler::Collilsion_Event(CObj * _pObj)
{
	switch (_pObj->Get_OBJID())
	{
	case SPELL:
		m_tCreature.iHp -= static_cast<CSpell*>(_pObj)->Get_SpellInfo().iAtk;
		SOUND->PlaySound(L"enemy_hit_light.wav", SOUND_EFFECT12, 0.3f);
		m_DamageList.push_back(CAbstractFactory<CDamageUI>::Create_DamageUI(m_tInfo.fX, m_tInfo.fY - 170 - m_iDamageY, static_cast<CSpell*>(_pObj)->Get_SpellInfo().iAtk));
		Set_DamageY(20);
		break;
	default:
		break;
	}
}

void CGateMissiler::Count_Trigger(int _iTriggerCnt)
{
	if (m_iRenderCnt == _iTriggerCnt)
	{
		if (m_iTileNum == 22)
		{
			m_iRenderOffsetY += 20;
		}
	}
	if (m_iRenderCnt)
		++m_iRenderCnt;
}
