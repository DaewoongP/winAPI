#include "stdafx.h"
#include "DogPet.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "EffectMgr.h"
#include "AbstractFactory.h"
#include "SoundMgr.h"

CDogPet::CDogPet()
	:m_bAttack(false)
{
	ZeroMemory(&m_tTargetInfo, sizeof(INFO));
}


CDogPet::~CDogPet()
{
	Release();
}

void CDogPet::Initialize()
{
	// 120 x 115
	m_tInfo.fCX = 192.f;
	m_tInfo.fCY = 184.f;
	m_eObjID = PET;
	BITMAP->Insert_Bmp(L"../Image/Pet/Dog.bmp", L"Dog");
	m_pFrameKey = L"Dog";
	m_ePreState = STATE_END;
	m_eCurState = IDLE;
	m_fSpeed = 15.f;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 0;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.iImageEnd = 7;
	m_tFrame.dwSpeed = 150;
	m_tFrame.dwTime = GetTickCount();

	m_iTileNum = Check_Tile();
	m_iPreTileNum = m_iTileNum;
	m_tCreature = { 800, 0, 0, 0 };
}

int CDogPet::Update()
{
	if (m_tCreature.iHp <= 0)
	{
		m_eCurState = DEAD;
		m_tCreature.iHp = 0;
		Reset_DamageFont();
		Motion_Change();

		if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
			return OBJ_DEAD;
		else
			return OBJ_NOEVENT;
	}
	if(!m_bAttack)
		Set_Pos(static_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Get_PreTileNum());
	//m_tInfo.fX = static_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Get_Info().fX;
	//m_tInfo.fY = static_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Get_Info().fY;
	Reset_DamageFont();
	Attack();

	Motion_Change();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CDogPet::Late_Update()
{
	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		m_eCurState = IDLE;
	__super::Move_Frame();
}

void CDogPet::Render(HDC hDC)
{
	CCreature::Render(hDC);
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 120,
		m_tFrame.iMotionCnt * 115,
		120,		// 텍스처 원본 사이즈
		115,
		RGB(1, 1, 1));

	for (auto& iter : m_DamageList)
	{
		iter->Render_Damage(hDC);
	}
}

void CDogPet::Release()
{
}

void CDogPet::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotion = 0;
			m_tFrame.iMotionEnd = 0;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.iImageEnd = 7;
			m_tFrame.dwSpeed = 150;
			m_tFrame.dwTime = GetTickCount();
			break;

		case ATTACK:
			m_tFrame.iFrameStart = 4;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotion = 1;
			m_tFrame.iMotionEnd = 2;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.iImageEnd = 7;
			m_tFrame.dwSpeed = 150;
			m_tFrame.dwTime = GetTickCount();
			break;

		case DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 10;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotion = 4;
			m_tFrame.iMotionEnd = 5;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.iImageEnd = 7;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CDogPet::Collilsion_Event(CObj * _pObj)
{
	if (_pObj->Get_OBJID() == ENEMIE || _pObj->Get_OBJID() == ENEMIE_SPELL)
	{
		m_tCreature.iHp -= dynamic_cast<CEnemie*>(_pObj)->Get_Attack();
		m_DamageList.push_back(CAbstractFactory<CDamageUI>::Create_DamageUI(m_tInfo.fX - 50, m_tInfo.fY - 100 - m_iDamageY, static_cast<CEnemie*>(_pObj)->Get_Attack()));
		Set_DamageY(20);
		SOUND->PlaySound(L"enemy_hit_light.wav", SOUND_EFFECT12, 0.3f);
		EFFECT->Add_Effect(EFFECT_HIT, CAbstractFactory<CHitEffect>::Create_Effect(m_tInfo.fX, m_tInfo.fY));
	}
}

void CDogPet::Count_Trigger(int _iTriggerCnt)
{
}

void CDogPet::Attack()
{
	if (m_dwTime + 10000 < GetTickCount())
	{
		m_eCurState = ATTACK;
		m_bAttack = true;

		m_dwTime = GetTickCount();
	}

	if (m_bAttack && !SCENE->Get_Scene()->Get_ObjList(ENEMIE).empty())
	{
		m_iTargetTile = SCENE->Get_Scene()->Get_ObjList(ENEMIE).front()->Get_TileNum();
		Check_Tile();
		Angle();

		m_tInfo.fX += abs(m_fWidth) / m_fSpeed;
		m_tInfo.fY -= m_fHeight / m_fSpeed;

		if (m_iTargetTile == m_iTileNum)
		{
			SOUND->PlaySound(L"bark_high.wav", SOUND_EFFECT25, 0.3f);
			static_cast<CCreature*>(SCENE->Get_Scene()->Get_ObjList(ENEMIE).front())->Set_Damage(100);
			Set_Pos(m_iTileNum);
			m_bAttack = false;
			m_eCurState = IDLE;
			return;
		}
	}
	else if (m_bAttack == false && !SCENE->Get_Scene()->Get_ObjList(ENEMIE).empty())
	{
		Set_Pos(static_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Get_PreTileNum());
	}
	if (SCENE->Get_Scene()->Get_ObjList(ENEMIE).empty())
		Set_Pos(static_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Get_PreTileNum());
}

void CDogPet::Angle()
{
	m_tTargetInfo = TILE->Get_TileInfo(m_iTargetTile);

	m_fWidth = m_tInfo.fX - m_tTargetInfo.fX;
	m_fHeight = m_tInfo.fY - m_tTargetInfo.fY;

	//float	fDiagonal = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight);

}

