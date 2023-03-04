#include "stdafx.h"
#include "Serif.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "AbstractFactory.h"
#include "SoundMgr.h"
#include "EffectMgr.h"

CSerif::CSerif()
	:bPattern(false), m_iPatternSpeed(4000), m_iMoveNum(-1), 
	m_iBlastCnt(-1), m_iSwordCnt(-1), m_iBallCnt(-1),
	m_iWingCnt(0), m_iRainCnt(0), m_pLightSwordRain(nullptr)
{
}


CSerif::~CSerif()
{
	Release();
}

void CSerif::Initialize()
{
	m_tInfo.fCX = 192.f;
	m_tInfo.fCY = 384.f;
	m_fSpeed = 8.6f;
	m_iAttack = 10;
	m_bAttacked = false;
	m_eObjID = ENEMIE;
	m_ePreState = STATE_END;
	m_eCurState = IDLE;
	m_iTileNum = Check_Tile();
	m_iPreTileNum = m_iTileNum;
	m_tPreLocation = m_tInfo;
	m_tCreature = { 2400, 0, 100, 10 };

	BITMAP->Insert_Bmp(L"../Image/Enemie/Serif/Serif.bmp", L"Serif");
	BITMAP->Insert_Bmp(L"../Image/Enemie/Serif/LightTileBlast.bmp", L"LightTileBlast");
	BITMAP->Insert_Bmp(L"../Image/Enemie/Serif/LightSwordShot.bmp", L"LightSwordShot");
	BITMAP->Insert_Bmp(L"../Image/Enemie/Serif/LightSwordRain.bmp", L"LightSwordRain");
	BITMAP->Insert_Bmp(L"../Image/Enemie/Serif/ExcaliburRain.bmp", L"LightBallRain");
	BITMAP->Insert_Bmp(L"../Image/Enemie/Serif/LightBall.bmp", L"LightBall");
	BITMAP->Insert_Bmp(L"../Image/Enemie/Serif/Bomblight.bmp", L"Bomblight");
	BITMAP->Insert_Bmp(L"../Image/Enemie/Serif/WingSwords.bmp", L"WingSwords");
	BITMAP->Insert_Bmp(L"../Image/Enemie/Serif/Plg.bmp", L"Plg");
	BITMAP->Insert_Bmp(L"../Image/Enemie/Serif/Reset.bmp", L"Reset");
	m_pFrameKey = L"Serif";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 8;
	m_tFrame.iImageEnd = 9;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 0;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 150;
	m_tFrame.dwTime = GetTickCount();

	m_dwPatternTime = GetTickCount();
	m_dwMoveTime = GetTickCount();
}

