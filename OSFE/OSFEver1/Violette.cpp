#include "stdafx.h"
#include "Violette.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "AbstractFactory.h"
#include "SoundMgr.h"
#include "EffectMgr.h"

CViolette::CViolette()
	:bPattern(false), m_iPatternSpeed(2000), m_iMoveNum(-1)
{
}


CViolette::~CViolette()
{
	Release();
}

void CViolette::Initialize()
{
	m_tInfo.fCX = 192.f;
	m_tInfo.fCY = 192.f;
	m_fSpeed = 8.6f;
	m_iAttack = 10;
	m_bAttacked = false;
	m_eObjID = ENEMIE;
	m_ePreState = STATE_END;
	m_eCurState = IDLE;
	m_iTileNum = Check_Tile();
	m_iPreTileNum = m_iTileNum;
	m_tPreLocation = m_tInfo;
	m_tCreature = { 1800, 0, 100, 10 };

	BITMAP->Insert_Bmp(L"../Image/Enemie/Violette/Violette.bmp", L"Violette");
	// 사망
	BITMAP->Insert_Bmp(L"../Image/Enemie/Violette/Violette 60.bmp", L"Violette 60");
	BITMAP->Insert_Bmp(L"../Image/Enemie/Violette/BreakerBeam.bmp", L"BreakerBeam");
	BITMAP->Insert_Bmp(L"../Image/Enemie/Violette/MusicMeasureShot.bmp", L"MusicMeasureShot");
	BITMAP->Insert_Bmp(L"../Image/Enemie/Violette/FateLine.bmp", L"FateLine");
	BITMAP->Insert_Bmp(L"../Image/Enemie/Violette/DiagBeamShot.bmp", L"DiagBeamShot");
	m_pFrameKey = L"Violette";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 8;
	m_tFrame.iImageEnd = 8;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 0;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 200;
	m_tFrame.dwTime = GetTickCount();
	m_dwPatternTime = GetTickCount();
	m_dwMoveTime = GetTickCount();
}

int CViolette::Update()
{
	if (m_tCreature.iHp <= 0)
		Select_Dead();
	if (m_bDead)
	{
		static_cast<CCreature*>(SCENE->Get_Scene()->Get_Player())->Set_Loot(
			m_tCreature.fExp, m_tCreature.iMoney);

		EFFECT->Add_Effect(EFFECT_PARTICLE, CAbstractFactory<CParticleEffect>::Create_Effect(m_tInfo.fX, m_tInfo.fY));
		return OBJ_DEAD;
	}
		
	if (m_eCurState == DEAD)
	{
		Save_Violette();
		return 0;
	}

	Reset_DamageFont();

	Check_Tile();
	Move_Rand();

	Attack();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CViolette::Late_Update()
{
	if (m_eCurState == DEAD)
		return;
	Motion_Change();

	__super::Move_Frame();
}

void CViolette::Render(HDC hDC)
{
	CCreature::Render(hDC);
	if (m_eCurState == DEAD)
	{
		HDC		hMemDC = BITMAP->Find_Img(L"Violette 60");
		GdiTransparentBlt(hDC,
			int(m_tRect.left),
			int(m_tRect.top),
			int(m_tInfo.fCX), // 키울 사이즈
			int(m_tInfo.fCY),
			hMemDC,
			0,
			0,
			126,		// 텍스처 원본 사이즈
			144,
			RGB(1, 1, 1));
		return;
	}

	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		(8 - m_tFrame.iFrameCnt) * 126,
		m_tFrame.iMotionCnt * 144,
		126,		// 텍스처 원본 사이즈
		144,
		RGB(1, 1, 1));

	for (auto& iter : m_DamageList)
	{
		iter->Render_Damage(hDC);
	}
}

void CViolette::Release()
{
}

void CViolette::Attack()
{
	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		m_eCurState = IDLE;

	if (m_dwTime + m_iPatternSpeed < GetTickCount())
	{
		if (!bPattern)
			Select_Pattern();
		m_dwTime = GetTickCount();
	}
	if (bPattern)
		m_eCurState = MUSIC;
}

