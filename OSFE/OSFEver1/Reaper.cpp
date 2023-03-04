#include "stdafx.h"
#include "Reaper.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "TileMgr.h"
#include "SoundMgr.h"
#include "AbstractFactory.h"
#include "EffectMgr.h"
CReaper::CReaper()
{
}


CReaper::~CReaper()
{
	Release();
}

void CReaper::Initialize()
{
	m_tInfo.fCX = 192.f;
	m_tInfo.fCY = 192.f;
	m_fSpeed = 10.f;
	m_iAttack = 10;
	m_bAttacked = false;
	m_eObjID = ENEMIE;
	m_eCurState = IDLE;
	m_iTileNum = Check_Tile();
	m_iPreTileNum = m_iTileNum;

	m_tCreature = { 700, 0, 50, 20};
	BITMAP->Insert_Bmp(L"../Image/Enemie/Reaper.bmp", L"Reaper");
	BITMAP->Insert_Bmp(L"../Image/Enemie/Reaper_Attack.bmp", L"Reaper_Attack");
	m_pFrameKey = L"Reaper";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iImageEnd = 1;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 1;

	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;

	m_tFrame.dwSpeed = 400;
	m_tFrame.dwTime = GetTickCount();
}

int CReaper::Update()
{
	if (m_tCreature.iHp <= 0)
	{
		static_cast<CCreature*>(SCENE->Get_Scene()->Get_Player())->Set_Loot(
			m_tCreature.fExp, m_tCreature.iMoney);
		return OBJ_DEAD;
	}
		

	if (m_tFrame.iFrameCnt == 3)
		m_tFrame.dwSpeed = 800;

	Reset_DamageFont();

	Attack();
	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CReaper::Late_Update()
{
	Motion_Change();
	__super::Move_Frame();
}

void CReaper::Render(HDC hDC)
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
		m_tFrame.iMotionCnt * 120,
		120,		// 텍스처 원본 사이즈
		120,
		RGB(255, 255, 255));

	for (auto& iter : m_DamageList)
	{
		iter->Render_Damage(hDC);
	}
}

void CReaper::Release()
{
	// 타일 초기화
	for (int i = 0; i < 3; ++i)
	{
		for (int j = -9; j < -6; ++j)
		{
			// 플레이어 뒤로가서 0번째줄로 이동해버리면 예외처리
			if (m_iPlayerTile % 8 == 1 && j == -9)
				continue;
			if (0 <= m_iPlayerTile - 1 + j + 8 * i &&
				32 > m_iPlayerTile - 1 + j + 8 * i)
			{
				// 씬을 지울때 타일도 같이없어져서생기는문제
				dynamic_cast<CTile*>(
					TILE->Get_Tile(m_iPlayerTile - 1 + j + 8 * i))->Set_TileState(TILE_IDLE);

				m_AttackTileList.push_back(
					dynamic_cast<CTile*>(
						TILE->Get_Tile(m_iPlayerTile - 1 + j + 8 * i)));
			}
		}
	}

	EFFECT->Add_Effect(EFFECT_PARTICLE, CAbstractFactory<CParticleEffect>::Create_Effect(m_tInfo.fX, m_tInfo.fY));
}

