#include "stdafx.h"
#include "IceNeedle.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

CIceNeedle::CIceNeedle()
	:m_preTileNum(-1)
{
}


CIceNeedle::~CIceNeedle()
{
	Release();
}

void CIceNeedle::Initialize()
{
	// 가로 40 세로 6,6,6
	m_eSpellID = SPELL_ICENEEDLE;
	m_pSpellName = L"얼음 바늘";
	m_tInfo.fCX = 64.f;
	m_tInfo.fCY = 12.6f;
	m_fSpeed = 25.f;

	m_tSpellInfo = { 20, 1, 2, 40, 0, L"적을 꿰뚫고, 다시 돌아오는 가시를 발사" };

	BITMAP->Insert_Bmp(L"../Image/Spell/IceNeedle.bmp", m_pSpellName);
	m_pFrameKey = L"IceNeedleShot";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iImageEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 2;

	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;

	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();
	m_tSoundInfo = { L"spear_swoosh-0.wav" , SOUND_EFFECT01 , 0.3f };
}

int CIceNeedle::Update()
{
	if (m_bDead)
		return OBJ_DEAD;
	m_tInfo.fX += m_fSpeed;

	Check_Tile();

	// Damage Once
	if (m_preTileNum != m_iTileNum)
	{
		m_tSpellInfo.iAtk = 20;
		SCENE->Get_Scene()->Set_Hit(1.f);
		m_preTileNum = m_iTileNum;
	}
	else
		m_tSpellInfo.iAtk = 0;

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CIceNeedle::Late_Update()
{
	if (m_tRect.left > WINCX)
	{
		m_preTileNum = -1;
		m_pFrameKey = L"IceNeedleShotRev";
		m_fSpeed *= -1;
	}
	if (m_tRect.right < 0)
		m_bDead = true;

	__super::Move_Frame();
}

void CIceNeedle::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left + 50),
		int(m_tRect.top - 45),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 40,
		m_tFrame.iMotionCnt * 6,
		40,		// 텍스처 원본 사이즈
		6,
		RGB(1, 1, 1));
}

void CIceNeedle::Release()
{
}

void CIceNeedle::Collilsion_Event(CObj * _pObj)
{
}

void CIceNeedle::Count_Trigger(int _iTriggerCnt)
{
}

void CIceNeedle::Player_Effect()
{
}
