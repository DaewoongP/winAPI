#include "stdafx.h"
#include "TurretBullet.h"
#include "SceneMgr.h"
#include "BitmapMgr.h"
#include "SoundMgr.h"

CTurretBullet::CTurretBullet()
{
}


CTurretBullet::~CTurretBullet()
{
	Release();
}

void CTurretBullet::Initialize()
{
	m_tInfo.fCX = 44.8f;
	m_tInfo.fCY = 6.4f;
	m_fSpeed = 20.f;

	m_tSpellInfo.iAtk = 10;

	m_pFrameKey = L"TurretBullet";

	SOUND->PlaySound(L"mark_02_low.wav", SOUND_EFFECT02, 0.3f);
}

int CTurretBullet::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Count_Trigger(1);

	m_tInfo.fX += m_fSpeed;

	Check_Tile();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CTurretBullet::Late_Update()
{
	if (m_tRect.right > WINCX)
		m_bDead = true;
}

void CTurretBullet::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left) + 30,
		int(m_tRect.top) - 30,
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		0,
		0,
		28,		// 텍스처 원본 사이즈
		4,
		RGB(1, 1, 1));
}

void CTurretBullet::Release()
{
}

void CTurretBullet::Collilsion_Event(CObj * _pObj)
{
	this->Set_Dead();
}

void CTurretBullet::Count_Trigger(int _iTriggerCnt)
{
	if (m_iRenderCnt == _iTriggerCnt)
	{
		m_tInfo.fX += m_fSpeed * 1.6f;
	}
	if (m_iRenderCnt)
		++m_iRenderCnt;
}

void CTurretBullet::Player_Effect()
{
}
