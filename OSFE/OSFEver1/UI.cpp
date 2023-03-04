#include "stdafx.h"
#include "UI.h"


CUI::CUI()
	:m_pFrameKey(nullptr), m_eID(UI_END)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}


CUI::~CUI()
{
}

void CUI::Update_Rect()
{
	m_tRect = { long(m_tInfo.fX - m_tInfo.fCX * 0.5f),
		long(m_tInfo.fY - m_tInfo.fCY * 0.5f),
		long(m_tInfo.fX + m_tInfo.fCX * 0.5f),
		long(m_tInfo.fY + m_tInfo.fCY * 0.5f) };
}
