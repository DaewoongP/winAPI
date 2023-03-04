#include "stdafx.h"
#include "SpeakerSpawn.h"
#include "TileMgr.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "EffectMgr.h"
#include "AbstractFactory.h"

CSpeakerSpawn::CSpeakerSpawn()
{
}


CSpeakerSpawn::~CSpeakerSpawn()
{
	Release();
}

void CSpeakerSpawn::Initialize()
{
	// 30 x 18
	m_tInfo.fCX = 56.f;
	m_tInfo.fCY = 28.8f;
	// 비트맵은미리 대입 (여러번 적용을 피하기 위해서.)
	m_pFrameKey = L"DiagBeamShot";
	m_eObjID = ENEMIE_SPELL;
	m_iAttack = 100;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iImageEnd = 1;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 2;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();
	m_fSpeed = 5.f;
	m_SpeakerInfo = SCENE->Get_Scene()->Get_ObjList(ENEMIE).front()->Get_Info();
}

int CSpeakerSpawn::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Check_Tile();
	Attack();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CSpeakerSpawn::Late_Update()
{
	if (m_tRect.top <= m_SpeakerInfo.fY && m_tRect.bottom >= m_SpeakerInfo.fY &&
		m_tRect.left <= m_SpeakerInfo.fX && m_tRect.right >= m_SpeakerInfo.fX)
	{
		static_cast<CTile*>(TILE->Get_Tile(m_iTileNum))->Set_TileState(TILE_IDLE);

		if (SCENE->Get_Scene()->Get_Player()->Check_Tile() == m_iTileNum)
			m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum)));
		else
			SCENE->Get_Scene()->Add_Object(ENEMIE, CAbstractFactory<CSpeaker>::Create(m_SpeakerInfo.fX, m_SpeakerInfo.fY));
		m_bDead = true;
	}

	__super::Move_Frame();
}

void CSpeakerSpawn::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_SpeakerInfo.fX - m_tInfo.fCX * 0.5f),
		int(m_SpeakerInfo.fY - m_tInfo.fCY * 0.5f),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 30,
		m_tFrame.iMotionCnt * 18,
		30,		// 텍스처 원본 사이즈
		18,
		RGB(1, 1, 1));
}

void CSpeakerSpawn::Release()
{
	static_cast<CViolette*>(SCENE->Get_Scene()->Get_ObjList(ENEMIE).front())->Set_Pattern(false);
}

void CSpeakerSpawn::Attack()
{
	m_AttackTileList.clear();
	static_cast<CTile*>(TILE->Get_Tile(m_iTileNum))->Set_TileState(TILE_ATTACKED);

	if (m_tInfo.fY > m_SpeakerInfo.fY)
		m_SpeakerInfo.fY += 4.f;
	else
		m_SpeakerInfo.fY -= 4.f;

	if (m_tInfo.fX > m_SpeakerInfo.fX)
		m_SpeakerInfo.fX += 5.f;
	else
		m_SpeakerInfo.fX -= 5.f;
}

void CSpeakerSpawn::Count_Trigger(int _iTriggerCnt)
{
}
