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
		// 이미지 가로 장수보다 프레임이 작을경우 그냥 회전.
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
		{	// 세로가 엔드보다 커지면 초기화
			if (m_tFrame.iMotionCnt > m_tFrame.iMotionEnd)
			{
				m_tFrame.iMotionCnt = m_tFrame.iMotion;
			}
			else
			{
				// 이미지 가로 끝에 도달하면 초기화
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

