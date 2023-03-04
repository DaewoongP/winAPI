#include "stdafx.h"
#include "RhythmEffect.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"

CRhythmEffect::CRhythmEffect()
{
}


CRhythmEffect::~CRhythmEffect()
{
	Release();
}

void CRhythmEffect::Initialize()
{
	BITMAP->Insert_Bmp(L"../Image/FX/RhythmBlast.bmp", L"RhythmBlast");
	m_pFrameKey = L"RhythmBlast";

	m_iEffectCnt = 1;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.iImageEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 1;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();

	// 매개변수 이펙트 생성 안했으면 포지션 세팅 안되어있음.
	m_tInfo = SCENE->Get_Scene()->Get_Player()->Get_Info();
}

void CRhythmEffect::Update()
{

	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		--m_iEffectCnt;

	__super::Update_Rect();
}

void CRhythmEffect::Late_Update()
{
	__super::Move_Frame();
}

void CRhythmEffect::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tInfo.fX - 48),
		int(m_tInfo.fY - 48),
		96,
		96,
		hMemDC,
		m_tFrame.iFrameCnt * 60,
		m_tFrame.iMotionCnt * 60,
		60,
		60,
		RGB(1, 1, 1));
}

void CRhythmEffect::Release()
{
}
