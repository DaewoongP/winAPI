#include "stdafx.h"
#include "Speaker.h"
#include "BitmapMgr.h"
#include "TileMgr.h"
#include "SoundMgr.h"
#include "SceneMgr.h"
#include "AbstractFactory.h"

CSpeaker::CSpeaker()
	:iRender(0), m_pAttackKey(nullptr), iCnt(0), iReleaseCnt(0)
{
	ZeroMemory(&m_tFrame_Attack, sizeof(FRAME));
}


CSpeaker::~CSpeaker()
{
	Release();
}

void CSpeaker::Initialize()
{
	m_tInfo.fCX = 192.f;
	m_tInfo.fCY = 192.f;
	m_iAttack = 80;
	m_eObjID = ENEMIE;
	m_iTileNum = Check_Tile();

	m_tCreature = { 10, 0, 0, 0 };
	BITMAP->Insert_Bmp(L"../Image/Enemie/Violette/ViSpeaker.bmp", L"ViSpeaker");
	// 124 x 114
	BITMAP->Insert_Bmp(L"../Image/Enemie/Violette/SoundCircle.bmp", L"SoundCircle");
	// 200 x 85
	BITMAP->Insert_Bmp(L"../Image/Enemie/Violette/SoundCones.bmp", L"SoundCones");
	// 200 x 154
	BITMAP->Insert_Bmp(L"../Image/Enemie/Violette/SoundCross.bmp", L"SoundCross");
	m_pFrameKey = L"ViSpeaker";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iImageEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 1;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 200;
	m_tFrame.dwTime = GetTickCount();
	m_bAttackEnd = true;

}

int CSpeaker::Update()
{
	if (m_tCreature.iHp <= 0)
		return OBJ_DEAD;
	if (iReleaseCnt > 5)
		return OBJ_DEAD;

	Check_Tile();

	Reset_DamageFont();

	Attack();
	Motion_Change();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CSpeaker::Late_Update()
{
	Attack_Frame();
	__super::Move_Frame();
}

void CSpeaker::Render(HDC hDC)
{
	if (iRender < 10)
	{
		++iRender;
		return;
	}

	CCreature::Render(hDC);

	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,
		int(m_tRect.left) - 23,
		int(m_tRect.top),
		int(m_tInfo.fCX), // 키울 사이즈
		int(m_tInfo.fCY),
		hMemDC,
		m_tFrame.iFrameCnt * 120,
		m_tFrame.iMotionCnt * 120,
		120,		// 텍스처 원본 사이즈
		120,
		RGB(1, 1, 1));

	for (auto& iter : m_DamageList)
	{
		iter->Render_Damage(hDC);
	}

	HDC		hAttackDC = BITMAP->Find_Img(m_pAttackKey);
	switch (m_eCurAttack)
	{
	case SOUNDCIRCLE:
		// 124 x 114
		GdiTransparentBlt(hDC,
			int(m_tRect.left) - 23,
			int(m_tRect.top),
			int(m_tInfo.fCX), // 키울 사이즈
			int(m_tInfo.fCY),
			hAttackDC,
			m_tFrame_Attack.iFrameCnt * 124,
			m_tFrame_Attack.iMotionCnt * 114,
			124,		// 텍스처 원본 사이즈
			114,
			RGB(1, 1, 1));
		break;
	case SOUNDCONES:
		// 200 x 85
		GdiTransparentBlt(hDC,
			int(m_tRect.left) - 23,
			int(m_tRect.top),
			int(m_tInfo.fCX), // 키울 사이즈
			int(m_tInfo.fCY),
			hAttackDC,
			m_tFrame_Attack.iFrameCnt * 200,
			m_tFrame_Attack.iMotionCnt * 85,
			200,		// 텍스처 원본 사이즈
			85,
			RGB(1, 1, 1));
		break;
	case SOUNDCROSS:
		// 200 x 154
		GdiTransparentBlt(hDC,
			int(m_tRect.left) - 23,
			int(m_tRect.top),
			int(m_tInfo.fCX), // 키울 사이즈
			int(m_tInfo.fCY),
			hAttackDC,
			m_tFrame_Attack.iFrameCnt * 200,
			m_tFrame_Attack.iMotionCnt * 154,
			200,		// 텍스처 원본 사이즈
			154,
			RGB(1, 1, 1));
		break;
	default:
		break;
	}
}

