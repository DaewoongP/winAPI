#include "stdafx.h"
#include "Effect.h"
#include "TileMgr.h"

CEffect::CEffect()
	:m_pFrameKey(nullptr), m_iEffectCnt(0)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}


CEffect::~CEffect()
{
}

void CEffect::Update_Rect()
{
	m_tRect = { long(m_tInfo.fX - m_tInfo.fCX * 0.5f),
		long(m_tInfo.fY - m_tInfo.fCY * 0.5f),
		long(m_tInfo.fX + m_tInfo.fCX * 0.5f),
		long(m_tInfo.fY + m_tInfo.fCY * 0.5f) };
}


void CEffect::Move_Frame(void)
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iFrameCnt;
		// �̹��� ���� ������� �������� ������� �׳� ȸ��.
		if (m_tFrame.iFrameEnd <= m_tFrame.iImageEnd)
		{
			if (m_tFrame.iFrameCnt > m_tFrame.iFrameEnd)
			{
				m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
				m_tFrame.iMotionCnt = m_tFrame.iMotion;
				m_tFrame.iFrameEnd += (m_tFrame.iImageEnd + 1) * (m_tFrame.iMotionEnd - m_tFrame.iMotion);
			}
		}
		else
		{	// ���ΰ� ���庸�� Ŀ���� �ʱ�ȭ
			if (m_tFrame.iMotionCnt > m_tFrame.iMotionEnd)
			{
				m_tFrame.iMotionCnt = m_tFrame.iMotion;
			}
			else
			{
				// �̹��� ���� ���� �����ϸ� �ʱ�ȭ
				if (m_tFrame.iFrameCnt > m_tFrame.iImageEnd)
				{
					m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
					++m_tFrame.iMotionCnt;
					m_tFrame.iFrameEnd -= m_tFrame.iImageEnd + 1;
				}
			}
		}
		m_tFrame.dwTime = GetTickCount();
	}
}

