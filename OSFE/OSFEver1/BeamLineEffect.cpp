#include "stdafx.h"
#include "BeamLineEffect.h"


CBeamLineEffect::CBeamLineEffect()
{
	ZeroMemory(&m_tPenInfo, sizeof(PENINFO));
}


CBeamLineEffect::~CBeamLineEffect()
{
	Release();
}

void CBeamLineEffect::Initialize()
{
	m_iEffectCnt = 1;
}

void CBeamLineEffect::Update()
{
	if (*m_tPenInfo.Point_End.fpX > 600)
		--m_iEffectCnt;
}

void CBeamLineEffect::Late_Update()
{
}

void CBeamLineEffect::Render(HDC hDC)
{
	if (m_tPenInfo.Point_End.fpX == nullptr ||
		m_tPenInfo.Point_End.fpY == nullptr)
		return;

	m_tPenInfo.CurPen = CreatePen(PS_SOLID, 3, RGB(244, 92, 253));
	m_tPenInfo.CurBrush = CreateSolidBrush(RGB(244, 92, 253));
	m_tPenInfo.OldPen = (HPEN)SelectObject(hDC, m_tPenInfo.CurPen);
	m_tPenInfo.OldBrush = (HBRUSH)SelectObject(hDC, m_tPenInfo.CurBrush);

	MoveToEx(hDC, m_tPenInfo.Point_Start.x, m_tPenInfo.Point_Start.y, nullptr);

	LineTo(hDC, *m_tPenInfo.Point_End.fpX, *m_tPenInfo.Point_End.fpY);

	DeleteObject(m_tPenInfo.OldPen);
	DeleteObject(m_tPenInfo.OldBrush);
}

void CBeamLineEffect::Release()
{
}

void CBeamLineEffect::Set_LineStart(float _fX, float _fY)
{
	m_tPenInfo.Point_Start.x = _fX;
	m_tPenInfo.Point_Start.y = _fY;
}

void CBeamLineEffect::Set_LineEnd(POINTPTR _Pointptr)
{
	m_tPenInfo.Point_End = _Pointptr;
}
