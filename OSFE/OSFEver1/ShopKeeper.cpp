#include "stdafx.h"
#include "ShopKeeper.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "AbstractFactory.h"

CShopKeeper::CShopKeeper()
{
}


CShopKeeper::~CShopKeeper()
{
	Release();
}

void CShopKeeper::Initialize()
{
	// 160 x 160
	m_tInfo.fCX = 256.f;
	m_tInfo.fCY = 256.f;
	m_fSpeed = 10.f;
	//m_iAttack = 10;
	m_eObjID = SHOP;
	m_eCurState = IDLE;
	m_iTileNum = Check_Tile();
	m_iPreTileNum = m_iTileNum;

	m_tCreature = { 999999, 0, 50, 20 };
	BITMAP->Insert_Bmp(L"../Image/Enemie/Shopkeeper.bmp", L"Shopkeeper");

	m_pFrameKey = L"Shopkeeper";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 10;
	m_tFrame.iImageEnd = 7;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 1;

	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;

	m_tFrame.dwSpeed = 200;
	m_tFrame.dwTime = GetTickCount();
}

int CShopKeeper::Update()
{
	if (m_tCreature.iHp <= 0)
		return OBJ_DEAD;

	ShopKeeper_Position();
	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CShopKeeper::Late_Update()
{
	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		m_eCurState = IDLE;

	Motion_Change();
	
	__super::Move_Frame();
}

void CShopKeeper::Render(HDC hDC)
{
	CCreature::Render(hDC);

	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		(7 - m_tFrame.iFrameCnt) * 160,
		m_tFrame.iMotionCnt * 160,
		160,		// 텍스처 원본 사이즈
		160,
		RGB(1, 1, 1));
}

void CShopKeeper::Release()
{
}

void CShopKeeper::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 10;
			m_tFrame.iImageEnd = 7;
			m_tFrame.iMotion = 0;
			m_tFrame.iMotionEnd = 1;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;

		case SHOP_OPEN:
			m_tFrame.iFrameStart = 3;
			m_tFrame.iFrameEnd = 10;
			m_tFrame.iImageEnd = 7;
			m_tFrame.iMotion = 1;
			m_tFrame.iMotionEnd = 2;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			break;
		case SHOP_CLOSE:
			m_tFrame.iFrameStart = 4;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iImageEnd = 7;
			m_tFrame.iMotion = 5;
			m_tFrame.iMotionEnd = 5;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CShopKeeper::ShopKeeper_Position()
{
	/*Check_Tile();
	if (m_eCurState == SHOP_OPEN)
	{
		if (m_tInfo.fX > SCENE->Get_Scene()->Get_Player()->Get_Info().fX)
		{
			m_tInfo.fX -= 20.f;
		}
		
		if (m_tInfo.fY > SCENE->Get_Scene()->Get_Player()->Get_Info().fY)
		{
			m_tInfo.fY -= 10.f;
		}
		else if (m_tInfo.fY < SCENE->Get_Scene()->Get_Player()->Get_Info().fY)
		{
			m_tInfo.fY += 10.f;
		}
		else if (m_iTileNum == SCENE->Get_Scene()->Get_Player()->Get_TileNum() - 1)
		{
			Set_Pos(m_iTileNum);
			m_eCurState = IDLE;
		}
	}
	else if (m_eCurState == SHOP_CLOSE)
	{
		if (Get_TileNum() == m_iPreTileNum % 8)
		{
			m_tInfo.fX += 20.f;
		}
	}*/
}

void CShopKeeper::Collilsion_Event(CObj * _pObj)
{
}

void CShopKeeper::Count_Trigger(int _iTriggerCnt)
{
}