int CSerif::Update()
{
	if (SCENE->Get_SceneID() == SC_GATE)
	{
		m_tCreature.iHp = 999999;
		Count_Trigger(1);
		__super::Update_Rect();
		return 0;
	}
	if (!m_bDead && m_tCreature.iHp <= 0)
		Revive(); // 체력 999999로 되살리고 플레이어 한테 야미 스펠 다량으로 대입
	if (m_bDead)
	{
		if (m_tCreature.iHp < 0)
		{
			m_tCreature.iHp = 0;
		}
		m_eCurState = YAMI;
		Motion_Change();

		if (m_eCurState == YAMI && m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
			return OBJ_DEAD;
		else
			return 0;
	}
	if (m_eCurState == DEAD)
	{
		Reset_DamageFont();
		return 0;
	}
	Count_Trigger(1);

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CSerif::Late_Update()
{
	Motion_Change();

	__super::Move_Frame();
}

void CSerif::Render(HDC hDC)
{
	if (m_tCreature.iHp < 0)
		m_tCreature.iHp = 0;
		
	CCreature::Render(hDC);

	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		(9 - m_tFrame.iFrameCnt) * 120,
		m_tFrame.iMotionCnt * 240,
		120,		// 텍스처 원본 사이즈
		240,
		RGB(1, 1, 1));

	for (auto& iter : m_DamageList)
	{
		iter->Render_Damage(hDC);
	}
}

void CSerif::Release()
{
	Safe_Delete<CObj*>(m_pLightSwordRain);
	EFFECT->Add_Effect(EFFECT_PARTICLE, CAbstractFactory<CParticleEffect>::Create_Effect(m_tInfo.fX, m_tInfo.fY));
}

void CSerif::Attack()
{
	Pattern_Blast();
	Pattern_Sword();
	Pattern_Ball();
	Pattern_Wing();
	Pattern_Rain();

	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
		m_eCurState = IDLE;

	if (m_dwTime + m_iPatternSpeed < GetTickCount())
	{
		if (!bPattern)
			Select_Pattern();
		m_dwTime = GetTickCount();
	}
}

void CSerif::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iImageEnd = 9;
			m_tFrame.iMotion = 0;
			m_tFrame.iMotionEnd = 0;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 150;
			m_tFrame.dwTime = GetTickCount();
			break;
		case ATTACK_BLAST:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 13;
			m_tFrame.iImageEnd = 9;
			m_tFrame.iMotion = 3;
			m_tFrame.iMotionEnd = 4;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			break;
		case ATTACK_SWORD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 22;
			m_tFrame.iImageEnd = 9;
			m_tFrame.iMotion = 0;
			m_tFrame.iMotionEnd = 2;
			m_tFrame.iFrameCnt = 9;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 150;
			m_tFrame.dwTime = GetTickCount();
			break;
		case ATTACK_BALL:
			m_tFrame.iFrameStart = 4;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iImageEnd = 9;
			m_tFrame.iMotion = 2;
			m_tFrame.iMotionEnd = 2;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			break;
		case ATTACK_WING:
			m_tFrame.iFrameStart = 4;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iImageEnd = 9;
			m_tFrame.iMotion = 4;
			m_tFrame.iMotionEnd = 4;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			break;
		case DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iImageEnd = 9;
			m_tFrame.iMotion = 0;
			m_tFrame.iMotionEnd = 0;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 150;
			m_tFrame.dwTime = GetTickCount();
			break;
		case YAMI:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 27;
			m_tFrame.iImageEnd = 9;
			m_tFrame.iMotion = 5;
			m_tFrame.iMotionEnd = 7;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 150;
			m_tFrame.dwTime = GetTickCount();
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CSerif::Collilsion_Event(CObj * _pObj)
{
	switch (_pObj->Get_OBJID())
	{
	case SPELL:
		m_tCreature.iHp -= static_cast<CSpell*>(_pObj)->Get_SpellInfo().iAtk;
		SOUND->PlaySound(L"enemy_hit_light.wav", SOUND_EFFECT12, 0.3f);
		m_DamageList.push_back(CAbstractFactory<CDamageUI>::Create_DamageUI(m_tInfo.fX - 50, m_tInfo.fY - 100 - m_iDamageY, static_cast<CSpell*>(_pObj)->Get_SpellInfo().iAtk));
		Set_DamageY(20);
		if (static_cast<CSpell*>(_pObj)->Get_SpellID() == SPELL_YAMI)
		{
			m_bDead = true;
		}
		break;
	default:
		break;
	}
}

void CSerif::Count_Trigger(int _iTriggerCnt)
{
	if (m_iRenderCnt == 1)
	{
		m_tInfo.fX = WINCX;
		m_tInfo.fY = 0;
	}
	else if (m_iRenderCnt == 2)
	{
		// 내려오는 비율 유지
		m_tInfo.fX -= 1.1f;
		m_tInfo.fY += 1.25f;
		Reset_DamageFont();
		Check_Tile();
		if (m_iTileNum == 20)
		{
			Set_Pos(m_iTileNum);
			m_iRenderCnt++;
		}
		--m_iRenderCnt;
	}
	else if (300 <= m_iRenderCnt)
	{
		if (SCENE->Get_SceneID() == SC_GATE)
		{
			Reset_DamageFont();
			Check_Tile();
			Attack();
			return;
		}
		Reset_DamageFont();

		Check_Tile();
		Move_Rand();

		Attack();
	}
	if (m_iRenderCnt)
		++m_iRenderCnt;
}

void CSerif::Select_Pattern()
{
	int iNum = rand() % PATTERN_END;
	if (SCENE->Get_SceneID() == SC_GATE)
		iNum = PATTERN_RAIN;
	switch (iNum)
	{
	case PATTERN_BLAST:
		m_eCurState = ATTACK_BLAST;
		m_iBlastCnt = 3;
		break;
	case PATTERN_SWORD:
		m_eCurState = ATTACK_SWORD;
		m_iSwordCnt = 3;
		break;
	case PATTERN_BALL:
		m_eCurState = ATTACK_BALL;
		m_iBallCnt = 2;
		break;
	case PATTERN_WING:
		m_eCurState = ATTACK_WING;
		SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CWingSword>::Create(m_tInfo.fX, m_tInfo.fY));
		m_iPreTileNum = m_iTileNum;
		m_iWingCnt = 1;
		break;
	case PATTERN_RAIN:
		m_eCurState = IDLE;
		m_pLightSwordRain = CAbstractFactory<CLightSwordRain>::Create(m_tInfo.fX, m_tInfo.fY - 70);
		m_iRainCnt = 0;
		m_dwRainTime = GetTickCount();
		break;
	default:
		break;
	}
	bPattern = true;
}

