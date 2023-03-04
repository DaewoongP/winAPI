#include "stdafx.h"
#include "Liger.h"
#include "BitmapMgr.h"
#include "TileMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "EffectMgr.h"
#include "AbstractFactory.h"

CLiger::CLiger()
{
}


CLiger::~CLiger()
{
	Release();
}

void CLiger::Initialize()
{
	m_tInfo.fCX = 104.f;
	m_tInfo.fCY = 64.f;
	m_fSpeed = 5.f;
	m_iAttack = 10;
	m_bAttacked = false;	
	m_eObjID = ENEMIE;
	m_iTileNum = Check_Tile();
	m_iPreTileNum = m_iTileNum;

	m_tCreature = { 300, 0, 10, 3};
	BITMAP->Insert_Bmp(L"../Image/Enemie/Liger.bmp", L"Liger");
	m_pFrameKey = L"Liger";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iImageEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 0;

	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;

	m_tFrame.dwSpeed = 400;
	m_tFrame.dwTime = GetTickCount();
	m_bMove = false;
}

int CLiger::Update()
{
	if (m_tCreature.iHp <= 0)
	{
		static_cast<CCreature*>(SCENE->Get_Scene()->Get_Player())->Set_Loot(
			m_tCreature.fExp, m_tCreature.iMoney);
		return OBJ_DEAD;
	}
		
	// 이동하는 몹에게는 항상 넣어서 예외처리 해줄것.
	if (TILEINIT_X > m_tInfo.fX)
	{
		m_fSpeed = -10.f;
		m_eCurState = IDLE;
	}


	Reset_DamageFont();

	Move();
	Attack();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CLiger::Late_Update()
{
	Motion_Change();

	__super::Move_Frame();
}

void CLiger::Render(HDC hDC)
{
	CCreature::Render(hDC);

	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	GdiTransparentBlt(hDC,		// 복사 받을 최종 DC
		int(m_tRect.left), // 복사 받을 텍스처 위치 좌표 X
		int(m_tRect.top - m_tInfo.fCY * 0.5f), // 복사 받을 텍스처 위치 좌표 Y
		int(m_tInfo.fCX), // 텍스처 가로 사이즈
		int(m_tInfo.fCY), // 텍스처 세로 사이즈
		hMemDC,	// 복사할 DC
		m_tFrame.iFrameCnt * 65,	// 비트맵 출력 좌표 X
		m_tFrame.iMotionCnt * 40,	// 비트맵 출력 좌표 Y
		65, // 복사할 비트맵 텍스처 가로 사이즈
		40, // 복사할 비트맵 텍스처 세로 사이즈
		RGB(255, 255, 255)); // 제거할 픽셀 색상

	for (auto& iter : m_DamageList)
	{
		iter->Render_Damage(hDC);
	}
}

void CLiger::Release()
{
	if ((m_iPreTileNum - 4) % 8 != 0)
	{
		for (int i = -1; i < 2; ++i)
		{
			if (0 <= m_iPreTileNum - 4 + i && 32 > m_iPreTileNum - 4 + i)
			{
				dynamic_cast<CTile*>(TILE->Get_Tile(m_iPreTileNum - 4 + i))->Set_TileState(TILE_IDLE);
				m_AttackTileList.push_back(dynamic_cast<CTile*>(TILE->Get_Tile(m_iPreTileNum - 4 + i)));
			}
		}
	}
	else
	{
		for (int i = 0; i < 2; ++i)
		{
			if (0 <= m_iPreTileNum - 4 + i && 32 > m_iPreTileNum - 4 + i)
			{
				dynamic_cast<CTile*>(TILE->Get_Tile(m_iPreTileNum - 4 + i))->Set_TileState(TILE_IDLE);
				m_AttackTileList.push_back(dynamic_cast<CTile*>(TILE->Get_Tile(m_iPreTileNum - 4 + i)));
			}
		}
	}

	EFFECT->Add_Effect(EFFECT_PARTICLE, CAbstractFactory<CParticleEffect>::Create_Effect(m_tInfo.fX, m_tInfo.fY));
}

void CLiger::Attack()
{
	if (m_bAttacked)
	{
		if (0 > m_fSpeed)
		{
			m_AttackTileList.clear();
			m_tInfo.fX -= m_fSpeed;
			m_iTileNum = Check_Tile();
			if (m_iTileNum == m_iPreTileNum)
			{
				Set_Pos(m_iTileNum);
				m_fSpeed *= -1;
				m_bAttacked = false;
			}
		}
		else
		{
			m_tInfo.fX -= m_fSpeed;
			m_iTileNum = Check_Tile();
			if (m_iTileNum == m_iPlayerTile)
			{
				Set_Pos(m_iTileNum);
				if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
				{
					m_fSpeed *= -1;
					m_eCurState = IDLE;
					if (m_iTileNum % 8 != 4)
					{
						for (int i = -1; i < 2; ++i)
						{
							if (0 <= m_iTileNum + i && 32 > m_iTileNum + i)
							{
								dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i))->Set_TileState(TILE_IDLE);
								m_AttackTileList.push_back(dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i)));
							}
						}
					}
					else
					{
						for (int i = 0; i < 2; ++i)
						{
							if (0 <= m_iTileNum + i && 32 > m_iTileNum + i)
							{
								dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i))->Set_TileState(TILE_IDLE);
								m_AttackTileList.push_back(dynamic_cast<CTile*>(TILE->Get_Tile(m_iTileNum + i)));
							}
						}
					}
				}
			}
		}
	}
}

void CLiger::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iImageEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.iMotionEnd = 0;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;
		case ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iImageEnd = 3;
			m_tFrame.iMotion = 1;
			m_tFrame.iMotionEnd = 1;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CLiger::Collilsion_Event(CObj * _pObj)
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

void CLiger::Count_Trigger(int _iTriggerCnt)
{
}

void CLiger::Move()
{
	if (m_dwTime + 4000 < GetTickCount())
	{
		SOUND->PlaySound(L"bark_low.wav", SOUND_EFFECT13, 0.3f);
		m_iPlayerTile = SCENE->Get_Scene()->Get_Player()->Get_TileNum();
		m_tPlayerInfo = SCENE->Get_Scene()->Get_Player()->Get_Info();
		m_bMove = true;
		m_dwTime = GetTickCount();
	}
	if (m_bMove)
	{
		if (m_tPlayerInfo.fY < m_tInfo.fY)
			m_tInfo.fY -= m_fSpeed * 0.5f;
		else if (m_tPlayerInfo.fY > m_tInfo.fY)
			m_tInfo.fY += m_fSpeed * 0.5f;
		Check_Tile();
		if (m_iTileNum / 8 == m_iPlayerTile / 8)
		{
			m_bMove = false;
			m_bAttacked = true;
			m_eCurState = ATTACK;
			m_iPreTileNum = m_iTileNum;
			if (m_iPlayerTile % 8 != 0)
			{
				for (int i = -1; i < 2; ++i)
				{
					if (0 <= m_iPlayerTile + i && 32 > m_iPlayerTile + i)
					{
						dynamic_cast<CTile*>(TILE->Get_Tile(m_iPlayerTile + i))->Set_TileState(TILE_ATTACKED);
					}
				}
			}
			else // 왼쪽 끝으로 붙으면
			{
				for (int i = 0; i < 2; ++i)
				{
					if (0 <= m_iPlayerTile + i && 32 > m_iPlayerTile + i)
					{
						dynamic_cast<CTile*>(TILE->Get_Tile(m_iPlayerTile + i))->Set_TileState(TILE_ATTACKED);
					}
				}
			}
		}
	}
}
