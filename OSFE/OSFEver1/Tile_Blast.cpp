#include "stdafx.h"
#include "Tile_Blast.h"
#include "TileMgr.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

CTile_Blast::CTile_Blast()
{
}


CTile_Blast::~CTile_Blast()
{
	Release();
}

void CTile_Blast::Initialize()
{
	m_tInfo.fCX = 64.f;
	m_tInfo.fCY = 192.f;
	
	m_eObjID = ENEMIE_SPELL;
	m_iAttack = 100;
	m_pFrameKey = L"LightTileBlast";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 8;
	m_tFrame.iImageEnd = 8;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 0;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();
}

int CTile_Blast::Update()
{
	if (m_bDead)
		return OBJ_DEAD;
	
	Count_Trigger(70);

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CTile_Blast::Late_Update()
{
	Motion_Change();
	if (m_iRenderCnt >= 70)
	{
		__super::Move_Frame();
	}
}

void CTile_Blast::Render(HDC hDC)
{
	if (m_iRenderCnt >= 70)
	{
		HDC		hBlastDC = BITMAP->Find_Img(L"LightTileBlast");

		GdiTransparentBlt(hDC,
			m_tRect.left,
			m_tRect.top,
			(int)m_tInfo.fCX, // 키울 사이즈
			(int)m_tInfo.fCY,
			hBlastDC,
			m_tFrame.iFrameCnt * 40,
			m_tFrame.iMotionCnt * 120,
			40,		// 텍스처 원본 사이즈
			120,
			RGB(1, 1, 1));
	}
}

void CTile_Blast::Release()
{
}

void CTile_Blast::Attack()
{
}

void CTile_Blast::Motion_Change()
{
}

void CTile_Blast::Collilsion_Event(CObj * _pObj)
{
}

void CTile_Blast::Count_Trigger(int _iTriggerCnt)
{
	if (m_iRenderCnt)
		++m_iRenderCnt;

	if (m_iRenderCnt == 1)
	{
		m_tInfo = dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum))->Get_Info();
	}

	if (m_iRenderCnt == _iTriggerCnt)
	{
		m_AttackTileList.push_back(dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum)));
	}
	if (m_iRenderCnt == _iTriggerCnt + 1)
	{
		m_AttackTileList.clear();
		SOUND->PlaySound(L"mark_02.wav", SOUND_EFFECT22, 0.3f);
	}
	if (m_iRenderCnt == 90)
	{
		m_bDead = true;
	}
	
}
