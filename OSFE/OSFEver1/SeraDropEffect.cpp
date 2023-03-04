#include "stdafx.h"
#include "SeraDropEffect.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"

CSeraDropEffect::CSeraDropEffect()
	:m_fSpeed(0.f), m_fTime(0.f), m_fPower(0.f), m_fGravity(0.f), m_dwTime(GetTickCount()), m_fPreY(0.f)
{
}


CSeraDropEffect::~CSeraDropEffect()
{
	Release();
}

void CSeraDropEffect::Initialize()
{
	m_tInfo.fCX = 24.f;
	m_tInfo.fCY = 24.f;

	m_fSpeed = 1.f;
	m_iEffectCnt = 1;
	m_fPower = 50.f;
	m_fGravity = m_fPower * 4.f;
	m_fAccel = 0.f;
	m_fPreY = m_tInfo.fY;
}

void CSeraDropEffect::Update()
{
	PlayerInfo = SCENE->Get_Scene()->Get_Player()->Get_Info();
	m_fJump = m_fPower * m_fTime - 0.5f * m_fGravity * m_fTime * m_fTime;

	if (PlayerInfo.fY - m_tInfo.fY <= 100 && m_fJump < 0)
		Chase();
	else
		Jump();

	if (abs(m_tInfo.fX - PlayerInfo.fX) < TILECX * 0.5f &&
		abs(m_tInfo.fY - PlayerInfo.fY) < TILECY * 0.5f)
		--m_iEffectCnt;

	__super::Update_Rect();
}

void CSeraDropEffect::Late_Update()
{
}

void CSeraDropEffect::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(L"SeraDropLarge");

	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		0,
		0,
		24,
		24,
		RGB(1, 1, 1));
}

void CSeraDropEffect::Release()
{
}

void CSeraDropEffect::Jump()
{
	if (m_dwTime + 50 < GetTickCount())
	{
		m_fAccel += 1.f;
		m_fTime += 0.05f;
		m_dwTime = GetTickCount();
	}

	m_tInfo.fX -= m_fSpeed + m_fAccel;
	m_tInfo.fY += m_fJump;
}

void CSeraDropEffect::Chase()
{
	m_fWidth = m_tInfo.fX - PlayerInfo.fX;
	m_fHeight = m_tInfo.fY - PlayerInfo.fY;

	m_tInfo.fX -= m_fWidth / 15.f;
	m_tInfo.fY -= m_fHeight / 15.f;
}