void CSpeaker::Release()
{
	switch (m_eCurAttack)
	{
	case SOUNDCIRCLE:
		Circle_Idle();
		break;
	case SOUNDCONES:
		Cone_Idle();
		break;
	case SOUNDCROSS:
		Cross_Idle();
		break;
	}
}

void CSpeaker::Attack()
{
	if (m_bAttackEnd)
	{
		m_AttackTileList.clear();
		switch (rand() % 3)
		{
		case 0:
			Circle_Attack();
			m_eCurAttack = SOUNDCIRCLE;
			m_pAttackKey = L"SoundCircle";
			break;
		case 1:
			Cone_Attack();
			m_eCurAttack = SOUNDCONES;
			m_pAttackKey = L"SoundCones";
			break;
		case 2:
			Cross_Attack();
			m_eCurAttack = SOUNDCROSS;
			m_pAttackKey = L"SoundCross";
			break;
		default:
			break;
		}
		m_bAttackEnd = false;
	}
	else
	{
		if (m_tFrame_Attack.iFrameCnt == m_tFrame_Attack.iFrameEnd)
		{
           	++iCnt;
			SOUND->PlaySound(L"flare-light.wav", SOUND_EFFECT09, 0.3f);
			if (iCnt > 10)
			{
				++iReleaseCnt;
				m_bAttackEnd = true;
				switch (m_eCurAttack)
				{
				case SOUNDCIRCLE:
					Circle_Idle();
					break;
				case SOUNDCONES:
					Cone_Idle();
					break;
				case SOUNDCROSS:
					Cross_Idle();
					break;
				}
				m_eCurAttack = SOUNDEND;
				iCnt = 0;
			}
		}
	}
}

void CSpeaker::Motion_Change()
{
	if (m_ePreAttack != m_eCurAttack)
	{
		switch (m_eCurAttack)
		{
		case SOUNDCIRCLE:
			m_tFrame_Attack.iFrameStart = 0;
			m_tFrame_Attack.iFrameEnd = 3;
			m_tFrame_Attack.iImageEnd = 1;
			m_tFrame_Attack.iMotion = 0;
			m_tFrame_Attack.iMotionEnd = 1;
			m_tFrame_Attack.iFrameCnt = m_tFrame_Attack.iFrameStart;
			m_tFrame_Attack.iMotionCnt = m_tFrame_Attack.iMotion;
			m_tFrame_Attack.dwSpeed = 200;
			m_tFrame_Attack.dwTime = GetTickCount();
			break;
		case SOUNDCONES:
			m_tFrame_Attack.iFrameStart = 0;
			m_tFrame_Attack.iFrameEnd = 3;
			m_tFrame_Attack.iImageEnd = 1;
			m_tFrame_Attack.iMotion = 0;
			m_tFrame_Attack.iMotionEnd = 1;
			m_tFrame_Attack.iFrameCnt = m_tFrame_Attack.iFrameStart;
			m_tFrame_Attack.iMotionCnt = m_tFrame_Attack.iMotion;
			m_tFrame_Attack.dwSpeed = 200;
			m_tFrame_Attack.dwTime = GetTickCount();
			break;
		case SOUNDCROSS:
			m_tFrame_Attack.iFrameStart = 0;
			m_tFrame_Attack.iFrameEnd = 3;
			m_tFrame_Attack.iImageEnd = 1;
			m_tFrame_Attack.iMotion = 0;
			m_tFrame_Attack.iMotionEnd = 1;
			m_tFrame_Attack.iFrameCnt = m_tFrame_Attack.iFrameStart;
			m_tFrame_Attack.iMotionCnt = m_tFrame_Attack.iMotion;
			m_tFrame_Attack.dwSpeed = 200;
			m_tFrame_Attack.dwTime = GetTickCount();
			break;
		}
		m_ePreAttack = m_eCurAttack;
	}
}

