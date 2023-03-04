#include "stdafx.h"
#include "RewardUI.h"
#include "BitmapMgr.h"
#include "KeyMgr.h"

CRewardUI::CRewardUI()
	:m_pReward(nullptr), m_bSelected(false)
{
}


CRewardUI::~CRewardUI()
{
	Release();
}

void CRewardUI::Initialize()
{
	BITMAP->Insert_Bmp(L"../Image/UI/Reward.bmp", L"Reward");
	if (!m_pReward)
	{
		m_pReward = new CReward;
		m_pReward->Initialize();
	}
}

void CRewardUI::Update()
{
	if (m_pReward->Update())
		m_bSelected = true;
}

void CRewardUI::Late_Update()
{
}

void CRewardUI::Render(HDC hDC)
{
	HDC	hMemDC = BITMAP->Find_Img(L"Reward");
	StretchBlt(hDC,
		0,
		0,
		WINCX, // 키울 사이즈
		WINCY,
		hMemDC,
		0,
		0,
		WINCX,		// 텍스처 원본 사이즈
		WINCY,
		SRCCOPY);

	m_pReward->Render(hDC);
}

void CRewardUI::Release()
{
	KEY->Destroy_Instance();
	Safe_Delete<CReward*>(m_pReward);
}
