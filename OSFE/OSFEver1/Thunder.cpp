#include "stdafx.h"
#include "Thunder.h"
#include "BitmapMgr.h"
#include "SoundMgr.h"
#include "SceneMgr.h"
#include "TileMgr.h"

CThunder::CThunder()
{
}


CThunder::~CThunder()
{
}

void CThunder::Initialize()
{
	m_eSpellID = SPELL_THUNDER;
	m_pSpellName = L"번개";
	m_tInfo.fCX = 224.f;
	m_tInfo.fCY = 736.f;
	m_tSpellInfo = { 100, 0, 1, 30, 0,
		L"4만큼 떨어진 곳에 번개를 내려칩니다" };
	BITMAP->Insert_Bmp(L"../Image/Spell/ThunderS.bmp", L"ThunderS");
	BITMAP->Insert_Bmp(L"../Image/Spell/Thunder.bmp", m_pSpellName);

	m_pFrameKey = L"ThunderS";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.iImageEnd = 2;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 1;

	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;

	m_tFrame.dwSpeed = 180;
	m_tFrame.dwTime = GetTickCount();
	Check_Tile();
	bOnce = true;
}

int CThunder::Update()
{
	if (bOnce)
	{
		m_iTileNum += 4;
		m_tInfo.fX = dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum))->Get_TileInfo().fX;
		m_tInfo.fY = dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum))->Get_TileInfo().fY;
		SOUND->PlaySound(L"thunderclap.wav", SOUND_EFFECT17, 0.3f);
		SCENE->Get_Scene()->Set_Hit(20.f);
		bOnce = false;
	}
	if (0 > m_tSpellInfo.iCnt)
		return OBJ_DEAD;
	if (m_bDead)
		return OBJ_DEAD;

	Count_Trigger(60);
	
	__super::Update_Rect();
	
	return OBJ_NOEVENT;
}

void CThunder::Late_Update()
{
	__super::Move_Frame();
}

void CThunder::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 140,
		m_tFrame.iMotionCnt * 460,
		140,		// 텍스처 원본 사이즈
		460,
		RGB(1, 1, 1));
}

void CThunder::Release()
{
}

void CThunder::Collilsion_Event(CObj * _pObj)
{
	// 어택 0이되면 충돌처리가 안되기때문에.
	m_tSpellInfo.iAtk = 0;
}

void CThunder::Count_Trigger(int _iTriggerCnt)
{
	if (m_iRenderCnt == _iTriggerCnt)
	{
		this->Set_Dead();
		
		m_iRenderCnt = 0;
	}

	if (m_iRenderCnt)
		++m_iRenderCnt;
}

void CThunder::Player_Effect()
{
}
