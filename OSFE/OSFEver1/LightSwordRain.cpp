#include "stdafx.h"
#include "LightSwordRain.h"
#include "SoundMgr.h"
#include "BitmapMgr.h"
#include "TileMgr.h"
#include "EffectMgr.h"
#include "AbstractFactory.h"
#include "SceneMgr.h"

CLightSwordRain::CLightSwordRain()
	:m_fAngle(0.f)
{
	ZeroMemory(&m_tTargetInfo, sizeof(INFO));
}


CLightSwordRain::~CLightSwordRain()
{
	Release();
}

void CLightSwordRain::Initialize()
{
	//원본의 * 8 / 5
	m_tInfo.fCX = 77.f;
	m_tInfo.fCY = 77.f;
	m_fSpeed = 5.f;
	// 48 x 20
	m_eObjID = ENEMIE_SPELL;
	m_iAttack = 100;
	m_pFrameKey = L"LightSwordRain";
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
	if (SCENE->Get_SceneID() == SC_GATE)
		m_iAttack = 999999;
}

int CLightSwordRain::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Check_Tile();

	Target_Angle();
	Rotate_Sword();

	Count_Trigger(22);

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CLightSwordRain::Late_Update()
{
	__super::Move_Frame();
}

void CLightSwordRain::Render(HDC hDC)
{
	//77 32
	HDC		hPlgDC = BITMAP->Find_Img(L"Plg");
	HDC		hResetDC = BITMAP->Find_Img(L"Reset");
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);

	// 비트맵을 회전시키는 함수
	PlgBlt(hPlgDC, // 회전 시킬 이미지를 어디에 그릴 것인가
		m_tPoint,// 그림을 그릴 사각형의 좌표 POINT 변수, 순서로 좌상단, 우상단, 좌하단
		hMemDC, // 어디에 있는 이미지를 그릴 것인가
		m_tFrame.iFrameCnt * 77,
		m_tFrame.iMotionCnt * 77, // 그림을 그릴 이미지의 X,Y 시작 좌표
		77,
		77, // 그림을 그릴 이미지의 가로와 세로의 길이를 전달
		NULL,
		NULL,
		NULL);
	

	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		77, // 키울 사이즈
		77,
		hPlgDC,
		0,
		0,
		77,	// 텍스처 원본 사이즈
		77,
		RGB(1, 1, 1));

	BitBlt(hPlgDC, 
		0,
		0,
		77,
		77,
		hResetDC, 0, 0, SRCCOPY);
}

void CLightSwordRain::Release()
{
	EFFECT->Add_Effect(EFFECT_TILE, CAbstractFactory<CLightSwordEffect>::Create_Effect(m_tTargetInfo.fX, m_tTargetInfo.fY));
	if (TILE->Get_Tile(m_iTargetTile))
		static_cast<CTile*>(TILE->Get_Tile(m_iTargetTile))->Set_TileState(TILE_IDLE);
}

void CLightSwordRain::Attack()
{
	m_tInfo.fX -= abs(m_fWidth) / m_fSpeed;
	m_tInfo.fY -= m_fHeight / m_fSpeed;

	if (m_iTileNum == m_iTargetTile)
		m_bDead = true;
}

void CLightSwordRain::Motion_Change()
{
}

void CLightSwordRain::Collilsion_Event(CObj * _pObj)
{
}

void CLightSwordRain::Count_Trigger(int _iTriggerCnt)
{
	if (m_iRenderCnt == 1)
	{
		static_cast<CTile*>(TILE->Get_Tile(m_iTargetTile))->Set_TileState(TILE_ATTACKED);
	}
	if (m_iRenderCnt >= _iTriggerCnt)
		Attack();
	if (m_iRenderCnt == _iTriggerCnt + 10)
	{
		static_cast<CTile*>(TILE->Get_Tile(m_iTargetTile))->Set_TileState(TILE_IDLE);
		m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTargetTile)));
	}
	if (m_iRenderCnt == _iTriggerCnt + 11)
	{
		m_AttackTileList.clear();
	}


	if (m_iRenderCnt)
		++m_iRenderCnt;
}

void CLightSwordRain::Rotate_Sword()
{
	float	fDiagonal = sqrtf(
		((m_tInfo.fCX / 2.f) * (m_tInfo.fCX / 2.f))
		+ ((m_tInfo.fCY / 2.f) * (m_tInfo.fCY / 2.f)));

	// 좌 상단
	m_tPoint[0].x = long((m_tInfo.fCX / 2.f) + fDiagonal * cosf((m_fAngle + 135.f) * PI / 180.f));
	m_tPoint[0].y = long((m_tInfo.fCY / 2.f) - fDiagonal * sinf((m_fAngle + 135.f) * PI / 180.f));

	// 우 상단		
	m_tPoint[1].x = long((m_tInfo.fCX / 2.f) + fDiagonal * cosf((m_fAngle + 45.f) * PI / 180.f));
	m_tPoint[1].y = long((m_tInfo.fCY / 2.f) - fDiagonal * sinf((m_fAngle + 45.f) * PI / 180.f));

	// 좌 하단		
	m_tPoint[2].x = long((m_tInfo.fCX / 2.f) + fDiagonal * cosf((m_fAngle + 225.f) * PI / 180.f));
	m_tPoint[2].y = long((m_tInfo.fCY / 2.f) - fDiagonal * sinf((m_fAngle + 225.f) * PI / 180.f));
}

void CLightSwordRain::Target_Angle()
{
	m_tTargetInfo = TILE->Get_TileInfo(m_iTargetTile);

	float fWidth = m_tInfo.fX - m_tTargetInfo.fX;
	float fHeight = m_tInfo.fY - m_tTargetInfo.fY;

	float	fDiagonal = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight);

	if (0 < m_fHeight)
	{
		//180 - 세타
		m_fAngle = 180.f - acosf(m_fWidth / fDiagonal) * 180.f / PI;
	}
	else
	{
		m_fAngle = 180.f + acosf(m_fWidth / fDiagonal) * 180.f / PI;
	}
	m_fWidth = fWidth;
	m_fHeight = fHeight;
}