void CViolette::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iImageEnd = 8;
			m_tFrame.iMotion = 0;
			m_tFrame.iMotionEnd = 0;
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
		case ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 10;
			m_tFrame.iImageEnd = 8;
			m_tFrame.iMotion = 1;
			m_tFrame.iMotionEnd = 2;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			break;
		case DEAD:
			m_tFrame.iFrameStart = 6;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iImageEnd = 8;
			m_tFrame.iMotion = 5;
			m_tFrame.iMotionEnd = 5;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 400;
			m_tFrame.dwTime = GetTickCount();
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CViolette::Collilsion_Event(CObj * _pObj)
{
	switch (_pObj->Get_OBJID())
	{
	case SPELL:
		m_tCreature.iHp -= static_cast<CSpell*>(_pObj)->Get_SpellInfo().iAtk;
		SOUND->PlaySound(L"enemy_hit_light.wav", SOUND_EFFECT12, 0.3f);
		m_DamageList.push_back(CAbstractFactory<CDamageUI>::Create_DamageUI(m_tInfo.fX - 50, m_tInfo.fY - 100 - m_iDamageY, static_cast<CSpell*>(_pObj)->Get_SpellInfo().iAtk));
		Set_DamageY(20);
		if (m_eCurState == DEAD)
			this->Set_Dead();
		break;
	default:
		break;
	}
}

void CViolette::Count_Trigger(int _iTriggerCnt)
{
}

void CViolette::Select_Pattern()
{
	int iNum = rand() % PATTERN_END;

	switch (iNum)
	{
	case PATTERN_BEAM:
		// 빔 패턴화 해서 클래스로 제작해야할듯 or 함수로 제작
		m_eCurState = MUSIC;
		SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CBeam>::Create(0));
		SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CBeam>::Create(9));
		SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CBeam>::Create(18));
		SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CBeam>::Create(27));
		break;
	case PATTERN_SHOT:
		m_eCurState = ATTACK;
		SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CMusicShot>::Create(m_iTileNum));
		break;
	case PATTERN_MUSIC:
		m_eCurState = MUSIC;
		SCENE->Get_Scene()->Add_Object(SPELL, CAbstractFactory<CMusicPattern>::Create());
		break;
	case PATTERN_SPEAKER:
		if (SCENE->Get_Scene()->Get_ObjList(ENEMIE).size() > 2)
			return;
		m_eCurState = ATTACK;
		Speaker_Spawn();
		break;
	default:
		break;
	}
	bPattern = true;
}

void CViolette::Speaker_Spawn()
{
	int iSpeaker1 = rand() % 4 * 8 + 1;
	int iSpeaker2 = rand() % 4 * 8 + 3;

	SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CSpeakerSpawn>::Create(iSpeaker1));
	SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CSpeakerSpawn>::Create(iSpeaker2));
}

void CViolette::Select_Dead()
{
	SOUND->StopSound(SOUND_BGM);
	for (auto& iter : SCENE->Get_Scene()->Get_ObjList(SPELL))
	{
		iter->Set_Dead();
	}
	for (auto& iter : SCENE->Get_Scene()->Get_ObjList(ENEMIE_SPELL))
	{
		iter->Set_Dead();
	}
	for (auto& iter : SCENE->Get_Scene()->Get_ObjList(TURRET))
	{
		iter->Set_Dead();
	}
	bool bVi = true;
	for (auto& iter : SCENE->Get_Scene()->Get_ObjList(ENEMIE))
	{
		if (bVi)
		{
			bVi = false;
			continue;
		}
		static_cast<CCreature*>(iter)->Set_Damage(999999);
	}
	m_tCreature.iHp = 1;
	m_eCurState = DEAD;
}

void CViolette::Move_Rand()
{
	if (m_dwMoveTime + 8000 < GetTickCount())
	{
		m_tPreLocation = m_tInfo;
		m_iMoveNum = rand() % MOVE_END;
		m_dwMoveTime = GetTickCount();
	}
	
	switch (m_iMoveNum)
	{
	case MOVE_LEFT:
		Move_Left();
		break;
	case MOVE_UP:
		Move_Up();
		break;
	case MOVE_RIGHT:
		Move_Right();
		break;
	case MOVE_DOWN:
		Move_Down();
		break;
	default:
		break;
	}
}

