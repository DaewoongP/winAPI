#include "stdafx.h"
#include "Bracer.h"
#include "BitmapMgr.h"
#include "SoundMgr.h"
#include "SceneMgr.h"
CBracer::CBracer()
{
}


CBracer::~CBracer()
{
	Release();
}

void CBracer::Initialize()
{
	m_tInfo.fCX = 35.2f;
	m_tInfo.fCY = 8.f;
	m_fSpeed = 13.f;
	m_tSpellInfo.iAtk = 2;

	m_pFrameKey = L"BasicSpell";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iImageEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 1;

	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;

	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();
	
	SOUND->PlaySound(L"mark_02.wav", SOUND_EFFECT00, 0.3f);
}

int CBracer::Update()
{
	if (m_bDead)
		return OBJ_DEAD;
	m_tInfo.fX += m_fSpeed;

	Check_Tile();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CBracer::Late_Update()
{
	if (m_tRect.right > WINCX)
		m_bDead = true;

	__super::Move_Frame();
}

void CBracer::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left + 50),
		int(m_tRect.top - 45),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		0,
		m_tFrame.iMotionCnt * 5,
		22,		// 텍스처 원본 사이즈
		5,
		RGB(1, 1, 1));
}

void CBracer::Release()
{
}

void CBracer::Collilsion_Event(CObj * _pObj)
{
	this->Set_Dead();
	SCENE->Get_Scene()->Set_Hit(2.f);
}

void CBracer::Count_Trigger(int _iTriggerCnt)
{
}

void CBracer::Player_Effect()
{
}
