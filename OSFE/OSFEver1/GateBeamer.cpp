#include "stdafx.h"
#include "GateBeamer.h"
#include "BitmapMgr.h"
#include "DamageUI.h"
#include "AbstractFactory.h"
#include "SoundMgr.h"
#include "SceneMgr.h"
#include "EffectMgr.h"
#include "ExplodeEffect.h"

CGateBeamer::CGateBeamer()
	:m_iRenderOffsetY(0)
{
}


CGateBeamer::~CGateBeamer()
{
	Release();
}

void CGateBeamer::Initialize()
{
	// 200 x 250
	m_tInfo.fCX = 320.f;
	m_tInfo.fCY = 400.f;

	m_bAttacked = false;
	m_eObjID = ENEMIE;

	m_tCreature = { 200, 0, 100, 10 };

	BITMAP->Insert_Bmp(L"../Image/Enemie/Gate/Gatebeamer.bmp", L"Gatebeamer");
	m_pFrameKey = L"Gatebeamer";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 8;
	m_tFrame.iImageEnd = 2;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 2;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 200;
	m_tFrame.dwTime = GetTickCount();
}

int CGateBeamer::Update()
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

void CGateBeamer::Late_Update()
{
	__super::Move_Frame();
}

void CGateBeamer::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top) + m_iRenderOffsetY,
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		(2 - m_tFrame.iFrameCnt) * 200,
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

void CGateBeamer::Release()
{
	EFFECT->Add_Effect(EFFECT_TILE, CAbstractFactory<CExplodeEffect>::Create_Effect(m_tInfo.fX, m_tInfo.fY));
	SOUND->PlaySound(L"explode_fire.wav", SOUND_EFFECT16, 0.3f);
}

void CGateBeamer::Attack()
{
	if (m_dwTime + 5000 < GetTickCount())
	{
		SOUND->PlaySound(L"long_buzz.wav", SOUND_EFFECT13, 0.3f);
		for (int i = 0; i < 3; ++i)
		{
			SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CGateBeam>::Create(i * 8));
			EFFECT->Add_Effect(EFFECT_LINE, CAbstractFactory<CBeamLineEffect>::Create_Effect());
			static_cast<CBeamLineEffect*>(EFFECT->Get_Effect(EFFECT_LINE).back())->Set_LineStart(
				float(m_tInfo.fX + 30), 
				float(m_tRect.top + 58 + m_iRenderOffsetY));
			static_cast<CBeamLineEffect*>(EFFECT->Get_Effect(EFFECT_LINE).back())->Set_LineEnd(
				static_cast<CGateBeam*>(SCENE->Get_Scene()->Get_ObjList(ENEMIE_SPELL).back())->Get_PointPtr());
		}
		
		m_dwTime = GetTickCount();
	}
}

void CGateBeamer::Motion_Change()
{
}

void CGateBeamer::Collilsion_Event(CObj * _pObj)
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

void CGateBeamer::Count_Trigger(int _iTriggerCnt)
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