void CSpeaker::Collilsion_Event(CObj * _pObj)
{
	switch (_pObj->Get_OBJID())
	{
	case SPELL:
		m_tCreature.iHp -= dynamic_cast<CSpell*>(_pObj)->Get_SpellInfo().iAtk;
		SOUND->PlaySound(L"enemy_hit_light.wav", SOUND_EFFECT12, 0.3f);
		m_DamageList.push_back(CAbstractFactory<CDamageUI>::Create_DamageUI(m_tInfo.fX - 50, m_tInfo.fY - 100 - m_iDamageY, static_cast<CSpell*>(_pObj)->Get_SpellInfo().iAtk));
		Set_DamageY(20);
		break;
	default:
		break;
	}
}

void CSpeaker::Count_Trigger(int _iTriggerCnt)
{
}

void CSpeaker::Attack_Frame()
{
	if (m_tFrame_Attack.dwTime + m_tFrame_Attack.dwSpeed < GetTickCount())
	{
		++m_tFrame_Attack.iFrameCnt;
		// 이미지 가로 장수보다 프레임이 작을경우 그냥 회전.
		if (m_tFrame_Attack.iFrameEnd <= m_tFrame_Attack.iImageEnd)
		{
			if (m_tFrame_Attack.iFrameCnt > m_tFrame_Attack.iFrameEnd)
			{
				m_tFrame_Attack.iFrameCnt = m_tFrame_Attack.iFrameStart;
				m_tFrame_Attack.iMotionCnt = m_tFrame_Attack.iMotion;
				m_tFrame_Attack.iFrameEnd += (m_tFrame_Attack.iImageEnd + 1) * (m_tFrame_Attack.iMotionEnd - m_tFrame_Attack.iMotion);
			}
		}
		else
		{	// 세로가 엔드보다 커지면 초기화
			if (m_tFrame_Attack.iMotionCnt > m_tFrame_Attack.iMotionEnd)
			{
				m_tFrame_Attack.iMotionCnt = m_tFrame_Attack.iMotion;
			}
			else
			{
				// 이미지 가로 끝에 도달하면 초기화
				if (m_tFrame_Attack.iFrameCnt > m_tFrame_Attack.iImageEnd)
				{
					m_tFrame_Attack.iFrameCnt = m_tFrame_Attack.iFrameStart;
					++m_tFrame_Attack.iMotionCnt;
					m_tFrame_Attack.iFrameEnd -= m_tFrame_Attack.iImageEnd + 1;
				}
			}
		}
		m_tFrame_Attack.dwTime = GetTickCount();
	}
}

void CSpeaker::Circle_Attack()
{
	if (m_iTileNum / 8 == 0) // 0행
	{
		for (int i = -1; i < 2; ++i)
		{
			dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i))->Set_TileState(TILE_ATTACKED);
		}
		for (int i = 7; i < 10; ++i)
		{
			dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i))->Set_TileState(TILE_ATTACKED);
		}
	}
	//-9,-8,-7, -1, 0 , 1
	else if (m_iTileNum / 8 == 3) // 3행
	{
		for (int i = -9; i < -6; ++i)
		{
			dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i))->Set_TileState(TILE_ATTACKED);
		}
		for (int i = -1; i < 2; ++i)
		{
			dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i))->Set_TileState(TILE_ATTACKED);
		}
	}
	//-9,-8,-7, -1, 0 , 1, +7, +8, +9
	else
	{
		for (int j = -8; j < 9; j += 8)
		{
			for (int i = -1; i < 2; ++i)
			{
				dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i + j))->Set_TileState(TILE_ATTACKED);
			}
		}
	}
}

void CSpeaker::Cone_Attack()
{
	if (m_iTileNum / 8 == 0) // 0행
	{
		for (int i = -1; i < 10; i += 8)
		{
			dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i))->Set_TileState(TILE_ATTACKED);
			dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i + 2))->Set_TileState(TILE_ATTACKED);
		}
	}
	else if (m_iTileNum / 8 == 3) // 3행
	{
		for (int i = -9; i < 2; i += 8)
		{
			dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i))->Set_TileState(TILE_ATTACKED);
			dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i + 2))->Set_TileState(TILE_ATTACKED);
		}
	}
	else
	{
		for (int i = -9; i < 10; i += 8)
		{
			dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i))->Set_TileState(TILE_ATTACKED);
			dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i + 2))->Set_TileState(TILE_ATTACKED);
		}
	}
}

