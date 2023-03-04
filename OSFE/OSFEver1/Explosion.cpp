#include "stdafx.h"
#include "Explosion.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "EffectMgr.h"
#include "AbstractFactory.h"
#include "SoundMgr.h"

CExplosion::CExplosion()
{
}


CExplosion::~CExplosion()
{
	Release();
}

void CExplosion::Initialize()
{
	m_eSpellID = SPELL_EXPLOSION;
	m_pSpellName = L"익스플로전!";
	m_tInfo.fCX = 666.f;
	m_tInfo.fCY = 666.f;
	m_tSpellInfo = { 666, 1, 6, 50, 0,
	L"3초 동안 충전 후 거대한 폭발을 생성"};

	BITMAP->Insert_Bmp(L"../Image/Spell/Explosion.bmp", m_pSpellName);
	
	m_pFrameKey = L"Explosion";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 44;
	m_tFrame.iImageEnd = 14;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 2;

	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;

	m_tFrame.dwSpeed = 120;
	m_tFrame.dwTime = GetTickCount();
}

int CExplosion::Update()
{
	m_tSpellInfo.iAtk = 0;
	static_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Set_Exhaust(true);
	if (m_bDead)
		return OBJ_DEAD;
	/*if (iCountDown == 249)
		SOUND->PlaySound(L"choir_megu.wav",SOUND_EFFECT15, 0.3f);
	if (iCountDown == 0)
		bSound = true;
	else
		--iCountDown;
	if (bSound)
	{
		SOUND->PlaySound(L"explosion_megu.wav", SOUND_EFFECT06, 0.3f);
		bSound = false;
	}*/

	Count_Trigger(250);
		
	Check_Tile();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CExplosion::Late_Update()
{
	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
	{
		SCENE->Get_Scene()->Set_Hit(30.f);
		for (auto& iter : SCENE->Get_Scene()->Get_ObjList(ENEMIE))
		{
			dynamic_cast<CCreature*>(iter)->Set_Damage(666);
		}
		m_bDead = true;
	}
	__super::Move_Frame();
}

void CExplosion::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		260,
		35,
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 264,
		m_tFrame.iMotionCnt * 400,
		264,		// 텍스처 원본 사이즈
		400,
		RGB(1, 1, 1));
}

void CExplosion::Release()
{
}

void CExplosion::Collilsion_Event(CObj * _pObj)
{
}

void CExplosion::Count_Trigger(int _iTriggerCnt)
{
	if (m_iRenderCnt == 1)
		SOUND->PlaySound(L"choir_megu.wav", SOUND_EFFECT15, 0.3f);
	else if (m_iRenderCnt == _iTriggerCnt)
	{
		SOUND->PlaySound(L"explosion_megu.wav", SOUND_EFFECT06, 0.3f);
		m_iRenderCnt = 0;
		SCENE->Get_Scene()->Set_Hit(30.f);
		static_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Set_Exhaust_byExplosion();
	}

	if (m_iRenderCnt)
		++m_iRenderCnt;
}

void CExplosion::Player_Effect()
{
	EFFECT->Add_Effect(EFFECT_SPELL, CAbstractFactory<CMagicFire>::Create_Effect(
		SCENE->Get_Scene()->Get_Player()->Get_Info().fX + 45.f,
		SCENE->Get_Scene()->Get_Player()->Get_Info().fY - 45.f));
}
