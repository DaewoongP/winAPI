#include "stdafx.h"
#include "LightBallRain.h"
#include "SoundMgr.h"
#include "BitmapMgr.h"
#include "TileMgr.h"
#include "AbstractFactory.h"
#include "SceneMgr.h"

CLightBallRain::CLightBallRain()
{
}


CLightBallRain::~CLightBallRain()
{
	Release();
}

void CLightBallRain::Initialize()
{
	m_tInfo.fCX = 102.4f;
	m_tInfo.fCY = 307.2f;
	// 64 x 192
	m_eObjID = ENEMIE_SPELL;
	m_iAttack = 100;
	m_pFrameKey = L"LightBallRain";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iImageEnd = 2;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 1;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 180;
	m_tFrame.dwTime = GetTickCount();
	SOUND->PlaySound(L"trinity_shine.wav", SOUND_EFFECT24, 0.3f);
}

int CLightBallRain::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Count_Trigger(50);

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CLightBallRain::Late_Update()
{
	__super::Move_Frame();
}

void CLightBallRain::Render(HDC hDC)
{
	if (m_iRenderCnt >= 10)
	{
		HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
		GdiTransparentBlt(hDC,
			int(m_tRect.left),
			int(m_tRect.top) - 140,
			int(m_tInfo.fCX), // 키울 사이즈
			int(m_tInfo.fCY),
			hMemDC,
			m_tFrame.iFrameCnt * 64,
			m_tFrame.iMotionCnt * 192,
			64,		// 텍스처 원본 사이즈
			192,
			RGB(1, 1, 1));
	}
}

void CLightBallRain::Release()
{
}

void CLightBallRain::Attack()
{
}

void CLightBallRain::Motion_Change()
{
}

void CLightBallRain::Collilsion_Event(CObj * _pObj)
{
	this->Set_Dead();
}

void CLightBallRain::Count_Trigger(int _iTriggerCnt)
{
	if (m_iRenderCnt == 1)
	{
		m_AttackTileList.clear();
		static_cast<CTile*>(TILE->Get_Tile(m_iTileNum))->Set_TileState(TILE_ATTACKED);
	}
	if (m_iRenderCnt == 10)
	{
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 4;
		m_tFrame.iImageEnd = 2;
		m_tFrame.iMotion = 0;
		m_tFrame.iMotionEnd = 1;
		m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
		m_tFrame.iMotionCnt = m_tFrame.iMotion;
		m_tFrame.dwSpeed = 180;
		m_tFrame.dwTime = GetTickCount();
	}
	if (m_iRenderCnt == _iTriggerCnt)
	{
		m_AttackTileList.push_back(dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum)));
		static_cast<CTile*>(TILE->Get_Tile(m_iTileNum))->Set_TileState(TILE_IDLE);
	}
	if (m_iRenderCnt == _iTriggerCnt + 1)
	{
		SCENE->Get_Scene()->Add_Object(ENEMIE, CAbstractFactory<CBombLight>::Create(m_iTileNum));
		this->Set_Dead();
	}
	
	if (m_iRenderCnt)
		++m_iRenderCnt;

}