void CSpeaker::Cross_Attack()
{
	if (m_iTileNum / 8 == 0)
	{
		dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum - 1))->Set_TileState(TILE_ATTACKED);
		dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum + 1))->Set_TileState(TILE_ATTACKED);
		dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum + 8))->Set_TileState(TILE_ATTACKED);
	}
	else if (m_iTileNum / 8 == 3)
	{
		dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum - 8))->Set_TileState(TILE_ATTACKED);
		dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum - 1))->Set_TileState(TILE_ATTACKED);
		dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum + 1))->Set_TileState(TILE_ATTACKED);
	}
	else
	{
		dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum - 8))->Set_TileState(TILE_ATTACKED);
		dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum - 1))->Set_TileState(TILE_ATTACKED);
		dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum + 1))->Set_TileState(TILE_ATTACKED);
		dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum + 8))->Set_TileState(TILE_ATTACKED);
	}
}

void CSpeaker::Circle_Idle()
{
	if (m_iTileNum / 8 == 0) // 0행
	{
		for (int i = -1; i < 2; ++i)
		{
			static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i))->Set_TileState(TILE_IDLE);
			m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i)));
		}
		for (int i = 7; i < 10; ++i)
		{
			static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i))->Set_TileState(TILE_IDLE);
			m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i)));
		}
	}
	//-9,-8,-7, -1, 0 , 1
	else if (m_iTileNum / 8 == 3) // 3행
	{
		for (int i = -9; i < -6; ++i)
		{
			static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i))->Set_TileState(TILE_IDLE);
			m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i)));
		}
		for (int i = -1; i < 2; ++i)
		{
			static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i))->Set_TileState(TILE_IDLE);
			m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i)));
		}
	}
	//-9,-8,-7, -1, 0 , 1, +7, +8, +9
	else
	{
		for (int j = -8; j < 9; j += 8)
		{
			for (int i = -1; i < 2; ++i)
			{
				static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i + j))->Set_TileState(TILE_IDLE);
				m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i + j)));
			}
		}
	}
}

void CSpeaker::Cone_Idle()
{
	if (m_iTileNum / 8 == 0) // 0행
	{
		for (int i = -1; i < 10; i += 8)
		{
			static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i))->Set_TileState(TILE_IDLE);
			m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i)));
			static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i + 2))->Set_TileState(TILE_IDLE);
			m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i + 2)));
		}
	}
	else if (m_iTileNum / 8 == 3) // 3행
	{
		for (int i = -9; i < 2; i += 8)
		{
			static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i))->Set_TileState(TILE_IDLE);
			m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i)));
			static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i + 2))->Set_TileState(TILE_IDLE);
			m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i + 2)));
		}
	}
	else
	{
		for (int i = -9; i < 10; i += 8)
		{
			static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i))->Set_TileState(TILE_IDLE);
			m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i)));
			static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i + 2))->Set_TileState(TILE_IDLE);
			m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i + 2)));
		}
	}
}

void CSpeaker::Cross_Idle()
{
	if (m_iTileNum / 8 == 0)
	{
		static_cast<CTile*>(TILE->Get_Tile(m_iTileNum - 1))->Set_TileState(TILE_IDLE);
		m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum - 1)));
		static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + 1))->Set_TileState(TILE_IDLE);
		m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + 1)));
		static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + 8))->Set_TileState(TILE_IDLE);
		m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + 8)));
	}
	else if (m_iTileNum / 8 == 3)
	{
		static_cast<CTile*>(TILE->Get_Tile(m_iTileNum - 8))->Set_TileState(TILE_IDLE);
		m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum - 8)));
		static_cast<CTile*>(TILE->Get_Tile(m_iTileNum - 1))->Set_TileState(TILE_IDLE);
		m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum - 1)));
		static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + 1))->Set_TileState(TILE_IDLE);
		m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + 1)));
	}
	else
	{
		static_cast<CTile*>(TILE->Get_Tile(m_iTileNum - 8))->Set_TileState(TILE_IDLE);
		m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum - 8)));
		static_cast<CTile*>(TILE->Get_Tile(m_iTileNum - 1))->Set_TileState(TILE_IDLE);
		m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum - 1)));
		static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + 1))->Set_TileState(TILE_IDLE);
		m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + 1)));
		static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + 8))->Set_TileState(TILE_IDLE);
		m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTileNum + 8)));
	}
	
}
