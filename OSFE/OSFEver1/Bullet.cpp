#include "stdafx.h"
#include "Bullet.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

CBullet::CBullet()
{
}


CBullet::~CBullet()
{
	Release();
}

void CBullet::Initialize()
{
	m_tInfo.fCX = 35.2f;
	m_tInfo.fCY = 6.4f;
	m_fSpeed = 20.f;

	m_tSpellInfo.iAtk = 2;

	m_pFrameKey = L"MinigunBullet";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iImageEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 2;

	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;

	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();
	SOUND->PlaySound(L"mark_02_low.wav", SOUND_EFFECT02, 0.3f);
}

int CBullet::Update()
{
	if (m_bDead)
		return OBJ_DEAD;
	m_tInfo.fX += m_fSpeed;

	Check_Tile();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CBullet::Late_Update()
{
	if (m_tRect.right > WINCX)
		m_bDead = true;

	__super::Move_Frame();
}

void CBullet::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left + 50),
		int(m_tRect.top - 45),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 22,
		m_tFrame.iMotionCnt * 4,
		22,		// 텍스처 원본 사이즈
		4,
		RGB(1, 1, 1));
}

void CBullet::Release()
{
}

void CBullet::Collilsion_Event(CObj * _pObj)
{
	this->Set_Dead();

	SCENE->Get_Scene()->Set_Hit(2.f);
}

void CBullet::Count_Trigger(int _iTriggerCnt)
{
}
