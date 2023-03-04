#include "stdafx.h"
#include "Ragnarok.h"
#include "BitmapMgr.h"
#include "SoundMgr.h"
#include "SceneMgr.h"
#include "TileMgr.h"

CRagnarok::CRagnarok()
{
}


CRagnarok::~CRagnarok()
{
	Release();
}

void CRagnarok::Initialize()
{
	m_eSpellID = SPELL_RAGNAROK;
	m_pSpellName = L"라그나로크";
	m_tInfo.fCX = 864.f;
	m_tInfo.fCY = 576.f;
	m_tSpellInfo = { 200, 0, 4, 30, 0,
		L"4만큼 떨어진 곳에 거대한 칼을 소환합니다" };
	BITMAP->Insert_Bmp(L"../Image/Spell/RagnarokStorm.bmp", L"RagnarokStorm");
	BITMAP->Insert_Bmp(L"../Image/Spell/Ragnarok.bmp", m_pSpellName);

	m_pFrameKey = L"RagnarokStorm";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 11;
	m_tFrame.iImageEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 2;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 120;
	m_tFrame.dwTime = GetTickCount();
	Check_Tile();
}

int CRagnarok::Update()
{
	if (0 > m_tSpellInfo.iCnt)
		return OBJ_DEAD;

	Count_Trigger(50);
	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CRagnarok::Late_Update()
{
	__super::Move_Frame();
}

void CRagnarok::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 562,
		m_tFrame.iMotionCnt * 360,
		562,		// 텍스처 원본 사이즈
		360,
		RGB(1, 1, 1));
}

void CRagnarok::Release()
{
}

void CRagnarok::Collilsion_Event(CObj * _pObj)
{
}

void CRagnarok::Count_Trigger(int _iTriggerCnt)
{
	if (m_iRenderCnt == 1)
	{
		m_iTileNum += 4;
		m_tInfo.fX = dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum))->Get_TileInfo().fX;
		m_tInfo.fY = dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum))->Get_TileInfo().fY;
		SOUND->PlaySound(L"explosion_artillery_25ms.wav", SOUND_EFFECT17, 0.3f);
	}

	if (m_iRenderCnt == 35)
		SCENE->Get_Scene()->Set_Hit(20.f);

	if (m_iRenderCnt == _iTriggerCnt)
	{
		m_tSpellInfo.iCnt = -1;
		m_tSpellInfo.iAtk = 200;

		m_iRenderCnt = 0;
	}
	else
		m_tSpellInfo.iAtk = 0;

	if (m_iRenderCnt)
		++m_iRenderCnt;
}

void CRagnarok::Player_Effect()
{
}