void CSerif::Pattern_Blast()
{
	if (m_iBlastCnt >= 0)
	{
		if (m_dwPatternTime + 50 < GetTickCount())
		{
			SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CTile_Blast>::Create(m_iBlastCnt));
			SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CTile_Blast>::Create(m_iBlastCnt + 8));
			SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CTile_Blast>::Create(m_iBlastCnt + 16));
			SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CTile_Blast>::Create(m_iBlastCnt + 24));
			--m_iBlastCnt;
			m_dwPatternTime = GetTickCount();
		}
		if (m_iBlastCnt == 0)
			bPattern = false;
	}
}

void CSerif::Pattern_Sword()
{
	if (m_iSwordCnt >= 0)
	{
		if (m_dwPatternTime + 400 < GetTickCount())
		{
			int iAttackTile = m_iTileNum % 8;
			switch (m_iSwordCnt)
			{
			case 3:
				SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CLightSwordShot>::Create(iAttackTile));
				SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CLightSwordShot>::Create(iAttackTile + 8));
				SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CLightSwordShot>::Create(iAttackTile + 16));
				SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CLightSwordShot>::Create(iAttackTile + 24));
				break;														  
			case 2:															  
				SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CLightSwordShot>::Create(iAttackTile));
				SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CLightSwordShot>::Create(iAttackTile + 16));
				SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CLightSwordShot>::Create(iAttackTile + 24));
				break;
			case 1:
				SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CLightSwordShot>::Create(iAttackTile));
				SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CLightSwordShot>::Create(iAttackTile + 16));
				break;
			case 0:
				SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CLightSwordShot>::Create(m_iTileNum));
				break;
			default:
				break;
			}
			
			--m_iSwordCnt;
			m_dwPatternTime = GetTickCount();
		}
		if (m_iSwordCnt == 0)
			bPattern = false;
	}
	
}

void CSerif::Pattern_Ball()
{
	if (m_iBallCnt >= 0)
	{
		if (m_dwPatternTime + 200 < GetTickCount())
		{
			m_eCurState = ATTACK_BALL;
			int iPos = (rand() * GetTickCount()) % 4 + (m_dwPatternTime % 4) * 8;
			SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CLightBallRain>::Create(iPos));

			--m_iBallCnt;
			m_dwPatternTime = GetTickCount();
		}
		if (m_iBallCnt == 0)
			bPattern = false;
	}
}

void CSerif::Pattern_Wing()
{
	if (m_iWingCnt)
	{
		m_tInfo.fX -= m_fSpeed;
		if (m_iTileNum % 8 == 1 && 0 < m_fSpeed)
			m_fSpeed *= -1;

		if (0 > m_fSpeed && m_iTileNum == m_iPreTileNum)
		{
			m_iWingCnt = 0;
			Set_Pos(m_iTileNum);
			bPattern = false;
			m_fSpeed *= -1;
			SCENE->Get_Scene()->Delete_Object(ENEMIE_SPELL);
		}
	}
}

void CSerif::Pattern_Rain()
{
	if (m_iRainCnt >= 0)
	{
		if (m_dwRainTime + 50 < GetTickCount())
		{
			SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CLightSwordRain>::Create(*static_cast<CLightSwordRain*>(m_pLightSwordRain), m_iRainCnt));
			SCENE->Get_Scene()->Add_Object(ENEMIE_SPELL, CAbstractFactory<CLightSwordRain>::Create(*static_cast<CLightSwordRain*>(m_pLightSwordRain), 27 - m_iRainCnt));

			if (m_iRainCnt == 24 || m_iRainCnt == 1 || m_iRainCnt == 26)
				++m_iRainCnt;
			else if (m_iRainCnt % 2) // 홀수
				m_iRainCnt -= 8;
			else if (m_iRainCnt % 2 == 0)
				m_iRainCnt += 8;

			m_dwRainTime = GetTickCount();
		}
	}
	else
		bPattern = false;
}

void CSerif::Revive()
{
	SOUND->StopSound(SOUND_BGM);
	for (auto& iter : SCENE->Get_Scene()->Get_ObjList(SPELL))
	{
		iter->Set_Dead();
	}

	m_tCreature.iHp = 999999;
	static_cast<CPlayer*>(SCENE->Get_Scene()->Get_Player())->Get_Deck()->Push_Yami();
	
	m_eCurState = DEAD;
}

void CSerif::Move_Rand()
{
	if (0 < m_iWingCnt || 0 < m_iRainCnt) // 패턴 중 이동 금지
		return;
	if (m_dwMoveTime + 3000 < GetTickCount())
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

void CSerif::Move_Left()
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

void CSerif::Move_Up()
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

void CSerif::Move_Right()
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

void CSerif::Move_Down()
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
