#include "stdafx.h"
#include "GateMissile.h"
#include "TileMgr.h"
#include "BitmapMgr.h"
#include "SoundMgr.h"

CGateMissile::CGateMissile()
{
}


CGateMissile::~CGateMissile()
{
	Release();
}

void CGateMissile::Initialize()
{
	m_tInfo.fCX = 72.f;
	m_tInfo.fCY = 72.f;
	m_fSpeed = 25.f;
	
	m_eObjID = ENEMIE_SPELL;
	m_iAttack = 100;
	m_pFrameKey = L"Missile";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 2;
	m_tFrame.iImageEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 2;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();
}

int CGateMissile::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Check_Tile();
	Count_Trigger(55);
	Target_Angle();
	Rotate_Missile();

	Attack();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CGateMissile::Late_Update()
{
	__super::Move_Frame();
}

void CGateMissile::Render(HDC hDC)
{
	//77 32
	HDC		hPlgDC = BITMAP->Find_Img(L"GatePlg");
	HDC		hResetDC = BITMAP->Find_Img(L"GateReset");
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);

	// ��Ʈ���� ȸ����Ű�� �Լ�
	PlgBlt(hPlgDC, // ȸ�� ��ų �̹����� ��� �׸� ���ΰ�
		m_tPoint,// �׸��� �׸� �簢���� ��ǥ POINT ����, ������ �»��, ����, ���ϴ�
		hMemDC, // ��� �ִ� �̹����� �׸� ���ΰ�
		m_tFrame.iFrameCnt * 72,
		m_tFrame.iMotionCnt * 72, // �׸��� �׸� �̹����� X,Y ���� ��ǥ
		72,
		72, // �׸��� �׸� �̹����� ���ο� ������ ���̸� ����
		NULL,
		NULL,
		NULL);


	GdiTransparentBlt(hDC,
		int(m_tRect.left),
		int(m_tRect.top),
		72, // Ű�� ������
		72,
		hPlgDC,
		0,
		0,
		72,	// �ؽ�ó ���� ������
		72,
		RGB(1, 1, 1));

	BitBlt(hPlgDC,
		0,
		0,
		72,
		72,
		hResetDC, 0, 0, SRCCOPY);
}

void CGateMissile::Release()
{
	SOUND->PlaySound(L"plastic_blast.wav", SOUND_EFFECT30, 0.2f);
}

void CGateMissile::Attack()
{
	m_tInfo.fX -= abs(m_fWidth) / m_fSpeed;
	m_tInfo.fY -= m_fHeight / m_fSpeed;

	if (m_iTileNum == m_iTargetTile)
		m_bDead = true;
}

void CGateMissile::Motion_Change()
{
}

void CGateMissile::Collilsion_Event(CObj * _pObj)
{
}

void CGateMissile::Count_Trigger(int _iTriggerCnt)
{
	if (m_iRenderCnt == 1)
	{
		SOUND->PlaySound(L"stealth.wav", SOUND_EFFECT31, 0.1f);
		static_cast<CTile*>(TILE->Get_Tile(m_iTargetTile))->Set_TileState(TILE_ATTACKED);
	}
	if (m_iRenderCnt == _iTriggerCnt)
	{
		static_cast<CTile*>(TILE->Get_Tile(m_iTargetTile))->Set_TileState(TILE_IDLE);
		m_AttackTileList.push_back(static_cast<CTile*>(TILE->Get_Tile(m_iTargetTile)));
	}
	if (m_iRenderCnt == _iTriggerCnt + 1)
	{
		static_cast<CTile*>(TILE->Get_Tile(m_iTargetTile))->Set_TileState(TILE_IDLE);
		m_AttackTileList.clear();
	}
	if (m_iRenderCnt)
		++m_iRenderCnt;
}

void CGateMissile::Rotate_Missile()
{
	float	fDiagonal = sqrtf(
		((m_tInfo.fCX / 2.f) * (m_tInfo.fCX / 2.f))
		+ ((m_tInfo.fCY / 2.f) * (m_tInfo.fCY / 2.f)));

	// �� ���
	m_tPoint[0].x = long((m_tInfo.fCX / 2.f) + fDiagonal * cosf((m_fAngle + 135.f) * PI / 180.f));
	m_tPoint[0].y = long((m_tInfo.fCY / 2.f) - fDiagonal * sinf((m_fAngle + 135.f) * PI / 180.f));

	// �� ���		
	m_tPoint[1].x = long((m_tInfo.fCX / 2.f) + fDiagonal * cosf((m_fAngle + 45.f) * PI / 180.f));
	m_tPoint[1].y = long((m_tInfo.fCY / 2.f) - fDiagonal * sinf((m_fAngle + 45.f) * PI / 180.f));

	// �� �ϴ�		
	m_tPoint[2].x = long((m_tInfo.fCX / 2.f) + fDiagonal * cosf((m_fAngle + 225.f) * PI / 180.f));
	m_tPoint[2].y = long((m_tInfo.fCY / 2.f) - fDiagonal * sinf((m_fAngle + 225.f) * PI / 180.f));
}

void CGateMissile::Target_Angle()
{
	m_tTargetInfo = TILE->Get_TileInfo(m_iTargetTile);

	float fWidth = m_tInfo.fX - m_tTargetInfo.fX;
	float fHeight = m_tInfo.fY - m_tTargetInfo.fY;

	float	fDiagonal = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight);

	if (0 < m_fHeight)
	{
		//180 - ��Ÿ
		m_fAngle = 180.f - acosf(m_fWidth / fDiagonal) * 180.f / PI;
	}
	else
	{
		m_fAngle = 180.f + acosf(m_fWidth / fDiagonal) * 180.f / PI;
	}
	m_fWidth = fWidth;
	m_fHeight = fHeight;
}
