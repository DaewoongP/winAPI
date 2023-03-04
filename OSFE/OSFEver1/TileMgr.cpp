#include "stdafx.h"
#include "TileMgr.h"
#include "AbstractFactory.h"

CTileMgr*	CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
{
	m_vecTile.reserve(TILEX * TILEY);
}

CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize(void)
{
	// left top -> ������� 0 ~ 15��
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			// Ÿ���� ���� �������� ��ġ�� ��Ƽ� Ÿ�� ��ġ
			float	fX = (float)(TILECX >> 1) + ((TILECX + TILEITV) * j) + TILEINIT_X;
			float	fY = (float)(TILECY >> 1) + ((TILECY + TILEITV) * i) + TILEINIT_Y;

			CObj* pObj = CAbstractFactory<CTile>::Create(fX, fY);
			m_vecTile.push_back(pObj);
		}
	}
}

void CTileMgr::Update(void)
{
	for (auto& iter : m_vecTile)
		iter->Update();
}

void CTileMgr::Late_Update(void)
{
	for (auto& iter : m_vecTile)
		iter->Late_Update();
}

void CTileMgr::Render(HDC hDC)
{
	for (auto& iter : m_vecTile)
		iter->Render(hDC);
}

void CTileMgr::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<CObj*>);
	m_vecTile.clear();
}
