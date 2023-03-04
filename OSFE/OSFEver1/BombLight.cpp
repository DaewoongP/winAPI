#include "stdafx.h"
#include "BombLight.h"
#include "BitmapMgr.h"
#include "TileMgr.h"
#include "SoundMgr.h"
#include "AbstractFactory.h"
#include "SceneMgr.h"

CBombLight::CBombLight()
{
}


CBombLight::~CBombLight()
{
	Release();
}

void CBombLight::Initialize()
{
	m_tInfo.fCX = 192.f;
	m_tInfo.fCY = 192.f;
	m_eObjID = ENEMIE;

	m_tCreature = { 40, 0, 0, 0 };

	m_pFrameKey = L"Bomblight";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.iImageEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 1;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 120;
	m_tFrame.dwTime = GetTickCount();
}

int CBombLight::Update()
{
	if (0 >= m_tCreature.iHp)
		return OBJ_DEAD;
	if (m_bDead)
		return OBJ_DEAD;

	Reset_DamageFont();

	Count_Trigger(150);

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CBombLight::Late_Update()
{
	__super::Move_Frame();
}

void CBombLight::Render(HDC hDC)
{
	if (m_iRenderCnt >= 5)
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
			m_tFrame.iMotionCnt * 120,
			120,		// 텍스처 원본 사이즈
			120,
			RGB(1, 1, 1));
	}

	for (auto& iter : m_DamageList)
	{
		iter->Render_Damage(hDC);
	}
}

void CBombLight::Release()
{
	SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CLightBall>::Create(m_tInfo.fX, m_tInfo.fY));
	SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CLightBall>::Create(m_tInfo.fX, m_tInfo.fY));
	SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CLightBall>::Create(m_tInfo.fX, m_tInfo.fY));
	SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CLightBall>::Create(m_tInfo.fX, m_tInfo.fY));
}

void CBombLight::Attack()
{
}

void CBombLight::Motion_Change()
{
}

void CBombLight::Collilsion_Event(CObj * _pObj)
{
	switch (_pObj->Get_OBJID())
	{
	case SPELL:
		m_tCreature.iHp -= static_cast<CSpell*>(_pObj)->Get_SpellInfo().iAtk;
		SOUND->PlaySound(L"enemy_hit_light.wav", SOUND_EFFECT12, 0.3f);
		m_DamageList.push_back(CAbstractFactory<CDamageUI>::Create_DamageUI(m_tInfo.fX - 50, m_tInfo.fY - 100 - m_iDamageY, static_cast<CSpell*>(_pObj)->Get_SpellInfo().iAtk));
		Set_DamageY(20);
		break;
	default:
		break;
	}
}

void CBombLight::Count_Trigger(int _iTriggerCnt)
{
	if (m_iRenderCnt == 5)
	{
		m_tInfo = dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum))->Get_Info();
		m_tInfo.fCX = 192.f;
		m_tInfo.fCY = 192.f;
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 5;
		m_tFrame.iImageEnd = 3;
		m_tFrame.iMotion = 0;
		m_tFrame.iMotionEnd = 1;
		m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
		m_tFrame.iMotionCnt = m_tFrame.iMotion;
		m_tFrame.dwSpeed = 120;
		m_tFrame.dwTime = GetTickCount();
	}

	if (m_iRenderCnt == _iTriggerCnt)
		this->Set_Dead();

	if (m_iRenderCnt)
		++m_iRenderCnt;
}
