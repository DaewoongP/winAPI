#include "stdafx.h"
#include "LightSwordShot.h"
#include "BitmapMgr.h"
#include "TileMgr.h"
#include "SoundMgr.h"

CLightSwordShot::CLightSwordShot()
{
}


CLightSwordShot::~CLightSwordShot()
{
	Release();
}

void CLightSwordShot::Initialize()
{
	m_tInfo.fCX = 76.8f;
	m_tInfo.fCY = 32.f;
	m_fSpeed = 10.f;
	// 48 x 20
	m_eObjID = ENEMIE_SPELL;
	m_iAttack = 100;
	m_pFrameKey = L"LightSwordShot";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iImageEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 4;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();
	SOUND->PlaySound(L"slash_light.wav", SOUND_EFFECT23, 0.2f);
}

int CLightSwordShot::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Check_Tile();

	if (m_iTileNum != m_iPreTileNum)
	{
		m_AttackTileList.push_back(dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum)));
		m_iPreTileNum = m_iTileNum;
	}
	else
		m_AttackTileList.clear();
		
	Count_Trigger(10);
	
	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CLightSwordShot::Late_Update()
{
	__super::Move_Frame();
}

void CLightSwordShot::Render(HDC hDC)
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
		m_tFrame.iFrameCnt * 48,
		m_tFrame.iMotionCnt * 20,
		48,		// 텍스처 원본 사이즈
		20,
		RGB(1, 1, 1));
}

void CLightSwordShot::Release()
{
}

void CLightSwordShot::Attack()
{
}

void CLightSwordShot::Motion_Change()
{
}

void CLightSwordShot::Collilsion_Event(CObj * _pObj)
{
	this->Set_Dead();
}

void CLightSwordShot::Count_Trigger(int _iTriggerCnt)
{
	if (m_iRenderCnt)
		++m_iRenderCnt;

	if (m_iRenderCnt == 1)
	{
		// 첫 생성 위치
		m_tInfo = dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum))->Get_Info();
		m_tInfo.fX += TILECX * 0.5f;
	}

	if (m_iRenderCnt >= _iTriggerCnt)
	{
		m_tInfo.fX -= m_fSpeed;
	}
}
