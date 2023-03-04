#include "stdafx.h"
#include "GateCannon.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "DamageUI.h"
#include "SoundMgr.h"
#include "AbstractFactory.h"

CGateCannon::CGateCannon()
	:m_iRenderOffsetX(0), m_bLaserRender(false), m_iFrameCnt(0)
{
}


CGateCannon::~CGateCannon()
{
	Release();
}

void CGateCannon::Initialize()
{
	m_tInfo.fCX = 320.f;
	m_tInfo.fCY = 400.f;

	m_bAttacked = false;
	m_eObjID = ENEMIE;
	m_iAttack = 200;
	m_tCreature = { 200, 0, 100, 10 };

	BITMAP->Insert_Bmp(L"../Image/Enemie/Gate/Gatecannon.bmp", L"Gatecannon");
	BITMAP->Insert_Bmp(L"../Image/Enemie/Gate/GatecannonCharge.bmp", L"GatecannonCharge");
	
	m_pFrameKey = L"Gatecannon";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 9;
	m_tFrame.iImageEnd = 9;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 0;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 150;
	m_tFrame.dwTime = GetTickCount();

	m_eCurState = IDLE;
	m_ePreState = STATE_END;
}

int CGateCannon::Update()
{
	if (m_bDead || 0 >= m_tCreature.iHp)
	{
		if (m_iTileNum == 22)
			static_cast<CGate*>(SCENE->Get_Scene()->Get_ObjList(ENEMIE).front())->Set_Cannon2Cnt(-1);
		else
			static_cast<CGate*>(SCENE->Get_Scene()->Get_ObjList(ENEMIE).front())->Set_Cannon1Cnt(-1);
		return OBJ_DEAD;
	}

	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		m_eCurState = STATE_END;

	Count_Trigger(1);

	Reset_DamageFont();

	Attack();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CGateCannon::Late_Update()
{
	Motion_Change();
	if (m_eCurState == STATE_END)
		return;
	__super::Move_Frame();
}

void CGateCannon::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		(9 - m_tFrame.iFrameCnt) * 200,
		m_tFrame.iMotionCnt * 250,
		200,		// 텍스처 원본 사이즈
		250,
		RGB(1, 1, 1));

	if (m_bLaserRender)
	{
		HDC		hLaserDC = BITMAP->Find_Img(L"GatecannonCharge");
		GdiTransparentBlt(hDC,
			m_tRect.left - 510,
			(int)m_tInfo.fY - 80,
			1200, // 키울 사이즈
			200,
			hLaserDC,
			m_iFrameCnt * 600,
			0,
			600,		// 텍스처 원본 사이즈
			200,
			RGB(1, 1, 1));

		if (m_dwLaserTime + 200 < GetTickCount())
		{
			++m_iFrameCnt;
			m_dwLaserTime = GetTickCount();
		}
		if (m_iFrameCnt > 2)
		{
			m_iFrameCnt = 0;
			m_bLaserRender = false;
		}
	}

	m_tInfo.fX -= m_iRenderOffsetX;

	CCreature::Render(hDC);

	for (auto& iter : m_DamageList)
	{
		iter->Render_Damage(hDC);
	}

	m_tInfo.fX += m_iRenderOffsetX;
}

void CGateCannon::Release()
{
}

void CGateCannon::Attack()
{
	if (m_bLaserRender)
		m_AttackTileList.clear();
	if (m_dwTime + 5000 < GetTickCount())
	{
		m_eCurState = ATTACK;
		m_bLaserRender = true;
		m_dwLaserTime = GetTickCount();
		if (m_iTileNum / 8 == 1)
		{
			for (int i = 0; i < 4; ++i)
			{
				m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(8 + i)));
			}
		}
		else if (m_iTileNum / 8 == 2)
		{
			for (int i = 0; i < 4; ++i)
			{
				m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(16 + i)));
			}
		}

		SOUND->PlaySound(L"laser_echo_zap.wav", SOUND_EFFECT26, 0.3f);
		m_dwTime = GetTickCount();
	}
}

void CGateCannon::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iImageEnd = 9;
			m_tFrame.iMotion = 0;
			m_tFrame.iMotionEnd = 0;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 150;
			m_tFrame.dwTime = GetTickCount();
			break;
		case ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iImageEnd = 9;
			m_tFrame.iMotion = 1;
			m_tFrame.iMotionEnd = 1;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 150;
			m_tFrame.dwTime = GetTickCount();
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CGateCannon::Collilsion_Event(CObj * _pObj)
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

void CGateCannon::Count_Trigger(int _iTriggerCnt)
{
	if (m_iRenderCnt == _iTriggerCnt)
	{
		m_iRenderOffsetX += 80;
	}
	if (m_iRenderCnt)
		++m_iRenderCnt;
}
