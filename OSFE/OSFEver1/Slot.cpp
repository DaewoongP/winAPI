#include "stdafx.h"
#include "Slot.h"


CSlot::CSlot()
{
	ZeroMemory(&m_tInfoQ, sizeof(INFO));
	ZeroMemory(&m_tInfoW, sizeof(INFO));
	ZeroMemory(&m_tRectQ, sizeof(RECT));
	ZeroMemory(&m_tRectW, sizeof(RECT));
}


CSlot::~CSlot()
{
	Release();
}

void CSlot::Initialize()
{
	// (35, 475) -> (135,553) , (205, 553)
	m_tInfoQ = { 35, 475, 50, 50 };
	m_tInfoW = { 35, 475, 50, 50 };
	m_SpellSlot.push_back(nullptr);
	m_SpellSlot.push_back(nullptr);
}

void CSlot::Update()
{
	if (m_SpellSlot.front() != nullptr)
	{
		if (m_tInfoQ.fX < 135)
			m_tInfoQ.fX += 10.f;
		else
			m_tInfoQ.fX = 135.f;
		if (m_tInfoQ.fY < 553)
			m_tInfoQ.fY += 7.8f;
		else
			m_tInfoQ.fY = 553.f;
	}

	if (m_SpellSlot.back() != nullptr)
	{
		if (m_tInfoW.fX < 205)
			m_tInfoW.fX += 17.f;
		else
			m_tInfoW.fX = 205.f;
		if (m_tInfoW.fY < 553)
			m_tInfoW.fY += 7.8f;
		else
			m_tInfoW.fY = 553.f;
	}
	
	Update_Rect();
}

void CSlot::Late_Update()
{
}

void CSlot::Render(HDC hDC)
{
	// 슬롯에 스킬 하나만 있을때 처리
	if (m_SpellSlot.front() != nullptr)
		m_SpellSlot.front()->Render_SpellIcon(hDC, m_tRectQ.left, m_tRectQ.top, 50, 50);
	if (m_SpellSlot.back() != nullptr)
		m_SpellSlot.back()->Render_SpellIcon(hDC, m_tRectW.left, m_tRectW.top, 50, 50);
}

void CSlot::Release()
{
	for (size_t i = 0; i < m_SpellSlot.size();)
	{
		Safe_Delete<CSpell*>(m_SpellSlot.front());
		m_SpellSlot.pop_front();
	}
}

bool CSlot::Check_Empty()
{
	if (m_SpellSlot.front() == nullptr && m_SpellSlot.back() == nullptr)
		return true;		
	else
		return false;
}

void CSlot::Update_Rect()
{
	m_tRectQ = { long(m_tInfoQ.fX - m_tInfoQ.fCX * 0.5f),
		long(m_tInfoQ.fY - m_tInfoQ.fCY * 0.5f),
		long(m_tInfoQ.fX + m_tInfoQ.fCX * 0.5f),
		long(m_tInfoQ.fY + m_tInfoQ.fCY * 0.5f) };

	m_tRectW = { long(m_tInfoW.fX - m_tInfoW.fCX * 0.5f),
		long(m_tInfoW.fY - m_tInfoW.fCY * 0.5f),
		long(m_tInfoW.fX + m_tInfoW.fCX * 0.5f),
		long(m_tInfoW.fY + m_tInfoW.fCY * 0.5f) };
}
