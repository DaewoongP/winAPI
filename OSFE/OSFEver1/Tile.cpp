#include "stdafx.h"
#include "Tile.h"
#include "BitmapMgr.h"

CTile::CTile()
	:m_eState(TILE_IDLE)
{
}


CTile::~CTile()
{
	Release();
}

void CTile::Initialize()
{
	m_eObjID = TILE;
	m_tInfo.fCX = (float)TILECX;
	m_tInfo.fCY = (float)TILECY;
	m_iAttack = 0;
	BITMAP->Insert_Bmp(L"../Image/Tile/TileBlue.bmp", L"TileBlue");
	BITMAP->Insert_Bmp(L"../Image/Tile/TileRed.bmp", L"TileRed");

	BITMAP->Insert_Bmp(L"../Image/Tile/WarningDangerS.bmp", L"WarningDangerS");
	BITMAP->Insert_Bmp(L"../Image/Tile/WarningShield.bmp", L"WarningShield");
	BITMAP->Insert_Bmp(L"../Image/Tile/FireTile.bmp", L"FireTile");
	BITMAP->Insert_Bmp(L"../Image/Tile/WarningNoteS.bmp", L"WarningNoteS");
	BITMAP->Insert_Bmp(L"../Image/Tile/WarningHealS.bmp", L"WarningHealS");
	m_pFrameKey = L"TileBlue";
	m_tFrame = { 0, 6, 0, 0, 2, 0, 2, 200, GetTickCount() };
}

int CTile::Update()
{
	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CTile::Late_Update()
{
	__super::Move_Frame();
}

void CTile::Render(HDC hDC)
{
	if (m_tRect.left > TILEINIT_X + TILECX * 4)
		m_pFrameKey = L"TileRed";
	else
		m_pFrameKey = L"TileBlue";

	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		0,
		0,
		40,		// 텍스처 원본 사이즈
		25,
		RGB(255, 255, 255));
	HDC		hAttackedDC = BITMAP->Find_Img(L"WarningDangerS");
	HDC		hShieldDC = BITMAP->Find_Img(L"WarningShield");
	HDC		hFireDC = BITMAP->Find_Img(L"FireTile");
	HDC		hMusicDC = BITMAP->Find_Img(L"WarningNoteS");
	HDC		hHealDC = BITMAP->Find_Img(L"WarningHealS");
	switch (m_eState)
	{
	case TILE_ATTACKED:
		GdiTransparentBlt(hDC,
			int(m_tRect.left) - 88,
			int(m_tRect.top) - 100,
			240, // 키울 사이즈
			240,
			hAttackedDC,
			m_tFrame.iFrameCnt * 120,
			m_tFrame.iMotionCnt * 120,
			120,		// 텍스처 원본 사이즈
			120,
			RGB(1, 1, 1));
		break;
	case TILE_CRACKED:
		break;
	case TILE_BROKEN:
		break;
	case TILE_FIRE:
		break;
	case TILE_SHIELD:
		GdiTransparentBlt(hDC,
			int(m_tRect.left),
			int(m_tRect.top),
			int(m_tInfo.fCX), // 키울 사이즈
			int(m_tInfo.fCY),
			hShieldDC,
			0,
			0,
			40,		// 텍스처 원본 사이즈
			25,
			RGB(1, 1, 1));
		break;
	case TILE_MUSIC:
		GdiTransparentBlt(hDC,
			int(m_tRect.left) - 88,
			int(m_tRect.top) - 100,
			240, // 키울 사이즈
			240,
			hMusicDC,
			m_tFrame.iFrameCnt * 120,
			m_tFrame.iMotionCnt * 120,
			120,		// 텍스처 원본 사이즈
			120,
			RGB(1, 1, 1));
		break;
	case TILE_HEAL:
		GdiTransparentBlt(hDC,
			int(m_tRect.left) - 88,
			int(m_tRect.top) - 100,
			240, // 키울 사이즈
			240,
			hHealDC,
			m_tFrame.iFrameCnt * 120,
			m_tFrame.iMotionCnt * 120,
			120,		// 텍스처 원본 사이즈
			120,
			RGB(1, 1, 1));
		break;
	case TILE_END:
		break;
	default:
		break;
	}
}

void CTile::Release()
{
}

void CTile::Collilsion_Event(CObj* _pObj)
{
}

void CTile::Count_Trigger(int _iTriggerCnt)
{
}
