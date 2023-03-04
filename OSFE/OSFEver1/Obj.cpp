#include "stdafx.h"
#include "Obj.h"
#include "TileMgr.h"

CObj::CObj()
	:m_dwTime(GetTickCount()), m_pFrameKey(nullptr), m_bDead(OBJ_NOEVENT), m_fSpeed(0), 
	m_eObjID(OBJ_END), m_iTileNum(-1), m_iRenderCnt(1), m_iTargetTile(-1), m_bRenderSkip(true)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}


CObj::~CObj()
{
}

void CObj::Update_Rect()
{
	m_tRect = { long(m_tInfo.fX - m_tInfo.fCX * 0.5f),
				long(m_tInfo.fY - m_tInfo.fCY * 0.5f),
				long(m_tInfo.fX + m_tInfo.fCX * 0.5f),
				long(m_tInfo.fY + m_tInfo.fCY * 0.5f)};
}

void CObj::Move_Frame(void)
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

int CObj::Check_Tile()
{
	for (size_t i = 0; i < 32; ++i)
	{
		INFO tagTile = TILE->Get_TileInfo(i);
		if (tagTile.fX - tagTile.fCX * 0.5f <= m_tInfo.fX &&
			tagTile.fX + tagTile.fCX * 0.5f >= m_tInfo.fX &&
			tagTile.fY - tagTile.fCY * 0.5f <= m_tInfo.fY &&
			tagTile.fY + tagTile.fCY * 0.5f >= m_tInfo.fY)
		{
			m_iTileNum = i;
			return i;
		}
	}
	return m_iTileNum;
}

void CObj::Set_Pos(int _num)
{
	if (0 <= _num && 32 > _num)
	{
		m_tInfo.fX = TILE->Get_TileInfo(_num).fX;
		m_tInfo.fY = TILE->Get_TileInfo(_num).fY;
		m_iTileNum = _num;
	}
	else
		return;
}