void CViolette::Move_Left()
{
	if (m_iMoveNum == MOVE_LEFT)
	{
		m_tInfo.fX -= m_fSpeed;
		// 타일 벗어나려하면 이동금지
		if (m_tInfo.fX <= (float)(TILEINIT_X + TILECX * 4.9f + TILEITV * 3.f) - m_fSpeed)
		{
			m_tInfo = m_tPreLocation;
			m_iMoveNum = MOVE_END;
		}
		else
		{
			// 자연스럽게 이동시키기.
			if (m_tInfo.fX < m_tPreLocation.fX - (TILECX + TILEITV))
			{
				// 위치 정확한곳에 고정
				m_tInfo.fX = m_tPreLocation.fX - (TILECX + TILEITV);
				m_tPreLocation = m_tInfo;
				m_iTileNum -= 1;
				m_iMoveNum = MOVE_END;
			}
		}
	}
}

void CViolette::Move_Up()
{
	if (m_iMoveNum == MOVE_UP)
	{
		m_tInfo.fY -= m_fSpeed;
		// 타일 벗어나려하면 이동금지
		if (m_tInfo.fY <= (float)(TILEINIT_Y + TILECY * 0.5f) - m_fSpeed)
		{
			m_tInfo = m_tPreLocation;
			m_iMoveNum = MOVE_END;
		}
		else
		{
			// 자연스럽게 이동시키기.
			if (m_tInfo.fY < m_tPreLocation.fY - (TILECY + TILEITV))
			{
				// 위치 정확한곳에 고정
				m_tInfo.fY = m_tPreLocation.fY - (TILECY + TILEITV);
				m_tPreLocation = m_tInfo;
				m_iTileNum -= 8;
				m_iMoveNum = MOVE_END;
			}
		}
	}
}

void CViolette::Move_Right()
{
	if (m_iMoveNum == MOVE_RIGHT)
	{
		m_tInfo.fX += m_fSpeed;
		// 타일 벗어나려하면 이동금지
		if (m_tInfo.fX >= (float)(TILEINIT_X + TILECX * 0.5f) + (float)(TILECX + TILEITV) * 7.f + m_fSpeed)
		{
			m_tInfo = m_tPreLocation;
			m_iMoveNum = MOVE_END;
		}
		else
		{
			// 자연스럽게 이동시키기.
			if (m_tInfo.fX > m_tPreLocation.fX + TILECX + TILEITV)
			{
				// 위치 정확한곳에 고정
				m_tInfo.fX = m_tPreLocation.fX + TILECX + TILEITV;
				m_tPreLocation = m_tInfo;
				m_iTileNum += 1;
				m_iMoveNum = MOVE_END;
			}
		}
	}
}

void CViolette::Move_Down()
{
	if (m_iMoveNum == MOVE_DOWN)
	{
		m_tInfo.fY += m_fSpeed;
		// 타일 벗어나려하면 이동금지
		if (m_tInfo.fY >= (float)(TILEINIT_Y + TILECY * 0.5f) + (float)(TILECY + TILEITV) * 3.f + m_fSpeed)
		{
			m_tInfo = m_tPreLocation;
			m_iMoveNum = MOVE_END;
		}
		else
		{
			// 자연스럽게 이동시키기.
			if (m_tInfo.fY > m_tPreLocation.fY + TILECY + TILEITV)
			{
				// 위치 정확한곳에 고정
				m_tInfo.fY = m_tPreLocation.fY + TILECY + TILEITV;
				m_tPreLocation = m_tInfo;
				m_iTileNum += 8;
				m_iMoveNum = MOVE_END;
			}
		}
	}
}

void CViolette::Save_Violette()
{
	if (KEY->Key_Down(VK_TAB))
	{
		if (SCENE->Get_Scene()->Get_UIList(UI_MAP).empty())
		{
			SCENE->Get_Scene()->AddUI(UI_MAP, CAbstractFactory<CMapUI>::Create_UI());
			static_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Set_SavedVi(true);
		}
			
		else
		{
			SCENE->Get_Scene()->DeleteUI(UI_MAP);
			static_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Set_SavedVi(false);
		}
	}
}

bool CViolette::Save_Event()
{
	m_eCurState = IDLE;
	// 살렸을때 이벤트.
	// 이벤트가 끝나면 리턴 false;
	// 시간값으로 대사치는시간 벌어주고
	if (m_dwTime + 1000 < GetTickCount())
	{
		static_cast<CPlayer*>(
			SCENE->Get_Scene()->Get_Player())->Get_Deck()->Get_Inventory().push_back(
				static_cast<CSpell*>(CAbstractFactory<CSpawnViolette>::Create()));
		return false;
	}

	return true;
}

