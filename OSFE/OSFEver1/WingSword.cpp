#include "stdafx.h"
#include "WingSword.h"
#include "TileMgr.h"
#include "BitmapMgr.h"
#include "SoundMgr.h"
#include "SceneMgr.h"

CWingSword::CWingSword()
{
}


CWingSword::~CWingSword()
{
	Release();
}

void CWingSword::Initialize()
{
	m_tInfo.fCX = 38.4f;
	m_tInfo.fCY = 337.6f;
	m_eObjID = ENEMIE_SPELL;
	m_iAttack = 100;
	m_pFrameKey = L"WingSwords";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 8;
	m_tFrame.iImageEnd = 8;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 0;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 120;
	m_tFrame.dwTime = GetTickCount();
}

int CWingSword::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fX = SCENE->Get_Scene()->Get_ObjList(ENEMIE).front()->Get_Info().fX;
	m_tInfo.fY = SCENE->Get_Scene()->Get_ObjList(ENEMIE).front()->Get_Info().fY;

	Check_Tile();

	if (m_iTileNum != m_iPreTileNum)
	{
		m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum)));
		if (m_iTileNum > 8)
			m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum - 8)));
		if (m_iTileNum > 16)
			m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum - 16)));
		if (m_iTileNum < 24)
			m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + 8)));
		if (m_iTileNum < 16)
			m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + 16)));
		m_iPreTileNum = m_iTileNum;
	}
	else
		m_AttackTileList.clear();

	Attack();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CWingSword::Late_Update()
{
	__super::Move_Frame();
}

void CWingSword::Render(HDC hDC)
{
	if (m_bRenderSkip)
	{
		m_bRenderSkip = false;
		return;
	}
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 24,
		m_tFrame.iMotionCnt * 211,
		24,		// 텍스처 원본 사이즈
		211,
		RGB(1, 1, 1));
}

void CWingSword::Release()
{
}

void CWingSword::Attack()
{
}

void CWingSword::Motion_Change()
{
}

void CWingSword::Collilsion_Event(CObj * _pObj)
{
}

void CWingSword::Count_Trigger(int _iTriggerCnt)
{

	if (m_iRenderCnt == 1)
	{
	}

	if (m_iRenderCnt)
		++m_iRenderCnt;

}