void CReaper::Attack()
{
	if (m_dwTime + 5000 < GetTickCount())
	{
		m_iPlayerTile = SCENE->Get_Scene()->Get_Player()->Get_TileNum();
		m_tPlayerInfo = SCENE->Get_Scene()->Get_Player()->Get_Info();
		m_bAttacked = true;
		m_eCurState = ATTACK;
		m_iPreTileNum = m_iTileNum;
		// 플레이어 타일이 첫번째열일때
		// 타일 공격주의 표시
		if (m_iPlayerTile % 8 == 0)
		{
			for (int i = 0; i < 3; ++i)
			{
				for (int j = -8; j < -6; ++j)
				{
					if (0 <= m_iPlayerTile + j + 8 * i &&
						32 > m_iPlayerTile + j + 8 * i)
						dynamic_cast<CTile*>(TILE->Get_Tile(m_iPlayerTile + j + 8 * i))->Set_TileState(TILE_ATTACKED);
				}
			}
		}
		else
		{
			for (int i = 0; i < 3; ++i)
			{
				for (int j = -9; j < -6; ++j)
				{
					// 플레이어 뒤로가서 0번째줄로 이동해버리면 예외처리
					if (m_iPlayerTile % 8 == 1 && j == -9)
						continue;
					if (0 <= m_iPlayerTile - 1 + j + 8 * i &&
						32 > m_iPlayerTile - 1 + j + 8 * i)
						dynamic_cast<CTile*>(TILE->Get_Tile(m_iPlayerTile - 1 + j + 8 * i))->Set_TileState(TILE_ATTACKED);
				}
			}
		}
		
		m_dwTime = GetTickCount();
	}

	if (m_bAttacked)
	{
		// 공격 후 돌아가는 모션
		if (0 > m_fSpeed)
		{
			m_AttackTileList.clear();
			m_tInfo.fX -= m_fSpeed;
			m_iTileNum = Check_Tile();

			if (m_iTileNum == m_iPlayerTile + 4)
			{
				Set_Pos(m_iTileNum);
				m_fSpeed *= -1;
				m_bAttacked = false;
			}
		}
		// 공격 모션
		else
		{
			m_tInfo.fX -= m_fSpeed;
			// 플레이어 타일 Y위치 확인
			if (m_tPlayerInfo.fY < m_tInfo.fY)
				m_tInfo.fY -= m_fSpeed * 0.5f;
			else if (m_tPlayerInfo.fY > m_tInfo.fY)
				m_tInfo.fY += m_fSpeed * 0.5f;
			m_iTileNum = Check_Tile();
			// 공격 시전, 플레이어 위치 0열
			if (m_iPlayerTile % 8 == 0)
			{
				if (m_iTileNum == m_iPlayerTile)
				{
					Set_Pos(m_iPlayerTile);
					if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
					{
						SOUND->PlaySound(L"slice_clang.wav", SOUND_EFFECT16, 0.3f);
						m_fSpeed *= -1;
						m_pFrameKey = L"Reaper";
						m_eCurState = IDLE;
						// 타일 공격, IDLE상태 처리
						for (int i = 0; i < 3; ++i)
						{
							for (int j = -8; j < -6; ++j)
							{
								if (0 <= m_iPlayerTile + j + 8 * i &&
									32 > m_iPlayerTile + j + 8 * i)
								{
									dynamic_cast<CTile*>(
										TILE->Get_Tile(m_iPlayerTile + j + 8 * i))->Set_TileState(TILE_IDLE);
									m_AttackTileList.push_back(
										dynamic_cast<CTile*>(
											TILE->Get_Tile(m_iPlayerTile + j + 8 * i)));
								}
							}
						}
					}
				}
			}
			// 플레이어 위치 1~3열
			else
			{
				if (m_iTileNum == m_iPlayerTile - 1)
				{
					Set_Pos(m_iPlayerTile - 1);
					if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
					{
						SOUND->PlaySound(L"slice_clang.wav", SOUND_EFFECT16, 0.3f);
						m_fSpeed *= -1;
						m_pFrameKey = L"Reaper";
						m_eCurState = IDLE;
						// 타일 공격, IDLE상태 처리
						for (int i = 0; i < 3; ++i)
						{
							for (int j = -9; j < -6; ++j)
							{
								// 플레이어 뒤로가서 0번째줄로 이동해버리면 예외처리
								if (m_iPlayerTile % 8 == 1 && j == -9)
									continue;
								if (0 <= m_iPlayerTile - 1 + j + 8 * i &&
									32 > m_iPlayerTile - 1 + j + 8 * i)
								{
									dynamic_cast<CTile*>(
										TILE->Get_Tile(m_iPlayerTile - 1 + j + 8 * i))->Set_TileState(TILE_IDLE);
									m_AttackTileList.push_back(
										dynamic_cast<CTile*>(
											TILE->Get_Tile(m_iPlayerTile - 1 + j + 8 * i)));
								}
							}
						}
					}
				}
			}
		
		}
	}
}

void CReaper::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iImageEnd = 1;
			m_tFrame.iMotion = 0;
			m_tFrame.iMotionEnd = 1;

			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;

			m_tFrame.dwSpeed = 400;
			m_tFrame.dwTime = GetTickCount();
			break;
		case ATTACK:
			m_pFrameKey = L"Reaper_Attack";
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iImageEnd = 2;
			m_tFrame.iMotion = 0;
			m_tFrame.iMotionEnd = 2;

			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;

			m_tFrame.dwSpeed = 170;
			m_tFrame.dwTime = GetTickCount();
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CReaper::Collilsion_Event(CObj * _pObj)
{
	switch (_pObj->Get_OBJID())
	{
	case SPELL:
		m_tCreature.iHp -= static_cast<CSpell*>(_pObj)->Get_SpellInfo().iAtk;
		m_DamageList.push_back(CAbstractFactory<CDamageUI>::Create_DamageUI(m_tInfo.fX - 50, m_tInfo.fY - 100 - m_iDamageY, static_cast<CSpell*>(_pObj)->Get_SpellInfo().iAtk));
		Set_DamageY(20);
		SOUND->PlaySound(L"enemy_hit_light.wav", SOUND_EFFECT12, 0.3f);
		break;
	default:
		break;
	}
}

void CReaper::Count_Trigger(int _iTriggerCnt)
{
}
