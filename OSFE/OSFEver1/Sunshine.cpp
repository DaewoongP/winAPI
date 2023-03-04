#include "stdafx.h"
#include "Sunshine.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "AbstractFactory.h"
#include "SoundMgr.h"
CSunshine::CSunshine()
{
}


CSunshine::~CSunshine()
{
	Release();
}

void CSunshine::Initialize()
{

	//240 x 360
	m_tInfo.fCX = 384.f;
	m_tInfo.fCY = 800.f;
	m_eSpellID = SPELL_SUNSHINE;
	m_pSpellName = L"햇빛";
	m_tSpellInfo = { 4, 50, 4, 50, 0,
		L"적에게 빛을 퍼붓는다" };

	BITMAP->Insert_Bmp(L"../Image/Spell/Sunshine.bmp", m_pSpellName);
	BITMAP->Insert_Bmp(L"../Image/Spell/StormSunshineS.bmp", L"StormSunshineS");

	m_pFrameKey = L"StormSunshineS";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 2;
	m_tFrame.iImageEnd = 1;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 1;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();
}

int CSunshine::Update()
{
	if (m_bDead)
		return OBJ_DEAD;
	if (0 >= m_tSpellInfo.iCnt)
		return OBJ_DEAD;
	if (SCENE->Get_Scene()->Get_ObjList(ENEMIE).empty())
		return OBJ_DEAD;
	m_tInfo.fX = SCENE->Get_Scene()->Get_ObjList(ENEMIE).front()->Get_Info().fX;
	m_tInfo.fY = SCENE->Get_Scene()->Get_ObjList(ENEMIE).front()->Get_Info().fY;

	if (m_dwTime + 200 < GetTickCount())
	{
		if (m_tSpellInfo.iCnt)
		{
			SOUND->PlaySound(L"revita_fire.wav", SOUND_EFFECT28, 0.3f);
			static_cast<CCreature*>(SCENE->Get_Scene()->Get_ObjList(ENEMIE).front())->Set_Damage(m_tSpellInfo.iAtk);
			--m_tSpellInfo.iCnt;
		}
		m_dwTime = GetTickCount();
	}
	
	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CSunshine::Late_Update()
{
	__super::Move_Frame();
}

void CSunshine::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);

	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 240,
		m_tFrame.iMotionCnt * 360,
		240,		// 텍스처 원본 사이즈
		360,
		RGB(1, 1, 1));
}

void CSunshine::Release()
{
}

void CSunshine::Collilsion_Event(CObj * _pObj)
{
}

void CSunshine::Count_Trigger(int _iTriggerCnt)
{
}

void CSunshine::Player_Effect()
{
}
