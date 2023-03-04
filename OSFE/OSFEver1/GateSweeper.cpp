#include "stdafx.h"
#include "GateSweeper.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "DamageUI.h"
#include "SoundMgr.h"
#include "AbstractFactory.h"

CGateSweeper::CGateSweeper()
	:m_iRenderOffsetY(0), m_pBullet(nullptr), m_iTargetCnt(0)
{
}


CGateSweeper::~CGateSweeper()
{
	Release();
}

void CGateSweeper::Initialize()
{
	m_tInfo.fCX = 320.f;
	m_tInfo.fCY = 400.f;

	m_eObjID = ENEMIE;

	m_tCreature = { 200, 0, 100, 10 };

	BITMAP->Insert_Bmp(L"../Image/Enemie/Gate/GateSweeper.bmp", L"GateSweeper");
	BITMAP->Insert_Bmp(L"../Image/Enemie/Gate/Sweeper_Bullet.bmp", L"Sweeper_Bullet");
	m_pFrameKey = L"GateSweeper";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iImageEnd = 2;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 1;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 120;
	m_tFrame.dwTime = GetTickCount();

	if (!m_pBullet)
	{
		m_pBullet = new CGateBullet;
	}
}

int CGateSweeper::Update()
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

void CGateSweeper::Late_Update()
{
	__super::Move_Frame();
}

void CGateSweeper::Render(HDC hDC)
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

void CGateSweeper::Release()
{
	Safe_Delete<CGateBullet*>(m_pBullet);
}

void CGateSweeper::Attack()
{
	if (15 < m_iTargetCnt)
		m_iTargetCnt = 0;
	int TargetArray[16] = 
	{
		27,19,11,3,2,10,18,26,25,17,9,1,0,8,16,24
	};
	if (m_dwTime + 100 < GetTickCount())
	{
		m_pBullet->Set_Pos(
			float(m_tInfo.fX + 30),
			float(m_tRect.top + 58 + m_iRenderOffsetY));

		SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL,
			CAbstractFactory<CGateBullet>::Create(*m_pBullet, TargetArray[m_iTargetCnt]));
		SOUND->PlaySound(L"gun_rifle_shot.wav", SOUND_EFFECT29, 0.1f);
		++m_iTargetCnt;

		m_dwTime = GetTickCount();
	}
}

void CGateSweeper::Motion_Change()
{
}

void CGateSweeper::Collilsion_Event(CObj * _pObj)
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

void CGateSweeper::Count_Trigger(int _iTriggerCnt)
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
