#include "stdafx.h"
#include "SpawnViolette.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "AbstractFactory.h"

CSpawnViolette::CSpawnViolette()
	:m_pMusicShot(nullptr), m_bSpawned(true), m_ePreState(STATE_END), m_eCurState(MOVE)
{
}


CSpawnViolette::~CSpawnViolette()
{
	Release();
}

void CSpawnViolette::Initialize()
{
	m_tInfo = {0.f, 450.f, 192.f, 192.f};
	m_eSpellID = SPELL_VIOLETTE;
	m_pSpellName = L"���̿÷� ��ȯ";
	BITMAP->Insert_Bmp(L"../Image/Spell/Violette.bmp", m_pSpellName);
	BITMAP->Insert_Bmp(L"../Image/Enemie/Violette/VioletteRev.bmp", L"VioletteRev");
	BITMAP->Insert_Bmp(L"../Image/Enemie/Violette/MusicMeasureShotRev.bmp", L"MusicMeasureShotRev");
	m_tSpellInfo = { 50, 30, 6, 999, 0,
		L"���̿÷��� ���ָ� �����մϴ�" };
	m_fSpeed = 5.f;

	m_tFrame.iFrameStart = 6;
	m_tFrame.iFrameEnd = 8;
	m_tFrame.iImageEnd = 8;
	m_tFrame.iMotion = 3;
	m_tFrame.iMotionEnd = 3;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 200;
	m_tFrame.dwTime = GetTickCount();

	if (!m_pMusicShot)
	{
		m_pMusicShot = new CSpellMusicShot;
		m_pMusicShot->Set_Pos(m_tInfo.fX, m_tInfo.fY);
	}
}

int CSpawnViolette::Update()
{
	if (0 >= m_tSpellInfo.iCnt)
		return OBJ_DEAD;

	Count_Trigger(50);

	if (m_bSpawned)
	{
		if (m_tInfo.fX < 132) // 24�� Ÿ�� x��
		{
			m_tInfo.fX += m_fSpeed;
		}
		else
		{
			Set_Pos(24); // 24��Ÿ�Ͽ� ����
			m_eCurState = MUSIC;
			m_bSpawned = false;
		}
		__super::Update_Rect();
		return OBJ_NOEVENT;
	}

	// ���� 1��� -1
	if (m_dwTime + 100 < GetTickCount())
	{
		SCENE->Get_Scene()->Add_Object(SPELL, CAbstractFactory<CSpellMusicShot>::Create(m_iTileNum + 1));
		--m_tSpellInfo.iCnt;
		m_dwTime = GetTickCount();
	}
	
	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CSpawnViolette::Late_Update()
{
	Motion_Change();
	__super::Move_Frame();
}

void CSpawnViolette::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(L"VioletteRev");
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX), // Ű�� ������
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 126,
		m_tFrame.iMotionCnt * 144,
		126,		// �ؽ�ó ���� ������
		144,
		RGB(1, 1, 1));
}

void CSpawnViolette::Release()
{
	Safe_Delete<CObj*>(m_pMusicShot);
}

void CSpawnViolette::Collilsion_Event(CObj * _pObj)
{
}

void CSpawnViolette::Count_Trigger(int _iTriggerCnt)
{
	if (m_iRenderCnt == 1)
	{
		m_tInfo = { 0.f, 450.f, 192.f, 192.f };
	}
	if (m_iRenderCnt)
		++m_iRenderCnt;
}

void CSpawnViolette::Player_Effect()
{
}

void CSpawnViolette::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case MOVE:
			m_tFrame.iFrameStart = 6;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iImageEnd = 8;
			m_tFrame.iMotion = 3;
			m_tFrame.iMotionEnd = 3;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;
		case MUSIC:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iImageEnd = 8;
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
