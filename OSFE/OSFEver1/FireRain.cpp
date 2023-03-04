#include "stdafx.h"
#include "FireRain.h"
#include "BitmapMgr.h"
#include "TileMgr.h"
#include "SoundMgr.h"
#include "SceneMgr.h"

CFireRain::CFireRain()
{
}


CFireRain::~CFireRain()
{
}

void CFireRain::Initialize()
{
	//240 x 360
	m_tInfo.fCX = 384.f;
	m_tInfo.fCY = 576.f;
	// 비트맵은 캐스터에서 미리 대입 (여러번 적용을 피하기 위해서.)
	m_pFrameKey = L"FireStorm";
	m_tSpellInfo.iAtk = 0;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iImageEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 1;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();

	m_iTileNum = Check_Tile();
	SOUND->PlaySound(L"flare-deploy.wav", SOUND_EFFECT03, 0.1f);
}

int CFireRain::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Count_Trigger(25);

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CFireRain::Late_Update()
{
	__super::Move_Frame();
}

void CFireRain::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 240,
		m_tFrame.iMotionCnt * 360,
		240,		// 텍스처 원본 사이즈
		360,
		RGB(1, 1, 1));
}

void CFireRain::Release()
{
}

void CFireRain::Collilsion_Event(CObj * _pObj)
{
}

void CFireRain::Count_Trigger(int _iTriggerCnt)
{
	if (m_iRenderCnt == _iTriggerCnt)
	{
		m_tSpellInfo.iAtk = 30;
		m_iRenderCnt = 100;
		SCENE->Get_Scene()->Set_Hit(2.f);
	}
	else if (m_iRenderCnt == 101)
	{
		m_tSpellInfo.iAtk = 0;
	}
	else if (m_iRenderCnt == 111)
		m_bDead = true;
		
	if (m_iRenderCnt)
		++m_iRenderCnt;
}

void CFireRain::Player_Effect()
{
}
