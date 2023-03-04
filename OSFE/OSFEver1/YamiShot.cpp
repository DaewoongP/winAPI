#include "stdafx.h"
#include "YamiShot.h"
#include "SoundMgr.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"

CYamiShot::CYamiShot()
{
}


CYamiShot::~CYamiShot()
{
	Release();
}

void CYamiShot::Initialize()
{
	//44 100
	m_eSpellID = SPELL_YAMI;
	m_tInfo.fCX = 70.4f;
	m_tInfo.fCY = 32.f;
	m_fSpeed = 20.f;

	m_tSpellInfo.iAtk = 99999;

	m_pFrameKey = L"YamiShot";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iImageEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 4;

	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;

	m_tFrame.dwSpeed = 150;
	m_tFrame.dwTime = GetTickCount();
	SOUND->PlaySound(L"mark_02.wav", SOUND_EFFECT02, 0.3f);
}

int CYamiShot::Update()
{
	if (m_bDead)
		return OBJ_DEAD;
	m_tInfo.fX += m_fSpeed;

	Check_Tile();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CYamiShot::Late_Update()
{
	if (m_tRect.right > WINCX)
		m_bDead = true;

	__super::Move_Frame();
}

void CYamiShot::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left + 50),
		int(m_tRect.top - 45),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 44,
		m_tFrame.iMotionCnt * 20,
		44,		// 텍스처 원본 사이즈
		20,
		RGB(1, 1, 1));
}

void CYamiShot::Release()
{
}

void CYamiShot::Collilsion_Event(CObj * _pObj)
{
	this->Set_Dead();

	SCENE->Get_Scene()->Set_Hit(5.f);
}

void CYamiShot::Count_Trigger(int _iTriggerCnt)
{
	
}

void CYamiShot::Player_Effect()
{
}
