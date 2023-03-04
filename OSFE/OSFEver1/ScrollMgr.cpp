#include "stdafx.h"
#include "ScrollMgr.h"

CScrollMgr*	 CScrollMgr::m_pInstance = nullptr;

CScrollMgr::CScrollMgr()
	: m_fScrollX(0.f), m_fScrollY(0.f), m_fHit(0.f)
{
}


CScrollMgr::~CScrollMgr()
{
}

void CScrollMgr::Scroll_Lock(void)
{
	if (0.f < m_fScrollX)
		m_fScrollX = 0.f;

	if (0.f < m_fScrollY)
		m_fScrollY = 0.f;

	if (WINCX - TILEX * TILECX > m_fScrollX)
		m_fScrollX = WINCX - TILEX * TILECX;

	if (WINCY - TILEY * TILECY > m_fScrollY)
		m_fScrollY = WINCY - TILEY * TILECY;
}

float CScrollMgr::Hit_ScrollX(float _fscroll)
{
	if (m_fHit < 0)
		return m_fHit += _fscroll;
	else
		return m_fHit -= _fscroll;
}