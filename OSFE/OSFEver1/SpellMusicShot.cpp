#include "stdafx.h"
#include "SpellMusicShot.h"
#include "TileMgr.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "EffectMgr.h"
#include "AbstractFactory.h"
#include "SoundMgr.h"

int CSpellMusicShot::m_iSoundCnt = 0;

CSpellMusicShot::CSpellMusicShot()
	:m_fWidth(0.f), m_fHeight(0.f)
{
	++m_iSoundCnt;
	ZeroMemory(&m_tTargetInfo, sizeof(INFO));
}


CSpellMusicShot::~CSpellMusicShot()
{
	Release();
}

void CSpellMusicShot::Initialize()
{
	m_eObjID = SPELL;
	m_tInfo.fCX = 56.f;
	m_tInfo.fCY = 28.8f;
	m_fSpeed = 20.f;

	m_tSpellInfo.iAtk = 50;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 9;
	m_tFrame.iImageEnd = 2;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 3;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();

	m_pFrameKey = L"MusicMeasureShotRev";
}

int CSpellMusicShot::Update()
{

	Count_Trigger(1);

	if (m_bDead)
		return OBJ_DEAD;

	Check_Tile();

	Target_Angle();

	Attack();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CSpellMusicShot::Late_Update()
{
	__super::Move_Frame();
}

void CSpellMusicShot::Render(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 35,
		m_tFrame.iMotionCnt * 18,
		35,		// 텍스처 원본 사이즈
		18,
		RGB(1, 1, 1));
}

void CSpellMusicShot::Release()
{
	Select_Music();
	EFFECT->Add_Effect(EFFECT_SPELL, CAbstractFactory<CMusicEffect>::Create_Effect(m_tInfo.fX + TILECX * 0.5f, m_tInfo.fY));
}

void CSpellMusicShot::Collilsion_Event(CObj * _pObj)
{
}

void CSpellMusicShot::Count_Trigger(int _iTriggerCnt)
{
	if (m_iRenderCnt == _iTriggerCnt)
	{
		if (SCENE->Get_Scene()->Get_ObjList(ENEMIE).empty())
			this->Set_Dead();
		else
		{
			m_iTargetTile = SCENE->Get_Scene()->Get_ObjList(ENEMIE).front()->Get_TileNum();
		}
	}
}

void CSpellMusicShot::Player_Effect()
{
}

void CSpellMusicShot::Select_Music()
{
	if (m_iSoundCnt % 2)
		return;
	switch (rand() * m_iSoundCnt % 7)
	{
	case 0:
		SOUND->PlaySound(L"violin_shot_01.wav", SOUND_EFFECT02, 0.2f);
		break;
	case 1:
		SOUND->PlaySound(L"violin_shot_02.wav", SOUND_EFFECT02, 0.2f);
		break;
	case 2:
		SOUND->PlaySound(L"violin_shot_03.wav", SOUND_EFFECT02, 0.2f);
		break;
	case 3:
		SOUND->PlaySound(L"violin_shot_04.wav", SOUND_EFFECT02, 0.2f);
		break;
	case 4:
		SOUND->PlaySound(L"violin_shot_05.wav", SOUND_EFFECT02, 0.2f);
		break;
	case 5:
		SOUND->PlaySound(L"violin_shot_06.wav", SOUND_EFFECT02, 0.2f);
		break;
	case 6:
		SOUND->PlaySound(L"violin_shot_07.wav", SOUND_EFFECT02, 0.2f);
		break;
	}
	
}

void CSpellMusicShot::Target_Angle()
{
	m_tTargetInfo = TILE->Get_TileInfo(m_iTargetTile);

	m_fWidth = m_tInfo.fX - m_tTargetInfo.fX;
	m_fHeight = m_tInfo.fY - m_tTargetInfo.fY;

	float	fDiagonal = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight);

}

void CSpellMusicShot::Attack()
{
	m_tInfo.fX += abs(m_fWidth) / m_fSpeed;
	m_tInfo.fY -= m_fHeight / m_fSpeed;

	if (m_iTileNum == m_iTargetTile)
		m_bDead = true;
}
