#include "stdafx.h"
#include "DamageUI.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
CDamageUI::CDamageUI()
	:m_dwTime(GetTickCount())
{
}


CDamageUI::~CDamageUI()
{
}

void CDamageUI::Initialize()
{
	m_eID = UI_DAMAGE;
}

void CDamageUI::Update()
{
}

void CDamageUI::Late_Update()
{
}

void CDamageUI::Render(HDC hDC)
{
	Render_Damage(hDC);
}

void CDamageUI::Release()
{
}

void CDamageUI::Render_Damage(HDC hDC)
{
	TCHAR buf[32] = {};
	_itow_s(m_iDmg, buf, 10);
	HFONT hFont = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("quadaptor"));
	HFONT oldFont = (HFONT)SelectObject(hDC, hFont);
	SetTextAlign(hDC, TA_CENTER);
	SetTextColor(hDC, RGB(255, 0, 0));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC,
		(int)m_tInfo.fX,
		(int)m_tInfo.fY,
		buf, lstrlen(buf));
	(HFONT)DeleteObject(oldFont);
}
